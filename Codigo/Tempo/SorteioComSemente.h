#pragma once

// O sorteio com semente (ficha da IA, regra 4.6, FL5-5): o gerador PCG32,
// pequeno e próprio. A mesma semente dá sempre os mesmos números, em
// qualquer computador e compilador (as funções de sorteio do C++ não garantem isso).
//
// Cada sistema tem o seu gerador, ligado à mesma semente da partida, com um
// número de sequência diferente: os NPCs usam a sequência 1; os eventos (marco 7), a 2.

#include <cstdint>

constexpr uint64_t sequenciaDoSorteioDosNpcs = 1;

class SorteioComSemente {
public:
    SorteioComSemente(uint64_t sementeDaPartida, uint64_t numeroDaSequencia);

    // Um número de 0 a 4.294.967.295, todos com a mesma chance.
    uint32_t proximoNumero();

    // Um inteiro de "minimo" a "maximo" (os dois entram), todos com a mesma chance.
    int sortearNumeroInteiroEntre(int minimo, int maximo);

private:
    uint64_t estadoDoGerador = 0;
    uint64_t incrementoDaSequencia = 1;
};
