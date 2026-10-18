#pragma once

// O Mapa Local lido do Tiled: as camadas de tiles, a passagem (onde dá para
// andar), os lotes e as áreas de recurso. Ficha Lógica: Mapa.
//
// Aqui não se desenha nada: o Jogo/ lê estes dados e desenha.

#include <cstdint>
#include <string>
#include <vector>

// Os nomes das camadas de tiles no Tiled (FL1-5), na ordem de desenho.
inline const char* const nomesDasCamadasObrigatorias[] = {
    "chao", "detalhes_do_chao", "objetos_baixos", "construcoes_e_paredes", "acima_do_personagem", "colisao",
};
inline const char* const nomeDaCamadaOpcionalDeElevacao = "elevacao";

struct CamadaDeTiles {
    std::string nomeDaCamada;
    // Um número por tile, linha por linha (índice = y x largura + x). 0 = vazio.
    std::vector<uint16_t> numeroDoTilePorPosicao;
};

struct ConjuntoDeTiles {
    int primeiroNumeroDoConjunto = 1;
    std::string caminhoDaImagem;  // já com a pasta do mapa na frente
    int colunasNaImagem = 0;
    int quantidadeDeTiles = 0;
};

struct PosicaoDeTile {
    int tileX = 0;
    int tileY = 0;
};

// Um retângulo medido em tiles.
struct RetanguloEmTiles {
    int tileX = 0;
    int tileY = 0;
    int larguraEmTiles = 0;
    int alturaEmTiles = 0;
};

enum class PorteDoLote { Pequeno, Medio, Grande };

constexpr int loteSemConstrucao = -1;

struct LoteDeConstrucao {
    int identificadorDoLote = 0;
    PosicaoDeTile tileDoCantoSuperiorEsquerdo;
    PorteDoLote porteDoLote = PorteDoLote::Pequeno;
    std::string tipoDeConstrucaoPermitido = "qualquer";
    int idDaConstrucaoQueOcupa = loteSemConstrucao;  // estado: muda no marco 6
};

struct AreaDeRecurso {
    std::string tipoDeRecurso;  // o nome da área (campo_de_grao, poco, bosque), ligado a um recurso do recursos.csv
    RetanguloEmTiles areaEmTiles;
    std::string textoDaProdutividade;  // a propriedade "produtividade" do Tiled: "", "normal" ou "destaque" (FL3-6)

    // Preenchidos pela ligação aos recursos (ficha dos Recursos, regra 4.2).
    int numeroDoRecurso = -1;
    int produtividadeEmPorcento = 100;
};

// Onde recortar um tile dentro da imagem do conjunto, em pixels.
struct RecorteNaImagem {
    int pixelX = 0;
    int pixelY = 0;
    int larguraEmPixels = 0;
    int alturaEmPixels = 0;
};

struct MapaLocal {
    int larguraEmTiles = 0;
    int alturaEmTiles = 0;
    int tamanhoDoTileEmPixels = 16;

    std::vector<CamadaDeTiles> camadasDeTiles;
    std::vector<ConjuntoDeTiles> conjuntosDeTiles;
    std::vector<LoteDeConstrucao> lotesDeConstrucao;
    std::vector<AreaDeRecurso> areasDeRecurso;

    // 1 = dá para andar, 0 = bloqueado. Recalculada da camada colisao (não vai no save).
    std::vector<uint8_t> passagemPorTile;

    // Posição do tile (x, y) na lista de uma camada.
    int indiceDoTileNaLista(int tileX, int tileY) const { return tileY * larguraEmTiles + tileX; }

    bool tileEstaDentroDoMapa(int tileX, int tileY) const;

    // Regra 4.2: fora do mapa, a resposta é "não".
    bool tileDaParaAndar(int tileX, int tileY) const;

    // Devolve nullptr se a camada não existe.
    const CamadaDeTiles* procurarCamadaPeloNome(const std::string& nomeDaCamada) const;

    // O conjunto de maior primeiro número que ainda seja menor ou igual ao tile.
    // Devolve nullptr se o tile não pertence a nenhum conjunto.
    const ConjuntoDeTiles* procurarConjuntoDoTile(int numeroDoTile) const;
};

// O resultado da leitura: o mapa e todos os erros achados (não só o primeiro).
struct ResultadoDaLeituraDoMapa {
    MapaLocal mapa;
    std::vector<std::string> listaDeErrosDoMapa;
    std::vector<std::string> listaDeAvisosDoMapa;  // por exemplo, uma camada com nome desconhecido

    bool mapaEstaPronto() const { return listaDeErrosDoMapa.empty(); }
};

// Regra 4.1: lê o arquivo .tmj do Tiled e confere tudo.
ResultadoDaLeituraDoMapa carregarMapaDoArquivoDoTiled(const std::string& caminhoDoArquivo);

// O mesmo, a partir do texto do arquivo (usado pelos testes).
// A pasta serve para achar a imagem do conjunto de tiles.
ResultadoDaLeituraDoMapa carregarMapaDoTextoDoTiled(const std::string& textoDoArquivo, const std::string& pastaDoArquivo);

// Contas da parte 5 da ficha, separadas para poderem ser testadas sozinhas.
RecorteNaImagem calcularRecorteDoTileNaImagem(const ConjuntoDeTiles& conjunto, int numeroDoTile, int tamanhoDoTileEmPixels);

// Converte um retângulo do Tiled (em pixels) para tiles. Devolve false se a
// posição ou o tamanho não forem múltiplos do tamanho do tile.
bool converterRetanguloDePixelsParaTiles(double pixelX, double pixelY, double larguraEmPixels, double alturaEmPixels,
                                         int tamanhoDoTileEmPixels, RetanguloEmTiles& retanguloEmTiles);

// 4 x 4 = Pequeno, 6 x 6 = Médio, 8 x 8 = Grande (FT1-4). Devolve false para outro tamanho.
bool descobrirPorteDoLote(int larguraEmTiles, int alturaEmTiles, PorteDoLote& porteDoLote);
