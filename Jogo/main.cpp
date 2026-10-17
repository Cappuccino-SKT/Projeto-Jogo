// main.cpp: o começo do programa (marco 0).
//
// Abre uma janela vazia com a SDL3, pinta o fundo de verde-escuro e espera
// o jogador fechar a janela (no X ou com a tecla Esc).
// Ainda não há regra de jogo: o relógio e o mapa chegam no marco 1.

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>  // deixa o main() funcionar como programa de janela no Windows

// Tamanho da janela ao abrir: a resolução em que o autor joga (1600 x 900).
constexpr int larguraInicialDaJanelaEmPixels = 1600;
constexpr int alturaInicialDaJanelaEmPixels = 900;

// Cor do fundo, em vermelho, verde e azul (de 0 a 255).
constexpr Uint8 vermelhoDoFundo = 34;
constexpr Uint8 verdeDoFundo = 60;
constexpr Uint8 azulDoFundo = 34;

int main(int quantidadeDeArgumentos, char* argumentosDaLinhaDeComando[])
{
    // O marco 0 ainda não lê nada da linha de comando (o --simular vem no marco 7).
    (void)quantidadeDeArgumentos;
    (void)argumentosDaLinhaDeComando;

    // 1. Liga a parte de vídeo da SDL (janelas e desenho).
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Não foi possível ligar a SDL: %s", SDL_GetError());
        return 1;
    }

    // 2. Cria a janela e o "desenhista" (renderer), que pinta dentro dela.
    SDL_Window* janelaDoJogo = nullptr;
    SDL_Renderer* desenhistaDaJanela = nullptr;
    const bool janelaFoiCriada = SDL_CreateWindowAndRenderer(
        "Projeto Jogo (marco 0)",
        larguraInicialDaJanelaEmPixels,
        alturaInicialDaJanelaEmPixels,
        SDL_WINDOW_RESIZABLE,
        &janelaDoJogo,
        &desenhistaDaJanela);
    if (!janelaFoiCriada) {
        SDL_Log("Não foi possível criar a janela: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Espera o monitor terminar cada imagem antes de desenhar a próxima
    // (sincronia vertical). Sem isso, o laço abaixo usaria 100% do processador.
    SDL_SetRenderVSync(desenhistaDaJanela, 1);

    SDL_Log("Olá, janela! Feche no X ou aperte Esc para sair.");

    // 3. O laço principal: roda uma vez por quadro, até o jogador fechar.
    bool jogadorPediuParaFechar = false;
    while (!jogadorPediuParaFechar) {
        // 3a. Lê tudo o que aconteceu desde o último quadro (teclas, mouse, janela).
        SDL_Event eventoRecebido;
        while (SDL_PollEvent(&eventoRecebido)) {
            if (eventoRecebido.type == SDL_EVENT_QUIT) {
                jogadorPediuParaFechar = true;
            }
            if (eventoRecebido.type == SDL_EVENT_KEY_DOWN && eventoRecebido.key.key == SDLK_ESCAPE) {
                jogadorPediuParaFechar = true;
            }
        }

        // 3b. Pinta o fundo inteiro e mostra o quadro na tela.
        SDL_SetRenderDrawColor(desenhistaDaJanela, vermelhoDoFundo, verdeDoFundo, azulDoFundo, 255);
        SDL_RenderClear(desenhistaDaJanela);
        SDL_RenderPresent(desenhistaDaJanela);
    }

    SDL_Log("Janela fechada. Até a próxima!");

    // 4. Devolve ao sistema tudo o que foi pedido, na ordem contrária.
    SDL_DestroyRenderer(desenhistaDaJanela);
    SDL_DestroyWindow(janelaDoJogo);
    SDL_Quit();
    return 0;
}
