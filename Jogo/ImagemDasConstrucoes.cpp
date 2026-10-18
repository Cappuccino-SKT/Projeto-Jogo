#include "ImagemDasConstrucoes.h"

std::string caminhoDaImagemDaConstrucao(const std::string& tipoDaConstrucao, int nivel)
{
    const std::string prefixo = "construcao.";
    const std::string nomeSemPrefixo = tipoDaConstrucao.rfind(prefixo, 0) == 0 ? tipoDaConstrucao.substr(prefixo.size()) : tipoDaConstrucao;
    return "imagens/construcoes/" + nomeSemPrefixo + "_nivel_" + std::to_string(nivel) + ".png";
}

bool tamanhoDaImagemDaConstrucaoEstaCerto(int larguraEmPixels, int alturaEmPixels, const RetanguloEmTiles& areaDoLote, int tamanhoDoTileEmPixels)
{
    return larguraEmPixels == areaDoLote.larguraEmTiles * tamanhoDoTileEmPixels
        && alturaEmPixels == areaDoLote.alturaEmTiles * tamanhoDoTileEmPixels + alturaDaImagemAcimaDoLoteEmPixels;
}
