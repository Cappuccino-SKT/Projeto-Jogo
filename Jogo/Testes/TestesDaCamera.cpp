// Os testes da câmera: a tabela da parte 8 da Ficha Lógica: Interface.
// Tela de 1600 x 837 e mapa de 1.024 x 1.024 pixels do mundo (a vila, 64 x 64 tiles).

#include <cmath>
#include <cstdio>

#include "CameraDoMapa.h"
#include "ConferenciaDosTestes.h"

namespace {

// Números com vírgula quase nunca dão exatos: basta chegar a 0,05 de distância.
bool quaseIgual(double valorCalculado, double valorEsperado)
{
    return std::fabs(valorCalculado - valorEsperado) < 0.05;
}

CameraDoMapa criarCameraDaVila()
{
    CameraDoMapa camera;
    camera.atualizarTamanhoDaTela(1600, 837);
    camera.definirTamanhoDoMapa(1024, 1024);
    camera.centralizarNoMapa();
    return camera;
}

void testarPosicaoInicial()
{
    const CameraDoMapa camera = criarCameraDaVila();
    conferir(camera.zoomAtual() == 3, "a câmera começa no zoom 3x");
    conferir(quaseIgual(camera.posicaoXNoMundoAtual(), 245.33) && quaseIgual(camera.posicaoYNoMundoAtual(), 372.5),
             "a câmera nova, centralizada a 3x, fica em (245,3; 372,5)");
}

void testarTeclado()
{
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.posicionarNoMundo(0.0, 0.0);
        camera.moverPeloTeclado(1, 0, 1.0, false);
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), 300.0), "direita por 1 s a 3x anda 300 pixels do mundo");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.posicionarNoMundo(0.0, 0.0);
        camera.moverPeloTeclado(1, 0, 0.5, false);
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), 150.0), "direita por 0,5 s a 3x anda 150 pixels do mundo");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.mudarZoomPelaRoda(-1.0, 0.0, 0.0);  // 3x -> 2x
        camera.posicionarNoMundo(0.0, 0.0);
        camera.moverPeloTeclado(1, 0, 0.4, false);
        conferir(camera.zoomAtual() == 2 && quaseIgual(camera.posicaoXNoMundoAtual(), 180.0),
                 "a 2x, a câmera anda 450 pixels do mundo por segundo (180 em 0,4 s)");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.posicionarNoMundo(0.0, 0.0);
        camera.moverPeloTeclado(1, 0, 0.1, true);
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), 60.0), "direita com Shift por 0,1 s a 3x anda 60");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.posicionarNoMundo(0.0, 0.0);
        camera.moverPeloTeclado(1, 1, 0.1, false);
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), 21.21) && quaseIgual(camera.posicaoYNoMundoAtual(), 21.21),
                 "na diagonal por 0,1 s a 3x, x e y andam 21,2 cada");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        const double xAntes = camera.posicaoXNoMundoAtual();
        camera.moverPeloTeclado(0, 0, 1.0, false);
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), xAntes), "esquerda e direita juntas (direção 0) não andam");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.moverPeloTeclado(-1, 0, 10.0, false);
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), 0.0), "esquerda por 10 s para na borda: x = 0");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.moverPeloTeclado(0, 1, 10.0, false);
        conferir(quaseIgual(camera.posicaoYNoMundoAtual(), 745.0), "baixo por 10 s a 3x para em y = 745");
    }
}

void testarZoom()
{
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.posicionarNoMundo(100.0, 200.0);
        const bool mudou = camera.mudarZoomPelaRoda(1.0, 800.0, 400.0);
        conferir(mudou && camera.zoomAtual() == 4, "a roda para cima troca 3x por 4x");
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), 166.67) && quaseIgual(camera.posicaoYNoMundoAtual(), 233.33),
                 "o zoom deixa o ponto embaixo do mouse parado: câmera (166,7; 233,3)");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.mudarZoomPelaRoda(1.0, 0.0, 0.0);
        const bool mudou = camera.mudarZoomPelaRoda(1.0, 0.0, 0.0);
        conferir(!mudou && camera.zoomAtual() == 4, "no 4x, a roda para cima não passa do 4x");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        const bool primeira = camera.mudarZoomPelaRoda(0.4, 0.0, 0.0);
        const bool segunda = camera.mudarZoomPelaRoda(0.4, 0.0, 0.0);
        const bool terceira = camera.mudarZoomPelaRoda(0.4, 0.0, 0.0);
        conferir(!primeira && !segunda && terceira && camera.zoomAtual() == 4,
                 "a roda do touchpad (0,4 de cada vez) troca o zoom na terceira vez");
    }
}

void testarArrastoELimites()
{
    {
        CameraDoMapa camera = criarCameraDaVila();
        const double xAntes = camera.posicaoXNoMundoAtual();
        camera.comecarArrasto();
        camera.arrastarPeloMouse(30.0, 0.0);
        camera.terminarArrasto();
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), xAntes - 10.0), "arrastar 30 pixels para a direita a 3x tira 10 do x");
        camera.arrastarPeloMouse(30.0, 0.0);
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), xAntes - 10.0), "sem o botão do meio apertado, o mouse não arrasta");
    }
    {
        CameraDoMapa camera;
        camera.atualizarTamanhoDaTela(2400, 2400);
        camera.definirTamanhoDoMapa(1024, 1024);
        camera.mudarZoomPelaRoda(-1.0, 0.0, 0.0);  // 2x
        camera.centralizarNoMapa();
        camera.moverPeloTeclado(1, 1, 1.0, false);
        conferir(quaseIgual(camera.posicaoXNoMundoAtual(), -88.0) && quaseIgual(camera.posicaoYNoMundoAtual(), -88.0),
                 "numa tela maior que o mapa, o mapa fica centralizado e parado: (-88, -88)");
    }
    {
        CameraDoMapa camera = criarCameraDaVila();
        camera.posicionarNoMundo(245.33, 0.0);
        conferir(camera.deslocamentoXNaTela() == 736, "câmera em x = 245,3 a 3x desenha com deslocamento 736");
    }
}

}  // namespace

void testarCameraDoMapa()
{
    std::printf("Câmera do mapa\n");
    testarPosicaoInicial();
    testarTeclado();
    testarZoom();
    testarArrastoELimites();
}
