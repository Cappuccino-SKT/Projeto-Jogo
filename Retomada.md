> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-16, com as rodadas P1-4 e P1-5 aplicadas e a P1-6 enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** a **rodada P1-6 (Efeitos de itens e Escravidão), a última do P1, foi enviada e aguarda respostas** (seção 4). Em 2026-10-16 foram aplicadas a P1-4 (Guerra, Batalha e Diplomacia; pull request #7, já mesclado) e a P1-5 (Religião e Fé, Clima e Estações, Notícias: três documentos novos), esta na branch `docs/p1-5-religiao-clima-noticias`, com o pull request aberto para o autor aceitar. **Ao retomar, rodar `git fetch` e conferir se ele foi mesclado no `main`.** Antes disso, a sessão de 2026-10-15 aplicou cinco rodadas (P0-5 Mundo, P0-6 Pipeline de arte, P1-1 Viagem, P1-2 Reputação e P1-3 Companheiros e Diálogo), fechou o P0, criou o `CLAUDE.md` e o padrão de perguntas detalhadas (seção 2) e registrou a ideia futura dos Títulos de Nobreza.
3. **Próximo passo:** pedir o `Respostas.txt` da P1-6 (conferir se é desta rodada; o arquivo pode ainda ser o da P1-5) e escrever os documentos de Escravidão e de Efeitos de Itens. Depois, seguir a resposta da D2 (o que vem depois do P1).
4. Depois do P1: pela sugestão da D2, 1 ou 2 rodadas de pendências que travam a ficha técnica.
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
| P1 | **Efeitos de itens e Escravidão** | **Rodada P1-6 enviada, aguardando respostas** |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, ordens de produção em lote, modo observador, eventos de dinastia. A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: P1-6, Efeitos de itens e Escravidão

**Perguntas enviadas em 2026-10-16, aguardando respostas.** É a última rodada do P1 (item 4.16 da [[Revisão do Projeto]]). Junta dois temas que hoje estão espalhados: o que os itens especiais fazem ao serem usados (Itens, Cura, Magia, Classe Social) e a escravidão (NPC, Crime, Cultura, Trabalho, Guerra). Responder no `Respostas.txt` pelos códigos (I1, E2...). "Seguir sugestão" aceita a proposta. Valores numéricos são iniciais, a testar.

**Já decidido (base das perguntas):**

- **Artefatos** na lista de itens: Livro (Rank 2), Livro Antigo (Rank 4), Livro de Atributos (Rank 1-5), Joias Mágicas de Pedra Mágica (Rank 1-5) e Artefato (Rank 1-5); a cadastrar: Grimório (ensina feitiços) e Anel Mágico (Artesão nível 5). A **Realeza consome Artefatos** (Livro, Joia Mágica e Artefato); as **masmorras lendárias** guardam Artefatos ([[Sistema de Itens/Descrição|Itens]], [[Sistema de Classe Social/Descrição|Classe Social]], [[Objetivo e Progressão/Descrição|Objetivo]]).
- **Medicamento** existe como tipo, sem itens; o **Boticário** produz; proposta antiga: um curativo (Tier 1) e um remédio forte (Tier 3) com Flor Espiritual. Em aberto: a doença em LOD 0 é um estado "Doente" ou só um modificador da cidade ([[Sistema de Cura/Descrição|Cura]]).
- A **Pedra de Mana** recupera Mana; não há poção de mana ([[Sistema de Magia/Descrição|Magia]]).
- **Narcóticos** (Fumo, Pó Espiritual) são Bens de Luxo e proibidos na Religiosa; o grupo come 1 refeição por dia na viagem ([[Sistema de Viagem/Descrição|Viagem]]).
- **Escravidão:**
  - É uma lei de facção (permitida ou proibida). Escravos não têm salário e produzem para o dono, que paga a comida e a Senzala.
  - O NPC escravo tem **Obediência**, **Revolta** e **Vínculo**; o Escravo do jogador é Semi-Controlado e segue a Lista de Prioridades.
  - **Trabalho forçado** é pena com prazo. **Prisioneiros de guerra:** 100 = 1 NPC Escravo, se a lei permitir. **Grilhões** são feitos pelo Ferreiro nível 2.
  - **Crimes:** "Fuga ou ajuda a escravo" (gravidade 3); libertar escravo é crime cultural na Escravista. Libertar escravos dá −5 de reputação nas facções escravistas.
  - **Estilo Escravo:** Ambição "Conquistar a liberdade". A Escravista tem a "infantaria de escravos" (metade do soldo, −15 de Moral). A Guerreira vende prisioneiros à Escravista; o 2º vilarejo Escravista (6,6) é porto de pesca e de escravos.
  - Em aberto na Felicidade: como funcionam a **fuga e a rebelião local** de Escravos.

### I. Efeitos de itens

**I1. Livro e Livro Antigo.**

- *Contexto:* existem na lista, sem efeito e sem quem os produza; a Realeza os "consome" (ver I4).
- *Pergunta:* o que acontece quando um NPC lê um Livro?
- *Opções:* (a) **livro de estudo que não se gasta**: ensina uma habilidade de fora do combate, uma vez por leitor; (b) livro que se gasta e dá bônus temporário; (c) só objeto de valor e de consumo da Realeza, sem leitura.
- *Sugestão:* (a).
  - Cada Livro tem um **assunto**, sorteado quando é criado ou achado: Inteligência, Carisma, Percepção, Trabalho Braçal, Manufaturado ou Especialista.
  - **Ler** é uma ação de 8 horas, fora de combate (no descanso ou no Período Livre), só para Controlados e Importantes.
  - **Efeito:** Livro **+3** e Livro Antigo **+6** na habilidade do assunto, uma vez por NPC. O livro não leva a habilidade além de **60** (Livro) ou **80** (Livro Antigo).
  - O livro **não se gasta**: pode ser emprestado aos companheiros, vendido ou guardado.
  - **Universidade:** cada livro guardado nela dá +2% ao bônus de educação da cidade, até 10 livros.
  - **De onde vem:** Mercado das cidades nível 3 ou mais e masmorras; Livro Antigo só em masmorras. Produção futura com uma tecnologia de "Imprensa" (Era 2).
  - *Exemplo:* o Mercador lê "Tratado de Comércio" (Carisma) e passa de 48 para 51; empresta ao companheiro, que passa de 30 para 33.
- *Impacto:* NPC (lista de livros lidos, só para Controlados e Importantes), Itens, Trabalho (Universidade), Interface (botão "Ler").

**I2. Livro de Atributos (Rank 1 a 5).**

- *Contexto:* o nome sugere um aumento de atributo, e o Rank vai de 1 a 5.
- *Sugestão:* é o livro raro de **combate**, e ele **se gasta** ao ser lido.
  - **Assunto:** uma habilidade de combate (Corpo a Corpo, Longo Alcance, Defesa, Força, Agilidade, Destreza; Magia só para Magos).
  - **Efeito:** **+2 × Rank** na habilidade (Rank 5 = +10), sem teto além de 100. A leitura leva 8 horas.
  - **De onde vem:** só de masmorras e de recompensas de missão; Rank 4 e 5 só nas masmorras de dificuldade 4 e nas lendárias. Não é vendido no Mercado comum.
  - **Preço:** **[Proposta]** cerca de 300 × Rank.
- *Por quê:* combina com a evolução por uso do Combate. O livro é um atalho raro, não a regra.

**I3. Joia Mágica e Anel Mágico.**

- *Contexto:* a Joia Mágica (Pedra Mágica, Rank 1 a 5) é um Artefato; o Anel Mágico é um Equipamento com "Buff de habilidade".
- *Opções:* (a) **Joia carregada no inventário que reforça a Mana do Mago**, e o Anel equipado que dá bônus de habilidade; (b) um novo slot "Amuleto" para a Joia: mais um encaixe de equipamento e mais arte; (c) Joia só como objeto de valor.
- *Sugestão:* (a).
  - **Joia Mágica:** no inventário de um Mago, dá **+10 × Rank de Mana máxima** e **+5% × Rank de regeneração**. Só a melhor joia carregada conta. Para quem não é Mago, é só objeto de valor e de consumo da Realeza.
  - **Anel Mágico:** equipado no slot Anel; **+3% × Rank** numa habilidade, escolhida na produção. Também pode receber encantamento a partir do Rank 3, como os outros equipamentos.
  - *Exemplo:* um Mago de Suporte com Joia Rank 3 tem 30 de Mana a mais e regenera 15% mais rápido.
- *Impacto:* Magia (Mana), Itens, Combate. Não precisa de arte nova (a joia não aparece no boneco).

**I4. Artefato e o consumo de Artefatos da Realeza.**

- *Contexto:* "Artefato" (Rank 1 a 5) não tem efeito. A Realeza "consome" Artefatos, mas gastar um livro ou um artefato por estação não faz sentido.
- *Pergunta:* o que é um Artefato, e como a Realeza o "consome"?
- *Sugestão:*
  - **Dois tipos de Artefato:**
    - **Relíquias** (Rank 1 a 3): genéricas e sem efeito em combate. Valem muito, servem ao consumo da Realeza e, doadas a um Templo ou Palácio, dão +2 de Renome.
    - **Artefatos lendários** (Rank 4 e 5): **únicos**, com nome e efeito próprios, escritos à mão (cerca de 6 na primeira versão, 1 a 2 por masmorra lendária). *Exemplos:* "Estandarte do Primeiro Rei" (+10 de Moral no exército do portador quando ele é General); "Elmo do Juramento" (+15 de Coragem, imune ao Medo).
  - **Consumo da Realeza como posse:** a cidade precisa ter **2 itens de Artefato** (Livro, Joia ou Relíquia) **por NPC real de Realeza**, guardados no Palácio. Eles **não somem**: 1 em cada 10 sai por estação (presentes, roubos), e isso gera a demanda.
- *Conflito:* a Classe Social trata todo consumo como gasto do estoque. A Realeza passa a ter um consumo de **posse com desgaste**. Saída: só os Artefatos funcionam assim, e o resto continua igual.
- *Impacto:* Classe Social, Itens, Objetivo (Loot das lendárias), Mundo (as 3 masmorras lendárias).

**I5. Medicamentos e doença.**

- *Contexto:* o tipo existe, sem itens; o Boticário produz; a doença em LOD 0 está em aberto.
- *Sugestão:*
  - **Curativo** (Tier 1; Tecido + Água; Boticário nível 1): por 8 horas, a regeneração fica **× 2**; estanca o **sangramento** do Inconsciente em combate letal.
  - **Remédio** (Tier 3; Flor Espiritual + Fardo de Especiaria; Boticário nível 3): por 24 horas, a regeneração fica **× 3**; **cura o estado Doente**.
  - **Doença:** nos **NPCs comuns**, é só um modificador da cidade (o evento de Saúde). Nos **Controlados e Importantes**, existe o estado **Doente**: −20% nas habilidades e sem regeneração, até tomar Remédio, ir ao Hospital ou passar 8 dias. Isso fecha a pergunta em aberto da Cura.
  - **Na cidade:** durante uma epidemia, a cidade gasta **1 Remédio por 10 NPCs reais por dia**. Com estoque, a mortalidade da doença cai pela metade. Isso dá demanda real ao Boticário.
  - *Exemplo:* uma peste na Cidade Autoritária (100 NPCs) gasta 10 Remédios por dia; com 80 no estoque, ela aguenta 8 dias protegida, e o preço do Remédio sobe pela simulação.
- *Impacto:* Cura, Eventos (Saúde), Produção (Boticário), Comércio.

**I6. Comida, bebida e narcóticos no grupo.**

- *Contexto:* o grupo come 1 refeição por dia; a Felicidade dos Controlados tem o fator "comida e descanso"; os narcóticos são Luxo.
- *Opções:* (a) **qualidade da comida e efeitos curtos, com vício simples** só para Controlados e Importantes; (b) o mesmo, sem vício; (c) toda comida é igual.
- *Sugestão:* (a).

| Item | Efeito no Controlado (no dia) |
|---|---|
| Alimento Básico (Pão, Sopa, Legumes Cozidos, Ração) | Fator comida 0 |
| Alimento Refinado (Torta, Linguiça, Queijo) | +3 |
| Alimento de Luxo (Licor, Carne Envelhecida, Doce) | +6 |
| Cerveja | +2 de Felicidade |
| Fumo | +3 de Felicidade por 1 dia |
| Pó Espiritual | +6 de Felicidade; Mago: +25% de regeneração de Mana por 1 dia |

  - **Vício:** cada uso tem chance de criar o traço **Viciado** (5% no Fumo, 10% no Pó Espiritual). Viciado sem a droga por 2 dias: Felicidade −10. Some depois de 1 estação sem uso.
  - Nas cidades, nada muda: os narcóticos continuam sendo consumo de Luxo agregado, sem vício.
  - O grupo come a melhor comida do inventário, a menos que o jogador mande "economizar".
- *Impacto:* Felicidade (Controlados), Viagem (comida do grupo), Magia (Pó Espiritual), Crime (narcótico proibido na Religiosa).

### E. Escravidão

**E1. De onde vêm os escravos.**

- *Contexto:* já existem prisioneiros de guerra, trabalho forçado e o estilo Escravo; falta a lista completa das fontes.
- *Sugestão:* 5 fontes, todas só onde a lei de **Escravidão** permite:

| Fonte | Como |
|---|---|
| Prisioneiros de guerra | Já decidido: 100 prisioneiros = 1 NPC Escravo |
| Pena de trabalho forçado | Já decidido: Escravo com prazo |
| Captura por bandidos e caçadores | Viajantes e vítimas capturados em encontros são vendidos no porto (6,6) ou na Vila Livre do Sul (5,6) |
| Nascimento | Nas facções que permitem, filhos de Escravos nascem Escravos (pela natalidade da classe) |
| Compra | No Mercado de Escravos (ver E2) |

  - **Venda de prisioneiros:** uma facção que **proíbe** a escravidão pode **vender** prisioneiros a uma que permite, se houver contato e relação 0 ou mais. É o caso já previsto da Guerreira, que vende à Escravista. Ela recebe o preço de mercado e não guarda escravos.
  - **Dívida** (vender-se por dívida) fica para o futuro.
- *Impacto:* Guerra, Crime, População, Viagem (encontros), Diplomacia.

**E2. Mercado de Escravos e preço.**

- *Contexto:* a Revisão pede compra e venda; a Escravista é mercantil; o porto (6,6) é de escravos.
- *Pergunta:* onde se compra, quanto custa e o que exatamente se compra?
- *Sugestão:*
  - **Mercado de Escravos:** construção nova (Facção ou Rico), só em facção que permite a escravidão. No primeiro dia, a Escravista tem um na Capital (8,6), na Cidade (7,5) e no porto (6,6).
  - **O que se compra:** **1 NPC real**, que trabalha como **1 trabalhador** numa vaga (a mesma escala de qualquer NPC real que ocupa uma vaga).
  - **Preço base:** **200 + 4 × a melhor habilidade de Trabalho** (Braçal, Manufaturado ou Especialista).
  - **Modificadores:** Mago × 2; velho × 0,5; cultura hostil à do comprador × 0,8. Depois disso, oferta e procura no Mercado Local, como qualquer item.
  - *Exemplo:* um escravo com Trabalho Braçal 80 custa 520; o mesmo escravo velho, 260.
  - **Quem compra:** o jogador (o escravo vira Escravo do jogador, Semi-Controlado, com Vínculo = jogador), os Ricos (para as empresas) e a cidade (para vagas Braçais sem trabalhador). Em LOD 2, a compra da cidade é um fluxo por estação.
  - **Vender:** o dono pode vender o próprio escravo no Mercado de Escravos por 70% do preço.
- *Conflito:* o NPC real representa um Fator de Escala de pessoas nas contas da cidade, mas trabalha como um trabalhador numa vaga. Saída: o preço é **por NPC real como trabalhador**; o Fator de Escala só pesa na população total, como já acontece.
- *Impacto:* Construção (nova), Comércio e Econômico, Trabalho, NPC, Mundo (Mercados iniciais).

**E3. Obediência e tratamento.**

- *Contexto:* o atributo Obediência existe, sem fórmula; a IA diz que o Medo previne revoltas.
- *Sugestão:*
  - **Obediência** (0 a 100) = 50 + Medo da cidade (0 a 30) + Grilhões (+15) + tempo de cativeiro (+1 por estação, até +10) − (30 − Felicidade, se a Felicidade estiver abaixo de 30) − 10 se o escravo é Guerreiro (orgulho).
  - **Efeito:** a chance de cumprir a prioridade a cada tarefa é igual à Obediência. Abaixo de 30, ele pode fugir (E4).
  - **Escolhas do dono** (o jogador, para os escravos dele; a cidade, pela lei):

| Tratamento | Efeito |
|---|---|
| Comida Básica (padrão) | — |
| Comida Refinada | Felicidade +5; custa mais |
| Grilhões | Obediência +15; Felicidade −5; velocidade −20%; não fogem |
| Castigo público | Medo +5 na cidade por 8 dias; Felicidade dos Escravos −10; Reputação −2 com a Religiosa |

  - *Exemplo:* um escravo novo, sem Grilhões, numa cidade com Medo 10 e Felicidade 25: 50 + 10 − 5 = 55 de Obediência; com Grilhões, 70.
- *Impacto:* NPC, IA (Medo), Felicidade, Itens (Grilhões).

**E4. Fuga e rebelião.**

- *Contexto:* pergunta em aberto da Felicidade (como a fuga e a rebelião local funcionam em LOD 0 e quanto duram).
- *Sugestão:*
  - **Fuga em LOD 0:** à noite, o escravo com Obediência abaixo de 30 tem chance diária de fugir (30 − Obediência)%. A fuga usa a detecção do crime (Percepção dos guardas × Destreza dele). Se escapa, vira **fugitivo**: crime "Fuga" (gravidade 3) e Recompensa na facção. Ele vai para uma Vila Livre ou um mapa sem dono; metade vira bandido.
  - **Fuga em LOD 2:** a cada estação, foge (25 − Felicidade dos Escravos)% dos escravos da cidade, se a Felicidade estiver abaixo de 25. *Exemplo:* Felicidade 15 = 10% fogem.
  - **Rebelião local:** Felicidade dos Escravos abaixo de 10 **e** Controle abaixo de 10 por 4 dias seguidos. Em LOD 0, os escravos se armam com o que acham: 1 Esquadrão de Milícia rebelde para cada 10 NPCs escravos, contra os guardas e a guarnição. Se vencem, fogem todos, a produção Braçal da cidade cai e a cidade perde 5 de Estabilidade por 1 estação; se perdem, 20% são executados ou vendidos. Em LOD 2, é resolvida pelo cálculo rápido de forças.
  - **Caçadores de escravos:** uma fuga grande cria contratos de recaptura no quadro da Taverna.
  - **Grande revolta** (várias cidades ao mesmo tempo, como a de Espártaco) fica para o futuro.
- *Impacto:* Felicidade (fecha a pergunta 3), Crime, Guerra (cálculo rápido), Objetivo (contratos), Eventos.

**E5. Libertação e abolição.**

- *Contexto:* o Escravo libertado vira Civil e Plebeu; libertar dá −5 de reputação nas facções escravistas; a Escravista tem o pensamento extremo "a liberdade de escravos é crime".
- *Sugestão:*
  - **O dono liberta** (botão "Libertar"): o NPC vira Civil e Plebeu. **[Proposta]** +2 de reputação com a Religiosa e −5 com as escravistas (já decidido). O libertado ganha +20 de Opinião sobre o jogador.
  - **Alforria:** nas facções com Tolerância (ver Religião), 1% dos escravos da cidade compram a liberdade por estação; na Escravista, 0%.
  - **Abolição:** a facção muda a lei de Escravidão para "proibida" na assembleia. Todos os Escravos viram **Civis em 1 estação**, as Senzalas viram Casas do mesmo nível, os Ricos perdem 15 de Felicidade por 1 ano (perdem a mão de obra) e a produção Braçal cai até os Civis ocuparem as vagas com salário. Na Escravista, isso viola o pensamento extremo.
  - **Conquista:** uma cidade que passa a uma facção que proíbe a escravidão liberta os escravos dela no tratado (+10 de Felicidade dos libertos).
  - **Estilo Escravo**, 3 caminhos para a liberdade:
    - **comprar** (1,5 × o preço de mercado do próprio jogador; ele pode juntar dinheiro com tarefas extras);
    - **fugir** (vira fugitivo com Recompensa na facção do dono, sem Game Over);
    - **servir** (o dono liberta depois de 4 estações com Opinião +40).
- *Impacto:* Facção (lei), Classe Social, População (Senzala), Felicidade, Início de Jogo, Reputação.

**E6. Tráfico, crime e diplomacia.**

- *Contexto:* a Religiosa condena o tráfico (afinidade hostil com a Escravista); libertar escravo é crime na Escravista.
- *Sugestão:*
  - **Tráfico onde é proibido:** é **Contrabando** (gravidade 3). Escravos só são vendidos em lojas ilegais (mercado negro), com o preço × 1,5.
  - **Libertar escravo alheio:** crime "Fuga ou ajuda a escravo" (gravidade 3); na Escravista, gravidade 5 (crime cultural).
  - **Reputação do jogador:** cada venda de escravo dá −3 com a Religiosa, e cada compra, −1. Libertar dá +2 (ver E5).
  - **Diplomacia:** facções que proíbem a escravidão têm −10 de relação com as que mantêm Mercado de Escravos. **[Proposta]** Só a Religiosa aplica esse −10 na primeira versão, por já ser hostil à Escravista.
  - **Missões:** da Religiosa, libertar caravanas de escravos; da Escravista, recapturar fugitivos e escoltar carregamentos.
- *Impacto:* Crime, Reputação, Diplomacia, Objetivo, Comércio.

### D. Escopo e depois do P1

**D1. Documentos.**

- *Sugestão:*
  - **Dois documentos novos:**
    - `Sistema de Escravidão/Descrição.md`, que junta o que está espalhado;
    - `Sistema de Itens/Efeitos de Itens.md`, uma tabela mestre como a de Funções e Construções.
  - **Atualizar:** Itens, Cura (estado Doente), Magia (Joia, Pó Espiritual), Classe Social (posse de Artefatos, escravos), NPC (Obediência, livros lidos, Viciado), Crime, Facção (lei de Escravidão), Trabalho, População (natalidade e Senzala), Felicidade (fuga e rebelião), Construção (Mercado de Escravos), Comércio, Reputação, Início de Jogo (estilo Escravo), Guerra e Batalha (venda de prisioneiros), Diplomacia, Objetivo (Artefatos lendários e contratos), Mundo (Mercados de Escravos iniciais), Interface (botões "Usar" e "Ler"), Glossário.
  - **Dados:** `efeitos_itens.json` e `escravidao.json`.
  - **Fatia vertical:** só a qualidade da comida na Felicidade (I6); o resto fica para depois.

**D2. O que vem depois do P1?**

- *Contexto:* com a P1-6, todos os sistemas P1 estarão escritos. A ordem decidida é: documentação, depois **ficha técnica lógica**, depois programação. Há uma lista grande de itens "Em aberto" no [[Estado do Projeto]].
- *Opções:*
  - (a) **1 ou 2 rodadas de pendências**: só os "Em aberto" que travam a ficha técnica (Fator de Escala, Espada no Ferreiro, peça de armadura Média, consumo Luxo e Exclusivo por Tier, custos das construções novas), depois a ficha técnica da **fatia vertical**;
  - (b) ir direto para a ficha técnica e resolver as pendências quando aparecerem;
  - (c) escrever primeiro os sistemas P2 (áudio, tutorial, configurações...).
- *Sugestão:* (a). A ficha técnica começa pela **fatia vertical** (1 cidade, Fazenda → Cozinha → Mercado), que é o primeiro código. Os sistemas P2 ficam para perto do lançamento, como a Revisão já diz.

Depois das respostas: escrever os dois documentos, atualizar os vizinhos e seguir pela resposta da D2.

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). O Ferreiro não lista Espada (provável erro; ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]).
- **Arte:** o autor desenha por enquanto (sem experiência em pixel art), no **Pixelorama** (grátis), com pacotes de terceiros provisórios na fatia vertical. Mapas no **Tiled** (explicar em linguagem simples). Sem verba para programas pagos. Projeto **sem prazo**: o limite da arte é o peso no jogo, não o tempo (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
- **Estações na arte:** só mudança de cor por enquanto; **toda arte extra de estação fica para o futuro** (neve, rios congelados, folhas, flores).
- **Viagem:** 4 h por Mapa Local a pé, multiplicada por terreno, infraestrutura, tamanho do grupo, montaria e carga; caravanas sempre reais no Mapa Mundial (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).
- **Grupo:** controlado = 4 + Renome ÷ 20; vinculados = 10 + Renome ÷ 5 + 3 por cidade; **sem retratos** nas conversas (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).
- **Linguagem:** o foco é **C++**; ferramentas em outras linguagens só como ajuda, nunca essenciais.
- **Guerra e Batalha:** Placar de Guerra (−100 a +100) paga a paz; cidade ocupada só muda de dono no tratado; exército de até 20 Esquadrões com General; **o mundo para durante a batalha**; a simulação é sempre por Esquadrão de 100 soldados, e o jogador escolhe o **tamanho visual** (20, 50 ou 100 bonecos por Esquadrão; até 4.000 soldados na tela, pedido do autor). A 7ª camada do Tiled é a Elevação. Na programação, a guerra por cálculo rápido vem antes do Mapa de Guerra (ver [[Sistema de Guerra/Descrição|Guerra]], [[Sistema de Batalha/Descrição|Batalha]]).
- **Diplomacia:** estado (Paz, Guerra, Trégua) + tratados que se somam + atitude (faixa da relação); Confiabilidade; vassalo tem leis próprias (ver [[Sistema de Diplomacia/Descrição|Diplomacia]]).
- **Religião:** uma fé por cultura, com Devoção (Alta na Religiosa, nenhuma na Tecnocrata); cada Sacerdote atende 20 NPCs; fator Religião de −5 a +10; 2 festivais por ano (4 na Religiosa); lei de Tolerância religiosa; heresia é um ato; assimilação de 1% a 4% por estação; Bênção e Anátema do Sumo Sacerdote (ver [[Sistema de Religião/Descrição|Religião e Fé]]).
- **Clima:** Fazenda 75%, 100%, 150% e 25% nas quatro estações (base temperada); clima do dia sorteado por região, com 60% de repetir; lenha no inverno (1 Madeira Bruta por NPC real a cada 4 dias, × bioma); vontade de guerra −15 no inverno; noite com visão −30%; arte só com paleta, tom e partículas (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Notícias:** 3 h por mapa (o mundo em 1 dia); Local, Regional e Mundial; precisão cai com a distância; a IA sabe tudo na hora (simplificação) (ver [[Sistema de Notícias/Descrição|Notícias]]).
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
