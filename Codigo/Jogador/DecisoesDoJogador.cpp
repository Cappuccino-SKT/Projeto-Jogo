#include "Jogador/DecisoesDoJogador.h"

#include <algorithm>

#include "Construcao/ContasDasConstrucoes.h"
#include "Dados/Receitas.h"
#include "Economia/MercadoLocal.h"
#include "Npc/ComponentesDoNpc.h"

int calcularPrecoDoItemDoJogador(int precoLocalEmDecimos, int porcentagem)
{
    return static_cast<int>(arredondarMetadeParaCima(static_cast<long long>(precoLocalEmDecimos) * (100 + porcentagem), 100));
}

int calcularQuantidadeDoGastoExtraComOPreco(int quantidadeQueQueria, int porcentagem, const RegrasDasDecisoesDoJogador& regras)
{
    if (porcentagem <= 0) {
        return quantidadeQueQueria;
    }
    if (porcentagem >= regras.faixaDoPrecoEmPorcento) {
        return 0;
    }
    return quantidadeQueQueria * (regras.faixaDoPrecoEmPorcento - porcentagem) / regras.faixaDoPrecoEmPorcento;
}

bool precoEstaNaFaixaPermitida(int porcentagem, const RegrasDasDecisoesDoJogador& regras)
{
    return porcentagem >= -regras.faixaDoPrecoEmPorcento && porcentagem <= regras.faixaDoPrecoEmPorcento
        && porcentagem % regras.passoDoPrecoEmPorcento == 0;
}

int calcularPrecoQueALojaDaCidadePaga(int precoLocalEmDecimos, const RegrasDaEconomia& regras)
{
    return static_cast<int>(arredondarMetadeParaCima(static_cast<long long>(precoLocalEmDecimos) * regras.lojaDaCidadeCompraEmPorcento, 100));
}

int salarioDoNivelEmPorcento(NivelDeSalario nivel, const RegrasDasDecisoesDoJogador& regras)
{
    switch (nivel) {
    case NivelDeSalario::Baixo: return regras.salarioBaixoEmPorcento;
    case NivelDeSalario::Alto: return regras.salarioAltoEmPorcento;
    case NivelDeSalario::Normal: break;
    }
    return 100;
}

int velocidadeDoNivelEmPorcento(NivelDeSalario nivel, const RegrasDasDecisoesDoJogador& regras)
{
    switch (nivel) {
    case NivelDeSalario::Baixo: return regras.velocidadeComSalarioBaixoEmPorcento;
    case NivelDeSalario::Alto: return regras.velocidadeComSalarioAltoEmPorcento;
    case NivelDeSalario::Normal: break;
    }
    return 100;
}

namespace {

// O NPC pelo número na vila, ou entt::null.
entt::entity procurarONpc(ContextoDaProducao& contexto, int numeroDoNpc)
{
    for (const auto [npc, identidade] : contexto.registroDasEntidades.view<IdentidadeDoNpc>().each()) {
        if (identidade.numeroDoNpc == numeroDoNpc) {
            return npc;
        }
    }
    return entt::null;
}

int contarOsEmpregados(ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    int empregados = 0;
    for (const auto [npc, casaETrabalho] : contexto.registroDasEntidades.view<CasaETrabalhoDoNpc>().each()) {
        empregados += casaETrabalho.indiceDoTrabalho == indiceDaConstrucao ? 1 : 0;
    }
    return empregados;
}

}  // namespace

std::string contratarUmNpc(ContextoDaProducao& contexto, int indiceDaConstrucao, int numeroDoNpc)
{
    if (indiceDaConstrucao < 0 || indiceDaConstrucao >= static_cast<int>(contexto.vila.construcoes.size())) {
        return "a construção não existe";
    }
    const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indiceDaConstrucao];
    if (!construcao.donoEhOJogador || construcao.emObra) {
        return "só dá para contratar numa construção pronta do jogador";
    }
    const entt::entity npc = procurarONpc(contexto, numeroDoNpc);
    if (npc == entt::null || contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho >= 0) {
        return "o NPC não está desempregado";
    }
    if (contarOsEmpregados(contexto, indiceDaConstrucao) >= calcularVagas(construcao.nivel, contexto.dados.regras.construcoes)) {
        return "não há vaga";
    }
    contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho = indiceDaConstrucao;
    escolherOsGerentes(contexto);
    return "";
}

std::string demitirUmNpc(ContextoDaProducao& contexto, int numeroDoNpc)
{
    const entt::entity npc = procurarONpc(contexto, numeroDoNpc);
    if (npc == entt::null) {
        return "o NPC não existe";
    }
    CasaETrabalhoDoNpc& casaETrabalho = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc);
    if (casaETrabalho.indiceDoTrabalho < 0 || !contexto.vila.construcoes[casaETrabalho.indiceDoTrabalho].donoEhOJogador) {
        return "só dá para demitir um empregado do jogador";
    }
    casaETrabalho.indiceDoTrabalho = -1;
    escolherOsGerentes(contexto);
    return "";
}

std::string comprarDaLojaDaCidade(ContextoDaProducao& contexto, int numeroDoItem, int quantidade)
{
    if (quantidade <= 0 || numeroDoItem < 0 || numeroDoItem >= contexto.dados.catalogoDeItens.quantidadeDeItens()) {
        return "a quantidade ou o item não vale";
    }
    if (contexto.inventario.quantidadeDe(numeroDoItem, DonoDoItem::Cidade) < quantidade) {
        return "a Cidade não tem tanto";
    }
    const int64_t total = static_cast<int64_t>(quantidade) * calcularPrecoDaLojaDaCidade(precoLocalDoItem(contexto, numeroDoItem), contexto.dados.regras.economia);
    if (contexto.dinheiro.dinheiroDoJogador < total) {
        return "falta dinheiro";
    }
    contexto.inventario.tentarTirar(numeroDoItem, DonoDoItem::Cidade, quantidade);
    if (!contexto.inventario.tentarPor(numeroDoItem, DonoDoItem::Jogador, quantidade)) {
        contexto.inventario.tentarPor(numeroDoItem, DonoDoItem::Cidade, quantidade);
        return "não cabe no inventário";
    }
    contexto.dinheiro.dinheiroDoJogador -= total;
    contexto.dinheiro.bancoDaCidade += total;
    return "";
}

std::string venderParaALojaDaCidade(ContextoDaProducao& contexto, int numeroDoItem, int quantidade)
{
    if (quantidade <= 0 || numeroDoItem < 0 || numeroDoItem >= contexto.dados.catalogoDeItens.quantidadeDeItens()) {
        return "a quantidade ou o item não vale";
    }
    if (contexto.inventario.quantidadeDe(numeroDoItem, DonoDoItem::Jogador) < quantidade) {
        return "o jogador não tem tanto";
    }
    const int64_t total = static_cast<int64_t>(quantidade) * calcularPrecoQueALojaDaCidadePaga(precoLocalDoItem(contexto, numeroDoItem), contexto.dados.regras.economia);
    if (contexto.dinheiro.bancoDaCidade < total) {
        return "o Banco não tem o dinheiro";
    }
    contexto.inventario.tentarTirar(numeroDoItem, DonoDoItem::Jogador, quantidade);
    if (!contexto.inventario.tentarPor(numeroDoItem, DonoDoItem::Cidade, quantidade)) {
        contexto.inventario.tentarPor(numeroDoItem, DonoDoItem::Jogador, quantidade);
        return "não cabe no inventário da Cidade";
    }
    contexto.dinheiro.bancoDaCidade -= total;
    contexto.dinheiro.dinheiroDoJogador += total;
    return "";
}
