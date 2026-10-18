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

    // A cesta mínima (Classe Social, IP2-11): o alimento é comida; a mobília
    // e o vestuário são Materiais Básicos.
    bool ehComida() const { return tipoDoItem == "alimento"; }
    bool ehMaterialBasico() const { return tipoDoItem == "mobilia" || tipoDoItem == "vestuario"; }
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

// "materia_prima" -> MateriaPrima, e assim por diante; vazio para outro texto.
// Usado também pelo "guarda" do construcoes.json.
std::optional<CategoriaDoItem> lerCategoriaDoTexto(const std::string& texto);
