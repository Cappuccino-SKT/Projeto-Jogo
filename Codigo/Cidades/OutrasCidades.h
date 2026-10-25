#pragma once

// As outras cidades do mundo (Ficha Lógica: Outras cidades, LOD 1 e 2, fase B1b).
// Cada cidade é uma SimulacaoDaVila em LOD 2: as mesmas regras do dia e a mesma
// produção por hora da vila, mas os NPCs não andam; o trabalho de cada empregado
// no turno é presumido (98% da hora, OC1-1). Aqui ficam a criação da cidade (4.1),
// o mapa sem desenho dela e o trabalho presumido (4.2 e 4.3).

#include <cstdint>

#include "Producao/ProducaoDaVila.h"
#include "Viagem/MundoDoJogo.h"

// OC1-3: cada cidade sorteia com as suas sequências: 100 + mapa × 10 + tipo (1 NPCs,
// 2 eventos, 3 clima, 4 População, 7 a criação da cidade).
uint64_t sequenciaDoSorteioDaCidade(int numeroDoMapa, int tipo);

// Regra 4.1: as construções (o conjunto da vila inicial a cada 15 NPCs, as Braçais de
// comida × 100 ÷ produtividade), os NPCs, as casas, o estoque e o dinheiro.
VilaDoJogo gerarAVilaDaCidade(const CidadeDoMundo& cidade, const DadosDoJogo& dados, uint64_t sementeDaPartida);

// O "mapa" de uma cidade sem desenho: 1 tile e uma área por recurso do mapa do mundo,
// com a produtividade dele (as Braçais usam a área; os NPCs não andam).
MapaLocal montarOMapaDaCidadeEmLod2(const MapaDoMundo& mapaDoMundo, const DadosDoJogo& dados);

// 800 × 98 ÷ 100 = 784.
int ticksPresumidosPorHora(const DadosDoJogo& dados, int ticksPorHoraDeJogo);

// Regra 4.2: o empregado conta na hora que acabou se ela é do turno dele e ele não está doente.
bool empregadoPresenteNestaHora(const ContextoDaProducao& contexto, entt::entity npc, int horaQueAcabou);

// Regra 4.3: põe os ticks presumidos em cada empregado presente (no lugar dos ticks contados).
void presumirOTrabalhoDaHora(ContextoDaProducao& contexto, int horaQueAcabou);
