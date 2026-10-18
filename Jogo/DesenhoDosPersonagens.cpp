#include "DesenhoDosPersonagens.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <tuple>

#include <stb_image.h>

namespace {

// Junta a camada de cima sobre a de baixo, pixel por pixel, respeitando a
// transparência (a conta "por cima" das imagens).
void juntarCamadaPorCima(std::vector<unsigned char>& pixelsDeBaixo, const unsigned char* pixelsDeCima)
{
    for (std::size_t inicio = 0; inicio < pixelsDeBaixo.size(); inicio += 4) {
        const int transparenciaDeCima = pixelsDeCima[inicio + 3];
        if (transparenciaDeCima == 0) {
            continue;
        }
        const int transparenciaDeBaixo = pixelsDeBaixo[inicio + 3];
        const int transparenciaFinal = transparenciaDeCima + transparenciaDeBaixo * (255 - transparenciaDeCima) / 255;
        for (int canal = 0; canal < 3; ++canal) {
            const int corDeCima = pixelsDeCima[inicio + canal];
            const int corDeBaixo = pixelsDeBaixo[inicio + canal];
            const int corMisturada = (corDeCima * transparenciaDeCima + corDeBaixo * transparenciaDeBaixo * (255 - transparenciaDeCima) / 255)
                                     / std::max(transparenciaFinal, 1);
            pixelsDeBaixo[inicio + canal] = static_cast<unsigned char>(std::min(corMisturada, 255));
        }
        pixelsDeBaixo[inicio + 3] = static_cast<unsigned char>(transparenciaFinal);
    }
}

// Lê as camadas e junta todas numa folha só. Devolve false e explica se
// alguma faltar ou tiver um tamanho diferente das outras.
bool juntarAsCamadasDoBoneco(const std::vector<std::string>& caminhosDasCamadas, int tamanhoDoQuadro,
                             std::vector<unsigned char>& pixelsDaFolha, int& larguraDaFolha, int& alturaDaFolha, std::string& motivo)
{
    pixelsDaFolha.clear();
    for (const std::string& caminho : caminhosDasCamadas) {
        int largura = 0;
        int altura = 0;
        int canais = 0;
        unsigned char* pixels = stbi_load(caminho.c_str(), &largura, &altura, &canais, 4);
        if (pixels == nullptr) {
            motivo = "não foi possível abrir " + caminho;
            return false;
        }
        const bool tamanhoCerto = largura % tamanhoDoQuadro == 0 && altura % tamanhoDoQuadro == 0
                               && (pixelsDaFolha.empty() || (largura == larguraDaFolha && altura == alturaDaFolha));
        if (!tamanhoCerto) {
            stbi_image_free(pixels);
            motivo = caminho + " precisa ter o mesmo tamanho das outras camadas, múltiplo de " + std::to_string(tamanhoDoQuadro);
            return false;
        }
        if (pixelsDaFolha.empty()) {
            larguraDaFolha = largura;
            alturaDaFolha = altura;
            pixelsDaFolha.assign(pixels, pixels + static_cast<std::size_t>(largura) * altura * 4);
        } else {
            juntarCamadaPorCima(pixelsDaFolha, pixels);
        }
        stbi_image_free(pixels);
    }
    return !pixelsDaFolha.empty();
}

}  // namespace

bool DesenhistaDosPersonagens::preparar(const DefinicaoDoBoneco& definicao, const std::string& pastaDosDados,
                                        std::vector<std::string>& avisos, std::string& mensagemDeErro)
{
    definicaoDoBoneco = definicao;
    if (!pincel.preparar(mensagemDeErro)) {
        return false;
    }

    std::vector<std::string> caminhosDasCamadas;
    for (const std::string& camada : definicao.caminhosDasCamadas) {
        caminhosDasCamadas.push_back(pastaDosDados + "/" + camada);
    }
    std::vector<unsigned char> pixelsDaFolha;
    int larguraDaFolha = 0;
    int alturaDaFolha = 0;
    std::string motivo;
    if (!juntarAsCamadasDoBoneco(caminhosDasCamadas, definicao.tamanhoDoQuadroEmPixels, pixelsDaFolha, larguraDaFolha, alturaDaFolha, motivo)) {
        // Regra 4.7, passo 2: sem a arte provisória (fora do Git), usa o boneco de teste.
        avisos.push_back("boneco: " + motivo + "; usando o boneco de teste");
        const std::vector<std::string> soAReserva = {pastaDosDados + "/" + definicao.caminhoDaImagemDeReserva};
        if (!juntarAsCamadasDoBoneco(soAReserva, definicao.tamanhoDoQuadroEmPixels, pixelsDaFolha, larguraDaFolha, alturaDaFolha, motivo)) {
            mensagemDeErro = "boneco: " + motivo;
            return false;
        }
    }
    carregarPixelsNaPlacaDeVideo(pixelsDaFolha.data(), larguraDaFolha, alturaDaFolha, folhaDoBoneco);
    return true;
}

void DesenhistaDosPersonagens::desenharNpcs(const entt::registry& registroDasEntidades, double fracaoDoTick, int tamanhoDoTileEmPixels,
                                            const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels)
{
    // Passos 1 e 3: a posição suave e o quadro de cada NPC.
    npcsParaDesenhar.clear();
    for (const auto [npc, posicao] : registroDasEntidades.view<PosicaoDoNpc>().each()) {
        const PontoEmPixelsDoMundo pesNoTickAnterior = calcularPesEmPixelsDoMundo(posicao.lugarNoTickAnteriorParaDesenho, tamanhoDoTileEmPixels);
        const PontoEmPixelsDoMundo pesNoTickAtual = calcularPesEmPixelsDoMundo(posicao.lugarAtual, tamanhoDoTileEmPixels);
        npcsParaDesenhar.push_back({interpolarPesEntreOsTicks(pesNoTickAnterior, pesNoTickAtual, fracaoDoTick),
                                    escolherQuadroDoBoneco(posicao, definicaoDoBoneco), entt::to_integral(npc)});
    }

    // Passo 2: quem está mais acima na tela é desenhado primeiro; no empate, o
    // de menor x e depois o de menor número.
    std::sort(npcsParaDesenhar.begin(), npcsParaDesenhar.end(), [](const NpcParaDesenhar& primeiro, const NpcParaDesenhar& segundo) {
        return std::tie(primeiro.pes.pixelY, primeiro.pes.pixelX, primeiro.numeroDaEntidade)
             < std::tie(segundo.pes.pixelY, segundo.pes.pixelX, segundo.numeroDaEntidade);
    });

    // Passos 4 e 5: o recorte na folha e o lugar na tela.
    pincel.comecarDesenho(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
    pincel.comecarLoteComImagem(folhaDoBoneco);
    const int zoom = camera.zoomAtual();
    const float ladoDoQuadro = static_cast<float>(definicaoDoBoneco.tamanhoDoQuadroEmPixels);
    const float ladoDoQuadroNaTela = ladoDoQuadro * zoom;
    for (const NpcParaDesenhar& npc : npcsParaDesenhar) {
        // O canto do quadro fica nos pés menos o ponto dos pés no quadro.
        const double cantoXNoMundo = npc.pes.pixelX - definicaoDoBoneco.pesXNoQuadro;
        const double cantoYNoMundo = npc.pes.pixelY - definicaoDoBoneco.pesYNoQuadro;
        // Pixel inteiro da tela, como o mapa (ficha da Interface, regra 4.6).
        const float esquerda = static_cast<float>(std::lround(cantoXNoMundo * zoom) - camera.deslocamentoXNaTela());
        const float topo = static_cast<float>(std::lround(cantoYNoMundo * zoom) - camera.deslocamentoYNaTela());
        const bool estaForaDaTela = esquerda + ladoDoQuadroNaTela < 0 || topo + ladoDoQuadroNaTela < 0
                                 || esquerda > larguraDaTelaEmPixels || topo > alturaDaTelaEmPixels;
        if (estaForaDaTela) {
            continue;
        }
        pincel.acrescentarQuadrado(esquerda, topo, esquerda + ladoDoQuadroNaTela, topo + ladoDoQuadroNaTela,
                                   npc.quadro.coluna * ladoDoQuadro, npc.quadro.linha * ladoDoQuadro, ladoDoQuadro, ladoDoQuadro);
    }
    pincel.mandarLoteParaAPlacaDeVideo();
}

void DesenhistaDosPersonagens::liberar()
{
    liberarTexturaDaPlacaDeVideo(folhaDoBoneco);
    pincel.liberar();
}
