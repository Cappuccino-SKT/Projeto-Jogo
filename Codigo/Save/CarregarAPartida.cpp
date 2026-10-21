#include <stdexcept>

#include <nlohmann/json.hpp>

#include "Save/AcessoAoEstadoParaOSave.h"
#include "Save/ArquivoDeSave.h"

// Regra 4.2 da ficha do Save: lê a árvore, confere a versão e cada ID em texto,
// monta uma SimulacaoDaVila nova e preenche cada parte pela seção dela. A
// partida antiga nunca é tocada: quem chama só a troca se tudo deu certo (FL9-5:
// um save de outra versão ou com dados que mudaram é recusado, nada é adivinhado).

namespace {

using nlohmann::json;
using Acesso = AcessoAoEstadoParaOSave;

// Um motivo de recusa que vai direto para a mensagem ao jogador.
class RecusaDoSave : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

PosicaoDeTile lerOTile(const json& tile) { return PosicaoDeTile{tile.at(0).get<int>(), tile.at(1).get<int>()}; }

LugarDoNpc lerOLugar(const json& lugar)
{
    LugarDoNpc resultado;
    resultado.tileAtual = lerOTile(lugar.at("atual"));
    resultado.proximoTile = lerOTile(lugar.at("proximo"));
    resultado.progressoAteOProximoTileEmMilesimos = lugar.at("progresso").get<int>();
    return resultado;
}

// O número do item que tem este ID em texto; um ID que sumiu dos dados recusa o save.
int numeroDoItemPeloTexto(const DadosDoJogo& dados, const std::string& identificador)
{
    const std::optional<int> numero = dados.catalogoDeItens.procurarNumeroDoItem(identificador);
    if (!numero.has_value()) {
        throw RecusaDoSave("O save usa o " + identificador + ", que não existe mais.");
    }
    return *numero;
}

int indiceDaReceitaPeloTexto(const DadosDoJogo& dados, const json& identificador)
{
    if (identificador.is_null()) {
        return -1;
    }
    const std::vector<DefinicaoDeReceita>& receitas = dados.catalogoDeReceitas.todasAsReceitas();
    for (std::size_t indice = 0; indice < receitas.size(); ++indice) {
        if (receitas[indice].identificadorDeTexto == identificador.get<std::string>()) {
            return static_cast<int>(indice);
        }
    }
    throw RecusaDoSave("O save usa a " + identificador.get<std::string>() + ", que não existe mais.");
}

int indiceDoEventoPeloTexto(const DadosDoJogo& dados, const std::string& identificador)
{
    const std::vector<DefinicaoDeEvento>& eventos = dados.catalogoDeEventos.todosOsEventos();
    for (std::size_t indice = 0; indice < eventos.size(); ++indice) {
        if (eventos[indice].identificadorDeTexto == identificador) {
            return static_cast<int>(indice);
        }
    }
    throw RecusaDoSave("O save usa o " + identificador + ", que não existe mais.");
}

// Um objeto "ID do item -> valor" que precisa ter todos os itens do jogo, nem mais nem menos.
void exigirTodosOsItens(const DadosDoJogo& dados, const json& objeto, const char* secao)
{
    if (!objeto.is_object() || static_cast<int>(objeto.size()) != dados.catalogoDeItens.quantidadeDeItens()) {
        throw RecusaDoSave(std::string("O save tem outros itens que os do jogo (seção ") + secao + ").");
    }
}

// Chama f(numeroDoItem, valor) para cada item do objeto; confere os IDs.
template <class Funcao>
void paraCadaItem(const DadosDoJogo& dados, const json& objeto, Funcao f)
{
    for (auto linha = objeto.begin(); linha != objeto.end(); ++linha) {
        f(numeroDoItemPeloTexto(dados, linha.key()), linha.value());
    }
}

void conferirOIndiceDaConstrucao(int indice, std::size_t quantidadeDeConstrucoes, const char* onde)
{
    if (indice < -1 || indice >= static_cast<int>(quantidadeDeConstrucoes)) {
        throw RecusaDoSave(std::string("O save tem uma construção que não existe (") + onde + ").");
    }
}

std::vector<OrdemDoJogador> lerAsOrdens(const DadosDoJogo& dados, const json& lista)
{
    std::vector<OrdemDoJogador> ordens;
    for (const json& linha : lista) {
        OrdemDoJogador ordem;
        const int tipo = linha.at("tipo").get<int>();
        if (tipo < 0 || tipo > static_cast<int>(TipoDeOrdem::RepetirSempre)) {
            throw RecusaDoSave("O save tem uma ordem de tipo desconhecido.");
        }
        ordem.tipo = static_cast<TipoDeOrdem>(tipo);
        ordem.numeroDoItem = numeroDoItemPeloTexto(dados, linha.at("item").get<std::string>());
        ordem.quantidade = linha.at("quantidade").get<int>();
        ordem.quantidadeJaFeita = linha.at("feita").get<int>();
        ordens.push_back(ordem);
    }
    return ordens;
}

void conferirAVersao(const json& arvore)
{
    const int versaoDoSave = arvore.at("cabecalho").at("versaoDoFormato").get<int>();
    if (versaoDoSave != versaoDoFormatoDoSave) {
        throw RecusaDoSave("Este save é de outra versão do jogo (versão " + std::to_string(versaoDoSave) + "; o jogo usa a " +
                           std::to_string(versaoDoFormatoDoSave) + ").");
    }
}

void restaurarORelogio(SimulacaoDaVila& simulacao, const json& relogio)
{
    const int velocidade = relogio.at("velocidade").get<int>();
    const int velocidadeAntesDaPausa = relogio.at("velocidadeAntesDaPausa").get<int>();
    const bool velocidadeValida = velocidade == 0 || velocidade == 1 || velocidade == 2 || velocidade == 4;
    const bool velocidadeAntesValida = velocidadeAntesDaPausa == 1 || velocidadeAntesDaPausa == 2 || velocidadeAntesDaPausa == 4;
    if (!velocidadeValida || !velocidadeAntesValida) {
        throw RecusaDoSave("O save tem uma velocidade de jogo que não existe.");
    }
    Acesso::tickAtualDaPartida(simulacao.relogio) = relogio.at("tick").get<int64_t>();
    Acesso::velocidadeDoJogo(simulacao.relogio) = velocidade;
    Acesso::velocidadeAntesDaPausa(simulacao.relogio) = velocidadeAntesDaPausa;
}

// Fase A1 (ficha do Clima): um clima que não existe recusa o save.
TipoDeClima lerOClimaDoSave(const json& nome)
{
    const std::optional<TipoDeClima> clima = climaPeloNome(nome.get<std::string>());
    if (!clima) {
        throw RecusaDoSave("O save tem um clima que o jogo não conhece (" + nome.get<std::string>() + ").");
    }
    return *clima;
}

void restaurarOClima(SimulacaoDaVila& partida, const json& secao)
{
    partida.clima.climaDeHoje = lerOClimaDoSave(secao.at("climaDeHoje"));
    partida.clima.climaDeOntem = lerOClimaDoSave(secao.at("climaDeOntem"));
    partida.clima.lenhaDevidaEmCentesimos = secao.at("lenhaDevidaEmCentesimos").get<int>();
    partida.clima.diasEmDeficitDeLenha = secao.at("diasEmDeficitDeLenha").get<int>();
    guardarODiaDeHojeNoClima(partida.clima, partida.relogio);
}

TipoDeEncontro lerOEncontroDoSave(const json& nome)
{
    const std::string texto = nome.get<std::string>();
    if (texto == "bandidos") {
        return TipoDeEncontro::Bandidos;
    }
    if (texto == "mercador_viajante") {
        return TipoDeEncontro::MercadorViajante;
    }
    if (texto != "nenhum") {
        throw RecusaDoSave("O save tem um encontro que o jogo não conhece (" + texto + ").");
    }
    return TipoDeEncontro::Nenhum;
}

// Fase B1 (ficha da Viagem).
void restaurarAViagem(SimulacaoDaVila& partida, const json& secao)
{
    const DadosDoJogo& dados = partida.dados;
    const json& entrada = secao.at("grupo");
    EntidadeEmTransito& grupo = partida.viagem.grupoDoJogador;
    grupo.tipoDeEntidade = entrada.at("tipoDeEntidade").get<std::string>();
    grupo.emTransito = entrada.at("emTransito").get<bool>();
    grupo.mapaAtual = entrada.at("mapaAtual").get<int>();
    grupo.proximoMapa = entrada.at("proximoMapa").get<int>();
    grupo.rota = entrada.at("rota").get<std::vector<int>>();
    grupo.ticksDoTrecho = entrada.at("ticksDoTrecho").get<int>();
    grupo.ticksAndadosNoTrecho = entrada.at("ticksAndadosNoTrecho").get<int>();
    grupo.pessoas = entrada.at("pessoas").get<int>();
    grupo.montados = entrada.at("montados").get<bool>();
    grupo.cargaAcimaDoLimite = entrada.at("cargaAcimaDoLimite").get<bool>();
    grupo.marchaForcada = entrada.at("marchaForcada").get<bool>();
    grupo.noitesSemDormir = entrada.at("noitesSemDormir").get<int>();
    grupo.ticksSemDormir = entrada.at("ticksSemDormir").get<int>();
    grupo.encontroMarcado = EncontroMarcado{lerOEncontroDoSave(entrada.at("encontroMarcado")), entrada.at("bandidos").get<int>()};
    grupo.encontroAcontecendo = entrada.at("encontroAcontecendo").get<bool>();
    grupo.ticksParadoNaLuta = entrada.at("ticksParadoNaLuta").get<int>();
    grupo.dinheiroEmDecimos = entrada.at("dinheiro").get<int64_t>();
    exigirTodosOsItens(dados, entrada.at("itens"), "viagem");
    grupo.quantidadePorItem.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
    paraCadaItem(dados, entrada.at("itens"), [&](int numeroDoItem, const json& quantidade) {
        grupo.quantidadePorItem[static_cast<std::size_t>(numeroDoItem)] = quantidade.get<int>();
    });
    const json& nevoa = secao.at("nevoa");
    if (nevoa.size() != partida.viagem.nevoa.size()) {
        throw RecusaDoSave("O save tem uma névoa de " + std::to_string(nevoa.size()) + " mapas, e o mundo tem " + std::to_string(partida.viagem.nevoa.size()) + ".");
    }
    for (std::size_t mapa = 0; mapa < nevoa.size(); ++mapa) {
        partida.viagem.nevoa[mapa] = RegistroDaNevoa{nevoa[mapa].at(0).get<int64_t>(), nevoa[mapa].at(1).get<std::string>(), nevoa[mapa].at(2).get<int>(),
                                                     static_cast<EstadoDoTerritorio>(nevoa[mapa].at(3).get<int>())};
    }
    const json& climas = secao.at("climaDeHojePorRegiao");
    partida.viagem.climaDeHojePorRegiao.clear();
    for (const json& clima : climas) {
        partida.viagem.climaDeHojePorRegiao.push_back(lerOClimaDoSave(clima));
    }
}

void restaurarOSorteio(SorteioComSemente& sorteio, const json& dadosDoSorteio)
{
    Acesso::estadoDoGerador(sorteio) = dadosDoSorteio.at("estado").get<uint64_t>();
    Acesso::incrementoDaSequencia(sorteio) = dadosDoSorteio.at("incremento").get<uint64_t>();
}

void restaurarAVila(SimulacaoDaVila& simulacao, const json& lista)
{
    const DadosDoJogo& dados = simulacao.dados;
    std::vector<ConstrucaoDaVila> construcoes;
    for (const json& linha : lista) {
        ConstrucaoDaVila construcao;
        construcao.identificador = linha.at("identificador").get<std::string>();
        construcao.tipo = linha.at("tipo").get<std::string>();
        if (dados.catalogoDeConstrucoes.procurarConstrucao(construcao.tipo) == nullptr) {
            throw RecusaDoSave("O save usa a " + construcao.tipo + ", que não existe mais.");
        }
        construcao.numeroDoLote = linha.at("lote").get<int>();
        bool loteExiste = false;
        for (const LoteDeConstrucao& lote : simulacao.mapa.lotesDeConstrucao) {
            loteExiste = loteExiste || lote.identificadorDoLote == construcao.numeroDoLote;
        }
        if (!loteExiste) {
            throw RecusaDoSave("O save usa o lote " + std::to_string(construcao.numeroDoLote) + ", que não existe mais no mapa.");
        }
        construcao.donoEhOJogador = linha.at("donoEhOJogador").get<bool>();
        const json& area = linha.at("area");
        construcao.areaDoLote = RetanguloEmTiles{area.at(0).get<int>(), area.at(1).get<int>(), area.at(2).get<int>(), area.at(3).get<int>()};
        construcao.tileDaPorta = lerOTile(linha.at("porta"));
        construcao.emObra = linha.at("emObra").get<bool>();
        construcao.nivel = linha.at("nivel").get<int>();
        construcao.ordensDoJogador = lerAsOrdens(dados, linha.at("ordens"));
        construcoes.push_back(construcao);
    }
    if (construcoes.size() < simulacao.vila.construcoes.size()) {
        throw RecusaDoSave("O save tem menos construções que a vila inicial do jogo.");
    }
    simulacao.vila.construcoes = std::move(construcoes);
    // Regra 4.2, passo 6: o mapa bloqueado pelos lotes sai da lista das construções.
    bloquearOsLotesOcupados(simulacao.mapa, simulacao.vila);
}

void restaurarAProducao(SimulacaoDaVila& simulacao, const json& lista)
{
    const DadosDoJogo& dados = simulacao.dados;
    if (lista.size() != simulacao.vila.construcoes.size()) {
        throw RecusaDoSave("O save tem a produção de outra quantidade de construções.");
    }
    simulacao.producaoDasConstrucoes.assign(lista.size(), ProducaoDaConstrucao{});
    for (std::size_t indice = 0; indice < lista.size(); ++indice) {
        const json& linha = lista[indice];
        ProducaoDaConstrucao& producao = simulacao.producaoDasConstrucoes[indice];
        producao.ehDeProducao = linha.at("ehDeProducao").get<bool>();
        producao.receitaDoCiclo = indiceDaReceitaPeloTexto(dados, linha.at("receitaDoCiclo"));
        producao.receitaEscolhidaParaOProximoCiclo = indiceDaReceitaPeloTexto(dados, linha.at("receitaEscolhida"));
        producao.cicloComecou = linha.at("cicloComecou").get<bool>();
        producao.progressoDoCiclo = linha.at("progressoDoCiclo").get<int64_t>();
        producao.restoDaProducaoBracal = linha.at("restoDaProducaoBracal").get<int>();
        producao.estaParada = linha.at("estaParada").get<bool>();
        producao.motivoDaParada = linha.at("motivoDaParada").get<std::string>();
        producao.estaDeFolgaHoje = linha.at("estaDeFolgaHoje").get<bool>();
        producao.ordens = lerAsOrdens(dados, linha.at("ordens"));
        const int nivelDeSalario = linha.at("nivelDeSalario").get<int>();
        if (nivelDeSalario < 0 || nivelDeSalario > static_cast<int>(NivelDeSalario::Alto)) {
            throw RecusaDoSave("O save tem um nível de salário que não existe.");
        }
        producao.nivelDeSalario = static_cast<NivelDeSalario>(nivelDeSalario);
        producao.diasSeguidosComSalarioBaixo = linha.at("diasSeguidosComSalarioBaixo").get<int>();
        producao.manutencaoAtrasada = linha.at("manutencaoAtrasada").get<bool>();
        producao.emObra = linha.at("emObra").get<bool>();
        producao.numeroDoGerente = linha.at("gerente").get<int>();
        producao.diasDeSalarioAtrasado = linha.at("diasDeSalarioAtrasado").get<int>();
        producao.diasSemEmpregados = linha.at("diasSemEmpregados").get<int>();
        producao.modoAutomatico = linha.at("modoAutomatico").get<bool>();
        producao.salarioDevidoEmDecimos = linha.at("salarioDevido").get<int64_t>();
        producao.areaDoCampoNoMapa = linha.at("areaDoCampo").get<int>();
        const json& tilesDoCampo = linha.at("tilesDoCampo");
        for (std::size_t posicao = 0; posicao + 1 < tilesDoCampo.size(); posicao += 2) {
            producao.tilesDeTrabalhoNoCampo.push_back(PosicaoDeTile{tilesDoCampo[posicao].get<int>(), tilesDoCampo[posicao + 1].get<int>()});
        }
        paraCadaItem(dados, linha.at("producaoDeHoje"), [&](int numeroDoItem, const json& quantidade) {
            producao.producaoDeHojePorItem[numeroDoItem] = quantidade.get<int>();
        });
    }
}

void restaurarAsObras(SimulacaoDaVila& simulacao, const json& lista)
{
    simulacao.obras = ObrasDaVila{};
    for (const json& linha : lista) {
        ObraNaFila obra;
        const int tipo = linha.at("tipo").get<int>();
        if (tipo < 0 || tipo > static_cast<int>(TipoDeObra::Manutencao)) {
            throw RecusaDoSave("O save tem uma obra de tipo desconhecido.");
        }
        obra.tipo = static_cast<TipoDeObra>(tipo);
        obra.indiceDaConstrucao = linha.at("construcao").get<int>();
        conferirOIndiceDaConstrucao(obra.indiceDaConstrucao, simulacao.vila.construcoes.size(), "obra");
        obra.nivelDeDestino = linha.at("nivelDeDestino").get<int>();
        obra.centesimosDeHoraQueFaltam = linha.at("centesimosDeHoraQueFaltam").get<int64_t>();
        obra.esperaMaterial = linha.at("esperaMaterial").get<bool>();
        simulacao.obras.fila.push_back(obra);
    }
}

void restaurarOInventario(SimulacaoDaVila& simulacao, const json& inventario)
{
    const DadosDoJogo& dados = simulacao.dados;
    exigirTodosOsItens(dados, inventario.at("cidade"), "inventario");
    exigirTodosOsItens(dados, inventario.at("jogador"), "inventario");
    InventarioDaCidade novo(dados.catalogoDeItens);
    paraCadaItem(dados, inventario.at("cidade"), [&](int numeroDoItem, const json& quantidade) {
        Acesso::quantidadePorItemDaCidade(novo)[static_cast<std::size_t>(numeroDoItem)] = quantidade.get<int>();
    });
    paraCadaItem(dados, inventario.at("jogador"), [&](int numeroDoItem, const json& quantidade) {
        Acesso::quantidadePorItemDoJogador(novo)[static_cast<std::size_t>(numeroDoItem)] = quantidade.get<int>();
    });
    // A capacidade não vai no arquivo: é a soma das construções.
    recalcularCapacidade(novo, simulacao.vila, dados.catalogoDeConstrucoes, dados.regras.construcoes);
    simulacao.inventario = novo;
}

void restaurarODinheiro(DinheiroDaVila& dinheiro, const json& linha)
{
    dinheiro.bancoDaCidade = linha.at("bancoDaCidade").get<int64_t>();
    dinheiro.dinheiroDoJogador = linha.at("dinheiroDoJogador").get<int64_t>();
    dinheiro.dinheiroDaClasseCivil = linha.at("dinheiroDaClasseCivil").get<int64_t>();
    dinheiro.quantidadeDeCivis = linha.at("quantidadeDeCivis").get<int>();
    dinheiro.rendaDoDiaDaClasseCivil = linha.at("rendaDoDiaDaClasseCivil").get<int64_t>();
    dinheiro.diasEmDeficitDeComida = linha.at("diasEmDeficitDeComida").get<int>();
    dinheiro.diasEmDeficitDeMateriais = linha.at("diasEmDeficitDeMateriais").get<int>();
    dinheiro.materiaisDevidosEmOitavos = linha.at("materiaisDevidosEmOitavos").get<int>();
    dinheiro.bancoNoComecoDaEstacao = linha.at("bancoNoComecoDaEstacao").get<int64_t>();
    dinheiro.ajudaDoEstabilizadorNaEstacao = linha.at("ajudaDoEstabilizadorNaEstacao").get<int64_t>();
    dinheiro.dinheiroQueEntrouDeFora = linha.at("dinheiroQueEntrouDeFora").get<int64_t>();
    dinheiro.consumoNaoPagoHoje = linha.at("consumoNaoPagoHoje").get<int64_t>();
    dinheiro.comidaQueFaltouHoje = linha.at("comidaQueFaltouHoje").get<int>();
    dinheiro.gastoDoJogadorComInsumosHoje = linha.at("gastoDoJogadorComInsumosHoje").get<int64_t>();
    dinheiro.gastoDaCestaDeHoje = linha.at("gastoDaCestaDeHoje").get<int64_t>();
    dinheiro.gastoExtraDeHoje = linha.at("gastoExtraDeHoje").get<int64_t>();
    dinheiro.impostoDeHoje = linha.at("impostoDeHoje").get<int64_t>();
    dinheiro.ajudaDoEstabilizadorHoje = linha.at("ajudaDoEstabilizadorHoje").get<int64_t>();
}

void restaurarOMercado(SimulacaoDaVila& simulacao, const json& mercado)
{
    const DadosDoJogo& dados = simulacao.dados;
    exigirTodosOsItens(dados, mercado.at("precos"), "mercado");
    simulacao.mercadoLocal.precoLocalPorItemEmDecimos.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
    paraCadaItem(dados, mercado.at("precos"), [&](int numeroDoItem, const json& preco) {
        simulacao.mercadoLocal.precoLocalPorItemEmDecimos[static_cast<std::size_t>(numeroDoItem)] = preco.get<int>();
    });
}

void restaurarOConsumoMedido(SimulacaoDaVila& simulacao, const json& consumo)
{
    const DadosDoJogo& dados = simulacao.dados;
    exigirTodosOsItens(dados, consumo.at("ultimosDias"), "consumoMedido");
    exigirTodosOsItens(dados, consumo.at("hoje"), "consumoMedido");
    exigirTodosOsItens(dados, consumo.at("sobra"), "consumoMedido");
    simulacao.consumoMedido = ConsumoMedidoDaVila(dados.catalogoDeItens.quantidadeDeItens());
    simulacao.consumoMedido.produtoEstaComEstoqueDeSobra.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), false);
    paraCadaItem(dados, consumo.at("ultimosDias"), [&](int numeroDoItem, const json& dias) {
        if (dias.size() != static_cast<std::size_t>(diasGuardadosDoConsumo)) {
            throw RecusaDoSave("O save guarda outra quantidade de dias de consumo.");
        }
        for (std::size_t dia = 0; dia < dias.size(); ++dia) {
            Acesso::consumoDosUltimosDiasPorItem(simulacao.consumoMedido)[static_cast<std::size_t>(numeroDoItem)][dia] = dias[dia].get<int64_t>();
        }
    });
    paraCadaItem(dados, consumo.at("hoje"), [&](int numeroDoItem, const json& valor) {
        Acesso::consumoDeHojePorItem(simulacao.consumoMedido)[static_cast<std::size_t>(numeroDoItem)] = valor.get<int64_t>();
    });
    paraCadaItem(dados, consumo.at("sobra"), [&](int numeroDoItem, const json& valor) {
        simulacao.consumoMedido.produtoEstaComEstoqueDeSobra[static_cast<std::size_t>(numeroDoItem)] = valor.get<bool>();
    });
    Acesso::posicaoDoDiaMaisAntigo(simulacao.consumoMedido) = consumo.at("posicaoDoDiaMaisAntigo").get<int>();
}

void restaurarAFelicidade(FelicidadeDaClasse& felicidade, const json& linha)
{
    felicidade.felicidadeEmDecimos = linha.at("felicidade").get<int>();
    felicidade.alvoEmDecimos = linha.at("alvo").get<int>();
    felicidade.fatoresDoDia.consumoAtendido = linha.at("consumoAtendido").get<int>();
    felicidade.fatoresDoDia.desempregoERenda = linha.at("desempregoERenda").get<int>();
    felicidade.fatoresDoDia.moradia = linha.at("moradia").get<int>();
    felicidade.fatoresDoDia.eventosDuradouros = linha.at("eventosDuradouros").get<int>();
}

void restaurarOsEventos(SimulacaoDaVila& simulacao, const json& eventos)
{
    const DadosDoJogo& dados = simulacao.dados;
    const std::size_t quantidadeDeEventos = static_cast<std::size_t>(dados.catalogoDeEventos.quantidadeDeEventos());
    if (eventos.at("cooldown").size() != quantidadeDeEventos || eventos.at("diasSeguidosNaCondicao").size() != quantidadeDeEventos) {
        throw RecusaDoSave("O save tem outros eventos que os do jogo.");
    }
    simulacao.eventos.eventosAtivos.clear();
    for (const json& ativo : eventos.at("ativos")) {
        simulacao.eventos.eventosAtivos.push_back(
            EventoAtivo{indiceDoEventoPeloTexto(dados, ativo.at("evento").get<std::string>()), ativo.at("dias").get<int>()});
    }
    simulacao.eventos.diaEmQuePodeSairDeNovo.assign(quantidadeDeEventos, 0);
    simulacao.eventos.diasSeguidosNaCondicao.assign(quantidadeDeEventos, 0);
    for (auto linha = eventos.at("cooldown").begin(); linha != eventos.at("cooldown").end(); ++linha) {
        simulacao.eventos.diaEmQuePodeSairDeNovo[static_cast<std::size_t>(indiceDoEventoPeloTexto(dados, linha.key()))] = linha.value().get<int64_t>();
    }
    for (auto linha = eventos.at("diasSeguidosNaCondicao").begin(); linha != eventos.at("diasSeguidosNaCondicao").end(); ++linha) {
        simulacao.eventos.diasSeguidosNaCondicao[static_cast<std::size_t>(indiceDoEventoPeloTexto(dados, linha.key()))] = linha.value().get<int>();
    }
    simulacao.eventos.mudancasDeHoje = eventos.at("mudancasDeHoje").get<std::vector<std::string>>();
    simulacao.eventos.linhasParaOConsole = eventos.at("linhasParaOConsole").get<std::vector<std::string>>();
}

void restaurarAsDecisoes(SimulacaoDaVila& simulacao, const json& decisoes)
{
    simulacao.decisoes = DecisoesDoJogador{};
    paraCadaItem(simulacao.dados, decisoes.at("porcentagemDoPreco"), [&](int numeroDoItem, const json& porcentagem) {
        simulacao.decisoes.porcentagemDoPrecoPorItem[numeroDoItem] = porcentagem.get<int>();
    });
}

void conferirOTileDoMapa(const MapaLocal& mapa, const PosicaoDeTile& tile)
{
    if (!mapa.tileEstaDentroDoMapa(tile.tileX, tile.tileY)) {
        throw RecusaDoSave("O save tem um NPC fora do mapa.");
    }
}

// Os NPCs são criados no registro novo na ordem gravada, cada um com o mesmo
// número de entidade de antes: a ordem dos pedidos de caminho depende dele.
void restaurarOsNpcs(SimulacaoDaVila& simulacao, const json& lista)
{
    const std::size_t quantidadeDeConstrucoes = simulacao.vila.construcoes.size();
    simulacao.registroDasEntidades = entt::registry{};
    simulacao.npcsDaVila.clear();
    entt::registry& registro = simulacao.registroDasEntidades;
    for (const json& linha : lista) {
        const entt::entity pedida = static_cast<entt::entity>(linha.at("entidade").get<uint32_t>());
        const entt::entity npc = registro.create(pedida);
        if (npc != pedida) {
            throw RecusaDoSave("O save tem dois NPCs com o mesmo número.");
        }
        registro.emplace<IdentidadeDoNpc>(npc, IdentidadeDoNpc{linha.at("nome").get<std::string>(), linha.at("classe").get<std::string>(),
                                                                 linha.at("numero").get<int>()});
        CasaETrabalhoDoNpc casaETrabalho{linha.at("casa").get<int>(), linha.at("trabalho").get<int>(), linha.at("saiuPorAtrasoDe").get<int>()};
        conferirOIndiceDaConstrucao(casaETrabalho.indiceDaCasa, quantidadeDeConstrucoes, "casa");
        conferirOIndiceDaConstrucao(casaETrabalho.indiceDoTrabalho, quantidadeDeConstrucoes, "trabalho");
        registro.emplace<CasaETrabalhoDoNpc>(npc, casaETrabalho);

        DecisaoDoNpc decisao;
        const int acao = linha.at("acao").get<int>();
        if (acao < 0 || acao > static_cast<int>(AcaoDoNpc::Passear)) {
            throw RecusaDoSave("O save tem uma ação de NPC que não existe.");
        }
        decisao.acaoAtual = static_cast<AcaoDoNpc>(acao);
        decisao.indiceDaConstrucaoOndeEsta = linha.at("ondeEsta").get<int>();
        decisao.indiceDaConstrucaoDoDestino = linha.at("destino").get<int>();
        conferirOIndiceDaConstrucao(decisao.indiceDaConstrucaoOndeEsta, quantidadeDeConstrucoes, "onde o NPC está");
        conferirOIndiceDaConstrucao(decisao.indiceDaConstrucaoDoDestino, quantidadeDeConstrucoes, "destino do NPC");
        decisao.estaACaminho = linha.at("aCaminho").get<bool>();
        decisao.tickEmQueAEsperaAcaba = linha.at("esperaAcabaNoTick").get<int64_t>();
        decisao.diaDaUltimaCompra = linha.at("diaDaUltimaCompra").get<int64_t>();
        registro.emplace<DecisaoDoNpc>(npc, decisao);

        PosicaoDoNpc posicao;
        posicao.lugarAtual = lerOLugar(linha.at("lugar"));
        conferirOTileDoMapa(simulacao.mapa, posicao.lugarAtual.tileAtual);
        conferirOTileDoMapa(simulacao.mapa, posicao.lugarAtual.proximoTile);
        const int olhar = linha.at("olhar").get<int>();
        if (olhar < 0 || olhar > static_cast<int>(DirecaoDoOlhar::Esquerda)) {
            throw RecusaDoSave("O save tem um NPC olhando para um lado que não existe.");
        }
        posicao.direcaoDoOlhar = static_cast<DirecaoDoOlhar>(olhar);
        posicao.lugarNoTickAnteriorParaDesenho = posicao.lugarAtual;  // só desenho: não vai no arquivo
        registro.emplace<PosicaoDoNpc>(npc, posicao);

        if (linha.contains("habilidades")) {
            registro.emplace<HabilidadesDoNpc>(npc, HabilidadesDoNpc{linha.at("habilidades").at(0).get<int>(), linha.at("habilidades").at(1).get<int>()});
        }
        if (linha.contains("idade")) {
            registro.emplace<IdadeDoNpc>(npc, IdadeDoNpc{linha.at("idade").get<int>()});
        }
        if (linha.contains("trabalhoDoDia")) {
            registro.emplace<TrabalhoDoDia>(npc, TrabalhoDoDia{linha.at("trabalhoDoDia").at(0).get<int>(), linha.at("trabalhoDoDia").at(1).get<int>()});
        }
        if (linha.contains("doenca")) {
            registro.emplace<DoencaDoNpc>(npc, DoencaDoNpc{linha.at("doenca").get<int>()});
        }
        if (linha.contains("caminho")) {
            CaminhoDoNpc caminho;
            const json& tiles = linha.at("caminho").at("tiles");
            for (std::size_t posicaoNaLista = 0; posicaoNaLista + 1 < tiles.size(); posicaoNaLista += 2) {
                caminho.tilesDoCaminho.push_back(PosicaoDeTile{tiles[posicaoNaLista].get<int>(), tiles[posicaoNaLista + 1].get<int>()});
            }
            caminho.indiceDoProximoTileDoCaminho = linha.at("caminho").at("indice").get<std::size_t>();
            caminho.tileDeDestino = lerOTile(linha.at("caminho").at("destino"));
            if (caminho.indiceDoProximoTileDoCaminho > caminho.tilesDoCaminho.size()) {
                throw RecusaDoSave("O save tem um caminho de NPC quebrado.");
            }
            registro.emplace<CaminhoDoNpc>(npc, std::move(caminho));
        }
        if (linha.contains("pedidoDeCaminho")) {
            registro.emplace<PedidoDeCaminho>(npc, PedidoDeCaminho{lerOTile(linha.at("pedidoDeCaminho").at("destino")),
                                                                    linha.at("pedidoDeCaminho").at("tick").get<int64_t>()});
        }
        simulacao.npcsDaVila.push_back(npc);
    }
}

void restaurarODiario(SimulacaoDaVila& simulacao, const json& lista)
{
    simulacao.notificacoes = NotificacoesDaVila{};
    for (const json& linha : lista) {
        const int nivel = linha.at(0).get<int>();
        if (nivel < 0 || nivel > static_cast<int>(NivelDaNotificacao::Aviso)) {
            throw RecusaDoSave("O save tem uma notificação de nível desconhecido.");
        }
        simulacao.notificacoes.acrescentar(static_cast<NivelDaNotificacao>(nivel), linha.at(1).get<std::string>(), linha.at(2).get<int64_t>(),
                                           linha.at(3).get<int64_t>());
    }
}

}  // namespace

ResultadoDoCarregamento carregarAPartidaDeBytes(const DadosDoJogo& dados, const RegrasDoTempo& regrasDoTempo,
                                                int distanciaDoCicloDeAndarEmMilesimos, const std::vector<uint8_t>& bytes)
{
    ResultadoDoCarregamento resultado;
    try {
        // Passo 1: o arquivo cortado ou estranho cai aqui.
        json arvore;
        try {
            arvore = json::from_msgpack(bytes, true, true);
        } catch (const json::exception&) {
            throw RecusaDoSave("Não foi possível ler o save (o arquivo está quebrado).");
        }
        if (!arvore.is_object()) {
            throw RecusaDoSave("Não foi possível ler o save (o arquivo está quebrado).");
        }
        // Passos 2 e 3: a versão e, ao ler cada seção, os IDs em texto.
        conferirAVersao(arvore);

        // Passo 4: uma vila nova, com os mesmos dados e regras.
        auto partida = std::make_unique<SimulacaoDaVila>(dados, regrasDoTempo, distanciaDoCicloDeAndarEmMilesimos);

        // Passo 5: cada parte pela seção dela, na ordem do salvamento.
        restaurarORelogio(*partida, arvore.at("relogio"));
        restaurarOSorteio(Acesso::sorteioDosNpcs(*partida), arvore.at("sorteios").at("npcs"));
        restaurarOSorteio(Acesso::sorteioDosEventos(*partida), arvore.at("sorteios").at("eventos"));
        restaurarOSorteio(Acesso::sorteioDoClima(*partida), arvore.at("sorteios").at("clima"));
        restaurarOSorteio(Acesso::sorteioDaPopulacao(*partida), arvore.at("sorteios").at("populacao"));
        partida->populacao.acumuladoDeCrescimentoEmPessoas = arvore.at("populacao").at("acumulado").get<int>();
        partida->populacao.proximoNumeroDeNpc = arvore.at("populacao").at("proximoNumeroDeNpc").get<int>();
        partida->populacao.diasDeFomeNaEstacao = arvore.at("populacao").at("diasDeFomeNaEstacao").get<int>();
        restaurarOClima(*partida, arvore.at("clima"));
        restaurarOSorteio(Acesso::sorteioDaViagem(*partida), arvore.at("sorteios").at("viagem"));
        restaurarOSorteio(Acesso::sorteioDoClimaDasOutrasRegioes(*partida), arvore.at("sorteios").at("climaDasOutrasRegioes"));
        restaurarAViagem(*partida, arvore.at("viagem"));
        {
            const json& leilao = arvore.at("leilao");
            partida->trabalho.leilao = LeilaoDeConstrucao{leilao.at("construcao").get<int>(), leilao.at("diasQueFaltam").get<int>(),
                                                          leilao.at("maiorLance").get<int64_t>(), leilao.at("lanceEhDoJogador").get<bool>()};
        }
        restaurarAVila(*partida, arvore.at("vila"));
        restaurarAProducao(*partida, arvore.at("producao"));
        restaurarAsObras(*partida, arvore.at("obras"));
        restaurarOInventario(*partida, arvore.at("inventario"));
        restaurarODinheiro(partida->dinheiro, arvore.at("dinheiro"));
        restaurarOMercado(*partida, arvore.at("mercado"));
        restaurarOConsumoMedido(*partida, arvore.at("consumoMedido"));
        restaurarAFelicidade(partida->felicidade, arvore.at("felicidade"));
        restaurarOsEventos(*partida, arvore.at("eventos"));
        restaurarAsDecisoes(*partida, arvore.at("decisoes"));
        restaurarOsNpcs(*partida, arvore.at("npcs"));
        restaurarODiario(*partida, arvore.at("diario"));

        // Passo 6: o resto já foi recalculado (o mapa bloqueado e a capacidade).
        partida->avisosAoMontar.clear();
        resultado.partida = std::move(partida);
    } catch (const RecusaDoSave& recusa) {
        resultado.motivoDaRecusa = recusa.what();
    } catch (const json::exception& erro) {
        resultado.motivoDaRecusa = std::string("Não foi possível ler o save (") + erro.what() + ").";
    }
    return resultado;
}
