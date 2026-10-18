#include "BonecoDoNpc.h"

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

namespace {

// Lê um número inteiro maior ou igual ao mínimo; se faltar, anota o erro.
void lerInteiroDoBoneco(const Json& documento, const char* nomeDoCampo, int valorMinimo, int& valorLido,
                        std::vector<std::string>& errosDoBoneco)
{
    const auto campo = documento.find(nomeDoCampo);
    if (campo == documento.end() || !campo->is_number_integer() || campo->get<int>() < valorMinimo) {
        errosDoBoneco.push_back("falta o número inteiro \"" + std::string(nomeDoCampo) + "\" (no mínimo "
                                + std::to_string(valorMinimo) + ")");
        return;
    }
    valorLido = campo->get<int>();
}

}  // namespace

DefinicaoDoBoneco lerDefinicaoDoBoneco(const std::string& textoDoArquivo, std::vector<std::string>& errosDoBoneco)
{
    DefinicaoDoBoneco definicao;
    const Json documento = Json::parse(textoDoArquivo, nullptr, false);
    if (documento.is_discarded() || !documento.is_object()) {
        errosDoBoneco.push_back("o arquivo não é um JSON válido");
        return definicao;
    }

    lerInteiroDoBoneco(documento, "tamanho_do_quadro_em_pixels", 1, definicao.tamanhoDoQuadroEmPixels, errosDoBoneco);
    lerInteiroDoBoneco(documento, "linha_parado", 0, definicao.linhaParado, errosDoBoneco);
    lerInteiroDoBoneco(documento, "linha_andando", 0, definicao.linhaAndando, errosDoBoneco);
    lerInteiroDoBoneco(documento, "quadros_de_andar", 1, definicao.quadrosDeAndar, errosDoBoneco);
    lerInteiroDoBoneco(documento, "distancia_por_quadro_de_andar_em_milesimos", 1, definicao.distanciaPorQuadroDeAndarEmMilesimos, errosDoBoneco);

    const auto pes = documento.find("pes_no_quadro");
    if (pes == documento.end() || !pes->is_array() || pes->size() != 2 || !(*pes)[0].is_number_integer() || !(*pes)[1].is_number_integer()) {
        errosDoBoneco.push_back("falta \"pes_no_quadro\": dois números inteiros, x e y");
    } else {
        definicao.pesXNoQuadro = (*pes)[0].get<int>();
        definicao.pesYNoQuadro = (*pes)[1].get<int>();
    }

    const auto camadas = documento.find("camadas");
    if (camadas == documento.end() || !camadas->is_array() || camadas->empty()) {
        errosDoBoneco.push_back("falta \"camadas\": a lista das imagens, de baixo para cima");
    } else {
        for (const Json& camada : *camadas) {
            if (camada.is_string()) {
                definicao.caminhosDasCamadas.push_back(camada.get<std::string>());
            } else {
                errosDoBoneco.push_back("cada camada precisa ser o caminho de uma imagem, entre aspas");
            }
        }
    }

    const auto reserva = documento.find("imagem_de_reserva");
    if (reserva == documento.end() || !reserva->is_string()) {
        errosDoBoneco.push_back("falta \"imagem_de_reserva\": o caminho do boneco de teste");
    } else {
        definicao.caminhoDaImagemDeReserva = reserva->get<std::string>();
    }
    return definicao;
}

QuadroNaFolha escolherQuadroDoBoneco(const PosicaoDoNpc& posicao, const DefinicaoDoBoneco& definicao)
{
    const int direcao = static_cast<int>(posicao.direcaoDoOlhar);
    if (posicao.lugarAtual.estaParado()) {
        return {0, definicao.linhaParado + direcao};
    }
    // Um quadro novo a cada 400 milésimos andados: os pés acompanham o chão.
    const int coluna = (posicao.distanciaNoCicloDeAndarEmMilesimos / definicao.distanciaPorQuadroDeAndarEmMilesimos) % definicao.quadrosDeAndar;
    return {coluna, definicao.linhaAndando + direcao};
}

PontoEmPixelsDoMundo interpolarPesEntreOsTicks(const PontoEmPixelsDoMundo& pesNoTickAnterior,
                                               const PontoEmPixelsDoMundo& pesNoTickAtual, double fracaoDoTick)
{
    return {pesNoTickAnterior.pixelX + (pesNoTickAtual.pixelX - pesNoTickAnterior.pixelX) * fracaoDoTick,
            pesNoTickAnterior.pixelY + (pesNoTickAtual.pixelY - pesNoTickAnterior.pixelY) * fracaoDoTick};
}
