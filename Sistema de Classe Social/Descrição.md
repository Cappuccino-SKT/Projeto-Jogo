> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Classe Social

**Descrição geral**

Adiciona profundidade ao jogo conforme o avanço da gameplay, ou dependendo do estilo de jogo escolhido pelo jogador. Serve para criar **demandas de recursos específicas por classe social**: se a demanda não for atendida, cada classe pode causar um evento diferente.

### Distribuição das classes (5 níveis)

1. **Realeza** — majoritariamente líderes de facções
2. **Nobreza** — majoritariamente personagens com importância no mundo
3. **Rico** — majoritariamente donos de fábricas ou de terras
4. **Civil** — a maioria da população
5. **Escravo** — todos os escravos

**Títulos.** Os títulos do Objetivo e Progressão (Plebeu, Cidadão, Cavaleiro, Nobre, Senhor de cidade e Líder de facção) são **rótulos políticos**. A classe só muda pelas regras de mobilidade, com a exceção do Rito de Admissão da cultura (ver [[Objetivo e Progressão/Descrição|Objetivo e Progressão]]).

**Mobilidade social:** as transições de classe são específicas e raras, não livres.

- Um Civil de alta habilidade pode virar Rico quando a cidade abre uma empresa nova (as vagas de Rico dependem do número de empresas, e o jogador sobe com dinheiro próprio; ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]]).
- Um Escravo pode ser libertado e virar Civil (ver [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]).
- A queda de classe também é possível (por exemplo, um Rico que perde a fábrica).

Isso dá profundidade sem exigir um sistema de mobilidade totalmente livre.

### Consumo

O consumo **não é atribuído a cada NPC**: isso gastaria muito processamento em informações pouco úteis. Ele é vinculado às **cidades**: o jogo verifica o inventário da cidade e, se ele tiver os recursos exigidos, a demanda é atendida. Com o tempo, esses recursos são gastos do inventário, na proporção da quantidade de pessoas de cada classe. Nas contas, **1 NPC real é 1 consumidor** (rodada IP1-5): o Fator de Escala só muda a população exibida.

| Classe | Consome |
|---|---|
| **Escravo** | Comida |
| **Civil** | Comida, Materiais Básicos |
| **Rico** | Comida, Materiais Básicos, Bens de Luxo |
| **Nobreza** | Comida, Materiais Básicos, Bens de Luxo, Bens Exclusivos |
| **Realeza** | Comida, Materiais Básicos, Bens de Luxo, Bens Exclusivos, Artefatos |

Cada classe pode causar um evento diferente conforme o recurso em falta. Os mais comuns: revoltas de Escravos e Civis, perda de empresas dos Ricos e mudanças de leis por Nobreza e Realeza. A Felicidade de cada classe e as reações estão no [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]].

**Dias em déficit.** Um evento de escassez só dispara por um contador de **"dias em déficit"** por recurso e cidade. O estoque precisa ficar abaixo de um limiar (por exemplo, 10%) por um número mínimo de dias seguidos, com chance crescente de evento quanto mais tempo durar o déficit. Isso evita eventos disparando por uma flutuação momentânea do estoque. É um caso da condição com duração mínima e histerese do [[Sistema de Eventos/Descrição|Sistema de Eventos]]: as reações por classe são eventos condicionais, com as causas registradas e visíveis ao jogador.

### Mapeamento do consumo para itens

Cada tipo de consumo corresponde a um grupo de itens, definido pelo **Tier** do bem (rodada IP2-11; ver [[Sistema de Itens/Descrição|Itens]]). Um item novo já cai no grupo certo, sem mexer na tabela.

| Consumo | O que conta | Quem consome |
|---|---|---|
| Comida | Alimentos e bebidas. O Escravo come Tier 1; o Civil, Tier 1 e 2; o Rico e acima, Tier 2 ou mais (nobre não come Ração) | Todas as classes |
| Materiais Básicos | Mobília e Vestuário de Tier 1 e 2 (Cerâmica, Móveis de Madeira, Roupa Comum), mais a Tora no inverno | Civil e acima (a Tora, todas as classes) |
| Bens de Luxo | Tudo de Tier 3 que não é remédio (Móveis Refinados, Roupa Fina, Perfume, Vinho, Assado Temperado), mais os Narcóticos | Rico e acima |
| Bens Exclusivos | Tudo de Tier 4 (Licor, Presunto Curado, Doce de Especiarias, Joias, Ornamento) | Nobreza e Realeza |
| Artefatos | Livro, Joia de Mana e Relíquia, como **posse**: 2 por NPC real de Realeza, guardados no Palácio; 1 em cada 10 sai por estação (ver [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]]) | Realeza |

- Os **remédios** ficam fora do consumo de classe: a cidade os gasta só nas epidemias ([[Sistema de Cura/Descrição|Cura]]).
- Onde a lei proíbe os Narcóticos (todas as facções menos a Escravista; rodada V3-1), o Luxo vem só dos outros itens de Tier 3.
- Até **25%** da Comida pode ser Bebida; as quantidades estão em "Quantidades" (rodada V2-6).

**Lenha no inverno.** No inverno, cada NPC real consome 1 Tora a cada 4 dias como aquecimento, dentro do consumo de Materiais e em **todas as classes**, inclusive os Escravos. O bioma multiplica (frio × 2; litoral quente e terras áridas × 0,25). Sem lenha: Felicidade −10 e mais mortes de velhos e crianças (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).

**Casos especiais.**

- **Soldados em Esquadrão** não têm Classe Social. O consumo deles é definido pelo próprio Esquadrão (Ração Militar e materiais de manutenção; ver [[Sistema de Guerra/Descrição|Guerra]]).
- **Dissolução de exército:** os soldados voltam à população como novos NPCs das classes Escravo e Civil.
- **Trabalho forçado** (punição) torna o NPC Escravo por um período, e depois ele volta à classe anterior.
- **Escravos:** fontes, compra e venda, Obediência, fuga, rebelião e libertação estão no [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]. O Escravo libertado vira Civil e Plebeu.
- **Controlados e Importantes** comem do próprio inventário, com Fome individual (ver [[Sistema de NPC/Descrição|NPC]] e [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).

### Quantidades (rodada V2-6)

**Cesta mínima por NPC real.** É ela que, se faltar, conta os dias em déficit:

| Classe | Comida por dia | Materiais Básicos | Luxo | Exclusivo |
|---|---|---|---|---|
| Escravo | 1 (Tier 1) | — | — | — |
| Civil | 1 (Tier 1 ou 2) | 1 a cada 8 dias | — | — |
| Rico | 1 (Tier 2 ou mais) | 1 a cada 4 dias | 1 a cada 4 dias | — |
| Nobreza | 1 (Tier 2 ou mais) | 1 a cada 4 dias | 1 a cada 2 dias | 1 a cada 4 dias |
| Realeza | 1 (Tier 2 ou mais) | 1 a cada 2 dias | 1 por dia | 1 a cada 2 dias |

Mais a Tora no inverno e os Artefatos como posse da Realeza.

- **Regra do gasto:** cada classe gasta por dia **até 80% da renda**.
  - Primeiro vem a cesta mínima.
  - O que sobra sobe o Tier da comida, compra mais materiais e paga a **Bebida na Taverna** (até 25% da Comida pode ser Bebida).
  - Os outros 20% são imposto e poupança, que vira a riqueza média.

  Assim o dinheiro volta às lojas, e uma cidade rica come melhor sozinha.
- **Bônus de Felicidade:** consumo acima da cesta dá até +5 no fator de consumo da classe.
- *Exemplo:* um Civil ganha 8 por dia e gasta até 6,4: o Pão (1,7), uma Cerveja (3,3) e um pouco de Materiais.

**Composição das cidades por classe:**

| Classe | Parcela da cidade |
|---|---|
| Escravo | 10% (30% na Escravista; zero onde a lei proíbe, e esses NPCs viram Civis) |
| Civil | 76% (56% na Escravista) |
| Rico | 10% |
| Nobreza | 4% |
| Realeza | Só na capital: 1% (3 NPCs de 300) |

### Influência sobre habilidades

A classe social também afeta quais habilidades principais o NPC tem e o nível delas, com faixas de partida (0 a 100, ajustáveis) usadas pela Geração de NPC:

| Classe | Faixas de partida |
|---|---|
| **Escravo** | Trabalho Braçal 60–100; demais 0–30 |
| **Civil** | 20–60 em todas |
| **Rico** | Trabalho Especialista 60–90; demais 30–60 |
| **Nobreza** | Inteligência e Carisma 70–100; demais 20–50 |
| **Realeza** | Combate, Inteligência e Carisma 70–100; demais 30–60 |

---

## Parte técnica

- **Por cidade:** quantidade de NPCs reais por classe, riqueza média por classe, Felicidade por classe e os contadores de dias em déficit por tipo de consumo.
- **Frequência:** o consumo é descontado por dia em LOD 0 e 1 e por estação em LOD 2.
- **Dados:** tabela de consumo por classe e o mapeamento consumo → itens em JSON; faixas de habilidade por classe (e por cultura) na Geração de NPC.
- **Save:** os valores por cidade acima.
- **Fatia vertical:** só Civil e Rico (ou só Civil), com Comida e Materiais Básicos.

## Ligação com outros sistemas

| Sistema | O que a Classe Social fornece ou recebe |
|---|---|
| Cidade | NPCs por classe; bens de consumo por classe |
| Felicidade e Estabilidade | Consumo atendido como fator; reações por classe |
| Eventos | Dias em déficit como condição; revoltas, perda de empresas e mudança de leis |
| Itens | Grupos de itens de cada tipo de consumo |
| Trabalho e Propriedade | Vagas de Rico pelas empresas; riqueza média por classe |
| NPC | Faixas de habilidade na Geração de NPC |
| Objetivo e Progressão | Títulos como rótulos políticos; Rito de Admissão |
| Escravidão | Classe Escravo; libertação |
| Clima e Estações | Lenha no inverno |
| Crime e Justiça | Penas variam por classe |

## Perguntas em aberto

1. **Valores a testar:** cesta mínima, regra do gasto (80%), composição por classe e bônus de Felicidade (rodada V2-6).
