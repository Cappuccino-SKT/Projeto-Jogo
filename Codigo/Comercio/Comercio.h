#pragma once

// O comércio entre as cidades (Ficha Lógica: Comércio, fase B2): a loja que compra até o
// estoque-alvo e paga pouco acima dele (4.7), a sobra e a falta de cada cidade (4.2), a
// conta de uma viagem de caravana (4.3), a decisão de sair (4.4) e a chegada (4.5 e 4.6).
// As cidades são as SimulacaoDaVila do mundo: a vila e as outras cidades (B1b).

#include <cstdint>
#include <string>
#include <vector>

#include "Vila/SimulacaoDaVila.h"

// O que uma loja comprou ou vendeu: quantas unidades e o valor, em décimos.
struct ResultadoDaLoja {
    int quantidade = 0;
    int64_t valorEmDecimos = 0;
};

// O estoque-alvo do Mercado Local, em unidades.
int estoqueAlvoDoItem(const ContextoDaProducao& cidade, int numeroDoItem);

// Regra 4.2: a sobra (estoque da Cidade acima do alvo) e a falta (abaixo do alvo, menos o
// que as caravanas já levam para lá).
int sobraDoItem(const ContextoDaProducao& cidade, int numeroDoItem);
int faltaDoItem(const ContextoDaProducao& cidade, const std::vector<int>& aCaminhoPorItem, int numeroDoItem);

// Regra 4.7, passos 2 e 3: quantas unidades a loja compra e quanto paga. 90% do preço local
// até o estoque-alvo, 45% do preço base além dele, e só enquanto o Banco fica acima da reserva.
// Fase B4 (Reputação, 4.4): "faixaEmPorcento" é a venda_em_porcento da faixa do jogador (100 para os outros).
ResultadoDaLoja calcularAVendaParaALoja(const ContextoDaProducao& loja, int numeroDoItem, int quantidade, int faixaEmPorcento = 100);
// A mesma conta só com os números (para os testes): 40 Tábuas, 20 até o alvo, preço local 3,0,
// base 1,5: 20 × 2,7 + 20 × 0,7 = 68 moedas.
ResultadoDaLoja calcularAVendaComOsNumeros(int quantidade, int ateOAlvo, int precoLocalEmDecimos, int precoBaseEmDecimos, int64_t dinheiroAcimaDaReserva,
                                           const RegrasDoComercio& regras, int faixaEmPorcento = 100);
// A mesma conta, aplicada: os itens entram no estoque da Cidade e o dinheiro sai do Banco.
ResultadoDaLoja venderNaLoja(ContextoDaProducao& loja, int numeroDoItem, int quantidade, int faixaEmPorcento = 100);

// Regra 4.7, passos 1 e 4: a loja vende a 110% do preço local, até o dinheiro de quem compra;
// ao jogador, nunca abaixo de 2 dias do consumo previsto (RE1-4).
// Fase B4 (Reputação, 4.4): "faixaEmPorcento" é a compra_em_porcento da faixa do jogador.
ResultadoDaLoja calcularACompraNaLoja(const ContextoDaProducao& loja, int numeroDoItem, int quantidade, int64_t dinheiroEmDecimos, bool ehOJogador,
                                     int faixaEmPorcento = 100);
// A mesma conta, aplicada: os itens saem do estoque da Cidade e o dinheiro entra no Banco.
ResultadoDaLoja comprarNaLoja(ContextoDaProducao& loja, int numeroDoItem, int quantidade, int64_t dinheiroEmDecimos, bool ehOJogador, int faixaEmPorcento = 100);

// A força da caravana: condutores como Civis (1) e guardas (3): 2 + 6 = 8.
int forcaDaCaravana(const DadosDoJogo& dados);

// Regra 4.3: a conta de uma viagem da cidade "casa" até "destino".
struct PlanoDaViagemDaCaravana {
    int destino = 0;
    std::vector<int> ida;    // a carga por item
    std::vector<int> volta;  // o que comprar lá, por item
    int64_t ganhoDaIda = 0;
    int64_t ganhoDaVolta = 0;
    int64_t valorDaVendaDaIda = 0;
    int dias = 0;
    int64_t custo = 0;
    int riscoPorMil = 0;
    int64_t lucroEsperado = 0;
};
PlanoDaViagemDaCaravana calcularLucroEsperadoDaViagem(SimulacaoDaVila& casa, const CaravanaDaCidade& caravana, SimulacaoDaVila& destino);

// Regra 4.4, no passo 3b de cada dia: cada caravana em casa escolhe se sai e para onde. As
// cidades pelo número do mapa; as caravanas de uma cidade, pela ordem de criação.
std::vector<std::string> decidirAsViagensDasCaravanas(SimulacaoDaVila& vila);

// A cada tick: as caravanas andam (Viagem), são atacadas (4.11), chegam ao destino (4.5) e em casa (4.6).
void avancarAsCaravanasUmTick(SimulacaoDaVila& vila);

// As cidades do mundo (a vila e as outras), na ordem do número do mapa.
std::vector<SimulacaoDaVila*> cidadesDoMundo(SimulacaoDaVila& vila);

// O dinheiro de todas as cidades, do jogador, do grupo e das caravanas (teste 4).
int64_t dinheiroDoMundo(const SimulacaoDaVila& vila);

// "Comércio: 31 viagens, 3 atacadas, perda de 7% do valor levado; ..." (o fim do --simular).
std::string resumoDoComercio(const SimulacaoDaVila& vila);

// Regra 4.8: os preços que o grupo do jogador viu numa cidade, com a data.
void anotarOsPrecosDaCidade(SimulacaoDaVila& vila, int numeroDoMapa);

// O jogador compra ou vende na loja da cidade onde o grupo está (fora da vila), com o que o
// grupo leva. Devolve o motivo da recusa, ou vazio; a linha do console vai em "linhas".
std::string comerciarNaCidadeDoGrupo(SimulacaoDaVila& vila, int numeroDoItem, int quantidade, bool comprar, std::vector<std::string>& linhas);
