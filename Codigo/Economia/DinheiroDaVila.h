#pragma once

// O dinheiro da vila (Ficha Lógica: Economia, partes 2 e 10.2). Sempre
// inteiro, em décimos de moeda (V2-1): 17 = 1,7 moeda. Usa 64 bits para o
// teste de 1 ano (marco 7) nunca estourar.

#include <cstdint>

struct DinheiroDaVila {
    int64_t bancoDaCidade = 0;
    int64_t dinheiroDoJogador = 0;
    // A soma dos Civis: a riqueza média é esta soma ÷ a quantidade de Civis,
    // e guardar a soma nunca perde décimos na divisão.
    int64_t dinheiroDaClasseCivil = 0;
    int quantidadeDeCivis = 0;
    int64_t rendaDoDiaDaClasseCivil = 0;  // os salários brutos do último dia (antes do imposto)

    int diasEmDeficitDeComida = 0;
    int diasEmDeficitDeMateriais = 0;
    // A fração de Material Básico que a vila já deve (em "dias" do
    // materiais_a_cada_dias: de 0 a 7 com 8 dias). Sem números com vírgula.
    int materiaisDevidosEmOitavos = 0;

    // O estabilizador (marco 7, regra 10.9): vão no save.
    int64_t bancoNoComecoDaEstacao = 0;
    int64_t ajudaDoEstabilizadorNaEstacao = 0;
    // O dinheiro que entrou de fora da vila (o Mercador viajante): é a única
    // coisa que muda o dinheiro total. Não vai no save (só para os testes).
    int64_t dinheiroQueEntrouDeFora = 0;

    // Só para o console, a Felicidade e o relatório (não vão no save).
    int64_t consumoNaoPagoHoje = 0;
    int comidaQueFaltouHoje = 0;
    int64_t gastoDoJogadorComInsumosHoje = 0;
    int64_t gastoDaCestaDeHoje = 0;
    int64_t gastoExtraDeHoje = 0;
    int64_t impostoDeHoje = 0;
    int64_t ajudaDoEstabilizadorHoje = 0;

    int64_t dinheiroTotal() const { return bancoDaCidade + dinheiroDoJogador + dinheiroDaClasseCivil; }
};
