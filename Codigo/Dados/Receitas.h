#pragma once

// As receitas lidas do receitas.csv e o preço base dos itens:
// Ficha Lógica: Receitas e preço base.

#include <optional>
#include <string>
#include <vector>

#include "Dados/Construcoes.h"
#include "Dados/Itens.h"
#include "Dados/ProblemasDosDados.h"
#include "Dados/TextosERegras.h"

struct InsumoDaReceita {
    int numeroDoItem = 0;
    int quantidade = 1;
};

struct DefinicaoDeReceita {
    std::string identificadorDeTexto;  // receita.pao
    int numeroDoProduto = 0;
    int loteProduzido = 1;
    std::vector<InsumoDaReceita> insumos;  // até 3 tipos (IP3-1)
    std::string construcaoQueProduz;
    int nivelMinimoDaConstrucao = 1;
    TrabalhoDaReceita trabalhoUsado = TrabalhoDaReceita::Manufaturado;
    int habilidadeMinima = 10;
    int horasPorCiclo = 4;
    int eraMinima = 1;
    int numeroDaLinhaNoArquivo = 0;  // para as mensagens de erro e de aviso
};

class CatalogoDeReceitas {
public:
    const std::vector<DefinicaoDeReceita>& todasAsReceitas() const { return receitas; }
    int quantidadeDeReceitas() const { return static_cast<int>(receitas.size()); }
    // A receita que produz o item (uma por produto, IP1-1), ou vazio.
    std::optional<int> procurarReceitaDoProduto(int numeroDoProduto) const;
    void acrescentarReceita(const DefinicaoDeReceita& receita) { receitas.push_back(receita); }

private:
    std::vector<DefinicaoDeReceita> receitas;
};

// Regra 4.1: lê e confere o receitas.csv.
CatalogoDeReceitas lerArquivoDeReceitas(const std::string& textoDoArquivo, const CatalogoDeItens& catalogoDeItens,
                                        const CatalogoDeConstrucoes& catalogoDeConstrucoes, ListaDeProblemasDosDados& listaDeProblemas);

// Regra 4.2: calcula o preço base de todos os itens, em rodadas, e guarda em cada item.
void calcularPrecosBaseDosItens(CatalogoDeItens& catalogoDeItens, const CatalogoDeReceitas& catalogoDeReceitas,
                                const RegrasDoPreco& regrasDoPreco, ListaDeProblemasDosDados& listaDeProblemas);

// A conta da parte 5, para uma receita cujos insumos já têm preço.
int calcularPrecoDeUmaReceitaEmDecimos(const DefinicaoDeReceita& receita, const CatalogoDeItens& catalogoDeItens,
                                       const RegrasDoPreco& regrasDoPreco);

// Divide arredondando a metade para cima: (33, 2) dá 17; (9, 2) dá 5.
long long arredondarMetadeParaCima(long long numerador, long long denominador);

// Ficha da Construção, regra 4.1, passo 2: uma construção sem receitas e sem
// moradores precisa do "trabalho_sem_receita" (o Mercado, o Construtor).
void conferirTrabalhoDasConstrucoes(const CatalogoDeConstrucoes& catalogoDeConstrucoes, const CatalogoDeReceitas& catalogoDeReceitas,
                                    ListaDeProblemasDosDados& listaDeProblemas);

// Regra 4.3: avisos quando a receita foge da escada de níveis da IP3-1.
void conferirEscadaDeNiveis(const CatalogoDeItens& catalogoDeItens, const CatalogoDeReceitas& catalogoDeReceitas,
                            ListaDeProblemasDosDados& listaDeProblemas);
