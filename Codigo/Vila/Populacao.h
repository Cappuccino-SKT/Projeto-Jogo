#pragma once

// A população da vila (Ficha Lógica: População, fase A3): a idade e a
// velhice, os nascimentos e as mortes em pessoas (o acumulado), a fome e o
// frio que matam aos poucos, a migração, e os NPCs que chegam e saem.

#include <string>
#include <vector>

#include "Producao/ProducaoDaVila.h"
#include "Vila/RegrasDaPopulacao.h"

// O passo "populacao" da ordem do dia (depois da Felicidade): a fome e o frio
// todo dia; no dia 1 de cada estação, a idade, a velhice, os nascimentos e a
// migração. Devolve as linhas do Diário.
std::vector<std::string> rodarAPopulacaoDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio);

// A capacidade de NPCs reais da vila (nível 1 = 15; a Cidade, fase A5, muda o nível).
int capacidadeDaVila(const ContextoDaProducao& contexto);

// Regra 4.2: o que entra no acumulado numa estação (nascimentos − mortalidade).
// 15 NPCs, Felicidade 58, sem fome: 69 − 15 = 54.
int calcularCrescimentoDaEstacao(int npcsReais, int felicidadeEmDecimos, bool houveFome, const RegrasDaPopulacao& regras);

// Regra 4.1, passo 2: a chance de morrer de velhice, em %. 23 anos: 30.
int chanceDeMorrerDeVelhice(int idade, bool invernoRigoroso, const RegrasDaPopulacao& regras);

// Regra 4.5: cria um NPC Civil na casa com mais camas livres. Sem cama, não
// cria e devolve entt::null.
// Fase C2: o nascido tem a cultura base da cidade; o migrante, a da região (75%) ou a de uma vizinha.
entt::entity criarNpcNaVila(ContextoDaProducao& contexto, int idade, int habilidadeBracal, int habilidadeManufaturado, bool ehMigrante = false);

// Regra 4.6: tira o NPC da vila (a vaga e a cama ficam livres; o número não volta).
void tirarNpcDaVila(ContextoDaProducao& contexto, entt::entity npc);
