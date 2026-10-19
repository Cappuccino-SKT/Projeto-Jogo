#pragma once

// A previsão do consumo, medida (Ficha Lógica: Produção, regra 10.1, FL7-4):
// o consumo previsto de cada item é a média dos 5 últimos dias do que a vila
// gastou ou quis gastar. A escolha das receitas (o estoque-alvo) e o Mercado
// Local usam a mesma conta.
//
// Tudo em centésimos de unidade: 1,875 Móveis por dia = 187.

#include <array>
#include <cstdint>
#include <vector>

constexpr int diasGuardadosDoConsumo = 5;

class ConsumoMedidoDaVila {
public:
    ConsumoMedidoDaVila() = default;
    explicit ConsumoMedidoDaVila(int quantidadeDeItens);

    // Os 5 dias guardados ficam todos iguais a este consumo (o primeiro dia
    // usa a previsão do marco 6, pelas receitas atuais).
    void preencherOsDiasGuardados(const std::vector<int64_t>& consumoDeUmDiaPorItemEmCentesimos);

    // Passo 1 (gastou): o que saiu do inventário para a cesta, o gasto extra e os insumos.
    void anotarOQueSaiu(int numeroDoItem, int quantidade);
    // Passo 2 (quis gastar e faltou), já em centésimos.
    void anotarADemandaQueFaltou(int numeroDoItem, int64_t centesimos);

    // Fecha o dia: o de hoje entra no lugar do mais antigo, e o de hoje volta a 0.
    void fecharODia();

    // A média dos 5 dias guardados, em centésimos por dia.
    int64_t consumoPrevistoDoDiaEmCentesimos(int numeroDoItem) const;
    // O que já foi anotado hoje (para o relatório e os testes).
    int64_t consumoDeHojeEmCentesimos(int numeroDoItem) const;

    // A histerese da folga (regra 10.2): o produto passou de 125% do alvo e
    // ainda não voltou para baixo do alvo.
    std::vector<bool> produtoEstaComEstoqueDeSobra;

private:
    friend struct AcessoAoEstadoParaOSave;  // o save grava e restaura o estado (marco 9)
    std::vector<std::array<int64_t, diasGuardadosDoConsumo>> consumoDosUltimosDiasPorItem;
    std::vector<int64_t> consumoDeHojePorItem;
    int posicaoDoDiaMaisAntigo = 0;  // os 5 dias são uma roda: o mais antigo sai primeiro

    bool numeroDoItemExiste(int numeroDoItem) const { return numeroDoItem >= 0 && numeroDoItem < static_cast<int>(consumoDeHojePorItem.size()); }
};
