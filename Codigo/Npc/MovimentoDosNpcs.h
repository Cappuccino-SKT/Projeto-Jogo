#pragma once

// O movimento dos NPCs (Ficha Lógica: NPC, regras 4.1 a 4.5, marco 4):
// criar o NPC, pedir um caminho, atender os pedidos e andar a cada tick.
// O desenho fica no Jogo/; aqui só as regras, que o teste sem tela também usa.

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include "Mapa/BuscaDeCaminho.h"
#include "Mapa/MapaLocal.h"
#include "Npc/ComponentesDoNpc.h"

struct RegrasDoMovimento {
    int velocidadeDeAndarEmMilesimosPorTick = 160;  // 128 tiles por hora de jogo (FL4-1)
    int caminhosCalculadosPorTick = 4;              // FT1-3: o cálculo distribuído entre os ticks
    // O tamanho do ciclo da animação de andar. Quem sabe é o boneco (no Jogo/):
    // 6 quadros × 400 milésimos no boneco provisório.
    int distanciaDoCicloDeAndarEmMilesimos = 2400;
};

// 128 tiles por hora e 800 ticks por hora -> 160 milésimos por tick (a metade arredonda para cima).
int converterVelocidadeParaMilesimosPorTick(int velocidadeEmTilesPorHora, int ticksPorHoraDeJogo);

// O passo reto mede 1.000 milésimos; o diagonal, 1.414.
int comprimentoDoPassoEmMilesimos(PosicaoDeTile tileDeOrigem, PosicaoDeTile tileDeDestino);

// O lado para onde o NPC olha num passo. Na diagonal vale o lado (direita ou
// esquerda); parado (passo 0, 0), continua olhando para onde olhava.
DirecaoDoOlhar descobrirDirecaoDoPasso(int passoX, int passoY, DirecaoDoOlhar direcaoAtual);

// Regra 4.1: o tile livre em volta do centro, em quadrados cada vez maiores
// (linha por linha, de cima para baixo e da esquerda para a direita).
// Pula os primeiros tiles livres (a tecla N usa isso para não empilhar os NPCs
// novos). Devolve nada se o mapa não tem tantos tiles livres.
std::optional<PosicaoDeTile> acharTileLivreEmVoltaDe(const MapaLocal& mapa, PosicaoDeTile tileDoCentro,
                                                     int quantidadeDeTilesLivresParaPular);

// Cria o NPC parado no tile, olhando para baixo.
entt::entity criarNpcNoTile(entt::registry& registroDasEntidades, PosicaoDeTile tileOndeNasce);

// Regra 4.2, passo 2: cria ou troca o pedido de caminho do NPC.
void pedirCaminhoParaONpc(entt::registry& registroDasEntidades, entt::entity npc, PosicaoDeTile tileDeDestino, int64_t tickAtual);

// Regra 4.3: atende até caminhosCalculadosPorTick pedidos, os mais antigos
// primeiro. Devolve os avisos para o console (por exemplo, um destino sem caminho).
std::vector<std::string> atenderPedidosDeCaminho(entt::registry& registroDasEntidades, const MapaLocal& mapa,
                                                 BuscadorDeCaminho& buscadorDeCaminho, const RegrasDoMovimento& regras);

// Regra 4.4: anda todos os NPCs que têm caminho. Antes, todos os NPCs guardam
// a posição atual como a "do tick anterior", para o desenho suave (FL4-5).
void andarOsNpcsUmTick(entt::registry& registroDasEntidades, const MapaLocal& mapa, const RegrasDoMovimento& regras,
                       int64_t tickAtual);

// Regra 4.5: onde ficam os pés do NPC, em pixels do mundo (no centro do tile).
struct PontoEmPixelsDoMundo {
    double pixelX = 0.0;
    double pixelY = 0.0;
};
PontoEmPixelsDoMundo calcularPesEmPixelsDoMundo(const LugarDoNpc& lugar, int tamanhoDoTileEmPixels);
