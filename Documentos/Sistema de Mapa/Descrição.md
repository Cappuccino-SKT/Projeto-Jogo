> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Mapa

**Descrição geral**

Toda a parte gráfica é feita à mão: nada é procedural ou aleatório. O mapa é separado em **4 tipos**: 3 mapas de escala (Mundial, Local e Interno) e 1 mapa de situação (Guerra), cada um com sua própria função e importância. O Nível 4 (Guerra) serve a uma funcionalidade específica: o combate massivo, inspirado na franquia *Total War*.

| Nível | Mapa | Estilo | Para que serve |
|---|---|---|---|
| 1 | Mundial | RTS | Estado do mundo, domínio das facções, viagens longas |
| 2 | Local | RPG | Onde quase tudo acontece |
| 3 | Interno | RPG | Interior das construções e masmorras |
| 4 | Guerra | RTS | Batalhas de Esquadrões |

### Nível 1 — Mapa Mundial

O Mapa Mundial é formado por **quadrados conectados**, numa malha quadriculada. Cada quadrado representa um Mapa Local (Nível 2). Ele **não guarda dados de jogo** das cidades nem dos NPCs: é um contêiner leve (grafo e gráfico geral), barato de manter sempre carregado, sem precisar de LOD. A única exceção é a lista de **entidades em trânsito** (grupo do jogador, caravanas, exércitos, patrulhas e bandos); as regras de viagem estão no [[Sistema de Viagem/Descrição|Sistema de Viagem]].

O Mapa Mundial serve para:

- dar informações globais sobre o mundo: onde fica cada cidade e o domínio de cada facção sobre os Mapas Locais;
- o gerenciamento em escala global, junto com outros sistemas (importante quando o jogador tiver a própria facção);
- **viagens de longa distância**, sem que o jogador precise ir à borda de cada Mapa Local.

Inclui um **índice de busca** por Mapa Local (nome, facção, tipo; por exemplo, "mostrar cidades da Facção X") e um **ponto de entrada de Viagem Rápida**, que define em qual Mapa Local, e em que posição, o grupo reaparece ao viajar direto pelo Mapa Mundial.

Novas funções ainda podem ser acrescentadas, e algumas das atuais podem ser removidas, se for preciso.

- **População no Mapa Mundial:** calculada com o **Fator de Escala** (global): 1 NPC real representa N pessoas ([[Sistema de Facção/Descrição|Facção]]).
- **Tamanho do mundo:** 48 Mapas Locais numa malha de 8 × 6, divididos em 6 regiões de 8 mapas (uma por cultura inicial). Pode mudar no futuro; o tamanho vem dos dados. A disposição das regiões, cidades e masmorras está no [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]].

### Nível 2 — Mapa Local

O Mapa Local é um dos quadrados da malha do Mapa Mundial. É onde boa parte do jogo acontece: acesso direto aos NPCs, interações, conversas, lutas, comércio e produção.

O terreno (chão, água, paredes, recursos pontuais) é uma **grade de tiles**, usada para colisão e desenho. NPCs e itens têm **posição livre** (contínua) por cima dessa grade. Cada Mapa Local guarda também uma **lista de Mapas Internos** (as construções presentes nele) e um **Nível de Detalhe de Simulação (LOD)**.

**Tamanho inicial (provisório):** 128 × 128 tiles de 16 px, a ajustar depois dos testes de desempenho.

**Como ir de um Mapa Local a outro:**

- **Pela borda:** ir até a borda do mapa entra no Mapa Local vizinho. A transição usa **alinhamento proporcional**: a posição de saída (0% a 100% ao longo da borda) vira a mesma porcentagem na borda de entrada do vizinho, mesmo que os dois tenham tamanhos diferentes.
- **Pelo Mapa Mundial:** usado de preferência quando o destino está longe, sem ligação direta com o mapa atual.
- Outros métodos ainda estão sendo pensados.

**LOD por Mapa Local:**

| LOD | Quando | O que roda |
|---|---|---|
| 0 (Completo) | O mapa está na visão atual | Simulação completa, tick a tick |
| 1 (Reduzido) | Há NPC Controlado no mapa, mas fora da visão da câmera | Simulação sem busca de caminho fina nem combate detalhado |
| 2 (Abstrato) | Não há NPC Controlado | Necessidades e eventos avançam em blocos de tempo, sem simulação tile a tile |

Todo NPC dentro do Mapa Local **herda** esse LOD, que não é calculado NPC por NPC; isso barateia muito o processamento. Mais de um Mapa Local pode estar ativo ao mesmo tempo, cada um no seu nível.

#### Cidade e território

Cada Mapa Local comporta **só uma cidade**, mas nem todo Mapa Local tem uma. O mapa tem um de quatro **estados de território**:

| Estado | Quando |
|---|---|
| **Sem dono** | Sem cidade e sem cidade vizinha. Existe para o jogador explorar e, se quiser, fundar um assentamento |
| **Território de facção** | Sem cidade, mas **vizinho** (os 4 vizinhos na malha) de uma cidade de facção. É vinculado à facção dessa cidade e pode ser tratado como vila ou entreposto em desenvolvimento |
| **Em disputa** | Vizinho de cidades de **facções diferentes**. Causa guerras constantes até uma facção dominá-lo ou o jogador interferir. É o único estado que guarda quem domina o mapa; ele só muda de dono no tratado de paz ([[Sistema de Guerra/Descrição\|Guerra]]) |
| **Com cidade** | Tem uma cidade; a facção do Mapa Local é a da cidade |

O estado e a facção de um Mapa Local são **derivados** das cidades vizinhas, não guardados (a exceção é o domínio do mapa em disputa): se uma cidade muda de facção por conquista, os mapas vizinhos a seguem. O jogador pode fundar um assentamento (nível 1) num mapa sem dono ou em território de facção; no segundo caso, ele nasce vinculado à facção dominante.

### Nível 3 — Mapa Interno

O Mapa Interno é o interior das construções do Mapa Local, sempre ligado ao Mapa Local pai. Existem 3 tipos:

| Tipo | O que é |
|---|---|
| **Moradia** | Tem um Dono definido; alvo de furto |
| **Masmorra** | Sem dono; tem relíquias, itens e às vezes Criaturas |
| **Construção Pública** | Tavernas, mercados, ferrarias, prisões; destino físico do trabalho na Rotina dos NPCs. A **Prisão** é onde os NPCs presos cumprem pena ([[Sistema de Crime e Justiça/Descrição\|Crime e Justiça]]) |

- Qualquer construção de produção pode funcionar como **loja ilegal** disfarçada.
- Os mapas são montados no Tiled com um padrão fixo de camadas ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]). Têm interior desenhado próprio: Taverna, Mercado, Templo, Prisão, Palácio, Quartel e as masmorras; Moradias e construções de produção usam interiores genéricos por tamanho e cultura.
- Cada Mapa Interno tem a **própria grade de tiles** (em escala menor) e um **Nível de Segurança**, que afeta a chance de detecção nos furtos e a dificuldade das masmorras.
- A entrada e a saída usam um **ponto fixo** (a porta), diferente da borda proporcional entre Mapas Locais. Ao sair, o NPC reaparece no "Endereço da volta" do Mapa Local pai.
- Todo Mapa Interno começa em **LOD 2**, mesmo que o Mapa Local pai esteja em LOD 0, e só sobe quando alguém entra de fato. Assim o jogo não simula o interior de todas as casas de todas as cidades o tempo todo.

### Nível 4 — Mapa de Guerra

Um mapa diferente dos outros: só existe em situações de combate entre duas facções ou exércitos, e normalmente é acessado pelo Mapa Mundial. Como a guerra é um sistema separado do combate principal do jogo, essa separação evita que os dois tipos de combate entrem em conflito.

- **Quando é criado:** só quando **o jogador participa**. As guerras entre facções de IA são resolvidas **automaticamente** pelo cálculo rápido de forças ([[Sistema de Guerra/Descrição|Guerra]]).
- **Exércitos e Esquadrões:** o jogador não controla soldados um a um, mas **Esquadrões** (por exemplo, 2.000 soldados = 20 Esquadrões). Soldados não são NPCs. As **Caravanas Militares** acompanham o exército com os suprimentos.
- **O campo:** uma **cópia simplificada do Mapa Local** onde o confronto começou (sem NPCs nem itens; as construções viram obstáculos), lida do mesmo arquivo do Tiled, com a camada opcional de Elevação. Assim a batalha faz sentido geograficamente. O mundo fica parado durante a batalha.
- **O que guarda:** as facções e os exércitos participantes (com Esquadrões, formação e Moral), a condição de vitória e o resultado pós-batalha (mudança de domínio do Mapa Local, baixas, saque, prisioneiros).
- **Vitória:** quando todos os Esquadrões de um lado estão **fugindo ou destruídos**, ou quando acaba o tempo limite (vence o defensor), como decidido na Batalha.
- **Temporário:** é criado e destruído sob demanda; não fica "adormecido" como os outros mapas.
- As regras da batalha (tipos de Esquadrão, Moral, formações, terreno, cerco, reforços e o tamanho visual das unidades) estão no [[Sistema de Batalha/Descrição|Sistema de Batalha]].

### Atributos

| Mapa | Atributos |
|---|---|
| **Mundial** | Grafo da malha de Mapas Locais; gráfico do Mapa Mundial; entidades em trânsito; por Mapa Local: terreno, infraestrutura e estradas (tempo de viagem), névoa e informações datadas para o jogador |
| **Local** | Parte gráfica; posição no grafo; os 4 vizinhos (pelo grafo); lista de recursos; estado de território (derivado); facção (derivada); cidade (no máximo uma); cultura da região; população total (NPCs reais × Fator de Escala); tamanho (x, y); nome; tem cidade (booleana); grade de tiles; Mapas Internos; LOD |
| **Interno** | Parte gráfica; Endereço da volta; Mapa Local pai; tipo (Moradia, Masmorra ou Construção Pública); grade de tiles própria; Dono (se Moradia); Nível de Segurança |
| **Guerra** | Parte gráfica; Mapa Local de origem; facções e exércitos (Esquadrões e Caravanas); condição de vitória e derrota; resultado pós-batalha |

---

## Parte técnica

- **Dados:** `mapas.json` com os 48 Mapas Locais (posição, região, bioma, recursos, terreno, infraestrutura e arquivo do Tiled); cada mapa é um arquivo do Tiled em JSON.
- **Carregamento:** o Mapa Mundial fica sempre carregado; os Mapas Locais carregam quando sobem de LOD 2 e podem ser liberados de uma vez (arenas de memória por mapa; ver [[Arquitetura/Descrição|Arquitetura]]).
- **Save:** por Mapa Local; mapas em LOD 2 guardam só os dados agregados. O Mapa de Guerra não entra no save.
- **Fatia vertical:** 1 Mapa Local com cidade, alguns Mapas Internos (Casa, Mercado), sem Mapa Mundial nem Mapa de Guerra.

## Ligação com outros sistemas

| Sistema | O que o Mapa fornece ou recebe |
|---|---|
| Viagem | Entidades em trânsito no Mapa Mundial; Viagem Rápida; terreno e infraestrutura |
| Mundo e Estado Inicial | Malha de 48 mapas, regiões, cidades e masmorras |
| Cidade e Facção | Uma cidade por mapa; estados de território derivados; domínio do mapa em disputa |
| Eventos | Sorteio e avaliação respeitam o LOD; em LOD 2 os efeitos são agregados; bandidos em mapas sem dono ou de fronteira ([[Sistema de Eventos/Descrição\|Eventos]]) |
| Combate | Combate local para grupos pequenos e médios (cerca de 40 combatentes); acima disso, ou com Esquadrões, o Mapa de Guerra ([[Sistema de Combate/Descrição\|Combate]]) |
| Guerra e Batalha | Mapa de Guerra a partir do Mapa Local; Elevação; praça central e Muralha marcadas no Tiled |
| População | Migrantes chegam pela borda em LOD 0; fluxo calculado em LOD 2 ([[Sistema de População/Descrição\|População]]) |
| Crime e Justiça | Nível de Segurança; Prisão como Mapa Interno |
| Pipeline | Padrão de camadas do Tiled; interiores próprios ou genéricos |
| Interface | Lentes (Política, Cultura, Felicidade, Comércio, Perigo, Tecnologia), busca, Viagem Rápida e névoa ([[Interface/Descrição\|Interface]]) |

## Perguntas em aberto

1. **Tamanho do Mapa Local** (128 × 128): a confirmar com protótipo ([[Arquitetura/Descrição|Arquitetura]]).
2. **Outros métodos** de ir de um mapa a outro, além da borda e do Mapa Mundial.
