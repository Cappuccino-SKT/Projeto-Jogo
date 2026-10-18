#pragma once

// O desenho do Mapa Local com o OpenGL (regra 4.3 da Ficha Lógica: Mapa).
// Desenha só os tiles que aparecem na tela, todos de um conjunto numa
// chamada só para a placa de vídeo (o "desenho em lote").

#include <string>
#include <vector>

#include "CameraDoMapa.h"
#include "DesenhoDeQuadrados.h"
#include "Mapa/MapaLocal.h"
#include "Vila/VilaInicial.h"

class DesenhistaDoMapa {
public:
    // Prepara o pincel da placa de vídeo e carrega as imagens dos conjuntos.
    // Precisa do OpenGL já ligado. Devolve false e explica o erro se algo falhar.
    bool preparar(const MapaLocal& mapa, std::string& mensagemDeErro);

    // A ordem de desenho: chao, detalhes_do_chao, objetos_baixos, construcoes_e_paredes...
    void desenharCamadasAbaixoDosPersonagens(const MapaLocal& mapa, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    // ...depois os personagens (marco 4), e por último acima_do_personagem.
    void desenharCamadasAcimaDosPersonagens(const MapaLocal& mapa, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    // F1 (FL1-3): pinta de vermelho translúcido os tiles bloqueados.
    void desenharTilesBloqueadosParaDepuracao(const MapaLocal& mapa, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    // F2 (FL4-6): pinta de azul translúcido uma lista de tiles (os caminhos dos NPCs).
    void desenharTilesDosCaminhosParaDepuracao(const std::vector<PosicaoDeTile>& tilesDosCaminhos, const MapaLocal& mapa,
                                               const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);

    // Marco 6 (ficha da Construção, regra 4.3): carrega a imagem de cada
    // construção que existir em data/imagens/construcoes/. Uma imagem de
    // tamanho errado vira um aviso, e a construção fica com o retângulo.
    void prepararImagensDasConstrucoes(const VilaDoJogo& vila, const MapaLocal& mapa, const std::string& pastaDosDados,
                                       std::vector<std::string>& avisos);

    // Antes dos personagens: a parte da imagem dentro do lote, ou, sem
    // imagem, o retângulo de cor do marco 5 (FL5-1), com a porta mais escura.
    void desenharConstrucoes(const VilaDoJogo& vila, const MapaLocal& mapa, const CameraDoMapa& camera,
                             int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
    // Depois dos personagens: os 32 px de cima da imagem (o telhado), que
    // cobrem quem passa atrás da construção (regra 4.3, passo 3).
    void desenharTelhadosDasConstrucoes(const VilaDoJogo& vila, const MapaLocal& mapa, const CameraDoMapa& camera,
                                        int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);

    // Devolve à placa de vídeo tudo o que foi pedido. Chamar antes de fechar o OpenGL.
    void liberar();

private:
    DesenhistaDeQuadrados pincel;

    // Uma textura para cada conjunto de tiles do mapa, na mesma ordem.
    std::vector<TexturaNaPlacaDeVideo> texturasDosConjuntos;

    // As imagens das construções (uma por arquivo) e qual cada construção da
    // vila usa (-1 = o retângulo de cor).
    std::vector<TexturaNaPlacaDeVideo> texturasDasConstrucoes;
    std::vector<int> texturaDeCadaConstrucao;

    void desenharCamadaPeloNome(const MapaLocal& mapa, const char* nomeDaCamada, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
};
