// Os testes da reputação: a tabela da parte 8 da Ficha Lógica: Reputação (fase B4).

#include <cstdio>
#include <string>

#include "Comercio/Comercio.h"
#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Jogador/PedidosDoJogador.h"
#include "Reputacao/Reputacao.h"
#include "Save/ArquivoDeSave.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

void testarAsFaixasEOsPrecos(const DadosDoJogo& dados)
{
    const RegrasDaReputacao& regras = dados.regrasDaReputacao;
    // Teste 1: as faixas.
    conferir(faixaDaReputacao(regras, -60) == 0 && faixaDaReputacao(regras, -59) == 1 && faixaDaReputacao(regras, 19) == 2 && faixaDaReputacao(regras, 20) == 3
                 && faixaDaReputacao(regras, 60) == 4,
             "as faixas: −60 Inimigo, −59 Hostil, 19 Neutro, 20 Amigável, 60 Aliado");
    // Teste 2: o Pão a 1,7 com Hostil. A loja cobra 110% (1,9) × 115% = 2,2; paga 90% (1,5) × 85% = 1,3.
    const ResultadoDaLoja venda = calcularAVendaComOsNumeros(1, 1, 17, 10, 100000, dados.regrasDoComercio, 85);
    conferir(precoComAFaixa(19, 115) == 22 && venda.valorEmDecimos == 13, "o preço com Hostil: o jogador compra o Pão a 2,2 e vende a 1,3");
    // Parte 10.3: 30 Tábuas, 20 até o alvo, preço local 3,0, base 1,5, Amigável: 20 × 2,8 + 10 × 0,7 = 63.
    conferir(calcularAVendaComOsNumeros(30, 20, 30, 15, 100000, dados.regrasDoComercio, 105).valorEmDecimos == 630,
             "vender 30 Tábuas com Amigável: 63 moedas (61 no Neutro)");
}

void testarAsAcoes(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    const MundoDoJogo& mundo = dados.mundo;
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    SimulacaoDaVila& cidade = *vila.outrasCidades.front();  // a capital Guerreira
    const int dono = donoDaCidadeDoMapa(mundo, cidade.numeroDoMapaDaCidade);
    conferir(vila.reputacao.reputacaoPorFaccao.size() == 9 && dono >= 0 && donoDaCidadeDoMapa(mundo, mundo.mapaDaVila) == -1,
             "9 donos (6 facções e 3 Vilas Livres); a vila do jogador não tem reputação");

    // Teste 3: Inimigo não compra nem vende.
    grupo.mapaAtual = cidade.numeroDoMapaDaCidade;
    grupo.dinheiroEmDecimos = 20000;
    vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)] = -80;
    vila.pedidos.acrescentar(PedidoComercioNaCidade{*dados.catalogoDeItens.procurarNumeroDoItem("item.pao"), 1, true});
    const std::vector<std::string> recusa = vila.aplicarOsPedidosPendentes();
    conferir(recusa.size() == 1 && recusa[0].find("se recusa a negociar") != std::string::npos && grupo.dinheiroEmDecimos == 20000,
             "Inimigo: a loja se recusa a negociar");

    // Teste 4: base 0, valor 15; doar 1.000 aceita só 500 e chega a +20. O dinheiro vai ao Banco da cidade.
    vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)] = 15;
    const int64_t bancoAntes = cidade.dinheiro.bancoDaCidade;
    vila.pedidos.acrescentar(PedidoDoarNaCidade{1000});
    const std::vector<std::string> doacao = vila.aplicarOsPedidosPendentes();
    conferir(vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)] == 20 && grupo.dinheiroEmDecimos == 15000 && cidade.dinheiro.bancoDaCidade == bancoAntes + 5000
                 && doacao.size() == 2 && doacao[1].find("Amigável") != std::string::npos,
             "doar 1.000 com o valor em 15: aceita 500, chega a +20 (Amigável) e o Banco da cidade recebe");
    vila.pedidos.acrescentar(PedidoDoarNaCidade{50});
    const std::vector<std::string> pouco = vila.aplicarOsPedidosPendentes();
    conferir(pouco.size() == 1 && pouco[0].find("pelo menos 100") != std::string::npos, "doar 50 moedas: \"doe pelo menos 100 moedas\"");

    // Teste 5: a volta. +20 vira 18, 16, 14, 12; +1 vira 0.
    std::string valores;
    for (int estacao = 0; estacao < 4; ++estacao) {
        voltarReputacaoParaABase(vila);
        valores += std::to_string(vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)]) + " ";
    }
    vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)] = 1;
    voltarReputacaoParaABase(vila);
    conferir(valores == "18 16 14 12 " && vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)] == 0, "a volta: 18, 16, 14, 12; +1 vira 0");

    // Teste 6: vencer um bando num mapa sem dono não muda nada; no território de uma facção, +1.
    int semDono = 0;
    int territorio = 0;
    for (const MapaDoMundo& mapa : mundo.mapas) {
        const EstadoDoTerritorio estado = mundo.estadoDoTerritorio(mapa.numeroDoMapa);
        semDono = semDono == 0 && estado == EstadoDoTerritorio::SemDono ? mapa.numeroDoMapa : semDono;
        territorio = territorio == 0 && estado == EstadoDoTerritorio::Territorio ? mapa.numeroDoMapa : territorio;
    }
    const EstadoDaReputacao antes = vila.reputacao;
    grupo.mapaAtual = semDono;
    anotarOBandoVencido(vila);
    const bool nadaMudou = semDono != 0 && vila.reputacao == antes;
    grupo.mapaAtual = territorio;
    anotarOBandoVencido(vila);
    const int faccao = faccaoDonaDoTerritorio(mundo, territorio);
    conferir(nadaMudou && faccao >= 0 && vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(faccao)] == antes.reputacaoPorFaccao[static_cast<std::size_t>(faccao)] + 1
                 && vila.reputacao.ultimasMudancas[static_cast<std::size_t>(faccao)].back().motivo.find("venceu um bando") != std::string::npos,
             "o bando vencido: sem dono, nada; no território de uma facção, +1 com o \"Por quê?\"");

    // O save leva a reputação.
    grupo.mapaAtual = mundo.mapaDaVila;
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(vila, "teste", "2026-10-24");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->reputacao == vila.reputacao, "salvar e carregar: a mesma reputação");
}

}  // namespace

void testarReputacao()
{
    std::printf("Reputação: as faixas, os preços, a doação, a volta e o bando (fase B4)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    testarAsFaixasEOsPrecos(dados);
    testarAsAcoes(dados);
}
