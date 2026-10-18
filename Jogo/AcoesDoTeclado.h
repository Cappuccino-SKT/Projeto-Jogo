#pragma once

// A tabela de ações do teclado (rodada FL2-4): o laço principal pergunta "a
// ação Pausar foi apertada?", nunca "o Espaço foi apertado?". Trocar uma
// tecla é mudar uma linha da tabela; no futuro, o perfil do jogador lê a
// tabela de um arquivo e deixa o jogador remapear (Interface).
//
// As teclas valem pela POSIÇÃO no teclado (scancode): o W é a tecla do W em
// qualquer layout. Só usa os tipos da SDL (o cabeçalho), sem ligar a SDL.

#include <optional>
#include <vector>

#include <SDL3/SDL_scancode.h>

enum class AcaoDoJogador {
    FecharOJogo,
    AlternarTelaCheia,
    MostrarColisaoParaDepuracao,
    PausarOuDespausar,
    Velocidade1x,
    Velocidade2x,
    Velocidade4x,
    MoverCameraParaCima,
    MoverCameraParaBaixo,
    MoverCameraParaEsquerda,
    MoverCameraParaDireita,
    CorrerComACamera,
    CriarNpcDeTeste,               // N (marco 4, FL4-6): mais um NPC, até 50
    MostrarCaminhosParaDepuracao,  // F2 (marco 4, FL4-6): o caminho de cada NPC em azul
};

// Uma linha da tabela: a ação, a tecla principal e uma alternativa (opcional).
struct TeclasDeUmaAcao {
    AcaoDoJogador acao;
    SDL_Scancode teclaPrincipal;
    SDL_Scancode teclaAlternativa = SDL_SCANCODE_UNKNOWN;
};

class TabelaDeAcoesDoTeclado {
public:
    // As teclas padrão: as provisórias dos marcos 1, 2 e 4 (FL1-3, FT1-4 e FL4-6).
    static TabelaDeAcoesDoTeclado criarComAsTeclasPadrao();

    // A ação de uma tecla que acabou de ser apertada, ou vazio se a tecla não faz nada.
    std::optional<AcaoDoJogador> acaoDaTeclaApertada(SDL_Scancode teclaApertada) const;

    // Se alguma tecla da ação está segurada agora. "teclaEstaSegurada" é o
    // estado do teclado da SDL (SDL_GetKeyboardState), indexado pelo scancode.
    bool acaoEstaSegurada(AcaoDoJogador acao, const bool* teclaEstaSegurada) const;

    const std::vector<TeclasDeUmaAcao>& todasAsLinhas() const { return teclasDeCadaAcao; }

private:
    std::vector<TeclasDeUmaAcao> teclasDeCadaAcao;
};
