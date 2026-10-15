> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado ao fechar o P0 (rodada P0-6) e abrir a P1-1 (2026-10-15).

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Rodada pendente: P1-1, Viagem e Mundo** (primeira do P1; o P0 está completo). A lista de perguntas (seção 4) foi enviada ao autor e **ainda não foi respondida**. Ao retomar, pedir o `Respostas.txt` (ou as respostas no chat) e conferir se ele é desta rodada: os itens vão de A1 a E2 e falam de viagem, encontros e névoa.
3. Depois da P1-1: os outros sistemas P1, na ordem da seção 3.
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
| P1 | **Viagem e Mundo** (entidades em trânsito, encontros, névoa) | **Perguntas enviadas, sem resposta** |
| P1 | Reputação e Relações do jogador | A fazer |
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

## 4. Rodada P1-1 pendente: Viagem e Mundo

Objetivo: definir **como o grupo do jogador, as caravanas, os exércitos e os bandos se movem pelo Mapa Mundial**, quanto tempo leva, o que acontece no caminho e o que o jogador sabe do mundo (névoa). Resolve a inconsistência das "entidades em trânsito" (ver 4.7 da [[Revisão do Projeto]]). É a base de Guerra, Comércio e Notícias.

**Fatos que as perguntas usam:** 24 h de jogo = 16 min reais a 1x (1 hora de jogo = 40 s reais); 8x só no Mapa Mundial; Mapa Local de 128 × 128 tiles; malha de 8 × 6 mapas; combate local de até cerca de 40 combatentes; derrota = captura, sem Game Over.

### A. Movimento no Mapa Mundial

**A1. Como uma entidade anda no Mapa Mundial**
- *Contexto:* o Mapa Mundial é uma malha de 48 quadrados, cada um um Mapa Local. Ele não guarda dados de jogo, só a lista de entidades em trânsito.
- *Opções:*
  - (a) **De mapa em mapa (grafo):** a posição é "mapa atual + próximo mapa + progresso de 0 a 100%". Na tela, um ícone desliza do centro de um quadrado ao centro do outro. Muito barato: cada entidade é só um contador.
  - (b) **Posição livre** sobre a imagem do mundo (como Mount & Blade): mais bonito, mas exige busca de caminho sobre a imagem e colisão entre entidades.
- *Sugestão:* **(a)**. Com 48 mapas, a rota é uma busca simples entre quadrados, e centenas de entidades custam quase nada. *Exemplo:* uma caravana sai da Cidade Escravista (7,5) para a Capital Autoritária (1,4): a rota passa por 6 ou 7 mapas, e o ícone dela anda quadrado a quadrado.

**A2. Quanto tempo leva atravessar um Mapa Local**
- *Contexto:* dentro de um Mapa Local, em LOD 0, o boneco anda de verdade. Com velocidade de cerca de 4 tiles por segundo real, atravessar 128 tiles leva cerca de 32 s reais, quase 1 hora de jogo. No Mapa Mundial o tempo pode ser outro: ele representa estradas, descanso e terreno.
- *Opções* (tempo por Mapa Local, a pé):
  - (a) **1 hora:** coerente com andar no Mapa Local, mas o mundo fica minúsculo: cruzar o mundo de leste a oeste (8 mapas) leva 8 horas, e um exército chega a qualquer lugar no mesmo dia.
  - (b) **4 horas:** cruzar o mundo leva cerca de 32 horas (1,3 dia). A 8x, cada mapa passa em cerca de 20 s reais, e cruzar o mundo leva menos de 3 minutos reais.
  - (c) **8 horas:** cruzar o mundo leva 2,7 dias; as distâncias pesam bastante (comércio, guerra, notícias), mas viajar fica lento.
- *Sugestão:* **(b) 4 horas**. Dá tempo para a distância importar (uma campanha de guerra leva alguns dias de uma estação de 16) sem deixar a viagem cansativa. A diferença para o andar no Mapa Local (1 hora) é aceita: andar até a borda é mais rápido, mas exige atenção; a viagem pelo Mapa Mundial é automática e pode correr a 8x.
- *Impacto:* Comércio (viagens por estação), Guerra (tempo de marcha), Notícias (atraso das informações).

**A3. De onde se pode começar uma viagem pelo Mapa Mundial**
- *Contexto:* já está decidido que, ao chegar, o grupo reaparece no ponto de entrada de Viagem Rápida do destino.
- *Opções:* (a) **de qualquer lugar**: abrir o Mapa Mundial e clicar no destino; (b) só **saindo pela borda** ou por um ponto de viagem (portão da cidade, estrada).
- *Sugestão:* **(a)**, com duas travas: não pode haver combate e nenhum inimigo dentro do raio de alerta (como em Kenshi e Mount & Blade). Ao partir, o grupo some do Mapa Local e vira um ícone no Mapa Mundial. Também vale viajar para um mapa vizinho; nesse caso, chega pelo ponto de entrada, e não pela borda.

**A4. Terreno e estradas mudam a velocidade?**
- *Contexto:* cada região tem um bioma, e os mapas desenhados no Tiled já podem ter estradas.
- *Sugestão:* sim, com um **custo de travessia** por bioma e um bônus de estrada:

| Terreno | Multiplicador do tempo |
|---|---|
| Planície, litoral | × 1,0 |
| Floresta, colinas, terras áridas | × 1,25 |
| Montanha | × 1,5 |
| Com estrada ligando as cidades | × 0,75 sobre o valor acima |

- A rota escolhe o caminho mais rápido, não o mais curto. *Exemplo:* da Capital Guerreira (1,1), na montanha, ir até (2,2) leva 6 h a pé (4 × 1,5); com estrada, 4,5 h.
- *Custo:* desprezível (busca de caminho em 48 quadrados). As estradas são marcadas no `mapas.json`.

### B. Encontros no caminho

**B1. Como acontece um encontro durante a viagem do jogador**
- *Contexto:* os bandidos ficam em mapas sem dono ou de fronteira, com tamanho ligado à Criminalidade da cidade mais próxima; em LOD 2 viram uma "ameaça bandida" por região ([[Sistema de Eventos/Descrição|Eventos]]).
- *Sugestão:*
  - A cada Mapa Local atravessado, sorteia-se um encontro. Chance base: **Território 5%, Em disputa 15%, Sem dono 20%**, mais a ameaça bandida da região.
  - **Os bandidos avaliam a força:** se o grupo do jogador for mais de 2 vezes mais forte, eles não atacam (como em Mount & Blade). Assim um grupo forte não é interrompido a toda hora.
  - **No encontro:** o jogo **pausa** e mostra uma janela com as opções **Lutar**, **Fugir**, **Negociar ou subornar** e **Render-se**.
  - **Lutar:** carrega aquele Mapa Local em LOD 0, com o grupo no ponto onde estava e os inimigos por perto; é o combate local normal.
  - **Fugir:** chance pela diferença de velocidade (montaria ajuda); se falhar, vira luta.
  - **Render-se:** é a captura que já existe no [[Sistema de Combate/Descrição|Combate]].
- *Exemplo:* o Mercador atravessa o mapa em disputa (6,4) com 4 guardas; sai 15% + ameaça; aparecem 8 bandidos, que se acham mais fortes e atacam; o jogador suborna com 200 moedas e segue.

**B2. Encontros das entidades da IA, longe do jogador**
- *Contexto:* as guerras de IA já são resolvidas por um cálculo rápido de forças, sem gerar o Mapa de Guerra.
- *Sugestão:* usar o **mesmo cálculo rápido** para caravana × bandidos e patrulha × bandidos: compara a força dos dois lados, com um pouco de sorte. A caravana que perde fica com **30% a 100% de perda da carga** (quanto maior a diferença, maior a perda); o resultado gera notícia (futuro sistema de Notícias) e pode criar um contrato de escolta.

**B3. Encontros que não são de luta**
- *Sugestão:* uma lista curta, sorteada junto com os encontros hostis:

| Encontro | O que acontece |
|---|---|
| Mercador viajante | Compra e venda ali mesmo, com preços de outra região |
| Patrulha da facção | Se o jogador tiver Recompensa nessa facção, tenta prender; se não, segue |
| Refugiados | Fogem de guerra ou fome; podem pedir comida ou virar recrutas |
| Peregrinos | Indo à Capital Religiosa; trazem rumores |
| Caravana em apuros | Sendo atacada; ajudar dá dinheiro e relação com o dono |

- *Impacto:* liga a Viagem ao Crime (Recompensa), à Reputação (P1, futuro) e às Notícias (rumores).

### C. Entidades em trânsito

**C1. Quais entidades andam no Mapa Mundial**
- *Sugestão:* grupo do jogador; caravanas civis; exércitos (Esquadrões com a Caravana Militar); bandos de bandidos; **patrulhas** das facções (andam pelo próprio território, caçam bandidos e cobram Tarifa).
- *Dados de cada uma:* tipo, dono, posição (mapa atual, próximo mapa e progresso), destino e rota, velocidade, força (para o cálculo rápido), carga e grupo de NPCs que ela representa.
- *Pergunta:* aceita as patrulhas? Elas dão vida ao mundo e reduzem bandidos, mas custam soldo do Banco da Facção.

**C2. O que acontece quando uma entidade passa por um Mapa Local**
- *Contexto:* em LOD 2, o mapa é só dado; em LOD 0, tudo é simulado tile a tile.
- *Sugestão:*
  - **Mapa em LOD 2:** a entidade só passa; o mapa registra o efeito (Tarifa cobrada, comércio feito, ameaça bandida que atacou).
  - **Mapa em LOD 0 ou 1** (com o jogador presente): a entidade **aparece de verdade** na borda, na posição proporcional, com seus NPCs (a caravana com os animais e os guardas), atravessa o mapa pela estrada e sai pela borda oposta. Enquanto isso, o progresso dela no Mapa Mundial fica parado. O jogador pode conversar, comerciar ou atacar.
- *Exemplo:* o jogador está na Cidade Religiosa (7,3) e vê chegar pela borda leste a caravana da Escravista; pode comprar dela antes que chegue ao Mercado.

**C3. Caravanas da IA: sempre reais no Mapa Mundial?** (conflito com uma decisão anterior)
- *Contexto:* o [[Sistema de Comércio/Descrição|Comércio]] decidiu que, em LOD 2, o comércio vira um fluxo calculado por estação, sem caravana individual, para não pesar.
- *Opções:*
  - (a) **Manter o fluxo**: longe do jogador não existe caravana; perto dele, ela "nasce" do fluxo. Mais barato, mas o jogador não pode emboscar uma caravana longe, nem seguir uma de uma ponta à outra.
  - (b) **Todas as caravanas são entidades reais** no Mapa Mundial, o tempo todo; só a simulação tile a tile fica para o LOD 0. Com o movimento de mapa em mapa (A1), 50 a 100 caravanas custam quase nada.
- *Sugestão:* **(b)**. O que pesava era a simulação tile a tile, não o ícone no Mapa Mundial. Fica mais coerente: toda caravana pode ser seguida, emboscada e protegida, e os preços mudam quando ela chega de verdade. Se aceita, o texto do Comércio muda.

### D. Névoa e exploração

**D1. O que o jogador vê do Mapa Mundial**
- *Opções:* (a) tudo visível desde o início (como Mount & Blade); (b) **névoa por Mapa Local**: explorado ou não; (c) névoa e **informação que envelhece**: o que você viu fica registrado com a data ("visto há 12 dias").
- *Sugestão:* **(b) com (c)**:
  - **Não explorado:** só o contorno do quadrado e o bioma aproximado.
  - **Explorado** (visitado ou atravessado): terreno, cidade, nível, dono e recursos, guardados com a data em que foram vistos.
  - **Visão atual:** só o mapa onde o grupo está e os 4 vizinhos mostram as entidades em trânsito ao vivo; o resto mostra a última informação.
  - **Conhecido desde o início:** as 6 capitais (são famosas) e os mapas vizinhos ao ponto de partida.

**D2. Névoa dentro do Mapa Local e do Mapa Interno**
- *Opções:* (a) sem névoa dentro do Mapa Local; (b) névoa também dentro (como as áreas escuras de muitos jogos de estratégia).
- *Sugestão:* **(a) nos Mapas Locais e névoa só nas masmorras**: as salas ainda não visitadas ficam escuras, o que dá tensão à exploração e custa pouco.

**D3. Conhecimento que vem do estilo de início e dos mapas comprados**
- *Sugestão:*

| Fonte | O que revela |
|---|---|
| Membro de facção | Todo o território da própria facção |
| Mercador | Todas as cidades com Posto Comercial e os preços delas no primeiro dia |
| Aventureiro | As masmorras não lendárias da região de partida |
| Mapa comprado (Mercado ou mercador viajante) | Uma região inteira, com as informações datadas do dia da compra |

### E. Viagem e outros sistemas

**E1. Comida, sono e noite durante a viagem**
- *Contexto:* os NPCs têm necessidades; a viagem pelo Mapa Mundial é abstrata.
- *Opções:* (a) viagem sem parar, 24 h por dia; (b) **parada automática à noite**: acampamento de 8 h (das 22 h às 6 h), com comida tirada do inventário do grupo; (c) parada automática, com **marcha forçada** opcional.
- *Sugestão:* **(c)**. Por padrão, o grupo acampa à noite e come uma refeição por pessoa por dia. Com a marcha forçada, viaja sem parar, mas a cada noite sem dormir a Felicidade do grupo cai e o risco de encontro sobe (cansaço). Sem comida, cai a Vida aos poucos e a Felicidade rápido. *Exemplo:* uma viagem de 5 mapas a pé (20 h) termina no segundo dia, com uma noite de acampamento.

**E2. Velocidade do grupo**
- *Sugestão:* o grupo anda na velocidade do **membro mais lento**, com estes multiplicadores sobre o tempo de travessia:

| Situação | Multiplicador do tempo |
|---|---|
| A pé | × 1,0 |
| Todos montados (Cavalo e Sela) | × 0,67 (50% mais rápido) |
| Carga acima do limite | × 1,25 |
| Caravana | × 1,33 |
| Exército | × 1,5, mais × 1,25 se tiver mais de 20 Esquadrões |
| Neve e chuva forte | definidos depois, no Clima e Estações |

- Durante a viagem a 8x, qualquer encontro ou chegada **volta para 1x**, como já acontece no combate.

Depois das respostas: escrever `Sistema de Viagem/Descrição.md`; atualizar Mapa (entidades em trânsito), Comércio (C3), Eventos (encontros), Interface (névoa), Combate (montaria), Tempo e Arquitetura (LOD das entidades); abrir a próxima rodada do P1 (Reputação e Relações do jogador).

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). O Ferreiro não lista Espada (provável erro; ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]).
- **Arte:** o autor desenha por enquanto (sem experiência em pixel art), no **Pixelorama** (grátis), com pacotes de terceiros provisórios na fatia vertical. Mapas no **Tiled** (explicar em linguagem simples). Sem verba para programas pagos. Projeto **sem prazo**: o limite da arte é o peso no jogo, não o tempo (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
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
