// Os testes do combate: a tabela da parte 8 da Ficha Lógica: Combate (fase D1).

#include <cstdio>
#include <string>

#include "Combate/Combate.h"
#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Tempo/SorteioComSemente.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

void testarAsContas(const RegrasDoCombate& regras)
{
    const Combatente jogador = combatenteDoKit(regras, "jogador");
    const Combatente bandido = combatenteDoKit(regras, "bandido");
    const Combatente guarda = combatenteDoKit(regras, "guarda");
    const Combatente civil = combatenteDoKit(regras, "civil");
    const int defesaDoGuarda = defesaDaArmaduraEmCentesimos(regras, guarda);
    // Teste 1: a tabela da parte 5 (em centésimos).
    conferir(defesaDoGuarda == 500 && classeDePesoDaArmadura(guarda.armadura) == "media" && classeDePesoDaArmadura({}) == "leve", "o guarda: Defesa 5, armadura Média");
    conferir(danoMedioEmCentesimos(regras, jogador, "leve", 0) == 1250 && danoMedioEmCentesimos(regras, jogador, "media", defesaDoGuarda) == 500,
             "o jogador com a Espada: 12,5 contra sem armadura; 5 contra o guarda");
    conferir(danoMedioEmCentesimos(regras, bandido, "leve", 0) == 840 && danoMedioEmCentesimos(regras, bandido, "media", defesaDoGuarda) == 620,
             "o bandido com a Clava: 8,4 e 6,2");
    conferir(danoMedioEmCentesimos(regras, guarda, "leve", 0) == 1125 && danoMedioEmCentesimos(regras, guarda, "media", defesaDoGuarda) == 400,
             "o guarda com a Lança: 11,25 e 4");
    // O soco não tem classe no combate.json: sem a vantagem, 1,8 (a tabela da parte 5 dizia 2,25).
    conferir(danoMedioEmCentesimos(regras, civil, "leve", 0) == 180 && danoMedioEmCentesimos(regras, civil, "media", defesaDoGuarda) == 100,
             "o civil com o soco: 1,8 e 1 (o mínimo)");
    // A parte 10.2: o golpe sorteado.
    conferir(calcularDanoDoGolpe(regras, jogador, bandido, 10) == 12 && calcularDanoDoGolpe(regras, bandido, jogador, 14) == 5,
             "os golpes da parte 10.2: 10 × 1,25 = 12; 14 × 0,8 × 0,75 − 3 = 5");
    // Teste 5: o civil contra o guarda tira 1.
    conferir(calcularDanoDoGolpe(regras, civil, guarda, 4) == 1, "o mínimo de 1: o civil contra o guarda");
    // Teste 6: 12 acertos com habilidade 20 dão cerca de +1 ponto.
    int habilidade = 2000;
    for (int golpe = 0; golpe < 12; ++golpe) {
        habilidade = evoluirHabilidade(regras, habilidade);
    }
    conferir(habilidade >= 2090 && habilidade <= 2100, "a evolução: 12 acertos com habilidade 20 dão cerca de +1");
    // Teste 7: 3 testemunhas à noite: 1 − 0,3³ = 97,3%.
    conferir(chanceDeSerVistoPorMil(3, true) == 973 && chanceDeSerVistoPorMil(1, false) == 1000 && chanceDeSerVistoPorMil(0, false) == 0,
             "a testemunha à noite: 3 veem com 97,3%");
    conferir(chanceDeAcertoADistancia(regras, 50, 3, false, false) == 90 && chanceDeAcertoADistancia(regras, 70, 9, true, false) == 60
                 && chanceDeAcertoADistancia(regras, 10, 20, false, true) == 10,
             "o acerto a distância: 90% a 3 tiles; com a chuva e 9 tiles, 60%; nunca menos de 10%");
    conferir(calcularMedo(regras, 11, 0, true) == 64, "o Medo da parte 10.2: Vida 11 e o inimigo 2 vezes mais forte dão 64");
}

void testarOCalculoRapido(const RegrasDoCombate& regras)
{
    // Teste 3 (parte 10.3): o duelo pelo cálculo rápido: cerca de 78% e 29% de Vida.
    const std::vector<Combatente> jogador = ladoDoKit(regras, "jogador", 1);
    const std::vector<Combatente> bandido = ladoDoKit(regras, "bandido", 1);
    const ResultadoDoCalculoRapido duelo = resolverPorCalculoRapido(regras, jogador, bandido, 0);
    conferir(poderDoLado(regras, jogador, bandido) == 62500 && poderDoLado(regras, bandido, jogador) == 18000 && duelo.chanceDoLadoAEmPorMil == 776
                 && duelo.perdaDeVidaDoVencedorEmPorcento == 28,
             "o cálculo rápido do duelo: 77,6% e 28% de Vida perdida");
    // Parte 10.4: sozinho contra 6 bandidos, 8,8%.
    conferir(resolverPorCalculoRapido(regras, jogador, ladoDoKit(regras, "bandido", 6), 0).chanceDoLadoAEmPorMil == 87, "o jogador sozinho contra 6 bandidos: 8,8%");
    // Parte 10.5: a caravana (2 guardas e 2 civis) contra 6 bandidos: cerca de 34%.
    std::vector<Combatente> caravana = ladoDoKit(regras, "guarda", 2);
    const std::vector<Combatente> civis = ladoDoKit(regras, "civil", 2);
    caravana.insert(caravana.end(), civis.begin(), civis.end());
    const int chanceDaCaravana = resolverPorCalculoRapido(regras, caravana, ladoDoKit(regras, "bandido", 6), 0).chanceDoLadoAEmPorMil;
    std::printf("    a caravana contra 6 bandidos: %d por mil\n", chanceDaCaravana);
    conferir(chanceDaCaravana >= 320 && chanceDaCaravana <= 360, "a caravana contra 6 bandidos: cerca de 34%");
}

void testarOCombateVisto(const RegrasDoCombate& regras)
{
    // Teste 2 (parte 10.2): o duelo visto; o jogador vence com cerca de 73 de Vida.
    SorteioComSemente sorteio(7, 1);
    const auto sortear = [&](int minimo, int maximo) { return sorteio.sortearNumeroInteiroEntre(minimo, maximo); };
    const ResultadoDoDuelo duelo = simularOCombate(regras, ladoDoKit(regras, "jogador", 1), ladoDoKit(regras, "bandido", 1), true, sortear);
    std::printf("    o duelo visto: %d golpes do jogador em %d ticks; Vida %d\n", duelo.golpesDoLadoA, duelo.ticks, duelo.vidaDoLadoA.front());
    conferir(duelo.venceuOLadoA && duelo.golpesDoLadoA >= 5 && duelo.golpesDoLadoA <= 10 && duelo.vidaDoLadoA.front() >= 55, "o duelo visto: o jogador vence com cerca de 73 de Vida");
    // Teste 4: a coerência com os lados iguais (2 guardas contra 2 guardas, sem o Medo).
    int vitorias = 0;
    for (int combate = 0; combate < 1000; ++combate) {
        vitorias += simularOCombate(regras, ladoDoKit(regras, "guarda", 2), ladoDoKit(regras, "guarda", 2), false, sortear).venceuOLadoA ? 1 : 0;
    }
    const int rapido = resolverPorCalculoRapido(regras, ladoDoKit(regras, "guarda", 2), ladoDoKit(regras, "guarda", 2), 0).chanceDoLadoAEmPorMil;
    std::printf("    2 guardas contra 2 guardas: visto %d por mil, rápido %d por mil\n", vitorias, rapido);
    conferir(std::abs(vitorias - rapido) <= 100, "a coerência com os lados iguais: o visto e o rápido diferem em até 10 pontos");
    // Achado (rodada CB2): com os lados diferentes, o rápido pesa demais o grupo maior.
    int vitoriasContraDois = 0;
    for (int combate = 0; combate < 1000; ++combate) {
        vitoriasContraDois += simularOCombate(regras, ladoDoKit(regras, "jogador", 1), ladoDoKit(regras, "bandido", 2), false, sortear).venceuOLadoA ? 1 : 0;
    }
    std::printf("    achado: 1 jogador contra 2 bandidos: visto %d por mil, rápido %d por mil\n", vitoriasContraDois,
                resolverPorCalculoRapido(regras, ladoDoKit(regras, "jogador", 1), ladoDoKit(regras, "bandido", 2), 0).chanceDoLadoAEmPorMil);
}

}  // namespace

void testarCombate()
{
    std::printf("Combate: a fórmula, o Medo, a evolução e o cálculo rápido (fase D1)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com o combate.json, não têm erro");
    testarAsContas(dados.regrasDoCombate);
    testarOCalculoRapido(dados.regrasDoCombate);
    testarOCombateVisto(dados.regrasDoCombate);
}
