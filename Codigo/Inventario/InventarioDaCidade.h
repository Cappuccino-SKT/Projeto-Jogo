#pragma once

// O inventário da cidade (Ficha Lógica: Inventário): quanto há de cada item,
// de quem é (da Cidade ou do jogador) e quanto cabe em cada categoria. Um
// inventário só para a vila inteira (V1-5): nada é levado de uma construção
// para outra.

#include <array>
#include <vector>

#include "Construcao/RegrasDasConstrucoes.h"
#include "Dados/Construcoes.h"
#include "Dados/Itens.h"
#include "Dados/Receitas.h"
#include "Inventario/DonoDoItem.h"
#include "Vila/VilaInicial.h"

class InventarioDaCidade {
public:
    InventarioDaCidade() = default;
    // Um inventário vazio, com uma posição por item do catálogo, e capacidade 0.
    explicit InventarioDaCidade(const CatalogoDeItens& catalogoDeItens);

    int quantidadeDe(int numeroDoItem, DonoDoItem dono) const;
    // Os dois donos juntos.
    int quantidadeTotalDe(int numeroDoItem) const { return quantidadeDe(numeroDoItem, DonoDoItem::Cidade) + quantidadeDe(numeroDoItem, DonoDoItem::Jogador); }

    // Regra 4.1: põe, se couber na categoria (os dois donos juntos). Devolve se deu certo.
    bool tentarPor(int numeroDoItem, DonoDoItem dono, int quantidade);
    // Regra 4.2: tira, se o dono tiver tudo. Se não tiver, não tira nada.
    bool tentarTirar(int numeroDoItem, DonoDoItem dono, int quantidade);
    // Tira todos ou nenhum (os insumos de um ciclo).
    bool tentarTirarVarios(const std::vector<InsumoDaReceita>& itensParaTirar, DonoDoItem dono);
    // Passa itens de um dono para o outro (a compra de insumos do jogador). A
    // categoria não muda, então sempre cabe. Devolve false se faltar.
    bool passarParaOutroDono(int numeroDoItem, DonoDoItem doDono, DonoDoItem paraODono, int quantidade);

    int quantidadeNaCategoria(CategoriaDoItem categoria) const;
    int capacidadeDaCategoria(CategoriaDoItem categoria) const { return capacidadePorCategoria[static_cast<int>(categoria)]; }
    int espacoLivreNaCategoria(CategoriaDoItem categoria) const { return capacidadeDaCategoria(categoria) - quantidadeNaCategoria(categoria); }
    void definirCapacidadeDaCategoria(CategoriaDoItem categoria, int capacidade) { capacidadePorCategoria[static_cast<int>(categoria)] = capacidade; }

private:
    std::vector<int> quantidadePorItemDaCidade;
    std::vector<int> quantidadePorItemDoJogador;
    std::vector<CategoriaDoItem> categoriaDeCadaItem;  // copiada do catálogo, para a conta da capacidade
    std::array<int, 6> capacidadePorCategoria = {};    // uma por CategoriaDoItem

    std::vector<int>& listaDoDono(DonoDoItem dono) { return dono == DonoDoItem::Cidade ? quantidadePorItemDaCidade : quantidadePorItemDoJogador; }
    bool numeroDoItemExiste(int numeroDoItem) const { return numeroDoItem >= 0 && numeroDoItem < static_cast<int>(categoriaDeCadaItem.size()); }
};

// Regra 4.3: para cada construção da vila e cada categoria do "guarda" dela,
// capacidade += capacidade do porte × nível.
void recalcularCapacidade(InventarioDaCidade& inventario, const VilaDoJogo& vila, const CatalogoDeConstrucoes& catalogoDeConstrucoes,
                          const RegrasDasConstrucoes& regrasDasConstrucoes);
