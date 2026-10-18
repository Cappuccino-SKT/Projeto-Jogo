> Guia de apoio, escrito em 2026-10-18 a pedido do autor. Não decide nada novo: junta, em prompts prontos para o **Gemini**, o que o [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]], a [[Arquitetura/Descrição|Arquitetura]], o [[Sistema de Clima e Estações/Descrição|Clima e Estações]] e as fichas já decidiram. Onde os documentos não decidem, o guia diz que é uma **recomendação**.

# Prompts da arte provisória (Gemini)

## Para que serve

A fatia vertical usa **arte provisória** (Pipeline: "pacotes de terceiros e rascunhos"). Estes prompts geram, com o Gemini, tudo o que a fatia precisa de arte agora: o chão, o ambiente, as 7 construções, o boneco, os ícones dos itens e a interface básica.

**Regras que valem para toda arte gerada:**

- **É provisória**, como os pacotes de terceiros: o jogo final tem arte feita à mão. Anote a origem de cada imagem no `Jogo/assets/provisorio/LEIA-ME.txt` (por exemplo, "terreno.png: gerado pelo Gemini em 2026-10-18, prompt 1"), como manda o Pipeline.
- **A IA não acerta a grade de pixels sozinha.** Ela desenha "parecido com pixel art", mas com pixels tortos, cores demais e sem fundo transparente. Por isso cada prompt pede a arte **ampliada 8 vezes** (cada pixel vira um quadrado de 8 × 8) sobre um **fundo magenta liso**, e a limpeza é feita no Pixelorama (veja "Como limpar", abaixo).
- **Um pedido por imagem.** O Gemini erra mais quando se pede tudo de uma vez. Os prompts já vêm separados; gere cada um algumas vezes e fique com o melhor.
- **Os prompts estão em inglês**, porque os geradores de imagem seguem melhor as palavras técnicas ("pixel art", "top-down 3/4") em inglês. A explicação de cada um está em português.

## O que os documentos já decidiram (e os prompts repetem)

| Tema | Decisão | Onde |
|---|---|---|
| Tile | 16 × 16 px | Pipeline, "Especificação da arte" |
| Vista | De cima, em 3/4, como Stardew Valley | Pipeline |
| Boneco | 16 × 24 px (1 tile de largura, 1,5 de altura) | Pipeline e Arquitetura |
| Direções | Cima, baixo, direita e esquerda (o boneco provisório usa as 4 desenhadas; a arte final desenha 3 e espelha a esquerda) | Pipeline; [[Ficha Logica/NPC\|ficha do NPC]] (FL4-4) |
| Animações da fatia | Parado (2 quadros), andar (4 quadros) e trabalhar | Pipeline, "Animação" e "Ordem de produção" |
| Paleta | Paleta mestre de cerca de 48 cores, partindo do **Endesga 32** ou do **Resurrect 64** (do Lospec) | Pipeline |
| Cultura da fatia | **Autoritária**: cores de destaque **dourado e preto** | Pipeline, "Cores das culturas" |
| Bioma da fatia | **Planície fértil cortada por um grande rio** (Grão, Pedra, Vacas) | [[Mundo e Estado Inicial/Descrição\|Mundo e Estado Inicial]] |
| Estações e noite | **Só tom de cor**, feito pelo jogo: nada de neve no chão, folhas de outono, flores da estação ou janelas acesas | Pipeline e Clima |
| Construções | Aparência muda nos níveis 1, 3 e 5; a fatia só precisa do **nível 1**. Lote pequeno = 4 × 4 tiles; médio = 6 × 6 | Pipeline; [[Ficha Logica/Mapa\|ficha do Mapa]] |
| Porta | O NPC entra pelo tile logo abaixo do meio do lote: a porta fica **no meio da parede de baixo** | [[Ficha Logica/IA\|ficha da IA]] (FL5-1) |
| Tiles no Tiled | Nenhum tile espelhado ou girado: **cada lado de uma borda é desenhado** | Ficha do Mapa (FL1-5) |
| Interface | Um tema só; letras com a fonte Noto Sans (não desenhadas); molduras, botões e ícones feitos à mão | [[Interface/Descrição\|Interface]] |

**Recomendações deste guia** (não estão nos documentos; o autor pode mudar):

- **Paleta Endesga 32** (a mais simples das duas do Pipeline), carregada no Pixelorama para "prender" as cores.
- **Luz vinda de cima à esquerda** e contorno escuro colorido (marrom ou verde-escuro), nunca preto puro: é o estilo de Stardew, a referência da vista.
- **Boneco gerado num quadro de 32 × 32**, com os pés no ponto (16, 29). O jogo aceita qualquer tamanho de quadro (o `boneco_provisorio.json`), e 32 dá folga para os braços e a sombra.

## O bloco de estilo (colar no começo de todo prompt)

```
Pixel art game asset for a top-down 3/4 perspective village-management RPG, camera angle like Stardew Valley.
Strict pixel art: hard square pixels, no anti-aliasing, no blur, no gradients, no noise, no text, no watermark, no signature.
Base grid: 16x16 pixel tiles. Characters are 16 pixels wide and 24 pixels tall.
Limited palette in the style of ENDESGA 32: warm, natural, medium-saturation colors, about 32 colors in total.
Light comes from the top-left. Shade each material with 2 or 3 tones. Use dark colored outlines (deep brown or dark green), never pure black.
Setting: low-fantasy medieval farming village on a fertile river plain. The culture is orderly and authoritarian: neat stone foundations, sturdy timber frames, dark slate roofs, small gold and black accents (trims, banners, belt buckles).
Neutral late-spring daylight colors. No snow, no autumn leaves, no lit windows, no night lighting, no long cast shadows: seasons and night are added later by the game engine with a color tint.
Draw every game pixel as an exact 8x8 block of screen pixels (8x upscale), perfectly aligned to the grid.
Background: one solid flat magenta color #FF00FF, with no shading, wherever the asset is transparent.
```

## Como limpar cada imagem no Pixelorama

1. Abrir a imagem e conferir a grade: cada pixel do jogo deve ser um quadrado de 8 × 8.
2. **Imagem → Redimensionar** para 12,5% (1/8), com o filtro **"Mais próximo" (Nearest)**. Uma imagem de 1024 × 256 vira 128 × 32. A folha do boneco é pedida a 4x (para caber no tamanho máximo do Gemini): redimensionar para 25%. Se o Gemini devolver outro tamanho, o que importa é a grade: redimensione até cada pixel do jogo virar 1 pixel.
3. Carregar a paleta **Endesga 32** (baixada do Lospec) e trocar as cores parecidas pelas da paleta. Apagar os pixels soltos que a IA deixou.
4. Com a **varinha mágica**, selecionar o magenta e apagar: vira transparente.
5. Conferir o tamanho final (a tabela de cada prompt) e exportar em **PNG**.
6. **Tiles:** juntar todos num PNG só, de 256 px de largura (16 colunas), sem margem e sem espaço entre os tiles, como pede a ficha do Mapa.

---

## 1. Grama (o chão)

**Para que serve:** a camada `chao`, a mais usada do mapa. **Tamanho final:** 4 tiles de 16 × 16.

```
[style block]
A tileset sheet of 4 seamless grass ground tiles in one row, each tile 16x16 game pixels (128x128 on screen at 8x), total image 512x128.
Tile 1: plain short grass. Tile 2: grass with a few darker blades. Tile 3: grass with tiny light-green highlights. Tile 4: slightly darker, denser grass.
All four tiles must tile seamlessly with each other in any combination, with no visible border or frame. Same base green in all tiles.
```

## 2. Caminho de terra

**Para que serve:** as ruas da vila, na camada `detalhes_do_chao`. **Tamanho final:** 13 tiles (3 × 3 de bordas, 4 cantos internos) mais 2 de terra lisa. Cada lado é desenhado, porque o Tiled do projeto não espelha tiles.

```
[style block]
A tileset sheet for a packed-dirt village path that sits on top of grass, every tile 16x16 game pixels, drawn on a grid with 1 empty magenta tile between tiles.
Row 1: a 3x3 block of tiles forming a dirt patch with grass borders: top-left corner, top edge, top-right corner; left edge, full dirt center, right edge; bottom-left corner, bottom edge, bottom-right corner. Grass outside the dirt is magenta (transparent).
Row 2: 4 inner-corner tiles (dirt with a small grass corner in the top-left, top-right, bottom-left and bottom-right), then 2 extra full-dirt center tiles with small pebbles.
Edges must connect seamlessly so the path can be painted in any shape.
```

## 3. Água do lago e margem

**Para que serve:** o lago e o rio (camada `chao`; os tiles de água também vão na `colisao`). **Tamanho final:** 13 tiles de borda mais 2 de água funda. Água parada, sem animação (o jogo ainda não anima tiles).

```
[style block]
A tileset sheet for a calm freshwater lake on grass, every tile 16x16 game pixels, with 1 empty magenta tile between tiles.
Row 1: a 3x3 block forming a lake: top-left corner, top edge, top-right corner; left edge, deep water center, right edge; bottom-left, bottom edge, bottom-right. The shore is a thin band of light sand and small stones between the grass and the water; the grass outside is magenta.
Row 2: 4 inner-corner tiles (water with a small shore corner in each corner), then 2 deep-water center tiles with subtle lighter ripple lines.
Blue-teal water, slightly darker near the center. No animation, no reflections of objects.
```

## 4. Campo de Grão (lavoura)

**Para que serve:** a área `campo_de_grao` da Fazenda (camada `objetos_baixos`). **Tamanho final:** 6 tiles.

```
[style block]
A tileset sheet of 6 farm tiles in one row, each 16x16 game pixels, with 1 empty magenta tile between them:
1) plowed brown soil with straight furrows, 2) soil with young green sprouts in rows, 3) mature golden wheat in rows, full tile,
4) mature wheat, top edge of the field (wheat with a little soil above), 5) mature wheat, bottom edge, 6) a single wheat sheaf bundle on magenta background.
Furrows run horizontally and must tile seamlessly left to right.
```

## 5. Detalhes do chão: flores, pedras e arbustos

**Para que serve:** enfeites nas camadas `detalhes_do_chao` (flores) e `objetos_baixos` (pedras e arbustos, que também entram na `colisao`). **Tamanho final:** 8 tiles de 16 × 16, com fundo transparente.

```
[style block]
A sheet of 8 small decoration objects, each centered in its own 16x16 game-pixel tile, on a magenta background, one row with 1 empty tile between them:
1) a few tiny yellow wildflowers, 2) tiny pink and white wildflowers, 3) a clump of tall grass, 4) a small round grey stone,
5) two small stones, 6) a small green bush, 7) a flowering bush with tiny red dots, 8) a tree stump.
Each object sits on the ground with a 1-pixel soft dark shadow under it. Objects must not touch the tile borders.
```

## 6. Árvores do bosque

**Para que serve:** o bosque (a área de Madeira). A árvore é dividida em duas partes, porque o jogo desenha o **tronco** embaixo do personagem (camada `objetos_baixos`, com colisão) e a **copa** por cima dele (camada `acima_do_personagem`): quem passa atrás da árvore some embaixo da copa. **Tamanho final:** cada árvore com 3 × 4 tiles (48 × 64 px).

```
[style block]
Two deciduous trees for a forest, drawn side by side on a magenta background, each tree 48 game pixels wide and 64 game pixels tall (3x4 tiles of 16x16), aligned to the 16-pixel grid.
Tree 1: round leafy oak-like crown. Tree 2: taller crown, slightly darker green.
The trunk occupies only the bottom-center 16x16 tile (with roots and a small shadow); the leafy crown fills the upper 3x3 tiles and must not overlap the trunk tile.
Leave 1 empty magenta tile between the two trees.
```

## 7. Cerca e muro de pedra

**Para que serve:** cercas (camada `objetos_baixos`) e muros fixos (camada `construcoes_e_paredes`), ambos com colisão. **Tamanho final:** 6 tiles de cerca e 6 de muro.

```
[style block]
A tileset sheet with two rows of wall pieces, every piece 16x16 game pixels, magenta background, 1 empty tile between pieces.
Row 1, wooden fence: horizontal segment, vertical segment, top-left corner, top-right corner, bottom-left corner, bottom-right corner.
Row 2, low dry-stone wall of grey stones with a dark slate top: the same 6 pieces (horizontal, vertical, four corners).
Pieces must connect seamlessly to each other. Seen from the top-down 3/4 angle: horizontal pieces show their front face.
```

## 8. Poço

**Para que serve:** a área `poco` (o recurso Água), um objeto com colisão. **Tamanho final:** 32 × 48 px (2 tiles de largura, 3 de altura).

```
[style block]
A village stone well, 32 game pixels wide and 48 game pixels tall (2x3 tiles of 16x16), on a magenta background.
Round grey stone ring with dark water inside, a small wooden roof with dark slate shingles held by two posts, a rope and a wooden bucket.
A tiny gold-painted detail on the roof beam. 1-pixel shadow under the stone ring.
```

## 9. As construções da fatia

**Para que servem:** as 7 construções da vila, desenhadas pelo jogo em cima dos lotes (não são pintadas no Tiled). Só o **nível 1**. **A porta fica no meio da parede de baixo**, porque o NPC entra pelo tile logo abaixo do lote. O telhado pode subir acima do lote (é a vista 3/4), então a imagem é mais alta que o lote.

| Construção | Lote | Tamanho final da imagem |
|---|---|---|
| Casa, Fazenda, Posto de Coleta, Construtor | Pequeno (4 × 4 tiles = 64 × 64 px) | 64 × 96 px |
| Cozinheiro, Carpintaria, Mercado | Médio (6 × 6 tiles = 96 × 96 px) | 96 × 128 px |

**O modelo** (trocar o que está entre colchetes por uma linha da tabela seguinte):

```
[style block]
A single building for a medieval village, level 1 (simple and modest), seen from the top-down 3/4 angle with the front wall facing the viewer.
Footprint: [FOOTPRINT] game pixels. Total image: [IMAGE SIZE] game pixels, with the roof rising above the footprint. Magenta background around the building.
The only door is centered on the bottom (front) wall, at ground level.
Common style for all buildings of this culture: stone foundation, timber frame with plastered walls, dark slate roof, small gold and black trims.
Building: [DESCRIPTION]
```

| Construção | [FOOTPRINT] / [IMAGE SIZE] | [DESCRIPTION] |
|---|---|---|
| Casa | 64x64 / 64x96 | `a small family house for 4 villagers, one small window beside the door, a stone chimney on the roof.` |
| Fazenda | 64x64 / 64x96 | `a small farm barn with a wide wooden door, a hay bundle and a sack of grain beside the wall, a pitchfork leaning on it.` |
| Posto de Coleta | 64x64 / 64x96 | `a gatherer's post: an open-front shed with a neat pile of logs, two water barrels and a wooden bucket, an axe stuck in a stump.` |
| Construtor | 64x64 / 64x96 | `a builder's yard shed with stacked stone blocks and planks beside it, a ladder against the wall and a wooden wheelbarrow.` |
| Cozinheiro | 96x96 / 96x128 | `a village kitchen and bakery with a big clay bread oven attached to the side, a smoking chimney, loaves of bread on a counter under a small awning.` |
| Carpintaria | 96x96 / 96x128 | `a carpenter's workshop with a wide open front, stacked planks, a sawhorse with a plank on it, and a finished wooden chair outside.` |
| Mercado | 96x96 / 96x128 | `a covered market hall with striped cloth awnings over open stalls, crates of grain, bread and wooden goods, and two small gold-and-black banners on the front posts.` |

## 10. O boneco (os NPCs Civis)

**Para que serve:** os NPCs da vila. O jogo lê o formato da folha do `data/personagens/boneco_provisorio.json` (FL4-4), então a folha nova só precisa seguir este formato. **Tamanho final:** uma folha de 128 × 256 px: 4 colunas × 8 linhas de quadros de 32 × 32, com o boneco de 16 × 24 dentro de cada quadro, os pés no ponto (16, 29).

| Linhas | Animação | Colunas usadas |
|---|---|---|
| 0, 1, 2, 3 | Parado, olhando para baixo, cima, direita e esquerda | 0 e 1 (2 quadros, Pipeline) |
| 4, 5, 6, 7 | Andando, nas mesmas 4 direções | 0 a 3 (4 quadros, Pipeline) |

Gerar **uma folha por aldeão** (a IA não acerta camadas separadas de corpo, roupa e cabelo). Sugestão de 4 aldeões: um camponês, uma camponesa, uma lojista de avental e um trabalhador braçal.

```
[style block]
A character sprite sheet for one villager, a [VILLAGER] in simple medieval clothes: linen tunic, trousers or long skirt, leather boots, a small gold-and-black detail on the belt.
The sheet is a grid of 4 columns and 8 rows of 32x32 game-pixel frames (total 128x256 game pixels). For this sheet, draw every game pixel as an exact 4x4 block (512x1024 on screen) instead of 8x8. Each frame has a magenta background.
In every frame the character is exactly 16 pixels wide and 24 pixels tall, horizontally centered, feet touching row 29 of the frame, with a small oval shadow under the feet.
Rows 0 to 3: standing idle facing down (toward the viewer), up (back view), right, and left; 2 frames each in columns 0 and 1 (gentle breathing); columns 2 and 3 empty.
Rows 4 to 7: walking cycle facing down, up, right, and left; 4 frames each in columns 0 to 3 (contact, passing, contact, passing), legs and arms alternating.
Same character, same colors and same proportions in every frame.
```

Trocar `[VILLAGER]` por: `young male peasant with short brown hair`, `female peasant with braided hair and a headscarf`, `shopkeeper woman with an apron`, ou `strong laborer man with rolled-up sleeves`.

**Para usar no jogo:** salvar em `Jogo/data/imagens/personagens/` (por exemplo, `aldeao_1.png`) e mudar o `boneco_provisorio.json` para `"tamanho_do_quadro_em_pixels": 32`, `"pes_no_quadro": [16, 29]`, `"quadros_de_andar": 4`, `"distancia_por_quadro_de_andar_em_milesimos": 600` (4 × 600 = 2.400, o mesmo passo do ciclo de hoje) e `"camadas": ["imagens/personagens/aldeao_1.png"]`.

**Depois (marco 6):** a animação **trabalhar** (golpear com ferramenta, abaixar e colher, carregar; 4 quadros cada, Pipeline) pode ser pedida no mesmo formato, em linhas novas.

## 11. Ícones dos itens

**Para que serve:** o inventário e o painel da cidade (marco 8). **Tamanho final:** 6 ícones de 16 × 16.

```
[style block]
A sheet of 6 item icons in one row, each centered in its own 16x16 game-pixel tile, magenta background, 1 empty tile between icons, readable at small size with a clear dark outline:
1) a small pile of golden wheat grain, 2) a wooden bucket full of water, 3) a cut wooden log, 4) a round loaf of bread,
5) two stacked wooden planks, 6) a simple wooden chair (furniture).
```

## 12. Interface básica (HUD)

**Para que serve:** o HUD da fatia (data e hora, velocidade, dinheiro, retrato do personagem) e os painéis da cidade e do inventário (marco 8). As **letras não são desenhadas**: o jogo usa a fonte Noto Sans. Um tema só. **Tamanho final:** uma moldura de 48 × 48 (cantos de 16 × 16, para esticar), 4 botões de 24 × 24 e 8 ícones de 16 × 16.

```
[style block]
A user-interface kit for the game, on a magenta background, with 1 empty tile between pieces:
1) a window frame panel 48x48 game pixels made of dark stained wood with a thin gold trim and dark iron corner studs; its 16x16 corners, edges and center must work as a 9-slice that can be stretched; the center is a flat parchment color.
2) four square buttons 24x24 game pixels in the same wood-and-gold style, each with a simple white symbol: pause (two vertical bars), play (one triangle), fast (two triangles), fastest (three triangles).
3) eight icons 16x16 game pixels: a gold coin, a small clock, a group of two people, a smiling face, a sad face, a house, a loaf of bread, a bed.
4) a portrait frame 48x48 game pixels with an empty magenta center.
No letters or numbers anywhere.
```

## 13. Depois: partículas do clima

O Clima e Estações prevê pingos, flocos e névoa por cima do mapa, mas o **clima do dia fica para depois da fatia** (Clima, "Fatia vertical"). Quando entrar:

```
[style block]
Tiny weather particle sprites on a magenta background, 1 empty tile between them: a raindrop streak 1x4 game pixels in light blue, a splash 3x2, a snowflake 3x3 in white, and a soft fog puff 32x16 in light grey (fog may use only 2 flat grey tones).
```

## Depois de gerar

1. Limpar cada imagem (a seção "Como limpar") e anotar a origem no `Jogo/assets/provisorio/LEIA-ME.txt`.
2. **O chão e o ambiente** (prompts 1 a 8): juntar os tiles num conjunto só e, no Tiled, abrir o `Jogo/data/mapas/vila.tmj`, embutir o conjunto novo e repintar as camadas, com os mesmos nomes (`chao`, `detalhes_do_chao`...). O `jogo --validar` confere o mapa depois. É a tarefa da rodada FL1-6 ("o autor escolhe um pacote e redesenha o mapa").
3. **As construções** (prompt 9) esperam o código que desenha imagens nos lotes; até lá, o jogo usa os retângulos de cor (FL5-1).
4. **O boneco** (prompt 10) já pode entrar, pelo `boneco_provisorio.json`.
5. **Os ícones e a interface** (prompts 11 e 12) entram no marco 8.
