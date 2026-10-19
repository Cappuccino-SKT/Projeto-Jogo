#include "ListaDeSaves.h"

#include <ctime>

#include <SDL3/SDL.h>

#include "imgui.h"

std::string descobrirAPastaDosSaves()
{
    // Sem organização no nome: a pasta fica em AppData\Roaming\Projeto-Jogo\.
    char* pastaDoJogo = SDL_GetPrefPath("", "Projeto-Jogo");
    if (pastaDoJogo == nullptr) {
        return "";
    }
    std::string pasta = std::string(pastaDoJogo) + "saves/";
    SDL_free(pastaDoJogo);
    return pasta;
}

std::vector<EspacoDeSave> listarOsEspacosDeSave(const std::string& pastaDosSaves)
{
    std::vector<EspacoDeSave> espacos;
    for (int espaco = 1; espaco <= espacosDeSaveManual; ++espaco) {
        EspacoDeSave linha;
        linha.espaco = espaco;
        linha.rotulo = "Espaço " + std::to_string(espaco);
        linha.caminhoDoArquivo = pastaDosSaves + nomeDoArquivoDoEspaco(espaco);
        linha.cabecalho = lerOCabecalhoDoArquivo(linha.caminhoDoArquivo);
        espacos.push_back(linha);
    }
    EspacoDeSave automatico;
    automatico.espaco = 0;
    automatico.rotulo = "Automático";
    automatico.caminhoDoArquivo = pastaDosSaves + nomeDoArquivoDoSaveAutomatico;
    automatico.cabecalho = lerOCabecalhoDoArquivo(automatico.caminhoDoArquivo);
    espacos.push_back(automatico);
    return espacos;
}

std::optional<EspacoDeSave> acharOSaveMaisNovo(const std::vector<EspacoDeSave>& espacos)
{
    std::optional<EspacoDeSave> maisNovo;
    for (const EspacoDeSave& espaco : espacos) {
        if (!espaco.temSave()) {
            continue;
        }
        // A data real "AAAA-MM-DD HH:MM" se compara como texto.
        const bool ehMaisNovo = !maisNovo.has_value() || espaco.cabecalho->dataReal > maisNovo->cabecalho->dataReal ||
                                (espaco.cabecalho->dataReal == maisNovo->cabecalho->dataReal && espaco.cabecalho->tickAtual > maisNovo->cabecalho->tickAtual);
        if (ehMaisNovo) {
            maisNovo = espaco;
        }
    }
    return maisNovo;
}

std::string escreverADataRealDeAgora()
{
    const std::time_t agora = std::time(nullptr);
    std::tm hora{};
#ifdef _WIN32
    localtime_s(&hora, &agora);
#else
    localtime_r(&agora, &hora);
#endif
    char texto[32] = {};
    std::strftime(texto, sizeof(texto), "%Y-%m-%d %H:%M", &hora);
    return texto;
}

std::string escreverUmEspacoParaALista(const EspacoDeSave& espaco)
{
    if (!espaco.temSave()) {
        return espaco.rotulo + ": (vazio)";
    }
    return espaco.rotulo + ": " + espaco.cabecalho->dataDoJogo + " (salvo em " + espaco.cabecalho->dataReal + ")";
}

int desenharAListaDeEspacos(const std::vector<EspacoDeSave>& espacos, bool paraSalvar)
{
    int espacoClicado = -1;
    for (const EspacoDeSave& espaco : espacos) {
        if (paraSalvar && espaco.espaco == 0) {
            continue;
        }
        const bool desligado = !paraSalvar && !espaco.temSave();
        if (desligado) {
            ImGui::BeginDisabled();
        }
        const std::string texto = escreverUmEspacoParaALista(espaco) + "##espaco" + std::to_string(espaco.espaco);
        if (ImGui::Button(texto.c_str(), ImVec2(-1.0f, 0.0f))) {
            espacoClicado = espaco.espaco;
        }
        if (desligado) {
            ImGui::EndDisabled();
        }
    }
    return espacoClicado;
}
