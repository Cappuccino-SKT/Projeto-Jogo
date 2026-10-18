// Os testes da vila inicial, da rotina e da Utilidade: a tabela da parte 8 da
// Ficha Lógica: IA (marco 5).

#include <cstdio>
#include <string>
#include <vector>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Mapa/BuscaDeCaminho.h"
#include "Npc/MovimentoDosNpcs.h"
#include "Npc/RotinaDosNpcs.h"
#include "Tempo/SorteioComSemente.h"

namespace {

const std::string pastaDosDadosDaFatia = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

void testarSorteio()
{
    // Os 6 primeiros números do programa de demonstração do PCG32 (semente 42, sequência 54).
    SorteioComSemente sorteioDeReferencia(42, 54);
    const uint32_t esperados[6] = {0xa15c02b7u, 0x7b47f409u, 0xba1d3330u, 0x83d2f293u, 0xbfa4784bu, 0xcbed606eu};
    bool todosIguais = true;
    for (const uint32_t esperado : esperados) {
        todosIguais = todosIguais && sorteioDeReferencia.proximoNumero() == esperado;
    }
    conferir(todosIguais, "o PCG32 com a semente 42 dá os mesmos números do programa de demonstração do autor");

    SorteioComSemente primeiro(1, sequenciaDoSorteioDosNpcs);
    SorteioComSemente segundo(1, sequenciaDoSorteioDosNpcs);
    bool mesmaLista = true;
    bool dentroDaFaixa = true;
    bool saiuOMenor = false;
    bool saiuOMaior = false;
    for (int vez = 0; vez < 1000; ++vez) {
        const int numero = primeiro.sortearNumeroInteiroEntre(15, 45);
        mesmaLista = mesmaLista && numero == segundo.sortearNumeroInteiroEntre(15, 45);
        dentroDaFaixa = dentroDaFaixa && numero >= 15 && numero <= 45;
        saiuOMenor = saiuOMenor || numero == 15;
        saiuOMaior = saiuOMaior || numero == 45;
    }
    conferir(mesmaLista, "dois sorteios com a mesma semente dão a mesma lista");
    conferir(dentroDaFaixa && saiuOMenor && saiuOMaior, "o sorteio de 15 a 45 fica na faixa e chega às duas pontas");
}

void testarPeriodosENotas()
{
    const RegrasDaRotina rotina;
    const RegrasDaUtilidade utilidade;
    conferir(descobrirPeriodoDaRotina(23, rotina) == PeriodoDaRotina::Dormir && descobrirPeriodoDaRotina(3, rotina) == PeriodoDaRotina::Dormir
                 && descobrirPeriodoDaRotina(6, rotina) == PeriodoDaRotina::Livre && descobrirPeriodoDaRotina(7, rotina) == PeriodoDaRotina::Trabalhar
                 && descobrirPeriodoDaRotina(14, rotina) == PeriodoDaRotina::Trabalhar && descobrirPeriodoDaRotina(15, rotina) == PeriodoDaRotina::Livre
                 && descobrirPeriodoDaRotina(22, rotina) == PeriodoDaRotina::Dormir,
             "rotina Civil: 23h e 3h dormir, 6h livre, 7h e 14h trabalhar, 15h livre, 22h dormir");

    SituacaoDoNpc almoco;
    almoco.periodo = PeriodoDaRotina::Trabalhar;
    almoco.temEmprego = true;
    almoco.estaNaHoraDeComer = estaNaHoraDeComer(12, 10, utilidade);
    const NotasDasAcoes notasDoAlmoco = calcularNotasDasAcoes(almoco, utilidade);
    conferir(notasDoAlmoco.comer == 150 && notasDoAlmoco.trabalhar == 100 && escolherAcaoPelaUtilidade(notasDoAlmoco) == AcaoDoNpc::Comer,
             "empregado às 12h10: Comer (150) ganha de Trabalhar (100)");
    conferir(!estaNaHoraDeComer(12, 30, utilidade) && estaNaHoraDeComer(19, 29, utilidade), "a refeição vai da hora cheia até 29 minutos depois");

    SituacaoDoNpc desempregado;
    desempregado.periodo = PeriodoDaRotina::Trabalhar;
    conferir(escolherAcaoPelaUtilidade(calcularNotasDasAcoes(desempregado, utilidade)) == AcaoDoNpc::Passear, "desempregado às 10h: Passear");

    SituacaoDoNpc depoisDoTrabalho;
    depoisDoTrabalho.periodo = PeriodoDaRotina::Livre;
    depoisDoTrabalho.temEmprego = true;
    depoisDoTrabalho.ehDiaDeCompra = true;
    const NotasDasAcoes notasDaCompra = calcularNotasDasAcoes(depoisDoTrabalho, utilidade);
    conferir(notasDaCompra.comprar == 120 && escolherAcaoPelaUtilidade(notasDaCompra) == AcaoDoNpc::Comprar,
             "às 15h, no dia de compra, sem ter comprado: Comprar (120)");
    depoisDoTrabalho.jaComprouHoje = true;
    conferir(escolherAcaoPelaUtilidade(calcularNotasDasAcoes(depoisDoTrabalho, utilidade)) == AcaoDoNpc::Passear, "depois de comprar: Passear");

    NotasDasAcoes empate;
    empate.trabalhar = 100;
    empate.passear = 100;
    conferir(escolherAcaoPelaUtilidade(empate) == AcaoDoNpc::Trabalhar, "empate de Trabalhar e Passear: Trabalhar, pela ordem fixa");

    conferir(!ehDiaDeCompraDoNpc(0, 3, 2) && ehDiaDeCompraDoNpc(1, 3, 2) && !ehDiaDeCompraDoNpc(2, 3, 2) && ehDiaDeCompraDoNpc(3, 3, 2),
             "o NPC número 3 compra nos dias 1 e 3");
    conferir(converterMinutosEmTicks(30, 800) == 400 && converterMinutosEmTicks(15, 800) == 200, "30 minutos são 400 ticks; 15, são 200");
    conferir(calcularTicksEntreReavaliacoes(utilidade, 800) == 67, "5 minutos de jogo são 67 ticks (66,7 para cima)");
}

void testarVilaInicial()
{
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    const VilaDoJogo& vila = dados.vilaInicial;
    conferir(dados.dadosEstaoProntos(), "os dados da fatia, com a vila inicial, não têm erro");
    conferir(vila.construcoes.size() == 10 && vila.npcsIniciais.size() == 15 && vila.quantidadeDeEmpregados() == 12,
             "a vila da fatia: 10 construções, 15 NPCs e 12 empregados");
    bool lote5Livre = false;
    for (const LoteDeConstrucao& lote : dados.mapaDaVila.lotesDeConstrucao) {
        lote5Livre = lote5Livre || (lote.identificadorDoLote == 5 && lote.idDaConstrucaoQueOcupa == loteSemConstrucao);
    }
    conferir(lote5Livre, "o lote 5 fica livre");
    conferir(!dados.mapaDaVila.tileDaParaAndar(17, 21) && !dados.mapaDaVila.tileDaParaAndar(20, 24) && dados.mapaDaVila.tileDaParaAndar(19, 25),
             "os tiles do lote 2 ficam bloqueados, e a porta (19, 25) livre");
    conferir(vila.indiceDoMercado() >= 0 && vila.construcoes[vila.indiceDoMercado()].tileDaPorta == PosicaoDeTile{48, 25},
             "a porta do Mercado, no lote médio 9, é (48, 25)");
    conferir(montarResumoDosDados(dados).find("1 vila (10 construções e 15 NPCs)") != std::string::npos, "o resumo conta a vila");

    // Vilas com defeito, montadas aqui mesmo, sobre o mapa e o catálogo da fatia.
    const CatalogosParaAVila catalogos{dados.catalogoDeConstrucoes, dados.catalogoDeItens, dados.regras.construcoes};
    ListaDeProblemasDosDados problemasDoLoteRepetido;
    lerVilaInicial(R"({"semente_da_partida": 1, "construcoes": [
        {"id": "casa_1", "tipo": "construcao.casa", "lote": 2, "dono": "cidade"},
        {"id": "casa_2", "tipo": "construcao.casa", "lote": 2, "dono": "cidade"}], "npcs": []})",
                   dados.mapaDaVila, catalogos, problemasDoLoteRepetido);
    conferir(problemasDoLoteRepetido.algumErroContem("o lote 2 já tem"), "duas construções no lote 2: erro");

    ListaDeProblemasDosDados problemasDaCasaErrada;
    lerVilaInicial(R"({"semente_da_partida": 1, "construcoes": [
        {"id": "fazenda", "tipo": "construcao.fazenda", "lote": 1, "dono": "cidade"}],
        "npcs": [{"nome": "Ana", "classe": "civil", "casa": "fazenda", "trabalho": ""}]})",
                   dados.mapaDaVila, catalogos, problemasDaCasaErrada);
    conferir(problemasDaCasaErrada.algumErroContem("não é uma Casa"), "um NPC morando na Fazenda: erro");

    std::string vilaLotada = R"({"semente_da_partida": 1, "construcoes": [
        {"id": "casa_1", "tipo": "construcao.casa", "lote": 2, "dono": "cidade"}], "npcs": [)";
    for (int numero = 0; numero < 5; ++numero) {
        vilaLotada += std::string(numero > 0 ? "," : "") + R"({"nome": "N", "classe": "civil", "casa": "casa_1", "trabalho": ""})";
    }
    vilaLotada += "]}";
    ListaDeProblemasDosDados problemasDaCasaLotada;
    lerVilaInicial(vilaLotada, dados.mapaDaVila, catalogos, problemasDaCasaLotada);
    conferir(problemasDaCasaLotada.quantidadeDeErros() == 1 && problemasDaCasaLotada.algumErroContem("já tem 4 moradores"),
             "5 NPCs na mesma Casa: erro no quinto");
}

// O retrato de um NPC no fim da simulação, para comparar duas rodadas.
struct RetratoDoNpc {
    PosicaoDeTile tileAtual;
    int progresso = 0;
    AcaoDoNpc acao = AcaoDoNpc::Nenhuma;
    int ondeEsta = -1;
    bool operator==(const RetratoDoNpc& outro) const = default;
};

struct ResultadoDoDiaSemTela {
    int dentroDeCasaAs3h = 0;
    int dentroDoTrabalhoAs10h = 0;
    int compraramNoPrimeiroDia = 0;
    std::vector<RetratoDoNpc> retratosNoFim;
};

// Roda da partida nova (6h do dia 0) até as 3h do dia 1, como o laço do jogo.
ResultadoDoDiaSemTela simularUmDiaSemTela()
{
    DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    const RegrasDoTempo regrasDoTempo;
    RelogioDoMundo relogio = RelogioDoMundo::criarParaPartidaNova(regrasDoTempo);
    entt::registry registro;
    BuscadorDeCaminho buscador;
    const RegrasDoMovimento regrasDoMovimento;
    SorteioComSemente sorteio(dados.vilaInicial.sementeDaPartida, sequenciaDoSorteioDosNpcs);
    ContextoDaRotina contexto{registro, dados.mapaDaVila, dados.vilaInicial, dados.regras.vidaDosNpcs, regrasDoTempo.ticksPorHoraDeJogo, sorteio};
    const std::vector<entt::entity> npcs = criarOsNpcsDaVila(contexto, relogio);

    ResultadoDoDiaSemTela resultado;
    const int64_t tickDas10h = 10 * 800;
    const int64_t tickDas3hDoDiaSeguinte = 19'200 + 3 * 800;
    while (relogio.tickAtual() < tickDas3hDoDiaSeguinte) {
        relogio.avancarUmTick();
        rodarARotinaAntesDeAndar(contexto, relogio);
        atenderPedidosDeCaminho(registro, dados.mapaDaVila, buscador, regrasDoMovimento);
        andarOsNpcsUmTick(registro, dados.mapaDaVila, regrasDoMovimento, relogio.tickAtual());
        cuidarDasChegadas(contexto, relogio);
        if (relogio.tickAtual() == tickDas10h) {
            for (const entt::entity npc : npcs) {
                const int trabalho = registro.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
                resultado.dentroDoTrabalhoAs10h += trabalho >= 0 && registro.get<DecisaoDoNpc>(npc).indiceDaConstrucaoOndeEsta == trabalho ? 1 : 0;
            }
        }
    }
    for (const entt::entity npc : npcs) {
        const DecisaoDoNpc& decisao = registro.get<DecisaoDoNpc>(npc);
        const LugarDoNpc& lugar = registro.get<PosicaoDoNpc>(npc).lugarAtual;
        resultado.dentroDeCasaAs3h += decisao.indiceDaConstrucaoOndeEsta == registro.get<CasaETrabalhoDoNpc>(npc).indiceDaCasa ? 1 : 0;
        resultado.compraramNoPrimeiroDia += decisao.diaDaUltimaCompra == 0 ? 1 : 0;
        resultado.retratosNoFim.push_back({lugar.tileAtual, lugar.progressoAteOProximoTileEmMilesimos, decisao.acaoAtual, decisao.indiceDaConstrucaoOndeEsta});
    }
    return resultado;
}

void testarUmDiaSemTela()
{
    const ResultadoDoDiaSemTela primeiraRodada = simularUmDiaSemTela();
    std::printf("    (um dia: %d no trabalho às 10h, %d em casa às 3h, %d compraram no dia 0)\n", primeiraRodada.dentroDoTrabalhoAs10h,
                primeiraRodada.dentroDeCasaAs3h, primeiraRodada.compraramNoPrimeiroDia);
    conferir(primeiraRodada.dentroDoTrabalhoAs10h == 12, "um dia sem tela: às 10h, os 12 empregados estão dentro do trabalho");
    conferir(primeiraRodada.dentroDeCasaAs3h == 15, "um dia sem tela: às 3h, os 15 estão dentro de casa");
    conferir(primeiraRodada.compraramNoPrimeiroDia == 8, "no dia 0, os 8 NPCs de número par foram ao Mercado");
    const ResultadoDoDiaSemTela segundaRodada = simularUmDiaSemTela();
    conferir(segundaRodada.retratosNoFim == primeiraRodada.retratosNoFim, "o mesmo dia duas vezes dá as mesmas posições, NPC por NPC");
}

}  // namespace

void testarRotinaDosNpcs()
{
    std::printf("Rotina e Utilidade\n");
    testarSorteio();
    testarPeriodosENotas();
    testarVilaInicial();
    testarUmDiaSemTela();
}
