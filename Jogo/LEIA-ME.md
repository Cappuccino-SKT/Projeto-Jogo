# Jogo

Aqui fica **o programa de verdade**: abre a janela, desenha o mapa e os NPCs, lê o mouse e o teclado, mostra a interface. As regras não ficam aqui: o programa chama a pasta `Codigo/` para saber o que acontece.

Também ficam aqui:

- `data/`: os arquivos que o jogo lê (itens, receitas, regras, a vila inicial);
- `assets/`: a arte e os mapas (Pixelorama e Tiled).

## Como compilar e rodar

**Pelo terminal** (PowerShell ou Git Bash, na pasta do projeto):

1. Preparar a pasta `build/`. Só precisa na primeira vez, ou quando um `CMakeLists.txt` mudar. Na primeira vez, o CMake baixa as bibliotecas e demora uns 2 minutos:

   ```
   cmake --preset padrao
   ```

2. Compilar. Na primeira vez, a SDL3 também é compilada (cerca de 30 segundos); depois, só o que mudou:

   ```
   cmake --build --preset padrao
   ```

3. Rodar:

   ```
   .\build\Jogo\jogo.exe
   ```

4. Conferir os dados sem abrir a janela (marco 3). Mostra cada erro com o arquivo e a linha, o resumo e os preços base, e termina com o código 0 (tudo certo) ou 1 (há erro). Bom para rodar depois de mexer numa planilha:

   ```
   .\build\Jogo\jogo.exe --validar
   ```

5. Rodar os testes automáticos das regras (rodada FL1-4). Termina com "100% tests passed" se tudo passou:

   ```
   ctest --test-dir build
   ```

   Para ver cada conferência que falhou, rode o programa de testes direto: `.\build\Codigo\Testes\testes_do_codigo.exe`.

**Pelo VS Code**, com as extensões "C/C++" e "CMake Tools":

1. Abrir a pasta do projeto. O CMake Tools pergunta qual configuração usar: escolher **"Padrão (g++, Ninja, com depuração)"**.
2. **F7** compila.
3. **Shift+F5** roda. **Ctrl+F5** roda com o depurador (gdb), que para nas linhas marcadas com um ponto vermelho.

**Os arquivos da compilação:**

| Arquivo | O que faz |
|---|---|
| `CMakeLists.txt` (na raiz) | A receita principal: C++20, baixa as bibliotecas e junta as pastas |
| `CMakePresets.json` (na raiz) | A configuração "padrao": g++, Ninja, pasta `build/`, com depuração |
| `Jogo/CMakeLists.txt` | Diz de que arquivos o `jogo.exe` é feito e que bibliotecas ele usa |
| `Codigo/CMakeLists.txt` | A biblioteca `regras_do_jogo` (as regras, sem janela) e o programa de testes |
| `Jogo/externo/glad/` | O glad (OpenGL 3.3), gerado uma vez e guardado aqui (ver o LEIA-ME de lá) |
| `build/` | Tudo o que o compilador gera. Fica fora do Git e do Obsidian, e pode ser apagada: o passo 1 a recria |

## O que cada marco trouxe

### Marco 0: o ambiente e o "olá, janela" (2026-10-17, confirmado pelo autor)

**O que faz:** abre uma janela com o fundo verde-escuro. Fecha no X ou com a tecla **Esc**. **F11** liga e desliga a tela cheia (tecla provisória). O console mostra "Olá, janela!" ao abrir e "Janela fechada. Até a próxima!" ao fechar.

**O tamanho da janela:** se 1600 × 900 cabe na tela com a barra de título, a janela abre com esse tamanho. Se não cabe (uma tela de 1600 × 900, como a do autor, só tem 1600 × 860 livres acima da barra de tarefas), ela abre **maximizada**, e ao restaurar fica com 90% da área livre, em 16:9. *Por quê:* no primeiro teste, a janela de 1600 × 900 não cabia, e o Windows escondeu a barra de título, com o X, acima da tela.

**Os arquivos:**

- `Jogo/main.cpp`: o começo do programa. Liga a SDL, escolhe o tamanho da janela (`escolherTamanhoInicialDaJanela`), cria a janela e repete o **laço principal** até o jogador fechar. O laço lê os eventos (teclas, o X da janela) e pinta o fundo, uma vez por quadro;
- a **SDL3** (versão 3.4.18) é baixada pelo CMake e entra dentro do `jogo.exe`, então o programa não precisa de nenhuma DLL ao lado dele.

**O que testar:**

1. A janela abre verde-escura, com a barra de título "Projeto Jogo (marco 0)" à vista (maximizada, numa tela de 1600 × 900).
2. O botão de restaurar, ao lado do X, diminui a janela; depois, dá para mudar o tamanho arrastando a borda.
3. O X e a tecla Esc fecham a janela, e o console mostra a mensagem de despedida.
4. F11 entra e sai da tela cheia.
5. Com a janela aberta, o processador fica perto de 0% no Gerenciador de Tarefas, porque o laço espera o monitor a cada quadro.

### Marco 1: janela, loop, relógio fixo e mapa de tiles (2026-10-17, confirmado pelo autor)

**O que faz:** a janela mostra o **mapa da vila** (64 × 64 tiles, arte de teste de cor lisa) com zoom 3x, a partir do canto de cima à esquerda. O **relógio do mundo** anda: a partida começa às 6h do dia 1 da Primavera do Ano 412, e o título da janela mostra a data, a hora, a fase do dia e a velocidade. A cada hora e a cada dia, o console escreve uma linha.

**Teclas (provisórias, FL1-3):**

| Tecla | O que faz |
|---|---|
| Espaço | Pausa e despausa (volta para a velocidade de antes) |
| 1, 2, 3 | Velocidade 1x, 2x e 4x |
| F1 | Mostra em vermelho os tiles onde não se anda (a camada `colisao`) |
| F11 | Tela cheia |
| Esc | Fecha |

Minimizar a janela pausa o jogo sozinho; ao voltar, ele continua pausado.

**Os arquivos novos:**

- `Codigo/Tempo/`: o relógio do mundo (`RelogioDoMundo`), seguindo a [[Ficha Logica/Tempo|ficha do Tempo]];
- `Codigo/Mapa/`: a leitura e a conferência do mapa do Tiled (`MapaLocal`), seguindo a [[Ficha Logica/Mapa|ficha do Mapa]]. Se o mapa tiver erro, o jogo mostra **todos** os erros numa caixa de mensagem e não abre;
- `Codigo/Testes/`: 63 conferências automáticas do relógio e do mapa;
- `Jogo/DesenhoDoMapa.cpp`: desenha só os tiles que aparecem na tela, todos de uma vez para a placa de vídeo (o "desenho em lote");
- `Jogo/TextoDaDataDoJogo.cpp`: monta o texto do título ("Primavera, dia 1, 06h00 (amanhecer), Ano 412, 1x");
- `Jogo/data/mapas/vila.tmj` e `Jogo/data/imagens/terreno_de_teste.png`: o mapa e a arte de teste (FL1-6). Os dois podem ser abertos no **Tiled**. Quem os criou foi o script `Jogo/assets/teste/gerar_arte_e_mapa_de_teste.py`; rodar o script de novo apaga o que foi mudado no Tiled;
- bibliotecas novas: **nlohmann/json** 3.12.0 (lê o mapa), **stb_image** (lê o PNG) e o **glad** (OpenGL 3.3).

**Achado no código:** se um quadro rodasse 2 ticks de uma vez, a virada de hora do primeiro tick se perderia. Por isso o relógio só **calcula** quantos ticks rodar, e o laço principal roda um por um, conferindo as viradas depois de cada tick (a ficha do Tempo foi ajustada).

**O que testar:**

1. O mapa aparece nítido: a lavoura com a cerca em cima, o poço, pedras, flores e a rua de terra no canto direito.
2. O título começa em "Primavera, dia 1, 06h00 (amanhecer), Ano 412 — 1x", e o minuto anda (cerca de 1,5 minuto de jogo por segundo real).
3. A tecla 3 (4x): em 10 segundos, passa 1 hora, e o console escreve "07h00 (dia)".
4. Espaço pausa (o título diz "pausado") e despausa na mesma velocidade.
5. F1 pinta de vermelho a cerca, as pedras e o poço.
6. `ctest --test-dir build` termina com "100% tests passed".
7. Abrir o `Jogo/data/mapas/vila.tmj` no Tiled (opcional): mudar um tile, salvar e abrir o jogo de novo.

### Marco 2: câmera e entrada (2026-10-18, confirmado pelo autor)

**O que faz:** a câmera começa com o centro do mapa no centro da tela, em 3x, e o jogador a move pelo mapa. Ela nunca mostra o vazio além da borda. Tudo segue a [[Ficha Logica/Interface|ficha da Interface]], com as sugestões da rodada FL2 (ainda sem resposta).

| Controle | O que faz |
|---|---|
| W, A, S, D ou as setas | Move a câmera (900 pixels da tela por segundo, em qualquer zoom) |
| Shift (segurado) | A câmera anda 2 vezes mais rápido |
| Roda do mouse | Zoom 2x, 3x ou 4x; o ponto embaixo do mouse fica parado |
| Botão do meio (segurado) | Arrasta o mapa junto com o mouse |

As teclas do marco 1 (Espaço, 1, 2, 3, F1, F11, Esc) continuam iguais. A câmera anda também com o jogo pausado, porque usa o tempo real, e não os ticks.

**Os arquivos novos:**

- `Jogo/CameraDoMapa.cpp`: as contas da câmera (teclado, arrasto, zoom, limite, centro e pixel inteiro), sem SDL nem OpenGL;
- `Jogo/Testes/`: o programa `testes_do_jogo`, com 18 conferências da câmera. O `ctest` agora roda os dois programas de testes;
- `Jogo/DesenhoDoMapa.cpp`: desenha a partir do deslocamento da câmera, arredondado uma vez só, para os tiles andarem juntos, sem linhas finas entre eles.

**O que testar:**

1. O jogo abre com o cruzamento das duas ruas perto do meio da tela.
2. WASD e as setas movem a câmera; na diagonal, a velocidade parece a mesma; com Shift, mais rápido.
3. Segurando A, a câmera para na borda esquerda do mapa, sem mostrar fundo preto. O mesmo nas outras bordas.
4. A roda troca o zoom (o console escreve "Zoom: 4x."), e o que está embaixo do mouse continua embaixo dele.
5. Arrastar com o botão do meio move o mapa junto com o mouse.
6. Com o jogo pausado (Espaço), a câmera ainda anda.
7. `ctest --test-dir build` termina com "100% tests passed, 0 tests failed out of 2".

### Marco 3: carregar os dados (2026-10-18, confirmado pelo autor)

**O que faz:** ao abrir, o jogo lê todos os dados da pasta `Jogo/data`, confere cada linha e calcula o **preço base** de cada item pela fórmula do Econômico. O console mostra o resumo e os preços. Se algum arquivo tiver erro, o jogo mostra **todos** os erros numa caixa de mensagem, com o arquivo e a linha, e não abre. Tudo segue as fichas dos [[Ficha Logica/Itens|Itens]], dos [[Ficha Logica/Recursos|Recursos]] e das [[Ficha Logica/Receitas|Receitas]], com a rodada FL3.

**Os arquivos de dados** (em `Jogo/data/`):

| Arquivo | O que tem | Como editar |
|---|---|---|
| `itens.csv` | Os 6 itens da fatia: categoria, tipo, Tier, peso | Planilha, salvando em CSV com ponto e vírgula |
| `receitas.csv` | Uma receita por item: lote, insumos, construção, horas | Planilha |
| `recursos.csv` | Grão, Água e Madeira: o item gerado e a área do Tiled | Planilha |
| `construcoes.json` | Só a lista das 7 construções (o resto vem antes do marco 6) | Editor de texto |
| `regras.json` | Salários por hora em centésimos, margem, produção por estação | Editor de texto |
| `textos/pt-BR.json` | O nome que aparece na tela de cada item e construção | Editor de texto |

Uma linha que começa com `#` nas planilhas é comentário.

**Os preços base da fatia** (iguais à tabela de preços de partida do Econômico):

| Item | Preço |
|---|---|
| Grão, Água, Tora | 0,5 |
| Tábua | 1,5 |
| Pão | 1,7 |
| Móveis de Madeira | 5,1 |

**As teclas agora passam por uma tabela de ações** (FL2-4, `Jogo/AcoesDoTeclado.cpp`): trocar uma tecla é mudar uma linha da tabela. As teclas valem pela posição no teclado, e continuam as mesmas dos marcos 1 e 2.

**Os arquivos novos:**

- `Codigo/Dados/`: o leitor de CSV próprio, os textos, as regras, os itens, as construções, os recursos, as receitas, o preço base e o `carregarDadosDoJogo`, que junta tudo na ordem da ficha dos Itens;
- `Codigo/Testes/TestesDosDados.cpp`: 61 conferências novas (124 no `Codigo/`), com os 6 preços, os laços na cadeia e cada tipo de erro;
- `Jogo/AcoesDoTeclado.cpp` e o teste dele: 7 conferências novas (25 no `Jogo/`).

**O que testar:**

1. `.\build\Jogo\jogo.exe --validar` mostra "0 erros, 0 avisos" e os 6 preços.
2. Estragar uma linha do `itens.csv` de propósito (por exemplo, trocar `alimento` por `comida`), rodar o `--validar` de novo e ver o erro com o arquivo e a linha. Depois, desfazer.
3. O jogo abre como antes, e o console mostra o resumo e os preços base.
4. Todas as teclas dos marcos 1 e 2 continuam funcionando.
5. `ctest --test-dir build` termina com "100% tests passed, 0 tests failed out of 2".

### Marco 4: NPC andando (2026-10-18, confirmado pelo autor)

**O que faz:** um NPC nasce no centro da vila, em (32, 32). Com o **botão direito** num tile, ele calcula o caminho mais curto (a busca A\* em 8 direções) e anda até lá, contornando o que bloqueia, sem cortar a quina de árvores e paredes. O boneco vira para o lado em que anda e mexe as pernas. Tudo segue a [[Ficha Logica/NPC|ficha do NPC]] e a regra 4.4 da [[Ficha Logica/Mapa|ficha do Mapa]], com a rodada FL4.

**Como ele anda** (FL4-1 e FL4-5):

- 128 tiles por hora de jogo: 3,2 tiles por segundo a 1x. A velocidade fica no `regras.json` (`velocidade_de_andar_em_tiles_por_hora`);
- ele anda pelos ticks: o Espaço o pausa, e as teclas 2 e 3 o aceleram junto com o relógio;
- o desenho fica entre um tick e o seguinte, então o movimento é liso, sem pulinhos.

**As teclas novas, provisórias (FL4-6):**

| Tecla | O que faz |
|---|---|
| Botão direito | Manda **todos** os NPCs ao tile clicado |
| N | Cria mais um NPC perto do centro (até 50) |
| F2 | Pinta de azul o caminho que falta a cada NPC |

Se o destino não tem caminho (um clique no meio do lago, por exemplo), o NPC vai até o tile mais perto, e o console avisa (FL4-3).

**O boneco** (FL4-4) é descrito no `data/personagens/boneco_provisorio.json`: o tamanho do quadro (64), o ponto dos pés, as linhas de cada animação e as 3 camadas do Mana Seed (corpo, roupa, cabelo), lidas da pasta `Assents Temporarios/`, que fica fora do Git. As camadas são juntadas numa folha só ao abrir. **Sem essa pasta** (por exemplo, em outro PC), o console mostra um aviso e o jogo usa o **boneco de teste** (`data/imagens/boneco_de_teste.png`, feito por `assets/teste/gerar_boneco_de_teste.py`), que tem o mesmo formato. Para trocar a roupa ou o cabelo, é só mudar os nomes das imagens no arquivo.

**A biblioteca nova:** a **EnTT** 3.16.0 (FT2-2), baixada pelo CMake na primeira vez. Ela guarda cada NPC como uma entidade com os componentes dele (a posição, o caminho, o pedido de caminho). Por isso, rode o `cmake --preset padrao` de novo antes de compilar.

**Os arquivos novos:**

- `Codigo/Mapa/BuscaDeCaminho.cpp`: a busca A\*, que reaproveita a memória de uma busca para a outra;
- `Codigo/Npc/`: os componentes do NPC e o movimento (criar, pedir caminho, atender os pedidos, andar a cada tick);
- `Jogo/BonecoDoNpc.cpp`: a leitura do arquivo do boneco e a escolha do quadro da animação;
- `Jogo/DesenhoDosPersonagens.cpp`: o desenho dos NPCs, de cima para baixo na tela;
- `Jogo/DesenhoDeQuadrados.cpp`: o "pincel" da placa de vídeo, separado do desenho do mapa para os dois usarem o mesmo;
- os testes: 43 conferências novas no `Codigo/` (167 no total) e 11 no `Jogo/` (36 no total).

**O que testar:**

1. `cmake --preset padrao` e depois `cmake --build --preset padrao` (a EnTT é baixada na primeira vez).
2. Ao abrir, o NPC aparece no centro da vila, parado, olhando para baixo, com a arte do Mana Seed. O console diz "NPC 1 criado em (32, 32)" e **não** mostra o aviso do boneco.
3. Clicar com o botão direito num tile perto: o NPC anda até lá, virado para o lado certo e mexendo as pernas, e para em cima do tile clicado.
4. Clicar com o botão direito do outro lado de algo que bloqueia (as árvores do bosque, embaixo à esquerda, ou o lago, embaixo à direita). Com o **F2** ligado, o caminho azul contorna o obstáculo; com o **F1**, dá para ver os tiles bloqueados.
5. Clicar no meio do lago: o NPC vai até a margem mais perto, e o console avisa "não tem caminho".
6. Apertar **N** umas 5 vezes e clicar com o botão direito: todos andam juntos e passam um pelo outro (eles não colidem). Quem está mais embaixo na tela aparece na frente.
7. Durante a caminhada: **Espaço** pausa o NPC (a câmera continua andando), e **3** (4x) o deixa 4 vezes mais rápido.
8. `ctest --test-dir build` termina com "100% tests passed, 0 tests failed out of 2".

### Marco 5: rotina (2026-10-18, confirmado pelo autor)

**O que faz:** a vila ganha as 10 construções e os 15 NPCs Civis. Cada NPC escolhe o que fazer pela **Utilidade** (uma nota para cada ação; ganha a maior) e vai até o lugar dela: dorme em casa, trabalha na construção do emprego, come em casa ou no trabalho, vai ao Mercado no dia de compra e passeia no Período Livre. Numa construção, ele **entra** e some da tela até sair. Tudo segue a [[Ficha Logica/IA|ficha da IA]], com a rodada FL5.

**A vila** (`data/vila_inicial.json`, FL5-1 e FL5-2): as construções aparecem como **retângulos de cor** nos lotes, com a porta mais escura embaixo, e os tiles do lote passam a bloquear. O lote 5 fica livre.

| Cor | Construção | Quem |
|---|---|---|
| Amarela | Fazenda | Ana, Bento |
| Verde | Posto de Coleta | Clara, Davi |
| Laranja | Cozinheiro | Elisa, Fábio |
| Azul | Mercado | Gil, Helena (Lojistas) |
| Cinza | Construtor | Iago, Joana |
| Castanha | Carpintaria do jogador | Lauro, Marta |
| Marrom | As 4 Casas | os 15, de 4 em 4 (a última com Nuno, Olga e Pedro, os desempregados) |

**O dia de um Civil:** dorme das 22h às 6h; das 6h às 7h, passeia (ou vai ao Mercado, no dia de compra); come às 7h, às 12h e às 19h, por meia hora; trabalha das 7h às 15h; das 15h às 22h, passeia. Cada um compra a cada 2 dias: metade da vila num dia, a outra metade no outro. Os números ficam no `regras.json` (`rotinas`, `utilidade`, `passeio`).

**As teclas de teste** (FL5-6): o botão direito e o N do marco 4 saíram, porque quem manda agora é a rotina. O **F2** continua mostrando os caminhos. O **F3** liga um quadradinho em cima de cada NPC, com a cor da ação, e escreve no console a tabela dos 15 (a ação, onde está e as notas):

| Cor do quadradinho | Ação |
|---|---|
| Verde | Passear |
| Azul | Comprar (a caminho do Mercado) |
| Laranja | Comer (a caminho de casa) |
| Cinza | Trabalhar ou Dormir (a caminho) |

**O sorteio:** o passeio usa um gerador próprio (o PCG32), ligado à `semente_da_partida` do `vila_inicial.json`. A mesma semente dá sempre a mesma partida.

**Os arquivos novos:**

- `Codigo/Vila/VilaInicial.cpp`: a leitura e a conferência do `vila_inicial.json` (o `--validar` também confere);
- `Codigo/Npc/RotinaDosNpcs.cpp` e `RegrasDaRotina.h`: os períodos, as notas, os destinos, as chegadas e o passeio;
- `Codigo/Tempo/SorteioComSemente.cpp`: o PCG32;
- `data/vila_inicial.json` e as partes novas do `data/regras.json`;
- os testes: 26 conferências novas no `Codigo/` (193 no total), entre elas **um dia inteiro sem tela** (às 10h, os 12 empregados estão no trabalho; às 3h, os 15 estão em casa; e o mesmo dia rodado duas vezes dá as mesmas posições), e 1 no `Jogo/` (37 no total).

**O que testar:**

1. `.\build\Jogo\jogo.exe --validar` diz "1 vila (10 construções e 15 NPCs). 0 erros, 0 avisos".
2. Ao abrir (6h), as construções aparecem nos lotes, e os NPCs saem das casas: uns passeiam, e os do dia de compra vão ao Mercado.
3. Apertar **F3**: os quadradinhos aparecem, e o console mostra a tabela dos 15.
4. Apertar **3** (4x): perto das 7h, todos voltam para casa para comer; às 7h30, os 12 empregados vão para o trabalho e somem dentro dele; só os 3 desempregados ficam na rua.
5. Às 12h, ninguém sai do trabalho (o almoço é lá dentro). Às 15h, os empregados saem e passeiam. Às 19h, todos voltam para casa para jantar.
6. Às 22h, todos voltam para casa, e a vila fica vazia até as 6h.
7. Apertar **F3** de novo em outro horário e conferir a tabela (por exemplo, às 10h: 12 "Trabalhar, dentro de...").
8. `ctest --test-dir build` termina com "100% tests passed, 0 tests failed out of 2".

**O que vem no marco 6:** a produção, o inventário da cidade e o consumo (o Pão sai do Cozinheiro, entra no estoque e é comido), depois das fichas da Produção, do Inventário, do Trabalho e Propriedade, do Econômico (consumo e salários) e da Construção.
