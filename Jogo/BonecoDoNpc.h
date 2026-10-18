#pragma once

// O boneco do NPC (Ficha Lógica: NPC, regras 4.6 e 4.7, marco 4): como a
// folha de quadros é organizada, qual quadro mostrar e onde desenhar.
// Sem OpenGL: as contas podem ser testadas pelo testes_do_jogo.

#include <string>
#include <vector>

#include "Npc/ComponentesDoNpc.h"
#include "Npc/MovimentoDosNpcs.h"

// O data/personagens/boneco_provisorio.json (FL4-4). Os caminhos das imagens
// são relativos à pasta Jogo/data.
struct DefinicaoDoBoneco {
    int tamanhoDoQuadroEmPixels = 64;
    int pesXNoQuadro = 32;
    int pesYNoQuadro = 43;
    int linhaParado = 0;     // + a direção (baixo 0, cima 1, direita 2, esquerda 3)
    int linhaAndando = 4;    // idem
    int quadrosDeAndar = 6;
    int distanciaPorQuadroDeAndarEmMilesimos = 400;
    std::vector<std::string> caminhosDasCamadas;  // de baixo para cima: corpo, roupa, cabelo
    std::string caminhoDaImagemDeReserva;

    int distanciaDoCicloDeAndarEmMilesimos() const { return quadrosDeAndar * distanciaPorQuadroDeAndarEmMilesimos; }
};

// Lê o texto do arquivo. Cada problema vira uma linha em "errosDoBoneco".
DefinicaoDoBoneco lerDefinicaoDoBoneco(const std::string& textoDoArquivo, std::vector<std::string>& errosDoBoneco);

// Um quadro da folha: a coluna e a linha.
struct QuadroNaFolha {
    int coluna = 0;
    int linha = 0;
};

// Regra 4.6, passo 3: o quadro de um NPC parado ou andando.
QuadroNaFolha escolherQuadroDoBoneco(const PosicaoDoNpc& posicao, const DefinicaoDoBoneco& definicao);

// Regra 4.6, passo 1 (FL4-5): a posição entre a do tick anterior e a do atual.
PontoEmPixelsDoMundo interpolarPesEntreOsTicks(const PontoEmPixelsDoMundo& pesNoTickAnterior,
                                               const PontoEmPixelsDoMundo& pesNoTickAtual, double fracaoDoTick);
