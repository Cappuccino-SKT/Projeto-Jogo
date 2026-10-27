// Os testes da tecnologia: a tabela da parte 8 da Ficha Lógica: Tecnologia (fase B5).

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/ProblemasDosDados.h"
#include "Npc/ComponentesDoNpc.h"
#include "Save/ArquivoDeSave.h"
#include "Tecnologia/Tecnologia.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

int posicaoDaFaccao(const DadosDoJogo& dados, const std::string& faccao)
{
    const std::vector<std::string> faccoes = faccoesQuePesquisam(dados);
    return static_cast<int>(std::find(faccoes.begin(), faccoes.end(), faccao) - faccoes.begin());
}

void testarAsContas(const DadosDoJogo& dados)
{
    const RegrasDaTecnologia& regras = dados.tecnologia.regras;
    // Teste 1: 6 Especialistas de habilidade 40 num Laboratório nível 1, 8 horas de 784 ticks: 5 pontos.
    const int64_t porDia = calcularProgressoDoTrabalho(784, 40, 1, 1) * 6 * 8;
    conferir(porDia / (static_cast<int64_t>(regras.horasPorPonto) * 800 * 100) == 5, "o Laboratório da parte 5: 5 pontos por dia");
    // Testes 2 e 3: a difusão e a vila do jogador.
    conferir(custoComOsNumeros(100, 1, false, regras) == 90 && custoComOsNumeros(100, 5, false, regras) == 50 && custoComOsNumeros(100, 6, false, regras) == 50,
             "a difusão: 90 com 1 facção, 50 com 5, nunca menos de 50");
    conferir(custoComOsNumeros(100, 0, true, regras) == 150, "a vila do jogador paga 150%");
    // Teste 4: o marco adianta.
    conferir(dataDoMarcoComOsNumeros(280, 129, 3, 6) == 204 && dataDoMarcoComOsNumeros(280, 129, 2, 6) == 229 && dataDoMarcoComOsNumeros(280, 129, 6, 6) == 129,
             "o marco: 204 com 3 facções completas, 229 com 2, 129 com as 6");
}

void testarOMundo(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    EstadoDaTecnologia& estado = vila.tecnologia;
    const int tecnocrata = posicaoDaFaccao(dados, "tecnocrata");
    const int escravista = posicaoDaFaccao(dados, "escravista");
    // 4.3: a Tecnocrata começa pela produção; a Escravista, pela economia.
    conferir(estado.temATecnologia.size() == 7 && estado.projetoAtual[static_cast<std::size_t>(tecnocrata)] == 1
                 && estado.projetoAtual[static_cast<std::size_t>(escravista)] == 0 && estado.projetoAtual.back() == -1,
             "os projetos iniciais: Ferramentas melhores na Tecnocrata, Contabilidade na Escravista; a vila sem Laboratório, nenhum");
    // Teste 5: o dia sorteado fica dentro da janela.
    const DefinicaoDeMarco& marco = dados.tecnologia.marcos.front();
    conferir(estado.diaSorteadoPorMarco[0] >= primeiroDiaDoMarco(marco) && estado.diaSorteadoPorMarco[0] <= ultimoDiaDoMarco(marco)
                 && dataAtualDoMarco(estado, dados, 0) >= primeiroDiaDoMarco(marco),
             "o marco fica dentro da janela (anos 3 a 5)");
    // TE1-1: o Laboratório nas 6 capitais e na (3,6), com empregados.
    int laboratorios = 0;
    int pesquisadores = 0;
    for (const auto& cidade : vila.outrasCidades) {
        for (std::size_t indice = 0; indice < cidade->vila.construcoes.size(); ++indice) {
            if (cidade->vila.construcoes[indice].tipo != "construcao.laboratorio") {
                continue;
            }
            ++laboratorios;
            for (const entt::entity npc : cidade->npcsDaVila) {
                pesquisadores += cidade->registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho == static_cast<int>(indice) ? 1 : 0;
            }
        }
    }
    conferir(laboratorios == 7 && pesquisadores == 7 * 5, "7 Laboratórios (as 6 capitais e a (3,6)), cada um com 5 pesquisadores");
    // 2 dias: os Laboratórios dão pontos (a Tecnocrata tem 2).
    while (vila.relogio.diaDaPartida() < 2) {
        vila.avancarUmTick();
    }
    std::printf("    pontos no dia 2: Tecnocrata %lld, Escravista %lld\n", static_cast<long long>(estado.pontosAcumulados[static_cast<std::size_t>(tecnocrata)]),
                static_cast<long long>(estado.pontosAcumulados[static_cast<std::size_t>(escravista)]));
    conferir(estado.pontosAcumulados[static_cast<std::size_t>(escravista)] > 0
                 && estado.pontosAcumulados[static_cast<std::size_t>(tecnocrata)] > estado.pontosAcumulados[static_cast<std::size_t>(escravista)],
             "os pontos chegam: a Tecnocrata, com 2 Laboratórios, junta mais");
    // Uma tecnologia completa: a sobra fica, a notícia nasce e o próximo projeto é escolhido.
    estado.pontosAcumulados[static_cast<std::size_t>(tecnocrata)] = 130;
    const std::size_t noticiasAntes = vila.noticias.ativas.size();
    somarPontosEConcluirTecnologias(vila);
    conferir(estado.temATecnologia[static_cast<std::size_t>(tecnocrata)][1] && estado.pontosAcumulados[static_cast<std::size_t>(tecnocrata)] == 30
                 && estado.projetoAtual[static_cast<std::size_t>(tecnocrata)] == 2 && vila.noticias.ativas.size() == noticiasAntes + 1
                 && vila.noticias.ativas.back().titulo == "A Comunidade de Ashmoor descobriu Ferramentas melhores",
             "130 pontos completam Ferramentas melhores: sobram 30, nasce a notícia, o próximo é Andaimes");
    // Com a Tecnocrata tendo Ferramentas, a Escravista paga 90 por ela.
    conferir(custoAtualDaTecnologia(estado, dados, escravista, 1) == 90, "a difusão no mundo: a Escravista paga 90");
    // O save leva a tecnologia.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(vila, "teste", "2026-10-24");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->tecnologia == vila.tecnologia, "salvar e carregar: a mesma tecnologia");
}

void testarAsTravas(const DadosDoJogo& dados)
{
    // Teste 6: uma receita no "libera" de uma tecnologia só vale para quem tem a tecnologia.
    DadosDoJogo comTrava = dados;
    comTrava.tecnologia.tecnologias[0].libera.push_back({"receita", "receita.tabua"});
    SimulacaoDaVila vila(comTrava, RegrasDoTempo{});
    const int escravista = posicaoDaFaccao(comTrava, "escravista");
    const bool semATecnologia = !estaLiberado(vila.tecnologia, comTrava, escravista, "receita", "receita.tabua");
    const bool livre = estaLiberado(vila.tecnologia, comTrava, escravista, "receita", "receita.grao");
    vila.tecnologia.temATecnologia[static_cast<std::size_t>(escravista)][0] = true;
    conferir(semATecnologia && livre && estaLiberado(vila.tecnologia, comTrava, escravista, "receita", "receita.tabua"),
             "a trava: a Tábua só com Contabilidade; o Grão, fora de todo libera, é livre");
    // As cidades recebem a trava: na capital Guerreira (sem a tecnologia), a Tábua fica travada.
    const auto tabua = std::find_if(comTrava.catalogoDeReceitas.todasAsReceitas().begin(), comTrava.catalogoDeReceitas.todasAsReceitas().end(),
                                    [](const DefinicaoDeReceita& receita) { return receita.identificadorDeTexto == "receita.tabua"; });
    const std::size_t posicao = static_cast<std::size_t>(tabua - comTrava.catalogoDeReceitas.todasAsReceitas().begin());
    conferir(!vila.outrasCidades.front()->receitasLiberadas[posicao] && vila.receitasLiberadas[posicao] == false,
             "as cidades sem a tecnologia (e a vila do jogador) não podem fazer Tábua");

    // Teste 7: uma tecnologia que libera uma construção que não existe dá erro ao carregar.
    ListaDeProblemasDosDados problemas;
    lerArquivosDaTecnologia(R"([{"id": "tec.x", "ramo": "economia", "era": 1, "libera": [{"tipo": "construcao", "id": "construcao.nao_existe"}]}])", "[]", "{}", dados,
                            problemas);
    conferir(problemas.algumErroContem("construcao.nao_existe"), "a validação: liberar uma construção que não existe é erro");
}

}  // namespace

void testarTecnologia()
{
    std::printf("Tecnologia: os pontos, a difusão, os marcos e as travas (fase B5)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com as tecnologias e os marcos, não têm erro");
    testarAsContas(dados);
    testarOMundo(dados);
    testarAsTravas(dados);
}
