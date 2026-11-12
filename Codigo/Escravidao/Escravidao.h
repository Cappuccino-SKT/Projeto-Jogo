#pragma once

// A Escravidão (Ficha Lógica: Escravidão, marco D3, parte 1): a classe Escravo nas cidades
// cuja lei permite. O escravo trabalha sem salário, come do estoque da Cidade, tem uma
// Felicidade própria (base 30) e uma Obediência, que decide o quanto ele trabalha e se foge.
// A rebelião, a alforria e a abolição tiram gente da classe.
//
// Parte 1: os escravos da Cidade, em todas as cidades do mundo.
// Parte 2: o Mercado de Escravos, os escravos do jogador e o jogador capturado.

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
    // Parte 2. O Mercado de Escravos (regra 4.2).
    std::vector<std::vector<int>> outrasCidadesComMercado;  // [coluna, linha]; as capitais dos reinos que permitem já têm
    int mercadoAlvoPorVagaBracalVazia = 2;
    int mercadoAlvoMinimo = 2;
    int vendaPeloDonoEmPorcento = 70;
    int reservaDoBancoParaComprarEmMoedas = 250;
    int cativosDosBandidosPorEstacao = 1;  // ES1-3: entram de graça no estoque que está abaixo do alvo
    // Os tratamentos do escravo do jogador (regra 4.7).
    int grilhoesNaFelicidade = -5;
    int grilhoesNoTrabalhoEmPorcento = 80;
    int castigoPublicoNoMedo = 5;
    int castigoPublicoEmDias = 8;
    int castigoPublicoNaFelicidade = -10;
    // A reputação do jogador (regra 4.6).
    int reputacaoReligiosaAoComprar = -1;
    int reputacaoReligiosaAoVender = -3;
    int reputacaoReligiosaAoLibertar = 2;
    int reputacaoReligiosaNoCastigoPublico = -2;
    int reputacaoDeQuemPermiteAoLibertar = -5;
    // O jogador capturado (regra 4.8).
    int capturaDoJogadorPelosBandidosEmPorcento = 50;
    int habilidadeDoJogadorNoPreco = 50;  // até o jogador ter atributos: 200 + 4 × 50 = 400
    int comprarALiberdadeEmPorcentoDoPreco = 150;
    int peculioMaximoPorDiaEmDecimos = 40;
    int recompensaPelaFugaEmMoedas = 300;
};

RegrasDaEscravidao lerArquivoDaEscravidao(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

// O que cada NPC escravo guarda (um componente do NPC; vai no save).
struct CativeiroDoNpc {
    int64_t diaDoInicio = 0;
    bool grilhoes = false;
    int obedienciaEmDecimos = 500;  // recalculada todo dia
    int64_t castigadoAte = -1;      // o último dia do castigo de uma fuga vista; −1 = sem castigo
    bool doJogador = false;         // parte 2: o dono é o jogador (senão, a Cidade)
    bool aVenda = false;            // parte 2: está no estoque do Mercado de Escravos da cidade

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
    int64_t castigoPublicoAte = -1;  // parte 2: o castigo público que o jogador mandou fazer na vila dele

    bool operator==(const EstadoDaEscravidaoDaCidade&) const = default;
};

// Parte 2 (regra 4.8): o jogador capturado pelos bandidos e vendido. Só na vila; vai no save.
struct EstadoDoCativeiroDoJogador {
    bool estaCativo = false;
    int mapaDoMercado = 0;      // a cidade para onde foi levado
    int precoEmMoedas = 0;
    bool foiComprado = false;   // a Cidade o compra no dia 1 da estação seguinte
    int64_t peculioEmDecimos = 0;
    int64_t diaDaCaptura = -1;
    int64_t semFugirAte = -1;   // depois de uma fuga vista: o castigo

    bool operator==(const EstadoDoCativeiroDoJogador&) const = default;
};

// O que o jogador pode pedir (o "numero" é o número do NPC, quando há um).
enum class AcaoDaEscravidao { ComprarNoMercado, Vender, Libertar, PorGrilhoes, TirarGrilhoes, CastigoPublico, ComprarALiberdade, TentarFugir };

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

// ---------------------------------------------------------------------------
// Parte 2: o Mercado de Escravos, os escravos do jogador e o jogador capturado.
// ---------------------------------------------------------------------------

// A cidade tem Mercado de Escravos: a lei permite e ela é a capital de um reino ou está na lista do escravidao.json.
bool cidadeTemMercadoDeEscravos(const SimulacaoDaVila& vila, int numeroDoMapa);
// Os escravos à venda na cidade, na ordem dos NPCs.
std::vector<entt::entity> escravosAVenda(const SimulacaoDaVila& cidade);
// O estoque-alvo: 2 × as vagas Braçais vazias da cidade, no mínimo 2.
int estoqueAlvoDoMercado(SimulacaoDaVila& cidade);
// O preço de um escravo no Mercado da cidade, em décimos de moeda: o preço base × o fator da oferta e
// da procura (o do Mercado Local, de 50 a 200%).
int64_t precoNoMercadoEmDecimos(SimulacaoDaVila& cidade, entt::entity npc, const std::string& culturaDoComprador);
// A cidade do mundo que fica no mapa (a vila do jogador também), ou nullptr.
SimulacaoDaVila* cidadeDoMundoNoMapa(SimulacaoDaVila& vila, int numeroDoMapa);
// Leva um escravo de uma cidade para outra (precisa de uma cama livre no destino). Devolve o NPC novo, ou entt::null.
entt::entity moverEscravoDeCidade(SimulacaoDaVila& origem, entt::entity npc, SimulacaoDaVila& destino, bool doJogador, bool aVenda);

// Os pedidos do jogador. Devolve o motivo da recusa, ou vazio se deu certo.
std::string fazerAcaoDaEscravidao(SimulacaoDaVila& vila, AcaoDaEscravidao acao, int numero, std::vector<std::string>& linhas);
// O custo de comprar a própria liberdade (1,5 × o preço), em décimos.
int64_t custoDaLiberdadeEmDecimos(const SimulacaoDaVila& vila);

// Regra 4.8 (ES1-4): o grupo perdeu para os bandidos. Sorteia a captura; se capturado, o grupo é
// levado ao Mercado de Escravos mais perto e perde o dinheiro que levava. Devolve se foi capturado.
bool sortearACapturaDoJogador(SimulacaoDaVila& vila, std::vector<std::string>& linhas);

// População: os filhos de escravos nascem escravos (a parte dos nascidos igual à parte de escravos da cidade).
void anotarONascimentoNaEscravidao(ContextoDaProducao& contexto, entt::entity npc);
