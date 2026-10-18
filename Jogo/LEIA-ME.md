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

4. Rodar os testes automáticos das regras (rodada FL1-4). Termina com "100% tests passed" se tudo passou:

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

**O que vem no marco 2:** a câmera que anda (WASD, setas, botão do meio) e o zoom de 2x a 4x pela roda do mouse, depois da ficha da Interface.
