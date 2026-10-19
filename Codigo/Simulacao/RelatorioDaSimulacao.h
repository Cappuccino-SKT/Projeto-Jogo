#pragma once

// O relatório do teste de 1 ano (Ficha Lógica: Simulação sem tela, FL7-8): uma
// linha por dia, com o estoque, o preço do Pão, o dinheiro, a Felicidade, os
// empregados, os déficits e os eventos; e os critérios do PASSOU ou FALHOU
// (Fatia Vertical, parte 6).
//
// O relatorio.csv usa ponto e vírgula entre as colunas e vírgula nos
// decimais: abre direto no Excel e no LibreOffice em português.

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "Producao/ProducaoDaVila.h"

// Os critérios do teste de 1 ano (regra 4.2).
constexpr int diasSeguidosDeFomeQueFalham = 4;
constexpr int menorPrecoDoPaoQuePassaEmDecimos = 12;
constexpr int maiorPrecoDoPaoQuePassaEmDecimos = 26;
constexpr const char* identificadorDoPao = "item.pao";

struct LinhaDoRelatorio {
    int64_t dia = 1;  // o dia que acabou, contando do 1
    Estacao estacao = Estacao::Primavera;
    int diaDaEstacao = 1;
    std::vector<int> estoquePorItem;  // os dois donos juntos, na ordem do itens.csv
    int precoDoPaoEmDecimos = 0;      // o preço local; 0 se o jogo não tem Pão
    int64_t bancoDaCidade = 0;
    int64_t dinheiroDoJogador = 0;
    int64_t riquezaMediaDosCivis = 0;
    int felicidadeEmDecimos = 0;
    int empregados = 0;
    int desempregados = 0;
    int diasEmDeficitDeComida = 0;
    int diasEmDeficitDeMateriais = 0;
    int64_t consumoNaoPago = 0;
    std::vector<std::string> eventosDoDia;  // "Seca começou"

    bool operator==(const LinhaDoRelatorio&) const = default;
};

// Monta a linha na meia-noite, depois da ordem do dia (regra 4.1, passo 4).
LinhaDoRelatorio montarLinhaDoRelatorio(const ContextoDaProducao& contexto, int64_t diaQueAcabouContandoDoZero, Estacao estacaoDoDia,
                                        int diaDaEstacao);

// "dia;estacao;dia_da_estacao;grao;agua;...;eventos_do_dia"
std::string escreverCabecalhoDoRelatorio(const DadosDoJogo& dados);

// "37;Outono;5;240;...;"Seca começou""
std::string escreverLinhaDoRelatorio(const LinhaDoRelatorio& linha);

// Regra 4.2: guarda o primeiro problema.
struct ConferenciaDoTesteDeUmAno {
    bool falhou = false;
    std::string motivoDaFalha;  // "fome por 4 dias seguidos no dia 37"
};

void conferirOsCriteriosDoDia(ConferenciaDoTesteDeUmAno& conferencia, const LinhaDoRelatorio& linha);

// O resultado do --simular (regra 4.1).
struct ResultadoDaSimulacaoSemTela {
    std::vector<LinhaDoRelatorio> linhas;
    std::string textoDoRelatorio;  // o relatorio.csv inteiro
    ConferenciaDoTesteDeUmAno conferencia;
    std::vector<std::string> linhasDoConsole;  // os eventos e os avisos (os resumos ficam no relatório)
    int64_t ticksRodados = 0;
    double segundosQueLevou = 0;
    double microssegundosPorTick = 0;
};

// Regra 4.1: monta a vila, roda os dias o mais rápido possível e confere os
// critérios em cada meia-noite. A semente vazia usa a do vila_inicial.json.
// Os NPCs a mais só servem ao teste de desempenho.
// Com "salvarECarregarNoDia" (marco 9, FL9-7), na meia-noite que começa esse dia a partida
// é salva, carregada de volta e segue: o relatório tem que sair igual ao de uma partida sem save.
ResultadoDaSimulacaoSemTela rodarASimulacaoSemTela(const DadosDoJogo& dados, int quantidadeDeDias, std::optional<uint64_t> semente,
                                                   int npcsAMais, std::optional<int> salvarECarregarNoDia = std::nullopt);
