#include "Dados/TabelaCsv.h"

#include <charconv>

namespace {

// Separa uma linha pelo ponto e vírgula. "a;;c" vira {"a", "", "c"}.
std::vector<std::string> separarColunas(const std::string& linha)
{
    std::vector<std::string> colunas;
    std::string colunaAtual;
    for (char caractere : linha) {
        if (caractere == ';') {
            colunas.push_back(colunaAtual);
            colunaAtual.clear();
        } else {
            colunaAtual += caractere;
        }
    }
    colunas.push_back(colunaAtual);
    return colunas;
}

}  // namespace

int TabelaCsv::posicaoDaColuna(const std::string& nomeDaColuna) const
{
    for (std::size_t posicao = 0; posicao < nomesDasColunas.size(); ++posicao) {
        if (nomesDasColunas[posicao] == nomeDaColuna) {
            return static_cast<int>(posicao);
        }
    }
    return -1;
}

std::string TabelaCsv::valorNaLinha(const LinhaDaTabelaCsv& linha, const std::string& nomeDaColuna) const
{
    const int posicao = posicaoDaColuna(nomeDaColuna);
    if (posicao < 0 || posicao >= static_cast<int>(linha.valoresDasColunas.size())) {
        return "";
    }
    return linha.valoresDasColunas[posicao];
}

TabelaCsv lerTabelaCsv(const std::string& textoDoArquivo)
{
    TabelaCsv tabela;
    std::string texto = textoDoArquivo;
    // A marca BOM (3 bytes invisíveis) que o Excel põe no começo do UTF-8.
    if (texto.size() >= 3 && texto.compare(0, 3, "\xEF\xBB\xBF") == 0) {
        texto.erase(0, 3);
    }

    bool jaLeuOCabecalho = false;
    int numeroDaLinha = 0;
    std::size_t inicioDaLinha = 0;
    while (inicioDaLinha <= texto.size()) {
        std::size_t fimDaLinha = texto.find('\n', inicioDaLinha);
        if (fimDaLinha == std::string::npos) {
            fimDaLinha = texto.size();
        }
        std::string linha = texto.substr(inicioDaLinha, fimDaLinha - inicioDaLinha);
        inicioDaLinha = fimDaLinha + 1;
        ++numeroDaLinha;

        if (!linha.empty() && linha.back() == '\r') {
            linha.pop_back();  // o fim de linha do Windows
        }
        if (linha.empty() || linha[0] == '#') {
            continue;
        }
        if (!jaLeuOCabecalho) {
            tabela.nomesDasColunas = separarColunas(linha);
            jaLeuOCabecalho = true;
            continue;
        }
        tabela.linhas.push_back({numeroDaLinha, separarColunas(linha)});
    }
    return tabela;
}

bool lerNumeroInteiroDoTexto(const std::string& texto, int& numeroLido)
{
    if (texto.empty()) {
        return false;
    }
    const char* inicio = texto.data();
    const char* fim = texto.data() + texto.size();
    const auto resultado = std::from_chars(inicio, fim, numeroLido);
    return resultado.ec == std::errc() && resultado.ptr == fim;
}

bool identificadorEhValido(const std::string& identificador, const std::string& prefixoEsperado)
{
    if (identificador.size() <= prefixoEsperado.size() || identificador.compare(0, prefixoEsperado.size(), prefixoEsperado) != 0) {
        return false;
    }
    for (std::size_t posicao = prefixoEsperado.size(); posicao < identificador.size(); ++posicao) {
        const char caractere = identificador[posicao];
        const bool permitido = (caractere >= 'a' && caractere <= 'z') || (caractere >= '0' && caractere <= '9') || caractere == '_';
        if (!permitido) {
            return false;
        }
    }
    return true;
}
