// Os testes do boneco e do clique no mapa: a tabela do Jogo/ na parte 8 da
// Ficha Lógica: NPC (marco 4).

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "BonecoDoNpc.h"
#include "CameraDoMapa.h"
#include "ConferenciaDosTestes.h"

namespace {

PosicaoDoNpc criarPosicao(PosicaoDeTile tileAtual, PosicaoDeTile proximoTile, DirecaoDoOlhar direcao, int distanciaNoCiclo)
{
    PosicaoDoNpc posicao;
    posicao.lugarAtual.tileAtual = tileAtual;
    posicao.lugarAtual.proximoTile = proximoTile;
    posicao.direcaoDoOlhar = direcao;
    posicao.distanciaNoCicloDeAndarEmMilesimos = distanciaNoCiclo;
    return posicao;
}

void testarQuadroDoBoneco()
{
    const DefinicaoDoBoneco definicao;  // os números do boneco provisório
    const QuadroNaFolha parado = escolherQuadroDoBoneco(criarPosicao({5, 5}, {5, 5}, DirecaoDoOlhar::Baixo, 0), definicao);
    conferir(parado.coluna == 0 && parado.linha == 0, "parado, olhando para Baixo: coluna 0, linha 0");

    const QuadroNaFolha paraADireita = escolherQuadroDoBoneco(criarPosicao({5, 5}, {6, 5}, DirecaoDoOlhar::Direita, 1000), definicao);
    conferir(paraADireita.coluna == 2 && paraADireita.linha == 6, "andando para a Direita, distância 1.000: coluna 2, linha 6");

    const QuadroNaFolha paraAEsquerda = escolherQuadroDoBoneco(criarPosicao({5, 5}, {4, 5}, DirecaoDoOlhar::Esquerda, 2399), definicao);
    conferir(paraAEsquerda.coluna == 5 && paraAEsquerda.linha == 7, "andando para a Esquerda, distância 2.399: coluna 5, linha 7");

    const QuadroNaFolha paradoDeCostas = escolherQuadroDoBoneco(criarPosicao({5, 5}, {5, 5}, DirecaoDoOlhar::Cima, 1000), definicao);
    conferir(paradoDeCostas.coluna == 0 && paradoDeCostas.linha == 1, "parado, olhando para Cima: coluna 0, linha 1 (a distância não conta)");

    const PontoEmPixelsDoMundo meioDoTick = interpolarPesEntreOsTicks({100.0, 56.0}, {102.56, 56.0}, 0.5);
    conferir(std::fabs(meioDoTick.pixelX - 101.28) < 0.001 && meioDoTick.pixelY == 56.0, "fração do tick 0,5 entre x = 100 e x = 102,56: x = 101,28");
}

void testarArquivoDoBoneco()
{
    std::ifstream arquivo(std::string(PASTA_DOS_DADOS_DO_JOGO) + "/personagens/boneco_provisorio.json", std::ios::binary);
    const std::string texto((std::istreambuf_iterator<char>(arquivo)), std::istreambuf_iterator<char>());
    std::vector<std::string> erros;
    const DefinicaoDoBoneco definicao = lerDefinicaoDoBoneco(texto, erros);
    conferir(erros.empty() && definicao.tamanhoDoQuadroEmPixels == 64 && definicao.pesXNoQuadro == 32 && definicao.pesYNoQuadro == 43
                 && definicao.caminhosDasCamadas.size() == 3 && definicao.caminhoDaImagemDeReserva == "imagens/boneco_de_teste.png",
             "o boneco_provisorio.json da fatia: quadro de 64, pés em (32, 43), 3 camadas e a reserva");
    conferir(definicao.distanciaDoCicloDeAndarEmMilesimos() == 2400, "o ciclo do andar mede 6 × 400 = 2.400 milésimos");

    std::vector<std::string> errosDoQuebrado;
    lerDefinicaoDoBoneco(R"({"tamanho_do_quadro_em_pixels": 0, "pes_no_quadro": [32]})", errosDoQuebrado);
    conferir(errosDoQuebrado.size() == 8, "um arquivo do boneco quebrado mostra todos os erros (8), não só o primeiro");
}

void testarCliqueNoMapa()
{
    CameraDoMapa camera;
    camera.atualizarTamanhoDaTela(1600, 837);
    camera.definirTamanhoDoMapa(1024, 1024);
    camera.posicionarNoMundo(0.0, 0.0);
    const std::optional<PosicaoDeTile> tile = camera.converterPontoDaTelaEmTile(480.0, 150.0, 16);
    conferir(tile.has_value() && *tile == PosicaoDeTile{10, 3}, "mouse em (480, 150), câmera (0, 0), zoom 3x: tile (10, 3)");

    camera.posicionarNoMundo(800.0, 0.0);
    conferir(!camera.converterPontoDaTelaEmTile(1500.0, 10.0, 16).has_value(), "mouse depois da borda direita do mapa: nenhum tile");
}

}  // namespace

void testarBonecoDoNpc()
{
    std::printf("Boneco do NPC e clique no mapa\n");
    testarQuadroDoBoneco();
    testarArquivoDoBoneco();
    testarCliqueNoMapa();
}
