#include "DesenhoDoMapa.h"

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>

#include <stb_image.h>

namespace {

// O programa que roda na placa de vídeo para cada canto (vértice): converte a
// posição na tela, em pixels, para a medida do OpenGL (de -1 a 1).
const char* const textoDoSombreadorDeVertices = R"(
#version 330 core
layout(location = 0) in vec2 posicaoNaTelaEmPixels;
layout(location = 1) in vec2 coordenadaNaImagem;
uniform vec2 tamanhoDaTelaEmPixels;
out vec2 coordenadaParaOPixel;
void main()
{
    vec2 posicaoDeMenosUmAUm = posicaoNaTelaEmPixels / tamanhoDaTelaEmPixels * 2.0 - 1.0;
    gl_Position = vec4(posicaoDeMenosUmAUm.x, -posicaoDeMenosUmAUm.y, 0.0, 1.0);
    coordenadaParaOPixel = coordenadaNaImagem;
}
)";

// O programa que roda para cada pixel: pega a cor da imagem do conjunto
// (ou uma cor lisa, para a depuração da colisão).
const char* const textoDoSombreadorDePixels = R"(
#version 330 core
in vec2 coordenadaParaOPixel;
uniform sampler2D imagemDoConjunto;
uniform bool usarTextura;
uniform vec4 corSemTextura;
out vec4 corFinal;
void main()
{
    corFinal = usarTextura ? texture(imagemDoConjunto, coordenadaParaOPixel) : corSemTextura;
}
)";

// Cor da depuração da colisão: vermelho translúcido.
constexpr float corDaColisaoParaDepuracao[4] = {0.9f, 0.1f, 0.1f, 0.35f};

GLuint compilarSombreador(GLenum tipoDoSombreador, const char* textoDoSombreador, std::string& mensagemDeErro)
{
    const GLuint sombreador = glCreateShader(tipoDoSombreador);
    glShaderSource(sombreador, 1, &textoDoSombreador, nullptr);
    glCompileShader(sombreador);
    GLint compilouCerto = GL_FALSE;
    glGetShaderiv(sombreador, GL_COMPILE_STATUS, &compilouCerto);
    if (compilouCerto != GL_TRUE) {
        char registroDoErro[1024] = {};
        glGetShaderInfoLog(sombreador, sizeof(registroDoErro), nullptr, registroDoErro);
        mensagemDeErro = std::string("erro ao compilar o sombreador: ") + registroDoErro;
        glDeleteShader(sombreador);
        return 0;
    }
    return sombreador;
}

// Quais tiles aparecem na tela (com a câmera e o zoom), já presos ao mapa.
struct FaixaDeTilesVisiveis {
    int primeiraColuna = 0;
    int ultimaColuna = -1;
    int primeiraLinha = 0;
    int ultimaLinha = -1;
};

FaixaDeTilesVisiveis calcularTilesVisiveis(const MapaLocal& mapa, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    const int tamanhoDoTile = mapa.tamanhoDoTileEmPixels;
    FaixaDeTilesVisiveis faixa;
    faixa.primeiraColuna = std::max(0, camera.posicaoXEmPixelsDoMundo / tamanhoDoTile);
    faixa.primeiraLinha = std::max(0, camera.posicaoYEmPixelsDoMundo / tamanhoDoTile);
    faixa.ultimaColuna = std::min(mapa.larguraEmTiles - 1, (camera.posicaoXEmPixelsDoMundo + larguraDaTelaEmPixels / camera.zoom) / tamanhoDoTile);
    faixa.ultimaLinha = std::min(mapa.alturaEmTiles - 1, (camera.posicaoYEmPixelsDoMundo + alturaDaTelaEmPixels / camera.zoom) / tamanhoDoTile);
    return faixa;
}

// Acrescenta um quadrado (dois triângulos, 6 cantos) ao lote.
void acrescentarQuadrado(std::vector<float>& vertices, float esquerda, float topo, float direita, float baixo,
                         float uEsquerda, float vTopo, float uDireita, float vBaixo)
{
    const float cantos[] = {
        esquerda, topo, uEsquerda, vTopo,   direita, topo, uDireita, vTopo,   direita, baixo, uDireita, vBaixo,
        esquerda, topo, uEsquerda, vTopo,   direita, baixo, uDireita, vBaixo, esquerda, baixo, uEsquerda, vBaixo,
    };
    vertices.insert(vertices.end(), std::begin(cantos), std::end(cantos));
}

}  // namespace

bool DesenhistaDoMapa::preparar(const MapaLocal& mapa, std::string& mensagemDeErro)
{
    // 1. O programa da placa de vídeo.
    const GLuint sombreadorDeVertices = compilarSombreador(GL_VERTEX_SHADER, textoDoSombreadorDeVertices, mensagemDeErro);
    const GLuint sombreadorDePixels = compilarSombreador(GL_FRAGMENT_SHADER, textoDoSombreadorDePixels, mensagemDeErro);
    if (sombreadorDeVertices == 0 || sombreadorDePixels == 0) {
        return false;
    }
    programaDeSombreamento = glCreateProgram();
    glAttachShader(programaDeSombreamento, sombreadorDeVertices);
    glAttachShader(programaDeSombreamento, sombreadorDePixels);
    glLinkProgram(programaDeSombreamento);
    glDeleteShader(sombreadorDeVertices);
    glDeleteShader(sombreadorDePixels);
    GLint ligouCerto = GL_FALSE;
    glGetProgramiv(programaDeSombreamento, GL_LINK_STATUS, &ligouCerto);
    if (ligouCerto != GL_TRUE) {
        mensagemDeErro = "erro ao ligar o programa da placa de vídeo";
        return false;
    }
    localDoTamanhoDaTela = glGetUniformLocation(programaDeSombreamento, "tamanhoDaTelaEmPixels");
    localDoUsarTextura = glGetUniformLocation(programaDeSombreamento, "usarTextura");
    localDaCorSemTextura = glGetUniformLocation(programaDeSombreamento, "corSemTextura");

    // 2. A memória dos cantos: 4 números por canto (x, y, u, v).
    glGenVertexArrays(1, &arranjoDeVertices);
    glGenBuffers(1, &memoriaDeVertices);
    glBindVertexArray(arranjoDeVertices);
    glBindBuffer(GL_ARRAY_BUFFER, memoriaDeVertices);
    const GLsizei bytesPorCanto = 4 * sizeof(float);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, bytesPorCanto, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, bytesPorCanto, reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3. As imagens dos conjuntos de tiles.
    for (const ConjuntoDeTiles& conjunto : mapa.conjuntosDeTiles) {
        int larguraDaImagem = 0;
        int alturaDaImagem = 0;
        int canaisNoArquivo = 0;
        unsigned char* pixelsDaImagem = stbi_load(conjunto.caminhoDaImagem.c_str(), &larguraDaImagem, &alturaDaImagem, &canaisNoArquivo, 4);
        if (pixelsDaImagem == nullptr) {
            mensagemDeErro = "não foi possível abrir a imagem " + conjunto.caminhoDaImagem;
            return false;
        }
        TexturaDoConjuntoDeTiles textura;
        textura.larguraEmPixels = larguraDaImagem;
        textura.alturaEmPixels = alturaDaImagem;
        glGenTextures(1, &textura.identificadorNaPlacaDeVideo);
        glBindTexture(GL_TEXTURE_2D, textura.identificadorNaPlacaDeVideo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, larguraDaImagem, alturaDaImagem, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsDaImagem);
        // "Mais próximo": a pixel-art ampliada fica nítida, sem borrar.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        stbi_image_free(pixelsDaImagem);
        texturasDosConjuntos.push_back(textura);
    }
    return true;
}

void DesenhistaDoMapa::comecarDesenho(int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    glUseProgram(programaDeSombreamento);
    glUniform2f(localDoTamanhoDaTela, static_cast<float>(larguraDaTelaEmPixels), static_cast<float>(alturaDaTelaEmPixels));
    glBindVertexArray(arranjoDeVertices);
    glBindBuffer(GL_ARRAY_BUFFER, memoriaDeVertices);
    // Mistura com transparência: as flores e a copa da árvore deixam ver o que está embaixo.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DesenhistaDoMapa::mandarLoteParaAPlacaDeVideo()
{
    if (verticesDoLote.empty()) {
        return;
    }
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(verticesDoLote.size() * sizeof(float)), verticesDoLote.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verticesDoLote.size() / 4));
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
    glUniform1i(localDoUsarTextura, GL_TRUE);

    // Um lote por conjunto de tiles: todos os tiles de um conjunto numa chamada só.
    for (std::size_t indiceDoConjunto = 0; indiceDoConjunto < mapa.conjuntosDeTiles.size(); ++indiceDoConjunto) {
        const ConjuntoDeTiles& conjunto = mapa.conjuntosDeTiles[indiceDoConjunto];
        const TexturaDoConjuntoDeTiles& textura = texturasDosConjuntos[indiceDoConjunto];
        verticesDoLote.clear();
        for (int tileY = faixa.primeiraLinha; tileY <= faixa.ultimaLinha; ++tileY) {
            for (int tileX = faixa.primeiraColuna; tileX <= faixa.ultimaColuna; ++tileX) {
                const int numeroDoTile = camada->numeroDoTilePorPosicao[mapa.indiceDoTileNaLista(tileX, tileY)];
                if (numeroDoTile == 0 || mapa.procurarConjuntoDoTile(numeroDoTile) != &conjunto) {
                    continue;  // vazio, ou de outro conjunto
                }
                // Do tile à tela: (tile x 16 - câmera) x zoom.
                const float esquerda = static_cast<float>((tileX * tamanhoDoTile - camera.posicaoXEmPixelsDoMundo) * camera.zoom);
                const float topo = static_cast<float>((tileY * tamanhoDoTile - camera.posicaoYEmPixelsDoMundo) * camera.zoom);
                const float ladoNaTela = static_cast<float>(tamanhoDoTile * camera.zoom);
                // Do número do tile ao recorte na imagem.
                const RecorteNaImagem recorte = calcularRecorteDoTileNaImagem(conjunto, numeroDoTile, tamanhoDoTile);
                const float uEsquerda = static_cast<float>(recorte.pixelX) / textura.larguraEmPixels;
                const float vTopo = static_cast<float>(recorte.pixelY) / textura.alturaEmPixels;
                const float uDireita = static_cast<float>(recorte.pixelX + recorte.larguraEmPixels) / textura.larguraEmPixels;
                const float vBaixo = static_cast<float>(recorte.pixelY + recorte.alturaEmPixels) / textura.alturaEmPixels;
                acrescentarQuadrado(verticesDoLote, esquerda, topo, esquerda + ladoNaTela, topo + ladoNaTela, uEsquerda, vTopo, uDireita, vBaixo);
            }
        }
        glBindTexture(GL_TEXTURE_2D, textura.identificadorNaPlacaDeVideo);
        mandarLoteParaAPlacaDeVideo();
    }
}

void DesenhistaDoMapa::desenharCamadasAbaixoDosPersonagens(const MapaLocal& mapa, const CameraDoMapa& camera,
                                                           int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    for (const char* nomeDaCamada : {"chao", "detalhes_do_chao", "objetos_baixos", "construcoes_e_paredes"}) {
        desenharCamadaPeloNome(mapa, nomeDaCamada, camera, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    }
}

void DesenhistaDoMapa::desenharCamadasAcimaDosPersonagens(const MapaLocal& mapa, const CameraDoMapa& camera,
                                                          int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    desenharCamadaPeloNome(mapa, "acima_do_personagem", camera, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
}

void DesenhistaDoMapa::desenharTilesBloqueadosParaDepuracao(const MapaLocal& mapa, const CameraDoMapa& camera,
                                                            int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    glUniform1i(localDoUsarTextura, GL_FALSE);
    glUniform4fv(localDaCorSemTextura, 1, corDaColisaoParaDepuracao);

    const FaixaDeTilesVisiveis faixa = calcularTilesVisiveis(mapa, camera, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    const int tamanhoDoTile = mapa.tamanhoDoTileEmPixels;
    verticesDoLote.clear();
    for (int tileY = faixa.primeiraLinha; tileY <= faixa.ultimaLinha; ++tileY) {
        for (int tileX = faixa.primeiraColuna; tileX <= faixa.ultimaColuna; ++tileX) {
            if (mapa.tileDaParaAndar(tileX, tileY)) {
                continue;
            }
            const float esquerda = static_cast<float>((tileX * tamanhoDoTile - camera.posicaoXEmPixelsDoMundo) * camera.zoom);
            const float topo = static_cast<float>((tileY * tamanhoDoTile - camera.posicaoYEmPixelsDoMundo) * camera.zoom);
            const float ladoNaTela = static_cast<float>(tamanhoDoTile * camera.zoom);
            acrescentarQuadrado(verticesDoLote, esquerda, topo, esquerda + ladoNaTela, topo + ladoNaTela, 0, 0, 0, 0);
        }
    }
    mandarLoteParaAPlacaDeVideo();
}

void DesenhistaDoMapa::liberar()
{
    for (TexturaDoConjuntoDeTiles& textura : texturasDosConjuntos) {
        glDeleteTextures(1, &textura.identificadorNaPlacaDeVideo);
    }
    texturasDosConjuntos.clear();
    glDeleteBuffers(1, &memoriaDeVertices);
    glDeleteVertexArrays(1, &arranjoDeVertices);
    glDeleteProgram(programaDeSombreamento);
}
