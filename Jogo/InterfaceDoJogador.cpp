#include "InterfaceDoJogador.h"

#include "Vila/Trabalho.h"
#include <algorithm>
#include <cstdio>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

#include "Construcao/ContasDasConstrucoes.h"
#include "Economia/ConsumoESalarios.h"
#include "Economia/FelicidadeDaClasse.h"
#include "Jogador/DecisoesDoJogador.h"
#include "Jogador/ObrasDaVila.h"
#include "Npc/ComponentesDoNpc.h"
#include "TextoDaDataDoJogo.h"
#include "Viagem/ViagemDaVila.h"
#include "Comercio/Comercio.h"

namespace {

constexpr float alturaDaBarraDeCima = 36.0f;
constexpr ImGuiWindowFlags opcoesDeUmaBarra = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
    | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

const ImVec4 corDoAviso(1.0f, 0.55f, 0.35f, 1.0f);
const ImVec4 corDaInformacao(0.75f, 0.9f, 1.0f, 1.0f);
const ImVec4 corDoDestaque(0.55f, 0.85f, 0.55f, 1.0f);

std::string nomeDoItem(const DadosDoJogo& dados, int numeroDoItem)
{
    return dados.tabelaDeTextos.textoDe(dados.catalogoDeItens.itemPeloNumero(numeroDoItem).identificadorDeTexto);
}

std::string nomeDoTipoDeConstrucao(const DadosDoJogo& dados, const std::string& tipo)
{
    return dados.tabelaDeTextos.textoDe(tipo);
}

std::string dinheiro(int64_t valorEmDecimos)
{
    return escreverDinheiroComMilhar(valorEmDecimos);
}

// Os itens que uma construção faz (as receitas dela liberadas no nível).
std::vector<int> produtosDaConstrucao(const DadosDoJogo& dados, const std::string& tipo, int nivel)
{
    std::vector<int> produtos;
    for (const DefinicaoDeReceita& receita : dados.catalogoDeReceitas.todasAsReceitas()) {
        if (receita.construcaoQueProduz == tipo && receita.nivelMinimoDaConstrucao <= nivel) {
            produtos.push_back(receita.numeroDoProduto);
        }
    }
    return produtos;
}

struct EmpregadoParaMostrar {
    int numeroDoNpc = 0;
    std::string nome;
};

std::vector<EmpregadoParaMostrar> npcsDoTrabalho(const SimulacaoDaVila& simulacao, int indiceDoTrabalho)
{
    std::vector<EmpregadoParaMostrar> npcs;
    for (const auto [npc, identidade, casaETrabalho] : simulacao.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc>().each()) {
        if (casaETrabalho.indiceDoTrabalho == indiceDoTrabalho && identidade.classe == "civil") {
            npcs.push_back({identidade.numeroDoNpc, identidade.nome});
        }
    }
    std::sort(npcs.begin(), npcs.end(), [](const auto& primeiro, const auto& segundo) { return primeiro.numeroDoNpc < segundo.numeroDoNpc; });
    return npcs;
}

int indiceDaConstrucaoNoLote(const SimulacaoDaVila& simulacao, int numeroDoLote)
{
    for (std::size_t indice = 0; indice < simulacao.vila.construcoes.size(); ++indice) {
        if (simulacao.vila.construcoes[indice].numeroDoLote == numeroDoLote) {
            return static_cast<int>(indice);
        }
    }
    return -1;
}

const char* nomeDoNivelDeSalario(NivelDeSalario nivel)
{
    switch (nivel) {
    case NivelDeSalario::Baixo: return "Baixo";
    case NivelDeSalario::Alto: return "Alto";
    case NivelDeSalario::Normal: break;
    }
    return "Normal";
}

std::string textoDaOrdem(const DadosDoJogo& dados, const OrdemDoJogador& ordem)
{
    const std::string item = nomeDoItem(dados, ordem.numeroDoItem);
    switch (ordem.tipo) {
    case TipoDeOrdem::ManterEstoque: return "Manter " + std::to_string(ordem.quantidade) + " " + item;
    case TipoDeOrdem::FazerQuantidade:
        return "Fazer " + std::to_string(ordem.quantidade) + " " + item + " (" + std::to_string(ordem.quantidadeJaFeita) + " feitos)";
    case TipoDeOrdem::RepetirSempre: break;
    }
    return "Repetir sempre " + item;
}

// O estado de uma construção em poucas palavras, para a tabela e o painel.
std::string estadoDaConstrucao(const ConstrucaoDaVila& construcao, const ProducaoDaConstrucao& producao)
{
    std::string estado;
    if (construcao.emObra) {
        estado = "em obra";
    } else if (producao.estaDeFolgaHoje) {
        estado = "de folga";
    } else if (producao.estaParada) {
        estado = producao.motivoDaParada;
    } else if (producao.ehDeProducao) {
        estado = "produzindo";
    }
    if (producao.manutencaoAtrasada) {
        estado += estado.empty() ? "manutenção atrasada" : " (manutenção atrasada)";
    }
    return estado;
}

int porcentagemDoCiclo(const ContextoDaProducao& contexto, const ProducaoDaConstrucao& producao)
{
    if (producao.receitaDoCiclo < 0) {
        return 0;
    }
    const DefinicaoDeReceita& receita = contexto.dados.catalogoDeReceitas.todasAsReceitas()[producao.receitaDoCiclo];
    const int64_t necessario = progressoNecessarioDoCiclo(receita, contexto.ticksPorHoraDeJogo);
    return necessario > 0 ? static_cast<int>(std::min<int64_t>(100, producao.progressoDoCiclo * 100 / necessario)) : 0;
}

}  // namespace

bool InterfaceDoJogador::preparar(SDL_Window* janela, SDL_GLContext ligacaoComOOpenGL, const std::string& pastaDosAssets, std::string& mensagemDeErro)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& entradaESaida = ImGui::GetIO();
    entradaESaida.IniFilename = nullptr;  // não grava a posição das janelas em arquivo
    // A fonte Noto Sans de 18 px, com os acentos do português (FL8-2); se faltar, fica a da ImGui.
    const std::string caminhoDaFonte = pastaDosAssets + "/fontes/NotoSans-Regular.ttf";
    if (entradaESaida.Fonts->AddFontFromFileTTF(caminhoDaFonte.c_str(), 18.0f, nullptr, entradaESaida.Fonts->GetGlyphRangesDefault()) == nullptr) {
        SDL_Log("Aviso: a fonte %s não abriu; usando a fonte da ImGui.", caminhoDaFonte.c_str());
    }
    ImGui::StyleColorsDark();
    ImGuiStyle& estilo = ImGui::GetStyle();
    estilo.WindowRounding = 4.0f;
    estilo.FrameRounding = 3.0f;
    estilo.WindowBorderSize = 1.0f;
    estilo.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.10f, 0.08f, 0.94f);
    estilo.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.22f, 0.12f, 1.0f);
    estilo.Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.27f, 0.15f, 1.0f);
    estilo.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.38f, 0.20f, 1.0f);
    estilo.Colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.45f, 0.24f, 1.0f);
    estilo.Colors[ImGuiCol_Tab] = ImVec4(0.26f, 0.20f, 0.12f, 1.0f);
    estilo.Colors[ImGuiCol_TabHovered] = ImVec4(0.50f, 0.38f, 0.20f, 1.0f);
    estilo.Colors[ImGuiCol_TabSelected] = ImVec4(0.42f, 0.31f, 0.16f, 1.0f);
    estilo.Colors[ImGuiCol_Header] = ImVec4(0.32f, 0.25f, 0.14f, 1.0f);
    estilo.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.35f, 0.19f, 1.0f);
    if (!ImGui_ImplSDL3_InitForOpenGL(janela, ligacaoComOOpenGL) || !ImGui_ImplOpenGL3_Init("#version 330 core")) {
        mensagemDeErro = "a Dear ImGui não ligou na janela e no OpenGL";
        return false;
    }
    prontaParaDesenhar = true;
    return true;
}

void InterfaceDoJogador::liberar()
{
    if (!prontaParaDesenhar) {
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    prontaParaDesenhar = false;
}

void InterfaceDoJogador::processarEvento(const SDL_Event& evento)
{
    if (prontaParaDesenhar) {
        ImGui_ImplSDL3_ProcessEvent(&evento);
    }
}

bool InterfaceDoJogador::querOMouse() const
{
    return prontaParaDesenhar && ImGui::GetIO().WantCaptureMouse;
}

bool InterfaceDoJogador::querOTeclado() const
{
    return prontaParaDesenhar && ImGui::GetIO().WantCaptureKeyboard;
}

bool InterfaceDoJogador::fecharOQueEstiverAberto()
{
    if (loteSelecionado >= 0) {
        loteSelecionado = -1;
        return true;
    }
    if (mapaMundialAberto) {
        mapaMundialAberto = false;
        return true;
    }
    if (janelaDeGestaoAberta) {
        janelaDeGestaoAberta = false;
        return true;
    }
    return false;
}

void InterfaceDoJogador::aoTrocarDePartida()
{
    loteSelecionado = -1;
    janelaDeGestaoAberta = false;
    mapaMundialAberto = false;
    destinoEscolhido = 0;
    abaPedida = -1;
    precosDeOntem.clear();
    precosDeHoje.clear();
    diaDosPrecosGuardados = -1;
    quantidadeDoMercado.clear();
}

void InterfaceDoJogador::desenharUmQuadroSemPartida(const std::function<void()>& desenharPorCima)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    if (desenharPorCima) {
        desenharPorCima();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void InterfaceDoJogador::desenharUmQuadro(SimulacaoDaVila& simulacao, int larguraDaTelaEmPixels, int alturaDaTelaEmPixels,
                                          const std::function<void()>& desenharPorCima)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ContextoDaProducao contexto = simulacao.contextoDaProducao();
    const float larguraDaTela = static_cast<float>(larguraDaTelaEmPixels);
    const float alturaDaTela = static_cast<float>(alturaDaTelaEmPixels);

    // Os preços de hoje e de ontem (para as setas da aba Inventário).
    const int64_t diaDaPartida = simulacao.relogio.diaDaPartida();
    if (diaDaPartida != diaDosPrecosGuardados) {
        precosDeOntem = precosDeHoje.empty() ? simulacao.mercadoLocal.precoLocalPorItemEmDecimos : precosDeHoje;
        precosDeHoje = simulacao.mercadoLocal.precoLocalPorItemEmDecimos;
        diaDosPrecosGuardados = diaDaPartida;
    }

    desenharABarraDeCima(simulacao, contexto, larguraDaTela);
    const std::vector<std::string> alertas = calcularOsAlertasDaVila(contexto);
    desenharOsAlertas(alertas, larguraDaTela);
    desenharAsNotificacoes(simulacao, larguraDaTela);
    if (janelaDeGestaoAberta) {
        desenharAJanelaDeGestao(simulacao, contexto, larguraDaTela, alturaDaTela);
    }
    if (loteSelecionado >= 0) {
        desenharOPainelDaConstrucao(simulacao, contexto, larguraDaTela, alturaDaTela);
    }
    if (mapaMundialAberto) {
        desenharOMapaMundial(simulacao, larguraDaTela, alturaDaTela);
    }
    if (simulacao.viagem.grupoDoJogador.encontroAcontecendo) {
        desenharOEncontro(simulacao, larguraDaTela, alturaDaTela);
    }
    if (desenharPorCima) {
        desenharPorCima();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void InterfaceDoJogador::desenharABarraDeCima(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, float larguraDaTela)
{
    RelogioDoMundo& relogio = simulacao.relogio;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(larguraDaTela, alturaDaBarraDeCima));
    ImGui::Begin("##barra_de_cima", nullptr, opcoesDeUmaBarra);

    // 1. A data e a hora.
    const std::string data = "Ano " + std::to_string(relogio.anoAtual()) + ", " + nomeDaEstacaoParaMostrar(relogio.estacaoAtual()) + ", dia "
        + std::to_string(relogio.diaDaEstacaoAtual()) + " | " + montarTextoDaHora(relogio) + " | " + nomeDaFaseDoDiaParaMostrar(relogio.faseDoDiaAtual())
        + " | " + simulacao.dados.tabelaDeTextos.textoDe(identificadorDeTextoDoClima(simulacao.clima.climaDeHoje));
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(data.c_str());
    ImGui::SameLine(0, 24);

    // 2. A velocidade: Pausa, 1x, 2x e 4x, com a atual destacada.
    const auto botaoDeVelocidade = [&](const char* nome, bool atual, int velocidade) {
        if (atual) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.55f, 0.30f, 1.0f));
        }
        if (ImGui::Button(nome)) {
            if (velocidade == 0) {
                if (!relogio.jogoEstaPausado()) {
                    relogio.alternarPausa();
                }
            } else {
                relogio.definirVelocidadeDoJogo(velocidade);
            }
        }
        if (atual) {
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();
    };
    botaoDeVelocidade("Pausa", relogio.jogoEstaPausado(), 0);
    botaoDeVelocidade("1x", !relogio.jogoEstaPausado() && relogio.velocidadeAtualDoJogo() == 1, 1);
    botaoDeVelocidade("2x", !relogio.jogoEstaPausado() && relogio.velocidadeAtualDoJogo() == 2, 2);
    botaoDeVelocidade("4x", !relogio.jogoEstaPausado() && relogio.velocidadeAtualDoJogo() == 4, 4);
    ImGui::SameLine(0, 16);

    // 3. O dinheiro, com o "Por quê?" no mouse.
    const DinheiroDaVila& banco = simulacao.dinheiro;
    ImGui::AlignTextToFramePadding();
    const std::string textoDoDinheiro = "Jogador " + dinheiro(banco.dinheiroDoJogador) + "   Banco " + dinheiro(banco.bancoDaCidade);
    ImGui::TextUnformatted(textoDoDinheiro.c_str());
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("No último dia:");
        ImGui::Text("Salários pagos aos Civis: %s", dinheiro(banco.rendaDoDiaDaClasseCivil).c_str());
        ImGui::Text("Imposto retido: %s", dinheiro(banco.impostoDeHoje).c_str());
        ImGui::Text("Cesta mínima: %s", dinheiro(banco.gastoDaCestaDeHoje).c_str());
        ImGui::Text("Gasto extra: %s", dinheiro(banco.gastoExtraDeHoje).c_str());
        ImGui::Text("Ajuda do estabilizador: %s", dinheiro(banco.ajudaDoEstabilizadorHoje).c_str());
        ImGui::EndTooltip();
    }
    ImGui::SameLine(0, 16);

    // 4. A janela de gestão.
    if (ImGui::Button("Gestão (Tab)")) {
        alternarAJanelaDeGestao();
    }
    ImGui::SameLine();
    if (ImGui::Button("Mapa Mundial")) {
        alternarOMapaMundial();
    }
    const EntidadeEmTransito& grupo = simulacao.viagem.grupoDoJogador;
    if (grupo.emTransito) {
        ImGui::SameLine(0, 16);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Viajando para %s", simulacao.dados.mundo.nomeDoMapa(grupo.destino()).c_str());
    } else if (!grupoEstaNaVila(simulacao.viagem, simulacao.dados)) {
        ImGui::SameLine(0, 16);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("O grupo está em %s", simulacao.dados.mundo.nomeDoMapa(grupo.mapaAtual).c_str());
    }
    (void)contexto;
    ImGui::End();
}

void InterfaceDoJogador::desenharOsAlertas(const std::vector<std::string>& alertas, float larguraDaTela)
{
    if (alertas.empty()) {
        return;
    }
    ImGui::SetNextWindowPos(ImVec2(0, alturaDaBarraDeCima));
    ImGui::SetNextWindowSize(ImVec2(larguraDaTela, alturaDaBarraDeCima - 4.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.30f, 0.10f, 0.06f, 0.85f));
    ImGui::Begin("##alertas", nullptr, opcoesDeUmaBarra);
    ImGui::TextColored(corDoAviso, "Alertas:");
    for (std::size_t posicao = 0; posicao < alertas.size(); ++posicao) {
        ImGui::SameLine();
        ImGui::PushID(static_cast<int>(posicao));
        if (ImGui::SmallButton(alertas[posicao].c_str())) {
            // Clicar abre a aba que mostra o problema (Interface, regra 10.4).
            janelaDeGestaoAberta = true;
            const bool ehDeComidaOuBanco = alertas[posicao].find("para menos de") != std::string::npos || alertas[posicao] == "Banco baixo";
            abaPedida = ehDeComidaOuBanco ? (alertas[posicao] == "Banco baixo" ? 0 : 2) : 1;
        }
        ImGui::PopID();
    }
    ImGui::End();
    ImGui::PopStyleColor();
}

void InterfaceDoJogador::desenharAsNotificacoes(const SimulacaoDaVila& simulacao, float larguraDaTela)
{
    const std::vector<const Notificacao*> naTela = simulacao.notificacoes.naTela(simulacao.relogio.tickAtual());
    if (naTela.empty()) {
        return;
    }
    const float largura = 380.0f;
    ImGui::SetNextWindowPos(ImVec2(larguraDaTela - largura - 8.0f, alturaDaBarraDeCima * 2.0f + 4.0f));
    ImGui::SetNextWindowSize(ImVec2(largura, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.75f);
    ImGui::Begin("##notificacoes", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
                     | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize);
    for (const Notificacao* notificacao : naTela) {
        ImGui::PushTextWrapPos(largura - 16.0f);
        ImGui::TextColored(notificacao->nivel == NivelDaNotificacao::Aviso ? corDoAviso : corDaInformacao, "%s", notificacao->texto.c_str());
        ImGui::PopTextWrapPos();
    }
    ImGui::End();
}

void InterfaceDoJogador::desenharAJanelaDeGestao(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, float larguraDaTela, float alturaDaTela)
{
    const float largura = std::min(960.0f, larguraDaTela - 40.0f);
    const float altura = std::min(600.0f, alturaDaTela - 120.0f);
    ImGui::SetNextWindowPos(ImVec2((larguraDaTela - largura) / 2.0f, alturaDaBarraDeCima * 2.0f + 8.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(largura, altura), ImGuiCond_FirstUseEver);
    bool aberta = true;
    if (ImGui::Begin("Gestão da vila", &aberta, ImGuiWindowFlags_NoSavedSettings)) {
        if (ImGui::BeginTabBar("abas_da_gestao")) {
            const auto aba = [&](int numero, const char* nome) {
                const ImGuiTabItemFlags opcoes = abaPedida == numero ? ImGuiTabItemFlags_SetSelected : 0;
                return ImGui::BeginTabItem(nome, nullptr, opcoes);
            };
            if (aba(0, "Cidade")) {
                abaDaCidade(simulacao, contexto);
                ImGui::EndTabItem();
            }
            if (aba(1, "Construções")) {
                abaDasConstrucoes(simulacao, contexto);
                ImGui::EndTabItem();
            }
            if (aba(2, "Inventário")) {
                abaDoInventario(simulacao, contexto);
                ImGui::EndTabItem();
            }
            if (aba(3, "Mercado")) {
                abaDoMercado(simulacao, contexto);
                ImGui::EndTabItem();
            }
            if (aba(4, "Diário")) {
                abaDoDiario(simulacao);
                ImGui::EndTabItem();
            }
            abaPedida = -1;
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
    janelaDeGestaoAberta = aberta;
}

void InterfaceDoJogador::abaDaCidade(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto)
{
    const DinheiroDaVila& banco = simulacao.dinheiro;
    const RegrasDaEconomia& economia = simulacao.dados.regras.economia;
    const int civis = banco.quantidadeDeCivis;
    const int desempregados = contarOsDesempregados(contexto);
    ImGui::SeparatorText("População");
    ImGui::Text("%d Civis: %d empregados e %d desempregados (desemprego de %d%%)", civis, civis - desempregados, desempregados,
                civis > 0 ? desempregados * 100 / civis : 0);

    ImGui::SeparatorText("Felicidade da classe");
    const FelicidadeDaClasse& felicidade = simulacao.felicidade;
    ImGui::Text("%d,%d (%s), indo para %d,%d", felicidade.felicidadeEmDecimos / 10, felicidade.felicidadeEmDecimos % 10,
                nomeDaFaixaDaFelicidade(felicidade.felicidadeEmDecimos).c_str(), felicidade.alvoEmDecimos / 10, felicidade.alvoEmDecimos % 10);
    if (ImGui::IsItemHovered()) {
        const FatoresDaFelicidade& fatores = felicidade.fatoresDoDia;
        ImGui::BeginTooltip();
        ImGui::Text("Os 4 fatores de hoje (em pontos):");
        ImGui::Text("Consumo atendido: %+.1f", fatores.consumoAtendido / 10.0);
        ImGui::Text("Desemprego e renda: %+.1f", fatores.desempregoERenda / 10.0);
        ImGui::Text("Moradia: %+.1f", fatores.moradia / 10.0);
        ImGui::Text("Eventos duradouros: %+.1f", fatores.eventosDuradouros / 10.0);
        ImGui::EndTooltip();
    }

    ImGui::SeparatorText("Dinheiro");
    const int64_t mediaDosCivis = civis > 0 ? banco.dinheiroDaClasseCivil / civis : 0;
    const int64_t cestaDeUmCivil = calcularCestaDeUmCivilEmDecimos(contexto);
    ImGui::Text("Banco da Cidade: %s", dinheiro(banco.bancoDaCidade).c_str());
    ImGui::Text("Jogador: %s", dinheiro(banco.dinheiroDoJogador).c_str());
    ImGui::Text("Civis: %s no total, %s em média (%.1f dias da cesta)", dinheiro(banco.dinheiroDaClasseCivil).c_str(), dinheiro(mediaDosCivis).c_str(),
                cestaDeUmCivil > 0 ? static_cast<double>(mediaDosCivis) * economia.diasEntreMateriaisDoCivil / static_cast<double>(cestaDeUmCivil) : 0.0);
    ImGui::Text("Imposto do último dia: %s", dinheiro(banco.impostoDeHoje).c_str());
    ImGui::Text("Estabilizador nesta estação: %s de no máximo %s", dinheiro(banco.ajudaDoEstabilizadorNaEstacao).c_str(),
                dinheiro(banco.bancoNoComecoDaEstacao * economia.estabilizador.tetoPorEstacaoEmPorcentoDoBanco / 100).c_str());

    ImGui::SeparatorText("Salários de hoje");
    ImGui::Text("Multiplicador da riqueza: %d%%", calcularMultiplicadorDaRiqueza(banco.dinheiroDaClasseCivil, civis, cestaDeUmCivil, economia.diasEntreMateriaisDoCivil,
                                                                                  economia.multiplicadoresDoSalario));
    ImGui::Text("Multiplicador do desemprego: %d%%", calcularMultiplicadorDoDesemprego(desempregados, civis, economia.multiplicadoresDoSalario));
}

void InterfaceDoJogador::abaDasConstrucoes(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto)
{
    const DadosDoJogo& dados = simulacao.dados;
    if (ImGui::BeginTable("tabela_das_construcoes", 7, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("Construção");
        ImGui::TableSetupColumn("Dono");
        ImGui::TableSetupColumn("Nível");
        ImGui::TableSetupColumn("Empregados");
        ImGui::TableSetupColumn("Receita");
        ImGui::TableSetupColumn("Ciclo");
        ImGui::TableSetupColumn("Estado");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();
        for (std::size_t indice = 0; indice < simulacao.vila.construcoes.size(); ++indice) {
            const ConstrucaoDaVila& construcao = simulacao.vila.construcoes[indice];
            const ProducaoDaConstrucao& producao = simulacao.producaoDasConstrucoes[indice];
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::PushID(static_cast<int>(indice));
            if (ImGui::Selectable(construcao.identificador.c_str(), false, ImGuiSelectableFlags_SpanAllColumns)) {
                loteSelecionado = construcao.numeroDoLote;
            }
            ImGui::PopID();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(construcao.donoEhOJogador ? "Jogador" : "Cidade");
            ImGui::TableNextColumn();
            ImGui::Text("%d", construcao.nivel);
            ImGui::TableNextColumn();
            ImGui::Text("%d / %d", static_cast<int>(npcsDoTrabalho(simulacao, static_cast<int>(indice)).size()),
                        calcularVagas(construcao.nivel, dados.regras.construcoes));
            ImGui::TableNextColumn();
            if (producao.receitaDoCiclo >= 0) {
                ImGui::TextUnformatted(nomeDoItem(dados, dados.catalogoDeReceitas.todasAsReceitas()[producao.receitaDoCiclo].numeroDoProduto).c_str());
            } else {
                ImGui::TextUnformatted("-");
            }
            ImGui::TableNextColumn();
            ImGui::Text("%d%%", porcentagemDoCiclo(contexto, producao));
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(estadoDaConstrucao(construcao, producao).c_str());
        }
        ImGui::EndTable();
    }
    if (!simulacao.obras.fila.empty()) {
        ImGui::SeparatorText("Fila de obras");
        for (const ObraNaFila& obra : simulacao.obras.fila) {
            const char* tipo = obra.tipo == TipoDeObra::ConstrucaoNova ? "Construção nova" : (obra.tipo == TipoDeObra::SubirDeNivel ? "Subir de nível" : "Manutenção");
            ImGui::Text("%s: %s, faltam %.1f h%s", tipo, simulacao.vila.construcoes[obra.indiceDaConstrucao].identificador.c_str(),
                        static_cast<double>(obra.centesimosDeHoraQueFaltam) / 100.0, obra.esperaMaterial ? " (espera material)" : "");
        }
    }
}

void InterfaceDoJogador::abaDoInventario(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto)
{
    const DadosDoJogo& dados = simulacao.dados;
    if (ImGui::BeginTable("tabela_do_inventario", 7, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("Item");
        ImGui::TableSetupColumn("Cidade");
        ImGui::TableSetupColumn("Jogador");
        ImGui::TableSetupColumn("Alvo");
        ImGui::TableSetupColumn("Preço base");
        ImGui::TableSetupColumn("Preço local");
        ImGui::TableSetupColumn("Consumo/dia");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();
        for (const DefinicaoDeItem& item : dados.catalogoDeItens.todosOsItens()) {
            const int numero = item.numeroInterno;
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(nomeDoItem(dados, numero).c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%d", simulacao.inventario.quantidadeDe(numero, DonoDoItem::Cidade));
            ImGui::TableNextColumn();
            ImGui::Text("%d", simulacao.inventario.quantidadeDe(numero, DonoDoItem::Jogador));
            ImGui::TableNextColumn();
            ImGui::Text("%.1f", static_cast<double>(estoqueAlvoDoItemEmCentesimos(contexto, numero)) / 100.0);
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(dinheiro(item.precoBaseEmDecimos).c_str());
            ImGui::TableNextColumn();
            const int precoLocal = precoLocalDoItem(contexto, numero);
            const int precoDeOntem = numero < static_cast<int>(precosDeOntem.size()) ? precosDeOntem[numero] : precoLocal;
            ImGui::Text("%s%s", dinheiro(precoLocal).c_str(), precoLocal > precoDeOntem ? " (sobe)" : (precoLocal < precoDeOntem ? " (desce)" : ""));
            ImGui::TableNextColumn();
            ImGui::Text("%.2f", static_cast<double>(simulacao.consumoMedido.consumoPrevistoDoDiaEmCentesimos(numero)) / 100.0);
        }
        ImGui::EndTable();
    }
}

void InterfaceDoJogador::abaDoMercado(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto)
{
    const DadosDoJogo& dados = simulacao.dados;
    ImGui::Text("A loja da cidade vende a %d%% do preço local e compra a %d%%. Você tem %s.", dados.regras.economia.lojaDaCidadeVendeEmPorcento,
                dados.regras.economia.lojaDaCidadeCompraEmPorcento, dinheiro(simulacao.dinheiro.dinheiroDoJogador).c_str());
    if (ImGui::BeginTable("tabela_do_mercado", 7, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Item");
        ImGui::TableSetupColumn("Cidade");
        ImGui::TableSetupColumn("Você");
        ImGui::TableSetupColumn("Compra por");
        ImGui::TableSetupColumn("Vende por");
        ImGui::TableSetupColumn("Quantidade");
        ImGui::TableSetupColumn("");
        ImGui::TableHeadersRow();
        for (const DefinicaoDeItem& item : dados.catalogoDeItens.todosOsItens()) {
            const int numero = item.numeroInterno;
            const int precoLocal = precoLocalDoItem(contexto, numero);
            int& quantidade = quantidadeDoMercado.try_emplace(numero, 1).first->second;
            ImGui::PushID(numero);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(nomeDoItem(dados, numero).c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%d", simulacao.inventario.quantidadeDe(numero, DonoDoItem::Cidade));
            ImGui::TableNextColumn();
            ImGui::Text("%d", simulacao.inventario.quantidadeDe(numero, DonoDoItem::Jogador));
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(dinheiro(calcularPrecoDaLojaDaCidade(precoLocal, dados.regras.economia)).c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(dinheiro(calcularPrecoQueALojaDaCidadePaga(precoLocal, dados.regras.economia)).c_str());
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(90.0f);
            ImGui::InputInt("##quantidade", &quantidade, 1, 10);
            quantidade = std::clamp(quantidade, 1, 999);
            ImGui::TableNextColumn();
            if (ImGui::SmallButton("Comprar")) {
                simulacao.pedidos.acrescentar(PedidoComprarDaLoja{numero, quantidade});
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Vender")) {
                simulacao.pedidos.acrescentar(PedidoVenderParaALoja{numero, quantidade});
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void InterfaceDoJogador::abaDoDiario(const SimulacaoDaVila& simulacao)
{
    const std::vector<Notificacao>& diario = simulacao.notificacoes.doDiario();
    if (diario.empty()) {
        ImGui::TextUnformatted("Nada aconteceu ainda.");
        return;
    }
    ImGui::BeginChild("lista_do_diario");
    for (auto notificacao = diario.rbegin(); notificacao != diario.rend(); ++notificacao) {
        ImGui::TextColored(notificacao->nivel == NivelDaNotificacao::Aviso ? corDoAviso : corDaInformacao, "Dia %d: %s",
                           static_cast<int>(notificacao->diaDaPartida) + 1, notificacao->texto.c_str());
    }
    ImGui::EndChild();
}

void InterfaceDoJogador::desenharOPainelDaConstrucao(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, float larguraDaTela, float alturaDaTela)
{
    const float largura = 460.0f;
    ImGui::SetNextWindowPos(ImVec2(larguraDaTela - largura - 8.0f, alturaDaBarraDeCima * 2.0f + 4.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(largura, alturaDaTela - alturaDaBarraDeCima * 2.0f - 16.0f), ImGuiCond_Always);
    const int indice = indiceDaConstrucaoNoLote(simulacao, loteSelecionado);
    bool aberta = true;
    const std::string titulo = indice >= 0 ? simulacao.vila.construcoes[indice].identificador : "Lote " + std::to_string(loteSelecionado) + " (livre)";
    ImGui::Begin((titulo + "###painel_da_construcao").c_str(), &aberta, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    if (indice < 0) {
        desenharOLoteLivre(simulacao, contexto, loteSelecionado);
    } else {
        const DadosDoJogo& dados = simulacao.dados;
        const ConstrucaoDaVila& construcao = simulacao.vila.construcoes[indice];
        const ProducaoDaConstrucao& producao = simulacao.producaoDasConstrucoes[indice];
        const int vagas = calcularVagas(construcao.nivel, dados.regras.construcoes);
        ImGui::Text("%s, nível %d, dono: %s", nomeDoTipoDeConstrucao(dados, construcao.tipo).c_str(), construcao.nivel, construcao.donoEhOJogador ? "Jogador" : "Cidade");
        const std::string estado = estadoDaConstrucao(construcao, producao);
        if (producao.receitaDoCiclo >= 0) {
            ImGui::Text("Fazendo %s: %d%%", nomeDoItem(dados, dados.catalogoDeReceitas.todasAsReceitas()[producao.receitaDoCiclo].numeroDoProduto).c_str(),
                        porcentagemDoCiclo(contexto, producao));
        }
        if (!estado.empty()) {
            ImGui::TextColored(producao.estaParada || producao.manutencaoAtrasada ? corDoAviso : corDoDestaque, "Estado: %s", estado.c_str());
        }
        for (const ObraNaFila& obra : simulacao.obras.fila) {
            if (obra.indiceDaConstrucao == indice) {
                ImGui::Text("Obra na fila: faltam %.1f h%s", static_cast<double>(obra.centesimosDeHoraQueFaltam) / 100.0, obra.esperaMaterial ? " (espera material)" : "");
            }
        }

        // Fase A2 (ficha do Trabalho e Propriedade, regras 4.3 e 4.4): comprar,
        // vender, o modo automático e o leilão.
        const RegrasDoTrabalho& regrasDoTrabalho = dados.regras.trabalho;
        const int64_t valorDaConstrucao = calcularValorDaConstrucao(contexto, indice);
        const bool estaEmLeilao = simulacao.trabalho.leilao.indiceDaConstrucao == indice;
        if (!construcao.donoEhOJogador && producao.ehDeProducao && !construcao.emObra) {
            ImGui::BeginDisabled(simulacao.dinheiro.dinheiroDoJogador < valorDaConstrucao || estaEmLeilao);
            if (ImGui::Button(("Comprar da Cidade por " + dinheiro(valorDaConstrucao)).c_str())) {
                simulacao.pedidos.acrescentar(PedidoComprarConstrucao{indice});
            }
            ImGui::EndDisabled();
        }
        if (construcao.donoEhOJogador && !construcao.emObra) {
            bool modoAutomatico = producao.modoAutomatico;
            if (ImGui::Checkbox("Modo automático (a IA da Cidade escolhe a receita)", &modoAutomatico)) {
                simulacao.pedidos.acrescentar(PedidoTrocarOModoAutomatico{indice, modoAutomatico});
            }
            const int64_t precoDeVenda = valorDaConstrucao * regrasDoTrabalho.cidadeCompraDeVoltaEmPorcento / 100;
            ImGui::BeginDisabled(estaEmLeilao);
            if (ImGui::Button(("Vender para a Cidade por " + dinheiro(precoDeVenda)).c_str())) {
                simulacao.pedidos.acrescentar(PedidoVenderConstrucao{indice});
            }
            ImGui::EndDisabled();
            if (producao.diasSemEmpregados > 0) {
                ImGui::TextColored(corDoAviso, "Sem empregados há %d dias (leilão com %d).", producao.diasSemEmpregados,
                                   regrasDoTrabalho.estacoesParadaParaOLeilao * simulacao.relogio.diasPorEstacao());
            }
        }
        if (estaEmLeilao) {
            const LeilaoDeConstrucao& leilao = simulacao.trabalho.leilao;
            const int64_t lanceMinimo = std::max(valorDaConstrucao * regrasDoTrabalho.lanceMinimoEmPorcentoDoValor / 100, leilao.maiorLanceEmDecimos + 1);
            ImGui::TextColored(corDoAviso, "Em leilão: faltam %d dias; maior lance %s.", leilao.diasQueFaltam, dinheiro(leilao.maiorLanceEmDecimos).c_str());
            if (ImGui::Button(("Dar o lance de " + dinheiro(lanceMinimo)).c_str())) {
                simulacao.pedidos.acrescentar(PedidoDarLance{lanceMinimo});
            }
        }

        ImGui::SeparatorText("Empregados");
        const std::vector<EmpregadoParaMostrar> empregados = npcsDoTrabalho(simulacao, indice);
        ImGui::Text("%d de %d vagas", static_cast<int>(empregados.size()), vagas);
        for (const EmpregadoParaMostrar& empregado : empregados) {
            ImGui::PushID(empregado.numeroDoNpc);
            ImGui::TextUnformatted(empregado.nome.c_str());
            if (construcao.donoEhOJogador && !construcao.emObra) {
                ImGui::SameLine();
                if (ImGui::SmallButton("Demitir")) {
                    simulacao.pedidos.acrescentar(PedidoDemitir{empregado.numeroDoNpc});
                }
            }
            ImGui::PopID();
        }

        if (construcao.donoEhOJogador && !construcao.emObra) {
            ImGui::SeparatorText("Desempregados");
            const std::vector<EmpregadoParaMostrar> desempregados = npcsDoTrabalho(simulacao, -1);
            if (desempregados.empty()) {
                ImGui::TextUnformatted("Ninguém.");
            }
            for (const EmpregadoParaMostrar& desempregado : desempregados) {
                ImGui::PushID(1000 + desempregado.numeroDoNpc);
                ImGui::TextUnformatted(desempregado.nome.c_str());
                ImGui::SameLine();
                ImGui::BeginDisabled(static_cast<int>(empregados.size()) >= vagas);
                if (ImGui::SmallButton("Contratar")) {
                    simulacao.pedidos.acrescentar(PedidoContratar{indice, desempregado.numeroDoNpc});
                }
                ImGui::EndDisabled();
                ImGui::PopID();
            }

            ImGui::SeparatorText("Nível de salário");
            for (const NivelDeSalario nivel : {NivelDeSalario::Baixo, NivelDeSalario::Normal, NivelDeSalario::Alto}) {
                ImGui::BeginDisabled(producao.nivelDeSalario == nivel);
                if (ImGui::Button(nomeDoNivelDeSalario(nivel))) {
                    simulacao.pedidos.acrescentar(PedidoTrocarONivelDeSalario{indice, nivel});
                }
                ImGui::EndDisabled();
                ImGui::SameLine();
            }
            ImGui::NewLine();
            ImGui::TextWrapped("Baixo: salário x0,75 e produção x0,9; depois de 8 dias, um empregado sai. Alto: salário x1,25 e produção x1,1.");

            // O preço de cada produto (FL8-6).
            const std::vector<int> produtos = produtosDaConstrucao(dados, construcao.tipo, construcao.nivel);
            if (!produtos.empty()) {
                ImGui::SeparatorText("Preço");
                const RegrasDasDecisoesDoJogador& regrasDoPreco = dados.regras.economia.decisoesDoJogador;
                for (const int produto : produtos) {
                    const int porcentagem = simulacao.decisoes.porcentagemDoPrecoDe(produto);
                    const int precoLocal = precoLocalDoItem(contexto, produto);
                    ImGui::PushID(2000 + produto);
                    ImGui::Text("%s: local %s, seu preço %s (%+d%%)", nomeDoItem(dados, produto).c_str(), dinheiro(precoLocal).c_str(),
                                dinheiro(calcularPrecoDoItemDoJogador(precoLocal, porcentagem)).c_str(), porcentagem);
                    ImGui::BeginDisabled(porcentagem - regrasDoPreco.passoDoPrecoEmPorcento < -regrasDoPreco.faixaDoPrecoEmPorcento);
                    if (ImGui::SmallButton("-5")) {
                        simulacao.pedidos.acrescentar(PedidoFixarOPreco{produto, porcentagem - regrasDoPreco.passoDoPrecoEmPorcento});
                    }
                    ImGui::EndDisabled();
                    ImGui::SameLine();
                    ImGui::BeginDisabled(porcentagem + regrasDoPreco.passoDoPrecoEmPorcento > regrasDoPreco.faixaDoPrecoEmPorcento);
                    if (ImGui::SmallButton("+5")) {
                        simulacao.pedidos.acrescentar(PedidoFixarOPreco{produto, porcentagem + regrasDoPreco.passoDoPrecoEmPorcento});
                    }
                    ImGui::EndDisabled();
                    ImGui::PopID();
                }
                // FE3-3: na vila de hoje ninguém mais vende o que você faz.
                ImGui::TextDisabled("Dica: ninguém mais vende o que você faz; mudar o preço reduz a venda.");
                desenharAsOrdensDoJogador(simulacao, contexto, indice);
            }

            // Subir de nível.
            ImGui::SeparatorText("Obra");
            const int nivelDeDestino = construcao.nivel + 1;
            const DefinicaoDeConstrucao* definicao = dados.catalogoDeConstrucoes.procurarConstrucao(construcao.tipo);
            if (definicao != nullptr) {
                const CustoDaObra custo = calcularCustoDaObra(definicao->porte, nivelDeDestino, dados.regras.construcoes);
                const bool podePedir = nivelDeDestino <= dados.regras.construcoes.nivelMaximoComOEngenheiro && !construcaoTemObraNaFila(contexto, indice);
                ImGui::Text("Nível %d: %d Tábuas, %d Blocos, %d h (%s de salários)", nivelDeDestino, custo.tabuas, custo.blocosDePedra, custo.horasDeTrabalho,
                            dinheiro(calcularPagamentoDasHorasDeObra(custo.horasDeTrabalho, contexto)).c_str());
                ImGui::BeginDisabled(!podePedir);
                if (ImGui::Button(("Subir para o nível " + std::to_string(nivelDeDestino)).c_str())) {
                    simulacao.pedidos.acrescentar(PedidoSubirDeNivel{indice});
                }
                ImGui::EndDisabled();
                if (nivelDeDestino > dados.regras.construcoes.nivelMaximoComOEngenheiro) {
                    ImGui::TextWrapped("O Engenheiro só faz obras até o nível %d.", dados.regras.construcoes.nivelMaximoComOEngenheiro);
                }
            }
        }
    }
    ImGui::End();
    if (!aberta) {
        loteSelecionado = -1;
    }
}

void InterfaceDoJogador::desenharAsOrdensDoJogador(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    (void)contexto;
    const DadosDoJogo& dados = simulacao.dados;
    const ConstrucaoDaVila& construcao = simulacao.vila.construcoes[indiceDaConstrucao];
    std::vector<OrdemDoJogador> ordens = simulacao.producaoDasConstrucoes[indiceDaConstrucao].ordens;
    bool mudou = false;
    ImGui::SeparatorText("Fila de ordens");
    if (ordens.empty()) {
        ImGui::TextUnformatted("Nenhuma ordem: a construção fica parada.");
    }
    for (std::size_t posicao = 0; posicao < ordens.size(); ++posicao) {
        ImGui::PushID(static_cast<int>(3000 + posicao));
        ImGui::Text("%d. %s", static_cast<int>(posicao) + 1, textoDaOrdem(dados, ordens[posicao]).c_str());
        ImGui::SameLine();
        if (posicao > 0 && ImGui::SmallButton("Sobe")) {
            std::swap(ordens[posicao], ordens[posicao - 1]);
            mudou = true;
        } else if (posicao + 1 < ordens.size() && ImGui::SmallButton("Desce")) {
            std::swap(ordens[posicao], ordens[posicao + 1]);
            mudou = true;
        } else if (ImGui::SmallButton("X")) {
            ordens.erase(ordens.begin() + static_cast<std::ptrdiff_t>(posicao));
            mudou = true;
        }
        ImGui::PopID();
        if (mudou) {
            break;
        }
    }

    // A ordem nova: o tipo, o item (só os que a construção faz) e o N.
    const std::vector<int> produtos = produtosDaConstrucao(dados, construcao.tipo, construcao.nivel);
    static const char* nomesDosTipos[3] = {"Manter estoque", "Fazer N", "Repetir sempre"};
    tipoDaNovaOrdem = std::clamp(tipoDaNovaOrdem, 0, 2);
    itemDaNovaOrdem = std::clamp(itemDaNovaOrdem, 0, std::max(0, static_cast<int>(produtos.size()) - 1));
    ImGui::SetNextItemWidth(140.0f);
    if (ImGui::BeginCombo("##tipo_da_ordem", nomesDosTipos[tipoDaNovaOrdem])) {
        for (int tipo = 0; tipo < 3; ++tipo) {
            if (ImGui::Selectable(nomesDosTipos[tipo], tipo == tipoDaNovaOrdem)) {
                tipoDaNovaOrdem = tipo;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(120.0f);
    if (!produtos.empty() && ImGui::BeginCombo("##item_da_ordem", nomeDoItem(dados, produtos[itemDaNovaOrdem]).c_str())) {
        for (std::size_t posicao = 0; posicao < produtos.size(); ++posicao) {
            if (ImGui::Selectable(nomeDoItem(dados, produtos[posicao]).c_str(), static_cast<int>(posicao) == itemDaNovaOrdem)) {
                itemDaNovaOrdem = static_cast<int>(posicao);
            }
        }
        ImGui::EndCombo();
    }
    if (tipoDaNovaOrdem != 2) {
        ImGui::SetNextItemWidth(100.0f);
        ImGui::InputInt("N", &quantidadeDaNovaOrdem, 1, 10);
        quantidadeDaNovaOrdem = std::clamp(quantidadeDaNovaOrdem, 1, 999);
    }
    if (!produtos.empty() && ImGui::Button("Adicionar ordem")) {
        OrdemDoJogador nova;
        nova.tipo = tipoDaNovaOrdem == 0 ? TipoDeOrdem::ManterEstoque : (tipoDaNovaOrdem == 1 ? TipoDeOrdem::FazerQuantidade : TipoDeOrdem::RepetirSempre);
        nova.numeroDoItem = produtos[itemDaNovaOrdem];
        nova.quantidade = tipoDaNovaOrdem == 2 ? 0 : quantidadeDaNovaOrdem;
        ordens.push_back(nova);
        mudou = true;
    }
    if (mudou) {
        simulacao.pedidos.acrescentar(PedidoTrocarAFilaDeOrdens{indiceDaConstrucao, ordens});
    }
}

void InterfaceDoJogador::desenharOLoteLivre(SimulacaoDaVila& simulacao, ContextoDaProducao& contexto, int numeroDoLote)
{
    const DadosDoJogo& dados = simulacao.dados;
    const LoteDeConstrucao* lote = nullptr;
    for (const LoteDeConstrucao& candidato : simulacao.mapa.lotesDeConstrucao) {
        if (candidato.identificadorDoLote == numeroDoLote) {
            lote = &candidato;
        }
    }
    if (lote == nullptr) {
        ImGui::TextUnformatted("Esse lote não existe.");
        return;
    }
    const char* porte = lote->porteDoLote == PorteDoLote::Pequeno ? "pequeno" : (lote->porteDoLote == PorteDoLote::Medio ? "médio" : "grande");
    ImGui::Text("Lote %s e livre. Escolha o que construir:", porte);
    for (const DefinicaoDeConstrucao& definicao : dados.catalogoDeConstrucoes.todasAsConstrucoes()) {
        if (definicao.porte != lote->porteDoLote) {
            continue;
        }
        const CustoDaObra custo = calcularCustoDaObra(definicao.porte, 1, dados.regras.construcoes);
        ImGui::PushID(definicao.identificadorDeTexto.c_str());
        ImGui::SeparatorText(nomeDoTipoDeConstrucao(dados, definicao.identificadorDeTexto).c_str());
        ImGui::Text("%d Tábuas, %d Blocos, %d h (%s de salários)", custo.tabuas, custo.blocosDePedra, custo.horasDeTrabalho,
                    dinheiro(calcularPagamentoDasHorasDeObra(custo.horasDeTrabalho, contexto)).c_str());
        if (ImGui::Button("Encomendar obra")) {
            simulacao.pedidos.acrescentar(PedidoEncomendarConstrucaoNova{numeroDoLote, definicao.identificadorDeTexto});
        }
        ImGui::PopID();
    }
}

// ---------------------------------------------------------------------------
// Fase B1 (ficha da Viagem): o Mapa Mundial, com a névoa, e a janela do encontro.

namespace {

const char* nomeDoEstadoDoTerritorio(EstadoDoTerritorio estado)
{
    switch (estado) {
    case EstadoDoTerritorio::ComCidade: return "com cidade";
    case EstadoDoTerritorio::Territorio: return "território de facção";
    case EstadoDoTerritorio::EmDisputa: return "em disputa";
    case EstadoDoTerritorio::SemDono: return "sem dono";
    }
    return "";
}

ImVec4 corDoEstadoDoTerritorio(EstadoDoTerritorio estado)
{
    switch (estado) {
    case EstadoDoTerritorio::ComCidade: return ImVec4(0.55f, 0.45f, 0.20f, 1.0f);
    case EstadoDoTerritorio::Territorio: return ImVec4(0.25f, 0.40f, 0.25f, 1.0f);
    case EstadoDoTerritorio::EmDisputa: return ImVec4(0.55f, 0.25f, 0.20f, 1.0f);
    case EstadoDoTerritorio::SemDono: return ImVec4(0.30f, 0.30f, 0.35f, 1.0f);
    }
    return ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
}

}  // namespace

void InterfaceDoJogador::desenharOMapaMundial(SimulacaoDaVila& simulacao, float larguraDaTela, float alturaDaTela)
{
    const MundoDoJogo& mundo = simulacao.dados.mundo;
    const RegrasDaViagem& regras = simulacao.dados.regrasDaViagem;
    const EstadoDaViagem& viagem = simulacao.viagem;
    const EntidadeEmTransito& grupo = viagem.grupoDoJogador;
    ImGui::SetNextWindowSize(ImVec2(760, 560), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(larguraDaTela * 0.5f, alturaDaTela * 0.5f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    if (!ImGui::Begin("Mapa Mundial", &mapaMundialAberto)) {
        ImGui::End();
        return;
    }
    if (mundo.mapas.empty()) {
        ImGui::TextUnformatted("O mundo não foi lido (data/mundo/).");
        ImGui::End();
        return;
    }
    // A malha: cada mapa é um botão. "?" = nunca visto (a névoa, regra 4.10).
    const int mapaDoGrupo = grupo.emTransito ? grupo.proximoMapa : grupo.mapaAtual;
    for (int linha = 1; linha <= mundo.linhas; ++linha) {
        for (int coluna = 1; coluna <= mundo.colunas; ++coluna) {
            const int numero = mundo.numeroDoMapaNa(coluna, linha);
            const RegistroDaNevoa& registro = viagem.nevoa[numero - 1];
            const bool visto = registro.diaEmQueFoiVisto >= 0;
            std::string rotulo = visto ? (registro.nivelDaCidade > 0 ? "Cidade " + std::to_string(registro.nivelDaCidade) : mundo.mapa(numero).terreno) : "?";
            if (numero == mapaDoGrupo) {
                rotulo = "[Grupo] " + rotulo;
            }
            rotulo += "##mapa" + std::to_string(numero);
            ImGui::PushStyleColor(ImGuiCol_Button, visto ? corDoEstadoDoTerritorio(registro.estado) : ImVec4(0.12f, 0.12f, 0.14f, 1.0f));
            if (numero == destinoEscolhido) {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
            }
            if (ImGui::Button(rotulo.c_str(), ImVec2(86, 52))) {
                destinoEscolhido = numero;
            }
            if (numero == destinoEscolhido) {
                ImGui::PopStyleVar();
            }
            ImGui::PopStyleColor();
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("%s, região %d", mundo.nomeDoMapa(numero).c_str(), mundo.mapa(numero).regiao);
                if (visto) {
                    ImGui::Text("%s, %s, %s", mundo.mapa(numero).terreno.c_str(), infraestruturaDoMapa(mundo, numero).c_str(), nomeDoEstadoDoTerritorio(registro.estado));
                    ImGui::Text("Visto no dia %lld", static_cast<long long>(registro.diaEmQueFoiVisto + 1));
                    // Comércio, 4.8: os preços que o jogador viu, e há quantos dias.
                    const PrecosConhecidos& precos = viagem.precosConhecidos[static_cast<std::size_t>(numero - 1)];
                    if (precos.diaEmQueFoiVisto >= 0) {
                        std::string texto = "Preços (há " + std::to_string(simulacao.relogio.diaDaPartida() - precos.diaEmQueFoiVisto) + " dias):";
                        for (std::size_t item = 0; item < precos.precoPorItemEmDecimos.size(); ++item) {
                            texto += " " + simulacao.dados.tabelaDeTextos.textoDe(simulacao.dados.catalogoDeItens.itemPeloNumero(static_cast<int>(item)).identificadorDeTexto)
                                + " " + dinheiro(precos.precoPorItemEmDecimos[item]) + ";";
                        }
                        ImGui::PushTextWrapPos(360);
                        ImGui::TextUnformatted(texto.c_str());
                        ImGui::PopTextWrapPos();
                    }
                } else {
                    ImGui::TextUnformatted("Nunca visto");
                }
                ImGui::EndTooltip();
            }
            if (coluna < mundo.colunas) {
                ImGui::SameLine();
            }
        }
    }
    ImGui::Separator();

    // O grupo e a partida (regra 4.1).
    if (grupo.emTransito) {
        ImGui::Text("Viajando para %s: trecho até %s, %d de %d ticks%s.", mundo.nomeDoMapa(grupo.destino()).c_str(), mundo.nomeDoMapa(grupo.proximoMapa).c_str(),
                    grupo.ticksAndadosNoTrecho, grupo.ticksDoTrecho, grupo.ticksParadoNaLuta > 0 ? " (parado depois da luta)" : "");
        ImGui::Text("O grupo leva %s moedas.%s", dinheiro(grupo.dinheiroEmDecimos).c_str(), viagem.grupoSemComida ? " Está sem comida." : "");
        ImGui::End();
        return;
    }
    ImGui::Text("O grupo está em %s.", mundo.nomeDoMapa(grupo.mapaAtual).c_str());
    // Comércio (4.7): numa outra cidade, o grupo compra e vende na loja dela com o que leva.
    if (!grupoEstaNaVila(viagem, simulacao.dados)) {
        SimulacaoDaVila* cidade = nullptr;
        for (const auto& outra : simulacao.outrasCidades) {
            cidade = outra->numeroDoMapaDaCidade == grupo.mapaAtual ? outra.get() : cidade;
        }
        if (cidade != nullptr && ImGui::CollapsingHeader("Loja desta cidade", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("O grupo leva %s moedas.", dinheiro(grupo.dinheiroEmDecimos).c_str());
            const ContextoDaProducao loja = cidade->contextoDaProducao();
            const RegrasDoComercio& regras = simulacao.dados.regrasDoComercio;
            if (ImGui::BeginTable("##loja_da_cidade", 5, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Item");
                ImGui::TableSetupColumn("A loja vende");
                ImGui::TableSetupColumn("A loja paga");
                ImGui::TableSetupColumn("O grupo tem");
                ImGui::TableSetupColumn("");
                ImGui::TableHeadersRow();
                for (int item = 0; item < simulacao.dados.catalogoDeItens.quantidadeDeItens(); ++item) {
                    const int precoLocal = precoLocalDoItem(loja, item);
                    ImGui::PushID(item);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(simulacao.dados.tabelaDeTextos.textoDe(simulacao.dados.catalogoDeItens.itemPeloNumero(item).identificadorDeTexto).c_str());
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(dinheiro(precoLocal * regras.lojaVendeEmPorcento / 100).c_str());
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(dinheiro(calcularAVendaParaALoja(loja, item, 1).valorEmDecimos).c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", grupo.quantidadePorItem[static_cast<std::size_t>(item)]);
                    ImGui::TableNextColumn();
                    for (const int quantidade : {1, 10}) {
                        if (ImGui::SmallButton(("Comprar " + std::to_string(quantidade)).c_str())) {
                            simulacao.pedidos.acrescentar(PedidoComercioNaCidade{item, quantidade, true});
                        }
                        ImGui::SameLine();
                    }
                    for (const int quantidade : {1, 10}) {
                        if (ImGui::SmallButton(("Vender " + std::to_string(quantidade)).c_str())) {
                            simulacao.pedidos.acrescentar(PedidoComercioNaCidade{item, quantidade, false});
                        }
                        ImGui::SameLine();
                    }
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
        }
    }
    if (destinoEscolhido == 0 || destinoEscolhido == grupo.mapaAtual) {
        ImGui::TextUnformatted("Clique num mapa para escolher o destino.");
        ImGui::End();
        return;
    }
    EntidadeEmTransito comoVai = grupo;
    comoVai.marchaForcada = marchaForcadaEscolhida;
    const std::vector<int> rota = buscarRotaMaisRapida(grupo.mapaAtual, destinoEscolhido, comoVai, mundo, regras);
    int ticksDaRota = 0;
    for (const int mapa : rota) {
        ticksDaRota += calcularTicksDoTrecho(comoVai, mundo, mapa, 100, regras);
    }
    ImGui::Text("Destino: %s, %d mapas, cerca de %d h andando (sem o clima e as noites).", mundo.nomeDoMapa(destinoEscolhido).c_str(), static_cast<int>(rota.size()),
                ticksDaRota / 800);
    const bool saiDaVila = grupoEstaNaVila(viagem, simulacao.dados);
    if (saiDaVila) {
        ImGui::SetNextItemWidth(120);
        ImGui::InputInt("Pães para levar", &paesParaLevar);
        ImGui::SetNextItemWidth(120);
        ImGui::InputInt("Moedas para levar", &moedasParaLevar);
        paesParaLevar = std::max(0, paesParaLevar);
        moedasParaLevar = std::max(0, moedasParaLevar);
    }
    ImGui::Checkbox("Marcha forçada (não acampa; mais encontros)", &marchaForcadaEscolhida);
    if (ImGui::Button("Partir")) {
        simulacao.pedidos.acrescentar(PedidoPartirEmViagem{destinoEscolhido, saiDaVila ? paesParaLevar : 0,
                                                           saiDaVila ? static_cast<int64_t>(moedasParaLevar) * 10 : 0, marchaForcadaEscolhida});
    }
    ImGui::End();
}

void InterfaceDoJogador::desenharOEncontro(SimulacaoDaVila& simulacao, float larguraDaTela, float alturaDaTela)
{
    const RegrasDaViagem& regras = simulacao.dados.regrasDaViagem;
    const EntidadeEmTransito& grupo = simulacao.viagem.grupoDoJogador;
    const int bandidos = grupo.encontroMarcado.bandidos;
    ImGui::SetNextWindowPos(ImVec2(larguraDaTela * 0.5f, alturaDaTela * 0.4f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Encontro na estrada", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Um bando de %d bandidos cerca o grupo (força %d contra %d).", bandidos, forcaDoBando(bandidos, regras), forcaDoGrupo(grupo, regras));
    const auto escolher = [&](EscolhaNoEncontro escolha) {
        simulacao.pedidos.acrescentar(PedidoResolverEncontro{escolha});
        if (simulacao.relogio.jogoEstaPausado()) {
            simulacao.relogio.alternarPausa();  // o encontro pausou; a escolha volta a andar
        }
    };
    const int64_t suborno = precoDoSubornoEmDecimos(bandidos, regras.carismaDoJogadorAteOsAtributos, regras);
    if (grupo.dinheiroEmDecimos >= suborno) {  // só aparece se o grupo tiver o dinheiro
        if (ImGui::Button(("Subornar (" + dinheiro(suborno) + " moedas)").c_str())) {
            escolher(EscolhaNoEncontro::Subornar);
        }
    }
    if (ImGui::Button(("Fugir (" + std::to_string(chanceDeFugirEmPorcento(grupo, bandidos, regras)) + "%; se falhar, luta)").c_str())) {
        escolher(EscolhaNoEncontro::Fugir);
    }
    const int forca = forcaDoGrupo(grupo, regras);
    const int chanceDeVencer = forca * 100 / std::max(1, forca + forcaDoBando(bandidos, regras));
    if (ImGui::Button(("Lutar (" + std::to_string(chanceDeVencer) + "% de vencer; perder tira " + std::to_string(regras.perdaAoPerderALutaEmPorcento) + "%)").c_str())) {
        escolher(EscolhaNoEncontro::Lutar);
    }
    if (ImGui::Button(("Render-se (perde " + std::to_string(regras.perdaAoSeRenderEmPorcento) + "% do que leva)").c_str())) {
        escolher(EscolhaNoEncontro::Render);
    }
    ImGui::End();
}
