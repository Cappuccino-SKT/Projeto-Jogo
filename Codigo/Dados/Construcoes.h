#pragma once

// Os tipos de construção lidos do construcoes.json (Ficha Lógica: Construção,
// parte 2). No marco 3 era só a lista dos IDs (FL3-5); no marco 6, cada tipo
// ganha o porte, o que guarda, a Função das vagas e o Trabalho.

#include <optional>
#include <string>
#include <vector>

#include "Dados/Itens.h"
#include "Dados/ProblemasDosDados.h"
#include "Dados/TextosERegras.h"
#include "Mapa/MapaLocal.h"

// O Trabalho de uma receita (IP3) ou de quem trabalha numa construção sem
// receita (o Lojista é Manufaturado; o Operário, Braçal). Decide o salário
// por hora e a habilidade usada.
enum class TrabalhoDaReceita { Bracal, Manufaturado, Especialista };

// "bracal", "manufaturado" ou "especialista"; vazio para outro texto.
std::optional<TrabalhoDaReceita> lerTrabalhoDoTexto(const std::string& texto);

struct DefinicaoDeConstrucao {
    std::string identificadorDeTexto;                // construcao.cozinheiro
    PorteDoLote porte = PorteDoLote::Pequeno;        // o mesmo tamanho dos lotes do mapa
    std::vector<CategoriaDoItem> categoriasQueGuarda;  // somam à capacidade do inventário da cidade
    std::string funcao;                              // o nome da Função das vagas: "cozinheiro"
    std::optional<TrabalhoDaReceita> trabalhoSemReceita;  // só nas construções sem receita
    int moradores = 0;                               // só nas moradias: quantos moram em cada uma

    bool ehMoradia() const { return moradores > 0; }
};

class CatalogoDeConstrucoes {
public:
    bool existeConstrucao(const std::string& identificadorDeTexto) const { return procurarConstrucao(identificadorDeTexto) != nullptr; }
    // A definição pelo ID, ou nullptr se o tipo não existe.
    const DefinicaoDeConstrucao* procurarConstrucao(const std::string& identificadorDeTexto) const;
    const std::vector<DefinicaoDeConstrucao>& todasAsConstrucoes() const { return definicoes; }
    int quantidadeDeConstrucoes() const { return static_cast<int>(definicoes.size()); }
    void acrescentarConstrucao(const DefinicaoDeConstrucao& definicao) { definicoes.push_back(definicao); }

private:
    std::vector<DefinicaoDeConstrucao> definicoes;
};

// Lê o construcoes.json e confere os campos (regra 4.1, passo 1).
CatalogoDeConstrucoes lerArquivoDeConstrucoes(const std::string& textoDoArquivo, const TabelaDeTextos& tabelaDeTextos,
                                              ListaDeProblemasDosDados& listaDeProblemas);
