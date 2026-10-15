> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Arquitetura

**Descrição geral**

Decisões técnicas da engine em C++. O objetivo do jogo é ser **extremamente eficiente**, rodando com facilidade em qualquer PC, então as escolhas priorizam desempenho e bom gerenciamento de memória. A aprovação formal das bibliotecas fica para o início do código.

### Ponto de partida

- **Experiência do autor:** C++ apenas na disciplina de orientação a objetos, cursada há cerca de 4 meses, e nenhuma experiência com bibliotecas. Isso é aceitável: o código será escrito em conjunto, e este documento serve para o autor entender e aprovar as decisões antes de elas virarem código.
- **Ferramentas já instaladas no PC:** g++ 15.2.0 (MinGW-W64, UCRT), CMake 4.3.0, Ninja 1.13.2, Git 2.53 e Python 3.11. **Não há** Visual Studio (MSVC) nem vcpkg.
- **[Proposta]** Usar o g++ (MinGW) que já existe, com CMake e Ninja, sem instalar o Visual Studio. Linguagem: **C++20**.

### Princípios

1. **Dados contíguos, sem alocação por frame.** Reservar memória no início (por Mapa Local) em vez de criar e destruir objetos o tempo todo.
2. **IDs em vez de ponteiros** para entidades (NPC, cidade, facção, mapa, item). Evita ponteiros inválidos e facilita salvar e carregar.
3. **Tudo que é conteúdo vira dado, não código.** Itens, receitas, construções, culturas, leis e estilos de início ficam em arquivos, lidos no início.
4. **Simulação separada do desenho.** A lógica do jogo não depende da biblioteca gráfica. Isso permite medir a simulação sozinha.
5. **Medir antes de otimizar.** Perfilar com ferramenta, não por suposição.

### Bibliotecas

**[Proposta]** Todas leves, de código aberto e com bom desempenho. Nenhuma foi instalada ainda.

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

**Como instalar:** baixadas pelo próprio CMake (`FetchContent`) em versões fixas, sem precisar de vcpkg.

### Como os documentos viram código

Os sistemas foram escritos com herança (por exemplo, Inventário abstrato que gera os inventários especializados). Para desempenho com milhares de NPCs, **[Proposta]** usar **composição**:

- **NPCs e Esquadrões** viram entidades do ECS, com componentes (Identidade, Personalidade, Habilidades, Inventário, Rotina...). O "Inventário abstrato" vira um componente reutilizado.
- **Cidades, Facções, Culturas e Mapas** são poucos objetos. Ficam em listas simples (`vector`) acessadas por ID, sem ECS.
- O que os documentos chamam de "derivado" (PIB, conscritos, estado de território, facção do Mapa Local) é calculado sob demanda e guardado em cache, atualizado quando a origem muda.

### Dados

- **[Proposta]** Formato **JSON** em uma pasta `data/`, com IDs de texto legíveis (por exemplo, `item.espada_curta_ferro`), convertidos em números ao carregar.
- Tipos de arquivo: itens, receitas, construções, culturas, traços, leis, estilos de início, crimes, eventos e narradores.
- **Validação ao carregar:** o jogo avisa se um item citado numa receita não existe. Isso evita as divergências de nome que apareceram nos canvases.
- Os documentos do Obsidian e os canvases continuam sendo a documentação, não o dado do jogo.

### Simulação e tempo

- **Passo de tempo fixo** (a frequência será definida em testes) e desenho independente.
- **Velocidades 1x, 2x, 4x e 8x** aumentam a quantidade de ticks por frame, não o tamanho de cada tick. A 8x vale só no Mapa Mundial; nos outros mapas o máximo é 4x.
- **Agendador por LOD:** cada Mapa Local roda no nível dele (0, 1 ou 2), com a frequência já definida no Sistema de Tempo.
- **Combate** no passo fixo, com grade espacial para achar alvos e reavaliação de alvo a cada poucos ticks (ver [[Sistema de Combate/Descrição|Sistema de Combate]]).
- **Números aleatórios** com semente: permite reproduzir bugs e testar. O Narrador e os eventos usam um gerador próprio (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]).

### Memória

- **[Proposta]** Arenas de memória por Mapa Local: tudo que pertence a um mapa é alocado junto e liberado junto.
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

- **[Proposta]** Arquivo binário com número de versão.
- Salvar por Mapa Local. Mapas em LOD 2 guardam só os dados agregados (população, estoques, Criminalidade).
- Estado de facções, diplomacia e relações em seções próprias.

### Desempenho

- Alvo: rodar em PCs modestos.
- **[Em aberto]** Definir o **PC alvo mínimo** (processador, memória, placa gráfica) e a meta de quadros por segundo (**[Proposta]** 60).
- **[Em aberto]** Meta de NPCs simultâneos em LOD 0, a medir.

### Ordem de trabalho

**Decisão do autor:** o foco agora é **100% na documentação**, sem nenhuma linha de código. Quando a documentação estiver satisfatória, será feita uma **ficha técnica lógica** das possibilidades (como cada sistema vira estruturas, dados e regras no código). Só depois dela começa a programação. Os marcos abaixo continuam como plano, mas não serão iniciados antes disso.

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
- Sem guerra, facções, crime nem magia.

**[Proposta]** Marcos, nesta ordem:

1. Janela, loop principal, relógio fixo e desenho de um mapa de tiles.
2. Câmera e entrada do jogador.
3. Carregar itens, receitas e construções dos arquivos de dados.
4. NPC se movendo na grade (busca de caminho).
5. Rotina diária dos NPCs.
6. Produção, inventário da cidade e consumo.
7. Interface básica (inventário, informações da cidade).
8. Salvar e carregar.

### Arte e mundo (provisório)

- **Mapa Local:** 128 × 128 tiles de 16 px (a ajustar após testes de desempenho).
- **Mundo:** 48 Mapas Locais (8 × 6), em 6 regiões de 8 mapas (ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- O desenho pequeno é uma escolha consciente: um jogo de gerenciamento precisa mostrar muita coisa na tela.

### Perguntas em aberto

1. **PC alvo mínimo** e meta de quadros por segundo.
2. **Aprovação final das bibliotecas** (início do código).
3. **Tamanho do Mapa Local** (128 × 128): a confirmar com protótipo. Tile (16 px), vista (3/4) e mundo (48 mapas, malha ajustável pelos dados) já decididos.
4. **Repositório Git do código:** criar quando o código começar, separado desta pasta de documentos.
