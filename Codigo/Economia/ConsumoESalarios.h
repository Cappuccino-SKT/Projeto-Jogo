#pragma once

// O consumo da cesta mínima e os salários (Ficha Lógica: Economia, marco 6):
// à meia-noite, a vila come (a classe paga ao dono do item) e os empregados
// recebem pelas horas trabalhadas (o dono da construção paga). Usa o mesmo
// contexto da produção.

#include <cstdint>
#include <string>

#include "Producao/ProducaoDaVila.h"

// A conta da parte 5: ticks × centésimos por hora × bônus de nível (%) ×
// gerente (%) ÷ (800 × 10 × 100 × 100), com a metade subindo. 6.320 ticks de
// Manufaturado (100), nível 1, sem ser gerente: 79 décimos.
int64_t calcularSalarioEmDecimos(int ticksTrabalhados, int centesimosPorHora, int bonusDeNivelEmPorcento, int gerenteEmPorcento,
                                 int ticksPorHoraDeJogo);

// Regra 4.1: cada Civil come (a comida mais barata; no empate, a da Cidade) e,
// a cada 8 dias, gasta um Material Básico. A classe paga ao dono: à Cidade, o
// preço da loja (110%); ao jogador, o preço base. Sem dinheiro, o item sai do
// mesmo jeito, e a parte que faltou vai para o consumoNaoPagoHoje.
void consumirACestaDoDia(ContextoDaProducao& contexto);

// Regra 4.2: o salário de cada empregado pelas horas do dia. Quem paga é o
// dono da construção; sem dinheiro, o empregado não recebe, e a construção
// soma 1 dia de salário atrasado. Zera as horas do dia.
void pagarOsSalariosDoDia(ContextoDaProducao& contexto);

// À meia-noite, os passos do "ordem_do_dia", na ordem (FT1-5).
void rodarAOrdemDoDia(ContextoDaProducao& contexto);

// A linha do console na virada do dia (FL6-8): o estoque, o dinheiro, a
// riqueza média, os salários do dia e os dias em déficit.
std::string montarResumoDoDia(const ContextoDaProducao& contexto, int64_t diaQueAcabou);
