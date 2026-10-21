#include "Vila/SimulacaoDaVila.h"

#include <algorithm>

#include "Desempenho/MedicaoDoDesempenho.h"
#include "Economia/ConsumoESalarios.h"
#include "Jogador/PedidosDoJogador.h"
#include "Cidades/OutrasCidades.h"
#include "Comercio/Comercio.h"
#include "Vila/Trabalho.h"
#include "Npc/ComponentesDoNpc.h"

SimulacaoDaVila::SimulacaoDaVila(const DadosDoJogo& dadosDoJogo, const RegrasDoTempo& regrasDoTempo, int distanciaDoCicloDeAndarEmMilesimos,
                                 std::optional<uint64_t> sementeNoLugarDaDoArquivo)
    : dados(dadosDoJogo), relogio(RelogioDoMundo::criarParaPartidaNova(regrasDoTempo)), vila(dadosDoJogo.vilaInicial), mapa(dadosDoJogo.mapaDaVila), ticksPorHoraDeJogo(regrasDoTempo.ticksPorHoraDeJogo),
      sorteioDosNpcs(sementeNoLugarDaDoArquivo.value_or(dadosDoJogo.vilaInicial.sementeDaPartida), sequenciaDoSorteioDosNpcs),
      sorteioDosEventos(sementeNoLugarDaDoArquivo.value_or(dadosDoJogo.vilaInicial.sementeDaPartida), sequenciaDoSorteioDosEventos),
      sorteioDoClima(sementeNoLugarDaDoArquivo.value_or(dadosDoJogo.vilaInicial.sementeDaPartida), sequenciaDoSorteioDoClima),
      sorteioDaPopulacao(sementeNoLugarDaDoArquivo.value_or(dadosDoJogo.vilaInicial.sementeDaPartida), sequenciaDoSorteioDaPopulacao),
      sorteioDaViagem(sementeNoLugarDaDoArquivo.value_or(dadosDoJogo.vilaInicial.sementeDaPartida), sequenciaDoSorteioDaViagem),
      sorteioDoClimaDasOutrasRegioes(sementeNoLugarDaDoArquivo.value_or(dadosDoJogo.vilaInicial.sementeDaPartida), sequenciaDoSorteioDoClimaDasOutrasRegioes)
{
    // O clima do dia 1 (ficha do Clima, regra 4.1, passo 1): não sorteia.
    clima.climaDeHoje = dados.regrasDoClima.climaDoPrimeiroDia;
    clima.climaDeOntem = clima.climaDeHoje;
    guardarODiaDeHojeNoClima(clima, relogio);
    montarAViagemDaPartidaNova(viagem, dados, clima);
    aCaminhoPorItem.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
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
    populacao.proximoNumeroDeNpc = static_cast<int>(npcsDaVila.size());  // os números nunca se repetem
    ContextoDaProducao producao = contextoDaProducao();
    avisosAoMontar = montarAProducaoDaVila(producao, npcsDaVila);

    // Fase B1b (Outras cidades, regra 4.1): as outras cidades do mundo, em LOD 2, na
    // ordem do número do mapa. Elas sorteiam com as suas sequências: a vila não muda.
    std::vector<const CidadeDoMundo*> cidades;
    for (const CidadeDoMundo& cidade : dados.mundo.cidades) {
        if (!cidade.ehAVilaDoJogador && dados.mundo.mapaExiste(cidade.numeroDoMapa)) {
            cidades.push_back(&cidade);
        }
    }
    std::sort(cidades.begin(), cidades.end(), [](const CidadeDoMundo* uma, const CidadeDoMundo* outra) { return uma->numeroDoMapa < outra->numeroDoMapa; });
    for (const CidadeDoMundo* cidade : cidades) {
        outrasCidades.push_back(std::make_unique<SimulacaoDaVila>(dados, regrasDoTempo, *cidade, sementeNoLugarDaDoArquivo.value_or(dados.vilaInicial.sementeDaPartida)));
    }
}

SimulacaoDaVila::SimulacaoDaVila(const DadosDoJogo& dadosDoJogo, const RegrasDoTempo& regrasDoTempo, const CidadeDoMundo& cidade, uint64_t sementeDaPartida)
    : dados(dadosDoJogo), relogio(RelogioDoMundo::criarParaPartidaNova(regrasDoTempo)), vila(gerarAVilaDaCidade(cidade, dadosDoJogo, sementeDaPartida)),
      mapa(montarOMapaDaCidadeEmLod2(dadosDoJogo.mundo.mapa(cidade.numeroDoMapa), dadosDoJogo)), ticksPorHoraDeJogo(regrasDoTempo.ticksPorHoraDeJogo),
      sorteioDosNpcs(sementeDaPartida, sequenciaDoSorteioDaCidade(cidade.numeroDoMapa, 1)),
      sorteioDosEventos(sementeDaPartida, sequenciaDoSorteioDaCidade(cidade.numeroDoMapa, 2)),
      sorteioDoClima(sementeDaPartida, sequenciaDoSorteioDaCidade(cidade.numeroDoMapa, 3)),
      sorteioDaPopulacao(sementeDaPartida, sequenciaDoSorteioDaCidade(cidade.numeroDoMapa, 4)),
      sorteioDaViagem(sementeDaPartida, sequenciaDoSorteioDaCidade(cidade.numeroDoMapa, 5)),
      sorteioDoClimaDasOutrasRegioes(sementeDaPartida, sequenciaDoSorteioDaCidade(cidade.numeroDoMapa, 9))
{
    lodDoMapa = 2;
    numeroDoMapaDaCidade = cidade.numeroDoMapa;
    // A cidade não tem grupo de viagem; o inventário vazio dele vai no save como o da vila.
    viagem.grupoDoJogador.quantidadePorItem.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
    aCaminhoPorItem.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
    // Comércio, 4.1: 2 caravanas por Posto Comercial (4 pessoas, 4 Cavalos abstratos).
    if (vila.temPostoComercial) {
        for (int numero = 0; numero < dados.regrasDoComercio.caravanasPorPostoComercial; ++numero) {
            CaravanaDaCidade caravana;
            caravana.entidade.tipoDeEntidade = "caravana";
            caravana.entidade.pessoas = dados.regrasDoComercio.pessoasDaCaravana();
            caravana.entidade.mapaAtual = cidade.numeroDoMapa;
            caravana.entidade.quantidadePorItem.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
            caravana.marcadoNoDestino.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
            caravanas.push_back(caravana);
        }
    }
    clima.climaDeHoje = dados.regrasDoClima.climaDoPrimeiroDia;
    clima.climaDeOntem = clima.climaDeHoje;
    guardarODiaDeHojeNoClima(clima, relogio);

    // Os NPCs sem posição (OC1-2): o registro existe, no tile 0 do mapa sem desenho.
    for (std::size_t numero = 0; numero < vila.npcsIniciais.size(); ++numero) {
        const NpcDaVilaInicial& npcInicial = vila.npcsIniciais[numero];
        const entt::entity npc = criarNpcNoTile(registroDasEntidades, PosicaoDeTile{0, 0});
        registroDasEntidades.emplace<IdentidadeDoNpc>(npc, IdentidadeDoNpc{npcInicial.nome, npcInicial.classe, static_cast<int>(numero)});
        registroDasEntidades.emplace<CasaETrabalhoDoNpc>(npc, CasaETrabalhoDoNpc{npcInicial.indiceDaCasa, npcInicial.indiceDoTrabalho});
        DecisaoDoNpc decisao;
        decisao.indiceDaConstrucaoOndeEsta = npcInicial.indiceDaCasa;
        registroDasEntidades.emplace<DecisaoDoNpc>(npc, decisao);
        registroDasEntidades.emplace<IdadeDoNpc>(npc, IdadeDoNpc{npcInicial.idade});
        npcsDaVila.push_back(npc);
    }
    populacao.proximoNumeroDeNpc = static_cast<int>(npcsDaVila.size());
    ContextoDaProducao producao = contextoDaProducao();
    avisosAoMontar = montarAProducaoDaVila(producao, npcsDaVila);
    // Passos 4 e 7: a contratação da Cidade e a escolha das receitas rodam uma vez.
    preencherVagasDaCidade(producao);
    escolherOsGerentes(producao);
    escolherAsReceitasDaCidade(producao);
}

void SimulacaoDaVila::zerarAsContasDoDia()
{
    dinheiro.consumoNaoPagoHoje = 0;
    dinheiro.comidaQueFaltouHoje = 0;
    dinheiro.gastoDoJogadorComInsumosHoje = 0;
    for (ProducaoDaConstrucao& producaoDaConstrucao : producaoDasConstrucoes) {
        producaoDaConstrucao.producaoDeHojePorItem.clear();
    }
}

void SimulacaoDaVila::avancarUmTickEmLod2()
{
    ContextoDaProducao producao = contextoDaProducao();
    relogio.avancarUmTick();
    if (relogio.comecouUmaHoraNova()) {
        presumirOTrabalhoDaHora(producao, (relogio.horaDoDiaAtual() + 23) % 24);
        rodarOsCiclosDaHora(producao, relogio);
        obras.linhasParaOConsole.clear();
    }
    if (relogio.comecouUmDiaNovo()) {
        rodarAOrdemDoDia(producao, relogio);
        zerarAsContasDoDia();
    }
}

ContextoDaRotina SimulacaoDaVila::contextoDaRotina()
{
    ContextoDaRotina contexto{registroDasEntidades, mapa, vila, dados.regras.vidaDosNpcs, ticksPorHoraDeJogo, sorteioDosNpcs};
    contexto.producaoDasConstrucoes = &producaoDasConstrucoes;
    contexto.periodoLivreEmCasa = periodoLivreEmCasaHoje(clima, dados.regrasDoClima);  // a tempestade (ficha do Clima)
    return contexto;
}

ContextoDaProducao SimulacaoDaVila::contextoDaProducao()
{
    ContextoDaProducao contexto{registroDasEntidades, dados,    inventario, dinheiro,          producaoDasConstrucoes,
                                consumoMedido,        mercadoLocal, felicidade, eventos, sorteioDosEventos, vila, mapa, decisoes, obras, clima, sorteioDoClima, trabalho, populacao, sorteioDaPopulacao, npcsDaVila,
                                ticksPorHoraDeJogo};
    contexto.viagem = &viagem;
    contexto.sorteioDaViagem = &sorteioDaViagem;
    contexto.sorteioDoClimaDasOutrasRegioes = &sorteioDoClimaDasOutrasRegioes;
    contexto.relogio = &relogio;
    return contexto;
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
    // Fase B2: o comércio do grupo numa outra cidade precisa das outras cidades; é tratado aqui.
    std::vector<std::string> linhas;
    FilaDePedidosDoJogador outrosPedidos;
    for (PedidoDoJogador& pedido : pedidos.tomarTodos()) {
        if (const auto* comercio = std::get_if<PedidoComercioNaCidade>(&pedido)) {
            const std::string motivo = comerciarNaCidadeDoGrupo(*this, comercio->numeroDoItem, comercio->quantidade, comercio->comprar, linhas);
            if (!motivo.empty()) {
                linhas.push_back("Aviso: pedido recusado (" + motivo + ").");
            }
        } else {
            outrosPedidos.acrescentar(std::move(pedido));
        }
    }
    ContextoDaProducao producao = contextoDaProducao();
    const std::vector<std::string> linhasDosOutros = aplicarOsPedidosDoJogador(producao, outrosPedidos);
    linhas.insert(linhas.end(), linhasDosOutros.begin(), linhasDosOutros.end());
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
        // A viagem (fase B1): o clima das outras regiões e a comida do grupo. Usa os
        // seus próprios sorteios, então a vila não muda.
        const std::vector<std::string> linhasDaViagem = rodarAViagemDoDia(producao);
        linhasParaOConsole.insert(linhasParaOConsole.end(), linhasDaViagem.begin(), linhasDaViagem.end());
        linhasParaOConsole.push_back(montarResumoDoDia(producao, diaAntesDoTick));
        linhaDoRelatorioDoUltimoDia = montarLinhaDoRelatorio(producao, diaAntesDoTick, estacaoAntesDoTick, diaDaEstacaoAntesDoTick);
        zerarAsContasDoDia();
    }

    // O grupo do jogador em viagem (fase B1): o encontro pausa o jogo; a chegada volta a 1x.
    {
        const ResultadoDoTickDaViagem tickDaViagem = avancarAViagemUmTick(producao, relogio);
        linhasParaOConsole.insert(linhasParaOConsole.end(), tickDaViagem.linhasParaOConsole.begin(), tickDaViagem.linhasParaOConsole.end());
        if (tickDaViagem.encontroComecou || tickDaViagem.chegouAoDestino) {
            relogio.definirVelocidadeDoJogo(1);
        }
        if (tickDaViagem.encontroComecou) {
            relogio.alternarPausa();
        }
        if (tickDaViagem.mapaOndeChegou != 0) {
            anotarOsPrecosDaCidade(*this, tickDaViagem.mapaOndeChegou);  // Comércio, 4.8
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

    // 6. As outras cidades (fase B1b), depois da vila e na ordem do número do mapa. Cada
    //    uma usa o clima da sua região, que a viagem já sorteou na virada do dia.
    {
        MEDIR_ESTE_TRECHO("outras cidades");
        for (const std::unique_ptr<SimulacaoDaVila>& cidade : outrasCidades) {
            const int regiao = dados.mundo.mapa(cidade->numeroDoMapaDaCidade).regiao;
            cidade->clima.climaDeHojeVindoDaRegiao = viagem.climaDeHojePorRegiao[static_cast<std::size_t>(regiao - 1)];
            if (relogio.comecouUmDiaNovo()) {
                cidade->viagem.climaDeHojePorRegiao = viagem.climaDeHojePorRegiao;  // para a conta das caravanas dela
            }
            cidade->avancarUmTickEmLod2();
        }
    }

    // 7. O comércio (fase B2): no dia novo, depois de todas as cidades, cada caravana em casa
    //    decide se sai (o passo 3b); a cada tick, as caravanas andam.
    if (lodDoMapa == 0 && !outrasCidades.empty()) {
        MEDIR_ESTE_TRECHO("caravanas");
        if (relogio.comecouUmDiaNovo()) {
            decidirAsViagensDasCaravanas(*this);
        }
        avancarAsCaravanasUmTick(*this);
    }
    registrarNasNotificacoes(linhasParaOConsole, linhasJaNasNotificacoes);
    MARCAR_O_FIM_DE_UM_TICK();
    return linhasParaOConsole;
}
