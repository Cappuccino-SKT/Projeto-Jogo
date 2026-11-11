#pragma once

// A Escravidão (Ficha Lógica: Escravidão, marco D3, parte 1): a classe Escravo nas cidades
// cuja lei permite. O escravo trabalha sem salário, come do estoque da Cidade, tem uma
// Felicidade própria (base 30) e uma Obediência, que decide o quanto ele trabalha e se foge.
// A rebelião, a alforria e a abolição tiram gente da classe.
//
// Parte 1 (este arquivo): os escravos da Cidade, em todas as cidades do mundo.
// Parte 2 (depois): o Mercado de Escravos, os escravos do jogador e o jogador capturado.

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <entt/entity/fwd.hpp>

class ListaDeProblemasDosDados;
class SimulacaoDaVila;
struct ContextoDaProducao;
struct DadosDoJogo;

// Os números do data/escravidao.json.
struct RegrasDaEscravidao {
    std::map<std::string, int> proporcaoInicialEmPorcento;  // por cultura da cidade: "autoritaria": 30
    // O preço (regra 4.2; a parte 2 usa).
    int precoBase = 200;
    int precoPorPontoDaMelhorHabilidade = 4;
    int precoDoMagoEmPorcento = 200;
    int precoDoVelhoEmPorcento = 50;
    int precoDaCulturaHostilEmPorcento = 80;
    int felicidadeBaseDoEscravo = 30;
    // A Obediência (regra 4.3).
    int obedienciaBase = 50;
    int medoMaximo = 30;
    int obedienciaDosGrilhoes = 15;
    int obedienciaPorEstacaoDeCativeiro = 1;
    int cativeiroMaximo = 10;
    int obedienciaCaiComFelicidadeAbaixoDe = 30;
    std::map<std::string, int> obedienciaPorCultura;  // "guerreira": −10
    // A fuga (regra 4.4).
    int fugaComObedienciaAbaixoDe = 30;
    int fugaDetectadaEmPorcento = 50;
    int castigoDaFugaNaFelicidade = -10;
    int castigoDaFugaEmDias = 8;
    int fugitivoViraBandidoEmPorcento = 50;
    int criminalidadePorBandido = 1;
    // A rebelião (regra 4.5).
    int rebeliaoComFelicidadeAbaixoDe = 10;
    int rebeliaoComControleAbaixoDe = 10;
    int rebeliaoDiasSeguidos = 4;
    int perdedoresExecutadosOuVendidosEmPorcento = 20;
    int guardasPorNivelDaCidade = 2;
    int felicidadeDosCivisSeOsEscravosVencem = -5;
    int felicidadeDosEscravosSePerdem = -10;
    // A alforria (regra 4.6): por estação, em %, pela lei de Tolerância ou pela cultura da cidade.
    std::map<std::string, int> alforriaPorEstacaoEmPorcento;  // "tolerancia": 1, "escravista": 0
};

RegrasDaEscravidao lerArquivoDaEscravidao(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

// O que cada NPC escravo guarda (um componente do NPC; vai no save).
struct CativeiroDoNpc {
    int64_t diaDoInicio = 0;
    bool grilhoes = false;
    int obedienciaEmDecimos = 500;  // recalculada todo dia
    int64_t castigadoAte = -1;      // o último dia do castigo de uma fuga vista; −1 = sem castigo

    bool operator==(const CativeiroDoNpc&) const = default;
};

// O que cada cidade guarda (vai no save).
struct EstadoDaEscravidaoDaCidade {
    int felicidadeDosEscravosEmDecimos = 300;
    int diasSeguidosParaRebeliao = 0;
    int alforriaAcumuladaEmCentesimos = 0;     // a fração de escravo que ainda não virou uma alforria
    int nascimentosAcumuladosEmMilesimos = 0;  // a fração dos nascidos que nasce escrava
    int64_t civisAbaladosAte = -1;             // depois de uma rebelião que venceu: −5 nos Civis
    int64_t escravosCastigadosAte = -1;        // depois de uma rebelião que perdeu: −10 nos Escravos
    int pontosNaFelicidadeDosCivisHoje = 0;    // o −5 de cima, já pronto para a Felicidade de hoje
    int escravosSemComidaHoje = 0;
    // As contas da partida (para o relatório e os testes).
    int fugasNoTotal = 0;
    int alforriasNoTotal = 0;
    int rebelioesNoTotal = 0;

    bool operator==(const EstadoDaEscravidaoDaCidade&) const = default;
};

// ---------------------------------------------------------------------------
// As contas (parte 5 da ficha). Felicidade, Obediência e Controle em décimos (767 = 76,7).
// ---------------------------------------------------------------------------

// Regra 4.2: 200 + 4 × a melhor habilidade; × 2 se Mago; × 0,5 se velho; × 0,8 se a cultura é hostil. Em moedas.
int precoBaseDoEscravo(const RegrasDaEscravidao& regras, int melhorHabilidade, bool ehMago, bool ehVelho, bool culturaHostilADoComprador);

// Regra 4.3: 50 + Medo + Grilhões + as estações de cativeiro − a Felicidade que falta para 30 + a cultura.
// O Medo é o Controle da cidade × 30 ÷ 100. Fica entre 0 e 100,0.
int calcularObedienciaEmDecimos(const RegrasDaEscravidao& regras, int controleEmDecimos, int felicidadeEmDecimos, bool grilhoes, int estacoesDeCativeiro,
                                const std::string& cultura);

// Regra 4.4, passo 1: (30 − Obediência)% por noite; com Grilhões, nunca. Em por mil.
int chanceDeTentarFugaEmPorMil(const RegrasDaEscravidao& regras, int obedienciaEmDecimos, bool grilhoes);

// O alvo da Felicidade dos Escravos: 30 + consumo (−25 × a parte sem comida) + moradia (−10 × a
// parte sem casa) + os eventos da cidade + o castigo depois de uma rebelião perdida. Em décimos.
int calcularAlvoDaFelicidadeDosEscravos(const RegrasDaEscravidao& regras, int escravos, int semComida, int semCasa, int pontosDosEventos, int pontosDoCastigo,
                                        int consumoSemComida, int moradiaSemCasa);

// Regra 4.6: quantos escravos a alforria liberta nesta estação, guardando a fração que sobra.
int alforriadosNaEstacao(int escravos, int porcento, int& acumuladoEmCentesimos);

// ---------------------------------------------------------------------------
// No mundo.
// ---------------------------------------------------------------------------

// A opção de uma lei na cidade do mapa ("ampla", "tolerancia"): a da facção dela; na cidade sem
// facção, a que a cultura da cidade deseja (rodada LE1-4). Vazio se a lei não existe ou ninguém a deseja.
std::string opcaoDaLeiNaCidade(const SimulacaoDaVila& vila, int numeroDoMapa, const std::string& lei);
// A lei de Escravidão da cidade é qualquer uma menos "proibida".
bool escravidaoPermitidaNaCidade(const SimulacaoDaVila& vila, int numeroDoMapa);

int contarOsEscravos(const SimulacaoDaVila& cidade);

// O NPC vira Escravo da Cidade (sai da conta dos Civis) ou volta a ser Civil.
void tornarEscravo(ContextoDaProducao& contexto, entt::entity npc, int64_t dia);
void libertarEscravo(ContextoDaProducao& contexto, entt::entity npc);

// Regra 4.1 (rodada LE1-1): no primeiro dia, a parte dos NPCs de cada cidade que começa escrava,
// pela cultura da cidade (os de Braçal mais alto, primeiro os dos empregos Braçais).
void montarAEscravidaoDaPartidaNova(SimulacaoDaVila& vila);

// Todo dia novo, em cada cidade: a comida, a Felicidade, a Obediência, a rebelião e a fuga; no
// dia 1 da estação, a abolição (se a lei mudou para "proibida") e a alforria.
std::vector<std::string> rodarAEscravidaoNoDiaNovo(SimulacaoDaVila& vila);

// População: os filhos de escravos nascem escravos (a parte dos nascidos igual à parte de escravos da cidade).
void anotarONascimentoNaEscravidao(ContextoDaProducao& contexto, entt::entity npc);
