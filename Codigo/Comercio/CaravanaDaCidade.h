#pragma once

// Uma caravana de uma cidade (Ficha Lógica: Comércio, parte 2): uma entidade em trânsito
// da Viagem (tipo "caravana") e o que ela leva, para onde e desde quando.

#include <cstdint>
#include <vector>

#include "Viagem/Viagem.h"

enum class SituacaoDaCaravana { EmCasa, Indo, Voltando };

struct CaravanaDaCidade {
    EntidadeEmTransito entidade;  // a carga é o inventário dela; o dinheiro, o da venda
    SituacaoDaCaravana situacao = SituacaoDaCaravana::EmCasa;
    int mapaDeDestino = 0;
    std::vector<int> marcadoNoDestino;  // o que ela somou ao aCaminhoPorItem do destino (CO1-2)
    int64_t tickDaPartida = 0;          // para o salário dos dias fora
};

// As contas do comércio do mundo (só para o relatório do --simular; não vão no save).
struct EstatisticasDoComercio {
    int viagens = 0;
    int atacadas = 0;
    int perdidas = 0;
    int64_t valorVendido = 0;
    int64_t valorDeIdaEsperado = 0;
    int64_t dinheiroPerdido = 0;
};
