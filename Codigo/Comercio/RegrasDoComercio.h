#pragma once

// O bloco "comercio" do data/regras.json (Ficha Lógica: Comércio, parte 2). Os valores
// padrão são os da ficha.

#include <string>

#include "Dados/ProblemasDosDados.h"

struct RegrasDoComercio {
    int caravanasPorPostoComercial = 2;
    int condutores = 2;  // as pessoas da caravana são abstratas: não saem dos NPCs da cidade
    int guardas = 2;
    int cavalos = 4;
    int capacidadePorCavalo = 100;  // em unidades de peso (Pão 1, Tábua 2, Tora 3, Móveis 8)
    int salarioPorDiaDaCaravanaEmDecimos = 60;  // por pessoa
    int lojaCompraEmPorcento = 90;
    int lojaVendeEmPorcento = 110;
    int acimaDoAlvoALojaPagaEmPorcentoDoBase = 45;  // CO1-3
    int reservaDoBancoEmDecimos = 2500;              // FE4-3: a loja só paga acima disso
    int reservaDaLojaEmDiasDeConsumo = 2;            // RE1-4: a loja não vende ao jogador abaixo disso
    int lucroMinimoEmPorcentoDoCusto = 20;           // CO1-4
    int riscoChanceDePerderEmPorcento = 60;
    int riscoPerdaMediaEmPorcento = 60;
    int paesPorPessoaPorDia = 1;  // a comida da caravana (4 Pães por dia)

    int pessoasDaCaravana() const { return condutores + guardas; }
    int capacidadeDaCaravana() const { return cavalos * capacidadePorCavalo; }
};

// Lê o bloco do regras.json inteiro; sem o bloco, ficam os valores da ficha.
RegrasDoComercio lerRegrasDoComercio(const std::string& textoDoRegras, ListaDeProblemasDosDados& listaDeProblemas);
