#pragma once

// A vila inteira andando no tempo (marcos 6 e 7): o relógio, os NPCs (a
// rotina e o movimento), o inventário, a produção, o dinheiro, o Mercado
// Local, a Felicidade e os eventos, juntos. O laço do jogo e o teste sem tela
// chamam a mesma avancarUmTick(), então os dois rodam exatamente a mesma partida.

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include "Dados/DadosDoJogo.h"
#include "Economia/DinheiroDaVila.h"
#include "Economia/FelicidadeDaClasse.h"
#include "Economia/MercadoLocal.h"
#include "Eventos/EventosDaVila.h"
#include "Inventario/InventarioDaCidade.h"
#include "Jogador/NotificacoesEAlertas.h"
#include "Jogador/PedidosDoJogador.h"
#include "Mapa/BuscaDeCaminho.h"
#include "Npc/MovimentoDosNpcs.h"
#include "Npc/RotinaDosNpcs.h"
#include "Producao/ConsumoMedidoDaVila.h"
#include "Producao/ProducaoDaVila.h"
#include "Simulacao/RelatorioDaSimulacao.h"
#include "Tempo/RelogioDoMundo.h"
#include "Tempo/SorteioComSemente.h"
#include "Clima/ClimaDaVila.h"
#include "Viagem/ViagemDaVila.h"

// Os eventos sorteiam com a sequência 2 (ficha dos Eventos): um NPC a mais não muda os eventos.
constexpr uint64_t sequenciaDoSorteioDosEventos = 2;

class SimulacaoDaVila {
public:
    // Monta a partida nova (6h do dia 1 da Primavera): os 15 NPCs nas casas,
    // o estoque e o dinheiro do primeiro dia. A distância do ciclo de andar
    // vem do boneco (no Jogo/); o teste sem tela usa o padrão. A semente pode
    // trocar a do vila_inicial.json (jogo --simular 64 --semente 1).
    SimulacaoDaVila(const DadosDoJogo& dadosDoJogo, const RegrasDoTempo& regrasDoTempo, int distanciaDoCicloDeAndarEmMilesimos = 2400,
                    std::optional<uint64_t> sementeNoLugarDaDoArquivo = std::nullopt);

    // Uma cópia levaria junto as referências da vila; não faz sentido.
    SimulacaoDaVila(const SimulacaoDaVila&) = delete;
    SimulacaoDaVila& operator=(const SimulacaoDaVila&) = delete;

    // Avança 1 tick, na ordem da ficha da Produção (parte 3):
    //   1. o relógio;
    //   2. na virada da hora, o ciclo de cada construção;
    //   3. na virada do dia, a ordem do dia (consumo, salários, Mercado Local,
    //      estabilizador, Felicidade, eventos, escolha das receitas), o resumo
    //      e a linha do relatório;
    //   4. a rotina, os caminhos, o movimento e as chegadas (marcos 4 e 5);
    //   5. quem está trabalhando soma 1 tick.
    // Devolve as linhas para o console (os avisos, os eventos e o resumo do dia).
    std::vector<std::string> avancarUmTick();

    // Marco 8 (Interface, regra 10.8): aplica os pedidos do jogador que estão
    // na fila. O avancarUmTick() faz isso no começo; o laço do jogo também
    // chama com o jogo pausado, para o jogador ver o efeito na hora.
    // Devolve as linhas do console (já viraram notificações).
    std::vector<std::string> aplicarOsPedidosPendentes();

    // O teste de desempenho (ficha da Simulação, regra 4.4): Civis a mais,
    // sem casa nem emprego, que só passeiam. Eles não entram na economia.
    void criarNpcsSoParaMedirODesempenho(int quantidade);

    ContextoDaRotina contextoDaRotina();
    ContextoDaProducao contextoDaProducao();

    const DadosDoJogo& dados;
    RelogioDoMundo relogio;
    // Marco 8: a vila e o mapa são cópias, porque as obras acrescentam construções
    // e bloqueiam lotes. O Jogo/ desenha a partir daqui, não dos dados.
    VilaDoJogo vila;
    MapaLocal mapa;
    entt::registry registroDasEntidades;
    RegrasDoMovimento regrasDoMovimento;
    InventarioDaCidade inventario;
    DinheiroDaVila dinheiro;
    std::vector<ProducaoDaConstrucao> producaoDasConstrucoes;
    ConsumoMedidoDaVila consumoMedido;
    MercadoLocalDaVila mercadoLocal;
    FelicidadeDaClasse felicidade;
    EstadoDosEventos eventos;
    EstadoDoClima clima;  // fase A1 (ficha do Clima)
    EstadoDoTrabalho trabalho;  // fase A2 (ficha do Trabalho): o leilão
    EstadoDaPopulacao populacao;  // fase A3 (ficha da População)
    EstadoDaViagem viagem;  // fase B1 (ficha da Viagem)
    DecisoesDoJogador decisoes;
    ObrasDaVila obras;
    FilaDePedidosDoJogador pedidos;
    NotificacoesDaVila notificacoes;
    std::vector<entt::entity> npcsDaVila;  // na ordem da vila inicial
    std::vector<std::string> avisosAoMontar;
    // A linha do relatório do último dia que acabou (montada na meia-noite,
    // antes de as contas do dia voltarem a zero). Vazia antes da primeira.
    std::optional<LinhaDoRelatorio> linhaDoRelatorioDoUltimoDia;

private:
    friend struct AcessoAoEstadoParaOSave;  // o save grava e restaura os sorteios (marco 9)
    // Cada linha do console que merece vira notificação (e entra no Diário).
    void registrarNasNotificacoes(const std::vector<std::string>& linhas, std::size_t aPartirDaLinha);
    int ticksPorHoraDeJogo = 800;
    BuscadorDeCaminho buscadorDeCaminho;
    SorteioComSemente sorteioDosNpcs;
    SorteioComSemente sorteioDosEventos;
    SorteioComSemente sorteioDoClima;  // a sequência 3
    SorteioComSemente sorteioDaPopulacao;  // a sequência 4
    SorteioComSemente sorteioDaViagem;  // a sequência 5
    SorteioComSemente sorteioDoClimaDasOutrasRegioes;  // a sequência 9
};
