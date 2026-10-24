#include "Faccao/Faccao.h"

#include <algorithm>
#include <cstdlib>

#include <nlohmann/json.hpp>

#include "Comercio/Comercio.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/ProblemasDosDados.h"
#include "Noticias/Noticias.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDasFaccoes = "faccoes.json";
const char* const nomeDoArquivoDasCulturas = "culturas.json";
// O líder (ao criar o mundo) e o veto da assembleia sorteiam com a sequência 7 (parte 2 da ficha).
constexpr uint64_t sequenciaDoSorteioDasFaccoes = 7;

int posicaoDaOpcao(const DefinicaoDaLei& lei, const std::string& opcao)
{
    const auto encontrada = std::find(lei.opcoes.begin(), lei.opcoes.end(), opcao);
    return encontrada == lei.opcoes.end() ? -1 : static_cast<int>(encontrada - lei.opcoes.begin());
}

// O item bate com o nome da lista: o id ("item.fumo"), o id sem "item." ou o tipo ("alimento").
bool itemBateCom(const DefinicaoDeItem& item, const std::string& nome)
{
    return item.identificadorDeTexto == nome || item.identificadorDeTexto == "item." + nome || item.tipoDoItem == nome;
}

std::string textoDaOpcao(const std::string& opcao)
{
    std::string texto = opcao;
    std::replace(texto.begin(), texto.end(), '_', ' ');
    return texto;
}

int leiDaFaccao(const SimulacaoDaVila& vila, int faccao, const char* lei)
{
    const int posicao = vila.dados.faccoes.posicaoDaLei(lei);
    if (faccao < 0 || posicao < 0 || faccao >= static_cast<int>(vila.faccoes.leisAtuais.size())) {
        return -1;
    }
    return vila.faccoes.leisAtuais[static_cast<std::size_t>(faccao)][static_cast<std::size_t>(posicao)];
}

// Uma cidade do mundo pelo número do mapa (a vila ou uma das outras).
SimulacaoDaVila* simulacaoDaCidade(SimulacaoDaVila& vila, int numeroDoMapa)
{
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        if (cidade->numeroDoMapaNoMundo() == numeroDoMapa) {
            return cidade;
        }
    }
    return nullptr;
}

std::vector<SimulacaoDaVila*> cidadesDaFaccao(SimulacaoDaVila& vila, int faccao)
{
    std::vector<SimulacaoDaVila*> cidades;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        if (faccaoDaCidade(vila.dados, cidade->numeroDoMapaNoMundo()) == faccao) {
            cidades.push_back(cidade);
        }
    }
    return cidades;
}

// A posição desejada pela cultura, ou −1.
int desejoDaCultura(const CatalogoDasFaccoes& catalogo, const std::string& cultura, int lei)
{
    const auto desejos = catalogo.leisDesejadasPorCultura.find(cultura);
    if (desejos == catalogo.leisDesejadasPorCultura.end() || lei >= static_cast<int>(desejos->second.size())) {
        return -1;
    }
    return desejos->second[static_cast<std::size_t>(lei)].valor;
}

// +1 = a mudança aproxima do desejo; −1 = afasta; 0 = não muda nada (ou a cultura não liga).
int opiniaoSobreAMudanca(const CatalogoDasFaccoes& catalogo, const std::string& cultura, int lei, int valorAtual, int valorNovo)
{
    const int desejo = desejoDaCultura(catalogo, cultura, lei);
    if (desejo < 0) {
        return 0;
    }
    const int antes = std::abs(valorAtual - desejo);
    const int depois = std::abs(valorNovo - desejo);
    return depois < antes ? 1 : (depois > antes ? -1 : 0);
}

}  // namespace

int CatalogoDasFaccoes::posicaoDaLei(const std::string& id) const
{
    for (std::size_t posicao = 0; posicao < leis.size(); ++posicao) {
        if (leis[posicao].id == id) {
            return static_cast<int>(posicao);
        }
    }
    return -1;
}

CatalogoDasFaccoes lerArquivosDasFaccoes(const std::string& textoDasFaccoes, const std::string& textoDasCulturas, ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDasFaccoes catalogo;
    Json documento;
    Json culturas;
    try {
        documento = Json::parse(textoDasFaccoes);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDasFaccoes, 0, std::string("o JSON está quebrado: ") + erro.what());
        return catalogo;
    }
    try {
        culturas = Json::parse(textoDasCulturas);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDasCulturas, 0, std::string("o JSON está quebrado: ") + erro.what());
    }
    // As leis, em fila.
    if (const auto leis = documento.find("leis"); leis != documento.end() && leis->is_object()) {
        for (const auto& [id, definicao] : leis->items()) {
            DefinicaoDaLei lei;
            lei.id = id;
            lei.opcoes = definicao.value("opcoes", std::vector<std::string>{});
            lei.valoresEmPorcento = definicao.value("valores_em_porcento", std::vector<int>{});
            lei.felicidade = definicao.value("felicidade", std::vector<int>{});
            if (lei.opcoes.empty()) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasFaccoes, 0, "a lei \"" + id + "\" precisa de \"opcoes\"");
            }
            catalogo.leis.push_back(std::move(lei));
        }
    } else {
        listaDeProblemas.anotarErro(nomeDoArquivoDasFaccoes, 0, "falta o objeto \"leis\"");
    }
    // A ordem do arquivo é a ordem das leis (o desempate da pauta); o nlohmann ordena as chaves,
    // então a ordem vem da lista "ordem_das_leis", se houver.
    if (const auto ordem = documento.find("ordem_das_leis"); ordem != documento.end() && ordem->is_array()) {
        std::vector<DefinicaoDaLei> ordenadas;
        for (const Json& id : *ordem) {
            const int posicao = catalogo.posicaoDaLei(id.get<std::string>());
            if (posicao < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasFaccoes, 0, "a lei \"" + id.get<std::string>() + "\" da \"ordem_das_leis\" não existe");
                continue;
            }
            ordenadas.push_back(catalogo.leis[static_cast<std::size_t>(posicao)]);
        }
        if (ordenadas.size() == catalogo.leis.size()) {
            catalogo.leis = std::move(ordenadas);
        } else {
            listaDeProblemas.anotarErro(nomeDoArquivoDasFaccoes, 0, "a \"ordem_das_leis\" precisa ter todas as leis");
        }
    }
    const auto lerLeis = [&](const Json& objeto, const std::string& dono, std::vector<int>& valores) {
        valores.assign(catalogo.leis.size(), 0);
        for (std::size_t lei = 0; lei < catalogo.leis.size(); ++lei) {
            const auto valor = objeto.find(catalogo.leis[lei].id);
            const int posicao = valor != objeto.end() && valor->is_string() ? posicaoDaOpcao(catalogo.leis[lei], valor->get<std::string>()) : -1;
            if (posicao < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasFaccoes, 0, dono + ": a lei \"" + catalogo.leis[lei].id + "\" precisa de uma das opções");
                continue;
            }
            valores[lei] = posicao;
        }
    };
    const Json listaDasFaccoes = documento.value("faccoes", Json::array());
    for (const Json& objeto : listaDasFaccoes) {
        DefinicaoDaFaccao faccao;
        faccao.id = objeto.value("id", std::string());
        faccao.cultura = objeto.value("cultura", std::string());
        const std::vector<int> capital = objeto.value("capital", std::vector<int>{1, 1});
        faccao.colunaDaCapital = capital.size() == 2 ? capital[0] : 1;
        faccao.linhaDaCapital = capital.size() == 2 ? capital[1] : 1;
        const Json leisDaFaccao = objeto.value("leis", Json::object());
        lerLeis(leisDaFaccao, faccao.id, faccao.leisIniciais);
        faccao.itensProibidos = objeto.value("itens_proibidos", std::vector<std::string>{});
        faccao.ajusteDePreco = objeto.value("ajuste_de_preco", std::map<std::string, int>{});
        catalogo.faccoes.push_back(std::move(faccao));
    }
    const auto lerInteiro = [&](const char* campo, int& valor) {
        if (const auto lido = documento.find(campo); lido != documento.end() && lido->is_number_integer()) {
            valor = lido->get<int>();
        }
    };
    lerInteiro("cunhagem_por_casa_da_moeda_por_dia_em_decimos", catalogo.cunhagemPorCasaDaMoedaPorDiaEmDecimos);
    lerInteiro("verba_por_npc_por_estacao_em_decimos", catalogo.verbaPorNpcPorEstacaoEmDecimos);
    lerInteiro("banco_inicial_em_porcento_do_pib", catalogo.bancoInicialEmPorcentoDoPib);
    lerInteiro("legitimidade_inicial", catalogo.legitimidadeInicial);
    lerInteiro("veto_custa_legitimidade", catalogo.vetoCustaLegitimidade);
    lerInteiro("leis_por_assembleia", catalogo.leisPorAssembleia);
    lerInteiro("felicidade_de_quem_perdeu_a_votacao", catalogo.felicidadeDeQuemPerdeuAVotacao);
    // O desejo de cada cultura (o culturas.json; a Cultura, C2, acrescenta o resto).
    for (const Json& cultura : culturas.is_array() ? culturas : Json::array()) {
        std::string id = cultura.value("id", std::string());
        if (id.rfind("cultura.", 0) == 0) {
            id = id.substr(8);
        }
        std::vector<LeiDesejada> desejos(catalogo.leis.size());
        const Json leisDesejadas = cultura.value("leis_desejadas", Json::object());
        for (const auto& [lei, desejo] : leisDesejadas.items()) {
            const int posicao = catalogo.posicaoDaLei(lei);
            if (posicao < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasCulturas, 0, id + ": a lei \"" + lei + "\" não existe");
                continue;
            }
            const int valor = posicaoDaOpcao(catalogo.leis[static_cast<std::size_t>(posicao)], desejo.value("valor", std::string()));
            if (valor < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasCulturas, 0, id + ": o valor desejado de \"" + lei + "\" não é uma das opções");
                continue;
            }
            desejos[static_cast<std::size_t>(posicao)] = LeiDesejada{valor, desejo.value("peso", 1)};
        }
        catalogo.leisDesejadasPorCultura[id] = std::move(desejos);
    }
    return catalogo;
}

int faccaoDaCidade(const DadosDoJogo& dados, int numeroDoMapa)
{
    const CidadeDoMundo* cidade = dados.mundo.cidadeNoMapa(numeroDoMapa);
    if (cidade == nullptr || cidade->faccao.empty()) {
        return -1;
    }
    for (std::size_t faccao = 0; faccao < dados.faccoes.faccoes.size(); ++faccao) {
        if (dados.faccoes.faccoes[faccao].cultura == cidade->faccao) {
            return static_cast<int>(faccao);
        }
    }
    return -1;
}

int faccaoDoMapa(const DadosDoJogo& dados, int numeroDoMapa)
{
    if (dados.mundo.cidadeNoMapa(numeroDoMapa) != nullptr) {
        return faccaoDaCidade(dados, numeroDoMapa);
    }
    if (dados.mundo.estadoDoTerritorio(numeroDoMapa) != EstadoDoTerritorio::Territorio) {
        return -1;
    }
    for (const int vizinho : dados.mundo.vizinhos(numeroDoMapa)) {
        const int faccao = faccaoDaCidade(dados, vizinho);
        if (faccao >= 0) {
            return faccao;
        }
    }
    return -1;
}

std::string nomeDaFaccao(const DadosDoJogo& dados, int faccao)
{
    if (faccao < 0 || faccao >= static_cast<int>(dados.faccoes.faccoes.size())) {
        return "?";
    }
    return nomeDoDonoDaReputacao(dados.regrasDaReputacao, dados.faccoes.faccoes[static_cast<std::size_t>(faccao)].cultura);
}

void montarAsFaccoesDaPartidaNova(SimulacaoDaVila& vila, uint64_t sementeDaPartida)
{
    const CatalogoDasFaccoes& catalogo = vila.dados.faccoes;
    EstadoDasFaccoes& estado = vila.faccoes;
    estado = EstadoDasFaccoes{};
    estado.perdeuAVotacaoAte.assign(vila.dados.mundo.mapas.size(), -1);
    SorteioComSemente sorteio(sementeDaPartida, sequenciaDoSorteioDasFaccoes);
    for (std::size_t faccao = 0; faccao < catalogo.faccoes.size(); ++faccao) {
        estado.leisAtuais.push_back(catalogo.faccoes[faccao].leisIniciais);
        // 4.4, passo 1: 25% do PIB (o dinheiro das cidades da facção).
        int64_t pib = 0;
        for (SimulacaoDaVila* cidade : cidadesDaFaccao(vila, static_cast<int>(faccao))) {
            pib += cidade->dinheiro.dinheiroTotal();
        }
        estado.bancoDaFaccao.push_back(pib * catalogo.bancoInicialEmPorcentoDoPib / 100);
        // 4.6: o líder (CP1-4: de 8 a 18 anos).
        LiderDaFaccao lider;
        lider.idade = sorteio.sortearNumeroInteiroEntre(8, 18);
        lider.agressividade = sorteio.sortearNumeroInteiroEntre(20, 80);
        lider.sociabilidade = sorteio.sortearNumeroInteiroEntre(20, 80);
        lider.legitimidade = catalogo.legitimidadeInicial;
        estado.lideres.push_back(lider);
    }
    atualizarAsCidadesPelasLeis(vila);
}

int taxaDoImposto(const SimulacaoDaVila& vila, int numeroDoMapa)
{
    const int faccao = faccaoDaCidade(vila.dados, numeroDoMapa);
    const int valor = leiDaFaccao(vila, faccao, "imposto");
    const int lei = vila.dados.faccoes.posicaoDaLei("imposto");
    if (valor < 0 || lei < 0 || valor >= static_cast<int>(vila.dados.faccoes.leis[static_cast<std::size_t>(lei)].valoresEmPorcento.size())) {
        return vila.dados.regras.economia.imposto.taxaEmPorcento;  // a cidade sem facção: 20%
    }
    return vila.dados.faccoes.leis[static_cast<std::size_t>(lei)].valoresEmPorcento[static_cast<std::size_t>(valor)];
}

int felicidadeDaLeiDoImposto(const SimulacaoDaVila& vila, int numeroDoMapa)
{
    const int valor = leiDaFaccao(vila, faccaoDaCidade(vila.dados, numeroDoMapa), "imposto");
    const int lei = vila.dados.faccoes.posicaoDaLei("imposto");
    if (valor < 0 || lei < 0 || valor >= static_cast<int>(vila.dados.faccoes.leis[static_cast<std::size_t>(lei)].felicidade.size())) {
        return 0;
    }
    return vila.dados.faccoes.leis[static_cast<std::size_t>(lei)].felicidade[static_cast<std::size_t>(valor)];
}

int taxaDaTarifa(const SimulacaoDaVila& vila, int faccao)
{
    const int valor = leiDaFaccao(vila, faccao, "tarifa");
    const int lei = vila.dados.faccoes.posicaoDaLei("tarifa");
    if (valor < 0 || lei < 0 || valor >= static_cast<int>(vila.dados.faccoes.leis[static_cast<std::size_t>(lei)].valoresEmPorcento.size())) {
        return 0;
    }
    return vila.dados.faccoes.leis[static_cast<std::size_t>(lei)].valoresEmPorcento[static_cast<std::size_t>(valor)];
}

bool itemEhProibidoNaFaccao(const DadosDoJogo& dados, int faccao, int numeroDoItem)
{
    if (faccao < 0 || faccao >= static_cast<int>(dados.faccoes.faccoes.size())) {
        return false;
    }
    const DefinicaoDeItem& item = dados.catalogoDeItens.itemPeloNumero(numeroDoItem);
    const std::vector<std::string>& proibidos = dados.faccoes.faccoes[static_cast<std::size_t>(faccao)].itensProibidos;
    return std::any_of(proibidos.begin(), proibidos.end(), [&](const std::string& nome) { return itemBateCom(item, nome); });
}

int ajusteDePrecoDaFaccaoEmPorcento(const DadosDoJogo& dados, int faccao, int numeroDoItem)
{
    if (faccao < 0 || faccao >= static_cast<int>(dados.faccoes.faccoes.size())) {
        return 0;
    }
    const DefinicaoDeItem& item = dados.catalogoDeItens.itemPeloNumero(numeroDoItem);
    for (const auto& [nome, ajuste] : dados.faccoes.faccoes[static_cast<std::size_t>(faccao)].ajusteDePreco) {
        if (itemBateCom(item, nome)) {
            return ajuste;
        }
    }
    return 0;
}

void atualizarAsCidadesPelasLeis(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    const int64_t hoje = vila.relogio.diaDaPartida();
    std::vector<int> tarifas;
    for (std::size_t faccao = 0; faccao < dados.faccoes.faccoes.size(); ++faccao) {
        tarifas.push_back(taxaDaTarifa(vila, static_cast<int>(faccao)));
    }
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        const int mapa = cidade->numeroDoMapaNoMundo();
        const int faccao = faccaoDaCidade(dados, mapa);
        cidade->impostoDaCidadeEmPorcento = faccao >= 0 ? taxaDoImposto(vila, mapa) : -1;
        const bool perdeu = mapa >= 1 && mapa <= static_cast<int>(vila.faccoes.perdeuAVotacaoAte.size())
            && vila.faccoes.perdeuAVotacaoAte[static_cast<std::size_t>(mapa - 1)] >= hoje;
        cidade->felicidadeDaFaccaoEmPontos = felicidadeDaLeiDoImposto(vila, mapa) + (perdeu ? dados.faccoes.felicidadeDeQuemPerdeuAVotacao : 0);
        cidade->tarifaDasFaccoesEmPorcento = tarifas;
        cidade->itensProibidos.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), false);
        for (int item = 0; item < dados.catalogoDeItens.quantidadeDeItens(); ++item) {
            cidade->itensProibidos[static_cast<std::size_t>(item)] = itemEhProibidoNaFaccao(dados, faccao, item);
        }
    }
}

std::vector<int64_t> dividirAVerba(int64_t bancoDaFaccao, const std::vector<int64_t>& verbaDeCadaCidade)
{
    int64_t total = 0;
    for (const int64_t verba : verbaDeCadaCidade) {
        total += verba;
    }
    if (total <= bancoDaFaccao) {
        return verbaDeCadaCidade;
    }
    std::vector<int64_t> pagas;
    for (const int64_t verba : verbaDeCadaCidade) {
        pagas.push_back(total > 0 ? verba * std::max<int64_t>(0, bancoDaFaccao) / total : 0);
    }
    return pagas;
}

int64_t tarifaComOsNumeros(int64_t valorDaCargaEmDecimos, int taxaEmPorcento)
{
    return valorDaCargaEmDecimos * taxaEmPorcento / 100;
}

int64_t cobrarTarifa(SimulacaoDaVila& vila, SimulacaoDaVila& casa, CaravanaDaCidade& caravana, int mapaAnterior, int mapaNovo)
{
    const DadosDoJogo& dados = vila.dados;
    const int dona = faccaoDoMapa(dados, mapaNovo);
    const int daCaravana = faccaoDaCidade(dados, casa.numeroDoMapaNoMundo());
    if (dona < 0 || dona == daCaravana || faccaoDoMapa(dados, mapaAnterior) == dona) {
        return 0;
    }
    // O valor da carga pelo preço local da cidade de onde ela saiu.
    SimulacaoDaVila* saida = simulacaoDaCidade(vila, caravana.situacao == SituacaoDaCaravana::Voltando ? caravana.mapaDeDestino : casa.numeroDoMapaNoMundo());
    const ContextoDaProducao precos = (saida != nullptr ? saida : &casa)->contextoDaProducao();
    int64_t valor = 0;
    for (std::size_t item = 0; item < caravana.entidade.quantidadePorItem.size(); ++item) {
        valor += static_cast<int64_t>(caravana.entidade.quantidadePorItem[item]) * precoLocalDoItem(precos, static_cast<int>(item));
    }
    const int64_t tarifa = std::min(tarifaComOsNumeros(valor, taxaDaTarifa(vila, dona)), std::max<int64_t>(0, casa.dinheiro.bancoDaCidade));
    casa.dinheiro.bancoDaCidade -= tarifa;
    vila.faccoes.bancoDaFaccao[static_cast<std::size_t>(dona)] += tarifa;
    return tarifa;
}

int distanciaDaLei(const CatalogoDasFaccoes& catalogo, int lei, int valorAtual, const std::string& cultura)
{
    const auto desejos = catalogo.leisDesejadasPorCultura.find(cultura);
    if (desejos == catalogo.leisDesejadasPorCultura.end() || lei >= static_cast<int>(desejos->second.size())) {
        return 0;
    }
    const LeiDesejada& desejo = desejos->second[static_cast<std::size_t>(lei)];
    return desejo.valor < 0 ? 0 : std::abs(valorAtual - desejo.valor) * desejo.peso;
}

ResultadoDaAssembleia fazerAssembleia(const CatalogoDasFaccoes& catalogo, const std::vector<int>& leisAtuais, const std::vector<VotanteDaAssembleia>& votantes,
                                      const std::string& culturaDoLider, LiderDaFaccao& lider, const std::function<int()>& sortearDe0A99)
{
    ResultadoDaAssembleia resultado;
    // Passo 1: cada cidade propõe a lei de maior distância, um passo na direção do desejo.
    std::map<std::pair<int, int>, int> propostas;  // (lei, novo valor) → cidades
    for (const VotanteDaAssembleia& votante : votantes) {
        int melhorLei = -1;
        int maiorDistancia = 0;
        for (int lei = 0; lei < static_cast<int>(leisAtuais.size()); ++lei) {
            const int distancia = distanciaDaLei(catalogo, lei, leisAtuais[static_cast<std::size_t>(lei)], votante.cultura);
            if (distancia > maiorDistancia) {
                maiorDistancia = distancia;
                melhorLei = lei;
            }
        }
        if (melhorLei >= 0) {
            const int atual = leisAtuais[static_cast<std::size_t>(melhorLei)];
            const int desejo = desejoDaCultura(catalogo, votante.cultura, melhorLei);
            ++propostas[{melhorLei, atual + (desejo > atual ? 1 : -1)}];
        }
    }
    // Passo 2: as 3 com mais cidades; empate, a ordem das leis.
    std::vector<std::pair<std::pair<int, int>, int>> ordenadas(propostas.begin(), propostas.end());
    std::stable_sort(ordenadas.begin(), ordenadas.end(), [](const auto& uma, const auto& outra) {
        return uma.second != outra.second ? uma.second > outra.second : uma.first.first < outra.first.first;
    });
    const int votacao = catalogo.posicaoDaLei("sistema_de_votacao");
    const int poder = catalogo.posicaoDaLei("poder_do_lider");
    const std::string sistema = votacao >= 0 ? catalogo.leis[static_cast<std::size_t>(votacao)].opcoes[static_cast<std::size_t>(leisAtuais[static_cast<std::size_t>(votacao)])] : "maioria_simples";
    const std::string poderDoLider = poder >= 0 ? catalogo.leis[static_cast<std::size_t>(poder)].opcoes[static_cast<std::size_t>(leisAtuais[static_cast<std::size_t>(poder)])] : "fraco";
    for (std::size_t posicao = 0; posicao < ordenadas.size() && static_cast<int>(posicao) < catalogo.leisPorAssembleia; ++posicao) {
        const int lei = ordenadas[posicao].first.first;
        const int novo = ordenadas[posicao].first.second;
        const int atual = leisAtuais[static_cast<std::size_t>(lei)];
        const int posicaoDoLider = opiniaoSobreAMudanca(catalogo, culturaDoLider, lei, atual, novo);
        // Passos 3 e 4: os votos e a contagem.
        int64_t sim = 0;
        int64_t total = 0;
        std::vector<std::pair<int, int>> votos;  // (mapa, +1 sim / −1 não)
        for (const VotanteDaAssembleia& votante : votantes) {
            if (sistema == "conselho" && !votante.ehACapital) {
                continue;  // FA1-4: só a capital, até a dinastia
            }
            int voto = opiniaoSobreAMudanca(catalogo, votante.cultura, lei, atual, novo);
            if (votante.felicidade < 30 && posicaoDoLider != 0) {
                voto = -posicaoDoLider;
            }
            if (voto == 0) {
                continue;
            }
            const int64_t peso = sistema == "por_populacao" ? votante.npcs : (sistema == "por_nivel" ? votante.nivel : 1);
            total += peso;
            sim += voto > 0 ? peso : 0;
            votos.emplace_back(votante.numeroDoMapa, voto);
        }
        const bool passou = total > 0 && (sistema == "dois_tercos" ? sim * 3 >= total * 2 : sim * 2 > total);
        // Passo 5: o veto do líder.
        bool vetada = false;
        if (passou && posicaoDoLider < 0) {
            if (poderDoLider == "autoritario" || poderDoLider == "pedir_lei_e_crime") {
                vetada = true;
            } else if (poderDoLider == "moderado") {
                vetada = sortearDe0A99() < std::clamp(50 + (lider.agressividade - lider.sociabilidade) / 2, 0, 100);
            }
            if (vetada) {
                lider.legitimidade = std::max(0, lider.legitimidade - catalogo.vetoCustaLegitimidade);
            }
        }
        const bool mudou = passou && !vetada;
        std::vector<int> perderam;
        for (const auto& [mapa, voto] : votos) {
            if ((mudou && voto < 0) || (!mudou && voto > 0)) {
                perderam.push_back(mapa);
            }
        }
        resultado.pauta.push_back(lei);
        resultado.novoValor.push_back(novo);
        resultado.passou.push_back(passou);
        resultado.vetada.push_back(vetada);
        resultado.perderam.push_back(std::move(perderam));
    }
    return resultado;
}

std::vector<std::string> rodarAsFaccoesNoDiaNovo(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    const CatalogoDasFaccoes& catalogo = dados.faccoes;
    EstadoDasFaccoes& estado = vila.faccoes;
    std::vector<std::string> linhas;
    if (estado.leisAtuais.empty()) {
        return linhas;
    }
    const int64_t hoje = vila.relogio.diaDaPartida();
    // As leis que a assembleia de ontem mudou passam a valer.
    for (const MudancaDeLei& mudanca : estado.mudancasParaAmanha) {
        estado.leisAtuais[static_cast<std::size_t>(mudanca.faccao)][static_cast<std::size_t>(mudanca.lei)] = mudanca.valor;
    }
    estado.mudancasParaAmanha.clear();

    for (int faccao = 0; faccao < static_cast<int>(catalogo.faccoes.size()); ++faccao) {
        const std::vector<SimulacaoDaVila*> cidades = cidadesDaFaccao(vila, faccao);
        // 4.4, passo 2: a cunhagem (nesta versão, uma Casa da Moeda em cada capital).
        int casasDaMoeda = 0;
        for (const SimulacaoDaVila* cidade : cidades) {
            casasDaMoeda += cidade->vila.nivelDaCidade == 4 ? 1 : 0;
        }
        const int64_t cunhagem = static_cast<int64_t>(casasDaMoeda) * catalogo.cunhagemPorCasaDaMoedaPorDiaEmDecimos;
        estado.bancoDaFaccao[static_cast<std::size_t>(faccao)] += cunhagem;
        estado.cunhagemTotal += cunhagem;
        // 4.4, passo 3: a verba no dia 1 de cada estação.
        if (vila.relogio.diaDaEstacaoAtual() == 1) {
            std::vector<int64_t> verbas;
            for (const SimulacaoDaVila* cidade : cidades) {
                verbas.push_back(static_cast<int64_t>(cidade->npcsDaVila.size()) * catalogo.verbaPorNpcPorEstacaoEmDecimos);
            }
            const std::vector<int64_t> pagas = dividirAVerba(estado.bancoDaFaccao[static_cast<std::size_t>(faccao)], verbas);
            for (std::size_t cidade = 0; cidade < cidades.size(); ++cidade) {
                cidades[cidade]->dinheiro.bancoDaCidade += pagas[cidade];
                estado.bancoDaFaccao[static_cast<std::size_t>(faccao)] -= pagas[cidade];
            }
        }
        // 4.5: a assembleia no dia 1 de cada ano (não no primeiro dia da partida).
        if (hoje > 0 && vila.relogio.diaDoAnoAtual() == 0) {
            std::vector<VotanteDaAssembleia> votantes;
            const DefinicaoDaFaccao& definicao = catalogo.faccoes[static_cast<std::size_t>(faccao)];
            for (const SimulacaoDaVila* cidade : cidades) {
                const MapaDoMundo& mapa = dados.mundo.mapa(cidade->numeroDoMapaNoMundo());
                votantes.push_back({definicao.cultura, cidade->felicidade.felicidadeEmDecimos / 10, static_cast<int>(cidade->npcsDaVila.size()), cidade->vila.nivelDaCidade,
                                    mapa.coluna == definicao.colunaDaCapital && mapa.linha == definicao.linhaDaCapital, cidade->numeroDoMapaNoMundo()});
            }
            // O veto sorteia com a sequência 7, a partir do ano e da facção (nada a guardar no save).
            SorteioComSemente sorteio(vila.vila.sementeDaPartida + static_cast<uint64_t>(vila.relogio.anoAtual()) * 16 + static_cast<uint64_t>(faccao), sequenciaDoSorteioDasFaccoes);
            const ResultadoDaAssembleia resultado = fazerAssembleia(catalogo, estado.leisAtuais[static_cast<std::size_t>(faccao)], votantes, definicao.cultura,
                                                                    estado.lideres[static_cast<std::size_t>(faccao)], [&]() { return sorteio.sortearNumeroInteiroEntre(0, 99); });
            for (std::size_t item = 0; item < resultado.pauta.size(); ++item) {
                for (const int mapa : resultado.perderam[item]) {
                    estado.perdeuAVotacaoAte[static_cast<std::size_t>(mapa - 1)] = hoje + vila.relogio.diasPorEstacao() - 1;
                }
                if (!resultado.passou[item] || resultado.vetada[item]) {
                    continue;
                }
                const DefinicaoDaLei& lei = catalogo.leis[static_cast<std::size_t>(resultado.pauta[item])];
                estado.mudancasParaAmanha.push_back({faccao, resultado.pauta[item], resultado.novoValor[item]});
                Noticia noticia;
                noticia.tipo = "faccao.lei";
                noticia.importancia = ImportanciaDaNoticia::Regional;
                noticia.mapaDeOrigem = dados.mundo.numeroDoMapaNa(definicao.colunaDaCapital, definicao.linhaDaCapital);
                noticia.tickDeOrigem = vila.relogio.tickAtual();
                noticia.diaDeOrigem = hoje;
                noticia.titulo = "A " + nomeDaFaccao(dados, faccao) + " mudou a lei " + textoDaOpcao(lei.id) + " para "
                    + textoDaOpcao(lei.opcoes[static_cast<std::size_t>(resultado.novoValor[item])]);
                criarNoticia(vila, std::move(noticia));
            }
        }
    }
    atualizarAsCidadesPelasLeis(vila);
    return linhas;
}
