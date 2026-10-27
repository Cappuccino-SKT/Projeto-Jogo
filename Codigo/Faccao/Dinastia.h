#pragma once

// As dinastias das facções (Ficha Lógica: Companheiros e Diálogo, regras 4.6 e 4.7, marco D2):
// o líder, a família e 2 nobres de cada facção. No dia 1 de cada estação eles envelhecem (na
// Primavera), morrem de velhice, casam e têm filhos; quando o líder morre, o herdeiro sai da
// regra de sucessão da cultura. As regras ficam no data/companheiros.json.

#include <cstdint>
#include <string>
#include <vector>

class ListaDeProblemasDosDados;
class SorteioComSemente;
struct RegrasDaPopulacao;

// Os 5 traços dos Importantes e dos Controlados, de 0 a 100 (ficha, parte 2; CP1-1).
struct Personalidade {
    int coragem = 50;
    int ganancia = 50;
    int agressividade = 50;
    int sociabilidade = 50;
    int lealdade = 50;

    bool operator==(const Personalidade&) const = default;
};

// O bloco "familia" e o bloco "dinastia" do companheiros.json.
struct RegrasDaFamilia {
    int gravidezEmEstacoes = 1;
    int chanceDeFilhoPorEstacaoEmPorcento = 10;
    int chanceDeCasarPorEstacaoEmPorcento = 25;
    int fertilAte = 30;  // a mulher tem filhos da idade de adulto até esta (rodada HI12)
    int nobresPorDinastia = 2;
    int liderCasadoAPartirDe = 12;  // CP1-5: o líder do começo com 12 anos ou mais já é casado e tem filhos adultos
    int membrosNoMaximo = 12;       // [Proposta] acima disto, ninguém mais casa nem tem filho na dinastia
    int legitimidadeNaCrise = 30;
    int tracoMinimo = 20;  // os traços e as habilidades sorteados vão de 20 a 80
    int tracoMaximo = 80;
};

RegrasDaFamilia lerArquivoDosCompanheiros(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

// Um NPC Importante da dinastia.
struct MembroDaDinastia {
    int numero = 0;  // único na dinastia; desempata os gêmeos (10.5)
    std::string nome;
    bool ehMulher = false;
    int idade = 5;
    Personalidade personalidade;
    int carisma = 50;
    int corpoACorpo = 50;
    int especialista = 50;
    bool ehNobre = false;      // um dos nobres de fora da família (só sobem na crise)
    int conjuge = -1;          // o número do cônjuge; −1 = solteiro
    int pai = -1;              // os números dos pais; −1 = de fora
    int mae = -1;
    int estacoesDeGravidez = 0;  // > 0: grávida; o filho nasce quando chega a 0
    bool vidaProlongada = false;  // HI11-1: não envelhece enquanto a Torre Arcana estiver de pé

    bool operator==(const MembroDaDinastia&) const = default;
};

struct DinastiaDaFaccao {
    std::vector<MembroDaDinastia> membros;
    int numeroDoLider = 0;
    int proximoNumero = 0;

    const MembroDaDinastia* procurar(int numero) const;
    MembroDaDinastia* procurar(int numero);
    const MembroDaDinastia& lider() const { return *procurar(numeroDoLider); }

    bool operator==(const DinastiaDaFaccao&) const = default;
};

// O que a dinastia precisa da cultura: os nomes e a regra de sucessão ("hereditaria", "duelo",
// "merito", "eleicao_do_clero").
struct CulturaDaDinastia {
    std::vector<std::string> nomesDeHomem;
    std::vector<std::string> nomesDeMulher;
    std::string sucessao;
};

// O líder fixo do começo (Facção 4.6) com o que a dinastia acrescenta (CP1-5).
struct LiderDoComeco {
    std::string nome;
    bool ehMulher = false;
    int idade = 12;
    Personalidade personalidade;
    bool vidaProlongada = false;
};

// Regra 4.6, passo 4: o líder, o cônjuge e 1 ou 2 filhos adultos (com 12 anos ou mais) e os nobres.
DinastiaDaFaccao montarADinastiaDoComeco(const LiderDoComeco& liderDoComeco, const CulturaDaDinastia& cultura, const RegrasDaFamilia& familia,
                                         const RegrasDaPopulacao& populacao, SorteioComSemente& sorteio);

// Regra 4.7: o número do herdeiro; −1 = ninguém adulto (crise). O líder (morto ou não) não entra.
int escolherHerdeiro(const DinastiaDaFaccao& dinastia, const std::string& sucessao, int idadeAdulto);

struct ResultadoDaEstacaoDaDinastia {
    bool liderMorreu = false;
    bool crise = false;  // ninguém adulto: um nobre subiu (4.7)
    std::string nomeDoLiderMorto;
    int idadeDoLiderMorto = 0;
    std::string nomeDoNovoLider;
};

// O dia 1 de uma estação (4.6 e 4.7): a idade (só na Primavera), a velhice, os partos, os
// casamentos, as gravidezes novas, os nobres repostos e a sucessão. "torreDePe": a Vida
// prolongada vale (até a Magia, D4, a Torre conta sempre de pé).
ResultadoDaEstacaoDaDinastia passarUmaEstacaoNaDinastia(DinastiaDaFaccao& dinastia, const CulturaDaDinastia& cultura, const RegrasDaFamilia& familia,
                                                        const RegrasDaPopulacao& populacao, bool ehPrimavera, bool torreDePe, SorteioComSemente& sorteio);
