## Sistema de Classe Social

**Descrição geral**

Adiciona profundidade ao jogo conforme o avanço da gameplay, ou dependendo do estilo de jogo escolhido pelo jogador. Serve para adicionar demandas de recursos específicos por classe social — e, se a demanda não for atendida, cada classe pode causar um evento diferente.

### Distribuição das classes (5 níveis)

1. **Realeza** — majoritariamente líderes de facções
2. **Nobreza** — majoritariamente personagens com importância no mundo
3. **Rico** — majoritariamente donos de fábricas ou de terras
4. **Civil** — a maioria da população
5. **Escravo** — todos os escravos

**Títulos.** Os títulos do Objetivo e Progressão (Plebeu, Cidadão, Cavaleiro, Nobre, Senhor de cidade e Líder de facção) são **rótulos políticos**. A classe só muda pelas regras de mobilidade, com a exceção do Rito de Admissão da cultura (ver [[Objetivo e Progressão/Descrição|Objetivo e Progressão]]).

**Mobilidade social:** transições de classe são específicas e raras, não livres. Um Civil pode virar Rico ao acumular Dinheiro acima de um limiar por tempo suficiente; um Escravo pode ser libertado e virar Civil; queda de classe também é possível (ex: um Rico que perde a fábrica). Isso dá profundidade sem exigir um sistema de mobilidade totalmente livre.

### Consumo

O consumo não é atribuído diretamente a cada NPC — isso geraria gasto considerável de processamento em informações pouco úteis. Em vez disso, o consumo é vinculado às cidades: verifica-se o inventário da cidade, e se ele possuir os recursos exigidos, a demanda é atendida. Com o passar do tempo, esses recursos são gastos do inventário, proporcionalmente à quantidade de pessoas em cada classe social.

Consumo por classe:

- **Escravo** — Comida
- **Civil** — Comida, Materiais Básicos
- **Rico** — Comida, Materiais Básicos, Bens de Luxo
- **Nobre** — Comida, Materiais Básicos, Bens de Luxo, Bens Exclusivos
- **Realeza** — Comida, Materiais Básicos, Bens de Luxo, Bens Exclusivos, Artefatos

Cada classe social pode causar um evento diferente dependendo do recurso em falta — os mais comuns seriam: revoltas por parte de Escravos e Civis, perda de empresas por parte dos Ricos, e mudanças de leis por parte de Nobres e Realeza.

Um evento de escassez só dispara com base num contador de **"dias em déficit"** por recurso/cidade: o estoque precisa ficar abaixo de um limiar (ex: 10%) por um número mínimo de dias consecutivos, com chance crescente de evento quanto mais tempo durar o déficit. Isso evita eventos disparando por uma flutuação momentânea de estoque. É um caso da condição com duração mínima e histerese do [[Sistema de Eventos/Descrição|Sistema de Eventos]]: as reações por classe (revolta, perda de empresas, mudança de lei) são eventos condicionais, com as causas registradas e visíveis ao jogador.

### Mapeamento do consumo para itens

Para o consumo da cidade funcionar, cada tipo de consumo corresponde a um grupo de itens (ver Sistema de Itens). Ajuste provisório:

| Consumo | Itens |
|---|---|
| Comida | Alimento Básico, Alimento Refinado e Bebida |
| Materiais Básicos | Mobília Básica |
| Bens de Luxo | Perfumes, Mobília Refinada e Narcóticos |
| Bens Exclusivos | Bens de luxo de Tier 4 (Licor, Carne Envelhecida, Doce de Baunilha, Joias, Ornamento) |
| Artefatos | Livro, Joia Mágica e Artefato |

**Casos especiais.**

- **Soldados em Esquadrão** não têm Classe Social. O consumo deles é definido pelo próprio Esquadrão (Ração Militar e materiais de manutenção).
- **Dissolução de exército:** os soldados voltam à população como novos NPCs das classes Escravo e Civil.
- **Trabalho forçado** (punição) torna o NPC Escravo por um período, voltando depois à classe anterior.

### Influência sobre habilidades

A classe social também afeta quais habilidades principais o NPC possui e o nível delas, com faixas de partida (0–100, ajustáveis) usadas pela Geração de NPC:

- **Escravos** — atributos majoritariamente baixos, com foco apenas em Trabalho Braçal (60–100; demais habilidades: 0–30).
- **Civis** — atributos de baixo a médio, distribuídos entre todas as habilidades (20–60 em todas).
- **Ricos** — atributos médios, com foco em Trabalho Especialista (60–90; demais: 30–60).
- **Nobres** — atributos altos em Inteligência e Carisma (70–100; demais: 20–50).
- **Realeza** — atributos altos em combate, Inteligência e Carisma (70–100; demais: 30–60).

