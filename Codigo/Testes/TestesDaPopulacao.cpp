// Os testes da população: a tabela da parte 8 da Ficha Lógica: População.

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Npc/ComponentesDoNpc.h"
#include "Vila/Populacao.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";
constexpr int64_t ticksDeUmDia = 24 * 800;

// Avança até a meia-noite de um dia da partida (contando do 0).
void avancarAteODia(SimulacaoDaVila& simulacao, int64_t diaDaPartida)
{
    while (simulacao.relogio.diaDaPartida() < diaDaPartida) {
        simulacao.avancarUmTick();
    }
}

void testarAsContas(const RegrasDaPopulacao& regras)
{
    conferir(calcularCrescimentoDaEstacao(15, 580, false, regras) == 54, "15 NPCs, Felicidade 58, sem fome: + 54 (69 − 15)");
    conferir(calcularCrescimentoDaEstacao(15, 580, true, regras) == 34 - 15, "com fome na estação: os nascimentos pela metade");
    // Rodada HI12: velho aos 31.
    conferir(chanceDeMorrerDeVelhice(30, false, regras) == 0, "30 anos: não morre de velhice");
    conferir(chanceDeMorrerDeVelhice(31, false, regras) == 10, "31 anos: 10% por estação");
    conferir(chanceDeMorrerDeVelhice(33, false, regras) == 30, "33 anos: 30%");
    conferir(chanceDeMorrerDeVelhice(40, false, regras) >= 100, "40 anos: morre com qualquer sorteio");
    conferir(chanceDeMorrerDeVelhice(31, true, regras) == 15, "31 anos no Inverno rigoroso: 15%");
}

void testarAVila(const DadosDoJogo& dados)
{
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    ContextoDaProducao contexto = simulacao.contextoDaProducao();
    conferir(simulacao.npcsDaVila.size() == 15 && capacidadeDaVila(contexto) == 15, "a vila começa com 15 NPCs, capacidade 15");
    const int idadeDoPrimeiro = simulacao.registroDasEntidades.get<IdadeDoNpc>(simulacao.npcsDaVila.front()).anos;
    const int idadeDoUltimo = simulacao.registroDasEntidades.get<IdadeDoNpc>(simulacao.npcsDaVila.back()).anos;
    conferir(idadeDoPrimeiro == 5 && idadeDoUltimo == 19, "as idades da vila inicial vão de 5 a 19");

    // A vila cheia não cria ninguém: sem cama não há NPC novo (16 camas, 15 moradores: 1 cabe).
    const entt::entity novo = criarNpcNaVila(contexto, 4, 50, 50);
    conferir(novo != entt::null && simulacao.npcsDaVila.size() == 16 && simulacao.registroDasEntidades.get<IdentidadeDoNpc>(novo).numeroDoNpc == 15,
             "um NPC novo na cama livre: o número 15, desempregado");
    conferir(criarNpcNaVila(contexto, 4, 50, 50) == entt::null, "sem cama livre, nenhum NPC novo");

    // O NPC 7 sai: os outros continuam com os mesmos números; o próximo novo é o 16.
    entt::entity setimo = entt::null;
    for (const entt::entity npc : simulacao.npcsDaVila) {
        if (simulacao.registroDasEntidades.get<IdentidadeDoNpc>(npc).numeroDoNpc == 7) {
            setimo = npc;
        }
    }
    tirarNpcDaVila(contexto, setimo);
    const entt::entity outroNovo = criarNpcNaVila(contexto, 4, 50, 50);
    conferir(simulacao.npcsDaVila.size() == 16 && outroNovo != entt::null && simulacao.registroDasEntidades.get<IdentidadeDoNpc>(outroNovo).numeroDoNpc == 16,
             "o NPC 7 sai e o próximo novo é o 16 (os números nunca voltam)");
    conferir(simulacao.dinheiro.quantidadeDeCivis == 16, "a quantidade de Civis acompanha quem chega e quem sai");
}

void testarAFomeEAVelhice(const DadosDoJogo& dados)
{
    // O acumulado chega a −100: morre o mais velho (Pedro, 19) e o acumulado volta a 0.
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    simulacao.populacao.acumuladoDeCrescimentoEmPessoas = -95;
    simulacao.dinheiro.diasEmDeficitDeComida = 4;  // o 4º dia de fome: 2% de quem ficou sem comida
    avancarAteODia(simulacao, 1);  // a primeira meia-noite roda a ordem do dia
    conferir(simulacao.npcsDaVila.size() <= 15, "a fome não cria ninguém");

    // Um NPC de 31 anos no dia 1 de uma estação: morre com o sorteio baixo, e um jovem ocupa a vaga.
    SimulacaoDaVila velhos(dados, RegrasDoTempo{});
    for (const entt::entity npc : velhos.npcsDaVila) {
        velhos.registroDasEntidades.get<IdadeDoNpc>(npc).anos = 40;  // 100%: todos morrem
    }
    avancarAteODia(velhos, 16);  // o dia 1 do Verão
    int jovens = 0;
    for (const entt::entity npc : velhos.npcsDaVila) {
        jovens += velhos.registroDasEntidades.get<IdadeDoNpc>(npc).anos == 5 ? 1 : 0;
    }
    conferir(velhos.npcsDaVila.size() == 15 && jovens == 15, "15 NPCs de 40 anos no dia 1 do Verão: todos morrem e 15 jovens de 5 anos ocupam as vagas");
}

}  // namespace

void testarPopulacao()
{
    std::printf("População (fase A3)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    testarAsContas(dados.regras.populacao);
    testarAVila(dados);
    testarAFomeEAVelhice(dados);
}
