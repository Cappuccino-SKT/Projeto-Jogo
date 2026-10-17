// main.cpp: o começo do programa.
//
// Abre a janela com o OpenGL 3.3, lê o mapa da vila, cria o relógio do mundo
// e repete o laço principal até o jogador fechar (no X ou com a tecla Esc).
// A cada quadro: lê as teclas, avança o relógio pelo tempo real e desenha.

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>  // deixa o main() funcionar como programa de janela no Windows

#include <glad/gl.h>

#include <string>

#include "DesenhoDoMapa.h"
#include "Mapa/MapaLocal.h"
#include "Tempo/RelogioDoMundo.h"
#include "TextoDaDataDoJogo.h"

// Tamanho preferido da janela: a resolução de referência do jogo (1600 x 900).
constexpr int larguraPreferidaDaJanelaEmPixels = 1600;
constexpr int alturaPreferidaDaJanelaEmPixels = 900;

// Espaço que o Windows usa em volta da janela: as bordas dos lados e a barra
// de título em cima (onde ficam o X e o botão de maximizar).
constexpr int espacoDasBordasDosLadosEmPixels = 16;
constexpr int espacoDaBarraDeTituloEmPixels = 40;

// Cor do fundo, atrás do mapa (de 0 a 1: vermelho, verde, azul).
constexpr float corDoFundoAtrasDoMapa[3] = {0.05f, 0.07f, 0.05f};

// O arquivo do mapa da vila (FL1-1). PASTA_DOS_DADOS_DO_JOGO vem do CMake.
const std::string caminhoDoMapaDaVila = std::string(PASTA_DOS_DADOS_DO_JOGO) + "/mapas/vila.tmj";

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

// O glad pede as funções do OpenGL por nome; quem sabe onde elas estão é a SDL.
GLADapiproc buscarFuncaoDoOpenGL(const char* nomeDaFuncao)
{
    return reinterpret_cast<GLADapiproc>(SDL_GL_GetProcAddress(nomeDaFuncao));
}

// Mostra um erro grave numa caixa de mensagem (e no console) antes de fechar.
void mostrarErroGrave(const std::string& mensagem)
{
    SDL_Log("%s", mensagem.c_str());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Projeto Jogo", mensagem.c_str(), nullptr);
}

// Lê o mapa da vila. Se houver erros, mostra todos de uma vez e devolve false.
bool lerMapaDaVila(MapaLocal& mapaDaVila)
{
    ResultadoDaLeituraDoMapa resultado = carregarMapaDoArquivoDoTiled(caminhoDoMapaDaVila);
    for (const std::string& aviso : resultado.listaDeAvisosDoMapa) {
        SDL_Log("Aviso do mapa: %s", aviso.c_str());
    }
    if (!resultado.mapaEstaPronto()) {
        std::string todosOsErros = "O mapa da vila tem erros:\n";
        for (const std::string& erro : resultado.listaDeErrosDoMapa) {
            todosOsErros += "- " + erro + "\n";
        }
        mostrarErroGrave(todosOsErros);
        return false;
    }
    mapaDaVila = std::move(resultado.mapa);
    SDL_Log("Mapa da vila lido: %d x %d tiles, %d lotes, %d áreas de recurso.", mapaDaVila.larguraEmTiles, mapaDaVila.alturaEmTiles,
            static_cast<int>(mapaDaVila.lotesDeConstrucao.size()), static_cast<int>(mapaDaVila.areasDeRecurso.size()));
    return true;
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

    // 1. Lê o mapa antes de abrir a janela: se ele tiver erro, nem abre.
    MapaLocal mapaDaVila;
    if (!lerMapaDaVila(mapaDaVila)) {
        return 1;
    }

    // 2. Liga a parte de vídeo da SDL e pede o OpenGL 3.3 "core" (sem as funções antigas).
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        mostrarErroGrave(std::string("Não foi possível ligar a SDL: ") + SDL_GetError());
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // 3. Cria a janela e liga o OpenGL nela.
    const TamanhoInicialDaJanela tamanhoInicial = escolherTamanhoInicialDaJanela();
    SDL_Window* janelaDoJogo = SDL_CreateWindow("Projeto Jogo", tamanhoInicial.larguraEmPixels, tamanhoInicial.alturaEmPixels,
                                                SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (janelaDoJogo == nullptr) {
        mostrarErroGrave(std::string("Não foi possível criar a janela: ") + SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (tamanhoInicial.abrirMaximizada) {
        SDL_MaximizeWindow(janelaDoJogo);
    }
    SDL_GLContext ligacaoComOOpenGL = SDL_GL_CreateContext(janelaDoJogo);
    if (ligacaoComOOpenGL == nullptr) {
        mostrarErroGrave(std::string("Não foi possível ligar o OpenGL 3.3 (a placa de vídeo precisa dele): ") + SDL_GetError());
        SDL_DestroyWindow(janelaDoJogo);
        SDL_Quit();
        return 1;
    }
    const int versaoDoOpenGL = gladLoadGL(buscarFuncaoDoOpenGL);
    if (versaoDoOpenGL == 0) {
        mostrarErroGrave("Não foi possível carregar as funções do OpenGL.");
        SDL_GL_DestroyContext(ligacaoComOOpenGL);
        SDL_DestroyWindow(janelaDoJogo);
        SDL_Quit();
        return 1;
    }
    SDL_Log("OpenGL %d.%d ligado (%s).", GLAD_VERSION_MAJOR(versaoDoOpenGL), GLAD_VERSION_MINOR(versaoDoOpenGL),
            reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    // Espera o monitor terminar cada imagem antes de desenhar a próxima
    // (sincronia vertical). Sem isso, o laço abaixo usaria 100% do processador.
    SDL_GL_SetSwapInterval(1);

    // 4. Prepara o desenho do mapa (programa da placa de vídeo e imagens).
    DesenhistaDoMapa desenhistaDoMapa;
    std::string mensagemDeErroDoDesenho;
    if (!desenhistaDoMapa.preparar(mapaDaVila, mensagemDeErroDoDesenho)) {
        mostrarErroGrave("Não foi possível preparar o desenho do mapa: " + mensagemDeErroDoDesenho);
        desenhistaDoMapa.liberar();
        SDL_GL_DestroyContext(ligacaoComOOpenGL);
        SDL_DestroyWindow(janelaDoJogo);
        SDL_Quit();
        return 1;
    }
    const CameraDoMapa cameraDoMapa;  // parada no canto de cima à esquerda, zoom 3x (marco 2: câmera que anda)
    bool mostrarColisaoParaDepuracao = false;

    SDL_Log("Esc ou o X fecham; F11 liga a tela cheia; F1 mostra a colisão.");
    SDL_Log("Espaço pausa e despausa; 1, 2 e 3 escolhem 1x, 2x e 4x.");

    // 5. O relógio do mundo de uma partida nova (6h do dia 1 da Primavera).
    RelogioDoMundo relogioDoMundo = RelogioDoMundo::criarParaPartidaNova(RegrasDoTempo{});
    std::string tituloMostradoNaJanela;
    Uint64 instanteDoQuadroAnteriorEmNanossegundos = SDL_GetTicksNS();

    // 6. O laço principal: roda uma vez por quadro, até o jogador fechar.
    bool jogadorPediuParaFechar = false;
    while (!jogadorPediuParaFechar) {
        // 6a. Lê tudo o que aconteceu desde o último quadro (teclas, mouse, janela).
        SDL_Event eventoRecebido;
        while (SDL_PollEvent(&eventoRecebido)) {
            // O X da janela (ou "Fechar janela" na barra de tarefas).
            if (eventoRecebido.type == SDL_EVENT_QUIT || eventoRecebido.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                jogadorPediuParaFechar = true;
            }
            if (eventoRecebido.type == SDL_EVENT_KEY_DOWN && !eventoRecebido.key.repeat) {
                const SDL_Keycode teclaApertada = eventoRecebido.key.key;
                if (teclaApertada == SDLK_ESCAPE) {
                    jogadorPediuParaFechar = true;
                } else if (teclaApertada == SDLK_F11) {
                    // Tecla provisória: tela cheia, sem bordas, do tamanho do monitor.
                    const bool janelaEstaEmTelaCheia = (SDL_GetWindowFlags(janelaDoJogo) & SDL_WINDOW_FULLSCREEN) != 0;
                    SDL_SetWindowFullscreen(janelaDoJogo, !janelaEstaEmTelaCheia);
                } else if (teclaApertada == SDLK_F1) {
                    // Tecla provisória (FL1-3): mostra os tiles bloqueados em vermelho.
                    mostrarColisaoParaDepuracao = !mostrarColisaoParaDepuracao;
                } else {
                    tratarTeclaDoRelogio(teclaApertada, relogioDoMundo);
                }
            }
            // Janela minimizada: pausa sozinho (FL1-3). Ao voltar, continua pausado.
            if (eventoRecebido.type == SDL_EVENT_WINDOW_MINIMIZED && !relogioDoMundo.jogoEstaPausado()) {
                relogioDoMundo.alternarPausa();
                SDL_Log("Jogo pausado porque a janela foi minimizada.");
            }
        }

        // 6b. Avança o relógio pelo tempo real que passou, um tick por vez,
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

        // 6c. A data no título da janela, trocada só quando o texto muda.
        const std::string tituloDesteQuadro = montarTextoDoTituloDaJanela(relogioDoMundo);
        if (tituloDesteQuadro != tituloMostradoNaJanela) {
            SDL_SetWindowTitle(janelaDoJogo, tituloDesteQuadro.c_str());
            tituloMostradoNaJanela = tituloDesteQuadro;
        }

        // 6d. Desenha o quadro: fundo, mapa (personagens no meio, a partir do marco 4) e depuração.
        int larguraDaTelaEmPixels = 0;
        int alturaDaTelaEmPixels = 0;
        SDL_GetWindowSizeInPixels(janelaDoJogo, &larguraDaTelaEmPixels, &alturaDaTelaEmPixels);
        glViewport(0, 0, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
        glClearColor(corDoFundoAtrasDoMapa[0], corDoFundoAtrasDoMapa[1], corDoFundoAtrasDoMapa[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        desenhistaDoMapa.desenharCamadasAbaixoDosPersonagens(mapaDaVila, cameraDoMapa, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
        desenhistaDoMapa.desenharCamadasAcimaDosPersonagens(mapaDaVila, cameraDoMapa, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
        if (mostrarColisaoParaDepuracao) {
            desenhistaDoMapa.desenharTilesBloqueadosParaDepuracao(mapaDaVila, cameraDoMapa, larguraDaTelaEmPixels, alturaDaTelaEmPixels);
        }

        SDL_GL_SwapWindow(janelaDoJogo);
    }

    SDL_Log("Janela fechada. Até a próxima!");

    // 7. Devolve ao sistema tudo o que foi pedido, na ordem contrária.
    desenhistaDoMapa.liberar();
    SDL_GL_DestroyContext(ligacaoComOOpenGL);
    SDL_DestroyWindow(janelaDoJogo);
    SDL_Quit();
    return 0;
}
