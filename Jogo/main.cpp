// main.cpp: o começo do programa (marco 0).
//
// Abre uma janela vazia com a SDL3, pinta o fundo de verde-escuro e espera
// o jogador fechar a janela (no X ou com a tecla Esc).
// Ainda não há regra de jogo: o relógio e o mapa chegam no marco 1.

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>  // deixa o main() funcionar como programa de janela no Windows

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
    const TamanhoInicialDaJanela tamanhoInicial = escolherTamanhoInicialDaJanela();
    SDL_Window* janelaDoJogo = nullptr;
    SDL_Renderer* desenhistaDaJanela = nullptr;
    const bool janelaFoiCriada = SDL_CreateWindowAndRenderer(
        "Projeto Jogo (marco 0)",
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

    SDL_Log("Olá, janela! Feche no X ou aperte Esc para sair. F11 liga e desliga a tela cheia.");

    // 3. O laço principal: roda uma vez por quadro, até o jogador fechar.
    bool jogadorPediuParaFechar = false;
    while (!jogadorPediuParaFechar) {
        // 3a. Lê tudo o que aconteceu desde o último quadro (teclas, mouse, janela).
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
