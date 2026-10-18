"""Gera a arte e os mapas de teste do marco 1 (rodada FL1-6).

O que este script cria (sobrescreve se já existir):
  - Jogo/data/imagens/terreno_de_teste.png: 16 tiles de 16 x 16 px, de cor lisa;
  - Jogo/data/mapas/vila.tmj: o mapa da vila, 64 x 64 tiles, no padrão da
    ficha do Mapa (camadas, 8 lotes pequenos, 3 médios, 3 áreas de recurso);
  - Codigo/Testes/mapas/mapa_de_teste_8x8.tmj: um mapa pequeno para os testes.

Depois de gerado, o vila.tmj pode ser aberto e mudado no Tiled à vontade.
Rodar este script de novo APAGA as mudanças feitas no Tiled.

Como rodar, na pasta do projeto:
    python -I Jogo/assets/teste/gerar_arte_e_mapa_de_teste.py

Só usa a biblioteca padrão do Python (nada para instalar).
"""

import json
import pathlib
import random
import struct
import zlib

PASTA_DO_PROJETO = pathlib.Path(__file__).resolve().parents[3]
TAMANHO_DO_TILE = 16
COLUNAS_NO_CONJUNTO = 8
LINHAS_NO_CONJUNTO = 2

# Número de cada tile no mapa (o Tiled conta a partir de 1; 0 = vazio).
GRAMA, GRAMA_CLARA, TERRA, AGUA, CAMINHO, FLORES, PEDRA, CERCA = range(1, 9)
LAVOURA, PAREDE, COPA_DE_ARVORE, TRONCO, COLISAO, POCO, GRAMA_ESCURA, AREIA = range(9, 17)


# ---------------------------------------------------------------------------
# A imagem PNG, escrita à mão (sem biblioteca de imagens)
# ---------------------------------------------------------------------------

def gravar_png(caminho, largura, altura, pixels_rgba):
    """Grava uma lista de (r, g, b, a) como PNG."""
    linhas = bytearray()
    for y in range(altura):
        linhas.append(0)  # filtro "nenhum" no começo de cada linha
        for x in range(largura):
            linhas.extend(pixels_rgba[y * largura + x])

    def pedaco(tipo, dados):
        corpo = tipo + dados
        return struct.pack(">I", len(dados)) + corpo + struct.pack(">I", zlib.crc32(corpo) & 0xFFFFFFFF)

    cabecalho = struct.pack(">IIBBBBB", largura, altura, 8, 6, 0, 0, 0)  # 8 bits, RGBA
    arquivo = b"\x89PNG\r\n\x1a\n" + pedaco(b"IHDR", cabecalho) + pedaco(b"IDAT", zlib.compress(bytes(linhas), 9)) + pedaco(b"IEND", b"")
    caminho.parent.mkdir(parents=True, exist_ok=True)
    caminho.write_bytes(arquivo)


def desenhar_tile(numero_do_tile):
    """Devolve os 16 x 16 pixels (r, g, b, a) de um tile de teste."""
    sorteio = random.Random(numero_do_tile)  # cada tile sempre sai igual
    transparente = (0, 0, 0, 0)
    pixels = []
    for y in range(TAMANHO_DO_TILE):
        for x in range(TAMANHO_DO_TILE):
            borda = x in (0, 15) or y in (0, 15)
            centro = (x - 7.5) ** 2 + (y - 7.5) ** 2
            if numero_do_tile == GRAMA:
                cor = (60, 120, 50, 255) if sorteio.random() > 0.12 else (48, 100, 40, 255)
            elif numero_do_tile == GRAMA_CLARA:
                cor = (80, 140, 60, 255) if sorteio.random() > 0.12 else (64, 120, 50, 255)
            elif numero_do_tile == TERRA:
                cor = (130, 95, 60, 255) if sorteio.random() > 0.1 else (110, 80, 50, 255)
            elif numero_do_tile == AGUA:
                cor = (110, 160, 210, 255) if (x + 2 * y) % 8 == 0 else (50, 100, 170, 255)
            elif numero_do_tile == CAMINHO:
                cor = (200, 170, 115, 255) if sorteio.random() > 0.1 else (180, 150, 100, 255)
            elif numero_do_tile == FLORES:
                cor = transparente
                if (x * 7 + y * 3) % 23 == 0:
                    cor = (240, 220, 60, 255) if (x + y) % 2 else (230, 90, 120, 255)
            elif numero_do_tile == PEDRA:
                cor = (130, 130, 135, 255) if centro < 30 else transparente
                if 22 < centro < 30:
                    cor = (90, 90, 95, 255)
            elif numero_do_tile == CERCA:
                cor = (120, 80, 40, 255) if y in (5, 6, 10, 11) or x in (2, 3, 12, 13) else transparente
            elif numero_do_tile == LAVOURA:
                cor = (100, 70, 40, 255) if x % 4 in (0, 1) else (90, 150, 50, 255)
            elif numero_do_tile == PAREDE:
                tijolo_borda = y % 4 == 3 or (x + (4 if (y // 4) % 2 else 0)) % 8 == 7
                cor = (90, 90, 95, 255) if tijolo_borda else (150, 150, 155, 255)
            elif numero_do_tile == COPA_DE_ARVORE:
                cor = (30, 85, 35, 230) if centro < 56 else transparente
            elif numero_do_tile == TRONCO:
                cor = (100, 65, 30, 255) if 5 <= x <= 10 and y >= 2 else transparente
            elif numero_do_tile == COLISAO:
                no_x = abs(x - y) <= 1 or abs(x + y - 15) <= 1
                cor = (220, 30, 30, 255) if no_x or borda else (220, 30, 30, 90)
            elif numero_do_tile == POCO:
                cor = transparente
                if centro < 49:
                    cor = (120, 120, 125, 255)
                if centro < 20:
                    cor = (40, 80, 150, 255)
            elif numero_do_tile == GRAMA_ESCURA:
                cor = (40, 90, 40, 255) if sorteio.random() > 0.15 else (30, 75, 32, 255)
            elif numero_do_tile == AREIA:
                cor = (215, 200, 150, 255)
            else:
                cor = transparente
            pixels.append(cor)
    return pixels


def gerar_conjunto_de_tiles(caminho_da_imagem):
    largura = COLUNAS_NO_CONJUNTO * TAMANHO_DO_TILE
    altura = LINHAS_NO_CONJUNTO * TAMANHO_DO_TILE
    imagem = [(0, 0, 0, 0)] * (largura * altura)
    for numero_do_tile in range(1, COLUNAS_NO_CONJUNTO * LINHAS_NO_CONJUNTO + 1):
        local = numero_do_tile - 1
        coluna, linha = local % COLUNAS_NO_CONJUNTO, local // COLUNAS_NO_CONJUNTO
        pixels = desenhar_tile(numero_do_tile)
        for y in range(TAMANHO_DO_TILE):
            for x in range(TAMANHO_DO_TILE):
                imagem[(linha * TAMANHO_DO_TILE + y) * largura + coluna * TAMANHO_DO_TILE + x] = pixels[y * TAMANHO_DO_TILE + x]
    gravar_png(caminho_da_imagem, largura, altura, imagem)


# ---------------------------------------------------------------------------
# O arquivo do Tiled (.tmj), no padrão da ficha do Mapa
# ---------------------------------------------------------------------------

NOMES_DAS_CAMADAS_DE_TILES = ["chao", "detalhes_do_chao", "objetos_baixos", "construcoes_e_paredes", "acima_do_personagem", "colisao"]


def objeto_retangulo(identificador, nome, tile_x, tile_y, largura_em_tiles, altura_em_tiles, propriedades):
    return {
        "id": identificador, "name": nome, "type": "", "rotation": 0, "visible": True,
        "x": tile_x * TAMANHO_DO_TILE, "y": tile_y * TAMANHO_DO_TILE,
        "width": largura_em_tiles * TAMANHO_DO_TILE, "height": altura_em_tiles * TAMANHO_DO_TILE,
        "properties": [{"name": nome_da_propriedade, "type": "string", "value": valor} for nome_da_propriedade, valor in propriedades.items()],
    }


def gravar_tmj(caminho, largura, altura, camadas_de_tiles, lotes, recursos, caminho_da_imagem_a_partir_do_mapa):
    """Grava o mapa no formato JSON do Tiled, com as camadas em CSV (números simples)."""
    camadas = []
    identificador_da_camada = 1
    for nome in NOMES_DAS_CAMADAS_DE_TILES:
        camadas.append({"id": identificador_da_camada, "name": nome, "type": "tilelayer", "x": 0, "y": 0,
                        "width": largura, "height": altura, "opacity": 1, "visible": nome != "colisao",
                        "data": "__DADOS_%s__" % nome})
        identificador_da_camada += 1
    for nome, objetos in (("lotes", lotes), ("recursos", recursos)):
        camadas.append({"id": identificador_da_camada, "name": nome, "type": "objectgroup", "draworder": "topdown",
                        "x": 0, "y": 0, "opacity": 1, "visible": True, "objects": objetos})
        identificador_da_camada += 1

    mapa = {
        "type": "map", "version": "1.10", "tiledversion": "1.11.2", "orientation": "orthogonal",
        "renderorder": "right-down", "infinite": False, "compressionlevel": -1,
        "width": largura, "height": altura, "tilewidth": TAMANHO_DO_TILE, "tileheight": TAMANHO_DO_TILE,
        "nextlayerid": identificador_da_camada, "nextobjectid": max(o["id"] for o in lotes + recursos) + 1,
        "tilesets": [{
            "firstgid": 1, "name": "terreno_de_teste", "image": caminho_da_imagem_a_partir_do_mapa,
            "imagewidth": COLUNAS_NO_CONJUNTO * TAMANHO_DO_TILE, "imageheight": LINHAS_NO_CONJUNTO * TAMANHO_DO_TILE,
            "columns": COLUNAS_NO_CONJUNTO, "tilecount": COLUNAS_NO_CONJUNTO * LINHAS_NO_CONJUNTO,
            "tilewidth": TAMANHO_DO_TILE, "tileheight": TAMANHO_DO_TILE, "margin": 0, "spacing": 0,
        }],
        "layers": camadas,
    }
    texto = json.dumps(mapa, indent=1, ensure_ascii=False)
    # Cada camada vira uma linha de números por linha do mapa, como o Tiled faz.
    for nome in NOMES_DAS_CAMADAS_DE_TILES:
        dados = camadas_de_tiles[nome]
        linhas = [",".join(str(n) for n in dados[y * largura:(y + 1) * largura]) for y in range(altura)]
        texto = texto.replace('"__DADOS_%s__"' % nome, "[" + ",\n  ".join(linhas) + "]")
    caminho.parent.mkdir(parents=True, exist_ok=True)
    caminho.write_text(texto + "\n", encoding="utf-8", newline="\n")


def gerar_vila(caminho_do_mapa):
    largura = altura = 64
    camadas = {nome: [0] * (largura * altura) for nome in NOMES_DAS_CAMADAS_DE_TILES}
    sorteio = random.Random(412)

    def pintar(nome, x, y, tile):
        if 0 <= x < largura and 0 <= y < altura:
            camadas[nome][y * largura + x] = tile

    def tile_em(nome, x, y):
        return camadas[nome][y * largura + x]

    # Chão: grama, com manchas de grama clara.
    for y in range(altura):
        for x in range(largura):
            pintar("chao", x, y, GRAMA_CLARA if sorteio.random() < 0.15 else GRAMA)

    # O bosque (canto de baixo à esquerda), com chão mais escuro.
    for y in range(40, 60):
        for x in range(4, 20):
            pintar("chao", x, y, GRAMA_ESCURA)

    # O lago (canto de baixo à direita), com uma margem de areia. A água bloqueia.
    for y in range(43, 57):
        for x in range(43, 57):
            if 44 <= x <= 55 and 44 <= y <= 55:
                pintar("chao", x, y, AGUA)
                pintar("colisao", x, y, COLISAO)
            else:
                pintar("chao", x, y, AREIA)

    # O campo de Grão (em cima à esquerda): terra com lavoura, cerca em cima.
    for y in range(6, 14):
        for x in range(4, 14):
            pintar("chao", x, y, TERRA)
            pintar("objetos_baixos", x, y, LAVOURA)
    for x in range(4, 14):
        pintar("objetos_baixos", x, 5, CERCA)
        pintar("colisao", x, 5, COLISAO)

    # O poço.
    pintar("objetos_baixos", 24, 8, POCO)
    pintar("colisao", 24, 8, COLISAO)

    # Os caminhos: uma rua de leste a oeste e outra de norte a sul.
    for x in range(4, 61):
        for y in (26, 27):
            pintar("detalhes_do_chao", x, y, CAMINHO)
    for y in range(4, 61):
        for x in (31, 32):
            pintar("detalhes_do_chao", x, y, CAMINHO)

    # Um muro em ruínas (camada de construções fixas), em cima à direita.
    for x in range(54, 61):
        pintar("construcoes_e_paredes", x, 6, PAREDE)
        pintar("colisao", x, 6, COLISAO)

    # Os lotes: 8 pequenos (4 x 4) e 3 médios (6 x 6), na beira das ruas.
    posicoes_dos_lotes = (
        [("pequeno", x, 21, 4) for x in (12, 17, 22, 35, 40)]
        + [("pequeno", x, 29, 4) for x in (12, 17, 22)]
        + [("medio", 45, 19, 6), ("medio", 35, 29, 6), ("medio", 42, 29, 6)]
    )
    tiles_dos_lotes = set()
    lotes = []
    for numero, (porte, x, y, lado) in enumerate(posicoes_dos_lotes, start=1):
        lotes.append(objeto_retangulo(numero, "lote_%s_%d" % (porte, numero), x, y, lado, lado, {"tipo_permitido": "qualquer"}))
        tiles_dos_lotes.update((x + dx, y + dy) for dx in range(lado) for dy in range(lado))

    # As árvores do bosque: tronco embaixo (bloqueia) e copa por cima do personagem.
    for y in range(42, 60, 3):
        for x in range(5, 19, 3):
            tronco_x, tronco_y = x + sorteio.randint(0, 1), y + sorteio.randint(0, 1)
            pintar("objetos_baixos", tronco_x, tronco_y, TRONCO)
            pintar("colisao", tronco_x, tronco_y, COLISAO)
            pintar("acima_do_personagem", tronco_x, tronco_y - 1, COPA_DE_ARVORE)

    # Flores e pedras soltas, longe dos lotes, das ruas e das outras coisas.
    for _ in range(140):
        x, y = sorteio.randrange(largura), sorteio.randrange(altura)
        livre = (x, y) not in tiles_dos_lotes and tile_em("detalhes_do_chao", x, y) == 0 \
            and tile_em("objetos_baixos", x, y) == 0 and tile_em("colisao", x, y) == 0 and tile_em("chao", x, y) in (GRAMA, GRAMA_CLARA)
        if livre:
            if sorteio.random() < 0.8:
                pintar("detalhes_do_chao", x, y, FLORES)
            else:
                pintar("objetos_baixos", x, y, PEDRA)
                pintar("colisao", x, y, COLISAO)

    recursos = [
        objeto_retangulo(20, "campo_de_grao", 4, 6, 10, 8, {"tipo_de_recurso": "campo_de_grao"}),
        objeto_retangulo(21, "poco", 23, 7, 3, 3, {"tipo_de_recurso": "poco"}),
        objeto_retangulo(22, "bosque", 4, 40, 16, 20, {"tipo_de_recurso": "bosque"}),
    ]
    gravar_tmj(caminho_do_mapa, largura, altura, camadas, lotes, recursos, "../imagens/terreno_de_teste.png")


def gerar_mapa_de_teste_8x8(caminho_do_mapa):
    """O mapa pequeno dos testes: grama, uma colisão em (5, 6), um lote pequeno em (0, 0) e um poço."""
    largura = altura = 8
    camadas = {nome: [0] * (largura * altura) for nome in NOMES_DAS_CAMADAS_DE_TILES}
    camadas["chao"] = [GRAMA] * (largura * altura)
    camadas["objetos_baixos"][6 * largura + 5] = PEDRA
    camadas["colisao"][6 * largura + 5] = COLISAO
    lotes = [objeto_retangulo(1, "lote_pequeno_1", 0, 0, 4, 4, {"tipo_permitido": "qualquer"})]
    recursos = [objeto_retangulo(2, "poco", 6, 0, 2, 2, {"tipo_de_recurso": "poco"})]
    gravar_tmj(caminho_do_mapa, largura, altura, camadas, lotes, recursos, "terreno_de_teste.png")


if __name__ == "__main__":
    gerar_conjunto_de_tiles(PASTA_DO_PROJETO / "Jogo" / "data" / "imagens" / "terreno_de_teste.png")
    gerar_vila(PASTA_DO_PROJETO / "Jogo" / "data" / "mapas" / "vila.tmj")
    gerar_mapa_de_teste_8x8(PASTA_DO_PROJETO / "Codigo" / "Testes" / "mapas" / "mapa_de_teste_8x8.tmj")
    print("Arte e mapas de teste gerados.")
