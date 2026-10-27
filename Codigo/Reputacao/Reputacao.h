#pragma once

// A reputação do jogador (Ficha Lógica: Reputação, fase B4): o que cada facção e cada Vila
// Livre pensa dele, de −100 a +100. A faixa muda os preços das lojas das outras cidades;
// o valor volta devagar para a base. Nesta versão, só vencer bandidos e doar mudam o valor.

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class SimulacaoDaVila;
struct MundoDoJogo;
class ListaDeProblemasDosDados;

struct FaixaDaReputacao {
    std::string nome;              // "amigavel"
    std::string texto;             // "Amigável"
    int ate = 0;                   // a faixa vai até este valor, começando logo depois da anterior
    int compraEmPorcento = 100;    // −1 = a loja não negocia (Inimigo)
    int vendaEmPorcento = 100;
};

// O data/reputacao.json (parte 2 da ficha).
struct RegrasDaReputacao {
    std::vector<FaixaDaReputacao> faixas;
    int voltaPorEstacao = 2;
    std::map<std::string, int> acoes;              // "venceu_bando_no_territorio": 1
    std::map<std::string, std::string> nomesDasFaccoes;  // "escravista": "Sultanato de Zahran"
    std::map<std::string, std::string> artigosDasFaccoes;  // "escravista": "o"; sem entrada, "a"
    int mudancasNoPorQue = 5;

    int acao(const std::string& nome) const;
};

RegrasDaReputacao lerArquivoDaReputacao(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);

// Uma linha do "Por quê?": "+1: venceu um bando em (6,4), dia 12".
struct MudancaDaReputacao {
    int valor = 0;
    std::string motivo;
    int64_t dia = 0;

    bool operator==(const MudancaDaReputacao&) const = default;
};

// O estado (no save): um lugar por dono (as facções e as Vilas Livres, na ordem de donosDaReputacao).
struct EstadoDaReputacao {
    std::vector<int> reputacaoPorFaccao;
    std::vector<int> basePorFaccao;
    std::vector<std::vector<MudancaDaReputacao>> ultimasMudancas;  // da mais velha para a mais nova

    bool operator==(const EstadoDaReputacao&) const = default;
};

// Os donos da reputação, na ordem do cidades_iniciais.json: cada facção ("escravista") e cada
// Vila Livre (pelo nome da cidade). A vila do jogador entra na lista, mas é sempre Neutro.
std::vector<std::string> donosDaReputacao(const MundoDoJogo& mundo);
// O nome na tela: "Escravista", "Vila Livre do Norte".
std::string nomeDoDonoDaReputacao(const RegrasDaReputacao& regras, const std::string& dono);
// O nome com o artigo, junto da preposição: ("escravista", "de") → "do Sultanato de Zahran";
// ("guerreira", "por") → "pelos Clãs de Tian-Lang"; sem preposição, "os Clãs de Tian-Lang".
// Com "maiuscula", a primeira letra sai maiúscula (começo da frase).
std::string nomeDoDonoComArtigo(const RegrasDaReputacao& regras, const std::string& dono, const std::string& preposicao = "", bool maiuscula = false);
// Regra 4.1 (RE1-6): sem estilo de início, a base é 0 e o valor começa na base.
void montarAReputacaoDaPartidaNova(EstadoDaReputacao& reputacao, const MundoDoJogo& mundo);

// A posição do dono da cidade do mapa na lista; −1 = sem cidade, ou a vila do jogador (RE1-5).
int donoDaCidadeDoMapa(const MundoDoJogo& mundo, int numeroDoMapa);
// A posição da facção dona do mapa (a cidade com facção ou o território de uma facção só);
// −1 = em disputa, sem dono ou Vila Livre.
int faccaoDonaDoTerritorio(const MundoDoJogo& mundo, int numeroDoMapa);

// A posição da faixa do valor (0 = Inimigo ... 4 = Aliado).
int faixaDaReputacao(const RegrasDaReputacao& regras, int valor);
// A faixa da loja da cidade do mapa para o jogador (a vila do jogador: Neutro).
const FaixaDaReputacao& faixaDoJogadorNaCidade(const SimulacaoDaVila& vila, int numeroDoMapa);

// Regra 4.2: soma, prende em −100 e +100, guarda o "Por quê?" e avisa se a faixa mudou.
// Devolve a linha do console (vazia se a faixa não mudou).
std::string mudarReputacao(SimulacaoDaVila& vila, int dono, int valor, const std::string& motivo);

// Regra 4.3: no dia 1 de cada estação, cada valor anda 2 pontos para a base.
std::vector<std::string> voltarReputacaoParaABase(SimulacaoDaVila& vila);

// Regra 4.2: o grupo doa na cidade onde está; +1 a cada 100 moedas, só até a base + 20.
// O dinheiro vai para o Banco da cidade. Devolve o motivo da recusa, ou vazio.
std::string doarParaAFaccao(SimulacaoDaVila& vila, int moedas, std::vector<std::string>& linhas);

// Regra 4.2: o grupo venceu um bando no mapa onde está: +1 com a facção dona do território.
std::string anotarOBandoVencido(SimulacaoDaVila& vila);

// Regra 4.4, passo 4: o preço por unidade com a porcentagem da faixa, em décimos, a metade para cima.
int64_t precoComAFaixa(int64_t precoEmDecimos, int porcentagem);
