> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a ficha técnica da fatia vertical pronta (FT1 aplicada) e a rodada FT2 (começar a programar) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas estão escritos, e a **revisão completa terminou**. No `main` estão as rodadas até a V5 (PRs #9 a #16). Em 2026-10-17 foi aplicada a **FT1**, na branch `docs/ft1-ficha`: com ela, a [[Ficha Técnica/Fatia Vertical|Ficha Técnica: Fatia Vertical]] ficou completa para os primeiros marcos. A **rodada FT2 (começar a programar) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se o pull request da `docs/ft1-ficha` foi mesclado no `main`.**
3. **Próximo passo:** pedir o `Respostas.txt` da FT2. Se o autor liberar o código, atualizar o `CLAUDE.md` e começar o **marco 0** (ambiente e "olá, janela"), numa branch própria.
4. Depois: os marcos 1 a 9 da fatia vertical, um por vez. As listas de conteúdo ficam para as rodadas C1 a C3.
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
| Revisão | V5 (propostas, parte 2) | Aplicada; **revisão concluída** |
| Ficha técnica | FT1 (escolhas da ficha da fatia vertical) | Aplicada; ficha pronta |
| Código | **FT2 (liberar o código, bibliotecas, jeito de trabalhar, idioma)** | **Enviada, aguardando respostas** |
| Depois | Programação da fatia vertical; rodadas de conteúdo C1 a C3 | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, modo observador, eventos de dinastia. As ordens de produção em lote foram decididas na IP1-7 ([[Sistema de Produção/Descrição|Produção]]). A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: FT2, Começar a programar

**Perguntas enviadas em 2026-10-17, aguardando respostas.** Com a FT1, a [[Ficha Técnica/Fatia Vertical|Ficha Técnica: Fatia Vertical]] tem tudo o que o marco 1 precisa. Até aqui, a regra do projeto foi "só documentação" (decisão do autor). Esta rodada decide **se e como** o código começa.

Responder no `Respostas.txt` pelos códigos (FT2-1, FT2-2...). "Seguir sugestão" aceita a proposta.

**FT2-1. Liberar o código.**

- *Contexto:* o autor decidiu que, primeiro, viria a documentação, depois a ficha técnica lógica e só então a programação. A documentação e a revisão estão completas, e a ficha da fatia vertical também.
- *Opções:*
  - (a) **liberar o marco 0 e o marco 1 agora**. A documentação continua viva: o que o código mostrar volta para os documentos como rodada de perguntas;
  - (b) fazer antes as rodadas de conteúdo C1 a C3 (tecnologias, feitiços, missões). Nenhuma delas entra na fatia vertical;
  - (c) escrever a ficha técnica do jogo inteiro antes de programar: muito trabalho, sem saber ainda o que a fatia vai ensinar.
- *Sugestão:* (a). A fatia vertical existe justamente para testar as ideias antes de escrever o resto. A regra do `CLAUDE.md` passa de "só documentação" para "**documentação primeiro, código marco a marco**".
- *Impacto:* `CLAUDE.md`, Retomada, Arquitetura.

**FT2-2. Aprovar as bibliotecas (a A-2 adiada na V4-6).**

- *Contexto:* a [[Arquitetura/Descrição|Arquitetura]] propõe 8 bibliotecas, todas grátis, de código aberto e baixadas pelo próprio CMake: SDL3, OpenGL 3.3 (via glad), stb_image, miniaudio, Dear ImGui, EnTT, nlohmann/json e Tracy.
- *Sugestão:* aprovar todas, mas instalar **cada uma no marco em que for usada**, para o projeto crescer aos poucos:

| Marco | Bibliotecas | Para quê |
|---|---|---|
| 0 e 1 | SDL3, glad (OpenGL 3.3), stb_image | Janela, desenho, carregar os tiles |
| 3 | nlohmann/json (o CSV é lido por código próprio) | Arquivos de dados |
| 4 e 5 | EnTT | Entidades e componentes dos NPCs |
| 7 | Tracy | Medir o tempo do tick no teste sem tela |
| 8 | Dear ImGui | Painéis de depuração |
| Depois da fatia | miniaudio | Som (a fatia não tem áudio) |

- *Impacto:* Arquitetura; o arquivo `CMakeLists.txt` do código.

**FT2-3. Como trabalhamos no código.**

- *Contexto:* o autor tem pouca experiência em C++ e nenhuma com bibliotecas. O jeito das rodadas (perguntas, respostas, PR) funciona bem na documentação.
- *Sugestão:* o mesmo ritmo, adaptado:
  1. **Um marco por vez**, numa branch e num pull request próprios, como as rodadas.
  2. O assistente escreve o código em **passos pequenos**, cada um compilando e rodando.
  3. O autor compila e roda no PC dele. O assistente dá os comandos prontos, e o autor confirma o que viu ("a janela abriu").
  4. Cada marco termina com um **resumo em linguagem simples**: o que cada arquivo faz e o que testar. O resumo fica em `codigo/LEIA-ME.md`.
  5. Quando o código mostrar um problema de design, ele vira uma pergunta numa rodada, e os documentos são atualizados.
- *Impacto:* Retomada (jeito de trabalhar), Arquitetura.

**FT2-4. O idioma do código.**

- *Contexto:* os documentos e os IDs dos dados estão em português (`item.pao`, `construcao.cozinheiro`). As bibliotecas e os exemplos da internet estão em inglês.
- *Opções:*
  - (a) **nomes em português, sem acento** (`Cidade`, `Construcao`, `precoBase`), com comentários em português. Combina com os documentos e os dados, e o autor lê melhor. A mistura com o inglês das bibliotecas fica visível, o que até ajuda a saber o que é nosso;
  - (b) tudo em inglês, o padrão da indústria, mas cada nome exigiria tradução mental para o autor;
  - (c) nomes em inglês e comentários em português.
- *Sugestão:* (a). Os nomes técnicos de C++ continuam como são (`int`, `vector`, `for`), porque não têm tradução.
- *Impacto:* Arquitetura (convenções), todo o código.

Depois das respostas:

- atualizar o `CLAUDE.md` e a Retomada;
- começar o **marco 0**: o ambiente e um "olá, janela" com o SDL3.

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
- **Propostas (V4 e V5):** todas confirmadas. Fundar facção custa 2.000 moedas; dados em JSON e CSV; marco da simulação sem tela; capacidade das construções de 100, 200 ou 400 × nível; Saciedade pelo Tier.
- **Ficha técnica:** [[Ficha Técnica/Fatia Vertical|Ficha Técnica: Fatia Vertical]], com entidades e componentes, arquivos de dados, ciclo do tempo, regras passo a passo, save, testes e marcos. **1 tick = 4,5 s de jogo** (correção da V2-11); 1 hora = 800 ticks.
- **Ficha (FT1):**
  - **jogador:** 100 moedas e uma Carpintaria própria;
  - **Felicidade:** 4 fatores;
  - **movimento:** A\* em 8 direções, sem colisão entre NPCs;
  - **mapa e câmera:** mapa de 64 × 64 tiles, zoom de 2x a 4x;
  - **dia e teste:** virada do dia às 0h; teste com `jogo --simular 64 --semente 1`;
  - **ambiente:** VS Code com C/C++ e CMake Tools, e o gdb.
- **Mundo:** 48 Mapas Locais (8 × 6), 6 regiões de 8 mapas; partida no Ano 412; nomes significativos até a história ser escrita (ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- **Git:** o repositório é `FelipeSKT/Projeto-Jogo`. Cada rodada vira um pull request para o autor aceitar.

## 6. Cuidados técnicos (para quem edita)

- **Ambiente de código** (FT1-7, quando o código começar): VS Code com as extensões "C/C++" e "CMake Tools"; depurador gdb; pasta `codigo/` no mesmo repositório.
- **Ferramentas do PC:** g++ 15.2, CMake, Ninja, Git e Python 3.11. **Não há** `gh` (GitHub CLI) nem Visual Studio. O Python precisa ser chamado com `-I` ao ler arquivos do vault.
- **Edição em lote:** usar um script Python que carrega cada arquivo, troca trechos exatos com **verificação de que o trecho existe uma única vez**, e só então grava (com modo de teste "dry-run" antes). Preservar as quebras de linha (CRLF ou LF) e o BOM de cada arquivo. Backup da pasta antes de edições grandes (a pasta é um repositório Git, mas o backup extra é barato).
- **Conferir links:** o vault não deve ter `[[link]]` quebrado. Em tabelas, o separador do alias precisa ser escapado: `[[Pasta/Descrição\|Texto]]`.
- **Console do Windows:** a saída do PowerShell mostra acentos corrompidos; é só na tela, os arquivos estão em UTF-8.
- **Git:** o primeiro commit desta sessão falhou por causa da mensagem em *here-string*; usar `git commit -F arquivo`. Rodar `git fetch` antes de editar, porque outras sessões podem ter mexido no repositório. Não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json` (o Obsidian os altera sozinho). O `.gitignore` já ignora o workspace do Obsidian.
- **Sessões na nuvem (claude.ai/code):** o `Respostas.txt` chega como anexo na conversa; trabalhar na branch indicada pela sessão. O assistente só cria o pull request se o autor pedir.
- **Pull request (no PC):** sem o `gh`, o assistente envia a branch e o autor abre o PR pelo link `https://github.com/FelipeSKT/Projeto-Jogo/pull/new/NOME-DA-BRANCH`. O navegador interno não está logado no GitHub e o assistente nunca digita senhas. A extensão Claude in Chrome pode permitir criar o PR, se estiver conectada.
- **Nomes de itens:** a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] manda sobre os canvases.
