#pragma once

// Os itens lidos do itens.csv: Ficha Lógica: Itens.

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "Dados/ProblemasDosDados.h"
#include "Dados/TextosERegras.h"

enum class CategoriaDoItem { MateriaPrima, BemDeConsumo, Arma, ArmamentoDeExercito, Equipamento, Artefato };

struct DefinicaoDeItem {
    std::string identificadorDeTexto;   // item.pao
    int numeroInterno = 0;              // a posição no arquivo, contando do 0
    CategoriaDoItem categoriaDoItem = CategoriaDoItem::MateriaPrima;
    std::string tipoDoItem;             // alimento, bruto_vegetal... (conferido pela lista da categoria)
    std::optional<int> tier;            // 0 a 4; vazio em armas, equipamentos e artefatos
    std::optional<int> rankMinimo;
    std::optional<int> rankMaximo;
    int pesoEmUnidades = 1;             // o Pão vale 1 (FL3-4)
    int multiplicadorDoPrecoEmPorcento = 100;
    int precoBaseEmDecimos = 0;         // calculado pelas receitas; 0 = ainda sem preço
};

class CatalogoDeItens {
public:
    const std::vector<DefinicaoDeItem>& todosOsItens() const { return itens; }
    std::vector<DefinicaoDeItem>& todosOsItensParaMudar() { return itens; }
    int quantidadeDeItens() const { return static_cast<int>(itens.size()); }

    // Regra 4.3: o número interno pelo ID, ou vazio se o item não existe.
    std::optional<int> procurarNumeroDoItem(const std::string& identificadorDeTexto) const;
    const DefinicaoDeItem& itemPeloNumero(int numeroInterno) const { return itens[numeroInterno]; }

    // Usado só pela leitura do arquivo.
    void acrescentarItem(DefinicaoDeItem item);

private:
    std::vector<DefinicaoDeItem> itens;
    std::map<std::string, int> numeroPorIdentificador;
};

// Regras 4.1 e 4.2 da ficha: lê o itens.csv e confere os nomes.
CatalogoDeItens lerArquivoDeItens(const std::string& textoDoArquivo, const TabelaDeTextos& tabelaDeTextos,
                                  ListaDeProblemasDosDados& listaDeProblemas);

// A lista das construções. No marco 3, só os IDs (FL3-5).
class CatalogoDeConstrucoes {
public:
    bool existeConstrucao(const std::string& identificadorDeTexto) const;
    int quantidadeDeConstrucoes() const { return static_cast<int>(identificadores.size()); }
    void acrescentarConstrucao(const std::string& identificadorDeTexto) { identificadores.push_back(identificadorDeTexto); }

private:
    std::vector<std::string> identificadores;
};

// Lê o construcoes.json: uma lista [{"id": "construcao.fazenda"}, ...].
CatalogoDeConstrucoes lerArquivoDeConstrucoes(const std::string& textoDoArquivo, const TabelaDeTextos& tabelaDeTextos,
                                              ListaDeProblemasDosDados& listaDeProblemas);
