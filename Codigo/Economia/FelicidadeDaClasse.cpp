#include "Economia/FelicidadeDaClasse.h"

#include <algorithm>

FatoresDaFelicidade calcularFatoresDaFelicidade(const SituacaoDoDiaParaAFelicidade& situacao, const RegrasDaFelicidade& regras)
{
    FatoresDaFelicidade fatores;
    const long long civis = std::max(situacao.quantidadeDeCivis, 1);

    // 1. Consumo atendido: a fome, os Materiais em déficit e o bônus do gasto
    //    extra (+5 × gasto extra ÷ gasto da cesta, no máximo +5).
    long long consumo = 10LL * regras.consumoSemComida * situacao.civisQueFicaramSemComida / civis;
    if (situacao.materiaisEmDeficit) {
        consumo += 10LL * regras.consumoComMateriaisEmDeficit;
    }
    if (situacao.gastoDaCestaDeHoje > 0 && situacao.gastoExtraDeHoje > 0) {
        const long long bonusMaximo = 10LL * regras.consumoGastoExtraMaximo;
        consumo += std::min(bonusMaximo, bonusMaximo * situacao.gastoExtraDeHoje / situacao.gastoDaCestaDeHoje);
    }
    consumo += 10LL * situacao.pontosDoClimaNoConsumo;
    fatores.consumoAtendido = static_cast<int>(consumo);

    // 2. Desemprego e renda: +10 − 20 × a taxa, preso entre −10 e +10.
    const long long desemprego = 10LL * regras.desempregoSemDesemprego + 10LL * regras.desempregoPorPontoDaTaxa * situacao.civisDesempregados / civis;
    fatores.desempregoERenda = static_cast<int>(std::clamp<long long>(desemprego, 10LL * regras.desempregoMinimo, 10LL * regras.desempregoMaximo));

    // 3. Moradia: −10 × a parte dos Civis sem casa.
    fatores.moradia = static_cast<int>(10LL * regras.moradiaSemCasa * situacao.civisSemCasa / civis);

    // 4. Eventos duradouros, presos entre −20 e +20.
    fatores.eventosDuradouros = std::clamp(10 * situacao.felicidadeDosEventosEmPontos, 10 * regras.eventosMinimo, 10 * regras.eventosMaximo);

    // 5. Leis × cultura (fase C2), de −30 a +10.
    fatores.leisECultura = std::clamp(situacao.fatorLeisECulturaEmDecimos, -300, 100);
    return fatores;
}

int calcularAlvoDaFelicidade(const FatoresDaFelicidade& fatores, const RegrasDaFelicidade& regras)
{
    return std::clamp(10 * regras.baseDoAlvo + fatores.soma(), 0, 1000);
}

int darOPassoDaFelicidade(int felicidadeEmDecimos, int alvoEmDecimos, const RegrasDaFelicidade& regras)
{
    const int diferenca = alvoEmDecimos - felicidadeEmDecimos;
    if (diferenca == 0) {
        return felicidadeEmDecimos;
    }
    // A parte do passo, arredondada para longe do zero: 0,1 de diferença ainda anda 0,1.
    const int tamanhoDaDiferenca = diferenca > 0 ? diferenca : -diferenca;
    const int passo = (tamanhoDaDiferenca * regras.passoPorDiaEmPorcento + 99) / 100;
    return felicidadeEmDecimos + (diferenca > 0 ? passo : -passo);
}

std::string nomeDaFaixaDaFelicidade(int felicidadeEmDecimos)
{
    if (felicidadeEmDecimos > 700) {
        return "Contente";
    }
    if (felicidadeEmDecimos >= 400) {
        return "Estável";
    }
    if (felicidadeEmDecimos >= 250) {
        return "Inquieta";
    }
    if (felicidadeEmDecimos >= 100) {
        return "Protesto";
    }
    return "Revolta";
}
