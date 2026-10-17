#include "TextoDaDataDoJogo.h"

#include <cstdio>

const char* nomeDaEstacaoParaMostrar(Estacao estacao)
{
    switch (estacao) {
    case Estacao::Primavera: return "Primavera";
    case Estacao::Verao: return "Verão";
    case Estacao::Outono: return "Outono";
    case Estacao::Inverno: return "Inverno";
    }
    return "?";
}

const char* nomeDaFaseDoDiaParaMostrar(FaseDoDia faseDoDia)
{
    switch (faseDoDia) {
    case FaseDoDia::Amanhecer: return "amanhecer";
    case FaseDoDia::Dia: return "dia";
    case FaseDoDia::Entardecer: return "entardecer";
    case FaseDoDia::Noite: return "noite";
    }
    return "?";
}

std::string montarTextoDaHora(const RelogioDoMundo& relogio)
{
    char textoDaHora[16];
    std::snprintf(textoDaHora, sizeof(textoDaHora), "%02dh%02d", relogio.horaDoDiaAtual(), relogio.minutoDaHoraAtual());
    return textoDaHora;
}

std::string montarTextoDoTituloDaJanela(const RelogioDoMundo& relogio)
{
    std::string textoDaVelocidade = "pausado";
    if (!relogio.jogoEstaPausado()) {
        textoDaVelocidade = std::to_string(relogio.velocidadeAtualDoJogo()) + "x";
    }

    return "Projeto Jogo — "
        + std::string(nomeDaEstacaoParaMostrar(relogio.estacaoAtual()))
        + ", dia " + std::to_string(relogio.diaDaEstacaoAtual())
        + ", " + montarTextoDaHora(relogio)
        + " (" + nomeDaFaseDoDiaParaMostrar(relogio.faseDoDiaAtual()) + ")"
        + ", Ano " + std::to_string(relogio.anoAtual())
        + " — " + textoDaVelocidade;
}
