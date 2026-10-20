#pragma once

// Os números do consumo, dos salários e (marco 7) do Mercado Local, do gasto
// extra, do imposto e do estabilizador (Ficha Lógica: Economia, partes 2 e
// 10.2), lidos do data/regras.json.

#include <vector>

// Os passos da virada do dia, na ordem do "ordem_do_dia" (FT1-5).
enum class PassoDaOrdemDoDia { Manutencao, Consumo, Salarios, MercadoLocal, Estabilizador, Felicidade, Eventos, EscolhaDasReceitas, Clima, Populacao };

// O preço que anda com o estoque (V2-5, regra 10.4).
struct RegrasDoMercadoLocal {
    int diasDoEstoqueAlvo = 5;
    int fatorMinimoEmPorcento = 50;
    int fatorMaximoEmPorcento = 200;
    int passoMaximoPorDiaEmPorcento = 10;
};

// FL7-2: 20%, retido no salário, e a cidade fica com tudo (na fatia, sem facção).
struct RegrasDoImposto {
    int taxaEmPorcento = 20;
    int parteDaCidadeEmPorcento = 100;
};

// V2-7: abaixo de 3 dias da cesta, o Banco paga até 5 dias, com o teto de
// 10% do Banco do começo da estação.
struct RegrasDoEstabilizador {
    int ajudaAbaixoDeDiasDaCesta = 3;
    int ajudaAteDiasDaCesta = 5;
    int tetoPorEstacaoEmPorcentoDoBanco = 10;
};

// FL7-3: a riqueza da cidade e o desemprego mudam o salário (regra 10.6).
struct RegrasDosMultiplicadoresDoSalario {
    int riquezaMinimaEmPorcento = 80;
    int riquezaMaximaEmPorcento = 120;
    int diasDaCestaParaOMaximo = 10;
    int desempregoMinimoEmPorcento = 90;
    int desempregoMaximoEmPorcento = 110;
};

// FL8-6 e FL8-7: as decisões do jogador (Ficha Lógica: Economia, parte 11).
struct RegrasDasDecisoesDoJogador {
    int faixaDoPrecoEmPorcento = 30;
    int passoDoPrecoEmPorcento = 5;
    int salarioBaixoEmPorcento = 75;
    int salarioAltoEmPorcento = 125;
    int velocidadeComSalarioBaixoEmPorcento = 90;
    int velocidadeComSalarioAltoEmPorcento = 110;
    int diasDeSalarioBaixoParaUmEmpregadoSair = 8;
};

struct RegrasDaEconomia {
    // A cesta mínima do Civil: 1 comida por dia e 1 Material Básico a cada 8 dias.
    int comidaPorDiaDoCivil = 1;
    int diasEntreMateriaisDoCivil = 8;
    // A loja da cidade vende a 110% do preço local e (a partir do marco 8,
    // e já no Mercador viajante) compra a 90% (V2-5).
    int lojaDaCidadeVendeEmPorcento = 110;
    int lojaDaCidadeCompraEmPorcento = 90;
    // +10% de salário por nível acima do 1, e o gerente ganha × 1,5.
    int bonusDoSalarioPorNivelEmPorcento = 10;
    int salarioDoGerenteEmPorcento = 150;

    // Marco 7.
    RegrasDoMercadoLocal mercadoLocal;
    int gastoDaClasseEmPorcentoDaRenda = 80;  // o teto do gasto do dia (Classe Social)
    RegrasDoImposto imposto;
    RegrasDoEstabilizador estabilizador;
    // FE4-1 e FE4-2: a verba da Coroa, fixa, entra de fora no Banco no dia 1 de cada estação.
    int verbaDaCoroaPorEstacaoEmDecimos = 750;
    RegrasDosMultiplicadoresDoSalario multiplicadoresDoSalario;
    RegrasDasDecisoesDoJogador decisoesDoJogador;
    int diasDeAtrasoAteOsEmpregadosSairem = 16;  // uma estação (Fatia Vertical, 4.5)

    std::vector<PassoDaOrdemDoDia> ordemDoDia = {PassoDaOrdemDoDia::Consumo,       PassoDaOrdemDoDia::Salarios,
                                                 PassoDaOrdemDoDia::MercadoLocal,  PassoDaOrdemDoDia::Estabilizador,
                                                 PassoDaOrdemDoDia::Felicidade,    PassoDaOrdemDoDia::Eventos,
                                                 PassoDaOrdemDoDia::EscolhaDasReceitas};
};
