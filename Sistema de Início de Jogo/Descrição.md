> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Início de Jogo

**Descrição geral**

O jogo é livre: o jogador pode fazer o que quiser no mundo. O início de jogo serve para o jogador **decidir como quer jogar**. É algo parecido com o que o **Kenshi** faz.

O estilo de início **não influencia nenhuma mecânica** do jogo. Ele só dá **bônus iniciais**, para o jogador ter uma vantagem inicial no estilo de jogo que quer testar. Qualquer jogador, com qualquer estilo, pode depois fazer qualquer coisa.

### Tipos de início

- **Estilos pré-definidos.** Já vêm com uma **cultura definida** (ver Sistema de Cultura).
- **Estilo personalizado.** O jogador decide tudo, incluindo a cultura. Funciona com um **orçamento de pontos**: cada escolha (cultura, dinheiro, itens, grupo) custa pontos, para o personalizado não ficar sempre melhor que os estilos prontos.

### Bônus iniciais

Os bônus valem **só no começo**: itens, dinheiro, habilidades e relação inicial com as facções. Nada é permanente.

### O que um estilo define

**[Proposta]** Cada estilo de início é um conjunto de dados, lido de arquivo, com:

- cultura do personagem;
- local de partida (Mapa Local e posição);
- dinheiro inicial;
- itens e equipamento iniciais;
- companheiros iniciais;
- relação inicial com as facções (bônus sobre a base de reputação, ver [[Sistema de Reputação/Descrição|Sistema de Reputação]]);
- bônus de habilidades.

Como é só dado, novos estilos podem ser acrescentados sem mexer em código.

**Tamanho do grupo.** Depende de cada estilo de início. Não há um número fixo.

### Estilos

Por enquanto, os estilos abaixo. Novos estilos serão acrescentados quando o autor tiver novas ideias. Os bônus finais estão em "Bônus de cada estilo", abaixo (rodada V3-2).

| Estilo | Ideia | Bônus inicial (exemplo) |
|---|---|---|
| **Dono de vila** | Começa com um assentamento em território de facção (vinculado) ou num mapa livre (independente), à escolha, e tenta ascendê-lo a uma cidade independente | Assentamento nível 1, recursos de construção |
| **Aventureiro** | Grupo errante, sem cidade, vivendo de masmorras e missões | Equipamento e habilidades de combate |
| **Membro de facção** | Começa já como cidadão ou nobre de uma das 6 facções | Título, relação alta com a facção |
| **Mercador** | Compra e vende entre cidades | Dinheiro, itens de comércio |
| **Nobre exilado** | Herdeiro de uma dinastia caída, quer retomar o poder | Companheiros nobres, relação alta com sua cultura |
| **Escravo** | Começa como propriedade de alguém e tenta se libertar: comprar, fugir ou servir (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) | Habilidades de Trabalho Braçal, aliados entre escravos |
| **Mago** | Começa com a habilidade mágica desenvolvida | Magia, Mana, cajado |

### Bônus de cada estilo (rodada V3-2)

| Estilo | Dinheiro | Itens e grupo | Habilidade e relação |
|---|---|---|---|
| Dono de vila | 300 | Assentamento nível 1 (1 Casa, 1 Fazenda, 1 Posto de Coleta), 40 Tábuas, 40 Blocos de Pedra, 4 colonos Civis | +10 de Inteligência |
| Aventureiro | 150 | 2 companheiros; armas e armadura Leve de Rank 2 | +10 em Corpo a Corpo ou Longo Alcance |
| Membro de facção | 200 | Título Cidadão (ou Cavaleiro, se Nobre); uma Casa na cidade | Relação +30 com a facção |
| Mercador | 800 | 2 Cavalos e 1 companheiro (líder de caravana); preços de 3 cidades vizinhas já conhecidos | +15 de Carisma |
| Nobre exilado | 400 | 2 companheiros de Nobreza; Espada Longa de Aço de Rank 2 | Relação +20 com a sua cultura e −20 com a facção que tomou o trono |
| Escravo | 0 | 2 escravos aliados (Vínculo alto); Grilhões | +20 de Trabalho Braçal |
| Mago | 100 | Cajado Simples de Rank 2, 3 Pedras de Mana | Mago; +20 de Magia |

**Personalizado:** 100 pontos, a partir de um personagem Civil sem nada. A cultura é grátis.

| Escolha | Custo |
|---|---|
| 10 moedas | 1 ponto |
| Companheiro | 15 pontos |
| +10 numa habilidade | 10 pontos |
| Item | 1 ponto a cada 10 moedas de preço |
| Ser Mago | 30 pontos |
| Assentamento nível 1 | 40 pontos |
| Título Cidadão | 10 pontos |

Cada estilo pronto vale cerca de 100 pontos, o que mantém o personalizado justo.

### Mundo fixo, estado inicial variável

O mundo (mapas, cidades e regiões) é fixo e desenhado à mão. O que varia a cada partida é o **estado inicial**: relações entre facções, guerras em andamento, quais tecnologias extras cada cultura recebe, a localização do Loot das masmorras e a semente do Narrador. Isso dá rejogabilidade sem arte nova. A lista exata do que varia, os limites e os pontos de partida de cada estilo estão no [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]].

### Ligação com outros sistemas

| Sistema | O que o Início de Jogo fornece ou recebe |
|---|---|
| Cultura | Cultura inicial do personagem; arte e estilo de itens |
| Facção | Relação inicial com as facções; o estilo "Dono de vila" usa a vinculação a território |
| NPC | Personagem e companheiros Controlados; geração dos atributos iniciais |
| Itens e Inventário | Itens e dinheiro iniciais |
| Mapa | Local de partida: 2 ou 3 pontos fixos por estilo (ver [[Mundo e Estado Inicial/Descrição\|Mundo e Estado Inicial]]) |
| Eventos | O jogador escolhe um narrador na criação do jogo; o narrador é a dificuldade, mas cada um tem um foco, para combinar com o estilo de jogo (ver [[Sistema de Eventos/Descrição\|Sistema de Eventos]]) |
| Viagem | Conhecimento inicial do mapa: Membro de facção vê o território da facção; Mercador, as cidades com Posto Comercial; Aventureiro, as masmorras da região (ver [[Sistema de Viagem/Descrição\|Sistema de Viagem]]) |
| Combate | Se todo o grupo cai, o jogador é capturado em vez de ter Game Over, o que conversa com os estilos Escravo e Aventureiro (ver [[Sistema de Combate/Descrição\|Sistema de Combate]]) |
| Interface | Novo jogo em 3 passos: estilo de início, narrador e personagem (ver [[Interface/Descrição\|Interface e Controles]]) |

### Perguntas em aberto

1. **Custo em pontos do personalizado:** decidido na V3-2 (valores a testar).
2. **Mercador:** decidido na V3-2.
