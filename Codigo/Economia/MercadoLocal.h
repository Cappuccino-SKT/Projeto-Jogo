#pragma once

// O Mercado Local (Ficha Lógica: Economia, regra 10.4, V2-5): todo dia, o
// preço local de cada item anda na direção do preço que o estoque pede.
// Pouco estoque (perto de 0) sobe o preço até 200% do base; muito estoque
// (o dobro do alvo ou mais) desce até 50%. O passo é de no máximo 10% por dia.

#include <cstdint>
#include <vector>

#include "Economia/RegrasDaEconomia.h"
#include "Producao/ProducaoDaVila.h"

struct MercadoLocalDaVila {
    std::vector<int> precoLocalPorItemEmDecimos;  // um por item; no primeiro dia, o preço base
};

// Passo 2: 100 + (alvo − estoque) × 100 ÷ alvo, preso entre 50 e 200. O alvo
// vem em centésimos de unidade. Com alvo 0 (ninguém usa o item), 100.
int calcularFatorDoMercadoLocal(int estoque, int64_t estoqueAlvoEmCentesimos, const RegrasDoMercadoLocal& regras);

// Passo 3: o preço base × o fator, com a metade subindo (1,7 × 140% = 2,38 → 24 décimos).
int calcularPrecoDesejado(int precoBaseEmDecimos, int fatorEmPorcento);

// Passo 4: anda na direção do desejado, no máximo 10% do preço local do dia
// (arredondado, a metade subindo, e no mínimo 1 décimo), sem passar dele.
int darOPassoDoPrecoLocal(int precoLocalEmDecimos, int precoDesejadoEmDecimos, const RegrasDoMercadoLocal& regras);

// Ao montar a vila: todos os preços locais começam no preço base.
void comecarOMercadoLocal(ContextoDaProducao& contexto);

// À meia-noite, o passo "mercado_local" da ordem do dia.
void rodarOMercadoLocalDoDia(ContextoDaProducao& contexto);
