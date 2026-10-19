#include "Vila/SimulacaoDaVila.h"

#include "Desempenho/MedicaoDoDesempenho.h"
#include "Economia/ConsumoESalarios.h"
#include "Jogador/PedidosDoJogador.h"

SimulacaoDaVila::SimulacaoDaVila(const DadosDoJogo& dadosDoJogo, const RegrasDoTempo& regrasDoTempo, int distanciaDoCicloDeAndarEmMilesimos,
                                 std::optional<uint64_t> sementeNoLugarDaDoArquivo)
    : dados(dadosDoJogo), relogio(RelogioDoMundo::criarParaPartidaNova(regrasDoTempo)), vila(dadosDoJogo.vilaInicial), mapa(dadosDoJogo.mapaDaVila), ticksPorHoraDeJogo(regrasDoTempo.ticksPorHoraDeJogo),
      sorteioDosNpcs(sementeNoLugarDaDoArquivo.value_or(dadosDoJogo.vilaInicial.sementeDaPartida), sequenciaDoSorteioDosNpcs),
      sorteioDosEventos(sementeNoLugarDaDoArquivo.value_or(dadosDoJogo.vilaInicial.sementeDaPartida), sequenciaDoSorteioDosEventos)
{
    regrasDoMovimento.velocidadeDeAndarEmMilesimosPorTick =
        converterVelocidadeParaMilesimosPorTick(dados.regras.velocidadeDeAndarEmTilesPorHora, ticksPorHoraDeJogo);
    regrasDoMovimento.caminhosCalculadosPorTick = dados.regras.caminhosCalculadosPorTick;
    regrasDoMovimento.distanciaDoCicloDeAndarEmMilesimos = distanciaDoCicloDeAndarEmMilesimos;

    // Os NPCs nascem dentro das casas (marco 5) sem a produção, que precisa
    // deles; depois a produção monta tudo, e cada NPC decide de novo com o
    // campo dos Braçais já conhecido.
    ContextoDaRotina rotinaSemProducao{registroDasEntidades, mapa, vila, dados.regras.vidaDosNpcs, ticksPorHoraDeJogo,
                                       sorteioDosNpcs};
    npcsDaVila = criarOsNpcsDaVila(rotinaSemProducao, relogio);
    ContextoDaProducao producao = contextoDaProducao();
    avisosAoMontar = montarAProducaoDaVila(producao, npcsDaVila);
}

ContextoDaRotina SimulacaoDaVila::contextoDaRotina()
{
    ContextoDaRotina contexto{registroDasEntidades, mapa, vila, dados.regras.vidaDosNpcs, ticksPorHoraDeJogo, sorteioDosNpcs};
    contexto.producaoDasConstrucoes = &producaoDasConstrucoes;
    return contexto;
}

ContextoDaProducao SimulacaoDaVila::contextoDaProducao()
{
    return ContextoDaProducao{registroDasEntidades, dados,    inventario, dinheiro,          producaoDasConstrucoes,
                              consumoMedido,        mercadoLocal, felicidade, eventos, sorteioDosEventos, vila, mapa, decisoes, obras, ticksPorHoraDeJogo};
}

void SimulacaoDaVila::criarNpcsSoParaMedirODesempenho(int quantidade)
{
    // Espalha os NPCs pelos tiles que dão para andar, um a cada 7 (sem sorteio:
    // assim o sorteio dos NPCs da vila continua o mesmo).
    const int totalDeTiles = mapa.larguraEmTiles * mapa.alturaEmTiles;
    int posicao = 0;
    for (int criado = 0; criado < quantidade && totalDeTiles > 0; ++criado) {
        for (int tentativa = 0; tentativa < totalDeTiles; ++tentativa) {
            posicao = (posicao + 7) % totalDeTiles;
            if (mapa.tileDaParaAndar(posicao % mapa.larguraEmTiles, posicao / mapa.larguraEmTiles)) {
                break;
            }
        }
        const entt::entity npc = criarNpcNoTile(registroDasEntidades, {posicao % mapa.larguraEmTiles, posicao / mapa.larguraEmTiles});
        const int numeroDoNpc = static_cast<int>(vila.npcsIniciais.size()) + criado;
        registroDasEntidades.emplace<IdentidadeDoNpc>(npc, IdentidadeDoNpc{"Visitante " + std::to_string(criado + 1), "civil", numeroDoNpc});
        registroDasEntidades.emplace<CasaETrabalhoDoNpc>(npc, CasaETrabalhoDoNpc{-1, -1});
        registroDasEntidades.emplace<DecisaoDoNpc>(npc);
    }
}

void SimulacaoDaVila::registrarNasNotificacoes(const std::vector<std::string>& linhas, std::size_t aPartirDaLinha)
{
    const ContextoDaProducao producao = contextoDaProducao();
    for (std::size_t posicao = aPartirDaLinha; posicao < linhas.size(); ++posicao) {
        NivelDaNotificacao nivel = NivelDaNotificacao::Info;
        if (classificarALinhaDoConsole(producao, linhas[posicao], nivel)) {
            notificacoes.acrescentar(nivel, linhas[posicao], relogio.diaDaPartida(), relogio.tickAtual());
        }
    }
}

std::vector<std::string> SimulacaoDaVila::aplicarOsPedidosPendentes()
{
    if (pedidos.estaVazia()) {
        return {};
    }
    ContextoDaProducao producao = contextoDaProducao();
    const std::vector<std::string> linhas = aplicarOsPedidosDoJogador(producao, pedidos);
    registrarNasNotificacoes(linhas, 0);
    return linhas;
}

std::vector<std::string> SimulacaoDaVila::avancarUmTick()
{
    MEDIR_ESTE_TRECHO("um tick");
    // Os pedidos do jogador entram antes de qualquer regra (Interface, regra 10.8).
    std::vector<std::string> linhasParaOConsole = aplicarOsPedidosPendentes();
    const std::size_t linhasJaNasNotificacoes = linhasParaOConsole.size();
    ContextoDaProducao producao = contextoDaProducao();
    ContextoDaRotina rotina = contextoDaRotina();

    // 1 e 2. O relógio e, na virada da hora, o ciclo da hora que acabou.
    const int64_t diaAntesDoTick = relogio.diaDaPartida();
    const Estacao estacaoAntesDoTick = relogio.estacaoAtual();
    const int diaDaEstacaoAntesDoTick = relogio.diaDaEstacaoAtual();
    relogio.avancarUmTick();
    if (relogio.comecouUmaHoraNova()) {
        MEDIR_ESTE_TRECHO("ciclos da hora");
        rodarOsCiclosDaHora(producao, relogio);
        linhasParaOConsole.insert(linhasParaOConsole.end(), obras.linhasParaOConsole.begin(), obras.linhasParaOConsole.end());
        obras.linhasParaOConsole.clear();
    }

    // 3. Na virada do dia: a ordem do dia, o resumo no console, a linha do
    //    relatório e as contas do dia zeradas.
    if (relogio.comecouUmDiaNovo()) {
        MEDIR_ESTE_TRECHO("ordem do dia");
        const std::vector<std::string> linhasDaOrdemDoDia = rodarAOrdemDoDia(producao, relogio);
        linhasParaOConsole.insert(linhasParaOConsole.end(), linhasDaOrdemDoDia.begin(), linhasDaOrdemDoDia.end());
        linhasParaOConsole.push_back(montarResumoDoDia(producao, diaAntesDoTick));
        linhaDoRelatorioDoUltimoDia = montarLinhaDoRelatorio(producao, diaAntesDoTick, estacaoAntesDoTick, diaDaEstacaoAntesDoTick);
        dinheiro.consumoNaoPagoHoje = 0;
        dinheiro.comidaQueFaltouHoje = 0;
        dinheiro.gastoDoJogadorComInsumosHoje = 0;
        for (ProducaoDaConstrucao& producaoDaConstrucao : producaoDasConstrucoes) {
            producaoDaConstrucao.producaoDeHojePorItem.clear();
        }
    }

    // 4. Os NPCs (marcos 4 e 5): a rotina decide, os caminhos pedidos são
    //    calculados, os NPCs andam e, por fim, quem chegou entra.
    {
        MEDIR_ESTE_TRECHO("rotina dos NPCs");
        rodarARotinaAntesDeAndar(rotina, relogio);
    }
    {
        MEDIR_ESTE_TRECHO("caminhos");
        for (const std::string& aviso : atenderPedidosDeCaminho(registroDasEntidades, mapa, buscadorDeCaminho, regrasDoMovimento)) {
            linhasParaOConsole.push_back("Aviso: " + aviso + ".");
        }
    }
    {
        MEDIR_ESTE_TRECHO("movimento");
        andarOsNpcsUmTick(registroDasEntidades, mapa, regrasDoMovimento, relogio.tickAtual());
        cuidarDasChegadas(rotina, relogio);
    }

    // 5. Quem está trabalhando agora soma 1 tick (Produção, regra 4.1).
    contarOsTicksTrabalhados(producao);
    registrarNasNotificacoes(linhasParaOConsole, linhasJaNasNotificacoes);
    MARCAR_O_FIM_DE_UM_TICK();
    return linhasParaOConsole;
}
