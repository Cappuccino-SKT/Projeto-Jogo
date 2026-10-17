> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a rodada IP3 (Insumos de cada produto) aplicada e a V2 (Base numérica da economia) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas P0 e P1 estão escritos, e a **revisão completa** está em andamento (levantamento e plano na [[Revisão Completa]]). A V1, a IP1 e a IP2 já estão no `main` (PRs #9 a #11). Em 2026-10-17 foi aplicada a **IP3** (receitas de todos os produtos, em [[Sistema de Produção/Receitas|Receitas]], e a nova [[Sistema de Produção/Funções e Construções|Funções e Construções]]), na branch `docs/ip3-receitas`. A **rodada V2 (Base numérica da economia) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se o pull request da `docs/ip3-receitas` foi mesclado no `main`.**
3. **Próximo passo:** pedir o `Respostas.txt` da V2 (conferir se é desta rodada), pôr os números no Econômico, Trabalho, Classe Social, Construção, Recursos, IA, Tempo e Arquitetura, e abrir a **V3** (pendências de regra e listas de conteúdo).
4. Depois: V4 e V5 (propostas), como no plano da [[Revisão Completa]], e então a **ficha técnica lógica da fatia vertical**.
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
| Revisão | Itens e Produção: IP3 (insumos) | Aplicada |
| Revisão | **V2 (economia numérica)** | **Enviada, aguardando respostas** |
| Revisão | V3 (pendências), V4 e V5 (propostas) | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, modo observador, eventos de dinastia. As ordens de produção em lote foram decididas na IP1-7 ([[Sistema de Produção/Descrição|Produção]]). A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: V2, Base numérica da economia

**Perguntas enviadas em 2026-10-17, aguardando respostas.** Com a lista de itens (IP2) e as receitas (IP3) prontas, esta rodada dá os **números** que faltam para a ficha técnica da fatia vertical. Ela resolve as partes abstratas A-1 e A-3 a A-9 da [[Revisão Completa]].

Responder no `Respostas.txt` pelos códigos (V2-1, V2-2...). "Seguir sugestão" aceita a proposta. Todos os números são **valores de partida**: o modo sem tela da fatia vertical vai testá-los.

**Já decidido (base das perguntas):**

- **Escala (IP1-5):** 1 NPC real = 1 trabalhador e 1 consumidor.
- **Preço base (IP1-9):** ≈ custo dos insumos + horas de trabalho × salário.
- **Receitas:** estão em [[Sistema de Produção/Receitas|Receitas]].
- **Tempo:** 1 hora de jogo = 40 s reais a 1x; estação = 16 dias.
- **Banco inicial das cidades** (Mundo): 20.000 no nível 4, 6.000 no 3, 1.500 no 2 e 400 no 1, multiplicado pela Riqueza da cultura.
- **Escravo** (Escravidão): 200 + 4 × a melhor habilidade de Trabalho. **Contratar um companheiro comum:** 100 moedas.
- **Vagas por nível:** 2, 3, 5, 7 e 10. Os salários são pagos por dia (LOD 0 e 1) ou por estação (LOD 2), pelo dono da construção.

**V2-1. A moeda.**

- *Contexto:* os documentos já falam em "moedas", mas sem regra de unidade nem de casas decimais.
- *Sugestão:*
  - uma **unidade só, a moeda**, com **1 casa decimal** na interface (o Pão custa 1,7);
  - por dentro, o jogo guarda o dinheiro em **décimos inteiros** (1,7 vira 17). Isso evita os erros de arredondamento dos números com vírgula, que fariam o dinheiro "vazar" ao longo de uma partida longa;
  - o nome próprio da moeda de cada cultura fica para a história.
- *Impacto:* Econômico, Interface, Arquitetura (tipo de dado do dinheiro).

**V2-2. A rotina de cada classe (A-7).**

- *Contexto:* a [[Sistema de IA/Descrição|IA]] divide o dia em dormir, trabalhar e Período Livre, mas não diz as horas. Elas também definem quanto cada NPC produz (IP1-6).
- *Sugestão:*

| Classe | Dormir | Trabalhar | Período Livre |
|---|---|---|---|
| Escravo | 22h às 5h (7 h) | 5h às 17h (12 h) | 17h às 22h (5 h) |
| Civil | 22h às 6h (8 h) | 7h às 15h (8 h) | 6h às 7h e 15h às 22h (8 h) |
| Rico | 23h às 7h (8 h) | 9h às 15h (6 h, gerência) | 10 h |
| Nobreza e Realeza | 0h às 8h (8 h) | 10h às 14h (4 h, política e corte) | 12 h |
| Guardas e Funções contínuas | Turnos de 8 h (6h às 14h, 14h às 22h, 22h às 6h); cada guarda faz 1 turno | | |

  - Um turno de 8 h dura **5 min e 20 s reais** a 1x.
  - A madrugada (22h às 5h), com só o turno da noite acordado, é a hora do crime, como já decidido.
- *Impacto:* IA, Trabalho, Produção (horas do turno), Crime.

**V2-3. Salários e soldo (A-6).**

- *Contexto:* o Trabalho decidiu uma "tabela por Função e nível", multiplicada pela riqueza e pelo desemprego, com três níveis de salário no jogador. Faltam os números.
- *Sugestão:* **salário por hora pela habilidade que a receita usa** (IP3-11):

| Trabalho | Moedas por hora | Por dia (turno de 8 h) |
|---|---|---|
| Braçal | 0,75 | 6 |
| Manufaturado | 1,0 | 8 |
| Especialista | 1,5 | 12 |
| Mago de Ofício | 2,5 | 20 |

  - **Multiplicadores:**
    - +10% por nível da construção acima do 1;
    - gerente × 1,5;
    - riqueza da cidade de × 0,8 a × 1,2;
    - desemprego de × 0,9 (muito) a × 1,1 (falta gente);
    - jogador: Baixo × 0,75, Normal × 1, Alto × 1,25.
  - **Guarda:** 1,0 por hora, pago pelo Banco da Cidade.
  - **Soldo do Esquadrão:** 8 moedas por dia, o salário de 1 trabalhador, porque 1 Esquadrão = 1 NPC alistado. O Esquadrão de Magos custa 3 vezes (24) e o mercenário 2 vezes por estação, adiantado (256), como já decidido.
- *Exemplo:* um Cozinheiro Civil num Cozinheiro nível 1 ganha 8 por dia, 128 por estação. Um escravo de habilidade 60 custa 440, cerca de 55 dias desse salário.
- *Impacto:* Trabalho, Facção (Banco da Facção paga o soldo), Guerra, Escravidão.

**V2-4. Preço base de cada item (A-1).**

- *Contexto:* o Mercado Mundial dá o valor primário de cada item, mas não há nenhum número.
- *Sugestão:* uma **fórmula** aplicada a todas as receitas:

> **preço base = (custo dos insumos + horas × salário por hora da receita) ÷ lote × 1,2**

  - O 1,2 é a **margem do dono** (20%).
  - O jogo calcula todos os preços ao carregar os dados, na ordem da cadeia: primeiro os brutos, depois o que usa os brutos, e assim por diante. Assim, mudar uma receita muda o preço sozinho.
  - **Rank:** o preço do Rank 1 é multiplicado por 1 (Rank 1), 1,5 (2), 2,2 (3), 3,2 (4) e 4,5 (5). O Rank 0 vale metade.
  - **Ajuste manual:** um campo opcional "multiplicador" por item, para casos como o Cavalo (raro e caro) ou os Artefatos.
  - *Resultado* (preços de partida, em moedas):

| Grupo | Item | Preço |
|---|---|---|
| Bruto Tier 0 | Grão, Legumes, Água, Pedra, Argila, Carvão, Tora, Minérios de Ferro e Cobre, Algodão, Folha de Fumo, Ovo, Leite | 0,5 |
| Bruto Tier 1 e 2 | Fruta, Flor Espiritual, Pedra de Mana, Minério de Ouro / Especiarias | 0,9 / 1,8 |
| Açougue | Carne Crua, Couro Cru, Ossos | 1,7 |
| Animais | Galinha / Porco e Ovelha / Vaca / Cavalo (com multiplicador × 3) | 3,6 / 7,2 / 14,4 / 65 |
| Intermediários | Tábua 1,5; Bloco de Pedra 1,8; Barra de Ferro 2,1; Barra de Cobre 2,0; Couro Refinado 3,5; Pólvora 3,6; Barra de Aço 5,5; Barra de Ouro 11,0; Barra de Aço Mágico 22,1 | — |
| Comida Tier 1 | Pão, Sopa 1,7; Peixe Defumado 1,8; Ração Militar 2,0 | — |
| Comida e bebida Tier 2 | Queijo, Cerveja 3,3; Torta de Fruta 3,5; Linguiça 5,5 | — |
| Tier 3 | Vinho, Pó Espiritual, Remédio 10,4; Perfume 11,0; Roupa Fina 13,7; Assado Temperado 14,0; Móveis Refinados 15,4 | — |
| Tier 4 | Doce de Especiarias 19,3; Presunto Curado 22,7; Licor 29,0; Ornamento 32,4; Joias 40,8 | — |
| Materiais básicos | Cerâmica 1,8; Roupa Comum 2,3; Móveis de Madeira 5,1 | — |
| Armas (Rank 1) | Flecha 0,5; Cartucho 1,0; Espada Curta de Ferro 13,9; Arco Longo de Madeira 15,5; Espada Longa de Ferro 16,4; Arcabuz 23,6; Espada Curta de Aço 30; de Aço Mágico 71,5 | — |
| Armaduras (Rank 1, 3 peças) | Leve de Couro 54; Pesada de Ferro 57; Pesada de Aço 126 | — |
| Artilharia | Bala de Canhão 25; Bombarda 67 | — |

- *Exemplo:* um kit de Infantaria Pesada de Aço de Rank 3 custa cerca de 200 × 2,2 = 440 moedas, o mesmo que um escravo bom ou 55 dias de soldo.
- *Impacto:* Itens (Valor Global), Econômico, Comércio, Batalha (custo dos kits).

**V2-5. Mercado da Facção e Mercado Local (oferta e demanda).**

- *Contexto:* o Econômico tem as 3 camadas, mas não a fórmula (pergunta aberta 3).
- *Sugestão:*
  - **Mercado da Facção:** preço mundial × ajuste da lei por categoria (−20%, 0 ou +20%). Os itens proibidos só aparecem nas lojas ilegais, a × 2.
  - **Mercado Local:**
    - **estoque-alvo** = 5 dias do consumo da cidade, somando as classes e as receitas que usam o item;
    - **fator** = 1 + (alvo − estoque) ÷ alvo, limitado entre 0,5 e 2,0. Estoque zero dobra o preço; estoque de 1,5 vez o alvo corta pela metade;
    - o preço anda no máximo **10% por dia** na direção do fator, para não pular.
  - **Lojas da Cidade:** compram a 90% e vendem a 110% do preço local.
  - O preço do jogador continua na faixa de ±30%, como já decidido.
- *Exemplo:* uma cidade de 100 NPCs come 100 Pães por dia; o alvo é 500. Com 250 no estoque, o fator é 1,5: o Pão vai de 1,7 a 2,6 em cerca de 5 dias. Uma caravana que traga 400 Pães faz o preço cair.
- *Impacto:* Econômico, Comércio (o lucro das rotas sai dessa diferença), Notícias (preço 30% fora da média vira notícia).

**V2-6. O consumo de cada classe (A-3).**

- *Contexto:*
  - a Classe Social decidiu os tipos de consumo e o mapeamento pelo Tier (IP2-11);
  - faltam as quantidades e a composição das cidades por classe;
  - o salário de um Civil (8 por dia) é bem maior que a comida mínima (1,7). Se ele gastar só o mínimo, o dinheiro se acumula, e as lojas ficam sem clientes.
- *Sugestão:*
  - **Cesta mínima por NPC real** (é ela que, se faltar, conta os dias em déficit):

| Classe | Comida por dia | Materiais Básicos | Luxo | Exclusivo |
|---|---|---|---|---|
| Escravo | 1 (Tier 1) | — | — | — |
| Civil | 1 (Tier 1 ou 2) | 1 a cada 8 dias | — | — |
| Rico | 1 (Tier 2 ou mais) | 1 a cada 4 dias | 1 a cada 4 dias | — |
| Nobreza | 1 (Tier 2 ou mais) | 1 a cada 4 dias | 1 a cada 2 dias | 1 a cada 4 dias |
| Realeza | 1 (Tier 2 ou mais) | 1 a cada 2 dias | 1 por dia | 1 a cada 2 dias |

  - Mais a **Tora no inverno** (já decidido) e os **Artefatos como posse** da Realeza.
  - **Regra do gasto:** cada classe gasta por dia **até 80% da renda**. Primeiro a cesta mínima; o que sobra sobe o Tier da comida, compra mais materiais e paga a **Bebida na Taverna** (até 25% da Comida pode ser Bebida). Os outros 20% são imposto e poupança, que vira a riqueza média. Assim o dinheiro volta às lojas, e uma cidade rica come melhor sozinha.
  - **Bônus de Felicidade:** consumo acima da cesta dá até +5 no fator de consumo da classe.
  - **Composição por classe** das cidades:

| Classe | Parcela da cidade |
|---|---|
| Escravo | 10% (30% na Escravista; zero onde a lei proíbe, e esses NPCs viram Civis) |
| Civil | 76% (56% na Escravista) |
| Rico | 10% |
| Nobreza | 4% |
| Realeza | Só na capital: 1% (3 NPCs de 300) |

- *Exemplo:* um Civil ganha 8 por dia e gasta até 6,4: o Pão (1,7), uma Cerveja (3,3) e um pouco de Materiais; os outros 1,6 vão para o imposto e a poupança.
- *Impacto:* Classe Social, Felicidade, Econômico (fecha o ciclo do dinheiro), População (composição inicial), Mundo.

**V2-7. Bancos, estabilizador, imposto, cunhagem e empresas.**

- *Contexto:* o Econômico e o Trabalho deixaram em aberto o limiar do estabilizador, a taxa do imposto, a cunhagem e a fração de empresas dos Ricos.
- *Sugestão:*
  - **Estabilizador:** quando a riqueza média de uma classe cai abaixo de **3 dias da cesta mínima**, o Banco da Cidade paga até ela voltar a 5 dias. O teto é 10% do Banco por estação.
  - **Lei de Imposto** (sobre salários e lucros, por estação):

| Nível | Taxa | Felicidade |
|---|---|---|
| Baixo | 10% | 0 |
| Normal | 20% | −5 |
| Alto | 30% | −10 |

    A cidade fica com 30% do imposto, e o Banco da Facção com 70%.
  - **Cunhagem:** a Casa da Moeda transforma **1 Barra de Ouro em 25 moedas** (a barra vale 11). Ela cunha até 4 × o nível da Casa barras por estação: o ouro vira dinheiro novo, a fonte contra a economia fixa.
  - **Empresas dos Ricos:** 30% das construções de produção de cada cidade são de Ricos. As vagas de Rico = número de empresas.
- *Exemplo:*
  - uma Casa da Moeda nível 2 cunha 8 barras por estação = 200 moedas novas, com lucro de 112;
  - a cidade de nível 3 com Banco de 6.000 sustenta o estabilizador por várias estações de crise.
- *Impacto:* Econômico, Trabalho, Facção (lei de Imposto), Felicidade, Construção (Casa da Moeda).

**V2-8. Construções: custo, tempo, manutenção e moradias (A-4).**

- *Contexto:* a Construção decidiu o desconto de nível a nível, a manutenção por estação com um Construtor, e o Engenheiro (IP3-11). Faltam os números de todas as construções.
- *Sugestão:* **três portes**, para não escrever cada construção à mão:

| Porte | Exemplos | Nível 1 | Horas de obra |
|---|---|---|---|
| Pequeno | Casa, Senzala, Fazenda, Pasto, Rancho, Posto de Coleta, Pesqueiro, Posto da Guarda | 10 Tábua + 10 Bloco de Pedra | 40 |
| Médio | As oficinas (Cozinheiro, Carpinteiro, Ferreiro...), Mercado, Taverna, Casarão, Boticário, Posto Comercial | 20 Tábua + 30 Bloco de Pedra | 120 |
| Grande | Templo, Quartel, Universidade, Laboratório, Torre e Forja Arcana, Hospital, Casa da Moeda, Prisão, Mansão, Palácio, Mercado de Escravos | 40 Tábua + 80 Bloco de Pedra + 10 Barra de Ferro | 400 |

  - **Subir para o nível N** custa o nível 1 × N. Construir direto no nível N custa a soma de todos os níveis × 1,25: é o "custo integral" sem o desconto, já decidido.
  - **Tempo:** horas de obra ÷ (Operários × velocidade). O Construtor tem as vagas da tabela geral.
  - **Engenheiro:** Inteligência mínima de 30, 45, 60 e 75 nos níveis 2 a 5 (+10 nas construções grandes). Sem Engenheiro, só nível 1.
  - **Manutenção por estação:** 5% dos materiais do nível atual (no mínimo 1 Tábua) + 5% das horas de obra, feitas pelos Operários.
  - **Moradias** (NPCs reais por construção):

| Moradia | Nível 1 | Por nível a mais |
|---|---|---|
| Senzala | 8 | +4 |
| Casa | 4 | +2 |
| Casarão | 2 | +1 |
| Mansão | 2 | +1 |
| Palácio | 3 | +1 |

- *Exemplo:*
  - **Custo:** um Cozinheiro nível 1 custa 20 Tábuas (30 moedas) + 30 Blocos (54) e 120 h de obra (90 moedas de salário de Operário), cerca de 175 moedas;
  - **Tempo:** com 3 Operários, a obra leva 5 dias;
  - **Subir de nível:** do nível 2 para o 3 custa 60 Tábuas + 90 Blocos.
- *Impacto:* Construção, População (capacidade das moradias, pergunta aberta), Trabalho (Operários), Guerra (Muralha como construção grande).

**V2-9. Produtividade dos recursos e lotes (A-5).**

- *Contexto:* a IP3-2 decidiu 1 unidade por 0,5 h de trabalho Braçal, multiplicada pela produtividade e pela estação. Falta o valor da produtividade e um limite, para uma cidade não pôr 20 Fazendas num mapa.
- *Sugestão:*
  - **Produtividade:** recurso normal × 1,0; de destaque × 1,5; ausente, não existe.
  - **Construções Braçais por recurso, em cada Mapa Local:** até 2 num recurso normal e até 4 num de destaque. A Água não tem limite (poço).
  - **Conta de comida** de uma cidade de 100 NPCs (100 Pães por dia):

| Precisa de | Quanto | Quem faz | Trabalhadores |
|---|---|---|---|
| Grão | 50 por dia | 1 Fazendeiro (habilidade 50, 8 h) faz 16 por dia a 100% | 3 a 4 |
| Água | 25 por dia | Coletor | 1 a 2 |
| Pão | 100 por dia | 1 Cozinheiro faz 8 por dia | 12 a 13 |

    Isso dá cerca de 18% da cidade para a comida.
  - O **inverno** (Fazenda a 25%) obriga a estocar no outono, como já decidido.
- *Conflito:* o **Cozinheiro é o gargalo**: são 13 cozinheiros para 4 fazendeiros, porque a IP1-6 deu 1 h por unidade de Tier 1. Saída: manter e testar. Se incomodar, o lote do Pão sobe para 8 (2 Pães por hora).
- *Impacto:* Recursos, Mundo (lotes por mapa), População (lotes), Clima.

**V2-10. A IA de Utilidade simples (A-8).**

- *Contexto:* a IA decidiu Utilidade em LOD 0 e Regras Fixas em LOD 1; a fatia vertical precisa só de "trabalhar, comer, comprar, dormir".
- *Sugestão:*
  - **Ações:** Dormir, Trabalhar, Comer, Comprar, Lazer (Taverna) e Fugir.
  - **Nota de cada ação:** 1,0 se a ação é a do período da rotina, 0 se não, mais os bônus:
    - **Comer:** +1,5 nas horas das refeições (7h, 12h e 19h, por 30 min);
    - **Comprar:** +1,2 uma vez a cada 2 dias, no Período Livre. A compra de verdade é a da cidade; a visita mostra isso na tela;
    - **Lazer:** +0,5 × (1 − Felicidade ÷ 100);
    - **Fugir:** +3,0 com combate a menos de 10 tiles, o que passa por cima de tudo.
  - O NPC faz a ação de maior nota e reavalia a cada **5 minutos de jogo** (cerca de 3 s reais) ou quando algo muda.
  - **LOD 1 (Regras Fixas):** a ação do período, sem as notas.
- *Impacto:* IA, Arquitetura (custo da reavaliação), Combate (Fugir).

**V2-11. O tick e as metas de desempenho (A-9).**

- *Contexto:* o Tempo amarrou a frequência ao LOD, sem números; a Arquitetura tem "meta de NPCs em LOD 0 a medir".
- *Sugestão:*

| LOD | Frequência | Equivale a |
|---|---|---|
| 0 | **20 ticks por segundo real** a 1x (passo fixo) | 1 tick = 2 s de jogo |
| 1 | 1 vez a cada 10 ticks (2 por segundo) | — |
| 2 | 1 vez por hora de jogo (a cada 40 s reais a 1x) | Economia somada por dia e por estação |

  - A 4x, o LOD 0 roda 80 ticks por segundo. Por isso a velocidade 8x fica só no Mapa Mundial, como já decidido.
  - **Metas de partida**, a medir na fatia vertical:
    - 500 NPCs em LOD 0 e 5.000 em LOD 1 a 4x;
    - 60 quadros por segundo em 1600 × 900;
    - num PC de gama média.
- *Impacto:* Tempo, Arquitetura, IA, Combate.

**V2-12. A fatia vertical em números.**

- *Contexto:* a fatia vertical é 1 cidade com Fazenda → Cozinheiro → Mercado, mas sem números ninguém sabe se o ciclo fecha.
- *Sugestão:* uma **vila de nível 1, com 15 NPCs reais Civis**, Banco de 400, e 12 vagas:

| Construção | Vagas | Produz por dia |
|---|---|---|
| Fazenda nível 1 | 2 | 32 Grão (8 no inverno) |
| Posto de Coleta nível 1 | 2 | 16 Água e 16 Tora |
| Cozinheiro nível 1 | 2 | 16 Pães |
| Carpinteiro nível 1 | 2 | Tábuas e, no nível 2, Móveis de Madeira |
| Mercado | 2 Lojistas | — |
| Construtor | 2 Operários | — |
| Casas nível 1 | 4 Casas | 16 lugares |

  - **Comida:** a vila come 15 Pães por dia, que gastam 7,5 Grão e 3,75 Água. O Grão que sobra vai para o estoque do inverno e para a venda.
  - **Dinheiro por dia:**
    - entram cerca de 85 em salários (12 × 6 a 8);
    - saem cerca de 25 da cesta mínima (15 × 1,7);
    - até 68 voltam às lojas pela regra do gasto (80%).
  - **O teste passa se**, em 1 ano de jogo sem tela, a vila não passa fome, o Banco não zera e o preço do Pão fica entre 1,2 e 2,6.
- *Impacto:* Arquitetura (marcos da fatia vertical), todos os números acima. É a base da ficha técnica lógica.

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). A família de cada uma foi fixada na IP1-2 (a Espada está no Ferreiro), e as receitas estão em [[Sistema de Produção/Receitas|Receitas]] (IP3).
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
- **Receitas (IP3):** lote 4/2/1/1 pelo Tier; 8, 12 ou 24 h pelo degrau; armadura faz as 3 peças juntas; moldes no Ferreiro, Carpinteiro e Armeiro; munição em lote (20 Flechas); Pólvora = Carvão + Dejetos Animais; Aço Mágico = Barra de Aço + Pedra de Mana; **a era trava só equipamento**, os bens de luxo existem desde a Era 1; Operário e Engenheiro no Construtor; cadeia máxima de 4 passos e nenhum órfão.
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
