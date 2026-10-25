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

## Rodada HI1: a história do mundo, o geral (2026-10-25)

**Contexto.** Pausa no código para começar a história, inspirada no Kenshi: um mundo cruel e desigual, de fantasia pré-industrial, onde o jogador pode ser bom, mas o mal é mais divertido. As opções completas estão no [[Documentos/Mundo e História/Descrição|Mundo e História]]. Não muda regra de jogo; muda os nomes e os textos depois.

**Pergunta HI1-1: qual é o tema do mundo?** (parte 2)

| Opção | Tema | Ganha | Perde |
|---|---|---|---|
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
