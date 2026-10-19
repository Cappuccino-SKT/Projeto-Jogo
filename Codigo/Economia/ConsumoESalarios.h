#pragma once

// O consumo da cesta mínima e os salários (Ficha Lógica: Economia, marco 6):
// à meia-noite, a vila come (a classe paga ao dono do item) e os empregados
// recebem pelas horas trabalhadas (o dono da construção paga). Usa o mesmo
// contexto da produção.
//
// O marco 7 (parte 10) acrescenta: o gasto extra no consumo; os
// multiplicadores, o imposto e a saída por atraso nos salários; o
// estabilizador e a Felicidade como passos novos da ordem do dia.

#include <cstdint>
#include <string>
#include <vector>

#include "Producao/ProducaoDaVila.h"

// A conta da parte 5, com os multiplicadores da regra 10.6: ticks ×
// centésimos por hora × bônus de nível (%) × gerente (%) × riqueza (%) ×
// desemprego (%) ÷ (800 × 10 × 100 × 100 × 100 × 100), com a metade subindo.
// 6.320 ticks de Manufaturado (100), nível 1, sem ser gerente, riqueza 98 e
// desemprego 90: 70 décimos (69,7).
int64_t calcularSalarioEmDecimos(int ticksTrabalhados, int centesimosPorHora, int bonusDeNivelEmPorcento, int gerenteEmPorcento,
                                 int ticksPorHoraDeJogo, int riquezaEmPorcento = 100, int desempregoEmPorcento = 100);

// Regra 10.6: a riqueza da cidade = 80 + 40 × riqueza média ÷ (10 dias da
// cesta), presa entre 80 e 120, arredondada para baixo. A cesta vem em
// décimos por "dias entre Materiais" (8 dias): 8 Pães a 1,9 e 1 Móvel a 5,1 = 203.
int calcularMultiplicadorDaRiqueza(int64_t dinheiroDaClasse, int quantidadeDeCivis, int64_t cestaDeUmCivilEmDecimos, int diasDaCesta,
                                   const RegrasDosMultiplicadoresDoSalario& regras);

// Regra 10.6: o desemprego = 110 − a taxa de desemprego (em %), presa entre 90 e 110.
int calcularMultiplicadorDoDesemprego(int desempregados, int quantidadeDeCivis, const RegrasDosMultiplicadoresDoSalario& regras);

// Regra 10.7: 20% do salário, com a metade subindo (70 décimos: 14).
int64_t calcularImpostoDoSalario(int64_t salarioEmDecimos, const RegrasDoImposto& regras);

// O custo da cesta de um Civil em "dias entre Materiais" (8 dias): 8 comidas
// e 1 Material Básico, pelo preço que a classe paga hoje (o Pão na loja da
// cidade, o Móvel no preço local do jogador). Com 1,9 e 5,1: 203.
int64_t calcularCestaDeUmCivilEmDecimos(const ContextoDaProducao& contexto);

// Os desempregados da classe (sem trabalho, entre os NPCs da vila).
int contarOsDesempregados(const ContextoDaProducao& contexto);

// Regra 4.1, com o gasto extra da regra 10.5: cada Civil come (a comida mais
// barata; no empate, a da Cidade) e, a cada 8 dias, gasta um Material
// Básico. A classe paga ao dono: à Cidade, o preço da loja (110% do local);
// ao jogador, o preço local. Sem dinheiro, o item sai do mesmo jeito, e a
// parte que faltou vai para o consumoNaoPagoHoje. Depois, o gasto extra:
// Materiais a mais e, o resto, a loja da cidade. No fim, o dia do consumo
// medido fecha (regra 10.1 da Produção).
void consumirACestaDoDia(ContextoDaProducao& contexto);

// Regra 4.2, com as regras 10.6 a 10.8: o salário de cada empregado pelas
// horas do dia, com os multiplicadores e o imposto. Quem paga é o dono da
// construção; sem dinheiro, o empregado não recebe, e a construção soma 1 dia
// de salário atrasado; com 16, os empregados saem. Zera as horas do dia.
// Devolve as linhas do console (quem saiu).
std::vector<std::string> pagarOsSalariosDoDia(ContextoDaProducao& contexto);

// Regra 10.9: o Banco ajuda a classe abaixo de 3 dias da cesta, até 5 dias,
// com o teto de 10% do Banco do começo da estação.
void rodarOEstabilizador(ContextoDaProducao& contexto, const RelogioDoMundo& relogio);

// O passo "felicidade" (ficha da Felicidade, regras 4.1 e 4.2).
void rodarAFelicidadeDoDia(ContextoDaProducao& contexto);

// À meia-noite, os passos do "ordem_do_dia", na ordem (FT1-5). O relógio já
// está no dia novo. Devolve as linhas do console (os eventos, quem saiu).
std::vector<std::string> rodarAOrdemDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio);

// A linha do console na virada do dia (FL6-8): o estoque, os preços, o
// dinheiro, a riqueza média, os salários e o imposto, o gasto extra, a
// Felicidade e os dias em déficit.
std::string montarResumoDoDia(const ContextoDaProducao& contexto, int64_t diaQueAcabou);
