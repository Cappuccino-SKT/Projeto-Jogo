#pragma once

// O "pincel" da placa de vídeo, usado pelo desenho do mapa e dos personagens:
// junta muitos quadrados (cada um com um recorte de uma imagem, ou uma cor
// lisa) e manda todos numa chamada só, o "desenho em lote".

#include <string>
#include <vector>

#include <glad/gl.h>

// Uma imagem já carregada na placa de vídeo.
struct TexturaNaPlacaDeVideo {
    GLuint identificadorNaPlacaDeVideo = 0;
    int larguraEmPixels = 0;
    int alturaEmPixels = 0;
};

// Carrega um PNG para a placa de vídeo, com o filtro "mais próximo" (a
// pixel-art ampliada fica nítida). Devolve false e explica se falhar.
bool carregarImagemNaPlacaDeVideo(const std::string& caminhoDaImagem, TexturaNaPlacaDeVideo& textura, std::string& mensagemDeErro);
// O mesmo, a partir de pixels já na memória (4 números por pixel: vermelho, verde, azul, transparência).
void carregarPixelsNaPlacaDeVideo(const unsigned char* pixelsRgba, int larguraEmPixels, int alturaEmPixels, TexturaNaPlacaDeVideo& textura);
void liberarTexturaDaPlacaDeVideo(TexturaNaPlacaDeVideo& textura);

class DesenhistaDeQuadrados {
public:
    // Prepara o programa da placa de vídeo. Precisa do OpenGL já ligado.
    bool preparar(std::string& mensagemDeErro);

    // Liga o programa, o tamanho da tela e a mistura com transparência.
    void comecarDesenho(int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);

    // Começa um lote novo: com uma imagem, ou com uma cor lisa (de 0 a 1: vermelho, verde, azul, transparência).
    void comecarLoteComImagem(const TexturaNaPlacaDeVideo& textura);
    void comecarLoteComCorLisa(const float corRgba[4]);

    // Um quadrado na tela (em pixels), com o recorte da imagem em pixels da imagem.
    // No lote de cor lisa, o recorte é ignorado.
    void acrescentarQuadrado(float esquerda, float topo, float direita, float baixo,
                             float recorteX, float recorteY, float recorteLargura, float recorteAltura);

    // Manda o lote para a placa de vídeo e desenha.
    void mandarLoteParaAPlacaDeVideo();

    // Devolve à placa de vídeo tudo o que foi pedido.
    void liberar();

private:
    GLuint programaDeSombreamento = 0;  // o "shader": o programa que roda na placa de vídeo
    GLuint arranjoDeVertices = 0;       // VAO: como ler os números de cada canto
    GLuint memoriaDeVertices = 0;       // VBO: os números dos cantos, mandados a cada lote
    GLint localDoTamanhoDaTela = -1;
    GLint localDoUsarTextura = -1;
    GLint localDaCorSemTextura = -1;

    // O tamanho da imagem do lote atual, para converter o recorte em u e v (de 0 a 1).
    float larguraDaImagemDoLote = 1.0f;
    float alturaDaImagemDoLote = 1.0f;

    // Os cantos dos quadrados deste lote: x e y na tela, u e v na imagem.
    std::vector<float> verticesDoLote;
};
