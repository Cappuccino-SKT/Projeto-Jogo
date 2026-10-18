#pragma once

// As contas da Ficha Lógica: Construção (partes 4 e 5): as vagas, a
// capacidade e o gerente.

#include <vector>

#include "Construcao/RegrasDasConstrucoes.h"
#include "Mapa/MapaLocal.h"

// As vagas do nível: 2, 3, 5, 7 e 10 (do nível 1 ao 5).
int calcularVagas(int nivel, const RegrasDasConstrucoes& regras);

// A capacidade do porte × o nível: uma média no nível 2 guarda 200 × 2 = 400.
int calcularCapacidade(PorteDoLote porte, int nivel, const RegrasDasConstrucoes& regras);

// Um empregado que pode ser o gerente.
struct CandidatoAGerente {
    int numeroDoNpc = 0;
    int habilidadeNoTrabalho = 0;
};

// Regra 4.2 (V3-8): o de maior habilidade no Trabalho da construção; no
// empate, o de menor número na vila. Devolve o número do NPC, ou -1 sem ninguém.
int escolherGerente(const std::vector<CandidatoAGerente>& empregados);
