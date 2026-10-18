#include "Tempo/SorteioComSemente.h"

// O PCG32 de Melissa O'Neill (pcg-random.org), na versão mínima "pcg32_random_r".

SorteioComSemente::SorteioComSemente(uint64_t sementeDaPartida, uint64_t numeroDaSequencia)
{
    estadoDoGerador = 0;
    incrementoDaSequencia = (numeroDaSequencia << 1u) | 1u;  // precisa ser ímpar
    proximoNumero();
    estadoDoGerador += sementeDaPartida;
    proximoNumero();
}

uint32_t SorteioComSemente::proximoNumero()
{
    const uint64_t estadoAnterior = estadoDoGerador;
    estadoDoGerador = estadoAnterior * 6364136223846793005ULL + incrementoDaSequencia;
    // Mistura os bits do estado anterior e gira o resultado.
    const uint32_t bitsMisturados = static_cast<uint32_t>(((estadoAnterior >> 18u) ^ estadoAnterior) >> 27u);
    const uint32_t giro = static_cast<uint32_t>(estadoAnterior >> 59u);
    return (bitsMisturados >> giro) | (bitsMisturados << ((0u - giro) & 31u));
}

int SorteioComSemente::sortearNumeroInteiroEntre(int minimo, int maximo)
{
    if (maximo <= minimo) {
        return minimo;
    }
    const uint32_t tamanhoDaFaixa = static_cast<uint32_t>(maximo - minimo) + 1u;
    // Joga fora os números do "resto" no topo, para todos terem a mesma chance.
    const uint32_t limiteDeBaixo = (0u - tamanhoDaFaixa) % tamanhoDaFaixa;
    while (true) {
        const uint32_t numero = proximoNumero();
        if (numero >= limiteDeBaixo) {
            return minimo + static_cast<int>(numero % tamanhoDaFaixa);
        }
    }
}
