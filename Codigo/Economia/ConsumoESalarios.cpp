#include "Economia/ConsumoESalarios.h"

#include <algorithm>
#include <optional>

#include "Npc/ComponentesDoNpc.h"

namespace {

// Uma pilha do inventário: o item e o dono.
struct PilhaDoInventario {
    int numeroDoItem = -1;
    DonoDoItem dono = DonoDoItem::Cidade;
};

// O item mais barato do tipo pedido (comida ou Material Básico) que existe
// no estoque; no empate de preço, o da Cidade antes do do jogador, e depois o
// de menor número. Vazio se não há nenhum.
std::optional<PilhaDoInventario> procurarOMaisBarato(const ContextoDaProducao& contexto, bool procurarComida)
{
    std::optional<PilhaDoInventario> maisBarato;
    int menorPreco = 0;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        if (procurarComida ? !item.ehComida() : !item.ehMaterialBasico()) {
            continue;
        }
        for (const DonoDoItem dono : {DonoDoItem::Cidade, DonoDoItem::Jogador}) {
            if (contexto.inventario.quantidadeDe(item.numeroInterno, dono) <= 0) {
                continue;
            }
            if (!maisBarato.has_value() || item.precoBaseEmDecimos < menorPreco) {
                maisBarato = PilhaDoInventario{item.numeroInterno, dono};
                menorPreco = item.precoBaseEmDecimos;
            }
        }
    }
    return maisBarato;
}

// Passo 3 da regra 4.1: a classe paga ao dono do item. Sem dinheiro, paga o
// que tem, e o resto vira consumo não pago.
void pagarOItemConsumido(ContextoDaProducao& contexto, const PilhaDoInventario& pilha)
{
    const int precoBase = contexto.dados.catalogoDeItens.itemPeloNumero(pilha.numeroDoItem).precoBaseEmDecimos;
    const int64_t preco = pilha.dono == DonoDoItem::Cidade ? calcularPrecoDaLojaDaCidade(precoBase, contexto.dados.regras.economia) : precoBase;
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    const int64_t pago = std::min(preco, std::max<int64_t>(dinheiro.dinheiroDaClasseCivil, 0));
    dinheiro.dinheiroDaClasseCivil -= pago;
    (pilha.dono == DonoDoItem::Cidade ? dinheiro.bancoDaCidade : dinheiro.dinheiroDoJogador) += pago;
    dinheiro.consumoNaoPagoHoje += preco - pago;
}

// Tira 1 item do tipo pedido e paga. Devolve false se não havia nenhum.
bool consumirUmItem(ContextoDaProducao& contexto, bool procurarComida)
{
    const std::optional<PilhaDoInventario> pilha = procurarOMaisBarato(contexto, procurarComida);
    if (!pilha.has_value()) {
        return false;
    }
    contexto.inventario.tentarTirar(pilha->numeroDoItem, pilha->dono, 1);
    pagarOItemConsumido(contexto, *pilha);
    return true;
}

int salarioPorHoraDoTrabalho(TrabalhoDaReceita trabalho, const RegrasDoPreco& regrasDoPreco)
{
    switch (trabalho) {
    case TrabalhoDaReceita::Bracal: return regrasDoPreco.salarioPorHoraBracalEmCentesimos;
    case TrabalhoDaReceita::Manufaturado: return regrasDoPreco.salarioPorHoraManufaturadoEmCentesimos;
    case TrabalhoDaReceita::Especialista: return regrasDoPreco.salarioPorHoraEspecialistaEmCentesimos;
    }
    return regrasDoPreco.salarioPorHoraManufaturadoEmCentesimos;
}

std::string escreverDinheiro(int64_t valorEmDecimos)
{
    return escreverDinheiroEmMoedas(static_cast<int>(valorEmDecimos));
}

}  // namespace

int64_t calcularSalarioEmDecimos(int ticksTrabalhados, int centesimosPorHora, int bonusDeNivelEmPorcento, int gerenteEmPorcento,
                                 int ticksPorHoraDeJogo)
{
    const long long numerador = static_cast<long long>(ticksTrabalhados) * centesimosPorHora * bonusDeNivelEmPorcento * gerenteEmPorcento;
    const long long denominador = static_cast<long long>(ticksPorHoraDeJogo) * 10 * 100 * 100;
    return arredondarMetadeParaCima(numerador, denominador);
}

void consumirACestaDoDia(ContextoDaProducao& contexto)
{
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;

    // Passo 1: a comida de cada Civil.
    int comidaQueFaltou = 0;
    for (int civil = 0; civil < dinheiro.quantidadeDeCivis; ++civil) {
        for (int comida = 0; comida < economia.comidaPorDiaDoCivil; ++comida) {
            comidaQueFaltou += consumirUmItem(contexto, true) ? 0 : 1;
        }
    }

    // Passo 2: os Materiais Básicos, pelo acumulador (15 Civis gastam 1,875 por dia).
    bool faltouMaterial = false;
    dinheiro.materiaisDevidosEmOitavos += dinheiro.quantidadeDeCivis;
    while (dinheiro.materiaisDevidosEmOitavos >= economia.diasEntreMateriaisDoCivil) {
        dinheiro.materiaisDevidosEmOitavos -= economia.diasEntreMateriaisDoCivil;
        faltouMaterial = !consumirUmItem(contexto, false) || faltouMaterial;
    }

    // Passo 4: os dias em déficit.
    dinheiro.comidaQueFaltouHoje = comidaQueFaltou;
    dinheiro.diasEmDeficitDeComida = comidaQueFaltou > 0 ? dinheiro.diasEmDeficitDeComida + 1 : 0;
    dinheiro.diasEmDeficitDeMateriais = faltouMaterial ? dinheiro.diasEmDeficitDeMateriais + 1 : 0;
}

void pagarOsSalariosDoDia(ContextoDaProducao& contexto)
{
    entt::registry& registro = contexto.registroDasEntidades;
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;
    const VilaDoJogo& vila = contexto.dados.vilaInicial;

    // Os empregados de cada construção, em ordem pelo número (quem vem antes recebe antes).
    std::vector<std::vector<std::pair<int, entt::entity>>> empregadosDeCadaConstrucao(vila.construcoes.size());
    for (const auto [npc, identidade, casaETrabalho] : registro.view<IdentidadeDoNpc, CasaETrabalhoDoNpc>().each()) {
        if (casaETrabalho.indiceDoTrabalho >= 0) {
            empregadosDeCadaConstrucao[casaETrabalho.indiceDoTrabalho].emplace_back(identidade.numeroDoNpc, npc);
        }
    }

    int64_t rendaDoDia = 0;
    for (std::size_t indice = 0; indice < vila.construcoes.size(); ++indice) {
        std::vector<std::pair<int, entt::entity>>& empregados = empregadosDeCadaConstrucao[indice];
        std::sort(empregados.begin(), empregados.end(), [](const auto& primeiro, const auto& segundo) { return primeiro.first < segundo.first; });
        const ConstrucaoDaVila& construcao = vila.construcoes[indice];
        ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indice];
        // Passo 2: quem paga é o dono da construção.
        int64_t& dinheiroDeQuemPaga = construcao.donoEhOJogador ? dinheiro.dinheiroDoJogador : dinheiro.bancoDaCidade;
        const int centesimosPorHora = salarioPorHoraDoTrabalho(trabalhoDaConstrucao(contexto, static_cast<int>(indice)), contexto.dados.regras.regrasDoPreco);
        const int bonusDeNivel = 100 + economia.bonusDoSalarioPorNivelEmPorcento * (construcao.nivel - 1);
        bool alguemFicouSemReceber = false;
        for (const auto& [numeroDoNpc, npc] : empregados) {
            const int ticksDoDia = registro.get<TrabalhoDoDia>(npc).ticksTrabalhadosHoje;
            if (ticksDoDia <= 0) {
                continue;
            }
            const int gerente = numeroDoNpc == producao.numeroDoGerente ? economia.salarioDoGerenteEmPorcento : 100;
            const int64_t salario = calcularSalarioEmDecimos(ticksDoDia, centesimosPorHora, bonusDeNivel, gerente, contexto.ticksPorHoraDeJogo);
            // Passo 3: sem dinheiro para o salário inteiro, não recebe nada.
            if (dinheiroDeQuemPaga < salario) {
                alguemFicouSemReceber = true;
                continue;
            }
            dinheiroDeQuemPaga -= salario;
            dinheiro.dinheiroDaClasseCivil += salario;  // passo 4
            rendaDoDia += salario;
        }
        producao.diasDeSalarioAtrasado = alguemFicouSemReceber ? producao.diasDeSalarioAtrasado + 1 : 0;
    }
    dinheiro.rendaDoDiaDaClasseCivil = rendaDoDia;

    // Passo 5: as horas do dia voltam a zero.
    for (const auto [npc, trabalhoDoDia] : registro.view<TrabalhoDoDia>().each()) {
        trabalhoDoDia.ticksTrabalhadosHoje = 0;
    }
}

void rodarAOrdemDoDia(ContextoDaProducao& contexto)
{
    for (const PassoDaOrdemDoDia passo : contexto.dados.regras.economia.ordemDoDia) {
        switch (passo) {
        case PassoDaOrdemDoDia::Consumo:
            consumirACestaDoDia(contexto);
            break;
        case PassoDaOrdemDoDia::Salarios:
            // O gerente de cada construção é escolhido antes dos salários (ficha da Construção).
            escolherOsGerentes(contexto);
            pagarOsSalariosDoDia(contexto);
            break;
        case PassoDaOrdemDoDia::EscolhaDasReceitas:
            escolherAsReceitasDaCidade(contexto);
            break;
        }
    }
}

std::string montarResumoDoDia(const ContextoDaProducao& contexto, int64_t diaQueAcabou)
{
    std::string resumo = "Resumo do dia " + std::to_string(diaQueAcabou + 1) + ":";
    bool primeiroItem = true;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        resumo += std::string(primeiroItem ? " " : ", ") + contexto.dados.tabelaDeTextos.textoDe(item.identificadorDeTexto) + " "
            + std::to_string(contexto.inventario.quantidadeTotalDe(item.numeroInterno));
        primeiroItem = false;
    }
    const DinheiroDaVila& dinheiro = contexto.dinheiro;
    const int64_t mediaDosCivis = dinheiro.quantidadeDeCivis > 0 ? dinheiro.dinheiroDaClasseCivil / dinheiro.quantidadeDeCivis : 0;
    resumo += " | Banco " + escreverDinheiro(dinheiro.bancoDaCidade) + ", jogador " + escreverDinheiro(dinheiro.dinheiroDoJogador) + ", Civis "
        + escreverDinheiro(mediaDosCivis) + " em média, salários " + escreverDinheiro(dinheiro.rendaDoDiaDaClasseCivil);
    resumo += " | dias em déficit: comida " + std::to_string(dinheiro.diasEmDeficitDeComida) + ", materiais "
        + std::to_string(dinheiro.diasEmDeficitDeMateriais);
    if (dinheiro.comidaQueFaltouHoje > 0) {
        resumo += " (faltou comida para " + std::to_string(dinheiro.comidaQueFaltouHoje) + ")";
    }
    if (dinheiro.consumoNaoPagoHoje > 0) {
        resumo += " | consumo não pago " + escreverDinheiro(dinheiro.consumoNaoPagoHoje);
    }
    return resumo;
}
