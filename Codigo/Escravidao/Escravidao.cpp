#include "Escravidao/Escravidao.h"

#include <algorithm>

#include <entt/entity/registry.hpp>
#include <nlohmann/json.hpp>

#include "Combate/Combate.h"
#include "Comercio/Comercio.h"
#include "Cultura/Cultura.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/ProblemasDosDados.h"
#include "Economia/ConsumoESalarios.h"
#include "Economia/FelicidadeDaClasse.h"
#include "Economia/MercadoLocal.h"
#include "Reputacao/Reputacao.h"
#include "Vila/Trabalho.h"
#include "Eventos/EventosDaVila.h"
#include "Faccao/Faccao.h"
#include "Npc/ComponentesDoNpc.h"
#include "Producao/ProducaoDaVila.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/Populacao.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDaEscravidao = "escravidao.json";

// O sorteio da escravidão de uma cidade num dia: sai da semente da partida, do mapa e do dia.
// Assim ele não mexe nos sorteios dos outros sistemas e não precisa ir no save.
SorteioComSemente sorteioDoDia(const SimulacaoDaVila& cidade, int64_t dia)
{
    const uint64_t semente = cidade.vila.sementeDaPartida ^ (static_cast<uint64_t>(dia + 1) * 0x9E3779B97F4A7C15ULL);
    return SorteioComSemente(semente, 5000 + static_cast<uint64_t>(cidade.numeroDoMapaNoMundo()));
}

// Os escravos da cidade, na ordem da lista de NPCs (sempre a mesma ordem).
std::vector<entt::entity> escravosDaCidade(const SimulacaoDaVila& cidade)
{
    std::vector<entt::entity> escravos;
    for (const entt::entity npc : cidade.npcsDaVila) {
        if (cidade.registroDasEntidades.all_of<CativeiroDoNpc>(npc)) {
            escravos.push_back(npc);
        }
    }
    return escravos;
}

// A cidade do mundo que fica no mapa pedido, ou nullptr.
SimulacaoDaVila* cidadeDoMapa(SimulacaoDaVila& vila, int numeroDoMapa)
{
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        if (cidade->numeroDoMapaNoMundo() == numeroDoMapa) {
            return cidade;
        }
    }
    return nullptr;
}

// Regra 4.4, passo 3: o fugitivo sai da cidade. Metade vira bandido (soma na Criminalidade da
// cidade de onde fugiu); a outra metade vai para uma Vila Livre sem escravos e vira Civil lá.
// Sem Vila Livre com cama livre, ele some do mundo conhecido.
void fazerOEscravoFugir(SimulacaoDaVila& vila, SimulacaoDaVila& cidade, entt::entity npc, SorteioComSemente& sorteio)
{
    const RegrasDaEscravidao& regras = vila.dados.regrasDaEscravidao;
    const IdentidadeDoNpc identidade = cidade.registroDasEntidades.get<IdentidadeDoNpc>(npc);
    const HabilidadesDoNpc habilidades = cidade.registroDasEntidades.get<HabilidadesDoNpc>(npc);
    const int idade = cidade.registroDasEntidades.get<IdadeDoNpc>(npc).anos;
    ContextoDaProducao contextoDaCidade = cidade.contextoDaProducao();
    tirarNpcDaVila(contextoDaCidade, npc);
    ++cidade.escravidao.fugasNoTotal;

    if (sorteio.sortearNumeroInteiroEntre(1, 100) <= regras.fugitivoViraBandidoEmPorcento) {
        cidade.felicidade.criminalidadeEmDecimos = std::min(1000, cidade.felicidade.criminalidadeEmDecimos + regras.criminalidadePorBandido * 10);
        return;
    }
    for (const CidadeDoMundo& candidata : vila.dados.mundo.cidades) {
        if (!candidata.faccao.empty() || candidata.ehAVilaDoJogador) {
            continue;
        }
        SimulacaoDaVila* vilaLivre = cidadeDoMapa(vila, candidata.numeroDoMapa);
        if (vilaLivre == nullptr || contarOsEscravos(*vilaLivre) > 0) {
            continue;
        }
        ContextoDaProducao contextoDaVilaLivre = vilaLivre->contextoDaProducao();
        const entt::entity novo = criarNpcNaVila(contextoDaVilaLivre, idade, habilidades.bracal, habilidades.manufaturado, true);
        if (novo != entt::null) {
            vilaLivre->registroDasEntidades.get<IdentidadeDoNpc>(novo).cultura = identidade.cultura;
            return;
        }
    }
}

// Regra 4.5: os escravos contra os guardas e os Civis adultos, pelo cálculo rápido do Combate.
void resolverARebeliao(SimulacaoDaVila& vila, SimulacaoDaVila& cidade, int64_t hoje, SorteioComSemente& sorteio, std::vector<std::string>& linhas)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaEscravidao& regras = dados.regrasDaEscravidao;
    const std::vector<entt::entity> escravos = escravosDaCidade(cidade);
    int civisAdultos = 0;
    for (const entt::entity npc : cidade.npcsDaVila) {
        const bool ehEscravo = cidade.registroDasEntidades.all_of<CativeiroDoNpc>(npc);
        const IdadeDoNpc* idade = cidade.registroDasEntidades.try_get<IdadeDoNpc>(npc);
        civisAdultos += !ehEscravo && idade != nullptr && idade->anos >= dados.regras.populacao.idadeAdulto ? 1 : 0;
    }
    const std::vector<Combatente> rebeldes = ladoDoKit(dados.regrasDoCombate, "civil", static_cast<int>(escravos.size()));
    std::vector<Combatente> defensores = ladoDoKit(dados.regrasDoCombate, "guarda", regras.guardasPorNivelDaCidade * cidade.vila.nivelDaCidade);
    const std::vector<Combatente> civis = ladoDoKit(dados.regrasDoCombate, "civil", civisAdultos);
    defensores.insert(defensores.end(), civis.begin(), civis.end());
    const uint32_t sementeDaLuta = static_cast<uint32_t>(hoje * 131 + cidade.numeroDoMapaNoMundo());
    const bool osEscravosVenceram = resolverPorCalculoRapido(dados.regrasDoCombate, rebeldes, defensores, sementeDaLuta).venceuOLadoA;

    ++cidade.escravidao.rebelioesNoTotal;
    cidade.escravidao.diasSeguidosParaRebeliao = 0;
    const int64_t fimDoEfeito = hoje + vila.relogio.diasPorEstacao();
    const std::string nome = dados.mundo.cidadeNoMapa(cidade.numeroDoMapaNoMundo()) != nullptr ? dados.mundo.cidadeNoMapa(cidade.numeroDoMapaNoMundo())->nome : "a vila";
    if (osEscravosVenceram) {
        for (const entt::entity npc : escravos) {
            fazerOEscravoFugir(vila, cidade, npc, sorteio);
        }
        cidade.escravidao.civisAbaladosAte = fimDoEfeito;
        linhas.push_back("Escravidão: os " + std::to_string(escravos.size()) + " escravos de " + nome + " se rebelaram, venceram e fugiram.");
        return;
    }
    // Perderam: 20% são executados ou vendidos para fora (os últimos da lista); o resto volta castigado.
    const int perdidos = static_cast<int>(escravos.size()) * regras.perdedoresExecutadosOuVendidosEmPorcento / 100;
    ContextoDaProducao contexto = cidade.contextoDaProducao();
    for (int posicao = 0; posicao < perdidos; ++posicao) {
        tirarNpcDaVila(contexto, escravos[escravos.size() - 1 - static_cast<std::size_t>(posicao)]);
    }
    cidade.escravidao.escravosCastigadosAte = fimDoEfeito;
    linhas.push_back("Escravidão: a rebelião dos escravos de " + nome + " foi esmagada (" + std::to_string(perdidos) + " executados ou vendidos).");
}

void rodarOsMercadosNaEstacao(SimulacaoDaVila& vila);
void rodarOCativeiroDoJogador(SimulacaoDaVila& vila, std::vector<std::string>& linhas);

// O dia de uma cidade.
void rodarODiaDaCidade(SimulacaoDaVila& vila, SimulacaoDaVila& cidade, std::vector<std::string>& linhas)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaEscravidao& regras = dados.regrasDaEscravidao;
    EstadoDaEscravidaoDaCidade& estado = cidade.escravidao;
    const int64_t hoje = vila.relogio.diaDaPartida();
    const int mapa = cidade.numeroDoMapaNoMundo();
    estado.pontosNaFelicidadeDosCivisHoje = estado.civisAbaladosAte >= hoje ? regras.felicidadeDosCivisSeOsEscravosVencem : 0;
    estado.escravosSemComidaHoje = 0;

    std::vector<entt::entity> escravos = escravosDaCidade(cidade);
    if (escravos.empty()) {
        estado.diasSeguidosParaRebeliao = 0;
        return;
    }
    ContextoDaProducao contexto = cidade.contextoDaProducao();
    const bool primeiroDiaDaEstacao = vila.relogio.diaDaEstacaoAtual() == 1;

    // Regra 4.6, a abolição: a lei virou "proibida"; no dia 1 da estação, todos viram Civis.
    if (primeiroDiaDaEstacao && !escravidaoPermitidaNaCidade(vila, mapa)) {
        for (const entt::entity npc : escravos) {
            libertarEscravo(contexto, npc);
        }
        estado.diasSeguidosParaRebeliao = 0;
        const CidadeDoMundo* definicao = dados.mundo.cidadeNoMapa(mapa);
        linhas.push_back("Escravidão: a abolição libertou " + std::to_string(escravos.size()) + " escravos em " + (definicao != nullptr ? definicao->nome : "a vila") + ".");
        return;
    }

    // Passo 1: a comida. 1 por escravo, do estoque da Cidade (o dono), sem dinheiro.
    int escravosDoJogador = 0;
    for (const entt::entity npc : escravos) {
        escravosDoJogador += cidade.registroDasEntidades.get<CativeiroDoNpc>(npc).doJogador ? 1 : 0;
    }
    estado.escravosSemComidaHoje = alimentarOsEscravosDaCidade(contexto, static_cast<int>(escravos.size()) - escravosDoJogador)
        + alimentarOsEscravosDaCidade(contexto, escravosDoJogador, true);

    // Passo 5: a Felicidade dos Escravos.
    int semCasa = 0;
    for (const entt::entity npc : escravos) {
        semCasa += cidade.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDaCasa < 0 ? 1 : 0;
    }
    const RegrasDaFelicidade& regrasDaFelicidade = dados.regras.felicidade;
    const int eventos = std::clamp(felicidadeDosEventosAtivos(contexto), regrasDaFelicidade.eventosMinimo, regrasDaFelicidade.eventosMaximo);
    const int castigoGeral = estado.escravosCastigadosAte >= hoje ? regras.felicidadeDosEscravosSePerdem : 0;
    const int alvo = calcularAlvoDaFelicidadeDosEscravos(regras, static_cast<int>(escravos.size()), estado.escravosSemComidaHoje, semCasa, eventos, castigoGeral,
                                                         regrasDaFelicidade.consumoSemComida, regrasDaFelicidade.moradiaSemCasa);
    estado.felicidadeDosEscravosEmDecimos = darOPassoDaFelicidade(estado.felicidadeDosEscravosEmDecimos, alvo, regrasDaFelicidade);

    // A Obediência de cada um (regra 4.3).
    const int controle = 1000 - cidade.felicidade.criminalidadeEmDecimos;
    for (const entt::entity npc : escravos) {
        CativeiroDoNpc& cativeiro = cidade.registroDasEntidades.get<CativeiroDoNpc>(npc);
        const bool castigoPublico = estado.castigoPublicoAte >= hoje;
        const int felicidadeDele = estado.felicidadeDosEscravosEmDecimos + (cativeiro.castigadoAte >= hoje ? regras.castigoDaFugaNaFelicidade * 10 : 0)
            + (cativeiro.grilhoes ? regras.grilhoesNaFelicidade * 10 : 0) + (castigoPublico ? regras.castigoPublicoNaFelicidade * 10 : 0);
        const int estacoes = static_cast<int>((hoje - cativeiro.diaDoInicio) / std::max(1, vila.relogio.diasPorEstacao()));
        cativeiro.obedienciaEmDecimos = calcularObedienciaEmDecimos(regras, controle, std::max(0, felicidadeDele), cativeiro.grilhoes, estacoes,
                                                                    cidade.registroDasEntidades.get<IdentidadeDoNpc>(npc).cultura);
        if (castigoPublico) {  // o castigo público soma 5 ao Medo
            cativeiro.obedienciaEmDecimos = std::min(1000, cativeiro.obedienciaEmDecimos + regras.castigoPublicoNoMedo * 10);
        }
    }

    SorteioComSemente sorteio = sorteioDoDia(cidade, hoje);

    // Regra 4.5: a rebelião, depois de 4 dias seguidos de Felicidade e Controle abaixo de 10.
    const bool emColapso = estado.felicidadeDosEscravosEmDecimos < regras.rebeliaoComFelicidadeAbaixoDe * 10 && controle < regras.rebeliaoComControleAbaixoDe * 10;
    estado.diasSeguidosParaRebeliao = emColapso ? estado.diasSeguidosParaRebeliao + 1 : 0;
    if (estado.diasSeguidosParaRebeliao >= regras.rebeliaoDiasSeguidos) {
        resolverARebeliao(vila, cidade, hoje, sorteio, linhas);
        return;
    }

    // Regra 4.4: a fuga, toda noite.
    for (const entt::entity npc : escravos) {
        CativeiroDoNpc& cativeiro = cidade.registroDasEntidades.get<CativeiroDoNpc>(npc);
        const int chance = chanceDeTentarFugaEmPorMil(regras, cativeiro.obedienciaEmDecimos, cativeiro.grilhoes);
        if (chance <= 0 || sorteio.sortearNumeroInteiroEntre(1, 1000) > chance) {
            continue;
        }
        if (sorteio.sortearNumeroInteiroEntre(1, 100) <= regras.fugaDetectadaEmPorcento) {
            cativeiro.castigadoAte = hoje + regras.castigoDaFugaEmDias;  // visto: volta e é castigado
        } else {
            fazerOEscravoFugir(vila, cidade, npc, sorteio);
        }
    }

    // Regra 4.6, a alforria: no dia 1 da estação.
    if (primeiroDiaDaEstacao) {
        const std::string cultura = culturaBaseDaCidade(dados, mapa);
        int porcento = 0;
        if (const auto daCultura = regras.alforriaPorEstacaoEmPorcento.find(cultura); daCultura != regras.alforriaPorEstacaoEmPorcento.end()) {
            porcento = daCultura->second;
        } else if (const auto daLei = regras.alforriaPorEstacaoEmPorcento.find(opcaoDaLeiNaCidade(vila, mapa, "tolerancia_religiosa"));
                   daLei != regras.alforriaPorEstacaoEmPorcento.end()) {
            porcento = daLei->second;
        }
        escravos = escravosDaCidade(cidade);
        const int alforriados = std::min(static_cast<int>(escravos.size()), alforriadosNaEstacao(static_cast<int>(escravos.size()), porcento, estado.alforriaAcumuladaEmCentesimos));
        for (int posicao = 0; posicao < alforriados; ++posicao) {
            // Os mais antigos no cativeiro primeiro (a lista já está na ordem dos NPCs).
            libertarEscravo(contexto, escravos[static_cast<std::size_t>(posicao)]);
            ++estado.alforriasNoTotal;
        }
    }
}

}  // namespace

RegrasDaEscravidao lerArquivoDaEscravidao(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDaEscravidao regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaEscravidao, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    regras.proporcaoInicialEmPorcento = documento.value("proporcao_inicial_em_porcento", std::map<std::string, int>{});
    for (const auto& [cultura, porcento] : regras.proporcaoInicialEmPorcento) {
        if (porcento < 0 || porcento > 90) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaEscravidao, 0, "\"proporcao_inicial_em_porcento\" de \"" + cultura + "\" precisa ficar entre 0 e 90");
        }
    }
    const Json preco = documento.value("preco", Json::object());
    regras.precoBase = preco.value("base", regras.precoBase);
    regras.precoPorPontoDaMelhorHabilidade = preco.value("por_ponto_da_melhor_habilidade", regras.precoPorPontoDaMelhorHabilidade);
    regras.precoDoMagoEmPorcento = preco.value("mago_em_porcento", regras.precoDoMagoEmPorcento);
    regras.precoDoVelhoEmPorcento = preco.value("velho_em_porcento", regras.precoDoVelhoEmPorcento);
    regras.precoDaCulturaHostilEmPorcento = preco.value("cultura_hostil_em_porcento", regras.precoDaCulturaHostilEmPorcento);
    regras.felicidadeBaseDoEscravo = documento.value("felicidade_base_do_escravo", regras.felicidadeBaseDoEscravo);

    const Json obediencia = documento.value("obediencia", Json::object());
    regras.obedienciaBase = obediencia.value("base", regras.obedienciaBase);
    regras.medoMaximo = obediencia.value("medo_maximo", regras.medoMaximo);
    regras.obedienciaDosGrilhoes = obediencia.value("grilhoes", regras.obedienciaDosGrilhoes);
    regras.obedienciaPorEstacaoDeCativeiro = obediencia.value("por_estacao_de_cativeiro", regras.obedienciaPorEstacaoDeCativeiro);
    regras.cativeiroMaximo = obediencia.value("cativeiro_maximo", regras.cativeiroMaximo);
    regras.obedienciaCaiComFelicidadeAbaixoDe = obediencia.value("felicidade_abaixo_de", regras.obedienciaCaiComFelicidadeAbaixoDe);
    regras.obedienciaPorCultura = obediencia.value("por_cultura", std::map<std::string, int>{});

    const Json fuga = documento.value("fuga", Json::object());
    regras.fugaComObedienciaAbaixoDe = fuga.value("obediencia_abaixo_de", regras.fugaComObedienciaAbaixoDe);
    regras.fugaDetectadaEmPorcento = fuga.value("detectado_em_porcento", regras.fugaDetectadaEmPorcento);
    regras.castigoDaFugaNaFelicidade = fuga.value("castigo_felicidade", regras.castigoDaFugaNaFelicidade);
    regras.castigoDaFugaEmDias = fuga.value("castigo_dias", regras.castigoDaFugaEmDias);
    regras.fugitivoViraBandidoEmPorcento = fuga.value("vira_bandido_em_porcento", regras.fugitivoViraBandidoEmPorcento);
    regras.criminalidadePorBandido = fuga.value("criminalidade_por_bandido", regras.criminalidadePorBandido);

    const Json rebeliao = documento.value("rebeliao", Json::object());
    regras.rebeliaoComFelicidadeAbaixoDe = rebeliao.value("felicidade_abaixo_de", regras.rebeliaoComFelicidadeAbaixoDe);
    regras.rebeliaoComControleAbaixoDe = rebeliao.value("controle_abaixo_de", regras.rebeliaoComControleAbaixoDe);
    regras.rebeliaoDiasSeguidos = std::max(1, rebeliao.value("dias_seguidos", regras.rebeliaoDiasSeguidos));
    regras.perdedoresExecutadosOuVendidosEmPorcento = rebeliao.value("perdedores_executados_ou_vendidos_em_porcento", regras.perdedoresExecutadosOuVendidosEmPorcento);
    regras.guardasPorNivelDaCidade = rebeliao.value("guardas_por_nivel_da_cidade", regras.guardasPorNivelDaCidade);
    regras.felicidadeDosCivisSeOsEscravosVencem = rebeliao.value("felicidade_dos_civis_se_vencem", regras.felicidadeDosCivisSeOsEscravosVencem);
    regras.felicidadeDosEscravosSePerdem = rebeliao.value("felicidade_dos_escravos_se_perdem", regras.felicidadeDosEscravosSePerdem);

    regras.alforriaPorEstacaoEmPorcento = documento.value("alforria_por_estacao_em_porcento", std::map<std::string, int>{});

    // Parte 2.
    regras.outrasCidadesComMercado = documento.value("outras_cidades_com_mercado", std::vector<std::vector<int>>{});
    for (const std::vector<int>& par : regras.outrasCidadesComMercado) {
        if (par.size() != 2) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaEscravidao, 0, "\"outras_cidades_com_mercado\" precisa de pares [coluna, linha]");
        }
    }
    const Json alvo = documento.value("mercado_alvo", Json::object());
    regras.mercadoAlvoPorVagaBracalVazia = alvo.value("por_vaga_bracal_vazia", regras.mercadoAlvoPorVagaBracalVazia);
    regras.mercadoAlvoMinimo = alvo.value("minimo", regras.mercadoAlvoMinimo);
    regras.vendaPeloDonoEmPorcento = documento.value("venda_pelo_dono_em_porcento", regras.vendaPeloDonoEmPorcento);
    regras.reservaDoBancoParaComprarEmMoedas = documento.value("reserva_do_banco_para_comprar_em_moedas", regras.reservaDoBancoParaComprarEmMoedas);
    regras.cativosDosBandidosPorEstacao = documento.value("cativos_dos_bandidos_por_estacao", regras.cativosDosBandidosPorEstacao);
    const Json tratamentos = documento.value("tratamentos", Json::object());
    const Json grilhoes = tratamentos.value("grilhoes", Json::object());
    regras.grilhoesNaFelicidade = grilhoes.value("felicidade", regras.grilhoesNaFelicidade);
    regras.grilhoesNoTrabalhoEmPorcento = grilhoes.value("velocidade_em_porcento", regras.grilhoesNoTrabalhoEmPorcento);
    const Json castigo = tratamentos.value("castigo_publico", Json::object());
    regras.castigoPublicoNoMedo = castigo.value("medo", regras.castigoPublicoNoMedo);
    regras.castigoPublicoEmDias = castigo.value("dias", regras.castigoPublicoEmDias);
    regras.castigoPublicoNaFelicidade = castigo.value("felicidade", regras.castigoPublicoNaFelicidade);
    regras.reputacaoReligiosaNoCastigoPublico = castigo.value("reputacao_religiosa", regras.reputacaoReligiosaNoCastigoPublico);
    const Json reputacao = documento.value("reputacao", Json::object());
    regras.reputacaoReligiosaAoComprar = reputacao.value("religiosa_ao_comprar", regras.reputacaoReligiosaAoComprar);
    regras.reputacaoReligiosaAoVender = reputacao.value("religiosa_ao_vender", regras.reputacaoReligiosaAoVender);
    regras.reputacaoReligiosaAoLibertar = reputacao.value("religiosa_ao_libertar", regras.reputacaoReligiosaAoLibertar);
    regras.reputacaoDeQuemPermiteAoLibertar = reputacao.value("de_quem_permite_ao_libertar", regras.reputacaoDeQuemPermiteAoLibertar);
    regras.capturaDoJogadorPelosBandidosEmPorcento = documento.value("captura_do_jogador_pelos_bandidos_em_porcento", regras.capturaDoJogadorPelosBandidosEmPorcento);
    regras.habilidadeDoJogadorNoPreco = documento.value("habilidade_do_jogador_no_preco", regras.habilidadeDoJogadorNoPreco);
    regras.comprarALiberdadeEmPorcentoDoPreco = documento.value("comprar_a_liberdade_em_porcento_do_preco", regras.comprarALiberdadeEmPorcentoDoPreco);
    regras.peculioMaximoPorDiaEmDecimos = documento.value("peculio_maximo_por_dia_em_decimos", regras.peculioMaximoPorDiaEmDecimos);
    regras.recompensaPelaFugaEmMoedas = documento.value("recompensa_pela_fuga_em_moedas", regras.recompensaPelaFugaEmMoedas);
    return regras;
}

int precoBaseDoEscravo(const RegrasDaEscravidao& regras, int melhorHabilidade, bool ehMago, bool ehVelho, bool culturaHostilADoComprador)
{
    int64_t preco = regras.precoBase + static_cast<int64_t>(regras.precoPorPontoDaMelhorHabilidade) * melhorHabilidade;
    preco = ehMago ? preco * regras.precoDoMagoEmPorcento / 100 : preco;
    preco = ehVelho ? preco * regras.precoDoVelhoEmPorcento / 100 : preco;
    preco = culturaHostilADoComprador ? preco * regras.precoDaCulturaHostilEmPorcento / 100 : preco;
    return static_cast<int>(preco);
}

int calcularObedienciaEmDecimos(const RegrasDaEscravidao& regras, int controleEmDecimos, int felicidadeEmDecimos, bool grilhoes, int estacoesDeCativeiro,
                                const std::string& cultura)
{
    const int medo = std::clamp(controleEmDecimos, 0, 1000) * regras.medoMaximo / 100;
    const int cativeiro = std::clamp(estacoesDeCativeiro * regras.obedienciaPorEstacaoDeCativeiro, 0, regras.cativeiroMaximo) * 10;
    const int faltaDeFelicidade = std::max(0, regras.obedienciaCaiComFelicidadeAbaixoDe * 10 - felicidadeEmDecimos);
    const auto daCultura = regras.obedienciaPorCultura.find(cultura);
    const int obediencia = regras.obedienciaBase * 10 + medo + (grilhoes ? regras.obedienciaDosGrilhoes * 10 : 0) + cativeiro - faltaDeFelicidade
        + (daCultura != regras.obedienciaPorCultura.end() ? daCultura->second * 10 : 0);
    return std::clamp(obediencia, 0, 1000);
}

int chanceDeTentarFugaEmPorMil(const RegrasDaEscravidao& regras, int obedienciaEmDecimos, bool grilhoes)
{
    return grilhoes ? 0 : std::max(0, regras.fugaComObedienciaAbaixoDe * 10 - obedienciaEmDecimos);
}

int calcularAlvoDaFelicidadeDosEscravos(const RegrasDaEscravidao& regras, int escravos, int semComida, int semCasa, int pontosDosEventos, int pontosDoCastigo,
                                        int consumoSemComida, int moradiaSemCasa)
{
    if (escravos <= 0) {
        return regras.felicidadeBaseDoEscravo * 10;
    }
    const int consumo = consumoSemComida * 10 * std::min(semComida, escravos) / escravos;
    const int moradia = moradiaSemCasa * 10 * std::min(semCasa, escravos) / escravos;
    return std::clamp(regras.felicidadeBaseDoEscravo * 10 + consumo + moradia + pontosDosEventos * 10 + pontosDoCastigo * 10, 0, 1000);
}

int alforriadosNaEstacao(int escravos, int porcento, int& acumuladoEmCentesimos)
{
    if (escravos <= 0 || porcento <= 0) {
        return 0;
    }
    acumuladoEmCentesimos += escravos * porcento;
    const int alforriados = acumuladoEmCentesimos / 100;
    acumuladoEmCentesimos -= alforriados * 100;
    return alforriados;
}

std::string opcaoDaLeiNaCidade(const SimulacaoDaVila& vila, int numeroDoMapa, const std::string& lei)
{
    const DadosDoJogo& dados = vila.dados;
    const int posicaoDaLei = dados.faccoes.posicaoDaLei(lei);
    if (posicaoDaLei < 0 || posicaoDaLei >= static_cast<int>(dados.faccoes.leis.size())) {
        return "";
    }
    int valor = -1;
    const int faccao = faccaoDaCidade(dados, numeroDoMapa);
    if (faccao >= 0 && faccao < static_cast<int>(vila.faccoes.leisAtuais.size())
        && posicaoDaLei < static_cast<int>(vila.faccoes.leisAtuais[static_cast<std::size_t>(faccao)].size())) {
        valor = vila.faccoes.leisAtuais[static_cast<std::size_t>(faccao)][static_cast<std::size_t>(posicaoDaLei)];
    } else if (faccao < 0) {
        // Rodada LE1-4: a cidade sem facção segue o que a cultura dela deseja.
        const auto desejos = dados.faccoes.leisDesejadasPorCultura.find(culturaBaseDaCidade(dados, numeroDoMapa));
        if (desejos != dados.faccoes.leisDesejadasPorCultura.end() && posicaoDaLei < static_cast<int>(desejos->second.size())) {
            valor = desejos->second[static_cast<std::size_t>(posicaoDaLei)].valor;
        }
    }
    const std::vector<std::string>& opcoes = dados.faccoes.leis[static_cast<std::size_t>(posicaoDaLei)].opcoes;
    return valor >= 0 && valor < static_cast<int>(opcoes.size()) ? opcoes[static_cast<std::size_t>(valor)] : "";
}

bool escravidaoPermitidaNaCidade(const SimulacaoDaVila& vila, int numeroDoMapa)
{
    const std::string opcao = opcaoDaLeiNaCidade(vila, numeroDoMapa, "escravidao");
    return !opcao.empty() && opcao != "proibida";
}

int contarOsEscravos(const SimulacaoDaVila& cidade)
{
    return static_cast<int>(escravosDaCidade(cidade).size());
}

void tornarEscravo(ContextoDaProducao& contexto, entt::entity npc, int64_t dia)
{
    entt::registry& registro = contexto.registroDasEntidades;
    if (registro.all_of<CativeiroDoNpc>(npc)) {
        return;
    }
    registro.get<IdentidadeDoNpc>(npc).classe = "escravo";
    CativeiroDoNpc cativeiro;
    cativeiro.diaDoInicio = dia;
    registro.emplace<CativeiroDoNpc>(npc, cativeiro);
    contexto.dinheiro.quantidadeDeCivis = std::max(0, contexto.dinheiro.quantidadeDeCivis - 1);
}

void libertarEscravo(ContextoDaProducao& contexto, entt::entity npc)
{
    entt::registry& registro = contexto.registroDasEntidades;
    if (!registro.all_of<CativeiroDoNpc>(npc)) {
        return;
    }
    registro.remove<CativeiroDoNpc>(npc);
    registro.get<IdentidadeDoNpc>(npc).classe = "civil";
    ++contexto.dinheiro.quantidadeDeCivis;
}

void montarAEscravidaoDaPartidaNova(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaEscravidao& regras = dados.regrasDaEscravidao;
    for (const std::unique_ptr<SimulacaoDaVila>& cidade : vila.outrasCidades) {
        const int mapa = cidade->numeroDoMapaNoMundo();
        const auto proporcao = regras.proporcaoInicialEmPorcento.find(culturaBaseDaCidade(dados, mapa));
        if (proporcao == regras.proporcaoInicialEmPorcento.end() || proporcao->second <= 0 || !escravidaoPermitidaNaCidade(vila, mapa)) {
            continue;
        }
        // Rodada LE1-4: das cidades sem facção, só as de uma cultura que não é a de nenhum reino (as 4 da
        // antiga Escravista) começam com escravos; as Vilas Livres começam sem nenhum.
        bool aCulturaTemReino = false;
        for (const DefinicaoDaFaccao& faccao : dados.faccoes.faccoes) {
            aCulturaTemReino = aCulturaTemReino || faccao.cultura == culturaBaseDaCidade(dados, mapa);
        }
        if (faccaoDaCidade(dados, mapa) < 0 && aCulturaTemReino) {
            continue;
        }
        ContextoDaProducao contexto = cidade->contextoDaProducao();
        // Primeiro os dos empregos Braçais; depois, o Braçal mais alto; no empate, o de menor número.
        struct Candidato {
            entt::entity npc;
            bool temEmpregoBracal;
            int bracal;
            int numero;
        };
        std::vector<Candidato> candidatos;
        for (const entt::entity npc : cidade->npcsDaVila) {
            const int trabalho = cidade->registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
            const HabilidadesDoNpc* habilidades = cidade->registroDasEntidades.try_get<HabilidadesDoNpc>(npc);
            candidatos.push_back({npc, trabalho >= 0 && trabalhoDaConstrucao(contexto, trabalho) == TrabalhoDaReceita::Bracal,
                                  habilidades != nullptr ? habilidades->bracal : 0, cidade->registroDasEntidades.get<IdentidadeDoNpc>(npc).numeroDoNpc});
        }
        std::sort(candidatos.begin(), candidatos.end(), [](const Candidato& um, const Candidato& outro) {
            if (um.temEmpregoBracal != outro.temEmpregoBracal) {
                return um.temEmpregoBracal;
            }
            return um.bracal != outro.bracal ? um.bracal > outro.bracal : um.numero < outro.numero;
        });
        const std::size_t quantos = candidatos.size() * static_cast<std::size_t>(proporcao->second) / 100;
        for (std::size_t posicao = 0; posicao < quantos; ++posicao) {
            tornarEscravo(contexto, candidatos[posicao].npc, 0);
        }
        // Parte 2 (4.2): a cidade com Mercado começa com o estoque-alvo à venda (os últimos da lista).
        if (cidadeTemMercadoDeEscravos(vila, mapa)) {
            const std::size_t aVenda = std::min(quantos, static_cast<std::size_t>(std::max(0, estoqueAlvoDoMercado(*cidade))));
            for (std::size_t posicao = quantos - aVenda; posicao < quantos; ++posicao) {
                cidade->registroDasEntidades.get<CativeiroDoNpc>(candidatos[posicao].npc).aVenda = true;
            }
        }
    }
}

std::vector<std::string> rodarAEscravidaoNoDiaNovo(SimulacaoDaVila& vila)
{
    std::vector<std::string> linhas;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        rodarODiaDaCidade(vila, *cidade, linhas);
    }
    if (vila.relogio.diaDaEstacaoAtual() == 1) {
        rodarOsMercadosNaEstacao(vila);
    }
    rodarOCativeiroDoJogador(vila, linhas);
    return linhas;
}

// ---------------------------------------------------------------------------
// Parte 2: o Mercado de Escravos, os escravos do jogador e o jogador capturado.
// ---------------------------------------------------------------------------

namespace {

// A posição do dono da reputação ("religiosa", "Vila Livre do Norte"), ou −1.
int posicaoDoDonoDaReputacao(const DadosDoJogo& dados, const std::string& dono)
{
    const std::vector<std::string> donos = donosDaReputacao(dados.mundo);
    const auto achado = std::find(donos.begin(), donos.end(), dono);
    return achado == donos.end() ? -1 : static_cast<int>(achado - donos.begin());
}

void mudarAReputacaoCom(SimulacaoDaVila& vila, const std::string& dono, int valor, const std::string& motivo, std::vector<std::string>& linhas)
{
    const int posicao = posicaoDoDonoDaReputacao(vila.dados, dono);
    if (posicao < 0 || valor == 0) {
        return;
    }
    const std::string linha = mudarReputacao(vila, posicao, valor, motivo);
    if (!linha.empty()) {
        linhas.push_back(linha);
    }
}

// O NPC da cidade com esse número, ou entt::null.
entt::entity npcDeNumero(const SimulacaoDaVila& cidade, int numero)
{
    for (const entt::entity npc : cidade.npcsDaVila) {
        if (cidade.registroDasEntidades.get<IdentidadeDoNpc>(npc).numeroDoNpc == numero) {
            return npc;
        }
    }
    return entt::null;
}

std::string nomeDaCidadeNoMapa(const DadosDoJogo& dados, int numeroDoMapa)
{
    const CidadeDoMundo* cidade = dados.mundo.cidadeNoMapa(numeroDoMapa);
    return cidade != nullptr ? cidade->nome : "a vila";
}

// A distância entre dois mapas da malha, em passos (sem diagonal).
int distanciaEntreMapas(const MundoDoJogo& mundo, int umMapa, int outroMapa)
{
    if (!mundo.mapaExiste(umMapa) || !mundo.mapaExiste(outroMapa)) {
        return 9999;
    }
    const MapaDoMundo& um = mundo.mapa(umMapa);
    const MapaDoMundo& outro = mundo.mapa(outroMapa);
    return std::abs(um.coluna - outro.coluna) + std::abs(um.linha - outro.linha);
}

// A cidade onde o grupo do jogador está parado (não a vila dele), ou nullptr.
SimulacaoDaVila* cidadeOndeOGrupoEsta(SimulacaoDaVila& vila)
{
    const EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    if (grupo.emTransito) {
        return nullptr;
    }
    SimulacaoDaVila* cidade = cidadeDoMundoNoMapa(vila, grupo.mapaAtual);
    return cidade == &vila ? nullptr : cidade;
}

// Regra 4.2: no dia 1 de cada estação, os cativos dos bandidos entram de graça nos Mercados abaixo
// do alvo (ES1-3), e cada Cidade compra escravos para as vagas Braçais vazias dela.
void rodarOsMercadosNaEstacao(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaEscravidao& regras = dados.regrasDaEscravidao;
    const int64_t hoje = vila.relogio.diaDaPartida();
    std::vector<SimulacaoDaVila*> mercados;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        if (cidade == &vila || !cidadeTemMercadoDeEscravos(vila, cidade->numeroDoMapaNoMundo())) {
            continue;
        }
        mercados.push_back(cidade);
        ContextoDaProducao contexto = cidade->contextoDaProducao();
        const int faltam = estoqueAlvoDoMercado(*cidade) - static_cast<int>(escravosAVenda(*cidade).size());
        for (int cativo = 0; cativo < std::min(faltam, regras.cativosDosBandidosPorEstacao); ++cativo) {
            const entt::entity novo = criarNpcNaVila(contexto, dados.regras.populacao.idadeAdulto + 5, 50, 50, true);
            if (novo == entt::null) {
                break;  // sem cama livre na cidade
            }
            tornarEscravo(contexto, novo, hoje);
            cidade->registroDasEntidades.get<CativeiroDoNpc>(novo).aVenda = true;
        }
    }
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        const int mapa = cidade->numeroDoMapaNoMundo();
        if (cidade == &vila || !escravidaoPermitidaNaCidade(vila, mapa)) {
            continue;
        }
        ContextoDaProducao contexto = cidade->contextoDaProducao();
        int vagas = contarVagasBracaisVaziasDaCidade(contexto);
        // O Mercado da própria cidade: o dono é a Cidade, então o escravo só sai da venda.
        for (const entt::entity npc : escravosAVenda(*cidade)) {
            if (vagas <= 0) {
                break;
            }
            cidade->registroDasEntidades.get<CativeiroDoNpc>(npc).aVenda = false;
            --vagas;
        }
        // Os outros Mercados, na ordem do número do mapa: o Banco precisa da reserva mais o preço.
        const std::string cultura = culturaBaseDaCidade(dados, mapa);
        for (SimulacaoDaVila* mercado : mercados) {
            if (mercado == cidade) {
                continue;
            }
            for (const entt::entity npc : escravosAVenda(*mercado)) {
                const int64_t preco = precoNoMercadoEmDecimos(*mercado, npc, cultura);
                if (vagas <= 0 || cidade->dinheiro.bancoDaCidade < static_cast<int64_t>(regras.reservaDoBancoParaComprarEmMoedas) * 10 + preco) {
                    break;
                }
                if (moverEscravoDeCidade(*mercado, npc, *cidade, false, false) == entt::null) {
                    vagas = 0;  // sem cama livre
                    break;
                }
                cidade->dinheiro.bancoDaCidade -= preco;
                mercado->dinheiro.bancoDaCidade += preco;
                --vagas;
            }
        }
    }
}

void soltarOJogador(SimulacaoDaVila& vila)
{
    vila.cativeiroDoJogador = EstadoDoCativeiroDoJogador{};
}

// Regra 4.8: o jogador cativo, todo dia. A Cidade o compra no dia 1 da estação seguinte; comprado, ele
// junta o pecúlio (ES1-7); a abolição o liberta.
void rodarOCativeiroDoJogador(SimulacaoDaVila& vila, std::vector<std::string>& linhas)
{
    EstadoDoCativeiroDoJogador& cativeiro = vila.cativeiroDoJogador;
    if (!cativeiro.estaCativo) {
        return;
    }
    const RegrasDaEscravidao& regras = vila.dados.regrasDaEscravidao;
    const int64_t hoje = vila.relogio.diaDaPartida();
    const bool primeiroDiaDaEstacao = vila.relogio.diaDaEstacaoAtual() == 1;
    if (primeiroDiaDaEstacao && !escravidaoPermitidaNaCidade(vila, cativeiro.mapaDoMercado)) {
        soltarOJogador(vila);
        linhas.push_back("Escravidão: a abolição libertou você.");
        return;
    }
    if (!cativeiro.foiComprado) {
        if (primeiroDiaDaEstacao && hoje > cativeiro.diaDaCaptura) {
            cativeiro.foiComprado = true;
            linhas.push_back("Escravidão: a Cidade de " + nomeDaCidadeNoMapa(vila.dados, cativeiro.mapaDoMercado) + " comprou você. Trabalhe e junte o pecúlio, ou tente fugir.");
        }
        return;
    }
    cativeiro.peculioEmDecimos += regras.peculioMaximoPorDiaEmDecimos;
}

}  // namespace

SimulacaoDaVila* cidadeDoMundoNoMapa(SimulacaoDaVila& vila, int numeroDoMapa)
{
    return cidadeDoMapa(vila, numeroDoMapa);
}

bool cidadeTemMercadoDeEscravos(const SimulacaoDaVila& vila, int numeroDoMapa)
{
    const DadosDoJogo& dados = vila.dados;
    if (dados.mundo.cidadeNoMapa(numeroDoMapa) == nullptr || !escravidaoPermitidaNaCidade(vila, numeroDoMapa)) {
        return false;
    }
    for (const DefinicaoDaFaccao& faccao : dados.faccoes.faccoes) {
        if (dados.mundo.numeroDoMapaNa(faccao.colunaDaCapital, faccao.linhaDaCapital) == numeroDoMapa) {
            return true;
        }
    }
    for (const std::vector<int>& par : dados.regrasDaEscravidao.outrasCidadesComMercado) {
        if (par.size() == 2 && dados.mundo.numeroDoMapaNa(par[0], par[1]) == numeroDoMapa) {
            return true;
        }
    }
    return false;
}

std::vector<entt::entity> escravosAVenda(const SimulacaoDaVila& cidade)
{
    std::vector<entt::entity> aVenda;
    for (const entt::entity npc : cidade.npcsDaVila) {
        const CativeiroDoNpc* cativeiro = cidade.registroDasEntidades.try_get<CativeiroDoNpc>(npc);
        if (cativeiro != nullptr && cativeiro->aVenda) {
            aVenda.push_back(npc);
        }
    }
    return aVenda;
}

int estoqueAlvoDoMercado(SimulacaoDaVila& cidade)
{
    const RegrasDaEscravidao& regras = cidade.dados.regrasDaEscravidao;
    const ContextoDaProducao contexto = cidade.contextoDaProducao();
    return std::max(regras.mercadoAlvoMinimo, regras.mercadoAlvoPorVagaBracalVazia * contarVagasBracaisVaziasDaCidade(contexto));
}

int64_t precoNoMercadoEmDecimos(SimulacaoDaVila& cidade, entt::entity npc, const std::string& culturaDoComprador)
{
    const DadosDoJogo& dados = cidade.dados;
    const HabilidadesDoNpc* habilidades = cidade.registroDasEntidades.try_get<HabilidadesDoNpc>(npc);
    const IdadeDoNpc* idade = cidade.registroDasEntidades.try_get<IdadeDoNpc>(npc);
    const std::string& cultura = cidade.registroDasEntidades.get<IdentidadeDoNpc>(npc).cultura;
    const bool hostil = !culturaDoComprador.empty() && dados.culturas.afinidade(culturaDoComprador, cultura) == AfinidadeEntreCulturas::Hostil;
    const int precoBase = precoBaseDoEscravo(dados.regrasDaEscravidao, habilidades != nullptr ? std::max(habilidades->bracal, habilidades->manufaturado) : 0, false,
                                             idade != nullptr && idade->anos >= dados.regras.populacao.idadeVelho, hostil);
    const int fator = calcularFatorDoMercadoLocal(static_cast<int>(escravosAVenda(cidade).size()), static_cast<int64_t>(estoqueAlvoDoMercado(cidade)) * 100,
                                                  dados.regras.economia.mercadoLocal);
    return static_cast<int64_t>(precoBase) * fator / 100 * 10;
}

entt::entity moverEscravoDeCidade(SimulacaoDaVila& origem, entt::entity npc, SimulacaoDaVila& destino, bool doJogador, bool aVenda)
{
    const IdentidadeDoNpc identidade = origem.registroDasEntidades.get<IdentidadeDoNpc>(npc);
    const HabilidadesDoNpc habilidades = origem.registroDasEntidades.get<HabilidadesDoNpc>(npc);
    const int idade = origem.registroDasEntidades.get<IdadeDoNpc>(npc).anos;
    const CativeiroDoNpc cativeiroAntigo = origem.registroDasEntidades.get<CativeiroDoNpc>(npc);
    ContextoDaProducao contextoDoDestino = destino.contextoDaProducao();
    const entt::entity novo = criarNpcNaVila(contextoDoDestino, idade, habilidades.bracal, habilidades.manufaturado, true);
    if (novo == entt::null) {
        return entt::null;
    }
    IdentidadeDoNpc& identidadeNova = destino.registroDasEntidades.get<IdentidadeDoNpc>(novo);
    identidadeNova.nome = identidade.nome;
    identidadeNova.cultura = identidade.cultura;
    // O criarNpcNaVila soma os pontos da cultura nas outras cidades; a habilidade dele não muda na viagem.
    destino.registroDasEntidades.get<HabilidadesDoNpc>(novo) = habilidades;
    tornarEscravo(contextoDoDestino, novo, cativeiroAntigo.diaDoInicio);
    CativeiroDoNpc& cativeiroNovo = destino.registroDasEntidades.get<CativeiroDoNpc>(novo);
    cativeiroNovo.obedienciaEmDecimos = cativeiroAntigo.obedienciaEmDecimos;
    cativeiroNovo.doJogador = doJogador;
    cativeiroNovo.aVenda = aVenda;
    ContextoDaProducao contextoDaOrigem = origem.contextoDaProducao();
    tirarNpcDaVila(contextoDaOrigem, npc);
    return novo;
}

int64_t custoDaLiberdadeEmDecimos(const SimulacaoDaVila& vila)
{
    return static_cast<int64_t>(vila.cativeiroDoJogador.precoEmMoedas) * vila.dados.regrasDaEscravidao.comprarALiberdadeEmPorcentoDoPreco / 100 * 10;
}

std::string fazerAcaoDaEscravidao(SimulacaoDaVila& vila, AcaoDaEscravidao acao, int numero, std::vector<std::string>& linhas)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaEscravidao& regras = dados.regrasDaEscravidao;
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    EstadoDoCativeiroDoJogador& cativeiro = vila.cativeiroDoJogador;
    const int64_t hoje = vila.relogio.diaDaPartida();
    const bool acaoDeCativo = acao == AcaoDaEscravidao::ComprarALiberdade || acao == AcaoDaEscravidao::TentarFugir;
    if (acaoDeCativo != cativeiro.estaCativo) {
        return cativeiro.estaCativo ? "você é escravo" : "você não é escravo";
    }

    switch (acao) {
    case AcaoDaEscravidao::ComprarALiberdade: {
        const int64_t custo = custoDaLiberdadeEmDecimos(vila);
        if (cativeiro.peculioEmDecimos + grupo.dinheiroEmDecimos < custo) {
            return "faltam " + std::to_string((custo - cativeiro.peculioEmDecimos - grupo.dinheiroEmDecimos + 9) / 10) + " moedas para a liberdade";
        }
        grupo.dinheiroEmDecimos -= std::max<int64_t>(0, custo - cativeiro.peculioEmDecimos);
        if (SimulacaoDaVila* cidade = cidadeDoMundoNoMapa(vila, cativeiro.mapaDoMercado); cidade != nullptr) {
            cidade->dinheiro.bancoDaCidade += custo;
        }
        soltarOJogador(vila);
        linhas.push_back("Escravidão: você comprou a sua liberdade por " + std::to_string(custo / 10) + " moedas.");
        return "";
    }
    case AcaoDaEscravidao::TentarFugir: {
        if (hoje <= cativeiro.semFugirAte) {
            return "você foi castigado e só pode tentar de novo depois do dia " + std::to_string(cativeiro.semFugirAte + 1);
        }
        SorteioComSemente sorteio(vila.vila.sementeDaPartida ^ (static_cast<uint64_t>(vila.relogio.tickAtual() + 1) * 0x9E3779B97F4A7C15ULL), 6000);
        if (sorteio.sortearNumeroInteiroEntre(1, 100) <= regras.fugaDetectadaEmPorcento) {
            cativeiro.semFugirAte = hoje + regras.castigoDaFugaEmDias;
            linhas.push_back("Escravidão: a fuga falhou. Você foi visto e castigado; só pode tentar de novo em " + std::to_string(regras.castigoDaFugaEmDias) + " dias.");
            return "";
        }
        const int mapa = cativeiro.mapaDoMercado;
        soltarOJogador(vila);
        std::string linha = "Escravidão: você fugiu de " + nomeDaCidadeNoMapa(dados, mapa) + ".";
        const int faccao = faccaoDaCidade(dados, mapa);
        if (faccao >= 0 && faccao < static_cast<int>(vila.crimeDoJogador.recompensaPorFaccao.size())) {
            vila.crimeDoJogador.recompensaPorFaccao[static_cast<std::size_t>(faccao)] += regras.recompensaPelaFugaEmMoedas;
            linha += " " + nomeDaFaccaoComArtigo(dados, faccao, "", true) + " pôs uma Recompensa de " + std::to_string(regras.recompensaPelaFugaEmMoedas) + " moedas por você.";
        }
        linhas.push_back(linha);
        return "";
    }
    case AcaoDaEscravidao::ComprarNoMercado: {
        SimulacaoDaVila* cidade = cidadeOndeOGrupoEsta(vila);
        if (cidade == nullptr || !cidadeTemMercadoDeEscravos(vila, cidade->numeroDoMapaNoMundo())) {
            return "o grupo precisa estar parado numa cidade com Mercado de Escravos";
        }
        if (!escravidaoPermitidaNaCidade(vila, dados.mundo.mapaDaVila)) {
            return "a lei da sua vila proíbe a escravidão";
        }
        const entt::entity npc = npcDeNumero(*cidade, numero);
        const CativeiroDoNpc* doNpc = npc != entt::null ? cidade->registroDasEntidades.try_get<CativeiroDoNpc>(npc) : nullptr;
        if (doNpc == nullptr || !doNpc->aVenda) {
            return "esse escravo não está à venda";
        }
        const int64_t preco = precoNoMercadoEmDecimos(*cidade, npc, culturaBaseDaCidade(dados, dados.mundo.mapaDaVila));
        if (grupo.dinheiroEmDecimos < preco) {
            return "o grupo não tem as " + std::to_string((preco + 9) / 10) + " moedas";
        }
        const std::string nome = cidade->registroDasEntidades.get<IdentidadeDoNpc>(npc).nome;
        if (moverEscravoDeCidade(*cidade, npc, vila, true, false) == entt::null) {
            return "não há cama livre na sua vila";
        }
        grupo.dinheiroEmDecimos -= preco;
        cidade->dinheiro.bancoDaCidade += preco;
        linhas.push_back("Escravidão: você comprou " + nome + " por " + std::to_string(preco / 10) + " moedas. Ele foi levado para a sua vila.");
        mudarAReputacaoCom(vila, "religiosa", regras.reputacaoReligiosaAoComprar, "comprou um escravo", linhas);
        return "";
    }
    default:
        break;
    }

    // As ações sobre um escravo do jogador, na vila dele.
    ContextoDaProducao contextoDaVila = vila.contextoDaProducao();
    if (acao == AcaoDaEscravidao::CastigoPublico) {
        bool temEscravo = false;
        for (const entt::entity outro : vila.npcsDaVila) {
            const CativeiroDoNpc* doOutro = vila.registroDasEntidades.try_get<CativeiroDoNpc>(outro);
            temEscravo = temEscravo || (doOutro != nullptr && doOutro->doJogador);
        }
        if (!temEscravo) {
            return "você não tem escravos";
        }
        if (vila.escravidao.castigoPublicoAte >= hoje) {
            return "já há um castigo público valendo";
        }
        vila.escravidao.castigoPublicoAte = hoje + regras.castigoPublicoEmDias;
        linhas.push_back("Escravidão: você mandou fazer um castigo público. O Medo sobe e a Felicidade dos escravos cai por " + std::to_string(regras.castigoPublicoEmDias) + " dias.");
        mudarAReputacaoCom(vila, "religiosa", regras.reputacaoReligiosaNoCastigoPublico, "castigou escravos em público", linhas);
        return "";
    }
    const entt::entity npc = npcDeNumero(vila, numero);
    CativeiroDoNpc* doNpc = npc != entt::null ? vila.registroDasEntidades.try_get<CativeiroDoNpc>(npc) : nullptr;
    if (doNpc == nullptr || !doNpc->doJogador) {
        return "esse NPC não é um escravo seu";
    }
    const std::string nome = vila.registroDasEntidades.get<IdentidadeDoNpc>(npc).nome;
    switch (acao) {
    case AcaoDaEscravidao::PorGrilhoes:
    case AcaoDaEscravidao::TirarGrilhoes:
        doNpc->grilhoes = acao == AcaoDaEscravidao::PorGrilhoes;
        linhas.push_back(std::string("Escravidão: ") + nome + (doNpc->grilhoes ? " agora usa Grilhões." : " não usa mais Grilhões."));
        return "";
    case AcaoDaEscravidao::Libertar: {
        libertarEscravo(contextoDaVila, npc);
        linhas.push_back("Escravidão: você libertou " + nome + ". Agora é um Civil da vila.");
        mudarAReputacaoCom(vila, "religiosa", regras.reputacaoReligiosaAoLibertar, "libertou um escravo", linhas);
        for (std::size_t faccao = 0; faccao < dados.faccoes.faccoes.size(); ++faccao) {
            const DefinicaoDaFaccao& definicao = dados.faccoes.faccoes[faccao];
            if (escravidaoPermitidaNaCidade(vila, dados.mundo.numeroDoMapaNa(definicao.colunaDaCapital, definicao.linhaDaCapital))) {
                mudarAReputacaoCom(vila, definicao.cultura, regras.reputacaoDeQuemPermiteAoLibertar, "libertou um escravo", linhas);
            }
        }
        return "";
    }
    case AcaoDaEscravidao::Vender: {
        SimulacaoDaVila* cidade = cidadeOndeOGrupoEsta(vila);
        if (cidade == nullptr || !cidadeTemMercadoDeEscravos(vila, cidade->numeroDoMapaNoMundo())) {
            return "o grupo precisa estar parado numa cidade com Mercado de Escravos";
        }
        // O dono vende por 70% do preço do Mercado (o preço é calculado como se ele já estivesse lá).
        const HabilidadesDoNpc habilidades = vila.registroDasEntidades.get<HabilidadesDoNpc>(npc);
        const bool velho = vila.registroDasEntidades.get<IdadeDoNpc>(npc).anos >= dados.regras.populacao.idadeVelho;
        // O estoque conta com ele (+1): sem isso, comprar com o Mercado cheio e revender com ele vazio daria lucro.
        const int fator = calcularFatorDoMercadoLocal(static_cast<int>(escravosAVenda(*cidade).size()) + 1, static_cast<int64_t>(estoqueAlvoDoMercado(*cidade)) * 100,
                                                      dados.regras.economia.mercadoLocal);
        const int64_t valor = static_cast<int64_t>(precoBaseDoEscravo(regras, std::max(habilidades.bracal, habilidades.manufaturado), false, velho, false)) * fator / 100
            * regras.vendaPeloDonoEmPorcento / 100 * 10;
        if (cidade->dinheiro.bancoDaCidade < valor) {
            return "o Mercado não tem as " + std::to_string((valor + 9) / 10) + " moedas";
        }
        if (moverEscravoDeCidade(vila, npc, *cidade, false, true) == entt::null) {
            return "o Mercado não tem lugar para mais um escravo";
        }
        cidade->dinheiro.bancoDaCidade -= valor;
        grupo.dinheiroEmDecimos += valor;
        linhas.push_back("Escravidão: você vendeu " + nome + " por " + std::to_string(valor / 10) + " moedas.");
        mudarAReputacaoCom(vila, "religiosa", regras.reputacaoReligiosaAoVender, "vendeu um escravo", linhas);
        return "";
    }
    default:
        return "ação desconhecida";
    }
}

bool sortearACapturaDoJogador(SimulacaoDaVila& vila, std::vector<std::string>& linhas)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaEscravidao& regras = dados.regrasDaEscravidao;
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    if (vila.cativeiroDoJogador.estaCativo) {
        return false;
    }
    SorteioComSemente sorteio(vila.vila.sementeDaPartida ^ (static_cast<uint64_t>(vila.relogio.tickAtual() + 1) * 0x9E3779B97F4A7C15ULL), 6001);
    if (sorteio.sortearNumeroInteiroEntre(1, 100) > regras.capturaDoJogadorPelosBandidosEmPorcento) {
        return false;
    }
    // O Mercado mais perto; no empate, o de menor número de mapa.
    int mapaDoMercado = 0;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        const int mapa = cidade->numeroDoMapaNoMundo();
        if (cidade != &vila && cidadeTemMercadoDeEscravos(vila, mapa)
            && (mapaDoMercado == 0 || distanciaEntreMapas(dados.mundo, grupo.mapaAtual, mapa) < distanciaEntreMapas(dados.mundo, grupo.mapaAtual, mapaDoMercado))) {
            mapaDoMercado = mapa;
        }
    }
    if (mapaDoMercado == 0) {
        return false;  // nenhum Mercado no mundo: os bandidos só roubam
    }
    grupo.emTransito = false;
    grupo.mapaAtual = mapaDoMercado;
    grupo.proximoMapa = 0;
    grupo.rota.clear();
    grupo.ticksDoTrecho = 0;
    grupo.ticksAndadosNoTrecho = 0;
    grupo.dinheiroEmDecimos = 0;  // os captores ficam com o dinheiro que sobrou
    EstadoDoCativeiroDoJogador& cativeiro = vila.cativeiroDoJogador;
    cativeiro = EstadoDoCativeiroDoJogador{};
    cativeiro.estaCativo = true;
    cativeiro.mapaDoMercado = mapaDoMercado;
    cativeiro.precoEmMoedas = precoBaseDoEscravo(regras, regras.habilidadeDoJogadorNoPreco, false, false, false);
    cativeiro.diaDaCaptura = vila.relogio.diaDaPartida();
    linhas.push_back("Escravidão: os bandidos capturaram o grupo e o levaram ao Mercado de Escravos de " + nomeDaCidadeNoMapa(dados, mapaDoMercado) + " (preço: "
                     + std::to_string(cativeiro.precoEmMoedas) + " moedas).");
    return true;
}

void anotarONascimentoNaEscravidao(ContextoDaProducao& contexto, entt::entity npc)
{
    if (contexto.escravidao == nullptr || contexto.npcsDaVila.empty()) {
        return;
    }
    int escravos = 0;
    for (const entt::entity outro : contexto.npcsDaVila) {
        escravos += contexto.registroDasEntidades.all_of<CativeiroDoNpc>(outro) ? 1 : 0;
    }
    // O nascido já está na lista; a parte de escravos é sobre quem já vivia na cidade.
    const int moradores = static_cast<int>(contexto.npcsDaVila.size()) - 1;
    if (escravos <= 0 || moradores <= 0) {
        return;
    }
    contexto.escravidao->nascimentosAcumuladosEmMilesimos += escravos * 1000 / moradores;
    if (contexto.escravidao->nascimentosAcumuladosEmMilesimos >= 1000) {
        contexto.escravidao->nascimentosAcumuladosEmMilesimos -= 1000;
        tornarEscravo(contexto, npc, contexto.relogio != nullptr ? contexto.relogio->diaDaPartida() : 0);
    }
}
