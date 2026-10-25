#pragma once

// O crime e a justiça (Ficha Lógica: Crime e Justiça, fase C3): a Criminalidade de cada cidade
// (que tira Felicidade, causa furtos e põe bandidos nas estradas) e o crime do jogador (a
// revista na entrada das cidades de facção, a multa, o suborno, a fuga, a Recompensa, a prisão
// e os Caçadores de Recompensa).

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class SimulacaoDaVila;
struct ContextoDaProducao;
class ListaDeProblemasDosDados;
enum class EscolhaNoEncontro;

// O data/crimes.json (parte 2 da ficha).
struct RegrasDoCrime {
    std::map<std::string, int> gravidadePorCrime;  // "crime.contrabando": 2
    int multaPorGravidadeAoQuadrado = 25;          // em moedas
    int recompensaVezesAMulta = 2;
    int recompensaCaiPorEstacaoEmPorcento = 10;
    int cacadoresAPartirDe = 500;                   // em moedas
    std::map<std::string, int> revistaPorFaixaEmPorcento;  // "neutro": 10
    int subornoBaseEmPorcento = 20;
    int subornoPorMultaOferecidaEmPorcento = 40;
    int subornoMaximoEmPorcento = 90;
    int diasDePrisaoPorGravidade = 2;
    int fiancaVezesAMulta = 2;
    int cacadoresChancePorMil = 100;
    // A Criminalidade (4.1 a 4.3), em pontos.
    int criminalidadeBase = 10;
    int porPontoDeFelicidadeAbaixoDe50 = 1;
    int porPontoDaTaxaDeDesemprego = 20;
    int semDinheiroParaDoisDiasDaCesta = 15;
    int passoPorDiaEmPorcento = 10;
    int felicidadeAPartirDe = 20;
    int felicidadePorPontoAcimaEmDecimos = -2;  // −0,2 por ponto
    int furtoAPartirDe = 25;
    int furtoPorMilPorPonto = 1;
    int furtoTiraEmPorcento = 10;
    int furtoCooldownEmDias = 8;
    int ameacaBandidaAPartirDe = 10;
    int ameacaBandidaPorMilPorPonto = 3;

    int multaDaGravidade(int gravidade) const { return multaPorGravidadeAoQuadrado * gravidade * gravidade; }
};

RegrasDoCrime lerArquivoDosCrimes(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

// --- A Criminalidade da cidade (4.1 a 4.3), em décimos ---

// Regra 4.1, passo 1: 10 + (50 − Felicidade) + 20 × a taxa de desemprego + 15 sem dinheiro para 2 dias da cesta.
int calcularAlvoDaCriminalidade(int felicidadeEmDecimos, int civis, int desempregados, bool semDinheiroParaDoisDias, const RegrasDoCrime& regras);
// Regra 4.1, passo 2: 10% ao dia até o alvo, como a Felicidade.
int darOPassoDaCriminalidade(int criminalidadeEmDecimos, int alvoEmDecimos, const RegrasDoCrime& regras);
// Regra 4.1, passo 3 (CJ1-2): −0,2 de Felicidade por ponto acima de 20, em décimos.
int felicidadeDaCriminalidadeEmDecimos(int criminalidadeEmDecimos, const RegrasDoCrime& regras);
// O passo 5b da ordem do dia (depois da Felicidade): a Criminalidade anda e, com 25 ou mais, o
// furto no armazém (4.2) pode acontecer. Devolve as linhas do console.
std::vector<std::string> rodarACriminalidadeDoDia(ContextoDaProducao& contexto, int civis, int desempregados, bool semDinheiroParaDoisDias);
// Regra 4.3: a ameaça bandida de uma região pela média da Criminalidade dela, em por mil.
int ameacaBandidaComOsNumeros(int mediaDaCriminalidadeEmDecimos, const RegrasDoCrime& regras);
// Na vila, no dia novo: a ameaça de cada região, pelas cidades do mundo.
void atualizarAAmeacaBandida(SimulacaoDaVila& vila);

// --- O crime do jogador (4.4 a 4.7) ---

// A revista que achou alguma coisa e espera a escolha do jogador.
struct RevistaPendente {
    bool ativa = false;
    int faccao = -1;
    int mapa = 0;
    int mapaAnterior = 0;
    std::vector<std::string> crimes;  // "crime.contrabando", "crime.heresia_sem_livro"
    int gravidade = 0;                 // a maior (é a que conta)
    int multa = 0;                     // em moedas

    bool operator==(const RevistaPendente&) const = default;
};

struct EstadoDoCrimeDoJogador {
    std::vector<int> recompensaPorFaccao;     // em moedas
    std::vector<int> gravidadesNaFicha;       // a soma das gravidades dos crimes não pagos
    std::vector<int> fiancaNaFicha;           // em moedas
    std::vector<int> estacaoDaUltimaHeresia;  // a estação da partida (dia ÷ 16); −1 = nunca
    int64_t presoAteODia = -1;                // −1 = solto
    int faccaoDaPrisao = -1;
    int faccaoDosCacadores = -1;              // a facção dos Caçadores do encontro de agora
    int proximoMapaNaUltimaConferencia = 0;   // para saber quando o grupo entra num trecho novo
    RevistaPendente revista;

    bool operator==(const EstadoDoCrimeDoJogador&) const = default;
    bool estaPreso(int64_t hoje) const { return presoAteODia >= hoje; }
};

enum class EscolhaNaRevista { Pagar, Subornar, Fugir };

void montarOCrimeDaPartidaNova(SimulacaoDaVila& vila);
// Regra 4.4, passo 1 (CJ1-1 e CJ1-6): a chance pela faixa; com Recompensa, 100%.
int chanceDaRevistaEmPorcento(const RegrasDoCrime& regras, const std::string& faixa, int recompensa);
// Regra 4.4, passo 3: 20% + 40% × oferta ÷ multa, até 90%.
int chanceDoSubornoEmPorcento(int64_t ofertaEmDecimos, int multaEmMoedas, const RegrasDoCrime& regras);
// O grupo entrou no mapa: numa cidade de facção, a revista (4.4). Devolve true se achou algo
// (o jogo pausa e espera a escolha); as linhas vão para o console.
bool revistarJogador(SimulacaoDaVila& vila, int mapa, int mapaAnterior, std::vector<std::string>& linhas);
// A escolha do jogador na revista. Devolve o motivo da recusa, ou vazio.
std::string resolverARevista(SimulacaoDaVila& vila, EscolhaNaRevista escolha, int64_t ofertaEmDecimos, std::vector<std::string>& linhas);
// Regra 4.6: preso pela ficha da facção, no mapa da capital dela se estiver na estrada.
void prenderJogador(SimulacaoDaVila& vila, int faccao, std::vector<std::string>& linhas);
// Regra 4.6, passo 2: a fiança. Devolve o motivo da recusa, ou vazio.
std::string pagarAFianca(SimulacaoDaVila& vila, std::vector<std::string>& linhas);
// No dia novo: a soltura (4.6) e, no dia 1 da estação, a queda da Recompensa (4.9).
std::vector<std::string> rodarOCrimeDoJogadorNoDiaNovo(SimulacaoDaVila& vila);
// Regra 4.9: 10% a menos, para baixo; abaixo de 10, zera.
int recompensaDepoisDaQueda(int recompensa, const RegrasDoCrime& regras);
// Regra 4.7: a cada tick, se o grupo entrou num trecho que chega a um mapa de uma facção com
// Recompensa de 500 ou mais, sorteia o encontro de Caçadores. Devolve as linhas.
std::vector<std::string> conferirOsCacadores(SimulacaoDaVila& vila);
// Regra 4.7: a escolha no encontro com os Caçadores: lutar (a luta rápida) ou ser preso.
std::string resolverOsCacadores(SimulacaoDaVila& vila, EscolhaNoEncontro escolha, std::vector<std::string>& linhas);
// A força dos Caçadores: Recompensa ÷ 100 × 3.
int forcaDosCacadores(int recompensa);
