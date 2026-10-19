---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 6** (produção, inventário e consumo). Cobre **o ciclo de produção, o trabalho de cada empregado, a escolha da receita da cidade e as ordens do jogador**. Os pontos marcados com FL6 foram decididos na **rodada FL6** (2026-10-18, toda com "seguir sugestão").

# Ficha Lógica: Produção

## 1. O que faz

Cada construção de produção tem uma **receita atual**. Os empregados que estão **trabalhando** somam horas ao **ciclo**; quando o ciclo chega às horas da receita, o **lote** entra no inventário. As da cidade escolhem a receita **todo dia**, pela nota da IA; as do jogador seguem a **fila de ordens**.

Documentos do sistema: [[Sistema de Produção/Descrição|Produção]] (a receita padrão, o ciclo, a velocidade, a nota da IA, as ordens), as [[Sistema de Produção/Receitas|Receitas]] e a regra 4.2 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]. As receitas da fatia já são lidas desde o marco 3 ([[Ficha Logica/Receitas|ficha das Receitas]]); a estação e a produtividade, na [[Ficha Logica/Recursos|ficha dos Recursos]].

**A divisão das pastas:** tudo em `Codigo/Producao/`, sem janela (o teste de 1 ano do marco 7 roda isso).

## 2. Dados

### Definições

Já lidas (marco 3): as receitas (produto, lote, insumos, construção, nível mínimo, Trabalho, habilidade mínima, horas), os recursos (o item, a área do Tiled, o grupo da estação) e as porcentagens da estação. Nada novo no `receitas.csv`.

**As habilidades** dos NPCs, no `vila_inicial.json` (FL6-6): `"habilidades": { "bracal": 50, "manufaturado": 50 }` em cada NPC.

**As ordens do jogador**, no `vila_inicial.json` (FL6-3), na Carpintaria dele (até a interface do marco 8):

```json
"ordens": [
  { "tipo": "manter_estoque", "item": "item.moveis_madeira", "quantidade": 6 },
  { "tipo": "manter_estoque", "item": "item.tabua", "quantidade": 12 }
]
```

### Estado

**Em cada construção de produção:**

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `receitaDoCiclo` | a receita, ou nenhuma | `receita.pao` | Sim |
| `receitaEscolhidaParaOProximoCiclo` | a receita, ou nenhuma | `receita.pao` | Sim |
| `cicloComecou` | sim/não (os insumos já saíram) | sim | Sim |
| `progressoDoCiclo` | inteiro, em **ticks × porcentagem** (parte 5) | 160.000 (metade de um Pão) | Sim |
| `restoDaProducaoBracal` | inteiro, de 0 a 99 | 50 (meio Grão no inverno) | Sim |
| `estaParada` e o motivo | sim/não e texto | "sem Grão", "o estoque encheu" | Não |
| As ordens (só do jogador) | lista | parte 2 | Sim |

**Em cada NPC** (com os componentes dos marcos 4 e 5):

| Componente | Campo | Exemplo | Vai no save? |
|---|---|---|---|
| `HabilidadesDoNpc` | `bracal`, `manufaturado` (de 0 a 100) | 50, 50 | Sim |
| `TrabalhoDoDia` | `ticksTrabalhadosNestaHora`, `ticksTrabalhadosHoje` | 800, 6.300 | Sim |

## 3. Quando roda

| Quando | O que roda |
|---|---|
| **A cada tick** | Quem está trabalhando soma 1 tick (regra 4.1) |
| **A cada hora** (na virada) | O ciclo de cada construção (regra 4.2) |
| **A cada dia**, no passo 7 da ordem do dia (FT1-5) | A escolha da receita da cidade (regra 4.3) |
| **Ao começar um ciclo** | A escolha pela fila de ordens, nas construções do jogador (regra 4.4) |

## 4. Regras passo a passo

### 4.1 Quem está trabalhando (a cada tick)

Um empregado está trabalhando quando a ação dele é **Trabalhar** e:

- nas construções **Braçais** (Fazenda, Posto de Coleta), ele está **parado num tile da área do recurso da receita** (o campo de Grão, o poço, o bosque). É o que a ficha do Mapa já dizia: "as áreas de recurso (onde os Braçais trabalham)". Ele fica visível no campo;
- nas outras, ele está **dentro** da construção (marco 5).

**O almoço conta como trabalho** (FL6-7): quem está no lugar de trabalho com a ação **Comer** (a refeição das 12h) também está trabalhando. O Braçal almoça no campo, sem voltar para casa.

Se está trabalhando, some 1 a `ticksTrabalhadosNestaHora` e a `ticksTrabalhadosHoje`.

**Para onde o Braçal vai** (muda a regra 4.3 da [[Ficha Logica/IA|ficha da IA]]): na ação Trabalhar, o destino é um tile livre da área do recurso da receita atual da construção dele; o primeiro empregado fica no primeiro tile livre da área (de cima para baixo, da esquerda para a direita), o segundo no segundo, e assim por diante, para não ficarem um em cima do outro. Se a construção não tem receita, ele vai para a porta e entra, como os outros. Quando a receita muda de área (o Posto passa da Água para a Tora), o Braçal vê isso na reavaliação seguinte dele (a cada 5 minutos de jogo) e anda até o tile novo [Proposta].

### 4.2 O ciclo (a cada hora)

Para cada construção de produção da vila:

1. **Sem receita** (`receitaDoCiclo` vazia): pegue a `receitaEscolhidaParaOProximoCiclo` (na do jogador, a regra 4.4 escolhe agora). Se não houver, a construção fica parada ("sem receita").
2. **O ciclo não começou:** tente tirar os **insumos** do inventário (tudo ou nada, regra 4.2 do Inventário):
   - nas da Cidade, os itens da Cidade;
   - nas do jogador, os itens do jogador; o que faltar, ele **compra** da Cidade, pelo preço de venda da cidade (ficha da Economia), se tiver dinheiro (FL6-3);
   - se faltar, a construção fica parada ("sem Grão") e tenta de novo na hora seguinte. As receitas Braçais não têm insumo;
   - numa hora em que ninguém trabalhou (de noite), o ciclo não começa: os insumos só saem quando há trabalho para somar [Proposta].
3. **Some ao progresso** o trabalho de cada empregado na última hora:
   > ticks trabalhados na hora × velocidade × bônus de nível
   - velocidade, em porcentagem = 50 + a habilidade do Trabalho da receita (habilidade 50 = 100%);
   - bônus de nível, em porcentagem = 100 + 10 × (nível da construção − nível mínimo da receita).

   Depois zere o `ticksTrabalhadosNestaHora` de cada um.
4. **O ciclo terminou** quando o progresso chega a **horas da receita × 800 × 100** (parte 5):
   1. a quantidade é o **lote**; nas Braçais, o lote × o fator da estação e da produtividade (ficha dos Recursos), com o que sobrar de 100 guardado no `restoDaProducaoBracal` para o próximo ciclo;
   2. tente **pôr** a quantidade no inventário, com o dono da construção. Se não couber, a construção fica parada ("o estoque encheu"), com o progresso cheio, e tenta de novo na hora seguinte;
   3. se coube: tire as horas da receita do progresso (a sobra fica para o próximo ciclo), marque o ciclo como não começado e passe para a `receitaEscolhidaParaOProximoCiclo`;
   4. se o progresso ainda dá para outro ciclo inteiro (muitos empregados), repita o passo 4 (no máximo 10 vezes por hora).
5. **Um ciclo interrompido** (fim do turno, empregado faltando) guarda o progresso, e os insumos já gastos não voltam (Produção).

### 4.3 A escolha da receita da cidade (a cada dia, no passo 7)

Para cada construção de produção da **Cidade**:

1. **As candidatas:** as receitas da construção com o nível mínimo atendido e que **pelo menos um empregado** consegue fazer (habilidade ≥ a mínima). Uma receita Braçal só é candidata se o mapa tem a área do recurso dela [Proposta]: o Posto de Coleta da fatia tem a receita da Pedra (FL6-5), mas a vila não tem pedreira, então ele nunca a escolhe.
2. **A nota de cada uma** (Produção, IA):
   > nota = (preço do produto − custo dos insumos) × (100 + déficit) ÷ 100
   - os preços são os **preços base** no marco 6 (o Mercado Local entra no marco 7);
   - o **déficit** do produto, em porcentagem = 100 − estoque × 100 ÷ estoque-alvo, preso entre 0 e 100; o estoque conta os dois donos juntos [Proposta];
   - o **estoque-alvo** = 5 dias do consumo previsto do item (como o Mercado Local, V2-5): a cesta mínima da vila mais os insumos que as receitas atuais de todas as construções gastam num dia cheio (8 horas por empregado). Com alvo 0 (ninguém usa o item), o déficit é 0.
3. **A comida em perigo:** se o estoque de comida da cidade é menor que **3 dias** da cesta mínima, as receitas de comida ganham sempre.
4. Escolha a de **maior nota**; no empate, a primeira do `receitas.csv`. Ela vira a `receitaEscolhidaParaOProximoCiclo`: o ciclo atual termina com a receita dele.

Ao montar a vila, a escolha roda uma vez, antes do primeiro tick.

### 4.4 A fila de ordens do jogador (ao começar um ciclo)

As ordens valem na ordem da lista (a primeira tem a maior prioridade):

1. Para cada ordem, veja se ela **quer produzir agora**:
   - **Manter estoque de N:** o jogador tem menos de N do item;
   - **Fazer N:** ainda faltam itens para chegar a N (a ordem guarda quantos já fez; com N feitos, ela sai da lista);
   - **Repetir sempre:** sempre.
2. A receita do item precisa estar liberada (nível) e ter um empregado capaz, e os insumos precisam existir (do jogador, ou à venda na Cidade com o dinheiro dele).
3. A primeira ordem que passa nos passos 1 e 2 dá a receita do ciclo. Se nenhuma passa, a construção fica parada ("sem ordem").

*Exemplo:* com as ordens da parte 2, a Carpintaria faz Móveis enquanto o jogador tem menos de 6 Móveis e pelo menos 3 Tábuas; sem Tábuas, a primeira ordem não passa, e ela faz Tábuas.

## 5. Contas

**O progresso em ticks × porcentagem:** 1 hora de trabalho normal (habilidade 50, sem bônus) = 800 ticks × 100% = **80.000**. Um Pão (4 horas) precisa de **320.000**. Assim, tudo é inteiro, e a sobra de um ciclo passa para o seguinte sem erro.

*Exemplo:* uma Cozinheira com habilidade 60 num Cozinheiro nível 1 trabalha 1 hora inteira: 800 × 110 × 100 ÷ 100 = 88.000. Um Pão sai em 320.000 ÷ 88.000 = 3,6 horas.

**O que a vila produz num dia**, com habilidade 50 (FL6-6) e cerca de **7,9 horas** de trabalho por empregado (FL6-7):

| Construção | Conta | Por dia | A vila gasta |
|---|---|---|---|
| Cozinheiro (2) | 2 × 7,9 h ÷ 4 h × 4 Pães | **15,8 Pães** | 15 Pães |
| Fazenda (2) | 2 × 7,9 h × 2 Grãos | 31,6 Grãos (25% no inverno: 7,9) | 7,9 Grãos (no Pão) |
| Posto de Coleta (2) | 2 × 7,9 h × 2 | 31,6 de Água **ou** Tora | 4 Águas e cerca de 6 Toras |
| Carpintaria do jogador (2, nível 2) | Tábua: 4 por 4 h, com +10% de nível; Móveis: 2 por 4 h | Cerca de 2 Móveis (1,9 por dia é o consumo) e o resto em Tábuas | 1,9 Móveis |

**Sem o café às 6h** (com o café às 7h da FL5-4), o empregado trabalha cerca de 6,9 horas, e o Cozinheiro faz **13,8 Pães por dia**, menos que os 15 que a vila come: em 3 semanas, a vila passaria fome. Por isso, a FL6-7 passou o café para as 6h e contou o almoço como trabalho.

**O déficit e a nota** (a escolha do Posto de Coleta): Água e Tora valem 0,5 e não têm insumo. Com Água 16 e alvo 20 (4 por dia × 5), o déficit é 100 − 80 = 20%, e a nota é 0,5 × 1,2 = 0,6. Com Tora 4 e alvo 30, o déficit é 87%, e a nota 0,93: o Posto tira Tora.

## 6. Conversa com outros sistemas

| Sistema | O que a Produção lê | O que entrega |
|---|---|---|
| IA (marco 5) | Quem está Trabalhando e onde | O destino do Braçal (a área) |
| Inventário | Os insumos; o espaço | Tira os insumos, põe o lote |
| Recursos (marco 3) | A área de cada recurso, a estação, a produtividade | — |
| Construção | O nível, as vagas, o dono | — |
| Economia | Os preços (na nota), o dinheiro do jogador (insumos comprados) | As horas trabalhadas no dia (salários) |
| Tempo | A hora nova, o dia novo, a estação | — |
| Interface (marco 8) | — | A receita, o progresso e o motivo da parada; a fila de ordens |

## 7. Nomes no código

Em `Codigo/Producao/`:

| Nome | O que é |
|---|---|
| `ProducaoDaConstrucao` | O estado da parte 2, um por construção de produção |
| `OrdemDoJogador` e `TipoDeOrdem` | `ManterEstoque`, `FazerQuantidade`, `RepetirSempre` |
| `HabilidadesDoNpc`, `TrabalhoDoDia` | Os componentes do NPC |
| `contarOsTicksTrabalhados(...)` | A regra 4.1 |
| `rodarOsCiclosDaHora(...)` | A regra 4.2 |
| `escolherAsReceitasDaCidade(...)` | A regra 4.3 |
| `escolherReceitaPelaFilaDeOrdens(...)` | A regra 4.4 |
| `calcularNotaDaReceita(...)` e `calcularDeficitDoItem(...)` | As contas da regra 4.3 |
| `progressoNecessarioDoCiclo(receita)` | horas × 800 × 100 |
| `tileDeTrabalhoNaArea(mapa, area, posicaoDoEmpregado)` | O tile do Braçal (regra 4.1) |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Progresso de um Pão | 320.000 |
| 1 hora de uma Cozinheira com habilidade 60, nível 1 | +88.000 |
| 1 hora com habilidade 50 numa Carpintaria nível 2 fazendo Tábua (mínimo 1) | +88.000 (bônus de 10%) |
| Cozinheiro com 2 Grão e 0 Água no estoque | Parado ("sem Água"); o Grão continua lá |
| Ciclo de Pão terminado com a capacidade cheia | Parado ("o estoque encheu"); o Pão não some |
| Fazenda no inverno (25%), 4 ciclos de 2 Grãos | 2 Grãos no total (0,5 por ciclo, com o resto guardado) |
| Posto de Coleta com Água 16 (alvo 20) e Tora 4 (alvo 30) | Escolhe Tora |
| Comida abaixo de 3 dias | O Cozinheiro escolhe Pão mesmo com nota menor |
| Ordens: manter 6 Móveis e 12 Tábuas; o jogador tem 2 Móveis e 1 Tábua | Faz Tábua (não há 3 Tábuas para os Móveis) |
| O mesmo, com 5 Tábuas | Faz Móveis |
| Um Braçal fora da área, com a ação Trabalhar | Não soma ticks |
| Três dias sem tela, com a vila da fatia | Depois do primeiro dia, o Cozinheiro faz pelo menos 15 Pães por dia (16; o primeiro dia fecha só 3 ciclos, e a sobra passa para o seguinte); a vila não fica sem Pão |

## 9. Fora da fatia (e fora do marco 6)

- O Rank do que sai (o teto do material, do nível e da habilidade, e a sorte): a fatia só tem itens de Tier.
- Os subprodutos, o "exige no estoque" e o insumo opcional (IP3-2).
- A produção em LOD 2 (por estação).
- As receitas de Ricos e as Funções de Especialista e Mago de Ofício.
- A animação de trabalhar (o boneco parado no campo, por enquanto).

---

## 10. Marco 7: a previsão do consumo e a folga

> Parte escrita em 2026-10-18 para o **marco 7**, com os 2 problemas que os 30 dias sem tela do marco 6 mostraram (a Tora que acaba no dia 12 e o estoque de matéria-prima cheio no dia 17). Os pontos marcados com FL7 vêm da **rodada FL7**, aplicada em 2026-10-18, toda com "seguir sugestão".

### 10.1 A previsão do consumo, medida (FL7-4)

**O problema:** no marco 6, o estoque-alvo da regra 4.3 conta só os insumos das receitas **atuais**. A Carpintaria do jogador passa a maior parte do tempo nos Móveis (que gastam Tábua), então a previsão de Tora quase sempre é 0, o Posto de Coleta fica na Água, e a Tora acaba.

**A regra nova:** o consumo previsto de cada item é a **média dos 5 últimos dias** do que a vila **gastou ou quis gastar**:

1. **Gastou:** o que saiu do inventário para a cesta mínima, o gasto extra e os insumos dos ciclos.
2. **Quis gastar e faltou:** em cada hora em que uma construção, com gente trabalhando, ficou parada por falta de um insumo, conta a parte de um ciclo daquela hora: quantidade do insumo × ticks trabalhados na hora × 100 ÷ (horas da receita × 800), em centésimos.
3. **A fila do jogador:** quando nenhuma ordem passa, mas alguma **quer produzir** e só falta insumo, a construção fica parada "sem Tora" (e não "sem ordem"), e cada ordem que quer produzir conta, no passo 2, os insumos que faltam a ela. Se os insumos existem na Cidade e só falta dinheiro ao jogador, a parada é "sem dinheiro para os insumos", sem demanda [Proposta].
4. **A cesta que faltou** também é demanda: cada comida ou Material Básico que a cesta não achou conta 1 unidade do item mais barato do tipo (assim, na fome, o alvo do Pão não cai) [Proposta].

O mesmo consumo previsto vale para o **Mercado Local** (o estoque-alvo do preço, ficha da [[Ficha Logica/Economia|Economia]], regra 10.4). No primeiro dia, os 5 dias guardados são a previsão do marco 6.

*Exemplo:* sem Tora, a Carpintaria passa 8 horas parada "sem Tora" com 2 empregados (1.600 ticks por hora no total): por hora, 2 Toras × 1.600 × 100 ÷ (4 × 800) = 100 centésimos (1 Tora); no dia, 8 Toras de demanda que faltou. O alvo da Tora sobe, e o Posto de Coleta escolhe a Tora no dia seguinte.

### 10.2 A folga (FL7-5)

**O problema:** com déficit 0, a nota da regra 4.3 continua positiva, então os Braçais nunca param. A Fazenda faz cerca de 31,6 Grãos por dia, e a vila gasta 8. O Grão e a Água enchem os 900 de matéria-prima por volta do dia 17, e a produção trava com "o estoque encheu". O Pão também sobe sem parar (15,8 feitos e 15 comidos por dia), e o preço local dele cairia para menos de 1,2, o que falha o teste de 1 ano.

**A regra nova**, na escolha das receitas da cidade (regra 4.3, passo 1):

1. Uma receita **não é candidata** se o produto já tem **125% do estoque-alvo** ou mais (6 dias e 6 horas de consumo).
2. Depois de sair, ela só volta a ser candidata quando o estoque cai **abaixo do alvo** (a histerese, para não ligar e desligar todo dia).
3. Sem nenhuma candidata, a construção fica **de folga** no dia seguinte.
4. A construção do jogador sem nenhuma ordem que queira produzir, na meia-noite, também fica de folga.
5. **De folga, os empregados não vão trabalhar**: passeiam no Período de Trabalho, como os desempregados, e **não recebem** (o salário é pelas horas trabalhadas).
6. **O ciclo que ainda não começou** já é o próximo: na Cidade, ele passa para a receita escolhida (ou para nenhuma, na folga); no jogador, a fila decide de novo na hora seguinte [Proposta].
7. **Ao montar a vila**, ninguém começa de folga, e a primeira escolha roda duas vezes: a previsão do marco 6 precisa das receitas atuais, que só existem depois da primeira escolha [Proposta].

*Exemplo:* o alvo do Pão é 75 (5 dias de 15). Com 94 Pães, o Cozinheiro folga; com 79, continua de folga (está acima do alvo); com 64, volta. O estoque de Pão fica entre cerca de 60 e 95, e o preço local, entre cerca de 1,3 e 2,0. A Fazenda, com alvo de 40 Grãos, trabalha cerca de 1 dia em cada 4.

### 10.3 Testes

| Entrada | Resultado esperado |
|---|---|
| A Carpintaria 8 horas parada "sem Tora" com 2 empregados | 8 Toras de demanda que faltou no dia |
| Tora com 0 de estoque e demanda medida de 8 por dia | O Posto de Coleta escolhe a Tora |
| A fila do jogador sem Tora nem Tábua | Parada "sem Tábua" (a primeira ordem que quer produzir), e não "sem ordem" |
| Pão com 94 (alvo 75) | O Cozinheiro fica de folga; os 2 empregados passeiam e não recebem |
| Pão com 79, depois de folgar | Continua de folga |
| Pão com 64 | Volta a produzir |
| 64 dias sem tela | A matéria-prima nunca enche; o preço do Pão fica entre 1,2 e 2,6 |
