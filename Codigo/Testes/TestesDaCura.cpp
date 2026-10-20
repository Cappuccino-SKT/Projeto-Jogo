// Os testes da cura: a tabela da parte 8 da Ficha Lógica: Cura (a Epidemia, fase A6).

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Eventos/EventosDaVila.h"
#include "Npc/ComponentesDoNpc.h"
#include "Vila/Populacao.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

int indiceDaEpidemia(const DadosDoJogo& dados)
{
    const std::vector<DefinicaoDeEvento>& eventos = dados.catalogoDeEventos.todosOsEventos();
    for (std::size_t indice = 0; indice < eventos.size(); ++indice) {
        if (eventos[indice].identificadorDeTexto == "evento.epidemia") {
            return static_cast<int>(indice);
        }
    }
    return -1;
}

void testarAChance(const DadosDoJogo& dados)
{
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    ContextoDaProducao contexto = simulacao.contextoDaProducao();
    const int epidemia = indiceDaEpidemia(dados);
    conferir(epidemia >= 0, "o eventos.json tem a Epidemia");
    const DefinicaoDeEvento& evento = dados.catalogoDeEventos.todosOsEventos()[epidemia];
    conferir(chanceDoEventoHoje(contexto, evento) == 10, "vila de 15 de 15, sem fome, sem Boticário: chance 10 por mil");
    simulacao.dinheiro.comidaQueFaltouHoje = 3;
    conferir(chanceDoEventoHoje(contexto, evento) == 20, "com fome também: 20 por mil");
    simulacao.dinheiro.comidaQueFaltouHoje = 0;
    for (int saem = 0; saem < 5; ++saem) {
        tirarNpcDaVila(contexto, simulacao.npcsDaVila.back());
    }
    conferir(chanceDoEventoHoje(contexto, evento) == 5, "vila de 10 de 15 (67%): chance 5 por mil");
}

void testarAEpidemia(const DadosDoJogo& dados)
{
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    ContextoDaProducao contexto = simulacao.contextoDaProducao();
    const int epidemia = indiceDaEpidemia(dados);

    // A Epidemia ativa: 15 NPCs, sem Remédio: o acumulado perde 15 pessoas por dia.
    simulacao.eventos.eventosAtivos.push_back(EventoAtivo{epidemia, 8});
    conferir(mortesDasDoencasHojeEmPessoas(contexto) == 15, "dia de Epidemia, 15 NPCs, sem Remédio: 15 pessoas");
    simulacao.eventos.remedioTomadoHoje = true;
    conferir(mortesDasDoencasHojeEmPessoas(contexto) == 7, "com a dose inteira do Remédio: 7 pessoas");
    simulacao.eventos.remedioTomadoHoje = false;

    // O acumulado passa de −100 durante a Epidemia: morre o doente mais velho.
    entt::entity doenteNovo = entt::null;
    for (const entt::entity npc : simulacao.npcsDaVila) {
        if (simulacao.registroDasEntidades.get<IdentidadeDoNpc>(npc).numeroDoNpc == 2) {
            doenteNovo = npc;  // Clara, 7 anos
        }
    }
    simulacao.registroDasEntidades.emplace<DoencaDoNpc>(doenteNovo, DoencaDoNpc{8});
    simulacao.populacao.acumuladoDeCrescimentoEmPessoas = -90;
    while (simulacao.relogio.diaDaPartida() < 1) {
        simulacao.avancarUmTick();
    }
    bool claraMorreu = true;
    bool pedroVive = false;
    for (const entt::entity npc : simulacao.npcsDaVila) {
        const int numero = simulacao.registroDasEntidades.get<IdentidadeDoNpc>(npc).numeroDoNpc;
        claraMorreu = claraMorreu && numero != 2;
        pedroVive = pedroVive || numero == 14;
    }
    conferir(claraMorreu && pedroVive, "na Epidemia, quem morre é o doente mais velho (a Clara), não o mais velho da vila (o Pedro)");
}

}  // namespace

void testarCura()
{
    std::printf("Cura: a Epidemia (fase A6)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    testarAChance(dados);
    testarAEpidemia(dados);
}
