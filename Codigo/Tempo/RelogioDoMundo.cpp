#include "Tempo/RelogioDoMundo.h"

RelogioDoMundo::RelogioDoMundo(const RegrasDoTempo& regrasDoTempo, int64_t tickInicialDaPartida)
    : regras(regrasDoTempo), tickAtualDaPartida(tickInicialDaPartida)
{
}

RelogioDoMundo RelogioDoMundo::criarParaPartidaNova(const RegrasDoTempo& regrasDoTempo)
{
    // O tick 0 é a meia-noite do dia 1. Às 6h, a partida começa no tick 6 x 800 = 4.800.
    const int64_t tickDaHoraInicial =
        static_cast<int64_t>(regrasDoTempo.horaInicialDaPartida) * regrasDoTempo.ticksPorHoraDeJogo;
    return RelogioDoMundo(regrasDoTempo, tickDaHoraInicial);
}

double RelogioDoMundo::fracaoJaPassadaDoProximoTick() const
{
    return static_cast<double>(nanossegundosAcumuladosParaOProximoTick) / regras.nanossegundosReaisDeUmTickEmVelocidade1();
}

int RelogioDoMundo::calcularTicksParaAvancarPeloTempoReal(int64_t nanossegundosReaisDesdeOUltimoQuadro)
{
    // Passo 2: o limite de segurança (FL1-3).
    const int64_t limiteEmNanossegundos =
        static_cast<int64_t>(regras.limiteDeTempoRealPorQuadroEmMilissegundos) * 1'000'000LL;
    if (nanossegundosReaisDesdeOUltimoQuadro > limiteEmNanossegundos) {
        nanossegundosReaisDesdeOUltimoQuadro = limiteEmNanossegundos;
    }
    if (nanossegundosReaisDesdeOUltimoQuadro < 0) {
        nanossegundosReaisDesdeOUltimoQuadro = 0;
    }

    // Passo 3: multiplica pela velocidade (na pausa, soma zero).
    nanossegundosAcumuladosParaOProximoTick += nanossegundosReaisDesdeOUltimoQuadro * velocidadeDoJogo;

    // Passo 4: conta 1 tick para cada 50 milissegundos acumulados.
    const int64_t nanossegundosDeUmTick = regras.nanossegundosReaisDeUmTickEmVelocidade1();
    int quantidadeDeTicksParaAvancar = 0;
    while (nanossegundosAcumuladosParaOProximoTick >= nanossegundosDeUmTick) {
        nanossegundosAcumuladosParaOProximoTick -= nanossegundosDeUmTick;
        ++quantidadeDeTicksParaAvancar;
    }

    // Passo 5: o que sobrou fica no acumulador para o próximo quadro.
    return quantidadeDeTicksParaAvancar;
}

void RelogioDoMundo::avancarUmTick()
{
    ++tickAtualDaPartida;
    tickAtualFoiAlcancadoAgora = true;
    // As regras "a cada tick" (movimento dos NPCs) entram no marco 4.
}

bool RelogioDoMundo::tickAtualEhMultiploDe(int64_t quantidadeDeTicks) const
{
    return tickAtualFoiAlcancadoAgora && tickAtualDaPartida % quantidadeDeTicks == 0;
}

bool RelogioDoMundo::comecouUmaHoraNova() const { return tickAtualEhMultiploDe(regras.ticksPorHoraDeJogo); }
bool RelogioDoMundo::comecouUmDiaNovo() const { return tickAtualEhMultiploDe(regras.ticksPorDia()); }
bool RelogioDoMundo::comecouUmaEstacaoNova() const { return tickAtualEhMultiploDe(regras.ticksPorEstacao()); }
bool RelogioDoMundo::comecouUmAnoNovo() const { return tickAtualEhMultiploDe(regras.ticksPorAno()); }

bool RelogioDoMundo::definirVelocidadeDoJogo(int novaVelocidade)
{
    const bool velocidadeEhPermitida =
        novaVelocidade == 0 || novaVelocidade == 1 || novaVelocidade == 2 || novaVelocidade == 4;
    if (!velocidadeEhPermitida) {
        return false;
    }
    velocidadeDoJogo = novaVelocidade;
    return true;
}

void RelogioDoMundo::alternarPausa()
{
    if (velocidadeDoJogo != 0) {
        // Andando: guarda a velocidade e pausa.
        velocidadeAntesDaPausa = velocidadeDoJogo;
        velocidadeDoJogo = 0;
    } else {
        // Pausado: volta para a velocidade de antes (2x continua 2x).
        velocidadeDoJogo = velocidadeAntesDaPausa;
    }
}

// --- As contas da parte 5. Todas as divisões são inteiras. ---

int RelogioDoMundo::horaDoDiaAtual() const
{
    return static_cast<int>((tickAtualDaPartida / regras.ticksPorHoraDeJogo) % regras.horasPorDia);
}

int RelogioDoMundo::minutoDaHoraAtual() const
{
    const int64_t ticksDesdeOComecoDaHora = tickAtualDaPartida % regras.ticksPorHoraDeJogo;
    return static_cast<int>(ticksDesdeOComecoDaHora * 60 / regras.ticksPorHoraDeJogo);
}

int64_t RelogioDoMundo::diaDaPartida() const
{
    return tickAtualDaPartida / regras.ticksPorDia();
}

int RelogioDoMundo::diaDoAnoAtual() const
{
    const int64_t diasPorAno = static_cast<int64_t>(regras.diasPorEstacao) * regras.estacoesPorAno;
    return static_cast<int>(diaDaPartida() % diasPorAno);
}

int RelogioDoMundo::diaDaEstacaoAtual() const
{
    return diaDoAnoAtual() % regras.diasPorEstacao + 1;
}

int RelogioDoMundo::anoAtual() const
{
    const int64_t diasPorAno = static_cast<int64_t>(regras.diasPorEstacao) * regras.estacoesPorAno;
    return regras.anoInicialDaPartida + static_cast<int>(diaDaPartida() / diasPorAno);
}

Estacao RelogioDoMundo::estacaoAtual() const
{
    return static_cast<Estacao>(diaDoAnoAtual() / regras.diasPorEstacao);
}

FaseDoDia RelogioDoMundo::faseDoDiaAtual() const
{
    const int hora = horaDoDiaAtual();
    if (hora >= regras.horaEmQueComecaANoite || hora < regras.horaEmQueComecaOAmanhecer) {
        return FaseDoDia::Noite;
    }
    if (hora < regras.horaEmQueComecaODia) {
        return FaseDoDia::Amanhecer;
    }
    if (hora < regras.horaEmQueComecaOEntardecer) {
        return FaseDoDia::Dia;
    }
    return FaseDoDia::Entardecer;
}
