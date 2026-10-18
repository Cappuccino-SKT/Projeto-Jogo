#pragma once

// A vila inteira andando no tempo (marco 6): o relógio, os NPCs (a rotina e o
// movimento), o inventário, a produção e o dinheiro, juntos. O laço do jogo
// e o teste sem tela chamam a mesma avancarUmTick(), então os dois rodam
// exatamente a mesma partida.

#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include "Dados/DadosDoJogo.h"
#include "Economia/DinheiroDaVila.h"
#include "Inventario/InventarioDaCidade.h"
#include "Mapa/BuscaDeCaminho.h"
#include "Npc/MovimentoDosNpcs.h"
#include "Npc/RotinaDosNpcs.h"
#include "Producao/ProducaoDaVila.h"
#include "Tempo/RelogioDoMundo.h"
#include "Tempo/SorteioComSemente.h"

class SimulacaoDaVila {
public:
    // Monta a partida nova (6h do dia 1 da Primavera): os 15 NPCs nas casas,
    // o estoque e o dinheiro do primeiro dia. A distância do ciclo de andar
    // vem do boneco (no Jogo/); o teste sem tela usa o padrão.
    SimulacaoDaVila(const DadosDoJogo& dadosDoJogo, const RegrasDoTempo& regrasDoTempo, int distanciaDoCicloDeAndarEmMilesimos = 2400);

    // Uma cópia levaria junto as referências da vila; não faz sentido.
    SimulacaoDaVila(const SimulacaoDaVila&) = delete;
    SimulacaoDaVila& operator=(const SimulacaoDaVila&) = delete;

    // Avança 1 tick, na ordem da ficha da Produção (parte 3):
    //   1. o relógio;
    //   2. na virada da hora, o ciclo de cada construção;
    //   3. na virada do dia, a ordem do dia (consumo, salários, escolha das receitas);
    //   4. a rotina, os caminhos, o movimento e as chegadas (marcos 4 e 5);
    //   5. quem está trabalhando soma 1 tick.
    // Devolve as linhas para o console (os avisos e o resumo do dia, FL6-8).
    std::vector<std::string> avancarUmTick();

    ContextoDaRotina contextoDaRotina();
    ContextoDaProducao contextoDaProducao();

    const DadosDoJogo& dados;
    RelogioDoMundo relogio;
    entt::registry registroDasEntidades;
    RegrasDoMovimento regrasDoMovimento;
    InventarioDaCidade inventario;
    DinheiroDaVila dinheiro;
    std::vector<ProducaoDaConstrucao> producaoDasConstrucoes;
    std::vector<entt::entity> npcsDaVila;  // na ordem da vila inicial
    std::vector<std::string> avisosAoMontar;

private:
    int ticksPorHoraDeJogo = 800;
    BuscadorDeCaminho buscadorDeCaminho;
    SorteioComSemente sorteioDosNpcs;
};
