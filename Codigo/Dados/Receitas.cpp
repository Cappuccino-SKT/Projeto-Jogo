#include "Dados/Receitas.h"

#include <set>

#include "Dados/TabelaCsv.h"

namespace {

const char* const nomeDoArquivoDeReceitas = "receitas.csv";

// Lê "item.grao:2|item.agua:1". Devolve false e explica o motivo se algo estiver errado.
bool lerInsumos(const std::string& textoDosInsumos, int numeroDoProduto, const CatalogoDeItens& catalogoDeItens,
                std::vector<InsumoDaReceita>& insumos, std::string& motivoDoErro)
{
    insumos.clear();
    if (textoDosInsumos.empty()) {
        return true;  // as Braçais não têm insumo
    }
    std::set<int> itensJaUsados;
    std::size_t inicio = 0;
    while (inicio <= textoDosInsumos.size()) {
        std::size_t fim = textoDosInsumos.find('|', inicio);
        if (fim == std::string::npos) {
            fim = textoDosInsumos.size();
        }
        const std::string pedaco = textoDosInsumos.substr(inicio, fim - inicio);
        inicio = fim + 1;

        const std::size_t doisPontos = pedaco.find(':');
        if (doisPontos == std::string::npos) {
            motivoDoErro = "o insumo \"" + pedaco + "\" precisa ser item:quantidade";
            return false;
        }
        const std::string identificadorDoItem = pedaco.substr(0, doisPontos);
        InsumoDaReceita insumo;
        if (!lerNumeroInteiroDoTexto(pedaco.substr(doisPontos + 1), insumo.quantidade) || insumo.quantidade < 1) {
            motivoDoErro = "a quantidade do insumo " + identificadorDoItem + " precisa ser um inteiro de 1 para cima";
            return false;
        }
        const std::optional<int> numeroDoItem = catalogoDeItens.procurarNumeroDoItem(identificadorDoItem);
        if (!numeroDoItem.has_value()) {
            motivoDoErro = "o insumo " + identificadorDoItem + " não existe";
            return false;
        }
        if (*numeroDoItem == numeroDoProduto) {
            motivoDoErro = "o produto não pode ser insumo dele mesmo";
            return false;
        }
        if (itensJaUsados.count(*numeroDoItem) > 0) {
            motivoDoErro = "o insumo " + identificadorDoItem + " aparece duas vezes";
            return false;
        }
        itensJaUsados.insert(*numeroDoItem);
        insumo.numeroDoItem = *numeroDoItem;
        insumos.push_back(insumo);
    }
    if (insumos.size() > 3) {
        motivoDoErro = "a receita tem " + std::to_string(insumos.size()) + " tipos de insumo; o máximo é 3";
        return false;
    }
    return true;
}

int salarioPorHoraEmCentesimos(TrabalhoDaReceita trabalho, const RegrasDoPreco& regrasDoPreco)
{
    switch (trabalho) {
    case TrabalhoDaReceita::Bracal: return regrasDoPreco.salarioPorHoraBracalEmCentesimos;
    case TrabalhoDaReceita::Manufaturado: return regrasDoPreco.salarioPorHoraManufaturadoEmCentesimos;
    case TrabalhoDaReceita::Especialista: return regrasDoPreco.salarioPorHoraEspecialistaEmCentesimos;
    }
    return 0;
}

}  // namespace

std::optional<int> CatalogoDeReceitas::procurarReceitaDoProduto(int numeroDoProduto) const
{
    for (std::size_t posicao = 0; posicao < receitas.size(); ++posicao) {
        if (receitas[posicao].numeroDoProduto == numeroDoProduto) {
            return static_cast<int>(posicao);
        }
    }
    return std::nullopt;
}

CatalogoDeReceitas lerArquivoDeReceitas(const std::string& textoDoArquivo, const CatalogoDeItens& catalogoDeItens,
                                        const CatalogoDeConstrucoes& catalogoDeConstrucoes, ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDeReceitas catalogo;
    const TabelaCsv tabela = lerTabelaCsv(textoDoArquivo);

    // Passo 1: as 10 colunas obrigatórias.
    bool faltaColuna = false;
    for (const char* coluna : {"id", "produto", "lote", "insumos", "construcao", "nivel_min", "trabalho", "hab_min", "horas", "era"}) {
        if (tabela.posicaoDaColuna(coluna) < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeReceitas, 1, std::string("falta a coluna \"") + coluna + "\" no cabeçalho");
            faltaColuna = true;
        }
    }
    if (faltaColuna) {
        return catalogo;
    }

    std::set<std::string> identificadoresJaLidos;
    for (const LinhaDaTabelaCsv& linha : tabela.linhas) {
        auto anotarErroDaLinha = [&](const std::string& motivo) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeReceitas, linha.numeroDaLinhaNoArquivo, motivo);
        };
        // Lê um inteiro e confere a faixa; anota o erro com o nome do campo.
        auto lerInteiroNaFaixa = [&](const char* coluna, const char* nomeParaMostrar, int minimo, int maximo, int& valor) {
            if (!lerNumeroInteiroDoTexto(tabela.valorNaLinha(linha, coluna), valor) || valor < minimo || valor > maximo) {
                anotarErroDaLinha(std::string(nomeParaMostrar) + " precisa ser um inteiro de " + std::to_string(minimo)
                                  + (maximo < 1000000 ? " a " + std::to_string(maximo) : " para cima"));
                return false;
            }
            return true;
        };
        bool linhaCerta = true;
        DefinicaoDeReceita receita;
        receita.numeroDaLinhaNoArquivo = linha.numeroDaLinhaNoArquivo;

        receita.identificadorDeTexto = tabela.valorNaLinha(linha, "id");
        if (!identificadorEhValido(receita.identificadorDeTexto, "receita.")) {
            anotarErroDaLinha("o ID \"" + receita.identificadorDeTexto + "\" precisa começar com \"receita.\" e ter só letras minúsculas, números e _");
            linhaCerta = false;
        } else if (identificadoresJaLidos.count(receita.identificadorDeTexto) > 0) {
            anotarErroDaLinha(receita.identificadorDeTexto + " aparece duas vezes");
            linhaCerta = false;
        }

        // O produto: existe e ainda não tem receita (uma por produto, IP1-1).
        const std::string produto = tabela.valorNaLinha(linha, "produto");
        const std::optional<int> numeroDoProduto = catalogoDeItens.procurarNumeroDoItem(produto);
        if (!numeroDoProduto.has_value()) {
            anotarErroDaLinha("o produto " + produto + " não existe");
            linhaCerta = false;
        } else {
            receita.numeroDoProduto = *numeroDoProduto;
            const std::optional<int> receitaQueJaExiste = catalogo.procurarReceitaDoProduto(*numeroDoProduto);
            if (receitaQueJaExiste.has_value()) {
                const DefinicaoDeReceita& outra = catalogo.todasAsReceitas()[*receitaQueJaExiste];
                anotarErroDaLinha(produto + " já tem a receita " + outra.identificadorDeTexto + " (linha "
                                  + std::to_string(outra.numeroDaLinhaNoArquivo) + ")");
                linhaCerta = false;
            }
        }

        linhaCerta = lerInteiroNaFaixa("lote", "o lote", 1, 1000000, receita.loteProduzido) && linhaCerta;

        std::string motivoDoErroDosInsumos;
        if (!lerInsumos(tabela.valorNaLinha(linha, "insumos"), receita.numeroDoProduto, catalogoDeItens, receita.insumos, motivoDoErroDosInsumos)) {
            anotarErroDaLinha(motivoDoErroDosInsumos);
            linhaCerta = false;
        }

        receita.construcaoQueProduz = tabela.valorNaLinha(linha, "construcao");
        if (!catalogoDeConstrucoes.existeConstrucao(receita.construcaoQueProduz)) {
            anotarErroDaLinha("a construção " + receita.construcaoQueProduz + " não existe");
            linhaCerta = false;
        }

        linhaCerta = lerInteiroNaFaixa("nivel_min", "o nível mínimo", 1, 5, receita.nivelMinimoDaConstrucao) && linhaCerta;

        const std::string trabalho = tabela.valorNaLinha(linha, "trabalho");
        if (trabalho == "bracal") {
            receita.trabalhoUsado = TrabalhoDaReceita::Bracal;
        } else if (trabalho == "manufaturado") {
            receita.trabalhoUsado = TrabalhoDaReceita::Manufaturado;
        } else if (trabalho == "especialista") {
            receita.trabalhoUsado = TrabalhoDaReceita::Especialista;
        } else {
            anotarErroDaLinha("o trabalho \"" + trabalho + "\" não existe; use bracal, manufaturado ou especialista");
            linhaCerta = false;
        }

        linhaCerta = lerInteiroNaFaixa("hab_min", "a habilidade mínima", 0, 100, receita.habilidadeMinima) && linhaCerta;
        linhaCerta = lerInteiroNaFaixa("horas", "as horas", 1, 1000000, receita.horasPorCiclo) && linhaCerta;
        linhaCerta = lerInteiroNaFaixa("era", "a era", 1, 4, receita.eraMinima) && linhaCerta;

        if (linhaCerta) {
            identificadoresJaLidos.insert(receita.identificadorDeTexto);
            catalogo.acrescentarReceita(receita);
        }
    }
    return catalogo;
}

long long arredondarMetadeParaCima(long long numerador, long long denominador)
{
    return (2 * numerador + denominador) / (2 * denominador);
}

int calcularPrecoDeUmaReceitaEmDecimos(const DefinicaoDeReceita& receita, const CatalogoDeItens& catalogoDeItens,
                                       const RegrasDoPreco& regrasDoPreco)
{
    // Passo 1: o custo em centésimos (insumos em décimos x 10, mais as horas x o salário).
    long long custoEmCentesimos = 0;
    for (const InsumoDaReceita& insumo : receita.insumos) {
        custoEmCentesimos += static_cast<long long>(insumo.quantidade) * catalogoDeItens.itemPeloNumero(insumo.numeroDoItem).precoBaseEmDecimos * 10;
    }
    custoEmCentesimos += static_cast<long long>(receita.horasPorCiclo) * salarioPorHoraEmCentesimos(receita.trabalhoUsado, regrasDoPreco);

    // Passo 2: x margem (%) x multiplicador do item (%) ÷ (lote x 100 x 100 x 10), para voltar a décimos.
    const int multiplicadorDoItem = catalogoDeItens.itemPeloNumero(receita.numeroDoProduto).multiplicadorDoPrecoEmPorcento;
    const long long numerador = custoEmCentesimos * regrasDoPreco.margemDoDonoEmPorcento * multiplicadorDoItem;
    const long long denominador = static_cast<long long>(receita.loteProduzido) * 100 * 100 * 10;

    // Passos 3 e 5: a metade sobe, e nada fica de graça.
    const long long precoEmDecimos = arredondarMetadeParaCima(numerador, denominador);
    return precoEmDecimos < 1 ? 1 : static_cast<int>(precoEmDecimos);
}

void calcularPrecosBaseDosItens(CatalogoDeItens& catalogoDeItens, const CatalogoDeReceitas& catalogoDeReceitas,
                                const RegrasDoPreco& regrasDoPreco, ListaDeProblemasDosDados& listaDeProblemas)
{
    // Passo 1: todos sem preço.
    std::vector<DefinicaoDeItem>& itens = catalogoDeItens.todosOsItensParaMudar();
    std::vector<bool> itemTemPreco(itens.size(), false);
    for (DefinicaoDeItem& item : itens) {
        item.precoBaseEmDecimos = 0;
    }

    // Passo 2: rodadas, até nenhuma receita conseguir dar preço a um item novo.
    bool algumItemGanhouPreco = true;
    while (algumItemGanhouPreco) {
        algumItemGanhouPreco = false;
        for (const DefinicaoDeReceita& receita : catalogoDeReceitas.todasAsReceitas()) {
            if (itemTemPreco[receita.numeroDoProduto]) {
                continue;
            }
            bool todosOsInsumosTemPreco = true;
            for (const InsumoDaReceita& insumo : receita.insumos) {
                todosOsInsumosTemPreco = todosOsInsumosTemPreco && itemTemPreco[insumo.numeroDoItem];
            }
            if (!todosOsInsumosTemPreco) {
                continue;
            }
            itens[receita.numeroDoProduto].precoBaseEmDecimos = calcularPrecoDeUmaReceitaEmDecimos(receita, catalogoDeItens, regrasDoPreco);
            itemTemPreco[receita.numeroDoProduto] = true;
            algumItemGanhouPreco = true;
        }
    }

    // Passo 3: quem sobrou sem preço.
    std::string itensPresosNoLaco;
    for (const DefinicaoDeItem& item : itens) {
        if (itemTemPreco[item.numeroInterno]) {
            continue;
        }
        if (!catalogoDeReceitas.procurarReceitaDoProduto(item.numeroInterno).has_value()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeReceitas, 0, item.identificadorDeTexto + " não tem receita, então não tem preço");
        } else {
            itensPresosNoLaco += (itensPresosNoLaco.empty() ? "" : ", ") + item.identificadorDeTexto;
        }
    }
    if (!itensPresosNoLaco.empty()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeReceitas, 0, "não foi possível calcular o preço de " + itensPresosNoLaco
                                    + ": a cadeia tem um laço (um precisa do outro)");
    }
}

void conferirEscadaDeNiveis(const CatalogoDeItens& catalogoDeItens, const CatalogoDeReceitas& catalogoDeReceitas,
                            ListaDeProblemasDosDados& listaDeProblemas)
{
    const int habilidadeMinimaPorNivel[6] = {0, 10, 25, 40, 55, 70};  // IP1-1, níveis 1 a 5
    for (const DefinicaoDeReceita& receita : catalogoDeReceitas.todasAsReceitas()) {
        const DefinicaoDeItem& produto = catalogoDeItens.itemPeloNumero(receita.numeroDoProduto);
        auto avisar = [&](const std::string& motivo) {
            listaDeProblemas.anotarAviso(nomeDoArquivoDeReceitas, receita.numeroDaLinhaNoArquivo, receita.identificadorDeTexto + ": " + motivo);
        };

        // A habilidade mínima pelo nível.
        const int habilidadeEsperada = habilidadeMinimaPorNivel[receita.nivelMinimoDaConstrucao];
        if (receita.habilidadeMinima != habilidadeEsperada) {
            avisar("habilidade mínima " + std::to_string(receita.habilidadeMinima) + ", mas o nível " + std::to_string(receita.nivelMinimoDaConstrucao)
                   + " pede " + std::to_string(habilidadeEsperada));
        }

        if (!produto.tier.has_value()) {
            continue;  // itens com Rank seguem a escada de materiais (fora da fatia)
        }
        const int tier = *produto.tier;

        // O nível mínimo pelo Tier (IP3-1): Tier 0 e 1 no 1, Tier 2 no 2, Tier 3 no 4, Tier 4 no 5.
        const int nivelPorTier[5] = {1, 1, 2, 4, 5};
        if (receita.nivelMinimoDaConstrucao != nivelPorTier[tier]) {
            avisar("nível mínimo " + std::to_string(receita.nivelMinimoDaConstrucao) + ", mas o Tier " + std::to_string(tier) + " pede nível "
                   + std::to_string(nivelPorTier[tier]));
        }

        // O lote e as horas pelo Tier, nos Tiers 1 a 4 (o Tier 0 é Braçal, com regra própria).
        if (tier >= 1) {
            const int lotePorTier[5] = {0, 4, 2, 1, 1};
            const int horasPorTier[5] = {0, 4, 4, 4, 8};
            if (receita.loteProduzido != lotePorTier[tier] || receita.horasPorCiclo != horasPorTier[tier]) {
                avisar("lote " + std::to_string(receita.loteProduzido) + " em " + std::to_string(receita.horasPorCiclo) + " h, mas o Tier "
                       + std::to_string(tier) + " pede " + std::to_string(lotePorTier[tier]) + " em " + std::to_string(horasPorTier[tier]) + " h");
            }
        }
    }
}
