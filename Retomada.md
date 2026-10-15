> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado ao aplicar a rodada P1-1 (Viagem) e abrir a P1-2 (2026-10-15).

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Rodada pendente: P1-2, Reputação e Relações do jogador** (o P0 está completo; a P1-1, Viagem, já foi aplicada). A lista de perguntas (seção 4) foi enviada ao autor e **ainda não foi respondida**. Ao retomar, pedir o `Respostas.txt` (ou as respostas no chat) e conferir se ele é desta rodada: os itens vão de A1 a D2 e falam de reputação, faixas e Opinião.
3. Depois da P1-2: os outros sistemas P1, na ordem da seção 3.
4. Quando o autor estiver satisfeito com a documentação: **ficha técnica lógica**, e só então a programação.

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
| P1 | **Reputação e Relações do jogador** | **Perguntas enviadas, sem resposta** |
| P1 | Companheiros, Relações familiares e Diálogo | A fazer |
| P1 | Guerra e Campanha | A fazer |
| P1 | Batalha (Mapa de Guerra) | A fazer |
| P1 | Diplomacia | A fazer |
| P1 | Religião | A fazer |
| P1 | Clima e Estações | A fazer |
| P1 | Notícias e Informação | A fazer |
| P1 | Efeitos de itens e Escravidão | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível, especialização de cidades, ordens de produção em lote, guarnição e defesa, modo observador, eventos de dinastia.

## 4. Rodada P1-2 pendente: Reputação e Relações do jogador

Objetivo: definir **o que cada facção e cada personagem importante pensa do jogador**, como isso muda e o que isso muda no jogo (preços, guardas, contratos, títulos). Hoje existem a **Recompensa** (crime, por facção) e o **Renome** (feitos, global, 0 a 100), mas falta a relação do jogador com as facções, que o Início de Jogo já usa ("relação inicial com as facções") e que a revolta do jogador também considera (ver 4.11 da [[Revisão do Projeto]]).

### A. Estrutura da reputação

**A1. Com quem o jogador tem reputação**
- *Contexto:* são 6 facções, 3 Vilas Livres, 19 cidades de facção e alguns NPCs Importantes (líderes, herdeiros, nobres e os 5 Importantes fixos).
- *Opções:*
  - (a) só **por facção**;
  - (b) por facção **e por cidade** (cada cidade com o seu valor);
  - (c) por facção (e por Vila Livre) **e opinião pessoal dos NPCs Importantes** que o jogador conheceu;
  - (d) tudo: facção, cidade e Importantes.
- *Sugestão:* **(c)**. Cada facção e cada Vila Livre tem um valor de **−100 a +100**; as cidades usam o valor da facção. Os NPCs Importantes têm uma **opinião pessoal**, também de −100 a +100, guardada só para quem o jogador já encontrou. É barato (algumas dezenas de números) e permite histórias como "a facção me odeia, mas o herdeiro é meu amigo". *Custo de (b) e (d):* 19 valores de cidade a mais, que confundem o jogador sem mudar muito o jogo.

**A2. Faixas de reputação**
- *Sugestão:* cinco faixas, com nome visível na interface:

| Faixa | Valor |
|---|---|
| Inimigo | −100 a −60 |
| Hostil | −59 a −20 |
| Neutro | −19 a +19 |
| Amigável | +20 a +59 |
| Aliado | +60 a +100 |

**A3. O que cada faixa muda**
- *Sugestão:*

| Faixa | Preços no Mercado | Guardas | Contratos e missões | Títulos da facção |
|---|---|---|---|---|
| Inimigo | Lojas se recusam a negociar | Mandam sair da cidade; quem fica vira alvo | Nenhum | Nenhum; perde o título que tiver |
| Hostil | +15% na compra, −15% na venda | Revistam na entrada (chance de achar itens proibidos) | Só os do quadro da Taverna | Nenhum |
| Neutro | Normal | Normal | Quadro da Taverna | Plebeu e Cidadão |
| Amigável | −5% na compra, +5% na venda | Normal | Missões de nobres | Cavaleiro (+20) e Nobre (+40) |
| Aliado | −10% na compra, +10% na venda | Ajudam o jogador em brigas | Missões do líder | Senhor de cidade (+60) |

- Os títulos continuam pedindo também **Renome** e o requisito concreto já definidos no [[Objetivo e Progressão/Descrição|Objetivo e Progressão]]; a reputação é um requisito a mais.
- *Exemplo:* o Mercador com Amigável na Escravista compra algodão 5% mais barato lá e, se cair para Hostil por contrabando, passa a pagar 15% a mais e a ser revistado na entrada.

**A4. Reputação e Recompensa: um valor ou dois?**
- *Contexto:* a Recompensa é a dívida legal de um crime, por facção; some quando é paga ou cumprida.
- *Opções:* (a) **juntar** tudo num único valor; (b) **manter separados**, mas ligados.
- *Sugestão:* **(b)**. A Recompensa diz "a lei quer você"; a reputação diz "o que pensam de você". Todo crime **detectado** também tira reputação: **−2 × a gravidade do crime** (um crime de gravidade 2 tira 4; um assassinato, de gravidade 5, tira 10). Pagar a Recompensa limpa a ficha, mas a reputação volta só aos poucos (ver B3).

### B. Como a reputação sobe e desce

**B1. Ações e valores**
- *Sugestão* (valores iniciais, a testar):

| Ação | Efeito na facção |
|---|---|
| Contrato da Taverna cumprido na cidade da facção | +2 |
| Missão de nobre cumprida | +5 |
| Missão longa (3 a 5 etapas) cumprida | +10 |
| Derrotar bandidos no território da facção | +1 por bando |
| Ajudar uma caravana em apuros | +2 com o dono |
| Lutar ao lado da facção numa batalha | +5 |
| Doar dinheiro à facção | +1 a cada 100 moedas, até +20 no total |
| Crime detectado | −2 × gravidade |
| Atacar caravana ou patrulha da facção | −10 |
| Lutar contra a facção numa batalha | −15 |
| Recusar o chamado às armas sendo vassalo | −10 |
| Libertar escravos (só nas facções que permitem escravidão) | −5 por grupo |

**B2. Efeito em cadeia entre facções**
- *Opções:* (a) **sem cadeia**: cada ação mexe só com quem foi afetado; (b) **cadeia na guerra**: ajudar uma facção numa guerra tira reputação do inimigo dela; (c) **cadeia total**: aliados e inimigos de quem você ajudou sempre mudam junto.
- *Sugestão:* **(b)**. Nas ações de guerra (batalha, saque, escolta de exército), o inimigo perde metade do que a facção ajudada ganhou. Nas outras ações, sem cadeia. *Exemplo:* lutar ao lado da Guerreira contra a Autoritária dá +5 na Guerreira e −2,5 na Autoritária. A cadeia total (c) pune o jogador por ações que ele nem sabia que tinham efeito.

**B3. A reputação volta sozinha com o tempo?**
- *Opções:* (a) não volta; (b) **volta devagar em direção à base**; (c) volta rápido.
- *Sugestão:* **(b)**: **2 pontos por estação** em direção ao valor base (ver B4). Um jogador que sumiu por um ano (4 estações) volta 8 pontos. Assim um erro antigo não dura para sempre, e uma amizade precisa de cuidado.

**B4. Valor inicial (base)**
- *Contexto:* o Início de Jogo já prevê a "relação inicial com as facções" por estilo.
- *Sugestão:* **base = afinidade da cultura do personagem + bônus do estilo de início**:

| Origem | Efeito na base |
|---|---|
| Facção da mesma cultura | +20 |
| Cultura afim | +10 |
| Cultura neutra | 0 |
| Cultura hostil | −15 |
| Estilo Membro de facção | +20 na própria facção |
| Estilo Nobre exilado | −30 na facção que derrubou a dinastia dele |
| Estilo Escravo | −20 com o dono e a facção dele |

*Exemplo:* um Mago de cultura Arcana começa com +20 na Arcana, 0 na Guerreira e −15 na Religiosa e na Tecnocrata.

### C. Opinião dos NPCs Importantes

**C1. Opinião pessoal: como funciona** (conflito de nome)
- *Contexto:* o NPC já tem um atributo chamado **Relação**, que é a lista de vínculos de família dos Importantes (filho, cônjuge).
- *Pergunta:* aceita chamar a nova medida de **Opinião**, para não confundir com o atributo Relação?
- *Sugestão:* a Opinião vai de −100 a +100 e existe só para os Importantes que o jogador já encontrou (líder, herdeiro, nobres, os 5 Importantes fixos e os companheiros). Ela muda por:
  - **conversa** (persuadir, elogiar, ameaçar), usando o Carisma (o sistema de Diálogo vem na rodada de Companheiros e Diálogo);
  - **presentes**: +1 a +10, conforme o valor do presente e a Ganância do NPC;
  - **missões** pedidas por ele: +10;
  - **ações contra a família ou a cidade dele**: −10 a −50.

**C2. O que a Opinião muda**
- *Sugestão:*
  - **Rito de Admissão por Indicação:** exige Opinião de pelo menos +40 de um nobre da facção.
  - **Decisões do líder sobre o jogador** (aceitar um pedido de título, responder a uma revolta, aceitar vassalagem): a Opinião do líder entra no cálculo junto com a reputação da facção.
  - **Herdeiro:** quando o líder morre, a Opinião do herdeiro passa a pesar nas decisões da facção.
  - **Companheiros:** a Opinião vira a lealdade deles (detalhada na rodada de Companheiros).

### D. Interface e dados

**D1. Onde o jogador vê a reputação**
- *Sugestão:* um painel de **Reputação** com a lista das facções e das Vilas Livres (valor, faixa e cor da faixa) e, para cada uma, o **"Por quê?"** com as 5 últimas mudanças ("+5: lutou ao lado da facção na batalha de (2,3), há 3 dias"). Uma **notificação** quando a faixa muda (por exemplo, de Neutro para Amigável). A Opinião aparece no painel do Importante, ao passar o mouse sobre ele.

**D2. Dados e save**
- *Sugestão:* as tabelas de faixas, efeitos e ações ficam em `reputacao.json` (para ajustar sem mexer no código). O save guarda o valor por facção e Vila Livre, a Opinião de cada Importante conhecido e o histórico das últimas mudanças, que alimenta o "Por quê?" e o diário.

Depois das respostas: escrever `Sistema de Reputação/Descrição.md`; atualizar Facção (revolta e vinculação), Crime (crime tira reputação), Objetivo (títulos), Início de Jogo (base por estilo), NPC (Opinião), Econômico (preço por faixa), Interface (painel) e Viagem (patrulhas e caravana em apuros); abrir a próxima rodada do P1 (Companheiros, Relações familiares e Diálogo).

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). O Ferreiro não lista Espada (provável erro; ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]).
- **Arte:** o autor desenha por enquanto (sem experiência em pixel art), no **Pixelorama** (grátis), com pacotes de terceiros provisórios na fatia vertical. Mapas no **Tiled** (explicar em linguagem simples). Sem verba para programas pagos. Projeto **sem prazo**: o limite da arte é o peso no jogo, não o tempo (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
- **Estações na arte:** só mudança de cor por enquanto; **toda arte extra de estação fica para o futuro** (neve, rios congelados, folhas, flores).
- **Viagem:** 4 h por Mapa Local a pé, multiplicada por terreno, infraestrutura, tamanho do grupo, montaria e carga; caravanas sempre reais no Mapa Mundial (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).
- **Linguagem:** o foco é **C++**; ferramentas em outras linguagens só como ajuda, nunca essenciais.
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
