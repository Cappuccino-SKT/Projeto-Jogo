#include "Mapa/BuscaDeCaminho.h"

#include <algorithm>
#include <cstdlib>
#include <tuple>

namespace {

// Os 8 vizinhos, sempre nesta ordem (passo 3.5): cima, direita, baixo,
// esquerda e as diagonais. A ordem fixa faz a busca dar sempre o mesmo caminho.
constexpr int passoXDoVizinho[8] = {0, 1, 0, -1, 1, 1, -1, -1};
constexpr int passoYDoVizinho[8] = {-1, 0, 1, 0, -1, 1, 1, -1};

}  // namespace

int estimarCustoAteODestino(PosicaoDeTile tile, PosicaoDeTile tileDeDestino)
{
    const int distanciaEmX = std::abs(tile.tileX - tileDeDestino.tileX);
    const int distanciaEmY = std::abs(tile.tileY - tileDeDestino.tileY);
    // Anda-se na diagonal enquanto dá (14 cada) e reto no resto (10 cada).
    return custoDoPassoReto * (distanciaEmX + distanciaEmY)
        + (custoDoPassoDiagonal - 2 * custoDoPassoReto) * std::min(distanciaEmX, distanciaEmY);
}

bool passoEntreVizinhosEhPermitido(const MapaLocal& mapa, PosicaoDeTile tileDeOrigem, PosicaoDeTile tileVizinho)
{
    if (!mapa.tileDaParaAndar(tileVizinho.tileX, tileVizinho.tileY)) {
        return false;
    }
    const int passoX = tileVizinho.tileX - tileDeOrigem.tileX;
    const int passoY = tileVizinho.tileY - tileDeOrigem.tileY;
    const bool passoEhDiagonal = passoX != 0 && passoY != 0;
    if (passoEhDiagonal) {
        // FL4-2: os dois tiles retos do lado precisam estar livres.
        return mapa.tileDaParaAndar(tileDeOrigem.tileX + passoX, tileDeOrigem.tileY)
            && mapa.tileDaParaAndar(tileDeOrigem.tileX, tileDeOrigem.tileY + passoY);
    }
    return true;
}

void BuscadorDeCaminho::prepararMemoriaParaOMapa(const MapaLocal& mapa)
{
    const std::size_t quantidadeDeTiles = static_cast<std::size_t>(mapa.larguraEmTiles) * mapa.alturaEmTiles;
    if (custoAteAquiPorTile.size() != quantidadeDeTiles) {
        custoAteAquiPorTile.assign(quantidadeDeTiles, 0);
        tileAnteriorPorTile.assign(quantidadeDeTiles, -1);
        numeroDaBuscaPorTile.assign(quantidadeDeTiles, 0);
        numeroDaBuscaQueExaminouOTile.assign(quantidadeDeTiles, 0);
        numeroDaBuscaAtual = 0;
    }
    ++numeroDaBuscaAtual;
    // Depois de 4 bilhões de buscas o número dá a volta: aí sim apaga as listas.
    if (numeroDaBuscaAtual == 0) {
        std::fill(numeroDaBuscaPorTile.begin(), numeroDaBuscaPorTile.end(), 0);
        std::fill(numeroDaBuscaQueExaminouOTile.begin(), numeroDaBuscaQueExaminouOTile.end(), 0);
        numeroDaBuscaAtual = 1;
    }
    filaDeTilesAbertos.clear();  // apaga os itens, mas guarda a memória
}

ResultadoDaBuscaDeCaminho BuscadorDeCaminho::buscarCaminho(const MapaLocal& mapa, PosicaoDeTile tileDeSaida, PosicaoDeTile tileDeDestino)
{
    ResultadoDaBuscaDeCaminho resultado;

    // Passo 1: os casos rápidos.
    if (!mapa.tileEstaDentroDoMapa(tileDeDestino.tileX, tileDeDestino.tileY)
        || !mapa.tileEstaDentroDoMapa(tileDeSaida.tileX, tileDeSaida.tileY)) {
        return resultado;
    }
    if (tileDeSaida == tileDeDestino) {
        resultado.chegaNoDestino = true;
        return resultado;
    }

    // Passo 2: prepara a memória e põe a saída na fila.
    prepararMemoriaParaOMapa(mapa);
    const int indiceDaSaida = mapa.indiceDoTileNaLista(tileDeSaida.tileX, tileDeSaida.tileY);
    const int indiceDoDestino = mapa.indiceDoTileNaLista(tileDeDestino.tileX, tileDeDestino.tileY);
    custoAteAquiPorTile[indiceDaSaida] = 0;
    tileAnteriorPorTile[indiceDaSaida] = -1;
    numeroDaBuscaPorTile[indiceDaSaida] = numeroDaBuscaAtual;

    // A fila com prioridade: o "menor" fica na frente. Como o std::push_heap
    // põe o maior na frente, a comparação é ao contrário.
    const auto vemDepoisNaFila = [](const TileNaFila& primeiro, const TileNaFila& segundo) {
        return std::tie(primeiro.custoEstimadoTotal, primeiro.estimativaAteODestino, primeiro.indiceDoTile)
             > std::tie(segundo.custoEstimadoTotal, segundo.estimativaAteODestino, segundo.indiceDoTile);
    };
    const int estimativaDaSaida = estimarCustoAteODestino(tileDeSaida, tileDeDestino);
    filaDeTilesAbertos.push_back({estimativaDaSaida, estimativaDaSaida, indiceDaSaida});

    // O tile mais perto do destino entre os examinados (para a FL4-3).
    int indiceDoTileMaisPerto = indiceDaSaida;
    int estimativaDoTileMaisPerto = estimativaDaSaida;
    bool achouODestino = false;

    // Passo 3: examina os tiles, o de menor custo estimado primeiro.
    while (!filaDeTilesAbertos.empty()) {
        std::pop_heap(filaDeTilesAbertos.begin(), filaDeTilesAbertos.end(), vemDepoisNaFila);
        const TileNaFila tileDaVez = filaDeTilesAbertos.back();
        filaDeTilesAbertos.pop_back();

        const int indiceDaVez = tileDaVez.indiceDoTile;
        if (numeroDaBuscaQueExaminouOTile[indiceDaVez] == numeroDaBuscaAtual) {
            continue;  // 3.2: já examinado (uma cópia mais cara que ficou na fila)
        }
        numeroDaBuscaQueExaminouOTile[indiceDaVez] = numeroDaBuscaAtual;
        ++resultado.tilesExaminados;

        // 3.3: guarda o tile mais perto (no empate, o de menor custo; depois, o de menor índice).
        const int custoDaVez = custoAteAquiPorTile[indiceDaVez];
        if (std::tie(tileDaVez.estimativaAteODestino, custoDaVez, indiceDaVez)
            < std::tie(estimativaDoTileMaisPerto, custoAteAquiPorTile[indiceDoTileMaisPerto], indiceDoTileMaisPerto)) {
            indiceDoTileMaisPerto = indiceDaVez;
            estimativaDoTileMaisPerto = tileDaVez.estimativaAteODestino;
        }

        // 3.4: chegou.
        if (indiceDaVez == indiceDoDestino) {
            achouODestino = true;
            break;
        }

        // 3.5 e 3.6: os 8 vizinhos.
        const PosicaoDeTile posicaoDaVez{indiceDaVez % mapa.larguraEmTiles, indiceDaVez / mapa.larguraEmTiles};
        for (int vizinho = 0; vizinho < 8; ++vizinho) {
            const PosicaoDeTile posicaoDoVizinho{posicaoDaVez.tileX + passoXDoVizinho[vizinho],
                                                 posicaoDaVez.tileY + passoYDoVizinho[vizinho]};
            if (!passoEntreVizinhosEhPermitido(mapa, posicaoDaVez, posicaoDoVizinho)) {
                continue;
            }
            const bool passoEhDiagonal = passoXDoVizinho[vizinho] != 0 && passoYDoVizinho[vizinho] != 0;
            const int custoNovo = custoDaVez + (passoEhDiagonal ? custoDoPassoDiagonal : custoDoPassoReto);
            const int indiceDoVizinho = mapa.indiceDoTileNaLista(posicaoDoVizinho.tileX, posicaoDoVizinho.tileY);
            const bool vizinhoAindaNaoFoiVisto = numeroDaBuscaPorTile[indiceDoVizinho] != numeroDaBuscaAtual;
            if (vizinhoAindaNaoFoiVisto || custoNovo < custoAteAquiPorTile[indiceDoVizinho]) {
                numeroDaBuscaPorTile[indiceDoVizinho] = numeroDaBuscaAtual;
                custoAteAquiPorTile[indiceDoVizinho] = custoNovo;
                tileAnteriorPorTile[indiceDoVizinho] = indiceDaVez;
                const int estimativaDoVizinho = estimarCustoAteODestino(posicaoDoVizinho, tileDeDestino);
                filaDeTilesAbertos.push_back({custoNovo + estimativaDoVizinho, estimativaDoVizinho, indiceDoVizinho});
                std::push_heap(filaDeTilesAbertos.begin(), filaDeTilesAbertos.end(), vemDepoisNaFila);
            }
        }
    }

    // Passo 4: monta o caminho de trás para a frente e inverte.
    const int indiceDoFimDoCaminho = achouODestino ? indiceDoDestino : indiceDoTileMaisPerto;
    for (int indice = indiceDoFimDoCaminho; indice != indiceDaSaida; indice = tileAnteriorPorTile[indice]) {
        resultado.tilesDoCaminho.push_back({indice % mapa.larguraEmTiles, indice / mapa.larguraEmTiles});
    }
    std::reverse(resultado.tilesDoCaminho.begin(), resultado.tilesDoCaminho.end());

    // Passo 5.
    resultado.chegaNoDestino = achouODestino;
    resultado.custoTotalEmDecimosDeTile = custoAteAquiPorTile[indiceDoFimDoCaminho];
    return resultado;
}
