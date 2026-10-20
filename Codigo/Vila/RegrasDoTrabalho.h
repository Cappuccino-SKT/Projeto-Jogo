#pragma once

// O bloco "trabalho" do data/regras.json (Ficha Lógica: Trabalho e
// Propriedade, parte 2) e o estado do trabalho que vai no save.

#include <cstdint>

struct RegrasDoTrabalho {
    int contratarSeOBancoTemDiasDeSalario = 3;  // regra 4.1, passo 1
    int diferencaMinimaParaTrocar = 10;         // regra 4.2
    int valorDaConstrucaoEmPorcentoDaObra = 100; // regra 4.3
    int cidadeCompraDeVoltaEmPorcento = 75;
    int estacoesParadaParaOLeilao = 2;          // regra 4.4
    int diasDeLeilao = 3;
    int lanceMinimoEmPorcentoDoValor = 50;
};

// Regra 4.4: o leilão aberto (no máximo um por vez na vila).
struct LeilaoDeConstrucao {
    int indiceDaConstrucao = -1;  // -1 = nenhum leilão
    int diasQueFaltam = 0;
    int64_t maiorLanceEmDecimos = 0;
    bool lanceEhDoJogador = false;  // o dinheiro do lance fica preso até o fim

    bool estaAberto() const { return indiceDaConstrucao >= 0; }
};

struct EstadoDoTrabalho {
    LeilaoDeConstrucao leilao;
    // Só do dia (para o relatório; não vão no save).
    int contratacoesDeHoje = 0;
    int trocasDeHoje = 0;
};
