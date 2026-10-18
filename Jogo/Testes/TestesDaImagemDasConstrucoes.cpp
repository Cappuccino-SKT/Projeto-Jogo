// Os testes das contas das imagens das construções (ficha da Construção,
// regra 4.3; FA1-5): o nome do arquivo e o tamanho certo.

#include <cstdio>

#include "ConferenciaDosTestes.h"
#include "ImagemDasConstrucoes.h"

void testarImagemDasConstrucoes()
{
    std::printf("Imagens das construções\n");
    conferir(caminhoDaImagemDaConstrucao("construcao.casa", 1) == "imagens/construcoes/casa_nivel_1.png",
             "a Casa no nível 1 é imagens/construcoes/casa_nivel_1.png");
    conferir(caminhoDaImagemDaConstrucao("construcao.carpinteiro", 2) == "imagens/construcoes/carpinteiro_nivel_2.png",
             "a Carpintaria do jogador, no nível 2, é carpinteiro_nivel_2.png");
    const RetanguloEmTiles lotePequeno{17, 21, 4, 4};
    const RetanguloEmTiles loteMedio{45, 19, 6, 6};
    conferir(tamanhoDaImagemDaConstrucaoEstaCerto(64, 96, lotePequeno, 16) && tamanhoDaImagemDaConstrucaoEstaCerto(96, 128, loteMedio, 16),
             "o lote pequeno pede 64 x 96, e o médio, 96 x 128 (FA1-5)");
    conferir(!tamanhoDaImagemDaConstrucaoEstaCerto(64, 64, lotePequeno, 16), "uma imagem de 64 x 64 no lote pequeno está errada (falta o telhado)");
}
