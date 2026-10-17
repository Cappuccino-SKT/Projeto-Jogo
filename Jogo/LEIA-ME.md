# Jogo

Aqui fica **o programa de verdade**: abre a janela, desenha o mapa e os NPCs, lê o mouse e o teclado, mostra a interface. As regras não ficam aqui: o programa chama a pasta `Codigo/` para saber o que acontece.

Também ficam aqui:

- `data/`: os arquivos que o jogo lê (itens, receitas, regras, a vila inicial);
- `assets/`: a arte e os mapas (Pixelorama e Tiled).

## Como compilar e rodar

**Pelo terminal** (PowerShell ou Git Bash, na pasta do projeto):

1. Preparar a pasta `build/`. Só precisa na primeira vez, ou quando um `CMakeLists.txt` mudar. Na primeira vez, o CMake baixa a SDL3 e demora uns 2 minutos:

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

**Pelo VS Code**, com as extensões "C/C++" e "CMake Tools":

1. Abrir a pasta do projeto. O CMake Tools pergunta qual configuração usar: escolher **"Padrão (g++, Ninja, com depuração)"**.
2. **F7** compila.
3. **Shift+F5** roda. **Ctrl+F5** roda com o depurador (gdb), que para nas linhas marcadas com um ponto vermelho.

**Os arquivos da compilação:**

| Arquivo | O que faz |
|---|---|
| `CMakeLists.txt` (na raiz) | A receita principal: C++20, baixa as bibliotecas e junta as pastas |
| `CMakePresets.json` (na raiz) | A configuração "padrao": g++, Ninja, pasta `build/`, com depuração |
| `Jogo/CMakeLists.txt` | Diz que o `jogo.exe` é feito do `main.cpp` e usa a SDL3 |
| `build/` | Tudo o que o compilador gera. Fica fora do Git e do Obsidian, e pode ser apagada: o passo 1 a recria |

## O que cada marco trouxe

### Marco 0: o ambiente e o "olá, janela" (2026-10-17)

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

**O que vem no marco 1:** a pasta `Codigo/` ganha o relógio do mundo ([[Ficha Logica/Tempo|ficha do Tempo]]), e a janela passa a desenhar o mapa do Tiled ([[Ficha Logica/Mapa|ficha do Mapa]]) com o OpenGL.
