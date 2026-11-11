// Os testes da escravidão: a tabela da parte 8 da Ficha Lógica: Escravidão (fase D3, parte 1).

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
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

}  // namespace

void testarEscravidao()
{
    std::printf("Escravidão: o preço, a Obediência, a fuga, a rebelião, a alforria e a abolição (fase D3, parte 1)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com o escravidao.json, não têm erro");
    testarAsContas(dados);
    testarOMundo(dados);
}
