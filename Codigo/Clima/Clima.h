#pragma once

// O clima do dia (Ficha Lógica: Clima e Estações, fase A1): o tipo de clima,
// as regras lidas do data/clima.json, o sorteio do clima do dia e a conta da
// lenha do inverno. Nada aqui conhece a vila: o ClimaDaVila.h liga isto à
// ordem do dia, à produção, à Felicidade e aos NPCs.

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "Dados/ProblemasDosDados.h"
#include "Tempo/RelogioDoMundo.h"
#include "Tempo/SorteioComSemente.h"

// O clima sorteia com a sequência 3 (os NPCs usam a 1 e os eventos a 2): um
// evento a mais não muda o clima.
constexpr uint64_t sequenciaDoSorteioDoClima = 3;

enum class TipoDeClima { Limpo, Nublado, Chuva, Tempestade, Neve, Nevoa, CalorForte };

// O "clima.json" lido. Os valores padrão são os da ficha.
struct RegrasDoClima {
    int chanceDeRepetirOClimaEmPorMil = 600;
    // Por estação (na ordem do Estacao), a lista de (clima, chance em por mil),
    // na ordem do arquivo: o sorteio anda por ela somando as chances.
    std::array<std::vector<std::pair<TipoDeClima, int>>, 4> chancesPorEstacao;
    int trabalhoAoArLivreNaTempestadeEmPorcento = 50;
    bool periodoLivreEmCasaNaTempestade = true;
    int lenhaNaNeveEmPorcento = 150;
    int felicidadeNoCalorForte = -2;
    int lenhaPorMoradorPorDiaEmCentesimos = 25;
    int lenhaDoBiomaEmPorcento = 100;
    int felicidadeSemLenha = -10;
    TipoDeClima climaDoPrimeiroDia = TipoDeClima::Limpo;
    // Regra 4.6 (CL1-2 e CL1-3): nos últimos dias do Outono, o estoque-alvo
    // da Tora e do Grão cresce.
    int diasDoFimDoOutonoParaEstocar = 4;
    int diasDeLenhaEstocada = 5;
    int diasDeGraoEstocado = 5;
    std::string itemDaLenha = "item.tora";
    std::string itemDoGraoEstocado = "item.grao";
};

// O estado do clima (vai no save, menos o que é "de hoje").
struct EstadoDoClima {
    TipoDeClima climaDeHoje = TipoDeClima::Limpo;
    TipoDeClima climaDeOntem = TipoDeClima::Limpo;  // o do dia que acabou (para o relatório)
    int lenhaDevidaEmCentesimos = 0;  // de 0 a 99 depois de pagar
    int diasEmDeficitDeLenha = 0;
    // O dia de hoje (guardado no passo "clima" e ao carregar um save).
    Estacao estacaoDeHoje = Estacao::Primavera;
    int diaDaEstacaoDeHoje = 1;
    int diasPorEstacao = 16;
    // Só do dia (não vão no save; o relatório e a Felicidade usam).
    int lenhaTiradaHoje = 0;
    bool faltouLenhaHoje = false;
    bool climaMudouHoje = false;
};

// "limpo" vira Limpo; um nome que não existe, nada.
std::optional<TipoDeClima> climaPeloNome(const std::string& nome);

// O nome no arquivo ("calor_forte") e o identificador do texto ("clima.calor_forte").
std::string nomeDoClimaNoArquivo(TipoDeClima clima);
std::string identificadorDeTextoDoClima(TipoDeClima clima);

// Lê o clima.json. As chances de cada estação precisam somar 1.000; um
// clima que não existe é erro (anotado na lista, sem parar).
RegrasDoClima lerArquivoDoClima(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

// A chance (em por mil) de um clima numa estação; 0 se ele não aparece.
int chanceDoClimaNaEstacao(TipoDeClima clima, Estacao estacao, const RegrasDoClima& regras);

// Regra 4.1, passos 2 e 3: repete o de ontem (60%, se ele existe na estação
// de hoje) ou sorteia da tabela, andando na ordem do arquivo. Sempre tira os
// dois números (o de repetir e o da tabela), para o sorteio andar igual.
TipoDeClima sortearClimaDoDia(TipoDeClima climaDeOntem, Estacao estacaoDeHoje, const RegrasDoClima& regras, SorteioComSemente& sorteio);

// A mesma regra, com os dois números já sorteados (de 0 a 999), para os testes.
TipoDeClima escolherClimaDoDia(TipoDeClima climaDeOntem, Estacao estacaoDeHoje, int numeroDeRepetir, int numeroDaTabela,
                               const RegrasDoClima& regras);

// Regra 4.2, passo 1: o trabalho Braçal (ao ar livre) na tempestade, em %.
int porcentagemDoTrabalhoAoArLivre(TipoDeClima climaDeHoje, const RegrasDoClima& regras);

// Regra 4.3, passo 1: a lenha de hoje, em centésimos de Tora: moradores × 25
// × bioma ÷ 100 × neve ÷ 100 × eventos (o Inverno rigoroso) ÷ 100, cada
// divisão para baixo. 15 moradores, neve: 562.
int calcularLenhaDoDiaEmCentesimos(int moradores, TipoDeClima climaDeHoje, int multiplicadorDosEventosEmPorcento, const RegrasDoClima& regras);

// Regra 4.3, passos 2 a 5, sem a vila: soma a lenha de hoje à dívida e tira as
// Toras que o estoque tem. Devolve quantas Toras tirar; a dívida e os dias em
// déficit mudam no estado. Com 637 devidos e 1 Tora: tira 1, faltou, dívida 37.
int pagarALenhaDevida(EstadoDoClima& estado, int lenhaDeHojeEmCentesimos, int torasNoEstoque);
