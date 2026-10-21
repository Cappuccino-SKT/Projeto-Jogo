// O programa de testes: roda todos os testes do Codigo/ e diz se passaram.
// Pelo terminal: ctest --test-dir build (ou .\build\Codigo\Testes\testes_do_codigo.exe)

#include <cstdio>

#ifdef _WIN32
#include <windows.h>  // só para o console do Windows mostrar os acentos
#endif

#include "ConferenciaDosTestes.h"

// Cada arquivo de testes tem uma função que roda todos os testes dele.
void testarRelogioDoMundo();
void testarMapaLocal();
void testarDadosDoJogo();
void testarBuscaDeCaminho();
void testarMovimentoDosNpcs();
void testarRotinaDosNpcs();
void testarProducaoEEconomia();
void testarSimulacaoDeUmAno();
void testarMarco8();
void testarSave();
void testarClima();
void testarTrabalho();
void testarPopulacao();
void testarCura();
void testarViagem();

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    testarRelogioDoMundo();
    testarMapaLocal();
    testarDadosDoJogo();
    testarBuscaDeCaminho();
    testarMovimentoDosNpcs();
    testarRotinaDosNpcs();
    testarProducaoEEconomia();
    testarSimulacaoDeUmAno();
    testarMarco8();
    testarSave();
    testarClima();
    testarTrabalho();
    testarPopulacao();
    testarCura();
    testarViagem();

    std::printf("\n%d conferências, %d falharam.\n", quantidadeDeConferenciasFeitas, quantidadeDeConferenciasQueFalharam);
    if (quantidadeDeConferenciasQueFalharam > 0) {
        std::printf("FALHOU\n");
        return 1;
    }
    std::printf("PASSOU\n");
    return 0;
}
