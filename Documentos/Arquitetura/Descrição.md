> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Arquitetura

**Descrição geral**

Decisões técnicas da engine em C++. O objetivo do jogo é ser **extremamente eficiente**, rodando com facilidade em qualquer PC, então as escolhas priorizam desempenho e bom gerenciamento de memória. A aprovação formal das bibliotecas fica para o início do código.

### Ponto de partida

- **Experiência do autor:** C++ apenas na disciplina de orientação a objetos, cursada há cerca de 4 meses, e nenhuma experiência com bibliotecas. Isso é aceitável: o código será escrito em conjunto, e este documento serve para o autor entender e aprovar as decisões antes de elas virarem código.
- **Ferramentas já instaladas no PC:** g++ 15.2.0 (MinGW-W64, UCRT), CMake 4.3.0, Ninja 1.13.2, Git 2.53 e Python 3.11. **Não há** Visual Studio (MSVC) nem vcpkg.
- Usar o g++ (MinGW) que já existe, com CMake e Ninja, sem instalar o Visual Studio. Linguagem: **C++20**.

### Princípios

1. **Dados contíguos, sem alocação por frame.** Reservar memória no início (por Mapa Local) em vez de criar e destruir objetos o tempo todo.
2. **IDs em vez de ponteiros** para entidades (NPC, cidade, facção, mapa, item). Evita ponteiros inválidos e facilita salvar e carregar.
3. **Tudo que é conteúdo vira dado, não código.** Itens, receitas, construções, culturas, leis e estilos de início ficam em arquivos, lidos no início.
4. **Simulação separada do desenho.** A lógica do jogo não depende da biblioteca gráfica. Isso permite medir a simulação sozinha.
5. **Medir antes de otimizar.** Perfilar com ferramenta, não por suposição.

### Bibliotecas

Todas leves, de código aberto e com bom desempenho. **Aprovadas na rodada FT2-2**: cada uma é instalada no marco em que é usada (ver abaixo da tabela).

| Função | Biblioteca | Por quê |
|---|---|---|
| Janela, teclado, mouse | **SDL3** | Leve e madura; funciona com o g++ MinGW |
| Desenho | **OpenGL 3.3** (via glad) | Funciona em PCs antigos; nosso renderer próprio por cima |
| Carregar imagens | **stb_image** | Um arquivo só |
| Áudio | **miniaudio** | Um arquivo só |
| Ferramentas de depuração | **Dear ImGui** | Painéis de debug dentro do jogo |
| Entidades (NPCs etc.) | **EnTT** | ECS muito rápido, só cabeçalhos |
| Arquivos de dados | **nlohmann/json** | Simples de ler; troca por algo mais rápido se virar gargalo |
| Medição de desempenho | **Tracy** | Mostra onde o tempo é gasto |

**Quando entra cada uma** (FT2-2):

| Marco | Bibliotecas |
|---|---|
| 0 e 1 | SDL3, glad (OpenGL 3.3), stb_image |
| 3 | nlohmann/json (o CSV é lido por código próprio) |
| 4 e 5 | EnTT |
| 7 | Tracy |
| 8 | Dear ImGui |
| Depois da fatia | miniaudio |

**Como instalar:** baixadas pelo próprio CMake (`FetchContent`) em versões fixas, sem precisar de vcpkg.

### Como os documentos viram código

Os sistemas foram escritos com herança (por exemplo, Inventário abstrato que gera os inventários especializados). Para desempenho com milhares de NPCs, usar **composição**:

- **NPCs e Esquadrões** viram entidades do ECS, com componentes (Identidade, Personalidade, Habilidades, Inventário, Rotina...). O "Inventário abstrato" vira um componente reutilizado.
- **Cidades, Facções, Culturas e Mapas** são poucos objetos. Ficam em listas simples (`vector`) acessadas por ID, sem ECS.
- O que os documentos chamam de "derivado" (PIB, conscritos, estado de território, facção do Mapa Local) é calculado sob demanda e guardado em cache, atualizado quando a origem muda.

### Dados

- Formato **JSON** em uma pasta `data/` para os dados com estrutura, e **CSV** para as tabelas grandes e simples (itens, receitas, preços), editadas em planilha (rodada V4-6). IDs de texto legíveis (por exemplo, `item.espada_curta_ferro`), convertidos em números ao carregar.
- Tipos de arquivo: itens, receitas, construções, culturas, traços, leis, estilos de início, crimes, eventos e narradores.
- **Moldes de conteúdo:** a ficha de cada tipo, para copiar e modificar, está em [[Arquitetura/Moldes de Conteúdo|Moldes de Conteúdo]] (ideia do autor, rodada V3-13).
- **Validação ao carregar:** o jogo avisa se um item citado numa receita não existe. Isso evita as divergências de nome que apareceram nos canvases.
- Os documentos do Obsidian e os canvases continuam sendo a documentação, não o dado do jogo.

### Simulação e tempo

- **Passo de tempo fixo** (a frequência será definida em testes) e desenho independente.
- **Velocidades 1x, 2x, 4x e 8x** aumentam a quantidade de ticks por frame, não o tamanho de cada tick. A 8x vale só no Mapa Mundial; nos outros mapas o máximo é 4x.
- **Agendador por LOD:** cada Mapa Local roda no nível dele (0, 1 ou 2), com a frequência já definida no Sistema de Tempo.
- **Combate** no passo fixo, com grade espacial para achar alvos e reavaliação de alvo a cada poucos ticks (ver [[Sistema de Combate/Descrição|Sistema de Combate]]).
- **Números aleatórios** com semente: permite reproduzir bugs e testar. O Narrador e os eventos usam um gerador próprio (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]).

### Memória

- Arenas de memória por Mapa Local: tudo que pertence a um mapa é alocado junto e liberado junto.
- NPCs e itens em arrays contíguos; evitar `new` dentro de loops de simulação.
- IDs com "geração" (número de versão), para detectar uso de entidade já destruída.

### Desenho

- **Tile de 16 px**, ampliado na tela em escala inteira; vista de cima em 3/4 (como Stardew); boneco de 16 × 24 px; zoom padrão 2x (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
- Atlas de texturas e desenho em lote, para poucas chamadas de desenho.
- Desenhar só o que está na câmera (culling).
- Como cada cultura tem arte própria de construção, as partes (paredes, telhados, portas) devem ser peças reaproveitáveis.
- Personagens em **camadas** (corpo, 3 peças de armadura, arma, escudo) sobre animações compartilhadas por tipo de arma, em 4 direções (ver [[Sistema de Combate/Descrição|Sistema de Combate]]).
- **Interface** desenhada pelo nosso próprio desenhista, em resolução nativa e separada do mundo, com layouts em JSON e perfil do jogador em arquivo próprio; o Dear ImGui fica só para depuração. Resolução de referência 1600 × 900 (ver [[Interface/Descrição|Interface e Controles]]).

### Salvar e carregar

- Arquivo binário com número de versão.
- Salvar por Mapa Local. Mapas em LOD 2 guardam só os dados agregados (população, estoques, Criminalidade).
- Estado de facções, diplomacia e relações em seções próprias.

### Desempenho

- Alvo: rodar em PCs modestos.
- **Meta de quadros:** 60 por segundo em 1600 × 900 (rodada V2-11). **PC alvo mínimo** (rodada V3-12): processador de 4 núcleos (2015 ou mais novo), 8 GB de memória, placa de vídeo com OpenGL 3.3 e 2 GB.
- **Meta de NPCs** (rodada V2-11): 500 em LOD 0 e 5.000 em LOD 1 a 4x, num PC de gama média; a medir na fatia vertical.
- **Vários núcleos (multithreading): objetivo futuro** (decisão do autor, 2026-10-17). A fatia roda num núcleo só. Depois dela, se o Tracy mostrar que falta tempo, a ideia é rodar cada Mapa Local numa thread, sem perder "mesma semente = mesma partida".
- **Dinheiro em décimos inteiros** (rodada V2-1): 1,7 moedas = 17. Nada de número com vírgula no dinheiro.

### Ordem de trabalho

**Decisão do autor:** primeiro a documentação, depois a ficha lógica e só então o código. Em 2026-10-17 (rodada FT2-1), com a ficha da fatia vertical pronta, o código foi liberado, **marco a marco**: um problema de design achado no código volta como pergunta numa rodada.

**Guerra:** depois que o mundo funcionar, a guerra entra primeiro pelo **cálculo rápido de forças** (o mundo já tem guerras e o jogador pode resolvê-las automaticamente). O **Mapa de Guerra** vem depois, como um marco próprio, porque é um jogo de estratégia inteiro (ver [[Sistema de Guerra/Descrição|Sistema de Guerra]] e [[Sistema de Batalha/Descrição|Sistema de Batalha]]).

### Ambiente de programação (rodada FT1-7)

- **Editor:** VS Code, com as extensões grátis "C/C++" e "CMake Tools" (autocompletar, botão de compilar, depurador).
- **Depurador:** gdb, que vem com o MinGW: para o programa numa linha e mostra os valores.
- **Primeiro código:** um "olá, janela" com o SDL3, compilado pelo CMake (marco 0 da [[Ficha Logica/Fatia Vertical|Ficha Lógica]]).
- **Pastas** (organização de 2026-10-17), todas no mesmo repositório:
  - `Documentos/`: o design;
  - `Ficha Logica/`: cada sistema virado lógica de programação, feito pouco antes do marco que o usa;
  - `Codigo/`: as regras em C++, uma biblioteca que **nunca abre janela nem desenha**. Assim o teste sem tela (`--simular`) roda só com ela, e cada regra pode ser testada sozinha;
  - `Jogo/`: o programa (janela, desenho, entrada, interface), que chama o `Codigo/`; também guarda `data/` e `assets/`;
  - `CMakeLists.txt` na raiz, que junta as duas; a pasta `build/` fica fora do Git e do Obsidian.

### Jeito de trabalhar e nomes no código (rodadas FT2-3 e FT2-4)

- **Um marco por vez:** cada marco tem uma branch e um pull request.
  - O assistente escreve em passos pequenos, e cada passo compila e roda.
  - O autor compila e roda no PC dele, com os comandos prontos, e conta o que viu.
  - Cada marco termina com um resumo simples em `Jogo/LEIA-ME.md`.
- **Nomes em português, sem acento:** `Cidade`, `Construcao`, `precoBase`. Os comentários também são em português.
  - **Ideia do autor:** nenhum nome abstrato demais. O nome da variável, da função ou da classe diz o motivo dela: `horasTrabalhadasNoCiclo`, não `h` nem `valor`; `calcularPrecoBaseDosItens`, não `processar`.
  - As palavras do C++ (`int`, `vector`, `for`) e os nomes das bibliotecas continuam como são.

### Ferramentas de conteúdo

- **Mapas:** serão desenhados no **Tiled**, um editor de mapas de tiles gratuito e de código aberto (<https://www.mapeditor.org>). Ele não faz parte do jogo: é só a ferramenta de desenho. Cada mapa é salvo em um arquivo JSON, que o jogo lê. O Tiled permite camadas (chão, objetos, colisão) e pontos marcados no mapa, como lotes de construção, portas e pontos de entrada.
- **Dados:** os arquivos JSON de `data/` (itens, eventos, tecnologias...) podem ser editados em um editor de texto comum, com validação ao carregar.
- **Arte:** desenhada no Pixelorama (grátis), com pacotes de terceiros provisórios na fatia vertical. Volume, ordem de produção e peso da arte na memória estão no [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]].
- **Validação dos dados em C++**, pelo próprio jogo (inclusive um modo `--validar`); tabelas grandes em CSV, lidas direto pelo jogo.

### Primeira meta: fatia vertical

Uma versão mínima jogável, só para validar a parte mais arriscada: a simulação com LOD e a economia.

- 1 cidade, 1 cadeia de produção (Fazenda → Cozinha → Mercado).
- NPCs com rotina (dormir, trabalhar, livre), consumo e Felicidade.
- 1 personagem controlável.
- Produção por estação e tom de dia e noite (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- Sem guerra, facções, crime nem magia.

**Números da fatia vertical** (rodada V2-12):

| Construção | Vagas | Produz por dia |
|---|---|---|
| Fazenda nível 1 | 2 | 32 Grão (8 no inverno) |
| Posto de Coleta nível 1 | 2 | 16 Água e 16 Tora |
| Cozinheiro nível 1 | 2 | 16 Pães |
| Carpinteiro nível 1 | 2 | Tábuas e, no nível 2, Móveis de Madeira |
| Mercado | 2 Lojistas | — |
| Construtor | 2 Operários | — |
| Casas nível 1 | 4 Casas | 16 lugares |

- **Vila:** nível 1, com 15 NPCs reais Civis e Banco de 400.
- **Comida:** 15 Pães por dia, que gastam 7,5 Grão e 3,75 Água.
- **Dinheiro por dia:** entram cerca de 85 em salários; saem cerca de 25 da cesta mínima; até 68 voltam às lojas pela regra do gasto.
- **O teste passa se**, em 1 ano de jogo sem tela, a vila não passa fome, o Banco não zera e o preço do Pão fica entre 1,2 e 2,6.

Marcos, nesta ordem (rodada V4-6):

1. Janela, loop principal, relógio fixo e desenho de um mapa de tiles.
2. Câmera e entrada do jogador.
3. Carregar itens, receitas e construções dos arquivos de dados.
4. NPC se movendo na grade (busca de caminho).
5. Rotina diária dos NPCs.
6. Produção, inventário da cidade e consumo.
7. **Simulação sem tela de 1 ano** (o teste da V2-12: a vila não passa fome, o Banco não zera e o Pão fica entre 1,2 e 2,6).
8. Interface básica (inventário, informações da cidade).
9. Salvar e carregar.

### Arte e mundo (provisório)

- **Mapa Local:** 128 × 128 tiles de 16 px (a ajustar após testes de desempenho).
- **Mundo:** 48 Mapas Locais (8 × 6), em 6 regiões de 8 mapas (ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- O desenho pequeno é uma escolha consciente: um jogo de gerenciamento precisa mostrar muita coisa na tela.

### Perguntas em aberto

1. **PC alvo mínimo** e meta de quadros por segundo.
2. **Aprovação final das bibliotecas** (início do código).
3. **Tamanho do Mapa Local** (128 × 128): a confirmar com protótipo. Tile (16 px), vista (3/4) e mundo (48 mapas, malha ajustável pelos dados) já decididos.
4. **Repositório Git do código:** criar quando o código começar, separado desta pasta de documentos.
