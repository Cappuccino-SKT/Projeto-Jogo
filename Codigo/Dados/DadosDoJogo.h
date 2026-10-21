#pragma once

// Todos os dados do jogo, lidos na ordem da ficha dos Itens (parte 3):
// textos, regras, itens, construções, recursos, receitas, preços, o mapa,
// desde o marco 5, a vila inicial (ficha da IA) e, desde o marco 7, os
// eventos (ficha dos Eventos).
// Usado pelo jogo ao abrir e pelo "jogo --validar".

#include <string>

#include "Clima/Clima.h"
#include "Dados/Itens.h"
#include "Dados/ProblemasDosDados.h"
#include "Dados/Receitas.h"
#include "Dados/Recursos.h"
#include "Dados/TextosERegras.h"
#include "Eventos/DefinicaoDosEventos.h"
#include "Mapa/MapaLocal.h"
#include "Vila/VilaInicial.h"
#include "Viagem/MundoDoJogo.h"
#include "Viagem/RegrasDaViagem.h"

struct DadosDoJogo {
    TabelaDeTextos tabelaDeTextos;
    RegrasDoJogo regras;
    CatalogoDeItens catalogoDeItens;
    CatalogoDeConstrucoes catalogoDeConstrucoes;
    CatalogoDeRecursos catalogoDeRecursos;
    CatalogoDeReceitas catalogoDeReceitas;
    MapaLocal mapaDaVila;  // com os lotes ocupados já bloqueados (FL5-1)
    VilaDoJogo vilaInicial;
    CatalogoDeEventos catalogoDeEventos;
    RegrasDoClima regrasDoClima;  // fase A1 (ficha do Clima)
    MundoDoJogo mundo;            // fase B1 (fichas do Mundo e da Viagem)
    RegrasDaViagem regrasDaViagem;
    ListaDeProblemasDosDados listaDeProblemas;

    bool dadosEstaoProntos() const { return !listaDeProblemas.temErro(); }
};

// Lê tudo a partir da pasta dos dados (Jogo/data). Nunca para no primeiro
// erro: junta todos na listaDeProblemas.
DadosDoJogo carregarDadosDoJogo(const std::string& pastaDosDados);

// "Dados conferidos: 8 itens, ..., 1 mapa, 1 vila (10 construções e 15 NPCs), 11 eventos. 0 erros, 0 avisos."
std::string montarResumoDosDados(const DadosDoJogo& dados);

// Uma linha por item: "Pão: 1,7 moeda". Mostrada ao abrir e no --validar.
std::string montarTabelaDePrecosBase(const DadosDoJogo& dados);

// Dinheiro em décimos para mostrar: 17 vira "1,7".
std::string escreverDinheiroEmMoedas(int valorEmDecimos);
