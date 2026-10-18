#include "Dados/Itens.h"

#include <nlohmann/json.hpp>

#include "Dados/TabelaCsv.h"

namespace {

const char* const nomeDoArquivoDeItens = "itens.csv";
const char* const nomeDoArquivoDeConstrucoes = "construcoes.json";

struct CategoriaEOsSeusTipos {
    const char* nomeNoArquivo;
    CategoriaDoItem categoria;
    std::vector<std::string> tiposAceitos;
    bool tierEhObrigatorio;  // matéria-prima e bem de consumo têm Tier; o resto, não
};

// As listas fechadas da parte 2 da ficha (do Sistema de Itens).
const std::vector<CategoriaEOsSeusTipos>& categoriasConhecidas()
{
    static const std::vector<CategoriaEOsSeusTipos> categorias = {
        {"materia_prima", CategoriaDoItem::MateriaPrima, {"bruto_mineral", "bruto_vegetal", "bruto_animal", "manufaturado"}, true},
        {"bem_de_consumo", CategoriaDoItem::BemDeConsumo,
         {"alimento", "bebida", "mobilia", "vestuario", "essencia_e_joia", "narcotico", "medicamento"}, true},
        {"arma", CategoriaDoItem::Arma, {"espada", "lanca", "machado", "clava", "arco", "besta", "arma_de_fogo", "cajado", "municao"}, false},
        {"armamento_de_exercito", CategoriaDoItem::ArmamentoDeExercito, {"artilharia"}, false},
        {"equipamento", CategoriaDoItem::Equipamento, {"capacete", "peitoral", "perneiras", "chapeu", "camisa", "calcas", "anel", "escudo"}, false},
        {"artefato", CategoriaDoItem::Artefato, {"livro", "joia_de_mana", "artefato"}, false},
    };
    return categorias;
}

// Lê um inteiro opcional: "" vira vazio; outro texto precisa ser um inteiro.
bool lerInteiroOpcional(const std::string& texto, std::optional<int>& valorLido)
{
    if (texto.empty()) {
        valorLido.reset();
        return true;
    }
    int numero = 0;
    if (!lerNumeroInteiroDoTexto(texto, numero)) {
        return false;
    }
    valorLido = numero;
    return true;
}

}  // namespace

std::optional<int> CatalogoDeItens::procurarNumeroDoItem(const std::string& identificadorDeTexto) const
{
    const auto encontrado = numeroPorIdentificador.find(identificadorDeTexto);
    if (encontrado == numeroPorIdentificador.end()) {
        return std::nullopt;
    }
    return encontrado->second;
}

void CatalogoDeItens::acrescentarItem(DefinicaoDeItem item)
{
    item.numeroInterno = static_cast<int>(itens.size());
    numeroPorIdentificador[item.identificadorDeTexto] = item.numeroInterno;
    itens.push_back(std::move(item));
}

CatalogoDeItens lerArquivoDeItens(const std::string& textoDoArquivo, const TabelaDeTextos& tabelaDeTextos,
                                  ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDeItens catalogo;
    const TabelaCsv tabela = lerTabelaCsv(textoDoArquivo);

    // Passo 2: as colunas obrigatórias.
    bool faltaColuna = false;
    for (const char* colunaObrigatoria : {"id", "categoria", "tipo", "tier", "peso"}) {
        if (tabela.posicaoDaColuna(colunaObrigatoria) < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeItens, 1, std::string("falta a coluna \"") + colunaObrigatoria + "\" no cabeçalho");
            faltaColuna = true;
        }
    }
    if (faltaColuna) {
        return catalogo;
    }

    // Guarda a linha de cada ID, para o erro de repetido citar as duas.
    std::map<std::string, int> linhaDeCadaIdentificador;

    // Passo 3: cada linha.
    for (const LinhaDaTabelaCsv& linha : tabela.linhas) {
        const int numeroDaLinha = linha.numeroDaLinhaNoArquivo;
        auto anotarErroDaLinha = [&](const std::string& motivo) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeItens, numeroDaLinha, motivo);
        };
        bool linhaCerta = true;
        DefinicaoDeItem item;

        // 3.1 O ID.
        item.identificadorDeTexto = tabela.valorNaLinha(linha, "id");
        if (!identificadorEhValido(item.identificadorDeTexto, "item.")) {
            anotarErroDaLinha("o ID \"" + item.identificadorDeTexto + "\" precisa começar com \"item.\" e ter só letras minúsculas sem acento, números e _");
            linhaCerta = false;
        } else if (linhaDeCadaIdentificador.count(item.identificadorDeTexto) > 0) {
            anotarErroDaLinha(item.identificadorDeTexto + " aparece duas vezes (linhas " + std::to_string(linhaDeCadaIdentificador[item.identificadorDeTexto])
                              + " e " + std::to_string(numeroDaLinha) + ")");
            linhaCerta = false;
        }

        // 3.2 A categoria e o tipo.
        const std::string nomeDaCategoria = tabela.valorNaLinha(linha, "categoria");
        const CategoriaEOsSeusTipos* categoriaEncontrada = nullptr;
        for (const CategoriaEOsSeusTipos& categoria : categoriasConhecidas()) {
            if (nomeDaCategoria == categoria.nomeNoArquivo) {
                categoriaEncontrada = &categoria;
            }
        }
        item.tipoDoItem = tabela.valorNaLinha(linha, "tipo");
        if (categoriaEncontrada == nullptr) {
            anotarErroDaLinha("a categoria \"" + nomeDaCategoria + "\" não existe");
            linhaCerta = false;
        } else {
            item.categoriaDoItem = categoriaEncontrada->categoria;
            bool tipoCombina = false;
            for (const std::string& tipoAceito : categoriaEncontrada->tiposAceitos) {
                tipoCombina = tipoCombina || item.tipoDoItem == tipoAceito;
            }
            if (!tipoCombina) {
                anotarErroDaLinha("o tipo \"" + item.tipoDoItem + "\" não existe na categoria " + nomeDaCategoria);
                linhaCerta = false;
            }

            // 3.3 O Tier.
            if (!lerInteiroOpcional(tabela.valorNaLinha(linha, "tier"), item.tier)) {
                anotarErroDaLinha("o Tier precisa ser um número inteiro");
                linhaCerta = false;
            } else if (categoriaEncontrada->tierEhObrigatorio && !item.tier.has_value()) {
                anotarErroDaLinha("falta o Tier: a categoria " + nomeDaCategoria + " sempre tem Tier");
                linhaCerta = false;
            } else if (!categoriaEncontrada->tierEhObrigatorio && item.tier.has_value()) {
                anotarErroDaLinha("a categoria " + nomeDaCategoria + " não tem Tier (tem Rank)");
                linhaCerta = false;
            } else if (item.tier.has_value() && (*item.tier < 0 || *item.tier > 4)) {
                anotarErroDaLinha("o Tier vai de 0 a 4");
                linhaCerta = false;
            }
        }

        // O Rank (fora da fatia, mas lido e conferido).
        if (!lerInteiroOpcional(tabela.valorNaLinha(linha, "rank_min"), item.rankMinimo)
            || !lerInteiroOpcional(tabela.valorNaLinha(linha, "rank_max"), item.rankMaximo)) {
            anotarErroDaLinha("o Rank precisa ser um número inteiro");
            linhaCerta = false;
        }

        // 3.4 O peso e o multiplicador do preço.
        if (!lerNumeroInteiroDoTexto(tabela.valorNaLinha(linha, "peso"), item.pesoEmUnidades) || item.pesoEmUnidades <= 0) {
            anotarErroDaLinha("o peso precisa ser um número inteiro maior que 0");
            linhaCerta = false;
        }
        const std::string textoDoMultiplicador = tabela.valorNaLinha(linha, "multiplicador_do_preco");
        if (!textoDoMultiplicador.empty()
            && (!lerNumeroInteiroDoTexto(textoDoMultiplicador, item.multiplicadorDoPrecoEmPorcento)
                || item.multiplicadorDoPrecoEmPorcento < 1 || item.multiplicadorDoPrecoEmPorcento > 1000)) {
            anotarErroDaLinha("o multiplicador do preço vai de 1 a 1000 (em %)");
            linhaCerta = false;
        }

        // 3.5 O número interno: só as linhas certas entram.
        if (linhaCerta) {
            linhaDeCadaIdentificador[item.identificadorDeTexto] = numeroDaLinha;
            catalogo.acrescentarItem(item);
        }
    }

    // Regra 4.2: os nomes no textos/pt-BR.json.
    for (const DefinicaoDeItem& item : catalogo.todosOsItens()) {
        if (!tabelaDeTextos.temTexto(item.identificadorDeTexto)) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeItens, 0, item.identificadorDeTexto + " não tem nome em textos/pt-BR.json");
        }
    }
    for (const auto& [identificador, texto] : tabelaDeTextos.todosOsTextos()) {
        if (identificador.rfind("item.", 0) == 0 && !catalogo.procurarNumeroDoItem(identificador).has_value()) {
            listaDeProblemas.anotarAviso("textos/pt-BR.json", 0, "o texto de " + identificador + " não é de nenhum item");
        }
    }
    return catalogo;
}

bool CatalogoDeConstrucoes::existeConstrucao(const std::string& identificadorDeTexto) const
{
    for (const std::string& identificador : identificadores) {
        if (identificador == identificadorDeTexto) {
            return true;
        }
    }
    return false;
}

CatalogoDeConstrucoes lerArquivoDeConstrucoes(const std::string& textoDoArquivo, const TabelaDeTextos& tabelaDeTextos,
                                              ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDeConstrucoes catalogo;
    const nlohmann::json documento = nlohmann::json::parse(textoDoArquivo, nullptr, false);
    if (documento.is_discarded() || !documento.is_array()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeConstrucoes, 0, "o arquivo precisa ser uma lista JSON: [{\"id\": \"construcao.fazenda\"}, ...]");
        return catalogo;
    }
    int posicaoNaLista = 0;
    for (const nlohmann::json& construcao : documento) {
        ++posicaoNaLista;
        const auto campoId = construcao.find("id");
        const std::string identificador = (campoId != construcao.end() && campoId->is_string()) ? campoId->get<std::string>() : "";
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
        catalogo.acrescentarConstrucao(identificador);
    }
    return catalogo;
}
