#include "Vila/Trabalho.h"

#include <algorithm>

#include "Construcao/ContasDasConstrucoes.h"
#include "Npc/ComponentesDoNpc.h"

namespace {

// O dinheiro em décimos para o Diário: 247,5.
std::string escreverDinheiro(int64_t valorEmDecimos)
{
    return escreverDinheiroEmMoedas(static_cast<int>(valorEmDecimos));
}

// O salário por hora do Trabalho, em centésimos (Economia, FL3-3).
int salarioPorHoraDoTrabalho(TrabalhoDaReceita trabalho, const RegrasDoPreco& regrasDoPreco)
{
    switch (trabalho) {
    case TrabalhoDaReceita::Bracal: return regrasDoPreco.salarioPorHoraBracalEmCentesimos;
    case TrabalhoDaReceita::Manufaturado: return regrasDoPreco.salarioPorHoraManufaturadoEmCentesimos;
    case TrabalhoDaReceita::Especialista: return regrasDoPreco.salarioPorHoraEspecialistaEmCentesimos;
    }
    return regrasDoPreco.salarioPorHoraManufaturadoEmCentesimos;
}

// Um NPC da vila (não os visitantes do teste de desempenho), com o número.
struct NpcDaVila {
    int numeroDoNpc = 0;
    entt::entity entidade = entt::null;
};

std::vector<NpcDaVila> npcsDaVilaEmOrdem(ContextoDaProducao& contexto)
{
    std::vector<NpcDaVila> npcs;
    for (const auto [npc, identidade, casaETrabalho, habilidades] :
         contexto.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc, HabilidadesDoNpc>().each()) {
        npcs.push_back({identidade.numeroDoNpc, npc});
    }
    std::sort(npcs.begin(), npcs.end(), [](const NpcDaVila& primeiro, const NpcDaVila& segundo) { return primeiro.numeroDoNpc < segundo.numeroDoNpc; });
    return npcs;
}

int contarEmpregadosDe(ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    int empregados = 0;
    for (const auto [npc, casaETrabalho] : contexto.registroDasEntidades.view<CasaETrabalhoDoNpc>().each()) {
        empregados += casaETrabalho.indiceDoTrabalho == indiceDaConstrucao ? 1 : 0;
    }
    return empregados;
}

// As construções que empregam (a Função das vagas não é vazia): não as casas.
bool construcaoEmprega(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    const DefinicaoDeConstrucao* definicao = contexto.dados.catalogoDeConstrucoes.procurarConstrucao(contexto.vila.construcoes[indiceDaConstrucao].tipo);
    return definicao != nullptr && !definicao->funcao.empty();
}

std::string nomeDoNpc(ContextoDaProducao& contexto, entt::entity npc)
{
    return contexto.registroDasEntidades.get<IdentidadeDoNpc>(npc).nome;
}

// A volta para a Cidade (venda ou leilão sem lance): a fila apagada, o modo
// automático desligado, as contas da construção zeradas.
void passarParaACidade(ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    ConstrucaoDaVila& construcao = contexto.vila.construcoes[indiceDaConstrucao];
    ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indiceDaConstrucao];
    construcao.donoEhOJogador = false;
    producao.ordens.clear();
    producao.modoAutomatico = false;
    producao.nivelDeSalario = NivelDeSalario::Normal;
    producao.diasSeguidosComSalarioBaixo = 0;
    producao.diasDeSalarioAtrasado = 0;
    producao.salarioDevidoEmDecimos = 0;
    producao.diasSemEmpregados = 0;
    escolherOsGerentes(contexto);
}

}  // namespace

bool construcaoSegueAFilaDoJogador(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    return contexto.vila.construcoes[indiceDaConstrucao].donoEhOJogador && !contexto.producaoDasConstrucoes[indiceDaConstrucao].modoAutomatico;
}

int64_t calcularValorDaConstrucao(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indiceDaConstrucao];
    const DefinicaoDeConstrucao* definicao = contexto.dados.catalogoDeConstrucoes.procurarConstrucao(construcao.tipo);
    if (definicao == nullptr) {
        return 0;
    }
    const CustoDaObra& obra = contexto.dados.regras.construcoes.obraDeCadaPorte[static_cast<int>(definicao->porte)];
    const auto precoDe = [&](const char* identificador) {
        const std::optional<int> item = contexto.dados.catalogoDeItens.procurarNumeroDoItem(identificador);
        return item ? static_cast<int64_t>(precoLocalDoItem(contexto, *item)) : 0;
    };
    // As horas da obra a 0,75 moeda (o salário Braçal): 7,5 décimos por hora.
    const int64_t horasEmDecimos = static_cast<int64_t>(obra.horasDeTrabalho) * contexto.dados.regras.regrasDoPreco.salarioPorHoraBracalEmCentesimos / 10;
    const int64_t valorDoNivel = obra.tabuas * precoDe("item.tabua") + obra.blocosDePedra * precoDe("item.bloco_de_pedra") + horasEmDecimos;
    return valorDoNivel * construcao.nivel * contexto.dados.regras.trabalho.valorDaConstrucaoEmPorcentoDaObra / 100;
}

std::vector<std::string> preencherVagasDaCidade(ContextoDaProducao& contexto)
{
    std::vector<std::string> linhas;
    const RegrasDoTrabalho& regras = contexto.dados.regras.trabalho;
    bool contratouAlguem = false;
    for (int indice = 0; indice < static_cast<int>(contexto.vila.construcoes.size()); ++indice) {
        const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indice];
        if (construcao.donoEhOJogador || construcao.emObra || !construcaoEmprega(contexto, indice)) {
            continue;
        }
        const int vagas = calcularVagas(construcao.nivel, contexto.dados.regras.construcoes);
        const TrabalhoDaReceita trabalho = trabalhoDaConstrucao(contexto, indice);
        // Passo 1: o Banco paga 3 dias do salário da vaga (centésimos por hora × 8 h × 3, em décimos)?
        const int64_t salarioDeTresDias = static_cast<int64_t>(salarioPorHoraDoTrabalho(trabalho, contexto.dados.regras.regrasDoPreco)) * 8
            * regras.contratarSeOBancoTemDiasDeSalario / 10;
        int empregados = contarEmpregadosDe(contexto, indice);
        while (empregados < vagas && contexto.dinheiro.bancoDaCidade >= salarioDeTresDias) {
            // Passos 2 e 3: o desempregado de maior habilidade que não saiu daqui por atraso.
            entt::entity escolhido = entt::null;
            int maiorHabilidade = -1;
            for (const NpcDaVila& npc : npcsDaVilaEmOrdem(contexto)) {
                const CasaETrabalhoDoNpc& casaETrabalho = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc.entidade);
                if (casaETrabalho.indiceDoTrabalho >= 0 || casaETrabalho.saiuPorAtrasoDe == indice) {
                    continue;
                }
                const int habilidade = contexto.registroDasEntidades.get<HabilidadesDoNpc>(npc.entidade).habilidadeNoTrabalho(trabalho);
                if (habilidade > maiorHabilidade) {
                    maiorHabilidade = habilidade;
                    escolhido = npc.entidade;
                }
            }
            if (escolhido == entt::null) {
                break;
            }
            // Passos 4 e 5.
            contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(escolhido).indiceDoTrabalho = indice;
            ++empregados;
            ++contexto.trabalho.contratacoesDeHoje;
            contratouAlguem = true;
            linhas.push_back("Contratação: " + nomeDoNpc(contexto, escolhido) + " foi contratado em " + construcao.identificador + ".");
        }
    }
    if (contratouAlguem) {
        escolherOsGerentes(contexto);
    }
    return linhas;
}

std::vector<std::string> redistribuirEmpregosDaCidade(ContextoDaProducao& contexto)
{
    std::vector<std::string> linhas;
    const int diferencaMinima = contexto.dados.regras.trabalho.diferencaMinimaParaTrocar;
    // Os empregados da Cidade, na ordem da vila (pela construção e pelo número).
    std::vector<NpcDaVila> empregados;
    for (const NpcDaVila& npc : npcsDaVilaEmOrdem(contexto)) {
        const int trabalho = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc.entidade).indiceDoTrabalho;
        if (trabalho >= 0 && !contexto.vila.construcoes[trabalho].donoEhOJogador) {
            empregados.push_back(npc);
        }
    }
    std::stable_sort(empregados.begin(), empregados.end(), [&](const NpcDaVila& primeiro, const NpcDaVila& segundo) {
        return contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(primeiro.entidade).indiceDoTrabalho
            < contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(segundo.entidade).indiceDoTrabalho;
    });
    // Passos 1 a 3: cada NPC troca no máximo uma vez (nunca fica em laço).
    std::vector<bool> jaTrocou(empregados.size(), false);
    for (std::size_t primeiro = 0; primeiro < empregados.size(); ++primeiro) {
        for (std::size_t segundo = primeiro + 1; segundo < empregados.size() && !jaTrocou[primeiro]; ++segundo) {
            if (jaTrocou[segundo]) {
                continue;
            }
            CasaETrabalhoDoNpc& empregoA = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(empregados[primeiro].entidade);
            CasaETrabalhoDoNpc& empregoB = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(empregados[segundo].entidade);
            const TrabalhoDaReceita trabalhoA = trabalhoDaConstrucao(contexto, empregoA.indiceDoTrabalho);
            const TrabalhoDaReceita trabalhoB = trabalhoDaConstrucao(contexto, empregoB.indiceDoTrabalho);
            if (trabalhoA == trabalhoB) {
                continue;
            }
            const HabilidadesDoNpc& habilidadesA = contexto.registroDasEntidades.get<HabilidadesDoNpc>(empregados[primeiro].entidade);
            const HabilidadesDoNpc& habilidadesB = contexto.registroDasEntidades.get<HabilidadesDoNpc>(empregados[segundo].entidade);
            const int somaAgora = habilidadesA.habilidadeNoTrabalho(trabalhoA) + habilidadesB.habilidadeNoTrabalho(trabalhoB);
            const int somaDepois = habilidadesA.habilidadeNoTrabalho(trabalhoB) + habilidadesB.habilidadeNoTrabalho(trabalhoA);
            if (somaDepois - somaAgora < diferencaMinima) {
                continue;
            }
            std::swap(empregoA.indiceDoTrabalho, empregoB.indiceDoTrabalho);
            jaTrocou[primeiro] = true;
            jaTrocou[segundo] = true;
            ++contexto.trabalho.trocasDeHoje;
            linhas.push_back("Redistribuição: " + nomeDoNpc(contexto, empregados[primeiro].entidade) + " e " + nomeDoNpc(contexto, empregados[segundo].entidade)
                             + " trocaram de emprego.");
        }
    }
    if (!linhas.empty()) {
        escolherOsGerentes(contexto);
    }
    return linhas;
}

std::vector<std::string> contarOsDiasSemEmpregados(ContextoDaProducao& contexto, int diasPorEstacao)
{
    std::vector<std::string> linhas;
    const RegrasDoTrabalho& regras = contexto.dados.regras.trabalho;
    for (int indice = 0; indice < static_cast<int>(contexto.vila.construcoes.size()); ++indice) {
        const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indice];
        ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indice];
        if (construcao.emObra || !construcaoEmprega(contexto, indice)) {
            continue;
        }
        producao.diasSemEmpregados = contarEmpregadosDe(contexto, indice) == 0 ? producao.diasSemEmpregados + 1 : 0;
        // Passo 2: só as do jogador vão a leilão, uma de cada vez.
        if (construcao.donoEhOJogador && !contexto.trabalho.leilao.estaAberto()
            && producao.diasSemEmpregados >= regras.estacoesParadaParaOLeilao * diasPorEstacao) {
            contexto.trabalho.leilao = LeilaoDeConstrucao{indice, regras.diasDeLeilao, 0, false};
            linhas.push_back("Leilão: " + construcao.identificador + " está parada há " + std::to_string(producao.diasSemEmpregados)
                             + " dias e vai a leilão por " + std::to_string(regras.diasDeLeilao) + " dias (lance mínimo "
                             + escreverDinheiro(calcularValorDaConstrucao(contexto, indice) * regras.lanceMinimoEmPorcentoDoValor / 100) + ").");
        }
    }
    return linhas;
}

std::vector<std::string> avancarOLeilaoDoDia(ContextoDaProducao& contexto)
{
    LeilaoDeConstrucao& leilao = contexto.trabalho.leilao;
    if (!leilao.estaAberto()) {
        return {};
    }
    if (--leilao.diasQueFaltam > 0) {
        return {};
    }
    const int indice = leilao.indiceDaConstrucao;
    ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indice];
    const std::string nome = contexto.vila.construcoes[indice].identificador;
    std::vector<std::string> linhas;
    if (leilao.maiorLanceEmDecimos > 0) {
        // Com lance (TP1-4): os salários atrasados primeiro, para a classe dos
        // empregados; o resto para o dono antigo (o jogador). Quem deu o lance
        // (só o jogador, até os Ricos) continua dono.
        const int64_t paraOsSalarios = std::min(leilao.maiorLanceEmDecimos, producao.salarioDevidoEmDecimos);
        contexto.dinheiro.dinheiroDaClasseCivil += paraOsSalarios;
        contexto.dinheiro.dinheiroDoJogador += leilao.maiorLanceEmDecimos - paraOsSalarios;
        producao.salarioDevidoEmDecimos = 0;
        producao.diasSemEmpregados = 0;
        producao.diasDeSalarioAtrasado = 0;
        linhas.push_back("Leilão: " + nome + " fica com o jogador pelo lance de " + escreverDinheiro(leilao.maiorLanceEmDecimos) + " ("
                         + escreverDinheiro(paraOsSalarios) + " pagaram os salários atrasados).");
    } else {
        passarParaACidade(contexto, indice);
        linhas.push_back("Leilão: ninguém deu lance; " + nome + " voltou para a Cidade.");
    }
    leilao = LeilaoDeConstrucao{};
    return linhas;
}

std::string comprarConstrucaoDaCidade(ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    if (indiceDaConstrucao < 0 || indiceDaConstrucao >= static_cast<int>(contexto.vila.construcoes.size())) {
        return "a construção não existe";
    }
    ConstrucaoDaVila& construcao = contexto.vila.construcoes[indiceDaConstrucao];
    ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indiceDaConstrucao];
    if (construcao.donoEhOJogador || construcao.emObra || !producao.ehDeProducao) {
        return "só dá para comprar uma construção de produção pronta da Cidade";
    }
    if (contexto.trabalho.leilao.indiceDaConstrucao == indiceDaConstrucao) {
        return "a construção está em leilão";
    }
    const int64_t valor = calcularValorDaConstrucao(contexto, indiceDaConstrucao);
    if (contexto.dinheiro.dinheiroDoJogador < valor) {
        return "falta dinheiro (custa " + escreverDinheiro(valor) + ")";
    }
    contexto.dinheiro.dinheiroDoJogador -= valor;
    contexto.dinheiro.bancoDaCidade += valor;
    construcao.donoEhOJogador = true;
    // TP1-3: a construção comprada começa no modo automático; os empregados continuam.
    producao.modoAutomatico = true;
    producao.nivelDeSalario = NivelDeSalario::Normal;
    producao.diasDeSalarioAtrasado = 0;
    producao.salarioDevidoEmDecimos = 0;
    producao.diasSemEmpregados = 0;
    escolherOsGerentes(contexto);
    return "";
}

std::string venderConstrucaoParaACidade(ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    if (indiceDaConstrucao < 0 || indiceDaConstrucao >= static_cast<int>(contexto.vila.construcoes.size())) {
        return "a construção não existe";
    }
    const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indiceDaConstrucao];
    if (!construcao.donoEhOJogador || construcao.emObra) {
        return "só dá para vender uma construção pronta do jogador";
    }
    if (contexto.trabalho.leilao.indiceDaConstrucao == indiceDaConstrucao) {
        return "a construção está em leilão";
    }
    const int64_t preco = calcularValorDaConstrucao(contexto, indiceDaConstrucao) * contexto.dados.regras.trabalho.cidadeCompraDeVoltaEmPorcento / 100;
    if (contexto.dinheiro.bancoDaCidade < preco) {
        return "o Banco não tem " + escreverDinheiro(preco);
    }
    contexto.dinheiro.bancoDaCidade -= preco;
    contexto.dinheiro.dinheiroDoJogador += preco;
    passarParaACidade(contexto, indiceDaConstrucao);
    return "";
}

std::string trocarOModoAutomatico(ContextoDaProducao& contexto, int indiceDaConstrucao, bool ligado)
{
    if (indiceDaConstrucao < 0 || indiceDaConstrucao >= static_cast<int>(contexto.vila.construcoes.size())
        || !contexto.vila.construcoes[indiceDaConstrucao].donoEhOJogador) {
        return "só as construções do jogador têm o modo automático";
    }
    contexto.producaoDasConstrucoes[indiceDaConstrucao].modoAutomatico = ligado;
    return "";
}

std::string darLanceNoLeilao(ContextoDaProducao& contexto, int64_t lanceEmDecimos)
{
    LeilaoDeConstrucao& leilao = contexto.trabalho.leilao;
    if (!leilao.estaAberto()) {
        return "não há leilão aberto";
    }
    const int64_t minimo = calcularValorDaConstrucao(contexto, leilao.indiceDaConstrucao) * contexto.dados.regras.trabalho.lanceMinimoEmPorcentoDoValor / 100;
    if (lanceEmDecimos < minimo || lanceEmDecimos <= leilao.maiorLanceEmDecimos) {
        return "o lance precisa ser de pelo menos " + escreverDinheiro(std::max(minimo, leilao.maiorLanceEmDecimos + 1));
    }
    // O lance anterior do jogador volta; o novo fica preso até o fim.
    const int64_t devolvido = leilao.lanceEhDoJogador ? leilao.maiorLanceEmDecimos : 0;
    if (contexto.dinheiro.dinheiroDoJogador + devolvido < lanceEmDecimos) {
        return "falta dinheiro";
    }
    contexto.dinheiro.dinheiroDoJogador += devolvido - lanceEmDecimos;
    leilao.maiorLanceEmDecimos = lanceEmDecimos;
    leilao.lanceEhDoJogador = true;
    return "";
}
