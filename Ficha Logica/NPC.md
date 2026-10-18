---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 4** (NPC andando). Cobre só **a posição, o caminho, o movimento e o desenho do boneco**. A rotina e a Utilidade entram nesta mesma ficha antes do marco 5. Os pontos marcados com FL4 estão na **rodada FL4** ([[Retomada]], seção 4), ainda sem resposta: a ficha já usa as sugestões, marcadas como [Proposta].

# Ficha Lógica: NPC (posição e movimento)

## 1. O que faz

Um NPC **existe num tile do mapa**, recebe um **destino**, pede um **caminho** à busca de caminho e **anda tile a tile** até lá, na velocidade de andar. O `Jogo/` desenha o boneco no lugar certo, virado para o lado em que anda, com a animação de andar.

No marco 4 há **um NPC** (mais os criados pela tecla de teste, FL4-6), e quem dá o destino é o jogador, com o botão direito do mouse. No marco 5, quem dá o destino passa a ser a rotina.

Documentos do sistema: [[Sistema de NPC/Descrição|Sistema de NPC]] (atributos 12, "Posição atual", 21, "Velocidade", e 27, "Destino"), [[Sistema de IA/Descrição|IA]] e a regra 4.10 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]. A busca de caminho está na [[Ficha Logica/Mapa|ficha do Mapa]], regra 4.4.

**A divisão das pastas:**

- **`Codigo/Npc/`:** a posição, o caminho e o movimento. São regras do jogo: o teste sem tela do marco 7 também anda os NPCs (para saber quem chegou ao trabalho);
- **`Jogo/`:** o desenho do boneco, a escolha do quadro da animação e o clique do mouse.

**A biblioteca EnTT entra neste marco** (FT2-2: marcos 4 e 5). Ela guarda as **entidades** (cada NPC é um número) e os **componentes** (as fichas de dados presas a cada NPC), como manda a parte 1 da Fatia Vertical. É baixada pelo CMake, em versão fixa, como as outras.

**O NPC anda pelos ticks, não pelo tempo real.** Com o jogo pausado, ele para; a 4x, anda 4 vezes mais rápido. É o contrário da câmera ([[Ficha Logica/Interface|ficha da Interface]]), que anda pelo tempo real.

## 2. Dados

**A unidade da posição: o milésimo de tile.** Dentro de um tile, a posição é um inteiro de 0 a 1.000 (1 tile = 1.000 milésimos = 16 pixels). Por que inteiro, e não número com vírgula: a mesma partida tem que dar sempre o mesmo resultado ("mesma semente, mesma partida", [[Arquitetura/Descrição|Arquitetura]]), e somas com vírgula acumulam erros pequenos que mudam de um computador para outro.

### Definições (lidas dos dados, não mudam durante o jogo)

No `data/regras.json`:

| Campo no arquivo | Nome no código | Unidade | Valor |
|---|---|---|---|
| `velocidade_de_andar_em_tiles_por_hora` | `velocidadeDeAndarEmMilesimosPorTick` (convertida ao carregar) | tiles por hora de jogo | 128 [Proposta] (FL4-1), ou seja, 160 milésimos por tick |
| `caminhos_calculados_por_tick` | `caminhosCalculadosPorTick` | pedidos | 4 [Proposta] (FT1-3: "cálculo distribuído entre os ticks") |

**O boneco** (FL4-4) [Proposta], num arquivo novo, o `data/personagens/boneco_provisorio.json`:

| Campo no arquivo | O que é | Valor da arte provisória |
|---|---|---|
| `tamanho_do_quadro_em_pixels` | O tamanho de cada quadro na folha | 64 |
| `pes_no_quadro` | O ponto, dentro do quadro, que fica sobre a posição do NPC | (32, 43) |
| `linha_parado` | A primeira linha da animação parado (baixo, cima, direita, esquerda, uma linha cada) | 0, só a coluna 0 |
| `linha_andando` | A primeira linha da animação de andar, na mesma ordem | 4, colunas 0 a 5 |
| `quadros_de_andar` | Quantos quadros tem o andar | 6 |
| `distancia_por_quadro_de_andar_em_milesimos` | Quanto o NPC anda até trocar de quadro | 400 |
| `camadas` | As imagens, de baixo para cima (corpo, roupa, cabelo) | 3 imagens da pasta `Assents Temporarios/` |
| `imagem_de_reserva` | A folha usada quando falta alguma camada | `imagens/boneco_de_teste.png` |

**A arte provisória** (Mana Seed, versão grátis, fora do Git; ver a [[Retomada]]): cada folha tem 512 × 512 pixels, com 8 × 8 quadros de 64 × 64. O boneco ocupa cerca de **14 × 29 pixels** dentro do quadro, com os pés na linha 43. Ou seja, ele é quase **1 tile de largura e 2 de altura**, e não o boneco de 16 × 24 da Arquitetura. Por isso o tamanho e os pés ficam no arquivo, não no código: a arte própria, de 16 × 24, só troca os números (FL4-4).

### Estado (muda durante o jogo): os componentes

| Componente | Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|---|
| `PosicaoDoNpc` | `tileAtual` | 2 inteiros | (10, 3) | Sim |
| | `proximoTile` | 2 inteiros | (11, 3); igual ao `tileAtual` quando está parado | Sim |
| | `progressoAteOProximoTileEmMilesimos` | inteiro | 480 (quase na metade do passo) | Sim |
| | `direcaoDoOlhar` | Baixo, Cima, Direita ou Esquerda | Direita | Sim |
| | `distanciaNoCicloDeAndarEmMilesimos` | inteiro, de 0 a 2.399 | 1.000 (quadro 2 do andar) | Não: é só a animação |
| | `tileAnteriorParaDesenho` e `progressoAnteriorParaDesenho` | como acima | a posição no tick anterior (FL4-5) | Não |
| `CaminhoDoNpc` | `tilesQueFaltam` | lista de tiles | (11, 3), (12, 3) | **Não**: é recalculado ao carregar (Fatia Vertical, parte 5) |
| | `tileDeDestino` | 2 inteiros | (12, 3) | Sim |
| `PedidoDeCaminho` | `tileDeDestino` e `tickDoPedido` | 2 inteiros e o tick | (12, 3), tick 4.810 | Sim (vira um pedido novo ao carregar) |

- Um NPC **parado** tem só a `PosicaoDoNpc`. **Andando**, tem também o `CaminhoDoNpc`. **Esperando o cálculo**, tem o `PedidoDeCaminho`.
- **O passo:** o NPC está sempre no `tileAtual` ou a caminho do `proximoTile`, que é vizinho dele (um dos 8 em volta).
- **O comprimento do passo:** 1.000 milésimos no passo reto e **1.414** no diagonal (a diagonal de um quadrado de lado 1 mede 1,414).

## 3. Quando roda

**A cada tick** (dentro do tick, nesta ordem):

1. **Atender os pedidos de caminho** (regra 4.3): no máximo 4 por tick, os mais antigos primeiro.
2. **Andar os NPCs** que têm caminho (regra 4.4).

**A cada quadro da tela** (no `Jogo/`):

1. O clique do botão direito vira um pedido de caminho (regra 4.2).
2. Os NPCs são desenhados entre as camadas de baixo e a `acima_do_personagem` (regra 4.6), como já previa a regra 4.3 do Mapa.

**Ao abrir o jogo:** o NPC do marco 4 nasce (regra 4.1).

Pausado, nenhum tick roda: nada anda e nenhum caminho é calculado. A câmera continua andando.

## 4. Regras passo a passo

### 4.1 Criar o NPC (ao abrir o jogo, no marco 4)

1. Comece pelo tile do centro do mapa: (largura ÷ 2, altura ÷ 2). Na vila, **(32, 32)**.
2. Se ele dá para andar, o NPC nasce nele. Se não, procure em quadrados cada vez maiores em volta (distância 1, depois 2...), linha por linha, de cima para baixo e da esquerda para a direita, e use o primeiro tile livre.
3. O NPC nasce parado, olhando para baixo.

**A tecla de teste N** [Proposta] (FL4-6) cria mais um NPC, até 50, pela mesma regra, começando do tile livre seguinte ao último usado. Serve para ver vários andando juntos e passando um pelo outro.

### 4.2 Dar um destino (botão direito do mouse)

1. Converta a posição do mouse na tela em tile (parte 5). Se cair fora do mapa, ignore o clique.
2. Para **cada NPC** [Proposta] (FL4-6), crie ou troque o `PedidoDeCaminho` dele, com esse tile e o tick atual. Se ele já tinha um pedido esperando, o novo substitui o antigo e vai para o fim da fila.
3. O caminho antigo continua valendo até o novo ser calculado, para o NPC não parar à toa.

O botão direito "dá ordem", como manda o padrão de RTS da [[Interface/Descrição|Interface]] ("botão esquerdo seleciona, botão direito dá ordem"). Selecionar um NPC fica para depois da fatia; por isso, no marco 4, a ordem vale para todos.

### 4.3 Atender os pedidos de caminho (a cada tick)

1. Pegue os pedidos pela ordem do `tickDoPedido` (no empate, pelo número da entidade), no máximo 4.
2. Para cada um, descubra de onde o caminho sai:
   - se o NPC está parado, do `tileAtual`;
   - se está no meio de um passo, do `proximoTile`. Ele termina o passo atual e só depois segue o caminho novo, sem voltar para trás.
3. Chame a busca de caminho (regra 4.4 do Mapa), do tile de saída ao destino.
4. Conforme o resultado:
   - **chega no destino:** troque o `CaminhoDoNpc` pelo novo;
   - **não chega** (destino bloqueado, como uma árvore, ou sem passagem): o caminho vai até o tile mais perto do destino a que dá para chegar [Proposta] (FL4-3), e o console mostra "o destino (50, 50) não tem caminho; o NPC vai até (56, 50)";
   - **o destino está fora do mapa:** não há caminho; o NPC continua o que fazia;
   - **o caminho está vazio** (já está lá): tire o `CaminhoDoNpc`.
5. Apague o pedido.

Os pedidos que sobram ficam para o tick seguinte. Com 4 por tick e 20 ticks por segundo, são até 80 caminhos por segundo a 1x: os 15 NPCs da fatia nunca esperam mais de 1 tick.

### 4.4 Andar (a cada tick)

Para cada NPC com `CaminhoDoNpc`:

1. **Se está parado** (o `proximoTile` é o próprio `tileAtual`), comece um passo:
   1. pegue o primeiro tile de `tilesQueFaltam`. Se não houver, tire o `CaminhoDoNpc`: ele chegou;
   2. confira se o tile ainda dá para andar e, na diagonal, se a quina está livre (regra 4.4 do Mapa, passo 5). Se não der (uma obra nova no marco 6), crie um `PedidoDeCaminho` para o mesmo destino e espere parado;
   3. guarde-o como `proximoTile` e tire-o da lista;
   4. vire o NPC para o lado do passo (parte 5).
2. **Some a velocidade** (160) ao `progressoAteOProximoTileEmMilesimos` e à `distanciaNoCicloDeAndarEmMilesimos` (esta volta a 0 quando passa de 2.399).
3. **Se o progresso chegou ao comprimento do passo** (1.000 reto, 1.414 diagonal):
   1. o `proximoTile` vira o `tileAtual`;
   2. a **sobra** é o progresso menos o comprimento;
   3. se ainda há tiles no caminho, comece o passo seguinte (passo 1) já com a sobra no progresso, para a velocidade não dar "soluços" a cada tile;
   4. se não há, zere o progresso e tire o `CaminhoDoNpc`.

Os NPCs **não colidem** entre si (FT1-3): dois podem ocupar o mesmo tile.

No começo do tick, **todos** os NPCs (também os parados) guardam a posição atual em `tileAnteriorParaDesenho` e `progressoAnteriorParaDesenho`, para a posição suave (FL4-5).

### 4.5 A posição em pixels do mundo

Os **pés** do NPC ficam no **centro do tile**:

1. fração do passo = progresso ÷ comprimento do passo (de 0 a 1);
2. x = (`tileAtual.x` + (`proximoTile.x` − `tileAtual.x`) × fração) × 16 + 8;
3. y, do mesmo jeito.

### 4.6 Desenhar os NPCs (no `Jogo/`, a cada quadro)

1. **Posição suave** [Proposta] (FL4-5): calcule a posição do tick anterior e a do tick atual (regra 4.5) e desenhe entre as duas, pela fração do tick que já passou. *Por quê:* a 1x, o NPC só se move 20 vezes por segundo, e a tela mostra 60 quadros; sem isso, ele anda aos pulinhos de 2,6 pixels do mundo (7,7 pixels da tela a 3x).
2. **Ordem:** desenhe primeiro os NPCs com os pés mais acima na tela (menor y). No empate, o de menor x, e depois o de menor número de entidade. Assim, quem está mais embaixo cobre quem está atrás.
3. **Quadro da animação:**
   - andando: linha = `linha_andando` + direção (baixo 0, cima 1, direita 2, esquerda 3); coluna = `distanciaNoCicloDeAndarEmMilesimos` ÷ 400;
   - parado: linha = `linha_parado` + direção; coluna 0.
4. **Recorte na folha:** x = coluna × 64; y = linha × 64; tamanho 64 × 64.
5. **Onde desenhar:** o canto de cima à esquerda do quadro fica nos pés menos `pes_no_quadro` (32, 43). Depois, a mesma conta do mapa: (posição − câmera) × zoom, com o pixel inteiro da [[Ficha Logica/Interface|Interface]] (regra 4.6 de lá).
6. Desenhe as camadas na ordem do arquivo (corpo, roupa, cabelo), no mesmo lugar.
7. **Na depuração** [Proposta] (FL4-6): a tecla **F2** pinta o caminho de cada NPC em azul translúcido, como o F1 pinta a colisão.

### 4.7 Carregar o boneco (no `Jogo/`, ao abrir)

1. Leia o `boneco_provisorio.json`. Se faltar o arquivo ou um campo, é erro, como nos outros dados.
2. Tente carregar cada imagem das `camadas`. Se **alguma** faltar (por exemplo, num computador que baixou o repositório sem a pasta `Assents Temporarios/`), mostre um **aviso**, não um erro, e use só a `imagem_de_reserva`.
3. Confira que cada folha tem largura e altura múltiplas de 64.

A `imagem_de_reserva` é feita pelo script da arte de teste, no mesmo formato de 8 × 8 quadros de 64 × 64: um boneco de cores lisas, com as pernas mudando de lugar nos 6 quadros de andar. Ela entra no Git; a arte do Mana Seed não.

## 5. Contas

**A velocidade de andar** [Proposta] (FL4-1):

- 128 tiles por hora de jogo ÷ 800 ticks por hora = 0,16 tile por tick = **160 milésimos por tick**;
- a 1x (20 ticks por segundo): 3,2 tiles por segundo, ou 154 pixels da tela por segundo a 3x;
- *por que 128:* a [[Sistema de Viagem/Descrição|Viagem]] diz que atravessar um Mapa Local a pé leva cerca de 1 hora, e o Mapa Local do jogo completo tem 128 tiles.

**Quanto tempo leva:**

| Trajeto | Conta | Ticks | Tempo real a 1x | Tempo de jogo |
|---|---|---|---|---|
| 1 tile reto | 1.000 ÷ 160 = 6,25 | 7 | 0,35 s | 32 s |
| 1 tile na diagonal | 1.414 ÷ 160 = 8,84 | 9 | 0,45 s | 40 s |
| 20 tiles retos (uma casa até o trabalho) | 20.000 ÷ 160 | 125 | 6,25 s | 9 min |
| Atravessar a vila (64 tiles) | 64.000 ÷ 160 | 400 | 20 s | 30 min |

Por causa da sobra (regra 4.4, passo 3), 2 tiles retos levam 13 ticks (2.000 ÷ 160 = 12,5), e não 14.

**De que lado o NPC olha:**

- passo para a direita ou para a esquerda: Direita ou Esquerda;
- passo para cima ou para baixo: Cima ou Baixo;
- passo na diagonal: vale o lado (Direita ou Esquerda), porque a arte tem só 4 direções;
- parado: continua olhando para onde olhava.

**A animação de andar:** um quadro novo a cada 400 milésimos andados. A 1x, isso dá 160 × 20 ÷ 400 = 8 quadros por segundo, ou 125 milissegundos por quadro, perto dos 135 que o autor do Mana Seed recomenda. Os 6 quadros cobrem 2,4 tiles. Como a troca vem da **distância**, e não do relógio, os pés acompanham o chão em qualquer velocidade.

**A posição em pixels**, com o tile de 16:

- passo reto de (10, 3) para (11, 3), progresso 480: fração 0,48; x = (10 + 0,48) × 16 + 8 = **175,68**; y = 3 × 16 + 8 = **56**;
- passo diagonal de (10, 3) para (11, 4), progresso 707: fração 0,5; pés em **(176, 64)**;
- o quadro do Mana Seed, com os pés em (32, 43), fica com o canto em (175,68 − 32; 56 − 43) = (143,68; 13).

**O mouse na tela vira um tile:**

- tile x = arredondar para baixo ((mouse x ÷ zoom + câmera x) ÷ 16), e o mesmo para y;
- *exemplo:* câmera em (0, 0), zoom 3x, mouse em (480, 150): (160 ÷ 16; 50 ÷ 16) = **(10, 3)**.

**A posição suave** [Proposta] (FL4-5): com o relógio a meio caminho do próximo tick (25 milissegundos guardados de 50), o desenho fica na metade entre a posição do tick anterior e a do atual. A imagem fica no máximo 1 tick (50 milissegundos) atrasada, o que não se percebe.

## 6. Conversa com outros sistemas

| Sistema | O que o NPC lê | O que o NPC entrega |
|---|---|---|
| Mapa (marco 1) | "Dá para andar aqui?" e o tamanho do mapa | — |
| Busca de caminho (marco 4, na ficha do Mapa) | O caminho e se ele chega ao destino | O tile de saída e o de destino |
| Tempo (marco 1) | Os ticks; a fração do tick para a posição suave (FL4-5) | — |
| Interface e câmera (marco 2) | A câmera e o zoom, para desenhar e para converter o mouse em tile | — |
| IA e rotina (marco 5) | — | Quem chegou; a IA passa a dar os destinos (a casa, o trabalho, o Mercado) |
| Construção (marco 6) | Os tiles que uma obra nova bloqueia | — |
| Save (marco 9) | — | Posição, direção e destino; nunca o caminho |

## 7. Nomes no código

Em `Codigo/Npc/` (sem SDL nem OpenGL):

| Nome | O que é |
|---|---|
| `PosicaoDoNpc`, `CaminhoDoNpc`, `PedidoDeCaminho` | Os componentes da parte 2 |
| `DirecaoDoOlhar` | `Baixo`, `Cima`, `Direita`, `Esquerda` (nessa ordem, a das linhas da folha) |
| `RegrasDoMovimento` | `velocidadeDeAndarEmMilesimosPorTick` e `caminhosCalculadosPorTick`, lidos do `regras.json` |
| `entt::registry registroDasEntidades` | Onde a EnTT guarda os NPCs e os componentes |
| `criarNpcNoTile(registroDasEntidades, tile)` | Cria a entidade com a `PosicaoDoNpc` |
| `acharTileLivreMaisPertoDe(mapa, tile)` | A regra 4.1, passos 1 e 2 |
| `pedirCaminhoParaONpc(registroDasEntidades, npc, tileDeDestino, tickAtual)` | A regra 4.2, passo 2 |
| `atenderPedidosDeCaminho(registroDasEntidades, mapa, buscadorDeCaminho, regras)` | A regra 4.3 |
| `andarOsNpcsUmTick(registroDasEntidades, mapa, regras)` | A regra 4.4 |
| `descobrirDirecaoDoPasso(passoX, passoY)` | O lado para onde o NPC olha |
| `comprimentoDoPassoEmMilesimos(tileDeOrigem, tileDeDestino)` | 1.000 ou 1.414 |
| `calcularPesEmPixelsDoMundo(posicao, tamanhoDoTile)` | A regra 4.5 |

Em `Jogo/`:

| Nome | O que é |
|---|---|
| `DefinicaoDoBoneco` | Os campos do `boneco_provisorio.json` |
| `escolherQuadroDoBoneco(posicao, estaAndando, definicao)` | A regra 4.6, passo 3. Devolve a coluna e a linha. Fica na biblioteca `interface_do_jogo`, para ser testada |
| `DesenhistaDosPersonagens` | Carrega as folhas (`preparar`) e desenha os NPCs (`desenharNpcs`) |
| `CameraDoMapa::converterPontoDaTelaEmTile(x, y)` | O mouse vira tile (parte 5) |
| `RelogioDoMundo::fracaoJaPassadaDoProximoTick()` | De 0 a 1, para a posição suave (FL4-5) |
| `AcaoDoJogador::CriarNpcDeTeste` e `AcaoDoJogador::MostrarCaminhosParaDepuracao` | As teclas N e F2 na tabela de ações (FL4-6) |

## 8. Testes

No `Codigo/` (com o mapa de teste de 8 × 8 e o da vila):

| Entrada | Resultado esperado |
|---|---|
| Velocidade de 128 tiles por hora | 160 milésimos por tick |
| NPC parado em (10, 3), caminho até (12, 3) | Depois de 7 ticks: `tileAtual` (11, 3), progresso 120. Depois de 13: `tileAtual` (12, 3), parado e sem `CaminhoDoNpc` |
| NPC em (10, 3), caminho até (11, 4) (diagonal) | Chega em 9 ticks; olhando para a Direita |
| Caminho para cima | Olhando para Cima |
| Passo reto de (10, 3) para (11, 3), progresso 480 | Pés em (175,68; 56) |
| Velocidade do jogo 0 (pausa) | Nenhum tick: o NPC não sai do lugar |
| O próximo tile do caminho fica bloqueado | O NPC para e cria um `PedidoDeCaminho` para o mesmo destino |
| Novo destino no meio de um passo de (10, 3) para (11, 3) | O caminho novo sai de (11, 3) |
| 6 NPCs pedem caminho no mesmo tick, limite 4 | 4 atendidos nesse tick, 2 no seguinte |
| Destino (5, 6) na pedra do mapa de teste, saindo de (0, 0) | Não chega; o caminho termina em (5, 5) |
| Criar o NPC na vila | Nasce em (32, 32) |
| Centro bloqueado no mapa de teste (a pedra posta no centro, (4, 4)) | Nasce em (3, 3), o primeiro tile livre da volta de distância 1 |
| `distanciaNoCicloDeAndarEmMilesimos` em 2.300, mais 160 | 60 (voltou ao começo) |

No `Jogo/` (programa `testes_do_jogo`):

| Entrada | Resultado esperado |
|---|---|
| Parado, olhando para Baixo | Quadro coluna 0, linha 0 |
| Andando para a Direita, distância 1.000 | Coluna 2, linha 6 |
| Andando para a Esquerda, distância 2.399 | Coluna 5, linha 7 |
| Mouse em (480, 150), câmera (0, 0), zoom 3x | Tile (10, 3) |
| Mouse fora do mapa | Nenhum tile |
| Fração do tick 0,5 entre x = 100 e x = 102,56 | Desenho em x = 101,28 |

## 9. Fora da fatia (e fora do marco 4)

- A rotina, a Utilidade e os destinos automáticos (marco 5, nesta ficha).
- O personagem do jogador andando (Controlado), selecionar um NPC e dar ordem só a ele.
- A Agilidade mudando a velocidade ([[Sistema de NPC/Descrição|NPC]]), correr e a marcha da Viagem.
- Os NPCs desviando uns dos outros.
- As outras animações (trabalhar, atacar, cair) e o boneco em camadas de armadura e arma.
- A arte própria em 3 direções com a esquerda espelhada ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]).
- O movimento em LOD 1 e 2 (sem andar tile a tile) e a passagem entre mapas pela borda.
