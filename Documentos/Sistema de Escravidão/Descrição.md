> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Escravidão

**Descrição geral**

Junta num lugar só o que estava espalhado entre NPC, Crime, Cultura, Trabalho, Guerra e Classe Social: **de onde vêm os escravos**, como são comprados e vendidos, como vivem e obedecem, como fogem e se rebelam, como são libertados e como o tráfico pesa no crime e na diplomacia. A escravidão é a base da cultura **Escravista** (a mercantil, que trata tudo como mercadoria) e um ponto de conflito com a **Religiosa**.

Já estava decidido e vale como base:

- **Lei de Escravidão** (permitida ou proibida): define se o atributo e a classe Escravo existem nas cidades da facção ([[Sistema de Facção/Descrição|Facção]]).
- Os **Escravos** não recebem salário, produzem para o dono (cidade, Rico ou jogador), que paga a comida e a **Senzala**, e não pagam imposto ([[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]], [[Sistema de População/Descrição|População]]).
- O NPC escravo tem **Obediência**, **Revolta** e **Vínculo**; o **Escravo do jogador** é Semi-Controlado e segue a Lista de Prioridades, editável no painel de trabalho ([[Sistema de NPC/Descrição|NPC]], [[Interface/Descrição|Interface]]).
- **Trabalho forçado** é uma pena com prazo; **prisioneiros de guerra:** 100 = 1 NPC Escravo, se a lei permitir ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]], [[Sistema de Batalha/Descrição|Batalha]]).
- **Grilhões:** feitos pelo Ferreiro nível 2.
- **Crimes:** "Fuga ou ajuda a escravo" (gravidade 3); libertar escravo é crime cultural na Escravista. Libertar dá −5 de reputação nas facções que permitem a escravidão ([[Sistema de Reputação/Descrição|Reputação]]).
- **Estilo Escravo:** Ambição "Conquistar a liberdade" ([[Sistema de Início de Jogo/Descrição|Início de Jogo]], [[Objetivo e Progressão/Descrição|Objetivo]]).
- A Escravista tem a **infantaria de escravos** (metade do soldo, −15 de Moral); a Guerreira vende prisioneiros à Escravista; o porto (6,6) é de pesca e de escravos ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- Os escravos capturados **mantêm a cultura de origem** ([[Sistema de Cultura/Descrição|Cultura]]).

Os valores numéricos são iniciais e ficam em dados (`escravidao.json`) para ajustar nos testes.

---

## De onde vêm os escravos

Todas as fontes só existem onde a lei de Escravidão **permite**:

| Fonte | Como |
|---|---|
| Prisioneiros de guerra | 100 prisioneiros = 1 NPC Escravo (já decidido) |
| Pena de trabalho forçado | Escravo com prazo; ao fim, volta à classe anterior (já decidido) |
| Captura por bandidos e caçadores | Viajantes e vítimas capturados em encontros são vendidos no porto (6,6) ou na Vila Livre do Sul (5,6) |
| Nascimento | Filhos de Escravos nascem Escravos, pela natalidade da classe |
| Compra | No Mercado de Escravos |

- **Venda de prisioneiros:** uma facção que **proíbe** a escravidão pode **vender** os prisioneiros a uma que permite, se houver contato e relação 0 ou mais. Recebe o preço de mercado e não guarda escravos. É o caso da Guerreira com a Escravista.
- **Dívida** (vender-se para pagar uma dívida) fica para o futuro. **[Refinar depois]**

## Mercado de Escravos

- **Construção nova** (dona: Facção ou Rico), só em facção que permite a escravidão. No primeiro dia, a Escravista tem um na Capital (8,6), na Cidade (7,5) e no porto (6,6) ([[Sistema de Construção/Descrição|Construção]]).
- **O que se compra:** **1 NPC real**, que trabalha como **1 trabalhador** numa vaga, a mesma escala de qualquer NPC real que ocupa uma vaga. O Fator de Escala só pesa na população total, como já acontece.
- **Preço base = 200 + 4 × a melhor habilidade de Trabalho** (Braçal, Manufaturado ou Especialista).

| Modificador | × |
|---|---|
| Mago | 2 |
| Velho | 0,5 |
| Cultura hostil à do comprador | 0,8 |

  Depois disso, vale a oferta e a procura do Mercado Local, como em qualquer item ([[Sistema Econômico/Descrição|Econômico]]).
- *Exemplo:* um escravo com Trabalho Braçal 80 custa 520; o mesmo escravo velho, 260.
- **Quem compra:**
  - o **jogador**: o escravo vira Escravo do jogador (Semi-Controlado, com Vínculo = jogador) e trabalha nas construções dele;
  - os **Ricos**, para as empresas;
  - a **cidade**, para vagas Braçais sem trabalhador. Em LOD 2, a compra da cidade é um fluxo por estação.
- **Vender:** o dono vende o próprio escravo no Mercado de Escravos por **70%** do preço.
- **Escravos do jogador e o grupo:** não contam no limite de vinculados (já decidido; ver [[Sistema de Companheiros e Diálogo/Descrição|Companheiros]]).

## Obediência e tratamento

**Obediência** (0 a 100):

> 50 + Medo da cidade (0 a 30) + Grilhões (+15) + tempo de cativeiro (+1 por estação, até +10) − (30 − Felicidade, se a Felicidade estiver abaixo de 30) − 10 se o escravo é de cultura Guerreira (orgulho)

- **Efeito:** a chance de cumprir a prioridade, a cada tarefa, é igual à Obediência. Abaixo de 30, ele pode fugir (ver "Fuga e rebelião").
- O Medo vem da [[Sistema de IA/Descrição|IA]] e do Controle da [[Sistema de Felicidade e Estabilidade/Descrição|Felicidade]].
- **Em LOD 2**, a Obediência é uma média por cidade, com a Felicidade da classe Escravo.

**Escolhas do dono** (o jogador, para os escravos dele; a cidade, pela IA do dono):

| Tratamento | Efeito |
|---|---|
| Comida Básica (padrão) | — |
| Comida Refinada | Felicidade +5; custa mais |
| Grilhões | Obediência +15; Felicidade −5; velocidade −20%; não fogem |
| Castigo público | Medo +5 na cidade por 8 dias; Felicidade dos Escravos −10; reputação −2 com a Religiosa |

*Exemplo:* um escravo novo, sem Grilhões, numa cidade com Medo 10 e Felicidade 25: 50 + 10 − 5 = **55** de Obediência; com Grilhões, **70**.

## Fuga e rebelião

Fecha a pergunta em aberto da Felicidade (fuga e rebelião local de Escravos).

**Fuga em LOD 0**

- À noite, o escravo com Obediência abaixo de 30 tem chance diária de fugir igual a **(30 − Obediência)%**.
- A fuga usa a detecção do crime: Percepção dos guardas contra a Destreza dele ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).
- Se escapa, vira **fugitivo**: crime "Fuga" (gravidade 3) e Recompensa na facção. Ele vai para uma Vila Livre ou um mapa sem dono; metade vira bandido ([[Sistema de Eventos/Descrição|Eventos]]).

**Fuga em LOD 2:** a cada estação, foge **(25 − Felicidade dos Escravos)%** dos escravos da cidade, se a Felicidade estiver abaixo de 25. *Exemplo:* Felicidade 15 = 10% fogem.

**Rebelião local**

- **Quando:** Felicidade dos Escravos abaixo de 10 **e** Controle abaixo de 10, por **4 dias seguidos**.
- **Em LOD 0:** os escravos se armam com o que acham e lutam no **combate local** contra os guardas (até cerca de 40 combatentes, o limite do combate local). Acima disso, vira batalha no Mapa de Guerra, onde cada NPC escravo real vira **1 Esquadrão de Milícia rebelde**, como os guardas na defesa da cidade (mesma escala: 1 NPC real = 1 Esquadrão; ver [[Sistema de Guerra/Descrição|Guerra]]).
- **Se vencem:** fogem todos, a produção Braçal da cidade cai e a cidade perde 5 de Estabilidade por 1 estação.
- **Se perdem:** 20% são executados ou vendidos.
- **Em LOD 2:** resolvida pelo cálculo rápido de forças ([[Sistema de Guerra/Descrição|Guerra]]).

**Caçadores de escravos:** uma fuga grande cria contratos de recaptura no quadro da Taverna ([[Objetivo e Progressão/Descrição|Objetivo]]).

**Grande revolta** (várias cidades ao mesmo tempo, como a de Espártaco) fica para o futuro. **[Refinar depois]**

## Libertação e abolição

- **O dono liberta** (botão "Libertar"): o NPC vira **Civil e Plebeu**, e o libertado ganha **+20 de Opinião** sobre o jogador. +2 de reputação com a Religiosa; −5 com as facções escravistas (já decidido).
- **Alforria:** nas facções com a lei de **Tolerância** religiosa, 1% dos escravos da cidade compram a liberdade por estação; na Escravista, 0% ([[Sistema de Religião/Descrição|Religião e Fé]]).
- **Abolição:** a facção muda a lei de Escravidão para "proibida" na assembleia.
  - Todos os Escravos viram **Civis em 1 estação**.
  - As Senzalas viram Casas do mesmo nível.
  - Os Ricos perdem **15 de Felicidade por 1 ano** (perdem a mão de obra).
  - A produção Braçal cai até os Civis ocuparem as vagas com salário.
  - Na Escravista, isso viola o pensamento extremo "a liberdade de escravos é crime".
- **Conquista:** uma cidade que passa a uma facção que proíbe a escravidão liberta os escravos no tratado (+10 de Felicidade dos libertos).

**Estilo Escravo: três caminhos para a liberdade**

| Caminho | Como |
|---|---|
| **Comprar** | Pagar 1,5 × o próprio preço de mercado; o jogador pode juntar dinheiro com tarefas extras |
| **Fugir** | Vira fugitivo com Recompensa na facção do dono, sem Game Over |
| **Servir** | O dono liberta depois de 4 estações com Opinião +40 |

## Tráfico, crime e diplomacia

- **Tráfico onde é proibido:** é **Contrabando** (gravidade 3). Escravos só são vendidos em lojas ilegais (mercado negro), com o preço × 1,5.
- **Libertar escravo alheio:** crime "Fuga ou ajuda a escravo" (gravidade 3); na Escravista, gravidade 5 (crime cultural).
- **Reputação do jogador:** cada venda de escravo dá −3 com a Religiosa; cada compra, −1; libertar dá +2.
- **Diplomacia:** facções que proíbem a escravidão têm −10 de relação com as que mantêm Mercado de Escravos. Na primeira versão, só a Religiosa aplica esse −10, por já ser hostil à Escravista ([[Sistema de Diplomacia/Descrição|Diplomacia]]).
- **Missões:** da Religiosa, libertar caravanas de escravos; da Escravista, recapturar fugitivos e escoltar carregamentos.

### Narcóticos (ideia do autor, rodada V3-1)

- A Escravista é a **única produtora legal** de Fumo e Pó Espiritual no início do jogo. As Fazendas de Folha de Fumo e os Misturadores dela são trabalhados sobretudo por escravos.
- Nas outras facções, os narcóticos são Contrabando ([[Sistema de Facção/Descrição|Facção]]).

---

## Parte técnica

- **Dados:** `escravidao.json` com as fontes, a fórmula e os modificadores do preço, a fórmula da Obediência, os tratamentos, as chances de fuga, as condições da rebelião, a alforria e os efeitos da abolição e do tráfico.
- **Por NPC escravo real:** Obediência, Vínculo (dono), data de início do cativeiro, Grilhões e o prazo, se for trabalho forçado.
- **Por cidade:** Obediência média, contador de dias para a rebelião e o estoque do Mercado de Escravos.
- **Frequência:** a chance de fuga em LOD 0 é sorteada uma vez por noite; em LOD 2, fuga, alforria e compra são calculadas por estação; a rebelião é um evento condicional com duração mínima ([[Sistema de Eventos/Descrição|Eventos]]).
- **Save:** os dados por NPC e por cidade acima, fugitivos com Recompensa e contratos de recaptura.
- **Fatia vertical:** não usa (a cidade da fatia vertical é Autoritária, sem escravos na primeira cadeia de produção).

## Ligação com outros sistemas

| Sistema | O que a Escravidão fornece ou recebe |
|---|---|
| Facção | Lei de Escravidão; abolição pela assembleia; venda de prisioneiros |
| Cultura | Escravista (pensamento extremo); Religiosa contra o tráfico; escravo mantém a cultura |
| NPC | Obediência, Revolta, Vínculo; Escravo do jogador Semi-Controlado |
| Trabalho e Propriedade | Escravos sem salário; vagas preenchidas por compra |
| População | Nascimento de Escravos; Senzala; Senzalas viram Casas na abolição |
| Classe Social | Classe Escravo; libertado vira Civil e Plebeu |
| Felicidade e Estabilidade | Felicidade da classe Escravo; fuga e rebelião local; Controle |
| IA | Medo na Obediência |
| Crime e Justiça | Trabalho forçado; fuga; ajuda a escravo; tráfico como Contrabando |
| Guerra e Batalha | Prisioneiros de guerra; infantaria de escravos; rebelião pelo cálculo rápido |
| Construção | Mercado de Escravos |
| Comércio e Econômico | Preço, oferta e procura; mercado negro |
| Itens | Grilhões |
| Reputação | Compra, venda e libertação |
| Diplomacia | −10 de relação da Religiosa com quem mantém Mercado de Escravos |
| Viagem | Captura em encontros; fugitivos em mapas sem dono |
| Início de Jogo e Objetivo | Estilo Escravo e os 3 caminhos; contratos de recaptura e de libertação |
| Interface | Painel de trabalho; botões "Libertar" e "Vender"; tratamento dos escravos |

## Perguntas em aberto

1. **Valores a testar:** preço, Obediência, chances de fuga, condições da rebelião, alforria, efeitos da abolição.
2. **Grande revolta** entre várias cidades (futuro).
3. **Dívida** como fonte de escravos (futuro).
4. **Escravos fora da Escravista:** a Guerreira (só prisioneiros de guerra) e a Autoritária (trabalho forçado como pena) também começam com a escravidão permitida (rodada V3-1).
