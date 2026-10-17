> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a rodada V4 (Confirmação das propostas, parte 1) aplicada e a V5 (parte 2, a última da revisão) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas P0 e P1 estão escritos, e a **revisão completa** está na última rodada (levantamento e plano na [[Revisão Completa]]). Já estão no `main` as rodadas V1, IP1, IP2, IP3, V2 e V3 (PRs #9 a #14). Em 2026-10-17 foi aplicada a **V4**, na branch `docs/v4-propostas`: as propostas de 7 documentos e os moldes de conteúdo foram confirmados, e a Arquitetura seguiu as recomendações, a pedido do autor. A **rodada V5 (Confirmação das propostas, parte 2) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se o pull request da `docs/v4-propostas` foi mesclado no `main`.**
3. **Próximo passo:** pedir o `Respostas.txt` da V5 (conferir se é desta rodada), tirar as marcações confirmadas, aplicar IT-4 e IT-5, marcar a revisão como concluída e começar a **ficha técnica lógica da fatia vertical** no formato da V5-9.
4. Depois: a programação da fatia vertical. As listas de conteúdo ficam para as rodadas C1 a C3 (ideia do autor na V3-13).
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
| Revisão | V3 (pendências de regra) | Aplicada |
| Revisão | V4 (propostas, parte 1) | Aplicada |
| Revisão | **V5 (propostas, parte 2)** | **Enviada, aguardando respostas** |
| Depois | Ficha técnica da fatia vertical; rodadas de conteúdo C1 a C3 | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, modo observador, eventos de dinastia. As ordens de produção em lote foram decididas na IP1-7 ([[Sistema de Produção/Descrição|Produção]]). A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: V5, Confirmação das propostas (parte 2)

**Perguntas enviadas em 2026-10-17, aguardando respostas.** Esta é a última rodada da revisão completa. Ela confirma as **58 propostas** que sobraram nos outros documentos, em 8 grupos, e pergunta o formato da **ficha técnica lógica da fatia vertical**, o passo seguinte.

**Como responder:** "Confirmar todas" ou "Confirmar todas, exceto PR-3 e CB-4 (e o que mudar)". A recomendação vem no fim de cada linha:

- **Confirmar:** a proposta vira decisão.
- **Mudar:** vem com o motivo.
- **Já decidida:** só limpar a marcação.

**V5-1. Produção, Funções e Receitas (8 propostas).**

- PR-1. Um ciclo interrompido guarda o progresso em horas; os insumos já gastos não voltam. **Confirmar.**
- PR-2. O teto do material é o máximo da faixa do item na lista (vale também para itens sem metal, como as roupas). **Confirmar.**
- PR-3. O Rank 0 não sai da produção: fica para itens quebrados ou improvisados. **Confirmar.**
- PR-4. Horas reais = horas da receita ÷ (velocidade × (1 + 0,1 × níveis acima do mínimo)); cada trabalhador faz o seu ciclo. **Confirmar.**
- PR-5. O déficit da nota da IA vai de 0 a 1, pelo contador de dias em déficit. **Confirmar.**
- PR-6. Funções dos serviços públicos: Lojista, Taverneiro, Oficial instrutor, Carcereiro, Moedeiro, Sacerdote, Mercador, Pesquisador, Mago, Curandeiro e Professor. **Confirmar.**
- PR-7. Habilidade mínima de cada nível da receita: 10, 25, 40, 55 e 70. **Confirmar.**
- PR-8. Peixe por estação: 100% na primavera, no verão e no outono; 75% no inverno; 25% no inverno dos biomas frios. **Confirmar.**

**V5-2. Itens, Inventário e NPC (5 propostas).**

- IT-1. Intervalo de Ataque por tipo de arma: Espada e Lança rápidas; Machado e Clava lentas; Besta e armas de fogo muito lentas. **Confirmar.**
- IT-2. Cetro: cajado de uma mão, que deixa usar Escudo, com dano 25% menor. **Confirmar.**
- IT-3. O conjunto Furtivo dá Destreza; o Mercador e o Nobre dão Carisma. **Confirmar** (os valores ficam a testar).
- IT-4. Capacidade de armazenamento de cada construção, que estava "para a V2" mas ficou sem número. **Mudar:** 100 × nível nas pequenas, 200 × nível nas médias e 400 × nível nas grandes, nas categorias que ela produz ou vende. Bate com o exemplo que já existe: Fazenda nível 2 = 200.
- IT-5. Saciedade da comida "Básico 60, Refinado 80, Luxo 100". **Mudar:** pelo Tier, porque esses grupos deixaram de existir na IP2. Tier 1 = 60, Tier 2 = 80, Tier 3 = 90 e Tier 4 = 100.

**V5-3. Combate e Batalha (7 propostas).**

- CB-1. Os valores iniciais da fórmula de dano e acerto. **Confirmar** (a testar).
- CB-2. A tabela de vantagem entre arma e armadura, com no máximo ±25%. **Confirmar.**
- CB-3. Armadura mista: vale o peso mais frequente entre as três peças (empate: Média). **Confirmar.** Era a I-18 da revisão.
- CB-4. Cerca de 40 combatentes ativos no combate local. **Confirmar** como valor a testar.
- CB-5. Atributos da Milícia na Batalha (só na defesa da própria cidade). **Confirmar.**
- CB-6. O tamanho visual dos soldados pode ser trocado nas Configurações, fora da batalha, sem mexer no save. **Confirmar.**
- CB-7. Com a lei de Magia "Restrita", só Magos de Suporte nos Esquadrões. **Confirmar.**

**V5-4. Guerra, Diplomacia e Viagem (9 propostas).**

- GU-1. Uma companhia mercenária sem contrato fica na sede e vira notícia. **Confirmar.**
- GU-2. Para pedir ao líder a cidade que tomou, o jogador precisa de Renome 40 (além da Opinião +30). **Confirmar.**
- GU-3. Numa guerra com aliados, cada lado negocia pelo seu líder de guerra; os aliados saem junto. **Confirmar.**
- GU-4. Assinar um tratado dá +5 de relação; cancelar um que ainda vale dá −10. **Confirmar.**
- GU-5. A IA procura Aliança Defensiva com facções afins quando um vizinho tem 1,5 vez a força dela. **Confirmar.**
- GU-6. Multiplicadores de infraestrutura na Viagem (selvagem, trilha, estrada...). **Confirmar.**
- GU-7. Multiplicadores do tamanho do grupo (um Esquadrão conta como 100 pessoas). **Confirmar.**
- GU-8. Tempo mínimo de 1 hora por Mapa Local. **Confirmar.**
- GU-9. O progresso das entidades em trânsito avança por hora, e os encontros são sorteados ao entrar num mapa. **Confirmar.**

**V5-5. Crime, Escravidão e Cultura (8 propostas).**

- CJ-1. Lista inicial de crimes e gravidades. **Confirmar.**
- CJ-2. A Criminalidade sobe com a infelicidade e a pobreza, e cai com os guardas. **Confirmar** (os pesos ficam a testar).
- CJ-3. O trabalho forçado marca o NPC como Escravo com prazo; ao fim, ele volta à classe anterior. **Confirmar.**
- CJ-4. A construção tem o atributo "Ilegal" e o dono guarda a Confiança no jogador. **Confirmar.** A V3-3 já usa isso.
- CJ-5. Libertar um escravo dá +2 de reputação com a Religiosa. **Confirmar.**
- CJ-6. Na primeira versão, só a Religiosa aplica o −10 de relação a quem mantém Mercado de Escravos. **Confirmar.**
- CJ-7. A primeira lista de traços das 6 culturas (leis desejadas, rito, sucessão, pensamento extremo). **Confirmar** como base; as leis iniciais da V3-1 já saem dela.
- CJ-8. O pensamento extremo é uma lei com valor ideal no limite e peso máximo. **Confirmar.**

**V5-6. Notícias e Eventos (7 propostas).**

- NE-1. A distância de uma notícia conta os passos no grafo dos 48 mapas, sem o terreno. **Confirmar.**
- NE-2. Uma notícia além do alcance só chega se alguém da rede do jogador estiver dentro dele. **Confirmar.**
- NE-3. A notícia ganha a versão mais precisa quando o jogador ou a rede dele chega mais perto. **Confirmar.**
- NE-4. Preços fora da média: conferidos uma vez por dia; o mesmo item na mesma cidade só volta a ser notícia depois de 8 dias. **Confirmar.**
- NE-5. Limite de 500 notícias ativas no mundo. **Confirmar.**
- NE-6. No máximo 2 eventos negativos ativos ao mesmo tempo por cidade. **Confirmar.**
- NE-7. Quatro focos de narrador. **Já decidida** (V3-5).

**V5-7. Cidade, População, Felicidade e Cura (6 propostas).**

- CI-1. NPCs reais por nível de cidade (15, 40, 100 e 300) e os outros valores iniciais da Cidade. **Confirmar.**
- CI-2. A Senzala pertence ao dono dos escravos (cidade, Rico ou jogador). **Confirmar.**
- CI-3. A Felicidade anda 10% por dia em direção ao alvo, sem pular. **Confirmar.**
- CI-4. O Controle vai de 0 a 30. **Confirmar.**
- CI-5. Regeneração por hora, maior dormindo ou descansando; em bloco no LOD 2; só fora de combate. **Confirmar.**
- CI-6. O Hospital cura o grupo mediante pagamento; em LOD 2 é um serviço agregado da cidade. **Confirmar.** A V3-4 já deu os números.

**V5-8. Interface, Início, Objetivo, Companheiros, Comércio, Reputação e Tecnologia (8 propostas).**

- OU-1. A ordem de produção mostra os insumos que faltam e o tempo estimado; parada por falta de insumo, ela vai para a barra de alertas. **Confirmar.**
- OU-2. Padrão das notificações: tudo ligado, com agrupamento; acima de 4x, só Aviso e Crítico; o Crítico pausa. **Confirmar.**
- OU-3. O estilo de início é um conjunto de dados lido de arquivo. **Já decidida** (moldes da V4-8).
- OU-4. Classe típica de cada título (Plebeu = Civil, Cavaleiro = Rico, Nobre = Nobreza...). **Confirmar.**
- OU-5. Companheiros vinculados: 10 + Renome ÷ 5 + 3 por cidade do jogador. **Confirmar.**
- OU-6. Cada Cavalo dá +100 de capacidade de peso à caravana. **Confirmar.**
- OU-7. As Vilas Livres começam com reputação 0 para todos. **Confirmar.**
- OU-8. Pontos de Pesquisa por trabalhador por dia = Inteligência ÷ 50 × multiplicador do nível do Laboratório. **Confirmar** (a testar).

**V5-9. O formato da ficha técnica lógica da fatia vertical.**

- *Contexto:* depois da V5, a revisão termina e começa a **ficha técnica lógica**: o documento que traduz os sistemas em **estruturas, dados e regras** que o código vai seguir. Ela cobre só a fatia vertical, a vila de 15 NPCs da V2-12. O autor tem pouca experiência em C++.
- *Opções:*
  - (a) **um documento único**, só da fatia vertical, em linguagem simples, com tabelas e sem código de verdade;
  - (b) um documento por sistema, já para o jogo inteiro: mais completo, mas muito maior, e a maior parte não entra na fatia vertical;
  - (c) ir direto ao código, explicando enquanto programa: mais rápido, mas sem um mapa para consultar.
- *Sugestão:* (a), com estas partes:
  1. **Entidades e componentes:** o que existe (NPC, construção, cidade, item, pilha) e os campos de cada um, com o tipo: número inteiro, número com vírgula, texto, lista.
  2. **Arquivos de dados:** quais arquivos `data/` a fatia lê (itens, receitas, construções, eventos) e um exemplo de cada.
  3. **O ciclo do tempo:** o que roda a cada tick, a cada hora, a cada dia e a cada estação, e em que ordem (rotina → produção → consumo → mercado → Felicidade → eventos).
  4. **As regras como receitas de bolo:** cada fórmula em passos numerados ("1. some os salários; 2. ...").
  5. **Save:** o que é gravado.
  6. **Os testes:** o teste de 1 ano sem tela e o que ele mede.
  7. **Os marcos de programação**, na ordem da Arquitetura, com o que fica pronto em cada um.

  A ficha vai num documento novo, `Ficha Técnica/Fatia Vertical.md`, e também em rodadas de perguntas, quando aparecer uma escolha.
- *Impacto:* fecha a revisão; prepara o início do código.

Depois das respostas:

- tirar as marcações confirmadas e aplicar as mudanças (IT-4, IT-5);
- marcar a [[Revisão Completa]] como concluída;
- atualizar o [[Estado do Projeto]];
- começar a ficha técnica.

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
- **Pendências (V3):**
  - **leis iniciais:** numa tabela única na Facção; **só a Escravista permite narcóticos** e é a única produtora (ideia do autor);
  - **crime:** multa = 25 × gravidade²; Caçadores a partir de 500 de Recompensa;
  - **narradores:** 4 (Cronista, Guerra, Terra, Corte), com intensidade de 50% a 150%;
  - **fatia vertical:** 12 eventos;
  - **interface:** fonte Noto Sans;
  - **PC mínimo:** 4 núcleos, 8 GB, OpenGL 3.3 e 2 GB de vídeo;
  - **conteúdo:** **moldes de conteúdo** prontos e listas nas rodadas C1 a C3 (ideia do autor).
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
