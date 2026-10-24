#include "Tecnologia/Tecnologia.h"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "Comercio/Comercio.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/ProblemasDosDados.h"
#include "Noticias/Noticias.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDasTecnologias = "tecnologias.json";
const char* const nomeDoArquivoDosMarcos = "marcos.json";
// Os marcos sorteiam com a sequência 6, uma vez, ao criar o mundo (parte 2 da ficha).
constexpr uint64_t sequenciaDoSorteioDosMarcos = 6;
constexpr int diasPorAno = 64;

std::vector<std::string> lerListaDeTextos(const Json& objeto, const char* campo)
{
    std::vector<std::string> lista;
    if (const auto valor = objeto.find(campo); valor != objeto.end() && valor->is_array()) {
        for (const Json& texto : *valor) {
            if (texto.is_string()) {
                lista.push_back(texto.get<std::string>());
            }
        }
    }
    return lista;
}

// Confere se o que o "libera" aponta existe (teste 7).
std::vector<LiberacaoDaTecnologia> lerOQueLibera(const Json& objeto, const DadosDoJogo& dados, const char* arquivo, const std::string& dono,
                                                 ListaDeProblemasDosDados& listaDeProblemas)
{
    std::vector<LiberacaoDaTecnologia> libera;
    const auto lista = objeto.find("libera");
    if (lista == objeto.end() || !lista->is_array()) {
        return libera;
    }
    for (const Json& entrada : *lista) {
        LiberacaoDaTecnologia liberacao{entrada.value("tipo", std::string()), entrada.value("id", std::string())};
        bool existe = false;
        if (liberacao.tipo == "construcao") {
            existe = dados.catalogoDeConstrucoes.procurarConstrucao(liberacao.id) != nullptr;
        } else if (liberacao.tipo == "receita") {
            for (const DefinicaoDeReceita& receita : dados.catalogoDeReceitas.todasAsReceitas()) {
                existe = existe || receita.identificadorDeTexto == liberacao.id;
            }
        } else if (liberacao.tipo == "item") {
            existe = dados.catalogoDeItens.procurarNumeroDoItem(liberacao.id).has_value();
        } else if (liberacao.tipo == "nivel_da_cidade") {
            existe = !liberacao.id.empty();
        }
        if (!existe) {
            listaDeProblemas.anotarErro(arquivo, 0, dono + " libera \"" + liberacao.tipo + "\" \"" + liberacao.id + "\", que não existe");
            continue;
        }
        libera.push_back(std::move(liberacao));
    }
    return libera;
}

int posicaoDaTecnologia(const std::vector<DefinicaoDeTecnologia>& tecnologias, const std::string& id)
{
    for (std::size_t posicao = 0; posicao < tecnologias.size(); ++posicao) {
        if (tecnologias[posicao].identificadorDeTexto == id) {
            return static_cast<int>(posicao);
        }
    }
    return -1;
}

void lerOBlocoDoRegras(const std::string& textoDoRegras, RegrasDaTecnologia& regras, ListaDeProblemasDosDados& listaDeProblemas)
{
    Json documento;
    try {
        documento = Json::parse(textoDoRegras);
    } catch (const Json::parse_error&) {
        return;  // a leitura das regras já anotou o erro
    }
    const auto bloco = documento.find("tecnologia");
    if (bloco == documento.end() || !bloco->is_object()) {
        listaDeProblemas.anotarErro("regras.json", 0, "falta o bloco \"tecnologia\"");
        return;
    }
    if (const auto custos = bloco->find("custo_por_era"); custos != bloco->end() && custos->is_array() && !custos->empty()) {
        regras.custoPorEra = custos->get<std::vector<int>>();
    }
    const auto lerInteiro = [&](const char* campo, int& valor) {
        if (const auto lido = bloco->find(campo); lido != bloco->end() && lido->is_number_integer()) {
            valor = lido->get<int>();
        }
    };
    lerInteiro("difusao_por_faccao_em_porcento", regras.difusaoPorFaccaoEmPorcento);
    lerInteiro("difusao_maxima_em_porcento", regras.difusaoMaximaEmPorcento);
    lerInteiro("vila_sem_faccao_custo_em_porcento", regras.vilaSemFaccaoCustoEmPorcento);
    lerInteiro("horas_por_ponto", regras.horasPorPonto);
    lerInteiro("tecnologias_por_dia_no_maximo", regras.tecnologiasPorDiaNoMaximo);
    lerInteiro("nivel_do_laboratorio", regras.nivelDoLaboratorio);
    if (const auto prioridades = bloco->find("prioridade_dos_ramos_por_cultura"); prioridades != bloco->end() && prioridades->is_object()) {
        for (const auto& [cultura, ramos] : prioridades->items()) {
            regras.prioridadeDosRamosPorCultura[cultura] = ramos.is_array() ? ramos.get<std::vector<std::string>>() : std::vector<std::string>{};
        }
    }
    if (const auto cidades = bloco->find("outras_cidades_com_laboratorio"); cidades != bloco->end() && cidades->is_array()) {
        for (const Json& par : *cidades) {
            if (par.is_array() && par.size() == 2) {
                regras.outrasCidadesComLaboratorio.emplace_back(par[0].get<int>(), par[1].get<int>());
            }
        }
    }
}

std::string nomeDoTexto(const DadosDoJogo& dados, const std::string& id)
{
    return dados.tabelaDeTextos.textoDe(id);
}

// O mapa de onde nasce a notícia de uma facção: a capital (a primeira cidade de nível 4 dela).
int mapaDaCapital(const DadosDoJogo& dados, int pesquisador)
{
    const std::vector<std::string> faccoes = faccoesQuePesquisam(dados);
    if (pesquisador < 0 || pesquisador >= static_cast<int>(faccoes.size())) {
        return dados.mundo.mapaDaVila;
    }
    int primeira = 0;
    for (const CidadeDoMundo& cidade : dados.mundo.cidades) {
        if (cidade.faccao == faccoes[static_cast<std::size_t>(pesquisador)]) {
            primeira = primeira == 0 ? cidade.numeroDoMapa : primeira;
            if (cidade.nivel == 4) {
                return cidade.numeroDoMapa;
            }
        }
    }
    return primeira;
}

std::string nomeDoPesquisador(const DadosDoJogo& dados, int pesquisador)
{
    const std::vector<std::string> faccoes = faccoesQuePesquisam(dados);
    if (pesquisador < 0 || pesquisador >= static_cast<int>(faccoes.size())) {
        return "Sua vila";
    }
    return "A " + nomeDoDonoDaReputacao(dados.regrasDaReputacao, faccoes[static_cast<std::size_t>(pesquisador)]);
}

int faccoesQueTem(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int tecnologia)
{
    const int faccoes = static_cast<int>(faccoesQuePesquisam(dados).size());
    int quantas = 0;
    for (int faccao = 0; faccao < faccoes && faccao < static_cast<int>(estado.temATecnologia.size()); ++faccao) {
        quantas += estado.temATecnologia[static_cast<std::size_t>(faccao)][static_cast<std::size_t>(tecnologia)] ? 1 : 0;
    }
    return quantas;
}

bool estaDisponivel(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int pesquisador, int tecnologia)
{
    const DefinicaoDeTecnologia& definicao = dados.tecnologia.tecnologias[static_cast<std::size_t>(tecnologia)];
    const std::vector<bool>& tem = estado.temATecnologia[static_cast<std::size_t>(pesquisador)];
    if (tem[static_cast<std::size_t>(tecnologia)] || definicao.era > estado.eraAtualDoMundo) {
        return false;
    }
    return std::all_of(definicao.preRequisitos.begin(), definicao.preRequisitos.end(), [&](int requisito) { return tem[static_cast<std::size_t>(requisito)]; });
}

bool cidadeTemLaboratorio(const SimulacaoDaVila& cidade)
{
    for (const ConstrucaoDaVila& construcao : cidade.vila.construcoes) {
        const DefinicaoDeConstrucao* definicao = cidade.dados.catalogoDeConstrucoes.procurarConstrucao(construcao.tipo);
        if (definicao != nullptr && definicao->produzPontosDePesquisa) {
            return true;
        }
    }
    return false;
}

}  // namespace

CatalogoDaTecnologia lerArquivosDaTecnologia(const std::string& textoDasTecnologias, const std::string& textoDosMarcos, const std::string& textoDoRegras,
                                             const DadosDoJogo& dados, ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDaTecnologia catalogo;
    lerOBlocoDoRegras(textoDoRegras, catalogo.regras, listaDeProblemas);
    Json tecnologias;
    Json marcos;
    try {
        tecnologias = Json::parse(textoDasTecnologias);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDasTecnologias, 0, std::string("o JSON está quebrado: ") + erro.what());
    }
    try {
        marcos = Json::parse(textoDosMarcos);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDosMarcos, 0, std::string("o JSON está quebrado: ") + erro.what());
    }
    // As tecnologias: primeiro os ids (os pré-requisitos podem vir depois no arquivo).
    std::vector<std::vector<std::string>> preRequisitos;
    for (const Json& objeto : tecnologias.is_array() ? tecnologias : Json::array()) {
        DefinicaoDeTecnologia tecnologia;
        tecnologia.identificadorDeTexto = objeto.value("id", std::string());
        tecnologia.ramo = objeto.value("ramo", std::string());
        tecnologia.era = objeto.value("era", 1);
        tecnologia.custo = objeto.value("custo", 0);
        if (tecnologia.identificadorDeTexto.empty() || tecnologia.era < 1 || tecnologia.era > static_cast<int>(catalogo.regras.custoPorEra.size())) {
            listaDeProblemas.anotarErro(nomeDoArquivoDasTecnologias, 0, "cada tecnologia precisa de \"id\" e de uma \"era\" de 1 a " + std::to_string(catalogo.regras.custoPorEra.size()));
            continue;
        }
        tecnologia.libera = lerOQueLibera(objeto, dados, nomeDoArquivoDasTecnologias, tecnologia.identificadorDeTexto, listaDeProblemas);
        tecnologia.culturasQueComecamCom = lerListaDeTextos(objeto, "culturas_que_comecam_com");
        preRequisitos.push_back(lerListaDeTextos(objeto, "pre_requisitos"));
        catalogo.tecnologias.push_back(std::move(tecnologia));
    }
    for (std::size_t posicao = 0; posicao < catalogo.tecnologias.size(); ++posicao) {
        for (const std::string& requisito : preRequisitos[posicao]) {
            const int encontrada = posicaoDaTecnologia(catalogo.tecnologias, requisito);
            if (encontrada < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasTecnologias, 0, catalogo.tecnologias[posicao].identificadorDeTexto + " pede " + requisito + ", que não existe");
                continue;
            }
            catalogo.tecnologias[posicao].preRequisitos.push_back(encontrada);
        }
    }
    for (const Json& objeto : marcos.is_array() ? marcos : Json::array()) {
        DefinicaoDeMarco marco;
        marco.identificadorDeTexto = objeto.value("id", std::string());
        const auto janela = objeto.find("janela_em_anos");
        if (marco.identificadorDeTexto.empty() || janela == objeto.end() || !janela->is_array() || janela->size() != 2) {
            listaDeProblemas.anotarErro(nomeDoArquivoDosMarcos, 0, "cada marco precisa de \"id\" e de \"janela_em_anos\": [primeiro, último]");
            continue;
        }
        marco.primeiroAno = (*janela)[0].get<int>();
        marco.ultimoAno = (*janela)[1].get<int>();
        if (marco.primeiroAno < 1 || marco.ultimoAno < marco.primeiroAno) {
            listaDeProblemas.anotarErro(nomeDoArquivoDosMarcos, 0, marco.identificadorDeTexto + ": a janela precisa ir de um ano 1 ou mais até um ano igual ou maior");
        }
        for (const std::string& ligada : lerListaDeTextos(objeto, "tecnologias_ligadas")) {
            const int encontrada = posicaoDaTecnologia(catalogo.tecnologias, ligada);
            if (encontrada < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDosMarcos, 0, marco.identificadorDeTexto + " liga " + ligada + ", que não existe");
                continue;
            }
            marco.tecnologiasLigadas.push_back(encontrada);
        }
        marco.abreEra = objeto.value("abre_era", 0);
        marco.libera = lerOQueLibera(objeto, dados, nomeDoArquivoDosMarcos, marco.identificadorDeTexto, listaDeProblemas);
        catalogo.marcos.push_back(std::move(marco));
    }
    return catalogo;
}

std::vector<std::string> faccoesQuePesquisam(const DadosDoJogo& dados)
{
    std::vector<std::string> faccoes;
    for (const CidadeDoMundo& cidade : dados.mundo.cidades) {
        if (!cidade.faccao.empty() && std::find(faccoes.begin(), faccoes.end(), cidade.faccao) == faccoes.end()) {
            faccoes.push_back(cidade.faccao);
        }
    }
    return faccoes;
}

int pesquisadorDaCidade(const DadosDoJogo& dados, int numeroDoMapa)
{
    const std::vector<std::string> faccoes = faccoesQuePesquisam(dados);
    if (numeroDoMapa == dados.mundo.mapaDaVila) {
        return static_cast<int>(faccoes.size());  // a vila do jogador, por último
    }
    const CidadeDoMundo* cidade = dados.mundo.cidadeNoMapa(numeroDoMapa);
    if (cidade == nullptr || cidade->faccao.empty()) {
        return -1;
    }
    return static_cast<int>(std::find(faccoes.begin(), faccoes.end(), cidade->faccao) - faccoes.begin());
}

int64_t primeiroDiaDoMarco(const DefinicaoDeMarco& marco)
{
    return static_cast<int64_t>(marco.primeiroAno - 1) * diasPorAno;
}

int64_t ultimoDiaDoMarco(const DefinicaoDeMarco& marco)
{
    return static_cast<int64_t>(marco.ultimoAno) * diasPorAno - 1;
}

void montarATecnologiaDaPartidaNova(EstadoDaTecnologia& estado, const DadosDoJogo& dados, uint64_t sementeDaPartida)
{
    const std::vector<std::string> faccoes = faccoesQuePesquisam(dados);
    const std::size_t pesquisadores = faccoes.size() + 1;
    const std::size_t tecnologias = dados.tecnologia.tecnologias.size();
    estado = EstadoDaTecnologia{};
    estado.temATecnologia.assign(pesquisadores, std::vector<bool>(tecnologias, false));
    estado.projetoAtual.assign(pesquisadores, -1);
    estado.pontosAcumulados.assign(pesquisadores, 0);
    // 4.6: cada facção começa com as tecnologias da sua cultura (a vila do jogador, sem nenhuma).
    for (std::size_t faccao = 0; faccao < faccoes.size(); ++faccao) {
        for (std::size_t tecnologia = 0; tecnologia < tecnologias; ++tecnologia) {
            const std::vector<std::string>& culturas = dados.tecnologia.tecnologias[tecnologia].culturasQueComecamCom;
            estado.temATecnologia[faccao][tecnologia] = std::find(culturas.begin(), culturas.end(), faccoes[faccao]) != culturas.end();
        }
    }
    // 4.4, passo 1: o dia de cada marco, dentro da janela.
    SorteioComSemente sorteio(sementeDaPartida, sequenciaDoSorteioDosMarcos);
    for (const DefinicaoDeMarco& marco : dados.tecnologia.marcos) {
        estado.diaSorteadoPorMarco.push_back(sorteio.sortearNumeroInteiroEntre(static_cast<int>(primeiroDiaDoMarco(marco)), static_cast<int>(ultimoDiaDoMarco(marco))));
        estado.marcoChegou.push_back(false);
    }
    for (std::size_t faccao = 0; faccao < faccoes.size(); ++faccao) {
        estado.projetoAtual[faccao] = escolherProjetoDaFaccao(estado, dados, static_cast<int>(faccao));
    }
}

int custoComOsNumeros(int custoDaTecnologia, int faccoesQueJaTem, bool ehAVilaDoJogador, const RegrasDaTecnologia& regras)
{
    const int desconto = std::min(regras.difusaoPorFaccaoEmPorcento * faccoesQueJaTem, regras.difusaoMaximaEmPorcento);
    const int custo = custoDaTecnologia * (100 - desconto) / 100;
    return ehAVilaDoJogador ? custo * regras.vilaSemFaccaoCustoEmPorcento / 100 : custo;
}

int custoAtualDaTecnologia(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int pesquisador, int tecnologia)
{
    const DefinicaoDeTecnologia& definicao = dados.tecnologia.tecnologias[static_cast<std::size_t>(tecnologia)];
    const RegrasDaTecnologia& regras = dados.tecnologia.regras;
    const int custo = definicao.custo > 0 ? definicao.custo : regras.custoPorEra[static_cast<std::size_t>(definicao.era - 1)];
    const bool ehAVila = pesquisador == static_cast<int>(faccoesQuePesquisam(dados).size());
    return custoComOsNumeros(custo, faccoesQueTem(estado, dados, tecnologia), ehAVila, regras);
}

int escolherProjetoDaFaccao(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int pesquisador)
{
    const std::vector<std::string> faccoes = faccoesQuePesquisam(dados);
    // A vila do jogador, se ele não escolher, segue a ordem da Tecnocrata (4.3).
    const std::string cultura = pesquisador < static_cast<int>(faccoes.size()) ? faccoes[static_cast<std::size_t>(pesquisador)] : "tecnocrata";
    const auto prioridades = dados.tecnologia.regras.prioridadeDosRamosPorCultura.find(cultura);
    const auto lugarDoRamo = [&](const std::string& ramo) {
        if (prioridades == dados.tecnologia.regras.prioridadeDosRamosPorCultura.end()) {
            return 0;
        }
        const auto encontrado = std::find(prioridades->second.begin(), prioridades->second.end(), ramo);
        return static_cast<int>(encontrado - prioridades->second.begin());
    };
    int melhor = -1;
    for (int tecnologia = 0; tecnologia < static_cast<int>(dados.tecnologia.tecnologias.size()); ++tecnologia) {
        if (!estaDisponivel(estado, dados, pesquisador, tecnologia)) {
            continue;
        }
        if (melhor < 0) {
            melhor = tecnologia;
            continue;
        }
        const int lugar = lugarDoRamo(dados.tecnologia.tecnologias[static_cast<std::size_t>(tecnologia)].ramo);
        const int lugarDoMelhor = lugarDoRamo(dados.tecnologia.tecnologias[static_cast<std::size_t>(melhor)].ramo);
        if (lugar < lugarDoMelhor
            || (lugar == lugarDoMelhor && custoAtualDaTecnologia(estado, dados, pesquisador, tecnologia) < custoAtualDaTecnologia(estado, dados, pesquisador, melhor))) {
            melhor = tecnologia;
        }
    }
    return melhor;
}

int64_t dataDoMarcoComOsNumeros(int64_t diaSorteado, int64_t primeiroDia, int faccoesCompletas, int faccoes)
{
    if (faccoes <= 0) {
        return diaSorteado;
    }
    // 280 − 151 × 3 ÷ 6 = 204,5 → 204: a conta inteira, arredondada para baixo no fim.
    const int64_t data = (diaSorteado * faccoes - (diaSorteado - primeiroDia) * faccoesCompletas) / faccoes;
    return std::max(data, primeiroDia);
}

int64_t dataAtualDoMarco(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int marco)
{
    const DefinicaoDeMarco& definicao = dados.tecnologia.marcos[static_cast<std::size_t>(marco)];
    const int faccoes = static_cast<int>(faccoesQuePesquisam(dados).size());
    int completas = 0;
    for (int faccao = 0; faccao < faccoes; ++faccao) {
        const std::vector<bool>& tem = estado.temATecnologia[static_cast<std::size_t>(faccao)];
        completas += std::all_of(definicao.tecnologiasLigadas.begin(), definicao.tecnologiasLigadas.end(), [&](int ligada) { return tem[static_cast<std::size_t>(ligada)]; }) ? 1 : 0;
    }
    return dataDoMarcoComOsNumeros(estado.diaSorteadoPorMarco[static_cast<std::size_t>(marco)], primeiroDiaDoMarco(definicao), completas, faccoes);
}

bool estaLiberado(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int pesquisador, const std::string& tipo, const std::string& id)
{
    const auto libera = [&](const std::vector<LiberacaoDaTecnologia>& lista) {
        return std::any_of(lista.begin(), lista.end(), [&](const LiberacaoDaTecnologia& liberacao) { return liberacao.tipo == tipo && liberacao.id == id; });
    };
    bool apareceEmAlgum = false;
    for (std::size_t marco = 0; marco < dados.tecnologia.marcos.size(); ++marco) {
        if (libera(dados.tecnologia.marcos[marco].libera)) {
            apareceEmAlgum = true;
            if (marco < estado.marcoChegou.size() && estado.marcoChegou[marco]) {
                return true;  // o marco vale para todos
            }
        }
    }
    for (std::size_t tecnologia = 0; tecnologia < dados.tecnologia.tecnologias.size(); ++tecnologia) {
        if (libera(dados.tecnologia.tecnologias[tecnologia].libera)) {
            apareceEmAlgum = true;
            if (pesquisador >= 0 && pesquisador < static_cast<int>(estado.temATecnologia.size())
                && estado.temATecnologia[static_cast<std::size_t>(pesquisador)][tecnologia]) {
                return true;
            }
        }
    }
    return !apareceEmAlgum;
}

void atualizarAsTravasDasCidades(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        const int pesquisador = pesquisadorDaCidade(dados, cidade->numeroDoMapaNoMundo());
        std::vector<bool>& liberadas = cidade->receitasLiberadas;
        liberadas.assign(dados.catalogoDeReceitas.todasAsReceitas().size(), true);
        for (std::size_t receita = 0; receita < liberadas.size(); ++receita) {
            liberadas[receita] = estaLiberado(vila.tecnologia, dados, pesquisador, "receita", dados.catalogoDeReceitas.todasAsReceitas()[receita].identificadorDeTexto);
        }
    }
}

std::vector<std::string> somarPontosEConcluirTecnologias(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    EstadoDaTecnologia& estado = vila.tecnologia;
    std::vector<std::string> linhas;
    if (estado.temATecnologia.empty()) {
        return linhas;
    }
    const int vilaDoJogador = static_cast<int>(faccoesQuePesquisam(dados).size());
    // 4.2, passos 1 e 2: os pontos dos Laboratórios de cada cidade vão para o dono.
    std::vector<bool> temLaboratorio(estado.pontosAcumulados.size(), false);
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        const int pesquisador = pesquisadorDaCidade(dados, cidade->numeroDoMapaNoMundo());
        int pontos = 0;
        for (ProducaoDaConstrucao& producao : cidade->producaoDasConstrucoes) {
            pontos += producao.pontosDePesquisaGuardados;
            producao.pontosDePesquisaGuardados = 0;
        }
        if (pesquisador >= 0) {
            estado.pontosAcumulados[static_cast<std::size_t>(pesquisador)] += pontos;
            temLaboratorio[static_cast<std::size_t>(pesquisador)] = temLaboratorio[static_cast<std::size_t>(pesquisador)] || cidadeTemLaboratorio(*cidade);
        }
    }
    bool algumaCompletou = false;
    for (int pesquisador = 0; pesquisador < static_cast<int>(estado.pontosAcumulados.size()); ++pesquisador) {
        // A vila do jogador só pesquisa com um Laboratório (4.1).
        if (pesquisador == vilaDoJogador && !temLaboratorio[static_cast<std::size_t>(pesquisador)]) {
            continue;
        }
        for (int completadas = 0; completadas < dados.tecnologia.regras.tecnologiasPorDiaNoMaximo; ++completadas) {
            int& projeto = estado.projetoAtual[static_cast<std::size_t>(pesquisador)];
            if (projeto < 0) {
                projeto = escolherProjetoDaFaccao(estado, dados, pesquisador);
            }
            if (projeto < 0) {
                break;  // nada disponível: os pontos ficam guardados
            }
            const int custo = custoAtualDaTecnologia(estado, dados, pesquisador, projeto);
            int64_t& pontos = estado.pontosAcumulados[static_cast<std::size_t>(pesquisador)];
            if (pontos < custo) {
                break;
            }
            // Passo 4: completa; a sobra fica; nasce a notícia Regional.
            pontos -= custo;
            estado.temATecnologia[static_cast<std::size_t>(pesquisador)][static_cast<std::size_t>(projeto)] = true;
            const std::string tecnologia = nomeDoTexto(dados, dados.tecnologia.tecnologias[static_cast<std::size_t>(projeto)].identificadorDeTexto);
            Noticia noticia;
            noticia.tipo = "tecnologia.descoberta";
            noticia.importancia = ImportanciaDaNoticia::Regional;
            noticia.mapaDeOrigem = mapaDaCapital(dados, pesquisador);
            noticia.tickDeOrigem = vila.relogio.tickAtual();
            noticia.diaDeOrigem = vila.relogio.diaDaPartida();
            noticia.titulo = nomeDoPesquisador(dados, pesquisador) + " descobriu " + tecnologia;
            criarNoticia(vila, std::move(noticia));
            if (pesquisador == vilaDoJogador) {
                linhas.push_back("Tecnologia: sua vila descobriu " + tecnologia + ".");
            }
            projeto = -1;
            algumaCompletou = true;
        }
    }
    // 4.4, passos 2 e 3: os marcos.
    const int64_t hoje = vila.relogio.diaDaPartida();
    for (int marco = 0; marco < static_cast<int>(dados.tecnologia.marcos.size()); ++marco) {
        if (estado.marcoChegou[static_cast<std::size_t>(marco)] || hoje < dataAtualDoMarco(estado, dados, marco)) {
            continue;
        }
        const DefinicaoDeMarco& definicao = dados.tecnologia.marcos[static_cast<std::size_t>(marco)];
        estado.marcoChegou[static_cast<std::size_t>(marco)] = true;
        estado.eraAtualDoMundo = std::max(estado.eraAtualDoMundo, definicao.abreEra);
        Noticia noticia;
        noticia.tipo = "tecnologia.marco";
        noticia.importancia = ImportanciaDaNoticia::Mundial;
        noticia.mapaDeOrigem = vila.viagem.grupoDoJogador.mapaAtual;  // chega ao mundo todo de uma vez
        noticia.tickDeOrigem = vila.relogio.tickAtual();
        noticia.diaDeOrigem = hoje;
        noticia.titulo = "Marco mundial: " + nomeDoTexto(dados, definicao.identificadorDeTexto)
            + (definicao.abreEra > 0 ? " (começa a era " + std::to_string(definicao.abreEra) + ")" : "");
        criarNoticia(vila, std::move(noticia));
        algumaCompletou = true;
    }
    if (algumaCompletou) {
        atualizarAsTravasDasCidades(vila);
    }
    return linhas;
}
