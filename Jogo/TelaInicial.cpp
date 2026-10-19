#include "TelaInicial.h"

#include "imgui.h"

void TelaInicial::abrir(const std::string& pastaDosSaves)
{
    this->pastaDosSaves = pastaDosSaves;
    espacos = listarOsEspacosDeSave(pastaDosSaves);
    mostrandoALista = false;
}

PedidoDeSave TelaInicial::desenhar(float larguraDaTela, float alturaDaTela, const std::string& mensagem)
{
    PedidoDeSave pedido;
    ImGui::SetNextWindowPos(ImVec2(larguraDaTela / 2.0f, alturaDaTela / 2.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(520.0f, 0.0f));
    ImGui::Begin("##tela_inicial", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowFontScale(1.8f);
    ImGui::TextUnformatted("Projeto Jogo");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Separator();

    const std::optional<EspacoDeSave> maisNovo = acharOSaveMaisNovo(espacos);
    if (!mostrandoALista) {
        ImGui::BeginDisabled(!maisNovo.has_value());
        const std::string textoDoContinuar = maisNovo.has_value() ? "Continuar (" + maisNovo->rotulo + ": " + maisNovo->cabecalho->dataDoJogo + ")" : "Continuar";
        if (ImGui::Button((textoDoContinuar + "##continuar").c_str(), ImVec2(-1.0f, 0.0f))) {
            pedido.tipo = TipoDePedidoDeSave::Continuar;
        }
        ImGui::EndDisabled();
        if (ImGui::Button("Novo jogo", ImVec2(-1.0f, 0.0f))) {
            pedido.tipo = TipoDePedidoDeSave::NovoJogo;
        }
        ImGui::BeginDisabled(!maisNovo.has_value());
        if (ImGui::Button("Carregar", ImVec2(-1.0f, 0.0f))) {
            espacos = listarOsEspacosDeSave(pastaDosSaves);
            mostrandoALista = true;
        }
        ImGui::EndDisabled();
        if (ImGui::Button("Sair", ImVec2(-1.0f, 0.0f))) {
            pedido.tipo = TipoDePedidoDeSave::FecharOJogo;
        }
    } else {
        ImGui::TextUnformatted("Carregar qual save?");
        const int espacoClicado = desenharAListaDeEspacos(espacos, false);
        if (espacoClicado >= 0) {
            pedido.tipo = TipoDePedidoDeSave::CarregarEspaco;
            pedido.espaco = espacoClicado;
        }
        if (ImGui::Button("Voltar", ImVec2(-1.0f, 0.0f))) {
            mostrandoALista = false;
        }
    }

    if (!mensagem.empty()) {
        ImGui::Separator();
        ImGui::PushTextWrapPos(0.0f);
        ImGui::TextColored(ImVec4(1.0f, 0.55f, 0.35f, 1.0f), "%s", mensagem.c_str());
        ImGui::PopTextWrapPos();
    }
    ImGui::End();
    return pedido;
}
