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

Os dois problemas de equilíbrio (a Febre no Cozinheiro e os Móveis caros) estão na **rodada FE1** ([[Rodada Pendente]]).

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
3. `.\build\Jogo\jogo.exe --simular 64 --semente 3` termina com "FALHOU: o preço do Pão foi a 2,7 no dia 31" (é a pergunta FE1-2).
4. Abrir o jogo: o boneco de teste aparece sem o aviso do Mana Seed. Deixar correr uns 3 dias a 4x e ler o resumo do dia: o Pão sobe de 1,9 para 2,3 nos primeiros dias (a vila começa com só 3 dias de Pão), e o gasto extra aparece a partir do dia 2.
5. Apertar **F4** depois de alguns dias: o preço local e o consumo previsto de cada item; em algum dia, uma construção "DE FOLGA HOJE".
6. `ctest --test-dir build` termina com "100% tests passed, 0 tests failed out of 3".
