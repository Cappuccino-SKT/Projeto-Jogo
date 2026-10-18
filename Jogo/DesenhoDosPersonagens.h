#pragma once

// O desenho dos NPCs (Ficha Lógica: NPC, regras 4.6 e 4.7, marco 4).
// As camadas do boneco (corpo, roupa, cabelo) são juntadas numa folha só ao
// carregar; assim todos os NPCs vão numa chamada só para a placa de vídeo.

#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include "BonecoDoNpc.h"
#include "CameraDoMapa.h"
#include "DesenhoDeQuadrados.h"

class DesenhistaDosPersonagens {
public:
    // Regra 4.7: carrega as camadas do boneco (caminhos relativos à pasta dos
    // dados). Se faltar alguma, usa a imagem de reserva e anota um aviso.
    // Devolve false (e explica) só se nem a reserva puder ser usada.
    bool preparar(const DefinicaoDoBoneco& definicao, const std::string& pastaDosDados,
                  std::vector<std::string>& avisos, std::string& mensagemDeErro);

    // Regra 4.6: desenha todos os NPCs, de cima para baixo na tela, na
    // posição suave entre os ticks (FL4-5).
    void desenharNpcs(const entt::registry& registroDasEntidades, double fracaoDoTick, int tamanhoDoTileEmPixels,
                      const CameraDoMapa& camera, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels);

    // Devolve à placa de vídeo tudo o que foi pedido.
    void liberar();

private:
    DefinicaoDoBoneco definicaoDoBoneco;
    DesenhistaDeQuadrados pincel;
    TexturaNaPlacaDeVideo folhaDoBoneco;  // as camadas já juntadas

    // Um NPC pronto para desenhar: onde ficam os pés e qual quadro.
    struct NpcParaDesenhar {
        PontoEmPixelsDoMundo pes;
        QuadroNaFolha quadro;
        unsigned int numeroDaEntidade = 0;
    };
    std::vector<NpcParaDesenhar> npcsParaDesenhar;  // reaproveitada a cada quadro
};
