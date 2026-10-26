#pragma once

// O bloco "populacao" do data/regras.json (Ficha Lógica: População, parte 2)
// e o estado da população que vai no save.

#include <string>
#include <vector>

struct RegrasDaPopulacao {
    int fatorDeEscala = 100;  // 1 NPC real = 100 pessoas
    std::vector<int> capacidadePorNivelDaCidade = {15, 40, 100, 300};
    int idadeAdulto = 5;
    int idadeVelho = 31;
    // 10 = 10% por estação para cada ano acima de 20 (21 anos: 10%; 30: 100%).
    int chanceDeMorrerDeVelhicePorAnoDeVelhiceEmPorcento = 10;
    int natalidadePorEstacaoEmPorMil = 40;
    int mortalidadePorEstacaoEmPorMil = 10;
    int mortalidadeSemLenhaEmPorcento = 150;
    int velhiceNoInvernoRigorosoEmPorcento = 150;
    int fomeMataPorDiaEmPorcentoDeQuemFicouSemComida = 2;
    int diasDeFomeAntesDeMatar = 3;
    int migranteChegaComFelicidadeMinima = 50;
    int migranteSaiComFelicidadeAbaixoDe = 30;
    int migranteSaiComDesempregoEmPorcento = 30;
    int idadeDoMigranteMinima = 4;
    int idadeDoMigranteMaxima = 12;
    int habilidadeDoMigranteMinima = 20;  // a faixa do Civil (PO1-3)
    int habilidadeDoMigranteMaxima = 60;
    std::vector<std::string> nomesDosNovosNpcs = {"Rita", "Tiago", "Úrsula", "Vítor", "Wanda", "Xavier", "Yara", "Zeca"};
};

struct EstadoDaPopulacao {
    int acumuladoDeCrescimentoEmPessoas = 0;  // entre −100 e +100
    int proximoNumeroDeNpc = 0;                // os números nunca se repetem
    int diasDeFomeNaEstacao = 0;
    // Só do dia (para o relatório; não vão no save).
    int nascimentosDeHoje = 0;  // NPCs que chegaram por nascimento ou reposição
    int mortesDeHoje = 0;       // NPCs que morreram
    int migracaoDeHoje = 0;     // +1 chegou, −1 saiu
};
