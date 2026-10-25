# Rodada pendente

## Rodada HI3: a cratera, as raças e os ódios (2026-10-25)

**Contexto.** A HI2 foi respondida pelo autor: a mana pura da outra dimensão apagou a magia antiga, trouxe a mana nova (rara de usar, tabu), criou as novas raças e os monstros, e cada facção nasceu de um grupo do fim do Império. Tudo está no [[Documentos/Mundo e História/Descrição|Mundo e História]] (partes 4.4 e 7). Pela regra do autor, a história manda; estas perguntas ajustam as fichas.

**Pergunta HI3-1: onde fica a cratera da antiga capital?** O autor quer perto da costa. A costa é a borda leste (coluna 8) e a sul (linha 6).

| Opção | Onde | Ganha | Perde |
|---|---|---|---|
| **A** | Mapa (5,6), na costa sul, entre a Tecnocrata e a Escravista, no lugar da Vila livre. A masmorra lendária de (5,5) passa a ser as ruínas da capital, em (5,6), e a Vila livre vai para (5,5) | Os vizinhos da cratera são os herdeiros do cerco (Tecnocrata) e os nobres que caçaram os sobreviventes (Escravista): a história explica o mapa | Muda duas células do mapa |
| **B** | Mapa (8,3) ou (8,4), na costa leste, dentro da região Religiosa | A fé que mais sofreu com a mana mora ao lado da cratera | Tira uma célula de uma região que já é pequena |
| **C** | Um mapa novo, fora da malha 8 × 6 | Não mexe em nada | Muda o Mapa e a Viagem (a malha deixa de ser 8 × 6) |

**Sugestão: A.**

**Pergunta HI3-2: quando as outras raças entram no jogo?** As fichas dizem "só humanos"; o NPC já tem o campo "Raça".

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Já na próxima fase do código: fichas novas de Raças (atributos, aparência, como cada facção trata cada raça) | O mundo fica como a história conta | Atrasa os marcos D2 a D7; muita arte nova |
| **B** | Depois dos marcos da fase D. Até lá a história já conta as raças, e o jogo tem só humanos, com o campo "Raça" pronto | Não trava o código; as fichas de raça saem com calma | Por um tempo, o jogo e a história não batem |
| **C** | Antes, como **marca** no NPC humano ("tocado pela mana"), sem raças novas | Dá o tema já, com pouco custo | Não é a variação de raças pedida |

**Sugestão: B**, e a ficha de Raças entra no Plano das Fichas.

**Pergunta HI3-3: os ódios entre os povos mudam?** (parte 7 do Mundo e História)

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Manter os ódios da ficha (Guerreira × Autoritária, Arcana × Religiosa, Religiosa × Escravista, Tecnocrata × Arcana) e **acrescentar** Guerreira × Tecnocrata e Religiosa × Tecnocrata (a Tecnocrata mata ou escraviza os mutantes) | Bate com a história | A Tecnocrata fica hostil a 3 povos; muda a tabela de afinidade da [[Ficha Logica/Cultura|Cultura]] |
| **B** | Manter só os ódios da ficha | Nada muda | A história e o jogo não batem |
| **C** | Acrescentar só Guerreira × Tecnocrata | Meio-termo | A Religiosa fica neutra com quem executa os "abençoados" |

**Sugestão: A.** A Tecnocrata fica isolada, mas é a mais forte em tecnologia: o mundo inteiro contra a pólvora.

**Conflitos:** a HI3-1 muda o mapa do [[Documentos/Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]] e o `data/mundo/`; a HI3-2 muda o Plano das Fichas; a HI3-3 muda a afinidade da Cultura (e, por ela, a Diplomacia e a Guerra).

---

A rodada CB2 (o cálculo rápido) foi respondida pelo autor com a opção C em 2026-10-25: o cálculo rápido roda o combate simulado; **falta aplicar no código**. As rodadas HI1 e HI2 (a história) foram respondidas pelo autor em 2026-10-25: estão no [[Documentos/Mundo e História/Descrição|Mundo e História]]. A rodada TE2 (a Tecnologia sem Especialistas e sem as listas) foi respondida pelo autor com a opção A em 2026-10-24: o B5 entrou com conteúdo provisório, e a habilidade Especialista é a Manufaturado até o A4 (ver a parte 10.8 da [[Ficha Logica/Tecnologia|Tecnologia]]). **Rodadas anteriores (já resolvidas):** A rodada CO2 (o comércio raro) foi respondida pelo autor com a opção A em 2026-10-21: as Braçais de comida cobrem 75% do consumo onde o recurso é escasso (ver o `Jogo/LEIA-ME.md`, marco B2). A rodada EQ1 (o Pão nas crises) foi decidida em modo automático em 2026-10-20 pelas sugestões A e B: o Pão acima de 2,6 só falha o teste de 1 ano se durar 8 dias seguidos; a EQ2 (a Padaria depois de uma crise) foi respondida pelo autor com a opção A: a Padaria começa no nível 2 (3 vagas), e o estoque-alvo do Pão voltou para 5 dias (ver o `Jogo/LEIA-ME.md`, marco A6). As rodadas CL1 e TP1 foram respondidas pelo autor com "seguir sugestão". As rodadas PO1, CS1, CI1 e CU1 (fase A) foram decididas em **modo automático** em 2026-10-19, a pedido do autor; as decisões estão na parte 10.7 de cada ficha ([[Ficha Logica/Populacao|População]], [[Ficha Logica/ClasseSocial|Classe Social]], [[Ficha Logica/Cidade|Cidade]], [[Ficha Logica/Cura|Cura]]).

A rodada VI1 ([[Ficha Logica/Viagem|Viagem]], fase B1) também foi decidida em modo automático (parte 10.7 da ficha). A OC1 ([[Ficha Logica/OutrasCidades|Outras cidades]], B1b) também. A CO1 ([[Ficha Logica/Comercio|Comércio]], B2) também. A NO1 ([[Ficha Logica/Noticias|Notícias]], B3) também. A RE1 ([[Ficha Logica/Reputacao|Reputação]], B4) também. A TE1 ([[Ficha Logica/Tecnologia|Tecnologia]], B5) também. A FA1 ([[Ficha Logica/Faccao|Facção]], C1) também. A CT1 ([[Ficha Logica/Cultura|Cultura]], C2) também. A CJ1 ([[Ficha Logica/Crime|Crime e Justiça]], C3) também. A RL1 ([[Ficha Logica/Religiao|Religião]], C4) também. A CB1 ([[Ficha Logica/Combate|Combate]], D1) também. A CP1 ([[Ficha Logica/Companheiros|Companheiros e Diálogo]], D2) também. A ES1 ([[Ficha Logica/Escravidao|Escravidão]], D3) também. A MG1 ([[Ficha Logica/Magia|Magia]], D4) também. A DP1 ([[Ficha Logica/Diplomacia|Diplomacia]], D5) também. A BA1 ([[Ficha Logica/Batalha|Batalha]], D6) também. A GU1 ([[Ficha Logica/Guerra|Guerra]], D7) também. A MU1 ([[Ficha Logica/Mundo|Mundo]], E1) também. A IN1 ([[Ficha Logica/Inicio|Início de Jogo]], E2) também. A OB1 ([[Ficha Logica/Objetivo|Objetivo e Progressão]], E3) também.
