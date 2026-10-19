// Os testes do marco 8: as tabelas de testes da parte 10 da Interface
// (pedidos, notificações, alertas), da parte 10 da Construção (obras e
// manutenção) e da parte 11 da Economia (preço, salário, contratação, loja).

#include <cstdio>
#include <string>
#include <vector>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Economia/ConsumoESalarios.h"
#include "Jogador/DecisoesDoJogador.h"
#include "Jogador/NotificacoesEAlertas.h"
#include "Jogador/ObrasDaVila.h"
#include "Jogador/PedidosDoJogador.h"
#include "Npc/ComponentesDoNpc.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDaFatia = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";
constexpr int64_t ticksDeUmDia = 19'200;

int numeroDoItem(const DadosDoJogo& dados, const char* identificador)
{
    return dados.catalogoDeItens.procurarNumeroDoItem(identificador).value_or(-1);
}

int indiceDaConstrucao(const SimulacaoDaVila& simulacao, const std::string& identificador)
{
    for (std::size_t indice = 0; indice < simulacao.vila.construcoes.size(); ++indice) {
        if (simulacao.vila.construcoes[indice].identificador == identificador) {
            return static_cast<int>(indice);
        }
    }
    return -1;
}

struct VilaDeTeste {
    SimulacaoDaVila simulacao;
    explicit VilaDeTeste(const DadosDoJogo& dados) : simulacao(dados, RegrasDoTempo{}) {}
    ContextoDaProducao contexto() { return simulacao.contextoDaProducao(); }

    void deixarOEstoque(int item, DonoDoItem dono, int quantidade)
    {
        simulacao.inventario.tentarTirar(item, dono, simulacao.inventario.quantidadeDe(item, dono));
        simulacao.inventario.tentarPor(item, dono, quantidade);
    }
    // Os números dos empregados de uma construção, e os dos desempregados.
    std::vector<int> numerosDosEmpregados(int indice)
    {
        std::vector<int> numeros;
        for (const auto [npc, identidade, casaETrabalho] : simulacao.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc>().each()) {
            if (casaETrabalho.indiceDoTrabalho == indice) {
                numeros.push_back(identidade.numeroDoNpc);
            }
        }
        return numeros;
    }
    std::vector<int> numerosDosDesempregados() { return numerosDosEmpregados(-1); }
    void pedirEAplicar(PedidoDoJogador pedido)
    {
        simulacao.pedidos.acrescentar(std::move(pedido));
        simulacao.aplicarOsPedidosPendentes();
    }
};

bool algumaLinhaComeca(const std::vector<std::string>& linhas, const char* prefixo)
{
    for (const std::string& linha : linhas) {
        if (linha.rfind(prefixo, 0) == 0) {
            return true;
        }
    }
    return false;
}

void testarContasPuras(const DadosDoJogo& dados)
{
    const RegrasDaEconomia& economia = dados.regras.economia;
    const RegrasDasDecisoesDoJogador& decisoes = economia.decisoesDoJogador;
    conferir(calcularPrecoDoItemDoJogador(51, 20) == 61 && calcularPrecoDoItemDoJogador(51, -30) == 36 && calcularPrecoDoItemDoJogador(51, 0) == 51,
             "Móveis a 5,1: +20% = 6,1; -30% = 3,6; 0% = 5,1");
    conferir(calcularQuantidadeDoGastoExtraComOPreco(2, 20, decisoes) == 0 && calcularQuantidadeDoGastoExtraComOPreco(10, 15, decisoes) == 5
                 && calcularQuantidadeDoGastoExtraComOPreco(2, -10, decisoes) == 2 && calcularQuantidadeDoGastoExtraComOPreco(10, 30, decisoes) == 0,
             "o gasto extra compra uma fração: +20% de 2 = 0; +15% de 10 = 5; -10% de 2 = 2; +30% = nada");
    conferir(precoEstaNaFaixaPermitida(30, decisoes) && precoEstaNaFaixaPermitida(-30, decisoes) && !precoEstaNaFaixaPermitida(7, decisoes)
                 && !precoEstaNaFaixaPermitida(35, decisoes),
             "o preço vai de -30 a +30, de 5 em 5");
    conferir(calcularPrecoQueALojaDaCidadePaga(15, economia) == 14, "a loja da cidade paga 90%: Tábua a 1,5 vale 1,4");

    const RegrasDasConstrucoes& construcoes = dados.regras.construcoes;
    const CustoDaObra media3 = calcularCustoDaObra(PorteDoLote::Medio, 3, construcoes);
    conferir(media3.tabuas == 60 && media3.blocosDePedra == 90 && media3.horasDeTrabalho == 360, "obra média até o nível 3: 60 Tábuas, 90 Blocos, 360 h");
    const CustoDaObra manutencaoPequena = calcularCustoDaManutencao(PorteDoLote::Pequeno, 1, construcoes);
    const CustoDaObra manutencaoMedia1 = calcularCustoDaManutencao(PorteDoLote::Medio, 1, construcoes);
    const CustoDaObra manutencaoMedia2 = calcularCustoDaManutencao(PorteDoLote::Medio, 2, construcoes);
    conferir(manutencaoPequena.tabuas == 1 && manutencaoPequena.blocosDePedra == 1 && manutencaoPequena.horasDeTrabalho == 2,
             "manutenção pequena nível 1: 1 Tábua, 1 Bloco, 2 h");
    conferir(manutencaoMedia1.tabuas == 1 && manutencaoMedia1.blocosDePedra == 2 && manutencaoMedia1.horasDeTrabalho == 6,
             "manutenção média nível 1: 1 Tábua, 2 Blocos, 6 h");
    conferir(manutencaoMedia2.tabuas == 2 && manutencaoMedia2.blocosDePedra == 3 && manutencaoMedia2.horasDeTrabalho == 12,
             "manutenção média nível 2: 2 Tábuas, 3 Blocos, 12 h");

    conferir(escreverDinheiroComMilhar(39500) == "3.950,0" && escreverDinheiroComMilhar(5) == "0,5" && escreverDinheiroComMilhar(0) == "0,0"
                 && escreverDinheiroComMilhar(-1234) == "-123,4",
             "o dinheiro na tela: 39500 = 3.950,0; 5 = 0,5; 0 = 0,0");
    conferir(calcularSalarioEmDecimos(800, 100, 100, 100, 800, 100, 100, 75) == 8 && calcularSalarioEmDecimos(800, 100, 100, 100, 800, 100, 100, 125) == 13,
             "1 hora de 1,0 em salário Baixo = 0,75 (0,8) e em Alto = 1,25 (1,3)");
}

void testarPrecoSalarioEContratacao(const DadosDoJogo& dados)
{
    const int moveis = numeroDoItem(dados, "item.moveis_madeira");
    VilaDeTeste vila(dados);
    SimulacaoDaVila& simulacao = vila.simulacao;
    const int carpintaria = indiceDaConstrucao(simulacao, "carpintaria_do_jogador");

    // Preço: +7 recusado com aviso; +20 aceito.
    simulacao.pedidos.acrescentar(PedidoFixarOPreco{moveis, 7});
    conferir(algumaLinhaComeca(simulacao.aplicarOsPedidosPendentes(), "Aviso: pedido recusado") && simulacao.decisoes.porcentagemDoPrecoDe(moveis) == 0,
             "o preço +7% é recusado, com aviso, e nada muda");
    vila.pedirEAplicar(PedidoFixarOPreco{moveis, 20});
    conferir(simulacao.decisoes.porcentagemDoPrecoDe(moveis) == 20, "o preço +20% é aceito");

    // O preço mais alto: a cesta paga o preço do jogador.
    {
        VilaDeTeste outra(dados);
        outra.deixarOEstoque(moveis, DonoDoItem::Jogador, 10);
        outra.simulacao.mercadoLocal.precoLocalPorItemEmDecimos[moveis] = 51;
        outra.simulacao.dinheiro.materiaisDevidosEmOitavos = 1;  // com os 15 de hoje, a cesta tira 2 Móveis
        outra.simulacao.dinheiro.rendaDoDiaDaClasseCivil = 0;    // sem gasto extra
        outra.pedirEAplicar(PedidoFixarOPreco{moveis, 20});
        const int64_t jogadorAntes = outra.simulacao.dinheiro.dinheiroDoJogador;
        ContextoDaProducao contexto = outra.contexto();
        consumirACestaDoDia(contexto);
        conferir(outra.simulacao.dinheiro.dinheiroDoJogador - jogadorAntes == 2 * 61, "a cesta paga 2 Móveis a 6,1 ao jogador (o preço dele, +20%)");
    }

    // Contratar e demitir.
    const std::vector<int> empregados = vila.numerosDosEmpregados(carpintaria);
    const std::vector<int> desempregados = vila.numerosDosDesempregados();
    conferir(empregados.size() == 2 && desempregados.size() == 3, "a Carpintaria começa com 2 empregados e a vila com 3 desempregados");
    vila.pedirEAplicar(PedidoContratar{carpintaria, desempregados[0]});
    conferir(vila.numerosDosEmpregados(carpintaria).size() == 3, "contratar um desempregado: 3 empregados (as 3 vagas do nível 2)");
    simulacao.pedidos.acrescentar(PedidoContratar{carpintaria, desempregados[1]});
    conferir(algumaLinhaComeca(simulacao.aplicarOsPedidosPendentes(), "Aviso: pedido recusado") && vila.numerosDosEmpregados(carpintaria).size() == 3,
             "contratar com as vagas cheias: recusado");
    vila.pedirEAplicar(PedidoDemitir{desempregados[0]});
    conferir(vila.numerosDosEmpregados(carpintaria).size() == 2 && vila.numerosDosDesempregados().size() == 3, "demitir: volta a 2 empregados e 3 desempregados");
    simulacao.pedidos.acrescentar(PedidoDemitir{vila.numerosDosEmpregados(indiceDaConstrucao(simulacao, "fazenda"))[0]});
    conferir(algumaLinhaComeca(simulacao.aplicarOsPedidosPendentes(), "Aviso: pedido recusado"), "demitir um empregado da Cidade: recusado");

    // Salário Baixo: no 8º dia, o mais novo sai; nos outros níveis, ninguém.
    vila.pedirEAplicar(PedidoTrocarONivelDeSalario{carpintaria, NivelDeSalario::Baixo});
    for (int dia = 0; dia < 7; ++dia) {
        ContextoDaProducao contexto = vila.contexto();
        pagarOsSalariosDoDia(contexto);
    }
    conferir(vila.numerosDosEmpregados(carpintaria).size() == 2, "7 dias em salário Baixo: ninguém saiu");
    {
        ContextoDaProducao contexto = vila.contexto();
        pagarOsSalariosDoDia(contexto);
    }
    conferir(vila.numerosDosEmpregados(carpintaria).size() == 1, "no 8º dia em salário Baixo, o empregado mais novo sai");
    vila.pedirEAplicar(PedidoTrocarONivelDeSalario{carpintaria, NivelDeSalario::Normal});
    conferir(simulacao.producaoDasConstrucoes[carpintaria].diasSeguidosComSalarioBaixo == 0, "voltar a Normal zera a contagem");

    // A loja da cidade.
    const int tabua = numeroDoItem(dados, "item.tabua");
    {
        VilaDeTeste loja(dados);
        ContextoDaProducao contexto = loja.contexto();
        const int64_t totalAntes = loja.simulacao.dinheiro.dinheiroTotal();
        const int precoLocal = precoLocalDoItem(contexto, tabua);
        const int64_t jogadorAntes = loja.simulacao.dinheiro.dinheiroDoJogador;
        const int tabuasDaCidadeAntes = loja.simulacao.inventario.quantidadeDe(tabua, DonoDoItem::Cidade);
        loja.pedirEAplicar(PedidoVenderParaALoja{tabua, 10});
        conferir(loja.simulacao.dinheiro.dinheiroDoJogador - jogadorAntes == 10 * calcularPrecoQueALojaDaCidadePaga(precoLocal, dados.regras.economia)
                     && loja.simulacao.inventario.quantidadeDe(tabua, DonoDoItem::Cidade) == tabuasDaCidadeAntes + 10,
                 "vender 10 Tábuas: o jogador recebe 90% do preço local de cada, e a Cidade fica com elas");
        loja.simulacao.pedidos.acrescentar(PedidoVenderParaALoja{tabua, 500});
        conferir(algumaLinhaComeca(loja.simulacao.aplicarOsPedidosPendentes(), "Aviso: pedido recusado"), "vender mais do que tem: recusado");
        const int blocos = numeroDoItem(dados, "item.bloco_de_pedra");
        loja.simulacao.pedidos.acrescentar(PedidoComprarDaLoja{blocos, 200});
        conferir(algumaLinhaComeca(loja.simulacao.aplicarOsPedidosPendentes(), "Aviso: pedido recusado"), "comprar 200 Blocos com 150 na Cidade: recusado");
        loja.pedirEAplicar(PedidoComprarDaLoja{blocos, 5});
        conferir(loja.simulacao.inventario.quantidadeDe(blocos, DonoDoItem::Jogador) == 5, "comprar 5 Blocos: ficam com o jogador");
        conferir(loja.simulacao.dinheiro.dinheiroTotal() == totalAntes, "o dinheiro total não muda com a compra e a venda");
    }
}

void testarObras(const DadosDoJogo& dados)
{
    const int tabua = numeroDoItem(dados, "item.tabua");
    const int bloco = numeroDoItem(dados, "item.bloco_de_pedra");
    VilaDeTeste vila(dados);
    SimulacaoDaVila& simulacao = vila.simulacao;
    ContextoDaProducao contexto = vila.contexto();

    // A Fazenda no lote 5: 10 Tábuas + 10 Blocos + 40 h.
    vila.deixarOEstoque(tabua, DonoDoItem::Jogador, 10);
    const int64_t custoDosBlocos = 10 * calcularPrecoDaLojaDaCidade(precoLocalDoItem(contexto, bloco), dados.regras.economia);
    const int64_t custoTotal = custoDosBlocos + calcularPagamentoDasHorasDeObra(40, contexto);
    conferir(calcularPagamentoDasHorasDeObra(40, contexto) == 300, "40 horas de obra a 0,75 = 30,0");

    simulacao.dinheiro.dinheiroDoJogador = custoTotal - 1;
    simulacao.pedidos.acrescentar(PedidoEncomendarConstrucaoNova{5, "construcao.fazenda"});
    conferir(algumaLinhaComeca(simulacao.aplicarOsPedidosPendentes(), "Aviso: pedido recusado") && simulacao.obras.fila.empty()
                 && simulacao.mapa.tileDaParaAndar(41, 22),
             "a Fazenda com 1 décimo a menos: recusada, nada muda, o lote continua livre");
    simulacao.dinheiro.dinheiroDoJogador = custoTotal;
    const int64_t totalAntes = simulacao.dinheiro.dinheiroTotal();
    const int blocosDaCidadeAntes = simulacao.inventario.quantidadeDe(bloco, DonoDoItem::Cidade);
    vila.pedirEAplicar(PedidoEncomendarConstrucaoNova{5, "construcao.fazenda"});
    conferir(simulacao.obras.fila.size() == 1 && simulacao.dinheiro.dinheiroDoJogador == 0 && simulacao.inventario.quantidadeDe(tabua, DonoDoItem::Jogador) == 0
                 && simulacao.inventario.quantidadeDe(bloco, DonoDoItem::Cidade) == blocosDaCidadeAntes - 10,
             "a Fazenda com o dinheiro exato: a obra entra na fila e os materiais saem");
    conferir(simulacao.dinheiro.dinheiroTotal() == totalAntes, "o dinheiro total não muda ao encomendar (o jogador paga ao Banco)");
    conferir(!simulacao.mapa.tileDaParaAndar(41, 22) && simulacao.vila.construcoes.back().emObra, "a construção nasce em obra e já bloqueia o lote");
    simulacao.pedidos.acrescentar(PedidoEncomendarConstrucaoNova{5, "construcao.fazenda"});
    conferir(algumaLinhaComeca(simulacao.aplicarOsPedidosPendentes(), "Aviso: pedido recusado"), "o lote ocupado não aceita outra obra");

    // 2 Operários por 1 hora: 2 h; a obra de 40 h passa a faltar 38 h.
    const int construtor = indiceDaConstrucao(simulacao, "construtor");
    for (const int numero : vila.numerosDosEmpregados(construtor)) {
        for (const auto [npc, identidade, trabalhoDoDia] : simulacao.registroDasEntidades.view<IdentidadeDoNpc, TrabalhoDoDia>().each()) {
            if (identidade.numeroDoNpc == numero) {
                trabalhoDoDia.ticksTrabalhadosNestaHora = 800;
            }
        }
    }
    andarAsObrasDaHora(contexto);
    conferir(simulacao.obras.fila[0].centesimosDeHoraQueFaltam == 4000 - 200, "2 Operários por 1 hora: a obra de 40 h passa a faltar 38 h");
    simulacao.obras.fila[0].centesimosDeHoraQueFaltam = 150;
    andarAsObrasDaHora(contexto);
    const ConstrucaoDaVila& fazenda = simulacao.vila.construcoes.back();
    conferir(simulacao.obras.fila.empty() && !fazenda.emObra && simulacao.producaoDasConstrucoes.back().ehDeProducao
                 && algumaLinhaComeca(simulacao.obras.linhasParaOConsole, "Obra pronta:"),
             "a obra chega a 0 h: a construção nasce, produz, e a linha 'Obra pronta' sai");

    // Subir a Carpintaria: do 2 ao 3 (média: 60 Tábuas, 90 Blocos). O 4 não.
    const int carpintaria = indiceDaConstrucao(simulacao, "carpintaria_do_jogador");
    simulacao.vila.construcoes[carpintaria].nivel = 3;
    simulacao.pedidos.acrescentar(PedidoSubirDeNivel{carpintaria});
    conferir(algumaLinhaComeca(simulacao.aplicarOsPedidosPendentes(), "Aviso: pedido recusado"), "subir a Carpintaria para o nível 4: o Engenheiro só vai até o 3");
    simulacao.vila.construcoes[carpintaria].nivel = 2;
    vila.deixarOEstoque(tabua, DonoDoItem::Jogador, 60);
    simulacao.dinheiro.dinheiroDoJogador = 100'000;
    vila.pedirEAplicar(PedidoSubirDeNivel{carpintaria});
    conferir(simulacao.obras.fila.size() == 1 && simulacao.obras.fila[0].tipo == TipoDeObra::SubirDeNivel
                 && simulacao.obras.fila[0].centesimosDeHoraQueFaltam == 36'000,
             "subir a Carpintaria ao nível 3: a obra de 360 h entra na fila");
}

void testarManutencao(const DadosDoJogo& dados)
{
    VilaDeTeste vila(dados);
    SimulacaoDaVila& simulacao = vila.simulacao;
    ContextoDaProducao contexto = vila.contexto();
    const RelogioDoMundo primeiroDiaDaEstacao(RegrasDoTempo{}, 16 * ticksDeUmDia);
    const std::size_t construcoes = simulacao.vila.construcoes.size();

    rodarAManutencaoDoDia(contexto, primeiroDiaDaEstacao);
    conferir(simulacao.obras.fila.size() == construcoes, "no primeiro dia da estação, cada construção entra na fila de manutenção");
    int pagas = 0;
    for (const ObraNaFila& obra : simulacao.obras.fila) {
        pagas += obra.esperaMaterial ? 0 : 1;
    }
    conferir(pagas > 0, "as manutenções que têm material são pagas na hora");
    rodarAManutencaoDoDia(contexto, RelogioDoMundo(RegrasDoTempo{}, 32 * ticksDeUmDia));
    bool todasAtrasadas = true;
    for (std::size_t indice = 0; indice < construcoes; ++indice) {
        todasAtrasadas = todasAtrasadas && simulacao.producaoDasConstrucoes[indice].manutencaoAtrasada;
    }
    conferir(todasAtrasadas && simulacao.obras.fila.size() == construcoes, "uma estação sem a manutenção pronta: atrasada, e a nova não entra");
    const int fazenda = indiceDaConstrucao(simulacao, "fazenda");
    conferir(calcularOsAlertasDaVila(contexto).size() >= construcoes, "cada manutenção atrasada vira um alerta");
    // A produção atrasada: 75% do trabalho.
    simulacao.producaoDasConstrucoes[fazenda].manutencaoAtrasada = false;
}

void testarNotificacoesEAlertas(const DadosDoJogo& dados)
{
    NotificacoesDaVila notificacoes;
    for (int numero = 0; numero < 6; ++numero) {
        notificacoes.acrescentar(NivelDaNotificacao::Info, "n" + std::to_string(numero), 0, 100 + numero);
    }
    conferir(notificacoes.naTela(110).size() == 5 && notificacoes.doDiario().size() == 6 && notificacoes.naTela(110)[0]->texto == "n5",
             "6 notificações seguidas: 5 na tela (a mais nova primeiro) e 6 no Diário");
    conferir(notificacoes.naTela(504).size() == 1 && notificacoes.naTela(600).empty(), "cada uma some 400 ticks depois de entrar");
    for (int numero = 0; numero < 400; ++numero) {
        notificacoes.acrescentar(NivelDaNotificacao::Aviso, "x", 0, 1000);
    }
    conferir(notificacoes.doDiario().size() == 300, "o Diário guarda só as 300 últimas");

    VilaDeTeste vila(dados);
    ContextoDaProducao contexto = vila.contexto();
    NivelDaNotificacao nivel = NivelDaNotificacao::Info;
    conferir(classificarALinhaDoConsole(contexto, "Aviso: algo.", nivel) && nivel == NivelDaNotificacao::Aviso
                 && classificarALinhaDoConsole(contexto, "Obra pronta: x.", nivel) && nivel == NivelDaNotificacao::Info
                 && !classificarALinhaDoConsole(contexto, "Resumo do dia 2: tudo.", nivel),
             "o Aviso e a Obra pronta viram notificação; o resumo do dia não");
    conferir(calcularOsAlertasDaVila(contexto).empty(), "a vila no começo não tem nenhum alerta");
    const int pao = numeroDoItem(dados, "item.pao");
    vila.deixarOEstoque(pao, DonoDoItem::Cidade, 0);
    const std::vector<std::string> alertas = calcularOsAlertasDaVila(contexto);
    conferir(alertas.size() == 1 && alertas[0].find("para menos de 2 dias") != std::string::npos, "sem Pão: o alerta 'Pão para menos de 2 dias'");
    vila.deixarOEstoque(pao, DonoDoItem::Cidade, 75);
    conferir(calcularOsAlertasDaVila(contexto).empty(), "com o Pão de volta, o alerta some");
    vila.simulacao.dinheiro.bancoDaCidade = vila.simulacao.dinheiro.bancoNoComecoDaEstacao / 10 - 1;
    conferir(calcularOsAlertasDaVila(contexto).size() == 1, "o Banco abaixo de 10% do começo da estação: o alerta 'Banco baixo'");
}

void testarDeterminismoComPedidos(const DadosDoJogo& dados)
{
    // A mesma semente e os mesmos pedidos, nos mesmos ticks, duas vezes: o mesmo resultado.
    int64_t totais[2] = {0, 0};
    int64_t bancos[2] = {0, 0};
    for (int rodada = 0; rodada < 2; ++rodada) {
        VilaDeTeste vila(dados);
        const int moveis = numeroDoItem(dados, "item.moveis_madeira");
        for (int tick = 0; tick < 3000; ++tick) {
            if (tick == 100) {
                vila.simulacao.pedidos.acrescentar(PedidoFixarOPreco{moveis, 10});
            }
            if (tick == 200) {
                vila.simulacao.pedidos.acrescentar(PedidoEncomendarConstrucaoNova{5, "construcao.casa"});
            }
            vila.simulacao.avancarUmTick();
        }
        totais[rodada] = vila.simulacao.dinheiro.dinheiroTotal();
        bancos[rodada] = vila.simulacao.dinheiro.bancoDaCidade;
    }
    conferir(totais[0] == totais[1] && bancos[0] == bancos[1], "os mesmos pedidos nos mesmos ticks, duas vezes: o mesmo dinheiro");
}

}  // namespace

void testarMarco8()
{
    std::printf("Marco 8: pedidos, preço, salário, contratação, loja, obras, manutenção, notificações e alertas\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    testarContasPuras(dados);
    testarPrecoSalarioEContratacao(dados);
    testarObras(dados);
    testarManutencao(dados);
    testarNotificacoesEAlertas(dados);
    testarDeterminismoComPedidos(dados);
}
