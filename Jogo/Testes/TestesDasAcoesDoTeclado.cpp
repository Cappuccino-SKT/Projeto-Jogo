// Os testes da tabela de ações do teclado (rodada FL2-4).

#include <cstdio>

#include "AcoesDoTeclado.h"
#include "ConferenciaDosTestes.h"

void testarTabelaDeAcoesDoTeclado()
{
    std::printf("Tabela de ações do teclado\n");
    const TabelaDeAcoesDoTeclado tabela = TabelaDeAcoesDoTeclado::criarComAsTeclasPadrao();
    conferir(tabela.acaoDaTeclaApertada(SDL_SCANCODE_SPACE) == AcaoDoJogador::PausarOuDespausar, "o Espaço é a ação Pausar");
    conferir(tabela.acaoDaTeclaApertada(SDL_SCANCODE_3) == AcaoDoJogador::Velocidade4x, "a tecla 3 é a velocidade 4x");
    conferir(tabela.acaoDaTeclaApertada(SDL_SCANCODE_UP) == AcaoDoJogador::MoverCameraParaCima, "a seta para cima é Mover para cima");
    conferir(!tabela.acaoDaTeclaApertada(SDL_SCANCODE_Q).has_value(), "a tecla Q não faz nada");
    conferir(tabela.acaoDaTeclaApertada(SDL_SCANCODE_N) == AcaoDoJogador::CriarNpcDeTeste
                 && tabela.acaoDaTeclaApertada(SDL_SCANCODE_F2) == AcaoDoJogador::MostrarCaminhosParaDepuracao,
             "o N cria um NPC de teste e o F2 mostra os caminhos (FL4-6)");

    bool teclaEstaSegurada[SDL_SCANCODE_COUNT] = {};
    conferir(!tabela.acaoEstaSegurada(AcaoDoJogador::MoverCameraParaDireita, teclaEstaSegurada), "sem tecla segurada, nada se move");
    teclaEstaSegurada[SDL_SCANCODE_RIGHT] = true;
    conferir(tabela.acaoEstaSegurada(AcaoDoJogador::MoverCameraParaDireita, teclaEstaSegurada), "a seta direita segurada move para a direita");
    teclaEstaSegurada[SDL_SCANCODE_RSHIFT] = true;
    conferir(tabela.acaoEstaSegurada(AcaoDoJogador::CorrerComACamera, teclaEstaSegurada), "o Shift da direita também corre");
}
