---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase A3 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com PO1 vêm da **rodada PO1**, decidida em modo automático (o autor pediu para seguir a sugestão nas dúvidas; parte 10.7). Os valores são iniciais e ficam no `regras.json` para ajustar.

# Ficha Lógica: População

## 1. O que faz

A vila deixa de ter 15 pessoas fixas:

- **Cada NPC tem idade** e envelhece 1 ano por ano de jogo. O velho pode **morrer de velhice**, e um jovem **ocupa a vaga** (reposição).
- **Nascer e morrer em números grandes:** a vila soma nascimentos e mortes **em pessoas** (cada NPC real vale 100 pessoas, o Fator de Escala). A cada 100 a mais, chega 1 NPC; a cada 100 a menos, morre 1.
- **Migração:** a cada estação, 1 pessoa pode chegar (se há casa, emprego e Felicidade) ou ir embora (se a vila está ruim).
- **Fome e frio matam aos poucos**, pelas mesmas contas em pessoas.

Documento do sistema: [[Sistema de População/Descrição|População]].

**Esta versão vale para a vila de nível 1:** capacidade de **15 NPCs reais**, 4 Casas com 16 camas. A cidade subir de nível e construir casas é da ficha da Cidade (fase A5).

## 2. Dados

### Definições (no `data/regras.json`, bloco novo `"populacao"`)

```json
"populacao": {
  "fator_de_escala": 100,
  "capacidade_por_nivel_da_cidade": [15, 40, 100, 300],
  "idade_adulto": 5, "idade_velho": 31,
  "chance_de_morrer_de_velhice_por_estacao_em_porcento_por_ano_de_velhice": 10,
  "natalidade_por_estacao_em_por_mil": 40,
  "mortalidade_por_estacao_em_por_mil": 10,
  "mortalidade_sem_lenha_em_porcento": 150,
  "velhice_no_inverno_rigoroso_em_porcento": 150,
  "fome_mata_por_dia_em_porcento_de_quem_ficou_sem_comida": 2,
  "dias_de_fome_antes_de_matar": 3,
  "migracao": { "chega_com_felicidade_minima": 50, "sai_com_felicidade_abaixo_de": 30,
                "sai_com_desemprego_em_porcento": 30 }
}
```

| Campo | O que é |
|---|---|
| `fator_de_escala` | 1 NPC real = 100 pessoas (Facção). Só conta para a população total e para os nascimentos e mortes; na economia, 1 NPC = 1 trabalhador e 1 consumidor |
| `capacidade_por_nivel_da_cidade` | O máximo de NPCs reais por nível da cidade. A vila é nível 1 |
| `idade_adulto`, `idade_velho` | **Rodada HI12 (2026-10-26):** criança de 0 a 4; adulto de 5 a 30; velho de 31 em diante (antes: adulto de 4 a 20, velho aos 21) |
| `chance_de_morrer_de_velhice...` | 10 = 10% por estação para cada ano de velhice (acima de 30): 31 anos, 10%; 35 anos, 50%; 40 anos, 100% |
| `natalidade_por_estacao_em_por_mil` | 40 = 4% da população por estação (antes dos modificadores) |
| `mortalidade_por_estacao_em_por_mil` | 10 = 1% por estação: as mortes das crianças e dos velhos que não são NPCs |
| `fome_mata_...` | Do 4º dia seguido de fome em diante, morrem por dia 2% das pessoas sem comida |

**No `vila_inicial.json`:** cada NPC ganha a `idade`. **(PO1-1)** Idades de 5 a 19, uma para cada NPC, na ordem da vila (Ana 5, Bento 6, ..., Pedro 19). Nenhum chega a 21 no teste de 1 ano.

### Estado (vai no save)

| Campo | Onde | Exemplo | No save? |
|---|---|---|---|
| `idade` | cada NPC, em anos | 12 | Sim |
| `acumuladoDeCrescimentoEmPessoas` | a vila: nascimentos − mortes, entre −100 e +100 | 37 | Sim |
| `proximoNumeroDeNpc` | a vila: o número do próximo NPC (os números nunca se repetem) | 16 | Sim |
| `diasDeFomeNaEstacao` | a vila | 0 | Sim |
| O sorteio da população | o estado do PCG32, **sequência 4** | — | Sim |

## 3. Quando roda

| Quando | O que roda |
|---|---|
| **A cada dia**, no passo 5 (dias em déficit e Felicidade), depois da Felicidade | A fome e o frio somam mortes no acumulado (regra 4.3) |
| **No dia 1 de cada estação**, no passo "nascimentos e mortes" | Nascimentos e mortalidade da estação (regra 4.2); velhice (regra 4.1); migração (regra 4.4) |
| **No dia 1 da Primavera** | Todo mundo faz 1 ano (regra 4.1, passo 1) |
| **Quando o acumulado passa de +100 ou de −100** | Chega ou morre 1 NPC (regras 4.5 e 4.6) |

## 4. Regras passo a passo

### 4.1 Idade e velhice (dia 1 de cada estação)

1. **No dia 1 da Primavera:** some 1 à idade de cada NPC.
2. **Para cada NPC com 31 anos ou mais** (a idade de velho), na ordem da vila: sorteie de 0 a 99. Se for menor que (idade − 30) × 10, ele morre de velhice (regra 4.6), e **na hora** chega um jovem para a vaga (reposição, regra 4.5, com a idade de adulto, 5). A reposição não mexe no acumulado.
3. **No Inverno rigoroso** (ficha do [[Ficha Logica/Clima|Clima]]), a chance × 150%.

### 4.2 Nascimentos e mortalidade da estação (dia 1 de cada estação)

1. **População total** = NPCs reais × 100.
2. **Nascimentos** = população × 40 ÷ 1.000 × os modificadores ÷ 100 (arredondado para baixo):
   - **Felicidade:** a Felicidade dos Civis × 2, entre 50% e 150% (Felicidade 50 = 100%);
   - **Comida:** 50% se houve algum dia de fome na estação que terminou; senão, 100%.
3. **Mortalidade** = população × 10 ÷ 1.000.
4. Some (nascimentos − mortalidade) ao acumulado.
5. **Depois de uma crise (PO1-4):** se os NPCs reais estão abaixo da capacidade e o acumulado é negativo, ele volta a 0 antes do passo 4.
6. **Vila cheia:** se os NPCs reais já são a capacidade, o acumulado fica preso em +100 no máximo (os que "nasceriam a mais" vão viver em outro lugar). Ele nunca passa de +100 nem de −100: a cada vez que passa, chega ou morre 1 NPC, e tira-se 100.

### 4.3 A fome e o frio (todo dia)

1. **Fome:** se a vila está no 4º dia seguido de déficit de comida ou mais (ficha da [[Ficha Logica/Economia|Economia]], `diasEmDeficitDeComida`), tire do acumulado: Civis sem comida hoje × 100 × 2 ÷ 100. *Exemplo:* 15 sem comida = −30 pessoas por dia; morre 1 NPC a cada 3 a 4 dias de fome.
2. **Frio:** se faltou lenha hoje (ficha do Clima, regra 4.3), tire do acumulado a mortalidade de um dia × 50% a mais: população × 10 ÷ 1.000 ÷ 16 × 50 ÷ 100. *Exemplo:* 1.500 pessoas: 15 ÷ 16 × 0,5 = 0 (arredondado para baixo). **(PO1-2)** Com a vila pequena, o frio quase não mata; ele pesa na Felicidade (−10). Vale mais nas cidades grandes.
3. Some 1 a `diasDeFomeNaEstacao` se houve fome hoje (zera no dia 1 de cada estação, depois da regra 4.2).

### 4.4 Migração (dia 1 de cada estação, depois da regra 4.2)

1. **Chega 1 migrante** se tudo isto vale: há cama livre; há vaga livre de emprego (na Cidade ou na construção do jogador); a Felicidade é 50 ou mais; não houve fome na estação; os NPCs reais estão abaixo da capacidade. Ele chega como na regra 4.5, com idade sorteada de 4 a 12.
2. **Sai 1 NPC** se a Felicidade está abaixo de 30, **ou** o desemprego é de 30% ou mais: vai embora o desempregado de menor habilidade (no empate, o de maior número). Sem desempregados, vai o empregado da Cidade de menor habilidade. Ele anda até a borda do mapa e some (regra 4.6, sem morrer).
- **Ajuste do código (marco A3, 2026-10-20):** a saída do passo 2 é decidida **antes** da velhice e dos nascimentos do dia, com a vila da estação que terminou. Sem isso, uma reposição grande (vários velhos morrem juntos) enchia a vila de desempregados por um dia (a Cidade só contrata no dia seguinte) e expulsava alguém na hora.
- **Ajuste do código:** enquanto o mapa não tiver o ponto `entrada_do_mapa`, o migrante aparece na porta da casa dele, como o nascido.
3. No máximo 1 chega e 1 sai por estação; chega **mais 1** se a ocupação está abaixo de 50% da capacidade (ficha da [[Ficha Logica/Cidade|Cidade]], CI1-5). **Na vila**, o migrante vem de "fora do mapa"; quando houver outras cidades, de uma delas (Viagem, fase B).

### 4.5 Chega um NPC

1. **Onde mora:** a Casa com mais camas livres (no empate, a primeira na ordem da vila). Sem cama livre, ele **não chega**: o acumulado fica em +100 (ou o migrante não vem).
2. **Quem é:** um Civil adulto com o número `proximoNumeroDeNpc` (que soma 1), nome tirado da lista de nomes (`textos`), idade 4 (nascido ou reposição) ou sorteada (migrante), habilidades 50 (o migrante: sorteadas na faixa da classe, 20 a 60 no Civil, PO1-3 e a [[Ficha Logica/ClasseSocial|ficha da Classe Social]]).
3. **Emprego:** chega **desempregado**. A regra 4.1 do [[Ficha Logica/Trabalho|Trabalho]] o contrata no dia seguinte, se houver vaga na Cidade.
4. **Onde aparece:** o nascido e a reposição, na porta da casa; o migrante, no tile de **entrada da vila** (a borda do mapa no fim da estrada; o Mapa ganha o ponto `entrada_do_mapa` no Tiled).
5. Escreva no Diário ("Rita chegou à vila", "Nasceu um novo morador: Tiago").

### 4.6 Sai ou morre um NPC

1. **Quem morre pelo acumulado** (fome ou mortalidade): o NPC **mais velho** (no empate, o de maior número).
2. **O que se desfaz:** o emprego vira vaga (o gerente é escolhido de novo); a cama fica livre; o dinheiro dele (só a riqueza média da classe existe, então nada) e as ações em andamento somem.
3. O NPC sai da lista da vila. **A ordem dos outros não muda**, e o número dele nunca volta a ser usado (o dia de compra e o sorteio dos passeios continuam iguais para os outros).
4. Escreva no Diário ("Olga morreu de velhice aos 23 anos").
5. **O personagem do jogador** não envelhece nem morre nesta versão (a vida dos Controlados fica para a fase de Companheiros).

## 5. Contas

| Conta | Exemplo |
|---|---|
| População total | 15 × 100 = **1.500** |
| Nascimentos de uma estação, Felicidade 58 | 1.500 × 40 ÷ 1.000 = 60; × 116% = **69** |
| Mortalidade de uma estação | 1.500 × 10 ÷ 1.000 = **15** |
| Saldo de uma estação | 69 − 15 = **+54**: com a vila abaixo da capacidade, 1 NPC a cada 2 estações |
| Fome de 15 Civis por 1 dia (do 4º dia em diante) | −30 pessoas |
| Chance de velhice aos 23 anos | (23 − 20) × 10 = **30%** por estação |
| Quanto vive um NPC, em média | Aos 21, 10% por estação (sobrevive ao ano com 66%); aos 22, 20% (41%); aos 23, 30% (24%). A morte vem, em média, aos **22 ou 23 anos** |
| Mortes por velhice numa vila de 15 | Cada NPC vive cerca de 19 anos como adulto: cerca de **0,8 morte por ano** |

## 6. Conversa com outros sistemas

| Sistema | Lê | Entrega |
|---|---|---|
| Tempo | O dia, a estação, o ano | — |
| Economia | `diasEmDeficitDeComida`, os Civis sem comida | O número de consumidores muda |
| Felicidade | A Felicidade dos Civis | O fator Moradia (ninguém fica sem casa nesta versão) |
| Clima | A falta de lenha; o Inverno rigoroso | — |
| Trabalho | As vagas livres; o desemprego | O emprego que vira vaga; o novo desempregado |
| Construção | As Casas e as camas | — |
| NPC e IA | — | O NPC novo (rotina, casa, dia de compra pelo número); o que sai anda até a borda |
| Mapa | O ponto `entrada_do_mapa` | — |
| Save | — | Os campos da parte 2; a lista de NPCs muda de tamanho |
| Simulação sem tela | — | Colunas novas: `npcs`, `populacao`, `acumulado`, `nascimentos`, `mortes`, `migracao` |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Codigo/Vila/Populacao.h`: `RegrasDaPopulacao` | O bloco `"populacao"` |
| `EstadoDaPopulacao` | O acumulado, o próximo número, os dias de fome, o sorteio |
| `IdadeDoNpc` | O componente com a idade |
| `envelhecerEAplicarVelhice(...)` | A regra 4.1 |
| `calcularNascimentosEMortalidadeDaEstacao(...)` | A regra 4.2 |
| `somarMortesPorFomeEFrio(...)` | A regra 4.3 |
| `decidirMigracaoDaEstacao(...)` | A regra 4.4 |
| `criarNpcNaVila(...)`, `tirarNpcDaVila(...)` | As regras 4.5 e 4.6 |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Dia 1 da Primavera do ano 2 | Todos com +1 ano |
| NPC de 31 anos, sorteio 5 | Morre (5 < 10); chega um jovem de 5 anos na mesma casa, desempregado |
| NPC de 31 anos, sorteio 50 | Vive |
| NPC de 30 anos | Morre com qualquer sorteio |
| 15 NPCs, Felicidade 58, sem fome | Acumulado + 54 |
| Vila cheia (15), acumulado 90, + 54 | Fica em 100; ninguém chega |
| 14 NPCs, acumulado 90, + 54 | Chega 1 NPC; acumulado 44 |
| 15 sem comida, 4º dia de fome | Acumulado − 30 |
| Acumulado chega a −100 | Morre o mais velho; acumulado volta a 0 |
| Migração: cama livre, vaga livre, Felicidade 60, sem fome, 14 NPCs | Chega 1 migrante pela entrada da vila |
| Desemprego de 33% (5 de 15) | Sai o desempregado de menor habilidade |
| O NPC 7 morre | Os NPCs 8 a 15 continuam com os mesmos números; o próximo novo é o 16 |
| Teste de 1 ano, 10 sementes | O mesmo de hoje: ninguém chega a 21, e a vila está cheia |

## 9. Fora desta versão

- **Subir de nível e construir casas:** ficha da Cidade (fase A5). Enquanto isso, a vila fica em 15.
- **Moradia por classe** (Senzala, Casarão, Mansão, Palácio): Classe Social (A4) e Escravidão (D).
- **Doença** como causa de morte: ficha da Cura (A6).
- **Guerra e crime** como causa de morte: fases C e D.
- **Vida individual dos Importantes e Controlados** (família, gravidez, herdeiros): fase de Companheiros.
- **Cultura dos migrantes**, Perseguição, fronteira fechada: fases C e D.
- **LOD 2** (soma por estação, sem NPCs): quando houver várias cidades.
- **Crianças decorativas:** não na primeira versão (V3-8).

## 10. Simulação de mesa

### 10.1 Cenário

A vila do teste de 1 ano: 15 Civis (idades 5 a 19), 4 Casas (16 camas), capacidade 15, Felicidade perto de 58, 3 desempregados (20%).

### 10.2 Dez anos em tabela (sem crise)

| Ano | O que acontece |
|---|---|
| 1 | Nada: a vila está cheia, e o acumulado fica em +100. O teste de 1 ano não muda |
| 2 e 3 | Pedro faz 20 e 21 anos. No ano 3, cerca de 34% de chance de morrer; se morre, um jovem de 4 anos ocupa a casa dele, desempregado; o emprego dele vira vaga, e a Cidade contrata um dos desempregados no dia seguinte |
| 4 a 10 | Cerca de 1 morte de velhice por ano (o NPC mais velho de cada vez), sempre com reposição. A vila fica em 15 |

(A simulação de mesa acima usa a regra antiga, com velho aos 21; com a HI12, Pedro só fica velho dez anos depois.)

**O que mostra:** com a vila cheia, a população **não muda de tamanho**: só gira. As idades vão se espalhando (os jovens de 4 anos entram no lugar dos velhos).

**Um efeito escondido:** quem morre é sempre um velho, e quem chega tem habilidade 50 e vem desempregado. Se o morto era um **empregado do jogador**, a vaga do jogador fica vazia até ele contratar (a Cidade não contrata para ele). O Diário avisa.

### 10.3 Uma fome longa (o pior caso)

A vila fica 12 dias sem comida (por exemplo, sem Água):

| Dias de fome | Acumulado (começando em +100) | O que acontece |
|---|---|---|
| 1 a 3 | 100 | Ainda não mata. Evento Fome (−10 na Felicidade) |
| 4 | 70 | — |
| 5 a 7 | 40, 10, −20 | — |
| 8 a 10 | −50, −80, **−110** | **Morre 1 NPC** (o mais velho); acumulado −10 |
| 11 e 12 | −38, −66 | (agora são 14: −28 por dia) |

Depois da fome, na estação seguinte: nascimentos com 50% (houve fome) = 1.400 × 4% × 50% × Felicidade (digamos 40 → 80%) = 22; menos 14 de mortalidade = **+8 por estação**. **Problema:** do −66 até +100, são cerca de **20 estações** (5 anos) para a vila voltar a 15.

A **migração** resolve antes: com Felicidade 50 ou mais, cama livre e vaga livre (o morto deixou uma vaga), chega 1 migrante na estação seguinte. Mas com a Felicidade baixa depois da fome (menos de 50), ninguém vem, e a vila fica com 14 por um bom tempo.

**Decidido (PO1-4):** quando a vila está **abaixo da capacidade**, o acumulado negativo **volta a 0** no dia 1 da estação seguinte à fome (as mortes já aconteceram; a dívida não precisa durar anos). Assim, a vila volta em 2 estações pelos nascimentos, ou em 1 pela migração.

### 10.4 Verificações

| O que nunca pode acontecer | Resultado |
|---|---|
| Mais NPCs que camas | **OK:** regra 4.5, passo 1 |
| Mais NPCs que a capacidade | **OK:** regra 4.2, passo 5, e 4.4, passo 1 |
| O acumulado crescendo sem fim | **OK:** preso entre −100 e +100 |
| Dois NPCs com o mesmo número | **OK:** `proximoNumeroDeNpc` só cresce |
| Um morto com emprego ou cama | **OK:** regra 4.6, passo 2 |
| O jogador morrendo | **OK:** fora desta versão |
| A vila sumindo de vez | **OK:** com 0 NPCs, os nascimentos são 0, mas a migração não exige ninguém; com cama e vaga livres e Felicidade 50, chega 1 por estação. Com a Felicidade presa abaixo de 50 sem ninguém, a vila fica vazia: é o fim da partida, e o teste dá FALHOU antes, pela fome |
| Mesma semente, mortes diferentes | **OK:** gerador próprio, sequência 4 |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Dois NPCs fazem 30 anos na mesma estação | Os dois morrem; duas reposições |
| O gerente morre | A vaga abre, o gerente é escolhido de novo; o salário de gerente passa ao outro |
| Morre o único empregado do Posto | O Posto produz pela metade até a Cidade contratar (no dia seguinte, se houver desempregado) |
| Desemprego 33% e Felicidade 25 na mesma estação | Sai só 1 (máximo por estação) |
| Um NPC morre andando na rua | Some do mapa na hora; o caminho dele é descartado |

### 10.6 O que muda no teste de 1 ano

Nada, se as idades da vila inicial forem as da PO1-1. O relatório ganha as colunas da parte 6, para conferir.

### 10.7 Decidido em modo automático (rodada PO1, seguir sugestão)

1. **PO1-1:** as idades da vila inicial vão de 5 a 19, uma por NPC (sorteá-las mudaria o teste de 1 ano a cada semente).
2. **PO1-2:** o frio usa a mortalidade da estação +50%; na vila pequena quase não mata. Não ganhou regra extra.
3. **PO1-3:** o nascido e a reposição têm habilidade 50; o migrante, sorteada na faixa da classe (20 a 60 no Civil). Assim a redistribuição da ficha do Trabalho passa a ter efeito.
4. **PO1-4:** abaixo da capacidade, o acumulado negativo volta a 0 no dia 1 da estação seguinte (parte 10.3).
