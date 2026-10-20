#pragma once

// O clima na vila (Ficha Lógica: Clima e Estações, regras 4.1 a 4.6): o passo
// "clima" da ordem do dia, a lenha do inverno, o estoque para o inverno e o
// que a Felicidade, a produção e os NPCs perguntam.

#include <cstdint>
#include <string>
#include <vector>

#include "Clima/Clima.h"
#include "Producao/ProducaoDaVila.h"

// O passo "clima" (o 0 da ordem do dia, antes do consumo): sorteia o clima de
// hoje e guarda a estação de hoje. Devolve a linha do console quando o clima
// muda ("Clima: começou a nevar.").
std::vector<std::string> rodarOClimaDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio);

// Ao montar a vila e ao carregar um save: o dia de hoje no estado do clima
// (a estação e o dia da estação), sem sortear nada.
void guardarODiaDeHojeNoClima(EstadoDoClima& clima, const RelogioDoMundo& relogio);

// Regra 4.3: a lenha do inverno, no passo do consumo, depois da cesta e antes
// do gasto extra. A Cidade é a dona da Tora; a classe paga o preço da loja.
void consumirALenhaDoDia(ContextoDaProducao& contexto);

// Regra 4.6: o que o estoque-alvo de um item ganha nos últimos 4 dias do
// Outono, em centésimos (Tora: 18 Toras para 15 Civis; Grão: 5 dias do consumo).
int64_t bonusDoEstoqueParaOInvernoEmCentesimos(const ContextoDaProducao& contexto, int numeroDoItem);

// Regra 4.4: os pontos que entram nos fatores da Felicidade hoje.
int pontosDoClimaNoConsumoAtendido(const ContextoDaProducao& contexto);  // −10 se faltou lenha
int pontosDoClimaNosEventos(const ContextoDaProducao& contexto);         // −2 no calor forte

// Regra 4.2, passo 2 (CL1-1): na tempestade, o Período Livre é em casa.
bool periodoLivreEmCasaHoje(const EstadoDoClima& clima, const RegrasDoClima& regras);
