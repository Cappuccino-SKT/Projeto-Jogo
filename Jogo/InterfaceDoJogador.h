#pragma once

// A interface do jogador (Ficha Lógica: Interface, parte 10, marco 8): a barra
// de cima, as notificações, a barra de alertas, a janela de gestão e o painel
// da construção. Usa a Dear ImGui só para desenhar e ler os cliques (FL8-1):
// toda decisão vira um pedido na fila da simulação, e as contas e os textos
// vêm do Codigo/.

#include <SDL3/SDL.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "Vila/SimulacaoDaVila.h"

class InterfaceDoJogador {
public:
    // Liga a Dear ImGui na janela e no OpenGL e carrega a fonte (FL8-2).
    bool preparar(SDL_Window* janela, SDL_GLContext ligacaoComOOpenGL, const std::string& pastaDosAssets, std::string& mensagemDeErro);
    void liberar();

    // Entrega à ImGui cada evento da SDL (mouse, teclado, texto).
    void processarEvento(const SDL_Event& evento);
    // A interface está com o mouse ou o teclado (uma janela embaixo do mouse, um campo em edição).
    bool querOMouse() const;
    bool querOTeclado() const;

    // Monta e desenha o quadro da interface, por cima do mapa.
    // "desenharPorCima" (opcional) roda dentro do mesmo quadro, depois das janelas do jogo: o menu do Esc (marco 9).
    void desenharUmQuadro(SimulacaoDaVila& simulacao, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels,
                          const std::function<void()>& desenharPorCima = {});
    // Sem partida (a tela inicial): só o que "desenharPorCima" desenha.
    void desenharUmQuadroSemPartida(const std::function<void()>& desenharPorCima);
    // Uma partida nova foi carregada ou começada: esquece a seleção, a janela aberta e os preços guardados.
    void aoTrocarDePartida();

    // O jogador clicou num lote do mapa (-1 limpa a seleção).
    void selecionarOLote(int numeroDoLote) { loteSelecionado = numeroDoLote; }
    void alternarAJanelaDeGestao() { janelaDeGestaoAberta = !janelaDeGestaoAberta; }
    void alternarOMapaMundial() { mapaMundialAberto = !mapaMundialAberto; }
    // O Esc: fecha o painel, depois a gestão. Devolve false se não havia nada aberto (aí o Esc fecha o jogo).
    bool fecharOQueEstiverAberto();

private:
    void desenharABarraDeCima(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, float larguraDaTela);
    void desenharOsAlertas(const std::vector<std::string>& alertas, float larguraDaTela);
    void desenharAsNotificacoes(const SimulacaoDaVila& simulacao, float larguraDaTela);
    void desenharAJanelaDeGestao(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, float larguraDaTela, float alturaDaTela);
    void desenharOPainelDaConstrucao(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, float larguraDaTela, float alturaDaTela);

    void abaDaCidade(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto);
    void abaDasConstrucoes(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto);
    void abaDoInventario(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto);
    void abaDoMercado(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto);
    void abaDoDiario(const SimulacaoDaVila& simulacao);

    void desenharAsOrdensDoJogador(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, int indiceDaConstrucao);
    void desenharOLoteLivre(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, int numeroDoLote);
    // Fase B1 (ficha da Viagem): o Mapa Mundial e a janela do encontro.
    void desenharOMapaMundial(SimulacaoDaVila& simulacao, float larguraDaTela, float alturaDaTela);
    void desenharOEncontro(SimulacaoDaVila& simulacao, float larguraDaTela, float alturaDaTela);
    // Fase B3 (ficha das Notícias): as notícias que o jogador sabe e a Crônica do mundo.
    void desenharAsNoticias(const SimulacaoDaVila& simulacao, float larguraDaTela, float alturaDaTela);
    // Fase B4 (ficha da Reputação): o valor e a faixa de cada facção e Vila Livre, com o "Por quê?".
    void desenharAReputacao(const SimulacaoDaVila& simulacao, float larguraDaTela, float alturaDaTela);
    // Fase B5 (ficha da Tecnologia): a era, o projeto e os pontos de cada facção, e os marcos.
    void desenharATecnologia(const SimulacaoDaVila& simulacao, float larguraDaTela, float alturaDaTela);
    // Fase C1 (ficha da Facção): as leis, o Banco e o líder de cada facção.
    void desenharAsFaccoes(const SimulacaoDaVila& simulacao, float larguraDaTela, float alturaDaTela);

    bool prontaParaDesenhar = false;
    bool janelaDeGestaoAberta = false;
    int abaPedida = -1;           // 0 a 4: o alerta pediu esta aba
    int loteSelecionado = -1;     // o número do lote clicado
    // Os preços locais de ontem, para mostrar se subiram ou desceram.
    std::vector<int> precosDeOntem;
    std::vector<int> precosDeHoje;
    int64_t diaDosPrecosGuardados = -1;
    // Os campos de quantidade da aba Mercado e do formulário de ordens.
    std::map<int, int> quantidadeDoMercado;
    int tipoDaNovaOrdem = 0;
    int itemDaNovaOrdem = 0;
    int quantidadeDaNovaOrdem = 10;
    // O Mapa Mundial: o destino escolhido e o que o grupo leva.
    bool mapaMundialAberto = false;
    int destinoEscolhido = 0;
    int paesParaLevar = 3;
    int moedasParaLevar = 50;
    bool marchaForcadaEscolhida = false;
    bool janelaDasNoticiasAberta = false;
    bool janelaDaReputacaoAberta = false;
    bool janelaDaTecnologiaAberta = false;
    bool janelaDasFaccoesAberta = false;
};
