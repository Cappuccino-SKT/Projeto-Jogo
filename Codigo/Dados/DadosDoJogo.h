#pragma once

// Todos os dados do jogo, lidos na ordem da ficha dos Itens (parte 3):
// textos, regras, itens, construções, recursos, receitas, preços e o mapa.
// Usado pelo jogo ao abrir e pelo "jogo --validar".

#include <string>

#include "Dados/Itens.h"
#include "Dados/ProblemasDosDados.h"
#include "Dados/Receitas.h"
#include "Dados/Recursos.h"
#include "Dados/TextosERegras.h"
#include "Mapa/MapaLocal.h"

struct DadosDoJogo {
    TabelaDeTextos tabelaDeTextos;
    RegrasDoJogo regras;
    CatalogoDeItens catalogoDeItens;
    CatalogoDeConstrucoes catalogoDeConstrucoes;
    CatalogoDeRecursos catalogoDeRecursos;
    CatalogoDeReceitas catalogoDeReceitas;
    MapaLocal mapaDaVila;
    ListaDeProblemasDosDados listaDeProblemas;

    bool dadosEstaoProntos() const { return !listaDeProblemas.temErro(); }
};

// Lê tudo a partir da pasta dos dados (Jogo/data). Nunca para no primeiro
// erro: junta todos na listaDeProblemas.
DadosDoJogo carregarDadosDoJogo(const std::string& pastaDosDados);

// "Dados conferidos: 6 itens, 3 recursos, 6 receitas, 1 mapa. 0 erros, 0 avisos."
std::string montarResumoDosDados(const DadosDoJogo& dados);

// Uma linha por item: "Pão: 1,7 moeda". Mostrada ao abrir e no --validar.
std::string montarTabelaDePrecosBase(const DadosDoJogo& dados);

// Dinheiro em décimos para mostrar: 17 vira "1,7".
std::string escreverDinheiroEmMoedas(int valorEmDecimos);
