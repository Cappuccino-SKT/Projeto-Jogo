#pragma once

// Os eventos da vila (Ficha Lógica: Eventos, marco 7): à meia-noite, os
// ativos descontam 1 dia, os condicionais conferem a condição, e os
// ocasionais são sorteados. Um evento ativo muda a produção, a Felicidade
// ou o estoque. Sem o Narrador na fatia (FL7-6).

#include <cstdint>
#include <string>
#include <vector>

#include "Eventos/DefinicaoDosEventos.h"
#include "Producao/ProducaoDaVila.h"

// No máximo 2 negativos ativos ao mesmo tempo (Eventos e Narrador).
constexpr int eventosNegativosAtivosNoMaximo = 2;

struct EventoAtivo {
    int indiceDoEvento = -1;  // a posição no CatalogoDeEventos
    int diasQueFaltam = 0;    // nos condicionais, não conta: o evento dura enquanto a condição valer
};

struct EstadoDosEventos {
    std::vector<EventoAtivo> eventosAtivos;
    std::vector<int64_t> diaEmQuePodeSairDeNovo;  // um por evento, no dia da partida (o cooldown)
    std::vector<int> diasSeguidosNaCondicao;      // um por evento (só os condicionais usam)
    // O que começou ou terminou hoje: "Seca começou" (para o relatório, não vai no save).
    std::vector<std::string> mudancasDeHoje;
    // As linhas do console de hoje: "Evento: Seca (a Fazenda produz 50% por 8 dias)."
    std::vector<std::string> linhasParaOConsole;

    bool eventoEstaAtivo(int indiceDoEvento) const;
};

// Ao montar a vila: um lugar para cada evento do catálogo.
void comecarOsEventos(ContextoDaProducao& contexto);

// À meia-noite, o passo "eventos" da ordem do dia (regras 4.1 a 4.5). O
// relógio já está no dia novo.
void rodarOsEventosDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio);

// Regra 4.4: o que a Produção pergunta. O produto das porcentagens dos
// eventos ativos para aquele tipo de construção (100 sem nenhum; a Seca e a
// Chuva boa juntas: 50% × 120% = 60).
int multiplicadorDaProducaoPorEventos(const ContextoDaProducao& contexto, const std::string& tipoDaConstrucao);

// O que a lenha do inverno pergunta (ficha do Clima, regra 4.3): o produto das
// porcentagens dos efeitos "lenha_em_porcento" ativos (100 sem nenhum).
int multiplicadorDaLenhaPorEventos(const ContextoDaProducao& contexto);

// O que a Felicidade pergunta: a soma dos pontos dos eventos ativos.
int felicidadeDosEventosAtivos(const ContextoDaProducao& contexto);

// A comida da vila em % do estoque-alvo da comida (a condição da Fome).
// Sem alvo, 100.
int comidaEmPorcentoDoAlvo(const ContextoDaProducao& contexto);
