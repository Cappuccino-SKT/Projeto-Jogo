#pragma once

// A busca de caminho A* (regra 4.4 da Ficha Lógica: Mapa, marco 4).
// Acha o caminho mais curto numa grade de 8 direções. Os custos são inteiros,
// em décimos de tile: o passo reto custa 10 e o diagonal, 14 (FT1-3).

#include <cstdint>
#include <vector>

#include "Mapa/MapaLocal.h"

constexpr int custoDoPassoReto = 10;
constexpr int custoDoPassoDiagonal = 14;

struct ResultadoDaBuscaDeCaminho {
    // Sem o tile de saída; termina no destino ou, se não há caminho, no tile
    // mais perto do destino a que dá para chegar (FL4-3).
    std::vector<PosicaoDeTile> tilesDoCaminho;
    bool chegaNoDestino = false;
    int custoTotalEmDecimosDeTile = 0;
    int tilesExaminados = 0;  // só para medir
};

// A estimativa até o destino (parte 5 da ficha): 10 × (dx + dy) − 6 × o menor dos dois.
int estimarCustoAteODestino(PosicaoDeTile tile, PosicaoDeTile tileDeDestino);

// O passo 3.5 da regra 4.4: o vizinho dá para andar e, na diagonal, os dois
// tiles retos do lado também (o NPC não corta quinas, FL4-2).
bool passoEntreVizinhosEhPermitido(const MapaLocal& mapa, PosicaoDeTile tileDeOrigem, PosicaoDeTile tileVizinho);

class BuscadorDeCaminho {
public:
    ResultadoDaBuscaDeCaminho buscarCaminho(const MapaLocal& mapa, PosicaoDeTile tileDeSaida, PosicaoDeTile tileDeDestino);

private:
    // Um tile esperando na fila, com o que decide a ordem (passo 3.1).
    struct TileNaFila {
        int custoEstimadoTotal = 0;
        int estimativaAteODestino = 0;
        int indiceDoTile = 0;
    };

    // A memória de trabalho (parte 2 da ficha), reservada uma vez e
    // reaproveitada. Um número de busca velho quer dizer "ainda não visto".
    std::vector<int> custoAteAquiPorTile;
    std::vector<int> tileAnteriorPorTile;
    std::vector<uint32_t> numeroDaBuscaPorTile;            // visto nesta busca
    std::vector<uint32_t> numeroDaBuscaQueExaminouOTile;   // já saiu da fila nesta busca
    std::vector<TileNaFila> filaDeTilesAbertos;
    uint32_t numeroDaBuscaAtual = 0;

    void prepararMemoriaParaOMapa(const MapaLocal& mapa);
};
