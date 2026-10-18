#pragma once

// Os recursos do mapa lidos do recursos.csv: Ficha Lógica: Recursos.

#include <array>
#include <optional>
#include <string>
#include <vector>

#include "Dados/Itens.h"
#include "Dados/ProblemasDosDados.h"
#include "Dados/TextosERegras.h"
#include "Mapa/MapaLocal.h"
#include "Tempo/RelogioDoMundo.h"

struct DefinicaoDeRecurso {
    std::string identificadorDeTexto;  // recurso.grao
    int numeroDoItemGerado = 0;
    std::string construcaoQueExtrai;   // construcao.fazenda
    std::string nomeDaAreaNoTiled;     // campo_de_grao
    std::string grupoDaEstacao;        // fazenda
};

class CatalogoDeRecursos {
public:
    const std::vector<DefinicaoDeRecurso>& todosOsRecursos() const { return recursos; }
    int quantidadeDeRecursos() const { return static_cast<int>(recursos.size()); }
    // O número do recurso pelo nome da área no Tiled, ou vazio.
    std::optional<int> procurarRecursoPelaArea(const std::string& nomeDaArea) const;
    void acrescentarRecurso(const DefinicaoDeRecurso& recurso) { recursos.push_back(recurso); }

private:
    std::vector<DefinicaoDeRecurso> recursos;
};

// Regra 4.1: lê o recursos.csv.
CatalogoDeRecursos lerArquivoDeRecursos(const std::string& textoDoArquivo, const CatalogoDeItens& catalogoDeItens,
                                        const CatalogoDeConstrucoes& catalogoDeConstrucoes, const RegrasDoJogo& regras,
                                        ListaDeProblemasDosDados& listaDeProblemas);

// Regra 4.2: liga cada área de recurso do mapa ao recurso e lê a produtividade.
void ligarAreasDoMapaAosRecursos(MapaLocal& mapaLocal, const std::string& nomeDoArquivoDoMapa, const CatalogoDeRecursos& catalogoDeRecursos,
                                 const RegrasDoJogo& regras, ListaDeProblemasDosDados& listaDeProblemas);

// Regra 4.3: produtividade × estação ÷ 100, em %. Exemplo: 100 e Outono (150) dão 150.
double calcularFatorDeProducaoEmPorcento(int produtividadeEmPorcento, const std::array<int, 4>& producaoDoGrupoPorEstacao,
                                         Estacao estacaoAtual);
