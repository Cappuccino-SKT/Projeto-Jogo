#pragma once

// As decisões do jogador (Ficha Lógica: Economia, parte 11, marco 8): o preço
// dos itens dele, o nível de salário, contratar e demitir, e a loja da cidade.
// As contas são funções puras; quem muda a vila são os pedidos
// (Jogador/PedidosDoJogador.h).

#include <cstdint>
#include <string>

#include "Economia/RegrasDaEconomia.h"
#include "Producao/ProducaoDaVila.h"

// Regra 11.2, passo 1: o preço local × (100 + p) ÷ 100, com a metade subindo.
// Móveis a 5,1 (51) e +20%: 61.
int calcularPrecoDoItemDoJogador(int precoLocalEmDecimos, int porcentagem);

// Regra 11.2, passo 3: com p ≤ 0, tudo; acima, quantidade × (faixa − p) ÷ faixa,
// para baixo. 2 Móveis a +20% (faixa 30): 0.
int calcularQuantidadeDoGastoExtraComOPreco(int quantidadeQueQueria, int porcentagem, const RegrasDasDecisoesDoJogador& regras);

// De −faixa a +faixa, de passo em passo.
bool precoEstaNaFaixaPermitida(int porcentagem, const RegrasDasDecisoesDoJogador& regras);

// Regra 11.5: a loja da cidade compra a 90% do preço local (a metade sobe).
int calcularPrecoQueALojaDaCidadePaga(int precoLocalEmDecimos, const RegrasDaEconomia& regras);

// Regra 11.3: o salário e a velocidade de cada nível, em porcento.
int salarioDoNivelEmPorcento(NivelDeSalario nivel, const RegrasDasDecisoesDoJogador& regras);
int velocidadeDoNivelEmPorcento(NivelDeSalario nivel, const RegrasDasDecisoesDoJogador& regras);

// Regras 11.4 e 11.5: devolvem o motivo da recusa em texto, ou vazio se deu certo.
std::string contratarUmNpc(ContextoDaProducao& contexto, int indiceDaConstrucao, int numeroDoNpc);
std::string demitirUmNpc(ContextoDaProducao& contexto, int numeroDoNpc);
std::string comprarDaLojaDaCidade(ContextoDaProducao& contexto, int numeroDoItem, int quantidade);
std::string venderParaALojaDaCidade(ContextoDaProducao& contexto, int numeroDoItem, int quantidade);
