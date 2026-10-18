---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-17 para o **marco 1** (janela, loop, relógio fixo e mapa de tiles). Cobre os **tiles e as camadas**. As decisões da rodada **FL1** (2026-10-17, todas "seguir sugestão") já estão aplicadas.
>
> **Busca de caminho** (2026-10-18, para o **marco 4**): a regra 4.4, com os dados, as contas, os nomes e os testes dela. Os pontos marcados com FL4 estão na **rodada FL4** ([[Retomada]], seção 4), ainda sem resposta, e a ficha já usa as sugestões, marcadas como [Proposta]. Quem usa a busca é a [[Ficha Logica/NPC|ficha do NPC]].

# Ficha Lógica: Mapa

## 1. O que faz

Lê o **Mapa Local** desenhado no Tiled, guarda as camadas de tiles, monta a **passagem** (onde dá para andar) e a lista de **lotes** e **áreas de recurso**, e diz ao `Jogo/` o que desenhar e em que ordem. Na fatia há um mapa só, de **64 × 64 tiles** de 16 px (FT1-4).

Documentos do sistema: [[Sistema de Mapa/Descrição|Sistema de Mapa]] e o padrão de camadas do [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]. Mapa geral: [[Ficha Logica/Fatia Vertical|Fatia Vertical]], parte 1.

**A divisão das pastas:** ler o arquivo, guardar os números e responder "dá para andar aqui?" fica em `Codigo/Mapa/`, porque o teste sem tela também precisa disso. Carregar a imagem e desenhar fica em `Jogo/`.

## 2. Dados

### O arquivo do mapa (FL1-1 e FL1-5)

- **Um arquivo:** `Jogo/data/mapas/vila.tmj`, salvo pelo Tiled direto no formato JSON (o `.tmj` é o JSON do Tiled), sem passo de exportar. A `Jogo/assets/` guarda o projeto do Tiled e as imagens-fonte.
- **Quem lê:** a biblioteca nlohmann/json, que entra já no marco 1 por causa do mapa (FL1-1).
- **A arte do marco 1 (FL1-6):** um conjunto de tiles de teste, de quadrados de cor lisa (grama verde, água azul, terra marrom, parede cinza, árvore verde-escura, colisão vermelha), e um `vila.tmj` de teste com os 11 lotes e as 3 áreas de recurso, feitos pelo assistente. Depois, numa tarefa à parte, o autor escolhe um pacote grátis de licença CC0 e redesenha o mapa.
- **Configurações no Tiled**, para o arquivo ficar simples de ler:
  - orientação "Ortogonal", tile de 16 × 16 e mapa de tamanho fixo (não "infinito");
  - formato das camadas "CSV" (números simples), e não "Base64" (texto comprimido);
  - conjunto de tiles (tileset) **embutido no mapa**, com a imagem PNG em `Jogo/data/imagens/`, sem margem e sem espaço entre os tiles;
  - nenhum tile espelhado ou girado.
- **Nomes das camadas, sem acento** (o jogo procura pelo nome):

| Ordem | Nome no Tiled | Tipo | O que tem | O jogo desenha? |
|---|---|---|---|---|
| 1 | `chao` | Tiles | Grama, terra, água | Sim, por baixo de tudo |
| 2 | `detalhes_do_chao` | Tiles | Flores, caminhos | Sim |
| 3 | `objetos_baixos` | Tiles | Pedras, cercas, lavouras | Sim |
| 4 | `construcoes_e_paredes` | Tiles | Paredes e muros fixos | Sim |
| 5 | `acima_do_personagem` | Tiles | Copas de árvore, telhados | Sim, por cima dos personagens |
| 6 | `colisao` | Tiles | Qualquer tile marca "não se anda" | Não (só na depuração) |
| 7 | `elevacao` | Tiles, opcional | Colinas, para o Mapa de Guerra | Não |
| — | `lotes` | Objetos | Retângulos dos lotes | Não |
| — | `recursos` | Objetos | Áreas de campo de Grão, poço, bosque | Não |

- **As construções não são pintadas no Tiled.** Elas aparecem nos lotes durante o jogo (a vila inicial e as obras), então o jogo as desenha por cima, a partir do marco 6. A camada 4 é só para o que nunca muda, como um muro.

### Definições (lidas do arquivo, não mudam durante o jogo)

| Campo | Tipo | Unidade | Exemplo |
|---|---|---|---|
| `larguraEmTiles`, `alturaEmTiles` | inteiros | tiles | 64 e 64 |
| `tamanhoDoTileEmPixels` | inteiro | pixels | 16 |
| `camadasDeTiles` | lista de camadas | — | as 6 ou 7 camadas acima |
| Cada camada: `nomeDaCamada` | texto | — | `chao` |
| Cada camada: `numeroDoTilePorPosicao` | lista de 4.096 inteiros de 16 bits | número do tile | 0 = vazio; 35 = um tile de grama |
| `conjuntosDeTiles` | lista | — | um na fatia |
| Cada conjunto: `primeiroNumeroDoConjunto` | inteiro | — | 1 |
| Cada conjunto: `caminhoDaImagem` | texto | — | `imagens/terreno.png` |
| Cada conjunto: `colunasNaImagem` e `quantidadeDeTiles` | inteiros | tiles | 16 e 256 |
| `lotesDeConstrucao` | lista de lotes | — | 8 pequenos e 3 médios (V3-8) |
| Cada lote: `identificadorDoLote` | inteiro | — | 4 |
| Cada lote: `tileDoCantoSuperiorEsquerdo` | 2 inteiros | tiles | (12, 20) |
| Cada lote: `porteDoLote` | Pequeno, Médio ou Grande | — | Pequeno = 4 × 4 tiles |
| Cada lote: `tipoDeConstrucaoPermitido` | texto | — | `qualquer` |
| `areasDeRecurso` | lista | — | 3 na fatia |
| Cada área: `tipoDeRecurso` (a propriedade `tipo_de_recurso` do objeto no Tiled) e o retângulo em tiles | texto e 4 inteiros | tiles | `campo_de_grao` em (4, 6), 10 × 8 |

- **Por que 16 bits por tile:** dá até 65.535 tiles diferentes, muito mais do que a fatia usa, e o mapa inteiro ocupa 64 × 64 × 7 × 2 bytes = **57 KB**.
- **O porte do lote** sai do tamanho do retângulo, não de um campo extra: 4 × 4 é pequeno, 6 × 6 é médio e 8 × 8 é grande (FT1-4).
- **O tipo permitido** vem de uma propriedade `tipo_permitido` no objeto do Tiled. Na fatia, todos os lotes usam `qualquer`. A propriedade só existe para quando houver lotes reservados, por exemplo para moradias.
- **A produtividade dos recursos** (normal ou destaque) não fica no Tiled. Ela vem dos dados, no marco 3, e no futuro do `mapas.json` ([[Sistema de Recursos/Descrição|Recursos]]).

### Estado (muda durante o jogo)

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `passagemPorTile` | lista de 4.096 sim/não | (5, 9) = não, por causa de uma árvore | Não: é recalculada da camada `colisao` e das construções |
| Cada lote: `idDaConstrucaoQueOcupa` | ID ou "nenhuma" | a Carpintaria do jogador | Sim, a partir do marco 6 |

No marco 1, a passagem vem só da camada `colisao`. A partir do marco 6, as construções nos lotes também bloqueiam os tiles delas.

### A memória de trabalho da busca de caminho (marco 4)

Não vai no save nem muda o mapa: é o rascunho de cada busca. É reservada **uma vez**, do tamanho do mapa, e reaproveitada em todas as buscas, para não criar memória nova a cada caminho ([[Arquitetura/Descrição|Arquitetura]], princípio 1).

| Campo | Tipo | O que guarda |
|---|---|---|
| `custoAteAquiPorTile` | lista de 4.096 inteiros | O menor custo já achado da saída até cada tile, em décimos de tile (o "g" do A\*) |
| `tileAnteriorPorTile` | lista de 4.096 índices | De onde se chegou a cada tile, para montar o caminho no fim |
| `numeroDaBuscaPorTile` | lista de 4.096 inteiros | Em qual busca o tile foi mexido pela última vez. Assim, não é preciso apagar as listas a cada busca: um número velho quer dizer "ainda não visto nesta busca" |
| `filaDeTilesAbertos` | fila com prioridade | Os tiles a examinar, o de menor custo estimado na frente |

## 3. Quando roda

- **Ao abrir o jogo:** a regra 4.1 lê e confere o mapa uma vez. Se houver erro, o jogo mostra a lista de erros e fecha, como no `--validar`.
- **A cada quadro da tela:** o `Jogo/` desenha as camadas visíveis (regra 4.3). Desenhar não muda nada no mapa.
- **Quando uma construção fica pronta ou é demolida** (marco 6): a passagem dos tiles do lote é atualizada (regra 4.2).
- **Quando um NPC pede um caminho** (marco 4): a regra 4.4 roda dentro do tick, no máximo 4 vezes por tick ([[Ficha Logica/NPC|ficha do NPC]], regra 4.3).
- **O relógio não mexe no mapa.** O mapa não roda nada por tick, hora ou dia.

## 4. Regras passo a passo

### 4.1 Carregar e conferir o mapa (ao abrir o jogo)

1. Leia o arquivo `vila.tmj`. Se ele não existir ou não for um JSON válido, anote o erro e pare.
2. **Confira o formato.** Cada item que falhar vira um erro com o motivo:
   - orientação ortogonal, mapa não infinito;
   - tile de 16 × 16;
   - largura e altura maiores que 0 (a fatia espera 64 × 64, e o tamanho vem do arquivo, não do código).
3. **Leia os conjuntos de tiles.** Para cada um, guarde o primeiro número, a imagem, as colunas e a quantidade. Erro se o conjunto estiver num arquivo separado (não embutido), ou se a imagem não for múltipla de 16 px.
4. **Leia as camadas de tiles.** Para cada camada:
   - se o nome não for um dos 7 da tabela, mostre um aviso e ignore a camada;
   - confira que ela tem largura × altura números (4.096 na fatia);
   - para cada número: se ele tiver os bits de espelhamento do Tiled (os 3 bits mais altos), anote o erro "tile espelhado em (x, y)"; se não pertencer a nenhum conjunto, anote o erro "tile desconhecido em (x, y)". Depois de 10 erros na mesma camada, anote só "e mais N tiles com erro", para a lista não ficar enorme quando a camada inteira estiver errada.
5. **Confira se as 6 camadas obrigatórias existem** (todas menos `elevacao`). Faltou uma: erro "falta a camada `colisao`".
6. **Monte a passagem:** cada tile da `colisao` com número 0 dá para andar; qualquer outro número, não.
7. **Leia os lotes** (camada de objetos `lotes`). Para cada retângulo:
   - converta de pixels para tiles (divida por 16). Erro se a posição ou o tamanho não forem múltiplos de 16;
   - descubra o porte pelo tamanho. Erro se não for 4 × 4, 6 × 6 ou 8 × 8 tiles;
   - erro se o lote sair do mapa, encostar em outro lote por cima ou tiver um tile bloqueado na `colisao` (FL1-5). Um lote precisa estar livre, porque a construção ainda vai nascer nele.
8. **Leia as áreas de recurso** (camada `recursos`). Converta para tiles como os lotes. Erro se o tipo não for conhecido. Na fatia, os tipos são `campo_de_grao`, `poco` e `bosque`. **A partir do marco 3 (FL3-6):** os tipos conhecidos vêm do `recursos.csv`, e a área ganha a propriedade opcional `produtividade` ([[Ficha Logica/Recursos|ficha dos Recursos]], regra 4.2).
9. Se a lista de erros estiver vazia, o mapa está pronto. Se não, devolva todos os erros de uma vez, não só o primeiro, para o autor corrigir tudo no Tiled numa passada.

### 4.2 Perguntar se dá para andar num tile

1. Se o tile está fora do mapa (x ou y menor que 0, ou maior ou igual ao tamanho), responda **não**.
2. Se não, responda o valor da `passagemPorTile` na posição (regra do índice, parte 5).

A busca de caminho do marco 4 vai fazer esta pergunta milhares de vezes, por isso ela é só uma leitura de lista, sem conta pesada.

### 4.3 Desenhar o mapa (no `Jogo/`, a cada quadro)

1. Descubra quais tiles aparecem na tela pela posição da câmera e pelo zoom (parte 5). Os outros não são desenhados (o "culling" da [[Arquitetura/Descrição|Arquitetura]]).
2. Desenhe, nesta ordem: `chao`, `detalhes_do_chao`, `objetos_baixos`, `construcoes_e_paredes`.
3. Depois, as construções, os NPCs e o jogador (marcos 4 a 6; no marco 1 não há nenhum).
4. Por último, `acima_do_personagem`, para a copa da árvore cobrir quem passa por baixo.
5. Pule os tiles com número 0 (vazios).
6. Todos os tiles do mesmo conjunto vão numa só chamada de desenho para a placa de vídeo, o "desenho em lote".
7. **Na depuração (FL1-3):** a tecla **F1** (provisória) pinta de vermelho translúcido os tiles bloqueados, para conferir a `colisao` sem abrir o Tiled.

**No marco 1**, a câmera fica parada no canto de cima à esquerda, com zoom 3x. A câmera que anda e o zoom que muda vêm no marco 2.

### 4.4 Buscar um caminho (A\*, marco 4)

A busca **A\*** (lê-se "A estrela") acha o caminho mais curto numa grade de **8 direções**, sem testar todos os caminhos: ela examina primeiro os tiles que parecem mais perto do destino (FT1-3). Os custos são inteiros, em **décimos de tile**: o passo reto custa **10** e o diagonal, **14** (o 1,4 da FT1-3).

1. **Casos rápidos:**
   - destino fora do mapa: não há caminho (devolva "não chega", com o caminho vazio);
   - saída igual ao destino: o caminho é vazio e chega.
2. **Prepare:** some 1 ao número da busca; marque a saída com custo 0 e ponha-a na fila.
3. **Repita enquanto a fila não estiver vazia:**
   1. tire da fila o tile com o menor **custo estimado** = custo até aqui + estimativa até o destino (parte 5). No empate, o de menor estimativa; depois, o de menor índice. A ordem fixa faz a mesma busca dar sempre o mesmo caminho;
   2. se ele já foi examinado nesta busca, pule-o;
   3. marque-o como examinado. Se a estimativa dele até o destino for a menor até agora, guarde-o como o **tile mais perto** (no empate, o de menor custo até aqui; depois, o de menor índice);
   4. se ele é o destino, pare: achou;
   5. olhe os **8 vizinhos**, sempre nesta ordem: cima, direita, baixo, esquerda, e depois as diagonais (cima-direita, baixo-direita, baixo-esquerda, cima-esquerda). Pule o vizinho que estiver fora do mapa ou não der para andar (regra 4.2). **Na diagonal**, pule também se **um dos dois tiles retos do lado** estiver bloqueado [Proposta] (FL4-2): o NPC não corta a quina de uma árvore ou de uma parede;
   6. custo novo = custo até aqui + 10 (reto) ou 14 (diagonal). Se o vizinho ainda não foi visto nesta busca, ou se o custo novo for menor que o dele, guarde o custo novo e de onde veio, e ponha-o na fila.
4. **Monte o caminho:**
   - se achou o destino, comece por ele;
   - se a fila acabou sem achar (o destino está bloqueado ou cercado), comece pelo **tile mais perto** [Proposta] (FL4-3);
   - volte de tile em tile pelo `tileAnteriorPorTile` até a saída, e inverta a lista. O caminho **não tem** o tile de saída e **termina** no destino (ou no tile mais perto).
5. **Devolva** o caminho, se ele chega no destino, o custo total e quantos tiles foram examinados (para medir).

*Por que a estimativa nunca passa do custo de verdade:* ela conta um mapa sem nenhum obstáculo. Com isso, o A\* sempre acha o caminho mais curto.

**O caso pior:** um destino sem caminho faz a busca examinar todos os tiles alcançáveis (na vila, 3.886 dos 4.096). Isso leva menos de 1 milissegundo, e o limite de 4 buscas por tick segura o pior caso.

## 5. Contas

**Posição na lista.** As camadas guardam os tiles em fila, linha por linha:

- índice = y × largura + x;
- *exemplo:* o tile (10, 3) de um mapa de 64 de largura fica em 3 × 64 + 10 = **202**.

**Do número do tile ao recorte na imagem:**

1. Ache o conjunto: o de maior primeiro número que ainda seja menor ou igual ao número do tile.
2. Número local = número do tile − primeiro número do conjunto.
3. Coluna = número local resto colunas; linha = número local ÷ colunas (divisão inteira).
4. Recorte na imagem: x = coluna × 16; y = linha × 16; tamanho 16 × 16.

*Exemplo:* conjunto `terreno` com primeiro número 1 e imagem de 256 px de largura (16 colunas). O tile 35 dá número local 34, coluna 34 resto 16 = **2** e linha 34 ÷ 16 = **2**. O recorte começa em (32, 32).

**Dos pixels do Tiled aos tiles:** um lote em (192, 320) pixels, com 64 × 64 pixels, fica no tile (12, 20) e tem 4 × 4 tiles, ou seja, é **pequeno**.

**Do tile à tela:**

- posição na tela = (tile × 16 − posição da câmera) × zoom;
- *exemplo:* com a câmera em (0, 0) e zoom 3x, o tile (10, 3) aparece em (480, 144) pixels da janela.

**A estimativa até o destino** (a "heurística" do A\*, marco 4), em décimos de tile:

- dx = a distância em x, dy = a distância em y (sem sinal);
- estimativa = 10 × (dx + dy) − 6 × o menor dos dois;
- *por quê:* anda-se na diagonal enquanto dá (14 cada) e reto no resto (10 cada). 14 = 10 + 10 − 6;
- *exemplo:* de (0, 0) a (5, 3): 10 × 8 − 6 × 3 = **62**, igual a 3 diagonais (42) + 2 retos (20).

**Um caminho de exemplo**, no mapa de teste de 8 × 8 com a pedra em (5, 6), de (0, 0) a (7, 7):

- sem a pedra, seriam 7 diagonais: 7 × 14 = 98;
- a diagonal de (5, 5) para (6, 6) passaria pela quina da pedra, e é proibida (FL4-2);
- o caminho fica (1, 1), (2, 2), (3, 3), (4, 4), (5, 5), (6, 5), (7, 6), (7, 7): 6 diagonais e 2 retos, custo **104**.

**Quantos tiles cabem na tela** (1600 × 900, zoom 3x, cada tile com 48 px):

- 1600 ÷ 48 = 33,3, ou seja, 34 colunas (a última aparece pela metade);
- 900 ÷ 48 = 18,75, ou seja, 19 linhas;
- com 1 tile de folga em cada lado: 36 × 21 = 756 tiles por camada, e 5 camadas desenhadas dão cerca de **3.800 tiles por quadro**. Em lote, isso é pouco para a placa de vídeo.

## 6. Conversa com outros sistemas

| Sistema | O que lê do mapa | O que entrega ao mapa |
|---|---|---|
| Jogo (desenho, marco 1) | Camadas, conjuntos de tiles, tamanho | A posição da câmera e o zoom |
| Interface e câmera (marco 2) | O tamanho do mapa, para a câmera não sair dele | — |
| Busca de caminho e Movimento (marco 4) | "Dá para andar aqui?" | — |
| Construção e População (marco 6) | Os lotes livres e o porte de cada um | Qual construção ocupa cada lote |
| Produção (marco 6) | As áreas de recurso (onde os Braçais trabalham) | — |
| Dados (marco 3) | — | A produtividade de cada recurso |
| Save (marco 9) | A ocupação dos lotes | — |

## 7. Nomes no código

Em `Codigo/Mapa/` (sem SDL nem OpenGL):

| Nome | O que é |
|---|---|
| `MapaLocal` | A estrutura com as definições e o estado da parte 2 |
| `CamadaDeTiles` | `nomeDaCamada` e `numeroDoTilePorPosicao` |
| `ConjuntoDeTiles` | `primeiroNumeroDoConjunto`, `caminhoDaImagem`, `colunasNaImagem`, `quantidadeDeTiles` |
| `LoteDeConstrucao` | `identificadorDoLote`, `tileDoCantoSuperiorEsquerdo`, `porteDoLote`, `tipoDeConstrucaoPermitido`, `idDaConstrucaoQueOcupa` |
| `PorteDoLote` | `Pequeno`, `Medio`, `Grande` |
| `AreaDeRecurso` | `tipoDeRecurso` e o retângulo em tiles |
| `carregarMapaDoArquivoDoTiled(caminhoDoArquivo)` | A regra 4.1. Devolve o mapa e a `listaDeErrosDoMapa` |
| `tileDaParaAndar(tileX, tileY)` | A regra 4.2 |
| `indiceDoTileNaLista(tileX, tileY)` | A conta do índice |
| `procurarCamadaPeloNome(nomeDaCamada)` | Acha uma camada, ou avisa que ela não existe |
| `procurarConjuntoDoTile(numeroDoTile)` | O passo 1 do recorte |
| `ResultadoDaLeituraDoMapa` | O mapa, a `listaDeErrosDoMapa` e a `listaDeAvisosDoMapa` |
| `carregarMapaDoTextoDoTiled(texto, pasta)` | A regra 4.1 a partir do texto do arquivo (usada pelos testes) |
| `calcularRecorteDoTileNaImagem(conjunto, numeroDoTile, tamanhoDoTile)` | Os passos 2 a 4 do recorte. Ficou no `Codigo/` (é só conta), para poder ser testada |
| `converterRetanguloDePixelsParaTiles(...)` e `descobrirPorteDoLote(...)` | As contas dos lotes e das áreas |
| `BuscadorDeCaminho` | A classe da busca (marco 4), com a memória de trabalho da parte 2 |
| `BuscadorDeCaminho::buscarCaminho(mapa, tileDeSaida, tileDeDestino)` | A regra 4.4 |
| `ResultadoDaBuscaDeCaminho` | `tilesDoCaminho`, `chegaNoDestino`, `custoTotalEmDecimosDeTile`, `tilesExaminados` |
| `estimarCustoAteODestino(tile, tileDeDestino)` | A estimativa da parte 5 |
| `passoEntreVizinhosEhPermitido(mapa, tileDeOrigem, tileVizinho)` | O passo 3.5 da regra 4.4 (o tile e a quina) |
| `custoDoPassoReto` e `custoDoPassoDiagonal` | 10 e 14 |

Em `Jogo/` (com OpenGL e stb_image):

| Nome | O que é |
|---|---|
| `TexturaDoConjuntoDeTiles` | A imagem do conjunto já na placa de vídeo |
| `CameraDoMapa` | A posição da câmera em pixels do mundo e o zoom |
| `DesenhistaDoMapa` | A classe que prepara o desenho (`preparar`) e desenha |
| `desenharCamadasAbaixoDosPersonagens(...)` e `desenharCamadasAcimaDosPersonagens(...)` | A regra 4.3 |
| `desenharTilesBloqueadosParaDepuracao(...)` | O passo 7 da regra 4.3 (tecla F1) |

## 8. Testes

Os testes usam **um mapa pequeno de teste**, de 8 × 8 tiles, em `Codigo/Testes/mapas/` (grama, uma pedra com colisão em (5, 6), um lote pequeno em (0, 0) e um poço). Os mapas com defeito são montados no próprio teste, estragando uma cópia dele (feito no código do marco 1: um arquivo só, em vez de um por defeito).

| Entrada | Resultado esperado |
|---|---|
| Mapa de teste correto, 8 × 8 | Carrega sem erros; 6 camadas; largura 8 |
| O mesmo mapa sem a camada `colisao` | Erro "falta a camada `colisao`" |
| Camada com 63 números num mapa 8 × 8 | Erro de tamanho na camada |
| Um tile espelhado em (2, 5) | Erro "tile espelhado em (2, 5)" |
| Um tile 20 num conjunto de 16 tiles | Erro "tile desconhecido" |
| Mapa com 3 erros diferentes | A lista traz os 3, não só o primeiro |
| Tile de colisão em (5, 6) | `tileDaParaAndar(5, 6)` = não; (5, 5) = sim |
| Perguntar (−1, 0) e (8, 0) num mapa 8 × 8 | Não, os dois |
| Índice de (10, 3) com largura 64 | 202 |
| Tile 35, conjunto com primeiro número 1 e 16 colunas | Recorte em (32, 32) |
| Lote de 64 × 64 px em (192, 320) | Tile (12, 20), porte Pequeno |
| Lote de 80 × 64 px | Erro de tamanho |
| Dois lotes que se cobrem | Erro de lotes sobrepostos |
| Lote sobre um tile de colisão | Erro |
| O `vila.tmj` da fatia | Sem erros; 64 × 64; 8 lotes pequenos e 3 médios; 3 áreas de recurso |

**Busca de caminho (marco 4)**, no mapa de teste de 8 × 8 (a pedra em (5, 6)) e na vila:

| Entrada | Resultado esperado |
|---|---|
| Estimativa de (0, 0) a (5, 3) | 62 |
| De (0, 0) a (0, 0) | Caminho vazio; chega |
| De (0, 0) a (3, 0) | (1, 0), (2, 0), (3, 0); custo 30 |
| De (0, 0) a (3, 3) | (1, 1), (2, 2), (3, 3); custo 42 |
| De (0, 0) a (7, 7) | Custo 104; nenhum passo diagonal corta a quina da pedra |
| De (4, 6) a (6, 6), com a pedra no meio | Contorna: (4, 5), (5, 5), (6, 5), (6, 6); custo 40 |
| De (0, 0) a (5, 6), a própria pedra | Não chega; o caminho termina em (5, 5), custo 70 |
| De (0, 0) a (8, 0), fora do mapa | Não chega; caminho vazio |
| A mesma busca duas vezes seguidas | O mesmo caminho (a memória reaproveitada não deixa sujeira) |
| Vila: de (32, 32) a (60, 50), do outro lado do lago | Chega; custo 370; nenhum tile do caminho é bloqueado |
| Vila: de (32, 32) a (50, 50), dentro do lago | Não chega; termina em (56, 50), a margem mais perto |

## 9. Fora da fatia

O documento do sistema tem, e esta versão ainda não faz:

- o Mapa Mundial, a malha de 48 mapas e a viagem entre mapas pela borda;
- os Mapas Internos (as portas) e o Mapa de Guerra (a camada `elevacao` é lida, mas não é usada);
- o LOD 1 e 2, porque a fatia fica sempre em LOD 0;
- as camadas de objetos `portas`, `bordas`, `aparicoes` e `pontos_de_interesse`;
- os estados de território, a cidade derivada e o `mapas.json`;
- o tamanho de 128 × 128 do jogo completo, que ainda espera o teste de desempenho;
- uma busca de caminho mais rápida para mapas grandes ou muitos NPCs (por regiões, ou um caminho só para muitos NPCs que vão ao mesmo lugar), se o Tracy mostrar que a A\* simples pesa.
