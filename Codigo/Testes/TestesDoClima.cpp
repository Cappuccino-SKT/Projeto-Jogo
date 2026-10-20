// Os testes do clima: a tabela da parte 8 da Ficha Lógica: Clima e Estações.

#include <cstdio>
#include <string>

#include "Clima/Clima.h"
#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Simulacao/RelatorioDaSimulacao.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

RegrasDoClima lerAsRegrasDoJogo()
{
    ListaDeProblemasDosDados problemas;
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    return dados.regrasDoClima;
}

void testarOSorteio(const RegrasDoClima& regras)
{
    // Inverno, número de repetir 300, ontem Neve: repete.
    conferir(escolherClimaDoDia(TipoDeClima::Neve, Estacao::Inverno, 300, 0, regras) == TipoDeClima::Neve, "Inverno, repetir 300, ontem Neve: Neve");
    // Dia 1 do Inverno, ontem Chuva, número de repetir 100: a chuva não existe no inverno.
    conferir(escolherClimaDoDia(TipoDeClima::Chuva, Estacao::Inverno, 100, 0, regras) == TipoDeClima::Limpo,
             "ontem Chuva, Inverno: não repete (sorteia da tabela: 0 é Limpo)");
    // Inverno, número da tabela 610: Neve; 999: Névoa (o último da lista).
    conferir(escolherClimaDoDia(TipoDeClima::Limpo, Estacao::Inverno, 900, 610, regras) == TipoDeClima::Neve, "Inverno, tabela 610: Neve");
    conferir(escolherClimaDoDia(TipoDeClima::Limpo, Estacao::Inverno, 900, 999, regras) == TipoDeClima::Nevoa, "Inverno, tabela 999: Névoa");
    conferir(escolherClimaDoDia(TipoDeClima::Limpo, Estacao::Inverno, 900, 549, regras) == TipoDeClima::Nublado, "Inverno, tabela 549: Nublado");
    conferir(chanceDoClimaNaEstacao(TipoDeClima::Chuva, Estacao::Inverno, regras) == 0, "a chuva tem chance 0 no inverno");

    // A mesma semente dá o mesmo clima.
    SorteioComSemente primeiro(1, sequenciaDoSorteioDoClima);
    SorteioComSemente segundo(1, sequenciaDoSorteioDoClima);
    TipoDeClima climaUm = TipoDeClima::Limpo;
    TipoDeClima climaDois = TipoDeClima::Limpo;
    bool iguais = true;
    for (int dia = 0; dia < 64; ++dia) {
        const Estacao estacao = static_cast<Estacao>(dia / 16);
        climaUm = sortearClimaDoDia(climaUm, estacao, regras, primeiro);
        climaDois = sortearClimaDoDia(climaDois, estacao, regras, segundo);
        iguais = iguais && climaUm == climaDois;
    }
    conferir(iguais, "a mesma semente, 64 dias, duas vezes: o mesmo clima nos mesmos dias");
}

void testarALenha(const RegrasDoClima& regras)
{
    conferir(calcularLenhaDoDiaEmCentesimos(15, TipoDeClima::Nublado, 100, regras) == 375, "15 moradores, dia comum: 375 centésimos");
    conferir(calcularLenhaDoDiaEmCentesimos(15, TipoDeClima::Neve, 100, regras) == 562, "15 moradores, neve: 562");
    conferir(calcularLenhaDoDiaEmCentesimos(15, TipoDeClima::Neve, 200, regras) == 1124, "15 moradores, neve e Inverno rigoroso: 1.124");

    EstadoDoClima estado;
    conferir(pagarALenhaDevida(estado, 375, 50) == 3 && estado.lenhaDevidaEmCentesimos == 75, "Nublado, dívida 0: 3 Toras, dívida 75");
    conferir(pagarALenhaDevida(estado, 562, 50) == 6 && estado.lenhaDevidaEmCentesimos == 37, "Neve, dívida 75: 6 Toras, dívida 37");
    estado.lenhaDevidaEmCentesimos = 75;
    const int tiradas = pagarALenhaDevida(estado, 562, 1);
    conferir(tiradas == 1 && estado.faltouLenhaHoje && estado.lenhaDevidaEmCentesimos == 37 && estado.diasEmDeficitDeLenha == 1,
             "Neve, dívida 637, 1 Tora: tira 1, faltou, dívida 37, 1 dia em déficit");
    pagarALenhaDevida(estado, 375, 50);
    conferir(!estado.faltouLenhaHoje && estado.diasEmDeficitDeLenha == 0, "no dia seguinte, com Tora: o déficit zera");
}

void testarATempestade(const RegrasDoClima& regras)
{
    conferir(porcentagemDoTrabalhoAoArLivre(TipoDeClima::Tempestade, regras) == 50, "tempestade: o trabalho ao ar livre a 50%");
    conferir(porcentagemDoTrabalhoAoArLivre(TipoDeClima::Chuva, regras) == 100, "chuva: o trabalho ao ar livre a 100%");
    EstadoDoClima estado;
    estado.climaDeHoje = TipoDeClima::Tempestade;
    conferir(periodoLivreEmCasaHoje(estado, regras), "tempestade: o Período Livre é em casa");
}

void testarALeituraDoArquivo()
{
    ListaDeProblemasDosDados problemas;
    lerArquivoDoClima(R"({ "chances_por_estacao_em_por_mil": { "primavera": { "limpo": 500, "granizo": 500 },
        "verao": { "limpo": 1000 }, "outono": { "limpo": 1000 }, "inverno": { "limpo": 900 } } })", problemas);
    conferir(problemas.quantidadeDeErros() == 3, "clima.json: um clima que não existe e chances que não somam 1000 dão erro");
}

void testarAVila()
{
    DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo com o clima.json: sem erro");
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    conferir(simulacao.clima.climaDeHoje == TipoDeClima::Limpo, "dia 1 da partida: Limpo, sem sortear");

    // Um NPC passeando na tempestade volta para casa: roda até as 19h do dia 1
    // com a tempestade ligada e confere quem está em casa ou indo para lá.
    simulacao.clima.climaDeHoje = TipoDeClima::Tempestade;
    const int ticksAte19h = 13 * 800;  // das 6h às 19h
    for (int tick = 0; tick < ticksAte19h; ++tick) {
        simulacao.avancarUmTick();
    }
    int passeandoNaRua = 0;
    for (const auto [npc, decisao, casaETrabalho] : simulacao.registroDasEntidades.view<DecisaoDoNpc, CasaETrabalhoDoNpc>().each()) {
        if (decisao.acaoAtual == AcaoDoNpc::Passear && casaETrabalho.indiceDaCasa >= 0 && decisao.indiceDaConstrucaoDoDestino != casaETrabalho.indiceDaCasa) {
            ++passeandoNaRua;
        }
    }
    conferir(passeandoNaRua == 0, "tempestade, 19h: nenhum NPC passeia fora de casa");
}

}  // namespace

void testarClima()
{
    std::printf("Clima e Estações (fase A1)\n");
    const RegrasDoClima regras = lerAsRegrasDoJogo();
    testarOSorteio(regras);
    testarALenha(regras);
    testarATempestade(regras);
    testarALeituraDoArquivo();
    testarAVila();
}
