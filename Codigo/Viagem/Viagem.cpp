#include "Viagem/Viagem.h"

#include <algorithm>
#include <limits>
#include <set>
#include <utility>

#include "Dados/DadosDoJogo.h"

namespace {

int valorDaTabela(const std::map<std::string, int>& tabela, const std::string& nome)
{
    const auto valor = tabela.find(nome);
    return valor == tabela.end() ? 100 : valor->second;
}

}  // namespace

std::string infraestruturaDoMapa(const MundoDoJogo& mundo, int numeroDoMapa)
{
    const CidadeDoMundo* cidade = mundo.cidadeNoMapa(numeroDoMapa);
    if (cidade != nullptr && cidade->nivel >= 3) {
        return "cidade_grande";
    }
    return mundo.mapa(numeroDoMapa).infraestrutura;
}

int multiplicadorDoTamanhoDoGrupo(int pessoas, const RegrasDaViagem& regras)
{
    for (const auto& [ateTantasPessoas, multiplicador] : regras.tamanhoDoGrupo) {
        if (pessoas <= ateTantasPessoas) {
            return multiplicador;
        }
    }
    return regras.tamanhoDoGrupo.empty() ? 100 : regras.tamanhoDoGrupo.back().second;
}

int multiplicadorDeClimaEEstacao(TipoDeClima climaDaRegiao, Estacao estacao, const MundoDoJogo& mundo, int mapaDeChegada,
                                 const RegrasDaViagem& regras)
{
    const MapaDoMundo& chegada = mundo.mapa(mapaDeChegada);
    int daEstacao = 100;
    if (estacao == Estacao::Inverno) {
        daEstacao = regras.biomasFrios.count(chegada.bioma) > 0 ? regras.invernoNoBiomaFrioEmPorcento : regras.invernoNosOutrosBiomasEmPorcento;
    } else if (estacao == Estacao::Primavera) {
        const std::string infraestrutura = infraestruturaDoMapa(mundo, mapaDeChegada);
        if (infraestrutura == "selvagem" || infraestrutura == "trilha") {
            daEstacao = regras.lamaNaPrimaveraSemEstradaEmPorcento;
        }
    }
    const int multiplicador = valorDaTabela(regras.climaEmPorcento, nomeDoClimaNoArquivo(climaDaRegiao)) * daEstacao / 100;
    return std::min(multiplicador, regras.tetoDoClimaVezesEstacaoEmPorcento);
}

int calcularTicksDoTrecho(const EntidadeEmTransito& entidade, const MundoDoJogo& mundo, int mapaDeChegada, int climaEEstacaoEmPorcento,
                          const RegrasDaViagem& regras)
{
    const MapaDoMundo& chegada = mundo.mapa(mapaDeChegada);
    int64_t ticks = regras.ticksBasePorMapa;
    ticks = ticks * valorDaTabela(regras.terrenoEmPorcento, chegada.terreno) / 100;
    ticks = ticks * valorDaTabela(regras.infraestruturaEmPorcento, infraestruturaDoMapa(mundo, mapaDeChegada)) / 100;
    ticks = ticks * multiplicadorDoTamanhoDoGrupo(entidade.pessoas, regras) / 100;
    ticks = ticks * (entidade.montados ? regras.montadosEmPorcento : 100) / 100;
    ticks = ticks * (entidade.cargaAcimaDoLimite ? regras.cargaAcimaDoLimiteEmPorcento : 100) / 100;
    ticks = ticks * (entidade.ehCaravana() ? regras.caravanaEmPorcento : 100) / 100;
    ticks = ticks * climaEEstacaoEmPorcento / 100;
    return static_cast<int>(std::max<int64_t>(ticks, regras.ticksMinimosPorMapa));
}

std::vector<int> buscarRotaMaisRapida(int origem, int destino, const EntidadeEmTransito& entidade, const MundoDoJogo& mundo,
                                      const RegrasDaViagem& regras)
{
    if (origem == destino || !mundo.mapaExiste(origem) || !mundo.mapaExiste(destino)) {
        return {};
    }
    // Com 48 mapas, cada um guarda o caminho inteiro: assim o desempate compara os
    // caminhos (o que passa primeiro pelo mapa de menor número ganha).
    const int64_t semCaminho = std::numeric_limits<int64_t>::max();
    std::vector<int64_t> custo(mundo.mapas.size() + 1, semCaminho);
    std::vector<std::vector<int>> caminho(mundo.mapas.size() + 1);
    std::set<std::pair<int64_t, int>> fila;
    custo[origem] = 0;
    fila.insert({0, origem});
    while (!fila.empty()) {
        const auto [custoAtual, mapaAtual] = *fila.begin();
        fila.erase(fila.begin());
        if (mapaAtual == destino) {
            break;
        }
        for (const int vizinho : mundo.vizinhos(mapaAtual)) {
            const int64_t novoCusto = custoAtual + calcularTicksDoTrecho(entidade, mundo, vizinho, 100, regras);
            std::vector<int> novoCaminho = caminho[mapaAtual];
            novoCaminho.push_back(vizinho);
            if (novoCusto < custo[vizinho] || (novoCusto == custo[vizinho] && novoCaminho < caminho[vizinho])) {
                fila.erase({custo[vizinho], vizinho});
                custo[vizinho] = novoCusto;
                caminho[vizinho] = std::move(novoCaminho);
                fila.insert({novoCusto, vizinho});
            }
        }
    }
    return caminho[destino];
}

int chanceDeEncontroPorMil(const EntidadeEmTransito& entidade, const MundoDoJogo& mundo, int mapaDeChegada, const RegrasDaViagem& regras)
{
    int chance = regras.encontroNoTerritorioPorMil;
    switch (mundo.estadoDoTerritorio(mapaDeChegada)) {
    case EstadoDoTerritorio::EmDisputa: chance = regras.encontroEmDisputaPorMil; break;
    case EstadoDoTerritorio::SemDono: chance = regras.encontroSemDonoPorMil; break;
    default: break;
    }
    return entidade.marchaForcada ? chance * regras.marchaForcadaEncontroEmPorcento / 100 : chance;
}

EncontroMarcado escolherOEncontro(int chancePorMil, int numeroDoEncontro, int numeroDoTipo, int numeroDoBando, const RegrasDaViagem& regras)
{
    if (numeroDoEncontro >= chancePorMil) {
        return {};
    }
    const int somaDosPesos = std::max(1, regras.pesoDosBandidos + regras.pesoDoMercadorViajante);
    if (numeroDoTipo < regras.pesoDosBandidos * 1000 / somaDosPesos) {
        return {TipoDeEncontro::Bandidos, regras.bandoDeBandidosMinimo + numeroDoBando};
    }
    return {TipoDeEncontro::MercadorViajante, 0};
}

EncontroMarcado sortearEncontroAoEntrarNoTrecho(const EntidadeEmTransito& entidade, const MundoDoJogo& mundo, int mapaDeChegada,
                                                const RegrasDaViagem& regras, SorteioComSemente& sorteio)
{
    const int numeroDoEncontro = sorteio.sortearNumeroInteiroEntre(0, 999);
    const int numeroDoTipo = sorteio.sortearNumeroInteiroEntre(0, 999);
    const int numeroDoBando = sorteio.sortearNumeroInteiroEntre(0, regras.bandoDeBandidosSorteioAte);
    return escolherOEncontro(chanceDeEncontroPorMil(entidade, mundo, mapaDeChegada, regras), numeroDoEncontro, numeroDoTipo, numeroDoBando, regras);
}

int forcaDoGrupo(const EntidadeEmTransito& entidade, const RegrasDaViagem& regras)
{
    return regras.forcaDe("jogador") + std::max(0, entidade.pessoas - 1) * regras.forcaDe("civil");
}

int forcaDoBando(int bandidos, const RegrasDaViagem& regras)
{
    return bandidos * regras.forcaDe("bandido");
}

bool bandidosAtacam(int forcaDoGrupo, int bandidos, const RegrasDaViagem& regras)
{
    return forcaDoGrupo <= regras.bandidosNaoAtacamSeOGrupoForVezesMaisForte * forcaDoBando(bandidos, regras);
}

int64_t precoDoSubornoEmDecimos(int bandidos, int carisma, const RegrasDaViagem& regras)
{
    return static_cast<int64_t>(bandidos) * regras.subornoPorBandidoEmDecimos * (150 - carisma) / 100;
}

int chanceDeFugirEmPorcento(const EntidadeEmTransito& entidade, int bandidos, const RegrasDaViagem& regras)
{
    const int tempoDoBando = multiplicadorDoTamanhoDoGrupo(bandidos, regras);
    const int tempoDoGrupo = multiplicadorDoTamanhoDoGrupo(entidade.pessoas, regras) * (entidade.montados ? regras.montadosEmPorcento : 100) / 100;
    const int chance = regras.fugaBaseEmPorcento + (tempoDoBando - tempoDoGrupo) * 50 / std::max(1, tempoDoBando);
    return std::clamp(chance, regras.fugaMinimaEmPorcento, regras.fugaMaximaEmPorcento);
}

bool resolverPorCalculoRapidoDeForcas(int forcaA, int forcaB, int numeroDe0A999)
{
    if (forcaA + forcaB <= 0) {
        return false;
    }
    return numeroDe0A999 < forcaA * 1000 / (forcaA + forcaB);
}

void aplicarAPerdaDoGrupo(EntidadeEmTransito& entidade, int porcentagem)
{
    entidade.dinheiroEmDecimos -= entidade.dinheiroEmDecimos * porcentagem / 100;
    for (int& quantidade : entidade.quantidadePorItem) {
        quantidade -= quantidade * porcentagem / 100;
    }
}

std::string resolverEncontroComBandidos(EntidadeEmTransito& entidade, EscolhaNoEncontro escolha, const RegrasDaViagem& regras,
                                        SorteioComSemente& sorteio)
{
    const int bandidos = entidade.encontroMarcado.bandidos;
    std::string linha;
    if (escolha == EscolhaNoEncontro::Subornar) {
        const int64_t preco = precoDoSubornoEmDecimos(bandidos, regras.carismaDoJogadorAteOsAtributos, regras);
        if (entidade.dinheiroEmDecimos < preco) {
            return "";
        }
        entidade.dinheiroEmDecimos -= preco;
        linha = "Viagem: o grupo pagou " + escreverDinheiroEmMoedas(static_cast<int>(preco)) + " moedas aos bandidos e segue.";
    }
    if (escolha == EscolhaNoEncontro::Fugir) {
        const int chance = chanceDeFugirEmPorcento(entidade, bandidos, regras);
        if (sorteio.sortearNumeroInteiroEntre(0, 999) < chance * 10) {
            linha = "Viagem: o grupo fugiu dos bandidos (chance de " + std::to_string(chance) + "%).";
        } else {
            linha = "Viagem: a fuga falhou (chance de " + std::to_string(chance) + "%). ";
            escolha = EscolhaNoEncontro::Lutar;
        }
    }
    if (escolha == EscolhaNoEncontro::Lutar) {
        const int forcaDoNossoGrupo = forcaDoGrupo(entidade, regras);
        entidade.ticksParadoNaLuta = regras.lutaRapidaDuraTicks;
        if (resolverPorCalculoRapidoDeForcas(forcaDoNossoGrupo, forcaDoBando(bandidos, regras), sorteio.sortearNumeroInteiroEntre(0, 999))) {
            linha += "Viagem: o grupo venceu a luta contra " + std::to_string(bandidos) + " bandidos.";
        } else {
            aplicarAPerdaDoGrupo(entidade, regras.perdaAoPerderALutaEmPorcento);
            linha += "Viagem: o grupo perdeu a luta e " + std::to_string(regras.perdaAoPerderALutaEmPorcento) + "% do que levava.";
        }
    }
    if (escolha == EscolhaNoEncontro::Render) {
        aplicarAPerdaDoGrupo(entidade, regras.perdaAoSeRenderEmPorcento);
        linha = "Viagem: o grupo se rendeu e perdeu " + std::to_string(regras.perdaAoSeRenderEmPorcento) + "% do que levava.";
    }
    entidade.encontroMarcado = {};
    entidade.encontroAcontecendo = false;
    return linha;
}
