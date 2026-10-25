#pragma once

// As culturas (Ficha Lógica: Cultura, fase C2): as 6 culturas em dados, a cultura de cada NPC
// e da cidade, o 5º fator da Felicidade (Leis × cultura, com as minorias hostis), os pontos de
// habilidade e as afinidades. O desejo das leis está no mesmo culturas.json e é lido pela Facção.

#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct DadosDoJogo;
struct CatalogoDasFaccoes;
class SimulacaoDaVila;
class ListaDeProblemasDosDados;

enum class AfinidadeEntreCulturas { Mesma, Afim, Neutra, Hostil };

struct DefinicaoDaCultura {
    std::string id;     // "tecnocrata" (no arquivo, "cultura.tecnocrata")
    int regiao = 0;     // a região de 1 a 6 onde ela é a cultura do lugar
    int leiDoExtremo = -1;    // a posição da lei no catálogo das facções; −1 = sem extremo nesta versão
    int valorDoExtremo = -1;  // a posição da opção
    std::map<std::string, AfinidadeEntreCulturas> afinidades;
    std::map<std::string, int> habilidadesEmPontos;  // "bracal": −5
    std::string ritoDeAdmissao;
    std::string sucessao;
};

struct CatalogoDasCulturas {
    std::vector<DefinicaoDaCultura> culturas;

    const DefinicaoDaCultura* procurar(const std::string& id) const;
    AfinidadeEntreCulturas afinidade(const std::string& uma, const std::string& outra) const;
};

CatalogoDasCulturas lerArquivoDasCulturas(const std::string& textoDoArquivo, const CatalogoDasFaccoes& faccoes, ListaDeProblemasDosDados& listaDeProblemas);

// Regra 4.1 e CT1-3: a cultura da facção da cidade, ou a da região (a cidade sem facção).
std::string culturaBaseDaCidade(const DadosDoJogo& dados, int numeroDoMapa);
// A cultura da região de um mapa ("" se nenhuma cultura é dessa região).
std::string culturaDaRegiao(const DadosDoJogo& dados, int regiao);
// As regiões que tocam a região na malha (sem ela), em ordem.
std::vector<int> regioesVizinhas(const DadosDoJogo& dados, int regiao);
// Regra 4.1, passo 3 (CT1-6): 75% a cultura da região, 25% a de uma região vizinha sorteada.
// O sorteio usa a semente, o mapa e o número do NPC (nada a guardar no save).
std::string escolherCulturaDoMigrante(const DadosDoJogo& dados, int numeroDoMapa, uint64_t sementeDaPartida, int numeroDoNpc);
// Regra 4.4: a habilidade com os pontos da cultura, presa entre 0 e 100.
int habilidadeComACultura(const DadosDoJogo& dados, const std::string& cultura, const std::string& trabalho, int habilidade);

// Regra 4.2, passos 1 a 3: o fator de uma cultura contra as leis, em pontos, de −30 a +10.
int fatorDaCulturaContraAsLeis(const DadosDoJogo& dados, const std::vector<int>& leis, const std::string& cultura);
// Regras 4.2, passo 4, e 4.3: o fator da classe, em décimos (a média pesada pelos NPCs de cada
// cultura e a minoria hostil à cultura base), de −30,0 a +10,0.
int fatorLeisECulturaDaClasseEmDecimos(const DadosDoJogo& dados, const std::vector<int>& leis, const std::map<std::string, int>& npcsPorCultura,
                                       const std::string& culturaBase);
// O fator da cidade, pelos NPCs dela (0 sem facção).
int fatorLeisECulturaDaCidadeEmDecimos(const SimulacaoDaVila& vila, const SimulacaoDaVila& cidade);

// Regra 4.5: a cultura com mais NPCs nas cidades da facção; empate, a da facção.
std::string culturaDominante(SimulacaoDaVila& vila, int faccao);
