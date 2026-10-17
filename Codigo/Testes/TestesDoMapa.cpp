// Os testes da leitura do mapa: a tabela da parte 8 da Ficha Lógica: Mapa.
//
// Usam o mapa pequeno Codigo/Testes/mapas/mapa_de_teste_8x8.tmj (grama, uma
// pedra com colisão em (5, 6), um lote pequeno em (0, 0) e um poço) e versões
// dele com um defeito de propósito, montadas aqui mesmo.

#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>

#include <nlohmann/json.hpp>

#include "ConferenciaDosTestes.h"
#include "Mapa/MapaLocal.h"

namespace {

using Json = nlohmann::json;

const std::string pastaDosMapasDeTeste = std::string(PASTA_DOS_TESTES) + "/mapas";
const std::string caminhoDoMapaDeTeste = pastaDosMapasDeTeste + "/mapa_de_teste_8x8.tmj";
const std::string caminhoDoMapaDaVila = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data/mapas/vila.tmj";

Json lerMapaDeTesteComoJson()
{
    std::ifstream arquivo(caminhoDoMapaDeTeste, std::ios::binary);
    const std::string texto((std::istreambuf_iterator<char>(arquivo)), std::istreambuf_iterator<char>());
    return Json::parse(texto);
}

// Acha uma camada do mapa de teste pelo nome, para estragar de propósito.
Json& camadaDoJson(Json& mapaEmJson, const std::string& nomeDaCamada)
{
    for (Json& camada : mapaEmJson["layers"]) {
        if (camada["name"] == nomeDaCamada) {
            return camada;
        }
    }
    std::printf("  (o mapa de teste não tem a camada %s)\n", nomeDaCamada.c_str());
    return mapaEmJson["layers"][0];
}

void removerCamadaDoJson(Json& mapaEmJson, const std::string& nomeDaCamada)
{
    Json& camadas = mapaEmJson["layers"];
    for (std::size_t indice = 0; indice < camadas.size(); ++indice) {
        if (camadas[indice]["name"] == nomeDaCamada) {
            camadas.erase(indice);
            return;
        }
    }
}

ResultadoDaLeituraDoMapa lerDoJson(const Json& mapaEmJson)
{
    return carregarMapaDoTextoDoTiled(mapaEmJson.dump(), pastaDosMapasDeTeste);
}

bool algumErroContem(const ResultadoDaLeituraDoMapa& resultado, const std::string& trecho)
{
    for (const std::string& erro : resultado.listaDeErrosDoMapa) {
        if (erro.find(trecho) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void mostrarErrosSeHouver(const ResultadoDaLeituraDoMapa& resultado)
{
    for (const std::string& erro : resultado.listaDeErrosDoMapa) {
        std::printf("    erro do mapa: %s\n", erro.c_str());
    }
}

void testarMapaCorreto()
{
    const ResultadoDaLeituraDoMapa resultado = carregarMapaDoArquivoDoTiled(caminhoDoMapaDeTeste);
    conferir(resultado.mapaEstaPronto(), "o mapa de teste 8 x 8 carrega sem erros");
    mostrarErrosSeHouver(resultado);
    conferir(resultado.mapa.camadasDeTiles.size() == 6, "o mapa de teste tem 6 camadas de tiles");
    conferir(resultado.mapa.larguraEmTiles == 8 && resultado.mapa.alturaEmTiles == 8, "o mapa de teste tem 8 x 8 tiles");
    conferir(!resultado.mapa.tileDaParaAndar(5, 6), "a colisão em (5, 6) bloqueia o tile");
    conferir(resultado.mapa.tileDaParaAndar(5, 5), "o tile (5, 5) dá para andar");
    conferir(!resultado.mapa.tileDaParaAndar(-1, 0) && !resultado.mapa.tileDaParaAndar(8, 0), "fora do mapa não dá para andar");
    conferir(resultado.mapa.lotesDeConstrucao.size() == 1
                 && resultado.mapa.lotesDeConstrucao[0].porteDoLote == PorteDoLote::Pequeno
                 && resultado.mapa.lotesDeConstrucao[0].tipoDeConstrucaoPermitido == "qualquer",
             "o mapa de teste tem 1 lote pequeno, de tipo qualquer");
    conferir(resultado.mapa.areasDeRecurso.size() == 1 && resultado.mapa.areasDeRecurso[0].tipoDeRecurso == "poco",
             "o mapa de teste tem 1 área de recurso: o poço");
}

void testarMapasComDefeito()
{
    {
        Json mapaEmJson = lerMapaDeTesteComoJson();
        removerCamadaDoJson(mapaEmJson, "colisao");
        conferir(algumErroContem(lerDoJson(mapaEmJson), "falta a camada `colisao`"), "sem a camada colisao, dá o erro \"falta a camada\"");
    }
    {
        Json mapaEmJson = lerMapaDeTesteComoJson();
        camadaDoJson(mapaEmJson, "chao")["data"].erase(0);  // fica com 63 números
        conferir(algumErroContem(lerDoJson(mapaEmJson), "tem 63 números"), "uma camada com 63 números num mapa 8 x 8 dá erro de tamanho");
    }
    {
        Json mapaEmJson = lerMapaDeTesteComoJson();
        camadaDoJson(mapaEmJson, "chao")["data"][5 * 8 + 2] = 1u | 0x80000000u;  // espelhado na horizontal
        conferir(algumErroContem(lerDoJson(mapaEmJson), "tile espelhado em (2, 5)"), "um tile espelhado em (2, 5) dá erro");
    }
    {
        Json mapaEmJson = lerMapaDeTesteComoJson();
        camadaDoJson(mapaEmJson, "chao")["data"][0] = 20;  // o conjunto só tem 16 tiles
        conferir(algumErroContem(lerDoJson(mapaEmJson), "tile desconhecido em (0, 0)"), "um tile 20 num conjunto de 16 dá erro");
    }
    {
        Json mapaEmJson = lerMapaDeTesteComoJson();
        removerCamadaDoJson(mapaEmJson, "colisao");
        camadaDoJson(mapaEmJson, "chao")["data"][5 * 8 + 2] = 1u | 0x80000000u;
        camadaDoJson(mapaEmJson, "chao")["data"][0] = 20;
        conferir(lerDoJson(mapaEmJson).listaDeErrosDoMapa.size() == 3, "um mapa com 3 erros traz os 3 na lista");
    }
    {
        Json mapaEmJson = lerMapaDeTesteComoJson();
        camadaDoJson(mapaEmJson, "lotes")["objects"][0]["width"] = 80;
        conferir(algumErroContem(lerDoJson(mapaEmJson), "o tamanho precisa ser"), "um lote de 80 x 64 pixels dá erro de tamanho");
    }
    {
        Json mapaEmJson = lerMapaDeTesteComoJson();
        Json segundoLote = camadaDoJson(mapaEmJson, "lotes")["objects"][0];
        segundoLote["id"] = 9;
        segundoLote["x"] = 32;
        segundoLote["y"] = 32;
        camadaDoJson(mapaEmJson, "lotes")["objects"].push_back(segundoLote);
        conferir(algumErroContem(lerDoJson(mapaEmJson), "fica por cima do lote"), "dois lotes que se cobrem dão erro");
    }
    {
        Json mapaEmJson = lerMapaDeTesteComoJson();
        camadaDoJson(mapaEmJson, "lotes")["objects"][0]["x"] = 64;  // tiles 4 a 7: pega a pedra em (5, 6)
        camadaDoJson(mapaEmJson, "lotes")["objects"][0]["y"] = 64;
        conferir(algumErroContem(lerDoJson(mapaEmJson), "tile bloqueado na colisao em (5, 6)"), "um lote sobre um tile de colisão dá erro");
    }
}

void testarContasDoMapa()
{
    MapaLocal mapaDe64Colunas;
    mapaDe64Colunas.larguraEmTiles = 64;
    conferir(mapaDe64Colunas.indiceDoTileNaLista(10, 3) == 202, "o tile (10, 3) num mapa de 64 de largura fica no índice 202");

    ConjuntoDeTiles conjunto;
    conjunto.primeiroNumeroDoConjunto = 1;
    conjunto.colunasNaImagem = 16;
    conjunto.quantidadeDeTiles = 256;
    const RecorteNaImagem recorte = calcularRecorteDoTileNaImagem(conjunto, 35, 16);
    conferir(recorte.pixelX == 32 && recorte.pixelY == 32, "o tile 35, num conjunto de 16 colunas, é recortado em (32, 32)");

    RetanguloEmTiles retangulo;
    PorteDoLote porte = PorteDoLote::Grande;
    const bool converteu = converterRetanguloDePixelsParaTiles(192, 320, 64, 64, 16, retangulo);
    conferir(converteu && retangulo.tileX == 12 && retangulo.tileY == 20, "um lote em (192, 320) pixels fica no tile (12, 20)");
    conferir(descobrirPorteDoLote(retangulo.larguraEmTiles, retangulo.alturaEmTiles, porte) && porte == PorteDoLote::Pequeno,
             "um lote de 64 x 64 pixels é pequeno");
}

void testarMapaDaVila()
{
    const ResultadoDaLeituraDoMapa resultado = carregarMapaDoArquivoDoTiled(caminhoDoMapaDaVila);
    conferir(resultado.mapaEstaPronto(), "o vila.tmj carrega sem erros");
    mostrarErrosSeHouver(resultado);
    conferir(resultado.mapa.larguraEmTiles == 64 && resultado.mapa.alturaEmTiles == 64, "o vila.tmj tem 64 x 64 tiles");

    int lotesPequenos = 0;
    int lotesMedios = 0;
    for (const LoteDeConstrucao& lote : resultado.mapa.lotesDeConstrucao) {
        lotesPequenos += lote.porteDoLote == PorteDoLote::Pequeno ? 1 : 0;
        lotesMedios += lote.porteDoLote == PorteDoLote::Medio ? 1 : 0;
    }
    conferir(lotesPequenos == 8 && lotesMedios == 3, "o vila.tmj tem 8 lotes pequenos e 3 médios");
    conferir(resultado.mapa.areasDeRecurso.size() == 3, "o vila.tmj tem 3 áreas de recurso");
}

}  // namespace

void testarMapaLocal()
{
    std::printf("Mapa Local\n");
    testarMapaCorreto();
    testarMapasComDefeito();
    testarContasDoMapa();
    testarMapaDaVila();
}
