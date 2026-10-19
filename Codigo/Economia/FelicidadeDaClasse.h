#pragma once

// A Felicidade dos Civis (Ficha Lógica: Felicidade, marco 7): todo dia, um
// alvo que sai de 4 fatores, e um passo de 10% em direção a ele. No marco 7
// ela só é medida e mostrada (FL7-7): ainda não muda a produção.
//
// Tudo em décimos de ponto (587 = 58,7), para o passo de 10% de uma
// diferença pequena não se perder.

#include <string>

// Os números do "felicidade" do regras.json, em pontos (como no arquivo).
struct RegrasDaFelicidade {
    int felicidadeInicial = 50;
    int baseDoAlvo = 50;
    int passoPorDiaEmPorcento = 10;
    // Consumo atendido.
    int consumoSemComida = -25;
    int consumoComMateriaisEmDeficit = -5;
    int consumoGastoExtraMaximo = 5;
    // Desemprego e renda: +10 − 20 × a taxa (de 0 a 1), preso entre −10 e +10.
    int desempregoSemDesemprego = 10;
    int desempregoPorPontoDaTaxa = -20;
    int desempregoMinimo = -10;
    int desempregoMaximo = 10;
    // Moradia: −10 × a parte dos Civis sem casa.
    int moradiaSemCasa = -10;
    // Eventos duradouros: a soma, presa entre −20 e +20.
    int eventosMinimo = -20;
    int eventosMaximo = 20;
};

// O que a regra 4.1 precisa saber do dia.
struct SituacaoDoDiaParaAFelicidade {
    int quantidadeDeCivis = 0;
    int civisQueFicaramSemComida = 0;
    bool materiaisEmDeficit = false;
    long long gastoExtraDeHoje = 0;     // em décimos de moeda
    long long gastoDaCestaDeHoje = 0;   // em décimos de moeda
    int civisDesempregados = 0;
    int civisSemCasa = 0;
    int felicidadeDosEventosEmPontos = 0;  // a soma dos eventos ativos (ficha dos Eventos)
};

// Os 4 fatores do dia, em décimos de ponto.
struct FatoresDaFelicidade {
    int consumoAtendido = 0;
    int desempregoERenda = 0;
    int moradia = 0;
    int eventosDuradouros = 0;

    int soma() const { return consumoAtendido + desempregoERenda + moradia + eventosDuradouros; }
};

struct FelicidadeDaClasse {
    int felicidadeEmDecimos = 500;
    int alvoEmDecimos = 500;      // não vai no save: recalculado
    FatoresDaFelicidade fatoresDoDia;  // só para o resumo e o relatório
};

// Regra 4.1.
FatoresDaFelicidade calcularFatoresDaFelicidade(const SituacaoDoDiaParaAFelicidade& situacao, const RegrasDaFelicidade& regras);

// Regra 4.2, passo 1: 50 + a soma dos fatores, preso entre 0 e 100 (em décimos).
int calcularAlvoDaFelicidade(const FatoresDaFelicidade& fatores, const RegrasDaFelicidade& regras);

// Regra 4.2, passo 2: anda 10% da diferença, arredondando na direção do alvo
// (assim ela sempre chega ao alvo). 500 e alvo 560: 506.
int darOPassoDaFelicidade(int felicidadeEmDecimos, int alvoEmDecimos, const RegrasDaFelicidade& regras);

// Regra 4.3: "Contente" (acima de 70), "Estável" (de 40 a 70), "Inquieta" (de
// 25 a 40), "Protesto" (de 10 a 25) ou "Revolta" (abaixo de 10).
std::string nomeDaFaixaDaFelicidade(int felicidadeEmDecimos);
