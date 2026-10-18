#pragma once

// A câmera do mapa: que pedaço do Mapa Local aparece na janela, e com que
// zoom. Ficha Lógica: Interface (câmera, zoom e teclas).
//
// Não usa a SDL nem o OpenGL: quem lê o teclado e o mouse é o main.cpp, que
// entrega os números. Assim as contas podem ser testadas sozinhas.

#include <array>

// As definições da câmera (ficha, parte 2). No futuro, o perfil do jogador.
struct RegrasDaCamera {
    std::array<int, 3> niveisDeZoom = {2, 3, 4};  // FT1-4
    int zoomInicial = 3;                           // FT1-4
    double velocidadeNoTecladoEmPixelsDaTelaPorSegundo = 900.0;  // FL2-1
    double multiplicadorDaVelocidadeComShift = 2.0;              // FL2-1
};

class CameraDoMapa {
public:
    explicit CameraDoMapa(const RegrasDaCamera& regrasDaCamera = RegrasDaCamera{});

    // O que a câmera precisa saber: o tamanho da tela (a cada quadro) e o do mapa.
    void atualizarTamanhoDaTela(int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    void definirTamanhoDoMapa(int larguraDoMapaEmPixelsDoMundo, int alturaDoMapaEmPixelsDoMundo);

    // Regra 4.1. direcaoX e direcaoY valem -1, 0 ou 1.
    void moverPeloTeclado(int direcaoX, int direcaoY, double segundosReaisDoQuadro, bool correndoComShift);

    // Regra 4.2: arrastar com o botão do meio.
    void comecarArrasto() { estaArrastandoComOBotaoDoMeio = true; }
    void arrastarPeloMouse(double deslocamentoXNaTela, double deslocamentoYNaTela);
    void terminarArrasto() { estaArrastandoComOBotaoDoMeio = false; }
    bool estaArrastando() const { return estaArrastandoComOBotaoDoMeio; }

    // Regra 4.3. Para cima é positivo. Devolve true se o zoom mudou.
    bool mudarZoomPelaRoda(double quantoARodaGirou, double mouseXNaTela, double mouseYNaTela);

    // Regra 4.4: a borda do mapa nunca sai da borda da tela.
    void limitarAoMapa();

    // Regra 4.5: o centro do mapa no centro da tela.
    void centralizarNoMapa();

    // Regra 4.6: o deslocamento do desenho, em pixels inteiros da tela.
    int deslocamentoXNaTela() const;
    int deslocamentoYNaTela() const;

    int zoomAtual() const { return zoom; }
    double posicaoXNoMundoAtual() const { return posicaoXNoMundo; }
    double posicaoYNoMundoAtual() const { return posicaoYNoMundo; }

    // Põe a câmera num lugar exato (usado nos testes).
    void posicionarNoMundo(double novaPosicaoXNoMundo, double novaPosicaoYNoMundo);

private:
    RegrasDaCamera regras;

    // O canto de cima à esquerda da tela, em pixels do mundo (com vírgula).
    double posicaoXNoMundo = 0.0;
    double posicaoYNoMundo = 0.0;
    int zoom = 3;

    int larguraDaTela = 1;
    int alturaDaTela = 1;
    int larguraDoMapaNoMundo = 0;
    int alturaDoMapaNoMundo = 0;

    double sobraDaRodaDoMouse = 0.0;
    bool estaArrastandoComOBotaoDoMeio = false;

    int indiceDoZoomAtual() const;
};
