> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a revisão V1 aplicada e a rodada IP1 (Padronização da produção) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas P0 e P1 estão escritos, e a **revisão completa** está em andamento (levantamento e plano na [[Revisão Completa]]). Em 2026-10-17 foi aplicada a **V1** (faxina dos 13 documentos antigos, correções, animais como itens, volta da pesca, inventário único, Fome dos Controlados, vagas do Templo), na branch `docs/v1-faxina`. O autor pediu rodadas próprias de Itens e Produção, e a **rodada IP1 (Padronização da produção) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se o pull request da `docs/v1-faxina` foi mesclado no `main`.**
**Pausa em 2026-10-17:** a sessão foi compactada logo depois de enviar a IP1. Tudo está no GitHub na branch `docs/v1-faxina` (pull request para o autor aceitar). O `Respostas.txt` da Área de Trabalho já tem o modelo da IP1 com os 9 códigos; o autor ainda não respondeu.
3. **Próximo passo:** pedir o `Respostas.txt` da IP1 (conferir se é desta rodada), aplicar as regras em Produção, Itens, Construção e Tecnologia e abrir a **IP2** (nomes e lista final dos itens).
4. Depois: IP3 (insumos de cada produto), V2 (economia numérica), V3 (pendências), V4 e V5 (propostas), como no plano da [[Revisão Completa]].
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
| Revisão | **Itens e Produção: IP1 (padronização), IP2 (nomes), IP3 (insumos)** | **IP1 enviada, aguardando respostas** |
| Revisão | V2 (economia numérica), V3 (pendências), V4 e V5 (propostas) | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, ordens de produção em lote, modo observador, eventos de dinastia. A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: IP1, Padronização da produção

**Perguntas enviadas em 2026-10-17, aguardando respostas.** Na revisão V1, o autor decidiu reformular os itens e a produção em rodadas próprias (respostas V1-2, V1-7 e V1-8):

- **IP1 (esta):** a forma padrão de produzir.
- **IP2:** os nomes e a lista final dos itens, encaixados no mundo.
- **IP3:** os insumos de cada produto.

Esta rodada decide as **regras**, e não ainda os itens um por um. Responder no `Respostas.txt` pelos códigos (IP1-1, IP1-2...). "Seguir sugestão" aceita a proposta. Valores numéricos são iniciais, a testar.

**Já decidido (base das perguntas):**

- **3 categorias de produção** ligadas às habilidades: Braçal (do mapa), Manufaturada (transforma o básico) e Especialista (item de alto nível). A produção é em **cadeia**: cada produto mais complexo exige produtos anteriores ([[Sistema de Produção/Descrição|Produção]]).
- **Tier** (0 a 4) nos materiais e bens; **Rank** (0 a 5) nas armas, equipamentos e artefatos. As faixas de Rank por material já existem: Ferro 1-3, Aço 1-4, Aço Mágico 1-5 ([[Sistema de Itens/Descrição|Itens]]).
- Construções de produção com **níveis 1 a 5**: o nível libera produtos novos e aumenta a eficiência. Nas Braçais, o nível só aumenta a eficiência.
- Tempo base por receita, reduzido pela habilidade; capacidade de armazenamento somada ao **inventário único da cidade** (V1-5); sem logística de entrega.
- As **eras** da tecnologia acompanham os Tiers (a Era 1 libera o Tier 1, e assim por diante); a tabela de Rank por era ainda está em aberto ([[Sistema de Tecnologia/Descrição|Tecnologia]]).
- Nas construções que não são do jogador, o que produzir segue a **demanda da cidade**; em LOD 2, a produção é um cálculo por estação.

**IP1-1. A estrutura padrão da receita.**

- *Contexto:* hoje o canvas só diz "Cozinheiro nível 1: Pão". Não há insumos, quantidades, tempo nem habilidade.
- *Pergunta:* quais campos toda receita tem?
- *Opções:*
  - (a) **uma receita fixa por produto**, com campos padronizados;
  - (b) receitas alternativas para o mesmo produto (Pão de Grão ou de Legumes): mais variedade, mas dobra os dados e as escolhas da IA.
- *Sugestão:* (a), com estes campos:

| Campo | O que é | Exemplo (Pão) |
|---|---|---|
| Produto e quantidade | O que sai de um ciclo | 4 Pão |
| Insumos | Itens e quantidades gastos | 2 Fardo de Grão + 1 Água |
| Subprodutos | Saídas extras, se houver | — |
| Construção e nível mínimo | Onde se faz | Cozinheiro, nível 1 |
| Função e Trabalho | Quem faz e qual habilidade conta | Cozinheiro; Trabalho Manufaturado |
| Habilidade mínima | Abaixo dela, o NPC não consegue fazer | 10 |
| Horas de trabalho | Quanto trabalho um ciclo leva | 4 horas |
| Era | Tecnologia mínima | Era 1 |

  Receitas alternativas podem entrar depois, como exceção, se a IP3 pedir.
- *Impacto:* Produção, Itens, Arquitetura (formato dos dados em CSV, uma linha por receita).

**IP1-2. Uma família de produtos por construção.**

- *Contexto:* hoje há sobreposições e lacunas. A Metalurgia e a Forja Arcana fazem Aço Mágico (I-22). O Artesão faz Fumo. A Armadura Leve de metal está no Costureiro (I-21). Várias peças de roupa e comida não têm produtor (I-10).
- *Opções:*
  - (a) **família por ofício e material**, como nos ofícios reais;
  - (b) família por uso (uma construção militar, uma civil, uma de luxo): menos construções, mas cada uma faria coisas muito diferentes.
- *Sugestão:* (a):

| Construção | Família (tudo o que é dela) |
|---|---|
| Fazenda | Vegetais |
| Pasto | Animais vivos |
| Rancho | Produtos de animais vivos (ovos, lã, leite, dejetos) |
| Pesqueiro | Peixe |
| Posto de Coleta | Madeira, pedra, argila, minérios, carvão, água, pedra mágica |
| Açougue | Carne, ossos e couro bruto (gasta o animal) |
| Metalurgia | Barras de Ferro, Cobre, Aço e Ouro |
| **Forja Arcana** | **Aço Mágico** e Pedra de Mana (precisa de Mago de Ofício). Resolve a I-22 |
| Pedreira | Tijolos e peças de argila |
| Carpinteiro | Tudo de madeira: tábuas, móveis, arcos, bestas, munição de arco e besta, escudo de madeira, cajado de madeira |
| Ferreiro | Tudo de metal batido: lâminas, hastes, clavas, escudos de metal, **armaduras Leves e Pesadas**, Grilhões, peças de metal das roupas |
| Armeiro | Armas de fogo, munição de fogo e artilharia |
| Costureiro | Tecido, couro refinado, roupas, **armadura Média**, Sela |
| Artesão | Joias, anéis, ornamentos, coroas, cajados de metal, Joia Mágica |
| Cozinheiro | Comida (do Pão à Carne Envelhecida) |
| Destilaria | Bebidas (Cerveja, Licor) e essências (Perfume) |
| Misturador | Misturas secas: Fertilizante, Pó Explosivo, **Fumo** e Pó Espiritual |
| Boticário | Medicamentos |

  *Regra:* um item só pode ser feito numa construção. Assim a IA sabe onde produzir, e o jogador sabe onde procurar.
- *Impacto:* Funções e Construções (a tabela inteira é refeita na IP3), Itens.

**IP1-3. O que cada nível da construção libera.**

- *Contexto:* hoje cada construção libera produtos por nível sem regra comum (o Cozinheiro faz Ração no nível 3; o Ferreiro faz Grilhões no 2). Isso deixa o balanceamento difícil.
- *Sugestão:* uma **escada padrão** para todas as construções de transformação:

| Nível | Produtos de Tier até | Rank máximo do que sai | Era típica |
|---|---|---|---|
| 1 | 1 | 1 | Era 1 |
| 2 | 2 | 2 | Era 1 |
| 3 | 2 | 3 | Era 2 |
| 4 | 3 | 4 | Era 3 |
| 5 | 4 | 5 | Era 4 |

  - **Duas travas ao mesmo tempo:** o **nível** da construção e a **era** da tecnologia (a era libera o Tier no mundo; o nível libera a receita na construção). O material também limita o Rank (Ferro até 3, Aço até 4, Aço Mágico até 5).
  - **Tabela de Rank por era** (fecha o "em aberto" da Tecnologia): Era 1, Rank até 3; Era 2, até 4 (Aço); Era 3, até 4; Era 4, até 5 (Aço Mágico).
  - As construções **Braçais** continuam como estão: o nível só aumenta a eficiência.
  - *Exemplo:* um Ferreiro nível 3, na Era 2, faz Espada de Aço até Rank 3; com nível 4 e Era 3, até Rank 4; a Espada de Aço Mágico só no nível 5, na Era 4.
- *Conflito:* hoje há construções de nível 4 e 5 nas capitais já na Era 1 (pacotes do [[Mundo e Estado Inicial/Descrição|Mundo]]). Saída: o nível alto nas capitais continua dando **eficiência e mais vagas**, mas as receitas de Tier e Rank maiores só liberam com a era.
- *Impacto:* Produção, Construção, Tecnologia, Itens.

**IP1-4. De onde sai o Rank do item produzido.**

- *Contexto:* "Espada Curta de Ferro (Rank 1-3)" diz o intervalo, mas não como se chega a um Rank.
- *Sugestão:* o Rank do item é o **menor** de três valores:
  1. o teto do **material** (Ferro 3, Aço 4, Aço Mágico 5);
  2. o teto do **nível** da construção (IP1-3);
  3. o Rank pela **habilidade** do trabalhador: **1 + habilidade de Trabalho ÷ 25**, arredondado para baixo (0 a 24 = 1; 25 a 49 = 2; 50 a 74 = 3; 75 a 99 = 4; 100 = 5).

  - **Sorte:** 20% de chance de sair 1 Rank acima (se os tetos deixarem) e 10% de sair 1 abaixo.
  - **Em LOD 2:** usa a habilidade média dos trabalhadores, sem sorte.
  - *Exemplo:* um Ferreiro com Trabalho Especialista 60, num Ferreiro nível 4, fazendo Espada de Ferro: habilidade dá 3, nível dá 4, material dá 3 → **Rank 3**. Com Aço, o material deixa 4, mas a habilidade continua dando 3.
  - Itens com **Tier** (materiais, comida) não têm qualidade: a habilidade só acelera.
- *Impacto:* Itens, Produção, Trabalho (habilidade dos empregados), Combate e Batalha (o Rank do kit).

**IP1-5. Escala: o que 1 NPC real produz e consome.**

- *Contexto:* 1 NPC real representa um Fator de Escala de pessoas (exemplo 100), e 1 Esquadrão (100 soldados) = 1 NPC alistado que come 1 Ração por dia. Ainda não está escrito se, nas contas da economia, 1 NPC real conta como 1 pessoa ou como 100. Isso muda **todos** os números da V2.
- *Opções:*
  - (a) **1 NPC real = 1 trabalhador e 1 consumidor nas contas.** O Fator de Escala só muda a **população exibida**, os conscritos e o tamanho visual dos soldados;
  - (b) o Fator de Escala multiplica a produção e o consumo de cada NPC real: números realistas e enormes (uma cidade de 100 NPCs come 10 mil pães por dia), e o preço de um escravo NPC real compraria 100 pessoas.
- *Sugestão:* (a). É coerente com o que já foi decidido:
  - 1 Ração por Esquadrão por dia = 1 Ração por NPC alistado;
  - o escravo é comprado como 1 NPC real que trabalha como 1 trabalhador;
  - o Controlado come do mesmo item que a cidade vende.
  - *Exemplo:* uma cidade de 100 NPCs reais come cerca de 100 a 150 unidades de comida por dia; a interface mostra "População: 10.000" (com o Fator 100), mas as contas usam 100.
- *Impacto:* Classe Social, Econômico, Produção, Guerra, Escravidão, V2 (todos os números ficam pequenos e fáceis de balancear).

**IP1-6. Tempo e quantidade por ciclo.**

- *Contexto:* "tempo base reduzido pela habilidade" está decidido, sem números.
- *Sugestão:*
  - **Horas de trabalho por unidade**, por Tier, como guia para a IP3:

| Produto | Horas por unidade |
|---|---|
| Tier 0 (Braçal) | 0,5 |
| Tier 1 | 1 |
| Tier 2 | 2 |
| Tier 3 | 4 |
| Tier 4 | 8 |
| Arma ou peça de Ferro e Cobre | 8 |
| Arma ou peça de Aço | 12 |
| Arma ou peça de Aço Mágico | 24 |

  - **Velocidade do trabalhador** = 0,5 + habilidade ÷ 100 (habilidade 0 = metade; 50 = normal; 100 = 1,5 vez).
  - **Nível acima do mínimo:** +10% por nível.
  - *Exemplo:* um Cozinheiro com habilidade 50 trabalha 8 horas e faz 8 Pães (Tier 1) por dia; uma cidade de 100 NPCs precisaria de cerca de 12 a 18 cozinheiros, ou de comidas que rendem mais por hora (a IP3 equilibra). As horas do turno vêm da rotina (V2).
- *Impacto:* Produção, Trabalho, IA (rotina), V2 (preços pelo custo das horas).

**IP1-7. Quem decide o que produzir.**

- *Contexto:* a cidade produz pela demanda; o jogador ainda não tem ordens de produção (a ideia 4 da [[Revisão do Projeto]], ordens em lote estilo RimWorld, nunca foi decidida).
- *Sugestão:*
  - **Construções do jogador:** fila de **ordens**, como no RimWorld:
    - **Fazer N** (por exemplo, 20 Espadas);
    - **Manter estoque de N** (produz até ter N; volta a produzir quando cai);
    - **Repetir sempre**.
    Mais uma prioridade por ordem.
  - **Construções da cidade e dos Ricos (IA):** a cada dia (LOD 0 e 1) ou estação (LOD 2), escolhem a receita liberada com a maior nota = **(preço local do produto − custo dos insumos) × (1 + déficit da cidade naquele item)**. A comida tem prioridade quando o estoque cai abaixo de 3 dias de consumo.
- *Impacto:* Produção, Interface (painel de ordens), IA, Econômico.

**IP1-8. Limites de complexidade.**

- *Contexto:* o jogo é de uma pessoa só, e cada receita é dado a escrever e a balancear.
- *Sugestão:*
  - No máximo **3 insumos** e **3 saídas** (produto e subprodutos) por receita.
  - Cadeia de no máximo **4 passos** do mapa ao produto final (por exemplo, Ferro Bruto → Barra de Ferro → Espada).
  - **Nenhum item órfão:** todo item intermediário precisa ser usado por pelo menos uma receita ou consumido por alguma classe. O `jogo --validar` avisa os órfãos.
  - **Sem ferramentas** como insumo na primeira versão; o desgaste das construções já é a manutenção.
- *Impacto:* Produção, Itens, Pipeline (validação dos dados).

**IP1-9. Ordem das rodadas de Itens e Produção.**

- *Sugestão:*
  - **IP2:** os nomes e a lista final dos itens. Inclui:
    - encaixar os nomes no mundo (I-11);
    - os itens que faltam e os que sobram;
    - Luxo × Exclusivo por Tier;
    - Pedra Mágica × Pedra de Mana;
    - a peça Média.
  - **IP3:** as receitas de cada produto, numa tabela por construção, com a nova Funções e Construções.
  - **Depois:** V2 (preços, consumo, salários e custos, já com as receitas prontas), V3, V4 e V5.

  Os preços ficam para a V2 porque saem das receitas: **preço base ≈ custo dos insumos + horas de trabalho × salário**.

Depois das respostas: atualizar Produção, Itens, Construção e Tecnologia com as regras e abrir a **IP2**.

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
- **Efeitos de itens:** só Controlados e Importantes usam itens com efeito individual; Livro +3 (até 60) e Livro Antigo +6 (até 80), sem se gastar; Livro de Atributos +2 × Rank, se gasta; **Livro Sagrado obrigatório** na Religiosa (ideia do autor; sem ele na revista, heresia de gravidade 2 na primeira vez); Artefatos da Realeza como posse; estado Doente e traço Viciado (ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).
- **Escravidão:** 5 fontes; Mercado de Escravos com preço 200 + 4 × a melhor habilidade de Trabalho; Obediência com fórmula; fuga abaixo de 30 de Obediência; rebelião com Felicidade e Controle abaixo de 10 por 4 dias; abolição liberta tudo em 1 estação; 3 caminhos do estilo Escravo (ver [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]).
- **Revisão V1:** a pesca voltou (Pesqueiro); animais são itens (Cavalo = montaria e carga); um inventário por cidade, com pilhas; Mosquete (não Musket); Fome dos Controlados cai 100 em 36 horas; Templo com a tabela geral de vagas (2 nas capitais); crime de Sedição; os 13 documentos antigos estão no formato padrão.
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
