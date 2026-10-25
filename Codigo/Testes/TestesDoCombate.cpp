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
    // Rodada CB2 (opção C): o cálculo rápido roda o combate simulado, então bate com o combate visto.
    const std::vector<Combatente> jogador = ladoDoKit(regras, "jogador", 1);
    // O poder do lado (4.8, passos 1 e 2) segue para o Medo e para a decisão dos bandidos.
    conferir(poderDoLado(regras, jogador, ladoDoKit(regras, "bandido", 1)) == 62500 && poderDoLado(regras, ladoDoKit(regras, "bandido", 1), jogador) == 18000,
             "o poder do duelo: 62.500 e 18.000");
    const ResultadoDoCalculoRapido duelo = resolverPorCalculoRapido(regras, jogador, ladoDoKit(regras, "bandido", 1), 0);
    std::printf("    o duelo rápido: venceu %d, perdeu %d%% de Vida\n", duelo.venceuOLadoA ? 1 : 0, duelo.perdaDeVidaDoVencedorEmPorcento);
    conferir(duelo.venceuOLadoA && duelo.perdaDeVidaDoVencedorEmPorcento >= 10 && duelo.perdaDeVidaDoVencedorEmPorcento <= 45,
             "o duelo rápido: o jogador vence e perde cerca de 27% de Vida, como no visto");
    conferir(resolverPorCalculoRapido(regras, jogador, ladoDoKit(regras, "bandido", 1), 0).perdaDeVidaDoVencedorEmPorcento == duelo.perdaDeVidaDoVencedorEmPorcento,
             "a mesma semente dá a mesma luta");
    const int contraUm = chanceDeVencerEmPorMil(regras, jogador, ladoDoKit(regras, "bandido", 1));
    const int contraDois = chanceDeVencerEmPorMil(regras, jogador, ladoDoKit(regras, "bandido", 2));
    const int contraSeis = chanceDeVencerEmPorMil(regras, jogador, ladoDoKit(regras, "bandido", 6));
    const int iguais = chanceDeVencerEmPorMil(regras, ladoDoKit(regras, "guarda", 2), ladoDoKit(regras, "guarda", 2), 400);
    std::vector<Combatente> caravana = ladoDoKit(regras, "guarda", 2);
    const std::vector<Combatente> civis = ladoDoKit(regras, "civil", 2);
    caravana.insert(caravana.end(), civis.begin(), civis.end());
    const int daCaravana = chanceDeVencerEmPorMil(regras, caravana, ladoDoKit(regras, "bandido", 6));
    std::printf("    por mil: contra 1 bandido %d, contra 2 %d, contra 6 %d; 2 guardas contra 2 guardas %d; a caravana contra 6 bandidos %d\n", contraUm, contraDois,
                contraSeis, iguais, daCaravana);
    conferir(contraUm >= 950, "o jogador contra 1 bandido: quase sempre vence");
    conferir(contraDois >= 850, "o jogador contra 2 bandidos: perto dos 97% do combate visto (o rápido antigo dava 46%)");
    conferir(contraSeis < contraDois, "contra 6 bandidos, a chance cai");
    conferir(iguais >= 380 && iguais <= 620, "os lados iguais: cerca de metade");
}

void testarOCombateVisto(const RegrasDoCombate& regras)
{
    // Teste 2 (parte 10.2): o duelo visto; o jogador vence com cerca de 73 de Vida.
    SorteioComSemente sorteio(7, 1);
    const auto sortear = [&](int minimo, int maximo) { return sorteio.sortearNumeroInteiroEntre(minimo, maximo); };
    const ResultadoDoDuelo duelo = simularOCombate(regras, ladoDoKit(regras, "jogador", 1), ladoDoKit(regras, "bandido", 1), true, sortear);
    std::printf("    o duelo visto: %d golpes do jogador em %d ticks; Vida %d\n", duelo.golpesDoLadoA, duelo.ticks, duelo.vidaDoLadoA.front());
    conferir(duelo.venceuOLadoA && duelo.golpesDoLadoA >= 5 && duelo.golpesDoLadoA <= 10 && duelo.vidaDoLadoA.front() >= 55, "o duelo visto: o jogador vence com cerca de 73 de Vida");
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
