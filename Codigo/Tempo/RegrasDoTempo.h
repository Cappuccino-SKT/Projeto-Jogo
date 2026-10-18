#pragma once

// As definições do tempo: números que não mudam durante o jogo.
// Ficha Lógica: Tempo, parte 2. No marco 1 os valores ficam aqui; a partir do
// marco 3, eles passam a ser lidos do data/regras.json.

#include <cstdint>

struct RegrasDoTempo {
    int ticksPorSegundoReal = 20;   // a 1x; 1 tick = 4,5 segundos de jogo
    int ticksPorHoraDeJogo = 800;   // 1 hora de jogo = 40 segundos reais a 1x
    int horasPorDia = 24;
    int diasPorEstacao = 16;
    int estacoesPorAno = 4;

    int anoInicialDaPartida = 412;  // Mundo e Estado Inicial
    int horaInicialDaPartida = 6;   // FL1-2: a partida começa quando os Civis acordam

    // FL1-3: se um quadro demorar mais que isso (o PC travou, a janela foi
    // arrastada), o relógio só anda este tanto, para não travar em cadeia.
    int limiteDeTempoRealPorQuadroEmMilissegundos = 250;

    // Fases do dia (Clima e Estações): a hora em que cada fase começa.
    int horaEmQueComecaOAmanhecer = 5;
    int horaEmQueComecaODia = 7;
    int horaEmQueComecaOEntardecer = 18;
    int horaEmQueComecaANoite = 20;

    // Valores derivados: calculados a partir dos de cima, nunca escritos à mão.
    int64_t ticksPorDia() const { return static_cast<int64_t>(ticksPorHoraDeJogo) * horasPorDia; }
    int64_t ticksPorEstacao() const { return ticksPorDia() * diasPorEstacao; }
    int64_t ticksPorAno() const { return ticksPorEstacao() * estacoesPorAno; }
    int64_t nanossegundosReaisDeUmTickEmVelocidade1() const { return 1'000'000'000LL / ticksPorSegundoReal; }
};
