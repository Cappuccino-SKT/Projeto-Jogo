---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 2** (câmera e entrada). Cobre só **a câmera, o zoom e as teclas**. A **parte 10** traz o **marco 8**: o HUD, os painéis e os pedidos do jogador (rodada FL8). As decisões da rodada **FL2** (2026-10-18, todas "seguir sugestão") já estão aplicadas, e o código do marco 2 as segue.

# Ficha Lógica: Interface (câmera, zoom e teclas)

## 1. O que faz

A **câmera** decide que pedaço do Mapa Local aparece na janela. O jogador a move com o teclado (WASD ou setas) ou arrastando com o botão do meio, e troca o **zoom** com a roda do mouse, sempre em níveis inteiros (2x, 3x e 4x), para a pixel-art continuar nítida (FT1-4).

Documento do sistema: [[Interface/Descrição|Interface e Controles]], seção "Controles". O desenho do mapa que usa a câmera está na [[Ficha Logica/Mapa|ficha do Mapa]], regra 4.3.

**Onde fica o código:** a câmera não é regra do jogo (o teste sem tela não precisa dela), então fica no `Jogo/`. Mas as contas dela não usam a SDL nem o OpenGL, para poderem ser testadas sozinhas, num programa de testes do `Jogo/`.

**A câmera anda pelo tempo real, não pelos ticks.** Com o jogo pausado, o jogador continua olhando o mapa; e a 4x a câmera não fica 4 vezes mais rápida.

## 2. Dados

### Definições (não mudam durante o jogo)

No marco 2, ficam numa estrutura do código. Quando o perfil do jogador existir (marco 8 ou depois), a velocidade e as teclas passam para ele ([[Interface/Descrição|Interface]], "Perfil do jogador").

| Campo | Tipo | Unidade | Valor |
|---|---|---|---|
| `niveisDeZoom` | lista de inteiros | multiplicador | 2, 3, 4 (FT1-4) |
| `zoomInicial` | inteiro | multiplicador | 3 (FT1-4) |
| `velocidadeNoTecladoEmPixelsDaTelaPorSegundo` | decimal | pixels da tela por segundo | 900 (FL2-1) |
| `multiplicadorDaVelocidadeComShift` | decimal | — | 2 (FL2-1) |

**Por que medir a velocidade em pixels da tela:** assim a câmera parece andar igual em qualquer zoom. A 3x, 900 pixels da tela por segundo são 300 pixels do mundo, ou seja, cerca de 19 tiles por segundo; o mapa de 64 tiles é atravessado em pouco mais de 3 segundos.

### Estado (muda durante o jogo)

| Campo | Tipo | Unidade | Exemplo | Vai no save? |
|---|---|---|---|---|
| `posicaoXNoMundo`, `posicaoYNoMundo` | decimais | pixels do mundo | (245,3; 372,5): o canto de cima à esquerda da tela | Não (FL2-2) |
| `zoom` | inteiro | 2, 3 ou 4 | 3 | Não |
| `larguraDaTela`, `alturaDaTela` | inteiros | pixels da tela | 1600 × 837 | Não (vem da janela a cada quadro) |
| `sobraDaRodaDoMouse` | decimal | passos da roda | 0,4 | Não (é só uma sobra) |
| Arrastando com o botão do meio | sim/não | — | sim | Não |

- **Pixels do mundo e pixels da tela:** um tile tem 16 pixels do mundo. Com zoom 3x, ele ocupa 48 pixels da tela.
- **Por que a posição tem vírgula:** com 15 pixels da tela por quadro a 3x, a câmera anda 5 pixels do mundo; mas a 60 quadros por segundo com Shift e zoom 4x, anda 7,5. Sem a vírgula, a sobra se perderia e a câmera andaria aos trancos.
- **A câmera não vai no save (FL2-2):** ao carregar, ela começa como numa partida nova (regra 4.5). Gravar a posição é fácil de fazer depois, se o autor quiser.

## 3. Quando roda

Tudo roda **a cada quadro da tela**, no `Jogo/`, antes de desenhar:

1. Os eventos da SDL: a roda do mouse (regra 4.3) e o botão do meio (regra 4.2).
2. As teclas seguradas (regra 4.1).
3. O limite ao mapa (regra 4.4), que também acerta a câmera quando a janela muda de tamanho.
4. O desenho usa o deslocamento em pixels inteiros (regra 4.6).

## 4. Regras passo a passo

### 4.1 Mover pelo teclado (a cada quadro)

1. Leia as teclas **seguradas** (não as apertadas): W ou ↑ para cima, S ou ↓ para baixo, A ou ← para a esquerda, D ou → para a direita. As letras valem pela **posição** no teclado, então funcionam em qualquer layout.
2. Monte a direção: cima soma −1 no y, baixo +1, esquerda −1 no x, direita +1. Segurando as duas opostas, elas se anulam.
3. **Diagonal (FL2-1):** se a direção tem x e y, multiplique os dois por 0,7071 (1 ÷ √2), para a diagonal não ser 41% mais rápida.
4. Distância na tela = velocidade × segundos reais do quadro; com Shift, × 2 (FL2-1).
5. Distância no mundo = distância na tela ÷ zoom. Some à posição.
6. Rode a regra 4.4.

### 4.2 Arrastar com o botão do meio

1. Ao apertar o botão do meio, marque "arrastando"; ao soltar, desmarque.
2. A cada movimento do mouse, enquanto arrasta: posição −= movimento do mouse na tela ÷ zoom. O ponto do mapa que estava embaixo do mouse continua embaixo dele, como quem puxa uma folha de papel.
3. Rode a regra 4.4.

### 4.3 Trocar o zoom pela roda do mouse

1. Some à `sobraDaRodaDoMouse` o quanto a roda girou (para cima é positivo). A roda comum gira de 1 em 1; o touchpad manda pedaços, como 0,3.
2. Enquanto a sobra for 1 ou mais: suba um nível (2x → 3x → 4x) e tire 1. Enquanto for −1 ou menos: desça um nível e some 1. No 4x não sobe, no 2x não desce, e a sobra do lado bloqueado é jogada fora.
3. **O ponto embaixo do mouse fica parado (FL2-2):** antes de trocar, calcule o ponto do mundo embaixo do mouse; depois, ponha a câmera de modo que esse ponto continue embaixo do mouse (contas na parte 5).
4. Rode a regra 4.4.

### 4.4 Limitar a câmera ao mapa (FL2-2)

Para cada eixo (x e y):

1. Tamanho visível no mundo = tamanho da tela ÷ zoom.
2. Se o mapa é **maior** que o visível: a posição fica entre 0 e (tamanho do mapa − visível). A borda do mapa nunca sai da borda da tela, e não aparece fundo preto.
3. Se o mapa é **menor** que o visível (uma janela muito grande no zoom 2x): a câmera fica parada, com o mapa **centralizado**: posição = −(visível − mapa) ÷ 2.

### 4.5 Posição inicial

Ao abrir o mapa, com o zoom 3x, a câmera fica com o **centro do mapa no centro da tela** (FL2-2), e depois roda a regra 4.4. No marco 6, quando a Carpintaria do jogador existir, a câmera pode começar nela.

### 4.6 Pixel inteiro no desenho

1. Deslocamento na tela = posição × zoom, **arredondado** para o inteiro mais perto.
2. Cada tile é desenhado em (tile × 16 × zoom − deslocamento).

**Por que arredondar uma vez só, e não cada tile:** se cada tile fosse arredondado por conta própria, uns iam para um lado e outros para o outro, e apareceriam linhas finas entre os tiles quando a câmera anda. Arredondando só o deslocamento, todos andam juntos, pixel a pixel.

## 5. Contas

Exemplos com o mapa da vila (64 × 64 tiles = 1.024 × 1.024 pixels do mundo) e a janela maximizada do autor (1600 × 837 pixels da tela).

**Tamanho visível e limites:**

| Zoom | Visível no mundo | x vai de | y vai de |
|---|---|---|---|
| 2x | 800 × 418,5 | 0 a 224 | 0 a 605,5 |
| 3x | 533,3 × 279 | 0 a 490,7 | 0 a 745 |
| 4x | 400 × 209,3 | 0 a 624 | 0 a 814,8 |

**Posição inicial (3x):** x = (1.024 − 533,3) ÷ 2 = **245,3**; y = (1.024 − 279) ÷ 2 = **372,5**.

**Teclado:** a 60 quadros por segundo, um quadro dura 1/60 s. 900 × 1/60 = 15 pixels da tela; a 3x, 15 ÷ 3 = **5 pixels do mundo** por quadro. Na diagonal: 5 × 0,7071 = 3,54 em cada eixo.

**Zoom com o ponto embaixo do mouse:** câmera em (100, 200), zoom 3x, mouse em (800, 400) na tela.

1. Ponto do mundo embaixo do mouse = (100 + 800 ÷ 3; 200 + 400 ÷ 3) = (366,7; 333,3).
2. Zoom 4x: câmera = (366,7 − 800 ÷ 4; 333,3 − 400 ÷ 4) = **(166,7; 233,3)**.
3. Conferindo: 166,7 + 800 ÷ 4 = 366,7. O ponto continua embaixo do mouse.

**Arrastar:** a 3x, o mouse vai 30 pixels para a direita; a câmera vai 30 ÷ 3 = 10 pixels do mundo para a **esquerda**, e o mapa parece andar junto com o mouse.

**Pixel inteiro:** câmera em x = 245,3 a 3x: 245,3 × 3 = 736 pixels da tela. O tile 16 aparece em 16 × 16 × 3 − 736 = **32**.

## 6. Conversa com outros sistemas

| Sistema | O que lê da câmera | O que entrega à câmera |
|---|---|---|
| Desenho do mapa ([[Ficha Logica/Mapa\|Mapa]], regra 4.3) | O deslocamento em pixels inteiros e o zoom | — |
| Mapa | — | O tamanho do mapa em pixels do mundo, para o limite |
| Janela (o laço principal) | — | O tamanho da tela, as teclas seguradas, a roda e o botão do meio, e os segundos reais do quadro |
| Relógio do mundo | Nada: a câmera anda pelo tempo real, mesmo pausado | — |
| Notificações (depois da fatia) | — | "Clicar centraliza a câmera no local" ([[Interface/Descrição\|Interface]]) |
| NPCs e jogador (marcos 4 a 6) | O deslocamento, para desenhar no lugar certo | — |

## 7. Nomes no código

Em `Jogo/` (sem SDL nem OpenGL, para os testes):

| Nome | O que é |
|---|---|
| `RegrasDaCamera` | As definições da parte 2 |
| `CameraDoMapa` | A classe com o estado da parte 2 e as regras da parte 4 |
| `moverPeloTeclado(direcaoX, direcaoY, segundosReaisDoQuadro, correndoComShift)` | A regra 4.1 |
| `comecarArrasto()`, `arrastarPeloMouse(deslocamentoXNaTela, deslocamentoYNaTela)`, `terminarArrasto()` | A regra 4.2 |
| `mudarZoomPelaRoda(quantoARodaGirou, mouseXNaTela, mouseYNaTela)` | A regra 4.3 |
| `limitarAoMapa()` | A regra 4.4 |
| `centralizarNoMapa()` | A regra 4.5 |
| `deslocamentoXNaTela()`, `deslocamentoYNaTela()` | A regra 4.6 |
| `atualizarTamanhoDaTela(largura, altura)` e `definirTamanhoDoMapa(larguraEmPixelsDoMundo, alturaEmPixelsDoMundo)` | O que a câmera precisa saber |

No `main.cpp`, a função `lerDirecaoDasTeclasSeguradas` monta a direção da regra 4.1 a partir do teclado da SDL.

**A tabela de ações** (marco 3, FL2-4), em `Jogo/AcoesDoTeclado.cpp`:

| Nome | O que é |
|---|---|
| `AcaoDoJogador` | As ações: fechar, tela cheia, mostrar a colisão, pausar, as 3 velocidades, as 4 direções da câmera e correr |
| `TeclasDeUmaAcao` | Uma linha da tabela: a ação, a tecla principal e uma alternativa (pela posição no teclado) |
| `TabelaDeAcoesDoTeclado`, `criarComAsTeclasPadrao()` | A tabela, com as teclas dos marcos 1 e 2 |
| `acaoDaTeclaApertada(tecla)` e `acaoEstaSegurada(acao, estadoDoTeclado)` | O que o laço principal pergunta |

## 8. Testes

Num programa de testes do `Jogo/` (`testes_do_jogo`), rodado pelo mesmo `ctest`. Tela de 1600 × 837 e mapa de 1.024 × 1.024, como na parte 5.

| Entrada | Resultado esperado |
|---|---|
| Câmera nova, centralizada, 3x | Posição (245,3; 372,5) |
| Direita por 1 s, 3x | x anda 300 pixels do mundo |
| Direita por 1 s, 2x | x anda 450 pixels do mundo (a mesma velocidade na tela) |
| Direita com Shift por 0,1 s, 3x | x anda 60 |
| Diagonal por 0,1 s, 3x | x e y andam 21,2 cada |
| Esquerda e direita juntas | Não anda |
| Esquerda por 10 s, a partir do centro | Para em x = 0 |
| Baixo por 10 s, 3x | Para em y = 745 |
| Zoom 3x → 4x, câmera (100, 200), mouse (800, 400) | Câmera (166,7; 233,3) |
| Roda +1 no 4x | Continua 4x |
| Roda +0,4 três vezes, no 3x | Na terceira vira 4x |
| Arrastar 30 pixels para a direita, 3x | x diminui 10 |
| Tela de 2400 × 2400 num zoom 2x (maior que o mapa) | Mapa centralizado: posição (−88, −88) |
| Câmera em x = 245,3, 3x | Deslocamento na tela = 736 |

## 9. Fora da fatia (e fora do marco 2)

- Mover a câmera pela **borda da tela**: uma opção futura nas configurações, desligada por padrão (FL2-3).
- **Atalhos remapeáveis** pelo jogador: no marco 2, as teclas continuam direto no código, e uma tabela de ações entra no marco 3, pronta para o perfil do jogador (FL2-4). As letras valem pela posição no teclado.
- O HUD, os painéis, as notificações e o modo de edição da interface (marco 8).
- Clicar numa notificação para centralizar a câmera; seguir um NPC com a câmera.
- O minimapa e as lentes do Mapa.

---

## 10. Marco 8: o HUD, os painéis e os pedidos do jogador

> Escrita em 2026-10-19. Depende da rodada **FL8** ([[Rodada Pendente]]); os valores sem decisão estão marcados [Proposta]. As obras estão na parte 10 da [[Ficha Logica/Construcao|Construção]], e o preço, o salário, a contratação e a compra e venda, na parte 11 da [[Ficha Logica/Economia|Economia]].

### 10.1 O que o marco 8 traz

| Item | No marco 8 |
|---|---|
| Como desenha | O **Dear ImGui**, só na fatia, com um tema de cores do jogo (FL8-1) [Proposta] |
| Fonte | **Noto Sans**, 18 px, em `Jogo/assets/fontes/` com a licença (FL8-2) [Proposta]; a escala fica em 100% até o perfil do jogador |
| HUD | A barra de cima, as notificações e a barra de alertas |
| Gestão | Uma janela por vez, com as abas **Cidade, Construções, Inventário, Mercado e Diário** |
| Seleção | O botão esquerdo num lote abre o painel da construção (ou do lote livre) |
| Pedidos | Tudo o que o jogador decide vira um **pedido**, aplicado no começo do próximo tick (regra 10.8) |

### 10.2 A barra de cima (sempre na tela)

Da esquerda para a direita, numa faixa de 32 px [Proposta]:

1. A data e a hora: "Ano 1, Primavera, dia 3 · 07:40 · Manhã" (o mesmo texto do título da janela de hoje).
2. A velocidade: 4 botões (⏸, 1x, 2x, 4x), com o atual destacado. As teclas Espaço, 1, 2 e 3 continuam valendo.
3. O dinheiro: "Jogador 104,5" e "Banco 3.950,0", com o "Por quê?" (regra 10.7): o que entrou e saiu ontem.
4. O botão "Gestão (Tab)", que abre e fecha a janela de gestão.

### 10.3 As notificações e o Diário (FL8-9) [Proposta]

- Entram: o começo e o fim de cada evento, uma obra ou manutenção pronta, um empregado que sai (o salário atrasado ou o salário Baixo).
- **Info** para as boas e **Aviso** para as ruins (a fatia não tem Crítico).
- À direita, abaixo da barra de cima: no máximo 5 na tela, a mais nova em cima; cada uma some depois de 20 segundos reais (contados a 1x; parados na pausa).
- A aba **Diário** guarda as 300 últimas, com a data. O console continua com as mesmas linhas.

### 10.4 A barra de alertas [Proposta]

Uma faixa abaixo da barra de cima, com os alertas que valem agora, conferidos a cada hora de jogo:

| Alerta | Quando |
|---|---|
| "Pão para menos de 2 dias" | O estoque de Pão < 2 × o consumo previsto do dia |
| "Banco baixo" | O Banco < 10% do que tinha no começo da estação |
| "<Construção> parada: falta <item>" | Uma construção do jogador parada por falta de insumo |
| "Manutenção atrasada: <construção>" | Parte 10 da [[Ficha Logica/Construcao\|Construção]] |

Clicar num alerta abre a aba que mostra o problema.

### 10.5 A janela de gestão (tecla Tab)

Uma janela no centro, com abas, uma por vez (o documento da [[Interface/Descrição|Interface]]). Enquanto ela está aberta, o jogo continua na velocidade escolhida.

| Aba | O que mostra |
|---|---|
| **Cidade** | A população (15: empregados, desempregados e a taxa de desemprego); a Felicidade da classe (o valor e a faixa, com os 4 fatores no "Por quê?"); o Banco, o dinheiro do jogador e o da classe (a média por Civil e quantos dias da cesta ela paga); o imposto de ontem; o estabilizador pago na estação e o teto; os 2 multiplicadores do salário |
| **Construções** | Uma linha por construção: nome, dono, nível, empregados / vagas, a receita atual e o progresso (%), o motivo da parada ou da folga, a manutenção (em dia ou atrasada), a obra em andamento. Clicar numa linha abre o painel da construção |
| **Inventário** | Uma linha por item e dono: quantidade, estoque-alvo, preço base, preço local (com ↑ ou ↓ desde ontem), consumo médio dos 5 dias |
| **Mercado** | Comprar e vender na loja da cidade (parte 11 da [[Ficha Logica/Economia\|Economia]]): o item, a quantidade, o preço de compra (110%) e o de venda (90%), e os botões "Comprar" e "Vender" |
| **Diário** | As notificações guardadas (regra 10.3) |

### 10.6 O painel da construção (à direita, ao selecionar)

Clicar com o botão esquerdo dentro de um lote seleciona o que está nele (o clique na janela do ImGui não chega ao mapa). Esc ou um clique fora de um lote fecha o painel; o Esc com o painel fechado continua fechando o jogo.

**Toda construção:** o nome, o dono, o nível, a receita e o progresso, o estoque que ela guarda, os empregados (nome e Função) e a manutenção.

**Construção do jogador**, também:

- **A fila de ordens** (regra 4.4 da [[Ficha Logica/Producao|Produção]]): cada linha com o tipo, o item e o N; os botões ↑ e ↓ (a prioridade) e ×; e uma linha nova com o tipo (Fazer N, Manter estoque, Repetir sempre), o item (só os das receitas que a construção pode fazer no nível dela) e o N (de 1 a 999).
- **O nível de salário:** 3 botões, Baixo, Normal e Alto.
- **Os empregados**, cada um com "Demitir", e a lista dos desempregados, cada um com "Contratar" (enquanto houver vaga).
- **O preço** de cada produto: de −30% a +30%, de 5 em 5 pontos, mostrando o preço que sai.
- **"Subir para o nível N"**, com o custo e o tempo (desligado se faltar dinheiro, se a obra passar do nível que o Engenheiro permite ou se já houver uma obra nela).

**Lote livre:** a lista dos tipos que cabem no porte dele, cada um com o custo, o tempo e as vagas, e o botão "Encomendar obra".

### 10.7 O "Por quê?"

Passar o mouse sobre o dinheiro, um preço ou a Felicidade abre um quadro com as parcelas: o dinheiro (as entradas e saídas de ontem por tipo: salários, vendas, imposto, obras), o preço (o base, o fator do estoque e o passo de hoje) e a Felicidade (os 4 fatores). Fixar o quadro fica para depois.

### 10.8 Os pedidos do jogador

O ImGui **só desenha e lê os cliques**. Cada decisão vira um **pedido**, guardado numa fila e aplicado **no começo do próximo tick**, antes de qualquer regra. Assim a simulação continua determinística, e os testes do `Codigo/` fazem os mesmos pedidos sem janela.

| Pedido | O que leva |
|---|---|
| Trocar a fila de ordens | A construção e a fila nova inteira |
| Fixar o preço | A construção, o item e a porcentagem |
| Trocar o nível de salário | A construção e o nível |
| Contratar / Demitir | A construção e o NPC |
| Encomendar uma obra | O lote (ou a construção, para subir de nível) e o tipo |
| Comprar / Vender na loja da cidade | O item e a quantidade |

1. Confira o pedido de novo no momento de aplicar (o estado pode ter mudado no tick anterior). Um pedido que não vale mais é descartado com uma notificação ("Sem dinheiro para a obra").
2. Aplique na ordem em que chegaram.
3. A velocidade e a pausa **não** são pedidos: continuam como no marco 1.

### 10.9 Contas

| Conta | Como | Exemplo |
|---|---|---|
| Dinheiro na tela | Décimos ÷ 10, com vírgula e ponto de milhar | 39500 décimos → "3.950,0" |
| Preço do jogador | Parte 11 da [[Ficha Logica/Economia\|Economia]] | Local 5,1, +20% → 6,1 |
| Lote clicado | Pixel da tela → tile do mundo (regra 5 desta ficha) → o lote que contém o tile | Tile (43, 31) → lote 11 |

### 10.10 Nomes no código

| Nome | O que é |
|---|---|
| `PedidoDoJogador` | Um pedido (um `std::variant` com um tipo por linha da tabela 10.8) |
| `FilaDePedidosDoJogador` | A fila, aplicada por `aplicarOsPedidosDoJogador` no começo do tick |
| `TextoDoDinheiro` | Décimos → "3.950,0" (no `Codigo/`, testado) |
| `NotificacoesDaVila` | As 5 da tela e as 300 do Diário (no `Codigo/`) |
| `AlertasDaVila` | Os alertas da regra 10.4 (no `Codigo/`) |
| `InterfaceDoJogador` | No `Jogo/`: desenha a barra, os alertas, a janela de gestão e o painel com o ImGui |
| `loteNoTile` | O lote que contém um tile, ou nenhum |

### 10.11 Testes

| Situação | Resultado esperado |
|---|---|
| 39500 décimos; 5 décimos; 0 | "3.950,0"; "0,5"; "0,0" |
| Um pedido "comprar 10 Blocos" sem dinheiro | Descartado, com a notificação; nada muda |
| A mesma semente, com e sem uma fila de pedidos vazia | O mesmo relatório |
| A mesma semente e os mesmos pedidos, nos mesmos ticks, duas vezes | O mesmo relatório |
| Clique no tile (43, 31) | O lote 11 (a Carpintaria) |
| Clique fora de todos os lotes | Nenhum; o painel fecha |
| 6 notificações seguidas | 5 na tela (a mais velha sai), 6 no Diário |
| O Pão abaixo de 2 dias do previsto | O alerta aparece; volta acima, some |

No `Jogo/`, à mão: abrir e fechar a gestão com Tab; cada aba mostra os números do F4; trocar a fila de ordens da Carpintaria e ver a receita mudar no próximo ciclo; o clique numa janela não move a câmera nem seleciona o lote embaixo.
