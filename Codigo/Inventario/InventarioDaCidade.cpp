#include "Inventario/InventarioDaCidade.h"

#include "Construcao/ContasDasConstrucoes.h"

InventarioDaCidade::InventarioDaCidade(const CatalogoDeItens& catalogoDeItens)
    : quantidadePorItemDaCidade(catalogoDeItens.quantidadeDeItens(), 0), quantidadePorItemDoJogador(catalogoDeItens.quantidadeDeItens(), 0)
{
    for (const DefinicaoDeItem& item : catalogoDeItens.todosOsItens()) {
        categoriaDeCadaItem.push_back(item.categoriaDoItem);
    }
}

int InventarioDaCidade::quantidadeDe(int numeroDoItem, DonoDoItem dono) const
{
    if (!numeroDoItemExiste(numeroDoItem)) {
        return 0;
    }
    return dono == DonoDoItem::Cidade ? quantidadePorItemDaCidade[numeroDoItem] : quantidadePorItemDoJogador[numeroDoItem];
}

int InventarioDaCidade::quantidadeNaCategoria(CategoriaDoItem categoria) const
{
    int quantidade = 0;
    for (std::size_t item = 0; item < categoriaDeCadaItem.size(); ++item) {
        if (categoriaDeCadaItem[item] == categoria) {
            quantidade += quantidadePorItemDaCidade[item] + quantidadePorItemDoJogador[item];
        }
    }
    return quantidade;
}

bool InventarioDaCidade::tentarPor(int numeroDoItem, DonoDoItem dono, int quantidade)
{
    if (!numeroDoItemExiste(numeroDoItem) || quantidade < 0) {
        return false;
    }
    // Passos 1 e 2: o que vai entrar não pode passar da capacidade da categoria.
    if (quantidade > espacoLivreNaCategoria(categoriaDeCadaItem[numeroDoItem])) {
        return false;
    }
    listaDoDono(dono)[numeroDoItem] += quantidade;
    return true;
}

bool InventarioDaCidade::tentarTirar(int numeroDoItem, DonoDoItem dono, int quantidade)
{
    if (!numeroDoItemExiste(numeroDoItem) || quantidade < 0 || quantidadeDe(numeroDoItem, dono) < quantidade) {
        return false;
    }
    listaDoDono(dono)[numeroDoItem] -= quantidade;
    return true;
}

bool InventarioDaCidade::tentarTirarVarios(const std::vector<InsumoDaReceita>& itensParaTirar, DonoDoItem dono)
{
    // Primeiro confere tudo; só depois tira (tudo ou nada).
    for (const InsumoDaReceita& item : itensParaTirar) {
        if (!numeroDoItemExiste(item.numeroDoItem) || quantidadeDe(item.numeroDoItem, dono) < item.quantidade) {
            return false;
        }
    }
    for (const InsumoDaReceita& item : itensParaTirar) {
        listaDoDono(dono)[item.numeroDoItem] -= item.quantidade;
    }
    return true;
}

bool InventarioDaCidade::passarParaOutroDono(int numeroDoItem, DonoDoItem doDono, DonoDoItem paraODono, int quantidade)
{
    if (!tentarTirar(numeroDoItem, doDono, quantidade)) {
        return false;
    }
    listaDoDono(paraODono)[numeroDoItem] += quantidade;
    return true;
}

void recalcularCapacidade(InventarioDaCidade& inventario, const VilaDoJogo& vila, const CatalogoDeConstrucoes& catalogoDeConstrucoes,
                          const RegrasDasConstrucoes& regrasDasConstrucoes)
{
    std::array<int, 6> capacidadeSomada = {};
    for (const ConstrucaoDaVila& construcao : vila.construcoes) {
        const DefinicaoDeConstrucao* definicao = catalogoDeConstrucoes.procurarConstrucao(construcao.tipo);
        if (definicao == nullptr) {
            continue;
        }
        const int capacidadeDaConstrucao = calcularCapacidade(definicao->porte, construcao.nivel, regrasDasConstrucoes);
        for (const CategoriaDoItem categoria : definicao->categoriasQueGuarda) {
            capacidadeSomada[static_cast<int>(categoria)] += capacidadeDaConstrucao;
        }
    }
    for (int categoria = 0; categoria < static_cast<int>(capacidadeSomada.size()); ++categoria) {
        inventario.definirCapacidadeDaCategoria(static_cast<CategoriaDoItem>(categoria), capacidadeSomada[categoria]);
    }
}
