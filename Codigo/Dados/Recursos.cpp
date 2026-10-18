#include "Dados/Recursos.h"

#include <set>

#include "Dados/TabelaCsv.h"

namespace {

const char* const nomeDoArquivoDeRecursos = "recursos.csv";

// O nome da área: letras minúsculas sem acento, números e _ (como o resto de um ID).
bool nomeDaAreaEhValido(const std::string& nomeDaArea)
{
    return identificadorEhValido("x." + nomeDaArea, "x.");
}

}  // namespace

std::optional<int> CatalogoDeRecursos::procurarRecursoPelaArea(const std::string& nomeDaArea) const
{
    for (std::size_t posicao = 0; posicao < recursos.size(); ++posicao) {
        if (recursos[posicao].nomeDaAreaNoTiled == nomeDaArea) {
            return static_cast<int>(posicao);
        }
    }
    return std::nullopt;
}

CatalogoDeRecursos lerArquivoDeRecursos(const std::string& textoDoArquivo, const CatalogoDeItens& catalogoDeItens,
                                        const CatalogoDeConstrucoes& catalogoDeConstrucoes, const RegrasDoJogo& regras,
                                        ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDeRecursos catalogo;
    const TabelaCsv tabela = lerTabelaCsv(textoDoArquivo);

    // Passo 1: as 5 colunas obrigatórias.
    bool faltaColuna = false;
    for (const char* coluna : {"id", "item_gerado", "construcao", "area_no_tiled", "grupo_da_estacao"}) {
        if (tabela.posicaoDaColuna(coluna) < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRecursos, 1, std::string("falta a coluna \"") + coluna + "\" no cabeçalho");
            faltaColuna = true;
        }
    }
    if (faltaColuna) {
        return catalogo;
    }

    std::set<std::string> identificadoresJaLidos;
    for (const LinhaDaTabelaCsv& linha : tabela.linhas) {
        auto anotarErroDaLinha = [&](const std::string& motivo) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRecursos, linha.numeroDaLinhaNoArquivo, motivo);
        };
        bool linhaCerta = true;
        DefinicaoDeRecurso recurso;

        // 2.1 O ID.
        recurso.identificadorDeTexto = tabela.valorNaLinha(linha, "id");
        if (!identificadorEhValido(recurso.identificadorDeTexto, "recurso.")) {
            anotarErroDaLinha("o ID \"" + recurso.identificadorDeTexto + "\" precisa começar com \"recurso.\" e ter só letras minúsculas, números e _");
            linhaCerta = false;
        } else if (identificadoresJaLidos.count(recurso.identificadorDeTexto) > 0) {
            anotarErroDaLinha(recurso.identificadorDeTexto + " aparece duas vezes");
            linhaCerta = false;
        }

        // 2.2 O item gerado.
        const std::string itemGerado = tabela.valorNaLinha(linha, "item_gerado");
        const std::optional<int> numeroDoItem = catalogoDeItens.procurarNumeroDoItem(itemGerado);
        if (!numeroDoItem.has_value()) {
            anotarErroDaLinha("o item " + itemGerado + " não existe");
            linhaCerta = false;
        } else {
            recurso.numeroDoItemGerado = *numeroDoItem;
        }

        // 2.3 A construção.
        recurso.construcaoQueExtrai = tabela.valorNaLinha(linha, "construcao");
        if (!catalogoDeConstrucoes.existeConstrucao(recurso.construcaoQueExtrai)) {
            anotarErroDaLinha("a construção " + recurso.construcaoQueExtrai + " não existe");
            linhaCerta = false;
        }

        // 2.4 O nome da área no Tiled.
        recurso.nomeDaAreaNoTiled = tabela.valorNaLinha(linha, "area_no_tiled");
        if (!nomeDaAreaEhValido(recurso.nomeDaAreaNoTiled)) {
            anotarErroDaLinha("o nome da área \"" + recurso.nomeDaAreaNoTiled + "\" precisa ter só letras minúsculas sem acento, números e _");
            linhaCerta = false;
        } else if (catalogo.procurarRecursoPelaArea(recurso.nomeDaAreaNoTiled).has_value()) {
            anotarErroDaLinha("a área " + recurso.nomeDaAreaNoTiled + " já é de outro recurso");
            linhaCerta = false;
        }

        // 2.5 O grupo da estação.
        recurso.grupoDaEstacao = tabela.valorNaLinha(linha, "grupo_da_estacao");
        if (regras.producaoPorEstacaoEmPorcento.count(recurso.grupoDaEstacao) == 0) {
            anotarErroDaLinha("o grupo da estação \"" + recurso.grupoDaEstacao + "\" não existe no regras.json");
            linhaCerta = false;
        }

        if (linhaCerta) {
            identificadoresJaLidos.insert(recurso.identificadorDeTexto);
            catalogo.acrescentarRecurso(recurso);
        }
    }
    return catalogo;
}

void ligarAreasDoMapaAosRecursos(MapaLocal& mapaLocal, const std::string& nomeDoArquivoDoMapa, const CatalogoDeRecursos& catalogoDeRecursos,
                                 const RegrasDoJogo& regras, ListaDeProblemasDosDados& listaDeProblemas)
{
    int posicaoDaArea = 0;
    for (AreaDeRecurso& area : mapaLocal.areasDeRecurso) {
        ++posicaoDaArea;
        const std::string nomeDaArea = "a área de recurso " + std::to_string(posicaoDaArea);

        // Passo 1: o recurso pelo nome da área.
        const std::optional<int> numeroDoRecurso = catalogoDeRecursos.procurarRecursoPelaArea(area.tipoDeRecurso);
        if (!numeroDoRecurso.has_value()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDoMapa, 0, nomeDaArea + " tem o tipo \"" + area.tipoDeRecurso + "\", que nenhum recurso usa");
            continue;
        }
        area.numeroDoRecurso = *numeroDoRecurso;

        // Passo 2: a produtividade (vazia = normal).
        if (area.textoDaProdutividade.empty() || area.textoDaProdutividade == "normal") {
            area.produtividadeEmPorcento = regras.produtividadeNormalEmPorcento;
        } else if (area.textoDaProdutividade == "destaque") {
            area.produtividadeEmPorcento = regras.produtividadeDeDestaqueEmPorcento;
        } else {
            listaDeProblemas.anotarErro(nomeDoArquivoDoMapa, 0, nomeDaArea + " tem a produtividade \"" + area.textoDaProdutividade
                                        + "\"; use normal ou destaque");
        }
    }
}

double calcularFatorDeProducaoEmPorcento(int produtividadeEmPorcento, const std::array<int, 4>& producaoDoGrupoPorEstacao,
                                         Estacao estacaoAtual)
{
    const int producaoDaEstacao = producaoDoGrupoPorEstacao[static_cast<int>(estacaoAtual)];
    return produtividadeEmPorcento * producaoDaEstacao / 100.0;
}
