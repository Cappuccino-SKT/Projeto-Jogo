# Gera um manequim no estilo RimWorld: 3 direções (frente, costas, lado) x 4 tons de pele.
# Desenha 4x maior e reduz no fim, para a borda ficar suave.
import sys
from PIL import Image, ImageDraw

ESCALA = 4
CELULA = 128
CONTORNO = (30, 24, 22, 255)
CUECA = (140, 136, 134, 255)
CUECA_SOMBRA = (112, 108, 106, 255)
TONS = [(244, 196, 164), (206, 150, 112), (140, 92, 62), (96, 60, 42)]


def s(valores):
    return [v * ESCALA for v in valores]


def escurecer(cor, fator):
    return tuple(int(c * fator) for c in cor[:3]) + (255,)


def formas(direcao):
    # Cada forma: (tipo, caixa, papel). Coordenadas numa célula de 128.
    if direcao == "lado":
        return [
            ("elipse", (52, 104, 80, 124), "pele"),        # pé
            ("redondo", (42, 60, 86, 114), "pele"),       # tronco
            ("redondo", (42, 96, 86, 114), "cueca"),
            ("elipse", (54, 64, 74, 110), "braco"),       # o braço de lado, o oval da referência
            ("elipse", (36, 8, 92, 64), "cabeca"),
        ]
    return [
        ("elipse", (44, 104, 62, 124), "pele"),            # pés
        ("elipse", (66, 104, 84, 124), "pele"),
        ("redondo", (34, 58, 94, 114), "pele"),           # tronco
        ("redondo", (34, 96, 94, 114), "cueca"),
        ("elipse", (22, 62, 44, 108), "braco"),           # braços, com contorno próprio
        ("elipse", (84, 62, 106, 108), "braco"),
        ("elipse", (34, 6, 94, 66), "cabeca"),
    ]


def desenhar(direcao, tom):
    pele = tom + (255,)
    sombra = escurecer(tom, 0.86)
    imagem = Image.new("RGBA", (CELULA * ESCALA, CELULA * ESCALA), (0, 0, 0, 0))
    lapis = ImageDraw.Draw(imagem)
    grossura = 3.2 * ESCALA
    lista = formas(direcao)
    # 1. O contorno: todas as formas um pouco maiores, na cor do contorno (as formas se fundem numa silhueta só).
    for tipo, caixa, _ in lista:
        x0, y0, x1, y1 = s(caixa)
        caixa_grande = (x0 - grossura, y0 - grossura, x1 + grossura, y1 + grossura)
        if tipo == "elipse":
            lapis.ellipse(caixa_grande, fill=CONTORNO)
        else:
            lapis.rounded_rectangle(caixa_grande, radius=16 * ESCALA + grossura, fill=CONTORNO)
    # 2. O preenchimento, na ordem (o que vem depois fica por cima).
    for tipo, caixa, papel in lista:
        x0, y0, x1, y1 = s(caixa)
        cor = CUECA if papel == "cueca" else pele
        if papel == "braco":
            fina = grossura * 0.7
            lapis.ellipse((x0 - fina, y0 - fina, x1 + fina, y1 + fina), fill=CONTORNO)
        if tipo == "elipse":
            lapis.ellipse((x0, y0, x1, y1), fill=cor)
        else:
            raio = 16 * ESCALA
            if papel == "cueca":
                # A cueca: só a parte de baixo do tronco, com os cantos de baixo redondos.
                lapis.rounded_rectangle((x0, y0 - raio, x1, y1), radius=raio, fill=cor)
                lapis.rectangle((x0, y0 - raio, x1, y0), fill=pele)
                lapis.line((x0, y0, x1, y0), fill=CUECA_SOMBRA, width=2 * ESCALA)
            else:
                lapis.rounded_rectangle((x0, y0, x1, y1), radius=raio, fill=cor)
    # 3. Sombra suave na cabeça (de baixo à direita) e os detalhes finos.
    cx0, cy0, cx1, cy1 = s(next(c for t, c, p in lista if p == "cabeca"))
    lapis.chord((cx0 + 6 * ESCALA, cy0 + 10 * ESCALA, cx1, cy1), 20, 110, fill=sombra)
    lapis.ellipse((cx0 - grossura * 0.5, cy0 - grossura * 0.5, cx1 + grossura * 0.5, cy1 + grossura * 0.5), outline=CONTORNO, width=int(grossura * 0.6))
    if direcao == "frente":
        lapis.ellipse(s((62, 88, 65, 91)), fill=escurecer(tom, 0.6))  # umbigo
    if direcao == "costas":
        lapis.arc(s((44, 64, 60, 84)), 200, 300, fill=escurecer(tom, 0.7), width=ESCALA)  # escápulas
        lapis.arc(s((68, 64, 84, 84)), 240, 340, fill=escurecer(tom, 0.7), width=ESCALA)
    return imagem.resize((CELULA, CELULA), Image.LANCZOS)


def main(saida):
    direcoes = ["frente", "costas", "lado"]
    folha = Image.new("RGBA", (CELULA * len(direcoes), CELULA * len(TONS)), (0, 0, 0, 0))
    for linha, tom in enumerate(TONS):
        for coluna, direcao in enumerate(direcoes):
            folha.alpha_composite(desenhar(direcao, tom), (coluna * CELULA, linha * CELULA))
    folha.save(saida)
    fundo = Image.new("RGBA", folha.size, (255, 255, 255, 255))
    fundo.alpha_composite(folha)
    fundo.convert("RGB").save(saida.replace(".png", "_fundo_branco.png"))


if __name__ == "__main__":
    main(sys.argv[1])
