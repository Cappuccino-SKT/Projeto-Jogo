#pragma once

// As facções (Ficha Lógica: Facção, fase C1): as 6 facções com as cidades delas, as leis, o
// Banco da Facção (cunhagem, tarifa e a verba às cidades), o líder e a assembleia anual. O
// território vem das cidades vizinhas (MundoDoJogo::estadoDoTerritorio, FA1-6).

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

class SimulacaoDaVila;
struct DadosDoJogo;
struct CaravanaDaCidade;
class ListaDeProblemasDosDados;

// Uma lei do faccoes.json, com as opções em fila (Cultura, CT1-1).
struct DefinicaoDaLei {
    std::string id;                      // "imposto"
    std::vector<std::string> opcoes;     // "baixo", "normal", "alto"
    std::vector<int> valoresEmPorcento;  // só no imposto e na tarifa
    std::vector<int> felicidade;         // só no imposto: 0, −5, −10
};

// O líder fixo do Ano 412 (rodada HI7-1; os nomes e os traços vêm do Mundo e História, parte 4.7).
struct LiderInicialDaFaccao {
    std::string nome;
    int idadeNaHistoria = 48;  // em anos da história; o jogo divide por "anos_da_historia_por_ano_do_jogo"
    // A Personalidade de 5 traços (0 a 100). Nesta versão o jogo usa só a Agressividade e a Sociabilidade;
    // a Coragem, a Ganância e a Lealdade ficam para os Companheiros (D2).
    int coragem = 50;
    int ganancia = 50;
    int agressividade = 50;
    int sociabilidade = 50;
    int lealdade = 50;
};

struct DefinicaoDaFaccao {
    std::string id;       // "faccao.guerreira"
    std::string cultura;  // "guerreira"; também o "faccao" das cidades no cidades_iniciais.json
    int colunaDaCapital = 1;
    int linhaDaCapital = 1;
    std::vector<int> leisIniciais;           // a posição da opção, uma por lei
    std::vector<std::string> itensProibidos;  // o tipo ou o id do item: "narcotico"
    std::map<std::string, int> ajusteDePreco;  // o tipo ou o id do item: "alimento": −20
    LiderInicialDaFaccao lider;
};

// O que a cultura deseja de uma lei (o data/culturas.json; a Cultura, C2, completa o arquivo).
struct LeiDesejada {
    int valor = -1;  // a posição da opção; −1 = a cultura não liga para esta lei
    int peso = 0;    // de 1 a 5
};

struct CatalogoDasFaccoes {
    std::vector<DefinicaoDaLei> leis;
    std::vector<DefinicaoDaFaccao> faccoes;
    std::map<std::string, std::vector<LeiDesejada>> leisDesejadasPorCultura;  // uma por lei
    int cunhagemPorCasaDaMoedaPorDiaEmDecimos = 1400;
    int verbaPorNpcPorEstacaoEmDecimos = 50;
    int bancoInicialEmPorcentoDoPib = 25;
    int legitimidadeInicial = 70;
    int anosDaHistoriaPorAnoDoJogo = 4;  // no jogo, o adulto vai de 4 a 20 anos: 1 ano do jogo = 4 da história
    int vetoCustaLegitimidade = 5;
    int leisPorAssembleia = 3;
    int felicidadeDeQuemPerdeuAVotacao = -5;

    int posicaoDaLei(const std::string& id) const;
};

CatalogoDasFaccoes lerArquivosDasFaccoes(const std::string& textoDasFaccoes, const std::string& textoDasCulturas, ListaDeProblemasDosDados& listaDeProblemas);

struct LiderDaFaccao {
    int idade = 12;
    int agressividade = 50;
    int sociabilidade = 50;
    int legitimidade = 70;

    bool operator==(const LiderDaFaccao&) const = default;
};

// Uma lei que a assembleia mudou e que vale no dia seguinte (4.5, passo 6).
struct MudancaDeLei {
    int faccao = 0;
    int lei = 0;
    int valor = 0;

    bool operator==(const MudancaDeLei&) const = default;
};

// O estado (no save).
struct EstadoDasFaccoes {
    std::vector<std::vector<int>> leisAtuais;  // [facção][lei]: a posição da opção
    std::vector<int64_t> bancoDaFaccao;
    std::vector<LiderDaFaccao> lideres;
    std::vector<MudancaDeLei> mudancasParaAmanha;
    std::vector<int64_t> perdeuAVotacaoAte;  // [número do mapa − 1]: o último dia do −5 de Felicidade; −1 = nunca
    int64_t cunhagemTotal = 0;               // o dinheiro novo do mundo (conta como "de fora")

    bool operator==(const EstadoDasFaccoes&) const = default;
};

// A posição da facção da cidade do mapa; −1 = sem facção (Vila Livre, a vila do jogador).
int faccaoDaCidade(const DadosDoJogo& dados, int numeroDoMapa);
// A posição da facção dona do mapa (a cidade ou o território de uma facção só); −1 = nenhuma.
int faccaoDoMapa(const DadosDoJogo& dados, int numeroDoMapa);
// O nome na tela: "Guerreira".
std::string nomeDaFaccao(const DadosDoJogo& dados, int faccao);

// Ao criar o mundo: as leis iniciais, o Banco (25% do PIB) e o líder (sequência 7).
void montarAsFaccoesDaPartidaNova(SimulacaoDaVila& vila, uint64_t sementeDaPartida);

// Regra 4.2: a taxa do imposto da cidade (a lei da facção; 20% sem facção) e a Felicidade dela.
int taxaDoImposto(const SimulacaoDaVila& vila, int numeroDoMapa);
int felicidadeDaLeiDoImposto(const SimulacaoDaVila& vila, int numeroDoMapa);
// A tarifa da facção, em %.
int taxaDaTarifa(const SimulacaoDaVila& vila, int faccao);

// O imposto, a tarifa, a Felicidade das leis e os itens proibidos de cada cidade, pelas leis de agora.
void atualizarAsCidadesPelasLeis(SimulacaoDaVila& vila);

// No dia novo, antes do dia das outras cidades: as leis de ontem passam a valer, as cidades
// recebem o imposto, a tarifa e a Felicidade das leis, a cunhagem (4.4), a verba no dia 1 da
// estação e a assembleia no dia 1 do ano. Devolve as linhas do console.
std::vector<std::string> rodarAsFaccoesNoDiaNovo(SimulacaoDaVila& vila);

// Regra 4.3: a caravana entrou em "mapaNovo" vindo de "mapaAnterior": paga a tarifa da facção
// dona, se não for a dela. Devolve o valor pago, em décimos.
int64_t cobrarTarifa(SimulacaoDaVila& vila, SimulacaoDaVila& casa, CaravanaDaCidade& caravana, int mapaAnterior, int mapaNovo);
// A mesma conta só com os números: 80 Tábuas a 0,8 com 20% = 12,8 moedas.
int64_t tarifaComOsNumeros(int64_t valorDaCargaEmDecimos, int taxaEmPorcento);

// A lei Itens proibidos e o ajuste de preço, para o item na cidade da facção.
bool itemEhProibidoNaFaccao(const DadosDoJogo& dados, int faccao, int numeroDoItem);
int ajusteDePrecoDaFaccaoEmPorcento(const DadosDoJogo& dados, int faccao, int numeroDoItem);

// Regra 4.4, passo 3: a verba de cada cidade, na mesma proporção se o Banco não tem o total.
std::vector<int64_t> dividirAVerba(int64_t bancoDaFaccao, const std::vector<int64_t>& verbaDeCadaCidade);

// Regra 4.5: quem vota na assembleia de uma facção.
struct VotanteDaAssembleia {
    std::string cultura;
    int felicidade = 50;
    int npcs = 1;
    int nivel = 1;
    bool ehACapital = false;
    int numeroDoMapa = 0;
};
struct ResultadoDaAssembleia {
    std::vector<int> pauta;                 // as leis votadas
    std::vector<int> novoValor;             // o valor proposto de cada uma
    std::vector<bool> passou;               // passou na votação
    std::vector<bool> vetada;               // passou, mas o líder vetou
    std::vector<std::vector<int>> perderam;  // os mapas das cidades que votaram e perderam
};
// "sortearDe0A99" é o sorteio do veto (a sequência 7 no jogo).
ResultadoDaAssembleia fazerAssembleia(const CatalogoDasFaccoes& catalogo, const std::vector<int>& leisAtuais, const std::vector<VotanteDaAssembleia>& votantes,
                                      const std::string& culturaDoLider, LiderDaFaccao& lider, const std::function<int()>& sortearDe0A99);
// Regra 4.5, passo 1: |posição atual − posição desejada| × peso (0 se a cultura não liga).
int distanciaDaLei(const CatalogoDasFaccoes& catalogo, int lei, int valorAtual, const std::string& cultura);
