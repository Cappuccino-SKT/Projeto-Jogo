#pragma once

// Os números da rotina, da Utilidade e do passeio (ficha da IA, parte 2;
// FL5-3), lidos do data/regras.json. As notas ficam em centésimos (1,0 = 100).

// As horas de uma classe. Cada período vai da hora de início (inclusive) até a
// de fim (exclusive) e pode passar da meia-noite: dormir das 22h às 6h.
// O que não é dormir nem trabalhar é o Período Livre.
struct RegrasDaRotina {
    int horaQueComecaADormir = 22;
    int horaQueAcabaDeDormir = 6;
    int horaQueComecaATrabalhar = 7;
    int horaQueAcabaDeTrabalhar = 15;
};

struct RegrasDaUtilidade {
    int notaDaAcaoDoPeriodo = 100;
    int bonusDeComer = 150;
    int horasDasRefeicoes[3] = {6, 12, 19};  // o café às 6h (FL6-7)
    int minutosDaRefeicao = 30;
    int bonusDeComprar = 120;
    int diasEntreCompras = 2;
    int minutosDaCompra = 30;
    int minutosEntreReavaliacoes = 5;
};

struct RegrasDoPasseio {
    int distanciaMaximaEmTiles = 12;
    int menorEsperaEmMinutos = 15;
    int maiorEsperaEmMinutos = 45;
};

struct RegrasDaVidaDosNpcs {
    RegrasDaRotina rotinaDoCivil;
    RegrasDaUtilidade utilidade;
    RegrasDoPasseio passeio;
};
