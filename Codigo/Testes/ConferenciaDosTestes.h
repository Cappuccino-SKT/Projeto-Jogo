#pragma once

// A ferramenta de testes do projeto (rodada FL1-4): sem biblioteca, só uma
// função que confere uma condição e anota quando ela falha.

#include <cstdio>

inline int quantidadeDeConferenciasFeitas = 0;
inline int quantidadeDeConferenciasQueFalharam = 0;

// Confere se a condição é verdadeira. Se não for, escreve o que falhou.
// Exemplo: conferir(relogio.horaDoDiaAtual() == 10, "o tick 27.200 é 10h");
inline void conferir(bool condicaoEsperada, const char* oQueEstaSendoConferido)
{
    ++quantidadeDeConferenciasFeitas;
    if (!condicaoEsperada) {
        ++quantidadeDeConferenciasQueFalharam;
        std::printf("  FALHOU: %s\n", oQueEstaSendoConferido);
    }
}
