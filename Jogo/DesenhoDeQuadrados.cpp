#include "DesenhoDeQuadrados.h"

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

// O programa que roda para cada pixel: pega a cor da imagem (ou uma cor lisa,
// para a depuração).
const char* const textoDoSombreadorDePixels = R"(
#version 330 core
in vec2 coordenadaParaOPixel;
uniform sampler2D imagemDoLote;
uniform bool usarTextura;
uniform vec4 corSemTextura;
out vec4 corFinal;
void main()
{
    corFinal = usarTextura ? texture(imagemDoLote, coordenadaParaOPixel) : corSemTextura;
}
)";

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

}  // namespace

bool carregarImagemNaPlacaDeVideo(const std::string& caminhoDaImagem, TexturaNaPlacaDeVideo& textura, std::string& mensagemDeErro)
{
    int larguraDaImagem = 0;
    int alturaDaImagem = 0;
    int canaisNoArquivo = 0;
    unsigned char* pixelsDaImagem = stbi_load(caminhoDaImagem.c_str(), &larguraDaImagem, &alturaDaImagem, &canaisNoArquivo, 4);
    if (pixelsDaImagem == nullptr) {
        mensagemDeErro = "não foi possível abrir a imagem " + caminhoDaImagem;
        return false;
    }
    carregarPixelsNaPlacaDeVideo(pixelsDaImagem, larguraDaImagem, alturaDaImagem, textura);
    stbi_image_free(pixelsDaImagem);
    return true;
}

void carregarPixelsNaPlacaDeVideo(const unsigned char* pixelsRgba, int larguraEmPixels, int alturaEmPixels, TexturaNaPlacaDeVideo& textura)
{
    textura.larguraEmPixels = larguraEmPixels;
    textura.alturaEmPixels = alturaEmPixels;
    glGenTextures(1, &textura.identificadorNaPlacaDeVideo);
    glBindTexture(GL_TEXTURE_2D, textura.identificadorNaPlacaDeVideo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, larguraEmPixels, alturaEmPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsRgba);
    // "Mais próximo": a pixel-art ampliada fica nítida, sem borrar.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void liberarTexturaDaPlacaDeVideo(TexturaNaPlacaDeVideo& textura)
{
    if (textura.identificadorNaPlacaDeVideo != 0) {
        glDeleteTextures(1, &textura.identificadorNaPlacaDeVideo);
        textura.identificadorNaPlacaDeVideo = 0;
    }
}

bool DesenhistaDeQuadrados::preparar(std::string& mensagemDeErro)
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
    return true;
}

void DesenhistaDeQuadrados::comecarDesenho(int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    glUseProgram(programaDeSombreamento);
    glUniform2f(localDoTamanhoDaTela, static_cast<float>(larguraDaTelaEmPixels), static_cast<float>(alturaDaTelaEmPixels));
    glBindVertexArray(arranjoDeVertices);
    glBindBuffer(GL_ARRAY_BUFFER, memoriaDeVertices);
    // Mistura com transparência: as flores, a copa da árvore e o boneco deixam ver o que está embaixo.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DesenhistaDeQuadrados::comecarLoteComImagem(const TexturaNaPlacaDeVideo& textura)
{
    verticesDoLote.clear();
    glUniform1i(localDoUsarTextura, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, textura.identificadorNaPlacaDeVideo);
    larguraDaImagemDoLote = static_cast<float>(textura.larguraEmPixels);
    alturaDaImagemDoLote = static_cast<float>(textura.alturaEmPixels);
}

void DesenhistaDeQuadrados::comecarLoteComCorLisa(const float corRgba[4])
{
    verticesDoLote.clear();
    glUniform1i(localDoUsarTextura, GL_FALSE);
    glUniform4fv(localDaCorSemTextura, 1, corRgba);
    larguraDaImagemDoLote = 1.0f;
    alturaDaImagemDoLote = 1.0f;
}

void DesenhistaDeQuadrados::acrescentarQuadrado(float esquerda, float topo, float direita, float baixo,
                                                float recorteX, float recorteY, float recorteLargura, float recorteAltura)
{
    const float uEsquerda = recorteX / larguraDaImagemDoLote;
    const float vTopo = recorteY / alturaDaImagemDoLote;
    const float uDireita = (recorteX + recorteLargura) / larguraDaImagemDoLote;
    const float vBaixo = (recorteY + recorteAltura) / alturaDaImagemDoLote;
    // Dois triângulos, 6 cantos.
    const float cantos[] = {
        esquerda, topo, uEsquerda, vTopo,   direita, topo, uDireita, vTopo,   direita, baixo, uDireita, vBaixo,
        esquerda, topo, uEsquerda, vTopo,   direita, baixo, uDireita, vBaixo, esquerda, baixo, uEsquerda, vBaixo,
    };
    verticesDoLote.insert(verticesDoLote.end(), std::begin(cantos), std::end(cantos));
}

void DesenhistaDeQuadrados::mandarLoteParaAPlacaDeVideo()
{
    if (verticesDoLote.empty()) {
        return;
    }
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(verticesDoLote.size() * sizeof(float)), verticesDoLote.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verticesDoLote.size() / 4));
}

void DesenhistaDeQuadrados::liberar()
{
    glDeleteBuffers(1, &memoriaDeVertices);
    glDeleteVertexArrays(1, &arranjoDeVertices);
    glDeleteProgram(programaDeSombreamento);
    memoriaDeVertices = 0;
    arranjoDeVertices = 0;
    programaDeSombreamento = 0;
}
