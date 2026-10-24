// Os testes das notícias: a tabela da parte 8 da Ficha Lógica: Notícias (fase B3).

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Noticias/Noticias.h"
#include "Save/ArquivoDeSave.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

Noticia noticiaDeTeste(const MundoDoJogo& mundo, ImportanciaDaNoticia importancia, int coluna, int linha, int64_t tickDeOrigem)
{
    Noticia noticia;
    noticia.tipo = "teste";
    noticia.importancia = importancia;
    noticia.mapaDeOrigem = mundo.numeroDoMapaNa(coluna, linha);
    noticia.tickDeOrigem = tickDeOrigem;
    noticia.titulo = "Epidemia na Cidade Religiosa";
    noticia.numero = 1880;
    noticia.rotuloDoNumero = "doentes";
    return noticia;
}

void testarAChegadaEAPrecisao(const DadosDoJogo& dados)
{
    const MundoDoJogo& mundo = dados.mundo;
    const RegrasDasNoticias& regras = dados.regrasDasNoticias;
    // Teste 1: guerra em (1,1) às 10 h; (7,3) fica a 8 mapas (24 h) e (4,1) a 3 mapas (9 h).
    const int64_t dezHoras = 10 * 800;
    const Noticia guerra = noticiaDeTeste(mundo, ImportanciaDaNoticia::Mundial, 1, 1, dezHoras);
    conferir(tickDeChegada(guerra, mundo.numeroDoMapaNa(7, 3), mundo, regras, 800) == dezHoras + 24 * 800
                 && tickDeChegada(guerra, mundo.numeroDoMapaNa(4, 1), mundo, regras, 800) == dezHoras + 9 * 800,
             "o exemplo do documento: em (7,3) às 10 h do dia seguinte; pela caravana em (4,1), às 19 h");
    // Teste 2: uma Regional em (7,3) nunca chega a (1,4), a 7 mapas.
    const Noticia regional = noticiaDeTeste(mundo, ImportanciaDaNoticia::Regional, 7, 3, 0);
    conferir(tickDeChegada(regional, mundo.numeroDoMapaNa(1, 4), mundo, regras, 800) == -1, "o alcance: uma Regional em (7,3) nunca chega a (1,4)");
    // Teste 3: a precisão.
    conferir(numeroComPrecisao(1880, 2, regras) == "1.880" && numeroComPrecisao(1880, 4, regras) == "cerca de 2.000" && numeroComPrecisao(1880, 7, regras) == "milhares de",
             "a precisão: 1.880, cerca de 2.000 e milhares de, a 2, 4 e 7 mapas");
    conferir(numeroComPrecisao(1450, 4, regras) == "cerca de 1.000" && numeroComPrecisao(1500, 4, regras) == "cerca de 2.000" && numeroComPrecisao(7, 9, regras) == "alguns",
             "o arredondamento: 1.450 → cerca de 1.000; 1.500 → cerca de 2.000; 7 de longe → alguns");
    conferir(textoComPrecisao(regional, 4, regras) == "Epidemia na Cidade Religiosa: cerca de 2.000 doentes", "o texto da notícia com a precisão da distância");
}

void testarOJogadorFicaSabendo(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    vila.outrasCidades.clear();
    const MundoDoJogo& mundo = dados.mundo;
    const int64_t agora = vila.relogio.tickAtual();
    // Teste 4: uma Regional em (7,3) nasceu há 15 horas. O jogador soube em (2,3), a 5 mapas.
    const int64_t numero = criarNoticia(vila, noticiaDeTeste(mundo, ImportanciaDaNoticia::Regional, 7, 3, agora - 15 * 800));
    const std::vector<std::string> primeira = conferirNoticiasDoJogador(vila, {mundo.numeroDoMapaNa(2, 3)}, false);
    const NoticiaQueOJogadorSabe* sabida = oQueOJogadorSabe(vila.noticias, numero);
    conferir(primeira.size() == 1 && sabida != nullptr && sabida->distancia == 5 && primeira[0].find("cerca de 2.000") != std::string::npos,
             "a 5 mapas: uma notificação com \"cerca de 2.000 doentes\"");
    const std::vector<std::string> segunda = conferirNoticiasDoJogador(vila, {mundo.numeroDoMapaNa(6, 3)}, false);
    conferir(segunda.empty() && oQueOJogadorSabe(vila.noticias, numero)->distancia == 1 && vila.noticias.cronicaDoMundo.size() == 1
                 && vila.noticias.cronicaDoMundo[0].texto == "Epidemia na Cidade Religiosa: 1.880 doentes",
             "melhora sem aviso: a 1 mapa, a Crônica fica com \"1.880 doentes\" e não há notificação nova");
    // Ao entrar num mapa, as notícias que já estavam lá chegam juntas (NO1-3).
    criarNoticia(vila, noticiaDeTeste(mundo, ImportanciaDaNoticia::Regional, 1, 1, agora));
    criarNoticia(vila, noticiaDeTeste(mundo, ImportanciaDaNoticia::Mundial, 1, 1, agora));
    const std::vector<std::string> aoEntrar = conferirNoticiasDoJogador(vila, {mundo.numeroDoMapaNa(1, 1)}, true);
    conferir(aoEntrar.size() == 1 && aoEntrar[0].find("2 notícias novas") != std::string::npos, "ao entrar num mapa: \"2 notícias novas\", num aviso só");

    // Teste 6: com 500 ativas, a nova tira a Local mais velha (a Regional mais velha fica).
    SimulacaoDaVila cheia(dados, RegrasDoTempo{});
    cheia.outrasCidades.clear();
    const int64_t primeiraRegional = criarNoticia(cheia, noticiaDeTeste(mundo, ImportanciaDaNoticia::Regional, 1, 1, agora));
    const int64_t primeiraLocal = criarNoticia(cheia, noticiaDeTeste(mundo, ImportanciaDaNoticia::Local, 1, 1, agora));
    while (cheia.noticias.ativas.size() < 500) {
        criarNoticia(cheia, noticiaDeTeste(mundo, ImportanciaDaNoticia::Local, 1, 1, agora));
    }
    criarNoticia(cheia, noticiaDeTeste(mundo, ImportanciaDaNoticia::Local, 1, 1, agora));
    conferir(cheia.noticias.ativas.size() == 500 && cheia.noticias.ativas.front().numeroDaNoticia == primeiraRegional
                 && cheia.noticias.ativas[1].numeroDaNoticia == primeiraLocal + 1,
             "o limite: com 500 ativas, a nova tira a Local mais velha");
}

void testarAsNoticiasDePreco(const DadosDoJogo& dados)
{
    // Teste 5: uma região com 4 cidades; nesta, a Tábua a +71% e o Grão a +40%.
    const RegrasDasNoticias& regras = dados.regrasDasNoticias;
    const std::vector<int> precos{30, 14};
    const std::vector<int64_t> soma{70, 40};
    std::vector<int64_t> ultimoDia{-1, -1};
    int64_t diferenca = 0;
    const int primeiroDia = escolherOItemDaNoticiaDePreco(precos, soma, 4, ultimoDia, 10, regras, diferenca);
    const bool tabuaPrimeiro = primeiroDia == 0 && diferenca == 71;
    ultimoDia[0] = 10;
    const int segundoDia = escolherOItemDaNoticiaDePreco(precos, soma, 4, ultimoDia, 11, regras, diferenca);
    const int depoisDe8Dias = escolherOItemDaNoticiaDePreco(precos, soma, 4, ultimoDia, 18, regras, diferenca);
    conferir(tabuaPrimeiro && segundoDia == 1 && depoisDe8Dias == 0, "o preço: só a Tábua no primeiro dia; o Grão no seguinte; a Tábua de novo depois de 8 dias");

    // No mundo: 2 dias com as 21 cidades; no máximo 1 notícia de preço por cidade por dia, e o
    // save leva as notícias.
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    while (vila.relogio.diaDaPartida() < 2) {
        vila.avancarUmTick();
    }
    int deHoje = 0;
    for (const Noticia& noticia : vila.noticias.ativas) {
        deHoje += noticia.tipo == "comercio.preco" && noticia.diaDeOrigem == 2 ? 1 : 0;
    }
    std::printf("    notícias ativas no dia 2: %zu (%d de preço hoje)\n", vila.noticias.ativas.size(), deHoje);
    conferir(deHoje >= 1 && deHoje <= 22, "no mundo: de 1 a 22 notícias de preço por dia");
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(vila, "teste", "2026-10-24");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->noticias == vila.noticias, "salvar e carregar: as mesmas notícias");
}

}  // namespace

void testarNoticias()
{
    std::printf("Notícias: a chegada, a precisão, o jogador e os preços (fase B3)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    testarAChegadaEAPrecisao(dados);
    testarOJogadorFicaSabendo(dados);
    testarAsNoticiasDePreco(dados);
}
