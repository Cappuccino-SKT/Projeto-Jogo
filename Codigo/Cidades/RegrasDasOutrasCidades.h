#pragma once

// O bloco "outras_cidades" do data/regras.json (Ficha Lógica: Outras cidades, parte 2).

#include <string>
#include <vector>

#include "Dados/ProblemasDosDados.h"

struct RegrasDasOutrasCidades {
    int trabalhoPresumidoEmPorcento = 98;  // 7,9 das 8 horas do turno (FL6-7)
    std::vector<int> npcsReaisPorNivel = {15, 40, 100, 300};
    std::vector<int> dinheiroInicialDoBancoPorNivel = {400, 1500, 6000, 20000};  // em moedas
    int verbaDaCoroaPorNpcPorEstacaoEmDecimos = 50;
    int construcoesDeProducaoACadaNpcs = 15;
    int coberturaDaComidaEscassaEmPorcento = 75;  // CO2-1: as Braçais de comida onde o recurso é escasso
};

// Lê o bloco do regras.json inteiro; sem o bloco, ficam os valores da ficha.
RegrasDasOutrasCidades lerRegrasDasOutrasCidades(const std::string& textoDoRegras, ListaDeProblemasDosDados& listaDeProblemas);
