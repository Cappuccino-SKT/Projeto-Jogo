#pragma once

// A única porta por onde o save mexe nos campos privados das classes da
// simulação (marco 9). Cada uma delas declara esta struct como "friend". Assim
// o resto do código continua sem poder mexer neles, e os campos que o save
// precisa não viram públicos.
//
// Os métodos aceitam o objeto com ou sem "const" (o gravar lê, o carregar escreve).

#include "Inventario/InventarioDaCidade.h"
#include "Producao/ConsumoMedidoDaVila.h"
#include "Tempo/RelogioDoMundo.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/SimulacaoDaVila.h"

struct AcessoAoEstadoParaOSave {
    // O sorteio.
    template <class Sorteio>
    static auto& estadoDoGerador(Sorteio& sorteio) { return sorteio.estadoDoGerador; }
    template <class Sorteio>
    static auto& incrementoDaSequencia(Sorteio& sorteio) { return sorteio.incrementoDaSequencia; }

    // O relógio.
    template <class Relogio>
    static auto& tickAtualDaPartida(Relogio& relogio) { return relogio.tickAtualDaPartida; }
    template <class Relogio>
    static auto& velocidadeDoJogo(Relogio& relogio) { return relogio.velocidadeDoJogo; }
    template <class Relogio>
    static auto& velocidadeAntesDaPausa(Relogio& relogio) { return relogio.velocidadeAntesDaPausa; }

    // O inventário.
    template <class Inventario>
    static auto& quantidadePorItemDaCidade(Inventario& inventario) { return inventario.quantidadePorItemDaCidade; }
    template <class Inventario>
    static auto& quantidadePorItemDoJogador(Inventario& inventario) { return inventario.quantidadePorItemDoJogador; }

    // O consumo medido.
    template <class Consumo>
    static auto& consumoDosUltimosDiasPorItem(Consumo& consumo) { return consumo.consumoDosUltimosDiasPorItem; }
    template <class Consumo>
    static auto& consumoDeHojePorItem(Consumo& consumo) { return consumo.consumoDeHojePorItem; }
    template <class Consumo>
    static auto& posicaoDoDiaMaisAntigo(Consumo& consumo) { return consumo.posicaoDoDiaMaisAntigo; }

    // Os dois sorteios da simulação.
    template <class Simulacao>
    static auto& sorteioDosNpcs(Simulacao& simulacao) { return simulacao.sorteioDosNpcs; }
    template <class Simulacao>
    static auto& sorteioDosEventos(Simulacao& simulacao) { return simulacao.sorteioDosEventos; }
    template <class Simulacao>
    static auto& sorteioDoClima(Simulacao& simulacao) { return simulacao.sorteioDoClima; }
    template <class Simulacao>
    static auto& sorteioDaPopulacao(Simulacao& simulacao) { return simulacao.sorteioDaPopulacao; }
};
