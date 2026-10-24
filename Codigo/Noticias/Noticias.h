#pragma once

// As notícias do mundo (Ficha Lógica: Notícias, fase B3): cada acontecimento importante
// nasce num mapa e numa hora e chega aos outros mapas 3 horas por mapa de distância. O
// jogador fica sabendo quando a notícia chega ao mapa onde ele está, com a precisão da
// distância. As Notícias não sorteiam nem mexem em estoque ou dinheiro.

#include <cstdint>
#include <string>
#include <vector>

class SimulacaoDaVila;
struct MundoDoJogo;
class ListaDeProblemasDosDados;

enum class ImportanciaDaNoticia { Nenhuma, Local, Regional, Mundial };

// O data/noticias.json (parte 2 da ficha).
struct RegrasDasNoticias {
    int horasPorMapa = 3;
    int alcanceEmMapas[4] = {0, 2, 5, 99};  // pela importância (Nenhuma, Local, Regional, Mundial)
    int diasNoQuadro[4] = {0, 8, 16, 64};
    int precisaoExataAteMapas = 2;
    int precisaoArredondadaAteMapas = 5;
    int precoForaDaMediaEmPorcento = 30;
    int precoMesmaNoticiaDepoisDeDias = 8;
    int precoNoticiasPorCidadePorDia = 1;
    int limiteDeNoticiasAtivas = 500;
};

RegrasDasNoticias lerArquivoDasNoticias(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);
// "regional" → Regional; um texto desconhecido → Nenhuma.
ImportanciaDaNoticia lerImportanciaDaNoticia(const std::string& texto);

// Um acontecimento de uma cidade que pode virar notícia (os eventos anotam, a vila recolhe).
struct AcontecimentoParaNoticia {
    std::string tipo;     // "evento.epidemia"
    std::string titulo;   // "Epidemia"
    ImportanciaDaNoticia importancia = ImportanciaDaNoticia::Nenhuma;
    int64_t numero = -1;  // em pessoas; −1 = sem número
    std::string rotuloDoNumero;  // "doentes"
};

// O registro da parte 2 da ficha.
struct Noticia {
    int64_t numeroDaNoticia = 0;
    std::string tipo;  // "evento.epidemia", "comercio.caravana_atacada", "comercio.preco"
    ImportanciaDaNoticia importancia = ImportanciaDaNoticia::Local;
    int mapaDeOrigem = 0;
    int64_t tickDeOrigem = 0;
    int64_t diaDeOrigem = 0;
    std::string titulo;          // "Epidemia na Cidade Religiosa"
    int64_t numero = -1;         // em pessoas; −1 = sem número
    std::string rotuloDoNumero;  // "doentes"
    std::string porQue;          // o fim do texto, sem precisão: "+69% da região"
    int64_t diaEmQueSaiDoQuadro = 0;
    int numeroDoItem = -1;  // só as notícias de preço
    int precoEmDecimos = 0;

    bool operator==(const Noticia&) const = default;
};

// O que o jogador sabe de uma notícia ativa: a melhor distância até agora.
struct NoticiaQueOJogadorSabe {
    int64_t numeroDaNoticia = 0;
    int64_t diaEmQueSoube = 0;
    int distancia = 0;

    bool operator==(const NoticiaQueOJogadorSabe&) const = default;
};

// Uma linha da Crônica do mundo (as Regionais e Mundiais que o jogador soube, para sempre).
struct LinhaDaCronica {
    int64_t numeroDaNoticia = 0;
    int64_t diaDoAcontecimento = 0;
    int64_t diaEmQueSoube = 0;
    std::string texto;

    bool operator==(const LinhaDaCronica&) const = default;
};

struct EstadoDasNoticias {
    std::vector<Noticia> ativas;  // da mais velha para a mais nova
    int64_t proximoNumeroDaNoticia = 1;
    std::vector<NoticiaQueOJogadorSabe> noticiasQueOJogadorSabe;
    std::vector<LinhaDaCronica> cronicaDoMundo;
    // As notícias de preço: o último dia em que cada item virou notícia em cada mapa
    // ([numero do mapa − 1][item]; −1 = nunca).
    std::vector<std::vector<int64_t>> diaDaUltimaNoticiaDePreco;
    int mapaDoJogadorNaUltimaConferencia = 0;  // para saber quando ele entrou num mapa novo

    bool operator==(const EstadoDasNoticias&) const = default;
};

// Regra 4.2: o menor número de passos na malha (sem mapas bloqueados nesta versão).
int distanciaEmMapas(const MundoDoJogo& mundo, int mapaA, int mapaB);
// Regra 4.2: o tick em que a notícia chega ao mapa; −1 = nunca (passa do alcance).
int64_t tickDeChegada(const Noticia& noticia, int numeroDoMapa, const MundoDoJogo& mundo, const RegrasDasNoticias& regras, int ticksPorHora);

// Regra 4.4: o número pela distância ("1.880", "cerca de 2.000", "milhares de").
std::string numeroComPrecisao(int64_t numero, int distancia, const RegrasDasNoticias& regras);
// O texto da notícia como o jogador sabe: "Epidemia na Cidade Religiosa: cerca de 2.000 doentes".
std::string textoComPrecisao(const Noticia& noticia, int distancia, const RegrasDasNoticias& regras);

// Regra 4.1: cria a notícia na vila (que guarda as notícias do mundo). Devolve o número dela.
int64_t criarNoticia(SimulacaoDaVila& vila, Noticia noticia);

// Na virada do dia: os acontecimentos de hoje da vila e das outras cidades viram notícias.
void recolherOsAcontecimentosDoDia(SimulacaoDaVila& vila);

// Regra 4.6, passos 2 e 3: o item com o preço 30% ou mais longe da média da região, de maior
// diferença, que não virou notícia nessa cidade nos últimos 8 dias; −1 = nenhum.
int escolherOItemDaNoticiaDePreco(const std::vector<int>& precosDaCidade, const std::vector<int64_t>& somaDaRegiao, int64_t cidadesDaRegiao,
                                  const std::vector<int64_t>& diaDaUltimaNoticia, int64_t hoje, const RegrasDasNoticias& regras, int64_t& diferencaEmPorcento);

// Regra 4.6 (o passo 3c): no máximo 1 notícia de preço por cidade por dia.
void criarNoticiasDePreco(SimulacaoDaVila& vila);

// Regra 4.3: o que chegou ao jogador pelos mapas da rede dele (o mapa do grupo e, quando
// existirem, as caravanas dele). "aoEntrarNumMapa" junta as novas num aviso só (NO1-3).
// Devolve as linhas do console (as notificações das Regionais e Mundiais).
std::vector<std::string> conferirNoticiasDoJogador(SimulacaoDaVila& vila, const std::vector<int>& mapasDaRede, bool aoEntrarNumMapa);

// A cada tick, na vila: a conferência na virada da hora e ao entrar num mapa, e as notícias
// que passaram do prazo saem (4.7). Devolve as linhas do console.
std::vector<std::string> avancarAsNoticiasUmTick(SimulacaoDaVila& vila);

// O que o jogador sabe de uma notícia, ou nullptr.
const NoticiaQueOJogadorSabe* oQueOJogadorSabe(const EstadoDasNoticias& noticias, int64_t numeroDaNoticia);
