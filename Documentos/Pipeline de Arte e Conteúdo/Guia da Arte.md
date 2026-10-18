> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Guia escrito em 2026-10-18, a pedido do autor, que vai **desenhar a arte da fatia à mão, no Pixelorama**, com a paleta **Resurrect 64**. Junta num lugar só o que o [[Pipeline de Arte e Conteúdo/Descrição|Pipeline]], a [[Arquitetura/Descrição|Arquitetura]], o [[Sistema de Clima e Estações/Descrição|Clima e Estações]] e as fichas já decidiram. Os critérios novos estão marcados como [Proposta] e vão para a **rodada FA1** ([[Retomada]], seção 4).

# Guia da Arte

## 1. Como usar este guia

Leia as partes 2 a 5 uma vez: elas valem para **tudo** o que for desenhado. Depois, para cada peça, vá direto à parte dela (6 a 11), que diz o tamanho, as cores, o arquivo e onde a peça entra no jogo. A parte 12 é a lista completa da fatia, na ordem para desenhar.

**O que já está decidido** (e este guia só repete):

| Tema | Decisão | Onde |
|---|---|---|
| Programa | **Pixelorama** (grátis) | Pipeline |
| Paleta | **Resurrect 64**, com algumas cores a mais se precisar | Decisão do autor, 2026-10-18 |
| Tile | **16 × 16 px** | Pipeline |
| Vista | **De cima, em 3/4**, como Stardew Valley | Pipeline |
| Boneco | **16 × 24 px** (1 tile de largura, 1,5 de altura) | Pipeline e Arquitetura |
| Zoom no jogo | 2x, 3x e 4x (padrão 3x): cada pixel seu vira um quadrado de 3 × 3 na tela | FT1-4 |
| Bioma e cultura da fatia | Planície fértil com um grande rio; cultura **Autoritária** (dourado e preto) | Pipeline e [[Mundo e Estado Inicial/Descrição\|Mundo e Estado Inicial]] |
| Estações e noite | **Só tom de cor**, feito pelo jogo. Nada de neve no chão, folhas de outono ou janelas acesas | Pipeline e Clima |
| Animações do boneco | Parado (2 quadros), andar (4) e, depois, trabalhar | Pipeline |
| Construções | Aparência nos níveis 1, 3 e 5; a fatia só precisa do **nível 1** | Pipeline |
| Mapa | Feito no **Tiled**, com as camadas pelo nome; **nenhum tile espelhado ou girado** | [[Ficha Logica/Mapa\|ficha do Mapa]] |
| Letras | Não são desenhadas: o jogo usa a fonte Noto Sans | [[Interface/Descrição\|Interface]] |

## 2. O que é cada coisa (para quem está começando)

- **Pixel art:** cada pixel é colocado à mão. Nada de pincel macio, borrado ou degradê automático.
- **Tile:** um quadrado de 16 × 16 pixels que se repete para formar o chão. Um tile bom **emenda sem marca** com ele mesmo e com os vizinhos.
- **Vista 3/4:** a câmera olha de cima, um pouco inclinada. Do chão e dos objetos baixos, vê-se o **topo**; das construções e das pessoas, vê-se o **topo e a frente** (a parede de baixo, virada para o jogador). Por isso uma casa ocupa o lote no chão, e o telhado "sobe" na tela.
- **Rampa de cor:** a sequência de tons de um material, do mais escuro ao mais claro (por exemplo, 3 ou 4 verdes para a grama). Desenha-se cada material só com a rampa dele.
- **Contorno:** a linha de 1 pixel em volta de um objeto, que o separa do fundo.
- **Folha de sprites:** uma imagem com todos os quadros de uma animação, em grade.
- **Transparente:** o pixel que não tem nada (o chão aparece por baixo).

## 3. O Pixelorama, passo a passo

Os nomes dos menus podem mudar um pouco conforme a versão do Pixelorama.

1. **Paleta:** no painel de paletas, importe o `Jogo/assets/paletas/resurrect-64.gpl` (se ele não aceitar, use o `resurrect-64.png`, uma imagem de 8 × 8 com uma cor por pixel). Desenhe **só com as cores dela**.
2. **Projeto novo:** o tamanho é o da imagem final (cada peça, abaixo, diz qual). Fundo **transparente**.
3. **Grade:** ligue a grade de **16 × 16** (nas preferências, na parte da grade). Para o boneco, uma grade de 32 × 32 mostra os quadros.
4. **Lápis com "Pixel Perfect" ligado:** ele evita os "degraus" duplos nas linhas.
5. **Espelho:** a opção de espelhar (horizontal) desenha os dois lados ao mesmo tempo. Ótima para o boneco de frente e de costas e para construções simétricas.
6. **Modo de tile** (no menu de exibição): repete o desenho em volta, para ver se o tile emenda sem marca.
7. **Camadas:** use camadas para separar as partes (no boneco: corpo, roupa e cabelo; ver a parte 8).
8. **Animação:** cada quadro é uma coluna da linha do tempo. A **pele de cebola** (onion skin) mostra o quadro anterior em transparência, para animar. Para ver o andar, use cerca de 8 quadros por segundo.
9. **Salvar a fonte:** o arquivo do Pixelorama (`.pxo`) vai para `Jogo/assets/` (por exemplo, `Jogo/assets/terreno.pxo`).
10. **Exportar:** em PNG, em escala de **100%** (nunca ampliado). Uma animação sai como **folha de sprites** (a aba de folha de sprites da exportação), com o número de colunas e linhas que a peça pede. O PNG vai para `Jogo/data/imagens/`.

**Nomes dos arquivos:** minúsculas, sem acento e sem espaço, com sublinhado: `terreno.png`, `casa_nivel_1.png`, `boneco_corpo.png`.

## 4. As regras gerais do desenho

### 4.1 Cor

- **Só a Resurrect 64** (a tabela da parte 5), mais as cores a mais anotadas na parte 5.3.
- **De 3 a 4 tons por material** (uma rampa): um de sombra, um ou dois de base e um de luz. Mais tons deixam o desenho "sujo" em 16 pixels.
- **Nada de pixels meio transparentes**, a não ser a sombra no chão (parte 5.3). Um pixel é cheio ou vazio.
- **Cores neutras de dia claro, entre a primavera e o verão.** O jogo pinta por cima o tom de cada estação (outono alaranjado, inverno azulado) e de cada fase do dia (noite escura). Se você desenhar com cor de outono, o outono do jogo fica laranja demais.

### 4.2 Luz, sombra e contorno [Proposta] (FA1-2)

- **A luz vem de cima, à esquerda.** O lado de cima e o da esquerda de cada objeto são os mais claros; o de baixo e o da direita, os mais escuros. Use a mesma luz em **todas** as peças, ou a cena fica estranha.
- **Contorno colorido:** o contorno de cada objeto usa o tom mais escuro da rampa dele (a madeira tem contorno marrom-escuro; a folhagem, verde-escuro). A cor 01 (`#2e222f`) serve de contorno geral, quando nenhuma rampa tem um tom escuro o bastante. Nunca preto puro.
- **Chão sem contorno:** os tiles de chão (grama, terra, água) não têm contorno, para emendar. Contorno é só para o que fica **em cima** do chão.
- **Sem dithering** (o xadrez de dois tons para fingir um degradê): em 16 pixels ele vira ruído.
- **Evite o "sombreado de travesseiro"** (escurecer todas as bordas por igual): a sombra é sempre do lado oposto à luz.

### 4.3 Tamanho e encaixe

- Tudo se encaixa na **grade de 16**. Um objeto pode ocupar vários tiles (uma árvore tem 3 × 4), mas começa e termina na grade.
- **Pés e bases no chão:** objetos e personagens "pisam" na linha de baixo do tile deles.
- **Nada espelhado no Tiled:** se uma borda tem lado esquerdo e direito, os dois são desenhados como tiles separados (pode desenhar um e copiar espelhado **no Pixelorama**, mas o arquivo final tem os dois).

## 5. A paleta Resurrect 64

Criada por Kerrie Lake, publicada no Lospec (lospec.com/palette-list/resurrect-64). Os arquivos estão em `Jogo/assets/paletas/`:

- `resurrect-64.gpl`: para importar no Pixelorama (as cores já vêm numeradas);
- `resurrect-64.png`: a mesma paleta, 1 pixel por cor;
- `resurrect-64-amostra.png`: a amostra grande, para ver as cores.

![[resurrect-64-amostra.png]]

### 5.1 As 64 cores, numeradas

Na ordem da amostra, linha por linha (8 por linha):

| Linha | Cores |
|---|---|
| 1 | 01 `#2e222f` · 02 `#3e3546` · 03 `#625565` · 04 `#966c6c` · 05 `#ab947a` · 06 `#694f62` · 07 `#7f708a` · 08 `#9babb2` |
| 2 | 09 `#c7dcd0` · 10 `#ffffff` · 11 `#6e2727` · 12 `#b33831` · 13 `#ea4f36` · 14 `#f57d4a` · 15 `#ae2334` · 16 `#e83b3b` |
| 3 | 17 `#fb6b1d` · 18 `#f79617` · 19 `#f9c22b` · 20 `#7a3045` · 21 `#9e4539` · 22 `#cd683d` · 23 `#e6904e` · 24 `#fbb954` |
| 4 | 25 `#4c3e24` · 26 `#676633` · 27 `#a2a947` · 28 `#d5e04b` · 29 `#fbff86` · 30 `#165a4c` · 31 `#239063` · 32 `#1ebc73` |
| 5 | 33 `#91db69` · 34 `#cddf6c` · 35 `#313638` · 36 `#374e4a` · 37 `#547e64` · 38 `#92a984` · 39 `#b2ba90` · 40 `#0b5e65` |
| 6 | 41 `#0b8a8f` · 42 `#0eaf9b` · 43 `#30e1b9` · 44 `#8ff8e2` · 45 `#323353` · 46 `#484a77` · 47 `#4d65b4` · 48 `#4d9be6` |
| 7 | 49 `#8fd3ff` · 50 `#45293f` · 51 `#6b3e75` · 52 `#905ea9` · 53 `#a884f3` · 54 `#eaaded` · 55 `#753c54` · 56 `#a24b6f` |
| 8 | 57 `#cf657f` · 58 `#ed8099` · 59 `#831c5d` · 60 `#c32454` · 61 `#f04f78` · 62 `#f68181` · 63 `#fca790` · 64 `#fdcbb0` |

### 5.2 As rampas de cada material [Proposta]

Um ponto de partida, da sombra à luz. Teste no Pixelorama e troque um tom se não ficar bom: o importante é cada material usar sempre a mesma rampa.

| Material | Rampa (números das cores) | Observação |
|---|---|---|
| Grama | 30 → 31 → 33 | O 34 para um brilho raro; 26 e 27 para manchas secas |
| Folhagem das árvores | 36 → 30 → 31 → 32 | Mais escura que a grama, para a copa se destacar do chão |
| Terra batida (caminho) | 25 → 04 → 05 | |
| Terra arada (lavoura) | 25 → 21 → 22 | |
| Trigo maduro | 18 → 19 → 29 | |
| Água | 45 → 46 → 47 → 48, com 49 nos reflexos | Para um rio mais verde, 40 → 41 → 42 |
| Areia e margem | 05 → 24 → 64 | |
| Madeira | 25 → 21 → 22 → 23 | |
| Pedra e muro | 01 → 03 → 07 → 08 → 09 | |
| Telhado (ardósia escura) | 01 → 02 → 03, com 46 num reflexo | O "preto" da cultura Autoritária |
| Dourado (enfeites) | 18 → 19 → 29 | O "dourado" da cultura Autoritária |
| Reboco das paredes | 05 → 39 → 09 | |
| Pele clara | 62 → 63 → 64 | |
| Pele média | 21 → 22 → 23 | |
| Pele escura | 50 → 11 → 21 | |
| Cabelo castanho | 25 → 21 → 22 | |
| Cabelo preto | 01 → 02 → 03 | |
| Cabelo loiro | 18 → 19 → 24 | |
| Linho (roupa simples) | 05 → 39 → 09 | |
| Tecido azul | 45 → 46 → 47 | |
| Tecido verde | 36 → 37 → 38 | |
| Couro (botas, cinto) | 25 → 21 | |
| Pão | 21 → 22 → 23 → 24 | |
| Flores | 16, 61, 19, 10 | Um pixel de cada, sobre a grama |

**As cores das 6 culturas** (Pipeline), na Resurrect 64, para bandeiras, telhados e roupas (só a Autoritária é usada na fatia):

| Cultura | Cores | Números |
|---|---|---|
| Autoritária | Dourado e preto | 19 e 01 |
| Guerreira | Vermelho-sangue e ferro | 15 e 08 |
| Arcana | Roxo e azul-noite | 52 e 45 |
| Religiosa | Branco e azul-celeste | 10 e 49 |
| Tecnocrata | Cobre e verde-oliva | 22 e 26 |
| Escravista | Púrpura e areia | 59 e 24 |

### 5.3 As cores a mais [Proposta] (FA1-3)

- **A sombra no chão** é a única cor transparente: a cor 01 (`#2e222f`) com **50% de transparência**. É a **cor 65**, a primeira a mais. Use-a nas sombras embaixo dos personagens, das árvores e das construções.
- **Outras cores a mais:** no máximo **8** (cores 66 a 73), e só quando uma rampa precisar de um tom que a Resurrect 64 não tem. Cada uma é anotada na tabela abaixo, com o motivo, e acrescentada no fim do `resurrect-64.gpl`.

| Número | Cor | Para quê |
|---|---|---|
| 65 | `#2e222f`, 50% de transparência | A sombra no chão |
| 66 a 73 | (livres) | |

## 6. O chão e o ambiente (o conjunto de tiles)

Tudo o que é pintado no Tiled fica **numa imagem só**: o `Jogo/data/imagens/terreno.png`, com **256 px de largura** (16 tiles por linha), **sem margem e sem espaço** entre os tiles (ficha do Mapa). A altura cresce conforme as linhas.

### 6.1 As camadas do Tiled (onde cada tile é pintado)

| Camada | O que vai nela |
|---|---|
| `chao` | Grama, terra, água, lavoura |
| `detalhes_do_chao` | Flores, caminhos, pedrinhas soltas |
| `objetos_baixos` | Pedras, cercas, troncos de árvore, a base do poço |
| `construcoes_e_paredes` | Muros fixos |
| `acima_do_personagem` | Copas de árvore, o telhado do poço (o personagem passa **por baixo**) |
| `colisao` | Um tile de cor qualquer, onde não se anda (o jogo não desenha esta camada) |

### 6.2 A arrumação do `terreno.png` [Proposta]

| Linha | Tiles |
|---|---|
| 0 | Grama lisa (4 variações) · grama seca (2) · terra batida lisa (2) · água funda (2) · terra arada · brotos · trigo maduro (2) · 2 livres |
| 1 | Grama ↔ terra batida: as 16 bordas (parte 6.3) |
| 2 | Grama ↔ água: as 16 bordas |
| 3 | Detalhes, com fundo transparente: flores (3), grama alta (2), pedrinhas (2), cogumelo (1); objetos: pedra, arbusto, toco, feixe de trigo; 4 livres |
| 4 | Cerca de madeira (6: reta deitada, reta em pé, 4 cantos) e poste solto · muro de pedra (6) · 3 livres |
| 5 a 8 | As árvores (2 tipos, 3 × 4 tiles cada) e o poço (2 × 2) |
| 9 | O tile de colisão (vermelho, só para o Tiled) |

### 6.3 As bordas entre dois chãos [Proposta] (FA1-4)

Onde a grama encontra a terra (ou a água), o tile de borda mostra os dois chãos. Use o **conjunto de cantos de 16 tiles**: cada canto do tile é grama ou terra, e há 16 combinações. No Tiled, esse conjunto vira um **pincel de terreno** (terrain set do tipo "cantos"): pinta-se "terra" e ele escolhe sozinho a borda certa.

As 16 combinações (G = grama, T = terra; a ordem é canto de cima à esquerda, de cima à direita, de baixo à esquerda, de baixo à direita):

| | | | |
|---|---|---|---|
| GGGG (só grama) | GGGT | GGTG | GGTT |
| GTGG | GTGT | GTTG (diagonal) | GTTT |
| TGGG | TGGT (diagonal) | TGTG | TGTT |
| TTGG | TTGT | TTTG | TTTT (só terra) |

- Desenhe a linha da borda **ondulada**, não reta, para não parecer uma grade.
- Os caminhos precisam de **pelo menos 2 tiles de largura** (com 1 tile, os cantos não fecham). As ruas da vila já têm 2.
- A água é igual (G = grama, A = água), com uma faixa fina de margem (rampa da areia) entre as duas. **Sem animação** por enquanto (o jogo ainda não anima tiles).

### 6.4 Os tiles de chão

- **Grama (4 variações):** a mesma base (cor 31), com pequenas diferenças: tufos escuros (30), pontos claros (33). As 4 precisam emendar entre si em qualquer ordem: confira no modo de tile. Evite um desenho que "salte aos olhos" quando repetido (um tufo grande vira um padrão visível).
- **Terra batida:** a rampa da terra, com 2 ou 3 pedrinhas de 1 pixel.
- **Lavoura** (a área `campo_de_grao` da Fazenda): sulcos **na horizontal**, que emendam de um lado ao outro. Terra arada, brotos (fileiras de pontinhos verdes) e trigo maduro (fileiras douradas, um pouco mais altas, com sombra embaixo).

### 6.5 Os objetos (com fundo transparente e contorno)

| Peça | Tamanho | Camada | Detalhes |
|---|---|---|---|
| Flores, grama alta, pedrinhas | 16 × 16, pequenas no meio do tile | `detalhes_do_chao` | Sem colisão |
| Pedra, arbusto, toco | 16 × 16 | `objetos_baixos` | Com colisão; sombra (cor 65) embaixo |
| Cerca de madeira | 16 × 16 cada peça | `objetos_baixos` | As peças precisam se ligar; a reta deitada mostra a frente |
| Muro de pedra baixo | 16 × 16 cada peça | `construcoes_e_paredes` | Pedras com o topo de ardósia |
| Árvore (2 tipos) | 48 × 64 (3 × 4 tiles) | Tronco: `objetos_baixos`; copa: `acima_do_personagem` | O **tronco ocupa só o tile de baixo, no meio** (16 × 16), e é o único com colisão. A **copa ocupa os 3 × 3 tiles de cima** e não cobre o tile do tronco. Assim, quem passa atrás da árvore some embaixo da copa |
| Poço | 32 × 32 (2 × 2 tiles) | Base: `objetos_baixos`; telhado: `acima_do_personagem` | Anel de pedra com água escura, telhadinho de ardósia em dois postes, balde e corda; um detalhe dourado na viga |

## 7. As construções

As construções **não são pintadas no Tiled**: o jogo as desenha nos lotes (hoje, como retângulos de cor; FL5-1). A fatia precisa das **7**, no **nível 1** (simples e modesto).

### 7.1 O tamanho [Proposta] (FA1-5)

A imagem tem a **largura do lote** e é **mais alta** que ele, porque o telhado sobe na vista 3/4:

| Lote | Chão do lote | Imagem |
|---|---|---|
| Pequeno (4 × 4 tiles) | 64 × 64 px | **64 × 96 px** |
| Médio (6 × 6 tiles) | 96 × 96 px | **96 × 128 px** |

- A linha de baixo da imagem fica na linha de baixo do lote; os 32 px a mais sobem para fora do lote, na tela.
- **A porta fica no meio da parede de baixo**, no chão: o NPC entra pelo tile logo abaixo do meio do lote ([[Ficha Logica/IA|ficha da IA]], FL5-1). Como esse tile é o da direita do meio (o lote tem um número par de tiles), a porta precisa **cobrir as colunas 32 a 47** da imagem no lote pequeno, e **48 a 63** no médio. Ela pode ser mais larga, para os lados, e ficar centralizada (uma porta dupla de 32 px, das colunas 16 a 47, por exemplo).
- Vê-se o **telhado** (em cima) e a **parede da frente** (embaixo). As paredes dos lados quase não aparecem.
- Uma **sombra** (cor 65) de 2 a 3 px no chão, do lado direito e embaixo (a luz vem de cima, à esquerda).

### 7.2 O estilo da cultura Autoritária

**Base de pedra** (rampa da pedra), **estrutura de madeira** com **paredes de reboco** claro, **telhado de ardósia escura** (o "preto"), e **pequenos enfeites dourados** (beirais, uma faixa, um brasão, uma bandeirinha): um povo organizado, de construções retas e bem acabadas.

Monte as construções com **peças que se repetem** (o mesmo telhado, a mesma janela, a mesma porta), como manda o Pipeline: a cidade completa e as outras culturas reaproveitam as peças.

### 7.3 As 7 da fatia

| Construção | Lote | Imagem | Arquivo | O que a diferencia |
|---|---|---|---|---|
| Casa | Pequeno | 64 × 96 | `casa_nivel_1.png` | Casa de família: uma janela ao lado da porta, chaminé de pedra |
| Fazenda | Pequeno | 64 × 96 | `fazenda_nivel_1.png` | Celeiro de porta larga, um fardo de feno e um saco de grão encostados, um forcado |
| Posto de Coleta | Pequeno | 64 × 96 | `posto_de_coleta_nivel_1.png` | Galpão aberto na frente, pilha de toras, dois barris de água, um machado num toco |
| Construtor | Pequeno | 64 × 96 | `construtor_nivel_1.png` | Galpão com blocos de pedra e tábuas empilhados, escada encostada, carrinho de mão |
| Cozinheiro | Médio | 96 × 128 | `cozinheiro_nivel_1.png` | Cozinha com um forno de barro grande ao lado, chaminé com fumaça, pães num balcão com toldo |
| Carpintaria | Médio | 96 × 128 | `carpintaria_nivel_1.png` | Oficina de frente aberta, tábuas empilhadas, um cavalete com uma tábua, uma cadeira pronta do lado de fora |
| Mercado | Médio | 96 × 128 | `mercado_nivel_1.png` | Pavilhão coberto com toldos listrados sobre bancas, caixotes de grão, pão e móveis; duas bandeirinhas douradas e pretas |

**No jogo:** o desenho das imagens nos lotes ainda não existe (hoje são retângulos). Ele entra com a ficha da Construção, no marco 6, que também diz o nome do arquivo de cada tipo.

## 8. O boneco (os NPCs)

### 8.1 O quadro e as direções [Proposta] (FA1-1)

- **Quadro de 32 × 32**, com o boneco de **16 × 24** dentro: das colunas 8 a 23 e das linhas 5 a 28. **Os pés tocam a linha 28**, no meio (ponto **(16, 28)**). A sombra (cor 65) fica nas linhas 28 e 29, embaixo dos pés. A folga do quadro serve para os braços balançarem e, depois, para as ferramentas.
- **4 linhas de direção** na folha, nesta ordem: **baixo, cima, direita, esquerda** (a ordem do jogo). A **esquerda é a direita espelhada**: desenhe a direita e, no Pixelorama, copie a linha e espelhe na horizontal (o Pipeline: "3 direções desenhadas e a esquerda espelhada"). O jogo não precisa saber disso.
- **Proporção:** cabeça grande (cerca de 8 a 9 px de altura), corpo curto, pernas de 6 a 7 px. Olhos de 1 pixel. É o estilo "cabeçudo" de Stardew, que lê bem de longe.

### 8.2 A folha de sprites

**128 × 256 px**: 4 colunas × 8 linhas de quadros de 32 × 32.

| Linhas | Animação | Colunas usadas |
|---|---|---|
| 0, 1, 2, 3 | **Parado**, olhando para baixo, cima, direita e esquerda | 0 e 1 (2 quadros: uma respiração leve, o corpo sobe 1 px) |
| 4, 5, 6, 7 | **Andar**, nas mesmas 4 direções | 0 a 3 (4 quadros: contato, passagem, contato, passagem) |

O jogo usa hoje só a coluna 0 do parado; o segundo quadro fica pronto para quando o parado ganhar animação.

**O andar em 4 quadros:**

1. **Contato:** a perna da frente esticada à frente, a de trás atrás; o corpo na altura normal.
2. **Passagem:** as pernas juntas, uma passando pela outra; o corpo **1 px mais alto**.
3. **Contato:** como o 1, com as pernas trocadas.
4. **Passagem:** como o 2, com as pernas trocadas.

Os braços balançam ao contrário das pernas. De frente e de costas, as pernas sobem e descem (não dá para ver o passo de lado).

### 8.3 As camadas

O jogo **junta camadas** ao abrir (FL4-4): cada camada é uma folha separada, **do mesmo tamanho e na mesma grade**, desenhada uma por cima da outra:

| Camada | Arquivo | O que tem |
|---|---|---|
| Corpo | `personagens/boneco_corpo.png` | O corpo e a pele (pode ter versões: `boneco_corpo_pele_media.png`) |
| Roupa | `personagens/boneco_roupa_campones.png` | Túnica de linho, calça, botas de couro, cinto com uma fivela dourada |
| Cabelo | `personagens/boneco_cabelo_curto.png` | Um corte de cabelo |

No Pixelorama, faça um projeto só, com uma camada para cada parte, e exporte cada camada separada. Assim, trocar a roupa ou o cabelo de um NPC é só trocar um arquivo.

**Para ver no jogo:** no `Jogo/data/personagens/boneco_provisorio.json`, troque:

```json
"tamanho_do_quadro_em_pixels": 32,
"pes_no_quadro": [16, 28],
"linha_parado": 0,
"linha_andando": 4,
"quadros_de_andar": 4,
"distancia_por_quadro_de_andar_em_milesimos": 600,
"camadas": [
  "imagens/personagens/boneco_corpo.png",
  "imagens/personagens/boneco_roupa_campones.png",
  "imagens/personagens/boneco_cabelo_curto.png"
]
```

Com 4 quadros de 600 milésimos, o ciclo do andar cobre 2,4 tiles, como hoje. O `jogo --validar` confere o arquivo.

### 8.4 Depois (marco 6 em diante)

A animação **trabalhar**, nas linhas 8 a 19 da mesma folha (4 direções × 3 movimentos, 4 quadros cada): **golpear** (machado, picareta, martelo), **abaixar e colher**, **carregar**. A ferramenta vai numa camada própria.

## 9. Os ícones dos itens

Para o inventário e o painel da cidade (marco 8). **16 × 16** cada, numa folha só, `Jogo/data/imagens/icones_dos_itens.png`, na ordem do `itens.csv`:

| Coluna | Item | Ideia |
|---|---|---|
| 0 | Grão | Um montinho de grãos dourados |
| 1 | Água | Um balde de madeira cheio |
| 2 | Tora | Uma tora cortada, com os anéis aparecendo |
| 3 | Pão | Um pão redondo |
| 4 | Tábua | Duas tábuas empilhadas |
| 5 | Móveis de Madeira | Uma cadeira simples |

Ícones precisam ser **lidos de longe**: forma simples, contorno escuro, pouca coisa dentro. Nada encosta na borda do tile (deixe 1 px de folga).

## 10. A interface (HUD)

O HUD da fatia (data e hora, velocidade, dinheiro, retrato do personagem) e os painéis da cidade e do inventário (marco 8). **Um tema só.** As letras **não** são desenhadas.

| Peça | Tamanho | Detalhes |
|---|---|---|
| Moldura de janela | 48 × 48 | Madeira escura com um filete dourado e cravos de ferro nos cantos. Os cantos de 16 × 16, as bordas e o meio precisam **esticar** (o "9 partes": os 4 cantos ficam iguais, as bordas e o meio se repetem). O meio é um pergaminho liso (cor 64) |
| Botões de velocidade | 24 × 24 cada (4) | Pausa (duas barras), 1x (um triângulo), 2x (dois), 4x (três), no mesmo estilo da moldura |
| Ícones do HUD | 16 × 16 cada (8) | Moeda de ouro, relógio, duas pessoas, rosto feliz, rosto triste, casa, pão, cama |
| Moldura do retrato | 48 × 48 | Com o meio vazio (transparente) |

**Nunca informação só por cor** (Interface): todo ícone tem uma forma que diz o que é.

## 11. Depois da fatia

- **Partículas do clima** (o clima do dia fica para depois): pingo (1 × 4), respingo (3 × 2), floco (3 × 3), névoa (32 × 16, só 2 tons).
- A arte extra das estações (neve no chão, folhas, flores), os interiores, as outras culturas e biomas.

## 12. A lista da fatia, na ordem para desenhar

A ordem segue o Pipeline e os exercícios dele: primeiro o que aparece mais na tela.

| Ordem | Peça | Tamanho | Arquivo | Entra no jogo |
|---|---|---|---|---|
| 1 | Grama (4 variações) | 4 tiles | `terreno.png`, linha 0 | Já: repintando o mapa no Tiled |
| 2 | Terra batida e as 16 bordas com a grama | 18 tiles | `terreno.png`, linhas 0 e 1 | Já |
| 3 | Água e as 16 bordas | 18 tiles | `terreno.png`, linhas 0 e 2 | Já |
| 4 | Detalhes e objetos pequenos | 12 tiles | `terreno.png`, linha 3 | Já |
| 5 | Árvores (2) e poço | 3 × 4 tiles cada; 2 × 2 | `terreno.png`, linhas 5 a 8 | Já |
| 6 | Lavoura | 4 tiles | `terreno.png`, linha 0 | Já |
| 7 | Cerca e muro | 13 tiles | `terreno.png`, linha 4 | Já |
| 8 | **Boneco** parado e andando (corpo, roupa, cabelo) | Folha de 128 × 256 por camada | `personagens/boneco_*.png` | Já: pelo `boneco_provisorio.json` |
| 9 | Casa | 64 × 96 | `casa_nivel_1.png` | Marco 6 |
| 10 | As outras 6 construções | 64 × 96 e 96 × 128 | `*_nivel_1.png` | Marco 6 |
| 11 | Ícones dos 6 itens | 6 tiles | `icones_dos_itens.png` | Marco 8 |
| 12 | A interface | parte 10 | `interface.png` | Marco 8 |

**Para pôr o chão no jogo:** abrir o `Jogo/data/mapas/vila.tmj` no Tiled, acrescentar o `terreno.png` como conjunto de tiles **embutido** (16 × 16, sem margem, sem espaço), montar os pincéis de terreno (parte 6.3) e repintar as camadas, com os mesmos nomes. As camadas de objetos (`lotes`, `recursos`) continuam como estão. Depois, rodar o `jogo --validar`. **Atenção:** o script `Jogo/assets/teste/gerar_arte_e_mapa_de_teste.py` sobrescreve o `vila.tmj`; não rode mais ele depois de repintar o mapa.

## 13. Para aprender

- **Lospec** (lospec.com): a paleta e tutoriais por assunto (tiles, sombra, animação de andar).
- **Pedro Medeiros (Saint11)**: tutoriais curtos e animados de pixel art, um assunto por vez.
- **Os exercícios do Pipeline**, na ordem: um tile de grama que emenda sem marca; um boneco de 16 × 24 parado; o andar de 4 quadros; uma Casa simples.
- **Dica:** olhe o desenho no zoom do jogo (300%) de vez em quando. Um detalhe que parece bom a 1.000% pode sumir ou virar ruído a 300%.
