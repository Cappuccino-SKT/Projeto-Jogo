#include "Producao/ConsumoMedidoDaVila.h"

ConsumoMedidoDaVila::ConsumoMedidoDaVila(int quantidadeDeItens)
    : produtoEstaComEstoqueDeSobra(quantidadeDeItens, false), consumoDosUltimosDiasPorItem(quantidadeDeItens),
      consumoDeHojePorItem(quantidadeDeItens, 0)
{
    for (std::array<int64_t, diasGuardadosDoConsumo>& dias : consumoDosUltimosDiasPorItem) {
        dias.fill(0);
    }
}

void ConsumoMedidoDaVila::preencherOsDiasGuardados(const std::vector<int64_t>& consumoDeUmDiaPorItemEmCentesimos)
{
    for (std::size_t item = 0; item < consumoDosUltimosDiasPorItem.size() && item < consumoDeUmDiaPorItemEmCentesimos.size(); ++item) {
        consumoDosUltimosDiasPorItem[item].fill(consumoDeUmDiaPorItemEmCentesimos[item]);
    }
}

void ConsumoMedidoDaVila::anotarOQueSaiu(int numeroDoItem, int quantidade)
{
    if (numeroDoItemExiste(numeroDoItem)) {
        consumoDeHojePorItem[numeroDoItem] += static_cast<int64_t>(quantidade) * 100;
    }
}

void ConsumoMedidoDaVila::anotarADemandaQueFaltou(int numeroDoItem, int64_t centesimos)
{
    if (numeroDoItemExiste(numeroDoItem)) {
        consumoDeHojePorItem[numeroDoItem] += centesimos;
    }
}

void ConsumoMedidoDaVila::fecharODia()
{
    for (std::size_t item = 0; item < consumoDeHojePorItem.size(); ++item) {
        consumoDosUltimosDiasPorItem[item][posicaoDoDiaMaisAntigo] = consumoDeHojePorItem[item];
        consumoDeHojePorItem[item] = 0;
    }
    posicaoDoDiaMaisAntigo = (posicaoDoDiaMaisAntigo + 1) % diasGuardadosDoConsumo;
}

int64_t ConsumoMedidoDaVila::consumoPrevistoDoDiaEmCentesimos(int numeroDoItem) const
{
    if (!numeroDoItemExiste(numeroDoItem)) {
        return 0;
    }
    int64_t soma = 0;
    for (const int64_t consumoDoDia : consumoDosUltimosDiasPorItem[numeroDoItem]) {
        soma += consumoDoDia;
    }
    return soma / diasGuardadosDoConsumo;
}

int64_t ConsumoMedidoDaVila::consumoDeHojeEmCentesimos(int numeroDoItem) const
{
    return numeroDoItemExiste(numeroDoItem) ? consumoDeHojePorItem[numeroDoItem] : 0;
}
