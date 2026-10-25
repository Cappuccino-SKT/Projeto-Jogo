#include "Religiao/Religiao.h"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "Comercio/Comercio.h"
#include "Cultura/Cultura.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/ProblemasDosDados.h"
#include "Noticias/Noticias.h"
#include "Npc/ComponentesDoNpc.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDaReligiao = "religiao.json";

Devocao lerDevocao(const std::string& texto)
{
    if (texto == "alta") {
        return Devocao::Alta;
    }
    if (texto == "media") {
        return Devocao::Media;
    }
    if (texto == "baixa") {
        return Devocao::Baixa;
    }
    return Devocao::Nenhuma;
}

int posicaoDaEstacao(const std::string& texto)
{
    const char* const estacoes[] = {"primavera", "verao", "outono", "inverno"};
    for (int estacao = 0; estacao < 4; ++estacao) {
        if (texto == estacoes[estacao]) {
            return estacao;
        }
    }
    return -1;
}

const FeDaCultura* feDa(const RegrasDaReligiao& regras, const std::string& cultura)
{
    const auto fe = regras.fes.find(cultura);
    return fe == regras.fes.end() ? nullptr : &fe->second;
}

std::string opcaoDaLei(const SimulacaoDaVila& vila, int faccao, const char* lei)
{
    const int posicao = vila.dados.faccoes.posicaoDaLei(lei);
    if (faccao < 0 || posicao < 0 || faccao >= static_cast<int>(vila.faccoes.leisAtuais.size())) {
        return "";
    }
    return vila.dados.faccoes.leis[static_cast<std::size_t>(posicao)].opcoes[static_cast<std::size_t>(vila.faccoes.leisAtuais[static_cast<std::size_t>(faccao)][static_cast<std::size_t>(posicao)])];
}

// A Tolerância da facção (a Heresia é crime capital: como a Perseguição).
const ToleranciaDaLei* toleranciaDaFaccao(const SimulacaoDaVila& vila, int faccao)
{
    std::string opcao = opcaoDaLei(vila, faccao, "tolerancia_religiosa");
    if (opcao == "heresia_e_crime_capital") {
        opcao = "perseguicao";
    }
    const auto tolerancia = vila.dados.regrasDaReligiao.tolerancia.find(opcao);
    return tolerancia == vila.dados.regrasDaReligiao.tolerancia.end() ? nullptr : &tolerancia->second;
}

// O culto da cidade existe? Templo e Torre Arcana pela construção; na Tecnocrata, o Laboratório.
bool temOCulto(const SimulacaoDaVila& cidade, const std::string& culto)
{
    const std::string tipo = culto == "templo" ? "construcao.templo" : culto == "torre_arcana" ? "construcao.torre_arcana" : "construcao.laboratorio";
    return std::any_of(cidade.vila.construcoes.begin(), cidade.vila.construcoes.end(), [&](const ConstrucaoDaVila& construcao) { return construcao.tipo == tipo; });
}

// Os Sacerdotes (ou Magos): os empregados dos Templos (ou da Torre Arcana) da cidade.
int sacerdotesDaCidade(const SimulacaoDaVila& cidade, const std::string& culto)
{
    const std::string tipo = culto == "templo" ? "construcao.templo" : culto == "torre_arcana" ? "construcao.torre_arcana" : "";
    if (tipo.empty()) {
        return 0;  // a Tecnocrata não tem cobertura
    }
    int sacerdotes = 0;
    for (const entt::entity npc : cidade.npcsDaVila) {
        const int trabalho = cidade.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
        sacerdotes += trabalho >= 0 && cidade.vila.construcoes[static_cast<std::size_t>(trabalho)].tipo == tipo ? 1 : 0;
    }
    return sacerdotes;
}

std::map<std::string, int> civisPorCultura(const SimulacaoDaVila& cidade)
{
    std::map<std::string, int> porCultura;
    for (const entt::entity npc : cidade.npcsDaVila) {
        const IdentidadeDoNpc& identidade = cidade.registroDasEntidades.get<IdentidadeDoNpc>(npc);
        if (identidade.classe == "civil") {
            ++porCultura[identidade.cultura];
        }
    }
    return porCultura;
}

// Regra 4.6: a cada 100 na fração, o NPC mais novo dessa cultura passa para a base (RL1-4).
void assimilarAsMinorias(SimulacaoDaVila& cidade, const std::string& base, int taxaEmPorcento)
{
    std::map<std::string, int>& fracoes = cidade.religiao.fracaoDeAssimilacaoEmPessoas;
    for (const auto& [cultura, npcs] : civisPorCultura(cidade)) {
        if (cultura == base || npcs <= 0) {
            continue;
        }
        int& fracao = fracoes[cultura];
        fracao += npcs * taxaEmPorcento;
        while (fracao >= 100) {
            entt::entity maisNovo = entt::null;
            int menorIdade = 1 << 30;
            for (const entt::entity npc : cidade.npcsDaVila) {
                const IdentidadeDoNpc& identidade = cidade.registroDasEntidades.get<IdentidadeDoNpc>(npc);
                const int idade = cidade.registroDasEntidades.get<IdadeDoNpc>(npc).anos;
                if (identidade.cultura == cultura && idade < menorIdade) {
                    menorIdade = idade;
                    maisNovo = npc;
                }
            }
            if (maisNovo == entt::null) {
                fracao = 0;
                break;
            }
            cidade.registroDasEntidades.get<IdentidadeDoNpc>(maisNovo).cultura = base;
            fracao -= 100;
        }
    }
    // A minoria que acabou sai do estado.
    const std::map<std::string, int> presentes = civisPorCultura(cidade);
    std::erase_if(fracoes, [&](const auto& par) { return presentes.find(par.first) == presentes.end(); });
}

}  // namespace

RegrasDaReligiao lerArquivoDaReligiao(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDaReligiao regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaReligiao, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    const Json fes = documento.value("fes", Json::object());
    for (const auto& [cultura, fe] : fes.items()) {
        regras.fes[cultura] = FeDaCultura{lerDevocao(fe.value("devocao", std::string())), fe.value("culto", std::string())};
    }
    regras.npcsPorSacerdote = std::max(1, documento.value("npcs_por_sacerdote", regras.npcsPorSacerdote));
    const Json porDevocao = documento.value("felicidade_por_devocao", Json::object());
    const std::vector<int> alta = porDevocao.value("alta", std::vector<int>{10, -5, 50});
    if (alta.size() == 3) {
        regras.felicidadeAlta = alta[0];
        regras.felicidadeAltaComCoberturaBaixa = alta[1];
        regras.coberturaMinimaDaAlta = alta[2];
    } else {
        listaDeProblemas.anotarErro(nomeDoArquivoDaReligiao, 0, "a Devoção \"alta\" precisa de [máximo, com cobertura baixa, limite da cobertura]");
    }
    regras.felicidadeMedia = porDevocao.value("media", std::vector<int>{7}).front();
    regras.felicidadeBaixa = porDevocao.value("baixa", std::vector<int>{4}).front();
    regras.oferendaEmPorcentoDaRenda = documento.value("oferenda_em_porcento_da_renda", regras.oferendaEmPorcentoDaRenda);
    const Json festival = documento.value("festival", Json::object());
    regras.festivalFelicidade = festival.value("felicidade", regras.festivalFelicidade);
    regras.festivalDiasDoBonus = festival.value("dias_do_bonus", regras.festivalDiasDoBonus);
    regras.festivalComidaExtraEmPorcento = festival.value("comida_extra_em_porcento", regras.festivalComidaExtraEmPorcento);
    regras.festivalProducaoNoPrimeiroDiaEmPorcento = festival.value("producao_no_primeiro_dia_em_porcento", regras.festivalProducaoNoPrimeiroDiaEmPorcento);
    regras.festivalCustoEmPorcentoDoBanco = festival.value("custo_em_porcento_do_banco", regras.festivalCustoEmPorcentoDoBanco);
    const Json festivais = documento.value("festivais", Json::array());
    for (const Json& objeto : festivais) {
        FestivalDoCalendario definicao;
        definicao.cultura = objeto.value("cultura", std::string("todas"));
        definicao.nome = objeto.value("nome", std::string());
        definicao.estacao = posicaoDaEstacao(objeto.value("estacao", std::string()));
        definicao.dias = objeto.value("dias", std::vector<int>{});
        definicao.metade = objeto.value("metade", false);
        if (definicao.estacao < 0 || definicao.dias.empty()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaReligiao, 0, "o festival \"" + definicao.nome + "\" precisa de uma estação e dos dias");
            continue;
        }
        regras.festivais.push_back(std::move(definicao));
    }
    const Json tolerancia = documento.value("tolerancia", Json::object());
    for (const auto& [opcao, efeitos] : tolerancia.items()) {
        regras.tolerancia[opcao] = ToleranciaDaLei{efeitos.value("imposto_a_mais_das_minorias_em_porcento", 0), efeitos.value("felicidade_das_minorias", 0),
                                                   efeitos.value("assimilacao_por_estacao_em_porcento", 0)};
    }
    regras.mortesNaEpidemiaComOTemploDaReligiosaEmPorcento = documento.value("templo_da_religiosa_mortes_na_epidemia_em_porcento", regras.mortesNaEpidemiaComOTemploDaReligiosaEmPorcento);
    const Json peregrinacao = documento.value("peregrinacao", Json::object());
    regras.peregrinacaoOferendaMinima = peregrinacao.value("oferenda_minima", regras.peregrinacaoOferendaMinima);
    regras.peregrinacaoReputacao = peregrinacao.value("reputacao", regras.peregrinacaoReputacao);
    const Json bencao = documento.value("bencao", Json::object());
    regras.bencaoReputacaoMinima = bencao.value("reputacao_minima", regras.bencaoReputacaoMinima);
    regras.bencaoReputacao = bencao.value("reputacao", regras.bencaoReputacao);
    return regras;
}

int coberturaComOsNumeros(int sacerdotes, int npcs, const RegrasDaReligiao& regras)
{
    return npcs <= 0 ? 0 : std::min(100, sacerdotes * regras.npcsPorSacerdote * 100 / npcs);
}

int fatorDaFeEmDecimos(Devocao devocao, int coberturaEmPorcento, const RegrasDaReligiao& regras)
{
    switch (devocao) {
    case Devocao::Alta:
        return coberturaEmPorcento < regras.coberturaMinimaDaAlta ? 10 * regras.felicidadeAltaComCoberturaBaixa : coberturaEmPorcento * regras.felicidadeAlta / 10;
    case Devocao::Media: return coberturaEmPorcento * regras.felicidadeMedia / 10;
    case Devocao::Baixa: return coberturaEmPorcento * regras.felicidadeBaixa / 10;
    case Devocao::Nenhuma: return 0;
    }
    return 0;
}

int fatorReligiaoDaClasseEmDecimos(const DadosDoJogo& dados, const std::map<std::string, int>& npcsPorCultura, const std::string& cultoDaCidade, int coberturaEmPorcento)
{
    const RegrasDaReligiao& regras = dados.regrasDaReligiao;
    int64_t npcs = 0;
    int64_t soma = 0;
    for (const auto& [cultura, quantos] : npcsPorCultura) {
        const FeDaCultura* fe = feDa(regras, cultura);
        const Devocao devocao = fe != nullptr ? fe->devocao : Devocao::Nenhuma;
        // Passo 1: a cobertura só conta para quem tem o culto da cidade.
        const int cobertura = fe != nullptr && fe->culto == cultoDaCidade ? coberturaEmPorcento : 0;
        soma += static_cast<int64_t>(quantos) * fatorDaFeEmDecimos(devocao, cobertura, regras);
        npcs += quantos;
    }
    if (npcs == 0) {
        return 0;
    }
    return static_cast<int>(soma >= 0 ? soma / npcs : -((-soma + npcs - 1) / npcs));
}

int comidaExtraDoFestival(int civis, int comidaPorDia, int sobra, const RegrasDaReligiao& regras)
{
    return std::max(0, std::min(civis * comidaPorDia * regras.festivalComidaExtraEmPorcento / 100, sobra));
}

int assimiladosEmEstacoes(int npcsDaMinoria, int taxaEmPorcento, int estacoes)
{
    int fracao = 0;
    int assimilados = 0;
    for (int estacao = 0; estacao < estacoes; ++estacao) {
        fracao += (npcsDaMinoria - assimilados) * taxaEmPorcento;
        while (fracao >= 100 && assimilados < npcsDaMinoria) {
            ++assimilados;
            fracao -= 100;
        }
    }
    return assimilados;
}

void comprarAComidaDoFestival(ContextoDaProducao& contexto)
{
    if (contexto.efeitosDaReligiao == nullptr || contexto.efeitosDaReligiao->comidaExtraEmPorcento <= 0) {
        return;
    }
    const RegrasDaReligiao& regras = contexto.dados.regrasDaReligiao;
    DinheiroDaVila& dinheiro = contexto.dinheiro;
    int falta = comidaExtraDoFestival(dinheiro.quantidadeDeCivis, contexto.dados.regras.economia.comidaPorDiaDoCivil, 1 << 30, regras);
    for (int item = 0; item < contexto.dados.catalogoDeItens.quantidadeDeItens() && falta > 0; ++item) {
        if (!contexto.dados.catalogoDeItens.itemPeloNumero(item).ehComida()) {
            continue;
        }
        // RL1-2: só da sobra acima do alvo, com o dinheiro da classe.
        const int sobra = contexto.inventario.quantidadeDe(item, DonoDoItem::Cidade) - estoqueAlvoDoItem(contexto, item);
        const int preco = std::max(1, precoLocalDoItem(contexto, item));
        const int quantidade = std::min({falta, std::max(0, sobra), static_cast<int>(std::max<int64_t>(0, dinheiro.dinheiroDaClasseCivil) / preco)});
        if (quantidade <= 0 || !contexto.inventario.tentarTirar(item, DonoDoItem::Cidade, quantidade)) {
            continue;
        }
        contexto.consumoMedido.anotarOQueSaiu(item, quantidade);
        dinheiro.dinheiroDaClasseCivil -= static_cast<int64_t>(quantidade) * preco;
        dinheiro.bancoDaCidade += static_cast<int64_t>(quantidade) * preco;
        falta -= quantidade;
    }
}

int mortesComOTemplo(int mortes, const RegrasDaReligiao& regras)
{
    return mortes * regras.mortesNaEpidemiaComOTemploDaReligiosaEmPorcento / 100;
}

int mapaDaCidadeSanta(const DadosDoJogo& dados)
{
    for (const DefinicaoDaFaccao& faccao : dados.faccoes.faccoes) {
        if (faccao.cultura == "religiosa") {
            return dados.mundo.numeroDoMapaNa(faccao.colunaDaCapital, faccao.linhaDaCapital);
        }
    }
    return 0;
}

void atualizarAsCidadesPelaReligiao(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaReligiao& regras = dados.regrasDaReligiao;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        EfeitosDaReligiaoNaCidade efeitos;
        const int mapa = cidade->numeroDoMapaNoMundo();
        const int faccao = faccaoDaCidade(dados, mapa);
        if (faccao < 0) {
            cidade->efeitosDaReligiao = efeitos;  // a cidade sem facção não muda (RL1-3)
            continue;
        }
        const std::string base = culturaBaseDaCidade(dados, mapa);
        const FeDaCultura* fe = feDa(regras, base);
        const std::string culto = fe != nullptr ? fe->culto : "";
        const std::map<std::string, int> porCultura = civisPorCultura(*cidade);
        const int cobertura = coberturaComOsNumeros(sacerdotesDaCidade(*cidade, culto), static_cast<int>(cidade->npcsDaVila.size()), regras);
        efeitos.fatorEmDecimos = fatorReligiaoDaClasseEmDecimos(dados, porCultura, culto, cobertura);
        efeitos.culturaBase = base;
        efeitos.semAFestaDaColheitaDaFatia = true;  // a Festa da Colheita é do calendário (4.4)
        efeitos.oferendaEmPorcentoDaRenda = fe != nullptr && fe->devocao != Devocao::Nenhuma ? regras.oferendaEmPorcentoDaRenda : 0;
        // 4.5: a Tolerância nas minorias.
        if (const ToleranciaDaLei* tolerancia = toleranciaDaFaccao(vila, faccao)) {
            efeitos.impostoAMaisDasMinoriasEmPorcento = tolerancia->impostoAMaisDasMinoriasEmPorcento;
            int civis = 0;
            int minorias = 0;
            for (const auto& [cultura, quantos] : porCultura) {
                civis += quantos;
                minorias += cultura != base ? quantos : 0;
            }
            if (civis > 0 && tolerancia->felicidadeDasMinorias != 0) {
                efeitos.fatorEmDecimos += 10 * tolerancia->felicidadeDasMinorias * minorias / civis;
            }
        }
        // 4.4: o festival.
        const EstadoDaReligiaoDaCidade& estado = cidade->religiao;
        efeitos.felicidadeDoFestivalEmPontos = estado.diasDoFestival > 0 ? estado.pontosDoFestival : 0;
        efeitos.producaoPresumidaEmPorcento = estado.primeiroDiaDoFestival ? regras.festivalProducaoNoPrimeiroDiaEmPorcento : 100;
        efeitos.comidaExtraEmPorcento = estado.diaDeFestival ? regras.festivalComidaExtraEmPorcento : 0;
        // 4.7: o Templo da Religiosa na Epidemia.
        if (base == "religiosa" && temOCulto(*cidade, "templo")) {
            efeitos.mortesDaEpidemiaEmPorcento = regras.mortesNaEpidemiaComOTemploDaReligiosaEmPorcento;
        }
        cidade->efeitosDaReligiao = efeitos;
    }
}

std::vector<std::string> rodarAReligiaoNoDiaNovo(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaReligiao& regras = dados.regrasDaReligiao;
    std::vector<std::string> linhas;
    const int estacao = static_cast<int>(vila.relogio.estacaoAtual());
    const int dia = vila.relogio.diaDaEstacaoAtual();
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        const int mapa = cidade->numeroDoMapaNoMundo();
        const int faccao = faccaoDaCidade(dados, mapa);
        if (faccao < 0) {
            continue;
        }
        EstadoDaReligiaoDaCidade& estado = cidade->religiao;
        const std::string base = culturaBaseDaCidade(dados, mapa);
        // 4.4: os dias do bônus contam; um festival do calendário começa no primeiro dia dele.
        estado.diasDoFestival = std::max(0, estado.diasDoFestival - 1);
        estado.primeiroDiaDoFestival = false;
        estado.diaDeFestival = false;
        for (const FestivalDoCalendario& festival : regras.festivais) {
            if ((festival.cultura != "todas" && festival.cultura != base) || festival.estacao != estacao
                || std::find(festival.dias.begin(), festival.dias.end(), dia) == festival.dias.end()) {
                continue;
            }
            estado.diaDeFestival = true;
            if (dia != festival.dias.front() || estado.primeiroDiaDoFestival) {
                continue;  // o festival já começou (ou outro começou hoje: só o primeiro da lista vale)
            }
            estado.primeiroDiaDoFestival = true;
            estado.diasDoFestival = regras.festivalDiasDoBonus;
            estado.pontosDoFestival = festival.metade ? regras.festivalFelicidade / 2 : regras.festivalFelicidade;
            // Passo 2: 2% do Banco vão para a classe Civil (a festa é paga a ela).
            const int64_t custo = std::max<int64_t>(0, cidade->dinheiro.bancoDaCidade) * regras.festivalCustoEmPorcentoDoBanco / 100;
            cidade->dinheiro.bancoDaCidade -= custo;
            cidade->dinheiro.dinheiroDaClasseCivil += custo;
            Noticia noticia;
            noticia.tipo = "religiao.festival";
            noticia.importancia = ImportanciaDaNoticia::Local;
            noticia.mapaDeOrigem = mapa;
            noticia.tickDeOrigem = vila.relogio.tickAtual();
            noticia.diaDeOrigem = vila.relogio.diaDaPartida();
            const CidadeDoMundo* daCidade = dados.mundo.cidadeNoMapa(mapa);
            noticia.titulo = festival.nome + " em " + (daCidade != nullptr ? daCidade->nome : dados.mundo.nomeDoMapa(mapa));
            criarNoticia(vila, std::move(noticia));
        }
        // 4.6: a assimilação, no dia 1 da estação, onde há o culto da cultura base.
        const FeDaCultura* fe = feDa(regras, base);
        const ToleranciaDaLei* tolerancia = toleranciaDaFaccao(vila, faccao);
        if (dia == 1 && vila.relogio.diaDaPartida() > 0 && fe != nullptr && tolerancia != nullptr && tolerancia->assimilacaoPorEstacaoEmPorcento > 0 && temOCulto(*cidade, fe->culto)) {
            assimilarAsMinorias(*cidade, base, tolerancia->assimilacaoPorEstacaoEmPorcento);
        }
    }
    // 4.8: a Bênção, no dia 1 da estação, a quem fez algo pela fé na estação passada.
    EstadoDaReligiaoDoJogador& jogador = vila.religiaoDoJogador;
    const int estacaoDaPartida = static_cast<int>(vila.relogio.diaDaPartida() / std::max(1, vila.relogio.diasPorEstacao()));
    const int religiosa = donoDaCidadeDoMapa(dados.mundo, mapaDaCidadeSanta(dados));
    if (dia == 1 && religiosa >= 0 && jogador.estacaoDaUltimaBencao != estacaoDaPartida && jogador.estacaoDoUltimoAtoDeFe == estacaoDaPartida - 1
        && vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(religiosa)] >= regras.bencaoReputacaoMinima) {
        jogador.estacaoDaUltimaBencao = estacaoDaPartida;
        linhas.push_back("Reputação: o Sumo Sacerdote abençoou o jogador.");
        const std::string faixa = mudarReputacao(vila, religiosa, regras.bencaoReputacao, "a Bênção do Sumo Sacerdote");
        if (!faixa.empty()) {
            linhas.push_back(faixa);
        }
    }
    atualizarAsCidadesPelaReligiao(vila);
    return linhas;
}

void anotarOAtoDeFe(SimulacaoDaVila& vila)
{
    vila.religiaoDoJogador.estacaoDoUltimoAtoDeFe = static_cast<int>(vila.relogio.diaDaPartida() / std::max(1, vila.relogio.diasPorEstacao()));
}

std::string fazerPeregrinacao(SimulacaoDaVila& vila, int moedas, std::vector<std::string>& linhas)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDaReligiao& regras = dados.regrasDaReligiao;
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    const int santa = mapaDaCidadeSanta(dados);
    if (grupo.emTransito || grupo.mapaAtual != santa) {
        return "a peregrinação é no Templo da Capital Religiosa";
    }
    if (moedas < regras.peregrinacaoOferendaMinima) {
        return "a oferenda precisa ser de " + std::to_string(regras.peregrinacaoOferendaMinima) + " moedas ou mais";
    }
    if (grupo.dinheiroEmDecimos < static_cast<int64_t>(moedas) * 10) {
        return "o grupo não leva " + std::to_string(moedas) + " moedas";
    }
    grupo.dinheiroEmDecimos -= static_cast<int64_t>(moedas) * 10;
    for (const std::unique_ptr<SimulacaoDaVila>& cidade : vila.outrasCidades) {
        if (cidade->numeroDoMapaDaCidade == santa) {
            cidade->dinheiro.bancoDaCidade += static_cast<int64_t>(moedas) * 10;
        }
    }
    anotarOAtoDeFe(vila);
    // Uma vez por ano dá reputação; a segunda oferenda é aceita, mas não dá.
    const int ano = vila.relogio.anoAtual();
    if (vila.religiaoDoJogador.anoDaUltimaPeregrinacao == ano) {
        linhas.push_back("Reputação: o Templo aceitou a oferenda de " + std::to_string(moedas) + " moedas (a peregrinação deste ano já foi feita).");
        return "";
    }
    vila.religiaoDoJogador.anoDaUltimaPeregrinacao = ano;
    linhas.push_back("Reputação: peregrinação à Cidade santa, com a oferenda de " + std::to_string(moedas) + " moedas.");
    const std::string faixa = mudarReputacao(vila, donoDaCidadeDoMapa(dados.mundo, santa), regras.peregrinacaoReputacao, "a peregrinação à Cidade santa");
    if (!faixa.empty()) {
        linhas.push_back(faixa);
    }
    return "";
}
