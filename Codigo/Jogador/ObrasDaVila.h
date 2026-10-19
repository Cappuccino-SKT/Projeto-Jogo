#pragma once

// As obras e a manutenção (Ficha Lógica: Construção, parte 10, marco 8):
// construir no lote livre, subir de nível e a manutenção por estação. Os
// Operários do Construtor somam as horas; a fila de obras é a de
// ObrasDaVila (Producao/EstadoDoJogador.h).

#include <cstdint>
#include <string>
#include <vector>

#include "Construcao/RegrasDasConstrucoes.h"
#include "Producao/ProducaoDaVila.h"
#include "Tempo/RelogioDoMundo.h"

constexpr const char* tipoDoConstrutor = "construcao.construtor";

// Regra 10.4, passo 1: o custo no nível 1 × o nível de destino (V2-8).
CustoDaObra calcularCustoDaObra(PorteDoLote porte, int nivelDeDestino, const RegrasDasConstrucoes& regras);

// Regra 10.6, passo 1: 5% dos materiais do nível atual (cada um para cima, e
// no mínimo 1 Tábua) e 5% das horas (para cima). Pequena no nível 1: 1 Tábua,
// 1 Bloco, 2 h.
CustoDaObra calcularCustoDaManutencao(PorteDoLote porte, int nivelAtual, const RegrasDasConstrucoes& regras);

// O dinheiro das horas de uma obra, em décimos: horas × o salário Braçal, com
// a metade subindo (40 h × 0,75 = 30,0).
int64_t calcularPagamentoDasHorasDeObra(int horas, const ContextoDaProducao& contexto);

// Regra 10.4. Devolve o motivo da recusa em texto, ou vazio se a obra entrou na fila.
// Uma construção nova nasce na vila "em obra" e já bloqueia o lote.
std::string encomendarUmaConstrucaoNova(ContextoDaProducao& contexto, int numeroDoLote, const std::string& tipoDeConstrucao);
std::string encomendarUmaSubidaDeNivel(ContextoDaProducao& contexto, int indiceDaConstrucao);

// Regra 10.5, a cada hora: as horas dos Operários vão para a obra da vez.
// As linhas ("Obra pronta: ...") ficam em contexto.obras.linhasParaOConsole.
void andarAsObrasDaHora(ContextoDaProducao& contexto);

// Regra 10.6, o passo "manutencao" da ordem do dia: no primeiro dia de cada
// estação entra a manutenção de cada construção (a que ficou sem ficar pronta
// vira "atrasada"); todo dia, as que esperam material tentam pagar de novo.
void rodarAManutencaoDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio);

// Se há uma obra na fila para a construção (para o painel e os alertas).
bool construcaoTemObraNaFila(const ContextoDaProducao& contexto, int indiceDaConstrucao);
