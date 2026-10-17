// main.cpp: o começo do programa.
//
// Abre a janela, cria o relógio do mundo e repete o laço principal até o
// jogador fechar (no X ou com a tecla Esc). A cada quadro: lê as teclas,
// avança o relógio pelo tempo real e desenha.

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>  // deixa o main() funcionar como programa de janela no Windows

#include <string>

#include "Tempo/RelogioDoMundo.h"
#include "TextoDaDataDoJogo.h"

// Tamanho preferido da janela: a resolução de referência do jogo (1600 x 900).
constexpr int larguraPreferidaDaJanelaEmPixels = 1600;
constexpr int alturaPreferidaDaJanelaEmPixels = 900;

// Espaço que o Windows usa em volta da janela: as bordas dos lados e a barra
// de título em cima (onde ficam o X e o botão de maximizar).
constexpr int espacoDasBordasDosLadosEmPixels = 16;
constexpr int espacoDaBarraDeTituloEmPixels = 40;

// Cor do fundo, em vermelho, verde e azul (de 0 a 255).
constexpr Uint8 vermelhoDoFundo = 34;
constexpr Uint8 verdeDoFundo = 60;
constexpr Uint8 azulDoFundo = 34;

// O tamanho com que a janela abre, e se ela já abre maximizada.
struct TamanhoInicialDaJanela {
    int larguraEmPixels;
    int alturaEmPixels;
    bool abrirMaximizada;
};

// Escolhe o tamanho da janela para ela caber na tela com a barra de título
// visível. Por quê: numa tela de 1600 x 900, uma janela de 1600 x 900 mais a
// barra de título não cabe, e o Windows esconde o X acima da tela.
TamanhoInicialDaJanela escolherTamanhoInicialDaJanela()
{
    // A "área livre" é a tela menos a barra de tarefas do Windows.
    SDL_Rect areaLivreDaTela{};
    if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &areaLivreDaTela)) {
        // Sem saber o tamanho da tela, usa um tamanho que cabe em quase todas.
        return {1280, 720, false};
    }

    const bool tamanhoPreferidoCabeNaTela =
        larguraPreferidaDaJanelaEmPixels + espacoDasBordasDosLadosEmPixels <= areaLivreDaTela.w &&
        alturaPreferidaDaJanelaEmPixels + espacoDaBarraDeTituloEmPixels <= areaLivreDaTela.h;
    if (tamanhoPreferidoCabeNaTela) {
        return {larguraPreferidaDaJanelaEmPixels, alturaPreferidaDaJanelaEmPixels, false};
    }

    // Não cabe: a janela abre maximizada (ocupa a área livre, com o X à vista).
    // Ao "restaurar", ela fica com 90% da área livre, no formato 16:9.
    int larguraAoRestaurar = areaLivreDaTela.w * 9 / 10;
    int alturaAoRestaurar = larguraAoRestaurar * 9 / 16;
    const int alturaMaximaAoRestaurar = areaLivreDaTela.h * 9 / 10;
    if (alturaAoRestaurar > alturaMaximaAoRestaurar) {
        alturaAoRestaurar = alturaMaximaAoRestaurar;
        larguraAoRestaurar = alturaAoRestaurar * 16 / 9;
    }
    return {larguraAoRestaurar, alturaAoRestaurar, true};
}

// Escreve no console as viradas do tick que acabou de rodar (no marco 1,
// é só isso que acontece nas viradas; as regras se penduram aqui depois).
void escreverAsViradasNoConsole(const RelogioDoMundo& relogio)
{
    const char* nomeDaEstacao = nomeDaEstacaoParaMostrar(relogio.estacaoAtual());
    if (relogio.comecouUmAnoNovo()) {
        SDL_Log("Começou o Ano %d.", relogio.anoAtual());
    }
    if (relogio.comecouUmaEstacaoNova()) {
        SDL_Log("Começou a estação: %s.", nomeDaEstacao);
    }
    if (relogio.comecouUmDiaNovo()) {
        SDL_Log("Começou o dia %d da estação %s, Ano %d.", relogio.diaDaEstacaoAtual(), nomeDaEstacao, relogio.anoAtual());
    }
    if (relogio.comecouUmaHoraNova()) {
        SDL_Log("  %s (%s)", montarTextoDaHora(relogio).c_str(), nomeDaFaseDoDiaParaMostrar(relogio.faseDoDiaAtual()));
    }
}

// As teclas provisórias do marco 1 (FL1-3) que mexem no relógio.
void tratarTeclaDoRelogio(SDL_Keycode teclaApertada, RelogioDoMundo& relogio)
{
    if (teclaApertada == SDLK_SPACE) {
        relogio.alternarPausa();
    } else if (teclaApertada == SDLK_1) {
        relogio.definirVelocidadeDoJogo(1);
    } else if (teclaApertada == SDLK_2) {
        relogio.definirVelocidadeDoJogo(2);
    } else if (teclaApertada == SDLK_3) {
        relogio.definirVelocidadeDoJogo(4);
    } else {
        return;
    }
    if (relogio.jogoEstaPausado()) {
        SDL_Log("Jogo pausado.");
    } else {
        SDL_Log("Velocidade: %dx.", relogio.velocidadeAtualDoJogo());
    }
}

int main(int quantidadeDeArgumentos, char* argumentosDaLinhaDeComando[])
{
    // Ainda não lê nada da linha de comando (o --simular vem no marco 7).
    (void)quantidadeDeArgumentos;
    (void)argumentosDaLinhaDeComando;

    // 1. Liga a parte de vídeo da SDL (janelas e desenho).
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Não foi possível ligar a SDL: %s", SDL_GetError());
        return 1;
    }

    // 2. Cria a janela e o "desenhista" (renderer), que pinta dentro dela.
    const TamanhoInicialDaJanela tamanhoInicial = escolherTamanhoInicialDaJanela();
    SDL_Window* janelaDoJogo = nullptr;
    SDL_Renderer* desenhistaDaJanela = nullptr;
    const bool janelaFoiCriada = SDL_CreateWindowAndRenderer(
        "Projeto Jogo",
        tamanhoInicial.larguraEmPixels,
        tamanhoInicial.alturaEmPixels,
        SDL_WINDOW_RESIZABLE,
        &janelaDoJogo,
        &desenhistaDaJanela);
    if (!janelaFoiCriada) {
        SDL_Log("Não foi possível criar a janela: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (tamanhoInicial.abrirMaximizada) {
        SDL_MaximizeWindow(janelaDoJogo);
    }

    // Espera o monitor terminar cada imagem antes de desenhar a próxima
    // (sincronia vertical). Sem isso, o laço abaixo usaria 100% do processador.
    SDL_SetRenderVSync(desenhistaDaJanela, 1);

    SDL_Log("Olá, janela! Esc ou o X fecham; F11 liga a tela cheia.");
    SDL_Log("Espaço pausa e despausa; 1, 2 e 3 escolhem 1x, 2x e 4x.");

    // 3. O relógio do mundo de uma partida nova (6h do dia 1 da Primavera).
    RelogioDoMundo relogioDoMundo = RelogioDoMundo::criarParaPartidaNova(RegrasDoTempo{});
    std::string tituloMostradoNaJanela;
    Uint64 instanteDoQuadroAnteriorEmNanossegundos = SDL_GetTicksNS();

    // 4. O laço principal: roda uma vez por quadro, até o jogador fechar.
    bool jogadorPediuParaFechar = false;
    while (!jogadorPediuParaFechar) {
        // 4a. Lê tudo o que aconteceu desde o último quadro (teclas, mouse, janela).
        SDL_Event eventoRecebido;
        while (SDL_PollEvent(&eventoRecebido)) {
            // O X da janela (ou "Fechar janela" na barra de tarefas).
            if (eventoRecebido.type == SDL_EVENT_QUIT ||
                eventoRecebido.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                jogadorPediuParaFechar = true;
            }
            if (eventoRecebido.type == SDL_EVENT_KEY_DOWN && eventoRecebido.key.key == SDLK_ESCAPE) {
                jogadorPediuParaFechar = true;
            }
            // F11, tecla provisória: tela cheia, sem bordas, do tamanho do monitor.
            if (eventoRecebido.type == SDL_EVENT_KEY_DOWN && eventoRecebido.key.key == SDLK_F11) {
                const bool janelaEstaEmTelaCheia = (SDL_GetWindowFlags(janelaDoJogo) & SDL_WINDOW_FULLSCREEN) != 0;
                SDL_SetWindowFullscreen(janelaDoJogo, !janelaEstaEmTelaCheia);
            }
            // Teclas do relógio. O "repeat" ignora a repetição de quem segura a tecla.
            if (eventoRecebido.type == SDL_EVENT_KEY_DOWN && !eventoRecebido.key.repeat) {
                tratarTeclaDoRelogio(eventoRecebido.key.key, relogioDoMundo);
            }
            // Janela minimizada: pausa sozinho (FL1-3). Ao voltar, continua pausado.
            if (eventoRecebido.type == SDL_EVENT_WINDOW_MINIMIZED && !relogioDoMundo.jogoEstaPausado()) {
                relogioDoMundo.alternarPausa();
                SDL_Log("Jogo pausado porque a janela foi minimizada.");
            }
        }

        // 4b. Avança o relógio pelo tempo real que passou, um tick por vez,
        //     conferindo as viradas depois de cada tick.
        const Uint64 instanteDesteQuadroEmNanossegundos = SDL_GetTicksNS();
        const int64_t nanossegundosDesdeOQuadroAnterior =
            static_cast<int64_t>(instanteDesteQuadroEmNanossegundos - instanteDoQuadroAnteriorEmNanossegundos);
        instanteDoQuadroAnteriorEmNanossegundos = instanteDesteQuadroEmNanossegundos;

        const int ticksParaAvancar = relogioDoMundo.calcularTicksParaAvancarPeloTempoReal(nanossegundosDesdeOQuadroAnterior);
        for (int tick = 0; tick < ticksParaAvancar; ++tick) {
            relogioDoMundo.avancarUmTick();
            escreverAsViradasNoConsole(relogioDoMundo);
        }

        // 4c. A data no título da janela, trocada só quando o texto muda.
        const std::string tituloDesteQuadro = montarTextoDoTituloDaJanela(relogioDoMundo);
        if (tituloDesteQuadro != tituloMostradoNaJanela) {
            SDL_SetWindowTitle(janelaDoJogo, tituloDesteQuadro.c_str());
            tituloMostradoNaJanela = tituloDesteQuadro;
        }

        // 4d. Pinta o fundo inteiro e mostra o quadro na tela.
        SDL_SetRenderDrawColor(desenhistaDaJanela, vermelhoDoFundo, verdeDoFundo, azulDoFundo, 255);
        SDL_RenderClear(desenhistaDaJanela);
        SDL_RenderPresent(desenhistaDaJanela);
    }

    SDL_Log("Janela fechada. Até a próxima!");

    // 5. Devolve ao sistema tudo o que foi pedido, na ordem contrária.
    SDL_DestroyRenderer(desenhistaDaJanela);
    SDL_DestroyWindow(janelaDoJogo);
    SDL_Quit();
    return 0;
}
