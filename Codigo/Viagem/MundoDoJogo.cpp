#include "Viagem/MundoDoJogo.h"

#include <set>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDosMapas = "mundo/mapas.json";
const char* const nomeDoArquivoDasCidades = "mundo/cidades_iniciais.json";

std::string lerTexto(const Json& objeto, const char* nomeDoCampo, const std::string& padrao)
{
    const auto campo = objeto.find(nomeDoCampo);
    return campo != objeto.end() && campo->is_string() ? campo->get<std::string>() : padrao;
}

int lerInteiro(const Json& objeto, const char* nomeDoCampo, int padrao)
{
    const auto campo = objeto.find(nomeDoCampo);
    return campo != objeto.end() && campo->is_number_integer() ? campo->get<int>() : padrao;
}

bool lerDocumento(const std::string& texto, const char* nomeDoArquivo, Json& documento, ListaDeProblemasDosDados& listaDeProblemas)
{
    try {
        documento = Json::parse(texto);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivo, 0, std::string("o JSON está quebrado: ") + erro.what());
        return false;
    }
    if (!documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivo, 0, "o arquivo precisa ser um objeto { ... }");
        return false;
    }
    return true;
}

}  // namespace

std::vector<int> MundoDoJogo::vizinhos(int numeroDoMapa) const
{
    std::vector<int> lista;
    if (!mapaExiste(numeroDoMapa)) {
        return lista;
    }
    const MapaDoMundo& centro = mapa(numeroDoMapa);
    const int deslocamentos[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
    for (const auto& deslocamento : deslocamentos) {
        const int coluna = centro.coluna + deslocamento[0];
        const int linha = centro.linha + deslocamento[1];
        if (coluna >= 1 && coluna <= colunas && linha >= 1 && linha <= this->linhas) {
            lista.push_back(numeroDoMapaNa(coluna, linha));
        }
    }
    return lista;
}

const CidadeDoMundo* MundoDoJogo::cidadeNoMapa(int numeroDoMapa) const
{
    for (const CidadeDoMundo& cidade : cidades) {
        if (cidade.numeroDoMapa == numeroDoMapa) {
            return &cidade;
        }
    }
    return nullptr;
}

EstadoDoTerritorio MundoDoJogo::estadoDoTerritorio(int numeroDoMapa) const
{
    if (cidadeNoMapa(numeroDoMapa) != nullptr) {
        return EstadoDoTerritorio::ComCidade;
    }
    std::set<std::string> faccoesVizinhas;
    for (const int vizinho : vizinhos(numeroDoMapa)) {
        const CidadeDoMundo* cidade = cidadeNoMapa(vizinho);
        if (cidade != nullptr && !cidade->faccao.empty()) {
            faccoesVizinhas.insert(cidade->faccao);  // as Vilas Livres não geram território
        }
    }
    if (faccoesVizinhas.empty()) {
        return EstadoDoTerritorio::SemDono;
    }
    return faccoesVizinhas.size() == 1 ? EstadoDoTerritorio::Territorio : EstadoDoTerritorio::EmDisputa;
}

std::string MundoDoJogo::nomeDoMapa(int numeroDoMapa) const
{
    if (!mapaExiste(numeroDoMapa)) {
        return "(?)";
    }
    const MapaDoMundo& esteMapa = mapa(numeroDoMapa);
    std::string nome = "(" + std::to_string(esteMapa.coluna) + "," + std::to_string(esteMapa.linha) + ")";
    if (const CidadeDoMundo* cidade = cidadeNoMapa(numeroDoMapa)) {
        nome += " " + cidade->nome;
    }
    return nome;
}

MundoDoJogo lerArquivosDoMundo(const std::string& textoDosMapas, const std::string& textoDasCidades, ListaDeProblemasDosDados& listaDeProblemas)
{
    MundoDoJogo mundo;
    Json documento;
    if (lerDocumento(textoDosMapas, nomeDoArquivoDosMapas, documento, listaDeProblemas)) {
        mundo.colunas = lerInteiro(documento, "colunas", 8);
        mundo.linhas = lerInteiro(documento, "linhas", 6);
        mundo.mapas.resize(static_cast<std::size_t>(mundo.colunas * mundo.linhas));
        std::vector<bool> lido(mundo.mapas.size(), false);
        const auto lista = documento.find("mapas");
        if (lista == documento.end() || !lista->is_array()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDosMapas, 0, "falta a lista \"mapas\"");
        } else {
            for (const Json& entrada : *lista) {
                const int coluna = lerInteiro(entrada, "coluna", 0);
                const int linha = lerInteiro(entrada, "linha", 0);
                if (coluna < 1 || coluna > mundo.colunas || linha < 1 || linha > mundo.linhas) {
                    listaDeProblemas.anotarErro(nomeDoArquivoDosMapas, 0, "um mapa está fora da malha (coluna " + std::to_string(coluna) + ", linha " + std::to_string(linha) + ")");
                    continue;
                }
                MapaDoMundo& mapa = mundo.mapas[mundo.numeroDoMapaNa(coluna, linha) - 1];
                mapa.numeroDoMapa = mundo.numeroDoMapaNa(coluna, linha);
                mapa.coluna = coluna;
                mapa.linha = linha;
                mapa.regiao = lerInteiro(entrada, "regiao", 1);
                mapa.bioma = lerTexto(entrada, "bioma", mapa.bioma);
                mapa.terreno = lerTexto(entrada, "terreno", mapa.terreno);
                mapa.infraestrutura = lerTexto(entrada, "infraestrutura", mapa.infraestrutura);
                mapa.arquivoTiled = lerTexto(entrada, "arquivo_tiled", "");
                if (const auto recursos = entrada.find("recursos"); recursos != entrada.end() && recursos->is_object()) {
                    for (const auto& [recurso, produtividade] : recursos->items()) {
                        if (produtividade.is_number_integer() && produtividade.get<int>() > 0) {
                            mapa.produtividadeDosRecursos[recurso] = produtividade.get<int>();
                        } else {
                            listaDeProblemas.anotarErro(nomeDoArquivoDosMapas, 0, "a produtividade de \"" + recurso + "\" precisa ser um inteiro maior que 0");
                        }
                    }
                }
                lido[mapa.numeroDoMapa - 1] = true;
            }
        }
        for (std::size_t posicao = 0; posicao < lido.size(); ++posicao) {
            if (!lido[posicao]) {
                listaDeProblemas.anotarErro(nomeDoArquivoDosMapas, 0, "falta o mapa número " + std::to_string(posicao + 1));
            }
        }
    }
    if (lerDocumento(textoDasCidades, nomeDoArquivoDasCidades, documento, listaDeProblemas)) {
        const auto lista = documento.find("cidades");
        if (lista == documento.end() || !lista->is_array()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDasCidades, 0, "falta a lista \"cidades\"");
            return mundo;
        }
        for (const Json& entrada : *lista) {
            CidadeDoMundo cidade;
            cidade.nome = lerTexto(entrada, "nome", "");
            const int coluna = lerInteiro(entrada, "coluna", 0);
            const int linha = lerInteiro(entrada, "linha", 0);
            cidade.numeroDoMapa = mundo.numeroDoMapaNa(coluna, linha);
            if (coluna < 1 || coluna > mundo.colunas || linha < 1 || linha > mundo.linhas) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasCidades, 0, "a cidade \"" + cidade.nome + "\" está fora da malha");
                continue;
            }
            if (mundo.cidadeNoMapa(cidade.numeroDoMapa) != nullptr) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasCidades, 0, "duas cidades no mesmo mapa (" + cidade.nome + ")");
                continue;
            }
            cidade.nivel = lerInteiro(entrada, "nivel", 1);
            cidade.faccao = lerTexto(entrada, "faccao", "");
            const auto vilaDoJogador = entrada.find("vila_do_jogador");
            cidade.ehAVilaDoJogador = vilaDoJogador != entrada.end() && vilaDoJogador->is_boolean() && vilaDoJogador->get<bool>();
            if (cidade.ehAVilaDoJogador) {
                mundo.mapaDaVila = cidade.numeroDoMapa;
            }
            mundo.cidades.push_back(cidade);
        }
        if (mundo.mapaDaVila == 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDasCidades, 0, "nenhuma cidade tem \"vila_do_jogador\": true");
        }
    }
    return mundo;
}
