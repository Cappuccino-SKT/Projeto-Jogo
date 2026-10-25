#pragma once

// A Felicidade dos Civis (Ficha Lógica: Felicidade, marco 7): todo dia, um
// alvo que sai de 4 fatores, e um passo de 10% em direção a ele. No marco 7
// ela só é medida e mostrada (FL7-7): ainda não muda a produção.
//
// Tudo em décimos de ponto (587 = 58,7), para o passo de 10% de uma
// diferença pequena não se perder.

#include <cstdint>
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
    int felicidadeDosEventosEmPontos = 0;
    int fatorLeisECulturaEmDecimos = 0;  // fase C2: já calculado com as leis da facção
    int felicidadeDaCriminalidadeEmDecimos = 0;  // fase C3 (Crime, 4.1, passo 3): entra nos eventos
    int fatorReligiaoEmDecimos = 0;              // fase C4  // a soma dos eventos ativos (ficha dos Eventos) e o calor forte (Clima)
    int pontosDoClimaNoConsumo = 0;        // ficha do Clima, regra 4.4: −10 se faltou lenha hoje
};

// Os 4 fatores do dia, em décimos de ponto.
struct FatoresDaFelicidade {
    int consumoAtendido = 0;
    int desempregoERenda = 0;
    int moradia = 0;
    int eventosDuradouros = 0;
    int leisECultura = 0;  // fase C2 (Cultura, 4.2 e 4.3), de −30,0 a +10,0
    int religiao = 0;      // fase C4 (Religião, 4.2 e 4.5)

    int soma() const { return consumoAtendido + desempregoERenda + moradia + eventosDuradouros + leisECultura + religiao; }
};

struct FelicidadeDaClasse {
    int felicidadeEmDecimos = 500;
    int alvoEmDecimos = 500;      // não vai no save: recalculado
    FatoresDaFelicidade fatoresDoDia;  // só para o resumo e o relatório
    // Fase C3 (Crime e Justiça, 4.1 e 4.2): a Criminalidade da cidade, em décimos (11,0 no começo),
    // e o primeiro dia em que pode haver outro furto no armazém.
    int criminalidadeEmDecimos = 110;
    int64_t diaDoProximoFurtoPossivel = 0;
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
