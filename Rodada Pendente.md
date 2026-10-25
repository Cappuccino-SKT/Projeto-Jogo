# Rodada pendente

## Rodada CB2: o cálculo rápido não bate com o combate visto (2026-10-24)

**Contexto.** O marco D1 (Combate) está programado. O cálculo rápido (CB1-1) dá a chance pelo **poder** de cada lado (a soma do dano por tick × a soma da Vida), e a ficha esperava que ele ficasse perto do combate visto. Os testes mostram que isso só vale com os lados parecidos:

| Luta | Combate visto (1.000 lutas) | Cálculo rápido |
|---|---|---|
| 2 guardas contra 2 guardas | 46% | 50% |
| O jogador contra 1 bandido | quase 100% | 78% |
| O jogador contra 2 bandidos | **97%** | **46%** |

Por quê: no combate visto, o jogador derruba um bandido de cada vez, e o lado dos bandidos perde dano enquanto luta. O poder conta o dano de todos até o fim. Assim o cálculo rápido dá vitória demais ao lado maior, e a sorte pesa muito mais do que na luta vista.

**Pergunta CB2-1: como fazer o cálculo rápido?**

| Opção | O que muda | Ganha | Perde |
|---|---|---|---|
| **A** | Manter a CB1-1 (o poder ÷ a soma dos poderes) | Nada muda; simples | Fora da tela, o bando grande vence muito mais do que venceria na tela; um jogador forte perde lutas que ganharia |
| **B** | Usar o **tempo** que cada lado leva para derrubar o outro (a Vida do outro ÷ o dano por tick do seu lado, com o dano caindo quando os seus caem) e uma chance que cresce rápido com a diferença | Fica perto do combate visto e continua barato | Uma conta nova para ajustar e testar |
| **C** | O cálculo rápido **roda o combate simulado** (o mesmo duelo dos testes, sem o mapa) e vê quem ganha | Bate com o combate visto por construção; uma regra só | Gasta mais conta (centenas de ticks por luta); as lutas fora da tela são poucas por ano |

**Sugestão: C.** É a mesma regra dentro e fora da tela, e as lutas fora da tela são raras (algumas por ano no mundo). Se ficar caro com as Batalhas (D6), elas usam a B.

**Conflitos:** muda a CB1-1 e o 4.8 do [[Ficha Logica/Combate|Combate]]; muda o que a Viagem, o Comércio e o Crime (Caçadores) recebem.

## Rodada HI2: o que o evento Y deixou (2026-10-25)

**Contexto.** A HI1 foi respondida: o tema é "Tudo tem dono"; o passado é a versão do autor (o Império Mágico, o ritual C, o feitiço de W e a fusão de duas dimensões no evento Y, que é o Ano 0); não há final real; a crueldade mistura texto e imagem rara. Tudo está no [[Documentos/Mundo e História/Descrição|Mundo e História]]. Pela regra do autor, **a história manda**. Estas perguntas resolvem onde ela bate nas fichas.

**Pergunta HI2-1: como ficou a magia depois de Y?** (a ficha diz "baixa magia")

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | A explosão consumiu quase toda a mana. A magia ficou **rara e instável**, forte só perto das marcas da fusão | Mantém a baixa magia e a ficha da Magia; as marcas viram lugares perigosos e cobiçados | Os magos de hoje são uma sombra dos antigos |
| **B** | A fusão trouxe **mais** magia, mas selvagem e difícil de controlar | Mundo mais fantástico | Muda a Magia e o equilíbrio com a pólvora |
| **C** | A magia segue como antes, só que proibida em quase todo lugar | Simples | Perde a ligação com o evento Y |

**Sugestão: A.** Combina com a pólvora: quem não tem magia procura outro poder.

**Pergunta HI2-2: o que veio da outra dimensão?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Só a terra mudou (biomas estranhos, minérios novos) | Pouco trabalho | Pouca fantasia |
| **B** | Terra mudada **e criaturas** (feras e monstros nas regiões transformadas e nas masmorras) | Dá inimigos ao Combate e às masmorras | Arte e dados de criaturas |
| **C** | B **e povos** de lá: novas raças | A variação de raças que o autor quer | Muda o "só humanos" de várias fichas |
| **D** | B agora; as raças nascem de **humanos transformados pela fusão** em cada região e entram numa versão depois (o campo "Raça" do NPC já existe) | Dá raças sem travar o código de agora | As raças ficam para depois |

**Sugestão: D.**

**Pergunta HI2-3: o que era a outra dimensão?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | O mundo dos deuses, de verdade. Eles estão mortos, presos ou furiosos | Mexe fundo nas fés | Fecha o mistério |
| **B** | Outro mundo qualquer; o ritual errou o alvo | Os deuses continuam incertos | Menos peso |
| **C** | Ninguém sabe; cada fé conta uma versão. A verdade (A ou B) fica escondida nas masmorras lendárias | O grande segredo do jogo; cada cultura tem a sua crença | Precisa decidir a verdade em segredo, para escrever as pistas |

**Sugestão: C**, com a verdade decidida pelo autor depois.

**Pergunta HI2-4: como nasceram as 6 facções?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Dos grupos da guerra B: os soldados rebeldes (Guerreira), os magos sobreviventes (Arcana), os nobres rebeldes (Autoritária), os que viram castigo divino (Religiosa), os que juraram largar a magia (Tecnocrata) e os mercadores que venderam os sobreviventes (Escravista) | Cada facção carrega uma parte da guerra; explica os ódios | W precisa de um destino (morreu na explosão?) |
| **B** | Das províncias do Império, cada região seguiu sozinha | Simples, ligado ao mapa | Quase não usa a guerra B |
| **C** | Misturar: as províncias deram as terras, os grupos da guerra deram as ideias | Os dois ganhos | Mais texto |

**Sugestão: A**, e W morre no evento Y (a Autoritária diz ser a herdeira dele).

**Pergunta HI2-5: onde fica a cratera da antiga capital?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | No centro do mapa, entre as regiões, como um mapa novo de terra morta e perigosa | O lugar mais marcante do mundo, perto de todos | Muda o mapa 8 × 6 do Mundo |
| **B** | Numa das 3 masmorras lendárias de hoje ((5,1), (3,4) ou (5,5)) | Não muda o mapa | A capital fica de lado |
| **C** | A cratera virou um **mar** ou um lago no meio do continente | Visual forte | Muda o mapa e a Viagem |

**Sugestão: B**, na lendária de (3,4), a mais central; as outras duas são restos dos laboratórios do ritual C.

**Conflitos:** a HI2-1 toca a [[Ficha Logica/Magia|Magia]]; a HI2-2, o Combate, o NPC e o Mundo; a HI2-3, a [[Ficha Logica/Religiao|Religião]]; a HI2-4, a Cultura e a Facção (só a história, não as regras); a HI2-5, o Mundo.

---|---|---|---|
| **T1** | Tudo tem dono (terra, lei, pessoas) | Casa com Escravidão, Classe Social e Crime | Menos épico |
| **T2** | O preço do poder | Serve a todo povo e a toda escolha do jogador | Mais vago |
| **T3** | As cinzas do império | Dá o pano de fundo e as ruínas | Sozinho, é comum |

**Sugestão: T1 como tema e T3 como pano de fundo.**

**Pergunta HI1-2: o que aconteceu no passado (o Ano 0)?** (parte 4)

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | A Queda do Império, por guerra civil | Político e simples | Pouco mistério |
| **B** | O Grande Sangramento: um ritual quebrou a magia | Explica a baixa magia e os ódios | Mais "fantasia" |
| **C** | A Peste Cinzenta | Explica a escravidão; usa a Epidemia | Menos grandioso |
| **D** | As três juntas: o ritual para salvar o Império causa o Sangramento, a fome e a peste, e o Império cai | Uma origem para cada povo e um segredo central (cada povo conta uma versão) | Mais coisa para escrever |

**Sugestão: D.**

**Pergunta HI1-3: como o mundo termina?** (parte 6)

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Sem final obrigatório: 6 destinos possíveis que a simulação alcança, com ou sem o jogador | Igual ao Kenshi; usa a simulação que já existe | Nenhum fim "de filme" |
| **B** | Um final principal (por exemplo, descobrir o Ano 0 e decidir o destino), com os destinos como alternativas | Dá um objetivo claro | Uma trama para escrever e manter |
| **C** | Sem destinos: o mundo só continua | Nada a fazer | Falta sensação de conquista |

**Sugestão: A**, com a verdade do Ano 0 como mistério opcional na masmorra lendária.

**Pergunta HI1-4: até onde vai a crueldade mostrada?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Cruel no que acontece (escravidão, fome, traição, tortura), mas contado sem detalhe gráfico, em texto e em consequência | Pesado como o Kenshi, sem chocar à toa | Menos impacto visual |
| **B** | Cruel e gráfico (descrições e imagens fortes) | Mais impacto | Arte mais cara; público menor |
| **C** | Cruel só no fundo; o jogador quase não vê | Mais leve | Perde o tema |

**Sugestão: A.** Crianças nunca como alvo de violência.

**Conflitos:** nenhum com as fichas; as decisões viram o documento Mundo e História e, depois, a rodada HI2 (os nomes).

---

A rodada TE2 (a Tecnologia sem Especialistas e sem as listas) foi respondida pelo autor com a opção A em 2026-10-24: o B5 entrou com conteúdo provisório, e a habilidade Especialista é a Manufaturado até o A4 (ver a parte 10.8 da [[Ficha Logica/Tecnologia|Tecnologia]]). **Rodadas anteriores (já resolvidas):** A rodada CO2 (o comércio raro) foi respondida pelo autor com a opção A em 2026-10-21: as Braçais de comida cobrem 75% do consumo onde o recurso é escasso (ver o `Jogo/LEIA-ME.md`, marco B2). A rodada EQ1 (o Pão nas crises) foi decidida em modo automático em 2026-10-20 pelas sugestões A e B: o Pão acima de 2,6 só falha o teste de 1 ano se durar 8 dias seguidos; a EQ2 (a Padaria depois de uma crise) foi respondida pelo autor com a opção A: a Padaria começa no nível 2 (3 vagas), e o estoque-alvo do Pão voltou para 5 dias (ver o `Jogo/LEIA-ME.md`, marco A6). As rodadas CL1 e TP1 foram respondidas pelo autor com "seguir sugestão". As rodadas PO1, CS1, CI1 e CU1 (fase A) foram decididas em **modo automático** em 2026-10-19, a pedido do autor; as decisões estão na parte 10.7 de cada ficha ([[Ficha Logica/Populacao|População]], [[Ficha Logica/ClasseSocial|Classe Social]], [[Ficha Logica/Cidade|Cidade]], [[Ficha Logica/Cura|Cura]]).

A rodada VI1 ([[Ficha Logica/Viagem|Viagem]], fase B1) também foi decidida em modo automático (parte 10.7 da ficha). A OC1 ([[Ficha Logica/OutrasCidades|Outras cidades]], B1b) também. A CO1 ([[Ficha Logica/Comercio|Comércio]], B2) também. A NO1 ([[Ficha Logica/Noticias|Notícias]], B3) também. A RE1 ([[Ficha Logica/Reputacao|Reputação]], B4) também. A TE1 ([[Ficha Logica/Tecnologia|Tecnologia]], B5) também. A FA1 ([[Ficha Logica/Faccao|Facção]], C1) também. A CT1 ([[Ficha Logica/Cultura|Cultura]], C2) também. A CJ1 ([[Ficha Logica/Crime|Crime e Justiça]], C3) também. A RL1 ([[Ficha Logica/Religiao|Religião]], C4) também. A CB1 ([[Ficha Logica/Combate|Combate]], D1) também. A CP1 ([[Ficha Logica/Companheiros|Companheiros e Diálogo]], D2) também. A ES1 ([[Ficha Logica/Escravidao|Escravidão]], D3) também. A MG1 ([[Ficha Logica/Magia|Magia]], D4) também. A DP1 ([[Ficha Logica/Diplomacia|Diplomacia]], D5) também. A BA1 ([[Ficha Logica/Batalha|Batalha]], D6) também. A GU1 ([[Ficha Logica/Guerra|Guerra]], D7) também. A MU1 ([[Ficha Logica/Mundo|Mundo]], E1) também. A IN1 ([[Ficha Logica/Inicio|Início de Jogo]], E2) também. A OB1 ([[Ficha Logica/Objetivo|Objetivo e Progressão]], E3) também.
