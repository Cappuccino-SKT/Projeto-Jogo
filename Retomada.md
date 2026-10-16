> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-15, com a rodada P1-4 enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** a **rodada P1-4 (Guerra, Batalha e Diplomacia) foi enviada e aguarda respostas** (seção 4). Antes dela, a sessão de 2026-10-15 aplicou cinco rodadas (P0-5 Mundo, P0-6 Pipeline de arte, P1-1 Viagem, P1-2 Reputação e P1-3 Companheiros e Diálogo), fechou o P0, criou o `CLAUDE.md` e o padrão de perguntas detalhadas (seção 2) e registrou a ideia futura dos Títulos de Nobreza.
3. **Próximo passo:** pedir o `Respostas.txt` da P1-4 (conferir se é desta rodada), escrever os documentos de Guerra, Batalha e Diplomacia e abrir a próxima rodada.
4. Depois: os outros sistemas P1, na ordem da seção 3.
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
| P1 | **Guerra e Campanha** | **Rodada P1-4 enviada, aguardando respostas** |
| P1 | **Batalha (Mapa de Guerra)** | **Rodada P1-4 enviada, aguardando respostas** |
| P1 | **Diplomacia** | **Rodada P1-4 enviada, aguardando respostas** |
| P1 | Religião | A fazer |
| P1 | Clima e Estações | A fazer |
| P1 | Notícias e Informação | A fazer |
| P1 | Efeitos de itens e Escravidão | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível, especialização de cidades, ordens de produção em lote, guarnição e defesa, modo observador, eventos de dinastia.

## 4. Rodada pendente: P1-4, Guerra, Batalha e Diplomacia

**Perguntas enviadas em 2026-10-15, aguardando respostas.** A rodada junta três sistemas da [[Revisão do Projeto]] (4.8, 4.9 e 4.10), porque um depende do outro. Responder no `Respostas.txt` pelos códigos (A1, B2...). "Seguir sugestão" aceita a proposta. Valores numéricos são iniciais, a testar.

**Já decidido (base das perguntas):**

- 1 Esquadrão = 100 soldados = 1 NPC alistado; formação no Quartel; soldo pelo Banco da Facção; Caravana Militar com Ração Militar e manutenção; sem suprimento a Moral cai e há deserção ([[Sistema de Facção/Descrição|Facção]]).
- Mapa de Guerra só quando o jogador participa; guerras de IA pelo cálculo rápido; mapas em disputa geram guerras constantes; o Mapa de Guerra herda o terreno do Mapa Local ([[Sistema de Mapa/Descrição|Mapa]]).
- Exércitos andam no Mapa Mundial com multiplicador de tamanho de 1,5 a 2,0; uma campanha cabe numa estação ([[Sistema de Viagem/Descrição|Viagem]]).
- Esquadrões iniciais: Guerreira 20, Autoritária 16, Escravista 14, Religiosa 13, Tecnocrata 11, Arcana 10; 1 ou 2 guerras iniciais ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- Lutar ao lado de uma facção dá +5 e contra dá −15, com cadeia só na guerra; recusar o chamado às armas sendo vassalo dá −10 ([[Sistema de Reputação/Descrição|Reputação]]).
- Companheiro como Comandante de Esquadrão; casamento entre dinastias dá +20 de relação ([[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]]).
- Combate local até cerca de 40 combatentes; arma × armadura até ±25% ([[Sistema de Combate/Descrição|Combate]]).
- Só líderes e Importantes viram prisioneiros individuais; soldados comuns podem ser escravizados em massa se a lei permitir ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).
- Canhão de Guerra (Rank 3) e Moderno (Rank 5) são Armamento de Exército; Esquadrão de Magos é caro e limitado pela lei de Magia ([[Sistema de Itens/Descrição|Itens]], [[Sistema de Magia/Descrição|Magia]]).

### A. Guerra e Campanha

**A1. Precisa de motivo para declarar guerra?**

- *Contexto:* hoje as guerras nascem dos mapas em disputa, do sorteio inicial e da revolta do jogador. A Religiosa tem "guerra santa contra culturas hostis" e a Guerreira vê a "guerra como honra". Não há regra para declarar guerra.
- *Pergunta:* uma facção (ou o jogador líder) pode declarar guerra a qualquer momento? O que muda se não tiver motivo?
- *Opções:* (a) livre, sem motivo: simples, mas a IA fica caótica; (b) só com motivo: previsível, mas engessa o jogador; (c) o motivo dá desconto: sem motivo pode, mas custa caro.
- *Sugestão:* (c). Cada motivo libera um objetivo de guerra (A2):

| Motivo | Quando existe | Objetivo que libera |
|---|---|---|
| Mapa em disputa | Sempre, entre as facções que o disputam | Dominar o mapa |
| Reivindicação | A cidade foi da facção nos últimos 2 anos, ou herança por casamento (C4) | Tomar a cidade |
| Guerra santa | Religiosa contra cultura hostil | Tomar cidade ou impor tributo |
| Honra | Guerreira ofendida (duelo recusado, refém executado, tratado quebrado contra ela) | Tributo |
| Ofensa | Tratado quebrado, refém executado, crime grave contra a dinastia | Indenização ou tributo |
| Independência | Cidade ou vassalo que rompe com o suserano | Independência |
| Chamado de aliado | Aliado em guerra pede ajuda | O mesmo do aliado |

  Sem motivo: −20 de relação com todas as facções em contato, −20 de Confiabilidade (C2) e o Cansaço de guerra (A9) já começa em 20. *Exemplo:* a Autoritária ataca a Tecnocrata (afim, +40) sem motivo; todos a veem como agressora e a população dela se cansa antes.
- *Impacto:* Diplomacia (a IA pesa o motivo), Felicidade, Notícias (o motivo aparece no anúncio da guerra).

**A2. Como se mede quem está ganhando e quando a guerra acaba?**

- *Contexto:* nenhum documento diz como uma guerra termina. A Facção propõe um "controle" do mapa em disputa, sem limiar definido.
- *Opções:* (a) até um lado ser destruído: épico, mas guerras eternas e facções sumindo; (b) **Placar de Guerra** (estilo Europa Universalis e Crusader Kings): cada vitória dá pontos, e a paz "custa" pontos; (c) só pelo cansaço: quem cansa primeiro pede paz.
- *Sugestão:* (b), com o Cansaço (A9) como freio. Placar de −100 a +100 por guerra; o +30 de um lado é o −30 do outro.

| Acontecimento | Pontos para quem fez |
|---|---|
| Batalha vencida | +2 a +15, proporcional aos Esquadrões inimigos destruídos ÷ total do exército inimigo |
| Cidade ocupada (enquanto ocupada) | Vilarejo +10, Cidade +20, Capital +35 |
| Mapa em disputa dominado | +10 |
| Objetivo de guerra mantido | +1 a cada 4 dias |
| Saque | +5 |
| Caravana Militar capturada | +3 |

  *Exemplo:* Guerreira × Autoritária pelo mapa (1,2). A Guerreira vence 2 batalhas (+8 e +12) e domina o mapa (+10): Placar +30, subindo 1 a cada 4 dias. A Autoritária, com Cansaço 60, aceita ceder o mapa (custo 10, ver A10).
- *Conflito:* o mapa em disputa é "derivado" das cidades vizinhas e fica "em guerra constante". Proposta: ele passa a **guardar quem o domina** (exceção à regra do derivado); domínio = vencer a última batalha ali e ficar 4 dias sem oposição. O mapa só muda de dono no tratado de paz. Isso substitui o "limiar de controle" em aberto na Facção.

**A3. Como se forma um exército e quem o comanda?**

- *Contexto:* os Esquadrões são formados no Quartel e pertencem às cidades. O companheiro pode ser Comandante de Esquadrão.
- *Pergunta:* qual o tamanho máximo de um exército, ele precisa de general, quanto tempo leva para formar um Esquadrão?
- *Sugestão:*
  - **Exército** = 1 a 20 Esquadrões + Caravanas Militares + **1 General** (NPC Importante: líder, herdeiro, nobre, companheiro ou o jogador). 20 Esquadrões = 2.000 soldados, o mesmo teto da tabela de Viagem e do limite por lado na batalha (B3).
  - Sem General, os Esquadrões só ficam de **guarnição** numa cidade (somam ao Controle e defendem no cerco).
  - **Comando do General** = média de Carisma e Inteligência; dá até +10 de Moral inicial ao exército (Comando 50 → +5).
  - **Formar um Esquadrão:** 1 conscrito + o kit do tipo (B1) + 2 dias no Quartel. O Quartel forma, ao mesmo tempo, tantos Esquadrões quanto o nível dele.
  - **Ordens no Mapa Mundial:** mover, atacar exército, cercar, saquear, guarnecer, seguir exército aliado, voltar.
- *Impacto:* Construção (Quartel), Companheiros (general), Viagem.

**A4. Existem mercenários?**

- *Contexto:* a Vila Livre do Norte "vive de mercenários". O Aventureiro e o dono de Vila Livre não têm conscrição de facção.
- *Pergunta:* há tropas de aluguel? Como o jogador sem facção entra numa guerra com mais do que o grupo?
- *Opções:* (a) não há; o Aventureiro luta só com o grupo; (b) **companhias de mercenários**: Esquadrões sem cidade, contratados por estação; (c) recrutar voluntários em qualquer Taverna.
- *Sugestão:* (b). 6 a 8 companhias no mundo, de 2 a 5 Esquadrões cada, com sede nas Vilas Livres e em cidades de nível 3+.
  - Preço: 2× o soldo normal por estação, adiantado.
  - Não tiram população de nenhuma cidade; repõem baixas devagar (1 Esquadrão por estação).
  - Vão embora se o pagamento atrasa ou se o Placar do contratante fica abaixo de −50.
  - Trabalham para facções de IA (ricas e em guerra) e para o jogador. O jogador dono de cidade, mesmo independente, também usa o próprio Quartel e a própria Conscrição.
- *Impacto:* Econômico (gasto do Banco), Notícias (companhias procurando contrato), Reputação (lutar como mercenário conta como batalha).

**A5. Suprimento: quanto consome, quanto carrega, pode pilhar?**

- *Contexto:* a Caravana Militar leva Ração Militar e materiais de manutenção; sem suprimento a Moral cai.
- *Sugestão* (mesma escala do Fator: 1 unidade abastece 100 soldados):
  - **Consumo por Esquadrão:** 1 Ração Militar por dia; 1 material de manutenção a cada 4 dias (Barra de Ferro para tropas pesadas, Couro para leves); tropas de tiro gastam 1 Munição do tipo por batalha.
  - **Caravana Militar:** carrega 60 unidades. Sem caravana, o exército leva 3 dias de ração. *Exemplo:* 10 Esquadrões com 2 Caravanas = 12 dias de ração, quase uma estação de campanha.
  - **Em território próprio ou aliado:** a Caravana reabastece sozinha ao passar por cidade com Mercado (pago pelo Banco da Facção).
  - **Em território inimigo, forrageio:** tira até 50% do consumo do próprio mapa (fazendas e pastos), o que reduz a produção ali e a Felicidade da cidade daquele mapa.
  - **Comboio:** a Caravana pode ir e voltar sozinha até a cidade amiga mais próxima. Ela é uma entidade em trânsito e pode ser emboscada. É a "linha de suprimento" sem sistema novo.
  - **Sem suprimento:** Moral −5 por dia; abaixo de 30, deserta 1 Esquadrão a cada 2 dias (o de menor Moral).
- *Impacto:* Produção (o Cozinheiro nível 3 vira estratégico), Comércio (o preço da Ração sobe na guerra pela própria simulação), Viagem.

**A6. Como uma cidade é tomada (cerco)?**

- *Contexto:* a Construção deixou Muralha e Posto da Guarda para depois; a População diz que só saque ou fome derrubam o nível de uma cidade.
- *Opções:* (a) assalto imediato sempre: simples, mas as cidades caem rápido demais; (b) **cerco com tempo e assalto opcional** (Total War, Mount & Blade); (c) só por fome: lento, sem batalha.
- *Sugestão:* (b).
  - Um exército hostil parado no Mapa Local da cidade **inicia o cerco**: param o comércio, a migração e as caravanas da cidade.
  - A cidade consome o estoque de comida sem repor. Quando acaba: fome (Felicidade e mortalidade) e Moral da guarnição −10 por dia.
  - **Sem Muralha:** o atacante pode atacar já no 1º dia.
  - **Com Muralha:** precisa de **Aríete** (o exército monta em 2 dias) para o portão, ou de **brecha** feita pela Artilharia (B7) ou por Magos de Combate (B8).
  - **Rendição:** quando a Moral da guarnição chega a 0, ou depois de 8 dias de fome.
  - **Assalto** = batalha de cerco (B12) ou cálculo rápido com o bônus da Muralha (A12).
  - *Exemplo:* a Guerreira cerca o Vilarejo Autoritário (1,3), nível 2, com Paliçada e 10 dias de comida. Monta o Aríete em 2 dias e ataca, ou espera cerca de 18 dias pela rendição.
- *Impacto:* Construção (Muralha), População (fome), Felicidade (fator de cerco), Comércio.

**A7. A cidade tomada muda de dono na hora?**

- *Opções:* (a) na hora (Total War): simples, mas uma campanha rápida engole meio mundo; (b) **ocupada até a paz** (Europa Universalis): só o tratado muda a facção.
- *Sugestão:* (b).
  - **Ocupada:** a bandeira muda e o imposto vai para o ocupante, mas as leis e o dono legal continuam. Felicidade −20 (fator "ocupação"); a guarnição do ocupante dá o Controle.
  - **Posse de fato:** ocupada por mais de 2 anos sem paz, passa ao ocupante.
  - **Ao mudar de dono:** adota as leis da nova facção (herança total); a população mantém a cultura, e a Cultura dominante da facção muda aos poucos, por estação; Felicidade −15 "conquistada", que some em 1 ano; os nobres antigos viram exilados.
  - **Cidade tomada pelo jogador a serviço de uma facção:** vai para o líder. O jogador pode pedi-la; o líder aceita se a Opinião sobre ele for +30 ou mais e o Renome for alto (como em Mount & Blade).
- *Conflito:* a Facção diz que uma cidade pertence "por conquista"; passa a valer "conquista confirmada no tratado ou por posse de fato".

**A8. O que é saquear?**

- *Contexto:* a População prevê queda de nível por saque; a Reputação já conta o saque como ação de guerra.
- *Sugestão:* ao tomar uma cidade, o vencedor escolhe:

| Escolha | Efeito |
|---|---|
| Ocupar (padrão) | Ver A7 |
| Saquear | Leva 50% do dinheiro e 30% dos estoques da cidade; mata 10% da população; Felicidade −30 por 1 ano; pode cair 1 nível; Moral do exército +10; Reputação −10 com o dono; Placar +5 |
| Saquear e ocupar | Os dois, com Felicidade −40 |

  - Mapas sem cidade também podem ser saqueados: produção do mapa −50% por 1 estação, pouco dinheiro, Placar +2.
  - Cultura: a Guerreira e a Escravista não se ofendem com saque; a Religiosa condena (−5 de relação das facções Religiosas com o saqueador).
- *Impacto:* População, Felicidade, Reputação, Eventos ("Por quê?": "Cidade saqueada pela Facção X").

**A9. Cansaço de guerra.**

- *Contexto:* a Felicidade tem o fator "Guerra e ameaça" de 0 a −15, sem regra.
- *Sugestão:* Cansaço de 0 a 100 por facção, somando todas as guerras dela.
  - **Sobe:** +1 a cada 4 dias em guerra; +1 a cada 2 Esquadrões perdidos; +5 por cidade própria ocupada, por estação; +5 por estação com comércio bloqueado.
  - **Efeito na Felicidade:** fator "Guerra e ameaça" = −Cansaço × 0,15 (Cansaço 100 → −15, o limite que já existe).
  - **Cansaço 60+:** a IA aceita paz pior. **80+:** assembleia extraordinária pedindo paz. **100:** aceita paz branca.
  - **Cai** 10 por estação em paz.
  - **Cultura:** a Guerreira se cansa pela metade.
- *Impacto:* Felicidade, Facção (assembleia), IA (C5).

**A10. Tratado de paz.**

- *Sugestão:* cada termo tem um custo; o vencedor exige termos até o total do Placar.

| Termo | Custo |
|---|---|
| Paz branca (ninguém ganha nada) | 0 |
| Ceder mapa em disputa | 10 |
| Ceder cidade ocupada | Vilarejo 15, Cidade 30, Capital 50 |
| Tributo: 10% da receita do Banco por 2 anos | 15 |
| Indenização única | 1 ponto = 1% do Banco do perdedor, até 30 |
| Libertar prisioneiros e devolver reféns | 5 |
| Romper aliança com terceiro | 10 |
| Independência de cidade ou vassalo | 30 |
| Vassalagem (C3) | 60 |

  - Só se cede cidade que esteja ocupada.
  - **Trégua obrigatória de 1 ano** entre os dois. Quebrar a trégua: −50 de relação com todos e −30 de Confiabilidade.
  - A IA aceita se o Placar contra ela cobre o custo, ou se o Cansaço dela é alto.
  - *Exemplo:* Placar +45 permite exigir "Ceder Cidade (30) + Tributo (15)".

**A11. Defesa de cidade: Muralha, Posto da Guarda e milícia.**

- *Contexto:* Muralha e Posto da Guarda ficaram para depois. Os guardas não são soldados e nunca são alistados.
- *Sugestão:*
  - **Muralha**, 3 níveis: Paliçada (madeira, Era 1), Muralha de Pedra (Era 1, cidade nível 3+) e Bastião (Era 3; a brecha leva o dobro do tempo). Custo e manutenção em Madeira e Pedra.
  - **Posto da Guarda:** mais vagas de guarda (mais Controle) e ponto de reunião da milícia.
  - **Milícia:** no assalto, cada guarda (NPC real) vira 1 Esquadrão de Milícia (fraco, equipamento leve) e volta a ser guarda depois, se sobreviver. Respeita a regra: guardas não vão à guerra de campo, só defendem a própria cidade.
  - **Convocação de emergência:** o dono pode armar até 5% da população adulta como Milícia; tira trabalhadores enquanto dura.
  - **Guarnição:** Esquadrões estacionados (A3).
- *Arte:* um tileset de Muralha (paliçada genérica; pedra com paleta por cultura), pintado no Mapa Local da cidade.
- *Impacto:* Construção, Felicidade (Controle), Pipeline (tiles de muralha).

**A12. Cálculo rápido de forças.**

- *Contexto:* usado nas guerras de IA e nos encontros de caravanas e patrulhas; precisa bater com a batalha real.
- *Sugestão:* as mesmas grandezas da batalha (B2), resolvidas em rodadas:
  1. **Força de um Esquadrão** = (Soldados ÷ 100) × (Ataque + Defesa) × (Moral ÷ 100) × bônus de Experiência × vantagem do tipo contra o exército inimigo (B5).
  2. **Força do exército** = soma × Comando do General (até ×1,1) × Muralha, só para quem defende (Paliçada ×1,3; Pedra ×1,6; Bastião ×2,0) × suprimento (×0,7 se faltar).
  3. **Até 5 rodadas:** em cada uma, cada lado perde soldados = força inimiga × 0,08 × sorte (0,8 a 1,2), espalhados pelos Esquadrões; a Moral cai com as perdas como na batalha.
  4. Quem fica com Moral média abaixo de 25 foge (perseguição em B11).
  - **Calibração:** nos testes, o jogo roda centenas de batalhas do Mapa de Guerra sem tela e compara com o cálculo rápido; os coeficientes ficam em JSON.
  - **O jogador também pode usar:** botão "Resolver automaticamente" na pré-batalha, como no Total War.
  - A comparação aparece como texto ("muito superior", "equilibrado"...) na pré-batalha e na lente Perigo.

**A13. Como o jogador participa da guerra?**

- *Sugestão:*

| Situação | O que o jogador faz |
|---|---|
| Aventureiro sem facção | Entra como mercenário (contrato de uma batalha ou de uma estação), com a Comitiva (B9) e companhias contratadas (A4) |
| Membro (nobre) | O líder pode lhe entregar um exército, se Renome e Opinião deixarem; senão, luta com a Comitiva junto do exército da facção |
| Vassalo | Chamado às armas: manda 30% dos Esquadrões ou vai pessoalmente; recusar dá −10 (já decidido) |
| Líder de facção | Controla tudo; pode delegar exércitos a generais da IA com ordens simples (atacar alvo, defender região, seguir) |
| Passando perto | Se uma batalha de IA acontece no mapa do jogador ou num vizinho, ele pode entrar por um dos lados (abre o Mapa de Guerra) |

  Batalha sem o jogador é sempre cálculo rápido.

### B. Batalha (Mapa de Guerra)

**B1. Tipos de Esquadrão.**

- *Contexto:* o Esquadrão tem Equipamento; os Itens têm todas as armas, Cavalos e Sela; o boneco é por camadas, com animações por tipo de arma.
- *Opções:* (a) equipamento livre, item a item: flexível, mas muito balanceamento e interface; (b) unidades fixas por cultura (Total War): muita arte; (c) **poucos tipos fixos, com qualidade pelo Rank do equipamento**.
- *Sugestão:* (c), 9 tipos:

| Tipo | Kit | Papel | Era |
|---|---|---|---|
| Infantaria Leve | Espada ou Lança Curta, armadura leve, escudo | Barata e versátil | 1 |
| Infantaria Pesada | Espada Longa, Machado ou Clava, armadura pesada, escudo | Segura a linha | 1 |
| Lanceiros | Lança Longa, armadura média | Contra cavalaria | 1 |
| Arqueiros | Arco, armadura leve, Munição de Arco | Tiro de longe, barato | 1 |
| Besteiros | Besta, armadura média, Munição de Besta | Tiro forte contra pesados | 1 |
| Cavalaria | Cavalo, Sela, Lança ou Espada, armadura média | Carga e perseguição | 1 |
| Atiradores | Musket, armadura leve, Munição de Canhão de Mão | Tiro devastador e lento | 2 |
| Artilharia | Canhão de Guerra, Munição de Canhão de Guerra | Brecha e dano em área | 2 |
| Magos | Cajado, Pedra de Mana (ver B8) | Dano em área ou suporte | 1 |

  - **Kit:** 1 item de cada peça equipa o Esquadrão inteiro (escala do Fator). O **Rank** dos itens define Ataque e Defesa.
  - **Bônus de cultura** (um tipo cada): Guerreira, Cavalaria +10% e mais barata; Autoritária, Lanceiros com +10 de Disciplina; Tecnocrata, Atiradores e Artilharia uma era antes; Arcana, Magos; Religiosa, +10 de Moral em todos; Escravista, Infantaria de escravos com metade do soldo e −15 de Moral.
- *Arte:* nenhuma unidade nova; usa o boneco por camadas e as animações que já existem. A cultura muda só a paleta.
- *Conflito:* o Combate local deixou a montaria fora da luta; a Cavalaria existe só no Mapa de Guerra.

**B2. Números do Esquadrão.**

- *Sugestão:*
  - **Atributos:** Soldados (0 a 100), Ataque, Defesa, Alcance, Velocidade, Moral (0 a 100), Disciplina (0 a 100), Experiência (0 a 5), Munição e Mana (só Magos).
  - Ataque e Defesa saem do kit (dano médio da arma e Defesa somada da armadura), como no Combate local, com a mesma tabela de ±25% de arma × armadura.
  - Cada soldado tem 10 de Vida; o dano num Esquadrão derruba soldados.
  - **Experiência:** cada nível dá +5% de Ataque e Defesa e +5 de Disciplina; sobe 1 nível a cada 2 batalhas sobrevividas.
  - Valores iniciais com equipamento Rank 1 (Alcance em relação ao Arco = 1):

| Tipo | Ataque | Defesa | Alcance | Velocidade | Observação |
|---|---|---|---|---|---|
| Infantaria Leve | 8 | 4 | — | 1,0 | |
| Infantaria Pesada | 10 | 8 | — | 0,8 | |
| Lanceiros | 8 | 6 | — | 0,9 | +50% contra Cavalaria |
| Arqueiros | 6 | 2 | 1,0 | 1,0 | |
| Besteiros | 9 | 5 | 0,8 | 0,9 | |
| Cavalaria | 10 | 6 | — | 2,0 | +50% na carga |
| Atiradores | 14 | 2 | 0,7 | 0,9 | Recarga longa |
| Artilharia | 20 em área | 1 | 3,0 | 0,4 | Não anda atirando |
| Magos | 12 em área | 2 | 1,0 | 1,0 | Gasta Mana |

**B3. Quantos bonecos na tela, e qual o limite?**

- *Contexto:* 20 Esquadrões = 2.000 soldados por lado; bonecos de 16 × 24 px; o jogo precisa rodar em qualquer PC.
- *Opções:* (a) 1 boneco por soldado: até 4.000 bonecos, épico, mas pesado para o PC e para a simulação; (b) **1 boneco a cada 5 soldados**: 20 por Esquadrão, até 800 na tela; (c) só um estandarte por Esquadrão: leve, mas perde o espetáculo.
- *Sugestão:* (b). O combate é calculado **por Esquadrão** (barato no ECS), e os 20 bonecos são **só visuais**: ocupam lugares na formação e animam ataque e queda; cai 1 boneco a cada 5 soldados perdidos. Limite de **20 Esquadrões por lado** no campo ao mesmo tempo; o resto entra como reforço (B13).
- *Impacto:* Pipeline (sem arte nova), Arquitetura (desenho em lote), Combate (os mesmos bonecos do Mapa Local).

**B4. Moral.**

- *Sugestão:*
  - **Início:** 70 + General (A3) + Comandante (B9) + cultura; −20 se sem suprimento.
  - **Cai:** −1 a cada 2% de soldados perdidos; flanco −10 no impacto; retaguarda −20; General derrotado −25 em todo o exército; Esquadrão aliado vizinho fugindo −5; atingido por artilharia ou magia −3 por golpe.
  - **Sobe:** inimigo fugindo perto +5; General por perto, +1 por segundo até a Moral inicial; vencendo o corpo a corpo, +1 a cada poucos segundos.
  - **Disciplina** reduz todas as quedas: queda × (1 − Disciplina ÷ 200). Disciplina 100 = metade da queda.
  - **Faixas:** Firme (50+); Abalado (25 a 49, −10% de Ataque); **Fugindo** (abaixo de 25: corre para a borda; reagrupa se a Moral voltar a 40 sem perseguição); **Destroçado** (fugiu 2 vezes: sai da batalha).

**B5. Formações, flanco e vantagens.**

- *Sugestão:*
  - **Formações:** Linha (padrão); Coluna (+30% de velocidade, −30% de Defesa); Muralha de Escudos (parado, anula a carga, −50% de velocidade); Dispersa (−50% do dano de tiro e artilharia recebido, −20% no corpo a corpo); Cunha (só Cavalaria, +25% na carga).
  - **Flanco:** +25% de dano e −10 de Moral no alvo. **Retaguarda:** +50% e −20.
  - **Carga:** a Cavalaria tem +50% no primeiro impacto; Lanceiros parados anulam a carga.
  - **Ciclo de vantagens:** Lanceiros vencem Cavalaria; Cavalaria vence tiro (Arqueiros, Besteiros, Atiradores, Artilharia); tiro vence infantaria lenta; Infantaria Pesada vence Lanceiros.

**B6. Terreno e tamanho do campo.**

- *Contexto:* o Mapa de Guerra herda o terreno do Mapa Local (128 × 128 tiles); o Tiled tem 6 camadas fixas; o jogo é 2D visto de cima, sem altura.
- *Opções:* (a) mapas de batalha próprios, desenhados por bioma: lindos, muita arte; (b) **o próprio Mapa Local simplificado** (sem NPCs e itens; construções viram obstáculos): nenhuma arte nova e geografia exata; (c) campo gerado: fere o "nada procedural".
- *Sugestão:* (b). A batalha acontece numa cópia do Mapa Local.
  - **Terreno lido dos tiles:** Floresta (−30% do dano de tiro recebido, −30% de velocidade, sem carga de Cavalaria); Rio (só se atravessa em ponte ou vau); Pântano (−50% de velocidade); Estrada (+20% de velocidade); construções (obstáculo e cobertura).
  - **Elevação (opcional):** 7ª camada no Tiled, pintada só nas colinas. De cima: +15% de alcance e de dano no tiro, +10% no corpo a corpo morro abaixo.
  - **Posicionamento:** o atacante entra pela borda de onde veio; o defensor se arma no centro ou no lado oposto.
- *Conflito:* o Pipeline fixou 6 camadas; a Elevação seria uma 7ª, opcional (mapa sem ela é plano).

**B7. Artilharia e pólvora.**

- *Contexto:* Canhão de Guerra (Rank 3, Armeiro nível 3) e Moderno (Rank 5); o mundo está "em transição para a pólvora".
- *Sugestão:*
  - Esquadrão de Artilharia = 100 artilheiros com o Canhão (kit). Alcance 3× o do Arco; dano em área; −3 de Moral por tiro nos atingidos.
  - Faz **brecha**: Paliçada em 20 s de tiro, Pedra em 60 s, Bastião em 120 s.
  - Não anda enquanto atira; muito lenta; quase sem defesa no corpo a corpo.
  - 1 Munição de Canhão de Guerra por batalha, com limite de 30 tiros.
  - Artilharia e Atiradores a partir da Era 2; a Tecnocrata pode tê-los na Era 1 por tecnologia inicial.
- *Impacto:* Tecnologia (ramo Militar), Produção (Armeiro), Defesa (o Bastião da Era 3 é a resposta).

**B8. Esquadrão de Magos.**

- *Contexto:* Magos são 5% da população (15% na Arcana). O Esquadrão de Magos é caro, limitado pela quantidade de Magos e pela lei de Magia. Feitiços de área acertam aliados.
- *Pergunta:* quantos podem existir, o que fazem e quanto custam?
- *Sugestão:*
  - 1 Esquadrão de Magos = 1 NPC Mago alistado (100 magos, pela escala). Limite: até 10% dos NPCs Magos da facção e no máximo 1 para cada 4 Esquadrões comuns do exército. *Exemplo:* a Arcana tem cerca de 66 NPCs Magos, logo até 6 Esquadrões de Magos.
  - **Custo:** 3× o soldo; 1 Pedra de Mana por batalha.
  - **Pela escola do Mago alistado:** Combate = dano em área de médio alcance (artilharia leve que anda), −5 de Moral nos atingidos, abre brecha devagar. Suporte = devolve 1 soldado a cada 10 s aos Esquadrões próximos (feridos voltam) e +10 de Moral em volta. Ofício não vai à guerra.
  - **Mana do Esquadrão:** gasta por feitiço e volta devagar; a Pedra de Mana da Caravana enche.
  - Área acerta aliados (regra do Combate), o que pede pausa e cuidado.
  - **Cálculo rápido:** Magos de Combate valem Força ×2; de Suporte reduzem em 15% as perdas do exército.
- *Conflito:* a Tecnocrata (magia proibida) não forma. **[Proposta]** Na lei de Magia "restrita" (Religiosa), só Magos de Suporte.

**B9. General, Comitiva e Comandantes.**

- *Contexto:* o companheiro pode ser Comandante de Esquadrão, com bônus em aberto. No Mapa Local, o jogador controla o grupo individualmente.
- *Sugestão:*
  - **Comitiva:** o General entra com a guarda pessoal. Se o General é o jogador, a Comitiva é o **grupo controlado de verdade**: cada boneco é um companheiro, com a Vida e o dano reais do Combate local. É o Esquadrão "herói", como a guarda do general no Total War.
  - **Aura:** +10 de Moral num raio. Se a Comitiva foge ou cai, −25 em todo o exército. Companheiros que caem seguem as regras do Combate (Inconsciente, morte permanente, captura).
  - **Comandante de Esquadrão** (companheiro ou nobre): +1% de Ataque e Defesa e +1 de Moral a cada 10 pontos da habilidade principal (Corpo a Corpo para infantaria e cavalaria; Longo Alcance para tiro). *Exemplo:* Corpo a Corpo 60 → +6% e +6 de Moral. Se o Esquadrão é destruído: 50% de chance de ferido, 30% de capturado e 20% de morto.
- *Conflito:* resolve o "bônus dos cargos" em aberto em Companheiros.

**B10. Controles, tempo e telas.**

- *Contexto:* a Interface deixou as telas do Mapa de Guerra em aberto; o Tempo limita o Mapa de Guerra a 4x.
- *Sugestão:*
  - **Controles** estilo Total War: clique ou caixa para selecionar; Ctrl+1 a 9 para grupos; botão direito para mover ou atacar; arrastar com o botão direito define a largura e a frente da linha; botões de formação; pausa ativa (espaço) com ordens enfileiradas; velocidades 1x, 2x e 4x.
  - **O mundo para durante a batalha.** Uma batalha de 15 minutos reais a 1x seria quase um dia de jogo (16 min = 24 h). O relógio do mundo congela no Mapa de Guerra e, no fim, avança 2 horas fixas.
  - **Telas:** (1) Pré-batalha: forças comparadas, terreno, previsão; botões Lutar, Resolver automaticamente e Recuar (perde 10% e foge 1 mapa). (2) Posicionamento: zona de deploy e formações. (3) Batalha: cartões dos Esquadrões (Soldados, Moral, Munição), minimapa, barra de força dos lados; HUD personalizável como o resto. (4) Resultado: baixas, prisioneiros, saque, Placar e Reputação.
- *Conflito:* nenhum; acrescenta ao Tempo a regra do mundo parado na batalha.

**B11. Fim da batalha e o que sobra.**

- *Sugestão:*
  - **Vitória:** todos os Esquadrões inimigos fugindo ou destruídos. Quem manda recuar sai pela borda.
  - **Tempo limite:** 30 minutos de batalha; se acabar, vence o defensor.
  - **Perseguição:** se o vencedor tem Cavalaria, quem fugiu perde mais 20% dos soldados restantes.
  - **Baixas:** dos soldados derrubados, 40% morrem, 40% ficam feridos e 20% são capturados pelo vencedor. Feridos do vencedor voltam em 16 dias (8 com Hospital na cidade de origem ou com Magos de Suporte); feridos do perdedor viram prisioneiros.
  - **Prisioneiros comuns:** escravizar (se a lei permite; 100 prisioneiros = 1 NPC Escravo), libertar (+Reputação) ou trocar no tratado. Importantes seguem a regra já decidida.
  - Esquadrão com menos de 20 soldados se desfaz; os restantes voltam à cidade de origem ou se juntam a outro do mesmo tipo.
- *Impacto:* População (baixas), Crime (prisioneiros), Escravidão (futuro), Reputação.

**B12. Batalha de cerco.**

- *Contexto:* a batalha usa o Mapa Local (B6), onde a cidade e a Muralha (A11) já estão desenhadas.
- *Opções:* (a) cerco completo (escadas, torres, muralhas andáveis): espetacular e caro; (b) **cerco simples**: portão e brechas; (c) cerco só por cálculo rápido.
- *Sugestão:* (b) na primeira versão.
  - O defensor fica atrás da Muralha (tiles com colisão), que tem Portão e trechos.
  - O atacante entra pelo **Portão** derrubado com o Aríete (a infantaria empurra por 60 s, sob fogo) ou pelas **brechas** da Artilharia ou dos Magos de Combate.
  - Tropas de tiro defensoras em posições marcadas na Muralha: +30% de Defesa e +20% de alcance.
  - **Vitória do atacante:** controlar a praça central por 60 s (ponto marcado no Tiled) ou afugentar a guarnição.
  - Escadas e torres de cerco ficam para o futuro.
- *Arte:* tileset de Muralha com Portão (aberto, fechado, destruído) e trecho (inteiro, em brecha).

**B13. Reforços e vários exércitos.**

- *Sugestão:* exércitos amigos ou inimigos num Mapa Local vizinho entram como **reforço** pela borda correspondente, com atraso de 1 minuto de batalha por hora de viagem. A guarnição da cidade também luta. Acima de 20 Esquadrões por lado, os extras entram quando algum sai do campo. Aliados de outras facções lutam do mesmo lado, controlados pela IA.

### C. Diplomacia

**C1. Estados diplomáticos.**

- *Contexto:* a Facção lista 6 estados exclusivos (Guerra, Hostil, Neutro, Comércio, Aliança, Vassalagem) e uma relação de −100 a +100.
- *Pergunta:* duas facções podem ter acordo comercial e aliança ao mesmo tempo? "Hostil" é um estado ou um sentimento?
- *Opções:* (a) manter os 6 estados exclusivos: simples, mas Comércio e Aliança não convivem; (b) **separar em 3 camadas**.
- *Sugestão:* (b).
  1. **Estado** (um só): Paz, Guerra ou Trégua.
  2. **Tratados** (vários ao mesmo tempo): Acordo Comercial, Passagem Militar, Não Agressão, Aliança Defensiva, Aliança Total, Vassalagem e Casamento (C2).
  3. **Atitude** (faixa da relação, só para leitura): Hostil (−100 a −31), Fria (−30 a −1), Neutra (0 a 29), Amigável (30 a 69), Leal (70+).
  - Com atitude Hostil, a fronteira pode ser fechada e a guerra sem motivo custa a metade.
- *Conflito:* muda a lista de estados da Facção; a seção dela passa a apontar para a Diplomacia.

**C2. Tratados e Confiabilidade.**

- *Sugestão:*

| Tratado | Efeito | Exige | Duração |
|---|---|---|---|
| Acordo Comercial | Tarifa 0 entre os dois | Relação +10 | Até cancelar (aviso de 16 dias) |
| Embargo (de um lado só) | Proíbe as caravanas do outro | — | Até cancelar |
| Passagem Militar | Exércitos atravessam o território sem guerra | Relação +20 | 1 ano |
| Não Agressão | Proíbe declarar guerra | Relação 0 | 1 ano |
| Aliança Defensiva | Entra na guerra se o aliado for atacado | Relação +40 | Até cancelar |
| Aliança Total | Entra também nas guerras de ataque do aliado | Relação +60 | Até cancelar |
| Vassalagem | Ver C3 | | |
| Casamento | Ver C4 | | |

  - **Confiabilidade** (0 a 100, começa em 70): nova nota de cada facção. Cai ao quebrar tratado (−20), ignorar chamado de aliado (−15) ou quebrar trégua (−30); sobe 5 por estação sem quebras. A IA não faz tratado com quem tem menos de 30.

**C3. Vassalagem.**

- *Contexto:* os estados do jogador são Independente, Membro, Vassalo e Líder; recusar o chamado dá −10.
- *Pergunta:* o que o vassalo deve, o que ganha, e qual a diferença para o membro?
- *Sugestão:*
  - **Membro** (cidade dentro da facção): segue todas as leis (herança total). **Vassalo** (facção ou cidade independente subordinada): tem leis próprias, mas paga e serve.
  - **Deve:** tributo de 15% do imposto ao Banco do suserano; chamado às armas (30% dos Esquadrões ou o valor em dinheiro); não declara guerra nem faz aliança por conta própria.
  - **Ganha:** Aliança Defensiva automática e Acordo Comercial com o suserano.
  - **Sai:** declarando independência (guerra), o que fica mais fácil se o suserano está em guerra ou cansado.
  - **Integração:** depois de 4 anos com relação +50, o suserano pode absorver o vassalo em paz (as cidades viram membros).
  - **Para o jogador:** o dono de uma Vila Livre pode pedir vassalagem e ganhar proteção sem perder as próprias leis.

**C4. Casamento entre dinastias e reféns.**

- *Contexto:* o casamento entre dinastias dá +20 de relação; as dinastias da IA casam por estação.
- *Sugestão:*
  - **Casamento:** proposto com relação 0 ou mais, se as duas dinastias têm adultos solteiros. Efeitos: +20 de relação, Não Agressão enquanto os dois viverem e **reivindicação**: se a outra dinastia fica sem herdeiro, o filho do casal pode reclamar o trono (motivo "Reivindicação", A1).
  - **Reféns**, em versão simples: um nobre entregue como garantia num tratado de paz ou de vassalagem, que vive 2 anos na capital da outra facção. Se quem o entregou quebrar o tratado, o refém pode ser preso ou executado (a execução dá o motivo "Ofensa"). Abre missões para o jogador: resgatar ou escoltar reféns.
  - *Opções para os reféns:* (a) versão simples, como acima; (b) deixar para o futuro.

**C5. Como a IA decide.**

- *Contexto:* a IA de Facção decide em baixa frequência, com Agressividade, Ganância e Lealdade do líder.
- *Sugestão:*
  - **Frequência:** a cada 4 dias, guerras e paz; a cada estação, tratados, casamentos e alianças.
  - **Vontade de guerra** contra a facção X; declara acima de 60:

| Fator | Pontos |
|---|---|
| Força própria ÷ força de X | 1,0 = 0; 1,5 = +25; 2,0 = +50 (abaixo de 1, negativo) |
| Agressividade do líder | até +30 |
| Ganância × riqueza de X | até +20 |
| Motivo | forte +20; nenhum −20 |
| Relação | −Relação ÷ 4 (relação −60 → +15) |
| Cultura | Guerreira +15; Religiosa +15 contra hostis |
| Guerras em andamento | −20 cada |
| Cansaço | −Cansaço ÷ 2 |

  - *Exemplo:* Guerreira (20 Esquadrões) contra Autoritária (16): força 1,25 → +12; Agressividade alta +25; Ganância média e celeiro rico +10; motivo (mapa em disputa) +20; relação −55 → +14; cultura +15. Total **96**: declara guerra.
  - **Lealdade** do líder: respeita tratados e chamados; abaixo de 30, pode trair.
  - **Sociabilidade:** mais propostas de tratado e casamento.
  - Cada decisão guarda o "Por quê?", para as Notícias e para o painel de depuração.

**C6. A diplomacia do jogador.**

- *Sugestão:*
  - **Líder de facção:** acesso total ao painel de Diplomacia.
  - **Dono de cidade independente:** só Acordo Comercial, Passagem, Não Agressão e pedido de vassalagem.
  - **Membro:** sem diplomacia de facção; influencia pela assembleia e pela Opinião do líder.
  - **Painel:** lista de facções com relação, atitude, tratados, Confiabilidade e Placar das guerras. Cada proposta mostra a **chance de aceite e o porquê** ("+20 pela relação; −30 porque vocês estão em guerra com o aliado deles").
  - **Bônus pessoal:** (Carisma − 50) ÷ 5 + Opinião do líder ÷ 5 na chance.
  - Propostas são instantâneas na primeira versão (sem mensageiro viajando); as **notícias** da guerra chegam com atraso (sistema de Notícias).

**C7. Freio contra quem cresce demais.**

- *Contexto:* 48 mapas e 6 facções; uma facção (ou o jogador) pode engolir o mundo depressa.
- *Opções:* (a) nenhum freio; (b) **coalizão**: os vizinhos se aliam contra o mais forte; (c) **superextensão**: cada conquista recente pesa na Estabilidade.
- *Sugestão:* (b) com (c) leve.
  - **Ameaça:** uma facção é ameaçadora se tem PIB acima de 1,6× a média, ou conquistou 3+ cidades em 2 anos. As facções em contato ganham +30 na decisão de formar uma **Coalizão** (Aliança Defensiva conjunta contra ela).
  - **Superextensão:** cada cidade conquistada há menos de 1 ano dá −3 de Estabilidade à facção.
  - Vale também para o jogador líder.

### D. Escopo

**D1. Documentos e ordem de programação.**

- *Sugestão:*
  - **Três documentos novos:** `Sistema de Guerra/Descrição.md` (campanha, cerco, ocupação, saque, cansaço, paz, defesa e cálculo rápido), `Sistema de Batalha/Descrição.md` (Mapa de Guerra) e `Sistema de Diplomacia/Descrição.md`. A seção "Relações diplomáticas" da Facção vira um resumo com link.
  - **Atualizar:** Facção, Mapa, Combate, Construção (Muralha, Posto da Guarda), Itens (kits), Magia, Companheiros, Felicidade, Reputação, Interface, Tempo, Tecnologia, IA, Pipeline (camada Elevação e tiles de Muralha) e Glossário.
  - **Dados:** `esquadroes.json` (tipos e kits), `guerra.json` (Placar, Cansaço, cerco, saque, coeficientes do cálculo rápido) e `diplomacia.json` (tratados, pesos da IA).
  - **Ordem de programação** (para a ficha técnica): o Mapa de Guerra é um jogo de estratégia inteiro dentro do jogo, então vem **depois** que o mundo funcionar. Primeiro a guerra por **cálculo rápido** (o mundo já tem guerras, e o jogador resolve as dele automaticamente); depois o Mapa de Guerra, como um marco próprio.
- *Conflito:* nenhum; a fatia vertical continua sem guerra.

Depois das respostas: escrever os três documentos, atualizar os vizinhos e abrir a próxima rodada (Religião, Clima e Estações, Notícias; depois Efeitos de itens e Escravidão).

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
