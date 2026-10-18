"""Gera o boneco de teste do marco 4 (rodada FL4-4).

O que este script cria (sobrescreve se já existir):
  - Jogo/data/imagens/boneco_de_teste.png: uma folha de 512 x 512 px, com
    8 x 8 quadros de 64 x 64, no mesmo formato da folha do Mana Seed:
      linhas 0 a 3, coluna 0: parado (baixo, cima, direita, esquerda);
      linhas 4 a 7, colunas 0 a 5: andando, na mesma ordem.
    O boneco tem cerca de 14 x 29 px, com os pés na linha 43 do quadro.

É o boneco de reserva: o jogo usa esta folha quando falta a arte do Mana
Seed (a pasta "Assents Temporarios", que fica fora do Git).

Como rodar, na pasta do projeto:
    python -I Jogo/assets/teste/gerar_boneco_de_teste.py

Só usa a biblioteca padrão do Python (nada para instalar).
"""

import pathlib
import struct
import zlib

PASTA_DO_PROJETO = pathlib.Path(__file__).resolve().parents[3]
TAMANHO_DO_QUADRO = 64
QUADROS_POR_LINHA = 8
LARGURA_DA_FOLHA = TAMANHO_DO_QUADRO * QUADROS_POR_LINHA
PES_X, PES_Y = 32, 43  # o ponto dos pés dentro do quadro (boneco_provisorio.json)

BAIXO, CIMA, DIREITA, ESQUERDA = range(4)

PELE = (232, 190, 150, 255)
CABELO = (90, 60, 35, 255)
CAMISA = (60, 110, 170, 255)
CALCA = (70, 60, 50, 255)
OLHO = (30, 30, 40, 255)
SOMBRA = (0, 0, 0, 70)


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


def desenhar_quadro(pixels, coluna, linha, direcao, quadro_do_andar):
    """Desenha um boneco no quadro (coluna, linha). quadro_do_andar = None é parado."""
    canto_x = coluna * TAMANHO_DO_QUADRO
    canto_y = linha * TAMANHO_DO_QUADRO

    def pintar(x, y, cor):
        if 0 <= x < TAMANHO_DO_QUADRO and 0 <= y < TAMANHO_DO_QUADRO:
            pixels[(canto_y + y) * LARGURA_DA_FOLHA + canto_x + x] = cor

    def retangulo(x0, y0, x1, y1, cor):
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                pintar(x, y, cor)

    # O balanço do andar: as pernas abrem e fecham, e o corpo sobe 1 px nos quadros 1 e 4.
    abertura_das_pernas = [0, 2, 3, 0, -2, -3][quadro_do_andar] if quadro_do_andar is not None else 0
    subida = 1 if quadro_do_andar in (1, 4) else 0

    # Sombra no chão, embaixo dos pés.
    for y in range(PES_Y - 1, PES_Y + 2):
        for x in range(PES_X - 6, PES_X + 6):
            if ((x - PES_X + 0.5) / 6) ** 2 + ((y - PES_Y) / 1.6) ** 2 <= 1:
                pintar(x, y, SOMBRA)

    # Pernas (do quadril, y 36, até os pés, y 43).
    if direcao in (BAIXO, CIMA):
        # De frente ou de costas, as pernas sobem e descem.
        retangulo(PES_X - 4, 36 - subida, PES_X - 2, PES_Y - max(0, abertura_das_pernas) // 2, CALCA)
        retangulo(PES_X + 1, 36 - subida, PES_X + 3, PES_Y - max(0, -abertura_das_pernas) // 2, CALCA)
    else:
        # De lado, as pernas vão para a frente e para trás.
        retangulo(PES_X - 2 + abertura_das_pernas, 36 - subida, PES_X + abertura_das_pernas, PES_Y, CALCA)
        retangulo(PES_X - 1 - abertura_das_pernas, 36 - subida, PES_X + 1 - abertura_das_pernas, PES_Y, CALCA)

    # Corpo (camisa) e braços.
    retangulo(PES_X - 5, 25 - subida, PES_X + 4, 36 - subida, CAMISA)
    if direcao in (BAIXO, CIMA):
        retangulo(PES_X - 7, 26 - subida, PES_X - 6, 34 - subida, PELE)
        retangulo(PES_X + 5, 26 - subida, PES_X + 6, 34 - subida, PELE)
    else:
        balanco_do_braco = -abertura_das_pernas // 2
        retangulo(PES_X - 1 + balanco_do_braco, 27 - subida, PES_X + 1 + balanco_do_braco, 34 - subida, PELE)

    # Cabeça (y 15 a 24), com o cabelo e os olhos mostrando para onde olha.
    for y in range(15, 25):
        for x in range(PES_X - 5, PES_X + 5):
            if ((x - PES_X + 0.5) / 5) ** 2 + ((y - 19.5) / 5) ** 2 <= 1:
                pintar(x, y - subida, PELE)
    retangulo(PES_X - 5, 15 - subida, PES_X + 4, 17 - subida, CABELO)
    if direcao == CIMA:
        retangulo(PES_X - 5, 15 - subida, PES_X + 4, 22 - subida, CABELO)  # de costas: só o cabelo
    elif direcao == BAIXO:
        pintar(PES_X - 3, 20 - subida, OLHO)
        pintar(PES_X + 2, 20 - subida, OLHO)
    elif direcao == DIREITA:
        retangulo(PES_X - 5, 15 - subida, PES_X - 3, 21 - subida, CABELO)
        pintar(PES_X + 3, 20 - subida, OLHO)
    else:
        retangulo(PES_X + 2, 15 - subida, PES_X + 4, 21 - subida, CABELO)
        pintar(PES_X - 4, 20 - subida, OLHO)


def main():
    pixels = [(0, 0, 0, 0)] * (LARGURA_DA_FOLHA * LARGURA_DA_FOLHA)
    for direcao in range(4):
        desenhar_quadro(pixels, 0, direcao, direcao, None)              # parado
        for quadro in range(6):
            desenhar_quadro(pixels, quadro, 4 + direcao, direcao, quadro)  # andando
    caminho = PASTA_DO_PROJETO / "Jogo" / "data" / "imagens" / "boneco_de_teste.png"
    gravar_png(caminho, LARGURA_DA_FOLHA, LARGURA_DA_FOLHA, pixels)
    print("Gravado:", caminho)


if __name__ == "__main__":
    main()
