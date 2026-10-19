// Os testes do marco 6: as tabelas da parte 8 das fichas da Construção, do
// Inventário, da Produção e da Economia, e alguns dias inteiros sem tela.

#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "ConferenciaDosTestes.h"
#include "Construcao/ContasDasConstrucoes.h"
#include "Dados/DadosDoJogo.h"
#include "Economia/ConsumoESalarios.h"
#include "Inventario/InventarioDaCidade.h"
#include "Producao/ProducaoDaVila.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDaFatia = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

int numeroDoItem(const DadosDoJogo& dados, const char* identificador)
{
    return dados.catalogoDeItens.procurarNumeroDoItem(identificador).value_or(-1);
}

int indiceDaConstrucaoNaVila(const DadosDoJogo& dados, const std::string& identificador)
{
    for (std::size_t indice = 0; indice < dados.vilaInicial.construcoes.size(); ++indice) {
        if (dados.vilaInicial.construcoes[indice].identificador == identificador) {
            return static_cast<int>(indice);
        }
    }
    return -1;
}

void testarConstrucoes(const DadosDoJogo& dados)
{
    const RegrasDasConstrucoes& regras = dados.regras.construcoes;
    conferir(calcularVagas(1, regras) == 2 && calcularVagas(2, regras) == 3 && calcularVagas(5, regras) == 10, "vagas nos níveis 1, 2 e 5: 2, 3 e 10");
    conferir(calcularCapacidade(PorteDoLote::Medio, 2, regras) == 400, "uma construção média no nível 2 guarda 400");
    conferir(dados.catalogoDeConstrucoes.quantidadeDeConstrucoes() == 8, "o construcoes.json da fatia tem 8 tipos, sem erro");
    const DefinicaoDeConstrucao* mercado = dados.catalogoDeConstrucoes.procurarConstrucao("construcao.mercado");
    conferir(mercado != nullptr && mercado->porte == PorteDoLote::Medio && mercado->trabalhoSemReceita == TrabalhoDaReceita::Manufaturado,
             "o Mercado é médio, e o Lojista é Manufaturado");

    ListaDeProblemasDosDados problemasDoPorte;
    lerArquivoDeConstrucoes(R"([{"id": "construcao.fazenda", "porte": "enorme", "guarda": [], "funcao": "fazendeiro"}])", dados.tabelaDeTextos,
                            problemasDoPorte);
    conferir(problemasDoPorte.algumErroContem("\"porte\""), "um porte \"enorme\": erro");

    ListaDeProblemasDosDados problemasDoMercado;
    const CatalogoDeConstrucoes mercadoSemTrabalho =
        lerArquivoDeConstrucoes(R"([{"id": "construcao.mercado", "porte": "medio", "guarda": ["bem_de_consumo"], "funcao": "lojista"}])",
                                dados.tabelaDeTextos, problemasDoMercado);
    conferirTrabalhoDasConstrucoes(mercadoSemTrabalho, dados.catalogoDeReceitas, problemasDoMercado);
    conferir(problemasDoMercado.algumErroContem("trabalho_sem_receita"), "o Mercado sem \"trabalho_sem_receita\": erro");

    ListaDeProblemasDosDados problemasDasVagas;
    const CatalogosParaAVila catalogos{dados.catalogoDeConstrucoes, dados.catalogoDeItens, dados.regras.construcoes};
    lerVilaInicial(R"({"semente_da_partida": 1, "construcoes": [
        {"id": "casa_1", "tipo": "construcao.casa", "lote": 2, "dono": "cidade"},
        {"id": "fazenda", "tipo": "construcao.fazenda", "lote": 1, "dono": "cidade", "nivel": 1}],
        "npcs": [{"nome": "A", "classe": "civil", "casa": "casa_1", "trabalho": "fazenda"},
                 {"nome": "B", "classe": "civil", "casa": "casa_1", "trabalho": "fazenda"},
                 {"nome": "C", "classe": "civil", "casa": "casa_1", "trabalho": "fazenda"}]})",
                   dados.mapaDaVila, catalogos, problemasDasVagas);
    conferir(problemasDasVagas.quantidadeDeErros() == 1 && problemasDasVagas.algumErroContem("só tem 2 vagas"), "3 empregados numa Fazenda nível 1: erro");

    conferir(escolherGerente({{0, 50}, {1, 60}}) == 1, "habilidade 50 e 60: o de 60 é o gerente");
    conferir(escolherGerente({{4, 50}, {2, 50}}) == 2, "empate de habilidade: o de menor número na vila");

    InventarioDaCidade inventario(dados.catalogoDeItens);
    recalcularCapacidade(inventario, dados.vilaInicial, dados.catalogoDeConstrucoes, dados.regras.construcoes);
    conferir(inventario.capacidadeDaCategoria(CategoriaDoItem::MateriaPrima) == 900 && inventario.capacidadeDaCategoria(CategoriaDoItem::BemDeConsumo) == 800,
             "a capacidade da vila da fatia: 900 de matéria-prima e 800 de bem de consumo");
}

void testarInventario(const DadosDoJogo& dados)
{
    const int pao = numeroDoItem(dados, "item.pao");
    const int tabua = numeroDoItem(dados, "item.tabua");
    const int grao = numeroDoItem(dados, "item.grao");
    const int agua = numeroDoItem(dados, "item.agua");

    InventarioDaCidade inventario(dados.catalogoDeItens);
    inventario.definirCapacidadeDaCategoria(CategoriaDoItem::BemDeConsumo, 800);
    inventario.definirCapacidadeDaCategoria(CategoriaDoItem::MateriaPrima, 100);
    conferir(inventario.tentarPor(pao, DonoDoItem::Cidade, 10) && inventario.quantidadeDe(pao, DonoDoItem::Cidade) == 10,
             "pôr 10 Pães da Cidade num inventário vazio: 10 Pães da Cidade");
    conferir(inventario.tentarPor(tabua, DonoDoItem::Jogador, 10) && inventario.quantidadeDe(tabua, DonoDoItem::Jogador) == 10
                 && inventario.quantidadeDe(tabua, DonoDoItem::Cidade) == 0,
             "pôr 10 Tábuas do jogador: 10 do jogador, 0 da Cidade");
    inventario.tentarPor(grao, DonoDoItem::Cidade, 85);
    conferir(!inventario.tentarPor(grao, DonoDoItem::Cidade, 6) && inventario.quantidadeNaCategoria(CategoriaDoItem::MateriaPrima) == 95,
             "capacidade 100 com 95 dentro (os dois donos juntos): pôr 6 não cabe, e continua com 95");

    InventarioDaCidade poucoGrao(dados.catalogoDeItens);
    poucoGrao.definirCapacidadeDaCategoria(CategoriaDoItem::MateriaPrima, 100);
    poucoGrao.tentarPor(grao, DonoDoItem::Cidade, 2);
    conferir(!poucoGrao.tentarTirar(grao, DonoDoItem::Cidade, 3) && poucoGrao.quantidadeDe(grao, DonoDoItem::Cidade) == 2,
             "tirar 3 Grãos com 2 no estoque: faltou, e continua com 2");
    conferir(!poucoGrao.tentarTirarVarios({{grao, 2}, {agua, 1}}, DonoDoItem::Cidade) && poucoGrao.quantidadeDe(grao, DonoDoItem::Cidade) == 2,
             "tirar 2 Grãos e 1 Água sem Água: faltou, e os 2 Grãos continuam lá");

    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    conferir(simulacao.avisosAoMontar.empty() && simulacao.inventario.quantidadeDe(pao, DonoDoItem::Cidade) == 75
                 && simulacao.inventario.quantidadeDe(tabua, DonoDoItem::Jogador) == 12,
             "o estoque inicial da fatia: 75 Pães da Cidade e 12 Tábuas do jogador");
}

void testarContasDaProducao(const DadosDoJogo& dados)
{
    const DefinicaoDeReceita& receitaDoPao = dados.catalogoDeReceitas.todasAsReceitas()[*dados.catalogoDeReceitas.procurarReceitaDoProduto(numeroDoItem(dados, "item.pao"))];
    conferir(progressoNecessarioDoCiclo(receitaDoPao, 800) == 320'000, "o progresso de um Pão: 320.000");
    conferir(calcularProgressoDoTrabalho(800, 60, 1, 1) == 88'000, "1 hora de uma Cozinheira com habilidade 60, nível 1: +88.000");
    conferir(calcularProgressoDoTrabalho(800, 50, 2, 1) == 88'000, "1 hora com habilidade 50 numa Carpintaria nível 2 fazendo Tábua: +88.000");

    // O Posto de Coleta: Água 16 (alvo 20) e Tora 4 (alvo 30). Água e Tora valem 5 décimos.
    const int64_t notaDaAgua = calcularNotaDaReceita(5, 0, calcularDeficitDoItem(16, 20 * 100));
    const int64_t notaDaTora = calcularNotaDaReceita(5, 0, calcularDeficitDoItem(4, 30 * 100));
    conferir(calcularDeficitDoItem(16, 2000) == 20 && calcularDeficitDoItem(4, 3000) == 87 && notaDaTora > notaDaAgua,
             "Água 16 (alvo 20) e Tora 4 (alvo 30): déficits de 20% e 87%, e o Posto escolhe a Tora");
    conferir(calcularDeficitDoItem(50, 0) == 0 && calcularDeficitDoItem(50, 2000) == 0, "sem alvo, ou com estoque acima do alvo, o déficit é 0");
    conferir(calcularPrecoDaLojaDaCidade(17, dados.regras.economia) == 19, "a loja da cidade vende o Pão de 1,7 a 1,9 (1,87 arredondado)");
}

// Uma vila da fatia montada, para mexer no estoque antes de rodar uma regra.
struct VilaDeTeste {
    DadosDoJogo dados;
    SimulacaoDaVila simulacao;
    explicit VilaDeTeste(DadosDoJogo dadosLidos) : dados(std::move(dadosLidos)), simulacao(dados, RegrasDoTempo{}) {}
};

void tirarTudoDoItem(SimulacaoDaVila& simulacao, int item)
{
    for (const DonoDoItem dono : {DonoDoItem::Cidade, DonoDoItem::Jogador}) {
        simulacao.inventario.tentarTirar(item, dono, simulacao.inventario.quantidadeDe(item, dono));
    }
}

void testarRegrasDaProducao(const DadosDoJogo& dadosLidos)
{
    const int grao = numeroDoItem(dadosLidos, "item.grao");
    const int agua = numeroDoItem(dadosLidos, "item.agua");
    const int pao = numeroDoItem(dadosLidos, "item.pao");
    const int tabua = numeroDoItem(dadosLidos, "item.tabua");
    const int moveis = numeroDoItem(dadosLidos, "item.moveis_madeira");
    const int tora = numeroDoItem(dadosLidos, "item.tora");
    const int receitaDaTabua = *dadosLidos.catalogoDeReceitas.procurarReceitaDoProduto(tabua);
    const int receitaDosMoveis = *dadosLidos.catalogoDeReceitas.procurarReceitaDoProduto(moveis);
    const int receitaDaTora = *dadosLidos.catalogoDeReceitas.procurarReceitaDoProduto(tora);
    const int receitaDoPao = *dadosLidos.catalogoDeReceitas.procurarReceitaDoProduto(pao);

    {
        // O Cozinheiro com 2 Grãos e nenhuma Água: parado, e o Grão continua lá.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        const int cozinheiro = indiceDaConstrucaoNaVila(vila.dados, "cozinheiro");
        tirarTudoDoItem(simulacao, grao);
        tirarTudoDoItem(simulacao, agua);
        simulacao.inventario.tentarPor(grao, DonoDoItem::Cidade, 2);
        for (const entt::entity npc : simulacao.npcsDaVila) {
            if (simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho == cozinheiro) {
                simulacao.registroDasEntidades.get<TrabalhoDoDia>(npc).ticksTrabalhadosNestaHora = 800;
            }
        }
        ContextoDaProducao contexto = simulacao.contextoDaProducao();
        rodarOsCiclosDaHora(contexto, simulacao.relogio);
        const ProducaoDaConstrucao& producao = simulacao.producaoDasConstrucoes[cozinheiro];
        conferir(producao.estaParada && producao.motivoDaParada == "sem Água" && simulacao.inventario.quantidadeDe(grao, DonoDoItem::Cidade) == 2,
                 "Cozinheiro com 2 Grãos e 0 Água: parado (\"sem Água\"), e o Grão continua lá");
    }
    {
        // O ciclo de Pão terminado com a capacidade cheia: parado, e o Pão não some.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        const int cozinheiro = indiceDaConstrucaoNaVila(vila.dados, "cozinheiro");
        ProducaoDaConstrucao& producao = simulacao.producaoDasConstrucoes[cozinheiro];
        producao.receitaDoCiclo = receitaDoPao;
        producao.cicloComecou = true;
        producao.progressoDoCiclo = 320'000;
        simulacao.inventario.definirCapacidadeDaCategoria(CategoriaDoItem::BemDeConsumo, simulacao.inventario.quantidadeNaCategoria(CategoriaDoItem::BemDeConsumo));
        const int paesAntes = simulacao.inventario.quantidadeTotalDe(pao);
        ContextoDaProducao contexto = simulacao.contextoDaProducao();
        rodarOsCiclosDaHora(contexto, simulacao.relogio);
        conferir(producao.estaParada && producao.motivoDaParada == "o estoque encheu" && producao.progressoDoCiclo == 320'000
                     && simulacao.inventario.quantidadeTotalDe(pao) == paesAntes,
                 "ciclo de Pão terminado com a capacidade cheia: parado (\"o estoque encheu\"), com o progresso guardado");
    }
    {
        // A Fazenda no inverno (25%): 4 ciclos de 2 Grãos dão 2 Grãos.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        const int fazenda = indiceDaConstrucaoNaVila(vila.dados, "fazenda");
        const RelogioDoMundo invernoAs10h(RegrasDoTempo{}, 48LL * 19'200 + 10 * 800);  // dia 48: o primeiro do Inverno
        const int graosAntes = simulacao.inventario.quantidadeTotalDe(grao);
        for (int hora = 0; hora < 4; ++hora) {
            for (const entt::entity npc : simulacao.npcsDaVila) {
                if (simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho == fazenda) {
                    simulacao.registroDasEntidades.get<TrabalhoDoDia>(npc).ticksTrabalhadosNestaHora = 400;  // 2 × meia hora = 1 ciclo
                }
            }
            ContextoDaProducao contexto = simulacao.contextoDaProducao();
            rodarOsCiclosDaHora(contexto, invernoAs10h);
        }
        conferir(invernoAs10h.estacaoAtual() == Estacao::Inverno && simulacao.inventario.quantidadeTotalDe(grao) - graosAntes == 2,
                 "Fazenda no inverno (25%), 4 ciclos de 2 Grãos: 2 Grãos no total (o resto guardado)");
    }
    {
        // A fila de ordens: manter 6 Móveis e 12 Tábuas, com 2 Móveis e 1 Tábua.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        const int carpintaria = indiceDaConstrucaoNaVila(vila.dados, "carpintaria_do_jogador");
        tirarTudoDoItem(simulacao, moveis);
        tirarTudoDoItem(simulacao, tabua);
        simulacao.inventario.tentarPor(moveis, DonoDoItem::Jogador, 2);
        simulacao.inventario.tentarPor(tabua, DonoDoItem::Jogador, 1);
        ContextoDaProducao contexto = simulacao.contextoDaProducao();
        conferir(escolherReceitaPelaFilaDeOrdens(contexto, carpintaria) == receitaDaTabua,
                 "ordens manter 6 Móveis e 12 Tábuas, com 2 Móveis e 1 Tábua: faz Tábua (não há 3 Tábuas)");
        simulacao.inventario.tentarPor(tabua, DonoDoItem::Jogador, 4);
        conferir(escolherReceitaPelaFilaDeOrdens(contexto, carpintaria) == receitaDosMoveis, "o mesmo, com 5 Tábuas: faz Móveis");

        // A compra da Tora: o jogador compra da Cidade a 110% (0,5 vira 0,6).
        ProducaoDaConstrucao& producao = simulacao.producaoDasConstrucoes[carpintaria];
        tirarTudoDoItem(simulacao, tabua);
        producao.receitaDoCiclo = -1;
        producao.cicloComecou = false;
        for (const entt::entity npc : simulacao.npcsDaVila) {
            if (simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho == carpintaria) {
                simulacao.registroDasEntidades.get<TrabalhoDoDia>(npc).ticksTrabalhadosNestaHora = 800;
            }
        }
        const int64_t dinheiroAntes = simulacao.dinheiro.dinheiroDoJogador;
        const int toraDaCidadeAntes = simulacao.inventario.quantidadeDe(tora, DonoDoItem::Cidade);
        rodarOsCiclosDaHora(contexto, simulacao.relogio);
        conferir(producao.receitaDoCiclo == receitaDaTabua && producao.cicloComecou && simulacao.dinheiro.dinheiroDoJogador == dinheiroAntes - 12
                     && simulacao.inventario.quantidadeDe(tora, DonoDoItem::Cidade) == toraDaCidadeAntes - 2,
                 "sem Tora, a Carpintaria do jogador compra 2 Toras da Cidade por 2 × 0,6 e começa a Tábua");
    }
    {
        // A escolha da cidade (marco 7, regra 10.1): o Posto de Coleta tira
        // Tora quando falta Tora e a demanda medida é de 8 por dia.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        const int posto = indiceDaConstrucaoNaVila(vila.dados, "posto_de_coleta");
        tirarTudoDoItem(simulacao, tora);
        std::vector<int64_t> consumoDeUmDia;
        for (int item = 0; item < vila.dados.catalogoDeItens.quantidadeDeItens(); ++item) {
            consumoDeUmDia.push_back(item == tora ? 800 : simulacao.consumoMedido.consumoPrevistoDoDiaEmCentesimos(item));
        }
        simulacao.consumoMedido.preencherOsDiasGuardados(consumoDeUmDia);
        ContextoDaProducao contexto = simulacao.contextoDaProducao();
        escolherAsReceitasDaCidade(contexto);
        conferir(simulacao.producaoDasConstrucoes[posto].receitaEscolhidaParaOProximoCiclo == receitaDaTora,
                 "Tora com 0 de estoque e demanda medida de 8 por dia: o Posto de Coleta escolhe a Tora");
        const int receitaDaPedra = *vila.dados.catalogoDeReceitas.procurarReceitaDoProduto(numeroDoItem(vila.dados, "item.pedra"));
        conferir(simulacao.producaoDasConstrucoes[posto].receitaEscolhidaParaOProximoCiclo != receitaDaPedra,
                 "o Posto nunca escolhe a Pedra: a vila não tem a área da pedreira (FL6-5)");
    }
}

void testarContasDaEconomia(const DadosDoJogo& dadosLidos)
{
    conferir(calcularSalarioEmDecimos(6'320, 100, 100, 100, 800) == 79, "salário de 6.320 ticks, Manufaturado, nível 1: 79 décimos");
    conferir(calcularSalarioEmDecimos(6'320, 100, 100, 150, 800) == 119, "o mesmo, gerente: 119 décimos (118,5, a metade sobe)");
    conferir(calcularSalarioEmDecimos(6'320, 100, 110, 100, 800) == 87, "o mesmo, nível 2: 87 décimos (86,9)");

    const int pao = numeroDoItem(dadosLidos, "item.pao");
    const int moveis = numeroDoItem(dadosLidos, "item.moveis_madeira");
    {
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        ContextoDaProducao contexto = simulacao.contextoDaProducao();
        const int64_t bancoAntes = simulacao.dinheiro.bancoDaCidade;
        consumirACestaDoDia(contexto);
        conferir(simulacao.inventario.quantidadeDe(pao, DonoDoItem::Cidade) == 60 && simulacao.dinheiro.bancoDaCidade - bancoAntes == 15 * 19,
                 "15 Civis e 75 Pães da Cidade: sobram 60, e a classe paga 15 × 1,9 ao Banco");
        tirarTudoDoItem(simulacao, pao);
        simulacao.inventario.tentarPor(pao, DonoDoItem::Cidade, 10);
        consumirACestaDoDia(contexto);
        conferir(simulacao.inventario.quantidadeTotalDe(pao) == 0 && simulacao.dinheiro.diasEmDeficitDeComida == 1,
                 "15 Civis e só 10 Pães: os 10 saem, e é 1 dia em déficit de comida");
        simulacao.inventario.tentarPor(pao, DonoDoItem::Cidade, 15);
        consumirACestaDoDia(contexto);
        conferir(simulacao.dinheiro.diasEmDeficitDeComida == 0, "no dia seguinte, com Pão para todos, os dias em déficit voltam a 0");
    }
    {
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        ContextoDaProducao contexto = simulacao.contextoDaProducao();
        simulacao.inventario.tentarPor(pao, DonoDoItem::Cidade, 100);
        simulacao.inventario.tentarPor(moveis, DonoDoItem::Jogador, 30);
        const int moveisAntes = simulacao.inventario.quantidadeTotalDe(moveis);
        for (int dia = 0; dia < 8; ++dia) {
            consumirACestaDoDia(contexto);
        }
        conferir(moveisAntes - simulacao.inventario.quantidadeTotalDe(moveis) == 15, "em 8 dias, 15 Civis gastam 15 Móveis");

        simulacao.dinheiro.dinheiroDaClasseCivil = 0;
        const int paesAntes = simulacao.inventario.quantidadeTotalDe(pao);
        const int64_t totalAntes = simulacao.dinheiro.dinheiroTotal();
        consumirACestaDoDia(contexto);
        conferir(paesAntes - simulacao.inventario.quantidadeTotalDe(pao) == 15 && simulacao.dinheiro.consumoNaoPagoHoje > 0
                     && simulacao.dinheiro.dinheiroTotal() == totalAntes,
                 "a classe sem dinheiro: a comida sai do mesmo jeito, e o consumo não pago é anotado");

        simulacao.dinheiro.bancoDaCidade = 0;
        for (const entt::entity npc : simulacao.npcsDaVila) {
            simulacao.registroDasEntidades.get<TrabalhoDoDia>(npc).ticksTrabalhadosHoje = 6'320;
        }
        const int64_t civisAntes = simulacao.dinheiro.dinheiroDaClasseCivil;
        pagarOsSalariosDoDia(contexto);
        const int cozinheiro = indiceDaConstrucaoNaVila(vila.dados, "cozinheiro");
        const int carpintaria = indiceDaConstrucaoNaVila(vila.dados, "carpintaria_do_jogador");
        conferir(simulacao.producaoDasConstrucoes[cozinheiro].diasDeSalarioAtrasado == 1 && simulacao.producaoDasConstrucoes[carpintaria].diasDeSalarioAtrasado == 0
                     && simulacao.dinheiro.rendaDoDiaDaClasseCivil > 0
                     && simulacao.dinheiro.dinheiroDaClasseCivil - civisAntes == simulacao.dinheiro.rendaDoDiaDaClasseCivil - simulacao.dinheiro.impostoDeHoje
                     && simulacao.dinheiro.bancoDaCidade == simulacao.dinheiro.impostoDeHoje,
                 "o Banco sem dinheiro: os empregados da Cidade não recebem (1 dia de atraso), e os do jogador recebem (menos o imposto, que vai para o Banco)");
    }
    {
        ListaDeProblemasDosDados problemas;
        lerArquivoDeRegras(R"({"ordem_do_dia": ["consumo", "festa"]})", problemas);
        conferir(problemas.algumErroContem("\"festa\""), "uma ordem do dia com um passo desconhecido: erro no regras.json");
    }
}

// Roda a vila da fatia sem tela, como o laço do jogo, por alguns dias.
struct ResultadoDeDiasSemTela {
    std::vector<std::string> resumos;
    std::vector<int> paesFeitosEmCadaDia;
    int menorEstoqueDePao = 1'000'000;
    int bracaisNoCampoAs10h = 0;
    int empregadosTrabalhandoAs10h = 0;
    bool dinheiroTotalSoMudouPeloMercador = true;
    int64_t bancoNoFim = 0;
    int estoqueDePaoNoFim = 0;
};

ResultadoDeDiasSemTela simularDiasSemTela(int quantidadeDeDias)
{
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    ResultadoDeDiasSemTela resultado;
    const int pao = numeroDoItem(dados, "item.pao");
    const int cozinheiro = indiceDaConstrucaoNaVila(dados, "cozinheiro");
    const int64_t dinheiroDoComeco = simulacao.dinheiro.dinheiroTotal();
    const int64_t tickDoFim = 4'800 + static_cast<int64_t>(quantidadeDeDias) * 19'200;
    while (simulacao.relogio.tickAtual() < tickDoFim) {
        for (const std::string& linha : simulacao.avancarUmTick()) {
            if (linha.rfind("Resumo", 0) == 0) {
                resultado.resumos.push_back(linha);
            }
        }
        resultado.menorEstoqueDePao = std::min(resultado.menorEstoqueDePao, simulacao.inventario.quantidadeTotalDe(pao));
        resultado.dinheiroTotalSoMudouPeloMercador = resultado.dinheiroTotalSoMudouPeloMercador
            && simulacao.dinheiro.dinheiroTotal() - simulacao.dinheiro.dinheiroQueEntrouDeFora == dinheiroDoComeco;
        if ((simulacao.relogio.tickAtual() + 1) % 19'200 == 0) {  // o último tick do dia (23h59)
            const std::map<int, int>& feitosHoje = simulacao.producaoDasConstrucoes[cozinheiro].producaoDeHojePorItem;
            resultado.paesFeitosEmCadaDia.push_back(feitosHoje.count(pao) > 0 ? feitosHoje.at(pao) : 0);
        }
        if (simulacao.relogio.tickAtual() == 10 * 800) {
            ContextoDaProducao contexto = simulacao.contextoDaProducao();
            for (const entt::entity npc : simulacao.npcsDaVila) {
                const int trabalho = simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
                const bool trabalhando = npcEstaTrabalhandoAgora(contexto, npc);
                resultado.empregadosTrabalhandoAs10h += trabalhando ? 1 : 0;
                resultado.bracaisNoCampoAs10h += trabalhando && trabalho >= 0 && !simulacao.producaoDasConstrucoes[trabalho].tilesDeTrabalhoNoCampo.empty() ? 1 : 0;
            }
        }
    }
    resultado.bancoNoFim = simulacao.dinheiro.bancoDaCidade;
    resultado.estoqueDePaoNoFim = simulacao.inventario.quantidadeTotalDe(pao);
    return resultado;
}

void testarDiasSemTela()
{
    const ResultadoDeDiasSemTela resultado = simularDiasSemTela(3);
    for (const std::string& resumo : resultado.resumos) {
        std::printf("    %s\n", resumo.c_str());
    }
    std::printf("    (às 10h: %d trabalhando, %d Braçais no campo; Pães feitos nos dias 1, 2 e 3: %d, %d e %d; o menor estoque de Pão: %d)\n",
                resultado.empregadosTrabalhandoAs10h, resultado.bracaisNoCampoAs10h, resultado.paesFeitosEmCadaDia.at(0), resultado.paesFeitosEmCadaDia.at(1),
                resultado.paesFeitosEmCadaDia.at(2), resultado.menorEstoqueDePao);
    conferir(resultado.resumos.size() == 3, "3 dias sem tela: um resumo por dia no console");
    conferir(resultado.empregadosTrabalhandoAs10h == 12 && resultado.bracaisNoCampoAs10h == 4,
             "às 10h, os 12 empregados trabalham, e os 4 Braçais estão no campo");
    // O primeiro dia fecha só 3 ciclos (12 Pães): a sobra do quarto passa para o dia seguinte.
    conferir(resultado.paesFeitosEmCadaDia.at(1) + resultado.paesFeitosEmCadaDia.at(2) >= 30,
             "nos dias 2 e 3, o Cozinheiro faz pelo menos 15 Pães por dia (15,8 em média, FL6-7)");
    conferir(resultado.menorEstoqueDePao > 0, "a vila nunca fica sem Pão");
    conferir(resultado.dinheiroTotalSoMudouPeloMercador,
             "o dinheiro total (Banco, jogador e Civis) só muda pelo Mercador viajante: nada some e nada aparece do nada");
    // A repetição (a mesma semente, o mesmo resultado) é conferida com 16 dias nos testes da simulação (marco 7).
}

}  // namespace

void testarProducaoEEconomia()
{
    std::printf("Produção, inventário e economia\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    testarConstrucoes(dados);
    testarInventario(dados);
    testarContasDaProducao(dados);
    testarRegrasDaProducao(dados);
    testarContasDaEconomia(dados);
    testarDiasSemTela();
}
