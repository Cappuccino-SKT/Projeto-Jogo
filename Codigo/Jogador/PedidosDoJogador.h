#pragma once

// Os pedidos do jogador (Ficha Lógica: Interface, regra 10.8, marco 8). A
// interface só desenha e lê os cliques: cada decisão vira um pedido, guardado
// numa fila e aplicado no começo do próximo tick (ou na hora, com o jogo
// pausado). Assim a simulação continua determinística, e os testes fazem os
// mesmos pedidos sem janela.

#include <string>
#include <variant>
#include <vector>

#include "Producao/OrdemDoJogador.h"
#include "Producao/ProducaoDaVila.h"
#include "Viagem/Viagem.h"

struct PedidoTrocarAFilaDeOrdens {
    int indiceDaConstrucao = 0;
    std::vector<OrdemDoJogador> ordens;
};
struct PedidoFixarOPreco {
    int numeroDoItem = 0;
    int porcentagem = 0;
};
struct PedidoTrocarONivelDeSalario {
    int indiceDaConstrucao = 0;
    NivelDeSalario nivel = NivelDeSalario::Normal;
};
struct PedidoContratar {
    int indiceDaConstrucao = 0;
    int numeroDoNpc = 0;
};
struct PedidoDemitir {
    int numeroDoNpc = 0;
};
struct PedidoEncomendarConstrucaoNova {
    int numeroDoLote = 0;
    std::string tipoDeConstrucao;
};
struct PedidoSubirDeNivel {
    int indiceDaConstrucao = 0;
};
struct PedidoComprarDaLoja {
    int numeroDoItem = 0;
    int quantidade = 0;
};
struct PedidoVenderParaALoja {
    int numeroDoItem = 0;
    int quantidade = 0;
};

// Fase A2 (ficha do Trabalho e Propriedade, regras 4.3 e 4.4).
struct PedidoComprarConstrucao {
    int indiceDaConstrucao = 0;
};
struct PedidoVenderConstrucao {
    int indiceDaConstrucao = 0;
};
struct PedidoTrocarOModoAutomatico {
    int indiceDaConstrucao = 0;
    bool ligado = true;
};
struct PedidoDarLance {
    int64_t lanceEmDecimos = 0;
};

// Fase B1 (ficha da Viagem, regras 4.1 e 4.6).
struct PedidoPartirEmViagem {
    int destino = 0;  // o número do mapa
    int paesLevados = 0;
    int64_t dinheiroLevadoEmDecimos = 0;
    bool marchaForcada = false;
};
struct PedidoResolverEncontro {
    EscolhaNoEncontro escolha = EscolhaNoEncontro::Render;
};
// Fase B2 (Comércio, 4.7): comprar ou vender na loja da cidade onde o grupo está, com o que ele leva.
struct PedidoComercioNaCidade {
    int numeroDoItem = 0;
    int quantidade = 0;
    bool comprar = true;
};
// Fase C3 (Crime, 4.4 e 4.6): a escolha na revista e a fiança.
struct PedidoResolverRevista {
    int escolha = 0;  // 0 pagar, 1 subornar, 2 fugir (EscolhaNaRevista)
    int64_t ofertaEmDecimos = 0;
};
struct PedidoPagarFianca {};
// Fase C4 (Religião, 4.9): a oferenda no Templo da Capital Religiosa.
struct PedidoPeregrinacao {
    int moedas = 0;
};
// Fase B4 (Reputação, 4.2): doar moedas na cidade onde o grupo está.
struct PedidoDoarNaCidade {
    int moedas = 0;
};

using PedidoDoJogador = std::variant<PedidoTrocarAFilaDeOrdens, PedidoFixarOPreco, PedidoTrocarONivelDeSalario, PedidoContratar, PedidoDemitir,
                                     PedidoEncomendarConstrucaoNova, PedidoSubirDeNivel, PedidoComprarDaLoja, PedidoVenderParaALoja,
                                     PedidoComprarConstrucao, PedidoVenderConstrucao, PedidoTrocarOModoAutomatico, PedidoDarLance,
                                     PedidoPartirEmViagem, PedidoResolverEncontro, PedidoComercioNaCidade, PedidoDoarNaCidade, PedidoResolverRevista, PedidoPagarFianca, PedidoPeregrinacao>;

class FilaDePedidosDoJogador {
public:
    void acrescentar(PedidoDoJogador pedido) { pedidos.push_back(std::move(pedido)); }
    bool estaVazia() const { return pedidos.empty(); }
    // Entrega todos, na ordem em que chegaram, e esvazia a fila.
    std::vector<PedidoDoJogador> tomarTodos();

private:
    std::vector<PedidoDoJogador> pedidos;
};

// Regra 10.8: confere cada pedido de novo e o aplica. Devolve as linhas do
// console: "Aviso: Pedido recusado (...)" quando não vale mais, e uma linha
// "Obra encomendada: ..." quando uma obra entra na fila.
std::vector<std::string> aplicarOsPedidosDoJogador(ContextoDaProducao& contexto, FilaDePedidosDoJogador& fila);
