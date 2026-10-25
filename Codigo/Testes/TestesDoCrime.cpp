// Os testes do crime e da justiça: a tabela da parte 8 da Ficha Lógica: Crime e Justiça (fase C3).

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Crime/Crime.h"
#include "Dados/DadosDoJogo.h"
#include "Save/ArquivoDeSave.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

int faccaoPeloNome(const DadosDoJogo& dados, const std::string& cultura)
{
    for (std::size_t faccao = 0; faccao < dados.faccoes.faccoes.size(); ++faccao) {
        if (dados.faccoes.faccoes[faccao].cultura == cultura) {
            return static_cast<int>(faccao);
        }
    }
    return -1;
}

void testarAsContas(const DadosDoJogo& dados)
{
    const RegrasDoCrime& regras = dados.regrasDoCrime;
    // Teste 2 (parte 10.3): o vilarejo em fome chega a 56 e tira 7,2 de Felicidade.
    const int alvo = calcularAlvoDaCriminalidade(250, 40, 12, true, regras);
    conferir(alvo == 560 && darOPassoDaCriminalidade(110, alvo, regras) == 155 && felicidadeDaCriminalidadeEmDecimos(560, regras) == -72
                 && felicidadeDaCriminalidadeEmDecimos(190, regras) == 0,
             "o vilarejo em fome: alvo 56, o primeiro passo 15,5 e −7,2 de Felicidade; abaixo de 20, nada");
    // Teste 3: a região com média 30: +60 por mil.
    conferir(ameacaBandidaComOsNumeros(300, regras) == 60 && ameacaBandidaComOsNumeros(110, regras) == 3 && ameacaBandidaComOsNumeros(90, regras) == 0,
             "a ameaça bandida: média 30 dá +60 por mil; 11 dá +3");
    // Teste 5: o suborno.
    conferir(chanceDoSubornoEmPorcento(500, 100, regras) == 40 && chanceDoSubornoEmPorcento(2000, 100, regras) == 90, "o suborno: metade da multa dá 40%; 2 multas, 90%");
    // Teste 8: a queda.
    conferir(recompensaDepoisDaQueda(600, regras) == 540 && recompensaDepoisDaQueda(540, regras) == 486 && recompensaDepoisDaQueda(486, regras) == 437
                 && recompensaDepoisDaQueda(10, regras) == 0,
             "a queda: 600, 540, 486, 437; abaixo de 10, zera");
    conferir(chanceDaRevistaEmPorcento(regras, "neutro", 0) == 10 && chanceDaRevistaEmPorcento(regras, "aliado", 0) == 0 && chanceDaRevistaEmPorcento(regras, "aliado", 50) == 100,
             "a revista pela faixa: Neutro 10%, Aliado 0%; com Recompensa, certa");
}

void testarARevista(const DadosDoJogo& dados)
{
    // Teste 4: no Neutro, 1.000 entradas numa cidade Arcana: cerca de 100 revistas (nada achado).
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    const int mapaArcano = dados.mundo.numeroDoMapaNa(8, 1);
    int revistas = 0;
    for (int entrada = 0; entrada < 1000; ++entrada) {
        std::vector<std::string> linhas;
        revistarJogador(vila, mapaArcano, mapaArcano, linhas);
        revistas += linhas.empty() ? 0 : 1;
    }
    std::printf("    revistas no Neutro: %d em 1.000\n", revistas);
    conferir(revistas >= 70 && revistas <= 130, "a revista no Neutro: cerca de 100 em 1.000 entradas");

    // Testes 6 e 7: a Guerreira proíbe a comida (só para o teste); o grupo leva Pão.
    DadosDoJogo comProibido = dados;
    const int guerreira = faccaoPeloNome(dados, "guerreira");
    comProibido.faccoes.faccoes[static_cast<std::size_t>(guerreira)].itensProibidos.push_back("alimento");
    SimulacaoDaVila outra(comProibido, RegrasDoTempo{});
    EntidadeEmTransito& grupo = outra.viagem.grupoDoJogador;
    const int capital = comProibido.mundo.numeroDoMapaNa(1, 1);
    grupo.mapaAtual = capital;
    grupo.quantidadePorItem[static_cast<std::size_t>(*comProibido.catalogoDeItens.procurarNumeroDoItem("item.pao"))] = 20;
    grupo.dinheiroEmDecimos = 10000;
    outra.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(donoDaCidadeDoMapa(comProibido.mundo, capital))] = -80;  // Inimigo: revista certa
    std::string recompensas;
    for (int fuga = 0; fuga < 3; ++fuga) {
        std::vector<std::string> linhas;
        const bool achou = revistarJogador(outra, capital, capital - 1, linhas);
        resolverARevista(outra, achou ? EscolhaNaRevista::Fugir : EscolhaNaRevista::Pagar, 0, linhas);
        recompensas += std::to_string(outra.crimeDoJogador.recompensaPorFaccao[static_cast<std::size_t>(guerreira)]) + " ";
    }
    conferir(recompensas == "200 400 600 " && forcaDosCacadores(600) == 18, "a Recompensa: 200, 400 e 600 (Caçadores, força 18)");
    std::vector<std::string> linhas;
    prenderJogador(outra, guerreira, linhas);
    const EstadoDoCrimeDoJogador& crime = outra.crimeDoJogador;
    conferir(crime.presoAteODia - outra.relogio.diaDaPartida() == 12 && crime.fiancaNaFicha[static_cast<std::size_t>(guerreira)] == 600 && crime.estaPreso(outra.relogio.diaDaPartida()),
             "a prisão: 3 crimes de gravidade 2 dão 12 dias e fiança de 600");
    // O save leva o crime do jogador.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(outra, "teste", "2026-10-24");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(comProibido, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->crimeDoJogador == outra.crimeDoJogador, "salvar e carregar: o mesmo crime do jogador");
    grupo.dinheiroEmDecimos = 7000;
    const std::string motivo = pagarAFianca(outra, linhas);
    conferir(motivo.empty() && !outra.crimeDoJogador.estaPreso(outra.relogio.diaDaPartida()) && outra.crimeDoJogador.recompensaPorFaccao[static_cast<std::size_t>(guerreira)] == 0
                 && grupo.dinheiroEmDecimos == 1000 && grupo.quantidadePorItem[static_cast<std::size_t>(*comProibido.catalogoDeItens.procurarNumeroDoItem("item.pao"))] == 0,
             "a fiança: 600 moedas; o grupo sai com a ficha limpa e sem o Pão proibido");
}

void testarAVila(const DadosDoJogo& dados)
{
    // Teste 1: a vila do teste fica perto de 11 (sem furto e sem efeito na Felicidade).
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    vila.outrasCidades.clear();
    while (vila.relogio.diaDaPartida() < 16) {
        vila.avancarUmTick();
    }
    std::printf("    Criminalidade da vila no dia 16: %d,%d\n", vila.felicidade.criminalidadeEmDecimos / 10, vila.felicidade.criminalidadeEmDecimos % 10);
    conferir(vila.felicidade.criminalidadeEmDecimos >= 90 && vila.felicidade.criminalidadeEmDecimos < 200, "a vila do teste: a Criminalidade perto de 11, abaixo de 20");
}

}  // namespace

void testarCrime()
{
    std::printf("Crime e Justiça: a Criminalidade, a revista, a Recompensa e a prisão (fase C3)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com o crimes.json, não têm erro");
    testarAsContas(dados);
    testarARevista(dados);
    testarAVila(dados);
}
