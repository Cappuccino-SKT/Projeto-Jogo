#include "Economia/ConsumoESalarios.h"

#include "Crime/Crime.h"

#include "Eventos/EventosDaVila.h"
#include "Vila/Populacao.h"
#include "Vila/Trabalho.h"
#include "Clima/ClimaDaVila.h"
#include <algorithm>
#include <optional>

#include "Economia/FelicidadeDaClasse.h"
#include "Economia/MercadoLocal.h"
#include "Eventos/EventosDaVila.h"
#include "Jogador/DecisoesDoJogador.h"
#include "Jogador/ObrasDaVila.h"
#include "Npc/ComponentesDoNpc.h"

namespace {

// Uma pilha do inventário: o item e o dono.
struct PilhaDoInventario {
    int numeroDoItem = -1;
    DonoDoItem dono = DonoDoItem::Cidade;
};

bool itemEhDoTipoPedido(const DefinicaoDeItem& item, bool procurarComida)
{
    return procurarComida ? item.ehComida() : item.ehMaterialBasico();
}

// O preço que a classe paga: à Cidade, o da loja (110% do preço local); ao
// jogador, o preço local mais a porcentagem que ele fixou (FL8-6, de -30% a +30%).
int precoQueAClassePaga(const ContextoDaProducao& contexto, const PilhaDoInventario& pilha)
{
    const int precoLocal = precoLocalDoItem(contexto, pilha.numeroDoItem);
    return pilha.dono == DonoDoItem::Cidade ? calcularPrecoDaLojaDaCidade(precoLocal, contexto.dados.regras.economia)
                                            : calcularPrecoDoItemDoJogador(precoLocal, contexto.decisoes.porcentagemDoPrecoDe(pilha.numeroDoItem));
}

// O item mais barato do tipo pedido (comida ou Material Básico) que existe
// no estoque, pelo preço que a classe paga; no empate, o da Cidade antes do
// do jogador, e depois o de menor número. Vazio se não há nenhum.
std::optional<PilhaDoInventario> procurarOMaisBarato(const ContextoDaProducao& contexto, bool procurarComida)
{
    std::optional<PilhaDoInventario> maisBarato;
    int menorPreco = 0;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        if (!itemEhDoTipoPedido(item, procurarComida)) {
            continue;
        }
        for (const DonoDoItem dono : {DonoDoItem::Cidade, DonoDoItem::Jogador}) {
            if (contexto.inventario.quantidadeDe(item.numeroInterno, dono) <= 0) {
                continue;
            }
            const PilhaDoInventario pilha{item.numeroInterno, dono};
            const int preco = precoQueAClassePaga(contexto, pilha);
            if (!maisBarato.has_value() || preco < menorPreco) {
                maisBarato = pilha;
                menorPreco = preco;
            }
        }
    }
    return maisBarato;
}

// O item do tipo pedido que a vila come quando ele falta (o de menor preço
// local), para a demanda que faltou (regra 10.1). -1 se não há nenhum.
int itemMaisBaratoDoTipo(const ContextoDaProducao& contexto, bool procurarComida)
{
    int maisBarato = -1;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        if (itemEhDoTipoPedido(item, procurarComida)
            && (maisBarato < 0 || precoLocalDoItem(contexto, item.numeroInterno) < precoLocalDoItem(contexto, maisBarato))) {
            maisBarato = item.numeroInterno;
        }
    }
    return maisBarato;
}

// Passo 3 da regra 4.1: a classe paga ao dono do item. Sem dinheiro, paga o
// que tem, e o resto vira consumo não pago. Devolve o preço cheio.
int64_t pagarOItemConsumido(ContextoDaProducao& contexto, const PilhaDoInventario& pilha)
{
    const int64_t preco = precoQueAClassePaga(contexto, pilha);
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    const int64_t pago = std::min(preco, std::max<int64_t>(dinheiro.dinheiroDaClasseCivil, 0));
    dinheiro.dinheiroDaClasseCivil -= pago;
    (pilha.dono == DonoDoItem::Cidade ? dinheiro.bancoDaCidade : dinheiro.dinheiroDoJogador) += pago;
    dinheiro.consumoNaoPagoHoje += preco - pago;
    return preco;
}

// Tira 1 item do tipo pedido e paga. Devolve o preço cheio, ou vazio se não havia nenhum.
std::optional<int64_t> consumirUmItem(ContextoDaProducao& contexto, bool procurarComida)
{
    const std::optional<PilhaDoInventario> pilha = procurarOMaisBarato(contexto, procurarComida);
    if (!pilha.has_value()) {
        return std::nullopt;
    }
    contexto.inventario.tentarTirar(pilha->numeroDoItem, pilha->dono, 1);
    contexto.consumoMedido.anotarOQueSaiu(pilha->numeroDoItem, 1);
    return pagarOItemConsumido(contexto, *pilha);
}

// O preço de um item para a cesta, mesmo sem estoque: o da pilha mais
// barata, ou, sem nenhuma, o da loja da cidade para o item mais barato.
int64_t precoDoTipoNaCesta(const ContextoDaProducao& contexto, bool procurarComida)
{
    if (const std::optional<PilhaDoInventario> pilha = procurarOMaisBarato(contexto, procurarComida)) {
        return precoQueAClassePaga(contexto, *pilha);
    }
    const int item = itemMaisBaratoDoTipo(contexto, procurarComida);
    return item < 0 ? 0 : calcularPrecoDaLojaDaCidade(precoLocalDoItem(contexto, item), contexto.dados.regras.economia);
}

// Regra 10.5: o gasto extra, depois da cesta.
void fazerOGastoExtra(ContextoDaProducao& contexto, int materiaisQueACestaTirou)
{
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;
    dinheiro.gastoExtraDeHoje = 0;

    // 1 e 2. O teto do dia (80% da renda de ontem) menos a cesta, sem passar do dinheiro da classe.
    //    Fase C1 (Facção, FA1-2): com a lei de imposto, o teto é 100% menos a taxa.
    const int tetoEmPorcento = contexto.impostoDaCidadeEmPorcento >= 0 ? 100 - contexto.impostoDaCidadeEmPorcento : economia.gastoDaClasseEmPorcentoDaRenda;
    const int64_t tetoDoDia = dinheiro.rendaDoDiaDaClasseCivil * tetoEmPorcento / 100;
    int64_t paraGastar = std::min(tetoDoDia - dinheiro.gastoDaCestaDeHoje, dinheiro.dinheiroDaClasseCivil);
    if (paraGastar <= 0) {
        return;
    }
    // 3. Materiais a mais: até o mesmo número que a cesta tirou hoje.
    int comprasDoJogador = 0;
    for (int material = 0; material < materiaisQueACestaTirou; ++material) {
        const std::optional<PilhaDoInventario> pilha = procurarOMaisBarato(contexto, false);
        if (!pilha.has_value() || precoQueAClassePaga(contexto, *pilha) > paraGastar) {
            break;
        }
        // FL8-6: com o preço do jogador acima do local, o gasto extra compra uma fração.
        if (pilha->dono == DonoDoItem::Jogador
            && comprasDoJogador >= calcularQuantidadeDoGastoExtraComOPreco(materiaisQueACestaTirou, contexto.decisoes.porcentagemDoPrecoDe(pilha->numeroDoItem),
                                                                           economia.decisoesDoJogador)) {
            break;
        }
        comprasDoJogador += pilha->dono == DonoDoItem::Jogador ? 1 : 0;
        contexto.inventario.tentarTirar(pilha->numeroDoItem, pilha->dono, 1);
        contexto.consumoMedido.anotarOQueSaiu(pilha->numeroDoItem, 1);
        const int64_t preco = pagarOItemConsumido(contexto, *pilha);
        paraGastar -= preco;
        dinheiro.gastoExtraDeHoje += preco;
    }
    // 4. O resto vai para a loja da cidade (o Banco): a comida de Tier 2, a
    //    Bebida e os serviços que a fatia ainda não tem. Nenhum item sai.
    dinheiro.dinheiroDaClasseCivil -= paraGastar;
    dinheiro.bancoDaCidade += paraGastar;
    dinheiro.gastoExtraDeHoje += paraGastar;
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
                                 int ticksPorHoraDeJogo, int riquezaEmPorcento, int desempregoEmPorcento, int nivelDeSalarioEmPorcento)
{
    const long long numerador = static_cast<long long>(ticksTrabalhados) * centesimosPorHora * bonusDeNivelEmPorcento * gerenteEmPorcento
        * riquezaEmPorcento * desempregoEmPorcento * nivelDeSalarioEmPorcento;
    const long long denominador = static_cast<long long>(ticksPorHoraDeJogo) * 10 * 100 * 100 * 100 * 100 * 100;
    return arredondarMetadeParaCima(numerador, denominador);
}

int calcularMultiplicadorDaRiqueza(int64_t dinheiroDaClasse, int quantidadeDeCivis, int64_t cestaDeUmCivilEmDecimos, int diasDaCesta,
                                   const RegrasDosMultiplicadoresDoSalario& regras)
{
    if (quantidadeDeCivis <= 0 || cestaDeUmCivilEmDecimos <= 0) {
        return 100;
    }
    // riqueza média ÷ (10 dias da cesta) = dinheiro × dias da cesta ÷ (10 × cesta × Civis).
    const int64_t faixa = regras.riquezaMaximaEmPorcento - regras.riquezaMinimaEmPorcento;
    const int64_t multiplicador = regras.riquezaMinimaEmPorcento
        + faixa * std::max<int64_t>(dinheiroDaClasse, 0) * diasDaCesta / (static_cast<int64_t>(regras.diasDaCestaParaOMaximo) * cestaDeUmCivilEmDecimos * quantidadeDeCivis);
    return static_cast<int>(std::clamp<int64_t>(multiplicador, regras.riquezaMinimaEmPorcento, regras.riquezaMaximaEmPorcento));
}

int calcularMultiplicadorDoDesemprego(int desempregados, int quantidadeDeCivis, const RegrasDosMultiplicadoresDoSalario& regras)
{
    const int taxaEmPorcento = quantidadeDeCivis > 0 ? desempregados * 100 / quantidadeDeCivis : 0;
    return std::clamp(regras.desempregoMaximoEmPorcento - taxaEmPorcento, regras.desempregoMinimoEmPorcento, regras.desempregoMaximoEmPorcento);
}

int64_t calcularImpostoDoSalario(int64_t salarioEmDecimos, const RegrasDoImposto& regras)
{
    return arredondarMetadeParaCima(salarioEmDecimos * regras.taxaEmPorcento, 100);
}

int64_t calcularCestaDeUmCivilEmDecimos(const ContextoDaProducao& contexto)
{
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;
    return static_cast<int64_t>(economia.diasEntreMateriaisDoCivil) * economia.comidaPorDiaDoCivil * precoDoTipoNaCesta(contexto, true)
        + precoDoTipoNaCesta(contexto, false);
}

int contarOsDesempregados(const ContextoDaProducao& contexto)
{
    int desempregados = 0;
    for (const auto [npc, identidade, casaETrabalho, trabalhoDoDia] :
         contexto.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc, TrabalhoDoDia>().each()) {
        desempregados += identidade.classe == "civil" && casaETrabalho.indiceDoTrabalho < 0 ? 1 : 0;
    }
    return desempregados;
}

void consumirACestaDoDia(ContextoDaProducao& contexto)
{
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;
    dinheiro.gastoDaCestaDeHoje = 0;

    // Passo 1: a comida de cada Civil.
    int comidaQueFaltou = 0;
    for (int civil = 0; civil < dinheiro.quantidadeDeCivis; ++civil) {
        for (int comida = 0; comida < economia.comidaPorDiaDoCivil; ++comida) {
            const std::optional<int64_t> preco = consumirUmItem(contexto, true);
            dinheiro.gastoDaCestaDeHoje += preco.value_or(0);
            comidaQueFaltou += preco.has_value() ? 0 : 1;
        }
    }

    // Passo 2: os Materiais Básicos, pelo acumulador (15 Civis gastam 1,875 por dia).
    int materiaisQueACestaTirou = 0;
    int materiaisQueFaltaram = 0;
    dinheiro.materiaisDevidosEmOitavos += dinheiro.quantidadeDeCivis;
    while (dinheiro.materiaisDevidosEmOitavos >= economia.diasEntreMateriaisDoCivil) {
        dinheiro.materiaisDevidosEmOitavos -= economia.diasEntreMateriaisDoCivil;
        const std::optional<int64_t> preco = consumirUmItem(contexto, false);
        dinheiro.gastoDaCestaDeHoje += preco.value_or(0);
        materiaisQueACestaTirou += preco.has_value() ? 1 : 0;
        materiaisQueFaltaram += preco.has_value() ? 0 : 1;
    }

    // Passo 4: os dias em déficit.
    dinheiro.comidaQueFaltouHoje = comidaQueFaltou;
    dinheiro.diasEmDeficitDeComida = comidaQueFaltou > 0 ? dinheiro.diasEmDeficitDeComida + 1 : 0;
    dinheiro.diasEmDeficitDeMateriais = materiaisQueFaltaram > 0 ? dinheiro.diasEmDeficitDeMateriais + 1 : 0;

    // A cesta que faltou também é demanda: a vila quis comer e não tinha (regra 10.1).
    const int comidaDaCesta = itemMaisBaratoDoTipo(contexto, true);
    const int materialDaCesta = itemMaisBaratoDoTipo(contexto, false);
    if (comidaDaCesta >= 0) {
        contexto.consumoMedido.anotarADemandaQueFaltou(comidaDaCesta, static_cast<int64_t>(comidaQueFaltou) * 100);
    }
    if (materialDaCesta >= 0) {
        contexto.consumoMedido.anotarADemandaQueFaltou(materialDaCesta, static_cast<int64_t>(materiaisQueFaltaram) * 100);
    }

    // Ficha do Clima, regra 4.3: a lenha do inverno, depois da cesta e antes do gasto extra.
    consumirALenhaDoDia(contexto);
    // Fase A6 (ficha da Cura, regra 4.2, passo 1): o Remédio da Epidemia.
    gastarORemedioDaEpidemia(contexto);

    // Regra 10.5: o gasto extra.
    fazerOGastoExtra(contexto, materiaisQueACestaTirou);

    // O consumo do dia está todo anotado: o dia fecha na média dos 5 (regra 10.1).
    contexto.consumoMedido.fecharODia();
}

std::vector<std::string> pagarOsSalariosDoDia(ContextoDaProducao& contexto)
{
    std::vector<std::string> linhasParaOConsole;
    entt::registry& registro = contexto.registroDasEntidades;
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;
    const VilaDoJogo& vila = contexto.vila;

    // Os multiplicadores do dia (regra 10.6), iguais para todos.
    const int riqueza = calcularMultiplicadorDaRiqueza(dinheiro.dinheiroDaClasseCivil, dinheiro.quantidadeDeCivis,
                                                       calcularCestaDeUmCivilEmDecimos(contexto), economia.diasEntreMateriaisDoCivil,
                                                       economia.multiplicadoresDoSalario);
    const int desemprego = calcularMultiplicadorDoDesemprego(contarOsDesempregados(contexto), dinheiro.quantidadeDeCivis, economia.multiplicadoresDoSalario);

    // Os empregados de cada construção, em ordem pelo número (quem vem antes recebe antes).
    std::vector<std::vector<std::pair<int, entt::entity>>> empregadosDeCadaConstrucao(vila.construcoes.size());
    for (const auto [npc, identidade, casaETrabalho] : registro.view<IdentidadeDoNpc, CasaETrabalhoDoNpc>().each()) {
        if (casaETrabalho.indiceDoTrabalho >= 0) {
            empregadosDeCadaConstrucao[casaETrabalho.indiceDoTrabalho].emplace_back(identidade.numeroDoNpc, npc);
        }
    }

    int64_t rendaDoDia = 0;
    dinheiro.impostoDeHoje = 0;
    for (std::size_t indice = 0; indice < vila.construcoes.size(); ++indice) {
        std::vector<std::pair<int, entt::entity>>& empregados = empregadosDeCadaConstrucao[indice];
        std::sort(empregados.begin(), empregados.end(), [](const auto& primeiro, const auto& segundo) { return primeiro.first < segundo.first; });
        const ConstrucaoDaVila& construcao = vila.construcoes[indice];
        ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indice];
        // Passo 2: quem paga é o dono da construção.
        int64_t& dinheiroDeQuemPaga = construcao.donoEhOJogador ? dinheiro.dinheiroDoJogador : dinheiro.bancoDaCidade;
        const int centesimosPorHora = salarioPorHoraDoTrabalho(trabalhoDaConstrucao(contexto, static_cast<int>(indice)), contexto.dados.regras.regrasDoPreco);
        const int bonusDeNivel = 100 + economia.bonusDoSalarioPorNivelEmPorcento * (construcao.nivel - 1);
        const int nivelDeSalario = construcao.donoEhOJogador ? salarioDoNivelEmPorcento(producao.nivelDeSalario, economia.decisoesDoJogador) : 100;
        bool alguemFicouSemReceber = false;
        for (const auto& [numeroDoNpc, npc] : empregados) {
            const int ticksDoDia = registro.get<TrabalhoDoDia>(npc).ticksTrabalhadosHoje;
            if (ticksDoDia <= 0) {
                continue;
            }
            const int gerente = numeroDoNpc == producao.numeroDoGerente ? economia.salarioDoGerenteEmPorcento : 100;
            const int64_t salario =
                calcularSalarioEmDecimos(ticksDoDia, centesimosPorHora, bonusDeNivel, gerente, contexto.ticksPorHoraDeJogo, riqueza, desemprego, nivelDeSalario);
            // Passo 3: sem dinheiro para o salário inteiro, não recebe nada.
            if (dinheiroDeQuemPaga < salario) {
                alguemFicouSemReceber = true;
                producao.salarioDevidoEmDecimos += salario;  // fase A2 (Trabalho, regra 4.4, passo 0)
                continue;
            }
            // Passo 4, com o imposto retido na hora (regra 10.7): a parte da
            // cidade vai para o Banco, e o empregado recebe o resto.
            RegrasDoImposto impostoDaCidade = economia.imposto;
            if (contexto.impostoDaCidadeEmPorcento >= 0) {
                impostoDaCidade.taxaEmPorcento = contexto.impostoDaCidadeEmPorcento;  // fase C1: a lei da facção
            }
            const int64_t imposto = calcularImpostoDoSalario(salario, impostoDaCidade);
            dinheiroDeQuemPaga -= salario;
            dinheiro.bancoDaCidade += imposto * economia.imposto.parteDaCidadeEmPorcento / 100;
            dinheiro.dinheiroDaClasseCivil += salario - imposto;
            dinheiro.impostoDeHoje += imposto;
            rendaDoDia += salario;
        }
        producao.diasDeSalarioAtrasado = alguemFicouSemReceber ? producao.diasDeSalarioAtrasado + 1 : 0;

        // FL8-7: depois de 8 dias seguidos em salário Baixo, o empregado mais novo sai.
        if (construcao.donoEhOJogador && producao.nivelDeSalario == NivelDeSalario::Baixo) {
            ++producao.diasSeguidosComSalarioBaixo;
            if (producao.diasSeguidosComSalarioBaixo >= economia.decisoesDoJogador.diasDeSalarioBaixoParaUmEmpregadoSair && !empregados.empty()) {
                registro.get<CasaETrabalhoDoNpc>(empregados.back().second).indiceDoTrabalho = -1;
                linhasParaOConsole.push_back("Aviso: um empregado de " + construcao.identificador + " saiu depois de "
                                             + std::to_string(producao.diasSeguidosComSalarioBaixo) + " dias com o salário Baixo.");
                empregados.pop_back();
                producao.diasSeguidosComSalarioBaixo = 0;
            }
        } else {
            producao.diasSeguidosComSalarioBaixo = 0;
        }

        // Regra 10.8: com 16 dias seguidos de atraso, os empregados saem.
        if (producao.diasDeSalarioAtrasado >= economia.diasDeAtrasoAteOsEmpregadosSairem) {
            for (const auto& [numeroDoNpc, npc] : empregados) {
                registro.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho = -1;
                registro.get<CasaETrabalhoDoNpc>(npc).saiuPorAtrasoDe = static_cast<int>(indice);  // Trabalho, regra 4.1, passo 3
            }
            producao.diasDeSalarioAtrasado = 0;
            linhasParaOConsole.push_back("Aviso: os " + std::to_string(empregados.size()) + " empregados de " + construcao.identificador
                                         + " saíram depois de " + std::to_string(economia.diasDeAtrasoAteOsEmpregadosSairem)
                                         + " dias sem salário.");
        }
    }
    dinheiro.rendaDoDiaDaClasseCivil = rendaDoDia;

    // Passo 5: as horas do dia voltam a zero.
    for (const auto [npc, trabalhoDoDia] : registro.view<TrabalhoDoDia>().each()) {
        trabalhoDoDia.ticksTrabalhadosHoje = 0;
    }
    return linhasParaOConsole;
}

void rodarOEstabilizador(ContextoDaProducao& contexto, const RelogioDoMundo& relogio)
{
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;
    const RegrasDoEstabilizador& regras = economia.estabilizador;
    dinheiro.ajudaDoEstabilizadorHoje = 0;

    // 1. No dia 1 de cada estação, entra a verba da Coroa (FE4-1: o único dinheiro de
    //    fora além do Mercador) e o teto recomeça.
    if (relogio.diaDaEstacaoAtual() == 1) {
        // As outras cidades têm a verba delas (Outras cidades, 4.7).
        const int64_t verba = contexto.vila.verbaDaCoroaPorEstacaoEmDecimos >= 0 ? contexto.vila.verbaDaCoroaPorEstacaoEmDecimos : economia.verbaDaCoroaPorEstacaoEmDecimos;
        dinheiro.bancoDaCidade += verba;
        dinheiro.dinheiroQueEntrouDeFora += verba;
        dinheiro.bancoNoComecoDaEstacao = dinheiro.bancoDaCidade;
        dinheiro.ajudaDoEstabilizadorNaEstacao = 0;
    }
    // 2. Abaixo de 3 dias da cesta, o Banco paga até a classe chegar a 5 dias.
    //    A cesta vem em "dias entre Materiais" (8 dias): N dias = N × cesta ÷ 8.
    const int64_t cesta = calcularCestaDeUmCivilEmDecimos(contexto);
    const int64_t civis = dinheiro.quantidadeDeCivis;
    const int64_t diasDaCesta = economia.diasEntreMateriaisDoCivil;
    if (civis <= 0 || cesta <= 0 || dinheiro.dinheiroDaClasseCivil * diasDaCesta >= regras.ajudaAbaixoDeDiasDaCesta * cesta * civis) {
        return;
    }
    const int64_t falta = regras.ajudaAteDiasDaCesta * cesta * civis / diasDaCesta - dinheiro.dinheiroDaClasseCivil;
    // 3. O teto da estação e o dinheiro do Banco.
    const int64_t tetoQueSobra = dinheiro.bancoNoComecoDaEstacao * regras.tetoPorEstacaoEmPorcentoDoBanco / 100 - dinheiro.ajudaDoEstabilizadorNaEstacao;
    const int64_t pago = std::max<int64_t>(0, std::min({falta, tetoQueSobra, dinheiro.bancoDaCidade}));
    dinheiro.bancoDaCidade -= pago;
    dinheiro.dinheiroDaClasseCivil += pago;
    dinheiro.ajudaDoEstabilizadorNaEstacao += pago;
    dinheiro.ajudaDoEstabilizadorHoje = pago;
}

void rodarAFelicidadeDoDia(ContextoDaProducao& contexto)
{
    const DinheiroDaVila& dinheiro = contexto.dinheiro;
    const RegrasDaFelicidade& regras = contexto.dados.regras.felicidade;
    const int comidaPorDia = std::max(contexto.dados.regras.economia.comidaPorDiaDoCivil, 1);

    SituacaoDoDiaParaAFelicidade situacao;
    situacao.quantidadeDeCivis = dinheiro.quantidadeDeCivis;
    situacao.civisQueFicaramSemComida = std::min(dinheiro.quantidadeDeCivis, (dinheiro.comidaQueFaltouHoje + comidaPorDia - 1) / comidaPorDia);
    situacao.materiaisEmDeficit = dinheiro.diasEmDeficitDeMateriais > 0;
    situacao.gastoExtraDeHoje = dinheiro.gastoExtraDeHoje;
    situacao.gastoDaCestaDeHoje = dinheiro.gastoDaCestaDeHoje;
    situacao.civisDesempregados = contarOsDesempregados(contexto);
    for (const auto [npc, identidade, casaETrabalho, trabalhoDoDia] :
         contexto.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc, TrabalhoDoDia>().each()) {
        situacao.civisSemCasa += identidade.classe == "civil" && casaETrabalho.indiceDaCasa < 0 ? 1 : 0;
    }
    situacao.felicidadeDosEventosEmPontos = felicidadeDosEventosAtivos(contexto) + pontosDoClimaNosEventos(contexto) + contexto.felicidadeDaFaccaoEmPontos;
    situacao.fatorLeisECulturaEmDecimos = contexto.fatorLeisECulturaEmDecimos;
    situacao.felicidadeDaCriminalidadeEmDecimos = felicidadeDaCriminalidadeEmDecimos(contexto.felicidade.criminalidadeEmDecimos, contexto.dados.regrasDoCrime);
    situacao.pontosDoClimaNoConsumo = pontosDoClimaNoConsumoAtendido(contexto);

    FelicidadeDaClasse& felicidade = contexto.felicidade;
    felicidade.fatoresDoDia = calcularFatoresDaFelicidade(situacao, regras);
    felicidade.alvoEmDecimos = calcularAlvoDaFelicidade(felicidade.fatoresDoDia, regras);
    felicidade.felicidadeEmDecimos = darOPassoDaFelicidade(felicidade.felicidadeEmDecimos, felicidade.alvoEmDecimos, regras);
}

std::vector<std::string> rodarAOrdemDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio)
{
    std::vector<std::string> linhasParaOConsole;
    contexto.eventos.mudancasDeHoje.clear();
    contexto.eventos.linhasParaOConsole.clear();
    contexto.trabalho.contratacoesDeHoje = 0;
    contexto.trabalho.trocasDeHoje = 0;
    for (const PassoDaOrdemDoDia passo : contexto.dados.regras.economia.ordemDoDia) {
        switch (passo) {
        case PassoDaOrdemDoDia::Populacao: {
            const std::vector<std::string> linhasDaPopulacao = rodarAPopulacaoDoDia(contexto, relogio);
            linhasParaOConsole.insert(linhasParaOConsole.end(), linhasDaPopulacao.begin(), linhasDaPopulacao.end());
            break;
        }
        case PassoDaOrdemDoDia::Clima: {
            const std::vector<std::string> linhasDoClima = rodarOClimaDoDia(contexto, relogio);
            linhasParaOConsole.insert(linhasParaOConsole.end(), linhasDoClima.begin(), linhasDoClima.end());
            break;
        }
        case PassoDaOrdemDoDia::Manutencao:
            rodarAManutencaoDoDia(contexto, relogio);
            break;
        case PassoDaOrdemDoDia::Consumo:
            consumirACestaDoDia(contexto);
            break;
        case PassoDaOrdemDoDia::Salarios: {
            // O gerente de cada construção é escolhido antes dos salários (ficha da Construção).
            escolherOsGerentes(contexto);
            const std::vector<std::string> saidas = pagarOsSalariosDoDia(contexto);
            linhasParaOConsole.insert(linhasParaOConsole.end(), saidas.begin(), saidas.end());
            // Fase A2 (ficha do Trabalho): no dia 1 da estação, a redistribuição;
            // todo dia, as vagas da Cidade e os dias sem empregados.
            if (relogio.diaDaEstacaoAtual() == 1) {
                for (const auto [npc, casaETrabalho] : contexto.registroDasEntidades.view<CasaETrabalhoDoNpc>().each()) {
                    casaETrabalho.saiuPorAtrasoDe = -1;
                }
                const std::vector<std::string> trocas = redistribuirEmpregosDaCidade(contexto);
                linhasParaOConsole.insert(linhasParaOConsole.end(), trocas.begin(), trocas.end());
            }
            const std::vector<std::string> contratacoes = preencherVagasDaCidade(contexto);
            linhasParaOConsole.insert(linhasParaOConsole.end(), contratacoes.begin(), contratacoes.end());
            const std::vector<std::string> leilao = contarOsDiasSemEmpregados(contexto, relogio.diasPorEstacao());
            linhasParaOConsole.insert(linhasParaOConsole.end(), leilao.begin(), leilao.end());
            break;
        }
        case PassoDaOrdemDoDia::MercadoLocal:
            rodarOMercadoLocalDoDia(contexto);
            break;
        case PassoDaOrdemDoDia::Estabilizador:
            rodarOEstabilizador(contexto, relogio);
            break;
        case PassoDaOrdemDoDia::Felicidade: {
            rodarAFelicidadeDoDia(contexto);
            // Fase C3 (Crime, passo 5b): a Criminalidade, com a Felicidade de hoje.
            const int civis = contexto.dinheiro.quantidadeDeCivis;
            const int64_t doisDiasDaCesta = 2 * calcularCestaDeUmCivilEmDecimos(contexto) * civis / std::max(1, contexto.dados.regras.economia.diasEntreMateriaisDoCivil);
            const std::vector<std::string> linhasDoCrime =
                rodarACriminalidadeDoDia(contexto, civis, contarOsDesempregados(contexto), contexto.dinheiro.dinheiroDaClasseCivil < doisDiasDaCesta);
            linhasParaOConsole.insert(linhasParaOConsole.end(), linhasDoCrime.begin(), linhasDoCrime.end());
            break;
        }
        case PassoDaOrdemDoDia::Eventos:
            rodarOsEventosDoDia(contexto, relogio);
            linhasParaOConsole.insert(linhasParaOConsole.end(), contexto.eventos.linhasParaOConsole.begin(), contexto.eventos.linhasParaOConsole.end());
            {
                // Fase A2 (Trabalho, regra 4.4): o leilão anda no passo dos eventos.
                const std::vector<std::string> leilao = avancarOLeilaoDoDia(contexto);
                linhasParaOConsole.insert(linhasParaOConsole.end(), leilao.begin(), leilao.end());
            }
            break;
        case PassoDaOrdemDoDia::EscolhaDasReceitas:
            escolherAsReceitasDaCidade(contexto);
            break;
        }
    }
    return linhasParaOConsole;
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
    // Os preços locais da cesta (marco 7).
    resumo += " | preços:";
    primeiroItem = true;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        if (item.ehComida() || item.ehMaterialBasico()) {
            resumo += std::string(primeiroItem ? " " : ", ") + contexto.dados.tabelaDeTextos.textoDe(item.identificadorDeTexto) + " "
                + escreverDinheiro(precoLocalDoItem(contexto, item.numeroInterno));
            primeiroItem = false;
        }
    }
    const DinheiroDaVila& dinheiro = contexto.dinheiro;
    const int64_t mediaDosCivis = dinheiro.quantidadeDeCivis > 0 ? dinheiro.dinheiroDaClasseCivil / dinheiro.quantidadeDeCivis : 0;
    resumo += " | Banco " + escreverDinheiro(dinheiro.bancoDaCidade) + ", jogador " + escreverDinheiro(dinheiro.dinheiroDoJogador) + ", Civis "
        + escreverDinheiro(mediaDosCivis) + " em média, salários " + escreverDinheiro(dinheiro.rendaDoDiaDaClasseCivil) + " (imposto "
        + escreverDinheiro(dinheiro.impostoDeHoje) + "), cesta " + escreverDinheiro(dinheiro.gastoDaCestaDeHoje) + ", gasto extra "
        + escreverDinheiro(dinheiro.gastoExtraDeHoje);
    if (dinheiro.ajudaDoEstabilizadorHoje > 0) {
        resumo += ", ajuda do Banco " + escreverDinheiro(dinheiro.ajudaDoEstabilizadorHoje);
    }
    resumo += " | Felicidade " + escreverDinheiro(contexto.felicidade.felicidadeEmDecimos) + " ("
        + nomeDaFaixaDaFelicidade(contexto.felicidade.felicidadeEmDecimos) + ")";
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
