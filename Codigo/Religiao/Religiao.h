#pragma once

// A religião (Ficha Lógica: Religião, fase C4): a fé de cada cultura, a cobertura dos Templos
// (e da Torre Arcana), o 6º fator da Felicidade, as oferendas, os festivais do calendário, a lei
// de Tolerância nas minorias, a assimilação, o Templo da Religiosa na Epidemia, a Bênção e a
// peregrinação do jogador. A vila sem facção não muda (RL1-3).

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class SimulacaoDaVila;
struct DadosDoJogo;
struct ContextoDaProducao;
class ListaDeProblemasDosDados;

enum class Devocao { Nenhuma, Baixa, Media, Alta };

struct FeDaCultura {
    Devocao devocao = Devocao::Nenhuma;
    std::string culto;  // "templo", "torre_arcana" ou "laboratorio"
};

struct FestivalDoCalendario {
    std::string cultura;  // "todas" ou a cultura
    std::string nome;
    int estacao = 0;      // a posição da estação (0 primavera ... 3 inverno)
    std::vector<int> dias;
    bool metade = false;  // os dias santos: +4 em vez de +8
};

struct ToleranciaDaLei {
    int impostoAMaisDasMinoriasEmPorcento = 0;
    int felicidadeDasMinorias = 0;  // em pontos
    int assimilacaoPorEstacaoEmPorcento = 0;
};

// O data/religiao.json (parte 2 da ficha).
struct RegrasDaReligiao {
    std::map<std::string, FeDaCultura> fes;
    int npcsPorSacerdote = 20;
    int felicidadeAlta = 10;
    int felicidadeAltaComCoberturaBaixa = -5;
    int coberturaMinimaDaAlta = 50;
    int felicidadeMedia = 7;
    int felicidadeBaixa = 4;
    int oferendaEmPorcentoDaRenda = 2;
    int festivalFelicidade = 8;
    int festivalDiasDoBonus = 8;
    int festivalComidaExtraEmPorcento = 50;
    int festivalProducaoNoPrimeiroDiaEmPorcento = 50;
    int festivalCustoEmPorcentoDoBanco = 2;
    std::vector<FestivalDoCalendario> festivais;
    std::map<std::string, ToleranciaDaLei> tolerancia;  // pela opção da lei: "fe_oficial"
    int mortesNaEpidemiaComOTemploDaReligiosaEmPorcento = 75;
    int peregrinacaoOferendaMinima = 50;
    int peregrinacaoReputacao = 10;
    int bencaoReputacaoMinima = 40;
    int bencaoReputacao = 10;
};

RegrasDaReligiao lerArquivoDaReligiao(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

// O estado de cada cidade (no save).
struct EstadoDaReligiaoDaCidade {
    int diasDoFestival = 0;        // os dias que faltam do bônus do festival
    int pontosDoFestival = 0;
    bool primeiroDiaDoFestival = false;  // hoje o turno vale metade
    bool diaDeFestival = false;          // hoje é dia do calendário (a comida extra)
    std::map<std::string, int> fracaoDeAssimilacaoEmPessoas;

    bool operator==(const EstadoDaReligiaoDaCidade&) const = default;
};

// O estado do jogador (no save, só na vila).
struct EstadoDaReligiaoDoJogador {
    int anoDaUltimaPeregrinacao = -1;
    int estacaoDaUltimaBencao = -1;     // a estação da partida (dia ÷ 16)
    int estacaoDoUltimoAtoDeFe = -1;    // peregrinação ou doação à Religiosa

    bool operator==(const EstadoDaReligiaoDoJogador&) const = default;
};

// O que a religião muda em cada cidade (derivado; a vila sem facção fica com o padrão).
struct EfeitosDaReligiaoNaCidade {
    int fatorEmDecimos = 0;               // o 6º fator da Felicidade (com a Perseguição nas minorias)
    int oferendaEmPorcentoDaRenda = 0;    // 4.3
    int felicidadeDoFestivalEmPontos = 0; // 4.4, no fator Eventos
    int producaoPresumidaEmPorcento = 100;  // 4.4, passo 3: o primeiro dia do festival
    int comidaExtraEmPorcento = 0;        // 4.4, passo 4: nos dias do festival
    bool semAFestaDaColheitaDaFatia = false;  // RL1-3: a cidade de facção usa o calendário
    int impostoAMaisDasMinoriasEmPorcento = 0;  // 4.5: a Fé oficial
    std::string culturaBase;
    int mortesDaEpidemiaEmPorcento = 100;  // 4.7
};

// Regra 4.4, passo 4: a classe compra a comida extra do festival (na ordem do dia, depois da cesta).
void comprarAComidaDoFestival(ContextoDaProducao& contexto);

// Regra 4.1: Sacerdotes × 20 ÷ NPCs, até 100%.
int coberturaComOsNumeros(int sacerdotes, int npcs, const RegrasDaReligiao& regras);
// Regra 4.2, passo 2: o fator de uma cultura pela Devoção e pela cobertura, em décimos.
int fatorDaFeEmDecimos(Devocao devocao, int coberturaEmPorcento, const RegrasDaReligiao& regras);
// Regra 4.2, passo 3: a média pesada pelos NPCs de cada cultura, em décimos (para baixo).
int fatorReligiaoDaClasseEmDecimos(const DadosDoJogo& dados, const std::map<std::string, int>& npcsPorCultura, const std::string& cultoDaCidade, int coberturaEmPorcento);
// Regra 4.4, passo 4 (RL1-2): a comida extra do festival, só da sobra acima do alvo.
int comidaExtraDoFestival(int civis, int comidaPorDia, int sobra, const RegrasDaReligiao& regras);
// Regra 4.6: quantos NPCs mudam em N estações (a fração anda e cada 100 vira 1 NPC).
int assimiladosEmEstacoes(int npcsDaMinoria, int taxaEmPorcento, int estacoes);
// Regra 4.7: as mortes da Epidemia com o Templo da Religiosa.
int mortesComOTemplo(int mortes, const RegrasDaReligiao& regras);

// Na vila, no dia novo, antes do dia das outras cidades: o festival do calendário, a cobertura,
// o fator, as oferendas, a Tolerância, o Templo na Epidemia e, no dia 1 da estação, a
// assimilação e a Bênção. Devolve as linhas do console.
std::vector<std::string> rodarAReligiaoNoDiaNovo(SimulacaoDaVila& vila);
// O que a religião passa para cada cidade (também ao carregar um save).
void atualizarAsCidadesPelaReligiao(SimulacaoDaVila& vila);
// Regra 4.9: a oferenda do grupo na Capital Religiosa. Devolve o motivo da recusa, ou vazio.
std::string fazerPeregrinacao(SimulacaoDaVila& vila, int moedas, std::vector<std::string>& linhas);
// A doação à Religiosa conta como ato de fé (4.8).
void anotarOAtoDeFe(SimulacaoDaVila& vila);
// A Capital Religiosa (8,4).
int mapaDaCidadeSanta(const DadosDoJogo& dados);
