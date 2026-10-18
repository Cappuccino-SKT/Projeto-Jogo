#include "Dados/Construcoes.h"

#include <nlohmann/json.hpp>

#include "Dados/TabelaCsv.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDeConstrucoes = "construcoes.json";

// "pequeno", "medio" ou "grande". Devolve false para outro texto.
bool lerPorteDoTexto(const std::string& texto, PorteDoLote& porte)
{
    if (texto == "pequeno") {
        porte = PorteDoLote::Pequeno;
    } else if (texto == "medio") {
        porte = PorteDoLote::Medio;
    } else if (texto == "grande") {
        porte = PorteDoLote::Grande;
    } else {
        return false;
    }
    return true;
}

// Lê os campos de um tipo de construção (regra 4.1, passo 1). Cada problema vira um erro.
void lerCamposDaConstrucao(const Json& objeto, DefinicaoDeConstrucao& definicao, ListaDeProblemasDosDados& listaDeProblemas)
{
    const std::string& nome = definicao.identificadorDeTexto;
    const auto anotarErro = [&](const std::string& motivo) { listaDeProblemas.anotarErro(nomeDoArquivoDeConstrucoes, 0, nome + ": " + motivo); };

    const auto porte = objeto.find("porte");
    if (porte == objeto.end() || !porte->is_string() || !lerPorteDoTexto(porte->get<std::string>(), definicao.porte)) {
        anotarErro("o \"porte\" precisa ser \"pequeno\", \"medio\" ou \"grande\"");
    }

    const auto guarda = objeto.find("guarda");
    if (guarda == objeto.end() || !guarda->is_array()) {
        anotarErro("falta a lista \"guarda\" (as categorias que ela guarda; [] se nenhuma)");
    } else {
        for (const Json& categoria : *guarda) {
            const std::optional<CategoriaDoItem> categoriaLida =
                categoria.is_string() ? lerCategoriaDoTexto(categoria.get<std::string>()) : std::nullopt;
            if (!categoriaLida.has_value()) {
                anotarErro("a categoria " + categoria.dump() + " do \"guarda\" não existe nos itens");
                continue;
            }
            definicao.categoriasQueGuarda.push_back(*categoriaLida);
        }
    }

    const auto moradores = objeto.find("moradores");
    if (moradores != objeto.end()) {
        if (!moradores->is_number_integer() || moradores->get<int>() <= 0) {
            anotarErro("\"moradores\" precisa ser um número inteiro maior que 0");
        } else {
            definicao.moradores = moradores->get<int>();
        }
    }

    const auto funcao = objeto.find("funcao");
    if (funcao != objeto.end() && funcao->is_string()) {
        definicao.funcao = funcao->get<std::string>();
    } else if (!definicao.ehMoradia()) {
        anotarErro("falta o texto \"funcao\" (o nome da Função das vagas)");
    }

    const auto trabalho = objeto.find("trabalho_sem_receita");
    if (trabalho != objeto.end()) {
        definicao.trabalhoSemReceita = trabalho->is_string() ? lerTrabalhoDoTexto(trabalho->get<std::string>()) : std::nullopt;
        if (!definicao.trabalhoSemReceita.has_value()) {
            anotarErro("o \"trabalho_sem_receita\" precisa ser \"bracal\", \"manufaturado\" ou \"especialista\"");
        }
    }
}

}  // namespace

std::optional<TrabalhoDaReceita> lerTrabalhoDoTexto(const std::string& texto)
{
    if (texto == "bracal") {
        return TrabalhoDaReceita::Bracal;
    }
    if (texto == "manufaturado") {
        return TrabalhoDaReceita::Manufaturado;
    }
    if (texto == "especialista") {
        return TrabalhoDaReceita::Especialista;
    }
    return std::nullopt;
}

const DefinicaoDeConstrucao* CatalogoDeConstrucoes::procurarConstrucao(const std::string& identificadorDeTexto) const
{
    for (const DefinicaoDeConstrucao& definicao : definicoes) {
        if (definicao.identificadorDeTexto == identificadorDeTexto) {
            return &definicao;
        }
    }
    return nullptr;
}

CatalogoDeConstrucoes lerArquivoDeConstrucoes(const std::string& textoDoArquivo, const TabelaDeTextos& tabelaDeTextos,
                                              ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDeConstrucoes catalogo;
    const Json documento = Json::parse(textoDoArquivo, nullptr, false);
    if (documento.is_discarded() || !documento.is_array()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeConstrucoes, 0, "o arquivo precisa ser uma lista JSON: [{\"id\": \"construcao.fazenda\", ...}, ...]");
        return catalogo;
    }
    int posicaoNaLista = 0;
    for (const Json& objeto : documento) {
        ++posicaoNaLista;
        const auto campoId = objeto.is_object() ? objeto.find("id") : objeto.end();
        const std::string identificador = (campoId != objeto.end() && campoId->is_string()) ? campoId->get<std::string>() : "";
        if (!identificadorEhValido(identificador, "construcao.")) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeConstrucoes, 0, "a construção " + std::to_string(posicaoNaLista)
                                        + " da lista precisa de um \"id\" começando com \"construcao.\"");
            continue;
        }
        if (catalogo.existeConstrucao(identificador)) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeConstrucoes, 0, identificador + " aparece duas vezes");
            continue;
        }
        if (!tabelaDeTextos.temTexto(identificador)) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeConstrucoes, 0, identificador + " não tem nome em textos/pt-BR.json");
        }
        DefinicaoDeConstrucao definicao;
        definicao.identificadorDeTexto = identificador;
        lerCamposDaConstrucao(objeto, definicao, listaDeProblemas);
        catalogo.acrescentarConstrucao(definicao);
    }
    return catalogo;
}
