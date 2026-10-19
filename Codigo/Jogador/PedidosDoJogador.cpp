#include "Jogador/PedidosDoJogador.h"

#include "Jogador/DecisoesDoJogador.h"
#include "Jogador/ObrasDaVila.h"

std::vector<PedidoDoJogador> FilaDePedidosDoJogador::tomarTodos()
{
    std::vector<PedidoDoJogador> entregues;
    entregues.swap(pedidos);
    return entregues;
}

namespace {

bool construcaoDoJogadorExiste(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    return indiceDaConstrucao >= 0 && indiceDaConstrucao < static_cast<int>(contexto.vila.construcoes.size())
        && contexto.vila.construcoes[indiceDaConstrucao].donoEhOJogador && !contexto.vila.construcoes[indiceDaConstrucao].emObra;
}

// Devolve o motivo da recusa, ou vazio.
std::string aplicarUmPedido(ContextoDaProducao& contexto, const PedidoDoJogador& pedido, std::vector<std::string>& linhas)
{
    const RegrasDasDecisoesDoJogador& regras = contexto.dados.regras.economia.decisoesDoJogador;
    if (const auto* fila = std::get_if<PedidoTrocarAFilaDeOrdens>(&pedido)) {
        if (!construcaoDoJogadorExiste(contexto, fila->indiceDaConstrucao)) {
            return "a construção não é do jogador";
        }
        contexto.producaoDasConstrucoes[fila->indiceDaConstrucao].ordens = fila->ordens;
        return "";
    }
    if (const auto* preco = std::get_if<PedidoFixarOPreco>(&pedido)) {
        if (!precoEstaNaFaixaPermitida(preco->porcentagem, regras)) {
            return "o preço sai da faixa de " + std::to_string(regras.faixaDoPrecoEmPorcento) + "%, de " + std::to_string(regras.passoDoPrecoEmPorcento)
                + " em " + std::to_string(regras.passoDoPrecoEmPorcento);
        }
        if (preco->numeroDoItem < 0 || preco->numeroDoItem >= contexto.dados.catalogoDeItens.quantidadeDeItens()) {
            return "o item não existe";
        }
        contexto.decisoes.porcentagemDoPrecoPorItem[preco->numeroDoItem] = preco->porcentagem;
        return "";
    }
    if (const auto* salario = std::get_if<PedidoTrocarONivelDeSalario>(&pedido)) {
        if (!construcaoDoJogadorExiste(contexto, salario->indiceDaConstrucao)) {
            return "a construção não é do jogador";
        }
        ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[salario->indiceDaConstrucao];
        if (salario->nivel != NivelDeSalario::Baixo || producao.nivelDeSalario != NivelDeSalario::Baixo) {
            producao.diasSeguidosComSalarioBaixo = 0;
        }
        producao.nivelDeSalario = salario->nivel;
        return "";
    }
    if (const auto* contratar = std::get_if<PedidoContratar>(&pedido)) {
        return contratarUmNpc(contexto, contratar->indiceDaConstrucao, contratar->numeroDoNpc);
    }
    if (const auto* demitir = std::get_if<PedidoDemitir>(&pedido)) {
        return demitirUmNpc(contexto, demitir->numeroDoNpc);
    }
    if (const auto* nova = std::get_if<PedidoEncomendarConstrucaoNova>(&pedido)) {
        const std::string motivo = encomendarUmaConstrucaoNova(contexto, nova->numeroDoLote, nova->tipoDeConstrucao);
        if (motivo.empty()) {
            linhas.push_back("Obra encomendada: " + nova->tipoDeConstrucao + " no lote " + std::to_string(nova->numeroDoLote) + ".");
        }
        return motivo;
    }
    if (const auto* subir = std::get_if<PedidoSubirDeNivel>(&pedido)) {
        const std::string motivo = encomendarUmaSubidaDeNivel(contexto, subir->indiceDaConstrucao);
        if (motivo.empty()) {
            linhas.push_back("Obra encomendada: " + contexto.vila.construcoes[subir->indiceDaConstrucao].identificador + " para o nível "
                             + std::to_string(contexto.vila.construcoes[subir->indiceDaConstrucao].nivel + 1) + ".");
        }
        return motivo;
    }
    if (const auto* comprar = std::get_if<PedidoComprarDaLoja>(&pedido)) {
        return comprarDaLojaDaCidade(contexto, comprar->numeroDoItem, comprar->quantidade);
    }
    if (const auto* vender = std::get_if<PedidoVenderParaALoja>(&pedido)) {
        return venderParaALojaDaCidade(contexto, vender->numeroDoItem, vender->quantidade);
    }
    return "";
}

}  // namespace

std::vector<std::string> aplicarOsPedidosDoJogador(ContextoDaProducao& contexto, FilaDePedidosDoJogador& fila)
{
    std::vector<std::string> linhas;
    for (const PedidoDoJogador& pedido : fila.tomarTodos()) {
        const std::string motivo = aplicarUmPedido(contexto, pedido, linhas);
        if (!motivo.empty()) {
            linhas.push_back("Aviso: pedido recusado (" + motivo + ").");
        }
    }
    return linhas;
}
