#include "Vila/SimulacaoDaVila.h"

#include "Economia/ConsumoESalarios.h"

SimulacaoDaVila::SimulacaoDaVila(const DadosDoJogo& dadosDoJogo, const RegrasDoTempo& regrasDoTempo, int distanciaDoCicloDeAndarEmMilesimos)
    : dados(dadosDoJogo), relogio(RelogioDoMundo::criarParaPartidaNova(regrasDoTempo)), ticksPorHoraDeJogo(regrasDoTempo.ticksPorHoraDeJogo),
      sorteioDosNpcs(dadosDoJogo.vilaInicial.sementeDaPartida, sequenciaDoSorteioDosNpcs)
{
    regrasDoMovimento.velocidadeDeAndarEmMilesimosPorTick =
        converterVelocidadeParaMilesimosPorTick(dados.regras.velocidadeDeAndarEmTilesPorHora, ticksPorHoraDeJogo);
    regrasDoMovimento.caminhosCalculadosPorTick = dados.regras.caminhosCalculadosPorTick;
    regrasDoMovimento.distanciaDoCicloDeAndarEmMilesimos = distanciaDoCicloDeAndarEmMilesimos;

    // Os NPCs nascem dentro das casas (marco 5) sem a produção, que precisa
    // deles; depois a produção monta tudo, e cada NPC decide de novo com o
    // campo dos Braçais já conhecido.
    ContextoDaRotina rotinaSemProducao{registroDasEntidades, dados.mapaDaVila, dados.vilaInicial, dados.regras.vidaDosNpcs, ticksPorHoraDeJogo,
                                       sorteioDosNpcs};
    npcsDaVila = criarOsNpcsDaVila(rotinaSemProducao, relogio);
    ContextoDaProducao producao = contextoDaProducao();
    avisosAoMontar = montarAProducaoDaVila(producao, npcsDaVila);
}

ContextoDaRotina SimulacaoDaVila::contextoDaRotina()
{
    ContextoDaRotina contexto{registroDasEntidades, dados.mapaDaVila, dados.vilaInicial, dados.regras.vidaDosNpcs, ticksPorHoraDeJogo, sorteioDosNpcs};
    contexto.producaoDasConstrucoes = &producaoDasConstrucoes;
    return contexto;
}

ContextoDaProducao SimulacaoDaVila::contextoDaProducao()
{
    return ContextoDaProducao{registroDasEntidades, dados, inventario, dinheiro, producaoDasConstrucoes, ticksPorHoraDeJogo};
}

std::vector<std::string> SimulacaoDaVila::avancarUmTick()
{
    std::vector<std::string> linhasParaOConsole;
    ContextoDaProducao producao = contextoDaProducao();
    ContextoDaRotina rotina = contextoDaRotina();

    // 1 e 2. O relógio e, na virada da hora, o ciclo da hora que acabou.
    const int64_t diaAntesDoTick = relogio.diaDaPartida();
    relogio.avancarUmTick();
    if (relogio.comecouUmaHoraNova()) {
        rodarOsCiclosDaHora(producao, relogio);
    }

    // 3. Na virada do dia: a ordem do dia, o resumo no console e as contas do dia zeradas.
    if (relogio.comecouUmDiaNovo()) {
        rodarAOrdemDoDia(producao);
        linhasParaOConsole.push_back(montarResumoDoDia(producao, diaAntesDoTick));
        dinheiro.consumoNaoPagoHoje = 0;
        dinheiro.comidaQueFaltouHoje = 0;
        dinheiro.gastoDoJogadorComInsumosHoje = 0;
        for (ProducaoDaConstrucao& producaoDaConstrucao : producaoDasConstrucoes) {
            producaoDaConstrucao.producaoDeHojePorItem.clear();
        }
    }

    // 4. Os NPCs (marcos 4 e 5): a rotina decide, os caminhos pedidos são
    //    calculados, os NPCs andam e, por fim, quem chegou entra.
    rodarARotinaAntesDeAndar(rotina, relogio);
    for (const std::string& aviso : atenderPedidosDeCaminho(registroDasEntidades, dados.mapaDaVila, buscadorDeCaminho, regrasDoMovimento)) {
        linhasParaOConsole.push_back("Aviso: " + aviso + ".");
    }
    andarOsNpcsUmTick(registroDasEntidades, dados.mapaDaVila, regrasDoMovimento, relogio.tickAtual());
    cuidarDasChegadas(rotina, relogio);

    // 5. Quem está trabalhando agora soma 1 tick (Produção, regra 4.1).
    contarOsTicksTrabalhados(producao);
    return linhasParaOConsole;
}
