#include "Reputacao/Reputacao.h"

#include <algorithm>
#include <set>

#include <nlohmann/json.hpp>

#include "Comercio/Comercio.h"
#include "Dados/ProblemasDosDados.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDaReputacao = "reputacao.json";

std::string escreverComSinal(int valor)
{
    return (valor > 0 ? "+" : "") + std::to_string(valor);
}

FaixaDaReputacao faixaNeutra()
{
    return FaixaDaReputacao{"neutro", "Neutro", 19, 100, 100};
}

}  // namespace

int RegrasDaReputacao::acao(const std::string& nome) const
{
    const auto encontrada = acoes.find(nome);
    return encontrada == acoes.end() ? 0 : encontrada->second;
}

RegrasDaReputacao lerArquivoDaReputacao(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDaReputacao regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaReputacao, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    if (!documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaReputacao, 0, "o arquivo precisa ser um objeto { ... }");
        return regras;
    }
    const auto faixas = documento.find("faixas");
    if (faixas == documento.end() || !faixas->is_array() || faixas->empty()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaReputacao, 0, "falta a lista \"faixas\"");
    } else {
        for (const Json& faixa : *faixas) {
            FaixaDaReputacao lida;
            const auto porcento = [&](const char* campo) {
                const auto valor = faixa.find(campo);
                return valor == faixa.end() || valor->is_null() ? -1 : valor->get<int>();
            };
            if (!faixa.is_object() || !faixa.contains("nome") || !faixa.contains("ate") || !faixa["ate"].is_number_integer()) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaReputacao, 0, "cada faixa precisa de \"nome\" e \"ate\"");
                continue;
            }
            lida.nome = faixa["nome"].get<std::string>();
            lida.texto = faixa.value("texto", lida.nome);
            lida.ate = faixa["ate"].get<int>();
            lida.compraEmPorcento = porcento("compra_em_porcento");
            lida.vendaEmPorcento = porcento("venda_em_porcento");
            if (!regras.faixas.empty() && lida.ate <= regras.faixas.back().ate) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaReputacao, 0, "as faixas precisam estar em ordem crescente de \"ate\"");
            }
            regras.faixas.push_back(std::move(lida));
        }
        if (!regras.faixas.empty() && regras.faixas.back().ate != 100) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaReputacao, 0, "a última faixa precisa ir até 100");
        }
    }
    if (const auto volta = documento.find("volta_por_estacao"); volta != documento.end() && volta->is_number_integer()) {
        regras.voltaPorEstacao = volta->get<int>();
    }
    if (const auto acoes = documento.find("acoes"); acoes != documento.end() && acoes->is_object()) {
        for (const auto& [nome, valor] : acoes->items()) {
            if (valor.is_number_integer()) {
                regras.acoes[nome] = valor.get<int>();
            } else {
                listaDeProblemas.anotarErro(nomeDoArquivoDaReputacao, 0, "a ação \"" + nome + "\" precisa ser um inteiro");
            }
        }
    }
    if (const auto nomes = documento.find("nomes_das_faccoes"); nomes != documento.end() && nomes->is_object()) {
        for (const auto& [faccao, nome] : nomes->items()) {
            regras.nomesDasFaccoes[faccao] = nome.is_string() ? nome.get<std::string>() : faccao;
        }
    }
    if (const auto artigos = documento.find("artigos_das_faccoes"); artigos != documento.end() && artigos->is_object()) {
        for (const auto& [faccao, artigo] : artigos->items()) {
            if (artigo.is_string()) {
                regras.artigosDasFaccoes[faccao] = artigo.get<std::string>();
            }
        }
    }
    if (const auto mudancas = documento.find("mudancas_no_por_que"); mudancas != documento.end() && mudancas->is_number_integer()) {
        regras.mudancasNoPorQue = std::max(1, mudancas->get<int>());
    }
    return regras;
}

std::vector<std::string> donosDaReputacao(const MundoDoJogo& mundo)
{
    std::vector<std::string> donos;
    for (const CidadeDoMundo& cidade : mundo.cidades) {
        const std::string dono = cidade.faccao.empty() ? cidade.nome : cidade.faccao;
        if (std::find(donos.begin(), donos.end(), dono) == donos.end()) {
            donos.push_back(dono);
        }
    }
    return donos;
}

std::string nomeDoDonoDaReputacao(const RegrasDaReputacao& regras, const std::string& dono)
{
    const auto nome = regras.nomesDasFaccoes.find(dono);
    return nome == regras.nomesDasFaccoes.end() ? dono : nome->second;
}

std::string nomeDoDonoComArtigo(const RegrasDaReputacao& regras, const std::string& dono, const std::string& preposicao, bool maiuscula)
{
    const auto encontrado = regras.artigosDasFaccoes.find(dono);
    const std::string artigo = encontrado == regras.artigosDasFaccoes.end() ? "a" : encontrado->second;
    std::string junto = artigo;  // "a", "o", "os"
    if (preposicao == "de") {
        junto = "d" + artigo;
    } else if (preposicao == "por") {
        junto = "pel" + artigo;
    } else if (preposicao == "em") {
        junto = "n" + artigo;
    } else if (!preposicao.empty()) {
        junto = preposicao + " " + artigo;
    }
    if (maiuscula && !junto.empty() && junto[0] >= 'a' && junto[0] <= 'z') {
        junto[0] = static_cast<char>(junto[0] - 'a' + 'A');
    }
    return junto + " " + nomeDoDonoDaReputacao(regras, dono);
}

void montarAReputacaoDaPartidaNova(EstadoDaReputacao& reputacao, const MundoDoJogo& mundo)
{
    const std::size_t donos = donosDaReputacao(mundo).size();
    reputacao.basePorFaccao.assign(donos, 0);
    reputacao.reputacaoPorFaccao = reputacao.basePorFaccao;
    reputacao.ultimasMudancas.assign(donos, {});
}

int donoDaCidadeDoMapa(const MundoDoJogo& mundo, int numeroDoMapa)
{
    const CidadeDoMundo* cidade = mundo.cidadeNoMapa(numeroDoMapa);
    if (cidade == nullptr || numeroDoMapa == mundo.mapaDaVila) {
        return -1;
    }
    const std::vector<std::string> donos = donosDaReputacao(mundo);
    const std::string dono = cidade->faccao.empty() ? cidade->nome : cidade->faccao;
    return static_cast<int>(std::find(donos.begin(), donos.end(), dono) - donos.begin());
}

int faccaoDonaDoTerritorio(const MundoDoJogo& mundo, int numeroDoMapa)
{
    std::set<std::string> faccoes;
    if (const CidadeDoMundo* cidade = mundo.cidadeNoMapa(numeroDoMapa)) {
        if (cidade->faccao.empty()) {
            return -1;
        }
        faccoes.insert(cidade->faccao);
    } else {
        for (const int vizinho : mundo.vizinhos(numeroDoMapa)) {
            const CidadeDoMundo* cidade = mundo.cidadeNoMapa(vizinho);
            if (cidade != nullptr && !cidade->faccao.empty()) {
                faccoes.insert(cidade->faccao);
            }
        }
    }
    if (faccoes.size() != 1) {
        return -1;  // em disputa ou sem dono
    }
    const std::vector<std::string> donos = donosDaReputacao(mundo);
    return static_cast<int>(std::find(donos.begin(), donos.end(), *faccoes.begin()) - donos.begin());
}

int faixaDaReputacao(const RegrasDaReputacao& regras, int valor)
{
    for (std::size_t faixa = 0; faixa < regras.faixas.size(); ++faixa) {
        if (valor <= regras.faixas[faixa].ate) {
            return static_cast<int>(faixa);
        }
    }
    return static_cast<int>(regras.faixas.size()) - 1;
}

const FaixaDaReputacao& faixaDoJogadorNaCidade(const SimulacaoDaVila& vila, int numeroDoMapa)
{
    static const FaixaDaReputacao neutra = faixaNeutra();
    const RegrasDaReputacao& regras = vila.dados.regrasDaReputacao;
    const int dono = donoDaCidadeDoMapa(vila.dados.mundo, numeroDoMapa);
    if (dono < 0 || dono >= static_cast<int>(vila.reputacao.reputacaoPorFaccao.size()) || regras.faixas.empty()) {
        return neutra;
    }
    return regras.faixas[static_cast<std::size_t>(faixaDaReputacao(regras, vila.reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)]))];
}

std::string mudarReputacao(SimulacaoDaVila& vila, int dono, int valor, const std::string& motivo)
{
    EstadoDaReputacao& reputacao = vila.reputacao;
    const RegrasDaReputacao& regras = vila.dados.regrasDaReputacao;
    if (dono < 0 || dono >= static_cast<int>(reputacao.reputacaoPorFaccao.size()) || valor == 0) {
        return "";
    }
    int& atual = reputacao.reputacaoPorFaccao[static_cast<std::size_t>(dono)];
    const int faixaAntes = faixaDaReputacao(regras, atual);
    const int novo = std::clamp(atual + valor, -100, 100);
    // Passo 2: as 5 últimas mudanças (o "Por quê?").
    std::vector<MudancaDaReputacao>& ultimas = reputacao.ultimasMudancas[static_cast<std::size_t>(dono)];
    ultimas.push_back({novo - atual, motivo, vila.relogio.diaDaPartida()});
    if (static_cast<int>(ultimas.size()) > regras.mudancasNoPorQue) {
        ultimas.erase(ultimas.begin());
    }
    atual = novo;
    const int faixaDepois = faixaDaReputacao(regras, atual);
    if (faixaDepois == faixaAntes || regras.faixas.empty()) {
        return "";
    }
    // Passo 3: a faixa mudou.
    const std::string nome = nomeDoDonoComArtigo(regras, donosDaReputacao(vila.dados.mundo)[static_cast<std::size_t>(dono)]);
    return "Reputação: sua reputação com " + nome + " agora é " + regras.faixas[static_cast<std::size_t>(faixaDepois)].texto + ".";
}

std::vector<std::string> voltarReputacaoParaABase(SimulacaoDaVila& vila)
{
    std::vector<std::string> linhas;
    const int volta = vila.dados.regrasDaReputacao.voltaPorEstacao;
    for (std::size_t dono = 0; dono < vila.reputacao.reputacaoPorFaccao.size(); ++dono) {
        const int diferenca = vila.reputacao.basePorFaccao[dono] - vila.reputacao.reputacaoPorFaccao[dono];
        if (diferenca == 0) {
            continue;
        }
        const int passo = std::abs(diferenca) <= volta ? diferenca : (diferenca > 0 ? volta : -volta);
        const std::string linha = mudarReputacao(vila, static_cast<int>(dono), passo, "a volta com o tempo");
        if (!linha.empty()) {
            linhas.push_back(linha);
        }
    }
    return linhas;
}

std::string doarParaAFaccao(SimulacaoDaVila& vila, int moedas, std::vector<std::string>& linhas)
{
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    const RegrasDaReputacao& regras = vila.dados.regrasDaReputacao;
    if (grupo.emTransito) {
        return "o grupo precisa estar parado numa cidade";
    }
    const int dono = donoDaCidadeDoMapa(vila.dados.mundo, grupo.mapaAtual);
    SimulacaoDaVila* cidade = nullptr;
    for (const std::unique_ptr<SimulacaoDaVila>& outra : vila.outrasCidades) {
        cidade = outra->numeroDoMapaDaCidade == grupo.mapaAtual ? outra.get() : cidade;
    }
    if (dono < 0 || cidade == nullptr) {
        return "só dá para doar numa outra cidade";
    }
    const int moedasPorPonto = 100;
    if (moedas < moedasPorPonto) {
        return "doe pelo menos 100 moedas";
    }
    const std::size_t posicao = static_cast<std::size_t>(dono);
    const int pontosAteOMaximo = vila.reputacao.basePorFaccao[posicao] + regras.acao("doacao_ate_acima_da_base") - vila.reputacao.reputacaoPorFaccao[posicao];
    const int pontosPorCem = std::max(1, regras.acao("doou_a_cada_100_moedas"));
    if (pontosAteOMaximo <= 0) {
        return "a reputação já está no máximo por doação";
    }
    // A sobra que não daria ponto não é aceita.
    const int pontos = std::min(moedas / moedasPorPonto * pontosPorCem, pontosAteOMaximo);
    const int aceitas = (pontos + pontosPorCem - 1) / pontosPorCem * moedasPorPonto;
    if (grupo.dinheiroEmDecimos < static_cast<int64_t>(aceitas) * 10) {
        return "o grupo não leva " + std::to_string(aceitas) + " moedas";
    }
    grupo.dinheiroEmDecimos -= static_cast<int64_t>(aceitas) * 10;
    cidade->dinheiro.bancoDaCidade += static_cast<int64_t>(aceitas) * 10;  // RE1-3: o Banco da cidade
    const std::string nome = nomeDoDonoComArtigo(regras, donosDaReputacao(vila.dados.mundo)[posicao], "com");
    linhas.push_back("Reputação: o grupo doou " + std::to_string(aceitas) + " moedas em " + vila.dados.mundo.nomeDoMapa(grupo.mapaAtual) + " ("
                     + escreverComSinal(pontos) + " " + nome + ")." + (aceitas < moedas ? " Com " + std::to_string(aceitas) + " moedas você chegou ao máximo por doação." : ""));
    const std::string faixa = mudarReputacao(vila, dono, pontos, "doou " + std::to_string(aceitas) + " moedas em " + vila.dados.mundo.nomeDoMapa(grupo.mapaAtual));
    if (!faixa.empty()) {
        linhas.push_back(faixa);
    }
    return "";
}

std::string anotarOBandoVencido(SimulacaoDaVila& vila)
{
    const int mapa = vila.viagem.grupoDoJogador.mapaAtual;
    const int dono = faccaoDonaDoTerritorio(vila.dados.mundo, mapa);
    return mudarReputacao(vila, dono, vila.dados.regrasDaReputacao.acao("venceu_bando_no_territorio"), "venceu um bando em " + vila.dados.mundo.nomeDoMapa(mapa));
}

int64_t precoComAFaixa(int64_t precoEmDecimos, int porcentagem)
{
    return (precoEmDecimos * porcentagem + 50) / 100;
}
