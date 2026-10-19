#pragma once

// A medição do desempenho com o Tracy (ficha da Simulação, regra 4.4, FL7-8).
//
// O Tracy é um programa à parte que mostra quanto tempo cada trecho do jogo
// gasta. Ele só entra quando o CMake é configurado com -DUSAR_TRACY=ON
// (desligado por padrão): aí o jogo manda as medições pela rede para o
// programa do Tracy. Desligado, estas duas linhas não fazem nada e não custam nada.

#ifdef USAR_TRACY
#include <tracy/Tracy.hpp>
// Mede o tempo do trecho entre esta linha e o fim do bloco { }.
#define MEDIR_ESTE_TRECHO(nomeDoTrecho) ZoneScopedN(nomeDoTrecho)
// Cada tick é um "quadro" no Tracy.
#define MARCAR_O_FIM_DE_UM_TICK() FrameMark
#else
#define MEDIR_ESTE_TRECHO(nomeDoTrecho)
#define MARCAR_O_FIM_DE_UM_TICK()
#endif
