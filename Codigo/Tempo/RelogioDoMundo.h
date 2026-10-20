#pragma once

// O relógio do mundo: conta o tempo do jogo em ticks (passos fixos de 4,5
// segundos de jogo) e diz a data, a fase do dia e quando começa uma hora, um
// dia, uma estação ou um ano novo. Ficha Lógica: Tempo.
//
// Ele nunca pergunta as horas ao Windows: quem mede o tempo real é o Jogo/,
// que entrega o número. Assim o teste sem tela pode avançar tick a tick.

#include <cstdint>

#include "Tempo/RegrasDoTempo.h"

enum class Estacao { Primavera, Verao, Outono, Inverno };

enum class FaseDoDia { Amanhecer, Dia, Entardecer, Noite };

class RelogioDoMundo {
public:
    // Cria o relógio num tick qualquer (usado nos testes e, no marco 9, ao carregar o save).
    RelogioDoMundo(const RegrasDoTempo& regrasDoTempo, int64_t tickInicialDaPartida);

    // Cria o relógio de uma partida nova: 1º dia da Primavera, na hora inicial (6h).
    static RelogioDoMundo criarParaPartidaNova(const RegrasDoTempo& regrasDoTempo);

    // --- Regra 4.1: quantos ticks rodar neste quadro da tela ---
    // Soma o tempo real ao acumulador e devolve quantos ticks devem rodar.
    // Quem chama roda avancarUmTick() essa quantidade de vezes, conferindo as
    // viradas depois de cada um: num quadro com 2 ticks, a virada do primeiro
    // não pode se perder.
    int calcularTicksParaAvancarPeloTempoReal(int64_t nanossegundosReaisDesdeOUltimoQuadro);

    // Quanto do próximo tick já passou, de 0 a 1 (marco 4, FL4-5): o desenho
    // põe os NPCs entre a posição do tick anterior e a do atual.
    double fracaoJaPassadaDoProximoTick() const;

    // --- Regra 4.2: avançar 1 tick ---
    void avancarUmTick();

    // As viradas do tick que acabou de chegar. Valem só para um tick alcançado
    // por avancarUmTick(): o tick em que a partida começa não dispara nada.
    bool comecouUmaHoraNova() const;
    bool comecouUmDiaNovo() const;
    bool comecouUmaEstacaoNova() const;
    bool comecouUmAnoNovo() const;

    // --- Regra 4.3: mudar a velocidade ---
    // Aceita só 0 (pausa), 1, 2 ou 4. Devolve false se o valor foi ignorado.
    bool definirVelocidadeDoJogo(int novaVelocidade);
    void alternarPausa();
    bool jogoEstaPausado() const { return velocidadeDoJogo == 0; }
    int velocidadeAtualDoJogo() const { return velocidadeDoJogo; }

    // --- Regra 4.4: ler a data e a fase do dia (contas da parte 5) ---
    int64_t tickAtual() const { return tickAtualDaPartida; }
    int horaDoDiaAtual() const;            // de 0 a 23
    int minutoDaHoraAtual() const;         // de 0 a 59
    int64_t diaDaPartida() const;          // contando do 0
    int diaDoAnoAtual() const;             // de 0 a 63
    int diaDaEstacaoAtual() const;         // de 1 a 16
    int anoAtual() const;                  // 412, 413...
    Estacao estacaoAtual() const;
    int diasPorEstacao() const { return regras.diasPorEstacao; }
    FaseDoDia faseDoDiaAtual() const;

private:
    friend struct AcessoAoEstadoParaOSave;  // o save grava e restaura o estado (marco 9)
    RegrasDoTempo regras;

    // O estado (ficha, parte 2). A data inteira sai do tickAtualDaPartida.
    int64_t tickAtualDaPartida = 0;
    int velocidadeDoJogo = 1;
    int velocidadeAntesDaPausa = 1;
    int64_t nanossegundosAcumuladosParaOProximoTick = 0;  // não vai no save: é menos de 1 tick

    // Verdadeiro depois de um avancarUmTick(). Garante que o tick inicial da
    // partida (por exemplo, 4.800 = 6h) não conte como "hora nova".
    bool tickAtualFoiAlcancadoAgora = false;

    bool tickAtualEhMultiploDe(int64_t quantidadeDeTicks) const;
};
