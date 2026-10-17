> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a rodada IP2 (Nomes e lista final dos itens) aplicada e a IP3 (Insumos de cada produto) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas P0 e P1 estão escritos, e a **revisão completa** está em andamento (levantamento e plano na [[Revisão Completa]]). A V1 e a IP1 já estão no `main` (PRs #9 e #10). Em 2026-10-17 foi aplicada a **IP2** (lista final e nomes dos itens; ver [[Sistema de Itens/Descrição|Sistema de Itens]]), na branch `docs/ip2-itens`. A **rodada IP3 (Insumos de cada produto) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se o pull request da `docs/ip2-itens` foi mesclado no `main`.**
3. **Próximo passo:** pedir o `Respostas.txt` da IP3 (conferir se é desta rodada), refazer a [[Sistema de Produção/Funções e Construções|Funções e Construções]] com as receitas, conferir cadeias e órfãos e abrir a **V2** (base numérica da economia).
4. Depois: V3 (pendências), V4 e V5 (propostas), como no plano da [[Revisão Completa]].
5. Quando o autor estiver satisfeito com a documentação: **ficha técnica lógica**, e só então a programação.

## 2. Como trabalhamos

- **Idioma:** sempre português do Brasil.
- **Fluxo:** o assistente manda uma lista de perguntas numeradas, cada uma com uma **sugestão**. O autor responde no arquivo `C:\Users\Felipe\Desktop\Respostas.txt` (anexado com @) ou no chat. "Seguir sugestão" aceita a proposta; a ideia própria do autor sempre prevalece. Depois o assistente escreve o documento do sistema, atualiza os outros documentos, e abre a próxima rodada.
- **Antes de aplicar respostas:** conferir se o `Respostas.txt` é mesmo da rodada atual (olhar os itens e a data de gravação).
- **Decisões do autor:** o que não tem marcação nos documentos foi decidido por ele. As marcações são **[Proposta]**, **[Em aberto]** e **[Refinar depois]** (ver [[Glossário]]).
- **Sem código por enquanto.** Foco total em documentação. Depois, uma **ficha técnica lógica** (como cada sistema vira estruturas, dados e regras) e só então a programação. O autor tem experiência mínima em C++ e nenhuma com bibliotecas: explicar em linguagem simples.
- **Preferências do autor:**
  - interface e HUD **bem personalizáveis** (ele joga em 1600 × 900 e não gosta de interface fixa);
  - notificações personalizáveis;
  - letras da interface em fontes comuns legíveis (não desenhadas à mão);
  - arte do mundo toda à mão, nada procedural;
  - só teclado e mouse.
- **Estilo dos documentos:** primeira linha com a legenda das marcações; "Descrição geral"; seções do sistema; "Parte técnica" (LOD, dados em JSON, save, fatia vertical); tabela "Ligação com outros sistemas"; "Perguntas em aberto". Cada sistema novo vira uma pasta `Sistema de X/Descrição.md`, entra no [[Descrição do Projeto]] (índice) e no Estado, e as ligações são acrescentadas nos documentos dos sistemas vizinhos. Não usar seções de "Atualizações recentes": o histórico fica no Git.
- **Princípios do projeto:** a simulação vem primeiro e o evento explica; tudo agregado por cidade e por LOD; dados em JSON; "por quê?" em tudo.

### Padrão das perguntas e sugestões (obrigatório)

Pedido do autor em 2026-10-15: as perguntas da P0-5 saíram abstratas ("B2. Tom", "D1. Equilíbrio") e as sugestões, curtas demais. A partir da P0-6, **toda rodada segue este padrão**:

1. **Contexto** (1 a 3 linhas): o que já está decidido sobre o tema e o que a pergunta destrava.
2. **Pergunta concreta:** o que exatamente precisa ser decidido, com números, nomes e um exemplo de situação de jogo. Uma pergunta por decisão; um tema com várias decisões vira vários itens.
3. **Opções** (quando houver): 2 a 4, cada uma com o que ganha e o que perde (arte, programação, desempenho, diversão).
4. **Sugestão completa:** a opção recomendada e o porquê, pronta para virar documento: valores iniciais, tabela ou lista, exemplo prático, impacto nos outros sistemas e o que fica para testar.
5. **Conflitos:** se a sugestão ou uma resposta anterior bate com outra decisão, apontar e propor a saída.

Regras extras:

- Explicar termos e ferramentas em linguagem simples.
- Quando um "seguir sugestão" deixar algo vago, o documento detalha com valores concretos marcados como **[Proposta]**.
- Economizar palavras sem cortar informação útil: nada de introduções longas.
- Formato fácil de responder no `Respostas.txt`: códigos (A1, B2...) e títulos curtos.

**Exemplo.** Abstrato (evitar): "B2. Tom: fantasia de baixa magia?". Detalhado (seguir): "B2. Quanta magia aparece no dia a dia? *Contexto:* a cultura Arcana dá privilégios aos Magos, e o Combate tem Cajado. *Opções:* (a) magia rara, cerca de 1 Mago para cada 50 NPCs, feitiços caros; (b) magia comum, todo guarda de cidade Arcana conjura. *Sugestão:* (a), porque mantém o tom sombrio e político e reduz a arte de efeitos. *Impacto:* Magia (quantidade de Magos por cidade), Combate (Esquadrão de Magos raro), arte (poucos efeitos de feitiço)."

## 3. Lista de sistemas (da Revisão)

| Prioridade | Sistema | Estado |
|---|---|---|
| P0 | Felicidade e Estabilidade | Escrito |
| P0 | População e Habitação | Escrito |
| P0 | Trabalho, Salários e Propriedade (+ Funções e Construções) | Escrito |
| P0 | Interface e Controles | Escrito |
| P0 | Mundo e Estado Inicial | Escrito (a história fica para depois) |
| P0 | Pipeline de arte e conteúdo | Escrito |
| P1 | Viagem e Mundo | Escrito |
| P1 | Reputação e Relações do jogador | Escrito |
| P1 | Companheiros, Relações familiares e Diálogo | Escrito |
| P1 | Guerra e Campanha | Escrito |
| P1 | Batalha (Mapa de Guerra) | Escrito |
| P1 | Diplomacia | Escrito |
| P1 | Religião | Escrito |
| P1 | Clima e Estações | Escrito |
| P1 | Notícias e Informação | Escrito |
| P1 | Efeitos de itens e Escravidão | Escrito |
| Revisão | Revisão completa: V1 (inconsistências e faxina) | Aplicada |
| Revisão | Itens e Produção: IP1 (padronização) | Aplicada |
| Revisão | Itens e Produção: IP2 (nomes) | Aplicada |
| Revisão | **Itens e Produção: IP3 (insumos)** | **IP3 enviada, aguardando respostas** |
| Revisão | V2 (economia numérica), V3 (pendências), V4 e V5 (propostas) | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, modo observador, eventos de dinastia. As ordens de produção em lote foram decididas na IP1-7 ([[Sistema de Produção/Descrição|Produção]]). A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: IP3, Insumos de cada produto

**Perguntas enviadas em 2026-10-17, aguardando respostas.** A IP1 decidiu a forma da receita e a IP2 fechou a lista dos itens. Esta rodada decide **do que cada produto é feito**: quantidades, nível e era de cada receita. Com ela, a [[Sistema de Produção/Funções e Construções|Funções e Construções]] será refeita.

Responder no `Respostas.txt` pelos códigos (IP3-1, IP3-2...). "Seguir sugestão" aceita a proposta. Nas tabelas, dá para aceitar e trocar só algumas linhas, por exemplo: "Seguir sugestão, mas o Queijo com 4 Leite". As quantidades são iniciais: a V2 ajusta os números quando calcular os preços.

**Já decidido (base das perguntas):**

- **Receita (IP1-1):** uma receita fixa por produto, com 8 campos:
  - produto e quantidade;
  - insumos;
  - subprodutos;
  - construção e nível mínimo;
  - Função e Trabalho;
  - habilidade mínima;
  - horas;
  - era.
- **Limites (IP1-8):** no máximo 3 insumos e 3 saídas; cadeia de até 4 passos do mapa ao produto; nenhum item órfão; sem ferramentas.
- **Níveis (IP1-3):**

| Nível da construção | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|
| Tier até | 1 | 2 | 2 | 3 | 4 |
| Rank até | 1 | 2 | 3 | 4 | 5 |

  O Rank também tem teto por era: até 3 na Era 1, 4 nas Eras 2 e 3, e 5 na Era 4.
- **Horas por unidade (IP1-6):** Tier 0 = 0,5; Tier 1 = 1; Tier 2 = 2; Tier 3 = 4; Tier 4 = 8. Armas e peças: Ferro 8, Aço 12, Aço Mágico 24.
- **Escada de materiais (IP2-5):**
  - Ferro, Aço, Aço Mágico;
  - de Madeira, Composto, Arcano;
  - de Couro, Cravejado, Arcano.
- **Pedra de Mana (IP2-3, ideia do autor):** um único item bruto, sem refino.
- **Armadura:** cada receita faz as 3 peças juntas (já decidido no [[Sistema de Itens/Descrição|Sistema de Itens]]).
- **Munição:** o NPC gasta 1 unidade por tiro; o Esquadrão gasta 1 unidade por batalha ([[Sistema de Combate/Descrição|Combate]], [[Sistema de Guerra/Descrição|Guerra]]).

**IP3-1. As regras de quantidade.**

- *Contexto:* sem uma regra comum, cada uma das cerca de 120 receitas teria números soltos, difíceis de balancear na V2.
- *Pergunta:* que regras todas as receitas seguem?
- *Sugestão:* 6 regras:
  1. **Lote pelo Tier**, para que um ciclo de bem de consumo leve cerca de 4 horas, meio turno:

| Tier do produto | Lote (unidades por ciclo) | Horas do ciclo |
|---|---|---|
| 1 | 4 | 4 |
| 2 | 2 | 4 |
| 3 | 1 | 4 |
| 4 | 1 | 8 |

  2. **Itens com Rank:** 1 por ciclo, e as horas seguem o degrau do material:
     - 8 h para Ferro, Cobre, Madeira e Couro;
     - 12 h para Aço, Composto e Cravejado;
     - 24 h para Aço Mágico e Arcano.

     A armadura faz 3 peças por ciclo, então leva 3 vezes isso (24, 36 ou 72 h). A artilharia leva 3 vezes o tempo de uma arma.
  3. **"3 insumos" são 3 tipos de item**, em qualquer quantidade: "8 Barra de Aço + 2 Tábua + 2 Barra de Cobre" conta como 3.
  4. **Nível mínimo:**
     - nos bens, pelo Tier do produto: Tier 1 no nível 1, Tier 2 no 2, Tier 3 no 4 e Tier 4 no 5;
     - nos itens com Rank, pelo degrau do material: degrau 3 no nível 1; degrau 4 no nível 2 (e Era 2); degrau 5 no nível 5 (e Era 4).

     Depois disso, subir de nível só aumenta o teto de Rank e a velocidade (+10% por nível).
  5. **Munição em lote:** 1 ciclo de 4 h faz 20 Flechas, 10 Virotes ou 20 Cartuchos, porque o NPC gasta 1 por tiro. A Bala de Canhão sai 1 por ciclo, porque 1 unidade é o suprimento de uma batalha (30 tiros).
  6. **Exceção da Forja Arcana:** ela tem um único produto, então faz a Barra de Aço Mágico desde o nível 1. A trava é só a era (Era 4), e o nível dá velocidade e vagas.
- *Exemplo:* um Ferreiro nível 2, na Era 2, pode fazer Espada Curta de Aço (degrau 4, nível 2) em 12 h. O Rank dela vai até 2, pelo teto do nível.
- *Impacto:* todas as receitas abaixo; a V2 (preço ≈ insumos + horas × salário).

**IP3-2. Fazenda, Posto de Coleta, Pesqueiro, Pasto, Rancho e Açougue.**

- *Contexto:*
  - As construções **Braçais** tiram do mapa: não têm insumos, e o rendimento depende do recurso do mapa e da estação.
  - O **Rancho** usa os animais sem gastá-los, e o **Açougue** gasta o animal.
  - A produtividade base de cada recurso fica para a V2.
- *Sugestão:*
  - **Braçais:** 1 unidade a cada 0,5 h de trabalho (Tier 0), 1 h (Tier 1: Minério de Ouro, Fruta, Flor Espiritual, Pedra de Mana) e 2 h (Tier 2: Especiarias). Isso é multiplicado pela produtividade do recurso no mapa e pela estação ([[Sistema de Clima e Estações/Descrição|Clima]]).
  - **[Proposta]** O **Adubo** é um insumo **opcional** da Fazenda: com Adubo no estoque, ela gasta 1 por ciclo e rende +25%. Esse é o uso que impede o Adubo de ficar órfão.
  - **Pasto:** sem insumo (o pasto é o capim do mapa). Precisa de **2 animais da mesma espécie no estoque**, que não são gastos, e faz 1 animal novo por ciclo:

| Animal | Horas por ciclo |
|---|---|
| Galinha | 4 |
| Porco | 8 |
| Ovelha | 8 |
| Vaca | 16 |
| Cavalo | 24 |

  - **Rancho:** cada animal no estoque permite **1 ciclo por dia**, de 2 h:

| Animal | Produto | Subproduto |
|---|---|---|
| Galinha | 4 Ovo | — |
| Ovelha | 2 Lã | 1 Dejetos Animais |
| Vaca | 4 Leite | 2 Dejetos Animais |
| Porco | — | 2 Dejetos Animais |

  - **Açougue** (gasta o animal; 4 h):

| Animal | Saídas |
|---|---|
| 1 Galinha | 2 Carne Crua |
| 1 Porco | 6 Carne Crua, 1 Couro Cru, 1 Ossos |
| 1 Ovelha | 4 Carne Crua, 1 Couro Cru, 1 Ossos |
| 1 Vaca | 8 Carne Crua, 2 Couro Cru, 2 Ossos |

    O Cavalo não vai para o Açougue: é montaria e carga.
- *Conflitos:*
  - O Açougue precisa de **uma receita por animal** para o mesmo produto (Carne Crua). Isso é a exceção que a IP1-1 permitiu "se a IP3 pedir". A saída é aceitar a exceção só no Açougue.
  - O Pasto e o Rancho precisam de um campo novo, "**Exige no estoque (não gasta)**", que não está entre os 8 da IP1-1. A saída é acrescentar esse campo opcional, usado só por eles.
- *Impacto:* Produção, Recursos (produtividade na V2), Comércio (animais comprados para começar um Pasto).

**IP3-3. Metalurgia, Forja Arcana e Pedreira.**

- *Sugestão:*

| Construção | Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|---|
| Metalurgia | Barra de Ferro | 4 | 4 Minério de Ferro + 2 Carvão | 1 | 1 | 4 |
| Metalurgia | Barra de Cobre | 4 | 4 Minério de Cobre + 1 Carvão | 1 | 1 | 4 |
| Metalurgia | Barra de Aço | 2 | 2 Barra de Ferro + 2 Carvão | 2 | 2 | 4 |
| Metalurgia | Barra de Ouro | 1 | 3 Minério de Ouro + 1 Carvão | 4 | 3 | 4 |
| Forja Arcana | Barra de Aço Mágico | 1 | 1 Barra de Aço + 1 Pedra de Mana | 1 (exceção) | 4 | 8 |
| Pedreira | Bloco de Pedra | 4 | 4 Pedra | 1 | 1 | 4 |
| Pedreira | Cerâmica | 4 | 3 Argila + 1 Tora (lenha do forno) | 1 | 1 | 4 |

  - **Cadeia mais longa do jogo**, exatamente no limite de 4 passos: Minério de Ferro → Barra de Ferro → Barra de Aço → Barra de Aço Mágico → Espada.
  - O **Carvão** vira o gargalo do metal: a Guerreira e a Tecnocrata (com destaque em Carvão) ganham força, e a Arcana (sem Carvão) precisa comprá-lo.
- *Impacto:* Itens, Comércio, Mundo (recursos de destaque), V2.

**IP3-4. Carpinteiro.**

- *Sugestão:* bens e munição:

| Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|
| Tábua | 4 | 2 Tora | 1 | 1 | 4 |
| Móveis de Madeira | 2 | 3 Tábua | 2 | 1 | 4 |
| Móveis Refinados | 1 | 2 Tábua + 1 Tecido de Algodão + 1 Barra de Cobre | 4 | 3 | 4 |
| Flecha | 20 | 2 Tábua + 1 Barra de Ferro | 1 | 1 | 4 |
| Virote | 10 | 2 Tábua + 1 Barra de Ferro | 1 | 1 | 4 |

  **Moldes** (1 por ciclo; as horas e o nível seguem o degrau, IP3-1):

| Molde | de Madeira (degrau 3) | Composto (degrau 4) | Arcano (degrau 5) |
|---|---|---|---|
| Arco Curto | 1 Tábua + 1 Tecido de Lã (corda) | 1 Tábua + 1 Ossos + 1 Couro Refinado | 1 Tábua + 1 Pedra de Mana + 1 Tecido de Lã |
| Arco Longo | 2 Tábua + 1 Tecido de Lã | 2 Tábua + 1 Ossos + 1 Couro Refinado | 2 Tábua + 1 Pedra de Mana + 1 Tecido de Lã |
| Escudo de Madeira | 2 Tábua + 1 Couro Refinado | — | — |
| Cajado Simples de Madeira | 2 Tábua + 1 Pedra de Mana | — | — |

| Molde (besta) | de Madeira | de Aço | de Aço Mágico |
|---|---|---|---|
| Besta Leve | 2 Tábua + 1 Barra de Ferro | 1 Tábua + 2 Barra de Aço | 1 Tábua + 2 Barra de Aço Mágico |
| Besta Pesada | — | 2 Tábua + 3 Barra de Aço | 2 Tábua + 3 Barra de Aço Mágico |
| Besta de Repetição | — | 2 Tábua + 2 Barra de Aço + 1 Barra de Cobre | 2 Tábua + 2 Barra de Aço Mágico + 1 Barra de Cobre |

- *Impacto:* Itens, Batalha (Arqueiros e Besteiros), Magia (o Cajado Simples gasta Pedra de Mana).

**IP3-5. Ferreiro.**

- *Sugestão:* **moldes**. "B" é a barra do material: Barra de Ferro (nível 1, 8 h), Barra de Aço (nível 2, Era 2, 12 h) ou Barra de Aço Mágico (nível 5, Era 4, 24 h).

| Molde | Insumos |
|---|---|
| Espada Curta, Lança Curta, Machado Curto, Clava Curta | 1 B + 1 Tábua (cabo) |
| Espada Longa, Machado Longo, Clava Longa | 2 B + 1 Tábua |
| Lança Longa | 1 B + 2 Tábua (haste) |
| Escudo de metal | 2 B + 1 Tábua |
| Armadura Média (Coifa, Cota e Perneiras de Malha) | 6 B + 1 Tecido de Lã (acolchoado) |
| Armadura Pesada (Elmo, Couraça e Grevas) | 8 B + 2 Couro Refinado (forro e correias) |

  Mais dois produtos fixos:

| Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|
| Grilhões (Tier 2) | 2 | 2 Barra de Ferro | 2 | 1 | 4 |
| Chapéu de Mineiro (1-2) | 1 | 1 Barra de Ferro + 1 Couro Refinado | 1 | 1 | 8 |

- *Exemplo:* um kit de Infantaria Pesada de Aço:
  - Espada Longa: 2 Barras;
  - armadura Pesada: 8 Barras;
  - Escudo: 2 Barras.

  São 12 Barras de Aço = 24 Barras de Ferro + 24 Carvão. Isso dá peso real à guerra na economia.
- *Impacto:* Itens, Batalha (custo dos kits), Escravidão (Grilhões), V2.

**IP3-6. Armeiro.**

- *Sugestão:* "B" é a barra do material. O cobre entra nas peças de latão (gatilho e guarnições), como a IP2-6 previu.

| Produto | Insumos | Nível | Era | Horas |
|---|---|---|---|---|
| Arcabuz de Ferro | 2 Barra de Ferro + 1 Tábua + 1 Barra de Cobre | 1 | 2 | 8 |
| Mosquete (Aço ou Aço Mágico) | 2 B + 1 Tábua + 1 Barra de Cobre | 2 ou 5 | 3 ou 4 | 12 ou 24 |
| Carabina (Aço ou Aço Mágico) | 1 B + 1 Tábua + 1 Barra de Cobre | 2 ou 5 | 3 ou 4 | 12 ou 24 |
| Fuzil Raiado de Aço Mágico | 3 Barra de Aço Mágico + 1 Tábua + 1 Barra de Cobre | 5 | 4 | 24 |
| Cartucho (lote 20) | 1 Barra de Ferro + 2 Pólvora + 1 Tecido de Algodão (o papel do cartucho) | 1 | 2 | 4 |
| Bombarda de Ferro | 8 Barra de Ferro + 2 Tábua | 1 | 2 | 24 |
| Canhão (Aço ou Aço Mágico) | 8 B + 2 Tábua + 2 Barra de Cobre | 2 ou 5 | 3 ou 4 | 36 ou 72 |
| Bala de Canhão (1 por batalha) | 2 Barra de Ferro + 3 Pólvora | 1 | 2 | 4 |

- *Impacto:* Itens, Batalha (Atiradores e Artilharia), Tecnologia (pólvora na Era 2).

**IP3-7. Costureiro.**

- *Sugestão:*

| Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|
| Tecido de Algodão | 4 | 4 Algodão | 1 | 1 | 4 |
| Tecido de Lã | 4 | 4 Lã | 1 | 1 | 4 |
| Couro Refinado | 4 | 4 Couro Cru + 2 Água | 1 | 1 | 4 |
| Roupa Comum | 4 | 2 Tecido de Lã | 1 | 1 | 4 |
| Roupa Fina | 1 | 2 Tecido de Algodão + 1 Especiarias (açafrão e outras tinturas) | 4 | 3 | 4 |
| Sela | 2 | 3 Couro Refinado + 1 Barra de Ferro (fivelas) | 2 | 1 | 4 |

  **Roupas equipáveis:** 1 peça por ciclo, de 4 h (2 h no conjunto Pobre).
  - Molde: chapéu ou capuz = 1 tecido; camisa ou túnica = 2; calças = 2. O tecido é o do conjunto (IP2-8).
  - Os conjuntos Mercador e Nobre gastam também 1 Especiarias (a tintura), e saem no nível 2.
  - Os outros conjuntos saem no nível 1.

  **Armadura Leve** (3 peças por ciclo):

| Material | Insumos | Nível | Era | Horas |
|---|---|---|---|---|
| de Couro | 6 Couro Refinado | 1 | 1 | 24 |
| Cravejado | 5 Couro Refinado + 2 Barra de Aço | 2 | 2 | 36 |
| Arcano | 5 Couro Refinado + 1 Pedra de Mana | 5 | 4 | 72 |

- *Impacto:* Itens, Classe Social (Roupa Comum e Roupa Fina), Batalha (kits leves), Comércio (Algodão da Escravista, Lã da Religiosa).

**IP3-8. Artesão.**

- *Sugestão:*

| Produto | Insumos | Nível | Era | Horas |
|---|---|---|---|---|
| Joias (Tier 4) | 2 Barra de Ouro | 5 | 4 | 8 |
| Ornamento (Tier 4) | 1 Barra de Ouro + 2 Barra de Cobre | 5 | 4 | 8 |
| Coroa (1-5) | 3 Barra de Ouro | 4 | 3 | 24 |
| Anel de Cobre (1-3) | 1 Barra de Cobre | 1 | 1 | 8 |
| Anel de Ouro (1-5) | 1 Barra de Ouro | 4 | 3 | 8 |
| Anel de Mana (1-5) | 1 Barra de Ouro + 1 Pedra de Mana | 4 | 3 | 12 |
| Joia de Mana (1-5) | 1 Barra de Cobre + 2 Pedra de Mana | 3 | 1 | 12 |
| Cajado Simples de Cobre (1-3) | 2 Barra de Cobre + 1 Pedra de Mana | 1 | 1 | 8 |
| Cajado Arcano (Aço ou Aço Mágico) | 2 B + 1 Tábua + 1 Pedra de Mana | 2 ou 5 | 2 ou 4 | 12 ou 24 |
| Cetro de Aço Mágico (1-4) | 1 Barra de Aço Mágico + 1 Pedra de Mana + 1 Barra de Ouro | 5 | 4 | 24 |

- *Conflito:* a regra do degrau (IP3-1) não cobre o ouro nem a Pedra de Mana nas joias. A saída é usar o Tier da barra principal: a Barra de Ouro é Tier 3, então nível 4. A **Joia de Mana** fica no nível 3, para os Magos terem acesso cedo; o Rank dela continua limitado pelo nível e pela era.
- *Impacto:* Itens, Magia (Joia e Anel de Mana), Classe Social (Joias e Ornamento são Exclusivos).

**IP3-9. Cozinheiro e Destilaria.**

- *Sugestão:*

| Construção | Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|---|
| Cozinheiro | Pão | 4 | 2 Grão + 1 Água | 1 | 1 | 4 |
| Cozinheiro | Sopa | 4 | 2 Legumes + 1 Água | 1 | 1 | 4 |
| Cozinheiro | Peixe Defumado | 4 | 3 Peixe + 1 Tora | 1 | 1 | 4 |
| Cozinheiro | Ração Militar | 4 | 2 Grão + 1 Carne Crua | 1 | 1 | 4 |
| Cozinheiro | Torta de Fruta | 2 | 1 Fruta + 1 Grão + 1 Ovo | 2 | 1 | 4 |
| Cozinheiro | Linguiça | 2 | 2 Carne Crua + 1 Especiarias | 2 | 1 | 4 |
| Cozinheiro | Queijo | 2 | 3 Leite | 2 | 1 | 4 |
| Cozinheiro | Assado Temperado | 1 | 2 Carne Crua + 1 Especiarias + 1 Tora | 4 | 3 | 4 |
| Cozinheiro | Presunto Curado | 1 | 3 Carne Crua + 1 Especiarias | 5 | 4 | 8 |
| Cozinheiro | Doce de Especiarias | 1 | 2 Fruta + 1 Especiarias + 1 Leite | 5 | 4 | 8 |
| Destilaria | Cerveja | 2 | 2 Grão + 1 Água | 2 | 1 | 4 |
| Destilaria | Vinho | 1 | 3 Fruta | 4 | 3 | 4 |
| Destilaria | Licor | 1 | 1 Vinho + 1 Especiarias | 5 | 4 | 8 |
| Destilaria | Perfume | 1 | 1 Flor Espiritual + 1 Especiarias + 1 Água | 4 | 3 | 4 |

- *Conflito:* a escada põe os bens de Tier 3 e 4 nas Eras 3 e 4. Com isso, **no começo do jogo (Era 1) não existem Luxo nem Exclusivos**, e a Nobreza e a Realeza ficariam sempre sem esses consumos. Saídas:
  - (a) separar a **era do Tier** dos bens de consumo: os bens de Tier 3 e 4 já existem na Era 1, e só as armas seguem a era;
  - (b) a Felicidade não cobra Luxo e Exclusivos enquanto a era não os liberar.

  **Sugestão:** (a), porque o mundo começa no Ano 412 com reinos estabelecidos, que já têm vinho e joias. Assim:
  - a **era trava só os materiais e os itens com Rank** (Aço, Pólvora, Aço Mágico);
  - os bens de consumo ficam travados só pelo nível da construção;
  - nas tabelas acima, os bens de Tier 3 e 4 passam a ser da Era 1. Isso vale também para a Barra de Ouro, que é material de joia e de luxo.
- *Impacto:* Tecnologia (o texto "a Era N libera o Tier N" passa a valer só para materiais de equipamento), Classe Social, Felicidade, Mundo (as capitais começam com construções de nível 4 e 5).

**IP3-10. Misturador e Boticário.**

- *Sugestão:*

| Construção | Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|---|
| Misturador | Adubo | 4 | 3 Dejetos Animais + 1 Ossos (farinha de osso) | 1 | 1 | 4 |
| Misturador | Pólvora | 2 | 2 Carvão + 2 Dejetos Animais (o salitre vinha do esterco curtido) | 2 | 2 | 4 |
| Misturador | Fumo | 2 | 2 Folha de Fumo | 2 | 1 | 4 |
| Misturador | Pó Espiritual | 1 | 3 Flor Espiritual | 4 | 1 (IP3-9) | 4 |
| Boticário | Curativo | 4 | 1 Tecido de Algodão + 1 Água | 1 | 1 | 4 |
| Boticário | Remédio | 1 | 1 Flor Espiritual + 1 Especiarias | 4 | 1 (IP3-9) | 4 |

  - A Pólvora feita de Carvão e Dejetos Animais liga a guerra da Era 2 ao Rancho e ao Posto de Coleta: quem tem gado e carvão faz pólvora.
- *Impacto:* Itens, Guerra (Pólvora), Cura (Curativo e Remédio, ingredientes já decididos), Efeitos de Itens.

**IP3-11. As Funções: nomes, categorias, Engenheiro e Operário.**

- *Contexto:* a [[Sistema de Produção/Funções e Construções|Funções e Construções]] tem 4 perguntas em aberto:
  - o Engenheiro e o Operário ainda não têm construção;
  - os nomes das Funções não foram confirmados;
  - falta a categoria de produção de cada Função;
  - faltam as vagas por nível.

  A [[Sistema de Construção/Descrição|Construção]] também pergunta de quem é a "Inteligência exigida para construir".
- *Sugestão:*
  - **Construtor** (a construção) passa a ter duas Funções:
    - **Operário** (Trabalho Braçal), que faz a obra e a manutenção;
    - **Engenheiro** (Inteligência), o gerente da obra.

    A **Inteligência exigida para construir** é a do Engenheiro, o que responde também à pergunta da Construção. Sem Engenheiro, o Construtor só faz obras de nível 1.
  - **Categoria:** a habilidade que conta é a da receita, não a da construção:
    - **Manufaturado** nos bens de Tier 1 e 2 e nos itens de degrau 3;
    - **Especialista** nos bens de Tier 3 e 4 e nos itens de degrau 4 e 5.

    Isso generaliza o "Ferreiro e Costureiro mudam nos níveis altos". O Armeiro, o Artesão, o Boticário e a Forja Arcana são sempre Especialistas.
  - **Nomes das Funções** confirmados como estão: Armeiro, Artesão, Açougueiro, Boticário, Carpinteiro, Coletor, Costureiro, Cozinheiro, Destilador, Fazendeiro, Ferreiro, Mago de Ofício, Metalúrgico, Misturador, Pedreiro, Pescador e Rancheiro.
  - **Vagas por nível:** a tabela geral do [[Sistema de Trabalho e Propriedade/Descrição|Trabalho]] (2, 3, 5, 7, 10) vale para todas; a pergunta fecha.
- *Impacto:* Funções e Construções, Construção, Trabalho, NPC (habilidade usada).

Depois das respostas:

- refazer a [[Sistema de Produção/Funções e Construções|Funções e Construções]] com as receitas, nível por nível;
- pôr as receitas no [[Sistema de Produção/Descrição|Produção]] (ou num documento próprio de receitas);
- conferir as cadeias, os órfãos e os limites;
- abrir a **V2** (base numérica da economia).

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). A família de cada uma foi fixada na IP1-2 (a Espada está no Ferreiro), e as receitas vêm na IP3 ([[Sistema de Produção/Funções e Construções|Funções e Construções]]).
- **Arte:** o autor desenha por enquanto (sem experiência em pixel art), no **Pixelorama** (grátis), com pacotes de terceiros provisórios na fatia vertical. Mapas no **Tiled** (explicar em linguagem simples). Sem verba para programas pagos. Projeto **sem prazo**: o limite da arte é o peso no jogo, não o tempo (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
- **Estações na arte:** só mudança de cor por enquanto; **toda arte extra de estação fica para o futuro** (neve, rios congelados, folhas, flores).
- **Viagem:** 4 h por Mapa Local a pé, multiplicada por terreno, infraestrutura, tamanho do grupo, montaria e carga; caravanas sempre reais no Mapa Mundial (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).
- **Grupo:** controlado = 4 + Renome ÷ 20; vinculados = 10 + Renome ÷ 5 + 3 por cidade; **sem retratos** nas conversas (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).
- **Linguagem:** o foco é **C++**; ferramentas em outras linguagens só como ajuda, nunca essenciais.
- **Guerra e Batalha:** Placar de Guerra (−100 a +100) paga a paz; cidade ocupada só muda de dono no tratado; exército de até 20 Esquadrões com General; **o mundo para durante a batalha**; a simulação é sempre por Esquadrão de 100 soldados, e o jogador escolhe o **tamanho visual** (20, 50 ou 100 bonecos por Esquadrão; até 4.000 soldados na tela, pedido do autor). A 7ª camada do Tiled é a Elevação. Na programação, a guerra por cálculo rápido vem antes do Mapa de Guerra (ver [[Sistema de Guerra/Descrição|Guerra]], [[Sistema de Batalha/Descrição|Batalha]]).
- **Diplomacia:** estado (Paz, Guerra, Trégua) + tratados que se somam + atitude (faixa da relação); Confiabilidade; vassalo tem leis próprias (ver [[Sistema de Diplomacia/Descrição|Diplomacia]]).
- **Religião:** uma fé por cultura, com Devoção (Alta na Religiosa, nenhuma na Tecnocrata); cada Sacerdote atende 20 NPCs; fator Religião de −5 a +10; 2 festivais por ano (4 na Religiosa); lei de Tolerância religiosa; heresia é um ato; assimilação de 1% a 4% por estação; Bênção e Anátema do Sumo Sacerdote (ver [[Sistema de Religião/Descrição|Religião e Fé]]).
- **Clima:** Fazenda 75%, 100%, 150% e 25% nas quatro estações (base temperada); clima do dia sorteado por região, com 60% de repetir; lenha no inverno (1 Tora por NPC real a cada 4 dias, × bioma); vontade de guerra −15 no inverno; noite com visão −30%; arte só com paleta, tom e partículas (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Notícias:** 3 h por mapa (o mundo em 1 dia); Local, Regional e Mundial; precisão cai com a distância; a IA sabe tudo na hora (simplificação) (ver [[Sistema de Notícias/Descrição|Notícias]]).
- **Efeitos de itens:** só Controlados e Importantes usam itens com efeito individual; Livro +3 (até 60) e Livro Antigo +6 (até 80), sem se gastar; Livro de Atributos +2 × Rank, se gasta; **Livro Sagrado obrigatório** na Religiosa (ideia do autor; sem ele na revista, heresia de gravidade 2 na primeira vez); Artefatos da Realeza como posse; estado Doente e traço Viciado (ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).
- **Escravidão:** 5 fontes; Mercado de Escravos com preço 200 + 4 × a melhor habilidade de Trabalho; Obediência com fórmula; fuga abaixo de 30 de Obediência; rebelião com Felicidade e Controle abaixo de 10 por 4 dias; abolição liberta tudo em 1 estação; 3 caminhos do estilo Escravo (ver [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]).
- **Revisão V1:** a pesca voltou (Pesqueiro); animais são itens (Cavalo = montaria e carga); um inventário por cidade, com pilhas; Mosquete (não Musket); Fome dos Controlados cai 100 em 36 horas; Templo com a tabela geral de vagas (2 nas capitais); crime de Sedição; os 13 documentos antigos estão no formato padrão.
- **Produção (IP1):** uma receita fixa por produto; cada item numa só construção (Aço Mágico só na Forja Arcana); escada de níveis 1 a 5 (Tier 1, 2, 2, 3, 4 e Rank 1 a 5) mais a trava da era (Rank até 3, 4, 4, 5); Rank = menor entre material, nível e 1 + habilidade ÷ 25; **1 NPC real = 1 trabalhador e 1 consumidor**; horas por unidade de 0,5 a 8 por Tier (armas 8, 12 e 24); fila de ordens do jogador (Fazer N, Manter estoque, Repetir); no máximo 3 insumos, 4 passos e nenhum item órfão.
- **Itens (IP2):** nomes de época, sem embalagem e no singular (Grão, Tora, Minério de Ferro, Carne Crua, Ovo); nomes de fantasia só no futuro (ideia do autor); **só a Pedra de Mana**, bruta e sem refino (ideia do autor); Pólvora, Adubo, Bloco de Pedra, Tecido de Algodão e de Lã; escada de materiais (de Madeira, Composto, Arcano; de Couro, Cravejado, Arcano); Arcabuz, Fuzil Raiado, Cetro, Flecha, Virote, Cartucho, Bombarda e Canhão; armadura **Leve de couro** (Costureiro), **Média de malha** e **Pesada de placas** (Ferreiro); roupas em conjuntos por papel social; Vinho, Peixe Defumado, Roupa Comum e Roupa Fina; **consumo pelo Tier** (Tier 3 = Luxo, Tier 4 = Exclusivo); cerca de 180 itens.
- **Mundo:** 48 Mapas Locais (8 × 6), 6 regiões de 8 mapas; partida no Ano 412; nomes significativos até a história ser escrita (ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- **Git:** o repositório é `FelipeSKT/Projeto-Jogo`. Cada rodada vira um pull request para o autor aceitar.

## 6. Cuidados técnicos (para quem edita)

- **Ferramentas do PC:** g++ 15.2, CMake, Ninja, Git e Python 3.11. **Não há** `gh` (GitHub CLI) nem Visual Studio. O Python precisa ser chamado com `-I` ao ler arquivos do vault.
- **Edição em lote:** usar um script Python que carrega cada arquivo, troca trechos exatos com **verificação de que o trecho existe uma única vez**, e só então grava (com modo de teste "dry-run" antes). Preservar as quebras de linha (CRLF ou LF) e o BOM de cada arquivo. Backup da pasta antes de edições grandes (a pasta é um repositório Git, mas o backup extra é barato).
- **Conferir links:** o vault não deve ter `[[link]]` quebrado. Em tabelas, o separador do alias precisa ser escapado: `[[Pasta/Descrição\|Texto]]`.
- **Console do Windows:** a saída do PowerShell mostra acentos corrompidos; é só na tela, os arquivos estão em UTF-8.
- **Git:** o primeiro commit desta sessão falhou por causa da mensagem em *here-string*; usar `git commit -F arquivo`. Rodar `git fetch` antes de editar, porque outras sessões podem ter mexido no repositório. Não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json` (o Obsidian os altera sozinho). O `.gitignore` já ignora o workspace do Obsidian.
- **Sessões na nuvem (claude.ai/code):** o `Respostas.txt` chega como anexo na conversa; trabalhar na branch indicada pela sessão. O assistente só cria o pull request se o autor pedir.
- **Pull request (no PC):** sem o `gh`, o assistente envia a branch e o autor abre o PR pelo link `https://github.com/FelipeSKT/Projeto-Jogo/pull/new/NOME-DA-BRANCH`. O navegador interno não está logado no GitHub e o assistente nunca digita senhas. A extensão Claude in Chrome pode permitir criar o PR, se estiver conectada.
- **Nomes de itens:** a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] manda sobre os canvases.
