// Os testes da busca de caminho: a tabela "Busca de caminho (marco 4)" da
// parte 8 da Ficha Lógica: Mapa. Usam o mapa de teste de 8 x 8 (a pedra em
// (5, 6)) e o mapa da vila.

#include <cstdio>
#include <cstdlib>
#include <initializer_list>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Mapa/BuscaDeCaminho.h"
#include "Mapa/MapaLocal.h"

namespace {

const std::string caminhoDoMapaDeTeste = std::string(PASTA_DOS_TESTES) + "/mapas/mapa_de_teste_8x8.tmj";
const std::string caminhoDoMapaDaVila = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data/mapas/vila.tmj";

bool caminhoEhIgual(const ResultadoDaBuscaDeCaminho& resultado, std::initializer_list<PosicaoDeTile> tilesEsperados)
{
    if (resultado.tilesDoCaminho.size() != tilesEsperados.size()) {
        return false;
    }
    std::size_t posicao = 0;
    for (const PosicaoDeTile& tileEsperado : tilesEsperados) {
        if (!(resultado.tilesDoCaminho[posicao] == tileEsperado)) {
            return false;
        }
        ++posicao;
    }
    return true;
}

// Cada passo do caminho é entre vizinhos, permitido (sem cortar quina), e o
// primeiro sai do vizinho da saída.
bool todosOsPassosSaoPermitidos(const MapaLocal& mapa, PosicaoDeTile tileDeSaida, const ResultadoDaBuscaDeCaminho& resultado)
{
    PosicaoDeTile tileAnterior = tileDeSaida;
    for (const PosicaoDeTile& tile : resultado.tilesDoCaminho) {
        const bool saoVizinhos = std::abs(tile.tileX - tileAnterior.tileX) <= 1 && std::abs(tile.tileY - tileAnterior.tileY) <= 1;
        if (!saoVizinhos || !passoEntreVizinhosEhPermitido(mapa, tileAnterior, tile)) {
            return false;
        }
        tileAnterior = tile;
    }
    return true;
}

void testarNoMapaDeTeste()
{
    const ResultadoDaLeituraDoMapa leitura = carregarMapaDoArquivoDoTiled(caminhoDoMapaDeTeste);
    conferir(leitura.mapaEstaPronto(), "o mapa de teste carrega para a busca de caminho");
    const MapaLocal& mapa = leitura.mapa;
    BuscadorDeCaminho buscador;

    conferir(estimarCustoAteODestino({0, 0}, {5, 3}) == 62, "a estimativa de (0, 0) a (5, 3) é 62");

    const ResultadoDaBuscaDeCaminho parado = buscador.buscarCaminho(mapa, {0, 0}, {0, 0});
    conferir(parado.chegaNoDestino && parado.tilesDoCaminho.empty(), "de (0, 0) a (0, 0): caminho vazio, chega");

    const ResultadoDaBuscaDeCaminho reto = buscador.buscarCaminho(mapa, {0, 0}, {3, 0});
    conferir(reto.chegaNoDestino && caminhoEhIgual(reto, {{1, 0}, {2, 0}, {3, 0}}) && reto.custoTotalEmDecimosDeTile == 30,
             "de (0, 0) a (3, 0): 3 passos retos, custo 30");

    const ResultadoDaBuscaDeCaminho diagonal = buscador.buscarCaminho(mapa, {0, 0}, {3, 3});
    conferir(diagonal.chegaNoDestino && caminhoEhIgual(diagonal, {{1, 1}, {2, 2}, {3, 3}}) && diagonal.custoTotalEmDecimosDeTile == 42,
             "de (0, 0) a (3, 3): 3 diagonais, custo 42");

    const ResultadoDaBuscaDeCaminho cruzando = buscador.buscarCaminho(mapa, {0, 0}, {7, 7});
    conferir(cruzando.chegaNoDestino && cruzando.custoTotalEmDecimosDeTile == 104, "de (0, 0) a (7, 7): custo 104 (e não 98)");
    conferir(todosOsPassosSaoPermitidos(mapa, {0, 0}, cruzando), "de (0, 0) a (7, 7): nenhum passo corta a quina da pedra");

    const ResultadoDaBuscaDeCaminho contornando = buscador.buscarCaminho(mapa, {4, 6}, {6, 6});
    conferir(contornando.chegaNoDestino && caminhoEhIgual(contornando, {{4, 5}, {5, 5}, {6, 5}, {6, 6}})
                 && contornando.custoTotalEmDecimosDeTile == 40,
             "de (4, 6) a (6, 6): contorna a pedra por cima, custo 40");

    const ResultadoDaBuscaDeCaminho naPedra = buscador.buscarCaminho(mapa, {0, 0}, {5, 6});
    conferir(!naPedra.chegaNoDestino && !naPedra.tilesDoCaminho.empty() && naPedra.tilesDoCaminho.back() == PosicaoDeTile{5, 5}
                 && naPedra.custoTotalEmDecimosDeTile == 70,
             "destino na pedra (5, 6): não chega, e o caminho termina em (5, 5), custo 70 (FL4-3)");

    const ResultadoDaBuscaDeCaminho foraDoMapa = buscador.buscarCaminho(mapa, {0, 0}, {8, 0});
    conferir(!foraDoMapa.chegaNoDestino && foraDoMapa.tilesDoCaminho.empty(), "destino (8, 0), fora do mapa: não chega, caminho vazio");

    const ResultadoDaBuscaDeCaminho deNovo = buscador.buscarCaminho(mapa, {0, 0}, {7, 7});
    conferir(deNovo.tilesDoCaminho == cruzando.tilesDoCaminho && deNovo.custoTotalEmDecimosDeTile == 104,
             "a mesma busca de novo dá o mesmo caminho (a memória reaproveitada não deixa sujeira)");

    conferir(!passoEntreVizinhosEhPermitido(mapa, {4, 6}, {5, 5}), "a diagonal de (4, 6) para (5, 5) corta a quina da pedra: proibida (FL4-2)");
    conferir(passoEntreVizinhosEhPermitido(mapa, {4, 5}, {5, 4}), "a diagonal de (4, 5) para (5, 4), longe da pedra, vale");
}

void testarNaVila()
{
    const ResultadoDaLeituraDoMapa leitura = carregarMapaDoArquivoDoTiled(caminhoDoMapaDaVila);
    conferir(leitura.mapaEstaPronto(), "o mapa da vila carrega para a busca de caminho");
    const MapaLocal& mapa = leitura.mapa;
    BuscadorDeCaminho buscador;

    const ResultadoDaBuscaDeCaminho outroLadoDoLago = buscador.buscarCaminho(mapa, {32, 32}, {60, 50});
    conferir(outroLadoDoLago.chegaNoDestino && outroLadoDoLago.custoTotalEmDecimosDeTile == 370,
             "vila: de (32, 32) a (60, 50), do outro lado do lago, chega com custo 370");
    conferir(todosOsPassosSaoPermitidos(mapa, {32, 32}, outroLadoDoLago), "vila: nenhum tile do caminho é bloqueado");

    const ResultadoDaBuscaDeCaminho dentroDoLago = buscador.buscarCaminho(mapa, {32, 32}, {50, 50});
    conferir(!dentroDoLago.chegaNoDestino && !dentroDoLago.tilesDoCaminho.empty()
                 && dentroDoLago.tilesDoCaminho.back() == PosicaoDeTile{56, 50},
             "vila: destino (50, 50), dentro do lago, termina em (56, 50), a margem mais perto");
}

}  // namespace

void testarBuscaDeCaminho()
{
    std::printf("Busca de caminho\n");
    testarNoMapaDeTeste();
    testarNaVila();
}
