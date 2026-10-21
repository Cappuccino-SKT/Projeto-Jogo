#pragma once

// O Mapa Mundial (Ficha Lógica: Mundo e Estado Inicial e Viagem, fase B1): os
// 48 mapas da malha 8 × 6, lidos do data/mundo/mapas.json, e as cidades do
// primeiro dia, do data/mundo/cidades_iniciais.json. O estado do território
// não fica no arquivo: sai das cidades vizinhas (Facção, regra 4.1; FA1-6).

#include <string>
#include <vector>

#include "Dados/ProblemasDosDados.h"

enum class EstadoDoTerritorio { ComCidade, Territorio, EmDisputa, SemDono };

struct MapaDoMundo {
    int numeroDoMapa = 1;  // (linha − 1) × colunas + coluna; a ordem dos desempates
    int coluna = 1;
    int linha = 1;
    int regiao = 1;  // de 1 a 6
    std::string bioma = "temperado";
    std::string terreno = "planicie";
    std::string infraestrutura = "selvagem";
    std::string arquivoTiled;  // vazio = sem desenho: só se atravessa pelo Mapa Mundial (VI1-8)
};

struct CidadeDoMundo {
    std::string nome;
    int numeroDoMapa = 1;
    int nivel = 1;
    std::string faccao;  // vazio = sem facção (as Vilas Livres)
    bool ehAVilaDoJogador = false;
};

struct MundoDoJogo {
    int colunas = 8;
    int linhas = 6;
    std::vector<MapaDoMundo> mapas;  // na ordem do número do mapa: mapas[numero − 1]
    std::vector<CidadeDoMundo> cidades;
    int mapaDaVila = 0;  // o mapa da vila da fatia; 0 se o mundo não foi lido

    bool mapaExiste(int numeroDoMapa) const { return numeroDoMapa >= 1 && numeroDoMapa <= static_cast<int>(mapas.size()); }
    const MapaDoMundo& mapa(int numeroDoMapa) const { return mapas[numeroDoMapa - 1]; }
    int numeroDoMapaNa(int coluna, int linha) const { return (linha - 1) * colunas + coluna; }
    // Os vizinhos dos 4 lados, na ordem de cima, esquerda, direita e baixo (a malha não dá a volta).
    std::vector<int> vizinhos(int numeroDoMapa) const;
    // A cidade do mapa, ou nenhuma.
    const CidadeDoMundo* cidadeNoMapa(int numeroDoMapa) const;
    // Facção, regra 4.1.
    EstadoDoTerritorio estadoDoTerritorio(int numeroDoMapa) const;
    // "(4,3) Vila Livre do Centro" ou "(5,3)".
    std::string nomeDoMapa(int numeroDoMapa) const;
};

// Lê os dois arquivos. O mapa da vila é o da cidade marcada com "vila_do_jogador".
MundoDoJogo lerArquivosDoMundo(const std::string& textoDosMapas, const std::string& textoDasCidades, ListaDeProblemasDosDados& listaDeProblemas);
