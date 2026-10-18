#include "Npc/MovimentoDosNpcs.h"

#include <algorithm>
#include <cstdlib>
#include <utility>

namespace {

constexpr int comprimentoDoPassoRetoEmMilesimos = 1000;
constexpr int comprimentoDoPassoDiagonalEmMilesimos = 1414;  // a diagonal de um quadrado de lado 1 mede 1,414

std::string textoDoTile(PosicaoDeTile tile)
{
    return "(" + std::to_string(tile.tileX) + ", " + std::to_string(tile.tileY) + ")";
}

// Regra 4.4, passo 1: começa o passo até o próximo tile do caminho.
// Devolve false se o NPC parou: o caminho acabou (ele chegou) ou o próximo
// tile ficou bloqueado (aí ele pede um caminho novo para o mesmo destino).
// Nos dois casos o CaminhoDoNpc é tirado: não use a referência depois disso.
bool comecarOProximoPasso(entt::registry& registroDasEntidades, entt::entity npc, PosicaoDoNpc& posicao,
                          CaminhoDoNpc& caminho, const MapaLocal& mapa, int64_t tickAtual)
{
    LugarDoNpc& lugar = posicao.lugarAtual;
    if (caminho.acabou()) {
        registroDasEntidades.remove<CaminhoDoNpc>(npc);
        return false;
    }
    const PosicaoDeTile tileSeguinte = caminho.tilesDoCaminho[caminho.indiceDoProximoTileDoCaminho];
    if (!passoEntreVizinhosEhPermitido(mapa, lugar.tileAtual, tileSeguinte)) {
        const PosicaoDeTile destinoDoCaminho = caminho.tileDeDestino;
        registroDasEntidades.remove<CaminhoDoNpc>(npc);
        pedirCaminhoParaONpc(registroDasEntidades, npc, destinoDoCaminho, tickAtual);
        return false;
    }
    lugar.proximoTile = tileSeguinte;
    ++caminho.indiceDoProximoTileDoCaminho;
    posicao.direcaoDoOlhar = descobrirDirecaoDoPasso(tileSeguinte.tileX - lugar.tileAtual.tileX,
                                                     tileSeguinte.tileY - lugar.tileAtual.tileY, posicao.direcaoDoOlhar);
    return true;
}

// Regra 4.4, passos 1 a 3, para um NPC.
void andarUmNpc(entt::registry& registroDasEntidades, entt::entity npc, PosicaoDoNpc& posicao, CaminhoDoNpc& caminho,
                const MapaLocal& mapa, const RegrasDoMovimento& regras, int64_t tickAtual)
{
    LugarDoNpc& lugar = posicao.lugarAtual;
    if (lugar.estaParado() && !comecarOProximoPasso(registroDasEntidades, npc, posicao, caminho, mapa, tickAtual)) {
        return;
    }

    // Passo 2: soma a velocidade ao progresso e à animação.
    lugar.progressoAteOProximoTileEmMilesimos += regras.velocidadeDeAndarEmMilesimosPorTick;
    if (regras.distanciaDoCicloDeAndarEmMilesimos > 0) {
        posicao.distanciaNoCicloDeAndarEmMilesimos =
            (posicao.distanciaNoCicloDeAndarEmMilesimos + regras.velocidadeDeAndarEmMilesimosPorTick)
            % regras.distanciaDoCicloDeAndarEmMilesimos;
    }

    // Passo 3: chegou ao próximo tile? A sobra já entra no passo seguinte,
    // para a velocidade não dar "soluços" a cada tile.
    while (true) {
        const int comprimentoDoPasso = comprimentoDoPassoEmMilesimos(lugar.tileAtual, lugar.proximoTile);
        if (lugar.progressoAteOProximoTileEmMilesimos < comprimentoDoPasso) {
            return;
        }
        const int sobraDoProgresso = lugar.progressoAteOProximoTileEmMilesimos - comprimentoDoPasso;
        lugar.tileAtual = lugar.proximoTile;
        lugar.progressoAteOProximoTileEmMilesimos = 0;
        if (!comecarOProximoPasso(registroDasEntidades, npc, posicao, caminho, mapa, tickAtual)) {
            return;  // chegou ao destino ou o caminho ficou bloqueado
        }
        lugar.progressoAteOProximoTileEmMilesimos = sobraDoProgresso;
    }
}

}  // namespace

int converterVelocidadeParaMilesimosPorTick(int velocidadeEmTilesPorHora, int ticksPorHoraDeJogo)
{
    // milésimos por tick = tiles por hora × 1.000 ÷ ticks por hora, com a metade subindo.
    const int64_t milesimosPorHora = static_cast<int64_t>(velocidadeEmTilesPorHora) * 1000;
    return static_cast<int>((2 * milesimosPorHora + ticksPorHoraDeJogo) / (2 * static_cast<int64_t>(ticksPorHoraDeJogo)));
}

int comprimentoDoPassoEmMilesimos(PosicaoDeTile tileDeOrigem, PosicaoDeTile tileDeDestino)
{
    const bool passoEhDiagonal = tileDeOrigem.tileX != tileDeDestino.tileX && tileDeOrigem.tileY != tileDeDestino.tileY;
    return passoEhDiagonal ? comprimentoDoPassoDiagonalEmMilesimos : comprimentoDoPassoRetoEmMilesimos;
}

DirecaoDoOlhar descobrirDirecaoDoPasso(int passoX, int passoY, DirecaoDoOlhar direcaoAtual)
{
    // Na diagonal vale o lado, porque a arte tem só 4 direções.
    if (passoX > 0) {
        return DirecaoDoOlhar::Direita;
    }
    if (passoX < 0) {
        return DirecaoDoOlhar::Esquerda;
    }
    if (passoY > 0) {
        return DirecaoDoOlhar::Baixo;
    }
    if (passoY < 0) {
        return DirecaoDoOlhar::Cima;
    }
    return direcaoAtual;
}

std::optional<PosicaoDeTile> acharTileLivreEmVoltaDe(const MapaLocal& mapa, PosicaoDeTile tileDoCentro,
                                                     int quantidadeDeTilesLivresParaPular)
{
    const int maiorDistancia = std::max(mapa.larguraEmTiles, mapa.alturaEmTiles);
    for (int distancia = 0; distancia <= maiorDistancia; ++distancia) {
        // A volta de cada distância: só os tiles da borda do quadrado.
        for (int tileY = tileDoCentro.tileY - distancia; tileY <= tileDoCentro.tileY + distancia; ++tileY) {
            for (int tileX = tileDoCentro.tileX - distancia; tileX <= tileDoCentro.tileX + distancia; ++tileX) {
                const bool estaNaBordaDoQuadrado = std::max(std::abs(tileX - tileDoCentro.tileX), std::abs(tileY - tileDoCentro.tileY)) == distancia;
                if (!estaNaBordaDoQuadrado || !mapa.tileDaParaAndar(tileX, tileY)) {
                    continue;
                }
                if (quantidadeDeTilesLivresParaPular == 0) {
                    return PosicaoDeTile{tileX, tileY};
                }
                --quantidadeDeTilesLivresParaPular;
            }
        }
    }
    return std::nullopt;
}

entt::entity criarNpcNoTile(entt::registry& registroDasEntidades, PosicaoDeTile tileOndeNasce)
{
    const entt::entity npc = registroDasEntidades.create();
    PosicaoDoNpc posicao;
    posicao.lugarAtual.tileAtual = tileOndeNasce;
    posicao.lugarAtual.proximoTile = tileOndeNasce;
    posicao.direcaoDoOlhar = DirecaoDoOlhar::Baixo;
    posicao.lugarNoTickAnteriorParaDesenho = posicao.lugarAtual;
    registroDasEntidades.emplace<PosicaoDoNpc>(npc, posicao);
    return npc;
}

void pedirCaminhoParaONpc(entt::registry& registroDasEntidades, entt::entity npc, PosicaoDeTile tileDeDestino, int64_t tickAtual)
{
    // Um pedido novo substitui o antigo e vai para o fim da fila (pelo tick).
    registroDasEntidades.emplace_or_replace<PedidoDeCaminho>(npc, PedidoDeCaminho{tileDeDestino, tickAtual});
}

std::vector<std::string> atenderPedidosDeCaminho(entt::registry& registroDasEntidades, const MapaLocal& mapa,
                                                 BuscadorDeCaminho& buscadorDeCaminho, const RegrasDoMovimento& regras)
{
    std::vector<std::string> avisosParaOConsole;

    // Passo 1: os pedidos pela ordem do tick (no empate, pelo número da entidade).
    std::vector<std::pair<int64_t, entt::entity>> pedidosNaOrdem;
    for (const auto [npc, pedido] : registroDasEntidades.view<PedidoDeCaminho>().each()) {
        pedidosNaOrdem.emplace_back(pedido.tickDoPedido, npc);
    }
    std::sort(pedidosNaOrdem.begin(), pedidosNaOrdem.end(), [](const auto& primeiro, const auto& segundo) {
        if (primeiro.first != segundo.first) {
            return primeiro.first < segundo.first;
        }
        return entt::to_integral(primeiro.second) < entt::to_integral(segundo.second);
    });
    if (static_cast<int>(pedidosNaOrdem.size()) > regras.caminhosCalculadosPorTick) {
        pedidosNaOrdem.resize(regras.caminhosCalculadosPorTick);
    }

    for (const auto& [tickDoPedido, npc] : pedidosNaOrdem) {
        const PosicaoDeTile tileDeDestino = registroDasEntidades.get<PedidoDeCaminho>(npc).tileDeDestino;
        registroDasEntidades.remove<PedidoDeCaminho>(npc);  // passo 5
        const PosicaoDoNpc* posicao = registroDasEntidades.try_get<PosicaoDoNpc>(npc);
        if (posicao == nullptr) {
            continue;
        }

        // Fora do mapa: não há caminho, e o NPC continua o que fazia.
        if (!mapa.tileEstaDentroDoMapa(tileDeDestino.tileX, tileDeDestino.tileY)) {
            avisosParaOConsole.push_back("o destino " + textoDoTile(tileDeDestino) + " está fora do mapa");
            continue;
        }

        // Passo 2: parado, sai do tile atual; no meio de um passo, do próximo
        // tile (ele termina o passo antes). Parado, os dois são o mesmo tile.
        const PosicaoDeTile tileDeSaida = posicao->lugarAtual.proximoTile;

        // Passos 3 e 4.
        ResultadoDaBuscaDeCaminho resultado = buscadorDeCaminho.buscarCaminho(mapa, tileDeSaida, tileDeDestino);
        if (!resultado.chegaNoDestino) {
            const PosicaoDeTile tileFinal = resultado.tilesDoCaminho.empty() ? tileDeSaida : resultado.tilesDoCaminho.back();
            avisosParaOConsole.push_back("o destino " + textoDoTile(tileDeDestino) + " não tem caminho; o NPC vai até " + textoDoTile(tileFinal));
        }
        // Mesmo vazio, o caminho entra: se o NPC está no meio de um passo, o
        // andar termina o passo e depois tira o caminho.
        CaminhoDoNpc caminhoNovo;
        caminhoNovo.tilesDoCaminho = std::move(resultado.tilesDoCaminho);
        caminhoNovo.tileDeDestino = tileDeDestino;
        registroDasEntidades.emplace_or_replace<CaminhoDoNpc>(npc, std::move(caminhoNovo));
    }
    return avisosParaOConsole;
}

void andarOsNpcsUmTick(entt::registry& registroDasEntidades, const MapaLocal& mapa, const RegrasDoMovimento& regras,
                       int64_t tickAtual)
{
    // Todos guardam a posição do tick anterior, também os parados (FL4-5).
    for (auto [npc, posicao] : registroDasEntidades.view<PosicaoDoNpc>().each()) {
        posicao.lugarNoTickAnteriorParaDesenho = posicao.lugarAtual;
    }

    // A EnTT deixa tirar componentes da entidade da vez durante a volta.
    for (auto [npc, posicao, caminho] : registroDasEntidades.view<PosicaoDoNpc, CaminhoDoNpc>().each()) {
        andarUmNpc(registroDasEntidades, npc, posicao, caminho, mapa, regras, tickAtual);
    }
}

PontoEmPixelsDoMundo calcularPesEmPixelsDoMundo(const LugarDoNpc& lugar, int tamanhoDoTileEmPixels)
{
    const int comprimentoDoPasso = comprimentoDoPassoEmMilesimos(lugar.tileAtual, lugar.proximoTile);
    const double fracaoDoPasso = static_cast<double>(lugar.progressoAteOProximoTileEmMilesimos) / comprimentoDoPasso;
    const double tileX = lugar.tileAtual.tileX + (lugar.proximoTile.tileX - lugar.tileAtual.tileX) * fracaoDoPasso;
    const double tileY = lugar.tileAtual.tileY + (lugar.proximoTile.tileY - lugar.tileAtual.tileY) * fracaoDoPasso;
    // Os pés ficam no centro do tile.
    return {tileX * tamanhoDoTileEmPixels + tamanhoDoTileEmPixels / 2.0,
            tileY * tamanhoDoTileEmPixels + tamanhoDoTileEmPixels / 2.0};
}
