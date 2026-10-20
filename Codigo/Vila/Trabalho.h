#pragma once

// O mercado de trabalho e a propriedade (Ficha Lógica: Trabalho e
// Propriedade, fase A2): a Cidade contrata sozinha, redistribui os empregos a
// cada estação, o jogador compra e vende construções, e a construção parada
// vai a leilão.

#include <cstdint>
#include <string>
#include <vector>

#include "Producao/ProducaoDaVila.h"
#include "Vila/RegrasDoTrabalho.h"

// Regra 4.3: (Tábuas × preço local + Blocos × preço local + horas × 0,75) ×
// nível, em décimos. O Posto (pequeno, nível 1), com Tábua e Bloco a 1,5: 600.
int64_t calcularValorDaConstrucao(const ContextoDaProducao& contexto, int indiceDaConstrucao);

// Regra 4.1 (todo dia, depois dos salários): cada vaga vazia de uma
// construção da Cidade é preenchida pelo desempregado de maior habilidade,
// se o Banco tem 3 dias do salário da vaga. Devolve as linhas do Diário.
std::vector<std::string> preencherVagasDaCidade(ContextoDaProducao& contexto);

// Regra 4.2 (dia 1 de cada estação): troca os empregados da Cidade de
// Trabalhos diferentes quando a soma das habilidades usadas sobe 10 ou mais.
std::vector<std::string> redistribuirEmpregosDaCidade(ContextoDaProducao& contexto);

// Regra 4.4, passo 1 (todo dia, depois dos salários): conta os dias sem
// empregados e abre o leilão da construção do jogador parada há 2 estações.
std::vector<std::string> contarOsDiasSemEmpregados(ContextoDaProducao& contexto, int diasPorEstacao);

// Regra 4.4, passos 3 e 4 (todo dia, no passo dos eventos): o leilão anda um
// dia e, no fim, vai para quem deu o maior lance ou volta para a Cidade.
std::vector<std::string> avancarOLeilaoDoDia(ContextoDaProducao& contexto);

// Regra 4.3: os pedidos do jogador. Devolvem o motivo da recusa, ou "".
std::string comprarConstrucaoDaCidade(ContextoDaProducao& contexto, int indiceDaConstrucao);
std::string venderConstrucaoParaACidade(ContextoDaProducao& contexto, int indiceDaConstrucao);
std::string trocarOModoAutomatico(ContextoDaProducao& contexto, int indiceDaConstrucao, bool ligado);
// Regra 4.4, passo 3: o lance do jogador, em décimos.
std::string darLanceNoLeilao(ContextoDaProducao& contexto, int64_t lanceEmDecimos);

// Se a construção segue a fila de ordens do jogador (do jogador e com o modo
// automático desligado). As do jogador no modo automático seguem a IA da Cidade.
bool construcaoSegueAFilaDoJogador(const ContextoDaProducao& contexto, int indiceDaConstrucao);
