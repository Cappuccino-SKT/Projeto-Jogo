// Os testes do movimento dos NPCs: a tabela do Codigo/ na parte 8 da Ficha
// Lógica: NPC. A maioria usa um mapa vazio de 16 x 16, montado aqui mesmo.

#include <cmath>
#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Mapa/BuscaDeCaminho.h"
#include "Mapa/MapaLocal.h"
#include "Npc/MovimentoDosNpcs.h"

namespace {

const std::string caminhoDoMapaDeTeste = std::string(PASTA_DOS_TESTES) + "/mapas/mapa_de_teste_8x8.tmj";
const std::string caminhoDoMapaDaVila = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data/mapas/vila.tmj";

// Um mapa sem nenhum obstáculo: só o tamanho e a passagem.
MapaLocal criarMapaVazio(int larguraEmTiles, int alturaEmTiles)
{
    MapaLocal mapa;
    mapa.larguraEmTiles = larguraEmTiles;
    mapa.alturaEmTiles = alturaEmTiles;
    mapa.passagemPorTile.assign(static_cast<std::size_t>(larguraEmTiles) * alturaEmTiles, 1);
    return mapa;
}

void bloquearTile(MapaLocal& mapa, int tileX, int tileY)
{
    mapa.passagemPorTile[mapa.indiceDoTileNaLista(tileX, tileY)] = 0;
}

// Pede o caminho e já atende o pedido, como no começo de um tick.
void mandarONpcPara(entt::registry& registro, entt::entity npc, PosicaoDeTile destino, const MapaLocal& mapa,
                    BuscadorDeCaminho& buscador, const RegrasDoMovimento& regras)
{
    pedirCaminhoParaONpc(registro, npc, destino, 0);
    atenderPedidosDeCaminho(registro, mapa, buscador, regras);
}

void andarVariosTicks(entt::registry& registro, const MapaLocal& mapa, const RegrasDoMovimento& regras, int quantidadeDeTicks)
{
    for (int tick = 0; tick < quantidadeDeTicks; ++tick) {
        andarOsNpcsUmTick(registro, mapa, regras, tick);
    }
}

void testarContasDoMovimento()
{
    conferir(converterVelocidadeParaMilesimosPorTick(128, 800) == 160, "128 tiles por hora são 160 milésimos por tick");
    conferir(comprimentoDoPassoEmMilesimos({10, 3}, {11, 3}) == 1000 && comprimentoDoPassoEmMilesimos({10, 3}, {11, 4}) == 1414,
             "o passo reto mede 1.000 e o diagonal, 1.414");
    conferir(descobrirDirecaoDoPasso(1, 1, DirecaoDoOlhar::Cima) == DirecaoDoOlhar::Direita
                 && descobrirDirecaoDoPasso(-1, -1, DirecaoDoOlhar::Baixo) == DirecaoDoOlhar::Esquerda,
             "na diagonal, o NPC olha para o lado");
    conferir(descobrirDirecaoDoPasso(0, 0, DirecaoDoOlhar::Esquerda) == DirecaoDoOlhar::Esquerda, "parado, continua olhando para onde olhava");

    LugarDoNpc passoReto;
    passoReto.tileAtual = {10, 3};
    passoReto.proximoTile = {11, 3};
    passoReto.progressoAteOProximoTileEmMilesimos = 480;
    const PontoEmPixelsDoMundo pesNoPassoReto = calcularPesEmPixelsDoMundo(passoReto, 16);
    conferir(std::fabs(pesNoPassoReto.pixelX - 175.68) < 0.001 && pesNoPassoReto.pixelY == 56.0,
             "de (10, 3) para (11, 3), progresso 480: pés em (175,68; 56)");

    LugarDoNpc passoDiagonal;
    passoDiagonal.tileAtual = {10, 3};
    passoDiagonal.proximoTile = {11, 4};
    passoDiagonal.progressoAteOProximoTileEmMilesimos = 707;
    const PontoEmPixelsDoMundo pesNoPassoDiagonal = calcularPesEmPixelsDoMundo(passoDiagonal, 16);
    conferir(pesNoPassoDiagonal.pixelX == 176.0 && pesNoPassoDiagonal.pixelY == 64.0, "de (10, 3) para (11, 4), progresso 707: pés em (176, 64)");
}

void testarAndarReto()
{
    const MapaLocal mapa = criarMapaVazio(16, 16);
    const RegrasDoMovimento regras;
    BuscadorDeCaminho buscador;
    entt::registry registro;
    const entt::entity npc = criarNpcNoTile(registro, {10, 3});
    mandarONpcPara(registro, npc, {12, 3}, mapa, buscador, regras);

    andarVariosTicks(registro, mapa, regras, 7);
    const LugarDoNpc& lugarDepoisDe7 = registro.get<PosicaoDoNpc>(npc).lugarAtual;
    conferir(lugarDepoisDe7.tileAtual == PosicaoDeTile{11, 3} && lugarDepoisDe7.progressoAteOProximoTileEmMilesimos == 120,
             "de (10, 3) a (12, 3): depois de 7 ticks, está em (11, 3) com progresso 120 (a sobra)");
    conferir(registro.get<PosicaoDoNpc>(npc).direcaoDoOlhar == DirecaoDoOlhar::Direita, "andando para a direita, olha para a Direita");

    andarVariosTicks(registro, mapa, regras, 5);
    conferir(registro.get<PosicaoDoNpc>(npc).lugarAtual.tileAtual == PosicaoDeTile{11, 3}, "depois de 12 ticks, ainda não chegou");
    andarVariosTicks(registro, mapa, regras, 1);
    const LugarDoNpc& lugarDepoisDe13 = registro.get<PosicaoDoNpc>(npc).lugarAtual;
    conferir(lugarDepoisDe13.tileAtual == PosicaoDeTile{12, 3} && lugarDepoisDe13.estaParado()
                 && lugarDepoisDe13.progressoAteOProximoTileEmMilesimos == 0 && !registro.all_of<CaminhoDoNpc>(npc),
             "depois de 13 ticks, chegou a (12, 3): parado e sem caminho");
}

void testarAndarNaDiagonalEParaCima()
{
    const MapaLocal mapa = criarMapaVazio(16, 16);
    const RegrasDoMovimento regras;
    BuscadorDeCaminho buscador;
    entt::registry registro;

    const entt::entity npcDaDiagonal = criarNpcNoTile(registro, {10, 3});
    mandarONpcPara(registro, npcDaDiagonal, {11, 4}, mapa, buscador, regras);
    andarVariosTicks(registro, mapa, regras, 8);
    conferir(registro.get<PosicaoDoNpc>(npcDaDiagonal).lugarAtual.tileAtual == PosicaoDeTile{10, 3}, "na diagonal, 8 ticks não bastam");
    andarVariosTicks(registro, mapa, regras, 1);
    conferir(registro.get<PosicaoDoNpc>(npcDaDiagonal).lugarAtual.tileAtual == PosicaoDeTile{11, 4}
                 && registro.get<PosicaoDoNpc>(npcDaDiagonal).direcaoDoOlhar == DirecaoDoOlhar::Direita,
             "na diagonal de (10, 3) a (11, 4): chega em 9 ticks, olhando para a Direita");

    const entt::entity npcParaCima = criarNpcNoTile(registro, {5, 8});
    mandarONpcPara(registro, npcParaCima, {5, 6}, mapa, buscador, regras);
    andarVariosTicks(registro, mapa, regras, 1);
    conferir(registro.get<PosicaoDoNpc>(npcParaCima).direcaoDoOlhar == DirecaoDoOlhar::Cima, "um caminho para cima: olha para Cima");
}

void testarCaminhoBloqueadoENovoDestino()
{
    MapaLocal mapa = criarMapaVazio(16, 16);
    const RegrasDoMovimento regras;
    BuscadorDeCaminho buscador;
    entt::registry registro;

    // O próximo tile do caminho fica bloqueado no meio da viagem (uma obra nova).
    const entt::entity npc = criarNpcNoTile(registro, {10, 3});
    mandarONpcPara(registro, npc, {13, 3}, mapa, buscador, regras);
    andarVariosTicks(registro, mapa, regras, 2);
    bloquearTile(mapa, 12, 3);
    andarVariosTicks(registro, mapa, regras, 5);
    const PedidoDeCaminho* pedido = registro.try_get<PedidoDeCaminho>(npc);
    conferir(registro.get<PosicaoDoNpc>(npc).lugarAtual.tileAtual == PosicaoDeTile{11, 3}
                 && registro.get<PosicaoDoNpc>(npc).lugarAtual.estaParado() && !registro.all_of<CaminhoDoNpc>(npc)
                 && pedido != nullptr && pedido->tileDeDestino == PosicaoDeTile{13, 3},
             "o próximo tile ficou bloqueado: o NPC para em (11, 3) e pede um caminho novo para (13, 3)");

    // Um destino novo no meio de um passo: o caminho novo sai do próximo tile.
    const MapaLocal mapaLivre = criarMapaVazio(16, 16);
    const entt::entity outroNpc = criarNpcNoTile(registro, {10, 3});
    mandarONpcPara(registro, outroNpc, {12, 3}, mapaLivre, buscador, regras);
    andarVariosTicks(registro, mapaLivre, regras, 3);
    mandarONpcPara(registro, outroNpc, {11, 8}, mapaLivre, buscador, regras);
    const CaminhoDoNpc& caminhoNovo = registro.get<CaminhoDoNpc>(outroNpc);
    conferir(!caminhoNovo.tilesDoCaminho.empty() && caminhoNovo.tilesDoCaminho.front() == PosicaoDeTile{11, 4},
             "destino novo no meio do passo de (10, 3) para (11, 3): o caminho novo sai de (11, 3)");
    andarVariosTicks(registro, mapaLivre, regras, 60);
    conferir(registro.get<PosicaoDoNpc>(outroNpc).lugarAtual.tileAtual == PosicaoDeTile{11, 8}, "e o NPC chega a (11, 8)");
}

void testarFilaDePedidos()
{
    const MapaLocal mapa = criarMapaVazio(16, 16);
    const RegrasDoMovimento regras;
    BuscadorDeCaminho buscador;
    entt::registry registro;
    for (int numero = 0; numero < 6; ++numero) {
        const entt::entity npc = criarNpcNoTile(registro, {numero, 0});
        pedirCaminhoParaONpc(registro, npc, {numero, 10}, 100);
    }
    atenderPedidosDeCaminho(registro, mapa, buscador, regras);
    const auto comCaminhoNoPrimeiroTick = registro.view<CaminhoDoNpc>().size();
    const auto esperandoNoPrimeiroTick = registro.view<PedidoDeCaminho>().size();
    atenderPedidosDeCaminho(registro, mapa, buscador, regras);
    conferir(comCaminhoNoPrimeiroTick == 4 && esperandoNoPrimeiroTick == 2 && registro.view<CaminhoDoNpc>().size() == 6,
             "6 pedidos no mesmo tick, limite 4: 4 atendidos nesse tick e 2 no seguinte");
}

void testarDestinoSemCaminho()
{
    const ResultadoDaLeituraDoMapa leitura = carregarMapaDoArquivoDoTiled(caminhoDoMapaDeTeste);
    const RegrasDoMovimento regras;
    BuscadorDeCaminho buscador;
    entt::registry registro;
    const entt::entity npc = criarNpcNoTile(registro, {0, 0});
    pedirCaminhoParaONpc(registro, npc, {5, 6}, 0);
    const std::vector<std::string> avisos = atenderPedidosDeCaminho(registro, leitura.mapa, buscador, regras);
    conferir(registro.get<CaminhoDoNpc>(npc).tilesDoCaminho.back() == PosicaoDeTile{5, 5} && avisos.size() == 1
                 && avisos[0].find("não tem caminho; o NPC vai até (5, 5)") != std::string::npos,
             "destino na pedra (5, 6): o caminho termina em (5, 5), com um aviso");

    pedirCaminhoParaONpc(registro, npc, {8, 0}, 1);
    const std::vector<std::string> avisosForaDoMapa = atenderPedidosDeCaminho(registro, leitura.mapa, buscador, regras);
    conferir(avisosForaDoMapa.size() == 1 && registro.get<CaminhoDoNpc>(npc).tileDeDestino == PosicaoDeTile{5, 6},
             "destino fora do mapa: aviso, e o NPC continua o que fazia");
}

void testarOndeONpcNasce()
{
    const ResultadoDaLeituraDoMapa vila = carregarMapaDoArquivoDoTiled(caminhoDoMapaDaVila);
    const std::optional<PosicaoDeTile> tileNaVila = acharTileLivreEmVoltaDe(vila.mapa, {32, 32}, 0);
    conferir(tileNaVila.has_value() && *tileNaVila == PosicaoDeTile{32, 32}, "na vila, o NPC nasce em (32, 32)");

    MapaLocal mapaComOCentroBloqueado = criarMapaVazio(8, 8);
    bloquearTile(mapaComOCentroBloqueado, 4, 4);
    const std::optional<PosicaoDeTile> primeiro = acharTileLivreEmVoltaDe(mapaComOCentroBloqueado, {4, 4}, 0);
    const std::optional<PosicaoDeTile> segundo = acharTileLivreEmVoltaDe(mapaComOCentroBloqueado, {4, 4}, 1);
    conferir(primeiro == PosicaoDeTile{3, 3} && segundo == PosicaoDeTile{4, 3},
             "com o centro (4, 4) bloqueado, o primeiro tile livre é (3, 3) e o seguinte, (4, 3)");

    MapaLocal mapaTodoBloqueado = criarMapaVazio(2, 2);
    for (int indice = 0; indice < 4; ++indice) {
        mapaTodoBloqueado.passagemPorTile[indice] = 0;
    }
    conferir(!acharTileLivreEmVoltaDe(mapaTodoBloqueado, {1, 1}, 0).has_value(), "num mapa todo bloqueado, não há onde nascer");
}

void testarAnimacaoEPosicaoAnterior()
{
    const MapaLocal mapa = criarMapaVazio(16, 16);
    const RegrasDoMovimento regras;
    BuscadorDeCaminho buscador;
    entt::registry registro;
    const entt::entity npc = criarNpcNoTile(registro, {2, 2});
    mandarONpcPara(registro, npc, {9, 2}, mapa, buscador, regras);
    andarVariosTicks(registro, mapa, regras, 2);
    registro.get<PosicaoDoNpc>(npc).distanciaNoCicloDeAndarEmMilesimos = 2300;
    const LugarDoNpc lugarAntes = registro.get<PosicaoDoNpc>(npc).lugarAtual;
    andarVariosTicks(registro, mapa, regras, 1);
    const PosicaoDoNpc& posicao = registro.get<PosicaoDoNpc>(npc);
    conferir(posicao.distanciaNoCicloDeAndarEmMilesimos == 60, "a animação: 2.300 + 160 volta ao começo do ciclo, em 60");
    conferir(posicao.lugarNoTickAnteriorParaDesenho.progressoAteOProximoTileEmMilesimos == lugarAntes.progressoAteOProximoTileEmMilesimos
                 && posicao.lugarAtual.progressoAteOProximoTileEmMilesimos == lugarAntes.progressoAteOProximoTileEmMilesimos + 160,
             "o NPC guarda a posição do tick anterior para o desenho suave (FL4-5)");
}

}  // namespace

void testarMovimentoDosNpcs()
{
    std::printf("Movimento dos NPCs\n");
    testarContasDoMovimento();
    testarAndarReto();
    testarAndarNaDiagonalEParaCima();
    testarCaminhoBloqueadoENovoDestino();
    testarFilaDePedidos();
    testarDestinoSemCaminho();
    testarOndeONpcNasce();
    testarAnimacaoEPosicaoAnterior();
}
