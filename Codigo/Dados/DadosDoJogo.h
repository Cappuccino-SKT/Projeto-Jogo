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
#include "Cidades/RegrasDasOutrasCidades.h"
#include "Comercio/RegrasDoComercio.h"
#include "Noticias/Noticias.h"
#include "Reputacao/Reputacao.h"
#include "Tecnologia/Tecnologia.h"
#include "Faccao/Faccao.h"
#include "Cultura/Cultura.h"
#include "Crime/Crime.h"
#include "Religiao/Religiao.h"
#include "Combate/Combate.h"
#include "Companheiros/Companheiros.h"
#include "Escravidao/Escravidao.h"

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
    RegrasDasOutrasCidades regrasDasOutrasCidades;  // fase B1b
    RegrasDoComercio regrasDoComercio;              // fase B2
    RegrasDasNoticias regrasDasNoticias;            // fase B3
    RegrasDaReputacao regrasDaReputacao;            // fase B4
    CatalogoDaTecnologia tecnologia;                // fase B5
    CatalogoDasFaccoes faccoes;                     // fase C1
    CatalogoDasCulturas culturas;                   // fase C2
    RegrasDoCrime regrasDoCrime;                    // fase C3
    RegrasDaReligiao regrasDaReligiao;              // fase C4
    RegrasDoCombate regrasDoCombate;                // fase D1
    RegrasDaEscravidao regrasDaEscravidao;          // fase D3
    RegrasDaFamilia regrasDaFamilia;                // fase D2 (o companheiros.json)
    RegrasDosCompanheiros regrasDosCompanheiros;    // fase D2 (o mesmo arquivo)
    ListaDeProblemasDosDados listaDeProblemas;

    bool dadosEstaoProntos() const { return !listaDeProblemas.temErro(); }
};

// Lê tudo a partir da pasta dos dados (Jogo/data). Nunca para no primeiro
// erro: junta todos na listaDeProblemas.
DadosDoJogo carregarDadosDoJogo(const std::string& pastaDosDados);

// Facções modulares (ficha da Facção, rodada HI13-1). Depois de ler o faccoes.json:
// 1. a cidade que aponta para uma facção que não existe vira cidade sem facção (terra sem
//    dono, com as regras das Vilas Livres), com um aviso no lugar de um erro;
// 2. o nome e o artigo de cada facção ("do Reino Bárbaro") passam para a Reputação, que é
//    quem escreve os nomes na tela.
void ajustarOMundoAsFaccoesQueExistem(DadosDoJogo& dados, ListaDeProblemasDosDados& listaDeProblemas);

// "Dados conferidos: 8 itens, ..., 1 mapa, 1 vila (10 construções e 15 NPCs), 11 eventos. 0 erros, 0 avisos."
std::string montarResumoDosDados(const DadosDoJogo& dados);

// Uma linha por item: "Pão: 1,7 moeda". Mostrada ao abrir e no --validar.
std::string montarTabelaDePrecosBase(const DadosDoJogo& dados);

// Dinheiro em décimos para mostrar: 17 vira "1,7".
std::string escreverDinheiroEmMoedas(int valorEmDecimos);
