#include "Producao/ProducaoDaVila.h"

#include "Vila/Trabalho.h"
#include "Clima/ClimaDaVila.h"
#include "Jogador/DecisoesDoJogador.h"
#include "Jogador/ObrasDaVila.h"

#include <algorithm>
#include <utility>

#include "Construcao/ContasDasConstrucoes.h"
#include "Economia/FelicidadeDaClasse.h"
#include "Economia/MercadoLocal.h"
#include "Eventos/EventosDaVila.h"
#include "Npc/ComponentesDoNpc.h"

namespace {

// No máximo 10 ciclos terminados por construção numa hora (regra 4.2, passo 4.4).
constexpr int ciclosPorHoraNoMaximo = 10;
// A folga (regra 10.2): com 125% do estoque-alvo, a receita deixa de ser candidata.
constexpr int estoqueDeSobraEmPorcentoDoAlvo = 125;
// A comida em perigo: menos de 3 dias da cesta mínima.
constexpr int diasDaComidaEmPerigo = 3;
// O consumo previsto das receitas conta um dia cheio: 8 horas por empregado.
constexpr int horasDeUmDiaCheioDeTrabalho = 8;

const DefinicaoDeReceita& receitaPeloIndice(const ContextoDaProducao& contexto, int indiceDaReceita)
{
    return contexto.dados.catalogoDeReceitas.todasAsReceitas()[indiceDaReceita];
}

std::string nomeDoItem(const ContextoDaProducao& contexto, int numeroDoItem)
{
    return contexto.dados.tabelaDeTextos.textoDe(contexto.dados.catalogoDeItens.itemPeloNumero(numeroDoItem).identificadorDeTexto);
}

bool construcaoEhDoJogador(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    return contexto.vila.construcoes[indiceDaConstrucao].donoEhOJogador;
}

DonoDoItem donoDaConstrucao(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    return construcaoEhDoJogador(contexto, indiceDaConstrucao) ? DonoDoItem::Jogador : DonoDoItem::Cidade;
}

// A receita que vale agora: a do ciclo, ou a escolhida para o próximo.
int receitaAtualDaConstrucao(const ProducaoDaConstrucao& producao)
{
    return producao.receitaDoCiclo >= 0 ? producao.receitaDoCiclo : producao.receitaEscolhidaParaOProximoCiclo;
}

// Os empregados de cada construção, montados uma vez no começo de um passo que não muda
// os empregos (a hora, a escolha das receitas, os gerentes). Sem isso, cada pergunta
// percorria todos os NPCs: nas outras cidades (fase B1b), com 300 NPCs e 200 construções,
// era a maior parte do tempo.
const std::vector<std::vector<entt::entity>>* empregadosJaMontados = nullptr;

class ListaDosEmpregadosDoPasso {
public:
    explicit ListaDosEmpregadosDoPasso(const ContextoDaProducao& contexto) : listaDeAntes(empregadosJaMontados)
    {
        std::vector<std::vector<std::pair<int, entt::entity>>> comNumero(contexto.vila.construcoes.size());
        for (const auto [npc, identidade, casaETrabalho] : contexto.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc>().each()) {
            if (casaETrabalho.indiceDoTrabalho >= 0 && casaETrabalho.indiceDoTrabalho < static_cast<int>(comNumero.size())) {
                comNumero[static_cast<std::size_t>(casaETrabalho.indiceDoTrabalho)].emplace_back(identidade.numeroDoNpc, npc);
            }
        }
        empregadosPorConstrucao.resize(comNumero.size());
        for (std::size_t construcao = 0; construcao < comNumero.size(); ++construcao) {
            std::sort(comNumero[construcao].begin(), comNumero[construcao].end(),
                      [](const auto& primeiro, const auto& segundo) { return primeiro.first < segundo.first; });
            for (const auto& [numero, npc] : comNumero[construcao]) {
                empregadosPorConstrucao[construcao].push_back(npc);
            }
        }
        empregadosJaMontados = &empregadosPorConstrucao;
    }
    ~ListaDosEmpregadosDoPasso() { empregadosJaMontados = listaDeAntes; }
    ListaDosEmpregadosDoPasso(const ListaDosEmpregadosDoPasso&) = delete;
    ListaDosEmpregadosDoPasso& operator=(const ListaDosEmpregadosDoPasso&) = delete;

private:
    std::vector<std::vector<entt::entity>> empregadosPorConstrucao;
    const std::vector<std::vector<entt::entity>>* listaDeAntes;
};

// Os empregados de uma construção, em ordem pelo número na vila.
std::vector<entt::entity> empregadosDaConstrucao(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    if (empregadosJaMontados != nullptr && indiceDaConstrucao >= 0 && indiceDaConstrucao < static_cast<int>(empregadosJaMontados->size())) {
        return (*empregadosJaMontados)[static_cast<std::size_t>(indiceDaConstrucao)];
    }
    std::vector<std::pair<int, entt::entity>> empregadosComNumero;
    for (const auto [npc, identidade, casaETrabalho] : contexto.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc>().each()) {
        if (casaETrabalho.indiceDoTrabalho == indiceDaConstrucao) {
            empregadosComNumero.emplace_back(identidade.numeroDoNpc, npc);
        }
    }
    std::sort(empregadosComNumero.begin(), empregadosComNumero.end(),
              [](const auto& primeiro, const auto& segundo) { return primeiro.first < segundo.first; });
    std::vector<entt::entity> empregados;
    for (const auto& [numero, npc] : empregadosComNumero) {
        empregados.push_back(npc);
    }
    return empregados;
}

// A área do mapa onde a receita Braçal tira o recurso, ou -1 (não é Braçal,
// ou o mapa não tem a área: a Pedra da fatia, FL6-5).
int areaDoRecursoDaReceita(const ContextoDaProducao& contexto, const DefinicaoDeReceita& receita)
{
    if (receita.trabalhoUsado != TrabalhoDaReceita::Bracal) {
        return -1;
    }
    const std::vector<DefinicaoDeRecurso>& recursos = contexto.dados.catalogoDeRecursos.todosOsRecursos();
    const std::vector<AreaDeRecurso>& areas = contexto.mapa.areasDeRecurso;
    for (std::size_t recurso = 0; recurso < recursos.size(); ++recurso) {
        if (recursos[recurso].numeroDoItemGerado != receita.numeroDoProduto || recursos[recurso].construcaoQueExtrai != receita.construcaoQueProduz) {
            continue;
        }
        for (std::size_t area = 0; area < areas.size(); ++area) {
            if (areas[area].numeroDoRecurso == static_cast<int>(recurso)) {
                return static_cast<int>(area);
            }
        }
    }
    return -1;
}

// Troca o campo dos Braçais quando a receita atual muda de área (regra 4.1).
void atualizarOCampoDeTrabalho(ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indiceDaConstrucao];
    const int receitaAtual = receitaAtualDaConstrucao(producao);
    const int areaNova = receitaAtual >= 0 ? areaDoRecursoDaReceita(contexto, receitaPeloIndice(contexto, receitaAtual)) : -1;
    if (areaNova == producao.areaDoCampoNoMapa) {
        return;
    }
    producao.areaDoCampoNoMapa = areaNova;
    producao.tilesDeTrabalhoNoCampo.clear();
    if (areaNova >= 0) {
        producao.tilesDeTrabalhoNoCampo =
            montarTilesDeTrabalhoDaArea(contexto.mapa, contexto.vila, contexto.mapa.areasDeRecurso[areaNova]);
    }
}

// Algum empregado consegue fazer a receita (habilidade ≥ a mínima)?
bool algumEmpregadoConsegueFazer(const ContextoDaProducao& contexto, int indiceDaConstrucao, const DefinicaoDeReceita& receita)
{
    for (const entt::entity empregado : empregadosDaConstrucao(contexto, indiceDaConstrucao)) {
        if (contexto.registroDasEntidades.get<HabilidadesDoNpc>(empregado).habilidadeNoTrabalho(receita.trabalhoUsado) >= receita.habilidadeMinima) {
            return true;
        }
    }
    return false;
}

// A receita pode rodar nesta construção: é dela, o nível dá, alguém sabe fazer
// e, se for Braçal, o mapa tem a área do recurso.
bool receitaPodeRodarNaConstrucao(const ContextoDaProducao& contexto, int indiceDaConstrucao, const DefinicaoDeReceita& receita)
{
    const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indiceDaConstrucao];
    return receita.construcaoQueProduz == construcao.tipo && construcao.nivel >= receita.nivelMinimoDaConstrucao
        && algumEmpregadoConsegueFazer(contexto, indiceDaConstrucao, receita)
        && (receita.trabalhoUsado != TrabalhoDaReceita::Bracal || areaDoRecursoDaReceita(contexto, receita) >= 0);
}

// Quanto o jogador gastaria comprando da Cidade o que falta dos insumos, ou
// -1 se a Cidade não tem o que falta.
int64_t custoDaCompraDosInsumosQueFaltam(const ContextoDaProducao& contexto, const DefinicaoDeReceita& receita)
{
    int64_t custo = 0;
    for (const InsumoDaReceita& insumo : receita.insumos) {
        const int falta = std::max(0, insumo.quantidade - contexto.inventario.quantidadeDe(insumo.numeroDoItem, DonoDoItem::Jogador));
        if (falta > contexto.inventario.quantidadeDe(insumo.numeroDoItem, DonoDoItem::Cidade)) {
            return -1;
        }
        custo += static_cast<int64_t>(falta) * calcularPrecoDaLojaDaCidade(precoLocalDoItem(contexto, insumo.numeroDoItem), contexto.dados.regras.economia);
    }
    return custo;
}

bool jogadorConsegueOsInsumos(const ContextoDaProducao& contexto, const DefinicaoDeReceita& receita)
{
    const int64_t custo = custoDaCompraDosInsumosQueFaltam(contexto, receita);
    return custo >= 0 && custo <= contexto.dinheiro.dinheiroDoJogador;
}

// Regra 4.2, passo 2: tira os insumos (tudo ou nada). O jogador compra da
// Cidade o que faltar (FL6-3). Devolve o motivo da parada, ou vazio se deu certo.
std::string tentarTirarOsInsumos(ContextoDaProducao& contexto, int indiceDaConstrucao, const DefinicaoDeReceita& receita)
{
    const DonoDoItem dono = donoDaConstrucao(contexto, indiceDaConstrucao);
    if (dono == DonoDoItem::Jogador && jogadorConsegueOsInsumos(contexto, receita)) {
        const int64_t custo = custoDaCompraDosInsumosQueFaltam(contexto, receita);
        for (const InsumoDaReceita& insumo : receita.insumos) {
            const int falta = std::max(0, insumo.quantidade - contexto.inventario.quantidadeDe(insumo.numeroDoItem, DonoDoItem::Jogador));
            contexto.inventario.passarParaOutroDono(insumo.numeroDoItem, DonoDoItem::Cidade, DonoDoItem::Jogador, falta);
        }
        contexto.dinheiro.dinheiroDoJogador -= custo;
        contexto.dinheiro.bancoDaCidade += custo;
        contexto.dinheiro.gastoDoJogadorComInsumosHoje += custo;
    }
    if (contexto.inventario.tentarTirarVarios(receita.insumos, dono)) {
        for (const InsumoDaReceita& insumo : receita.insumos) {
            contexto.consumoMedido.anotarOQueSaiu(insumo.numeroDoItem, insumo.quantidade);  // regra 10.1, passo 1
        }
        return "";
    }
    for (const InsumoDaReceita& insumo : receita.insumos) {
        if (contexto.inventario.quantidadeDe(insumo.numeroDoItem, dono) < insumo.quantidade) {
            return "sem " + nomeDoItem(contexto, insumo.numeroDoItem);
        }
    }
    return "sem insumos";
}

// Os insumos que faltam de verdade para a receita (não só o dinheiro): na
// Cidade, o que ela não tem; no jogador, o que nem ele nem a Cidade têm.
std::vector<InsumoDaReceita> insumosQueFaltam(const ContextoDaProducao& contexto, int indiceDaConstrucao, const DefinicaoDeReceita& receita)
{
    const bool ehDoJogador = construcaoEhDoJogador(contexto, indiceDaConstrucao);
    std::vector<InsumoDaReceita> faltam;
    for (const InsumoDaReceita& insumo : receita.insumos) {
        int quantidadeQueExiste = contexto.inventario.quantidadeDe(insumo.numeroDoItem, ehDoJogador ? DonoDoItem::Jogador : DonoDoItem::Cidade);
        quantidadeQueExiste += ehDoJogador ? contexto.inventario.quantidadeDe(insumo.numeroDoItem, DonoDoItem::Cidade) : 0;
        if (quantidadeQueExiste < insumo.quantidade) {
            faltam.push_back(insumo);
        }
    }
    return faltam;
}

// Regra 10.1, passo 2: a hora parada por falta de insumo, com gente
// trabalhando, conta como demanda que faltou.
void anotarADemandaDosInsumosQueFaltam(ContextoDaProducao& contexto, const std::vector<InsumoDaReceita>& faltam, const DefinicaoDeReceita& receita,
                                       int ticksDaHora)
{
    for (const InsumoDaReceita& insumo : faltam) {
        contexto.consumoMedido.anotarADemandaQueFaltou(
            insumo.numeroDoItem, calcularDemandaQueFaltouNaHora(insumo.quantidade, ticksDaHora, receita.horasPorCiclo, contexto.ticksPorHoraDeJogo));
    }
}

// A ordem do jogador quer produzir agora (regra 4.4, passo 1)?
bool ordemQuerProduzir(const ContextoDaProducao& contexto, const OrdemDoJogador& ordem)
{
    if (ordem.tipo == TipoDeOrdem::ManterEstoque) {
        return contexto.inventario.quantidadeDe(ordem.numeroDoItem, DonoDoItem::Jogador) < ordem.quantidade;
    }
    if (ordem.tipo == TipoDeOrdem::FazerQuantidade) {
        return ordem.quantidadeJaFeita < ordem.quantidade;
    }
    return true;
}

// A fila do jogador não deu receita (regra 10.1, passo 3): se alguma ordem
// quer produzir e só falta insumo, a parada é "sem Tábua" (a primeira dessas
// ordens), e cada uma conta a demanda dos insumos que faltam a ela.
std::string motivoDaFilaSemReceita(ContextoDaProducao& contexto, int indiceDaConstrucao, int ticksDaHora)
{
    std::string motivo = "sem ordem";
    for (const OrdemDoJogador& ordem : contexto.producaoDasConstrucoes[indiceDaConstrucao].ordens) {
        const std::optional<int> indiceDaReceita = contexto.dados.catalogoDeReceitas.procurarReceitaDoProduto(ordem.numeroDoItem);
        if (!ordemQuerProduzir(contexto, ordem) || !indiceDaReceita.has_value()) {
            continue;
        }
        const DefinicaoDeReceita& receita = receitaPeloIndice(contexto, *indiceDaReceita);
        if (!receitaPodeRodarNaConstrucao(contexto, indiceDaConstrucao, receita)) {
            continue;
        }
        const std::vector<InsumoDaReceita> faltam = insumosQueFaltam(contexto, indiceDaConstrucao, receita);
        if (motivo == "sem ordem") {
            motivo = faltam.empty() ? "sem dinheiro para os insumos" : "sem " + nomeDoItem(contexto, faltam.front().numeroDoItem);
        }
        if (ticksDaHora > 0) {
            anotarADemandaDosInsumosQueFaltam(contexto, faltam, receita, ticksDaHora);
        }
    }
    return motivo;
}

// O fator da estação e da produtividade da área, em % (ficha dos Recursos).
int fatorDaProducaoBracalEmPorcento(const ContextoDaProducao& contexto, const ProducaoDaConstrucao& producao, Estacao estacao)
{
    if (producao.areaDoCampoNoMapa < 0) {
        return 100;
    }
    const AreaDeRecurso& area = contexto.mapa.areasDeRecurso[producao.areaDoCampoNoMapa];
    const DefinicaoDeRecurso& recurso = contexto.dados.catalogoDeRecursos.todosOsRecursos()[area.numeroDoRecurso];
    const auto grupo = contexto.dados.regras.producaoPorEstacaoEmPorcento.find(recurso.grupoDaEstacao);
    const int producaoDaEstacao = grupo == contexto.dados.regras.producaoPorEstacaoEmPorcento.end() ? 100 : grupo->second[static_cast<int>(estacao)];
    return area.produtividadeEmPorcento * producaoDaEstacao / 100;
}

// O trabalho de todos os empregados na última hora (regra 4.2, passo 3).
int64_t somarOTrabalhoDaHora(const ContextoDaProducao& contexto, int indiceDaConstrucao, const DefinicaoDeReceita& receita)
{
    const int nivel = contexto.vila.construcoes[indiceDaConstrucao].nivel;
    int64_t trabalhoDaHora = 0;
    for (const entt::entity empregado : empregadosDaConstrucao(contexto, indiceDaConstrucao)) {
        const int ticks = contexto.registroDasEntidades.get<TrabalhoDoDia>(empregado).ticksTrabalhadosNestaHora;
        const int habilidade = contexto.registroDasEntidades.get<HabilidadesDoNpc>(empregado).habilidadeNoTrabalho(receita.trabalhoUsado);
        trabalhoDaHora += calcularProgressoDoTrabalho(ticks, habilidade, nivel, receita.nivelMinimoDaConstrucao);
    }
    // Marco 8: o nível de salário (FL8-7) e a manutenção atrasada (FL8-5) mudam a velocidade.
    const ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indiceDaConstrucao];
    const int velocidadeDoSalario = contexto.vila.construcoes[indiceDaConstrucao].donoEhOJogador
        ? velocidadeDoNivelEmPorcento(producao.nivelDeSalario, contexto.dados.regras.economia.decisoesDoJogador) : 100;
    const int velocidadeDaManutencao = producao.manutencaoAtrasada ? contexto.dados.regras.construcoes.producaoComManutencaoAtrasadaEmPorcento : 100;
    return trabalhoDaHora * velocidadeDoSalario * velocidadeDaManutencao / 10000;
}

// Fase B5 (Tecnologia, TE1-2): o Laboratório trabalha como uma receita de 8 horas, lote 1,
// sem insumo, e cada ciclo completo vira 1 Ponto de Pesquisa (não vai ao inventário).
void juntarOsPontosDePesquisaDaHora(ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indiceDaConstrucao];
    const DefinicaoDeConstrucao* definicao = contexto.dados.catalogoDeConstrucoes.procurarConstrucao(construcao.tipo);
    ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indiceDaConstrucao];
    if (definicao == nullptr || !definicao->produzPontosDePesquisa || construcao.emObra || producao.estaDeFolgaHoje) {
        return;
    }
    for (const entt::entity empregado : empregadosDaConstrucao(contexto, indiceDaConstrucao)) {
        const int ticks = contexto.registroDasEntidades.get<TrabalhoDoDia>(empregado).ticksTrabalhadosNestaHora;
        const int habilidade = contexto.registroDasEntidades.get<HabilidadesDoNpc>(empregado).habilidadeNoTrabalho(*definicao->trabalhoSemReceita);
        producao.progressoDoCiclo += calcularProgressoDoTrabalho(ticks, habilidade, construcao.nivel, 1);
    }
    const int64_t porPonto = static_cast<int64_t>(contexto.dados.tecnologia.regras.horasPorPonto) * contexto.ticksPorHoraDeJogo * 100;
    if (porPonto > 0 && producao.progressoDoCiclo >= porPonto) {
        producao.pontosDePesquisaGuardados += static_cast<int>(producao.progressoDoCiclo / porPonto);
        producao.progressoDoCiclo %= porPonto;
    }
}

void pararAConstrucao(ProducaoDaConstrucao& producao, const std::string& motivo)
{
    producao.estaParada = true;
    producao.motivoDaParada = motivo;
}

// Regra 4.2 inteira para uma construção de produção.
void rodarOCicloDeUmaConstrucao(ContextoDaProducao& contexto, int indiceDaConstrucao, Estacao estacao)
{
    ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indiceDaConstrucao];
    const bool ehDoJogador = construcaoEhDoJogador(contexto, indiceDaConstrucao);
    producao.estaParada = false;
    producao.motivoDaParada.clear();

    int ticksDaHora = 0;
    for (const entt::entity empregado : empregadosDaConstrucao(contexto, indiceDaConstrucao)) {
        ticksDaHora += contexto.registroDasEntidades.get<TrabalhoDoDia>(empregado).ticksTrabalhadosNestaHora;
    }

    // Regra 10.2: de folga, os empregados nem vêm.
    if (producao.estaDeFolgaHoje) {
        pararAConstrucao(producao, "de folga");
        return;
    }

    for (int rodada = 0; rodada < ciclosPorHoraNoMaximo; ++rodada) {
        // Passo 1: sem receita, pega a próxima (a do jogador, pela fila de ordens).
        if (producao.receitaDoCiclo < 0) {
            producao.receitaDoCiclo = construcaoSegueAFilaDoJogador(contexto, indiceDaConstrucao) ? escolherReceitaPelaFilaDeOrdens(contexto, indiceDaConstrucao)
                                                  : producao.receitaEscolhidaParaOProximoCiclo;
            atualizarOCampoDeTrabalho(contexto, indiceDaConstrucao);
            if (producao.receitaDoCiclo < 0) {
                pararAConstrucao(producao, construcaoSegueAFilaDoJogador(contexto, indiceDaConstrucao) ? motivoDaFilaSemReceita(contexto, indiceDaConstrucao, ticksDaHora) : "sem receita");
                return;
            }
        }
        const DefinicaoDeReceita& receita = receitaPeloIndice(contexto, producao.receitaDoCiclo);

        // Passo 2: o ciclo começa tirando os insumos. Sem ninguém trabalhando
        // (de noite), ele não começa: os insumos só saem quando há trabalho.
        if (!producao.cicloComecou) {
            if (ticksDaHora == 0 && rodada == 0) {
                return;
            }
            const std::string motivoDaFalta = tentarTirarOsInsumos(contexto, indiceDaConstrucao, receita);
            if (!motivoDaFalta.empty()) {
                anotarADemandaDosInsumosQueFaltam(contexto, insumosQueFaltam(contexto, indiceDaConstrucao, receita), receita, ticksDaHora);
                pararAConstrucao(producao, motivoDaFalta);
                return;
            }
            producao.cicloComecou = true;
        }

        // Passo 3: o trabalho da hora entra uma vez só.
        if (rodada == 0) {
            producao.progressoDoCiclo += somarOTrabalhoDaHora(contexto, indiceDaConstrucao, receita);
        }

        // Passo 4: o ciclo terminou?
        const int64_t progressoNecessario = progressoNecessarioDoCiclo(receita, contexto.ticksPorHoraDeJogo);
        if (producao.progressoDoCiclo < progressoNecessario) {
            return;
        }
        int quantidade = receita.loteProduzido;
        int restoNovo = producao.restoDaProducaoBracal;
        const int multiplicadorDosEventos =
            multiplicadorDaProducaoPorEventos(contexto, contexto.vila.construcoes[indiceDaConstrucao].tipo);  // a Seca: 50%
        if (receita.trabalhoUsado == TrabalhoDaReceita::Bracal || multiplicadorDosEventos != 100) {
            // O lote × o fator da estação e da produtividade (só nas Braçais)
            // × os eventos ativos, com a fração guardada.
            const int fatorDoLote = receita.trabalhoUsado == TrabalhoDaReceita::Bracal ? fatorDaProducaoBracalEmPorcento(contexto, producao, estacao) : 100;
            // Ficha do Clima, regra 4.7: × o clima do dia (a tempestade: 50% ao ar livre).
            const int climaDoDia = receita.trabalhoUsado == TrabalhoDaReceita::Bracal
                ? porcentagemDoTrabalhoAoArLivre(contexto.clima.climaDeHoje, contexto.dados.regrasDoClima) : 100;
            const int totalEmCentesimos =
                receita.loteProduzido * fatorDoLote * multiplicadorDosEventos / 100 * climaDoDia / 100 + producao.restoDaProducaoBracal;
            quantidade = totalEmCentesimos / 100;
            restoNovo = totalEmCentesimos % 100;
        }
        if (!contexto.inventario.tentarPor(receita.numeroDoProduto, donoDaConstrucao(contexto, indiceDaConstrucao), quantidade)) {
            pararAConstrucao(producao, "o estoque encheu");
            return;
        }
        producao.restoDaProducaoBracal = restoNovo;
        producao.producaoDeHojePorItem[receita.numeroDoProduto] += quantidade;
        if (ehDoJogador) {
            // Uma ordem "fazer N" conta o que já fez e sai da lista quando chega a N.
            for (std::size_t ordem = 0; ordem < producao.ordens.size(); ++ordem) {
                OrdemDoJogador& ordemDoJogador = producao.ordens[ordem];
                if (ordemDoJogador.tipo == TipoDeOrdem::FazerQuantidade && ordemDoJogador.numeroDoItem == receita.numeroDoProduto) {
                    ordemDoJogador.quantidadeJaFeita += quantidade;
                    if (ordemDoJogador.quantidadeJaFeita >= ordemDoJogador.quantidade) {
                        producao.ordens.erase(producao.ordens.begin() + static_cast<std::ptrdiff_t>(ordem));
                    }
                    break;
                }
            }
        }
        producao.progressoDoCiclo -= progressoNecessario;
        producao.cicloComecou = false;
        producao.receitaDoCiclo = construcaoSegueAFilaDoJogador(contexto, indiceDaConstrucao) ? -1 : producao.receitaEscolhidaParaOProximoCiclo;
        atualizarOCampoDeTrabalho(contexto, indiceDaConstrucao);
    }
}

}  // namespace

std::vector<int64_t> preverOConsumoPelasReceitasAtuais(const ContextoDaProducao& contexto)
{
    const CatalogoDeItens& itens = contexto.dados.catalogoDeItens;
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;
    std::vector<int64_t> consumoEmCentesimos(itens.quantidadeDeItens(), 0);

    // A cesta mínima: a comida e o Material Básico mais baratos (os que a vila come).
    int comidaMaisBarata = -1;
    int materialMaisBarato = -1;
    for (const DefinicaoDeItem& item : itens.todosOsItens()) {
        if (!item.ehComida() && !item.ehMaterialBasico()) {
            continue;
        }
        int& maisBarato = item.ehComida() ? comidaMaisBarata : materialMaisBarato;
        if (maisBarato < 0 || item.precoBaseEmDecimos < itens.itemPeloNumero(maisBarato).precoBaseEmDecimos) {
            maisBarato = item.numeroInterno;
        }
    }
    const int64_t civis = contexto.dinheiro.quantidadeDeCivis;
    if (comidaMaisBarata >= 0) {
        consumoEmCentesimos[comidaMaisBarata] += civis * economia.comidaPorDiaDoCivil * 100;
    }
    if (materialMaisBarato >= 0) {
        consumoEmCentesimos[materialMaisBarato] += civis * 100 / economia.diasEntreMateriaisDoCivil;
    }

    // Os insumos das receitas atuais, num dia cheio (8 horas por empregado).
    for (std::size_t construcao = 0; construcao < contexto.producaoDasConstrucoes.size(); ++construcao) {
        const ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[construcao];
        if (!producao.ehDeProducao) {
            continue;
        }
        int receitaAtual = receitaAtualDaConstrucao(producao);
        if (receitaAtual < 0 && construcaoEhDoJogador(contexto, static_cast<int>(construcao))) {
            receitaAtual = escolherReceitaPelaFilaDeOrdens(contexto, static_cast<int>(construcao));
        }
        if (receitaAtual < 0) {
            continue;
        }
        const DefinicaoDeReceita& receita = receitaPeloIndice(contexto, receitaAtual);
        const int64_t empregados = static_cast<int64_t>(empregadosDaConstrucao(contexto, static_cast<int>(construcao)).size());
        for (const InsumoDaReceita& insumo : receita.insumos) {
            consumoEmCentesimos[insumo.numeroDoItem] += insumo.quantidade * empregados * horasDeUmDiaCheioDeTrabalho * 100 / receita.horasPorCiclo;
        }
    }
    return consumoEmCentesimos;
}

int HabilidadesDoNpc::habilidadeNoTrabalho(TrabalhoDaReceita trabalho) const
{
    switch (trabalho) {
    case TrabalhoDaReceita::Bracal: return bracal;
    case TrabalhoDaReceita::Manufaturado: return manufaturado;
    case TrabalhoDaReceita::Especialista: return manufaturado;  // TE2-1, provisório
    }
    return 0;
}

int64_t progressoNecessarioDoCiclo(const DefinicaoDeReceita& receita, int ticksPorHoraDeJogo)
{
    return static_cast<int64_t>(receita.horasPorCiclo) * ticksPorHoraDeJogo * 100;
}

int64_t calcularProgressoDoTrabalho(int ticksTrabalhados, int habilidade, int nivelDaConstrucao, int nivelMinimoDaReceita)
{
    const int64_t velocidadeEmPorcento = 50 + habilidade;
    const int64_t bonusDeNivelEmPorcento = 100 + 10 * std::max(0, nivelDaConstrucao - nivelMinimoDaReceita);
    return ticksTrabalhados * velocidadeEmPorcento * bonusDeNivelEmPorcento / 100;
}

int calcularDeficitDoItem(int estoque, int64_t estoqueAlvoEmCentesimos)
{
    if (estoqueAlvoEmCentesimos <= 0) {
        return 0;
    }
    const int64_t deficit = 100 - static_cast<int64_t>(estoque) * 100 * 100 / estoqueAlvoEmCentesimos;
    return static_cast<int>(std::clamp<int64_t>(deficit, 0, 100));
}

int64_t calcularNotaDaReceita(int precoDoProdutoEmDecimos, int custoDosInsumosEmDecimos, int deficitEmPorcento)
{
    return static_cast<int64_t>(precoDoProdutoEmDecimos - custoDosInsumosEmDecimos) * (100 + deficitEmPorcento);
}

int calcularPrecoDaLojaDaCidade(int precoEmDecimos, const RegrasDaEconomia& regras)
{
    return static_cast<int>(arredondarMetadeParaCima(static_cast<long long>(precoEmDecimos) * regras.lojaDaCidadeVendeEmPorcento, 100));
}

int64_t calcularDemandaQueFaltouNaHora(int quantidadeDoInsumo, int ticksTrabalhadosNaHora, int horasDaReceita, int ticksPorHoraDeJogo)
{
    return static_cast<int64_t>(quantidadeDoInsumo) * ticksTrabalhadosNaHora * 100 / (static_cast<int64_t>(horasDaReceita) * ticksPorHoraDeJogo);
}

int precoLocalDoItem(const ContextoDaProducao& contexto, int numeroDoItem)
{
    const std::vector<int>& precos = contexto.mercadoLocal.precoLocalPorItemEmDecimos;
    if (numeroDoItem >= 0 && numeroDoItem < static_cast<int>(precos.size())) {
        return precos[numeroDoItem];
    }
    return contexto.dados.catalogoDeItens.itemPeloNumero(numeroDoItem).precoBaseEmDecimos;
}

int64_t estoqueAlvoDoItemEmCentesimos(const ContextoDaProducao& contexto, int numeroDoItem)
{
    // Ficha do Clima, regra 4.6: nos últimos dias do Outono, a Tora e o Grão ganham mais alvo.
    return contexto.consumoMedido.consumoPrevistoDoDiaEmCentesimos(numeroDoItem) * contexto.dados.regras.economia.mercadoLocal.diasDoEstoqueAlvo
        + bonusDoEstoqueParaOInvernoEmCentesimos(contexto, numeroDoItem);
}

std::vector<PosicaoDeTile> montarTilesDeTrabalhoDaArea(const MapaLocal& mapa, const VilaDoJogo& vila, const AreaDeRecurso& area)
{
    std::vector<PosicaoDeTile> tilesLivres;
    const RetanguloEmTiles& retangulo = area.areaEmTiles;
    for (int tileY = retangulo.tileY; tileY < retangulo.tileY + retangulo.alturaEmTiles; ++tileY) {
        for (int tileX = retangulo.tileX; tileX < retangulo.tileX + retangulo.larguraEmTiles; ++tileX) {
            const bool ehPorta = std::any_of(vila.construcoes.begin(), vila.construcoes.end(), [&](const ConstrucaoDaVila& construcao) {
                return construcao.tileDaPorta == PosicaoDeTile{tileX, tileY};
            });
            if (mapa.tileDaParaAndar(tileX, tileY) && !ehPorta) {
                tilesLivres.push_back({tileX, tileY});
            }
        }
    }
    return tilesLivres;
}

std::vector<std::string> montarAProducaoDaVila(ContextoDaProducao& contexto, const std::vector<entt::entity>& npcsDaVila)
{
    std::vector<std::string> avisos;
    const DadosDoJogo& dados = contexto.dados;
    const VilaDoJogo& vila = contexto.vila;
    entt::registry& registro = contexto.registroDasEntidades;

    // Os componentes do trabalho em cada NPC, na ordem da vila.
    for (std::size_t numero = 0; numero < npcsDaVila.size() && numero < vila.npcsIniciais.size(); ++numero) {
        const NpcDaVilaInicial& npcInicial = vila.npcsIniciais[numero];
        registro.emplace_or_replace<HabilidadesDoNpc>(npcsDaVila[numero], HabilidadesDoNpc{npcInicial.habilidadeBracal, npcInicial.habilidadeManufaturado});
        registro.emplace_or_replace<TrabalhoDoDia>(npcsDaVila[numero]);
    }

    // O inventário: a capacidade das construções e o estoque do primeiro dia.
    contexto.inventario = InventarioDaCidade(dados.catalogoDeItens);
    recalcularCapacidade(contexto.inventario, vila, dados.catalogoDeConstrucoes, dados.regras.construcoes);
    for (const ItemDoEstoqueInicial& item : vila.estoqueInicial) {
        if (!contexto.inventario.tentarPor(item.numeroDoItem, item.dono, item.quantidade)) {
            avisos.push_back("o estoque inicial de " + nomeDoItem(contexto, item.numeroDoItem) + " não coube na capacidade da vila");
        }
    }

    // O dinheiro do primeiro dia (FL6-4).
    DinheiroDaVila dinheiro;
    dinheiro.bancoDaCidade = vila.bancoDaCidadeEmDecimos;
    dinheiro.bancoNoComecoDaEstacao = vila.bancoDaCidadeEmDecimos;  // o estabilizador (marco 7)
    dinheiro.dinheiroDoJogador = vila.dinheiroDoJogadorEmDecimos;
    for (const NpcDaVilaInicial& npc : vila.npcsIniciais) {
        dinheiro.quantidadeDeCivis += npc.classe == "civil" ? 1 : 0;
    }
    dinheiro.dinheiroDaClasseCivil = static_cast<int64_t>(dinheiro.quantidadeDeCivis) * vila.dinheiroInicialPorCivilEmDecimos;
    contexto.dinheiro = dinheiro;

    // A produção de cada construção.
    contexto.producaoDasConstrucoes.assign(vila.construcoes.size(), ProducaoDaConstrucao{});
    for (std::size_t indice = 0; indice < vila.construcoes.size(); ++indice) {
        ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indice];
        for (const DefinicaoDeReceita& receita : dados.catalogoDeReceitas.todasAsReceitas()) {
            producao.ehDeProducao = producao.ehDeProducao || receita.construcaoQueProduz == vila.construcoes[indice].tipo;
        }
        producao.ordens = vila.construcoes[indice].ordensDoJogador;
    }

    // O marco 7: os preços locais começam no base, a Felicidade no inicial,
    // e os eventos sem nenhum ativo.
    comecarOMercadoLocal(contexto);
    contexto.felicidade = FelicidadeDaClasse{};
    contexto.felicidade.felicidadeEmDecimos = 10 * dados.regras.felicidade.felicidadeInicial;
    contexto.felicidade.alvoEmDecimos = contexto.felicidade.felicidadeEmDecimos;
    comecarOsEventos(contexto);

    // A primeira escolha (regra 4.3: "ao montar a vila, a escolha roda uma
    // vez"). Os 5 dias guardados do consumo começam com a previsão do marco 6
    // (regra 10.1), que precisa das receitas atuais: a escolha roda uma vez
    // só com a cesta e a fila do jogador, a previsão é refeita com as
    // receitas escolhidas, e a escolha roda de novo, valendo.
    contexto.consumoMedido = ConsumoMedidoDaVila(dados.catalogoDeItens.quantidadeDeItens());
    for (int rodadaDaPrimeiraEscolha = 0; rodadaDaPrimeiraEscolha < 2; ++rodadaDaPrimeiraEscolha) {
        contexto.consumoMedido.preencherOsDiasGuardados(preverOConsumoPelasReceitasAtuais(contexto));
        std::fill(contexto.consumoMedido.produtoEstaComEstoqueDeSobra.begin(), contexto.consumoMedido.produtoEstaComEstoqueDeSobra.end(), false);
        escolherAsReceitasDaCidade(contexto);
        for (std::size_t indice = 0; indice < vila.construcoes.size(); ++indice) {
            ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indice];
            producao.estaDeFolgaHoje = false;  // o primeiro dia é de trabalho para todos
            if (!producao.ehDeProducao) {
                continue;
            }
            producao.receitaDoCiclo = construcaoEhDoJogador(contexto, static_cast<int>(indice))
                                          ? escolherReceitaPelaFilaDeOrdens(contexto, static_cast<int>(indice))
                                          : producao.receitaEscolhidaParaOProximoCiclo;
            atualizarOCampoDeTrabalho(contexto, static_cast<int>(indice));
        }
    }
    escolherOsGerentes(contexto);
    return avisos;
}

bool npcEstaTrabalhandoAgora(const ContextoDaProducao& contexto, entt::entity npc)
{
    const entt::registry& registro = contexto.registroDasEntidades;
    const int indiceDoTrabalho = registro.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
    const DecisaoDoNpc& decisao = registro.get<DecisaoDoNpc>(npc);
    if (indiceDoTrabalho < 0 || (decisao.acaoAtual != AcaoDoNpc::Trabalhar && decisao.acaoAtual != AcaoDoNpc::Comer)) {
        return false;
    }
    const std::vector<PosicaoDeTile>& tilesDoCampo = contexto.producaoDasConstrucoes[indiceDoTrabalho].tilesDeTrabalhoNoCampo;
    if (tilesDoCampo.empty()) {
        return decisao.indiceDaConstrucaoOndeEsta == indiceDoTrabalho;
    }
    // O Braçal: parado num tile do campo, sem estar a caminho.
    const LugarDoNpc& lugar = registro.get<PosicaoDoNpc>(npc).lugarAtual;
    return !decisao.estaDentroDeUmaConstrucao() && !decisao.estaACaminho && lugar.estaParado()
        && std::find(tilesDoCampo.begin(), tilesDoCampo.end(), lugar.tileAtual) != tilesDoCampo.end();
}

void contarOsTicksTrabalhados(ContextoDaProducao& contexto)
{
    for (const auto [npc, trabalhoDoDia] : contexto.registroDasEntidades.view<TrabalhoDoDia>().each()) {
        if (npcEstaTrabalhandoAgora(contexto, npc)) {
            ++trabalhoDoDia.ticksTrabalhadosNestaHora;
            ++trabalhoDoDia.ticksTrabalhadosHoje;
        }
    }
}

void rodarOsCiclosDaHora(ContextoDaProducao& contexto, const RelogioDoMundo& relogio)
{
    andarAsObrasDaHora(contexto);
    const ListaDosEmpregadosDoPasso empregadosDoPasso(contexto);
    for (std::size_t indice = 0; indice < contexto.producaoDasConstrucoes.size(); ++indice) {
        if (contexto.producaoDasConstrucoes[indice].ehDeProducao) {
            rodarOCicloDeUmaConstrucao(contexto, static_cast<int>(indice), relogio.estacaoAtual());
        } else {
            juntarOsPontosDePesquisaDaHora(contexto, static_cast<int>(indice));
        }
    }
    // Depois zera o trabalho da hora de todos (regra 4.2, passo 3).
    for (const auto [npc, trabalhoDoDia] : contexto.registroDasEntidades.view<TrabalhoDoDia>().each()) {
        trabalhoDoDia.ticksTrabalhadosNestaHora = 0;
    }
}

void escolherAsReceitasDaCidade(ContextoDaProducao& contexto)
{
    const ListaDosEmpregadosDoPasso empregadosDoPasso(contexto);
    const CatalogoDeItens& itens = contexto.dados.catalogoDeItens;
    const std::vector<DefinicaoDeReceita>& receitas = contexto.dados.catalogoDeReceitas.todasAsReceitas();

    // Regra 10.2: a histerese do estoque de sobra de cada item. Passou de
    // 125% do alvo, sai; só volta quando o estoque cai abaixo do alvo.
    std::vector<bool>& comSobra = contexto.consumoMedido.produtoEstaComEstoqueDeSobra;
    comSobra.resize(itens.quantidadeDeItens(), false);
    for (const DefinicaoDeItem& item : itens.todosOsItens()) {
        const int64_t estoqueEmCentesimos = static_cast<int64_t>(contexto.inventario.quantidadeTotalDe(item.numeroInterno)) * 100;
        const int64_t alvoEmCentesimos = estoqueAlvoDoItemEmCentesimos(contexto, item.numeroInterno);
        if (comSobra[item.numeroInterno]) {
            comSobra[item.numeroInterno] = estoqueEmCentesimos >= alvoEmCentesimos;
        } else {
            comSobra[item.numeroInterno] = estoqueEmCentesimos * 100 >= alvoEmCentesimos * estoqueDeSobraEmPorcentoDoAlvo;
        }
    }

    // Passo 3: a comida em perigo (menos de 3 dias da cesta mínima).
    int comidaNoEstoque = 0;
    for (const DefinicaoDeItem& item : itens.todosOsItens()) {
        comidaNoEstoque += item.ehComida() ? contexto.inventario.quantidadeTotalDe(item.numeroInterno) : 0;
    }
    const int comidaDeTresDias = contexto.dinheiro.quantidadeDeCivis * contexto.dados.regras.economia.comidaPorDiaDoCivil * diasDaComidaEmPerigo;
    const bool comidaEmPerigo = comidaNoEstoque < comidaDeTresDias;

    for (std::size_t construcao = 0; construcao < contexto.producaoDasConstrucoes.size(); ++construcao) {
        ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[construcao];
        if (!producao.ehDeProducao) {
            continue;
        }
        if (construcaoSegueAFilaDoJogador(contexto, static_cast<int>(construcao))) {
            // Regra 10.2, passo 4: sem nenhuma ordem que queira produzir, folga.
            // O ciclo que ainda não começou volta para a fila decidir de novo.
            producao.estaDeFolgaHoje = std::none_of(producao.ordens.begin(), producao.ordens.end(),
                                                    [&](const OrdemDoJogador& ordem) { return ordemQuerProduzir(contexto, ordem); });
            if (!producao.cicloComecou) {
                producao.receitaDoCiclo = -1;
            }
            continue;
        }
        int melhorReceita = -1;
        bool melhorEhComida = false;
        int64_t melhorNota = 0;
        for (std::size_t indiceDaReceita = 0; indiceDaReceita < receitas.size(); ++indiceDaReceita) {
            const DefinicaoDeReceita& receita = receitas[indiceDaReceita];
            // Passo 1: as candidatas; com a regra 10.2, sem as de estoque de sobra.
            const bool travada = (contexto.receitasLiberadas != nullptr && indiceDaReceita < contexto.receitasLiberadas->size() && !(*contexto.receitasLiberadas)[indiceDaReceita])
                || (contexto.itensProibidos != nullptr && (*contexto.itensProibidos)[static_cast<std::size_t>(receita.numeroDoProduto)]);
            if (travada || !receitaPodeRodarNaConstrucao(contexto, static_cast<int>(construcao), receita) || comSobra[receita.numeroDoProduto]) {
                continue;
            }
            // Passo 2: a nota, com os preços locais (marco 7).
            int custoDosInsumos = 0;
            for (const InsumoDaReceita& insumo : receita.insumos) {
                custoDosInsumos += insumo.quantidade * precoLocalDoItem(contexto, insumo.numeroDoItem);
            }
            const int estoque = contexto.inventario.quantidadeTotalDe(receita.numeroDoProduto);
            const int deficit = calcularDeficitDoItem(estoque, estoqueAlvoDoItemEmCentesimos(contexto, receita.numeroDoProduto));
            const int64_t nota = calcularNotaDaReceita(precoLocalDoItem(contexto, receita.numeroDoProduto), custoDosInsumos, deficit);
            const bool ehComida = itens.itemPeloNumero(receita.numeroDoProduto).ehComida();
            // Passos 3 e 4: com a comida em perigo, a comida ganha sempre; se
            // não, a maior nota; no empate, a primeira do receitas.csv.
            const bool ganha = melhorReceita < 0 || (comidaEmPerigo && ehComida && !melhorEhComida)
                            || ((!comidaEmPerigo || ehComida == melhorEhComida) && nota > melhorNota);
            if (ganha) {
                melhorReceita = static_cast<int>(indiceDaReceita);
                melhorEhComida = ehComida;
                melhorNota = nota;
            }
        }
        producao.receitaEscolhidaParaOProximoCiclo = melhorReceita;
        // Regra 10.2, passo 3: sem nenhuma candidata, folga no dia seguinte.
        producao.estaDeFolgaHoje = melhorReceita < 0;
        // O ciclo que ainda não começou já é o próximo: ele usa a escolha nova.
        if (!producao.cicloComecou) {
            producao.receitaDoCiclo = melhorReceita;
            atualizarOCampoDeTrabalho(contexto, static_cast<int>(construcao));
        }
    }
}

int escolherReceitaPelaFilaDeOrdens(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    const ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indiceDaConstrucao];
    for (const OrdemDoJogador& ordem : producao.ordens) {
        // Passo 1: a ordem quer produzir agora?
        if (!ordemQuerProduzir(contexto, ordem)) {
            continue;
        }
        // Passo 2: a receita está liberada, alguém sabe fazer, e os insumos existem.
        const std::optional<int> receita = contexto.dados.catalogoDeReceitas.procurarReceitaDoProduto(ordem.numeroDoItem);
        if (!receita.has_value()) {
            continue;
        }
        const DefinicaoDeReceita& definicao = receitaPeloIndice(contexto, *receita);
        if (receitaPodeRodarNaConstrucao(contexto, indiceDaConstrucao, definicao) && jogadorConsegueOsInsumos(contexto, definicao)) {
            return *receita;  // passo 3: a primeira que passa
        }
    }
    return -1;
}

TrabalhoDaReceita trabalhoDaConstrucao(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    const int receitaAtual = receitaAtualDaConstrucao(contexto.producaoDasConstrucoes[indiceDaConstrucao]);
    if (receitaAtual >= 0) {
        return receitaPeloIndice(contexto, receitaAtual).trabalhoUsado;
    }
    const std::string& tipo = contexto.vila.construcoes[indiceDaConstrucao].tipo;
    const DefinicaoDeConstrucao* definicao = contexto.dados.catalogoDeConstrucoes.procurarConstrucao(tipo);
    if (definicao != nullptr && definicao->trabalhoSemReceita.has_value()) {
        return *definicao->trabalhoSemReceita;
    }
    // Uma construção de produção parada: o Trabalho da primeira receita dela.
    for (const DefinicaoDeReceita& receita : contexto.dados.catalogoDeReceitas.todasAsReceitas()) {
        if (receita.construcaoQueProduz == tipo) {
            return receita.trabalhoUsado;
        }
    }
    return TrabalhoDaReceita::Manufaturado;
}

void escolherOsGerentes(ContextoDaProducao& contexto)
{
    const ListaDosEmpregadosDoPasso empregadosDoPasso(contexto);
    for (std::size_t indice = 0; indice < contexto.producaoDasConstrucoes.size(); ++indice) {
        const TrabalhoDaReceita trabalho = trabalhoDaConstrucao(contexto, static_cast<int>(indice));
        std::vector<CandidatoAGerente> candidatos;
        for (const entt::entity empregado : empregadosDaConstrucao(contexto, static_cast<int>(indice))) {
            candidatos.push_back({contexto.registroDasEntidades.get<IdentidadeDoNpc>(empregado).numeroDoNpc,
                                  contexto.registroDasEntidades.get<HabilidadesDoNpc>(empregado).habilidadeNoTrabalho(trabalho)});
        }
        contexto.producaoDasConstrucoes[indice].numeroDoGerente = escolherGerente(candidatos);
    }
}

std::string montarTabelaDaProducao(const ContextoDaProducao& contexto)
{
    const DadosDoJogo& dados = contexto.dados;
    std::string tabela = "Estoque (da Cidade / do jogador), o preço local e o consumo previsto por dia:\n";
    for (const DefinicaoDeItem& item : dados.catalogoDeItens.todosOsItens()) {
        const int64_t consumo = contexto.consumoMedido.consumoPrevistoDoDiaEmCentesimos(item.numeroInterno);
        tabela += "  " + nomeDoItem(contexto, item.numeroInterno) + ": " + std::to_string(contexto.inventario.quantidadeDe(item.numeroInterno, DonoDoItem::Cidade))
            + " / " + std::to_string(contexto.inventario.quantidadeDe(item.numeroInterno, DonoDoItem::Jogador)) + "; preço "
            + escreverDinheiroEmMoedas(precoLocalDoItem(contexto, item.numeroInterno)) + " (base " + escreverDinheiroEmMoedas(item.precoBaseEmDecimos)
            + "); consome " + std::to_string(consumo / 100) + "," + std::to_string(consumo % 100 / 10) + " por dia"
            + (contexto.consumoMedido.produtoEstaComEstoqueDeSobra.size() > static_cast<std::size_t>(item.numeroInterno)
                       && contexto.consumoMedido.produtoEstaComEstoqueDeSobra[item.numeroInterno]
                   ? " (estoque de sobra)"
                   : "")
            + "\n";
    }
    tabela += "Capacidade: matéria-prima " + std::to_string(contexto.inventario.quantidadeNaCategoria(CategoriaDoItem::MateriaPrima)) + " de "
        + std::to_string(contexto.inventario.capacidadeDaCategoria(CategoriaDoItem::MateriaPrima)) + "; bem de consumo "
        + std::to_string(contexto.inventario.quantidadeNaCategoria(CategoriaDoItem::BemDeConsumo)) + " de "
        + std::to_string(contexto.inventario.capacidadeDaCategoria(CategoriaDoItem::BemDeConsumo)) + "\n";

    tabela += "Construções:\n";
    const VilaDoJogo& vila = contexto.vila;
    for (std::size_t indice = 0; indice < vila.construcoes.size(); ++indice) {
        const ConstrucaoDaVila& construcao = vila.construcoes[indice];
        const ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indice];
        const std::vector<entt::entity> empregados = empregadosDaConstrucao(contexto, static_cast<int>(indice));
        if (construcao.ehCasa()) {
            continue;
        }
        int trabalhandoAgora = 0;
        std::string nomeDoGerente = "ninguém";
        for (const entt::entity empregado : empregados) {
            trabalhandoAgora += npcEstaTrabalhandoAgora(contexto, empregado) ? 1 : 0;
            const IdentidadeDoNpc& identidade = contexto.registroDasEntidades.get<IdentidadeDoNpc>(empregado);
            if (identidade.numeroDoNpc == producao.numeroDoGerente) {
                nomeDoGerente = identidade.nome;
            }
        }
        std::string linha = "  " + construcao.identificador + " (nível " + std::to_string(construcao.nivel) + ", "
            + (construcao.donoEhOJogador ? "do jogador" : "da Cidade") + "): ";
        if (!producao.ehDeProducao) {
            linha += "sem receita (só salários)";
        } else if (producao.receitaDoCiclo < 0) {
            linha += "sem receita no ciclo";
        } else {
            const DefinicaoDeReceita& receita = receitaPeloIndice(contexto, producao.receitaDoCiclo);
            const int64_t porcentagem = producao.progressoDoCiclo * 100 / progressoNecessarioDoCiclo(receita, contexto.ticksPorHoraDeJogo);
            linha += nomeDoItem(contexto, receita.numeroDoProduto) + ", " + std::to_string(porcentagem) + "% do ciclo"
                + (producao.cicloComecou ? "" : " (o ciclo não começou)");
        }
        if (producao.estaDeFolgaHoje) {
            linha += "; DE FOLGA HOJE";
        } else if (producao.estaParada) {
            linha += "; PARADA: " + producao.motivoDaParada;
        }
        linha += "; " + std::to_string(trabalhandoAgora) + " de " + std::to_string(empregados.size()) + " trabalhando agora; gerente: " + nomeDoGerente;
        if (!producao.producaoDeHojePorItem.empty()) {
            linha += "; hoje fez";
            for (const auto& [item, quantidade] : producao.producaoDeHojePorItem) {
                linha += " " + std::to_string(quantidade) + " " + nomeDoItem(contexto, item);
            }
        }
        if (producao.diasDeSalarioAtrasado > 0) {
            linha += "; salário atrasado há " + std::to_string(producao.diasDeSalarioAtrasado) + " dias";
        }
        tabela += linha + "\n";
    }

    const DinheiroDaVila& dinheiro = contexto.dinheiro;
    const int64_t mediaDosCivis = dinheiro.quantidadeDeCivis > 0 ? dinheiro.dinheiroDaClasseCivil / dinheiro.quantidadeDeCivis : 0;
    tabela += "Dinheiro: Banco da Cidade " + escreverDinheiroEmMoedas(static_cast<int>(dinheiro.bancoDaCidade)) + "; jogador "
        + escreverDinheiroEmMoedas(static_cast<int>(dinheiro.dinheiroDoJogador)) + "; Civis "
        + escreverDinheiroEmMoedas(static_cast<int>(dinheiro.dinheiroDaClasseCivil)) + " (" + escreverDinheiroEmMoedas(static_cast<int>(mediaDosCivis))
        + " em média); salários do último dia " + escreverDinheiroEmMoedas(static_cast<int>(dinheiro.rendaDoDiaDaClasseCivil)) + "\n";
    return tabela;
}
