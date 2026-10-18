#pragma once

// O desenho do Mapa Local com o OpenGL (regra 4.3 da Ficha Lógica: Mapa).
// Desenha só os tiles que aparecem na tela, todos de um conjunto numa
// chamada só para a placa de vídeo (o "desenho em lote").

#include <string>
#include <vector>

#include <glad/gl.h>

#include "Mapa/MapaLocal.h"

// A imagem de um conjunto de tiles, já carregada na placa de vídeo.
struct TexturaDoConjuntoDeTiles {
    GLuint identificadorNaPlacaDeVideo = 0;
    int larguraEmPixels = 0;
    int alturaEmPixels = 0;
};

// Onde a câmera está olhando. No marco 1 ela fica parada no canto de cima à
// esquerda, com zoom 3x; a câmera que anda e o zoom que muda vêm no marco 2.
struct CameraDoMapa {
    int posicaoXEmPixelsDoMundo = 0;
    int posicaoYEmPixelsDoMundo = 0;
    int zoom = 3;
};

class DesenhistaDoMapa {
public:
    // Prepara os programas da placa de vídeo e carrega as imagens dos conjuntos.
    // Precisa do OpenGL já ligado. Devolve false e explica o erro se algo falhar.
    bool preparar(const MapaLocal& mapa, std::string& mensagemDeErro);

    // A ordem de desenho: chao, detalhes_do_chao, objetos_baixos, construcoes_e_paredes...
    void desenharCamadasAbaixoDosPersonagens(const MapaLocal& mapa, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    // ...depois os personagens (marco 4), e por último acima_do_personagem.
    void desenharCamadasAcimaDosPersonagens(const MapaLocal& mapa, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    // F1 (FL1-3): pinta de vermelho translúcido os tiles bloqueados.
    void desenharTilesBloqueadosParaDepuracao(const MapaLocal& mapa, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);

    // Devolve à placa de vídeo tudo o que foi pedido. Chamar antes de fechar o OpenGL.
    void liberar();

private:
    GLuint programaDeSombreamento = 0;  // o "shader": o programa que roda na placa de vídeo
    GLuint arranjoDeVertices = 0;       // VAO: como ler os números de cada canto
    GLuint memoriaDeVertices = 0;       // VBO: os números dos cantos, mandados a cada quadro
    GLint localDoTamanhoDaTela = -1;
    GLint localDoUsarTextura = -1;
    GLint localDaCorSemTextura = -1;

    // Uma textura para cada conjunto de tiles do mapa, na mesma ordem.
    std::vector<TexturaDoConjuntoDeTiles> texturasDosConjuntos;

    // Os cantos dos quadrados a desenhar neste lote: x e y na tela, u e v na imagem.
    std::vector<float> verticesDoLote;

    void desenharCamadaPeloNome(const MapaLocal& mapa, const char* nomeDaCamada, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    void comecarDesenho(int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    void mandarLoteParaAPlacaDeVideo();
};
