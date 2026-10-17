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

O consumo **não é atribuído a cada NPC**: isso gastaria muito processamento em informações pouco úteis. Ele é vinculado às **cidades**: o jogo verifica o inventário da cidade e, se ele tiver os recursos exigidos, a demanda é atendida. Com o tempo, esses recursos são gastos do inventário, na proporção da quantidade de pessoas de cada classe.

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

Cada tipo de consumo corresponde a um grupo de itens (ver [[Sistema de Itens/Descrição|Sistema de Itens]]). Ajuste provisório, a rever nas rodadas de Itens e Produção:

| Consumo | Itens |
|---|---|
| Comida | Alimento Básico, Alimento Refinado e Bebida |
| Materiais Básicos | Mobília Básica |
| Bens de Luxo | Perfumes, Mobília Refinada e Narcóticos |
| Bens Exclusivos | Bens de luxo de Tier 4 (Licor, Carne Envelhecida, Doce de Baunilha, Joias, Ornamento) |
| Artefatos | Livro, Joia Mágica e Relíquia, como **posse**: 2 por NPC real de Realeza, guardados no Palácio; 1 em cada 10 sai por estação (ver [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]]) |
| Lenha (só no inverno, todas as classes) | Madeira Bruta |

**Lenha no inverno.** No inverno, cada NPC real consome 1 Madeira Bruta a cada 4 dias como aquecimento, dentro do consumo de Materiais e em **todas as classes**, inclusive os Escravos. O bioma multiplica (frio × 2; litoral quente e terras áridas × 0,25). Sem lenha: Felicidade −10 e mais mortes de velhos e crianças (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).

**Casos especiais.**

- **Soldados em Esquadrão** não têm Classe Social. O consumo deles é definido pelo próprio Esquadrão (Ração Militar e materiais de manutenção; ver [[Sistema de Guerra/Descrição|Guerra]]).
- **Dissolução de exército:** os soldados voltam à população como novos NPCs das classes Escravo e Civil.
- **Trabalho forçado** (punição) torna o NPC Escravo por um período, e depois ele volta à classe anterior.
- **Escravos:** fontes, compra e venda, Obediência, fuga, rebelião e libertação estão no [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]. O Escravo libertado vira Civil e Plebeu.
- **Controlados e Importantes** comem do próprio inventário, com Fome individual (ver [[Sistema de NPC/Descrição|NPC]] e [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).

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

1. **Quantidades:** quanto cada NPC real de cada classe consome por dia (rodada V2 da [[Revisão Completa]]).
2. **Mapeamento Luxo × Exclusivo** por Tier e o Doce de Baunilha (rodadas de Itens e Produção).
