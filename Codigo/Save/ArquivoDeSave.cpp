#include "Save/ArquivoDeSave.h"

#include <filesystem>
#include <fstream>
#include <system_error>

#include <nlohmann/json.hpp>

namespace {

// Os textos do jogo são UTF-8; assim uma pasta com acento também funciona no Windows.
std::filesystem::path caminhoDoTexto(const std::string& texto)
{
    return std::filesystem::path(std::u8string(texto.begin(), texto.end()));
}

}  // namespace

std::string nomeDoArquivoDoEspaco(int espaco)
{
    return "espaco" + std::to_string(espaco) + extensaoDoArquivoDeSave;
}

std::optional<std::vector<uint8_t>> lerOsBytesDoArquivo(const std::string& caminhoDoArquivo)
{
    std::ifstream arquivo(caminhoDoTexto(caminhoDoArquivo), std::ios::binary);
    if (!arquivo) {
        return std::nullopt;
    }
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(arquivo), std::istreambuf_iterator<char>());
}

// Regra 4.1, passo 4: o arquivo temporário só vira o save de verdade depois de
// gravado por inteiro. Se o jogo fechar no meio, o save antigo continua inteiro.
std::string gravarOsBytesNoArquivo(const std::vector<uint8_t>& bytes, const std::string& caminhoDoArquivo)
{
    const std::filesystem::path caminho = caminhoDoTexto(caminhoDoArquivo);
    const std::filesystem::path caminhoTemporario = caminhoDoTexto(caminhoDoArquivo + ".tmp");
    std::error_code erroDoSistema;
    if (caminho.has_parent_path()) {
        std::filesystem::create_directories(caminho.parent_path(), erroDoSistema);
    }
    {
        std::ofstream arquivo(caminhoTemporario, std::ios::binary | std::ios::trunc);
        arquivo.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        arquivo.flush();
        if (!arquivo) {
            return "Não foi possível gravar o save em " + caminhoDoArquivo + ".";
        }
    }
    std::filesystem::rename(caminhoTemporario, caminho, erroDoSistema);
    if (erroDoSistema) {
        std::filesystem::remove(caminhoTemporario, erroDoSistema);
        return "Não foi possível trocar o save antigo pelo novo: " + erroDoSistema.message() + ".";
    }
    return "";
}

std::string gravarAPartidaNoArquivo(const SimulacaoDaVila& simulacao, const std::string& caminhoDoArquivo, const std::string& nomeDoEspaco,
                                    const std::string& dataReal)
{
    return gravarOsBytesNoArquivo(gravarAPartidaEmBytes(simulacao, nomeDoEspaco, dataReal), caminhoDoArquivo);
}

ResultadoDoCarregamento carregarAPartidaDoArquivo(const DadosDoJogo& dados, const RegrasDoTempo& regrasDoTempo,
                                                  int distanciaDoCicloDeAndarEmMilesimos, const std::string& caminhoDoArquivo)
{
    const std::optional<std::vector<uint8_t>> bytes = lerOsBytesDoArquivo(caminhoDoArquivo);
    if (!bytes.has_value()) {
        ResultadoDoCarregamento recusa;
        recusa.motivoDaRecusa = "Não foi possível ler o save (o arquivo não existe).";
        return recusa;
    }
    return carregarAPartidaDeBytes(dados, regrasDoTempo, distanciaDoCicloDeAndarEmMilesimos, *bytes);
}

std::optional<CabecalhoDoSave> lerOCabecalhoDoArquivo(const std::string& caminhoDoArquivo)
{
    const std::optional<std::vector<uint8_t>> bytes = lerOsBytesDoArquivo(caminhoDoArquivo);
    if (!bytes.has_value()) {
        return std::nullopt;
    }
    try {
        const nlohmann::json arvore = nlohmann::json::from_msgpack(*bytes, true, true);
        const nlohmann::json& cabecalho = arvore.at("cabecalho");
        CabecalhoDoSave resultado;
        resultado.versaoDoFormato = cabecalho.at("versaoDoFormato").get<int>();
        resultado.versaoDoJogo = cabecalho.at("versaoDoJogo").get<std::string>();
        resultado.dataReal = cabecalho.at("dataReal").get<std::string>();
        resultado.dataDoJogo = cabecalho.at("dataDoJogo").get<std::string>();
        resultado.nomeDoEspaco = cabecalho.at("nomeDoEspaco").get<std::string>();
        resultado.tickAtual = cabecalho.at("tick").get<int64_t>();
        return resultado;
    } catch (const nlohmann::json::exception&) {
        return std::nullopt;
    }
}

std::string converterOSaveEmTexto(const std::vector<uint8_t>& bytes, bool& deuCerto)
{
    try {
        const nlohmann::json arvore = nlohmann::json::from_msgpack(bytes, true, true);
        deuCerto = true;
        return arvore.dump(2);
    } catch (const nlohmann::json::exception& erro) {
        deuCerto = false;
        return std::string("Não foi possível ler o save: ") + erro.what();
    }
}
