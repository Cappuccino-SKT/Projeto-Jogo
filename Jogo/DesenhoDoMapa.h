#pragma once

// O desenho do Mapa Local com o OpenGL (regra 4.3 da Ficha Lógica: Mapa).
// Desenha só os tiles que aparecem na tela, todos de um conjunto numa
// chamada só para a placa de vídeo (o "desenho em lote").

#include <string>
#include <vector>

#include "CameraDoMapa.h"
#include "DesenhoDeQuadrados.h"
#include "Mapa/MapaLocal.h"

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

    // Devolve à placa de vídeo tudo o que foi pedido. Chamar antes de fechar o OpenGL.
    void liberar();

private:
    DesenhistaDeQuadrados pincel;

    // Uma textura para cada conjunto de tiles do mapa, na mesma ordem.
    std::vector<TexturaNaPlacaDeVideo> texturasDosConjuntos;

    void desenharCamadaPeloNome(const MapaLocal& mapa, const char* nomeDaCamada, const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);
};
