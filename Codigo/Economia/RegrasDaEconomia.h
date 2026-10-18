#pragma once

// Os números do consumo e dos salários (Ficha Lógica: Economia, parte 2),
// lidos do data/regras.json.

#include <vector>

// Os passos da virada do dia, na ordem do "ordem_do_dia" (FT1-5). O marco 7
// acrescenta os outros (Mercado Local, estabilizador, Felicidade, eventos).
enum class PassoDaOrdemDoDia { Consumo, Salarios, EscolhaDasReceitas };

struct RegrasDaEconomia {
    // A cesta mínima do Civil: 1 comida por dia e 1 Material Básico a cada 8 dias.
    int comidaPorDiaDoCivil = 1;
    int diasEntreMateriaisDoCivil = 8;
    // A loja da cidade vende a 110% do preço (V2-5).
    int lojaDaCidadeVendeEmPorcento = 110;
    // +10% de salário por nível acima do 1, e o gerente ganha × 1,5.
    int bonusDoSalarioPorNivelEmPorcento = 10;
    int salarioDoGerenteEmPorcento = 150;
    std::vector<PassoDaOrdemDoDia> ordemDoDia = {PassoDaOrdemDoDia::Consumo, PassoDaOrdemDoDia::Salarios,
                                                 PassoDaOrdemDoDia::EscolhaDasReceitas};
};
