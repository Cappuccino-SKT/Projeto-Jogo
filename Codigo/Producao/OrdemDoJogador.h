#pragma once

// As ordens das construções do jogador (Ficha Lógica: Produção, regra 4.4).
// Até a interface do marco 8, elas vêm do vila_inicial.json (FL6-3).

enum class TipoDeOrdem {
    ManterEstoque,    // "manter_estoque": produz enquanto o jogador tem menos de N
    FazerQuantidade,  // "fazer_n": produz N e sai da lista
    RepetirSempre,    // "repetir": produz sempre
};

struct OrdemDoJogador {
    TipoDeOrdem tipo = TipoDeOrdem::ManterEstoque;
    int numeroDoItem = 0;
    int quantidade = 0;         // o N (não vale no "repetir")
    int quantidadeJaFeita = 0;  // só no "fazer_n"
};
