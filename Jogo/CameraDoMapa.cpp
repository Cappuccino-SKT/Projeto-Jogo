#include "CameraDoMapa.h"

#include <cmath>

namespace {

// 1 ÷ raiz de 2: na diagonal, cada eixo anda isso, para a velocidade total ser a mesma.
constexpr double fatorDaDiagonal = 0.70710678118654752;

// Regra 4.4 para um eixo: devolve a posição dentro do limite.
double limitarUmEixo(double posicaoNoMundo, int tamanhoDaTelaEmPixels, int zoom, int tamanhoDoMapaNoMundo)
{
    const double tamanhoVisivelNoMundo = static_cast<double>(tamanhoDaTelaEmPixels) / zoom;
    if (tamanhoDoMapaNoMundo >= tamanhoVisivelNoMundo) {
        const double posicaoMaxima = tamanhoDoMapaNoMundo - tamanhoVisivelNoMundo;
        if (posicaoNoMundo < 0.0) {
            return 0.0;
        }
        if (posicaoNoMundo > posicaoMaxima) {
            return posicaoMaxima;
        }
        return posicaoNoMundo;
    }
    // O mapa é menor que a tela: fica centralizado e parado.
    return -(tamanhoVisivelNoMundo - tamanhoDoMapaNoMundo) / 2.0;
}

}  // namespace

CameraDoMapa::CameraDoMapa(const RegrasDaCamera& regrasDaCamera)
    : regras(regrasDaCamera), zoom(regrasDaCamera.zoomInicial)
{
}

void CameraDoMapa::atualizarTamanhoDaTela(int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    // Uma janela minimizada pode dizer 0 x 0; a câmera nunca divide por zero.
    larguraDaTela = larguraDaTelaEmPixels > 0 ? larguraDaTelaEmPixels : 1;
    alturaDaTela = alturaDaTelaEmPixels > 0 ? alturaDaTelaEmPixels : 1;
}

void CameraDoMapa::definirTamanhoDoMapa(int larguraDoMapaEmPixelsDoMundo, int alturaDoMapaEmPixelsDoMundo)
{
    larguraDoMapaNoMundo = larguraDoMapaEmPixelsDoMundo;
    alturaDoMapaNoMundo = alturaDoMapaEmPixelsDoMundo;
}

void CameraDoMapa::moverPeloTeclado(int direcaoX, int direcaoY, double segundosReaisDoQuadro, bool correndoComShift)
{
    if (direcaoX == 0 && direcaoY == 0) {
        return;
    }
    // Passo 4: a distância na tela; com Shift, mais rápido.
    double distanciaNaTela = regras.velocidadeNoTecladoEmPixelsDaTelaPorSegundo * segundosReaisDoQuadro;
    if (correndoComShift) {
        distanciaNaTela *= regras.multiplicadorDaVelocidadeComShift;
    }
    // Passo 3: na diagonal, cada eixo anda um pouco menos.
    if (direcaoX != 0 && direcaoY != 0) {
        distanciaNaTela *= fatorDaDiagonal;
    }
    // Passo 5: da tela para o mundo.
    const double distanciaNoMundo = distanciaNaTela / zoom;
    posicaoXNoMundo += direcaoX * distanciaNoMundo;
    posicaoYNoMundo += direcaoY * distanciaNoMundo;
    limitarAoMapa();
}

void CameraDoMapa::arrastarPeloMouse(double deslocamentoXNaTela, double deslocamentoYNaTela)
{
    if (!estaArrastandoComOBotaoDoMeio) {
        return;
    }
    // O mapa acompanha o mouse: a câmera anda ao contrário.
    posicaoXNoMundo -= deslocamentoXNaTela / zoom;
    posicaoYNoMundo -= deslocamentoYNaTela / zoom;
    limitarAoMapa();
}

int CameraDoMapa::indiceDoZoomAtual() const
{
    for (int indice = 0; indice < static_cast<int>(regras.niveisDeZoom.size()); ++indice) {
        if (regras.niveisDeZoom[indice] == zoom) {
            return indice;
        }
    }
    return 0;
}

bool CameraDoMapa::mudarZoomPelaRoda(double quantoARodaGirou, double mouseXNaTela, double mouseYNaTela)
{
    const int ultimoIndice = static_cast<int>(regras.niveisDeZoom.size()) - 1;
    int indiceDoNovoZoom = indiceDoZoomAtual();

    // Passos 1 e 2: a sobra da roda vira níveis inteiros.
    sobraDaRodaDoMouse += quantoARodaGirou;
    while (sobraDaRodaDoMouse >= 1.0) {
        sobraDaRodaDoMouse -= 1.0;
        if (indiceDoNovoZoom < ultimoIndice) {
            ++indiceDoNovoZoom;
        } else {
            sobraDaRodaDoMouse = 0.0;  // já no maior zoom: joga fora o resto
        }
    }
    while (sobraDaRodaDoMouse <= -1.0) {
        sobraDaRodaDoMouse += 1.0;
        if (indiceDoNovoZoom > 0) {
            --indiceDoNovoZoom;
        } else {
            sobraDaRodaDoMouse = 0.0;  // já no menor zoom
        }
    }

    const int novoZoom = regras.niveisDeZoom[indiceDoNovoZoom];
    if (novoZoom == zoom) {
        return false;
    }

    // Passo 3: o ponto do mundo embaixo do mouse continua embaixo dele.
    const double pontoXNoMundoEmbaixoDoMouse = posicaoXNoMundo + mouseXNaTela / zoom;
    const double pontoYNoMundoEmbaixoDoMouse = posicaoYNoMundo + mouseYNaTela / zoom;
    zoom = novoZoom;
    posicaoXNoMundo = pontoXNoMundoEmbaixoDoMouse - mouseXNaTela / zoom;
    posicaoYNoMundo = pontoYNoMundoEmbaixoDoMouse - mouseYNaTela / zoom;

    // Passo 4.
    limitarAoMapa();
    return true;
}

void CameraDoMapa::limitarAoMapa()
{
    posicaoXNoMundo = limitarUmEixo(posicaoXNoMundo, larguraDaTela, zoom, larguraDoMapaNoMundo);
    posicaoYNoMundo = limitarUmEixo(posicaoYNoMundo, alturaDaTela, zoom, alturaDoMapaNoMundo);
}

void CameraDoMapa::centralizarNoMapa()
{
    posicaoXNoMundo = (larguraDoMapaNoMundo - static_cast<double>(larguraDaTela) / zoom) / 2.0;
    posicaoYNoMundo = (alturaDoMapaNoMundo - static_cast<double>(alturaDaTela) / zoom) / 2.0;
    limitarAoMapa();
}

int CameraDoMapa::deslocamentoXNaTela() const
{
    return static_cast<int>(std::lround(posicaoXNoMundo * zoom));
}

int CameraDoMapa::deslocamentoYNaTela() const
{
    return static_cast<int>(std::lround(posicaoYNoMundo * zoom));
}

void CameraDoMapa::posicionarNoMundo(double novaPosicaoXNoMundo, double novaPosicaoYNoMundo)
{
    posicaoXNoMundo = novaPosicaoXNoMundo;
    posicaoYNoMundo = novaPosicaoYNoMundo;
}
