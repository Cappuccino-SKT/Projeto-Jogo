#pragma once

// O grupo do jogador (Ficha Lógica: Companheiros e Diálogo, regras 4.1 a 4.4, 4.8 e 4.9; marco D2,
// parte 2): recrutar na Taverna, o salário por estação, a Opinião e a lealdade, a Felicidade do
// Controlado, o casamento e os filhos do jogador, o sucessor e as ações de diálogo com teste.
// As regras ficam no data/companheiros.json; as dinastias das facções estão em Faccao/Dinastia.h.

#include <cstdint>
#include <string>
#include <vector>

#include "Faccao/Dinastia.h"

class ListaDeProblemasDosDados;
class SimulacaoDaVila;

struct QualidadeDoCompanheiro {
    std::string id;  // "comum", "bom", "excelente"
    int renomeMinimo = 0;
    int habilidadeMinima = 20;
    int habilidadeMaxima = 50;
    int entradaEmMoedas = 100;
    int salarioPorEstacaoEmMoedas = 50;
};

// Os outros blocos do companheiros.json (o "familia" e o "dinastia" são da RegrasDaFamilia).
struct RegrasDosCompanheiros {
    int grupoBase = 4;
    int grupoRenomePorUm = 20;
    int vinculadosBase = 10;
    int vinculadosRenomePorUm = 5;
    std::vector<int> recrutaveisPorNivelDaCidade = {0, 1, 2, 4};
    std::vector<QualidadeDoCompanheiro> qualidades;  // da pior para a melhor
    int estacoesSemSalarioParaSair = 2;
    int opiniaoInicial = 20;
    // A tabela "opiniao".
    int opiniaoVitoriaJunto = 2;
    int opiniaoSalarioEmDia = 1;
    int opiniaoSalarioAtrasado = -10;
    int opiniaoCompanheiroMorto = -5;
    int opiniaoPresentePorMoedas = 20;  // +1 a cada 20 moedas de valor
    int opiniaoPresenteMaximo = 10;
    int opiniaoAumentoRecusado = -10;
    int aumentoPedidoEmPorcento = 25;
    // A tabela "felicidade_do_controlado".
    int felicidadeSalarioEmDia = 10;
    int felicidadeSalarioAtrasado = -20;
    int felicidadeSemComida = -25;
    int felicidadePorCompanheiroMorto = -10;
    int felicidadeMinimoDosMortos = -30;
    // O bloco "dialogo".
    int dialogoBase = 50;
    int dialogoHabilidadeDivide = 2;
    int dialogoOpiniaoDivide = 4;
    int dialogoTracoDivide = 3;
    int dialogoMinimo = 5;
    int dialogoMaximo = 95;
    int falhaPersuadir = -5;
    int falhaIntimidar = -10;
    int habilidadeDoFilhoEducado = 50;  // [Proposta] aos 5 anos, a habilidade do foco nasce no topo da faixa comum
};

RegrasDosCompanheiros lerRegrasDosCompanheiros(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

enum class FocoDoCompanheiro { Combate = 0, Comercio = 1, Oficio = 2, Magia = 3, Governo = 4 };
std::string nomeDoFoco(FocoDoCompanheiro foco);

// Um Controlado: o recrutável da Taverna, o companheiro vinculado, o cônjuge ou o filho adulto.
struct Companheiro {
    int numero = 0;
    std::string nome;
    std::string cultura;
    bool ehMulher = false;
    int idade = 8;
    FocoDoCompanheiro foco = FocoDoCompanheiro::Combate;
    int qualidade = 0;  // a posição em RegrasDosCompanheiros::qualidades
    Personalidade personalidade;
    int carisma = 50;
    int corpoACorpo = 30;
    int especialista = 30;
    int salarioPorEstacaoEmDecimos = 500;
    int mapaDeOrigem = 0;  // o salário vai para a classe Civil desta cidade (CP1-3)
    int mapaOndeEsta = 0;  // fora do grupo, onde ele espera
    bool noGrupo = false;
    bool ehDaFamilia = false;  // o cônjuge e os filhos: sem salário e fora do limite
    int opiniao = 20;          // de −100 a +100
    int felicidade = 50;       // de 0 a 100
    int estacoesSemSalario = 0;
    bool salarioAtrasado = false;  // na última estação (o alvo da Felicidade)
    int diasComFelicidadeZero = 0;  // a Opinião +60 segura mais 1 estação
    bool ehFilhoDoJogador = false;
    bool pediuAumento = false;
    int estacoesDeGravidez = 0;  // só o cônjuge

    bool operator==(const Companheiro&) const = default;
};

struct FilhoDoJogador {
    int numero = 0;
    std::string nome;
    bool ehMulher = false;
    int idade = 0;
    FocoDoCompanheiro focoDeEducacao = FocoDoCompanheiro::Combate;

    bool operator==(const FilhoDoJogador&) const = default;
};

// O estado (no save; só na vila).
struct EstadoDosCompanheiros {
    std::vector<Companheiro> vinculados;  // na ordem do recrutamento (o salário paga nesta ordem)
    std::vector<std::vector<Companheiro>> recrutaveisPorMapa;  // [número do mapa − 1]
    std::vector<FilhoDoJogador> filhos;  // as crianças; o adulto vira um vinculado da família
    int numeroDoConjuge = -1;
    int sucessorEscolhido = -1;  // o número de um vinculado; −1 = nenhum
    int proximoNumero = 1;
    int mortosNaEstacao = 0;

    const Companheiro* procurar(int numero) const;
    Companheiro* procurar(int numero);

    bool operator==(const EstadoDosCompanheiros&) const = default;
};

// Regra 4.2: o salário de cada vinculado, na ordem do recrutamento (o último fica por último). Sem
// o dinheiro, conta uma estação sem salário; com 2, ele sai.
void pagarVinculados(SimulacaoDaVila& vila, std::vector<std::string>& linhas);

// Regra 4.1, passo 4: os limites com o Renome (0 até o Objetivo e Progressão, CP1-2).
int limiteDoGrupo(const RegrasDosCompanheiros& regras, int renome);
int limiteDosVinculados(const RegrasDosCompanheiros& regras, int renome);
// Quantos vinculados contam no limite (a família não conta).
int vinculadosQueContam(const EstadoDosCompanheiros& estado);

// Regra 4.3, passo 1: a mudança na Opinião; as quedas × (150 − Lealdade) ÷ 100. Entre −100 e +100.
void mudarOpiniao(Companheiro& companheiro, int valor);
// Regra 4.4: o alvo da Felicidade (50 + salário + comida + mortos).
int felicidadeAlvoDoControlado(const Companheiro& companheiro, bool grupoSemComida, int mortosNaEstacao, const RegrasDosCompanheiros& regras);
// O passo de 10% ao dia (ao menos 1); com a Opinião de −19 a +19, a queda é 25% mais rápida.
int passoDaFelicidade(int felicidade, int alvo, int opiniao);

// Regra 4.9: a chance de uma ação com teste, em %. 55 no exemplo do documento.
enum class AcaoDeDialogo { Persuadir, Barganhar, Intimidar, Subornar };
int chanceDaAcaoDeDialogo(AcaoDeDialogo acao, int habilidadeDoJogador, int opiniao, const Personalidade& personalidade, const RegrasDosCompanheiros& regras);
// O presente: +1 a cada 20 moedas × (100 + Ganância − 50) ÷ 100, até +10.
int opiniaoDoPresente(int moedas, int ganancia, const RegrasDosCompanheiros& regras);

// Regra 4.8, passo 3: o filho adulto mais velho; sem filho, o escolhido com Opinião +60, ou o de
// maior Opinião. −1 = ninguém (fim da partida, com o epílogo).
int escolherSucessorDoJogador(const EstadoDosCompanheiros& estado);

// Na partida nova: os recrutáveis da primeira estação.
void montarOsCompanheirosDaPartidaNova(SimulacaoDaVila& vila);
// Na vila: o dia novo. No dia 1 da estação, a idade (na Primavera), a velhice, os recrutáveis, o
// salário, os pedidos de aumento e os filhos; todo dia, a Felicidade, quem sai e quem deserta.
std::vector<std::string> rodarOsCompanheirosNoDiaNovo(SimulacaoDaVila& vila);
// Monta os lutadores do grupo e o número de pessoas pelos companheiros que estão nele.
void atualizarOGrupoPelosCompanheiros(SimulacaoDaVila& vila);
// O grupo venceu uma luta: a Opinião de quem estava nele sobe.
void anotarAVitoriaDoGrupo(SimulacaoDaVila& vila);

// As ações do jogador (o pedido PedidoDoCompanheiro). Devolvem "" ou o motivo da recusa.
enum class AcaoComOCompanheiro { Recrutar, ChamarParaOGrupo, DeixarNaCidade, Dispensar, Presentear, PedirEmCasamento, AceitarOAumento, RecusarOAumento,
                                 EscolherOSucessor, EscolherOFocoDoFilho };
std::string fazerAcaoComOCompanheiro(SimulacaoDaVila& vila, AcaoComOCompanheiro acao, int numero, int valor, std::vector<std::string>& linhas);
