#include <nlohmann/json.hpp>

#include "Save/AcessoAoEstadoParaOSave.h"
#include "Save/ArquivoDeSave.h"

// Regra 4.1 da ficha do Save: monta a árvore de seções, sempre na mesma
// ordem e com as listas na mesma ordem, e converte em MessagePack. Os mapas do
// JSON ficam em ordem alfabética, então a mesma partida dá sempre os mesmos bytes.

namespace {

using nlohmann::json;
using Acesso = AcessoAoEstadoParaOSave;

json escreverOTile(const PosicaoDeTile& tile) { return json::array({tile.tileX, tile.tileY}); }

json escreverOLugar(const LugarDoNpc& lugar)
{
    return json{{"atual", escreverOTile(lugar.tileAtual)},
                {"proximo", escreverOTile(lugar.proximoTile)},
                {"progresso", lugar.progressoAteOProximoTileEmMilesimos}};
}

const std::string& textoDoItem(const DadosDoJogo& dados, int numeroDoItem)
{
    return dados.catalogoDeItens.itemPeloNumero(numeroDoItem).identificadorDeTexto;
}

// -1 (nenhuma receita) vira null.
json textoDaReceita(const DadosDoJogo& dados, int indiceDaReceita)
{
    if (indiceDaReceita < 0 || indiceDaReceita >= dados.catalogoDeReceitas.quantidadeDeReceitas()) {
        return nullptr;
    }
    return dados.catalogoDeReceitas.todasAsReceitas()[static_cast<std::size_t>(indiceDaReceita)].identificadorDeTexto;
}

json escreverAsOrdens(const DadosDoJogo& dados, const std::vector<OrdemDoJogador>& ordens)
{
    json lista = json::array();
    for (const OrdemDoJogador& ordem : ordens) {
        lista.push_back(json{{"tipo", static_cast<int>(ordem.tipo)},
                             {"item", textoDoItem(dados, ordem.numeroDoItem)},
                             {"quantidade", ordem.quantidade},
                             {"feita", ordem.quantidadeJaFeita}});
    }
    return lista;
}

json escreverOSorteio(const SorteioComSemente& sorteio)
{
    return json{{"estado", Acesso::estadoDoGerador(sorteio)}, {"incremento", Acesso::incrementoDaSequencia(sorteio)}};
}

json escreverAVila(const SimulacaoDaVila& simulacao)
{
    json lista = json::array();
    for (const ConstrucaoDaVila& construcao : simulacao.vila.construcoes) {
        const RetanguloEmTiles& area = construcao.areaDoLote;
        lista.push_back(json{{"identificador", construcao.identificador},
                             {"tipo", construcao.tipo},
                             {"lote", construcao.numeroDoLote},
                             {"donoEhOJogador", construcao.donoEhOJogador},
                             {"area", json::array({area.tileX, area.tileY, area.larguraEmTiles, area.alturaEmTiles})},
                             {"porta", escreverOTile(construcao.tileDaPorta)},
                             {"emObra", construcao.emObra},
                             {"nivel", construcao.nivel},
                             {"ordens", escreverAsOrdens(simulacao.dados, construcao.ordensDoJogador)}});
    }
    return lista;
}

json escreverAProducao(const SimulacaoDaVila& simulacao)
{
    const DadosDoJogo& dados = simulacao.dados;
    json lista = json::array();
    for (const ProducaoDaConstrucao& producao : simulacao.producaoDasConstrucoes) {
        json tilesDoCampo = json::array();
        for (const PosicaoDeTile& tile : producao.tilesDeTrabalhoNoCampo) {
            tilesDoCampo.push_back(tile.tileX);
            tilesDoCampo.push_back(tile.tileY);
        }
        json producaoDeHoje = json::object();
        for (const auto& [numeroDoItem, quantidade] : producao.producaoDeHojePorItem) {
            producaoDeHoje[textoDoItem(dados, numeroDoItem)] = quantidade;
        }
        lista.push_back(json{{"ehDeProducao", producao.ehDeProducao},
                             {"receitaDoCiclo", textoDaReceita(dados, producao.receitaDoCiclo)},
                             {"receitaEscolhida", textoDaReceita(dados, producao.receitaEscolhidaParaOProximoCiclo)},
                             {"cicloComecou", producao.cicloComecou},
                             {"progressoDoCiclo", producao.progressoDoCiclo},
                             {"pontosDePesquisaGuardados", producao.pontosDePesquisaGuardados},
                             {"restoDaProducaoBracal", producao.restoDaProducaoBracal},
                             {"estaParada", producao.estaParada},
                             {"motivoDaParada", producao.motivoDaParada},
                             {"estaDeFolgaHoje", producao.estaDeFolgaHoje},
                             {"ordens", escreverAsOrdens(dados, producao.ordens)},
                             {"nivelDeSalario", static_cast<int>(producao.nivelDeSalario)},
                             {"diasSeguidosComSalarioBaixo", producao.diasSeguidosComSalarioBaixo},
                             {"manutencaoAtrasada", producao.manutencaoAtrasada},
                             {"emObra", producao.emObra},
                             {"gerente", producao.numeroDoGerente},
                             {"diasDeSalarioAtrasado", producao.diasDeSalarioAtrasado},
                             {"diasSemEmpregados", producao.diasSemEmpregados},
                             {"modoAutomatico", producao.modoAutomatico},
                             {"salarioDevido", producao.salarioDevidoEmDecimos},
                             {"areaDoCampo", producao.areaDoCampoNoMapa},
                             {"tilesDoCampo", tilesDoCampo},
                             {"producaoDeHoje", producaoDeHoje}});
    }
    return lista;
}

json escreverAsObras(const SimulacaoDaVila& simulacao)
{
    json lista = json::array();
    for (const ObraNaFila& obra : simulacao.obras.fila) {
        lista.push_back(json{{"tipo", static_cast<int>(obra.tipo)},
                             {"construcao", obra.indiceDaConstrucao},
                             {"nivelDeDestino", obra.nivelDeDestino},
                             {"centesimosDeHoraQueFaltam", obra.centesimosDeHoraQueFaltam},
                             {"esperaMaterial", obra.esperaMaterial}});
    }
    return lista;
}

json escreverOInventario(const SimulacaoDaVila& simulacao)
{
    const DadosDoJogo& dados = simulacao.dados;
    json cidade = json::object();
    json jogador = json::object();
    for (int numeroDoItem = 0; numeroDoItem < dados.catalogoDeItens.quantidadeDeItens(); ++numeroDoItem) {
        cidade[textoDoItem(dados, numeroDoItem)] = simulacao.inventario.quantidadeDe(numeroDoItem, DonoDoItem::Cidade);
        jogador[textoDoItem(dados, numeroDoItem)] = simulacao.inventario.quantidadeDe(numeroDoItem, DonoDoItem::Jogador);
    }
    return json{{"cidade", cidade}, {"jogador", jogador}};
}

json escreverODinheiro(const DinheiroDaVila& dinheiro)
{
    return json{{"bancoDaCidade", dinheiro.bancoDaCidade},
                {"dinheiroDoJogador", dinheiro.dinheiroDoJogador},
                {"dinheiroDaClasseCivil", dinheiro.dinheiroDaClasseCivil},
                {"quantidadeDeCivis", dinheiro.quantidadeDeCivis},
                {"rendaDoDiaDaClasseCivil", dinheiro.rendaDoDiaDaClasseCivil},
                {"diasEmDeficitDeComida", dinheiro.diasEmDeficitDeComida},
                {"diasEmDeficitDeMateriais", dinheiro.diasEmDeficitDeMateriais},
                {"materiaisDevidosEmOitavos", dinheiro.materiaisDevidosEmOitavos},
                {"bancoNoComecoDaEstacao", dinheiro.bancoNoComecoDaEstacao},
                {"ajudaDoEstabilizadorNaEstacao", dinheiro.ajudaDoEstabilizadorNaEstacao},
                {"dinheiroQueEntrouDeFora", dinheiro.dinheiroQueEntrouDeFora},
                // As contas do dia: se o save for no meio do dia, o fim do dia continua igual.
                {"consumoNaoPagoHoje", dinheiro.consumoNaoPagoHoje},
                {"comidaQueFaltouHoje", dinheiro.comidaQueFaltouHoje},
                {"gastoDoJogadorComInsumosHoje", dinheiro.gastoDoJogadorComInsumosHoje},
                {"gastoDaCestaDeHoje", dinheiro.gastoDaCestaDeHoje},
                {"gastoExtraDeHoje", dinheiro.gastoExtraDeHoje},
                {"impostoDeHoje", dinheiro.impostoDeHoje},
                {"ajudaDoEstabilizadorHoje", dinheiro.ajudaDoEstabilizadorHoje}};
}

json escreverOMercado(const SimulacaoDaVila& simulacao)
{
    json precos = json::object();
    for (std::size_t numeroDoItem = 0; numeroDoItem < simulacao.mercadoLocal.precoLocalPorItemEmDecimos.size(); ++numeroDoItem) {
        precos[textoDoItem(simulacao.dados, static_cast<int>(numeroDoItem))] = simulacao.mercadoLocal.precoLocalPorItemEmDecimos[numeroDoItem];
    }
    return json{{"precos", precos}};
}

json escreverOConsumoMedido(const SimulacaoDaVila& simulacao)
{
    const DadosDoJogo& dados = simulacao.dados;
    const auto& ultimosDias = Acesso::consumoDosUltimosDiasPorItem(simulacao.consumoMedido);
    const auto& deHoje = Acesso::consumoDeHojePorItem(simulacao.consumoMedido);
    json dias = json::object();
    json hoje = json::object();
    json sobra = json::object();
    for (std::size_t numeroDoItem = 0; numeroDoItem < ultimosDias.size(); ++numeroDoItem) {
        const std::string& texto = textoDoItem(dados, static_cast<int>(numeroDoItem));
        dias[texto] = json(ultimosDias[numeroDoItem]);
        hoje[texto] = deHoje[numeroDoItem];
        sobra[texto] = static_cast<bool>(simulacao.consumoMedido.produtoEstaComEstoqueDeSobra[numeroDoItem]);
    }
    return json{{"ultimosDias", dias},
                {"hoje", hoje},
                {"sobra", sobra},
                {"posicaoDoDiaMaisAntigo", Acesso::posicaoDoDiaMaisAntigo(simulacao.consumoMedido)}};
}

json escreverAFelicidade(const FelicidadeDaClasse& felicidade)
{
    return json{{"felicidade", felicidade.felicidadeEmDecimos},
                {"alvo", felicidade.alvoEmDecimos},
                {"consumoAtendido", felicidade.fatoresDoDia.consumoAtendido},
                {"desempregoERenda", felicidade.fatoresDoDia.desempregoERenda},
                {"moradia", felicidade.fatoresDoDia.moradia},
                {"eventosDuradouros", felicidade.fatoresDoDia.eventosDuradouros},
                {"leisECultura", felicidade.fatoresDoDia.leisECultura},
                {"criminalidade", felicidade.criminalidadeEmDecimos},
                {"diaDoProximoFurtoPossivel", felicidade.diaDoProximoFurtoPossivel}};
}

json escreverOsEventos(const SimulacaoDaVila& simulacao)
{
    const std::vector<DefinicaoDeEvento>& definicoes = simulacao.dados.catalogoDeEventos.todosOsEventos();
    json ativos = json::array();
    for (const EventoAtivo& ativo : simulacao.eventos.eventosAtivos) {
        ativos.push_back(json{{"evento", definicoes[static_cast<std::size_t>(ativo.indiceDoEvento)].identificadorDeTexto}, {"dias", ativo.diasQueFaltam}});
    }
    json cooldown = json::object();
    json diasNaCondicao = json::object();
    for (std::size_t indice = 0; indice < definicoes.size(); ++indice) {
        cooldown[definicoes[indice].identificadorDeTexto] = simulacao.eventos.diaEmQuePodeSairDeNovo[indice];
        diasNaCondicao[definicoes[indice].identificadorDeTexto] = simulacao.eventos.diasSeguidosNaCondicao[indice];
    }
    return json{{"ativos", ativos},
                {"cooldown", cooldown},
                {"diasSeguidosNaCondicao", diasNaCondicao},
                {"mudancasDeHoje", simulacao.eventos.mudancasDeHoje},
                {"linhasParaOConsole", simulacao.eventos.linhasParaOConsole}};
}

json escreverAsDecisoes(const SimulacaoDaVila& simulacao)
{
    json precos = json::object();
    for (const auto& [numeroDoItem, porcentagem] : simulacao.decisoes.porcentagemDoPrecoPorItem) {
        precos[textoDoItem(simulacao.dados, numeroDoItem)] = porcentagem;
    }
    return json{{"porcentagemDoPreco", precos}};
}

json escreverOsNpcs(const SimulacaoDaVila& simulacao)
{
    const entt::registry& registro = simulacao.registroDasEntidades;
    json lista = json::array();
    for (const entt::entity npc : simulacao.npcsDaVila) {
        const IdentidadeDoNpc& identidade = registro.get<IdentidadeDoNpc>(npc);
        const CasaETrabalhoDoNpc& casaETrabalho = registro.get<CasaETrabalhoDoNpc>(npc);
        const DecisaoDoNpc& decisao = registro.get<DecisaoDoNpc>(npc);
        const PosicaoDoNpc& posicao = registro.get<PosicaoDoNpc>(npc);
        json registroDoNpc = json{{"entidade", static_cast<uint32_t>(entt::to_integral(npc))},
                                  {"nome", identidade.nome},
                                  {"classe", identidade.classe},
                                  {"cultura", identidade.cultura},
                                  {"numero", identidade.numeroDoNpc},
                                  {"casa", casaETrabalho.indiceDaCasa},
                                  {"trabalho", casaETrabalho.indiceDoTrabalho},
                                  {"saiuPorAtrasoDe", casaETrabalho.saiuPorAtrasoDe},
                                  {"lugar", escreverOLugar(posicao.lugarAtual)},
                                  {"olhar", static_cast<int>(posicao.direcaoDoOlhar)},
                                  {"acao", static_cast<int>(decisao.acaoAtual)},
                                  {"ondeEsta", decisao.indiceDaConstrucaoOndeEsta},
                                  {"destino", decisao.indiceDaConstrucaoDoDestino},
                                  {"aCaminho", decisao.estaACaminho},
                                  {"esperaAcabaNoTick", decisao.tickEmQueAEsperaAcaba},
                                  {"diaDaUltimaCompra", decisao.diaDaUltimaCompra}};
        if (const HabilidadesDoNpc* habilidades = registro.try_get<HabilidadesDoNpc>(npc)) {
            registroDoNpc["habilidades"] = json::array({habilidades->bracal, habilidades->manufaturado});
        }
        if (const IdadeDoNpc* idade = registro.try_get<IdadeDoNpc>(npc)) {
            registroDoNpc["idade"] = idade->anos;
        }
        if (const TrabalhoDoDia* trabalho = registro.try_get<TrabalhoDoDia>(npc)) {
            registroDoNpc["trabalhoDoDia"] = json::array({trabalho->ticksTrabalhadosNestaHora, trabalho->ticksTrabalhadosHoje});
        }
        if (const DoencaDoNpc* doenca = registro.try_get<DoencaDoNpc>(npc)) {
            registroDoNpc["doenca"] = doenca->diasQueFaltam;
        }
        if (const CaminhoDoNpc* caminho = registro.try_get<CaminhoDoNpc>(npc)) {
            json tiles = json::array();
            for (const PosicaoDeTile& tile : caminho->tilesDoCaminho) {
                tiles.push_back(tile.tileX);
                tiles.push_back(tile.tileY);
            }
            registroDoNpc["caminho"] = json{{"tiles", tiles},
                                            {"indice", static_cast<uint64_t>(caminho->indiceDoProximoTileDoCaminho)},
                                            {"destino", escreverOTile(caminho->tileDeDestino)}};
        }
        if (const PedidoDeCaminho* pedido = registro.try_get<PedidoDeCaminho>(npc)) {
            registroDoNpc["pedidoDeCaminho"] = json{{"destino", escreverOTile(pedido->tileDeDestino)}, {"tick", pedido->tickDoPedido}};
        }
        lista.push_back(registroDoNpc);
    }
    return lista;
}

const char* nomeDoEncontro(TipoDeEncontro tipo)
{
    return tipo == TipoDeEncontro::Bandidos ? "bandidos" : tipo == TipoDeEncontro::MercadorViajante ? "mercador_viajante" : tipo == TipoDeEncontro::Cacadores ? "cacadores" : "nenhum";
}

// Fase B1 (ficha da Viagem): o grupo do jogador, a névoa e o clima de cada região.
// Uma entidade em trânsito (o grupo do jogador, fase B1; as caravanas, fase B2).
json escreverAEntidade(const DadosDoJogo& dados, const EntidadeEmTransito& entidade)
{
    json itens = json::object();
    for (int numeroDoItem = 0; numeroDoItem < static_cast<int>(entidade.quantidadePorItem.size()); ++numeroDoItem) {
        itens[textoDoItem(dados, numeroDoItem)] = entidade.quantidadePorItem[numeroDoItem];
    }
    return json{{"tipoDeEntidade", entidade.tipoDeEntidade},
                {"emTransito", entidade.emTransito},
                {"mapaAtual", entidade.mapaAtual},
                {"proximoMapa", entidade.proximoMapa},
                {"rota", entidade.rota},
                {"ticksDoTrecho", entidade.ticksDoTrecho},
                {"ticksAndadosNoTrecho", entidade.ticksAndadosNoTrecho},
                {"pessoas", entidade.pessoas},
                {"montados", entidade.montados},
                {"cargaAcimaDoLimite", entidade.cargaAcimaDoLimite},
                {"marchaForcada", entidade.marchaForcada},
                {"noitesSemDormir", entidade.noitesSemDormir},
                {"ticksSemDormir", entidade.ticksSemDormir},
                {"encontroMarcado", nomeDoEncontro(entidade.encontroMarcado.tipo)},
                {"bandidos", entidade.encontroMarcado.bandidos},
                {"encontroAcontecendo", entidade.encontroAcontecendo},
                {"ticksParadoNaLuta", entidade.ticksParadoNaLuta},
                {"itens", itens},
                {"dinheiro", entidade.dinheiroEmDecimos}};
}

json escreverOsItens(const DadosDoJogo& dados, const std::vector<int>& quantidadePorItem)
{
    json itens = json::object();
    for (int numeroDoItem = 0; numeroDoItem < static_cast<int>(quantidadePorItem.size()); ++numeroDoItem) {
        itens[textoDoItem(dados, numeroDoItem)] = quantidadePorItem[numeroDoItem];
    }
    return itens;
}

// Fase B1 (ficha da Viagem): o grupo do jogador, a névoa e o clima de cada região; fase B2:
// os preços que o jogador conhece.
json escreverAViagem(const SimulacaoDaVila& simulacao)
{
    json nevoa = json::array();
    for (const RegistroDaNevoa& registro : simulacao.viagem.nevoa) {
        nevoa.push_back(json::array({registro.diaEmQueFoiVisto, registro.dono, registro.nivelDaCidade, static_cast<int>(registro.estado)}));
    }
    json climas = json::array();
    for (const TipoDeClima clima : simulacao.viagem.climaDeHojePorRegiao) {
        climas.push_back(nomeDoClimaNoArquivo(clima));
    }
    json precos = json::array();
    for (const PrecosConhecidos& daCidade : simulacao.viagem.precosConhecidos) {
        precos.push_back(json::array({daCidade.diaEmQueFoiVisto, daCidade.precoPorItemEmDecimos}));
    }
    return json{{"grupo", escreverAEntidade(simulacao.dados, simulacao.viagem.grupoDoJogador)},
                {"nevoa", nevoa},
                {"climaDeHojePorRegiao", climas},
                {"ameacaBandidaPorRegiao", simulacao.viagem.ameacaBandidaPorRegiao},
                {"precosConhecidos", precos}};
}

// Fase B2 (ficha do Comércio): as caravanas da cidade e o que já vai a caminho dela.
json escreverOComercio(const SimulacaoDaVila& simulacao)
{
    json caravanas = json::array();
    for (const CaravanaDaCidade& caravana : simulacao.caravanas) {
        caravanas.push_back(json{{"entidade", escreverAEntidade(simulacao.dados, caravana.entidade)},
                                 {"situacao", static_cast<int>(caravana.situacao)},
                                 {"destino", caravana.mapaDeDestino},
                                 {"marcadoNoDestino", escreverOsItens(simulacao.dados, caravana.marcadoNoDestino)},
                                 {"tickDaPartida", caravana.tickDaPartida}});
    }
    return json{{"aCaminhoPorItem", escreverOsItens(simulacao.dados, simulacao.aCaminhoPorItem)}, {"caravanas", caravanas}};
}

// Fase B3 (ficha das Notícias): as notícias ativas, o que o jogador sabe e a Crônica.
json escreverAsNoticias(const SimulacaoDaVila& simulacao)
{
    const EstadoDasNoticias& noticias = simulacao.noticias;
    json ativas = json::array();
    for (const Noticia& noticia : noticias.ativas) {
        ativas.push_back(json{{"numero", noticia.numeroDaNoticia},
                              {"tipo", noticia.tipo},
                              {"importancia", static_cast<int>(noticia.importancia)},
                              {"mapaDeOrigem", noticia.mapaDeOrigem},
                              {"tickDeOrigem", noticia.tickDeOrigem},
                              {"diaDeOrigem", noticia.diaDeOrigem},
                              {"titulo", noticia.titulo},
                              {"numeroDoAcontecimento", noticia.numero},
                              {"rotuloDoNumero", noticia.rotuloDoNumero},
                              {"porQue", noticia.porQue},
                              {"diaEmQueSaiDoQuadro", noticia.diaEmQueSaiDoQuadro},
                              {"item", noticia.numeroDoItem >= 0 ? json(textoDoItem(simulacao.dados, noticia.numeroDoItem)) : json()},
                              {"preco", noticia.precoEmDecimos}});
    }
    json sabidas = json::array();
    for (const NoticiaQueOJogadorSabe& sabida : noticias.noticiasQueOJogadorSabe) {
        sabidas.push_back(json::array({sabida.numeroDaNoticia, sabida.diaEmQueSoube, sabida.distancia}));
    }
    json cronica = json::array();
    for (const LinhaDaCronica& linha : noticias.cronicaDoMundo) {
        cronica.push_back(json::array({linha.numeroDaNoticia, linha.diaDoAcontecimento, linha.diaEmQueSoube, linha.texto}));
    }
    json precos = json::array();
    for (const std::vector<int64_t>& doMapa : noticias.diaDaUltimaNoticiaDePreco) {
        json porItem = json::object();
        for (int numeroDoItem = 0; numeroDoItem < static_cast<int>(doMapa.size()); ++numeroDoItem) {
            if (doMapa[static_cast<std::size_t>(numeroDoItem)] >= 0) {
                porItem[textoDoItem(simulacao.dados, numeroDoItem)] = doMapa[static_cast<std::size_t>(numeroDoItem)];
            }
        }
        precos.push_back(std::move(porItem));
    }
    return json{{"ativas", ativas},
                {"proximoNumero", noticias.proximoNumeroDaNoticia},
                {"sabidas", sabidas},
                {"cronica", cronica},
                {"diaDaUltimaNoticiaDePreco", precos},
                {"mapaDoJogadorNaUltimaConferencia", noticias.mapaDoJogadorNaUltimaConferencia}};
}

// Fase B4 (ficha da Reputação): um registro por dono, com o nome dele.
json escreverAReputacao(const SimulacaoDaVila& simulacao)
{
    const std::vector<std::string> donos = donosDaReputacao(simulacao.dados.mundo);
    json porDono = json::array();
    for (std::size_t dono = 0; dono < simulacao.reputacao.reputacaoPorFaccao.size(); ++dono) {
        json mudancas = json::array();
        for (const MudancaDaReputacao& mudanca : simulacao.reputacao.ultimasMudancas[dono]) {
            mudancas.push_back(json::array({mudanca.valor, mudanca.motivo, mudanca.dia}));
        }
        porDono.push_back(json{{"dono", dono < donos.size() ? donos[dono] : std::string()},
                               {"valor", simulacao.reputacao.reputacaoPorFaccao[dono]},
                               {"base", simulacao.reputacao.basePorFaccao[dono]},
                               {"ultimasMudancas", mudancas}});
    }
    return porDono;
}

// Fase B5 (ficha da Tecnologia): por pesquisador, as tecnologias pelo id; os marcos pelo id.
json escreverATecnologia(const SimulacaoDaVila& simulacao)
{
    const EstadoDaTecnologia& estado = simulacao.tecnologia;
    const CatalogoDaTecnologia& catalogo = simulacao.dados.tecnologia;
    json pesquisadores = json::array();
    for (std::size_t pesquisador = 0; pesquisador < estado.temATecnologia.size(); ++pesquisador) {
        json tem = json::array();
        for (std::size_t tecnologia = 0; tecnologia < catalogo.tecnologias.size(); ++tecnologia) {
            if (estado.temATecnologia[pesquisador][tecnologia]) {
                tem.push_back(catalogo.tecnologias[tecnologia].identificadorDeTexto);
            }
        }
        const int projeto = estado.projetoAtual[pesquisador];
        pesquisadores.push_back(json{{"tecnologias", tem},
                                     {"projeto", projeto >= 0 ? json(catalogo.tecnologias[static_cast<std::size_t>(projeto)].identificadorDeTexto) : json()},
                                     {"pontos", estado.pontosAcumulados[pesquisador]}});
    }
    json marcos = json::array();
    for (std::size_t marco = 0; marco < catalogo.marcos.size(); ++marco) {
        marcos.push_back(json{{"id", catalogo.marcos[marco].identificadorDeTexto}, {"diaSorteado", estado.diaSorteadoPorMarco[marco]}, {"chegou", static_cast<bool>(estado.marcoChegou[marco])}});
    }
    return json{{"pesquisadores", pesquisadores}, {"marcos", marcos}, {"eraAtualDoMundo", estado.eraAtualDoMundo}};
}

// Fase C1 (ficha da Facção): as leis pelo nome da opção, o Banco, o líder e a votação perdida.
json escreverAsFaccoes(const SimulacaoDaVila& simulacao)
{
    const CatalogoDasFaccoes& catalogo = simulacao.dados.faccoes;
    const EstadoDasFaccoes& estado = simulacao.faccoes;
    json faccoes = json::array();
    for (std::size_t faccao = 0; faccao < estado.leisAtuais.size(); ++faccao) {
        json leis = json::object();
        for (std::size_t lei = 0; lei < catalogo.leis.size(); ++lei) {
            leis[catalogo.leis[lei].id] = catalogo.leis[lei].opcoes[static_cast<std::size_t>(estado.leisAtuais[faccao][lei])];
        }
        const LiderDaFaccao& lider = estado.lideres[faccao];
        faccoes.push_back(json{{"id", catalogo.faccoes[faccao].id},
                               {"leis", leis},
                               {"banco", estado.bancoDaFaccao[faccao]},
                               {"lider", json::array({lider.idade, lider.agressividade, lider.sociabilidade, lider.legitimidade})}});
    }
    json mudancas = json::array();
    for (const MudancaDeLei& mudanca : estado.mudancasParaAmanha) {
        mudancas.push_back(json::array({mudanca.faccao, mudanca.lei, mudanca.valor}));
    }
    return json{{"faccoes", faccoes}, {"mudancasParaAmanha", mudancas}, {"perdeuAVotacaoAte", estado.perdeuAVotacaoAte}, {"cunhagemTotal", estado.cunhagemTotal}};
}

// Fase C3 (ficha do Crime e Justiça): o crime do jogador.
json escreverOCrime(const SimulacaoDaVila& simulacao)
{
    const EstadoDoCrimeDoJogador& crime = simulacao.crimeDoJogador;
    const RevistaPendente& revista = crime.revista;
    return json{{"recompensaPorFaccao", crime.recompensaPorFaccao},
                {"gravidadesNaFicha", crime.gravidadesNaFicha},
                {"fiancaNaFicha", crime.fiancaNaFicha},
                {"estacaoDaUltimaHeresia", crime.estacaoDaUltimaHeresia},
                {"presoAteODia", crime.presoAteODia},
                {"faccaoDaPrisao", crime.faccaoDaPrisao},
                {"faccaoDosCacadores", crime.faccaoDosCacadores},
                {"proximoMapaNaUltimaConferencia", crime.proximoMapaNaUltimaConferencia},
                {"revista", json{{"ativa", revista.ativa}, {"faccao", revista.faccao}, {"mapa", revista.mapa}, {"mapaAnterior", revista.mapaAnterior},
                                 {"crimes", revista.crimes}, {"gravidade", revista.gravidade}, {"multa", revista.multa}}}};
}

json escreverOsDiario(const SimulacaoDaVila& simulacao)
{
    json lista = json::array();
    for (const Notificacao& notificacao : simulacao.notificacoes.doDiario()) {
        lista.push_back(json::array({static_cast<int>(notificacao.nivel), notificacao.texto, notificacao.diaDaPartida, notificacao.tickDeEntrada}));
    }
    return lista;
}

}  // namespace

std::string escreverADataDoJogoParaOSave(const RelogioDoMundo& relogio)
{
    static const char* const nomesDasEstacoes[] = {"da Primavera", "do Verão", "do Outono", "do Inverno"};
    return "Dia " + std::to_string(relogio.diaDaEstacaoAtual()) + " " + nomesDasEstacoes[static_cast<int>(relogio.estacaoAtual())] + ", Ano " +
           std::to_string(relogio.anoAtual()) + ", " + std::to_string(relogio.horaDoDiaAtual()) + "h";
}

// Tudo o que uma simulação guarda, menos o cabeçalho e o relógio: a vila e, desde a
// fase B1b, cada uma das outras cidades (Outras cidades, parte 2).
json escreverASimulacao(const SimulacaoDaVila& simulacao)
{
    json arvore = json::object();
    arvore["sorteios"] = json{{"npcs", escreverOSorteio(Acesso::sorteioDosNpcs(simulacao))}, {"eventos", escreverOSorteio(Acesso::sorteioDosEventos(simulacao))},
                              {"clima", escreverOSorteio(Acesso::sorteioDoClima(simulacao))},
                              {"populacao", escreverOSorteio(Acesso::sorteioDaPopulacao(simulacao))},
                              {"viagem", escreverOSorteio(Acesso::sorteioDaViagem(simulacao))},
                              {"climaDasOutrasRegioes", escreverOSorteio(Acesso::sorteioDoClimaDasOutrasRegioes(simulacao))}};
    arvore["populacao"] = json{{"acumulado", simulacao.populacao.acumuladoDeCrescimentoEmPessoas},
                               {"proximoNumeroDeNpc", simulacao.populacao.proximoNumeroDeNpc},
                               {"diasDeFomeNaEstacao", simulacao.populacao.diasDeFomeNaEstacao}};
    // Fase A1 (ficha do Clima): o clima de hoje, a lenha devida e os dias em déficit.
    arvore["leilao"] = json{{"construcao", simulacao.trabalho.leilao.indiceDaConstrucao},
                            {"diasQueFaltam", simulacao.trabalho.leilao.diasQueFaltam},
                            {"maiorLance", simulacao.trabalho.leilao.maiorLanceEmDecimos},
                            {"lanceEhDoJogador", simulacao.trabalho.leilao.lanceEhDoJogador}};
    arvore["clima"] = json{{"climaDeHoje", nomeDoClimaNoArquivo(simulacao.clima.climaDeHoje)},
                           {"climaDeOntem", nomeDoClimaNoArquivo(simulacao.clima.climaDeOntem)},
                           {"lenhaDevidaEmCentesimos", simulacao.clima.lenhaDevidaEmCentesimos},
                           {"diasEmDeficitDeLenha", simulacao.clima.diasEmDeficitDeLenha}};
    arvore["vila"] = escreverAVila(simulacao);
    arvore["producao"] = escreverAProducao(simulacao);
    arvore["obras"] = escreverAsObras(simulacao);
    arvore["inventario"] = escreverOInventario(simulacao);
    arvore["dinheiro"] = escreverODinheiro(simulacao.dinheiro);
    arvore["mercado"] = escreverOMercado(simulacao);
    arvore["consumoMedido"] = escreverOConsumoMedido(simulacao);
    arvore["felicidade"] = escreverAFelicidade(simulacao.felicidade);
    arvore["eventos"] = escreverOsEventos(simulacao);
    arvore["decisoes"] = escreverAsDecisoes(simulacao);
    arvore["npcs"] = escreverOsNpcs(simulacao);
    arvore["diario"] = escreverOsDiario(simulacao);
    arvore["viagem"] = escreverAViagem(simulacao);
    arvore["comercio"] = escreverOComercio(simulacao);
    return arvore;
}

std::vector<uint8_t> gravarAPartidaEmBytes(const SimulacaoDaVila& simulacao, const std::string& nomeDoEspaco, const std::string& dataReal)
{
    json arvore = escreverASimulacao(simulacao);
    arvore["cabecalho"] = json{{"versaoDoFormato", versaoDoFormatoDoSave},
                               {"versaoDoJogo", versaoDoJogoNoSave},
                               {"dataReal", dataReal},
                               {"dataDoJogo", escreverADataDoJogoParaOSave(simulacao.relogio)},
                               {"nomeDoEspaco", nomeDoEspaco},
                               {"tick", simulacao.relogio.tickAtual()}};
    arvore["relogio"] = json{{"tick", Acesso::tickAtualDaPartida(simulacao.relogio)},
                             {"velocidade", Acesso::velocidadeDoJogo(simulacao.relogio)},
                             {"velocidadeAntesDaPausa", Acesso::velocidadeAntesDaPausa(simulacao.relogio)}};
    json cidades = json::array();
    for (const auto& cidade : simulacao.outrasCidades) {
        json secao = escreverASimulacao(*cidade);
        secao["numeroDoMapa"] = cidade->numeroDoMapaDaCidade;
        cidades.push_back(std::move(secao));
    }
    arvore["outrasCidades"] = std::move(cidades);
    arvore["noticias"] = escreverAsNoticias(simulacao);
    arvore["reputacao"] = escreverAReputacao(simulacao);
    arvore["tecnologia"] = escreverATecnologia(simulacao);
    arvore["faccoes"] = escreverAsFaccoes(simulacao);
    arvore["crime"] = escreverOCrime(simulacao);
    return json::to_msgpack(arvore);
}
