// O programa de testes do Jogo/: só as partes que não abrem janela (a câmera e as teclas).
// Pelo terminal: ctest --test-dir build (roda este e o do Codigo/).

#include <cstdio>

#ifdef _WIN32
#include <windows.h>  // só para o console do Windows mostrar os acentos
#endif

#include "ConferenciaDosTestes.h"

void testarCameraDoMapa();
void testarTabelaDeAcoesDoTeclado();

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    testarCameraDoMapa();
    testarTabelaDeAcoesDoTeclado();

    std::printf("\n%d conferências, %d falharam.\n", quantidadeDeConferenciasFeitas, quantidadeDeConferenciasQueFalharam);
    if (quantidadeDeConferenciasQueFalharam > 0) {
        std::printf("FALHOU\n");
        return 1;
    }
    std::printf("PASSOU\n");
    return 0;
}
