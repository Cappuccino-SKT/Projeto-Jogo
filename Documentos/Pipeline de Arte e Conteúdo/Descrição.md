> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Pipeline de Arte e Conteúdo

**Descrição geral**

Este documento define **como a arte e os dados do jogo são produzidos**: quem desenha, com quais programas, em que formato, em que ordem, e como saber se a arte está pesando no jogo.

O projeto **não tem prazo**. O limite da arte não é o tempo de produção, e sim o **peso no jogo** (memória e desempenho), que deve ser medido (ver "Peso da arte").

### Quem desenha

- **Novo estilo (decisão do autor, 2026-10-26): parecido com o do RimWorld, no lugar da pixel art.** Bonecos simples vistos de cima: cabeça grande e redonda, corpo de "manequim", contorno escuro grosso, cores lisas com sombra leve, e as roupas, as armaduras e os chapéus desenhados por cima, em camadas. As referências do autor estão em `referencias/` (ao lado deste documento): `manequins_do_autor_gemini.jpg` (o corpo, em vários tons de pele, homem, mulher e criança) e `roupas_do_autor_gemini.jpg` (nobres, piqueiros, damas e camponeses). **Não são a arte final**; são a base em que o autor vai se apoiar.
- **IA de imagem volta a valer como ferramenta (2026-10-26):** neste estilo, a IA gera bases úteis, o que não acontecia com a pixel art. **A ferramenta é o ChatGPT** (decisão do autor, 2026-10-26): o Gemini não manteve o boneco igual entre os pedidos; o ChatGPT fez o **manequim oficial** (`referencias/manequim_oficial_homem.webp` e `manequim_oficial_mulher.webp`). O autor gera as bases e corrige à mão.
- **O assistente também consegue desenhar peças por código** (Python, com a biblioteca Pillow), no formato do jogo: bom para o corpo, as peças simples e os testes; roupas com muito detalhe ficam mais simples que as da IA.
- **Por enquanto, sem arte no jogo:** o jogo segue com a arte de teste do assistente (os quadrados de cor e o boneco de teste) até o autor ter a primeira leva de arte (AR1-6).
- **Pacotes de terceiros:** só onde ainda faltar uma peça, com a origem anotada em `assets/provisorio/LEIA-ME.txt`. Preferir licença **CC0** (domínio público) ou que permita uso em protótipo.
- **Jogo final:** toda a arte passa pelas mãos do artista do projeto; o que for provisório sai.

### Programas

| Uso | Programa | Custo |
|---|---|---|
| Montar os mapas | **Tiled** | Grátis |
| Gerar as bases dos bonecos e das roupas | **ChatGPT** (IA de imagem) | Grátis |
| Corrigir e pintar as imagens | [Em aberto] na rodada AR1 (sugestão: **Krita**, grátis) | Grátis |
| Peças geradas por código (corpo, testes) | Python + Pillow (o assistente) | Grátis |
| Editar os dados | **VS Code** | Grátis |

O **Pixelorama** e as regras da pixel art saem com a troca de estilo (2026-10-26).

### Mapas no Tiled

O Tiled funciona como um "Paint de azulejos": escolhe-se um tile e pinta-se no mapa. Cada Mapa Local segue o mesmo padrão, porque o jogo lê as camadas pelo nome. Entre parênteses, o **nome exato no Tiled**, sem acento e em minúsculas (rodada FL1-5).

**Camadas de tiles:**

1. **Chão** (`chao`): grama, terra, água.
2. **Detalhes do chão** (`detalhes_do_chao`): flores, caminhos.
3. **Objetos baixos** (`objetos_baixos`): pedras, cercas, lavouras.
4. **Construções e paredes** (`construcoes_e_paredes`): só o que nunca muda, como um muro. As construções da cidade **não são pintadas no Tiled**: o jogo as desenha nos lotes, porque elas mudam durante a partida.
5. **Acima do personagem** (`acima_do_personagem`): copas de árvore, telhados.
6. **Colisão** (`colisao`): invisível no jogo; marca onde não se anda.
7. **Elevação** (`elevacao`, opcional): pintada só nas colinas; usada no Mapa de Guerra (ver [[Sistema de Batalha/Descrição|Sistema de Batalha]]). Um mapa sem ela é plano.

**Configuração do Tiled (FL1-1 e FL1-5):** o mapa é salvo direto em JSON (`.tmj`) em `Jogo/data/mapas/`; camadas no formato CSV; conjunto de tiles embutido no mapa, sem margem; nenhum tile espelhado; um lote não pode ter tile de colisão. O jogo lista todos os erros do mapa de uma vez ([[Ficha Logica/Mapa|ficha do Mapa]]).

**Camadas de objetos:** Lotes (`lotes`: pequeno, médio e grande, com a propriedade `tipo_permitido`, que na fatia é sempre `qualquer`); Portas (ligam ao Mapa Interno); Bordas (saída para o mapa vizinho); Recursos (áreas de floresta, mina, pesca; a pesca voltou na revisão V1-4); Criaturas e bandidos (pontos de aparição); Pontos de interesse (quadro de contratos, entrada de masmorra, praça central para a batalha de cerco).

**Ajudas do editor:** **terrenos** (pinta-se "grama" e o Tiled escolhe sozinho o tile de borda certo) e **carimbos** (pedaços prontos, como "bosque pequeno", colados várias vezes). São só ajudas de desenho: o mapa salvo é fixo e feito à mão, não é gerado pelo jogo.

### Especificação da arte

**Estilo RimWorld (2026-10-26; rodada AR1).** As regras de desenho e o passo a passo com o ChatGPT estão no [[Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]].

| Item | Decisão |
|---|---|
| **Duas visões, pelo zoom** (ideia do autor, AR1) | **De perto:** o boneco visto em 3/4, com o corpo detalhado (como as primeiras linhas das referências). **De longe:** vista de cima para baixo, quase só a cabeça, o chapéu e os ombros (como as linhas de "cabeças vistas de cima" das referências). Entre as duas há uma **transição**. Vale para os **bonecos e as construções** (de perto, a fachada; de longe, o telhado) |
| Tamanho (AR1-1, opção C) | Os mapas e as contas continuam em tiles de 16 (o Tiled não muda). As **imagens** têm **64 × 64 por tile**; o boneco tem **128 × 128** (cerca de 1,5 tile). O jogo reduz ao desenhar |
| Direções (AR1-3) | **3 desenhadas:** frente, costas e lado; o outro lado é o espelho |
| Boneco | Um **manequim** (o corpo, por tom de pele e por raça) com as peças por cima, em camadas: roupa, armadura, chapéu, cabelo, arma |
| Traço | Contorno escuro grosso, cores lisas, sombra leve |
| Paleta | Livre; as cores das culturas continuam |
| Programa | **Krita** para corrigir e pintar (AR1-5); bases geradas no **ChatGPT** |
| Quando o código muda (AR1-6) | Quando o autor tiver a primeira leva de arte (o corpo e 2 ou 3 roupas). Até lá, a arte de teste continua |

**Cores das culturas** (bandeiras, roupas, telhados e a lente Política do Mapa Mundial): Guerreira (vermelho-sangue e ferro), Arcana (roxo e azul-noite), Autoritária (dourado e preto), Religiosa (branco e azul-celeste), Tecnocrata (cobre e verde-oliva), Escravista (púrpura e areia). As cores das culturas saem da paleta Resurrect 64 (o Guia da Arte diz quais).

### Variações por cultura

- **Primeiras versões:** **troca de paleta**. Cada peça (armadura, arma, roupa, parte de construção) é desenhada uma vez, à mão, e o jogo troca as cores por uma tabela: o "metal" vira ferro escuro na Guerreira e bronze polido na Escravista. A mesma técnica dá a cor do material (Tier) e da roupa.
- **Objetivo final:** cada cultura com **estilo de arte próprio** nas **armas, armaduras e construções** (formas diferentes, não só cores). **[Refinar depois]** A troca de paleta continua valendo para as variações de cor dentro de cada cultura.
- **Construções:** a aparência muda nos **níveis 1, 3 e 5** (os níveis 2 e 4 repetem a aparência anterior com detalhes pequenos), montada com **peças reaproveitáveis** por cultura (paredes, telhados, portas).

### Animação

**Rodada AR1-2 (pedido do autor):** animações **simples**, desenhadas pelo autor, diferentes em cada visão.

| Visão | O que anima |
|---|---|
| **De perto** | Andar (pernas se mexendo, braços balançando), o combate simples (o golpe da arma) e outras ações simples. Poucos quadros por ação |
| **De longe** | Quase nada: a visão é para ver a quantidade, a posição das construções e as batalhas grandes. No máximo, as armas se movendo no combate entre dois NPCs |

O número de quadros de cada ação fica no [[Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]] [Proposta]. A tabela antiga (162 quadros por peça) era da pixel art e saiu.

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

1. **Fatia vertical:** 1 bioma (planície), 1 cultura (**Autoritária**), 1 Mapa Local, as 7 construções da vila, 1 boneco com parado, andar e trabalhar. Desenhada pelo autor (a lista completa e a ordem estão no [[Pipeline de Arte e Conteúdo/Guia da Arte\|Guia da Arte]]).
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

O autor pediu atenção ao **peso da arte no jogo**. Estimativa, considerando imagens sem compressão na memória da placa de vídeo (4 bytes por pixel):

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

**Como verificar:** na fatia vertical, medir com o **Tracy** (já previsto na [[Arquitetura/Descrição|Arquitetura]]) e com um contador de memória de imagens no painel de depuração. Teto inicial: **256 MB** de arte na placa de vídeo, a revisar quando o PC alvo mínimo for definido.

### Dados e ferramentas

O jogo é feito em **C++**. Outras linguagens podem ajudar, mas nada essencial depende delas.

- **Edição:** **VS Code** com **esquemas JSON**: o editor completa os campos e sublinha os erros enquanto se digita.
- **Tabelas grandes e simples** (itens, receitas, preços): editadas em planilha (LibreOffice ou Excel) e salvas em **CSV**, um formato de texto simples. O próprio jogo lê o CSV direto, em C++, então não é preciso um script de conversão em outra linguagem.
- **Validação:** feita **pelo jogo, em C++**, ao carregar os dados. O jogo também tem um modo só de verificação (`jogo --validar`), que confere os dados, lista os erros ("a receita Pão cita o item Farinha de Trigo, que não existe") e fecha. Pode ser usado antes de cada commit.
- **Quem manda nas listas:** quando a pasta `data/` for criada (na ficha técnica), **os dados passam a mandar** nas listas (itens, receitas, construções, preços) e os documentos ficam com as regras. Os canvases viram arquivo histórico ou são gerados a partir dos dados. Até lá, a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] continua mandando.

### Onde guardar

- Tudo na pasta `Jogo/` deste repositório (rodada FT1-7 e organização de 2026-10-17): `Jogo/assets/` com as fontes (arquivos do Pixelorama e do Tiled) e `Jogo/data/` com o que o jogo lê.
- **Sem Git LFS** (complemento do Git para arquivos grandes): pixel art é leve.

### Parte técnica

- **Fluxo:** Pixelorama → PNG (folhas de sprites) → atlas; Tiled → mapa em JSON. **[Refinar depois]** A ferramenta que monta os atlas (de terceiros ou nossa, em C++) fica para o início do código.
- **Pastas:**

```
assets/            fontes (Pixelorama, Tiled)
  provisorio/      pacotes de terceiros + LEIA-ME.txt com a origem
data/
  imagens/         PNG exportados
  mapas/           mapas do Tiled em JSON
  textos/          pt-BR.json (os nomes da tela)
  *.csv            tabelas da planilha (itens, receitas, recursos)
  *.json           demais dados (regras, construções)
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

1. **Artista definitivo:** ainda não escolhido (rodada V3-13). Na fatia, desenha o autor. O marco 1 usa um conjunto de tiles de teste, de quadrados de cor lisa, feito pelo assistente; depois, o autor redesenha o mapa com os tiles dele (rodada FL1-6).
2. **Ferramenta de atlas:** de terceiros ou própria, no início do código.
3. **Teto de memória da arte:** 256 MB proposto; o PC alvo mínimo tem placa de vídeo de 2 GB (rodada V3-12).
4. **Estações no futuro:** quando entram neve, rios congelados, folhas e flores (com o Clima e Estações).
5. **Estilo próprio por cultura:** quando sai da troca de paleta para formas próprias.
