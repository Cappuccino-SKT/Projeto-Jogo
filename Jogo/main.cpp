// main.cpp: o começo do programa.
//
// Abre a janela com o OpenGL 3.3, lê o mapa da vila, cria o relógio do mundo
// e repete o laço principal até o jogador fechar (no X ou com a tecla Esc).
// A cada quadro: lê as teclas, avança o relógio pelo tempo real e desenha.

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>  // deixa o main() funcionar como programa de janela no Windows

#include <glad/gl.h>

#include <algorithm>
#include <optional>
#include <string>

#include "AcoesDoTeclado.h"
#include "Dados/DadosDoJogo.h"
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

// A pasta dos dados (Jogo/data). PASTA_DOS_DADOS_DO_JOGO vem do CMake.
const std::string pastaDosDadosDoJogo = PASTA_DOS_DADOS_DO_JOGO;

// Quantos problemas cabem na caixa de mensagem; o resto fica só no console.
constexpr int limiteDeProblemasNaCaixaDeMensagem = 25;

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

// Escreve no console cada erro e aviso dos dados, o resumo e os preços base.
void escreverProblemasEResumoNoConsole(const DadosDoJogo& dadosDoJogo)
{
    for (const ProblemaDosDados& problema : dadosDoJogo.listaDeProblemas.todosOsProblemas()) {
        SDL_Log("%s %s", problema.ehErro ? "ERRO: " : "Aviso:", problema.textoParaMostrar().c_str());
    }
    SDL_Log("%s", montarResumoDosDados(dadosDoJogo).c_str());
    if (dadosDoJogo.dadosEstaoProntos()) {
        SDL_Log("Preços base:\n%s", montarTabelaDePrecosBase(dadosDoJogo).c_str());
    }
}

// Lê todos os dados (ficha dos Itens, parte 3). Se houver erros, mostra todos
// de uma vez numa caixa de mensagem e devolve false.
bool lerDadosDoJogo(DadosDoJogo& dadosDoJogo)
{
    dadosDoJogo = carregarDadosDoJogo(pastaDosDadosDoJogo);
    escreverProblemasEResumoNoConsole(dadosDoJogo);
    if (!dadosDoJogo.dadosEstaoProntos()) {
        std::string todosOsErros = "Os dados do jogo têm erros:\n";
        int errosMostrados = 0;
        for (const ProblemaDosDados& problema : dadosDoJogo.listaDeProblemas.todosOsProblemas()) {
            if (problema.ehErro && errosMostrados < limiteDeProblemasNaCaixaDeMensagem) {
                todosOsErros += "- " + problema.textoParaMostrar() + "\n";
                ++errosMostrados;
            }
        }
        if (dadosDoJogo.listaDeProblemas.quantidadeDeErros() > errosMostrados) {
            todosOsErros += "(e mais " + std::to_string(dadosDoJogo.listaDeProblemas.quantidadeDeErros() - errosMostrados) + "; veja o console)\n";
        }
        mostrarErroGrave(todosOsErros);
        return false;
    }
    return true;
}

// O modo "jogo --validar" (ficha dos Itens, regra 4.4): lê e confere todos os
// dados, sem abrir janela, e termina com 0 (tudo certo) ou 1 (há erro).
int validarDadosSemJanela()
{
    const DadosDoJogo dadosDoJogo = carregarDadosDoJogo(pastaDosDadosDoJogo);
    escreverProblemasEResumoNoConsole(dadosDoJogo);
    return dadosDoJogo.dadosEstaoProntos() ? 0 : 1;
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

// A direção em que a câmera anda pelas teclas seguradas (ficha da Interface,
// regra 4.1). Usa a posição da tecla (scancode): o W é a tecla do W em
// qualquer layout de teclado.
struct DirecaoDasTeclasSeguradas {
    int direcaoX = 0;
    int direcaoY = 0;
    bool correndoComShift = false;
};

DirecaoDasTeclasSeguradas lerDirecaoDasTeclasSeguradas(const TabelaDeAcoesDoTeclado& tabelaDeAcoes)
{
    const bool* teclaEstaSegurada = SDL_GetKeyboardState(nullptr);
    DirecaoDasTeclasSeguradas direcao;
    if (tabelaDeAcoes.acaoEstaSegurada(AcaoDoJogador::MoverCameraParaCima, teclaEstaSegurada)) {
        direcao.direcaoY -= 1;
    }
    if (tabelaDeAcoes.acaoEstaSegurada(AcaoDoJogador::MoverCameraParaBaixo, teclaEstaSegurada)) {
        direcao.direcaoY += 1;
    }
    if (tabelaDeAcoes.acaoEstaSegurada(AcaoDoJogador::MoverCameraParaEsquerda, teclaEstaSegurada)) {
        direcao.direcaoX -= 1;
    }
    if (tabelaDeAcoes.acaoEstaSegurada(AcaoDoJogador::MoverCameraParaDireita, teclaEstaSegurada)) {
        direcao.direcaoX += 1;
    }
    direcao.correndoComShift = tabelaDeAcoes.acaoEstaSegurada(AcaoDoJogador::CorrerComACamera, teclaEstaSegurada);
    return direcao;
}

// As ações que mexem no relógio (pausa e velocidades).
void tratarAcaoDoRelogio(AcaoDoJogador acao, RelogioDoMundo& relogio)
{
    if (acao == AcaoDoJogador::PausarOuDespausar) {
        relogio.alternarPausa();
    } else if (acao == AcaoDoJogador::Velocidade1x) {
        relogio.definirVelocidadeDoJogo(1);
    } else if (acao == AcaoDoJogador::Velocidade2x) {
        relogio.definirVelocidadeDoJogo(2);
    } else if (acao == AcaoDoJogador::Velocidade4x) {
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
    // 0. A linha de comando: "jogo --validar" só confere os dados (o --simular vem no marco 7).
    for (int posicao = 1; posicao < quantidadeDeArgumentos; ++posicao) {
        if (std::string(argumentosDaLinhaDeComando[posicao]) == "--validar") {
            return validarDadosSemJanela();
        }
    }

    // 1. Lê todos os dados (e o mapa) antes de abrir a janela: se houver erro, nem abre.
    DadosDoJogo dadosDoJogo;
    if (!lerDadosDoJogo(dadosDoJogo)) {
        return 1;
    }
    MapaLocal& mapaDaVila = dadosDoJogo.mapaDaVila;

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
    // A câmera (ficha da Interface): zoom 3x, centralizada no primeiro quadro,
    // quando o tamanho da janela já é o certo.
    CameraDoMapa cameraDoMapa;
    cameraDoMapa.definirTamanhoDoMapa(mapaDaVila.larguraEmTiles * mapaDaVila.tamanhoDoTileEmPixels,
                                      mapaDaVila.alturaEmTiles * mapaDaVila.tamanhoDoTileEmPixels);
    bool cameraJaFoiCentralizada = false;
    bool mostrarColisaoParaDepuracao = false;
    const TabelaDeAcoesDoTeclado tabelaDeAcoes = TabelaDeAcoesDoTeclado::criarComAsTeclasPadrao();

    SDL_Log("Esc ou o X fecham; F11 liga a tela cheia; F1 mostra a colisão.");
    SDL_Log("Espaço pausa e despausa; 1, 2 e 3 escolhem 1x, 2x e 4x.");
    SDL_Log("Câmera: WASD ou setas (Shift corre), roda do mouse para o zoom, botão do meio para arrastar.");

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
            // As teclas apertadas viram ações pela tabela (FL2-4).
            if (eventoRecebido.type == SDL_EVENT_KEY_DOWN && !eventoRecebido.key.repeat) {
                const std::optional<AcaoDoJogador> acao = tabelaDeAcoes.acaoDaTeclaApertada(eventoRecebido.key.scancode);
                if (acao == AcaoDoJogador::FecharOJogo) {
                    jogadorPediuParaFechar = true;
                } else if (acao == AcaoDoJogador::AlternarTelaCheia) {
                    // Tela cheia, sem bordas, do tamanho do monitor.
                    const bool janelaEstaEmTelaCheia = (SDL_GetWindowFlags(janelaDoJogo) & SDL_WINDOW_FULLSCREEN) != 0;
                    SDL_SetWindowFullscreen(janelaDoJogo, !janelaEstaEmTelaCheia);
                } else if (acao == AcaoDoJogador::MostrarColisaoParaDepuracao) {
                    // Mostra os tiles bloqueados em vermelho (FL1-3).
                    mostrarColisaoParaDepuracao = !mostrarColisaoParaDepuracao;
                } else if (acao.has_value()) {
                    tratarAcaoDoRelogio(*acao, relogioDoMundo);
                }
            }
            // A câmera pelo mouse. A SDL mede o mouse em "pontos" da janela; a
            // densidade converte para pixels da tela (é 1 num monitor comum).
            const float densidadeDePixels = SDL_GetWindowPixelDensity(janelaDoJogo);
            if (eventoRecebido.type == SDL_EVENT_MOUSE_WHEEL) {
                float quantoARodaGirou = eventoRecebido.wheel.y;
                if (eventoRecebido.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                    quantoARodaGirou = -quantoARodaGirou;
                }
                const bool zoomMudou = cameraDoMapa.mudarZoomPelaRoda(quantoARodaGirou,
                                                                      eventoRecebido.wheel.mouse_x * densidadeDePixels,
                                                                      eventoRecebido.wheel.mouse_y * densidadeDePixels);
                if (zoomMudou) {
                    SDL_Log("Zoom: %dx.", cameraDoMapa.zoomAtual());
                }
            }
            if (eventoRecebido.type == SDL_EVENT_MOUSE_BUTTON_DOWN && eventoRecebido.button.button == SDL_BUTTON_MIDDLE) {
                cameraDoMapa.comecarArrasto();
            }
            if (eventoRecebido.type == SDL_EVENT_MOUSE_BUTTON_UP && eventoRecebido.button.button == SDL_BUTTON_MIDDLE) {
                cameraDoMapa.terminarArrasto();
            }
            if (eventoRecebido.type == SDL_EVENT_MOUSE_MOTION) {
                cameraDoMapa.arrastarPeloMouse(eventoRecebido.motion.xrel * densidadeDePixels,
                                               eventoRecebido.motion.yrel * densidadeDePixels);
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

        // 6d. A câmera: tamanho da tela, teclas seguradas e o limite ao mapa.
        //     Ela anda pelo tempo real (também com o jogo pausado), com o mesmo
        //     limite de 250 milissegundos do relógio para um quadro travado.
        int larguraDaTelaEmPixels = 0;
        int alturaDaTelaEmPixels = 0;
        SDL_GetWindowSizeInPixels(janelaDoJogo, &larguraDaTelaEmPixels, &alturaDaTelaEmPixels);
        cameraDoMapa.atualizarTamanhoDaTela(larguraDaTelaEmPixels, alturaDaTelaEmPixels);
        if (!cameraJaFoiCentralizada) {
            cameraDoMapa.centralizarNoMapa();
            cameraJaFoiCentralizada = true;
        }
        const double segundosReaisDoQuadro = std::min(nanossegundosDesdeOQuadroAnterior, int64_t{250'000'000}) / 1e9;
        const DirecaoDasTeclasSeguradas direcao = lerDirecaoDasTeclasSeguradas(tabelaDeAcoes);
        cameraDoMapa.moverPeloTeclado(direcao.direcaoX, direcao.direcaoY, segundosReaisDoQuadro, direcao.correndoComShift);
        cameraDoMapa.limitarAoMapa();

        // 6e. Desenha o quadro: fundo, mapa (personagens no meio, a partir do marco 4) e depuração.
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
