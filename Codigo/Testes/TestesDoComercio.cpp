// Os testes do comércio: a tabela da parte 8 da Ficha Lógica: Comércio (fase B2).

#include <cstdio>
#include <string>

#include "Comercio/Comercio.h"
#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Jogador/PedidosDoJogador.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

void testarALoja(const DadosDoJogo& dados)
{
    const RegrasDoComercio& regras = dados.regrasDoComercio;
    // Regra 4.7: 40 Tábuas, a loja tem 0 e o alvo é 20; preço local 3,0, base 1,5.
    const ResultadoDaLoja venda = calcularAVendaComOsNumeros(40, 20, 30, 15, 100000, regras);
    conferir(venda.quantidade == 40 && venda.valorEmDecimos == 680, "vender 40 Tábuas: 20 × 2,7 + 20 × 0,7 = 68 moedas, não 108");
    // O Banco com só 54 moedas acima da reserva: compra as 20 do alvo e para.
    const ResultadoDaLoja semDinheiro = calcularAVendaComOsNumeros(40, 20, 30, 15, 540, regras);
    conferir(semDinheiro.quantidade == 20 && semDinheiro.valorEmDecimos == 540, "a loja para de comprar quando o Banco chega à reserva de 250");
    conferir(calcularAVendaComOsNumeros(10, 0, 30, 15, 0, regras).quantidade == 0, "com o Banco na reserva, a loja não compra nada");
    conferir(forcaDaCaravana(dados) == 8, "a força da caravana: 2 condutores × 1 + 2 guardas × 3 = 8");
}

void testarOMundo(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    int caravanas = static_cast<int>(vila.caravanas.size());
    for (const auto& cidade : vila.outrasCidades) {
        caravanas += static_cast<int>(cidade->caravanas.size());
    }
    conferir(caravanas == 14, "14 caravanas no mundo: 2 em cada capital e 2 na Cidade dos mercados");

    // Regra 4.2: o que já vai a caminho desconta a falta (CO1-2).
    SimulacaoDaVila& capital = *vila.outrasCidades.front();
    const ContextoDaProducao contexto = capital.contextoDaProducao();
    const int pao = *dados.catalogoDeItens.procurarNumeroDoItem("item.pao");
    std::vector<int> aCaminho(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
    capital.inventario.tentarTirar(pao, DonoDoItem::Cidade, capital.inventario.quantidadeDe(pao, DonoDoItem::Cidade));
    const int faltaSemCaravana = faltaDoItem(contexto, aCaminho, pao);
    aCaminho[static_cast<std::size_t>(pao)] = 30;
    conferir(faltaSemCaravana > 30 && faltaDoItem(contexto, aCaminho, pao) == faltaSemCaravana - 30 && sobraDoItem(contexto, pao) == 0,
             "a falta de Pão desconta os 30 que uma caravana já leva; sem estoque, não há sobra");

    // Regra 4.3, passo 4: o custo = dias × 4 pessoas × (6 moedas + o Pão em casa).
    const PlanoDaViagemDaCaravana plano = calcularLucroEsperadoDaViagem(capital, capital.caravanas.front(), *vila.outrasCidades.back());
    conferir(plano.dias >= 1 && plano.custo == static_cast<int64_t>(plano.dias) * 4 * (60 + precoLocalDoItem(contexto, pao)) && plano.riscoPorMil > 0,
             "a conta da viagem: o custo pelos dias e o risco da rota");

    // O jogador comercia na cidade onde o grupo está, com o que o grupo leva.
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    grupo.mapaAtual = vila.outrasCidades[1]->numeroDoMapaDaCidade;
    grupo.dinheiroEmDecimos = 1000;
    const int64_t bancoAntes = vila.outrasCidades[1]->dinheiro.bancoDaCidade;
    vila.pedidos.acrescentar(PedidoComercioNaCidade{pao, 5, true});
    const std::vector<std::string> linhas = vila.aplicarOsPedidosPendentes();
    const int64_t pago = 1000 - grupo.dinheiroEmDecimos;
    conferir(grupo.quantidadePorItem[static_cast<std::size_t>(pao)] == 5 && pago > 0 && vila.outrasCidades[1]->dinheiro.bancoDaCidade == bancoAntes + pago
                 && vila.viagem.precosConhecidos[static_cast<std::size_t>(grupo.mapaAtual - 1)].diaEmQueFoiVisto == 0,
             "o grupo compra 5 Pães na outra cidade: o dinheiro vai ao Banco dela, e o jogador passa a conhecer os preços de lá");
    vila.pedidos.acrescentar(PedidoComercioNaCidade{pao, 5, false});
    vila.aplicarOsPedidosPendentes();
    conferir(grupo.quantidadePorItem[static_cast<std::size_t>(pao)] == 0 && grupo.dinheiroEmDecimos < 1000 && grupo.dinheiroEmDecimos > 1000 - pago,
             "vender de volta paga menos do que custou (90% ou 45% do base)");
    (void)linhas;
}

}  // namespace

void testarComercio()
{
    std::printf("Comércio: a loja, as caravanas e o jogador nas outras cidades (fase B2)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    testarALoja(dados);
    testarOMundo(dados);
}
