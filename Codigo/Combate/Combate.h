#pragma once

// O combate (Ficha Lógica: Combate, fase D1): a fórmula do golpe (o dano da arma × a habilidade,
// a vantagem contra a armadura e a Defesa), o acerto a distância, o Medo, a evolução, o duelo
// simulado e o cálculo rápido, que substitui a força simples da Viagem (VI1-4). O combate ao vivo
// no Mapa Local fica para quando houver quem lute lá (os Controlados, D2).

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

class ListaDeProblemasDosDados;

struct DefinicaoDaArma {
    int danoMinimo = 2;
    int danoMaximo = 4;
    int intervaloEmTicks = 20;
    int alcanceEmTiles = 1;
    std::string habilidade = "corpo_a_corpo";  // ou "longo_alcance"
    std::string classe;                         // "corte", "impacto", "arco", "perfurante"; vazio = sem vantagem
};

struct KitDeCombate {
    std::string arma = "soco";
    std::vector<std::string> armadura;  // as peças: "leve", "media", "pesada"
    int habilidade = 30;
};

// O data/combate.json (parte 2 da ficha).
struct RegrasDoCombate {
    int vidaMaxima = 100;
    std::map<std::string, DefinicaoDaArma> armas;
    int danoPorRankEmPorcento = 25;
    std::map<std::string, int> defesaPorPecaRank1;
    std::map<std::string, std::map<std::string, int>> vantagemEmPorcento;  // [classe da arma][peso da armadura]
    int acertoBase = 90;
    int acertoPorTileAcimaDe3 = -5;
    int acertoPorPontoDeHabilidadeEmDecimos = 5;  // 0,5% por ponto acima (ou abaixo) de 50
    int acertoMinimo = 10;
    int acertoMaximo = 95;
    int acertoNaChuva = -10;
    int acertoNaTempestade = -20;
    int medoPorVidaPerdidaEmDecimos = 5;  // 0,5 por ponto de Vida perdida
    int medoPorAliadoCaido = 10;
    int medoInimigoDuasVezesMaisForte = 20;
    int coragemPadrao = 60;
    int rendicaoAcimaDaCoragem = 30;
    int evolucaoPorGolpeEmCentesimos = 10;
    int perdaDoGrupoDerrotadoEmPorcento = 75;  // 4.7: o grupo Inconsciente perde 75% para os bandidos
    int vidaMaximaPerdidaPeloVencedorEmPorcento = 90;
    std::map<std::string, KitDeCombate> kits;

    const DefinicaoDaArma& arma(const std::string& id) const;
};

RegrasDoCombate lerArquivoDoCombate(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

// Um combatente: o kit (ou o equipamento de verdade, quando houver) e a Vida.
struct Combatente {
    std::string arma = "soco";
    int rank = 1;
    int habilidadeEmCentesimos = 3000;  // 30,00
    int defesaEmCentesimos = 0;         // a habilidade Defesa (só sobe nos Controlados)
    std::vector<std::string> armadura;
    int vida = 100;
    int coragem = 60;
};

Combatente combatenteDoKit(const RegrasDoCombate& regras, const std::string& kit, int vidaEmPorcento = 100);
// Um lado de N combatentes do mesmo kit.
std::vector<Combatente> ladoDoKit(const RegrasDoCombate& regras, const std::string& kit, int quantos, int vidaEmPorcento = 100);

// Regra 4.2, passo 4: o peso mais frequente das peças (sem armadura = Leve, CB1-2; empate = Média).
std::string classeDePesoDaArmadura(const std::vector<std::string>& armadura);
// A Defesa das peças, em centésimos (leve 1, média 2, pesada 3, + 25% por Rank).
int defesaDaArmaduraEmCentesimos(const RegrasDoCombate& regras, const Combatente& combatente);
// Regra 4.2, passos 1 a 3: o dano de um golpe com o dano sorteado da arma (mínimo 1).
int calcularDanoDoGolpe(const RegrasDoCombate& regras, const Combatente& atacante, const Combatente& alvo, int danoSorteado);
// O dano médio de um golpe contra a classe de peso e a Defesa dadas, em centésimos (mínimo 100).
int danoMedioEmCentesimos(const RegrasDoCombate& regras, const Combatente& atacante, const std::string& pesoDoAlvo, int defesaDoAlvoEmCentesimos);
// Regra 4.2: a chance de acertar a distância, em %.
int chanceDeAcertoADistancia(const RegrasDoCombate& regras, int habilidade, int distanciaEmTiles, bool chuva, bool tempestade);
// Regra 4.4: o Medo.
int calcularMedo(const RegrasDoCombate& regras, int vidaEmPorcento, int aliadosCaidos, bool inimigoDuasVezesMaisForte);
// Regra 4.5: a habilidade depois de um golpe, em centésimos.
int evoluirHabilidade(const RegrasDoCombate& regras, int habilidadeEmCentesimos);
// Regra 4.6 (CB1-4): a chance de alguma de N testemunhas ver, em por mil (à noite, 70% cada).
int chanceDeSerVistoPorMil(int testemunhas, bool noite);

// Regra 4.8, passos 1 e 2: o poder de um lado contra o outro (dano por tick em milésimos × Vida).
int64_t poderDoLado(const RegrasDoCombate& regras, const std::vector<Combatente>& lado, const std::vector<Combatente>& ladoOposto);

struct ResultadoDoCalculoRapido {
    bool venceuOLadoA = false;
    int perdaDeVidaDoVencedorEmPorcento = 0;  // a Vida média que o lado vencedor perdeu na luta, até 90%
};
// Regra 4.8 (rodada CB2, opção C): roda o combate simulado, com o Medo, sorteando o dano pela semente da luta.
ResultadoDoCalculoRapido resolverPorCalculoRapido(const RegrasDoCombate& regras, const std::vector<Combatente>& ladoA, const std::vector<Combatente>& ladoB,
                                                  uint32_t sementeDaLuta);
// A chance de o lado A vencer, em por mil: "lutas" combates simulados com sementes fixas (para mostrar ao jogador).
int chanceDeVencerEmPorMil(const RegrasDoCombate& regras, const std::vector<Combatente>& ladoA, const std::vector<Combatente>& ladoB, int lutas = 100);

// O combate visto (4.1 a 4.4), sem o mapa: todos no alcance, a tick, até um lado ficar fora (caído,
// fugido ou rendido). "sortear(min, max)" sorteia o dano; "comMedo" liga a fuga pelo Medo.
struct ResultadoDoDuelo {
    bool venceuOLadoA = false;
    int ticks = 0;
    int golpesDoLadoA = 0;
    std::vector<int> vidaDoLadoA;
    std::vector<int> vidaDoLadoB;
};
ResultadoDoDuelo simularOCombate(const RegrasDoCombate& regras, std::vector<Combatente> ladoA, std::vector<Combatente> ladoB, bool comMedo,
                                 const std::function<int(int, int)>& sortear);
