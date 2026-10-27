#include "Cultura/Cultura.h"

#include <algorithm>
#include <cstdlib>
#include <optional>
#include <set>

#include <nlohmann/json.hpp>

#include "Comercio/Comercio.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/ProblemasDosDados.h"
#include "Npc/ComponentesDoNpc.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDasCulturas = "culturas.json";
// O sorteio da cultura do migrante (CT1-6): a sequência 8, sem estado guardado.
constexpr uint64_t sequenciaDoSorteioDaCultura = 8;

std::optional<AfinidadeEntreCulturas> lerAfinidade(const std::string& texto)
{
    if (texto == "afim") {
        return AfinidadeEntreCulturas::Afim;
    }
    if (texto == "neutra") {
        return AfinidadeEntreCulturas::Neutra;
    }
    if (texto == "hostil") {
        return AfinidadeEntreCulturas::Hostil;
    }
    return std::nullopt;
}

}  // namespace

const DefinicaoDaCultura* CatalogoDasCulturas::procurar(const std::string& id) const
{
    for (const DefinicaoDaCultura& cultura : culturas) {
        if (cultura.id == id) {
            return &cultura;
        }
    }
    return nullptr;
}

AfinidadeEntreCulturas CatalogoDasCulturas::afinidade(const std::string& uma, const std::string& outra) const
{
    if (uma == outra) {
        return AfinidadeEntreCulturas::Mesma;
    }
    const DefinicaoDaCultura* cultura = procurar(uma);
    if (cultura == nullptr) {
        return AfinidadeEntreCulturas::Neutra;
    }
    const auto encontrada = cultura->afinidades.find(outra);
    return encontrada == cultura->afinidades.end() ? AfinidadeEntreCulturas::Neutra : encontrada->second;
}

CatalogoDasCulturas lerArquivoDasCulturas(const std::string& textoDoArquivo, const CatalogoDasFaccoes& faccoes, ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDasCulturas catalogo;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error&) {
        return catalogo;  // a leitura das facções já anotou o erro
    }
    for (const Json& objeto : documento.is_array() ? documento : Json::array()) {
        DefinicaoDaCultura cultura;
        cultura.id = objeto.value("id", std::string());
        if (cultura.id.rfind("cultura.", 0) == 0) {
            cultura.id = cultura.id.substr(8);
        }
        cultura.regiao = objeto.value("regiao", 0);
        if (const auto extremo = objeto.find("extremo"); extremo != objeto.end() && extremo->is_object()) {
            cultura.leiDoExtremo = faccoes.posicaoDaLei(extremo->value("lei", std::string()));
            if (cultura.leiDoExtremo >= 0) {
                const std::vector<std::string>& opcoes = faccoes.leis[static_cast<std::size_t>(cultura.leiDoExtremo)].opcoes;
                const auto opcao = std::find(opcoes.begin(), opcoes.end(), extremo->value("valor", std::string()));
                cultura.valorDoExtremo = opcao == opcoes.end() ? -1 : static_cast<int>(opcao - opcoes.begin());
            }
            if (cultura.leiDoExtremo < 0 || cultura.valorDoExtremo < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasCulturas, 0, cultura.id + ": o \"extremo\" precisa de uma lei e de uma das opções dela");
            }
        }
        const Json afinidades = objeto.value("afinidades", Json::object());
        for (const auto& [outra, texto] : afinidades.items()) {
            const std::optional<AfinidadeEntreCulturas> afinidade = texto.is_string() ? lerAfinidade(texto.get<std::string>()) : std::nullopt;
            if (!afinidade) {
                listaDeProblemas.anotarErro(nomeDoArquivoDasCulturas, 0, cultura.id + ": a afinidade com \"" + outra + "\" precisa ser afim, neutra ou hostil");
                continue;
            }
            cultura.afinidades[outra] = *afinidade;
        }
        cultura.habilidadesEmPontos = objeto.value("habilidades_em_pontos", std::map<std::string, int>{});
        cultura.ritoDeAdmissao = objeto.value("rito_de_admissao", std::string());
        cultura.sucessao = objeto.value("sucessao", std::string());
        cultura.nomesDeHomem = objeto.value("nomes_de_homem", std::vector<std::string>{});
        cultura.nomesDeMulher = objeto.value("nomes_de_mulher", std::vector<std::string>{});
        catalogo.culturas.push_back(std::move(cultura));
    }
    return catalogo;
}

std::string culturaDaRegiao(const DadosDoJogo& dados, int regiao)
{
    for (const DefinicaoDaCultura& cultura : dados.culturas.culturas) {
        if (cultura.regiao == regiao) {
            return cultura.id;
        }
    }
    return "";
}

std::string culturaBaseDaCidade(const DadosDoJogo& dados, int numeroDoMapa)
{
    const CidadeDoMundo* cidade = dados.mundo.cidadeNoMapa(numeroDoMapa);
    if (cidade != nullptr && !cidade->faccao.empty()) {
        return cidade->faccao;
    }
    return dados.mundo.mapaExiste(numeroDoMapa) ? culturaDaRegiao(dados, dados.mundo.mapa(numeroDoMapa).regiao) : "";
}

std::vector<int> regioesVizinhas(const DadosDoJogo& dados, int regiao)
{
    std::set<int> vizinhas;
    for (const MapaDoMundo& mapa : dados.mundo.mapas) {
        if (mapa.regiao != regiao) {
            continue;
        }
        for (const int vizinho : dados.mundo.vizinhos(mapa.numeroDoMapa)) {
            const int outra = dados.mundo.mapa(vizinho).regiao;
            if (outra != regiao) {
                vizinhas.insert(outra);
            }
        }
    }
    return std::vector<int>(vizinhas.begin(), vizinhas.end());
}

std::string escolherCulturaDoMigrante(const DadosDoJogo& dados, int numeroDoMapa, uint64_t sementeDaPartida, int numeroDoNpc)
{
    if (!dados.mundo.mapaExiste(numeroDoMapa)) {
        return "";
    }
    const int regiao = dados.mundo.mapa(numeroDoMapa).regiao;
    SorteioComSemente sorteio(sementeDaPartida + static_cast<uint64_t>(numeroDoMapa) * 100000 + static_cast<uint64_t>(numeroDoNpc), sequenciaDoSorteioDaCultura);
    const std::vector<int> vizinhas = regioesVizinhas(dados, regiao);
    if (sorteio.sortearNumeroInteiroEntre(0, 99) < 75 || vizinhas.empty()) {
        return culturaDaRegiao(dados, regiao);
    }
    const int sorteada = vizinhas[static_cast<std::size_t>(sorteio.sortearNumeroInteiroEntre(0, static_cast<int>(vizinhas.size()) - 1))];
    const std::string cultura = culturaDaRegiao(dados, sorteada);
    return cultura.empty() ? culturaDaRegiao(dados, regiao) : cultura;
}

int habilidadeComACultura(const DadosDoJogo& dados, const std::string& cultura, const std::string& trabalho, int habilidade)
{
    const DefinicaoDaCultura* definicao = dados.culturas.procurar(cultura);
    if (definicao == nullptr) {
        return habilidade;
    }
    const auto pontos = definicao->habilidadesEmPontos.find(trabalho);
    return pontos == definicao->habilidadesEmPontos.end() ? habilidade : std::clamp(habilidade + pontos->second, 0, 100);
}

int fatorDaCulturaContraAsLeis(const DadosDoJogo& dados, const std::vector<int>& leis, const std::string& cultura)
{
    const CatalogoDasFaccoes& faccoes = dados.faccoes;
    const auto desejos = faccoes.leisDesejadasPorCultura.find(cultura);
    if (desejos == faccoes.leisDesejadasPorCultura.end()) {
        return 0;
    }
    // 1. A soma: peso × passos entre a lei e o desejo.
    int soma = 0;
    for (std::size_t lei = 0; lei < leis.size() && lei < desejos->second.size(); ++lei) {
        const LeiDesejada& desejo = desejos->second[lei];
        if (desejo.valor >= 0) {
            soma += desejo.peso * std::abs(leis[lei] - desejo.valor);
        }
    }
    // 2. O extremo (CT1-4): no valor extremo, +10; do outro lado do desejo e a 2 passos ou mais, −15.
    int extremo = 0;
    const DefinicaoDaCultura* definicao = dados.culturas.procurar(cultura);
    if (definicao != nullptr && definicao->leiDoExtremo >= 0 && definicao->leiDoExtremo < static_cast<int>(leis.size())) {
        const int atual = leis[static_cast<std::size_t>(definicao->leiDoExtremo)];
        const int desejado = desejos->second[static_cast<std::size_t>(definicao->leiDoExtremo)].valor;
        if (atual == definicao->valorDoExtremo) {
            extremo = 10;
        } else if (desejado >= 0) {
            const bool doOutroLado = (definicao->valorDoExtremo > desejado && atual < desejado) || (definicao->valorDoExtremo < desejado && atual > desejado);
            extremo = doOutroLado && std::abs(atual - desejado) >= 2 ? -15 : 0;
        }
    }
    // 3. Preso entre −30 e +10.
    return std::clamp(-soma + extremo, -30, 10);
}

int fatorLeisECulturaDaClasseEmDecimos(const DadosDoJogo& dados, const std::vector<int>& leis, const std::map<std::string, int>& npcsPorCultura,
                                       const std::string& culturaBase)
{
    int64_t npcs = 0;
    int64_t somaEmDecimos = 0;
    int64_t hostis = 0;
    for (const auto& [cultura, quantos] : npcsPorCultura) {
        npcs += quantos;
        somaEmDecimos += static_cast<int64_t>(quantos) * 10 * fatorDaCulturaContraAsLeis(dados, leis, cultura);
        hostis += dados.culturas.afinidade(culturaBase, cultura) == AfinidadeEntreCulturas::Hostil ? quantos : 0;
    }
    if (npcs == 0) {
        return 0;
    }
    // 4. A média, arredondada para baixo; 4.3: a minoria hostil, −10 × a proporção.
    const auto paraBaixo = [](int64_t numerador, int64_t denominador) {
        return numerador >= 0 ? numerador / denominador : -((-numerador + denominador - 1) / denominador);
    };
    const int64_t media = paraBaixo(somaEmDecimos, npcs);
    const int64_t minoria = paraBaixo(-100 * hostis, npcs);
    return static_cast<int>(std::clamp<int64_t>(media + minoria, -300, 100));
}

int fatorLeisECulturaDaCidadeEmDecimos(const SimulacaoDaVila& vila, const SimulacaoDaVila& cidade)
{
    const int mapa = cidade.numeroDoMapaNoMundo();
    const int faccao = faccaoDaCidade(vila.dados, mapa);
    if (faccao < 0 || faccao >= static_cast<int>(vila.faccoes.leisAtuais.size())) {
        return 0;  // a cidade sem facção não tem leis
    }
    std::map<std::string, int> npcsPorCultura;
    for (const entt::entity npc : cidade.npcsDaVila) {
        const IdentidadeDoNpc& identidade = cidade.registroDasEntidades.get<IdentidadeDoNpc>(npc);
        if (identidade.classe == "civil") {
            ++npcsPorCultura[identidade.cultura];
        }
    }
    return fatorLeisECulturaDaClasseEmDecimos(vila.dados, vila.faccoes.leisAtuais[static_cast<std::size_t>(faccao)], npcsPorCultura, culturaBaseDaCidade(vila.dados, mapa));
}

std::string culturaDominante(SimulacaoDaVila& vila, int faccao)
{
    std::map<std::string, int> npcsPorCultura;
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        if (faccaoDaCidade(vila.dados, cidade->numeroDoMapaNoMundo()) != faccao) {
            continue;
        }
        for (const entt::entity npc : cidade->npcsDaVila) {
            ++npcsPorCultura[cidade->registroDasEntidades.get<IdentidadeDoNpc>(npc).cultura];
        }
    }
    const std::string daFaccao = faccao >= 0 && faccao < static_cast<int>(vila.dados.faccoes.faccoes.size()) ? vila.dados.faccoes.faccoes[static_cast<std::size_t>(faccao)].cultura : "";
    std::string melhor = daFaccao;
    int maior = npcsPorCultura.count(daFaccao) > 0 ? npcsPorCultura[daFaccao] : 0;
    for (const auto& [cultura, quantos] : npcsPorCultura) {
        if (quantos > maior) {
            melhor = cultura;
            maior = quantos;
        }
    }
    return melhor;
}
