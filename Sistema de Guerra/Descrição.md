> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Guerra e Campanha

**Descrição geral**

Define a guerra **no Mapa Mundial**: por que uma guerra começa, como os exércitos se formam, se abastecem e se movem, como as cidades são cercadas, ocupadas e saqueadas, como se mede quem está ganhando e como a paz é feita. A luta em si acontece de dois jeitos:

- **Cálculo rápido de forças**, quando o jogador não participa (e quando ele escolhe "Resolver automaticamente");
- **Mapa de Guerra**, a batalha em tempo real quando o jogador participa (ver [[Sistema de Batalha/Descrição|Sistema de Batalha]]).

As relações entre as facções, os tratados e as decisões da IA estão no [[Sistema de Diplomacia/Descrição|Sistema de Diplomacia]].

Já estava decidido e vale como base:

- 1 Esquadrão = 100 soldados = 1 NPC alistado no Quartel; soldo pelo Banco da Facção; manutenção de comida (Ração Militar) e de equipamento; baixas permanentes; dissolução gera NPCs Escravo e Civil ([[Sistema de Facção/Descrição|Facção]]).
- Os exércitos andam de mapa em mapa no Mapa Mundial, mais devagar quanto maiores (× 1,5 a × 2,0); uma campanha cabe numa estação ([[Sistema de Viagem/Descrição|Viagem]]).
- Guardas não são soldados e nunca são alistados.

Os valores numéricos são iniciais e ficam em dados (`guerra.json`) para ajustar nos testes.

---

## Motivos de guerra

Uma facção pode declarar guerra **com ou sem motivo**, mas sem motivo custa caro. Cada motivo libera um **objetivo de guerra**:

| Motivo | Quando existe | Objetivo que libera |
|---|---|---|
| Mapa em disputa | Sempre, entre as facções que o disputam | Dominar o mapa |
| Reivindicação | A cidade foi da facção nos últimos 2 anos, ou herança por casamento (ver [[Sistema de Diplomacia/Descrição\|Diplomacia]]) | Tomar a cidade |
| Guerra santa | Religiosa contra cultura hostil, ou facção Religiosa contra um líder sob Anátema (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) | Tomar cidade ou impor tributo |
| Honra | Guerreira ofendida (duelo recusado, refém executado, tratado quebrado contra ela) | Tributo |
| Ofensa | Tratado quebrado, refém executado, crime grave contra a dinastia | Indenização ou tributo |
| Independência | Cidade ou vassalo que rompe com o suserano | Independência |
| Chamado de aliado | Aliado em guerra pede ajuda | O mesmo do aliado |

**Guerra sem motivo:** −20 de relação com todas as facções em contato, −20 de Confiabilidade (ver Diplomacia) e o Cansaço de guerra já começa em 20. *Exemplo:* a Autoritária ataca a Tecnocrata (afim, +40) sem motivo; todos a veem como agressora, e a própria população se cansa antes.

Contra uma facção de atitude **Hostil** (relação −31 ou menos), a guerra sem motivo custa a metade.

O motivo aparece no anúncio da guerra e no "Por quê?" (ver [[Sistema de Eventos/Descrição|Eventos]]).

## Placar de Guerra

Cada guerra tem um **Placar de −100 a +100**, visto pelos dois lados: o +30 de um é o −30 do outro.

| Acontecimento | Pontos para quem fez |
|---|---|
| Batalha vencida | +2 a +15, proporcional aos Esquadrões inimigos destruídos ÷ total do exército inimigo |
| Cidade ocupada (enquanto ocupada) | Vilarejo +10, Cidade +20, Capital +35 |
| Mapa em disputa dominado | +10 |
| Objetivo de guerra mantido | +1 a cada 4 dias |
| Saque de cidade | +5 |
| Saque de mapa sem cidade | +2 |
| Caravana Militar capturada | +3 |

O objetivo é escolhido na declaração e sai do motivo. A paz é negociada com o Placar (ver "Tratado de paz").

*Exemplo:* Guerreira × Autoritária pelo mapa (1,2). A Guerreira vence 2 batalhas (+8 e +12) e domina o mapa (+10): Placar +30, subindo 1 a cada 4 dias. A Autoritária, com Cansaço 60, aceita ceder o mapa (custo 10).

### Mapa em disputa

O estado de um Mapa Local é derivado das cidades vizinhas (ver [[Sistema de Mapa/Descrição|Mapa]]). O **mapa em disputa** é a exceção: ele **guarda quem o domina**.

- **Domínio:** vencer a última batalha no mapa e ficar 4 dias sem oposição. Vale +10 no Placar.
- O mapa só **muda de dono** no tratado de paz ("Ceder mapa em disputa"). A partir daí ele conta como território do vencedor até uma nova guerra.
- Isso substitui o "limiar de controle" que estava em aberto na Facção.

## Exércitos

### Composição

- **Exército** = 1 a **20 Esquadrões** + Caravanas Militares + **1 General**. 20 Esquadrões = 2.000 soldados, o mesmo teto da tabela de Viagem e do limite por lado no campo de batalha.
- **General:** um NPC Importante (líder, herdeiro, nobre, companheiro ou o jogador).
- Sem General, os Esquadrões só ficam de **guarnição** numa cidade: somam ao Controle (ver [[Sistema de Felicidade e Estabilidade/Descrição|Felicidade e Estabilidade]]) e defendem no cerco.
- **Comando do General** = média de Carisma e Inteligência. Dá até **+10 de Moral inicial** a todo o exército (Comando 50 → +5).

### Formação de Esquadrões

- 1 conscrito + o **kit** do tipo de Esquadrão (ver [[Sistema de Batalha/Descrição|Batalha]]) + **2 dias** no Quartel.
- O Quartel forma, ao mesmo tempo, tantos Esquadrões quanto o nível dele (nível 3 = 3 ao mesmo tempo).

### Ordens no Mapa Mundial

Mover, atacar exército, cercar cidade, saquear, guarnecer, seguir exército aliado e voltar para casa.

### Mercenários

- **6 a 8 companhias** no mundo, de **2 a 5 Esquadrões** cada, com sede nas Vilas Livres e em cidades de nível 3+.
- **Preço:** 2× o soldo normal por estação, pago adiantado.
- Não tiram população de nenhuma cidade; repõem baixas devagar (1 Esquadrão por estação).
- **Vão embora** se o pagamento atrasa ou se o Placar do contratante fica abaixo de −50.
- Trabalham para facções de IA (ricas e em guerra) e para o jogador.
- O jogador dono de cidade, mesmo independente, também usa o próprio Quartel e a própria Conscrição.
- Uma companhia sem contrato fica na sede e aparece nas Notícias ("Companhia X procura contrato na Vila Livre do Norte").

## Suprimento

Mesma escala do Fator: **1 unidade abastece 100 soldados**, ou seja, um Esquadrão.

| Consumo por Esquadrão | Quanto |
|---|---|
| Ração Militar | 1 por dia |
| Manutenção de equipamento | 1 material a cada 4 dias (Barra de Ferro para tropas pesadas, Couro para leves) |
| Munição (tropas de tiro) | 1 unidade do tipo por batalha |
| Pedra de Mana (Esquadrão de Magos) | 1 por batalha |

- **Caravana Militar:** carrega **60 unidades**. Sem caravana, o exército leva 3 dias de ração. *Exemplo:* 10 Esquadrões com 2 Caravanas = 12 dias de ração, quase uma estação de campanha.
- **Em território próprio ou aliado:** a Caravana reabastece sozinha ao passar por cidade com Mercado (pago pelo Banco da Facção).
- **Em território inimigo, forrageio:** o exército tira até **50% do consumo** do próprio mapa (fazendas e pastos), o que reduz a produção ali e a Felicidade da cidade daquele mapa.
- **Comboio:** a Caravana pode ir e voltar sozinha até a cidade amiga mais próxima. Ela é uma entidade em trânsito (ver [[Sistema de Viagem/Descrição|Viagem]]) e pode ser emboscada. É a "linha de suprimento" sem um sistema novo.
- **Campanha de inverno:** fora de cidade amiga, cada Esquadrão gasta 1,25 Ração por dia; nos biomas frios, Moral −1 por dia (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Sem suprimento:** Moral −5 por dia; abaixo de 30, deserta **1 Esquadrão a cada 2 dias** (o de menor Moral).
- **Caravana capturada ou destruída:** o exército perde a carga, e quem capturou leva o que sobrou (+3 no Placar).

A alta do preço da Ração Militar em tempo de guerra **nasce da simulação** (mais consumo, menos trabalhadores); o evento só explica a causa.

## Cerco

Um exército hostil parado no Mapa Local de uma cidade **inicia o cerco**:

1. Param o comércio, a migração e as caravanas da cidade.
2. A cidade consome o estoque de comida sem repor. Quando acaba: **fome** (Felicidade e mortalidade, ver [[Sistema de População/Descrição|População]]) e Moral da guarnição −10 por dia.
3. **Sem Muralha:** o atacante pode atacar já no 1º dia.
4. **Com Muralha:** precisa de **Aríete** (o exército monta em 2 dias) para o portão, ou de **brecha** feita pela Artilharia ou por Magos de Combate.
5. **Rendição:** quando a Moral da guarnição chega a 0, ou depois de 8 dias de fome.
6. **Assalto:** batalha de cerco no Mapa de Guerra (ver [[Sistema de Batalha/Descrição|Batalha]]) ou cálculo rápido com o bônus da Muralha.

*Exemplo:* a Guerreira cerca o Vilarejo Autoritário (1,3), nível 2, com Paliçada e 10 dias de comida. Monta o Aríete em 2 dias e ataca, ou espera cerca de 18 dias pela rendição.

Um exército inimigo que chega ao mapa **levanta o cerco** se vencer a batalha contra os sitiantes.

## Ocupação e conquista

Uma cidade tomada fica **ocupada** até a paz; só o tratado muda a facção dela.

- **Ocupada:** a bandeira muda e o imposto vai para o ocupante, mas as leis e o dono legal continuam. Felicidade −20 (fator "ocupação"); a guarnição do ocupante dá o Controle.
- **Posse de fato:** ocupada por mais de **2 anos** sem paz, passa ao ocupante.
- **Ao mudar de dono:**
  - adota as leis da nova facção (herança total);
  - a população mantém a cultura, e a Cultura dominante da facção muda aos poucos, por estação ([[Sistema de Cultura/Descrição|Cultura]]);
  - Felicidade −15 "conquistada", que some em 1 ano;
  - os nobres antigos viram exilados.
- **Cidade tomada pelo jogador a serviço de uma facção:** vai para o líder. O jogador pode pedi-la; o líder aceita se a Opinião sobre o jogador for **+30 ou mais** e o Renome for alto (como em Mount & Blade). Renome 40 ou mais.

Uma cidade pertence a uma facção "por conquista" quando a conquista é confirmada no tratado ou pela posse de fato.

## Saque

Ao tomar uma cidade, o vencedor escolhe:

| Escolha | Efeito |
|---|---|
| **Ocupar** (padrão) | Ver "Ocupação e conquista" |
| **Saquear** | Leva 50% do dinheiro e 30% dos estoques da cidade; mata 10% da população; Felicidade −30 por 1 ano; pode cair 1 nível; Moral do exército +10; Reputação −10 com o dono; Placar +5 |
| **Saquear e ocupar** | Os dois, com Felicidade −40 |

- **Mapas sem cidade** também podem ser saqueados: produção do mapa −50% por 1 estação, pouco dinheiro, Placar +2.
- **Cultura:** a Guerreira e a Escravista não se ofendem com saque; a Religiosa condena (−5 de relação de todas as facções Religiosas com o saqueador).
- O saque é uma ação de guerra para a [[Sistema de Reputação/Descrição|Reputação]] (vale a cadeia de metade para o inimigo).
- O evento "Cidade saqueada pela Facção X" guarda a causa no "Por quê?".

## Cansaço de guerra

**Cansaço de 0 a 100** por facção, somando todas as guerras dela.

| Sobe | Quanto |
|---|---|
| Cada 4 dias em guerra | +1 |
| Cada 2 Esquadrões perdidos | +1 |
| Cada cidade própria ocupada, por estação | +5 |
| Cada estação com comércio bloqueado pela guerra | +5 |
| Cada estação de campanha no inverno | +5 |
| Guerra declarada sem motivo | começa em 20 |

- **Efeito na Felicidade:** fator "Guerra e ameaça" = **−Cansaço × 0,15** (Cansaço 100 → −15, o limite que já existe).
- **Cansaço 60+:** a IA aceita paz pior. **80+:** assembleia extraordinária pedindo paz. **100:** aceita paz branca.
- **Cai** 10 por estação em paz.
- **Cultura:** a Guerreira se cansa pela metade.

## Tratado de paz

Cada termo tem um custo; o vencedor exige termos até o total do Placar.

| Termo | Custo |
|---|---|
| Paz branca (ninguém ganha nada) | 0 |
| Libertar prisioneiros e devolver reféns | 5 |
| Ceder mapa em disputa | 10 |
| Romper aliança com terceiro | 10 |
| Ceder cidade ocupada | Vilarejo 15, Cidade 30, Capital 50 |
| Tributo: 10% da receita do Banco por 2 anos | 15 |
| Indenização única | 1 ponto = 1% do Banco do perdedor, até 30 |
| Independência de cidade ou vassalo | 30 |
| Vassalagem (ver [[Sistema de Diplomacia/Descrição\|Diplomacia]]) | 60 |

- Só se cede cidade que esteja **ocupada**.
- **Trégua obrigatória de 1 ano** entre os dois. Quebrar a trégua: −50 de relação com todos e −30 de Confiabilidade.
- A IA aceita se o Placar contra ela cobre o custo, ou se o Cansaço dela é alto.
- *Exemplo:* com Placar +45, o vencedor exige "Ceder Cidade (30) + Tributo (15)".
- Numa guerra com aliados, cada lado negocia pelo seu líder de guerra (quem declarou ou quem foi atacado); os aliados saem junto.

## Defesa de cidade

| Construção | Efeito |
|---|---|
| **Paliçada** (Muralha nível 1, madeira, Era 1) | Exige Aríete ou brecha; defensor × 1,3 no cálculo rápido |
| **Muralha de Pedra** (nível 2, Era 1, cidade nível 3+) | Defensor × 1,6 |
| **Bastião** (nível 3, Era 3) | Resiste à artilharia: a brecha leva o dobro do tempo; defensor × 2,0 |
| **Posto da Guarda** | Mais vagas de guarda (mais Controle) e ponto de reunião da milícia |

- Custo e manutenção da Muralha em Madeira e Pedra. **[Refinar depois]** valores, junto com a lista de construções novas ([[Sistema de Construção/Descrição|Construção]]).
- **Milícia:** no assalto, cada guarda (NPC real) vira **1 Esquadrão de Milícia** (fraco, equipamento leve) e volta a ser guarda depois, se sobreviver. Assim os guardas continuam fora da guerra de campo: só defendem a própria cidade.
- **Convocação de emergência:** o dono da cidade pode armar até **5% da população adulta** como Milícia; isso tira trabalhadores enquanto dura.
- **Guarnição:** Esquadrões estacionados (ver "Exércitos").
- **Arte:** um tileset de Muralha (paliçada genérica; pedra com paleta por cultura), pintado no Mapa Local da cidade (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).

## Cálculo rápido de forças

Usado nas guerras de IA, nos encontros de caravanas e patrulhas (ver [[Sistema de Viagem/Descrição|Viagem]]) e quando o jogador escolhe "Resolver automaticamente". Usa **as mesmas grandezas da batalha** (ver [[Sistema de Batalha/Descrição|Batalha]]), resolvidas em rodadas:

1. **Força de um Esquadrão** = (Soldados ÷ 100) × (Ataque + Defesa) × (Moral ÷ 100) × bônus de Experiência × vantagem do tipo contra o exército inimigo.
2. **Força do exército** = soma × Comando do General (até × 1,1) × Muralha, só para quem defende (Paliçada × 1,3; Pedra × 1,6; Bastião × 2,0) × suprimento (× 0,7 se faltar).
3. **Magos:** os de Combate valem Força × 2; os de Suporte reduzem em 15% as perdas do próprio exército.
4. **Até 5 rodadas.** Em cada uma, cada lado perde soldados = força inimiga × 0,08 × sorte (0,8 a 1,2), espalhados pelos Esquadrões; a Moral cai com as perdas, com as mesmas regras da batalha.
5. Quem fica com **Moral média abaixo de 25** foge. A perseguição e a divisão das baixas (mortos, feridos e capturados) seguem a Batalha.

- **Calibração:** nos testes, o jogo roda centenas de batalhas do Mapa de Guerra **sem tela** (só a simulação) e compara com o cálculo rápido; os coeficientes ficam em JSON.
- A comparação de forças aparece como texto ("muito superior", "superior", "equilibrado", "inferior", "muito inferior") na pré-batalha e na lente Perigo do Mapa Mundial ([[Interface/Descrição|Interface]]).
- **Quando acontece:** dois exércitos hostis no mesmo Mapa Local; um exército que ataca uma cidade sitiada; uma patrulha ou caravana que encontra bandidos.

## O jogador na guerra

| Situação | O que o jogador faz |
|---|---|
| Aventureiro sem facção | Entra como **mercenário** (contrato de uma batalha ou de uma estação), com a Comitiva e companhias contratadas |
| Membro (nobre) | O líder pode lhe entregar um exército, se Renome e Opinião deixarem; senão, luta com a Comitiva junto do exército da facção |
| Vassalo | **Chamado às armas:** manda 30% dos Esquadrões ou vai pessoalmente; recusar dá −10 de reputação |
| Líder de facção | Controla tudo; pode **delegar** exércitos a generais da IA com ordens simples (atacar alvo, defender região, seguir) |
| Passando perto | Se uma batalha de IA acontece no mapa do jogador ou num vizinho, ele pode entrar por um dos lados, o que abre o Mapa de Guerra |

Batalha sem o jogador é sempre cálculo rápido. A Comitiva (o grupo do jogador dentro da batalha) está no [[Sistema de Batalha/Descrição|Sistema de Batalha]].

## Muralha, Posto da Guarda e estradas (rodada V3-9)

| Nível da Muralha | Porte | Custo |
|---|---|---|
| Paliçada | Médio | 60 Tábuas |
| Muralha de Pedra | Grande | 200 Blocos de Pedra + 40 Tábuas |
| Bastião (Era 3) | Grande | 400 Blocos de Pedra + 40 Barras de Ferro |

- A manutenção segue a regra geral da [[Sistema de Construção/Descrição|Construção]].
- O **Posto da Guarda** é de porte pequeno.
- O saque e o cerco danificam a estrada do mapa ([[Sistema de Viagem/Descrição|Viagem]]).
- A **guerra civil** na crise de sucessão está na [[Sistema de Facção/Descrição|Facção]] (rodada V3-7).

---

## Parte técnica

- **Dados** (`data/guerra/`), em JSON:
  - `guerra.json`: motivos, pontos do Placar, Cansaço, termos de paz e custos, cerco, saque, Muralhas e coeficientes do cálculo rápido;
  - `mercenarios.json`: companhias, sedes, tamanho e preço.
- **Guerra** (registro): ID, lados (facções e aliados), motivo, objetivo, Placar, data de início, cidades ocupadas, prisioneiros e o "Por quê?" de cada mudança.
- **Exército** (entidade em trânsito da Viagem): General, lista de Esquadrões, Caravanas, ordem atual, suprimento e Moral média.
- **Frequência:** o Placar e o Cansaço são atualizados a cada 4 dias de jogo e a cada batalha; o cerco avança 1 vez por dia; o cálculo rápido roda na hora do encontro.
- **LOD:** a guerra vive no Mapa Mundial e nas facções, sem depender do LOD dos Mapas Locais. A cidade sitiada em LOD 2 calcula o estoque e a fome por dia.
- **Aleatoriedade com semente** (a sorte do cálculo rápido), como no resto do jogo.
- **Save:** guerras ativas, tréguas, exércitos, cercos, cidades ocupadas e datas, mapas em disputa dominados, Cansaço de cada facção e contratos de mercenários.
- **Depuração:** painel (Dear ImGui) para forçar uma guerra, ver o Placar e rodar um cálculo rápido com os detalhes de cada rodada.
- **Fatia vertical:** não tem guerra.
- **Ordem de programação:** primeiro a guerra por **cálculo rápido**, que já deixa o mundo com guerras e permite ao jogador resolver as dele automaticamente; o Mapa de Guerra vem depois, como um marco próprio (ver [[Arquitetura/Descrição|Arquitetura]]).

## Ligação com outros sistemas

| Sistema | O que a Guerra fornece ou recebe |
|---|---|
| Batalha | Mapa de Guerra quando o jogador participa; mesmas grandezas do cálculo rápido; batalha de cerco |
| Diplomacia | Estados de Guerra e Trégua; motivos; tratado de paz; aliados chamados; Confiabilidade |
| Facção | Esquadrões, Quartel, soldo, Banco da Facção, conquista e posse de cidades; mapa em disputa guarda o dominante |
| Viagem | Exércitos, comboios e mercenários como entidades em trânsito; cálculo rápido nos encontros |
| Mapa | Domínio do mapa em disputa; cerco no Mapa Local da cidade |
| Felicidade e Estabilidade | Fator "Guerra e ameaça" pelo Cansaço; ocupação, conquista, saque e cerco; guarnição soma ao Controle |
| População | Fome no cerco; mortes no saque; queda de nível; migração bloqueada |
| Escravidão | Prisioneiros escravizados ou vendidos a facções que permitem; cidade conquistada por quem proíbe liberta os escravos (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) |
| Produção e Comércio | Ração Militar, munição e manutenção; forrageio; comércio bloqueado no cerco e na guerra |
| Construção | Muralha (3 níveis), Posto da Guarda, Quartel |
| Reputação | Saque, batalha e escolta como ações de guerra, com cadeia de metade |
| Companheiros | Companheiro como General ou comandante; pedido de cidade conquistada pela Opinião |
| Cultura | Guerra santa, Honra, Guerreira cansa menos, reação ao saque |
| Eventos | Declaração, cerco, saque, rendição e paz como eventos com causa registrada |
| Interface | Painel de guerra (Placar, Cansaço, exércitos), lente Perigo, ordens no Mapa Mundial |
| Clima e Estações | Campanha de inverno mais cara; Moral −1 por dia no frio; IA prefere a primavera e o verão (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Religião | Anátema do Sumo Sacerdote libera a Guerra santa (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| Notícias | Declarações, batalhas, cercos e paz viram notícia, com atraso (ver [[Sistema de Notícias/Descrição\|Notícias]]) |

## Perguntas em aberto

1. **Valores a testar:** pontos do Placar, Cansaço, consumo e capacidade da Caravana, tempo de cerco, efeitos do saque e coeficientes do cálculo rápido.
2. **Muralha e Posto da Guarda:** custos decididos na V3-9.
3. **Estradas:** o saque e o cerco as danificam (rodada V3-9).
4. **Guerra civil:** decidida na V3-7 (ver Facção).
