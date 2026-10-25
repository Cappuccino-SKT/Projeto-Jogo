// Os testes da religião: a tabela da parte 8 da Ficha Lógica: Religião (fase C4).

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Jogador/PedidosDoJogador.h"
#include "Religiao/Religiao.h"
#include "Save/ArquivoDeSave.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

void testarAsContas(const DadosDoJogo& dados)
{
    const RegrasDaReligiao& regras = dados.regrasDaReligiao;
    // Teste 1: a tabela da parte 5.
    conferir(fatorDaFeEmDecimos(Devocao::Media, coberturaComOsNumeros(5, 100, regras), regras) == 70, "a Cidade Autoritária com 1 Templo nível 3: +7");
    conferir(coberturaComOsNumeros(7, 300, regras) == 46 && fatorDaFeEmDecimos(Devocao::Alta, 46, regras) == -50, "a Capital Religiosa com 1 Templo: 47% e −5");
    conferir(coberturaComOsNumeros(14, 300, regras) == 93 && fatorDaFeEmDecimos(Devocao::Alta, 93, regras) == 93, "com 2 Templos (RL1-1): 93% e +9,3");
    conferir(fatorDaFeEmDecimos(Devocao::Alta, 0, regras) == -50 && coberturaComOsNumeros(3, 40, regras) == 100 && fatorDaFeEmDecimos(Devocao::Alta, 100, regras) == 100,
             "o vilarejo Religioso: sem Templo −5; com Templo nível 2, +10");
    // Teste 2: 80 Arcanos (Torre Arcana, 100%) e 20 Religiosos (sem culto): +2,2.
    conferir(fatorReligiaoDaClasseEmDecimos(dados, {{"arcana", 80}, {"religiosa", 20}}, "torre_arcana", 100) == 22, "a minoria sem culto: a classe fica em +2,2");
    // Teste 3: o festival sem sobra e com sobra.
    conferir(comidaExtraDoFestival(40, 1, 0, regras) == 0 && comidaExtraDoFestival(40, 1, 60, regras) == 20, "o festival: sem sobra, nada; com 60 de sobra e 40 Civis, 20");
    // Teste 4: a assimilação de 40 Autoritários com a Fé oficial (2%).
    conferir(assimiladosEmEstacoes(40, 2, 1) == 0 && assimiladosEmEstacoes(40, 2, 2) == 1 && assimiladosEmEstacoes(40, 2, 5) == 3,
             "a assimilação: 1 NPC na 2ª estação e 3 em 5 estações");
    // Teste 5: as mortes da Epidemia com o Templo da Religiosa.
    conferir(mortesComOTemplo(4, regras) == 3 && mortesComOTemplo(4 * 50 / 100, regras) == 1, "o Templo da Religiosa: 4 mortes viram 3; com o Remédio, 1");
}

void testarOMundo(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    const SimulacaoDaVila* religiosa = nullptr;
    const SimulacaoDaVila* tecnocrata = nullptr;
    for (const auto& cidade : vila.outrasCidades) {
        religiosa = cidade->numeroDoMapaDaCidade == mapaDaCidadeSanta(dados) ? cidade.get() : religiosa;
        tecnocrata = cidade->numeroDoMapaDaCidade == dados.mundo.numeroDoMapaNa(2, 6) ? cidade.get() : tecnocrata;
    }
    int templos = 0;
    for (const ConstrucaoDaVila& construcao : religiosa->vila.construcoes) {
        templos += construcao.tipo == "construcao.templo" ? 1 : 0;
    }
    std::printf("    fator Religião: Capital Religiosa %d, Capital Tecnocrata %d (em décimos)\n", religiosa->efeitosDaReligiao.fatorEmDecimos,
                tecnocrata->efeitosDaReligiao.fatorEmDecimos);
    conferir(templos == 2 && religiosa->efeitosDaReligiao.fatorEmDecimos >= 60 && tecnocrata->efeitosDaReligiao.fatorEmDecimos == 0 && vila.efeitosDaReligiao.fatorEmDecimos == 0
                 && religiosa->efeitosDaReligiao.mortesDaEpidemiaEmPorcento == 75 && religiosa->efeitosDaReligiao.semAFestaDaColheitaDaFatia
                 && !vila.efeitosDaReligiao.semAFestaDaColheitaDaFatia,
             "RL1-1: a Capital Religiosa com 2 Templos e fator acima de +6; a Tecnocrata e a vila em 0; a vila com a Festa da Colheita da fatia");
    // Teste 6 da parte 4.9: a peregrinação dá +10 uma vez por ano.
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    grupo.mapaAtual = mapaDaCidadeSanta(dados);
    grupo.dinheiroEmDecimos = 2000;
    const int dono = donoDaCidadeDoMapa(dados.mundo, grupo.mapaAtual);
    vila.pedidos.acrescentar(PedidoPeregrinacao{50});
    vila.pedidos.acrescentar(PedidoPeregrinacao{50});
    vila.aplicarOsPedidosPendentes();
    conferir(vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)] == 10 && grupo.dinheiroEmDecimos == 1000 && vila.religiaoDoJogador.anoDaUltimaPeregrinacao == vila.relogio.anoAtual(),
             "a peregrinação: +10 de reputação na primeira; a segunda é aceita sem reputação");
    // O save leva a religião.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(vila, "teste", "2026-10-24");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->religiaoDoJogador == vila.religiaoDoJogador
                 && carregado.partida->outrasCidades.back()->efeitosDaReligiao.fatorEmDecimos == vila.outrasCidades.back()->efeitosDaReligiao.fatorEmDecimos,
             "salvar e carregar: a mesma religião");
}

}  // namespace

void testarReligiao()
{
    std::printf("Religião: a cobertura, o fator, o festival, a assimilação e a peregrinação (fase C4)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com o religiao.json, não têm erro");
    testarAsContas(dados);
    testarOMundo(dados);
}
