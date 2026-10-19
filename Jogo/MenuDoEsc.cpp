#include "MenuDoEsc.h"

#include "imgui.h"

void MenuDoEsc::abrir(const std::string& pastaDosSaves)
{
    this->pastaDosSaves = pastaDosSaves;
    espacos = listarOsEspacosDeSave(pastaDosSaves);
    subtela = Subtela::Principal;
    aberto = true;
}

void MenuDoEsc::abrirPerguntandoPor(const std::string& pastaDosSaves, const PedidoDeSave& pedido)
{
    abrir(pastaDosSaves);
    pedidoAguardandoConfirmacao = pedido;
    subtela = Subtela::Confirmar;
}

PedidoDeSave MenuDoEsc::pedirComConfirmacao(PedidoDeSave pedido, int horasSemSalvar)
{
    if (horasSemSalvar < 1) {
        return pedido;
    }
    pedidoAguardandoConfirmacao = pedido;
    subtela = Subtela::Confirmar;
    return PedidoDeSave{};
}

PedidoDeSave MenuDoEsc::desenhar(float larguraDaTela, float alturaDaTela, int horasSemSalvar, const std::string& mensagem)
{
    PedidoDeSave pedido;
    if (!aberto) {
        return pedido;
    }
    ImGui::SetNextWindowPos(ImVec2(larguraDaTela / 2.0f, alturaDaTela / 2.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(520.0f, 0.0f));
    ImGui::Begin("Menu##menu_do_esc", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_AlwaysAutoResize);

    if (subtela == Subtela::Principal) {
        if (ImGui::Button("Voltar", ImVec2(-1.0f, 0.0f))) {
            aberto = false;
        }
        if (ImGui::Button("Salvar", ImVec2(-1.0f, 0.0f))) {
            espacos = listarOsEspacosDeSave(pastaDosSaves);
            subtela = Subtela::Salvar;
        }
        if (ImGui::Button("Carregar", ImVec2(-1.0f, 0.0f))) {
            espacos = listarOsEspacosDeSave(pastaDosSaves);
            subtela = Subtela::Carregar;
        }
        if (ImGui::Button("Sair para a tela inicial", ImVec2(-1.0f, 0.0f))) {
            pedido = pedirComConfirmacao(PedidoDeSave{TipoDePedidoDeSave::SairParaATelaInicial, 0}, horasSemSalvar);
        }
    } else if (subtela == Subtela::Salvar || subtela == Subtela::Carregar) {
        const bool paraSalvar = subtela == Subtela::Salvar;
        ImGui::TextUnformatted(paraSalvar ? "Salvar em qual espaço?" : "Carregar qual save?");
        const int espacoClicado = desenharAListaDeEspacos(espacos, paraSalvar);
        if (espacoClicado >= 0) {
            if (paraSalvar) {
                pedido = PedidoDeSave{TipoDePedidoDeSave::SalvarNoEspaco, espacoClicado};
            } else {
                pedido = pedirComConfirmacao(PedidoDeSave{TipoDePedidoDeSave::CarregarEspaco, espacoClicado}, horasSemSalvar);
            }
        }
        if (ImGui::Button("Voltar", ImVec2(-1.0f, 0.0f))) {
            subtela = Subtela::Principal;
        }
    } else {
        ImGui::PushTextWrapPos(0.0f);
        ImGui::Text("Há %d hora%s de jogo sem salvar. Continuar mesmo assim?", horasSemSalvar, horasSemSalvar == 1 ? "" : "s");
        ImGui::PopTextWrapPos();
        if (ImGui::Button("Sim, continuar", ImVec2(-1.0f, 0.0f))) {
            pedido = pedidoAguardandoConfirmacao;
            pedidoAguardandoConfirmacao = PedidoDeSave{};
            subtela = Subtela::Principal;
        }
        if (ImGui::Button("Não, voltar", ImVec2(-1.0f, 0.0f))) {
            pedidoAguardandoConfirmacao = PedidoDeSave{};
            subtela = Subtela::Principal;
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
