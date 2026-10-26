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

   Para ver cada conferência que falhou, rode o programa de testes direto: `.\build\Codigo\Testes\testes_do_codigo.exe`. Desde o marco 7, o `ctest` leva cerca de 1 minuto e meio: o teste de 1 ano sozinho leva uns 45 segundos.

6. Rodar o teste de 1 ano sem abrir a janela (marco 7). Grava o `relatorio.csv` na pasta onde o comando foi dado e termina com **PASSOU** ou **FALHOU** e o motivo:

   ```
   .\build\Jogo\jogo.exe --simular 64 --semente 1
   ```

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
| `itens.csv` | Os itens da fatia (8 desde o marco 6): categoria, tipo, Tier, peso | Planilha, salvando em CSV com ponto e vírgula |
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

### Marco 6: produção, inventário e consumo (2026-10-18, o autor mandou seguir)

**O que faz:** a vila passa a **produzir, guardar, comer e pagar**. Os empregados que estão trabalhando somam horas; a cada hora, o trabalho vira progresso do ciclo de cada construção, e o ciclo terminado põe o lote no **inventário da cidade**. À meia-noite, a vila **come** (cada Civil, 1 Pão; a cada 8 dias, 1 Móvel) e paga ao dono do item, e os empregados **recebem** pelas horas do dia. Tudo segue as fichas da [[Ficha Logica/Producao|Produção]], do [[Ficha Logica/Inventario|Inventário]], da [[Ficha Logica/Economia|Economia]] e da [[Ficha Logica/Construcao|Construção]], com a rodada FL6.

**O que muda na tela:**

- os **Braçais** (Ana e Bento na Fazenda, Clara e Davi no Posto de Coleta) não entram mais na construção: trabalham **parados num tile do campo** (o campo de Grão, o poço ou o bosque) e almoçam lá;
- o **café da manhã** passou para as **6h** (FL6-7): às 7h, todos já vão direto para o trabalho;
- quando o autor desenhar uma construção e salvar em `data/imagens/construcoes/<tipo>_nivel_<n>.png` (por exemplo, `casa_nivel_1.png`, com 64 × 96 px; o Guia da Arte, parte 7), o jogo a desenha no lugar do retângulo, sem mexer no código. O telhado (os 32 px de cima) fica por cima de quem passa atrás. Uma imagem de tamanho errado vira um aviso no console.

**O console** (FL6-8):

- **à meia-noite**, sai sozinha uma linha de resumo do dia: o estoque de cada item, o Banco, o dinheiro do jogador, a riqueza média dos Civis, os salários pagos e os dias em déficit;
- a tecla **F4** escreve a tabela completa: o estoque de cada dono (Cidade / jogador), a capacidade, cada construção (a receita, o progresso do ciclo, o motivo de estar parada, quem trabalha agora, o gerente e o que fez hoje) e o dinheiro.

**O que esperar** (é de propósito, ficha da Economia, parte 5): o Banco perde cerca de **54 moedas por dia** e fica quase sem dinheiro por volta do **dia 8**; o jogador também. Quem devolve o dinheiro ao Banco (o Mercado Local, o gasto extra, o imposto e o estabilizador) entra no marco 7. Sem dinheiro, os salários deixam de ser pagos, e o console mostra isso.

**Os dados novos:**

| Arquivo | O que mudou |
|---|---|
| `construcoes.json` | Cada tipo ganhou o porte, o que guarda, a Função e o Trabalho de quem não tem receita; e a Pedreira (só nos dados, FL6-5) |
| `itens.csv`, `receitas.csv`, `recursos.csv` | A Pedra e o Bloco de Pedra (8 itens, FL6-5) |
| `regras.json` | As vagas e a capacidade por nível, o custo das obras, a cesta mínima, a loja da cidade (110%), o bônus de nível e do gerente, a ordem do dia e o café às 6h |
| `vila_inicial.json` | O dinheiro do primeiro dia (Banco 400, jogador 100, 12 por Civil), o estoque inicial, o nível de cada construção (a Carpintaria do jogador no 2), as ordens da Carpintaria e as habilidades (50) |

**Os arquivos novos:**

- `Codigo/Inventario/`: o inventário da cidade (duas listas, uma por dono, e a capacidade por categoria);
- `Codigo/Producao/`: o ciclo, a escolha da receita da cidade, a fila de ordens do jogador e o campo dos Braçais;
- `Codigo/Economia/`: o dinheiro, o consumo, os salários e a ordem do dia;
- `Codigo/Construcao/`: as vagas, a capacidade e o gerente;
- `Codigo/Dados/Construcoes.cpp`: a leitura do `construcoes.json` completo;
- `Codigo/Vila/SimulacaoDaVila.cpp`: a vila inteira num tick (o jogo e o teste sem tela usam a mesma);
- `Jogo/ImagemDasConstrucoes.cpp`: o nome e o tamanho das imagens das construções;
- os testes: 46 conferências novas no `Codigo/` (239 no total), entre elas **3 dias sem tela** (às 10h, os 12 empregados trabalham, e os 4 Braçais estão no campo; o Cozinheiro faz 16 Pães por dia depois do primeiro; a vila nunca fica sem Pão; o dinheiro total nunca muda), e 5 no `Jogo/` (42 no total).

**O que testar:**

1. `.\build\Jogo\jogo.exe --validar` diz "8 itens, 8 construções, 4 recursos, 8 receitas, 1 mapa, 1 vila (10 construções e 15 NPCs). 0 erros, 0 avisos", e os preços mostram a Pedra a 0,5 e o Bloco de Pedra a 1,8.
2. Abrir o jogo e apertar **3** (4x). Às 6h, todos comem em casa; às 7h, os empregados vão trabalhar. Arrastar a câmera para o campo de Grão (em cima, à esquerda) e para o poço: Ana, Bento, Clara e Davi ficam parados lá, visíveis.
3. Às 10h, apertar **F3**: os 4 Braçais aparecem "no campo de ...", e os outros 8 empregados "dentro de ...". Apertar **F4**: o estoque, a Fazenda e o Posto "2 de 2 trabalhando agora", o Pão subindo, e a Carpintaria "PARADA: sem ordem" no primeiro dia (o jogador já tem os 6 Móveis e as 12 Tábuas que as ordens pedem).
4. Às 12h, os Braçais continuam no campo (o almoço conta como trabalho).
5. À meia-noite, o console escreve o "Resumo do dia 1". Deixar correr uns 3 dias e comparar: o Pão fica perto de 45, o Banco cai cerca de 54 moedas por dia, os dias em déficit ficam em 0.
6. Apertar **F4** no dia 2: a Carpintaria passa a fazer Móveis (a vila comeu 1 Móvel na primeira meia-noite).
7. `ctest --test-dir build` termina com "100% tests passed, 0 tests failed out of 2".

### Marco 7: a simulação sem tela de 1 ano (2026-10-19)

**O que faz:** o dinheiro da vila passa a **fechar o ciclo**, e a vila ganha os eventos e a Felicidade. Tudo segue a parte 10 da [[Ficha Logica/Economia|Economia]] e da [[Ficha Logica/Producao|Produção]] e as fichas da [[Ficha Logica/Felicidade|Felicidade]], dos [[Ficha Logica/Eventos|Eventos]] e da [[Ficha Logica/Simulacao|Simulação sem tela]], com a rodada FL7.

- **O Mercado Local:** o preço de cada item anda até 10% por dia na direção do que o estoque pede (pouco estoque, mais caro; muito, mais barato), entre 50% e 200% do preço base.
- **O gasto extra:** depois da cesta, a classe gasta até 80% da renda de ontem: primeiro em Móveis a mais, e o resto na loja da cidade (o Banco).
- **O imposto:** 20% de cada salário vai para o Banco na hora.
- **Os salários** sobem com a riqueza da vila e descem com o desemprego (na fatia, × 0,9: 3 desempregados em 15).
- **O estabilizador:** o Banco ajuda a classe quando ela fica com menos de 3 dias da cesta.
- **16 dias sem salário:** os empregados saem.
- **A previsão do consumo, medida:** a média dos 5 últimos dias do que a vila gastou ou quis gastar. A Carpintaria parada por falta de Tora agora fica "sem Tora" (e não "sem ordem"), e o Posto de Coleta percebe.
- **A folga:** uma construção da Cidade com estoque de sobra (125% do alvo) fica de folga no dia seguinte; os empregados passeiam e não recebem. A Carpintaria do jogador sem nenhuma ordem que queira produzir também.
- **Os 11 eventos** do `data/eventos.json` (a Seca, a Chuva boa, a Febre, o Mercador...), sorteados à meia-noite.
- **A Felicidade dos Civis** (começa em 50), só medida e mostrada.
- **O boneco:** o `boneco_provisorio.json` aponta direto para o boneco de teste (a decisão de usar só a arte de teste): o aviso do Mana Seed some.

**O console:**

- o **resumo do dia** ganhou os preços locais do Pão e dos Móveis, o imposto, o gasto da cesta, o gasto extra, a ajuda do Banco (quando há) e a Felicidade com a faixa ("58,1 (Estável)");
- cada evento que começa ou termina vira uma linha ("Evento: Seca (a Fazenda produz 50% por 8 dias).");
- o **F4** mostra o preço local, o preço base e o consumo previsto de cada item, e "DE FOLGA HOJE" nas construções de folga; o **F3** mostra "doente em casa".

**O teste de 1 ano** (`jogo --simular 64 --semente 1`): roda os 64 dias o mais rápido possível e grava o `relatorio.csv`, uma linha por dia, com ponto e vírgula e vírgula decimal (abre direto no Excel ou no LibreOffice). Ele falha com fome por 4 dias seguidos, com o Banco em 0 ou com o Pão fora de 1,2 a 2,6. Outras opções:

- `--semente 2` (ou outro número) troca a semente do `vila_inicial.json`: mudam os passeios e os eventos;
- `--npcs-a-mais 485` cria 485 visitantes que só passeiam (500 NPCs no total), para medir o tempo, sem os critérios.

**O que esperar** (medido em 2026-10-18):

| Teste | Resultado |
|---|---|
| `--simular 64 --semente 1` | **PASSOU**. O Banco fica entre 375 e 450 até o dia 48 e cai para 272 no Inverno; o jogador termina com 237; o Pão fica entre 1,4 e 2,4; a Felicidade, perto de 58 (Estável). Eventos: Chuva boa, Febre, Seca, Mercador, Ratos, Colheita farta, Festa e Achado |
| As sementes de 1 a 10 | 8 passam; a **3** e a **4** falham ("o preço do Pão foi a 2,7"): a Febre pega um dos 2 Cozinheiros no primeiro mês |
| 256 dias (4 anos) | O dinheiro vai do Banco para o jogador, e o Banco fica quase vazio por volta do dia 190 |
| Tempo (com depuração) | 64 dias em cerca de 45 segundos (36 microssegundos por tick); com 500 NPCs, 0,95 milissegundo por tick, abaixo da meta de 3 |

Os dois problemas de equilíbrio (a Febre no Cozinheiro e os Móveis caros) foram resolvidos pela rodada FE1: 75 Pães no começo e "manter 20 Móveis"; as sementes de 1 a 10 passam.

**O Tracy** (opcional, desligado): para ver quanto tempo cada parte do tick gasta, configure com `cmake --preset padrao -DUSAR_TRACY=ON`, compile, abra o programa do Tracy (baixado à parte, do site do autor do Tracy) e rode o jogo. Para desligar: `cmake --preset padrao -DUSAR_TRACY=OFF`.

**Os dados novos:**

| Arquivo | O que mudou |
|---|---|
| `regras.json` | O Mercado Local, a loja que compra a 90%, o gasto da classe (80%), o imposto, o estabilizador, os multiplicadores do salário, os 16 dias de atraso, a Felicidade e a ordem do dia com os 7 passos |
| `eventos.json` (novo) | Os 11 eventos da fatia |
| `textos/pt-BR.json` | Os nomes dos eventos |
| `personagens/boneco_provisorio.json` | A camada é o boneco de teste |

**Os arquivos novos:**

- `Codigo/Economia/MercadoLocal.cpp` e `FelicidadeDaClasse.cpp`; o gasto extra, o imposto, os multiplicadores, o estabilizador e a saída por atraso no `ConsumoESalarios.cpp`;
- `Codigo/Producao/ConsumoMedidoDaVila.cpp`: os 5 dias do consumo medido; a folga e a parada "sem Tora" no `ProducaoDaVila.cpp`;
- `Codigo/Eventos/`: a leitura do `eventos.json` e as regras dos eventos;
- `Codigo/Simulacao/RelatorioDaSimulacao.cpp`: o relatório, os critérios e o `--simular`;
- `Codigo/Desempenho/MedicaoDoDesempenho.h`: as medições do Tracy;
- os testes: 51 conferências novas no `Codigo/` (290 no total), entre elas **16 dias sem tela duas vezes** (os mesmos relatórios); e o **teste de 1 ano** como o terceiro teste do `ctest`.

**O que testar:**

1. `.\build\Jogo\jogo.exe --validar` diz "... 1 vila (10 construções e 15 NPCs), 11 eventos. 0 erros, 0 avisos".
2. `.\build\Jogo\jogo.exe --simular 64 --semente 1` escreve os eventos, "Relatório gravado em relatorio.csv (64 linhas)", o tempo e **PASSOU**. Abrir o `relatorio.csv` no Excel: uma linha por dia; o Pão do dia 40 está na coluna `preco_do_pao`.
3. `.\build\Jogo\jogo.exe --simular 64 --semente 3` também termina em **PASSOU** (depois da rodada FE1, as sementes de 1 a 10 passam).
4. Abrir o jogo: o boneco de teste aparece sem o aviso do Mana Seed. Deixar correr uns 3 dias a 4x e ler o resumo do dia: o Pão fica perto do preço base (a vila começa com 5 dias de Pão), e o gasto extra aparece no resumo.
5. Apertar **F4** depois de alguns dias: o preço local e o consumo previsto de cada item; em algum dia, uma construção "DE FOLGA HOJE".
6. `ctest --test-dir build` termina com "100% tests passed, 0 tests failed out of 3".

### Marco 8: a interface do jogador, as obras e as decisões (2026-10-19, falta o teste do autor)

**O que faz:** a vila agora se joga pelo mouse. Uma barra de cima com a data, a velocidade (Pausa, 1x, 2x, 4x), o dinheiro e o botão "Gestão (Tab)"; uma faixa de alertas; notificações à direita; a janela de gestão (tecla **Tab**) com as abas Cidade, Construções, Inventário, Mercado e Diário; e, ao clicar num lote do mapa, o painel da construção. O jogador pode: trocar a fila de ordens da Carpintaria; fixar o preço de cada produto (de −30% a +30%); escolher o salário (Baixo, Normal, Alto); contratar e demitir; comprar e vender na loja da cidade; encomendar uma construção no lote livre (o 5) ou subir de nível (até o 3). A manutenção de cada construção entra no começo de cada estação.

**A interface** usa a **Dear ImGui** (v1.91.9, baixada pelo CMake) só na fatia, e a fonte **Noto Sans** (`Jogo/assets/fontes/`, licença OFL junto). Ela só desenha e lê os cliques: cada decisão vira um **pedido** (`Codigo/Jogador/PedidosDoJogador.h`), aplicado no começo do próximo tick (ou na hora, com o jogo pausado). Por isso o teste sem tela faz os mesmos pedidos e a simulação continua igual.

**Os arquivos novos:**

- `Codigo/Jogador/`: `PedidosDoJogador` (a fila de pedidos), `DecisoesDoJogador` (o preço, o salário, contratar, demitir, a loja), `ObrasDaVila` (as obras e a manutenção), `NotificacoesEAlertas` (as notificações, o Diário, os alertas e o texto do dinheiro);
- `Codigo/Producao/EstadoDoJogador.h`: o estado dessas decisões;
- `Jogo/InterfaceDoJogador.cpp`: a barra, os alertas, a gestão e o painel (a única parte que usa a ImGui);
- `Codigo/Testes/TestesDoMarco8.cpp`: os testes novos;
- a `SimulacaoDaVila` agora guarda a **cópia da vila e do mapa** (as obras acrescentam construções e bloqueiam lotes), e o `Jogo/` desenha a partir dela.

**Os dados novos:** no `regras.json`, `decisoes_do_jogador` (a faixa do preço, o salário e a velocidade de cada nível, os 8 dias), `obras` (o nível máximo do Engenheiro, os 5% da manutenção, os 75% da produção atrasada) e o passo `manutencao` no começo da `ordem_do_dia`.

**O que testar:**

1. Abrir o jogo: a barra de cima aparece; **Tab** abre a gestão; **Esc** fecha o painel, depois a gestão, e só então abre o menu (marco 9).
2. Clicar nos prédios até achar a "carpintaria_do_jogador": o painel mostra os 2 empregados, os 3 desempregados, o salário, os preços e a fila de ordens. Trocar o preço dos Móveis para +30% e ver o preço novo no painel; pôr uma ordem "Fazer 5 Móveis".
3. Aba **Mercado**: vender 10 Tábuas; o dinheiro do jogador sobe e as Tábuas passam para a Cidade.
4. Clicar no lote 5 (livre): escolher uma Fazenda e encomendar. O lote vira um retângulo marrom (a obra); em cerca de 3 dias de jogo ele vira a Fazenda, com a notificação "Obra pronta".
5. Deixar correr 16 dias: a manutenção entra no começo do Verão e as notificações "Obra pronta: manutenção de ..." aparecem.
6. `ctest --test-dir build` termina com "100% tests passed, 0 tests failed out of 3".

### Marco 9: salvar e carregar (2026-10-19, falta o teste do autor)

**O que faz:** o jogo grava a partida num arquivo e a reconstrói igual (mesmo tick, mesmo sorteio, NPCs no meio do mesmo passo). Abre numa **tela inicial** (Continuar, Novo jogo, Carregar, Sair); o **Esc** agora abre o menu (Voltar, Salvar, Carregar, Sair para a tela inicial) e **deixou de fechar o jogo**. A partida carregada começa **pausada**. Com o menu aberto, o tempo para.

**Teclas:** **F5** salva no último espaço usado (o 1, se nenhum foi); **F9** carrega o save mais novo (com mais de 1 hora de jogo sem salvar, o menu pergunta antes). Sair, carregar ou fechar a janela pedem confirmação se houver mais de 1 hora de jogo sem salvar.

**Os saves:** 3 espaços manuais (`espaco1.save` a `espaco3.save`) e 1 automático (`automatico.save`, gravado toda meia-noite do jogo). Ficam em `C:\Users\<usuário>\AppData\Roaming\Projeto-Jogo\saves\` (o caminho também sai no console ao abrir o jogo). O formato é MessagePack pela nlohmann/json (FL9-1). Um save de outra versão, ou com um item, receita, evento ou construção que não existe mais, é **recusado com uma mensagem** e a partida atual continua (FL9-5).

**Os arquivos novos:**

- `Codigo/Save/`: `ArquivoDeSave` (o ponto de entrada, o arquivo temporário `.tmp` e o cabeçalho), `GravarAPartida`, `CarregarAPartida` e `AcessoAoEstadoParaOSave.h` (a única porta para os campos privados);
- `Jogo/`: `TelaInicial`, `MenuDoEsc` e `ListaDeSaves` (a pasta, a lista e o desenho da lista);
- `Codigo/Testes/TestesDoSave.cpp`: os testes novos.

**Linha de comando:**

| Comando | O que faz |
|---|---|
| `jogo --carregar arquivo.save` | abre o jogo já com esse save (pausado) |
| `jogo --save-para-json arquivo.save` | escreve o save em texto no console, para investigar um erro |
| `jogo --simular 64 --semente 1 --salvar-e-carregar-no-dia 20` | salva às 0h do dia 20, carrega e segue; o relatório tem que sair igual ao de `--simular 64 --semente 1` |

**Não vai no save (ficou para depois):** a câmera (volta centralizada), a seleção da interface, as janelas abertas e os NPCs falsos do teste de desempenho. O personagem do jogador ainda não existe no código (só o dinheiro e o estoque dele, que vão no save).

**Verificado pelo assistente:** `testes_do_codigo` passa (salvar, carregar e salvar dá o mesmo arquivo; uma obra do jogador em andamento volta e a partida segue igual por 12 dias; versão errada, `item.vinho`, arquivo cortado e `.tmp` conferidos); `jogo --simular 64 --semente 1` com e sem `--salvar-e-carregar-no-dia 20` dá **o mesmo `relatorio.csv`, byte a byte**. As telas (tela inicial, menu, F5 e F9) ainda não foram vistas na janela.

**O que testar:**

1. Abrir o jogo: aparece a tela inicial; "Continuar" e "Carregar" estão desligados (ainda não há save). **Novo jogo** abre a vila.
2. Deixar correr uns minutos, apertar **F5** (notificação "Jogo salvo (...)"), deixar correr mais, apertar **F9**: a vila volta ao momento do F5, **pausada**; o Espaço despausa.
3. **Esc** → Salvar → Espaço 2; depois Esc → Carregar mostra o Espaço 1 e o 2 com a data do jogo e a hora real.
4. Fechar o jogo e abrir de novo: **Continuar** abre o save mais novo.
5. Com mais de 1 hora de jogo sem salvar (40 segundos reais a 1x), Esc → "Sair para a tela inicial" pergunta antes; o X da janela também.
6. Deixar passar uma meia-noite: o `automatico.save` aparece na lista de Carregar.
7. `.\build\Jogo\jogo.exe --save-para-json "$env:APPDATA\Projeto-Jogo\saves\espaco1.save"` mostra o save em texto (procurar `item.pao`).
8. `ctest --test-dir build` termina com "100% tests passed" (o `testes_do_codigo` agora leva cerca de 1 minuto a mais).

### Rodada FE3: o ritmo do dinheiro do jogador (2026-10-19)

O `multiplicador_do_preco` dos Móveis (`itens.csv`) foi de 100 para **140**, e o painel da construção do jogador ganhou a dica "ninguém mais vende o que você faz; mudar o preço reduz a venda". Medido com `jogo --simular 64 --semente N` (todas **PASSOU**): o jogador ganha **+3,4 por dia** na semente 1 (de 88,9 para 305,8) e **+4,4** na 2 (para 364,4), antes +0,45. O preço dos Móveis (Pão fica em 1,3 a 1,9) e a Felicidade (cerca de 58) quase não mudam. **Atenção:** o Banco perde o que o jogador ganha: termina o ano em 175 (semente 1) e 108 (semente 2), antes perto de 400, e não fecharia um segundo ano. Com 150 a semente 2 chegava a 45; por isso o valor ficou em 140 (o limite de cima da sugestão).

### Rodada FE4: o dinheiro do Banco da Cidade (2026-10-19)

A **verba da Coroa**: no dia 1 de cada estação entram de fora **75 moedas** no Banco (`verba_da_coroa_por_estacao_em_decimos`: 750 no `regras.json`). Medido com `--simular 64`: o Banco termina o ano em **475** (semente 1) e **408** (semente 2), nunca abaixo de 295 (antes da FE4: 175 e 108); o jogador continua com +3,4 e +4,4 por dia.

O teste de 1 ano ficou mais exigente (FE4-3): agora **FALHA** se o Banco ficar abaixo de 250 em algum dia, ou se no dia 64 estiver mais de 50 abaixo do dia 1. **Depois de mexer na economia**, rode também os 2 anos à mão (cerca de 90 segundos):

```
.\build\Jogo\jogo.exe --simular 128 --semente 1
```

**Os 2 anos (semente 1, 2026-10-19):** PASSOU, mas com uma tendência a olhar: no ano 2 o Banco **sobe** de 475 para 867, e o dinheiro do jogador **cai** de 305 para 260. A verba fixa sobra quando o jogador deixa de lucrar; a causa da queda do jogador no ano 2 foi medida (abaixo).

**Por que o jogador cai no ano 2 (medido, semente 1, por dia; registro temporário, não ficou no código):** só a Carpintaria vende e paga salário. Ano 1 → ano 2: venda de Móveis **22,7 → 17,2** (214 → 190 vendas; preço médio **6,8 → 5,8**); salários **18,3 → 17,5**; insumos (Tábuas e Tora) 1,2 → 0,9; horas de obra 0,56 e loja 0,38 iguais; recebido pelas obras 1,24 → 1,20. Saldo: **+3,4 → −0,9** por dia. Não é a Pedra (restam 202 Blocos no dia 128, e o jogador não comprou nada fora dos insumos), nem o salário (caiu junto) nem evento. O estoque de Móveis ficou em 16 a 19 o ano todo. A queda vem dos dias 81 a 112: o Pão fica escasso (92 → 43, preço de 1,3 → 2,4; Água baixa e a Febre do dia 85), a riqueza média cai de 14 para 10,5 e o dinheiro da classe civil de 145 para 110. Sobra menos para o gasto extra, a procura de Móveis cai (3,3 → 2,7 vendas/dia) e o preço local, que depende do estoque contra o alvo de 5 dias de consumo, cai para 4,3 (o do dia 1 era 7,1). Os custos do jogador (salários) são fixos e a receita não: com pouca procura, ele perde. Nos dias 113 a 128 a venda volta a 3,1/dia a 5,8, e o jogador já para de cair (+0,2/dia). **Decidido na FE5-1:** aceitar o prejuízo na crise, sem regra nova (ver a ficha da Economia).

### Marco A1: o clima e as estações (2026-10-19, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Clima|ficha do Clima]]):

- **O clima do dia**, sorteado à 0h (o passo novo "clima", o primeiro do `ordem_do_dia`), com a sequência 3 do sorteio: 60% de repetir o de ontem, ou a tabela da estação do `data/clima.json`. O dia 1 é sempre Limpo. A barra de cima do jogo mostra o clima de hoje.
- **Tempestade:** as receitas Braçais produzem 50%; no Período Livre, os NPCs ficam em casa (o Passear vai para a própria casa) e ninguém vai comprar.
- **A lenha do inverno:** cada Civil gasta 25 centésimos de Tora por dia (150% na neve), tirados da Tora da Cidade depois da cesta; a classe paga o preço da loja. Sem Tora, faltou lenha: −10 na Felicidade (fator Consumo) e mais 1 dia em déficit. A dívida zera no dia 1 da Primavera.
- **Calor forte:** −2 na Felicidade, no fator dos eventos.
- **Estocar para o inverno:** nos últimos 4 dias do Outono, o estoque-alvo da Tora ganha 18 (15 Civis × 25 × 5 ÷ 100) e o do Grão, 5 dias do consumo.
- **Inverno rigoroso:** um evento novo do `eventos.json` (25% no dia 1 do Inverno, cooldown de 64 dias): a Fazenda a 67% (o "÷ 1,5" da ficha: 25% vira cerca de 16%) e a lenha a 200% (um efeito novo, `lenha_em_porcento`).
- **Save:** a seção `clima` e o sorteio do clima; o formato passou para a **versão 2** (os saves da versão 1 são recusados).
- **Relatório:** as colunas `clima` (o do dia que acabou), `lenha_do_dia` (as Toras tiradas à 0h) e `deficit_lenha`.

Arquivos novos: `Codigo/Clima/Clima.h` e `.cpp` (o sorteio, a lenha, a leitura do `clima.json`), `Codigo/Clima/ClimaDaVila.h` e `.cpp` (a ligação com a vila), `Codigo/Testes/TestesDoClima.cpp`, `Jogo/data/clima.json`.

**Verificado pelo assistente:** `testes_do_codigo` passa (397 conferências, a tabela da parte 8 da ficha); `jogo --simular 64` **passa nas sementes de 1 a 10**. Na semente 10: 17 dias nublados, 16 limpos, 10 de chuva, 10 de tempestade, 6 de neve e 5 de névoa; o inverno gastou **71 Toras de lenha** (a ficha previa cerca de 69); a vila entrou no inverno com 37 a 48 Toras e **nenhum dia ficou sem lenha** (o estoque do fim do Outono funcionou). O Inverno rigoroso não saiu nessas sementes (25% de chance). Com `--salvar-e-carregar-no-dia 50` (no meio do inverno, com lenha devida), o `relatorio.csv` sai **igual, byte a byte**. **Falta o teste do autor** na janela (o clima na barra de cima; os NPCs em casa na tempestade).

### Marco A2: o trabalho e a propriedade (2026-10-19, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Trabalho|ficha do Trabalho e Propriedade]]):

- **A Cidade contrata sozinha:** depois dos salários, cada vaga vazia de uma construção da Cidade recebe o desempregado de maior habilidade no Trabalho dela, se o Banco tem 3 dias do salário da vaga. Quem saiu de uma construção por salário atrasado não volta para ela na mesma estação.
- **A redistribuição** (dia 1 de cada estação): dois empregados da Cidade de Trabalhos diferentes trocam de emprego se a soma das habilidades usadas sobe 10 ou mais. Ninguém perde o emprego.
- **Comprar e vender construções** (painel da construção): o valor é o custo da obra do porte a preço de hoje × o nível; a Cidade compra de volta por 75%. A construção comprada começa no **modo automático** (a IA da Cidade escolhe a receita); o jogador pode desligar e usar a fila de ordens.
- **O leilão:** a construção do jogador sem empregados por 32 dias vai a leilão por 3 dias (lance mínimo de 50% do valor). Com lance, os salários atrasados são pagos primeiro aos Civis e o resto volta ao jogador; sem lance, ela volta para a Cidade.
- **Save:** versão 3 (os campos novos dos NPCs e das construções e o leilão). **Relatório:** as colunas `contratacoes`, `trocas` e `leilao`.

Arquivos novos: `Codigo/Vila/Trabalho.h` e `.cpp`, `Codigo/Vila/RegrasDoTrabalho.h`, `Codigo/Testes/TestesDoTrabalho.cpp`; o bloco `"trabalho"` no `regras.json`.

**Verificado pelo assistente:** `testes_do_codigo` passa (414 conferências: os exemplos da parte 8 da ficha, como o Bruno e a Carla que trocam, o leilão com a dívida de 64 e a compra do Posto). `jogo --simular 64` passa nas **sementes de 1 a 10**, com **0 contratações e 0 trocas**: a fatia não tem vaga vazia na Cidade, e todos têm habilidade 50 (a ficha previa isso). **Falta o teste do autor** na janela: comprar o Posto de Coleta, ver o modo automático, vender a Carpintaria.

### Marco A3: a população (2026-10-20, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Populacao|ficha da População]]):

- **A idade:** cada NPC tem idade (a vila inicial vai de 5 a 19 anos, no `vila_inicial.json`) e faz 1 ano no dia 1 da Primavera. A partir dos 21, no dia 1 de cada estação, morre de velhice com 10% por ano acima de 20 (× 150% no Inverno rigoroso), e um jovem de 4 anos ocupa a cama dele na hora (a reposição).
- **O acumulado em pessoas** (1 NPC = 100 pessoas): no dia 1 de cada estação entram os nascimentos (4%, × a Felicidade × 2, pela metade se houve fome) e saem as mortes (1%); do 4º dia seguido de fome, 2% de quem ficou sem comida por dia; e um pouco quando falta lenha. A +100 chega um NPC (se cabe e há cama); a −100 morre o mais velho. Abaixo da capacidade, o acumulado negativo volta a 0 (PO1-4).
- **A migração:** chega 1 migrante (idade de 4 a 12, habilidades de 20 a 60) se há cama, vaga, Felicidade 50 ou mais e nenhuma fome; vai embora 1 se a Felicidade está abaixo de 30 ou o desemprego passa de 30%. A saída é decidida com a vila da estação que terminou (ajuste do código, anotado na ficha).
- O passo novo **"populacao"** na ordem do dia (depois da Felicidade); a sequência 4 do sorteio; o **save na versão 4**; as colunas `npcs`, `populacao`, `acumulado`, `nascimentos`, `mortes` e `migracao` no relatório.

Arquivos novos: `Codigo/Vila/Populacao.h` e `.cpp`, `Codigo/Vila/RegrasDaPopulacao.h`, `Codigo/Testes/TestesDaPopulacao.cpp`; o bloco `"populacao"` no `regras.json`.

**Verificado pelo assistente:** `testes_do_codigo` passa (429 conferências); `jogo --simular 64` passa nas **sementes de 1 a 10**, com a vila em 15 e nenhuma morte (ninguém chega a 21 no ano 1, como a ficha previa); `--salvar-e-carregar-no-dia 40` dá o mesmo relatório. **10 anos com a semente 1** (`--simular 640`): 8 mortes de velhice aos 21 ou 22 anos (cerca de 0,8 por ano, como a ficha previa), cada uma com reposição; a vila fica entre 14 e 15 NPCs.

**Achado para o autor (equilíbrio, não é erro da População):** nos 10 anos, o teste dá **FALHOU no dia 400** ("o preço do Pão foi a 2,7"; o limite do teste de 1 ano é 2,6). O dia 384 é o fim de um **Inverno rigoroso**: a vila sai dele com pouco Grão (9), e na Primavera o Pão cai de 83 para 31 em 16 dias. Ninguém passa fome. É a pergunta da ficha do Clima (o rigoroso passa raspando); fica para uma rodada de equilíbrio.

### Marco A6: a Epidemia (2026-10-20, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Cura|ficha da Cura]]):

- **A Epidemia**, um evento novo do `eventos.json` (no **fim** da lista, para mudar o mínimo os sorteios dos outros): 5 por mil por dia, × 2 com a vila 90% cheia, × 2 com fome hoje, × 0,5 com Boticário ou Hospital (os **modificadores da chance**, um campo novo dos eventos); 20% dos Civis doentes por 8 dias; Felicidade −10; 1% da população por dia sai do acumulado da População, e, quando alguém morre, é o **doente mais velho** (CU1-4).
- **O Remédio:** a regra está pronta (1 a cada 10 NPCs por dia, a dose inteira ou nada; com ele, as mortes pela metade), mas o item `item.remedio` ainda não existe no conteúdo: até lá, a Epidemia roda sem Remédio.
- A Vida e a regeneração ficam para o Combate (fase D).

**Verificado pelo assistente:** `testes_do_codigo` passa (436 conferências: a chance de 10, 20 e 5 por mil, as 15 e 7 pessoas por dia, o doente mais velho que morre). **`jogo --simular 64`: 8 das 10 sementes passam; a 5 e a 8 falham** pelo preço do Pão, **sem fome**:

| Semente | O que aconteceu |
|---|---|
| 5 | Inverno rigoroso (dia 48) e uma Febre (dia 50): o Pão vai a **2,8** no dia 55 |
| 8 | Epidemia no dia 45 (3 doentes, um Cozinheiro entre eles): o Pão vai a **2,7** no dia 51 |

É o mesmo achado do marco A3 (os 10 anos): as crises novas levam o Pão acima do limite de 2,6 do teste de 1 ano. **Está na [[Rodada Pendente]] (EQ1)** para o autor decidir.

**Rodada EQ1 (2026-10-20, modo automático, sugestão A):** o teste de 1 ano só falha pelo Pão caro se ele ficar acima de 2,6 por **8 dias seguidos** (`diasSeguidosComOPaoCaroQueFalham`); abaixo de 1,2, a fome e o Banco continuam como antes. Junto com a **sugestão B** (o estoque-alvo do Pão no `regras.json` passou de 5 para **7 dias**), porque só a A ainda deixava as sementes 5 e 8 falharem (Pão a 3,0). **Resultado: 9 das 10 sementes passam.** A semente 10 (Seca, Epidemia e Ratos no celeiro juntos) e os 10 anos da semente 1 falham pelo Pão a 2,8 por mais de 8 dias, **sem fome**: depois da crise, o estoque do Pão sobe só 1 por dia, com Grão e Água de sobra. Isso está na rodada EQ2.

**Rodada EQ2 (2026-10-20, opção A do autor):** a Padaria começa no **nível 2** no `vila_inicial.json` (3 vagas; a Cidade põe um desempregado nela) e o estoque-alvo do Pão **voltou para 5 dias** (com 7, o Pão ficava barato e o Banco caía abaixo de 250 no dia 14). A capacidade da vila passou para 1100 de matéria-prima e 1000 de bem de consumo. **Verificado pelo assistente:** `testes_do_codigo` passa (436 conferências); **as 10 sementes passam**; os 10 anos da semente 1 passam; o salvar e carregar no dia 30 (semente 3) dá o relatório igual.

### Marco B1: a Viagem (2026-10-20, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Viagem|ficha da Viagem]]; os ajustes estão na parte 10.8 dela):

- **O Mapa Mundial:** `data/mundo/mapas.json` (os 48 mapas da malha 8 × 6, com região, bioma, terreno e infraestrutura) e `data/mundo/cidades_iniciais.json` (as 22 cidades do documento do Mundo; a vila da fatia é a Vila Livre do Centro, em (4,3)). O estado do território sai das cidades vizinhas (Facção, 4.1): 22 mapas com cidade, 16 de território, 6 em disputa e 4 sem dono, como no documento. **Provisório, para o autor conferir:** o terreno e a infraestrutura de cada mapa saíram do bioma da região (montanha e colinas na Guerreira, floresta na Arcana, planície na Autoritária, colinas na Religiosa, árido na Tecnocrata, litoral na Escravista) e do estado (estrada nos mapas com cidade, trilha no território, selvagem no resto).
- **`data/viagem.json`:** os números da ficha.
- **A viagem do grupo do jogador:** a rota mais rápida (Dijkstra, desempate pelo menor número), o tempo de cada trecho (fixo desde a entrada), acampar das 22 h às 6 h, a marcha forçada, 1 Pão por pessoa por dia, os encontros na metade do trecho (bandidos ou mercador viajante) e as 4 escolhas contra os bandidos (subornar, fugir, lutar pelo cálculo rápido, render-se). O grupo sai da vila com os Pães e as moedas do jogador e devolve tudo ao voltar. A vila continua rodando igual enquanto ele viaja.
- **A névoa** e o **clima por região** (a região da vila usa o clima da vila; as outras sorteiam com a sequência 9).
- **Na janela:** o botão **Mapa Mundial** na barra de cima (a malha com a névoa, o destino, os Pães e as moedas para levar, a marcha forçada e o botão Partir) e a janela **Encontro na estrada**, que pausa o jogo.
- **Save versão 5** (a seção `viagem` e os sorteios 5 e 9).

**Verificado pelo assistente:** `testes_do_codigo` passa (467 conferências: os 4 exemplos da parte 5, a caravana no inverno, a rota e o desempate, o encontro, o suborno, a fuga de 54%, as perdas, a partida e a volta à vila, o encontro na metade com o jogo pausado, o save no meio do trecho, o acampamento até as 7 h 21). **As 10 sementes passam**, e o salvar e carregar no dia 30 dá o relatório igual.

**Para o autor testar na janela:** abrir o Mapa Mundial, escolher (5,3), levar 3 Pães (comprados na loja) e partir; ver o grupo chegar, voltar para a vila e conferir que os Pães e as moedas voltaram.

### Marco B1b: as outras cidades (2026-10-21, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/OutrasCidades|ficha das Outras cidades]]; os ajustes estão na parte 10.8 dela):

- **As 21 outras cidades do mundo** (as 22 do `cidades_iniciais.json` menos a vila), cada uma uma simulação em **LOD 2**: as mesmas regras do dia da vila e a mesma produção por hora, com o **trabalho presumido** (73% de cada hora do turno, medido). Os NPCs existem (2.750 no mundo), mas não andam.
- **A criação de cada cidade:** o conjunto de produção da vila inicial a cada 15 NPCs, as Fazendas × 100 ÷ a produtividade do Grão no mapa, as casas, os NPCs (idade de 5 a 19, habilidades de 20 a 60), o estoque, o Banco pelo nível e a verba da Coroa de 5 moedas por NPC por estação.
- **`mundo/mapas.json`** ganhou os recursos de cada mapa (destaque 150, normal 100, escasso 50, pela região: Grão alto na Autoritária e baixo na Guerreira e na Tecnocrata, Madeira alta na Arcana, Pedra alta na Autoritária...). **Provisório, para o autor conferir.**
- **`regras.json`** ganhou o bloco `outras_cidades`.
- **O save** leva as 21 cidades (a seção `outrasCidades`).
- **Mais rápido:** os empregados de cada construção são montados uma vez por passo, e isso deixou a vila mais rápida também.

**Verificado pelo assistente:** `testes_do_codigo` passa (481 conferências: as contas da parte 10.2, as 21 cidades e os 2.750 NPCs, a capital com 20 Padarias e 40 Fazendas, 16 dias sem fome em nenhuma cidade, a vila igual com e sem as cidades dia a dia, o save com as 21 cidades, a vila em LOD 2 contra LOD 0). **As 10 sementes passam**, e o salvar e carregar no dia 30 dá o relatório igual.

### Marco B2: o comércio (2026-10-21, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Comercio|ficha do Comércio]]; os ajustes estão na parte 10.8 dela):

- **A loja com limite (4.7)**, para todos: paga 90% do preço local até o estoque-alvo e 45% do preço base além dele, só enquanto o Banco fica acima de 250; ao jogador, não vende abaixo de 2 dias do consumo.
- **As 14 caravanas das cidades** (2 por Posto Comercial): a sobra e a falta (4.2), a conta da viagem com os dias, o custo, o risco da rota e o lucro (4.3), a decisão de sair (4.4), a venda e a volta no destino (4.5), o salário pago à classe Civil em casa (4.6) e o ataque pelo cálculo rápido (Viagem, 4.11).
- **O jogador comercia nas outras cidades:** com o grupo parado numa cidade, o Mapa Mundial mostra a loja dela (comprar e vender 1 ou 10). O Mapa Mundial também mostra os **preços que o jogador viu** e há quantos dias (4.8).
- **Os recursos (CO1-1):** a Tecnocrata não tem floresta, e a Escravista não tem pedreira.
- **O fim do `--simular`** mostra o resumo do comércio e confere o dinheiro do mundo.
- **Save versão 6** (as caravanas, o `aCaminhoPorItem` e os preços conhecidos).

**Verificado pelo assistente:** `testes_do_codigo` passa (490 conferências: a venda de 40 Tábuas por 68 moedas, a loja que para na reserva do Banco, a força 8 da caravana, as 14 caravanas, a falta que desconta o que vai a caminho, o custo da viagem, o grupo comprando e vendendo numa outra cidade). **As 10 sementes passam**, o dinheiro do mundo se conserva em todas, e o salvar e carregar no dia 30 dá o relatório igual. As caravanas fazem de 4 a 11 viagens por ano; a perda para bandidos fica entre 0% e 9% do valor levado (média de 1,6%; a ficha previa 8%).

**Achado (rodada CO2):** em 1 ano, as 14 caravanas fazem só de 4 a 11 viagens: as cidades são autossuficientes e os preços ficam parecidos. Ver a [[Rodada Pendente]].

**Rodada CO2 (2026-10-21, opção A do autor):** onde o Grão é escasso (Guerreira e Tecnocrata), as Fazendas cobrem só 75% do consumo; onde é normal ou de destaque, a cidade fica com o conjunto inteiro (a capital Guerreira passou de 40 para 30 Fazendas). **Verificado pelo assistente:** `testes_do_codigo` passa (490 conferências); **as 10 sementes passam**, o dinheiro do mundo se conserva, nenhuma das 21 cidades está com fome no fim do ano, e o salvar e carregar dá o relatório igual. As caravanas passaram de 4 a 11 para **6 a 15 viagens por ano**; o comércio ainda é pequeno, e a opção B da rodada (a caravana mais barata) continua disponível se o autor quiser mais.

### Marco B3: as notícias (2026-10-24, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Noticias|ficha das Notícias]]; os ajustes estão na parte 10.8 dela):

- **As notícias nascem** dos eventos das 22 cidades (Seca, Fome e Epidemia são Regionais; Geada, Colheita farta e Festa da Colheita, Locais; Inverno rigoroso, Mundial), das caravanas atacadas e dos preços fora da média da região (no máximo 1 por cidade por dia). O arquivo novo é o `data/noticias.json`; a importância de cada evento está no `eventos.json` (`"noticia"`).
- **Elas andam 3 horas por mapa** até o alcance da importância, e o jogador sabe quando chegam ao mapa do grupo, com a precisão da distância ("1.880", "cerca de 2.000", "milhares de").
- **Na tela:** as Regionais e Mundiais viram notificação; o botão **Notícias** mostra as que o jogador sabe e a **Crônica do mundo**; o Mapa Mundial mostra as notícias de cada mapa e há quantos dias o jogador soube.
- **O fim do `--simular`** mostra quantas notícias nasceram.
- **Save versão 7** (as notícias, o que o jogador sabe e a Crônica).

**Verificado pelo assistente:** os testes passam (502 conferências); as 10 sementes passam com as mesmas viagens de caravana de antes (as Notícias não mudam o jogo); de 255 a 289 notícias por ano, de 18 a 27 na Crônica do jogador; salvar e carregar no dia 30 dá o mesmo relatório.

### Marco B4: a reputação (2026-10-24, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Reputacao|ficha da Reputação]]; os ajustes estão na parte 10.8 dela):

- **A reputação com as 6 facções e as Vilas Livres**, de −100 a +100, com as 5 faixas (Inimigo a Aliado) do arquivo novo `data/reputacao.json`. Começa em 0 (a base, até o Início de Jogo).
- **Os preços das lojas das outras cidades** mudam pela faixa (Hostil compra a 115% e vende a 85%; Aliado, 90% e 110%); o Inimigo não negocia.
- **Duas ações:** vencer um bando no território de uma facção (+1) e **doar** numa cidade (+1 a cada 100 moedas, até a base + 20; o dinheiro vai ao Banco dela).
- **A volta:** no dia 1 de cada estação, cada valor anda 2 pontos para a base.
- **Na tela:** o botão **Reputação** mostra o valor, a base e a faixa de cada um, com o "Por quê?" ao passar o mouse; a loja da cidade mostra a faixa e tem os botões de doar.
- **Save versão 8** (os valores, as bases e as últimas mudanças).

**Verificado pelo assistente:** os testes passam (512 conferências); as 10 sementes passam com o relatório igual ao do B3 (a reputação não muda nada sem o jogador agir); salvar e carregar no dia 30 dá o mesmo relatório.

### Marco B5: a tecnologia (2026-10-24, programado com o Opus a pedido do autor; rodada TE2, opção A)

O que entrou ([[Ficha Logica/Tecnologia|ficha da Tecnologia]]; os ajustes estão na parte 10.8 dela):

- **O Laboratório** nas 6 capitais e na Cidade das minas de cobre (3,6), com 5 pesquisadores cada. Os pontos vão para a facção dona.
- **A pesquisa:** cada facção escolhe o projeto pela cultura, paga o custo com a difusão (−10% por facção que já tem) e, ao completar, gera uma notícia Regional.
- **Os marcos mundiais:** o dia é sorteado na janela ao criar o mundo (sequência 6) e fica mais cedo com as facções que já têm as tecnologias ligadas; ao chegar, abre a era e gera uma notícia Mundial.
- **As travas** nas receitas das cidades, pela facção dona.
- **Conteúdo provisório** (`data/tecnologias.json` e `data/marcos.json`): 3 tecnologias e 1 marco de exemplo, que não liberam nada. As listas de verdade vêm na rodada de conteúdo C1. Até os Ricos (A4), a habilidade Especialista é a Manufaturado.
- **Na tela:** o botão **Tecnologia** mostra a era, o projeto, os pontos e as tecnologias de cada facção, e os marcos. O fim do `--simular` mostra o resumo.
- **Save versão 9.**

**Verificado pelo assistente:** os testes passam (527 conferências); as 10 sementes passam com o relatório da vila igual ao do B4; no fim do ano, as 6 facções têm as 3 tecnologias de exemplo (18), nenhuma cidade com fome e o dinheiro do mundo conservado; o comércio mudou um pouco (de 4 a 21 viagens), porque as capitais pagam 5 pesquisadores; salvar e carregar no dia 30 dá o mesmo relatório.

### Marco C1: as facções (2026-10-24, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Faccao|ficha da Facção]]; os ajustes estão na parte 10.8 dela):

- **As 6 facções** com as leis iniciais da tabela do documento (arquivo novo `data/faccoes.json`) e um líder sorteado (sequência 7).
- **O imposto pela lei** (Baixo 10%, Normal 20%, Alto 30%) e a Felicidade dela (0, −5, −10); o teto do gasto da classe passa a ser 100% menos a taxa (FA1-2).
- **O Banco da Facção:** começa com 25% do PIB, recebe a cunhagem (140 moedas por dia por capital) e as tarifas, e paga a verba de 5 moedas por NPC às cidades dela no dia 1 de cada estação. As Vilas Livres e a vila do jogador continuam com a verba "de fora".
- **A tarifa** das caravanas que entram no território de outra facção, também na conta do lucro esperado.
- **Os itens proibidos e o ajuste de preço** da facção; **a assembleia** no dia 1 de cada ano, com a pauta, o voto, o sistema de votação, o veto do líder e a notícia Regional.
- **O desejo das leis de cada cultura** num `data/culturas.json` provisório (a Cultura, C2, completa).
- **Na tela:** o botão **Facções** mostra as leis, o Banco e o líder de cada uma. O fim do `--simular` mostra o Banco de cada facção.
- **Save versão 10.**

**Verificado pelo assistente:** os testes passam (541 conferências); as 10 sementes passam com o relatório da vila igual ao do B5; nenhuma cidade com fome e o dinheiro do mundo conservado (a cunhagem conta como dinheiro de fora); os Bancos das facções terminam o ano entre 8.000 e 8.600 moedas; o comércio caiu um pouco com as tarifas (de 3 a 13 viagens por ano); salvar e carregar no dia 30 dá o mesmo relatório.

### Marco C2: a cultura (2026-10-24, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Cultura|ficha da Cultura]]; os ajustes estão na parte 10.7 dela):

- **As 6 culturas** completas no `data/culturas.json` (alguns números são provisórios, para revisar).
- **A cultura de cada NPC:** a da cidade no começo e no nascimento; o migrante vem da região (75%) ou de uma região vizinha (25%). A vila do jogador é da cultura da região (Autoritária).
- **O 5º fator da Felicidade, Leis × cultura** (de −30 a +10), com o extremo e as minorias hostis. No começo, é 0 em todas as cidades.
- **Os pontos de habilidade** da cultura nos NPCs das outras cidades.
- **Na tela:** a janela **Facções** diz a cultura dos moradores da vila.
- **Save versão 11** (a cultura de cada NPC).

**Verificado pelo assistente:** os testes passam (551 conferências); as 10 sementes passam com o relatório da vila igual ao do C1; nenhuma cidade com fome e o dinheiro do mundo conservado; o comércio fica entre 4 e 11 viagens por ano; salvar e carregar no dia 30 dá o mesmo relatório.

### Marco C3: o crime e a justiça (2026-10-24, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Crime|ficha do Crime e Justiça]]; os ajustes estão na parte 10.8 dela):

- **A Criminalidade de cada cidade** (de 0 a 100; arquivo novo `data/crimes.json`): sobe com a infelicidade, o desemprego e a pobreza; acima de 20 tira Felicidade; com 25 ou mais, pode haver furto no armazém; e põe mais bandidos nas estradas da região.
- **A revista** na entrada das cidades de facção (a chance vem da faixa da Reputação): acha os itens proibidos e a falta do Livro Sagrado. O jogador **paga** a multa, **suborna** ou **foge**; cada crime tira reputação.
- **A Recompensa** por facção (cai 10% por estação), a **prisão** (os dias ou a fiança) e os **Caçadores de Recompensa** a partir de 500 moedas.
- **Na tela:** a janela da revista, a prisão com o botão da fiança na barra de cima, os Caçadores no encontro, e a Recompensa na janela da Reputação. O fim do `--simular` mostra a Criminalidade do mundo.
- **Save versão 12.**
- **Ficou para depois:** a loja ilegal do jogador (nenhum item é proibido ainda).

**Verificado pelo assistente:** os testes passam (563 conferências); as 10 sementes passam com o relatório da vila igual ao do C2 (a Criminalidade da vila fica em 12,6: abaixo de 20 e de 25); no mundo, a média fica em cerca de 12 e a maior em 16 a 18; nenhuma cidade com fome e o dinheiro do mundo conservado; salvar e carregar no dia 30 dá o mesmo relatório. A revista, a prisão e os Caçadores estão testados nos testes do código, não na janela.

### Marco C4: a religião (2026-10-24, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Religiao|ficha da Religião]]; os ajustes estão na parte 10.8 dela):

- **A fé de cada cultura** (arquivo novo `data/religiao.json`) e **os Templos** (a Torre Arcana na Arcana) nas cidades de facção, com a cobertura de 60% ou mais (RL1-1).
- **O 6º fator da Felicidade, Religião** (de −5 a +10), com a minoria sem culto e a Perseguição.
- **As oferendas** (2% da renda vão ao Banco), **os festivais** do calendário (+8 por 8 dias; o turno pela metade no primeiro dia; a comida extra só da sobra; notícia Local), **a Tolerância** (o imposto a mais das minorias na Fé oficial) e **a assimilação** das minorias.
- **O Templo da Religiosa na Epidemia** (as mortes × 75%), **a peregrinação** do jogador na Capital Religiosa (+10 de reputação, uma vez por ano) e **a Bênção** do Sumo Sacerdote.
- **Na tela:** o botão da peregrinação no painel da loja, na Capital Religiosa.
- **Save versão 13.**

**Verificado pelo assistente:** os testes passam (575 conferências); as 10 sementes passam com o relatório da vila igual ao do C3; o dinheiro do mundo se conserva; salvar e carregar no dia 30 dá o mesmo relatório. **Achado:** em 2 das 10 sementes, uma cidade termina o ano com fome curta (de 1 a 3 dias): a Vila Livre do Sul (5,6) ou o Entreposto do leste (4,5), as duas sem Templo e numa região com pouco Grão. Sem a comida extra do festival ou sem o turno pela metade, a semente 1 continua igual; a causa provável é indireta (as cidades de fé mais felizes crescem e sobra menos comida para as caravanas). Vale olhar numa rodada se aparecer de novo.

### Marco D1: o combate (2026-10-24, programado com o Opus a pedido do autor)

O que entrou ([[Ficha Logica/Combate|ficha do Combate]]; os ajustes estão na parte 10.9 dela):

- **A fórmula do golpe** (o dano da arma × o Rank × a habilidade, a vantagem contra a armadura, a Defesa, o mínimo de 1), **o acerto a distância**, **o Medo** e **a evolução**, com as armas, as armaduras e os kits no arquivo novo `data/combate.json`.
- **O duelo simulado** (sem o mapa) e **o cálculo rápido**, que substitui a força simples na estrada, nas caravanas e nos Caçadores.
- **A Vida do grupo e das caravanas** (o vencedor perde Vida; ela volta 1% por hora).
- **Na tela:** o encontro mostra a Vida do grupo e a chance de vencer pelo cálculo rápido.
- **Save versão 14.**
- **Ficou para depois:** o combate ao vivo no Mapa Local (não há hostis nem personagens controlados nos mapas ainda).

**Verificado pelo assistente:** os testes passam (592 conferências); as 10 sementes passam com o relatório da vila igual ao do C4; o dinheiro do mundo se conserva; salvar e carregar no dia 30 dá o mesmo relatório. As caravanas perdem mais lutas agora (de 0 a 3 por ano). A fome curta do C4 continua em 2 sementes.

**Rodada CB2 (opção C, 2026-10-25):** o cálculo rápido agora **roda o combate simulado**, com o Medo (regra 4.8 do [[Ficha Logica/Combate|Combate]]). O botão Lutar mostra a chance de 100 lutas simuladas. Nos testes: 1 jogador contra 2 bandidos vence 97% (antes 46%); contra 6 bandidos, o jogador e a caravana perdem sempre. A rodada HI3-3 deixou a Tecnocrata hostil à Guerreira e à Religiosa (`culturas.json`).

Como testar: `testes_do_codigo` imprime a linha "por mil: contra 1 bandido ..." na parte do Combate.

### Os líderes fixos do Ano 412 (2026-10-26, rodada HI7-1)

Cada facção começa com o líder da história (Mundo e História, parte 4.7): nome, idade e Personalidade no `faccoes.json` (campo `lider`). No jogo, a idade é a da história ÷ 4, de 8 a 18 (Cassius Varro, com 131, fica no teto de 18 até a rodada HI11). A Agressividade e a Sociabilidade valem já (o veto da assembleia); os outros 3 traços esperam os Companheiros (D2). O save não muda.

Como testar: na janela das Facções, passe o mouse sobre o nome de uma facção: a dica mostra o líder ("Líder: Grande Lobo Tian Feng, 13 anos, ...").
