#include "AcoesDoTeclado.h"

TabelaDeAcoesDoTeclado TabelaDeAcoesDoTeclado::criarComAsTeclasPadrao()
{
    TabelaDeAcoesDoTeclado tabela;
    tabela.teclasDeCadaAcao = {
        {AcaoDoJogador::FecharOJogo, SDL_SCANCODE_ESCAPE},
        {AcaoDoJogador::AlternarTelaCheia, SDL_SCANCODE_F11},
        {AcaoDoJogador::MostrarColisaoParaDepuracao, SDL_SCANCODE_F1},
        {AcaoDoJogador::PausarOuDespausar, SDL_SCANCODE_SPACE},
        {AcaoDoJogador::Velocidade1x, SDL_SCANCODE_1},
        {AcaoDoJogador::Velocidade2x, SDL_SCANCODE_2},
        {AcaoDoJogador::Velocidade4x, SDL_SCANCODE_3},
        {AcaoDoJogador::MoverCameraParaCima, SDL_SCANCODE_W, SDL_SCANCODE_UP},
        {AcaoDoJogador::MoverCameraParaBaixo, SDL_SCANCODE_S, SDL_SCANCODE_DOWN},
        {AcaoDoJogador::MoverCameraParaEsquerda, SDL_SCANCODE_A, SDL_SCANCODE_LEFT},
        {AcaoDoJogador::MoverCameraParaDireita, SDL_SCANCODE_D, SDL_SCANCODE_RIGHT},
        {AcaoDoJogador::CorrerComACamera, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT},
        {AcaoDoJogador::CriarNpcDeTeste, SDL_SCANCODE_N},
        {AcaoDoJogador::MostrarCaminhosParaDepuracao, SDL_SCANCODE_F2},
    };
    return tabela;
}

std::optional<AcaoDoJogador> TabelaDeAcoesDoTeclado::acaoDaTeclaApertada(SDL_Scancode teclaApertada) const
{
    if (teclaApertada == SDL_SCANCODE_UNKNOWN) {
        return std::nullopt;
    }
    for (const TeclasDeUmaAcao& linha : teclasDeCadaAcao) {
        if (linha.teclaPrincipal == teclaApertada || linha.teclaAlternativa == teclaApertada) {
            return linha.acao;
        }
    }
    return std::nullopt;
}

bool TabelaDeAcoesDoTeclado::acaoEstaSegurada(AcaoDoJogador acao, const bool* teclaEstaSegurada) const
{
    for (const TeclasDeUmaAcao& linha : teclasDeCadaAcao) {
        if (linha.acao != acao) {
            continue;
        }
        if (teclaEstaSegurada[linha.teclaPrincipal]) {
            return true;
        }
        if (linha.teclaAlternativa != SDL_SCANCODE_UNKNOWN && teclaEstaSegurada[linha.teclaAlternativa]) {
            return true;
        }
    }
    return false;
}
