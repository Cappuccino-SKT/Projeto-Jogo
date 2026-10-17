#pragma once

// Os textos da data para mostrar ao jogador (no marco 1, no título da janela
// e no console). Ficam no Jogo/ porque escrever na tela não é regra do jogo.
// Quando o arquivo textos/pt-BR.json existir, os nomes saem de lá.

#include <string>

#include "Tempo/RelogioDoMundo.h"

// "Primavera", "Verão", "Outono" ou "Inverno".
const char* nomeDaEstacaoParaMostrar(Estacao estacao);

// "amanhecer", "dia", "entardecer" ou "noite".
const char* nomeDaFaseDoDiaParaMostrar(FaseDoDia faseDoDia);

// Por exemplo: "06h00".
std::string montarTextoDaHora(const RelogioDoMundo& relogio);

// Por exemplo: "Projeto Jogo — Primavera, dia 1, 06h00 (amanhecer), Ano 412 — 1x".
std::string montarTextoDoTituloDaJanela(const RelogioDoMundo& relogio);
