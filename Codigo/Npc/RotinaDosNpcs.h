#pragma once

// A rotina e a Utilidade (Ficha Lógica: IA, regras 4.2 a 4.6, marco 5): cada
// NPC escolhe a ação pelas notas, vai até o lugar dela, entra nas construções
// e passeia por tiles sorteados. O movimento em si é o do marco 4.

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include "Mapa/MapaLocal.h"
#include "Npc/ComponentesDoNpc.h"
#include "Npc/RegrasDaRotina.h"
#include "Tempo/RelogioDoMundo.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/VilaInicial.h"

// A produção de cada construção (marco 6): a rotina só lê o campo dos Braçais.
struct ProducaoDaConstrucao;

enum class PeriodoDaRotina { Dormir, Trabalhar, Livre };

// Uma hora está no período [inicio, fim)? O período pode passar da meia-noite (22 a 6).
bool horaEstaNoPeriodo(int horaDoDia, int horaDeInicio, int horaDeFim);

// Regra 4.2, passo 1.
PeriodoDaRotina descobrirPeriodoDaRotina(int horaDoDia, const RegrasDaRotina& rotina);

// Da hora da refeição até 30 minutos depois (7h00 a 7h29, por exemplo).
bool estaNaHoraDeComer(int horaDoDia, int minutoDaHora, const RegrasDaUtilidade& utilidade);

// O dia de compra: (dia da partida + número do NPC) resto 2 = 0.
bool ehDiaDeCompraDoNpc(int64_t diaDaPartida, int numeroDoNpc, int diasEntreCompras);

// O que as notas precisam saber de um NPC.
struct SituacaoDoNpc {
    PeriodoDaRotina periodo = PeriodoDaRotina::Livre;
    bool temEmprego = false;
    bool estaNaHoraDeComer = false;
    bool ehDiaDeCompra = false;
    bool jaComprouHoje = false;
    bool vilaTemMercado = true;
    bool periodoLivreEmCasa = false;  // a tempestade (ficha do Clima, CL1-1): não compra
};

// As notas, em centésimos (1,0 = 100).
struct NotasDasAcoes {
    int dormir = 0;
    int trabalhar = 0;
    int comer = 0;
    int comprar = 0;
    int passear = 0;
};

// Regra 4.2, passo 2.
NotasDasAcoes calcularNotasDasAcoes(const SituacaoDoNpc& situacao, const RegrasDaUtilidade& utilidade);

// Regra 4.2, passo 3: a maior nota; no empate, Comer, Comprar, Trabalhar, Dormir, Passear.
AcaoDoNpc escolherAcaoPelaUtilidade(const NotasDasAcoes& notas);

// 30 minutos com 800 ticks por hora -> 400 ticks (arredonda para baixo).
int converterMinutosEmTicks(int minutos, int ticksPorHoraDeJogo);

// 5 minutos -> 66,7 ticks -> 67 (arredonda para cima, parte 5 da ficha).
int calcularTicksEntreReavaliacoes(const RegrasDaUtilidade& utilidade, int ticksPorHoraDeJogo);

const char* nomeDaAcaoParaMostrar(AcaoDoNpc acao);

// Tudo o que as regras da rotina usam, junto, para não passar 6 coisas a cada função.
struct ContextoDaRotina {
    entt::registry& registroDasEntidades;
    const MapaLocal& mapa;
    const VilaDoJogo& vila;
    const RegrasDaVidaDosNpcs& regras;
    int ticksPorHoraDeJogo;
    SorteioComSemente& sorteioDosNpcs;
    // Marco 6: o campo de cada construção (Produção, regra 4.1). Sem ele
    // (nullptr, como nos testes do marco 5), todos trabalham dentro.
    const std::vector<ProducaoDaConstrucao>* producaoDasConstrucoes = nullptr;
    // Fase A1 (ficha do Clima, CL1-1): na tempestade, o Passear vai para a
    // própria casa e o Comprar vale 0.
    bool periodoLivreEmCasa = false;
};

// Regra 4.1, passos 5 e 6: cria os NPCs da vila dentro das casas, e cada um já
// escolhe a ação. Devolve as entidades, na ordem da vila.
std::vector<entt::entity> criarOsNpcsDaVila(ContextoDaRotina& contexto, const RelogioDoMundo& relogio);

// A cada tick, antes do movimento: reavalia os NPCs da vez (regra 4.2) e
// termina as esperas que acabaram (regra 4.5).
void rodarARotinaAntesDeAndar(ContextoDaRotina& contexto, const RelogioDoMundo& relogio);

// A cada tick, depois do movimento (regra 4.4): quem chegou à porta entra;
// quem chegou ao ponto do passeio começa a esperar.
void cuidarDasChegadas(ContextoDaRotina& contexto, const RelogioDoMundo& relogio);

// Regra 4.6, passo 3: um tile livre a até 12 tiles, que não é porta de
// construção, em no máximo 20 tentativas.
std::optional<PosicaoDeTile> sortearTileDoPasseio(const MapaLocal& mapa, const VilaDoJogo& vila, PosicaoDeTile tileAtual,
                                                  const RegrasDoPasseio& passeio, SorteioComSemente& sorteio);

// A tabela da tecla F3: uma linha por NPC, com a ação, onde está e as notas.
std::string montarTabelaDasAcoesDosNpcs(const ContextoDaRotina& contexto, const RelogioDoMundo& relogio);
