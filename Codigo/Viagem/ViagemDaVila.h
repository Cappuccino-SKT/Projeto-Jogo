#pragma once

// A Viagem ligada à partida (Ficha Lógica: Viagem, fase B1): o grupo do jogador
// sai da vila com Pães e dinheiro, anda a cada tick, acampa das 22 h às 6 h,
// come no dia, encontra bandidos e chega. A vila continua rodando igual (VI1-7).
// O estado vai no save (seção "viagem").

#include <cstdint>
#include <string>
#include <vector>

#include "Producao/ProducaoDaVila.h"
#include "Viagem/Viagem.h"

// A névoa (regra 4.10): o que foi visto de cada mapa, e quando.
struct RegistroDaNevoa {
    int64_t diaEmQueFoiVisto = -1;  // −1 = nunca
    std::string dono;               // a facção da cidade, ou vazio
    int nivelDaCidade = 0;          // 0 = sem cidade
    EstadoDoTerritorio estado = EstadoDoTerritorio::SemDono;

    bool operator==(const RegistroDaNevoa&) const = default;
};

struct EstadoDaViagem {
    // O grupo do jogador. Parado na vila, ele não leva nada (o inventário é o do jogador).
    EntidadeEmTransito grupoDoJogador;
    std::vector<RegistroDaNevoa> nevoa;  // nevoa[numero do mapa − 1]
    std::vector<TipoDeClima> climaDeHojePorRegiao = std::vector<TipoDeClima>(6, TipoDeClima::Limpo);  // regra 4.8
    bool grupoSemComida = false;  // só do dia
};

// O que um tick da viagem fez: as linhas do console e se o jogo deve voltar a 1x ou pausar.
struct ResultadoDoTickDaViagem {
    std::vector<std::string> linhasParaOConsole;
    bool encontroComecou = false;  // pausa (regra 4.6, passo 1)
    bool chegouAoDestino = false;  // volta a 1x (regra 4.7)
};

// A partida nova: o grupo na vila; a névoa com as capitais e os vizinhos da vila (regra 4.10, passo 3).
void montarAViagemDaPartidaNova(EstadoDaViagem& viagem, const DadosDoJogo& dados, const EstadoDoClima& climaDaVila);

bool grupoEstaNaVila(const EstadoDaViagem& viagem, const DadosDoJogo& dados);

// Regra 4.10, passo 1: marca o mapa e os 4 vizinhos como vistos hoje.
void marcarMapaComoVisto(EstadoDaViagem& viagem, const MundoDoJogo& mundo, int numeroDoMapa, int64_t diaDaPartida);

// Regra 4.1. Saindo da vila, o grupo leva os Pães e o dinheiro do jogador; fora dela, segue
// com o que tem. Devolve o motivo da recusa, ou vazio.
std::string partirEmViagem(ContextoDaProducao& contexto, const RelogioDoMundo& relogio, int destino, int paesLevados, int64_t dinheiroLevadoEmDecimos,
                           bool marchaForcada);

// Regras 4.3 e 4.4, a cada tick.
ResultadoDoTickDaViagem avancarAViagemUmTick(ContextoDaProducao& contexto, const RelogioDoMundo& relogio);

// No dia (regras 4.4 e 4.8): o clima das outras regiões e a comida do grupo em viagem.
std::vector<std::string> rodarAViagemDoDia(ContextoDaProducao& contexto);

// Regra 4.6: a escolha do jogador no encontro com os bandidos. Devolve o motivo da recusa, ou vazio.
std::string resolverOEncontroDoGrupo(ContextoDaProducao& contexto, EscolhaNoEncontro escolha, std::vector<std::string>& linhas);
