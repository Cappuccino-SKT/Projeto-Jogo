---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 2** (câmera e entrada). Cobre só **a câmera, o zoom e as teclas**. O HUD e os painéis entram nesta mesma ficha antes do marco 8. As decisões da rodada **FL2** (2026-10-18, todas "seguir sugestão") já estão aplicadas, e o código do marco 2 as segue.

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
