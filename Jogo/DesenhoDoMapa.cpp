#include "DesenhoDoMapa.h"

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <string>

namespace {

// Cor da depuração da colisão: vermelho translúcido.
constexpr float corDaColisaoParaDepuracao[4] = {0.9f, 0.1f, 0.1f, 0.35f};

// Cor da depuração dos caminhos (F2, FL4-6): azul translúcido.
constexpr float corDosCaminhosParaDepuracao[4] = {0.2f, 0.45f, 0.95f, 0.4f};

// As cores provisórias das construções (ficha da IA, regra 4.7), de 0 a 1.
struct CorDaConstrucao {
    const char* tipo;
    float cor[4];
};
constexpr CorDaConstrucao coresDasConstrucoes[] = {
    {"construcao.casa", {0.55f, 0.33f, 0.18f, 1.0f}},           // marrom
    {"construcao.fazenda", {0.85f, 0.75f, 0.25f, 1.0f}},        // amarela
    {"construcao.posto_de_coleta", {0.30f, 0.55f, 0.30f, 1.0f}},// verde
    {"construcao.cozinheiro", {0.90f, 0.55f, 0.20f, 1.0f}},     // laranja
    {"construcao.carpinteiro", {0.62f, 0.45f, 0.30f, 1.0f}},    // castanha
    {"construcao.mercado", {0.25f, 0.45f, 0.80f, 1.0f}},        // azul
    {"construcao.construtor", {0.55f, 0.55f, 0.58f, 1.0f}},     // cinza
};
constexpr float corDeConstrucaoSemCor[4] = {0.8f, 0.2f, 0.8f, 1.0f};

const float* corDoTipoDeConstrucao(const std::string& tipo)
{
    for (const CorDaConstrucao& corDaConstrucao : coresDasConstrucoes) {
        if (tipo == corDaConstrucao.tipo) {
            return corDaConstrucao.cor;
        }
    }
    return corDeConstrucaoSemCor;
}

// Quais tiles aparecem na tela (com a câmera e o zoom), já presos ao mapa.
struct FaixaDeTilesVisiveis {
    int primeiraColuna = 0;
    int ultimaColuna = -1;
    int primeiraLinha = 0;
    int ultimaLinha = -1;
};

// Divisão que arredonda para baixo também nos negativos (-1 ÷ 48 dá -1, não 0).
int dividirArredondandoParaBaixo(int dividendo, int divisor)
{
    return dividendo >= 0 ? dividendo / divisor : -((-dividendo + divisor - 1) / divisor);
}

FaixaDeTilesVisiveis calcularTilesVisiveis(const MapaLocal& mapa, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    // O lado de um tile na tela, por exemplo 16 x 3 = 48 pixels.
    const int ladoDoTileNaTela = mapa.tamanhoDoTileEmPixels * camera.zoomAtual();
    const int deslocamentoX = camera.deslocamentoXNaTela();
    const int deslocamentoY = camera.deslocamentoYNaTela();
    FaixaDeTilesVisiveis faixa;
    faixa.primeiraColuna = std::max(0, dividirArredondandoParaBaixo(deslocamentoX, ladoDoTileNaTela));
    faixa.primeiraLinha = std::max(0, dividirArredondandoParaBaixo(deslocamentoY, ladoDoTileNaTela));
    faixa.ultimaColuna = std::min(mapa.larguraEmTiles - 1, dividirArredondandoParaBaixo(deslocamentoX + larguraDaTelaEmPixels, ladoDoTileNaTela));
    faixa.ultimaLinha = std::min(mapa.alturaEmTiles - 1, dividirArredondandoParaBaixo(deslocamentoY + alturaDaTelaEmPixels, ladoDoTileNaTela));
    return faixa;
}

}  // namespace

bool DesenhistaDoMapa::preparar(const MapaLocal& mapa, std::string& mensagemDeErro)
{
    // 1. O pincel (o programa da placa de vídeo).
    if (!pincel.preparar(mensagemDeErro)) {
        return false;
    }
    // 2. As imagens dos conjuntos de tiles.
    for (const ConjuntoDeTiles& conjunto : mapa.conjuntosDeTiles) {
        TexturaNaPlacaDeVideo textura;
        if (!carregarImagemNaPlacaDeVideo(conjunto.caminhoDaImagem, textura, mensagemDeErro)) {
            return false;
        }
        texturasDosConjuntos.push_back(textura);
    }
    return true;
}

void DesenhistaDoMapa::desenharCamadaPeloNome(const MapaLocal& mapa, const char* nomeDaCamada, const CameraDoMapa& camera,
                                              int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    const CamadaDeTiles* camada = mapa.procurarCamadaPeloNome(nomeDaCamada);
    if (camada == nullptr) {
        return;
    }
    const FaixaDeTilesVisiveis faixa = calcularTilesVisiveis(mapa, camera, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    const int tamanhoDoTile = mapa.tamanhoDoTileEmPixels;

    // Um lote por conjunto de tiles: todos os tiles de um conjunto numa chamada só.
    for (std::size_t indiceDoConjunto = 0; indiceDoConjunto < mapa.conjuntosDeTiles.size(); ++indiceDoConjunto) {
        const ConjuntoDeTiles& conjunto = mapa.conjuntosDeTiles[indiceDoConjunto];
        pincel.comecarLoteComImagem(texturasDosConjuntos[indiceDoConjunto]);
        for (int tileY = faixa.primeiraLinha; tileY <= faixa.ultimaLinha; ++tileY) {
            for (int tileX = faixa.primeiraColuna; tileX <= faixa.ultimaColuna; ++tileX) {
                const int numeroDoTile = camada->numeroDoTilePorPosicao[mapa.indiceDoTileNaLista(tileX, tileY)];
                if (numeroDoTile == 0 || mapa.procurarConjuntoDoTile(numeroDoTile) != &conjunto) {
                    continue;  // vazio, ou de outro conjunto
                }
                // Do tile à tela: tile x 16 x zoom - deslocamento (ficha da Interface, regra 4.6).
                const float ladoNaTela = static_cast<float>(tamanhoDoTile * camera.zoomAtual());
                const float esquerda = tileX * ladoNaTela - camera.deslocamentoXNaTela();
                const float topo = tileY * ladoNaTela - camera.deslocamentoYNaTela();
                // Do número do tile ao recorte na imagem.
                const RecorteNaImagem recorte = calcularRecorteDoTileNaImagem(conjunto, numeroDoTile, tamanhoDoTile);
                pincel.acrescentarQuadrado(esquerda, topo, esquerda + ladoNaTela, topo + ladoNaTela, static_cast<float>(recorte.pixelX),
                                           static_cast<float>(recorte.pixelY), static_cast<float>(recorte.larguraEmPixels),
                                           static_cast<float>(recorte.alturaEmPixels));
            }
        }
        pincel.mandarLoteParaAPlacaDeVideo();
    }
}

void DesenhistaDoMapa::desenharCamadasAbaixoDosPersonagens(const MapaLocal& mapa, const CameraDoMapa& camera,
                                                           int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    pincel.comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    for (const char* nomeDaCamada : {"chao", "detalhes_do_chao", "objetos_baixos", "construcoes_e_paredes"}) {
        desenharCamadaPeloNome(mapa, nomeDaCamada, camera, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    }
}

void DesenhistaDoMapa::desenharCamadasAcimaDosPersonagens(const MapaLocal& mapa, const CameraDoMapa& camera,
                                                          int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    pincel.comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    desenharCamadaPeloNome(mapa, "acima_do_personagem", camera, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
}

void DesenhistaDoMapa::desenharTilesBloqueadosParaDepuracao(const MapaLocal& mapa, const CameraDoMapa& camera,
                                                            int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    pincel.comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    pincel.comecarLoteComCorLisa(corDaColisaoParaDepuracao);

    const FaixaDeTilesVisiveis faixa = calcularTilesVisiveis(mapa, camera, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    const int tamanhoDoTile = mapa.tamanhoDoTileEmPixels;
    for (int tileY = faixa.primeiraLinha; tileY <= faixa.ultimaLinha; ++tileY) {
        for (int tileX = faixa.primeiraColuna; tileX <= faixa.ultimaColuna; ++tileX) {
            if (mapa.tileDaParaAndar(tileX, tileY)) {
                continue;
            }
            const float ladoNaTela = static_cast<float>(tamanhoDoTile * camera.zoomAtual());
            const float esquerda = tileX * ladoNaTela - camera.deslocamentoXNaTela();
            const float topo = tileY * ladoNaTela - camera.deslocamentoYNaTela();
            pincel.acrescentarQuadrado(esquerda, topo, esquerda + ladoNaTela, topo + ladoNaTela, 0, 0, 0, 0);
        }
    }
    pincel.mandarLoteParaAPlacaDeVideo();
}

void DesenhistaDoMapa::desenharTilesDosCaminhosParaDepuracao(const std::vector<PosicaoDeTile>& tilesDosCaminhos, const MapaLocal& mapa,
                                                             const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    pincel.comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    pincel.comecarLoteComCorLisa(corDosCaminhosParaDepuracao);
    const float ladoNaTela = static_cast<float>(mapa.tamanhoDoTileEmPixels * camera.zoomAtual());
    for (const PosicaoDeTile& tile : tilesDosCaminhos) {
        const float esquerda = tile.tileX * ladoNaTela - camera.deslocamentoXNaTela();
        const float topo = tile.tileY * ladoNaTela - camera.deslocamentoYNaTela();
        pincel.acrescentarQuadrado(esquerda, topo, esquerda + ladoNaTela, topo + ladoNaTela, 0, 0, 0, 0);
    }
    pincel.mandarLoteParaAPlacaDeVideo();
}

void DesenhistaDoMapa::desenharConstrucoesProvisorias(const VilaDoJogo& vila, const MapaLocal& mapa, const CameraDoMapa& camera,
                                                      int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    pincel.comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    const float ladoDoTileNaTela = static_cast<float>(mapa.tamanhoDoTileEmPixels * camera.zoomAtual());
    for (const ConstrucaoDaVila& construcao : vila.construcoes) {
        const RetanguloEmTiles& area = construcao.areaDoLote;
        const float esquerda = area.tileX * ladoDoTileNaTela - camera.deslocamentoXNaTela();
        const float topo = area.tileY * ladoDoTileNaTela - camera.deslocamentoYNaTela();
        const float direita = esquerda + area.larguraEmTiles * ladoDoTileNaTela;
        const float baixo = topo + area.alturaEmTiles * ladoDoTileNaTela;

        // O corpo da construção, na cor do tipo.
        const float* cor = corDoTipoDeConstrucao(construcao.tipo);
        pincel.comecarLoteComCorLisa(cor);
        pincel.acrescentarQuadrado(esquerda, topo, direita, baixo, 0, 0, 0, 0);
        pincel.mandarLoteParaAPlacaDeVideo();

        // A porta: o tile da parede de baixo, logo acima do tile da porta, mais escuro.
        const float corDaPorta[4] = {cor[0] * 0.45f, cor[1] * 0.45f, cor[2] * 0.45f, 1.0f};
        const float esquerdaDaPorta = construcao.tileDaPorta.tileX * ladoDoTileNaTela - camera.deslocamentoXNaTela();
        pincel.comecarLoteComCorLisa(corDaPorta);
        pincel.acrescentarQuadrado(esquerdaDaPorta + ladoDoTileNaTela * 0.2f, baixo - ladoDoTileNaTela * 0.8f,
                                   esquerdaDaPorta + ladoDoTileNaTela * 0.8f, baixo, 0, 0, 0, 0);
        pincel.mandarLoteParaAPlacaDeVideo();
    }
}

void DesenhistaDoMapa::liberar()
{
    for (TexturaNaPlacaDeVideo& textura : texturasDosConjuntos) {
        liberarTexturaDaPlacaDeVideo(textura);
    }
    texturasDosConjuntos.clear();
    pincel.liberar();
}
