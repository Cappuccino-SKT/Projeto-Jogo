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
    estado.escravosSemComidaHoje = alimentarOsEscravosDaCidade(contexto, static_cast<int>(escravos.size()));

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
        const int felicidadeDele = estado.felicidadeDosEscravosEmDecimos + (cativeiro.castigadoAte >= hoje ? regras.castigoDaFugaNaFelicidade * 10 : 0);
        const int estacoes = static_cast<int>((hoje - cativeiro.diaDoInicio) / std::max(1, vila.relogio.diasPorEstacao()));
        cativeiro.obedienciaEmDecimos = calcularObedienciaEmDecimos(regras, controle, std::max(0, felicidadeDele), cativeiro.grilhoes, estacoes,
                                                                    cidade.registroDasEntidades.get<IdentidadeDoNpc>(npc).cultura);
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
    registro.emplace<CativeiroDoNpc>(npc, CativeiroDoNpc{dia, false, 500, -1});
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
    }
}

std::vector<std::string> rodarAEscravidaoNoDiaNovo(SimulacaoDaVila& vila)
{
    std::vector<std::string> linhas;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        rodarODiaDaCidade(vila, *cidade, linhas);
    }
    return linhas;
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
