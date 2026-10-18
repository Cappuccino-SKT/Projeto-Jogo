#include "Construcao/ContasDasConstrucoes.h"

#include <algorithm>

int calcularVagas(int nivel, const RegrasDasConstrucoes& regras)
{
    const int nivelDentroDaTabela = std::clamp(nivel, 1, regras.nivelMaximo());
    return regras.vagasPorNivel[nivelDentroDaTabela - 1];
}

int calcularCapacidade(PorteDoLote porte, int nivel, const RegrasDasConstrucoes& regras)
{
    return regras.capacidadePorNivelDeCadaPorte[static_cast<int>(porte)] * nivel;
}

int escolherGerente(const std::vector<CandidatoAGerente>& empregados)
{
    int numeroDoGerente = -1;
    int maiorHabilidade = -1;
    for (const CandidatoAGerente& empregado : empregados) {
        const bool ganha = empregado.habilidadeNoTrabalho > maiorHabilidade
                        || (empregado.habilidadeNoTrabalho == maiorHabilidade && empregado.numeroDoNpc < numeroDoGerente);
        if (ganha) {
            maiorHabilidade = empregado.habilidadeNoTrabalho;
            numeroDoGerente = empregado.numeroDoNpc;
        }
    }
    return numeroDoGerente;
}
