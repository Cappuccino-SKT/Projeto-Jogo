#pragma once

// As regras da Viagem sem a vila (Ficha Lógica: Viagem, fase B1): o tempo de
// um trecho, a rota mais rápida, o sorteio do encontro e o encontro com os
// bandidos. O ViagemDaVila.h liga isto ao relógio, ao inventário e ao save.

#include <cstdint>
#include <string>
#include <vector>

#include "Clima/Clima.h"
#include "Tempo/RelogioDoMundo.h"
#include "Tempo/SorteioComSemente.h"
#include "Viagem/MundoDoJogo.h"
#include "Viagem/RegrasDaViagem.h"

// A viagem sorteia com a sequência 5 (NPCs 1, eventos 2, clima 3, população 4).
constexpr uint64_t sequenciaDoSorteioDaViagem = 5;
// O clima das outras regiões sorteia com a sequência 9: assim a sequência 3 continua
// só da vila, e o teste de 1 ano não muda (regra 4.8; ver o Jogo/LEIA-ME.md).
constexpr uint64_t sequenciaDoSorteioDoClimaDasOutrasRegioes = 9;

// Fase C3 (Crime, 4.7): os Caçadores de Recompensa; o "bandidos" do encontro marcado é a força deles.
enum class TipoDeEncontro { Nenhum, Bandidos, MercadorViajante, Cacadores };

struct EncontroMarcado {
    TipoDeEncontro tipo = TipoDeEncontro::Nenhum;
    int bandidos = 0;

    bool operator==(const EncontroMarcado&) const = default;
};

enum class EscolhaNoEncontro { Subornar, Fugir, Lutar, Render };

// A parte 2 da ficha: uma entidade em trânsito. O grupo do jogador é a única desta versão.
struct EntidadeEmTransito {
    std::string tipoDeEntidade = "grupo_do_jogador";  // ou "caravana" (Comércio, B2)
    bool emTransito = false;
    int mapaAtual = 0;
    int proximoMapa = 0;
    std::vector<int> rota;  // os mapas depois do próximo, até o destino
    int ticksDoTrecho = 0;  // calculado ao entrar no trecho e fixo até o fim dele (VI1-3)
    int ticksAndadosNoTrecho = 0;
    int pessoas = 1;
    bool montados = false;
    bool cargaAcimaDoLimite = false;
    bool marchaForcada = false;
    int noitesSemDormir = 0;
    int ticksSemDormir = 0;  // nas horas de acampar, com marcha forçada
    EncontroMarcado encontroMarcado;
    bool encontroAcontecendo = false;  // parado, esperando a escolha do jogador
    int ticksParadoNaLuta = 0;
    std::vector<int> quantidadePorItem;  // o inventário do grupo, na ordem do itens.csv
    int64_t dinheiroEmDecimos = 0;

    bool ehCaravana() const { return tipoDeEntidade == "caravana"; }
    int destino() const { return rota.empty() ? proximoMapa : rota.back(); }
};

// Regra 4.2: "estrada", ou "cidade_grande" com uma cidade de nível 3 ou 4 no mapa.
std::string infraestruturaDoMapa(const MundoDoJogo& mundo, int numeroDoMapa);

// O multiplicador do tamanho: o primeiro par [até tantas pessoas, multiplicador] que cabe.
int multiplicadorDoTamanhoDoGrupo(int pessoas, const RegrasDaViagem& regras);

// Clima × estação ÷ 100, com teto de 200 (regra 4.2). A lama da Primavera só sem estrada.
int multiplicadorDeClimaEEstacao(TipoDeClima climaDaRegiao, Estacao estacao, const MundoDoJogo& mundo, int mapaDeChegada,
                                 const RegrasDaViagem& regras);

// Regra 4.2: 3.200 × terreno × infraestrutura × tamanho × montaria × carga × caravana
// × clima e estação, cada divisão por 100 para baixo, nesta ordem; no mínimo 800.
int calcularTicksDoTrecho(const EntidadeEmTransito& entidade, const MundoDoJogo& mundo, int mapaDeChegada, int climaEEstacaoEmPorcento,
                          const RegrasDaViagem& regras);

// Regra 4.1: Dijkstra sem clima e estação. No empate, o caminho que passa pelo mapa de
// menor número primeiro. Devolve os mapas depois da origem, até o destino; vazio se for o mesmo.
std::vector<int> buscarRotaMaisRapida(int origem, int destino, const EntidadeEmTransito& entidade, const MundoDoJogo& mundo,
                                      const RegrasDaViagem& regras);

// Regra 4.5, passo 1: a chance do mapa de chegada, × 150% na marcha forçada.
// Fase C3 (Crime, 4.3): mais a ameaça bandida da região do mapa, em por mil (vazio = 0).
int chanceDeEncontroPorMil(const EntidadeEmTransito& entidade, const MundoDoJogo& mundo, int mapaDeChegada, const RegrasDaViagem& regras,
                           const std::vector<int>& ameacaBandidaPorRegiao = {});

// Regra 4.5, passos 2 a 4, com os três números já sorteados (de 0 a 999, de 0 a 999 e de 0 a 9).
EncontroMarcado escolherOEncontro(int chancePorMil, int numeroDoEncontro, int numeroDoTipo, int numeroDoBando, const RegrasDaViagem& regras);

// A mesma regra, sorteando. Sempre tira os três números, para o sorteio andar igual.
EncontroMarcado sortearEncontroAoEntrarNoTrecho(const EntidadeEmTransito& entidade, const MundoDoJogo& mundo, int mapaDeChegada,
                                                const RegrasDaViagem& regras, SorteioComSemente& sorteio, const std::vector<int>& ameacaBandidaPorRegiao = {});

// A força simples (VI1-4): o jogador e, a partir dos companheiros, os outros como Civis.
int forcaDoGrupo(const EntidadeEmTransito& entidade, const RegrasDaViagem& regras);
int forcaDoBando(int bandidos, const RegrasDaViagem& regras);
// Regra 4.6, passo 3: não atacam se a força do grupo for mais de 2 vezes a do bando.
bool bandidosAtacam(int forcaDoGrupo, int bandidos, const RegrasDaViagem& regras);
// Bandidos × 25 moedas × (150 − Carisma) ÷ 100, em décimos. 8 bandidos, Carisma 50: 2.000.
int64_t precoDoSubornoEmDecimos(int bandidos, int carisma, const RegrasDaViagem& regras);
// 50% + (tempo do bando − tempo do grupo) × 50 ÷ tempo do bando, entre 10% e 90%.
int chanceDeFugirEmPorcento(const EntidadeEmTransito& entidade, int bandidos, const RegrasDaViagem& regras);
// Regras 4.6 e 4.11: A vence se o número (de 0 a 999) for menor que A × 1.000 ÷ (A + B).
bool resolverPorCalculoRapidoDeForcas(int forcaA, int forcaB, int numeroDe0A999);
// Perde a porcentagem do dinheiro e de cada item, cada perda arredondada para baixo.
void aplicarAPerdaDoGrupo(EntidadeEmTransito& entidade, int porcentagem);

// O clima de hoje da região de um mapa (Limpo se a lista não tem a região).
TipoDeClima climaDaRegiaoDoMapa(const std::vector<TipoDeClima>& climaDeHojePorRegiao, const MundoDoJogo& mundo, int numeroDoMapa);

// Regras 4.2 e 4.5: a entidade entra no trecho até o próximo mapa (o tempo fixo e o encontro).
void entrarNoTrecho(EntidadeEmTransito& entidade, int proximoMapa, const MundoDoJogo& mundo, const RegrasDaViagem& regras,
                    const std::vector<TipoDeClima>& climaDeHojePorRegiao, Estacao estacao, SorteioComSemente& sorteio, const std::vector<int>& ameacaBandidaPorRegiao = {});

// O que aconteceu num tick de uma entidade em trânsito.
enum class PassoDaEntidade { Parada, Andou, EncontroNaMetade, ChegouAoProximoMapa, ChegouAoDestino };

// Regras 4.3 e 4.4 para qualquer entidade (o grupo do jogador e as caravanas, B2): parada
// na luta, acampada das 22 h às 6 h (com marcha forçada, conta as noites sem dormir), a
// caravana parada na Tempestade, o encontro na metade do trecho e a chegada. Ao chegar a um
// mapa que não é o destino, já entra no trecho seguinte. Quem chama cuida do encontro.
PassoDaEntidade andarUmTick(EntidadeEmTransito& entidade, const MundoDoJogo& mundo, const RegrasDaViagem& regras,
                            const std::vector<TipoDeClima>& climaDeHojePorRegiao, int horaDoDia, Estacao estacao, int ticksPorHoraDeJogo,
                            SorteioComSemente& sorteio, const std::vector<int>& ameacaBandidaPorRegiao = {});

// Regra 4.6: aplica a escolha. Devolve a linha do console; vazio se a escolha não vale
// (o suborno sem o dinheiro). Fugir que falha vira Lutar.
// Fase B4: "venceuALuta", se não for nulo, diz se o grupo venceu a luta.
std::string resolverEncontroComBandidos(EntidadeEmTransito& entidade, EscolhaNoEncontro escolha, const RegrasDaViagem& regras,
                                        SorteioComSemente& sorteio, bool* venceuALuta = nullptr);
