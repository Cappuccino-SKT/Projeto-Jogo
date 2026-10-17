> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a revisão completa concluída (V5 aplicada) e a rodada FT1 (ficha técnica da fatia vertical) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas P0 e P1 estão escritos, e a **revisão completa terminou** (ver a [[Revisão Completa]]). No `main` estão as rodadas até a V3 (PRs #9 a #14). A V4 está na branch `docs/v4-propostas`, e a V5 na `docs/v5-ficha`, criada a partir dela. A V5 também trouxe a primeira versão da **ficha técnica lógica**: [[Ficha Técnica/Fatia Vertical|Ficha Técnica: Fatia Vertical]]. A **rodada FT1 (escolhas da ficha técnica) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se os pull requests da `docs/v4-propostas` e da `docs/v5-ficha` foram mesclados no `main`, nessa ordem.**
3. **Próximo passo:** pedir o `Respostas.txt` da FT1 (conferir se é desta rodada), atualizar a ficha, tirar as marcações confirmadas e ver com o autor se a ficha está pronta para o **marco 1** (a primeira linha de código).
4. Depois: a programação da fatia vertical, marco a marco. As listas de conteúdo ficam para as rodadas C1 a C3 (ideia do autor na V3-13).
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
| Ficha técnica | **FT1 (escolhas da ficha da fatia vertical)** | **Enviada, aguardando respostas** |
| Depois | Programação da fatia vertical; rodadas de conteúdo C1 a C3 | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, modo observador, eventos de dinastia. As ordens de produção em lote foram decididas na IP1-7 ([[Sistema de Produção/Descrição|Produção]]). A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: FT1, Ficha técnica da fatia vertical

**Perguntas enviadas em 2026-10-17, aguardando respostas.** A revisão completa terminou na V5. A primeira versão da **ficha técnica lógica** está em [[Ficha Técnica/Fatia Vertical|Ficha Técnica: Fatia Vertical]], no formato da V5-9. Esta rodada fecha as escolhas novas que a ficha precisou fazer.

Responder no `Respostas.txt` pelos códigos (FT1-1, FT1-2...). "Seguir sugestão" aceita a proposta.

**Correção avisada:** a V2-11 dizia "1 tick = 2 s de jogo". A conta certa é 4,5 s:

- a 1x, 1 segundo real equivale a 90 segundos de jogo;
- com 20 ticks por segundo, cada tick vale 90 ÷ 20 = 4,5 s.

O [[Sistema de Tempo/Descrição|Tempo]] e a ficha já estão corrigidos. Nada mais mudou: 1 hora de jogo continua sendo 40 s reais.

**FT1-1. O que o jogador faz na fatia vertical.**

- *Contexto:* a Arquitetura diz "1 personagem controlável", mas não o que ele faz. Isso decide quais sistemas a fatia testa de verdade.
- *Opções:*
  - (a) **só observar:** o personagem anda e compra no Mercado. É simples, mas não testa as ordens, os salários do jogador nem o Dono dos itens;
  - (b) **dono de uma construção:** além de andar e comprar, ele tem uma Carpintaria própria;
  - (c) **prefeito da vila:** controla todas as construções. Testa mais coisas, mas muda o jogo (no jogo completo, o jogador não manda na cidade da IA).
- *Sugestão:* (b). O jogador começa com **100 moedas** e uma **Carpintaria nível 1 própria** (2 vagas). Ele pode:
  - contratar desempregados;
  - escolher o nível de salário (Baixo, Normal, Alto);
  - dar ordens (Fazer N, Manter estoque, Repetir);
  - fixar o preço na faixa de ±30%;
  - andar, comprar e vender no Mercado.

  Isso testa o atributo Dono, a fila de ordens, os salários do jogador e a venda para os NPCs. Os Móveis de Madeira dele são os Materiais Básicos da vila.
- *Impacto:* ficha (componentes Controlado e Dono), Interface (painel da construção), Trabalho, Econômico.

**FT1-2. A Felicidade da fatia vertical.**

- *Contexto:* o [[Sistema de Felicidade e Estabilidade/Descrição|Felicidade]] tem 11 fatores, com faixas, mas sem as contas. A fatia não tem leis, crime, guerra nem religião.
- *Sugestão:* só **4 fatores**, dentro das faixas que já existem:

| Fator | Conta |
|---|---|
| Consumo atendido | −25 × a fração de NPCs sem comida no dia; −5 se os Materiais Básicos estão em déficit; até +5 pelo gasto extra |
| Desemprego e renda | +10 − 20 × a taxa de desemprego, limitado entre −10 e +10 |
| Moradia | −10 × a fração de NPCs sem casa |
| Eventos duradouros | A soma dos eventos ativos, limitada entre −20 e +20 |

  - Alvo = 50 + a soma; a Felicidade anda 10% por dia na direção do alvo.
  - *Exemplo:* comida para todos, Materiais em dia, 2 de 15 desempregados (13%), todos com casa, sem evento: 50 + 0 + 7,3 + 0 + 0 = 57,3.
  - Os outros fatores entram quando os sistemas deles entrarem.
- *Impacto:* ficha (regra 4.8), Felicidade, Eventos (Fome).

**FT1-3. Movimento e busca de caminho.**

- *Contexto:* a Arquitetura prevê grade espacial e busca de caminho, sem detalhe.
- *Opções:*
  - (a) **A\* em 8 direções** (diagonal com custo 1,4), um caminho calculado por destino. A\* é o método mais usado em jogos de grade e fácil de achar explicado;
  - (b) 4 direções: ainda mais simples, mas os NPCs andam em "L", o que fica feio;
  - (c) mapas de fluxo (*flow fields*): ótimos para milhares de soldados, mas é coisa para a batalha, não para a fatia.
- *Sugestão:* (a). Mais:
  - na fatia, os NPCs **não colidem** entre si (passam um pelo outro), para simplificar;
  - cada NPC calcula no máximo **1 caminho por destino**, e o cálculo é distribuído entre os ticks.
- *Impacto:* ficha (regra 4.10), Arquitetura, Combate (mais tarde).

**FT1-4. O mapa da fatia e a câmera.**

- *Contexto:* os tiles são de 16 × 16 px e os bonecos de 16 × 24 ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]); o autor joga em 1600 × 900. A vila de nível 1 tem 8 lotes pequenos e 3 médios.
- *Sugestão:*
  - **Mapa de 64 × 64 tiles** (1.024 × 1.024 px).
  - **Zoom em números inteiros:** 2x, 3x e 4x (padrão 3x). Um número quebrado borra a pixel art. A 3x, a tela mostra cerca de 33 × 19 tiles.
  - **Câmera** com WASD ou setas e roda do mouse para o zoom; arrastar com o botão do meio.
  - **Lotes:** pequeno de 4 × 4 tiles; médio de 6 × 6; grande de 8 × 8.
  - **Recursos no mapa:** um campo de Grão, um poço (Água) e um bosque (Tora).
- *Impacto:* ficha (Mapa Local), Pipeline (o mapa a desenhar no Tiled), Interface (controles).

**FT1-5. A ordem dos sistemas no dia.**

- *Contexto:* várias regras rodam uma vez por dia, e a ordem muda o resultado. Por exemplo, consumir antes ou depois de pagar o salário.
- *Sugestão:* a **virada do dia às 0h**, quando só o turno da noite está acordado. A ordem é a da ficha (parte 3):
  1. consumo;
  2. salários;
  3. Mercado Local;
  4. estabilizador;
  5. dias em déficit e Felicidade;
  6. eventos;
  7. IA da produção.

  A ordem vem de um arquivo de dados, para ser trocada nos testes sem mexer no código.
- *Impacto:* ficha (parte 3), Tempo.

**FT1-6. O teste de 1 ano sem tela.**

- *Contexto:* a V2-12 definiu o critério (sem fome, Banco sem zerar, Pão entre 1,2 e 2,6); falta como rodar e como ler.
- *Sugestão:*
  - **Comando:** `jogo --simular 64 --semente 1`. Ele roda 64 dias na velocidade máxima, sem janela, e grava `relatorio.csv`.
  - **Relatório:** uma linha por dia, com estoque de cada item, preço do Pão, Banco, riqueza média, Felicidade, empregados e eventos. O autor abre no LibreOffice e faz um gráfico.
  - **Resultado:** no fim, o jogo escreve PASSOU ou FALHOU, com o motivo ("fome por 4 dias seguidos no dia 37").
  - **Repetição:** a mesma semente tem que dar o mesmo relatório.
- *Impacto:* ficha (parte 6), Arquitetura (marco 7).

**FT1-7. O ambiente de programação.**

- *Contexto:* já estão instalados o g++, o CMake, o Ninja e o Git; o VS Code foi escolhido para os dados (P-3). O autor tem pouca experiência em C++, e o primeiro dia de código também aprova as bibliotecas (A-2).
- *Sugestão:*
  - **Editor:** **VS Code**, com as extensões grátis "C/C++" e "CMake Tools", que dão autocompletar, botão de compilar e depurador.
  - **Depurador:** o **gdb**, que vem com o MinGW. Ele deixa parar o programa numa linha e olhar os valores.
  - **Primeiro passo de código:** um "olá, janela" com o SDL3, compilado pelo CMake, para provar que tudo funciona. Nesse dia se aprovam as bibliotecas.
  - **Organização:** uma pasta `codigo/` no mesmo repositório, separada da documentação.
- *Impacto:* Arquitetura, Pipeline (ferramentas), Retomada (cuidados técnicos).

Depois das respostas:

- atualizar a ficha;
- tirar as marcações confirmadas;
- decidir se a ficha está pronta para começar o marco 1.

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
