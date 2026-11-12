// Os testes da escravidão: a tabela da parte 8 da Ficha Lógica: Escravidão (fase D3, partes 1 e 2).

#include <algorithm>
#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Comercio/Comercio.h"
#include "Cultura/Cultura.h"
#include "Jogador/PedidosDoJogador.h"
#include "Reputacao/Reputacao.h"
#include "Vila/Populacao.h"
#include "Dados/DadosDoJogo.h"
#include "Escravidao/Escravidao.h"
#include "Faccao/Faccao.h"
#include "Npc/ComponentesDoNpc.h"
#include "Save/ArquivoDeSave.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

SimulacaoDaVila* cidadeNa(SimulacaoDaVila& vila, int coluna, int linha)
{
    const int mapa = vila.dados.mundo.numeroDoMapaNa(coluna, linha);
    for (const auto& cidade : vila.outrasCidades) {
        if (cidade->numeroDoMapaDaCidade == mapa) {
            return cidade.get();
        }
    }
    return nullptr;
}

int faccaoDaCultura(const DadosDoJogo& dados, const std::string& cultura)
{
    for (std::size_t posicao = 0; posicao < dados.faccoes.faccoes.size(); ++posicao) {
        if (dados.faccoes.faccoes[posicao].cultura == cultura) {
            return static_cast<int>(posicao);
        }
    }
    return -1;
}

void testarAsContas(const DadosDoJogo& dados)
{
    const RegrasDaEscravidao& regras = dados.regrasDaEscravidao;
    // Teste 1: os números da parte 5.
    conferir(precoBaseDoEscravo(regras, 80, false, false, false) == 520 && precoBaseDoEscravo(regras, 80, false, true, false) == 260,
             "o preço: Braçal 80 custa 520; velho, 260");
    conferir(precoBaseDoEscravo(regras, 50, true, true, true) == 320, "o preço: Mago, velho e de cultura hostil com habilidade 50: 400 × 2 × 0,5 × 0,8 = 320");
    conferir(calcularObedienciaEmDecimos(regras, 334, 250, false, 0, "autoritaria") == 550 && calcularObedienciaEmDecimos(regras, 334, 250, true, 0, "autoritaria") == 700,
             "a Obediência do documento: Medo 10 e Felicidade 25 dão 55; com Grilhões, 70");
    conferir(calcularObedienciaEmDecimos(regras, 890, 300, false, 0, "autoritaria") == 767, "uma cidade tranquila (Criminalidade 11): Obediência 76,7");
    conferir(calcularObedienciaEmDecimos(regras, 440, 100, false, 0, "autoritaria") == 432, "a mesma cidade em fome (Criminalidade 56, Felicidade 10): 43,2");
    conferir(calcularObedienciaEmDecimos(regras, 890, 300, false, 0, "guerreira") == 667 && calcularObedienciaEmDecimos(regras, 890, 300, false, 25, "autoritaria") == 867,
             "a cultura Guerreira tira 10; o cativeiro soma 1 por estação, até 10");
    // Teste 4: a fuga.
    conferir(chanceDeTentarFugaEmPorMil(regras, 200, false) == 100 && chanceDeTentarFugaEmPorMil(regras, 200, true) == 0 && chanceDeTentarFugaEmPorMil(regras, 767, false) == 0,
             "a fuga: Obediência 20 dá 10% por noite; com Grilhões ou Obediência acima de 30, nunca");
    // A Felicidade dos Escravos.
    conferir(calcularAlvoDaFelicidadeDosEscravos(regras, 10, 0, 0, 0, 0, -25, -10) == 300 && calcularAlvoDaFelicidadeDosEscravos(regras, 10, 10, 0, 0, 0, -25, -10) == 50
                 && calcularAlvoDaFelicidadeDosEscravos(regras, 10, 5, 10, 0, -10, -25, -10) == 0,
             "a Felicidade dos Escravos: base 30; sem comida, 5; sem comida pela metade, sem casa e castigados, 0");
    // A alforria: 1% de 40 escravos por estação.
    int acumulado = 0;
    const int primeira = alforriadosNaEstacao(40, 1, acumulado);
    const int segunda = alforriadosNaEstacao(40, 1, acumulado);
    const int terceira = alforriadosNaEstacao(40, 1, acumulado);
    conferir(primeira == 0 && segunda == 0 && terceira == 1 && acumulado == 20, "a alforria: 1% de 40 escravos liberta 1 na 3ª estação");
}

void testarOMundo(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    SimulacaoDaVila* imperio = cidadeNa(vila, 1, 4);     // a capital do Império Mágico (Autoritária)
    SimulacaoDaVila* uniao = cidadeNa(vila, 8, 1);       // a capital da União Mágica (Arcana)
    SimulacaoDaVila* clero = cidadeNa(vila, 8, 4);       // a capital do Clero de Rudeus (Religiosa)
    SimulacaoDaVila* porto = cidadeNa(vila, 8, 6);       // o Grande porto, sem facção, de cultura Escravista
    SimulacaoDaVila* vilaLivre = cidadeNa(vila, 4, 1);   // a Vila Livre do Norte
    conferir(imperio != nullptr && uniao != nullptr && clero != nullptr && porto != nullptr && vilaLivre != nullptr, "as cidades do teste existem");
    if (imperio == nullptr || uniao == nullptr || clero == nullptr || porto == nullptr || vilaLivre == nullptr) {
        return;
    }
    // Rodada LE1-1 e LE1-4: os escravos do primeiro dia.
    const int npcsDoImperio = static_cast<int>(imperio->npcsDaVila.size());
    const int npcsDaUniao = static_cast<int>(uniao->npcsDaVila.size());
    const int npcsDoPorto = static_cast<int>(porto->npcsDaVila.size());
    std::printf("    escravos no primeiro dia: Império %d de %d, União %d de %d, Clero %d, Grande porto %d de %d, Vila Livre do Norte %d\n", contarOsEscravos(*imperio),
                npcsDoImperio, contarOsEscravos(*uniao), npcsDaUniao, contarOsEscravos(*clero), contarOsEscravos(*porto), npcsDoPorto, contarOsEscravos(*vilaLivre));
    conferir(contarOsEscravos(*imperio) == npcsDoImperio * 30 / 100 && contarOsEscravos(*uniao) == npcsDaUniao * 25 / 100 && contarOsEscravos(*clero) == 0
                 && contarOsEscravos(*porto) == npcsDoPorto * 30 / 100 && contarOsEscravos(*vilaLivre) == 0 && contarOsEscravos(vila) == 0,
             "LE1-1: Império 30%, União 25%, Clero 0%; LE1-4: o Grande porto 30%; as Vilas Livres e a vila do jogador, nenhum");
    conferir(imperio->dinheiro.quantidadeDeCivis == npcsDoImperio - contarOsEscravos(*imperio), "o escravo sai da conta dos Civis da cidade");
    conferir(escravidaoPermitidaNaCidade(vila, imperio->numeroDoMapaDaCidade) && !escravidaoPermitidaNaCidade(vila, clero->numeroDoMapaDaCidade)
                 && opcaoDaLeiNaCidade(vila, porto->numeroDoMapaDaCidade, "escravidao") == "ampla",
             "a lei: permitida no Império, proibida no Clero; no Grande porto, sem facção, vale o desejo da cultura (ampla)");

    // Teste 5: a abolição. A União proíbe; no dia 1 da estação (hoje), todos viram Civis.
    const int arcana = faccaoDaCultura(dados, "arcana");
    vila.faccoes.leisAtuais[static_cast<std::size_t>(arcana)][static_cast<std::size_t>(dados.faccoes.posicaoDaLei("escravidao"))] = 0;
    const int escravosDoImperioAntes = contarOsEscravos(*imperio);
    rodarAEscravidaoNoDiaNovo(vila);
    conferir(contarOsEscravos(*uniao) == 0 && uniao->dinheiro.quantidadeDeCivis == npcsDaUniao && contarOsEscravos(*imperio) == escravosDoImperioAntes,
             "a abolição: os escravos da União viram Civis no dia 1 da estação; os do Império continuam");
    // A comida saiu do estoque da Cidade, e a Obediência de uma cidade tranquila ficou em 76,7.
    int obedienciaDeUmEscravo = -1;
    for (const entt::entity npc : imperio->npcsDaVila) {
        if (const CativeiroDoNpc* cativeiro = imperio->registroDasEntidades.try_get<CativeiroDoNpc>(npc)) {
            obedienciaDeUmEscravo = cativeiro->obedienciaEmDecimos;
            break;
        }
    }
    conferir(imperio->escravidao.escravosSemComidaHoje == 0 && imperio->escravidao.felicidadeDosEscravosEmDecimos == 300 && obedienciaDeUmEscravo == 767,
             "o dia dos escravos do Império: todos comem do estoque da Cidade; Felicidade 30; Obediência 76,7");

    // Teste 4 no mundo: numa cidade em colapso, os escravos tentam fugir.
    SimulacaoDaVila* cidadeEmColapso = cidadeNa(vila, 2, 3);  // a Cidade-fortaleza do Império
    SimulacaoDaVila* cidadeRebelde = cidadeNa(vila, 2, 6);    // a capital da Federação Humana
    conferir(cidadeEmColapso != nullptr && cidadeRebelde != nullptr && contarOsEscravos(*cidadeEmColapso) > 0 && contarOsEscravos(*cidadeRebelde) > 0,
             "a Cidade-fortaleza e a capital da Federação começam com escravos");
    if (cidadeEmColapso == nullptr || cidadeRebelde == nullptr) {
        return;
    }
    cidadeEmColapso->escravidao.felicidadeDosEscravosEmDecimos = 0;
    cidadeEmColapso->felicidade.criminalidadeEmDecimos = 1000;
    const int escravosAntesDaFuga = contarOsEscravos(*cidadeEmColapso);
    // Teste da rebelião: 3 dias seguidos já contados; o 4º é amanhã.
    cidadeRebelde->escravidao.felicidadeDosEscravosEmDecimos = 0;
    cidadeRebelde->felicidade.criminalidadeEmDecimos = 1000;
    cidadeRebelde->escravidao.diasSeguidosParaRebeliao = dados.regrasDaEscravidao.rebeliaoDiasSeguidos - 1;
    const int escravosAntesDaRebeliao = contarOsEscravos(*cidadeRebelde);
    while (vila.relogio.diaDaPartida() < 1) {
        vila.avancarUmTick();
    }
    int castigados = 0;
    for (const entt::entity npc : cidadeEmColapso->npcsDaVila) {
        const CativeiroDoNpc* cativeiro = cidadeEmColapso->registroDasEntidades.try_get<CativeiroDoNpc>(npc);
        castigados += cativeiro != nullptr && cativeiro->castigadoAte >= 0 ? 1 : 0;
    }
    std::printf("    a cidade em colapso: %d escravos, %d fugiram e %d foram vistos e castigados\n", escravosAntesDaFuga, cidadeEmColapso->escravidao.fugasNoTotal, castigados);
    conferir(cidadeEmColapso->escravidao.fugasNoTotal + castigados > 0 && contarOsEscravos(*cidadeEmColapso) == escravosAntesDaFuga - cidadeEmColapso->escravidao.fugasNoTotal,
             "a fuga: com Felicidade 0 e sem Controle, alguns tentam; quem não é visto sai da cidade");
    const int escravosDepoisDaRebeliao = contarOsEscravos(*cidadeRebelde);
    std::printf("    a rebelião: %d escravos antes, %d depois\n", escravosAntesDaRebeliao, escravosDepoisDaRebeliao);
    conferir(cidadeRebelde->escravidao.rebelioesNoTotal == 1 && cidadeRebelde->escravidao.diasSeguidosParaRebeliao <= 1
                 && (escravosDepoisDaRebeliao == 0
                     || (escravosDepoisDaRebeliao == escravosAntesDaRebeliao - escravosAntesDaRebeliao * 20 / 100 && cidadeRebelde->escravidao.escravosCastigadosAte > 0)),
             "a rebelião no 4º dia: ou todos fogem, ou 20% são executados e o resto volta castigado");

    // O save leva a escravidão.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(vila, "teste", "2026-11-11");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr, "o save com a escravidão carrega");
    if (carregado.partida != nullptr) {
        SimulacaoDaVila* imperioCarregado = cidadeNa(*carregado.partida, 1, 4);
        SimulacaoDaVila* rebeldeCarregada = cidadeNa(*carregado.partida, 2, 6);
        conferir(imperioCarregado != nullptr && rebeldeCarregada != nullptr && contarOsEscravos(*imperioCarregado) == contarOsEscravos(*imperio)
                     && imperioCarregado->escravidao == imperio->escravidao && rebeldeCarregada->escravidao == cidadeRebelde->escravidao
                     && imperioCarregado->dinheiro.quantidadeDeCivis == imperio->dinheiro.quantidadeDeCivis,
                 "salvar e carregar: os mesmos escravos e o mesmo estado da escravidão em cada cidade");
    }
}

int escravosDoJogadorNaVila(const SimulacaoDaVila& vila)
{
    int quantos = 0;
    for (const entt::entity npc : vila.npcsDaVila) {
        const CativeiroDoNpc* cativeiro = vila.registroDasEntidades.try_get<CativeiroDoNpc>(npc);
        quantos += cativeiro != nullptr && cativeiro->doJogador ? 1 : 0;
    }
    return quantos;
}

int64_t dinheiroDosBancosEDoGrupo(SimulacaoDaVila& vila)
{
    int64_t total = vila.viagem.grupoDoJogador.dinheiroEmDecimos;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        total += cidade->dinheiro.bancoDaCidade;
    }
    return total;
}

// Parte 2: o Mercado de Escravos, os escravos do jogador e o jogador capturado.
void testarOMercadoEOJogador(const DadosDoJogo& dadosDoJogo)
{
    // Para o teste, a cultura da vila do jogador passa a desejar a escravidão "ampla" (a lei da vila sem
    // facção é o desejo da cultura dela, LE1-4).
    DadosDoJogo dados = dadosDoJogo;
    const std::string culturaDaVila = culturaBaseDaCidade(dados, dados.mundo.mapaDaVila);
    const int leiDaEscravidao = dados.faccoes.posicaoDaLei("escravidao");
    const std::vector<std::string>& opcoes = dados.faccoes.leis[static_cast<std::size_t>(leiDaEscravidao)].opcoes;
    const int ampla = static_cast<int>(std::find(opcoes.begin(), opcoes.end(), "ampla") - opcoes.begin());
    {
        SimulacaoDaVila semALei(dadosDoJogo, RegrasDoTempo{});
        std::printf("    a vila do jogador (cultura %s): a lei de Escravidão é \"%s\"\n", culturaDaVila.c_str(),
                    opcaoDaLeiNaCidade(semALei, dadosDoJogo.mundo.mapaDaVila, "escravidao").c_str());
    }
    dados.faccoes.leisDesejadasPorCultura[culturaDaVila][static_cast<std::size_t>(leiDaEscravidao)] = LeiDesejada{ampla, 4};

    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    SimulacaoDaVila* porto = cidadeNa(vila, 8, 6);
    SimulacaoDaVila* imperio = cidadeNa(vila, 1, 4);
    if (porto == nullptr || imperio == nullptr) {
        conferir(false, "o Grande porto e a capital do Império existem");
        return;
    }
    const int mapaDoPorto = porto->numeroDoMapaDaCidade;
    // Os Mercados: as capitais dos reinos que permitem, o Grande porto e a Cidade dos mercados.
    conferir(cidadeTemMercadoDeEscravos(vila, imperio->numeroDoMapaDaCidade) && cidadeTemMercadoDeEscravos(vila, mapaDoPorto)
                 && cidadeTemMercadoDeEscravos(vila, dados.mundo.numeroDoMapaNa(7, 5)) && !cidadeTemMercadoDeEscravos(vila, dados.mundo.numeroDoMapaNa(8, 4))
                 && !cidadeTemMercadoDeEscravos(vila, dados.mundo.numeroDoMapaNa(2, 3)),
             "os Mercados: a capital do Império, o Grande porto e a Cidade dos mercados têm; a capital do Clero e a Cidade-fortaleza, não");
    const std::vector<entt::entity> estoque = escravosAVenda(*porto);
    const int alvo = estoqueAlvoDoMercado(*porto);
    conferir(static_cast<int>(estoque.size()) == alvo && alvo >= 2, "o Mercado do Grande porto começa com o estoque-alvo (2 ou mais)");
    if (estoque.empty()) {
        return;
    }
    const int numeroDoPrimeiro = porto->registroDasEntidades.get<IdentidadeDoNpc>(estoque.front()).numeroDoNpc;
    const HabilidadesDoNpc habilidadesDoPrimeiro = porto->registroDasEntidades.get<HabilidadesDoNpc>(estoque.front());
    const int64_t preco = precoNoMercadoEmDecimos(*porto, estoque.front(), culturaDaVila);
    std::printf("    o Mercado do Grande porto: %d à venda (alvo %d); o primeiro custa %lld moedas (Braçal %d)\n", static_cast<int>(estoque.size()), alvo,
                static_cast<long long>(preco / 10), habilidadesDoPrimeiro.bracal);
    conferir(preco >= 1000 && preco <= 20000, "o preço fica entre 100 e 2.000 moedas");

    // O jogador compra: o grupo parado no Grande porto, com dinheiro (a vila tem cama livre).
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    grupo.emTransito = false;
    grupo.mapaAtual = mapaDoPorto;
    grupo.dinheiroEmDecimos = 30000;
    const int religiosa = [&] {
        const std::vector<std::string> donos = donosDaReputacao(dados.mundo);
        return static_cast<int>(std::find(donos.begin(), donos.end(), "religiosa") - donos.begin());
    }();
    const int reputacaoAntes = vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(religiosa)];
    const int64_t bancoDoPortoAntes = porto->dinheiro.bancoDaCidade;
    const int civisDaVilaAntes = vila.dinheiro.quantidadeDeCivis;
    vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::ComprarNoMercado, numeroDoPrimeiro});
    vila.aplicarOsPedidosPendentes();
    conferir(escravosDoJogadorNaVila(vila) == 1 && grupo.dinheiroEmDecimos == 30000 - preco && porto->dinheiro.bancoDaCidade == bancoDoPortoAntes + preco
                 && static_cast<int>(escravosAVenda(*porto).size()) == alvo - 1 && vila.dinheiro.quantidadeDeCivis == civisDaVilaAntes
                 && vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(religiosa)] == reputacaoAntes - 1,
             "a compra: o escravo vai para a vila, o dinheiro passa do grupo para o Banco do porto, e a reputação com o Clero cai 1");
    entt::entity meuEscravo = entt::null;
    for (const entt::entity npc : vila.npcsDaVila) {
        const CativeiroDoNpc* cativeiro = vila.registroDasEntidades.try_get<CativeiroDoNpc>(npc);
        meuEscravo = cativeiro != nullptr && cativeiro->doJogador ? npc : meuEscravo;
    }
    if (meuEscravo == entt::null) {
        return;
    }
    const int numeroDoMeu = vila.registroDasEntidades.get<IdentidadeDoNpc>(meuEscravo).numeroDoNpc;
    conferir(vila.registroDasEntidades.get<HabilidadesDoNpc>(meuEscravo).bracal == habilidadesDoPrimeiro.bracal, "o escravo comprado chega com a mesma habilidade");

    // Os tratamentos: os Grilhões e o castigo público.
    vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::PorGrilhoes, numeroDoMeu});
    vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::CastigoPublico, 0});
    vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::CastigoPublico, 0});
    vila.aplicarOsPedidosPendentes();
    conferir(vila.registroDasEntidades.get<CativeiroDoNpc>(meuEscravo).grilhoes && vila.escravidao.castigoPublicoAte == 8
                 && vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(religiosa)] == reputacaoAntes - 3,
             "os Grilhões e o castigo público (8 dias; o segundo pedido é recusado; −2 de reputação com o Clero)");
    // A venda: 70% do preço do Mercado; o escravo volta para o estoque do porto.
    const int64_t dinheiroAntesDaVenda = grupo.dinheiroEmDecimos;
    vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::Vender, numeroDoMeu});
    vila.aplicarOsPedidosPendentes();
    conferir(escravosDoJogadorNaVila(vila) == 0 && grupo.dinheiroEmDecimos > dinheiroAntesDaVenda && grupo.dinheiroEmDecimos - dinheiroAntesDaVenda < preco
                 && vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(religiosa)] == reputacaoAntes - 6,
             "a venda: o jogador recebe menos do que pagou (70%), e a reputação com o Clero cai mais 3");

    // Comprar de novo e libertar: vira Civil da vila.
    const std::vector<entt::entity> estoqueDeNovo = escravosAVenda(*porto);
    if (!estoqueDeNovo.empty()) {
        vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::ComprarNoMercado, porto->registroDasEntidades.get<IdentidadeDoNpc>(estoqueDeNovo.back()).numeroDoNpc});
        vila.aplicarOsPedidosPendentes();
        int numeroDoSegundo = -1;
        for (const entt::entity npc : vila.npcsDaVila) {
            const CativeiroDoNpc* cativeiro = vila.registroDasEntidades.try_get<CativeiroDoNpc>(npc);
            numeroDoSegundo = cativeiro != nullptr && cativeiro->doJogador ? vila.registroDasEntidades.get<IdentidadeDoNpc>(npc).numeroDoNpc : numeroDoSegundo;
        }
        // O dia do escravo do jogador: ele come do estoque do jogador (que não tem comida no teste: fica
        // sem comer); com o castigo público valendo, o Medo sobe 5.
        rodarAEscravidaoNoDiaNovo(vila);
        int obedienciaDoSegundo = -1;
        for (const entt::entity npc : vila.npcsDaVila) {
            const CativeiroDoNpc* cativeiro = vila.registroDasEntidades.try_get<CativeiroDoNpc>(npc);
            obedienciaDoSegundo = cativeiro != nullptr && cativeiro->doJogador ? cativeiro->obedienciaEmDecimos : obedienciaDoSegundo;
        }
        std::printf("    o escravo do jogador: Obediência %d (em décimos), sem comida hoje: %d\n", obedienciaDoSegundo, vila.escravidao.escravosSemComidaHoje);
        conferir(vila.escravidao.escravosSemComidaHoje == 1 && obedienciaDoSegundo >= 600 && obedienciaDoSegundo <= 750,
                 "o escravo do jogador come do estoque do jogador (sem comida, a Felicidade cai); a Obediência fica perto de 69");
        const int civisAntes = vila.dinheiro.quantidadeDeCivis;
        vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::Libertar, numeroDoSegundo});
        vila.aplicarOsPedidosPendentes();
        conferir(numeroDoSegundo >= 0 && escravosDoJogadorNaVila(vila) == 0 && vila.dinheiro.quantidadeDeCivis == civisAntes + 1, "libertar: o escravo vira um Civil da vila");
    }

    // Teste 7: o dinheiro do mundo. O dia 1 da estação (os cativos de graça, as compras das Cidades) só passa dinheiro entre donos.
    const int64_t dinheiroAntes = dinheiroDosBancosEDoGrupo(vila);
    rodarAEscravidaoNoDiaNovo(vila);
    conferir(dinheiroDosBancosEDoGrupo(vila) == dinheiroAntes, "o dinheiro do mundo: as compras de escravos entre as cidades só passam dinheiro de um Banco para outro");

    // Teste 6: em 1.000 derrotas para bandidos, cerca de 500 capturas (o sorteio sai do tick).
    int capturas = 0;
    std::vector<std::string> linhas;
    for (int derrota = 0; derrota < 1000; ++derrota) {
        vila.relogio.avancarUmTick();
        grupo.mapaAtual = dados.mundo.numeroDoMapaNa(6, 6);
        capturas += sortearACapturaDoJogador(vila, linhas) ? 1 : 0;
        vila.cativeiroDoJogador = EstadoDoCativeiroDoJogador{};
    }
    std::printf("    capturas em 1.000 derrotas: %d\n", capturas);
    conferir(capturas >= 430 && capturas <= 570, "a captura: cerca de 500 em 1.000 derrotas para bandidos");

    // O jogador capturado: levado ao Mercado mais perto, sem dinheiro; não pode viajar.
    grupo.dinheiroEmDecimos = 5000;
    grupo.mapaAtual = dados.mundo.numeroDoMapaNa(6, 6);
    bool capturado = false;
    for (int tentativa = 0; tentativa < 50 && !capturado; ++tentativa) {
        vila.relogio.avancarUmTick();
        capturado = sortearACapturaDoJogador(vila, linhas);
    }
    EstadoDoCativeiroDoJogador& cativeiro = vila.cativeiroDoJogador;
    conferir(capturado && cativeiro.estaCativo && grupo.dinheiroEmDecimos == 0 && grupo.mapaAtual == cativeiro.mapaDoMercado && cativeiro.precoEmMoedas == 400
                 && cidadeTemMercadoDeEscravos(vila, cativeiro.mapaDoMercado) && custoDaLiberdadeEmDecimos(vila) == 6000,
             "capturado: levado a um Mercado, sem dinheiro; o preço dele é 400 e a liberdade custa 600");
    vila.pedidos.acrescentar(PedidoPartirEmViagem{dados.mundo.mapaDaVila, 0, 0});
    vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::ComprarALiberdade, 0});
    vila.aplicarOsPedidosPendentes();
    conferir(cativeiro.estaCativo && !grupo.emTransito, "escravo e sem pecúlio: não viaja e não compra a liberdade");
    // A Cidade o compra no dia 1 da estação seguinte; depois, 4 moedas de pecúlio por dia.
    cativeiro.diaDaCaptura = -1;
    const bool ehODia1 = vila.relogio.diaDaEstacaoAtual() == 1;
    rodarAEscravidaoNoDiaNovo(vila);
    rodarAEscravidaoNoDiaNovo(vila);
    conferir(ehODia1 && cativeiro.foiComprado && cativeiro.peculioEmDecimos == 40, "a Cidade compra o jogador no dia 1 da estação; no dia seguinte, 4 moedas de pecúlio");
    // 150 dias de pecúlio pagam a liberdade (600 moedas).
    cativeiro.peculioEmDecimos = 40 * 150;
    vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::ComprarALiberdade, 0});
    vila.aplicarOsPedidosPendentes();
    conferir(!cativeiro.estaCativo, "com 150 dias de pecúlio (600 moedas), o jogador compra a liberdade");

    // A fuga do jogador: ou é visto (castigo de 8 dias sem tentar), ou sai livre.
    cativeiro.estaCativo = true;
    cativeiro.mapaDoMercado = imperio->numeroDoMapaDaCidade;
    cativeiro.precoEmMoedas = 400;
    const int faccaoDoImperio = faccaoDaCultura(dados, "autoritaria");
    int fugas = 0;
    int vistos = 0;
    for (int tentativa = 0; tentativa < 40; ++tentativa) {
        vila.relogio.avancarUmTick();
        cativeiro.estaCativo = true;
        cativeiro.mapaDoMercado = imperio->numeroDoMapaDaCidade;
        cativeiro.semFugirAte = -1;
        vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::TentarFugir, 0});
        vila.aplicarOsPedidosPendentes();
        fugas += cativeiro.estaCativo ? 0 : 1;
        vistos += cativeiro.estaCativo && cativeiro.semFugirAte >= 0 ? 1 : 0;
    }
    std::printf("    a fuga do jogador em 40 tentativas: %d fugas, %d vistos\n", fugas, vistos);
    conferir(fugas + vistos == 40 && fugas >= 8 && vistos >= 8
                 && vila.crimeDoJogador.recompensaPorFaccao[static_cast<std::size_t>(faccaoDoImperio)] == fugas * dados.regrasDaEscravidao.recompensaPelaFugaEmMoedas,
             "a fuga do jogador: cerca de metade é vista; cada fuga põe uma Recompensa no Império");
    // Visto, ele não pode tentar de novo antes do fim do castigo.
    cativeiro.estaCativo = true;
    cativeiro.semFugirAte = vila.relogio.diaDaPartida() + 3;
    vila.pedidos.acrescentar(PedidoDaEscravidao{AcaoDaEscravidao::TentarFugir, 0});
    vila.aplicarOsPedidosPendentes();
    conferir(cativeiro.estaCativo, "castigado, o jogador não pode tentar fugir de novo");

    // O save leva o cativeiro do jogador e o Mercado.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(vila, "teste", "2026-11-11");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->cativeiroDoJogador == vila.cativeiroDoJogador && carregado.partida->escravidao == vila.escravidao
                 && cidadeNa(*carregado.partida, 8, 6) != nullptr && escravosAVenda(*cidadeNa(*carregado.partida, 8, 6)).size() == escravosAVenda(*porto).size(),
             "salvar e carregar: o mesmo cativeiro do jogador e o mesmo estoque do Mercado");
}

}  // namespace

void testarEscravidao()
{
    std::printf("Escravidão: o preço, a Obediência, a fuga, a rebelião, a alforria e a abolição (fase D3, parte 1)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com o escravidao.json, não têm erro");
    testarAsContas(dados);
    testarOMundo(dados);
    testarOMercadoEOJogador(dados);
}
