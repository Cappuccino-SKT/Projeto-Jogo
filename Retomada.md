> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a rodada V2 (Base numérica da economia) aplicada e a V3 (Pendências de regra) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas P0 e P1 estão escritos, e a **revisão completa** está em andamento (levantamento e plano na [[Revisão Completa]]). A V1, a IP1 e a IP2 já estão no `main` (PRs #9 a #11). A IP3 está na branch `docs/ip3-receitas`. Em 2026-10-17 foi aplicada a **V2** (a economia em números) na branch `docs/v2-economia`, criada a partir da `docs/ip3-receitas`. A **rodada V3 (Pendências de regra) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se os pull requests da `docs/ip3-receitas` e da `docs/v2-economia` foram mesclados no `main`, nessa ordem.**
3. **Próximo passo:** pedir o `Respostas.txt` da V3 (conferir se é desta rodada), aplicar cada decisão no seu documento (a tabela de leis iniciais vai para a Facção e a Cultura) e abrir a **V4** (confirmação das propostas, parte 1).
4. Depois: V5 e a **ficha técnica lógica da fatia vertical**. As listas de conteúdo (tecnologias, feitiços, missões) dependem da resposta da V3-13.
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
| Revisão | V2 (economia numérica) | Aplicada |
| Revisão | **V3 (pendências de regra)** | **Enviada, aguardando respostas** |
| Revisão | V4 e V5 (propostas) | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, modo observador, eventos de dinastia. As ordens de produção em lote foram decididas na IP1-7 ([[Sistema de Produção/Descrição|Produção]]). A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: V3, Pendências de regra

**Perguntas enviadas em 2026-10-17, aguardando respostas.** Com a economia em números (V2), sobram as **pendências de regra** dos sistemas (seção 5 da [[Revisão Completa]]), as **leis iniciais** (A-10) e os **bônus dos estilos de início** (A-11). As listas grandes de conteúdo (A-12) ganham uma pergunta própria sobre quando fazê-las (V3-13).

Responder no `Respostas.txt` pelos códigos (V3-1, V3-2...). "Seguir sugestão" aceita a proposta. Os números usam a moeda da V2: um Civil ganha 8 por dia, o Pão custa 1,7 e um escravo de habilidade 60 custa 440.

**V3-1. As leis iniciais de cada facção (A-10).**

- *Contexto:* as leis existem na [[Sistema de Facção/Descrição|Facção]], e o desejo de cada cultura está na [[Sistema de Cultura/Descrição|Cultura]]. Mas não há uma tabela única com a lei de cada facção no primeiro dia. Algumas já estão decididas:
  - a Religiosa começa com Perseguição e com o Livro Sagrado obrigatório;
  - a Escravista tem escravidão ampla e imposto alto;
  - a Guerreira vende prisioneiros à Escravista.
- *Sugestão:* as leis iniciais seguem o desejo de cada cultura, sem pensamento extremo (o extremo fica para o jogo produzir):

| Lei | Guerreira | Arcana | Autoritária | Religiosa | Tecnocrata | Escravista |
|---|---|---|---|---|---|---|
| Escravidão | Permitida (só prisioneiros de guerra) | Proibida | Permitida (trabalho forçado como pena) | Proibida | Proibida | Permitida, ampla |
| Imposto (V2-7) | Baixo | Normal | Normal | Normal | Normal, mais o imposto para pesquisa | Alto |
| Conscrição | Alta (10% da população) | Baixa (3%) | Alta (8%) | Média (5%) | Baixa (3%) | Média (5%; escravos podem ser alistados) |
| Magia | Restrita | Privilégio dos magos | Restrita | Restrita | Proibida | Livre |
| Poder do líder | Moderado | Moderado | Autoritário | Moderado | Fraco | Fraco |
| Sistema de votação | Maioria simples | Conselho (só Magos) | Conselho de nobres | Maioria qualificada (2/3) | Ponderada pela população | Ponderada pelo nível da cidade |
| Tolerância religiosa | Tolerância | Fé oficial | Fé oficial | Perseguição | Tolerância | Tolerância |
| Livro Sagrado obrigatório | Não | Não | Não | Sim | Não | Não |
| Tarifa | Média (10% da carga) | Média | Alta (20%) | Média | Média | Baixa (5%) |
| Itens proibidos | — | — | — | Narcóticos | Cajados, Cetro e Joia de Mana | — |
| Ajuste de preço da facção | Armas −20% | Itens de mana −20% | Comida −20% | Remédio e Curativo −20% | Armas de fogo e Pólvora −20% | — |

- *Conflito:* a Tecnocrata tem "magia proibida" como **pensamento extremo**, mas a lei inicial sugerida já é "Proibida".
  - **Saída:** a lei Proibida significa que **usar** magia é crime;
  - o extremo continua sendo **banir** os Magos da facção.
- *Impacto:*
  - Facção, Cultura, Crime (itens proibidos), Escravidão (fecha a pergunta "quem mais começa com escravidão");
  - Felicidade (leis × cultura), Comércio (Tarifa), Mundo.

**V3-2. Os bônus dos estilos de início (A-11).**

- *Contexto:* os 7 estilos têm só exemplos de bônus, e o personalizado tem um "orçamento de pontos" sem números.
- *Sugestão:*

| Estilo | Dinheiro | Itens e grupo | Habilidade e relação |
|---|---|---|---|
| Dono de vila | 300 | Assentamento nível 1 (1 Casa, 1 Fazenda, 1 Posto de Coleta), 40 Tábuas, 40 Blocos de Pedra, 4 colonos Civis | +10 de Inteligência |
| Aventureiro | 150 | 2 companheiros; armas e armadura Leve de Rank 2 | +10 em Corpo a Corpo ou Longo Alcance |
| Membro de facção | 200 | Título Cidadão (ou Cavaleiro, se Nobre); uma Casa na cidade | Relação +30 com a facção |
| Mercador | 800 | 2 Cavalos e 1 companheiro (líder de caravana); preços de 3 cidades vizinhas já conhecidos | +15 de Carisma |
| Nobre exilado | 400 | 2 companheiros de Nobreza; Espada Longa de Aço de Rank 2 | Relação +20 com a sua cultura e −20 com a facção que tomou o trono |
| Escravo | 0 | 2 escravos aliados (Vínculo alto); Grilhões | +20 de Trabalho Braçal |
| Mago | 100 | Cajado Simples de Rank 2, 3 Pedras de Mana | Mago; +20 de Magia |

  - **Personalizado:** 100 pontos, a partir de um personagem Civil sem nada:

| Escolha | Custo |
|---|---|
| 10 moedas | 1 ponto |
| Companheiro | 15 pontos |
| +10 numa habilidade | 10 pontos |
| Item | 1 ponto a cada 10 moedas de preço |
| Ser Mago | 30 pontos |
| Assentamento nível 1 | 40 pontos |
| Título Cidadão | 10 pontos |

    A cultura é grátis. Cada estilo pronto vale cerca de 100 pontos, o que mantém o personalizado justo.
- *Impacto:* Início de Jogo (fecha as 2 perguntas, inclusive o bônus do Mercador), Comércio, Companheiros.

**V3-3. Crime: multas, Caçadores, loja ilegal do jogador e missão de confiança.**

- *Contexto:*
  - o [[Sistema de Crime e Justiça/Descrição|Crime]] tem gravidades de 1 a 5, mas multas sem valor;
  - não diz se o jogador pode ter loja ilegal;
  - não diz quais são as missões de confiança.
- *Sugestão:*
  - **Multa** = 25 × gravidade² moedas:

| Gravidade | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|
| Multa (moedas) | 25 | 100 | 225 | 400 | 625 |

    A **Recompensa** é o dobro da multa, somada por crime. Os **Caçadores de Recompensa** surgem quando ela chega a **500** numa facção (um crime de gravidade 5, ou vários menores).
  - **Loja ilegal do jogador:** sim, qualquer construção de produção dele.
    - **Batida da guarda:** a cada estação, chance = Controle da cidade ÷ 200 (Controle 60 = 30%).
    - **Se descoberta:** confisco do estoque ilegal, crime de Contrabando (gravidade 3) para o gerente e para o jogador, se ele estiver no mapa.
  - **Missão de confiança:** a Confiança do NPC vai de 0 a 100. Com 50, o jogador compra na loja ilegal; com 80, também vende. Três tipos de missão, sorteados:

| Missão | O que é | Confiança |
|---|---|---|
| Entrega | Levar mercadoria proibida a outra cidade | +25 |
| Dívida | Cobrar um devedor do NPC | +20 |
| Informante | Achar quem falou com a guarda e calá-lo, por suborno ou à força | +35 |

- *Impacto:* Crime, Econômico, Reputação, Objetivo (missões).

**V3-4. Cura: quando o Inconsciente vira Morto.**

- *Contexto:* a [[Sistema de Cura/Descrição|Cura]] tem uma proposta (golpe de misericórdia ou sangramento), sem números.
- *Sugestão:* confirmar a proposta, com números:
  - **Modo não letal:** Vida 0 deixa Inconsciente, sem sangramento. Ele acorda em **4 horas de jogo** com 10% da Vida.
  - **Modo letal:** Vida 0 deixa Inconsciente **sangrando**. Sem Curativo, ele morre em **2 horas de jogo** (80 s reais a 1x). O Curativo estanca o sangramento, como já decidido.
  - **Golpe de misericórdia:** um ataque letal contra um Inconsciente mata na hora. É crime de Assassinato, se for contra um cidadão.
  - **Hospital:** cura 5% da Vida por hora de jogo; com um Mago de Suporte, 10%. Custa 2 moedas por dia de internação.
- *Impacto:* Cura, Combate, Crime.

**V3-5. Narradores, intensidade e diário.**

- *Contexto:* o [[Sistema de Eventos/Descrição|Narrador]] é a dificuldade, mas cada um tem um **foco**, e nenhum é mais difícil que outro. Faltam os nomes, os focos, um controle de intensidade e o formato do diário.
- *Sugestão:*
  - **4 narradores**, com nomes significativos até a história:

| Narrador | Foco | Combina com |
|---|---|---|
| O Cronista | Equilíbrio entre todas as categorias | Qualquer estilo |
| A Guerra | Ameaça, guerra e bandidos | Aventureiro, Membro de facção |
| A Terra | Clima, colheita, saúde e economia | Dono de vila, Mercador |
| A Corte | Política, intrigas e sucessão | Nobre exilado, Membro de facção |

  - **Intensidade:** um controle separado, de 50% a 150%, que multiplica a tensão do Narrador. Padrão: 100%. O foco muda **o quê**; a intensidade muda **quanto**.
  - **Diário:**
    - guarda as últimas **300 entradas**, e as mais antigas vão para um resumo por ano;
    - filtros por categoria, lugar e Aviso (Info, Aviso, Crítico);
    - busca por texto.
- *Impacto:* Eventos, Início de Jogo (escolha do narrador), Interface.

**V3-6. Os eventos iniciais da fatia vertical (parte da A-12).**

- *Contexto:* o Eventos sugere começar com 10 a 15 eventos, sobretudo Clima, Saúde, Sorte e o déficit de consumo. A fatia vertical tem só uma vila, sem guerra nem facção.
- *Sugestão:* **12 eventos**:

| Evento | Tipo | Gatilho e efeito |
|---|---|---|
| Chuva boa | Ocasional (Clima) | Primavera; Fazenda +20% por 4 dias |
| Seca | Ocasional (Clima) | Verão; Fazenda −50% por 8 dias |
| Geada precoce | Ocasional (Clima) | Fim do outono; a colheita para 2 dias antes |
| Tempestade forte | Ocasional (Clima) | Danifica 1 construção de madeira (−20% de Vida) |
| Febre | Ocasional (Saúde) | Mortalidade +50% por 8 dias; gasta Remédio se houver |
| Colheita farta | Ocasional (Sorte) | Outono; Fazenda +30% por 4 dias |
| Mercador viajante | Ocasional (Sorte) | Uma caravana pequena vende 3 itens de fora |
| Achado na floresta | Ocasional (Sorte) | +20 Tora ou +5 Pedra de Mana no estoque |
| Ratos no celeiro | Ocasional (Natureza) | −10% do Grão estocado |
| Fome | Condicional | Comida abaixo de 10% por 3 dias: Felicidade −10 e migração para fora |
| Falta de móveis | Condicional | Materiais Básicos em déficit por 8 dias: Felicidade dos Civis −5 |
| Festa da Colheita | Data fixa (festival) | Felicidade +10 por 4 dias; consumo de Cerveja × 1,5 |

- *Impacto:* Eventos, Clima, Classe Social (dias em déficit), Religião (festival), Arquitetura (dados da fatia vertical).

**V3-7. Assembleia e guerra civil na sucessão.**

- *Contexto:*
  - a assembleia anual está decidida (quem vota, o Sistema de votação, o veto do líder), mas os detalhes estão "a refinar";
  - a crise de sucessão "pode gerar guerra civil", sem dizer como os lados se formam.
- *Sugestão:*
  - **Pauta:** no máximo **3 leis** por assembleia. Cada cidade de IA propõe a lei mais distante do desejo da sua cultura. Ganham as 3 propostas por mais cidades.
  - **Voto da IA:** a cidade vota sim se a lei aproxima a facção do desejo da sua cultura. Com Felicidade abaixo de 30, ela vota contra o líder.
  - **Veto do líder** (quando a lei Poder do líder deixa): chance = 50% + (Agressividade − Sociabilidade) ÷ 2. Cada veto: legitimidade −5.
  - **Efeito:** a lei vale no dia seguinte. As cidades que votaram e perderam têm Felicidade −5 por 1 estação.
  - **Guerra civil:**
    - **candidatos:** até 3, o herdeiro pela regra de sucessão, o nobre de maior Renome e o general do maior exército;
    - **lados:** cada cidade escolhe pela Opinião do seu governador sobre cada candidato;
    - **luta:** os lados viram facções separadas em guerra, com o Placar de Guerra comum;
    - **fim:** quem chegar a +60 no Placar reúne a facção; se não houver acordo em 2 anos, o lado menor fica independente.
- *Impacto:* Facção, Diplomacia, Guerra, Companheiros (herdeiros), Notícias.

**V3-8. Cidade, população e trabalho.**

- *Contexto:* sobram 5 pendências pequenas na População e no Trabalho (as moradias já têm capacidade pela V2-8).
- *Sugestão:*
  - **Lotes por nível de cidade** (pequeno / médio / grande):

| Nível | 1 | 2 | 3 | 4 |
|---|---|---|---|---|
| Lotes pequenos | 8 | 16 | 30 | 60 |
| Lotes médios | 3 | 6 | 12 | 24 |
| Lotes grandes | 0 | 1 | 3 | 8 |

    O porte das construções vem da V2-8.
  - **Moradia dos Controlados:** um quarto na Taverna (2 moedas por dia por companheiro) ou uma Casa do jogador. Sem nenhum dos dois, eles dormem ao relento.
  - **Crianças decorativas:** não na primeira versão. No futuro, só alguns bonecos genéricos, sem simulação.
  - **Gerente das construções da Cidade (LOD 0):** o empregado de maior habilidade vira gerente, com salário × 1,5, sem precisar de outro NPC.
  - **Leilão:** uma construção parada por 2 estações vai a leilão por 3 dias. O lance mínimo é 50% do custo de obra, e podem dar lance os Ricos da cidade e o jogador.
  - **Universidade:** o Controlado paga 20 moedas por dia e estuda 8 h. Ganha +1 em Inteligência ou Trabalho Especialista a cada 2 dias, até 70.
- *Impacto:* População, Trabalho, Construção, Companheiros.

**V3-9. Estradas, patrulhas, muralhas e guerra.**

- *Contexto:*
  - a Viagem quer estradas construídas (quem, quanto, tecnologia) e o número de patrulhas;
  - a Guerra quer o custo da Muralha e do Posto da Guarda e as estradas destruídas.
- *Sugestão:*
  - **Estrada** (multiplicador 0,75 da Viagem): construída num Mapa Local inteiro.
    - **Custo:** 40 Blocos de Pedra + 20 Tábuas + 160 h de obra, pelo Construtor da facção ou do jogador.
    - **Requisitos:** tecnologia "Estradas" (Era 1, ramo Construção).
  - **Guerra e estradas:** o saque e o cerco danificam a estrada do mapa (multiplicador 0,9 até o reparo, com 25% do custo).
  - **Patrulhas:** 1 Esquadrão leve para cada 4 mapas de território. Custa o soldo normal, 8 por dia.
  - **Muralha** (3 níveis):

| Nível | Custo |
|---|---|
| Paliçada (porte médio) | 60 Tábuas |
| Muralha de Pedra (grande) | 200 Blocos de Pedra + 40 Tábuas |
| Bastião (grande, Era 3) | 400 Blocos de Pedra + 40 Barras de Ferro |

    A manutenção segue a regra geral da V2-8.
  - **Posto da Guarda:** porte pequeno.
- *Impacto:* Viagem, Guerra, Construção, Tecnologia, Facção.

**V3-10. Diplomacia e Companheiros.**

- *Contexto:*
  - a Diplomacia não diz quando a coalizão se desfaz;
  - os Companheiros têm os bônus dos cargos sem números e três costumes de casamento como proposta (Autoritária: permissão do líder; Tecnocrata: contrato com taxa; Arcana: os dois Magos, ou o não Mago aceita ficar abaixo).
- *Sugestão:*
  - **Coalizão:** se desfaz quando a Ameaça do alvo fica abaixo de 40 por 1 estação, ou quando o alvo assina paz com todos os membros.
  - **Bônus dos cargos:**

| Cargo | Bônus |
|---|---|
| Governador | +1 de Controle e +1% de imposto a cada 10 de Carisma acima de 30 (até +7) |
| Gerente | +1% de produção a cada 5 de Trabalho Especialista acima de 40 (até +12%) |
| Líder de caravana | −10% de risco e +5% de lucro a cada 20 de Carisma acima de 30 |
| Professor | +10% no bônus de educação da Universidade |

  - **Costumes de casamento:** confirmar os três propostos. A taxa do contrato Tecnocrata é de **50 moedas**.
- *Impacto:* Diplomacia, Companheiros, Trabalho, Comércio.

**V3-11. Magia: Grimório, filhos de Mago e feitiços fora do combate.**

- *Contexto:* a [[Sistema de Magia/Descrição|Magia]] não diz como se obtém o Grimório, qual a chance de filho de Mago nascer Mago, nem quais feitiços de Suporte existem fora do combate.
- *Sugestão:*
  - **Grimório:**
    - **onde se acha:** nas masmorras, como loot, e no Mercado das capitais Arcanas (cerca de 80 moedas);
    - **quem produz:** a **Torre Arcana nível 3** copia o Grimório de um feitiço que a facção já pesquisou, com 2 Couros Refinados + 1 Pedra de Mana, em 24 h, por um Mago.
  - **Chance de nascer Mago:** 2% sem pais Magos; 25% com um; 60% com os dois. Em LOD 2, vale a média da cidade.
  - **Feitiços de Suporte fora do combate** (só 3 na primeira versão):

| Feitiço | Efeito | Limite |
|---|---|---|
| Bênção da Colheita | Fazenda +20% por 1 estação | 1 por cidade por estação |
| Acalmar Tempestade | Cancela uma Tempestade forte no mapa | Custa 3 Pedras de Mana |
| Curar Ferimentos | Igual ao Hospital com Mago de Suporte, mas no campo | — |

- *Impacto:* Magia, Itens, Produção, Clima, Cura.

**V3-12. Interface, fonte e PC alvo.**

- *Contexto:* a Interface tem 5 pendências, e a Arquitetura o PC alvo mínimo. A V2-11 deu as metas de desempenho.
- *Sugestão:*
  - **HUD:** nenhum elemento fixo, nem o menu de pausa, que abre com Esc. As janelas de gestão também se movem e mudam de tamanho, e a posição fica salva no perfil do jogador.
  - **Fonte:** **Noto Sans**. É grátis, tem licença livre e tem os acentos e os alfabetos de outros idiomas. O tamanho padrão é 14 px em 1600 × 900, com escala de 80% a 150%.
  - **Painel "Por quê?":** uma classe por vez, em abas, com um resumo das 5 classes no topo.
  - **Idioma:** IDs de texto desde o primeiro dia, no arquivo `textos/pt-BR.json`.
  - **Notificações:** 15 tipos, cada um com Desligada, Diário, Ícone ou Pausa. Os padrões seguem o Aviso de cada evento.
  - **PC alvo mínimo:** processador de 4 núcleos (2015 ou mais novo), 8 GB de memória, placa de vídeo com OpenGL 3.3 e 2 GB.
- *Impacto:* Interface, Arquitetura, Pipeline (teto de memória da arte), Eventos.

**V3-13. Listas de conteúdo e pontas soltas.**

- *Contexto:* a A-12 pede listas grandes:
  - 40 a 60 tecnologias e os marcos mundiais;
  - os feitiços e a árvore arcana;
  - os contratos e as missões;
  - as condições das Ambições.

  A fatia vertical **não usa** tecnologia, magia nem guerra (só os eventos da V3-6). Sobram também três pontas pequenas.
- *Opções:*
  - (a) fazer as listas agora, em 3 rodadas de conteúdo, antes da ficha técnica;
  - (b) fazer a V4 e a V5 (propostas) e a **ficha técnica da fatia vertical** primeiro, e deixar as listas para as **rodadas de conteúdo C1 a C3**, durante a programação da fatia vertical.
- *Sugestão:* (b), porque as listas não travam nada do que vem primeiro e ficam melhores quando a base já roda.
  - **C1:** tecnologias e marcos mundiais.
  - **C2:** feitiços, árvore arcana e Grimórios.
  - **C3:** contratos, missões, Ambições e eventos além dos 12 da fatia vertical.

  **Pontas soltas:**
  - **Destilaria:** construída já no nível 2 (IP3), confirmar.
  - **Cultura Agrária:** fica fora da primeira versão, como traço futuro.
  - **Pipeline** (artista definitivo, pacotes provisórios, ferramenta de atlas): decidir no início do código.
- *Impacto:* o plano da [[Revisão Completa]], Tecnologia, Magia, Objetivo.

Depois das respostas:

- aplicar cada decisão no seu documento;
- montar a tabela de leis iniciais na Facção e na Cultura;
- abrir a **V4** (confirmação das propostas, parte 1).

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
- **Economia (V2):**
  - **dinheiro:** moeda com 1 casa decimal, guardada em décimos inteiros; preço = (insumos + horas × salário) ÷ lote × 1,2;
  - **salário por hora:** 0,75 Braçal, 1,0 Manufaturado, 1,5 Especialista; soldo 8 por Esquadrão por dia;
  - **preços:** Pão 1,7; Barra de Ferro 2,1; Espada Curta de Ferro 13,9;
  - **mercado:** Mercado Local de × 0,5 a × 2,0 pelo estoque (alvo de 5 dias), andando no máximo 10% por dia;
  - **consumo:** cesta mínima por classe e regra do gasto de 80%;
  - **leis:** imposto de 10, 20 ou 30%; cunhagem de 1 Barra de Ouro em 25 moedas;
  - **obras:** 3 portes de construção;
  - **rotina:** Civil trabalha das 7h às 15h; tick de 20 por segundo;
  - **fatia vertical:** vila de 15 NPCs.
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
