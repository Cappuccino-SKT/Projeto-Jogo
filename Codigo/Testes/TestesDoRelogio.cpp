// Os testes do relógio do mundo: a tabela da parte 8 da Ficha Lógica: Tempo.

#include <cstdio>

#include "ConferenciaDosTestes.h"
#include "Tempo/RelogioDoMundo.h"

namespace {

const RegrasDoTempo regrasPadraoDoTempo{};
constexpr int64_t umMilissegundoEmNanossegundos = 1'000'000;
constexpr int64_t umSegundoEmNanossegundos = 1'000'000'000;

// Passa "quantidadeDeQuadros" quadros de "milissegundosPorQuadro" cada e soma os ticks.
int simularQuadrosDeTela(RelogioDoMundo& relogio, int quantidadeDeQuadros, int milissegundosPorQuadro)
{
    int totalDeTicksAvancados = 0;
    for (int quadro = 0; quadro < quantidadeDeQuadros; ++quadro) {
        totalDeTicksAvancados += relogio.avancarTicksPeloTempoReal(milissegundosPorQuadro * umMilissegundoEmNanossegundos);
    }
    return totalDeTicksAvancados;
}

void testarAvancoPeloTempoReal()
{
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 0);
        const int ticks = simularQuadrosDeTela(relogio, 4000, 10);  // 40 s reais
        conferir(ticks == 800, "velocidade 1, 40 s reais avançam 800 ticks");
        conferir(relogio.horaDoDiaAtual() == 1, "velocidade 1, 40 s reais a partir do tick 0 chegam à 1h");
    }
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 0);
        relogio.definirVelocidadeDoJogo(0);
        const int ticks = simularQuadrosDeTela(relogio, 1000, 10);  // 10 s reais
        conferir(ticks == 0, "velocidade 0, 10 s reais não avançam nada");
    }
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 0);
        relogio.definirVelocidadeDoJogo(4);
        const int ticks = simularQuadrosDeTela(relogio, 1000, 10);  // 10 s reais
        conferir(ticks == 800, "velocidade 4, 10 s reais avançam 800 ticks");
    }
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 0);
        const int ticks = relogio.avancarTicksPeloTempoReal(2 * umSegundoEmNanossegundos);
        conferir(ticks == 5, "um quadro de 2 s é limitado a 250 ms: 5 ticks");
    }
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 0);
        const int ticksEsperadosPorQuadro[] = {0, 1, 0, 1, 1, 0, 1, 0, 1, 1};
        bool todosOsQuadrosCertos = true;
        for (int ticksEsperados : ticksEsperadosPorQuadro) {
            if (relogio.avancarTicksPeloTempoReal(30 * umMilissegundoEmNanossegundos) != ticksEsperados) {
                todosOsQuadrosCertos = false;
            }
        }
        conferir(todosOsQuadrosCertos, "quadros de 30 ms dão 0, 1, 0, 1, 1 tick, e repete");
        conferir(relogio.tickAtual() == 6, "quadros de 30 ms: 300 ms dão 6 ticks, sem perder tempo");
    }
}

void testarContasDaData()
{
    {
        const RelogioDoMundo relogio(regrasPadraoDoTempo, 27'200);
        conferir(relogio.diaDaPartida() == 1 && relogio.diaDaEstacaoAtual() == 2, "o tick 27.200 é o dia 2");
        conferir(relogio.estacaoAtual() == Estacao::Primavera, "o tick 27.200 é na Primavera");
        conferir(relogio.horaDoDiaAtual() == 10 && relogio.minutoDaHoraAtual() == 0, "o tick 27.200 é 10h00");
        conferir(relogio.faseDoDiaAtual() == FaseDoDia::Dia, "o tick 27.200 é de dia");
    }
    {
        const RelogioDoMundo relogio(regrasPadraoDoTempo, 19'199);
        conferir(relogio.diaDaEstacaoAtual() == 1, "o tick 19.199 ainda é o dia 1");
        conferir(relogio.horaDoDiaAtual() == 23 && relogio.minutoDaHoraAtual() == 59, "o tick 19.199 é 23h59");
        conferir(relogio.faseDoDiaAtual() == FaseDoDia::Noite, "o tick 19.199 é de noite");
    }
    {
        const RelogioDoMundo relogio(regrasPadraoDoTempo, 1'000'000);
        conferir(relogio.estacaoAtual() == Estacao::Inverno, "o tick 1.000.000 é no Inverno");
        conferir(relogio.diaDaEstacaoAtual() == 5, "o tick 1.000.000 é o dia 5 da estação");
        conferir(relogio.horaDoDiaAtual() == 2 && relogio.minutoDaHoraAtual() == 0, "o tick 1.000.000 é 2h00");
        conferir(relogio.anoAtual() == 412, "o tick 1.000.000 é no Ano 412");
    }
    {
        const RelogioDoMundo relogio(regrasPadraoDoTempo, 307'200);
        conferir(relogio.estacaoAtual() == Estacao::Verao && relogio.diaDaEstacaoAtual() == 1,
                 "o tick 307.200 é o dia 1 do Verão");
        conferir(relogio.horaDoDiaAtual() == 0, "o tick 307.200 é 0h");
    }
    {
        const RelogioDoMundo relogio(regrasPadraoDoTempo, 1'228'800);
        conferir(relogio.estacaoAtual() == Estacao::Primavera && relogio.diaDaEstacaoAtual() == 1,
                 "o tick 1.228.800 é o dia 1 da Primavera");
        conferir(relogio.anoAtual() == 413, "o tick 1.228.800 é no Ano 413");
    }
}

void testarFasesDoDia()
{
    // A fase de cada hora: o tick da hora é hora x 800.
    struct HoraEFase { int hora; FaseDoDia faseEsperada; const char* descricao; };
    const HoraEFase casos[] = {
        {4, FaseDoDia::Noite, "4h é noite"},
        {5, FaseDoDia::Amanhecer, "5h é amanhecer"},
        {7, FaseDoDia::Dia, "7h é dia"},
        {18, FaseDoDia::Entardecer, "18h é entardecer"},
        {20, FaseDoDia::Noite, "20h é noite"},
    };
    for (const HoraEFase& caso : casos) {
        const RelogioDoMundo relogio(regrasPadraoDoTempo, static_cast<int64_t>(caso.hora) * 800);
        conferir(relogio.faseDoDiaAtual() == caso.faseEsperada, caso.descricao);
    }
}

void testarViradas()
{
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 19'199);
        relogio.avancarUmTick();
        conferir(relogio.comecouUmaHoraNova(), "do 19.199 para o 19.200 começa uma hora nova");
        conferir(relogio.comecouUmDiaNovo(), "do 19.199 para o 19.200 começa um dia novo");
        conferir(!relogio.comecouUmaEstacaoNova(), "do 19.199 para o 19.200 não começa estação nova");
    }
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 19'200);
        relogio.avancarUmTick();
        conferir(!relogio.comecouUmaHoraNova() && !relogio.comecouUmDiaNovo(),
                 "do 19.200 para o 19.201 não há virada");
    }
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 1'228'799);
        relogio.avancarUmTick();
        conferir(relogio.comecouUmaEstacaoNova() && relogio.comecouUmAnoNovo(),
                 "do 1.228.799 para o 1.228.800 começam uma estação e um ano novos");
    }
}

void testarPartidaNovaEVelocidades()
{
    {
        const RelogioDoMundo relogio = RelogioDoMundo::criarParaPartidaNova(regrasPadraoDoTempo);
        conferir(relogio.tickAtual() == 4'800, "a partida nova começa no tick 4.800");
        conferir(relogio.horaDoDiaAtual() == 6 && relogio.minutoDaHoraAtual() == 0, "a partida nova começa às 6h00");
        conferir(relogio.diaDaEstacaoAtual() == 1 && relogio.estacaoAtual() == Estacao::Primavera
                     && relogio.anoAtual() == 412,
                 "a partida nova começa no dia 1 da Primavera do Ano 412");
        conferir(!relogio.comecouUmaHoraNova() && !relogio.comecouUmDiaNovo(),
                 "o começo da partida não dispara virada");
    }
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 0);
        relogio.definirVelocidadeDoJogo(2);
        relogio.alternarPausa();
        conferir(relogio.jogoEstaPausado(), "pausar a 2x deixa o jogo pausado");
        relogio.alternarPausa();
        conferir(relogio.velocidadeAtualDoJogo() == 2, "despausar volta para 2x");
    }
    {
        RelogioDoMundo relogio(regrasPadraoDoTempo, 0);
        relogio.definirVelocidadeDoJogo(2);
        const bool velocidade3FoiAceita = relogio.definirVelocidadeDoJogo(3);
        conferir(!velocidade3FoiAceita && relogio.velocidadeAtualDoJogo() == 2, "a velocidade 3 é ignorada");
    }
}

}  // namespace

void testarRelogioDoMundo()
{
    std::printf("Relógio do mundo\n");
    testarAvancoPeloTempoReal();
    testarContasDaData();
    testarFasesDoDia();
    testarViradas();
    testarPartidaNovaEVelocidades();
}
