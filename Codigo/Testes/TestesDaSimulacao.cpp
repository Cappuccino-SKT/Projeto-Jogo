// Os testes do marco 7: as tabelas de testes da parte 10 da Economia e da
// Produção, e das fichas da Felicidade, dos Eventos e da Simulação sem tela.

#include <cstdio>
#include <string>
#include <vector>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Economia/ConsumoESalarios.h"
#include "Economia/FelicidadeDaClasse.h"
#include "Economia/MercadoLocal.h"
#include "Eventos/EventosDaVila.h"
#include "Simulacao/RelatorioDaSimulacao.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDaFatia = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

constexpr int64_t ticksDeUmDia = 19'200;
constexpr int ticksPorHora = 800;

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

// Uma vila da fatia montada, com uma cópia própria dos dados (para trocar os eventos).
struct VilaDeTeste {
    DadosDoJogo dados;
    SimulacaoDaVila simulacao;
    explicit VilaDeTeste(DadosDoJogo dadosLidos) : dados(std::move(dadosLidos)), simulacao(dados, RegrasDoTempo{}) {}

    ContextoDaProducao contexto() { return simulacao.contextoDaProducao(); }

    void deixarOEstoque(int item, DonoDoItem dono, int quantidade)
    {
        simulacao.inventario.tentarTirar(item, dono, simulacao.inventario.quantidadeDe(item, dono));
        simulacao.inventario.tentarPor(item, dono, quantidade);
    }
    void tirarTudo(int item)
    {
        deixarOEstoque(item, DonoDoItem::Cidade, 0);
        deixarOEstoque(item, DonoDoItem::Jogador, 0);
    }
    // O consumo previsto de um item, nos 5 dias guardados (os outros ficam como estão).
    void definirOConsumoPrevisto(int item, int64_t centesimosPorDia)
    {
        std::vector<int64_t> consumo;
        for (int outro = 0; outro < dados.catalogoDeItens.quantidadeDeItens(); ++outro) {
            consumo.push_back(outro == item ? centesimosPorDia : simulacao.consumoMedido.consumoPrevistoDoDiaEmCentesimos(outro));
        }
        simulacao.consumoMedido.preencherOsDiasGuardados(consumo);
    }
    // Só os eventos pedidos, na ordem pedida (para testar um de cada vez).
    void deixarSoOsEventos(const std::vector<std::string>& identificadores)
    {
        CatalogoDeEventos escolhidos;
        for (const std::string& identificador : identificadores) {
            for (const DefinicaoDeEvento& evento : dados.catalogoDeEventos.todosOsEventos()) {
                if (evento.identificadorDeTexto == identificador) {
                    escolhidos.acrescentarEvento(evento);
                }
            }
        }
        dados.catalogoDeEventos = escolhidos;
        ContextoDaProducao contextoDosEventos = contexto();
        comecarOsEventos(contextoDosEventos);
    }
    void avancarTicks(int64_t quantidade)
    {
        for (int64_t tick = 0; tick < quantidade; ++tick) {
            simulacao.avancarUmTick();
        }
    }
    std::vector<entt::entity> empregadosDe(const std::string& identificadorDaConstrucao)
    {
        std::vector<entt::entity> empregados;
        const int indice = indiceDaConstrucaoNaVila(dados, identificadorDaConstrucao);
        for (const entt::entity npc : simulacao.npcsDaVila) {
            if (simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho == indice) {
                empregados.push_back(npc);
            }
        }
        return empregados;
    }
};

// O relógio à meia-noite (começo) de um dia da partida, contando do 0.
RelogioDoMundo relogioNoComecoDoDia(int64_t diaDaPartida, int hora = 0)
{
    return RelogioDoMundo(RegrasDoTempo{}, diaDaPartida * ticksDeUmDia + static_cast<int64_t>(hora) * ticksPorHora);
}

void testarMercadoLocal(const DadosDoJogo& dados)
{
    const RegrasDoMercadoLocal& regras = dados.regras.economia.mercadoLocal;
    const int fator = calcularFatorDoMercadoLocal(45, 7'500, regras);
    const int desejado = calcularPrecoDesejado(17, fator);
    conferir(fator == 140 && desejado == 24 && darOPassoDoPrecoLocal(17, desejado, regras) == 19,
             "Pão com 45 no estoque, alvo 75, preço 1,7: fator 140; desejado 2,38 (2,4); o preço vai a 1,9");
    int preco = 17;
    for (int dia = 0; dia < 4; ++dia) {
        preco = darOPassoDoPrecoLocal(preco, desejado, regras);
    }
    conferir(preco == 24, "o mesmo, 4 dias seguidos: 1,9, 2,1, 2,3 e 2,4 (sem passar do desejado)");

    const int fatorComMuitoPao = calcularFatorDoMercadoLocal(200, 7'500, regras);
    int precoComMuitoPao = 17;
    for (int dia = 0; dia < 20; ++dia) {
        precoComMuitoPao = darOPassoDoPrecoLocal(precoComMuitoPao, calcularPrecoDesejado(17, fatorComMuitoPao), regras);
    }
    conferir(fatorComMuitoPao == 50 && darOPassoDoPrecoLocal(17, 9, regras) == 15 && precoComMuitoPao == 9,
             "Pão com 200 no estoque, alvo 75: fator 50 (preso); o preço desce 10% por dia até 0,9 (0,85 arredondado)");
    conferir(calcularFatorDoMercadoLocal(150, 0, regras) == 100, "Bloco de Pedra sem consumo (alvo 0): fator 100, o preço fica no base");
    conferir(darOPassoDoPrecoLocal(5, 9, regras) == 6, "o passo mínimo é 1 décimo (10% de 0,5 daria 0,05)");
}

void testarGastoExtra(const DadosDoJogo& dadosLidos)
{
    const int pao = numeroDoItem(dadosLidos, "item.pao");
    const int moveis = numeroDoItem(dadosLidos, "item.moveis_madeira");
    {
        // O exemplo da regra 10.5: renda de 85, o Pão a 2,1 na loja, os Móveis a 5,1.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        simulacao.mercadoLocal.precoLocalPorItemEmDecimos[pao] = 19;  // na loja: 2,1
        simulacao.mercadoLocal.precoLocalPorItemEmDecimos[moveis] = 51;
        vila.deixarOEstoque(moveis, DonoDoItem::Jogador, 10);
        simulacao.dinheiro.rendaDoDiaDaClasseCivil = 850;
        simulacao.dinheiro.materiaisDevidosEmOitavos = 1;  // com os 15 de hoje, a cesta tira 2 Móveis
        const int64_t bancoAntes = simulacao.dinheiro.bancoDaCidade;
        const int64_t jogadorAntes = simulacao.dinheiro.dinheiroDoJogador;
        ContextoDaProducao contexto = vila.contexto();
        consumirACestaDoDia(contexto);
        conferir(simulacao.dinheiro.gastoDaCestaDeHoje == 417 && simulacao.dinheiro.gastoExtraDeHoje == 263
                     && simulacao.inventario.quantidadeDe(moveis, DonoDoItem::Jogador) == 6 && simulacao.dinheiro.dinheiroDoJogador - jogadorAntes == 4 * 51
                     && simulacao.dinheiro.bancoDaCidade - bancoAntes == 15 * 21 + 161,
                 "renda de 85 e cesta de 41,7 com 2 Móveis: 2 Móveis a mais (10,2 para o jogador) e 16,1 para o Banco");
    }
    {
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        simulacao.dinheiro.rendaDoDiaDaClasseCivil = 850;
        simulacao.dinheiro.dinheiroDaClasseCivil = 0;
        ContextoDaProducao contexto = vila.contexto();
        consumirACestaDoDia(contexto);
        conferir(simulacao.dinheiro.gastoExtraDeHoje == 0, "a classe sem dinheiro: não há gasto extra");
    }
}

void testarSalariosEImposto(const DadosDoJogo& dadosLidos)
{
    const RegrasDosMultiplicadoresDoSalario& multiplicadores = dadosLidos.regras.economia.multiplicadoresDoSalario;
    conferir(calcularMultiplicadorDaRiqueza(1'800, 15, 203, 8, multiplicadores) == 98 && calcularMultiplicadorDoDesemprego(3, 15, multiplicadores) == 90,
             "riqueza média de 12,0 com a cesta de 2,54 por dia: riqueza 98; 3 desempregados em 15: desemprego 90");
    conferir(calcularMultiplicadorDaRiqueza(1'000'000, 15, 203, 8, multiplicadores) == 120 && calcularMultiplicadorDaRiqueza(0, 15, 203, 8, multiplicadores) == 80
                 && calcularMultiplicadorDoDesemprego(0, 15, multiplicadores) == 110 && calcularMultiplicadorDoDesemprego(10, 15, multiplicadores) == 90,
             "os multiplicadores ficam presos: riqueza de 80 a 120, desemprego de 90 a 110");
    const int64_t salario = calcularSalarioEmDecimos(6'320, 100, 100, 100, 800, 98, 90);
    conferir(salario == 70 && calcularImpostoDoSalario(salario, dadosLidos.regras.economia.imposto) == 14,
             "o Cozinheiro com 6.320 ticks, riqueza 98 e desemprego 90: 70 décimos (69,7), com 14 de imposto");
    conferir(calcularImpostoDoSalario(79, dadosLidos.regras.economia.imposto) == 16, "o imposto arredonda por salário, a metade subindo (15,8 vira 16)");

    {
        // A cesta de um Civil em 8 dias: 8 Pães a 1,9 (na loja) e 1 Móvel a 5,1.
        VilaDeTeste vila(dadosLidos);
        vila.simulacao.mercadoLocal.precoLocalPorItemEmDecimos[numeroDoItem(dadosLidos, "item.moveis_madeira")] = 51;
        conferir(calcularCestaDeUmCivilEmDecimos(vila.contexto()) == 203, "a cesta de 8 dias de um Civil, com o Pão a 1,9 e o Móvel a 5,1: 20,3");
    }
    {
        // Regra 10.8: com 16 dias de atraso, os empregados saem.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        const int cozinheiro = indiceDaConstrucaoNaVila(vila.dados, "cozinheiro");
        const std::vector<entt::entity> cozinheiros = vila.empregadosDe("cozinheiro");
        simulacao.dinheiro.bancoDaCidade = 0;
        simulacao.producaoDasConstrucoes[cozinheiro].diasDeSalarioAtrasado = 15;
        for (const entt::entity npc : cozinheiros) {
            simulacao.registroDasEntidades.get<TrabalhoDoDia>(npc).ticksTrabalhadosHoje = 6'320;
        }
        ContextoDaProducao contexto = vila.contexto();
        const std::vector<std::string> avisos = pagarOsSalariosDoDia(contexto);
        bool todosSairam = !cozinheiros.empty();
        for (const entt::entity npc : cozinheiros) {
            todosSairam = todosSairam && simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho < 0;
        }
        conferir(todosSairam && simulacao.producaoDasConstrucoes[cozinheiro].diasDeSalarioAtrasado == 0 && avisos.size() == 1,
                 "o Cozinheiro chega a 16 dias de atraso: os 2 empregados viram desempregados, e sai um aviso no console");
    }
    {
        ListaDeProblemasDosDados problemas;
        const RegrasDoJogo regras = lerArquivoDeRegras(R"({"ordem_do_dia": ["mercado_local", "consumo"]})", problemas);
        conferir(regras.economia.ordemDoDia.size() == 2 && regras.economia.ordemDoDia[0] == PassoDaOrdemDoDia::MercadoLocal
                     && !problemas.algumErroContem("ordem_do_dia"),
                 "uma ordem do dia com \"mercado_local\" antes de \"consumo\": roda nessa ordem (a ordem é do arquivo)");
    }
}

void testarEstabilizador(const DadosDoJogo& dadosLidos)
{
    VilaDeTeste vila(dadosLidos);
    SimulacaoDaVila& simulacao = vila.simulacao;
    simulacao.mercadoLocal.precoLocalPorItemEmDecimos[numeroDoItem(dadosLidos, "item.moveis_madeira")] = 51;  // a cesta: 2,54 por dia
    simulacao.dinheiro.bancoDaCidade = 4'000;
    simulacao.dinheiro.dinheiroDaClasseCivil = 900;  // 6,0 em média
    simulacao.dinheiro.bancoNoComecoDaEstacao = 4'000;
    simulacao.dinheiro.ajudaDoEstabilizadorNaEstacao = 0;
    ContextoDaProducao contexto = vila.contexto();
    rodarOEstabilizador(contexto, relogioNoComecoDoDia(1));  // o dia 2 da Primavera
    conferir(simulacao.dinheiro.ajudaDoEstabilizadorHoje == 400 && simulacao.dinheiro.bancoDaCidade == 3'600 && simulacao.dinheiro.dinheiroDaClasseCivil == 1'300,
             "a riqueza média em 6,0 (abaixo de 3 dias da cesta): o Banco paga 40, o teto da estação (10% de 400)");
    rodarOEstabilizador(contexto, relogioNoComecoDoDia(2));
    conferir(simulacao.dinheiro.ajudaDoEstabilizadorHoje == 0, "no dia seguinte, com o teto da estação gasto, o Banco não paga mais nada");
    simulacao.dinheiro.dinheiroDaClasseCivil = 900;
    rodarOEstabilizador(contexto, relogioNoComecoDoDia(16));  // o dia 1 do Verão: o teto recomeça
    conferir(simulacao.dinheiro.bancoNoComecoDaEstacao == 4'350 && simulacao.dinheiro.ajudaDoEstabilizadorHoje == 435,
             "no dia 1 da estação seguinte, entra a verba da Coroa (360 + 75 = 435) e o teto recomeça: 10% de 435 = 43,5 (FE4-1)");
}

void testarFelicidade(const DadosDoJogo& dados)
{
    const RegrasDaFelicidade& regras = dados.regras.felicidade;
    SituacaoDoDiaParaAFelicidade primeiroDia;
    primeiroDia.quantidadeDeCivis = 15;
    primeiroDia.civisDesempregados = 3;
    const FatoresDaFelicidade fatores = calcularFatoresDaFelicidade(primeiroDia, regras);
    conferir(fatores.consumoAtendido == 0 && fatores.desempregoERenda == 60 && fatores.moradia == 0 && fatores.eventosDuradouros == 0
                 && calcularAlvoDaFelicidade(fatores, regras) == 560,
             "os fatores do primeiro dia da fatia: consumo 0, desemprego +6, moradia 0, eventos 0; alvo 56");
    conferir(darOPassoDaFelicidade(500, 560, regras) == 506, "Felicidade 50,0 e alvo 56,0: 50,6");
    conferir(darOPassoDaFelicidade(586, 587, regras) == 587 && darOPassoDaFelicidade(588, 587, regras) == 587,
             "Felicidade 58,6 e alvo 58,7: 58,7 (chega ao alvo, dos dois lados)");

    SituacaoDoDiaParaAFelicidade fome = primeiroDia;
    fome.civisQueFicaramSemComida = 5;
    conferir(calcularFatoresDaFelicidade(fome, regras).consumoAtendido == -83, "5 de 15 sem comida: consumo −8,3");
    SituacaoDoDiaParaAFelicidade gastoGrande = primeiroDia;
    gastoGrande.gastoDaCestaDeHoje = 440;
    gastoGrande.gastoExtraDeHoje = 1'320;
    SituacaoDoDiaParaAFelicidade diaComum = primeiroDia;
    diaComum.gastoDaCestaDeHoje = 440;
    diaComum.gastoExtraDeHoje = 240;
    conferir(calcularFatoresDaFelicidade(gastoGrande, regras).consumoAtendido == 50 && calcularFatoresDaFelicidade(diaComum, regras).consumoAtendido == 27,
             "gasto extra 3 vezes a cesta: o pedaço fica em +5; 24 sobre 44: +2,7");
    SituacaoDoDiaParaAFelicidade festa = primeiroDia;
    festa.felicidadeDosEventosEmPontos = 30;
    conferir(calcularFatoresDaFelicidade(festa, regras).eventosDuradouros == 200, "eventos somando +30: +20 (preso)");
    FatoresDaFelicidade muitoRuins;
    muitoRuins.consumoAtendido = -600;
    conferir(calcularAlvoDaFelicidade(muitoRuins, regras) == 0, "alvo abaixo de 0: 0");
    conferir(nomeDaFaixaDaFelicidade(587) == "Estável" && nomeDaFaixaDaFelicidade(701) == "Contente" && nomeDaFaixaDaFelicidade(377) == "Inquieta"
                 && nomeDaFaixaDaFelicidade(150) == "Protesto" && nomeDaFaixaDaFelicidade(99) == "Revolta",
             "as faixas: 58,7 Estável, 70,1 Contente, 37,7 Inquieta, 15 Protesto, 9,9 Revolta");
}

void testarEventos(const DadosDoJogo& dadosLidos)
{
    conferir(dadosLidos.catalogoDeEventos.quantidadeDeEventos() == 13 && !dadosLidos.listaDeProblemas.temErro(), "o eventos.json: 13 eventos (com o Inverno rigoroso e a Epidemia), sem erro");
    {
        ListaDeProblemasDosDados problemas;
        const CatalogosParaOsEventos catalogos{dadosLidos.tabelaDeTextos, dadosLidos.catalogoDeItens, dadosLidos.catalogoDeConstrucoes};
        lerArquivoDeEventos(R"([{"id": "evento.ratos_no_celeiro", "tipo": "ocasional", "efeitos": [{"mudar_estoque": "item.ouro", "porcentagem": -10}]},
                               {"id": "evento.seca", "tipo": "ocasional", "estacoes": ["monção"], "efeitos": [{"felicidade": -1}]}])",
                            catalogos, problemas);
        conferir(problemas.algumErroContem("item.ouro") && problemas.algumErroContem("monção"), "um evento com o item.ouro ou com a estação \"monção\": erro");
    }
    const int grao = numeroDoItem(dadosLidos, "item.grao");
    const int agua = numeroDoItem(dadosLidos, "item.agua");
    const int pao = numeroDoItem(dadosLidos, "item.pao");
    {
        // A Seca só sai no Verão (com a chance de 100%, para o teste não depender do sorteio).
        VilaDeTeste vila(dadosLidos);
        vila.deixarSoOsEventos({"evento.seca"});
        vila.dados.catalogoDeEventos = [&] {
            CatalogoDeEventos catalogo;
            DefinicaoDeEvento seca = vila.dados.catalogoDeEventos.todosOsEventos()[0];
            seca.chancePorDiaEmPorMil = 1000;
            catalogo.acrescentarEvento(seca);
            return catalogo;
        }();
        ContextoDaProducao contexto = vila.contexto();
        rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(50));  // Inverno
        const bool saiuNoInverno = vila.simulacao.eventos.eventoEstaAtivo(0);
        rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(20));  // Verão
        conferir(!saiuNoInverno && vila.simulacao.eventos.eventoEstaAtivo(0) && multiplicadorDaProducaoPorEventos(contexto, "construcao.fazenda") == 50,
                 "a Seca no Inverno não pode sair (a estação não bate); no Verão, sai e a Fazenda produz 50%");
        for (int dia = 21; dia <= 28; ++dia) {
            rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(dia));
        }
        conferir(!vila.simulacao.eventos.eventoEstaAtivo(0) && vila.simulacao.eventos.diaEmQuePodeSairDeNovo[0] == 28 + 32,
                 "a Seca dura 8 dias e, no fim, começa o cooldown de 32 dias");
    }
    {
        // A Seca ativa num ciclo de 2 Grãos, no Verão; e a Seca com a Chuva boa juntas.
        VilaDeTeste vila(dadosLidos);
        vila.deixarSoOsEventos({"evento.seca", "evento.chuva_boa"});
        SimulacaoDaVila& simulacao = vila.simulacao;
        const RelogioDoMundo veraoAs10h = relogioNoComecoDoDia(20, 10);
        const auto umaHoraDeFazenda = [&] {
            for (const entt::entity npc : vila.empregadosDe("fazenda")) {
                simulacao.registroDasEntidades.get<TrabalhoDoDia>(npc).ticksTrabalhadosNestaHora = 400;  // 2 × meia hora = 1 ciclo
            }
            ContextoDaProducao contexto = vila.contexto();
            rodarOsCiclosDaHora(contexto, veraoAs10h);
        };
        const int fazenda = indiceDaConstrucaoNaVila(vila.dados, "fazenda");
        simulacao.producaoDasConstrucoes[fazenda].restoDaProducaoBracal = 0;
        simulacao.eventos.eventosAtivos = {{0, 8}};
        int graosAntes = simulacao.inventario.quantidadeTotalDe(grao);
        umaHoraDeFazenda();
        conferir(simulacao.inventario.quantidadeTotalDe(grao) - graosAntes == 1, "a Seca ativa, num ciclo de 2 Grãos no Verão: 1 Grão (50%)");
        simulacao.producaoDasConstrucoes[fazenda].restoDaProducaoBracal = 0;
        simulacao.eventos.eventosAtivos = {{0, 8}, {1, 4}};
        graosAntes = simulacao.inventario.quantidadeTotalDe(grao);
        umaHoraDeFazenda();
        conferir(simulacao.inventario.quantidadeTotalDe(grao) - graosAntes == 1 && simulacao.producaoDasConstrucoes[fazenda].restoDaProducaoBracal == 20,
                 "a Seca e a Chuva boa juntas: 2 × 50% × 120% = 1,2 Grão: 1, com 20 de resto");
    }
    {
        // Dois negativos ativos: um terceiro negativo não pode sair, e um positivo pode.
        VilaDeTeste vila(dadosLidos);
        vila.deixarSoOsEventos({"evento.seca", "evento.febre", "evento.ratos_no_celeiro", "evento.achado_na_floresta"});
        CatalogoDeEventos catalogo = vila.dados.catalogoDeEventos;
        vila.dados.catalogoDeEventos = CatalogoDeEventos{};
        for (DefinicaoDeEvento evento : catalogo.todosOsEventos()) {
            evento.chancePorDiaEmPorMil = 1000;
            vila.dados.catalogoDeEventos.acrescentarEvento(evento);
        }
        vila.simulacao.eventos.eventosAtivos = {{0, 5}, {1, 3}};
        const int torasAntes = vila.simulacao.inventario.quantidadeTotalDe(numeroDoItem(dadosLidos, "item.tora"));
        ContextoDaProducao contexto = vila.contexto();
        const int graosAntes = vila.simulacao.inventario.quantidadeTotalDe(grao);
        rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(3));
        conferir(vila.simulacao.inventario.quantidadeTotalDe(grao) == graosAntes
                     && vila.simulacao.inventario.quantidadeTotalDe(numeroDoItem(dadosLidos, "item.tora")) == torasAntes + 20,
                 "dois negativos ativos: os Ratos não podem sair, e o Achado (positivo) sai e põe 20 Toras");
    }
    {
        // A Fome: abaixo de 10% do alvo por 3 dias, começa; com 25%, continua; com 31%, termina.
        VilaDeTeste vila(dadosLidos);
        vila.deixarSoOsEventos({"evento.fome"});
        vila.definirOConsumoPrevisto(pao, 1'500);  // alvo 75
        ContextoDaProducao contexto = vila.contexto();
        vila.deixarOEstoque(pao, DonoDoItem::Cidade, 7);
        for (int dia = 1; dia <= 2; ++dia) {
            rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(dia));
        }
        const bool comecouCedo = vila.simulacao.eventos.eventoEstaAtivo(0);
        rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(3));
        const bool comecouNoTerceiroDia = vila.simulacao.eventos.eventoEstaAtivo(0) && felicidadeDosEventosAtivos(contexto) == -10;
        vila.deixarOEstoque(pao, DonoDoItem::Cidade, 19);
        rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(4));
        const bool continuouCom25 = vila.simulacao.eventos.eventoEstaAtivo(0);
        vila.deixarOEstoque(pao, DonoDoItem::Cidade, 24);
        rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(5));
        conferir(!comecouCedo && comecouNoTerceiroDia && continuouCom25 && !vila.simulacao.eventos.eventoEstaAtivo(0),
                 "a comida abaixo de 10% do alvo por 3 dias: a Fome começa (Felicidade −10); com 25% do alvo, continua; com 32%, termina");
    }
    {
        // Os Ratos com 95 Grãos; o Mercador com Água de sobra.
        VilaDeTeste vila(dadosLidos);
        vila.deixarSoOsEventos({"evento.ratos_no_celeiro", "evento.mercador_viajante"});
        CatalogoDeEventos catalogo = vila.dados.catalogoDeEventos;
        vila.dados.catalogoDeEventos = CatalogoDeEventos{};
        for (DefinicaoDeEvento evento : catalogo.todosOsEventos()) {
            evento.chancePorDiaEmPorMil = 1000;
            vila.dados.catalogoDeEventos.acrescentarEvento(evento);
        }
        vila.deixarOEstoque(grao, DonoDoItem::Cidade, 95);
        vila.deixarOEstoque(agua, DonoDoItem::Cidade, 300);
        vila.definirOConsumoPrevisto(agua, 400);
        const int64_t bancoAntes = vila.simulacao.dinheiro.bancoDaCidade;
        const int64_t totalAntes = vila.simulacao.dinheiro.dinheiroTotal();
        ContextoDaProducao contexto = vila.contexto();
        rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(3));
        conferir(vila.simulacao.inventario.quantidadeDe(grao, DonoDoItem::Cidade) == 86, "os Ratos com 95 Grãos: ficam 86 (9 tirados, 9,5 arredondado para baixo)");
        conferir(vila.simulacao.inventario.quantidadeDe(agua, DonoDoItem::Cidade) == 280 && vila.simulacao.dinheiro.bancoDaCidade - bancoAntes == 90
                     && vila.simulacao.dinheiro.dinheiroTotal() - totalAntes == vila.simulacao.dinheiro.dinheiroQueEntrouDeFora,
                 "o Mercador com 300 Águas (75 dias de estoque): leva 20 a 90% de 0,5, e entram 9 moedas de fora no Banco");
    }
    {
        // A Febre: 2 Civis com emprego ficam em casa 4 dias.
        VilaDeTeste vila(dadosLidos);
        vila.deixarSoOsEventos({"evento.febre"});
        CatalogoDeEventos catalogo = vila.dados.catalogoDeEventos;
        DefinicaoDeEvento febre = catalogo.todosOsEventos()[0];
        febre.chancePorDiaEmPorMil = 1000;
        vila.dados.catalogoDeEventos = CatalogoDeEventos{};
        vila.dados.catalogoDeEventos.acrescentarEvento(febre);
        ContextoDaProducao contexto = vila.contexto();
        rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(3));
        entt::registry& registro = vila.simulacao.registroDasEntidades;
        int doentesComEmprego = 0;
        for (const auto [npc, doenca, casaETrabalho] : registro.view<DoencaDoNpc, CasaETrabalhoDoNpc>().each()) {
            doentesComEmprego += doenca.diasQueFaltam == 4 && casaETrabalho.indiceDoTrabalho >= 0 ? 1 : 0;
        }
        for (int dia = 4; dia <= 7; ++dia) {
            rodarOsEventosDoDia(contexto, relogioNoComecoDoDia(dia));
        }
        conferir(doentesComEmprego == 2 && registro.view<DoencaDoNpc>().empty(), "a Febre: 2 Civis com emprego ficam doentes por 4 dias, e depois voltam");
    }
    {
        // O doente fica em casa (a ação Dormir) e não soma horas.
        VilaDeTeste vila(dadosLidos);
        const entt::entity cozinheira = vila.empregadosDe("cozinheiro").front();
        vila.simulacao.registroDasEntidades.emplace<DoencaDoNpc>(cozinheira, DoencaDoNpc{4});
        vila.avancarTicks(4 * ticksPorHora);  // das 6h às 10h
        conferir(vila.simulacao.registroDasEntidades.get<DecisaoDoNpc>(cozinheira).acaoAtual == AcaoDoNpc::Dormir
                     && vila.simulacao.registroDasEntidades.get<TrabalhoDoDia>(cozinheira).ticksTrabalhadosHoje == 0,
                 "às 10h, a Cozinheira doente está dormindo em casa e não somou nenhum tick de trabalho");
    }
}

void testarPrevisaoEFolga(const DadosDoJogo& dadosLidos)
{
    const int pao = numeroDoItem(dadosLidos, "item.pao");
    const int tora = numeroDoItem(dadosLidos, "item.tora");
    const int tabua = numeroDoItem(dadosLidos, "item.tabua");
    const int moveis = numeroDoItem(dadosLidos, "item.moveis_madeira");
    conferir(calcularDemandaQueFaltouNaHora(2, 1'600, 4, 800) == 100, "1 hora parada \"sem Tora\" com 2 empregados (1.600 ticks): 1 Tora de demanda");
    {
        // A fila do jogador sem Tora nem Tábua, 8 horas.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        vila.tirarTudo(tora);
        vila.tirarTudo(tabua);
        vila.tirarTudo(moveis);
        const int carpintaria = indiceDaConstrucaoNaVila(vila.dados, "carpintaria_do_jogador");
        ProducaoDaConstrucao& producao = simulacao.producaoDasConstrucoes[carpintaria];
        producao.receitaDoCiclo = -1;
        producao.cicloComecou = false;
        for (int hora = 0; hora < 8; ++hora) {
            for (const entt::entity npc : vila.empregadosDe("carpintaria_do_jogador")) {
                simulacao.registroDasEntidades.get<TrabalhoDoDia>(npc).ticksTrabalhadosNestaHora = 800;
            }
            ContextoDaProducao contexto = vila.contexto();
            rodarOsCiclosDaHora(contexto, relogioNoComecoDoDia(0, 8 + hora));
        }
        conferir(producao.estaParada && producao.motivoDaParada == "sem Tábua",
                 "a fila do jogador sem Tora nem Tábua: parada \"sem Tábua\" (a primeira ordem que quer produzir), e não \"sem ordem\"");
        conferir(simulacao.consumoMedido.consumoDeHojeEmCentesimos(tora) == 800 && simulacao.consumoMedido.consumoDeHojeEmCentesimos(tabua) == 1'200,
                 "a Carpintaria 8 horas parada com 2 empregados: 8 Toras (para a Tábua) e 12 Tábuas (para os Móveis) de demanda que faltou");
    }
    {
        // A folga do Cozinheiro: alvo 75 Pães.
        VilaDeTeste vila(dadosLidos);
        SimulacaoDaVila& simulacao = vila.simulacao;
        const int cozinheiro = indiceDaConstrucaoNaVila(vila.dados, "cozinheiro");
        vila.definirOConsumoPrevisto(pao, 1'500);
        ContextoDaProducao contexto = vila.contexto();
        vila.deixarOEstoque(pao, DonoDoItem::Cidade, 94);
        escolherAsReceitasDaCidade(contexto);
        const bool folgaCom94 = simulacao.producaoDasConstrucoes[cozinheiro].estaDeFolgaHoje;
        vila.deixarOEstoque(pao, DonoDoItem::Cidade, 79);
        escolherAsReceitasDaCidade(contexto);
        const bool folgaCom79 = simulacao.producaoDasConstrucoes[cozinheiro].estaDeFolgaHoje;
        vila.deixarOEstoque(pao, DonoDoItem::Cidade, 64);
        escolherAsReceitasDaCidade(contexto);
        conferir(folgaCom94 && folgaCom79 && !simulacao.producaoDasConstrucoes[cozinheiro].estaDeFolgaHoje,
                 "Pão com 94 (alvo 75): o Cozinheiro folga; com 79, continua de folga; com 64, volta a produzir");
    }
    {
        // De folga, os empregados passeiam e não recebem.
        VilaDeTeste vila(dadosLidos);
        const int cozinheiro = indiceDaConstrucaoNaVila(vila.dados, "cozinheiro");
        vila.simulacao.producaoDasConstrucoes[cozinheiro].estaDeFolgaHoje = true;
        vila.avancarTicks(4 * ticksPorHora);  // das 6h às 10h
        bool passeiamSemTrabalhar = true;
        for (const entt::entity npc : vila.empregadosDe("cozinheiro")) {
            passeiamSemTrabalhar = passeiamSemTrabalhar && vila.simulacao.registroDasEntidades.get<DecisaoDoNpc>(npc).acaoAtual == AcaoDoNpc::Passear
                && vila.simulacao.registroDasEntidades.get<TrabalhoDoDia>(npc).ticksTrabalhadosHoje == 0;
        }
        conferir(passeiamSemTrabalhar && vila.simulacao.producaoDasConstrucoes[cozinheiro].motivoDaParada == "de folga",
                 "o Cozinheiro de folga: às 10h, os 2 empregados passeiam e não somaram ticks (o salário é pelas horas)");
    }
}

void testarRelatorio(const DadosDoJogo& dados)
{
    const std::string cabecalho = escreverCabecalhoDoRelatorio(dados);
    conferir(cabecalho.rfind("dia;estacao;dia_da_estacao;grao;agua;tora;pao;tabua;moveis_madeira;pedra;bloco_de_pedra;preco_do_pao;banco;jogador", 0) == 0
                 && cabecalho.size() > 14 && cabecalho.substr(cabecalho.size() - 14) == "eventos_do_dia",
             "o cabeçalho do relatorio.csv: o dia, um estoque por item, o preço do Pão, o dinheiro... e os eventos do dia");
    LinhaDoRelatorio linha;
    linha.dia = 37;
    linha.estacao = Estacao::Outono;
    linha.diaDaEstacao = 5;
    linha.estoquePorItem = {240, 52};
    linha.precoDoPaoEmDecimos = 19;
    linha.bancoDaCidade = 4'023;
    linha.dinheiroDoJogador = 810;
    linha.riquezaMediaDosCivis = 121;
    linha.felicidadeEmDecimos = 584;
    linha.empregados = 12;
    linha.desempregados = 3;
    linha.eventosDoDia = {"Seca começou", "Ratos no celeiro"};
    conferir(escreverLinhaDoRelatorio(linha) == "37;Outono;5;240;52;1,9;402,3;81,0;12,1;58,4;12;3;0;0;0,0;limpo;0;0;0;0;;0;0;0;0;0;0;\"Seca começou, Ratos no celeiro\"",
             "uma linha do relatório: ponto e vírgula entre as colunas e vírgula nos decimais");

    ConferenciaDoTesteDeUmAno fome;
    LinhaDoRelatorio diaDeFome = linha;
    diaDeFome.diasEmDeficitDeComida = 4;
    conferirOsCriteriosDoDia(fome, diaDeFome);
    LinhaDoRelatorio bancoZerado = linha;
    bancoZerado.dia = 40;
    bancoZerado.bancoDaCidade = 0;
    conferirOsCriteriosDoDia(fome, bancoZerado);
    conferir(fome.falhou && fome.motivoDaFalha == "fome por 4 dias seguidos no dia 37", "4 dias seguidos sem comida: FALHOU (e fica o primeiro problema)");
    ConferenciaDoTesteDeUmAno banco;
    conferirOsCriteriosDoDia(banco, bancoZerado);
    conferir(banco.motivoDaFalha == "o Banco zerou no dia 40", "o Banco em 0: FALHOU");
    ConferenciaDoTesteDeUmAno precoAlto;
    LinhaDoRelatorio paoCaro = linha;
    paoCaro.precoDoPaoEmDecimos = 27;
    for (int dia = 0; dia < 7; ++dia) {
        conferirOsCriteriosDoDia(precoAlto, paoCaro);
    }
    const bool seteDiasPassam = !precoAlto.falhou;
    conferirOsCriteriosDoDia(precoAlto, paoCaro);
    ConferenciaDoTesteDeUmAno precoNoLimite;
    LinhaDoRelatorio paoNoLimite = linha;
    paoNoLimite.precoDoPaoEmDecimos = 26;
    conferirOsCriteriosDoDia(precoNoLimite, paoNoLimite);
    conferir(seteDiasPassam && precoAlto.motivoDaFalha == "o preço do Pão ficou acima de 2,6 por 8 dias seguidos (2,7 no dia 37)"
                 && !precoNoLimite.falhou,
             "EQ1-1: o Pão a 2,7 por 7 dias passa, por 8 dias FALHOU; a 2,6 (o limite), passa");

    // FE4-3: o Banco abaixo de 250, e a queda no ano.
    ConferenciaDoTesteDeUmAno bancoBaixo;
    LinhaDoRelatorio bancoEm249 = linha;
    bancoEm249.bancoDaCidade = 2'499;
    conferirOsCriteriosDoDia(bancoBaixo, bancoEm249);
    conferir(bancoBaixo.motivoDaFalha == "o Banco caiu para 249,9 no dia 37", "o Banco em 249,9: FALHOU (FE4-3)");
    std::vector<LinhaDoRelatorio> umAno(64, linha);
    umAno.front().bancoDaCidade = 4'000;
    umAno.back().bancoDaCidade = 3'500;
    ConferenciaDoTesteDeUmAno quedaNoLimite;
    conferirOBancoNoFimDoAno(quedaNoLimite, umAno);
    umAno.back().bancoDaCidade = 3'499;
    ConferenciaDoTesteDeUmAno quedaDemais;
    conferirOBancoNoFimDoAno(quedaDemais, umAno);
    conferir(!quedaNoLimite.falhou && quedaDemais.motivoDaFalha == "o Banco caiu de 400,0 para 349,9 no ano",
             "o Banco 50 abaixo do dia 1 no fim do ano passa; 50,1 abaixo, FALHOU (FE4-3)");
}

void testarSimulacaoSemTela()
{
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    const ResultadoDaSimulacaoSemTela primeira = rodarASimulacaoSemTela(dados, 16, 1, 0);
    const ResultadoDaSimulacaoSemTela segunda = rodarASimulacaoSemTela(dados, 16, 1, 0);
    std::printf("    (16 dias com a semente 1: %.1f segundos, %.1f microssegundos por tick)\n", primeira.segundosQueLevou, primeira.microssegundosPorTick);
    for (const std::string& linha : primeira.linhasDoConsole) {
        std::printf("    %s\n", linha.c_str());
    }
    conferir(primeira.linhas.size() == 16 && primeira.ticksRodados == 16 * ticksDeUmDia - 4'800, "16 dias sem tela: 16 linhas no relatório (a partida começa às 6h)");
    conferir(!primeira.conferencia.falhou, "16 dias sem tela com a semente 1: nenhum critério falha");
    conferir(primeira.textoDoRelatorio == segunda.textoDoRelatorio && primeira.linhas == segunda.linhas,
             "a mesma semente, 16 dias, duas vezes: os mesmos relatórios, linha por linha");

    // A matéria-prima nunca enche (a folga, regra 10.2): o maior estoque de Grão e Água juntos fica longe de 900.
    int maiorMateriaPrima = 0;
    for (const LinhaDoRelatorio& linha : primeira.linhas) {
        int materiaPrima = 0;
        for (const DefinicaoDeItem& item : dados.catalogoDeItens.todosOsItens()) {
            materiaPrima += item.categoriaDoItem == CategoriaDoItem::MateriaPrima ? linha.estoquePorItem[item.numeroInterno] : 0;
        }
        maiorMateriaPrima = std::max(maiorMateriaPrima, materiaPrima);
    }
    conferir(maiorMateriaPrima < 900 / 2, "em 16 dias, a matéria-prima nunca passa da metade da capacidade (a folga para os Braçais)");

    const ResultadoDaSimulacaoSemTela outraSemente = rodarASimulacaoSemTela(dados, 16, 2, 0);
    conferir(outraSemente.textoDoRelatorio != primeira.textoDoRelatorio, "semente 1 e semente 2: relatórios diferentes (os passeios e os eventos mudam)");
}

}  // namespace

void testarSimulacaoDeUmAno()
{
    std::printf("Marco 7: Mercado Local, gasto extra, imposto, Felicidade, eventos e o teste sem tela\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    testarMercadoLocal(dados);
    testarGastoExtra(dados);
    testarSalariosEImposto(dados);
    testarEstabilizador(dados);
    testarFelicidade(dados);
    testarEventos(dados);
    testarPrevisaoEFolga(dados);
    testarRelatorio(dados);
    testarSimulacaoSemTela();
}
