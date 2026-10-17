#include "Mapa/MapaLocal.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iterator>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

// ---------------------------------------------------------------------------
// Perguntas sobre o mapa já lido
// ---------------------------------------------------------------------------

bool MapaLocal::tileEstaDentroDoMapa(int tileX, int tileY) const
{
    return tileX >= 0 && tileY >= 0 && tileX < larguraEmTiles && tileY < alturaEmTiles;
}

bool MapaLocal::tileDaParaAndar(int tileX, int tileY) const
{
    if (!tileEstaDentroDoMapa(tileX, tileY)) {
        return false;
    }
    return passagemPorTile[indiceDoTileNaLista(tileX, tileY)] == 1;
}

const CamadaDeTiles* MapaLocal::procurarCamadaPeloNome(const std::string& nomeDaCamada) const
{
    for (const CamadaDeTiles& camada : camadasDeTiles) {
        if (camada.nomeDaCamada == nomeDaCamada) {
            return &camada;
        }
    }
    return nullptr;
}

const ConjuntoDeTiles* MapaLocal::procurarConjuntoDoTile(int numeroDoTile) const
{
    const ConjuntoDeTiles* conjuntoEncontrado = nullptr;
    for (const ConjuntoDeTiles& conjunto : conjuntosDeTiles) {
        if (conjunto.primeiroNumeroDoConjunto <= numeroDoTile) {
            if (conjuntoEncontrado == nullptr || conjunto.primeiroNumeroDoConjunto > conjuntoEncontrado->primeiroNumeroDoConjunto) {
                conjuntoEncontrado = &conjunto;
            }
        }
    }
    if (conjuntoEncontrado == nullptr) {
        return nullptr;
    }
    const int numeroLocalDoTile = numeroDoTile - conjuntoEncontrado->primeiroNumeroDoConjunto;
    if (numeroLocalDoTile >= conjuntoEncontrado->quantidadeDeTiles) {
        return nullptr;  // passou do último tile do conjunto
    }
    return conjuntoEncontrado;
}

// ---------------------------------------------------------------------------
// Contas da parte 5
// ---------------------------------------------------------------------------

RecorteNaImagem calcularRecorteDoTileNaImagem(const ConjuntoDeTiles& conjunto, int numeroDoTile, int tamanhoDoTileEmPixels)
{
    const int numeroLocalDoTile = numeroDoTile - conjunto.primeiroNumeroDoConjunto;
    const int colunaNaImagem = numeroLocalDoTile % conjunto.colunasNaImagem;
    const int linhaNaImagem = numeroLocalDoTile / conjunto.colunasNaImagem;
    return {colunaNaImagem * tamanhoDoTileEmPixels, linhaNaImagem * tamanhoDoTileEmPixels,
            tamanhoDoTileEmPixels, tamanhoDoTileEmPixels};
}

bool converterRetanguloDePixelsParaTiles(double pixelX, double pixelY, double larguraEmPixels, double alturaEmPixels,
                                         int tamanhoDoTileEmPixels, RetanguloEmTiles& retanguloEmTiles)
{
    const auto ehMultiploDoTile = [tamanhoDoTileEmPixels](double valorEmPixels) {
        return std::fmod(valorEmPixels, tamanhoDoTileEmPixels) == 0.0;
    };
    if (!ehMultiploDoTile(pixelX) || !ehMultiploDoTile(pixelY) || !ehMultiploDoTile(larguraEmPixels) || !ehMultiploDoTile(alturaEmPixels)) {
        return false;
    }
    retanguloEmTiles.tileX = static_cast<int>(pixelX) / tamanhoDoTileEmPixels;
    retanguloEmTiles.tileY = static_cast<int>(pixelY) / tamanhoDoTileEmPixels;
    retanguloEmTiles.larguraEmTiles = static_cast<int>(larguraEmPixels) / tamanhoDoTileEmPixels;
    retanguloEmTiles.alturaEmTiles = static_cast<int>(alturaEmPixels) / tamanhoDoTileEmPixels;
    return true;
}

bool descobrirPorteDoLote(int larguraEmTiles, int alturaEmTiles, PorteDoLote& porteDoLote)
{
    if (larguraEmTiles != alturaEmTiles) {
        return false;
    }
    switch (larguraEmTiles) {
    case 4: porteDoLote = PorteDoLote::Pequeno; return true;
    case 6: porteDoLote = PorteDoLote::Medio; return true;
    case 8: porteDoLote = PorteDoLote::Grande; return true;
    default: return false;
    }
}

// ---------------------------------------------------------------------------
// Ajudantes para ler o JSON sem quebrar quando um campo falta
// ---------------------------------------------------------------------------

namespace {

bool lerNumeroInteiro(const Json& objeto, const char* nomeDoCampo, int& valorLido)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end() || !campo->is_number_integer()) {
        return false;
    }
    valorLido = campo->get<int>();
    return true;
}

bool lerNumeroComVirgula(const Json& objeto, const char* nomeDoCampo, double& valorLido)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end() || !campo->is_number()) {
        return false;
    }
    valorLido = campo->get<double>();
    return true;
}

bool lerTexto(const Json& objeto, const char* nomeDoCampo, std::string& valorLido)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end() || !campo->is_string()) {
        return false;
    }
    valorLido = campo->get<std::string>();
    return true;
}

// As propriedades próprias de um objeto do Tiled ficam numa lista
// [{"name": ..., "type": ..., "value": ...}]. Devolve "" se não existir.
std::string lerPropriedadeDeTexto(const Json& objetoDoTiled, const std::string& nomeDaPropriedade)
{
    const auto propriedades = objetoDoTiled.find("properties");
    if (propriedades == objetoDoTiled.end() || !propriedades->is_array()) {
        return "";
    }
    for (const Json& propriedade : *propriedades) {
        std::string nome;
        std::string valor;
        if (lerTexto(propriedade, "name", nome) && nome == nomeDaPropriedade && lerTexto(propriedade, "value", valor)) {
            return valor;
        }
    }
    return "";
}

std::string textoDaPosicao(int tileX, int tileY)
{
    return "(" + std::to_string(tileX) + ", " + std::to_string(tileY) + ")";
}

bool nomeEhDeCamadaConhecida(const std::string& nomeDaCamada)
{
    for (const char* nomeObrigatorio : nomesDasCamadasObrigatorias) {
        if (nomeDaCamada == nomeObrigatorio) {
            return true;
        }
    }
    return nomeDaCamada == nomeDaCamadaOpcionalDeElevacao;
}

bool retangulosSeCobrem(const RetanguloEmTiles& primeiro, const RetanguloEmTiles& segundo)
{
    return primeiro.tileX < segundo.tileX + segundo.larguraEmTiles && segundo.tileX < primeiro.tileX + primeiro.larguraEmTiles
        && primeiro.tileY < segundo.tileY + segundo.alturaEmTiles && segundo.tileY < primeiro.tileY + primeiro.alturaEmTiles;
}

// Os bits mais altos de um número de tile do Tiled marcam espelhado ou girado.
constexpr uint64_t bitsDeEspelhamentoDoTiled = 0xF0000000u;

// Para não encher a lista se uma camada inteira estiver errada.
constexpr int limiteDeErrosPorCamada = 10;

}  // namespace

// ---------------------------------------------------------------------------
// Regra 4.1: carregar e conferir o mapa
// ---------------------------------------------------------------------------

ResultadoDaLeituraDoMapa carregarMapaDoArquivoDoTiled(const std::string& caminhoDoArquivo)
{
    std::ifstream arquivo(caminhoDoArquivo, std::ios::binary);
    if (!arquivo) {
        ResultadoDaLeituraDoMapa resultado;
        resultado.listaDeErrosDoMapa.push_back("não foi possível abrir o arquivo do mapa: " + caminhoDoArquivo);
        return resultado;
    }
    const std::string textoDoArquivo((std::istreambuf_iterator<char>(arquivo)), std::istreambuf_iterator<char>());
    const std::string pastaDoArquivo = std::filesystem::path(caminhoDoArquivo).parent_path().string();
    return carregarMapaDoTextoDoTiled(textoDoArquivo, pastaDoArquivo);
}

ResultadoDaLeituraDoMapa carregarMapaDoTextoDoTiled(const std::string& textoDoArquivo, const std::string& pastaDoArquivo)
{
    ResultadoDaLeituraDoMapa resultado;
    MapaLocal& mapa = resultado.mapa;
    std::vector<std::string>& erros = resultado.listaDeErrosDoMapa;
    std::vector<std::string>& avisos = resultado.listaDeAvisosDoMapa;

    // Passo 1: o texto precisa ser um JSON válido.
    const Json documento = Json::parse(textoDoArquivo, nullptr, false);
    if (documento.is_discarded() || !documento.is_object()) {
        erros.push_back("o arquivo do mapa não é um JSON válido");
        return resultado;
    }

    // Passo 2: o formato.
    std::string orientacao;
    if (!lerTexto(documento, "orientation", orientacao) || orientacao != "orthogonal") {
        erros.push_back("o mapa precisa ter a orientação ortogonal");
    }
    const auto campoInfinito = documento.find("infinite");
    if (campoInfinito != documento.end() && campoInfinito->is_boolean() && campoInfinito->get<bool>()) {
        erros.push_back("o mapa não pode ser infinito (desmarque \"infinito\" nas propriedades do mapa no Tiled)");
    }
    int larguraDoTile = 0;
    int alturaDoTile = 0;
    if (!lerNumeroInteiro(documento, "tilewidth", larguraDoTile) || !lerNumeroInteiro(documento, "tileheight", alturaDoTile)
        || larguraDoTile != mapa.tamanhoDoTileEmPixels || alturaDoTile != mapa.tamanhoDoTileEmPixels) {
        erros.push_back("o tile do mapa precisa ter 16 x 16 pixels");
    }
    if (!lerNumeroInteiro(documento, "width", mapa.larguraEmTiles) || !lerNumeroInteiro(documento, "height", mapa.alturaEmTiles)
        || mapa.larguraEmTiles <= 0 || mapa.alturaEmTiles <= 0) {
        erros.push_back("o mapa precisa ter largura e altura maiores que zero");
        return resultado;  // sem o tamanho, não dá para conferir o resto
    }
    const int quantidadeDeTilesNoMapa = mapa.larguraEmTiles * mapa.alturaEmTiles;

    // Passo 3: os conjuntos de tiles.
    const auto campoConjuntos = documento.find("tilesets");
    if (campoConjuntos == documento.end() || !campoConjuntos->is_array() || campoConjuntos->empty()) {
        erros.push_back("o mapa não tem nenhum conjunto de tiles");
    } else {
        for (const Json& conjuntoNoArquivo : *campoConjuntos) {
            std::string nomeDoConjunto = "sem nome";
            lerTexto(conjuntoNoArquivo, "name", nomeDoConjunto);
            if (conjuntoNoArquivo.contains("source")) {
                erros.push_back("o conjunto de tiles \"" + nomeDoConjunto + "\" está num arquivo separado; ele precisa estar embutido no mapa");
                continue;
            }
            ConjuntoDeTiles conjunto;
            std::string caminhoDaImagemNoArquivo;
            int larguraDaImagem = 0;
            int alturaDaImagem = 0;
            int margem = 0;
            int espacoEntreTiles = 0;
            lerNumeroInteiro(conjuntoNoArquivo, "margin", margem);
            lerNumeroInteiro(conjuntoNoArquivo, "spacing", espacoEntreTiles);
            const bool camposCompletos = lerNumeroInteiro(conjuntoNoArquivo, "firstgid", conjunto.primeiroNumeroDoConjunto)
                && lerNumeroInteiro(conjuntoNoArquivo, "columns", conjunto.colunasNaImagem)
                && lerNumeroInteiro(conjuntoNoArquivo, "tilecount", conjunto.quantidadeDeTiles)
                && lerNumeroInteiro(conjuntoNoArquivo, "imagewidth", larguraDaImagem)
                && lerNumeroInteiro(conjuntoNoArquivo, "imageheight", alturaDaImagem)
                && lerTexto(conjuntoNoArquivo, "image", caminhoDaImagemNoArquivo);
            if (!camposCompletos || conjunto.colunasNaImagem <= 0) {
                erros.push_back("o conjunto de tiles \"" + nomeDoConjunto + "\" está incompleto (falta a imagem, as colunas ou a quantidade)");
                continue;
            }
            if (larguraDaImagem % mapa.tamanhoDoTileEmPixels != 0 || alturaDaImagem % mapa.tamanhoDoTileEmPixels != 0) {
                erros.push_back("a imagem do conjunto \"" + nomeDoConjunto + "\" não tem tamanho múltiplo de 16 pixels");
            }
            if (margem != 0 || espacoEntreTiles != 0) {
                erros.push_back("o conjunto \"" + nomeDoConjunto + "\" precisa ter margem e espaço entre os tiles iguais a 0");
            }
            conjunto.caminhoDaImagem =
                (std::filesystem::path(pastaDoArquivo) / caminhoDaImagemNoArquivo).lexically_normal().string();
            mapa.conjuntosDeTiles.push_back(conjunto);
        }
    }

    // Passo 4: as camadas de tiles (e a localização das camadas de objetos).
    const Json* camadaDeLotes = nullptr;
    const Json* camadaDeRecursos = nullptr;
    const auto campoCamadas = documento.find("layers");
    if (campoCamadas == documento.end() || !campoCamadas->is_array()) {
        erros.push_back("o mapa não tem nenhuma camada");
    } else {
        for (const Json& camadaNoArquivo : *campoCamadas) {
            std::string tipoDaCamada;
            std::string nomeDaCamada;
            lerTexto(camadaNoArquivo, "type", tipoDaCamada);
            lerTexto(camadaNoArquivo, "name", nomeDaCamada);

            if (tipoDaCamada == "objectgroup") {
                if (nomeDaCamada == "lotes") {
                    camadaDeLotes = &camadaNoArquivo;
                } else if (nomeDaCamada == "recursos") {
                    camadaDeRecursos = &camadaNoArquivo;
                } else {
                    avisos.push_back("a camada de objetos \"" + nomeDaCamada + "\" ainda não é usada e foi ignorada");
                }
                continue;
            }
            if (tipoDaCamada != "tilelayer") {
                avisos.push_back("a camada \"" + nomeDaCamada + "\" é de um tipo que o jogo não usa e foi ignorada");
                continue;
            }
            if (!nomeEhDeCamadaConhecida(nomeDaCamada)) {
                avisos.push_back("a camada \"" + nomeDaCamada + "\" tem um nome desconhecido e foi ignorada");
                continue;
            }
            if (mapa.procurarCamadaPeloNome(nomeDaCamada) != nullptr) {
                erros.push_back("a camada \"" + nomeDaCamada + "\" aparece duas vezes");
                continue;
            }

            const auto campoDados = camadaNoArquivo.find("data");
            if (campoDados == camadaNoArquivo.end() || !campoDados->is_array()) {
                erros.push_back("a camada \"" + nomeDaCamada + "\" não está no formato CSV (mude o formato da camada de tiles para CSV nas propriedades do mapa no Tiled)");
                continue;
            }
            if (static_cast<int>(campoDados->size()) != quantidadeDeTilesNoMapa) {
                erros.push_back("a camada \"" + nomeDaCamada + "\" tem " + std::to_string(campoDados->size())
                                + " números, mas o mapa tem " + std::to_string(quantidadeDeTilesNoMapa) + " tiles");
                continue;
            }

            CamadaDeTiles camada;
            camada.nomeDaCamada = nomeDaCamada;
            camada.numeroDoTilePorPosicao.assign(quantidadeDeTilesNoMapa, 0);
            int errosNestaCamada = 0;
            for (int indice = 0; indice < quantidadeDeTilesNoMapa; ++indice) {
                const Json& valorNoArquivo = (*campoDados)[indice];
                const uint64_t numeroDoTile = valorNoArquivo.is_number_unsigned() ? valorNoArquivo.get<uint64_t>() : 0;
                if (numeroDoTile == 0) {
                    continue;
                }
                const int tileX = indice % mapa.larguraEmTiles;
                const int tileY = indice / mapa.larguraEmTiles;
                std::string erroDoTile;
                if ((numeroDoTile & bitsDeEspelhamentoDoTiled) != 0) {
                    erroDoTile = "tile espelhado em " + textoDaPosicao(tileX, tileY) + " na camada \"" + nomeDaCamada + "\"";
                } else if (numeroDoTile > 65535 || mapa.procurarConjuntoDoTile(static_cast<int>(numeroDoTile)) == nullptr) {
                    erroDoTile = "tile desconhecido em " + textoDaPosicao(tileX, tileY) + " na camada \"" + nomeDaCamada + "\"";
                }
                if (!erroDoTile.empty()) {
                    ++errosNestaCamada;
                    if (errosNestaCamada <= limiteDeErrosPorCamada) {
                        erros.push_back(erroDoTile);
                    }
                    continue;
                }
                camada.numeroDoTilePorPosicao[indice] = static_cast<uint16_t>(numeroDoTile);
            }
            if (errosNestaCamada > limiteDeErrosPorCamada) {
                erros.push_back("e mais " + std::to_string(errosNestaCamada - limiteDeErrosPorCamada)
                                + " tiles com erro na camada \"" + nomeDaCamada + "\"");
            }
            mapa.camadasDeTiles.push_back(std::move(camada));
        }
    }

    // Passo 5: as camadas obrigatórias.
    for (const char* nomeObrigatorio : nomesDasCamadasObrigatorias) {
        if (mapa.procurarCamadaPeloNome(nomeObrigatorio) == nullptr) {
            erros.push_back(std::string("falta a camada `") + nomeObrigatorio + "`");
        }
    }

    // Passo 6: a passagem. Tile 0 na colisao = dá para andar.
    mapa.passagemPorTile.assign(quantidadeDeTilesNoMapa, 1);
    if (const CamadaDeTiles* camadaDeColisao = mapa.procurarCamadaPeloNome("colisao")) {
        for (int indice = 0; indice < quantidadeDeTilesNoMapa; ++indice) {
            if (camadaDeColisao->numeroDoTilePorPosicao[indice] != 0) {
                mapa.passagemPorTile[indice] = 0;
            }
        }
    }

    // Passo 7: os lotes.
    std::vector<RetanguloEmTiles> areasDosLotes;
    if (camadaDeLotes == nullptr) {
        erros.push_back("falta a camada de objetos `lotes`");
    } else if (camadaDeLotes->contains("objects") && (*camadaDeLotes)["objects"].is_array()) {
        for (const Json& objetoDoLote : (*camadaDeLotes)["objects"]) {
            LoteDeConstrucao lote;
            lerNumeroInteiro(objetoDoLote, "id", lote.identificadorDoLote);
            const std::string nomeDoLote = "o lote " + std::to_string(lote.identificadorDoLote);

            double pixelX = 0, pixelY = 0, larguraEmPixels = 0, alturaEmPixels = 0;
            lerNumeroComVirgula(objetoDoLote, "x", pixelX);
            lerNumeroComVirgula(objetoDoLote, "y", pixelY);
            lerNumeroComVirgula(objetoDoLote, "width", larguraEmPixels);
            lerNumeroComVirgula(objetoDoLote, "height", alturaEmPixels);

            RetanguloEmTiles areaDoLote;
            if (!converterRetanguloDePixelsParaTiles(pixelX, pixelY, larguraEmPixels, alturaEmPixels, mapa.tamanhoDoTileEmPixels, areaDoLote)) {
                erros.push_back(nomeDoLote + " não está alinhado aos tiles (posição e tamanho precisam ser múltiplos de 16 pixels)");
                continue;
            }
            if (!descobrirPorteDoLote(areaDoLote.larguraEmTiles, areaDoLote.alturaEmTiles, lote.porteDoLote)) {
                erros.push_back(nomeDoLote + " tem " + std::to_string(areaDoLote.larguraEmTiles) + " x " + std::to_string(areaDoLote.alturaEmTiles)
                                + " tiles; o tamanho precisa ser 4 x 4, 6 x 6 ou 8 x 8");
                continue;
            }
            const bool loteCabeNoMapa = mapa.tileEstaDentroDoMapa(areaDoLote.tileX, areaDoLote.tileY)
                && mapa.tileEstaDentroDoMapa(areaDoLote.tileX + areaDoLote.larguraEmTiles - 1, areaDoLote.tileY + areaDoLote.alturaEmTiles - 1);
            if (!loteCabeNoMapa) {
                erros.push_back(nomeDoLote + " sai do mapa");
                continue;
            }
            for (std::size_t outro = 0; outro < areasDosLotes.size(); ++outro) {
                if (retangulosSeCobrem(areaDoLote, areasDosLotes[outro])) {
                    erros.push_back(nomeDoLote + " fica por cima do lote " + std::to_string(mapa.lotesDeConstrucao[outro].identificadorDoLote));
                }
            }
            bool achouTileBloqueado = false;
            for (int tileY = areaDoLote.tileY; tileY < areaDoLote.tileY + areaDoLote.alturaEmTiles && !achouTileBloqueado; ++tileY) {
                for (int tileX = areaDoLote.tileX; tileX < areaDoLote.tileX + areaDoLote.larguraEmTiles; ++tileX) {
                    if (!mapa.tileDaParaAndar(tileX, tileY)) {
                        erros.push_back(nomeDoLote + " tem um tile bloqueado na colisao em " + textoDaPosicao(tileX, tileY));
                        achouTileBloqueado = true;
                        break;
                    }
                }
            }

            const std::string tipoPermitido = lerPropriedadeDeTexto(objetoDoLote, "tipo_permitido");
            if (!tipoPermitido.empty()) {
                lote.tipoDeConstrucaoPermitido = tipoPermitido;
            }
            lote.tileDoCantoSuperiorEsquerdo = {areaDoLote.tileX, areaDoLote.tileY};
            mapa.lotesDeConstrucao.push_back(lote);
            areasDosLotes.push_back(areaDoLote);
        }
    }

    // Passo 8: as áreas de recurso.
    if (camadaDeRecursos == nullptr) {
        erros.push_back("falta a camada de objetos `recursos`");
    } else if (camadaDeRecursos->contains("objects") && (*camadaDeRecursos)["objects"].is_array()) {
        for (const Json& objetoDoRecurso : (*camadaDeRecursos)["objects"]) {
            int identificador = 0;
            lerNumeroInteiro(objetoDoRecurso, "id", identificador);
            const std::string nomeDoRecurso = "a área de recurso " + std::to_string(identificador);

            AreaDeRecurso area;
            area.tipoDeRecurso = lerPropriedadeDeTexto(objetoDoRecurso, "tipo_de_recurso");
            const bool tipoConhecido = area.tipoDeRecurso == "campo_de_grao" || area.tipoDeRecurso == "poco" || area.tipoDeRecurso == "bosque";
            if (!tipoConhecido) {
                erros.push_back(nomeDoRecurso + " tem o tipo \"" + area.tipoDeRecurso + "\"; os tipos conhecidos são campo_de_grao, poco e bosque");
                continue;
            }
            double pixelX = 0, pixelY = 0, larguraEmPixels = 0, alturaEmPixels = 0;
            lerNumeroComVirgula(objetoDoRecurso, "x", pixelX);
            lerNumeroComVirgula(objetoDoRecurso, "y", pixelY);
            lerNumeroComVirgula(objetoDoRecurso, "width", larguraEmPixels);
            lerNumeroComVirgula(objetoDoRecurso, "height", alturaEmPixels);
            if (!converterRetanguloDePixelsParaTiles(pixelX, pixelY, larguraEmPixels, alturaEmPixels, mapa.tamanhoDoTileEmPixels, area.areaEmTiles)
                || area.areaEmTiles.larguraEmTiles <= 0 || area.areaEmTiles.alturaEmTiles <= 0) {
                erros.push_back(nomeDoRecurso + " não está alinhada aos tiles (posição e tamanho precisam ser múltiplos de 16 pixels)");
                continue;
            }
            mapa.areasDeRecurso.push_back(area);
        }
    }

    // Passo 9: com a lista de erros vazia, o mapa está pronto.
    return resultado;
}
