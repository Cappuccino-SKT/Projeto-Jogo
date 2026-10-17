> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Inventário

**Descrição geral**

O inventário guarda **quais itens estão armazenados**, a quantidade de cada um, o dono, o dinheiro e outras informações relevantes. No código ele é um **componente** reaproveitado por quem precisa dele (composição; ver [[Arquitetura/Descrição|Arquitetura]]), não uma cadeia de herança.

Decisão do autor na revisão (V1-5): **um inventário lógico por cidade**, com a capacidade somada das construções. Isso junta o que o [[Sistema Econômico/Descrição|Econômico]] já dizia (um inventário único, sem logística de entrega) com a capacidade de armazenamento de cada construção que a [[Sistema de Produção/Descrição|Produção]] pedia.

### Quem tem inventário

| Dono do inventário | O que ele tem |
|---|---|
| **Cidade** | Uma **tabela única** (item, dono, quantidade) com tudo o que está nas lojas e construções da cidade. Passa informações ao [[Sistema de Eventos/Descrição\|Sistema de Eventos]] (por exemplo, dias em déficit de consumo) |
| **NPC Controlado** | A parte gráfica (a janela do jogador) e um limite de **peso**: a quantidade que ele carrega depende da Capacidade de peso, influenciada pela Força. Inclui o **Equipamento** (o que está vestido e empunhado) |
| **NPC Importante** | Itens e Equipamento, sem a parte gráfica |
| **Caravana** | **Carga** por peso: a Caravana Militar leva Ração Militar, materiais de manutenção e munição (ver [[Sistema de Guerra/Descrição\|Guerra]]); a Caravana Civil leva mercadorias (ver [[Sistema de Comércio/Descrição\|Comércio]]). Cada Cavalo soma capacidade |
| **Esquadrão** | Não é inventário: tem o **kit** de equipamento (ver [[Sistema de Batalha/Descrição\|Batalha]]) |

Os NPCs comuns (Não-Controlados) não têm inventário próprio: o consumo deles é da cidade ([[Sistema de Classe Social/Descrição|Classe Social]]).

### Capacidade da cidade

- Cada construção **acrescenta capacidade** às categorias que produz ou vende. *Exemplo:* a Fazenda nível 2 dá +200 de capacidade de matéria-prima vegetal. **[Proposta]** Os valores por construção e nível ficam com os custos das construções (rodada V2).
- Quando a capacidade de uma categoria **enche**, a produção dela **para** até alguém consumir ou vender: é o limite contra o acúmulo infinito.
- O jogador tem os itens dele separados pelo atributo **Dono**, dentro da mesma tabela.

### Componentes

| Componente | Quem usa |
|---|---|
| Itens (lista de pilhas e instâncias) | Todos |
| Equipamento | Controlados e Importantes |
| Capacidade de peso | Controlados |
| Capacidade por categoria | Cidade |
| Carga | Caravanas |

---

## Parte técnica

**Por que é eficiente** (o autor pediu a análise):

- **Pilhas, não objetos.** Materiais e bens de consumo não existem um por um: são **pilhas** (item, dono, quantidade). 10 mil pães são um número, não 10 mil objetos. Só armas, equipamentos e artefatos guardam algo a mais (Rank, cultura, encantamento), e mesmo eles se empilham quando são iguais (por exemplo, 30 Lanças Curtas de Ferro Rank 2 da cultura Guerreira).
- **Tabela densa para a cidade.** A cidade guarda a quantidade **de cada item num vetor indexado pelo ID do item** (cerca de 200 itens × 4 bytes = menos de 1 KB por cidade). Somar o consumo, conferir o déficit ou mostrar a loja é uma passada direta pela memória, o tipo de operação mais rápida que o computador faz. Os itens de outros donos (jogador e Ricos) e as pilhas com Rank ficam numa lista pequena separada.
- **Sem logística.** Como não há transporte entre construções, não há busca de caminho para carregadores nem filas de entrega, que são o maior custo de jogos como o RimWorld.
- **Custo total:** com 22 cidades, todos os inventários do mundo cabem em menos de 100 KB.

**Alternativas e por que não:**

| Alternativa | Problema |
|---|---|
| Um inventário por construção, com transporte | Cria logística (carregadores, rotas, filas), que custa simulação e foi evitada de propósito |
| Cada item como entidade do ECS | Milhões de entidades para coisas que só precisam de um número |
| Itens espalhados no chão do mapa | Bonito, mas caro; fica só para o loot de combate e masmorras, em pouca quantidade |

**[Refinar depois]** Se um dia o jogo quiser logística visível (carroças levando grãos à cozinha, como no Songs of Syx), ela pode ser **só visual**, desenhada por cima da tabela única, sem mudar a regra.

- **Save:** a tabela de cada cidade, os inventários dos Controlados, Importantes e caravanas.
- **Fatia vertical:** a tabela da cidade, a capacidade da Fazenda, do Cozinheiro e do Mercado e o inventário do personagem.

## Ligação com outros sistemas

| Sistema | O que o Inventário fornece ou recebe |
|---|---|
| Econômico | Inventário único da cidade, compartilhado pelas lojas; atributo Dono |
| Produção e Construção | Capacidade por categoria; a produção para quando enche |
| Itens | Pilhas e instâncias; peso |
| Classe Social e Eventos | Estoques para o consumo e os dias em déficit |
| NPC | Inventário e Equipamento dos Controlados e Importantes; Capacidade de peso pela Força |
| Comércio e Guerra | Carga das caravanas; Cavalos somam capacidade |
| Batalha | Kit dos Esquadrões (não é inventário) |
| Interface | Janela de inventário e equipamento |

## Perguntas em aberto

1. **Capacidade** de cada construção por categoria e nível (rodada V2).
2. **Capacidade de peso** por ponto de Força (rodada V2).
