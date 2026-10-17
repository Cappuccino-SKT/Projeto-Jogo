> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Pipeline de Arte e Conteúdo

**Descrição geral**

Este documento define **como a arte e os dados do jogo são produzidos**: quem desenha, com quais programas, em que formato, em que ordem, e como saber se a arte está pesando no jogo.

O projeto **não tem prazo**. O limite da arte não é o tempo de produção, e sim o **peso no jogo** (memória e desempenho), que deve ser medido (ver "Peso da arte").

### Quem desenha

- O **artista definitivo ainda não foi escolhido**. Por enquanto, quem desenha é o autor, que **não tem experiência com pixel art**.
- **Fatia vertical e testes:** usar **pacotes de arte de terceiros** para agilizar, e rascunhos feitos pelo autor onde faltar alguma peça.
- **Jogo final:** toda a arte é feita à mão pelo artista do projeto; os pacotes de terceiros saem.
- **[Proposta]** Preferir pacotes com licença **CC0** (domínio público, uso livre) ou que permitam uso em protótipo, e anotar a origem de cada um num arquivo `assets/provisorio/LEIA-ME.txt`, para saber o que precisa ser trocado antes de qualquer versão pública. Procurar pacotes de **16 px em vista 3/4** (como Stardew), para os testes já usarem o tamanho final.

### Programas

| Uso | Programa | Custo |
|---|---|---|
| Montar os mapas | **Tiled** | Grátis |
| Desenhar tiles, bonecos e construções | **[Proposta]** **Pixelorama** | Grátis |
| Editar os dados | **[Proposta]** **VS Code** | Grátis |

**Por que o Pixelorama [Proposta].** O autor pediu um programa **grátis**, **fácil** e com **bastante suporte**, para quem nunca fez pixel art. O Aseprite foi descartado por ser pago.

- É de código aberto e está em desenvolvimento ativo, com manual on-line e comunidade.
- Tem o que o projeto precisa: camadas, linha do tempo de animação, pele de cebola (mostra o quadro anterior em transparência, para animar), paletas, espelhamento (desenha os dois lados ao mesmo tempo) e exportação em **folha de sprites** (uma imagem com todos os quadros).
- **Alternativa:** o **LibreSprite**, também grátis. Ele é uma versão antiga do Aseprite, então os muitos tutoriais de Aseprite servem para ele, mas não recebe novidades. O Aseprite só pode ser usado de graça compilando o código-fonte, o que é difícil para quem está começando; não vale agora.

**[Proposta] Primeiros passos para aprender:**

1. **Lospec** (lospec.com): paletas prontas (inclusive a paleta mestre, ver abaixo) e uma lista de tutoriais por assunto.
2. Tutoriais curtos de **Pedro Medeiros (Saint11)**, artista brasileiro de pixel art (Celeste, TowerFall), com dicas animadas de um assunto por vez.
3. Exercícios na ordem em que a fatia vertical precisa: um tile de grama de 16 × 16 que se repete sem emenda; um boneco de 16 × 24 parado; a animação de andar (4 quadros); uma Casa simples.

### Mapas no Tiled

O Tiled funciona como um "Paint de azulejos": escolhe-se um tile e pinta-se no mapa. Cada Mapa Local segue o mesmo padrão, porque o jogo lê as camadas pelo nome.

**Camadas de tiles:**

1. **Chão:** grama, terra, água.
2. **Detalhes do chão:** flores, caminhos.
3. **Objetos baixos:** pedras, cercas, lavouras.
4. **Construções e paredes.**
5. **Acima do personagem:** copas de árvore, telhados.
6. **Colisão:** invisível no jogo; marca onde não se anda.
7. **Elevação** (opcional): pintada só nas colinas; usada no Mapa de Guerra (ver [[Sistema de Batalha/Descrição|Sistema de Batalha]]). Um mapa sem ela é plano.

**Camadas de objetos:** Lotes (pequeno, médio e grande, com o tipo permitido); Portas (ligam ao Mapa Interno); Bordas (saída para o mapa vizinho); Recursos (áreas de floresta, mina, pesca); Criaturas e bandidos (pontos de aparição); Pontos de interesse (quadro de contratos, entrada de masmorra, praça central para a batalha de cerco).

**Ajudas do editor:** **terrenos** (pinta-se "grama" e o Tiled escolhe sozinho o tile de borda certo) e **carimbos** (pedaços prontos, como "bosque pequeno", colados várias vezes). São só ajudas de desenho: o mapa salvo é fixo e feito à mão, não é gerado pelo jogo.

### Especificação da arte

| Item | Decisão |
|---|---|
| Tile | 16 × 16 px |
| Vista | De cima, em 3/4 (como Stardew) |
| Boneco | 16 × 24 px (1 tile de largura, 1,5 de altura) |
| Zoom padrão | 2x: em 1600 × 900 aparecem 50 × 28 tiles (3x: 33 × 19; 4x: 25 × 14) |
| Paleta | Paleta mestre de cerca de 48 cores + 2 cores de destaque por cultura |

**Cores das culturas** (bandeiras, roupas, telhados e a lente Política do Mapa Mundial): Guerreira (vermelho-sangue e ferro), Arcana (roxo e azul-noite), Autoritária (dourado e preto), Religiosa (branco e azul-celeste), Tecnocrata (cobre e verde-oliva), Escravista (púrpura e areia). **[Proposta]** A paleta mestre parte de uma paleta pública do Lospec (Endesga 32 ou Resurrect 64), ajustada.

### Variações por cultura

- **Primeiras versões:** **troca de paleta**. Cada peça (armadura, arma, roupa, parte de construção) é desenhada uma vez, à mão, e o jogo troca as cores por uma tabela: o "metal" vira ferro escuro na Guerreira e bronze polido na Escravista. A mesma técnica dá a cor do material (Tier) e da roupa.
- **Objetivo final:** cada cultura com **estilo de arte próprio** nas **armas, armaduras e construções** (formas diferentes, não só cores). **[Refinar depois]** A troca de paleta continua valendo para as variações de cor dentro de cada cultura.
- **Construções:** a aparência muda nos **níveis 1, 3 e 5** (os níveis 2 e 4 repetem a aparência anterior com detalhes pequenos), montada com **peças reaproveitáveis** por cultura (paredes, telhados, portas).

### Animação

Animações compartilhadas por tipo de arma, por camada do boneco (corpo, 3 peças de armadura, arma e escudo):

| Animação | Quadros por direção |
|---|---|
| Parado | 2 |
| Andar (correr usa o andar acelerado) | 4 |
| Ataque, para cada um dos 8 tipos de arma | 4 (32 no total) |
| Ferido | 1 |
| Cair | 3 |
| Trabalhar: golpear (machado, picareta, martelo), abaixar e colher, carregar | 4 cada (12 no total) |
| **Total** | **54** |

São desenhadas **3 direções** (cima, baixo, direita); a esquerda é a direita espelhada. Total: cerca de **162 quadros por camada**. Com o espelhamento, o escudo troca de mão quando o boneco vira.

### Estações

- **Primeiras versões:** **só mudança de cor** (tingimento) do mapa por estação: mais amarelado no outono, azulado no inverno. Nenhum tile a mais.
- **Regra do autor:** tudo o que exige arte a mais nas estações fica para o futuro.
- **Clima e dia** (decididos no [[Sistema de Clima e Estações/Descrição|Clima e Estações]]): paleta por estação na placa de vídeo (primavera verde vivo, verão quente, outono alaranjado, inverno frio e dessaturado); tom de cor nas 4 fases do dia, sem luzes dinâmicas; camada de clima com partículas (pingos, flocos) e névoa translúcida. Nenhum tile novo: **neve caindo sim, neve no chão no futuro**.
- **Futuro:** telhados com neve, rios congelados, árvores sem folhas, folhas caindo no outono, vegetação com flores na primavera, tochas e janelas acesas à noite. **[Refinar depois]**

### Interiores

- **Com interior próprio:** Taverna, Mercado, Templo, Prisão, Palácio, Quartel e as 12 masmorras.
- **Com interior genérico** por tamanho (pequeno, médio e grande) e cultura, decorado com os móveis do tipo: Moradias e construções de produção. Isso cobre o furto e a loja ilegal disfarçada (ver [[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).

### Ordem de produção

A arte é feita em camadas, igual à programação:

1. **Fatia vertical:** 1 bioma (planície), 1 cultura (**Autoritária**), 1 Mapa Local, Fazenda, Cozinheiro, Mercado e Casa, 1 boneco com parado, andar e trabalhar. Com pacotes de terceiros e rascunhos.
2. **Cidade completa** da mesma cultura.
3. **A primeira região** inteira (8 mapas).
4. As **outras culturas e biomas**.

**Volume estimado do jogo completo** (para dimensionar, não é limite):

| Item | Conta | Na primeira cultura |
|---|---|---|
| Tiles de terreno | cerca de 200 por bioma × 6 = 1.200 | cerca de 200 |
| Construções | 22 tipos × 3 aparências × 6 culturas = 396 | 66 |
| Bonecos | cerca de 20 camadas × 162 quadros = 3.200 quadros | cerca de 1.500 (troca de paleta) |
| Mapas Locais | 48 de 128 × 128 tiles | 1 na fatia vertical; 8 na primeira região |
| Criaturas | cerca de 10 tipos × 40 quadros | 3 tipos |
| Interface | molduras, botões e cerca de 150 ícones de itens | ícones da fatia vertical |
| Muralhas | tileset de paliçada e de pedra (paleta por cultura), com portão e brecha | 1 tileset |
| Clima | poucos sprites de partícula (pingo, floco) e uma textura de névoa | os mesmos |

### Peso da arte

O autor pediu atenção ao **peso da arte no jogo**. **[Proposta]** Estimativa, considerando imagens sem compressão na memória da placa de vídeo (4 bytes por pixel):

| Item | Conta | Memória |
|---|---|---|
| 1 tile (16 × 16) | 256 pixels × 4 bytes | 1 KB |
| Tiles de terreno (sem arte extra de estações) | cerca de 1.200 tiles | cerca de 1,2 MB |
| Bonecos, no pior caso (sem espelhamento) | 20 camadas × 216 quadros × 1,5 KB | cerca de 6,6 MB |
| Bonecos com forma própria por cultura (meta final) | o mesmo × 6 | cerca de 40 MB |
| Construções (média de 64 × 64 px) | 22 tipos × 5 níveis × 6 culturas × 16 KB | cerca de 11 MB |
| Mapas Locais | não são imagens, são números: 128 × 128 × 7 camadas × 2 bytes | cerca de 230 KB por mapa; 11 MB os 48 |

**Conclusão:** mesmo o jogo completo deve ficar abaixo de **100 MB de arte** na placa de vídeo. Pixel art é leve; o que pesa de verdade é a **quantidade de coisas desenhadas por quadro** e a **simulação dos NPCs**.

**Regras para manter leve:**

- Juntar as imagens em **atlas** (uma imagem grande, até 2048 × 2048, com várias imagens menores) e desenhar em lote.
- **Carregar só o necessário:** a arte das culturas e biomas dos mapas ativos; o resto fica no disco.
- O Mapa Mundial usa arte própria, pequena.

**Como verificar:** na fatia vertical, medir com o **Tracy** (já previsto na [[Arquitetura/Descrição|Arquitetura]]) e com um contador de memória de imagens no painel de depuração. **[Proposta]** Teto inicial: **256 MB** de arte na placa de vídeo, a revisar quando o PC alvo mínimo for definido.

### Dados e ferramentas

O jogo é feito em **C++**. Outras linguagens podem ajudar, mas nada essencial depende delas.

- **Edição:** **VS Code** com **esquemas JSON**: o editor completa os campos e sublinha os erros enquanto se digita.
- **Tabelas grandes e simples** (itens, receitas, preços): editadas em planilha (LibreOffice ou Excel) e salvas em **CSV**, um formato de texto simples. **[Proposta]** O próprio jogo lê o CSV direto, em C++, então não é preciso um script de conversão em outra linguagem.
- **Validação:** feita **pelo jogo, em C++**, ao carregar os dados. **[Proposta]** O jogo também tem um modo só de verificação (`jogo --validar`), que confere os dados, lista os erros ("a receita Pão cita o item Farinha de Trigo, que não existe") e fecha. Pode ser usado antes de cada commit.
- **Quem manda nas listas:** quando a pasta `data/` for criada (na ficha técnica), **os dados passam a mandar** nas listas (itens, receitas, construções, preços) e os documentos ficam com as regras. Os canvases viram arquivo histórico ou são gerados a partir dos dados. Até lá, a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] continua mandando.

### Onde guardar

- Tudo no **repositório do código**, criado quando a programação começar: `assets/` com as fontes (arquivos do Pixelorama e do Tiled) e `data/` com o que o jogo lê.
- **Sem Git LFS** (complemento do Git para arquivos grandes): pixel art é leve.
- Este repositório continua só com os documentos.

### Parte técnica

- **Fluxo:** Pixelorama → PNG (folhas de sprites) → atlas; Tiled → mapa em JSON. **[Refinar depois]** A ferramenta que monta os atlas (de terceiros ou nossa, em C++) fica para o início do código.
- **[Proposta] Pastas:**

```
assets/            fontes (Pixelorama, Tiled)
  provisorio/      pacotes de terceiros + LEIA-ME.txt com a origem
data/
  imagens/         PNG exportados
  mapas/           mapas do Tiled em JSON
  tabelas/         CSV (itens, receitas, preços)
  *.json           demais dados
```

- **Mundo ajustável:** o tamanho da malha do Mapa Mundial vem dos dados (`mapas.json`), não de números fixos no código, para o mundo poder crescer depois dos testes (ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- **Troca de paleta:** feita na placa de vídeo, na hora de desenhar, com uma tabela de cores por cultura e material. Não custa memória extra.
- **Fatia vertical:** usa só arte provisória; serve também para medir o peso da arte.

### Ligação com outros sistemas

| Sistema | O que o Pipeline fornece ou recebe |
|---|---|
| Arquitetura | Atlas, desenho em lote, Tracy, validação em C++, pastas `assets/` e `data/` |
| Mapa | Padrão de camadas do Tiled; interiores próprios ou genéricos |
| Mundo e Estado Inicial | Malha ajustável pelos dados |
| Cultura | Troca de paleta agora, estilo próprio no futuro; cores das culturas |
| Construção | Aparência nos níveis 1, 3 e 5, com peças reaproveitáveis |
| Combate | Boneco de 16 × 24, 54 quadros por direção, 3 direções e espelho |
| Batalha e Guerra | 7ª camada Elevação; tileset de Muralha; os bonecos da batalha são os mesmos, com paleta por cultura (ver [[Sistema de Batalha/Descrição\|Sistema de Batalha]]) |
| Interface | Cores das culturas na lente Política; ícones de itens |
| Clima e Estações | Paleta por estação, tom do dia, partículas e névoa; neve no chão, rios congelados, folhas e flores no futuro (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Itens | A lista de itens manda até a pasta `data/` existir |

### Perguntas em aberto

1. **Artista definitivo:** ainda não escolhido.
2. **Pacotes provisórios:** quais usar na fatia vertical (escolher na hora, conferindo a licença).
3. **Ferramenta de atlas:** de terceiros ou própria, no início do código.
4. **Teto de memória da arte:** depende do PC alvo mínimo (Arquitetura).
5. **Estações no futuro:** quando entram neve, rios congelados, folhas e flores (com o Clima e Estações).
6. **Estilo próprio por cultura:** quando sai da troca de paleta para formas próprias.
