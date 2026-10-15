> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado ao aplicar a rodada P1-2 (Reputação) e abrir a P1-3 (2026-10-15).

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Rodada pendente: P1-3, Companheiros, Relações familiares e Diálogo** (o P0 está completo; Viagem e Reputação já foram aplicadas). A lista de perguntas (seção 4) foi enviada ao autor e **ainda não foi respondida**. Ao retomar, pedir o `Respostas.txt` (ou as respostas no chat) e conferir se ele é desta rodada: os itens vão de A1 a D1 e falam de recrutamento, casamento e diálogo.
3. Depois da P1-3: os outros sistemas P1, na ordem da seção 3.
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
| P1 | Reputação e Relações do jogador | Escrito |
| P1 | **Companheiros, Relações familiares e Diálogo** | **Perguntas enviadas, sem resposta** |
| P1 | Guerra e Campanha | A fazer |
| P1 | Batalha (Mapa de Guerra) | A fazer |
| P1 | Diplomacia | A fazer |
| P1 | Religião | A fazer |
| P1 | Clima e Estações | A fazer |
| P1 | Notícias e Informação | A fazer |
| P1 | Efeitos de itens e Escravidão | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível, especialização de cidades, ordens de produção em lote, guarnição e defesa, modo observador, eventos de dinastia.

## 4. Rodada P1-3 pendente: Companheiros, Relações familiares e Diálogo

Objetivo: definir **como o jogador forma o grupo** (recrutar, pagar, manter leais), **como funcionam casamento, filhos e herdeiros**, e **como se conversa** com os NPCs. Hoje há peças soltas: o Renome "atrai companheiros melhores", a Felicidade dos Controlados (6 fatores, saída no 0), a Opinião (que vira lealdade), os atributos Grávida, Estágio e Relação, e o herdeiro que assume quando o personagem morre (ver 4.12 da [[Revisão do Projeto]]).

**Fatos que as perguntas usam:** 1 ano = 64 dias (cerca de 17 h reais a 1x); 1 estação = 16 dias; criança de 0 a 3 anos, adulto de 4 a 20, velho a partir de 21, morte natural entre 21 e 30 anos; combate local de cerca de 40 combatentes; derrota = captura.

### A. Companheiros

**A1. Onde e como recrutar**
- *Opções:* (a) só na **Taverna**; (b) Taverna e **encontros** (refugiados, prisioneiros libertados, escravos comprados ou libertados); (c) Taverna, encontros e **NPCs Importantes** convencidos por missão ou conversa.
- *Sugestão:* **(c)**:
  - **Taverna:** cada cidade tem de 1 a 4 recrutáveis (mais em cidades maiores), trocados a cada estação.
  - **Encontros:** refugiados e prisioneiros libertados podem aceitar entrar no grupo de graça, com salário baixo.
  - **Importantes:** alguns aceitam depois de uma missão pessoal, se a Opinião for de pelo menos +40.
  - **O Renome define a qualidade de quem aceita:** Renome 0 a 19, só companheiros comuns; 20 a 49, também os bons; 50 ou mais, também os excelentes. "Qualidade" é a soma das habilidades na geração do NPC.
- *Exemplo:* com Renome 25, a Taverna da Cidade Guerreira mostra um arqueiro bom e dois lanceiros comuns; um mago excelente ali recusaria ("não sigo desconhecidos").

**A2. Quantos companheiros no grupo**
- *Contexto:* o combate local aguenta cerca de 40 combatentes, somando os dois lados.
- *Opções:* (a) limite fixo de 8; (b) **limite que cresce com o Renome**: 4 + Renome ÷ 20 (4 no começo, 9 com Renome 100); (c) sem limite, só o salário segura.
- *Sugestão:* **(b)**. O começo fica pequeno e pessoal, e o crescimento vira recompensa. Companheiros que **ficam numa cidade** como governador, gerente ou comandante de Esquadrão **não contam** no limite.

**A3. Salário e contratação**
- *Sugestão* (valores a testar, por estação):

| Qualidade | Taxa de entrada | Salário por estação |
|---|---|---|
| Comum | 100 moedas | 50 |
| Bom | 300 | 150 |
| Excelente | 1.000 | 400 |

- O salário sai do dinheiro do jogador no início de cada estação. **Salário atrasado** derruba o fator "salário" da Felicidade (ver [[Sistema de Felicidade e Estabilidade/Descrição|Felicidade]]); com 2 estações sem pagar, o companheiro sai.
- Escravos do jogador não recebem salário, mas têm Felicidade baixa e tendência a fugir.

**A4. Lealdade: Felicidade e Opinião juntas**
- *Contexto:* a **Felicidade** do Controlado é o humor do momento (6 fatores); a **Opinião** é o que ele pensa do jogador a longo prazo (decidida na Reputação).
- *Sugestão:*

| Opinião | Efeito |
|---|---|
| +60 ou mais | Fiel: com Felicidade 0, ainda fica 1 estação inteira antes de pedir para sair |
| +20 a +59 | Normal: pede para sair com Felicidade 0, como já decidido |
| −19 a +19 | Instável: a Felicidade cai 25% mais rápido |
| −20 a −59 | Pede aumento de salário a cada estação |
| −60 ou menos | Pode desertar à noite, levando o próprio equipamento |

- O traço **Lealdade** da Personalidade muda a velocidade com que a Opinião sobe e desce.
- A Opinião do companheiro sobe com vitórias juntos, salário em dia, presentes e missões pessoais; cai com companheiros mortos, ordens contra a Personalidade dele (por exemplo, mandar um não agressivo massacrar civis) e salário atrasado.

**A5. Morte de companheiro**
- *Opções:* (a) **morte permanente** (como Kenshi); (b) só ferido, volta depois de algum tempo.
- *Sugestão:* **(a)**. Dá peso às decisões e liga ao fator "companheiros mortos" da Felicidade. O companheiro **Inconsciente** ainda pode ser salvo (ver [[Sistema de Cura/Descrição|Cura]]) e o **capturado** pode ser resgatado ou comprado de volta.

**A6. O que um companheiro pode fazer fora do grupo**
- *Sugestão:* cargos que usam as habilidades dele:

| Cargo | Habilidade principal | Efeito |
|---|---|---|
| Governador da cidade do jogador | Inteligência e Carisma | Bônus de Controle e de imposto |
| Gerente de construção do jogador | Trabalho Especialista | Bônus de produção |
| Líder de caravana | Carisma | Preço melhor na venda |
| Comandante de Esquadrão | Corpo a Corpo ou Longo Alcance | Moral e eficiência no Mapa de Guerra |
| Professor (na Universidade) | A habilidade mais alta dele | Treina outro companheiro |

### B. Relações familiares

**B1. Casamento**
- *Sugestão:*
  - **Quem:** o personagem pode casar com um NPC Importante ou um companheiro.
  - **Requisitos:** Opinião de pelo menos +60; título compatível (Nobreza e Realeza só casam com quem tem título de Cavaleiro ou acima); o costume da cultura (por exemplo, a Escravista pede um **dote** em dinheiro, a Guerreira pede um **duelo** vencido, a Religiosa pede uma **cerimônia** no Templo).
  - **Efeitos:** o cônjuge vira Controlado (pode entrar no grupo ou cuidar da casa); +20 de reputação com a facção da família dele; casar com alguém da dinastia de uma facção é um dos Ritos de Admissão já previstos.
- *Exemplo:* o Nobre exilado casa com a herdeira da Autoritária: entra na dinastia, ganha +20 na facção e uma chance real de "Retomar o trono".

**B2. Filhos e educação**
- *Contexto:* os atributos Grávida e Estágio já existem só para Importantes e Controlados.
- *Sugestão:*
  - **Gravidez:** dura **1 estação** (16 dias, cerca de 4,3 h reais a 1x). Só depois do casamento, com chance por estação.
  - **Infância:** 3 anos de jogo (cerca de 51 h reais a 1x). A criança é um NPC que fica na casa da família.
  - **Educação:** o jogador escolhe um foco (**Combate, Magia, Comércio ou Governo**); ao virar adulto, as habilidades daquele foco nascem no topo da faixa da cultura e classe. A Magia só pode ser escolhida se a criança for Maga.
  - Aos 4 anos, o filho vira adulto e pode entrar no grupo como companheiro (sem salário, Opinião inicial +60).

**B3. Se o personagem morrer**
- *Contexto:* já está decidido que o herdeiro assume como Controlado. E se não houver herdeiro?
- *Opções:* (a) a partida acaba, com um epílogo; (b) o jogador escolhe um **companheiro com Opinião de +60 ou mais** como sucessor; (c) o jogador sempre continua com qualquer companheiro.
- *Sugestão:* **(b)**, e só se ninguém cumprir isso a partida acaba, com o epílogo da Crônica. Mantém a ideia de "sem Game Over comum" (a derrota leva à captura) e dá valor à família e à lealdade. O sucessor herda o dinheiro, as propriedades e o título (se a lei de sucessão da facção permitir).

**B4. Famílias dos NPCs Importantes da IA**
- *Sugestão:* as dinastias da IA também casam e têm filhos, com as mesmas regras, mas resolvidas **uma vez por estação** (sorteio de casamento e de nascimento), sem simulação no dia a dia. Os **casamentos entre dinastias** são eventos diplomáticos (relação +20 entre as facções) e notícia. Isso garante que toda facção tenha herdeiros e cria crises de sucessão de verdade.

### C. Diálogo

**C1. Formato dos diálogos**
- *Opções:*
  - (a) **Árvores escritas à mão**, como nos RPGs clássicos: muito texto, inviável para uma pessoa só em centenas de NPCs.
  - (b) **Modular por modelos de texto:** frases prontas com espaços ({nome}, {cidade}, {item}), e o tom muda conforme a Personalidade e a cultura do NPC.
  - (c) **Misto:** modular nas conversas comuns e escrito à mão nos Importantes fixos e nas missões longas.
- *Sugestão:* **(c)**. *Exemplo modular:* um guarda Guerreiro agressivo diz "Fala logo, {título}. O que quer em {cidade}?"; um mercador Escravista ganancioso diz "Ah, {nome}! Tenho {item} pelo melhor preço de {região}."

**C2. Ações de conversa e chance de sucesso**
- *Sugestão:* ações por menu:

| Ação | O que faz | Comparado com |
|---|---|---|
| Conversar | Rumores e notícias | Sempre funciona |
| Perguntar sobre | Preços, masmorras, contratos, caminhos | Sempre funciona; a resposta pode estar velha |
| Persuadir | Pedir um favor, desconto ou informação | Carisma do jogador × Opinião do NPC |
| Barganhar | Melhorar o preço de uma venda | Carisma × Ganância |
| Intimidar | Forçar algo | Força e Renome × Coragem |
| Subornar | Pagar por algo | Dinheiro oferecido × Ganância |
| Elogiar ou presentear | Subir a Opinião | Presente × Ganância |
| Recrutar, pedir casamento, aceitar missão | Ações especiais | Regras próprias (A1, B1) |

- **Chance** (ações com teste) = 50 + (habilidade do jogador − 50) ÷ 2 + Opinião ÷ 4 ± o traço de Personalidade do NPC, limitada entre 5% e 95%.
- *Exemplo:* Carisma 70 (+10) e Opinião +20 (+5) contra um NPC de Ganância alta (−10 para persuadir) dão 55%.

**C3. O que acontece numa falha**
- *Sugestão:* Persuadir ou Barganhar falhos: −5 de Opinião. Intimidar falho: −10 de Opinião e o NPC pode atacar (se tiver Agressividade alta). Subornar um guarda e falhar: **crime de suborno** (a acrescentar no [[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]) e tentativa de prisão.

**C4. Quem tem conversa completa**
- *Sugestão:* **Importantes e companheiros** têm o menu completo. **NPCs comuns** têm um menu curto: comprar e vender (se for comerciante), um rumor, pedir caminho e as ações de crime (furtar, ameaçar). Isso reduz texto e processamento.

**C5. Onde ficam os textos**
- *Sugestão:* em `dialogos.json`, com **IDs de texto** (já previstos na [[Interface/Descrição|Interface]] para traduções futuras) e **frases por cultura** (cada cultura tem cumprimentos e despedidas próprios). As conversas escritas à mão dos Importantes ficam em arquivos separados, um por personagem.

### D. Interface da conversa

**D1. Retratos dos personagens**
- *Contexto:* regra do autor: toda arte extra fica para o futuro.
- *Opções:* (a) **sem retrato:** a janela mostra o boneco de 16 × 24 ampliado 4x, com nome, título e Opinião; (b) retratos montados por camadas (rosto, cabelo e roupa desenhados à mão e combinados); (c) retrato desenhado para cada Importante.
- *Sugestão:* **(a)** nas primeiras versões; **(b)** fica para o futuro, junto com a arte extra. A janela de conversa fica na parte de baixo da tela, móvel como o resto do HUD.

Depois das respostas: escrever `Sistema de Companheiros e Diálogo/Descrição.md`; atualizar NPC, Felicidade (lealdade), Facção (dinastia e sucessão), População (gravidez e infância), Objetivo (recrutamento e Renome), Crime (suborno), Cultura (costumes de casamento), Interface (janela de conversa) e Reputação (Opinião); abrir a próxima rodada do P1 (Guerra, Batalha e Diplomacia).

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
