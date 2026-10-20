// Os testes do trabalho e da propriedade: a tabela da parte 8 da Ficha Lógica: Trabalho e Propriedade.

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Npc/ComponentesDoNpc.h"
#include "Vila/SimulacaoDaVila.h"
#include "Vila/Trabalho.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

int indiceDoTipo(const SimulacaoDaVila& simulacao, const std::string& tipo)
{
    for (std::size_t indice = 0; indice < simulacao.vila.construcoes.size(); ++indice) {
        if (simulacao.vila.construcoes[indice].tipo == tipo) {
            return static_cast<int>(indice);
        }
    }
    return -1;
}

std::vector<entt::entity> npcsComOTrabalho(SimulacaoDaVila& simulacao, int indiceDoTrabalho)
{
    std::vector<std::pair<int, entt::entity>> encontrados;
    for (const auto [npc, identidade, casaETrabalho] : simulacao.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc>().each()) {
        if (casaETrabalho.indiceDoTrabalho == indiceDoTrabalho) {
            encontrados.emplace_back(identidade.numeroDoNpc, npc);
        }
    }
    std::sort(encontrados.begin(), encontrados.end(), [](const auto& primeiro, const auto& segundo) { return primeiro.first < segundo.first; });
    std::vector<entt::entity> npcs;
    for (const auto& [numero, npc] : encontrados) {
        npcs.push_back(npc);
    }
    return npcs;
}

int64_t precoLocalPeloId(ContextoDaProducao& contexto, const char* identificador)
{
    return precoLocalDoItem(contexto, *contexto.dados.catalogoDeItens.procurarNumeroDoItem(identificador));
}

void testarAsVagasDaCidade(const DadosDoJogo& dados)
{
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    ContextoDaProducao contexto = simulacao.contextoDaProducao();
    const int fazenda = indiceDoTipo(simulacao, "construcao.fazenda");
    const std::vector<entt::entity> fazendeiros = npcsComOTrabalho(simulacao, fazenda);
    const std::vector<entt::entity> desempregados = npcsComOTrabalho(simulacao, -1);
    conferir(fazenda >= 0 && !fazendeiros.empty() && desempregados.size() >= 2, "a vila tem a Fazenda e pelo menos 2 desempregados");

    // Uma vaga vazia na Fazenda; desempregados com Braçal 40 e 60: contrata o de 60.
    simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(fazendeiros[0]).indiceDoTrabalho = -1;
    for (const entt::entity npc : npcsComOTrabalho(simulacao, -1)) {
        simulacao.registroDasEntidades.get<HabilidadesDoNpc>(npc).bracal = 30;
    }
    simulacao.registroDasEntidades.get<HabilidadesDoNpc>(desempregados[0]).bracal = 40;
    simulacao.registroDasEntidades.get<HabilidadesDoNpc>(desempregados[1]).bracal = 60;
    preencherVagasDaCidade(contexto);
    conferir(simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(desempregados[1]).indiceDoTrabalho == fazenda,
             "vaga na Fazenda, desempregados com Braçal 40 e 60: contrata o de 60");

    // O Banco sem 3 dias do salário (18 moedas para o Coletor): não contrata.
    simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(desempregados[1]).indiceDoTrabalho = -1;
    simulacao.dinheiro.bancoDaCidade = 100;  // 10 moedas
    preencherVagasDaCidade(contexto);
    conferir(simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(desempregados[1]).indiceDoTrabalho == -1, "Banco com 10 moedas: não contrata");

    // Quem saiu por atraso desta construção não volta nesta estação.
    simulacao.dinheiro.bancoDaCidade = 10'000;
    for (const entt::entity npc : npcsComOTrabalho(simulacao, -1)) {
        simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).saiuPorAtrasoDe = fazenda;
    }
    preencherVagasDaCidade(contexto);
    conferir(npcsComOTrabalho(simulacao, fazenda).size() == fazendeiros.size() - 1, "quem saiu da Fazenda por atraso não volta para ela nesta estação");

    // A vaga do jogador não é preenchida pela Cidade.
    const int carpintaria = indiceDoTipo(simulacao, "construcao.carpinteiro");
    const std::size_t carpinteirosAntes = npcsComOTrabalho(simulacao, carpintaria).size();
    for (const entt::entity npc : npcsComOTrabalho(simulacao, -1)) {
        simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).saiuPorAtrasoDe = -1;
    }
    preencherVagasDaCidade(contexto);
    conferir(npcsComOTrabalho(simulacao, carpintaria).size() == carpinteirosAntes, "a vaga da Carpintaria do jogador não é preenchida pela Cidade");
}

void testarARedistribuicao(const DadosDoJogo& dados)
{
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    ContextoDaProducao contexto = simulacao.contextoDaProducao();
    conferir(redistribuirEmpregosDaCidade(contexto).empty(), "redistribuição com todos em 50: nada muda");

    // Bruno (Braçal 40, Manufaturado 70) na Fazenda; Carla (Braçal 65, Manufaturado 45) no Cozinheiro: trocam.
    const int fazenda = indiceDoTipo(simulacao, "construcao.fazenda");
    const int cozinheiro = indiceDoTipo(simulacao, "construcao.cozinheiro");
    const entt::entity bruno = npcsComOTrabalho(simulacao, fazenda)[0];
    const entt::entity carla = npcsComOTrabalho(simulacao, cozinheiro)[0];
    simulacao.registroDasEntidades.get<HabilidadesDoNpc>(bruno) = HabilidadesDoNpc{40, 70};
    simulacao.registroDasEntidades.get<HabilidadesDoNpc>(carla) = HabilidadesDoNpc{65, 45};
    redistribuirEmpregosDaCidade(contexto);
    conferir(simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(bruno).indiceDoTrabalho == cozinheiro
                 && simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(carla).indiceDoTrabalho == fazenda,
             "Bruno e Carla do exemplo: trocam");
}

void testarComprarEVender(const DadosDoJogo& dados)
{
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    ContextoDaProducao contexto = simulacao.contextoDaProducao();
    const int posto = indiceDoTipo(simulacao, "construcao.posto_de_coleta");
    const int64_t valorEsperado = 10 * precoLocalPeloId(contexto, "item.tabua") + 10 * precoLocalPeloId(contexto, "item.bloco_de_pedra") + 300;
    const int64_t valor = calcularValorDaConstrucao(contexto, posto);
    conferir(valor == valorEsperado, "o valor do Posto: (10 Tábuas + 10 Blocos + 40 h × 0,75) × 1");

    simulacao.dinheiro.dinheiroDoJogador = valor - 1;
    conferir(!comprarConstrucaoDaCidade(contexto, posto).empty(), "comprar o Posto sem dinheiro: recusado");
    simulacao.dinheiro.dinheiroDoJogador = valor + 400;
    const int64_t bancoAntes = simulacao.dinheiro.bancoDaCidade;
    const std::size_t empregadosAntes = npcsComOTrabalho(simulacao, posto).size();
    conferir(comprarConstrucaoDaCidade(contexto, posto).empty() && simulacao.vila.construcoes[posto].donoEhOJogador
                 && simulacao.producaoDasConstrucoes[posto].modoAutomatico && simulacao.dinheiro.dinheiroDoJogador == 400
                 && simulacao.dinheiro.bancoDaCidade == bancoAntes + valor && npcsComOTrabalho(simulacao, posto).size() == empregadosAntes,
             "comprar o Posto: o dinheiro vai ao Banco, o modo automático liga, os empregados continuam");
    conferir(!construcaoSegueAFilaDoJogador(contexto, posto), "o Posto no modo automático segue a IA da Cidade");

    const int carpintaria = indiceDoTipo(simulacao, "construcao.carpinteiro");
    const int64_t precoDeVenda = calcularValorDaConstrucao(contexto, carpintaria) * 75 / 100;
    simulacao.dinheiro.bancoDaCidade = precoDeVenda - 1;
    conferir(!venderConstrucaoParaACidade(contexto, carpintaria).empty(), "vender a Carpintaria com o Banco sem o dinheiro: recusado");
    simulacao.dinheiro.bancoDaCidade = precoDeVenda + 1000;
    conferir(venderConstrucaoParaACidade(contexto, carpintaria).empty() && !simulacao.vila.construcoes[carpintaria].donoEhOJogador,
             "vender a Carpintaria com o Banco cheio: ela volta para a Cidade");
}

void testarOLeilao(const DadosDoJogo& dados)
{
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    ContextoDaProducao contexto = simulacao.contextoDaProducao();
    const int carpintaria = indiceDoTipo(simulacao, "construcao.carpinteiro");
    for (const entt::entity npc : npcsComOTrabalho(simulacao, carpintaria)) {
        simulacao.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho = -1;
    }
    simulacao.producaoDasConstrucoes[carpintaria].diasSemEmpregados = 31;
    contarOsDiasSemEmpregados(contexto, 16);
    conferir(simulacao.trabalho.leilao.indiceDaConstrucao == carpintaria && simulacao.trabalho.leilao.diasQueFaltam == 3,
             "Carpintaria sem empregados por 32 dias: abre o leilão de 3 dias");

    // Sem lance: volta para a Cidade.
    for (int dia = 0; dia < 3; ++dia) {
        avancarOLeilaoDoDia(contexto);
    }
    conferir(!simulacao.vila.construcoes[carpintaria].donoEhOJogador && !simulacao.trabalho.leilao.estaAberto(), "leilão sem lance: volta para a Cidade");

    // Com lance de 165 e dívida de salários de 64: 64 para os Civis, 101 de volta ao jogador.
    simulacao.vila.construcoes[carpintaria].donoEhOJogador = true;
    simulacao.producaoDasConstrucoes[carpintaria].salarioDevidoEmDecimos = 640;
    simulacao.trabalho.leilao = LeilaoDeConstrucao{carpintaria, 1, 0, false};
    simulacao.dinheiro.dinheiroDoJogador = 5'000;
    const int64_t valor = calcularValorDaConstrucao(contexto, carpintaria);
    const int64_t lance = valor / 2;
    conferir(darLanceNoLeilao(contexto, lance).empty() && simulacao.dinheiro.dinheiroDoJogador == 5'000 - lance, "o lance mínimo é aceito e fica preso");
    const int64_t civisAntes = simulacao.dinheiro.dinheiroDaClasseCivil;
    avancarOLeilaoDoDia(contexto);
    conferir(simulacao.dinheiro.dinheiroDaClasseCivil == civisAntes + 640 && simulacao.dinheiro.dinheiroDoJogador == 5'000 - 640
                 && simulacao.vila.construcoes[carpintaria].donoEhOJogador,
             "lance com dívida de 64: 64 para os Civis, o resto volta ao jogador, que continua dono");
}

}  // namespace

void testarTrabalho()
{
    std::printf("Trabalho e Propriedade (fase A2)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    testarAsVagasDaCidade(dados);
    testarARedistribuicao(dados);
    testarComprarEVender(dados);
    testarOLeilao(dados);
}
