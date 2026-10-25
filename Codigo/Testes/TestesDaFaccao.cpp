// Os testes das facções: a tabela da parte 8 da Ficha Lógica: Facção (fase C1).

#include <algorithm>
#include <cstdio>
#include <string>

#include "Comercio/Comercio.h"
#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Faccao/Faccao.h"
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

void testarOTerritorioEAsContas(const DadosDoJogo& dados)
{
    const MundoDoJogo& mundo = dados.mundo;
    // Teste 1: (5,2) é Arcano; (5,3) é sem dono; (3,3) em disputa.
    conferir(faccaoDoMapa(dados, mundo.numeroDoMapaNa(5, 2)) == faccaoPeloNome(dados, "arcana") && mundo.estadoDoTerritorio(mundo.numeroDoMapaNa(5, 3)) == EstadoDoTerritorio::SemDono
                 && mundo.estadoDoTerritorio(mundo.numeroDoMapaNa(3, 3)) == EstadoDoTerritorio::EmDisputa && faccaoDoMapa(dados, mundo.numeroDoMapaNa(3, 3)) == -1,
             "o território: (5,2) Arcano, (5,3) sem dono, (3,3) em disputa");
    // Teste 4: 80 Tábuas a 0,8 entrando na Autoritária (20%): 12,8 moedas.
    conferir(tarifaComOsNumeros(80 * 8, 20) == 128, "a tarifa: 80 Tábuas a 0,8 com 20% = 12,8 moedas");
    // Teste 5: o Banco da Facção com 1.100 e a verba de 2.200: cada cidade recebe a metade.
    const std::vector<int64_t> pagas = dividirAVerba(11000, {15000, 5000, 2000});
    conferir(pagas == std::vector<int64_t>{7500, 2500, 1000}, "a verba sem dinheiro: cada cidade recebe a metade");
}

void testarAAssembleia(const DadosDoJogo& dados)
{
    // Teste 6 (parte 10.4): a Arcana, com a vila do jogador de cultura Tecnocrata.
    const CatalogoDasFaccoes& catalogo = dados.faccoes;
    const int arcana = faccaoPeloNome(dados, "arcana");
    std::vector<int> leis = catalogo.faccoes[static_cast<std::size_t>(arcana)].leisIniciais;
    std::vector<VotanteDaAssembleia> votantes{{"arcana", 60, 300, 4, true, 8}, {"arcana", 60, 100, 3, false, 15}, {"arcana", 60, 40, 2, false, 14},
                                              {"tecnocrata", 60, 15, 1, false, 20}};
    LiderDaFaccao lider{12, 60, 40, 70};
    const auto nuncaVeta = []() { return 99; };
    const int magia = catalogo.posicaoDaLei("magia");
    const ResultadoDaAssembleia noConselho = fazerAssembleia(catalogo, leis, votantes, "arcana", lider, nuncaVeta);
    conferir(noConselho.pauta == std::vector<int>{magia} && noConselho.novoValor[0] == leis[static_cast<std::size_t>(magia)] + 1 && !noConselho.passou[0],
             "a assembleia da parte 10.4: a vila propõe Magia (privilégio → livre); no Conselho, não passa");
    leis[static_cast<std::size_t>(catalogo.posicaoDaLei("sistema_de_votacao"))] = 1;  // maioria simples
    const ResultadoDaAssembleia naMaioria = fazerAssembleia(catalogo, leis, votantes, "arcana", lider, nuncaVeta);
    // As 3 Arcanas também propõem voltar ao Conselho (mais cidades: vem primeiro na pauta).
    const std::size_t daMagia = static_cast<std::size_t>(std::find(naMaioria.pauta.begin(), naMaioria.pauta.end(), magia) - naMaioria.pauta.begin());
    conferir(naMaioria.pauta.size() == 2 && naMaioria.pauta[0] == catalogo.posicaoDaLei("sistema_de_votacao") && daMagia == 1 && !naMaioria.passou[daMagia]
                 && naMaioria.perderam[daMagia] == std::vector<int>{20},
             "com maioria simples: a Magia tem 3 não contra 1 sim, e a vila perdeu");
    // Com 3 cidades Tecnocratas, passa; o líder Arcano (moderado, 60 e 40) veta com 60% de chance.
    const std::vector<VotanteDaAssembleia> conquistadas{{"tecnocrata", 60, 100, 3, false, 1}, {"tecnocrata", 60, 100, 3, false, 2}, {"tecnocrata", 60, 100, 3, false, 3}};
    const ResultadoDaAssembleia vetada = fazerAssembleia(catalogo, leis, conquistadas, "arcana", lider, []() { return 59; });
    LiderDaFaccao outro{12, 60, 40, 70};
    const ResultadoDaAssembleia semVeto = fazerAssembleia(catalogo, leis, conquistadas, "arcana", outro, []() { return 60; });
    conferir(vetada.passou[0] && vetada.vetada[0] && lider.legitimidade == 65 && semVeto.passou[0] && !semVeto.vetada[0],
             "o veto: 60% de chance (59 veta, 60 não); o veto custa 5 de legitimidade");
    // No começo, as leis já são o desejo da cultura: nenhuma cidade propõe.
    const std::vector<VotanteDaAssembleia> soArcanas{{"arcana", 60, 300, 4, true, 8}};
    conferir(fazerAssembleia(catalogo, catalogo.faccoes[static_cast<std::size_t>(arcana)].leisIniciais, soArcanas, "arcana", lider, nuncaVeta).pauta.empty(),
             "no começo, as leis são o desejo da cultura: a pauta fica vazia");
}

void testarOMundo(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    const int guerreira = faccaoPeloNome(dados, "guerreira");
    const int escravista = faccaoPeloNome(dados, "escravista");
    SimulacaoDaVila& capitalGuerreira = *vila.outrasCidades.front();
    conferir(vila.faccoes.bancoDaFaccao.size() == 6 && vila.faccoes.bancoDaFaccao[static_cast<std::size_t>(guerreira)] > 0 && capitalGuerreira.impostoDaCidadeEmPorcento == 10
                 && capitalGuerreira.vila.verbaDaCoroaPorEstacaoEmDecimos == 0 && vila.impostoDaCidadeEmPorcento == -1,
             "6 facções com Banco; a capital Guerreira com imposto de 10% e a verba da facção; a vila sem facção, com o do regras.json");
    const SimulacaoDaVila* escravistaCapital = nullptr;
    for (const auto& cidade : vila.outrasCidades) {
        escravistaCapital = faccaoDaCidade(dados, cidade->numeroDoMapaDaCidade) == escravista ? cidade.get() : escravistaCapital;
    }
    conferir(escravistaCapital != nullptr && escravistaCapital->impostoDaCidadeEmPorcento == 30 && escravistaCapital->felicidadeDaFaccaoEmPontos == -10,
             "a Escravista: imposto Alto, 30% e Felicidade −10");
    // 1 dia: a cunhagem da Casa da Moeda da capital entra no Banco da Facção.
    const int64_t bancoAntes = vila.faccoes.bancoDaFaccao[static_cast<std::size_t>(guerreira)];
    while (vila.relogio.diaDaPartida() < 1) {
        vila.avancarUmTick();
    }
    conferir(vila.faccoes.bancoDaFaccao[static_cast<std::size_t>(guerreira)] >= bancoAntes + 1400, "a cunhagem: +140 moedas por dia no Banco da Guerreira");
    // O save leva as facções.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(vila, "teste", "2026-10-24");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->faccoes == vila.faccoes
                 && carregado.partida->outrasCidades.front()->impostoDaCidadeEmPorcento == 10,
             "salvar e carregar: as mesmas facções e o mesmo imposto nas cidades");

    // Teste 7: um item proibido na facção: a loja dela não vende nem compra.
    DadosDoJogo comProibido = dados;
    comProibido.faccoes.faccoes[static_cast<std::size_t>(guerreira)].itensProibidos.push_back("alimento");
    SimulacaoDaVila outra(comProibido, RegrasDoTempo{});
    const ContextoDaProducao loja = outra.outrasCidades.front()->contextoDaProducao();
    const int pao = *dados.catalogoDeItens.procurarNumeroDoItem("item.pao");
    conferir(calcularACompraNaLoja(loja, pao, 5, 100000, false).quantidade == 0 && calcularAVendaParaALoja(loja, pao, 5).quantidade == 0
                 && calcularACompraNaLoja(vila.outrasCidades.front()->contextoDaProducao(), pao, 5, 100000, false).quantidade > 0,
             "os itens proibidos: com a comida proibida, a loja Guerreira não vende nem compra Pão");
}

void testarOImpostoBaixo(const DadosDoJogo& dados)
{
    // Teste 2: a vila com imposto Baixo (10%) e o teto novo (90%) por 64 dias: o Banco fica
    // dentro da regra FE4-3 (nunca abaixo de 250; no máximo 50 abaixo do dia 1).
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    vila.outrasCidades.clear();
    vila.impostoDaCidadeEmPorcento = 10;
    const int64_t bancoNoDia1 = vila.dinheiro.bancoDaCidade;
    int64_t menor = bancoNoDia1;
    while (vila.relogio.diaDaPartida() < 64) {
        vila.avancarUmTick();
        menor = std::min(menor, vila.dinheiro.bancoDaCidade);
    }
    std::printf("    imposto de 10%%: Banco no dia 1 %lld, menor %lld, no fim %lld\n", static_cast<long long>(bancoNoDia1 / 10), static_cast<long long>(menor / 10),
                static_cast<long long>(vila.dinheiro.bancoDaCidade / 10));
    conferir(menor >= 2500 && vila.dinheiro.bancoDaCidade >= bancoNoDia1 - 500, "imposto Baixo por 64 dias: o Banco nunca abaixo de 250 e no máximo 50 abaixo do dia 1");
}

}  // namespace

void testarFaccao()
{
    std::printf("Facção: o território, a tarifa, a verba, a assembleia, o imposto e os itens proibidos (fase C1)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com as facções e as culturas, não têm erro");
    testarOTerritorioEAsContas(dados);
    testarAAssembleia(dados);
    testarOMundo(dados);
    testarOImpostoBaixo(dados);
}
