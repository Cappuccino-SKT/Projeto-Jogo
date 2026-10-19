---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 6** (produção, inventário e consumo). Cobre **o dinheiro, o consumo diário e os salários**. A parte 10 traz o **marco 7**: o Mercado Local, o gasto extra, o imposto, o estabilizador, os multiplicadores do salário e a saída por salário atrasado. Os pontos marcados com FL6 foram decididos na **rodada FL6** (2026-10-18, toda com "seguir sugestão").

# Ficha Lógica: Economia (consumo e salários)

## 1. O que faz

Todo dia, à meia-noite, a vila **come** (a cesta mínima sai do inventário, e a classe paga ao dono do item) e os empregados **recebem** pelas horas trabalhadas (o dono da construção paga). É o ciclo do dinheiro: **os consumidores gastam nas lojas, as lojas pagam os salários, e os trabalhadores gastam de novo**.

Documentos do sistema: [[Sistema Econômico/Descrição|Econômico]] (a moeda, o preço base, as lojas), [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]] (os salários, os donos) e [[Sistema de Classe Social/Descrição|Classe Social]] (a cesta mínima, a regra do gasto); as regras 4.4 e 4.5 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]].

**O dinheiro é sempre inteiro, em décimos de moeda** (V2-1): 17 = 1,7 moeda.

## 2. Dados

### Definições

Já no `regras.json`: o salário por hora em centésimos (75, 100, 150; FL3-3). Novos (FL6-1):

```json
"cesta_minima_civil": { "comida_por_dia": 1, "materiais_a_cada_dias": 8 },
"loja_da_cidade_vende_em_porcento": 110,
"bonus_do_salario_por_nivel_em_porcento": 10,
"salario_do_gerente_em_porcento": 150,
"ordem_do_dia": ["consumo", "salarios", "escolha_das_receitas"]
```

- A **ordem do dia** fica no `regras.json` (FT1-5), para ser trocada nos testes. No marco 6 ela tem só os 3 passos que já existem (o 1, o 2 e o 7 da Fatia Vertical); o marco 7 acrescenta os outros (Mercado Local, estabilizador, Felicidade, eventos). Um nome desconhecido é erro.
- **Que item é comida e que item é Material Básico** sai do tipo do item (Classe Social, IP2-11): `alimento` é comida; `mobilia` (e, depois, `vestuario`) é Material Básico.

### Estado (muda durante o jogo)

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `bancoDaCidade` | dinheiro | 4.000 (400 moedas) | Sim |
| `dinheiroDoJogador` | dinheiro | 1.000 (100 moedas) | Sim |
| `dinheiroDaClasseCivil` | dinheiro (a soma dos 15) | 1.800 (12 moedas por Civil) | Sim |
| `rendaDoDiaDaClasseCivil` | dinheiro | 840 (os salários de ontem) | Sim |
| `diasEmDeficitDeComida`, `diasEmDeficitDeMateriais` | inteiros | 0 | Sim |
| `materiaisDevidosEmOitavos` | inteiro, de 0 a 7 | 7 | Sim |
| `consumoNaoPagoHoje` | dinheiro | 0 | Não: só para o console e o marco 7 |
| Cada construção: `diasDeSalarioAtrasado` | inteiro | 0 | Sim |

- A **riqueza média** do Civil é o `dinheiroDaClasseCivil` ÷ 15: o jogo guarda a soma, para nunca perder décimos na divisão.
- O dinheiro do primeiro dia vem do `vila_inicial.json` (FL6-4): `"banco_da_cidade": 4000`, `"dinheiro_do_jogador": 1000`, `"dinheiro_inicial_por_civil": 120`.

## 3. Quando roda

À **meia-noite** (a virada do dia, FT1-5), na ordem do `ordem_do_dia`:

1. **Consumo** (regra 4.1).
2. **Salários** (regra 4.2), das horas trabalhadas no dia que acabou.
3. **Escolha das receitas** da cidade ([[Ficha Logica/Producao|Produção]], regra 4.3).

E, durante o dia, quando a Carpintaria do jogador **compra insumos** da Cidade (Produção, regra 4.2).

## 4. Regras passo a passo

### 4.1 Consumo (a cada dia)

Para a classe Civil (a única da fatia):

1. **Comida:** para cada NPC, tire 1 item de comida do inventário: o **mais barato**; no empate, o da Cidade antes do do jogador. Na fatia, é o Pão.
2. **Materiais Básicos:** some o número de NPCs a `materiaisDevidosEmOitavos`. Enquanto ele for 8 ou mais, tire 1 Material Básico (o mais barato; na fatia, os Móveis de Madeira do jogador) e subtraia 8. Assim, 15 NPCs gastam 1,875 Móveis por dia, em média, sem número com vírgula.
3. **Pagar ao dono:** cada item tirado é pago do `dinheiroDaClasseCivil`:
   - item da **Cidade**: o preço de venda da loja da cidade, **110% do preço** (V2-5), vai para o `bancoDaCidade`;
   - item do **jogador**: o preço do item, que ele fixará na faixa de ±30% no marco 8 (até lá, o preço base), vai para o `dinheiroDoJogador`;
   - **sem dinheiro**, o item sai do mesmo jeito, e o dono não recebe a parte que faltou (FL6-1): a conta vai para o `consumoNaoPagoHoje`, que o estabilizador do marco 7 usa.
4. **Dias em déficit:** se faltou comida para alguém, some 1 a `diasEmDeficitDeComida`; se não, zere. O mesmo para os Materiais.
5. **O gasto extra** (a regra do gasto, até 80% da renda) fica para o marco 7 (FL6-1): na fatia, não há comida de Tier 2 nem Bebida para comprar, e para onde vai esse dinheiro é o que o teste de 1 ano vai medir.

### 4.2 Salários (a cada dia)

Para cada NPC empregado, com as horas que ele trabalhou no dia (`ticksTrabalhadosHoje`, da Produção):

1. **O salário:**
   > horas trabalhadas × salário por hora do Trabalho × bônus de nível × gerente
   - o Trabalho é o da receita da construção (ou o `trabalho_sem_receita`: o Lojista é Manufaturado, o Operário é Braçal);
   - bônus de nível = +10% por nível acima do 1 (Trabalho e Propriedade);
   - gerente = × 1,5 (ficha da Construção, regra 4.2);
   - os multiplicadores da riqueza da cidade e do desemprego ficam em × 1 no marco 6 (FL6-1); as fórmulas deles entram no marco 7;
   - nas construções do jogador, o nível de salário Normal (× 1); Baixo e Alto entram com a interface (marco 8).
2. **Quem paga:** nas construções da Cidade, o `bancoDaCidade`; nas do jogador, o `dinheiroDoJogador`.
3. **Sem dinheiro para o salário inteiro:** o empregado não recebe nada, e a construção soma 1 a `diasDeSalarioAtrasado` (Fatia Vertical, 4.5). Os empregados saírem depois de 1 estação em atraso entra no marco 7.
4. **Quem recebe:** o `dinheiroDaClasseCivil`; e o total do dia vira a `rendaDoDiaDaClasseCivil`.
5. Zere o `ticksTrabalhadosHoje` de todos.

## 5. Contas

**O salário em décimos, sem vírgula:** ticks trabalhados × salário em centésimos por hora × bônus de nível (%) × gerente (%) ÷ (800 × 10 × 100 × 100), com a metade arredondando para cima (como o preço, FL3-3).

*Exemplo:* um Cozinheiro com 7,9 horas (6.320 ticks), Manufaturado (100 centésimos), nível 1, sem ser gerente: 6.320 × 100 × 100 × 100 ÷ 80.000.000 = **79 décimos** (7,9 moedas).

**O dinheiro de um dia da vila** (com 7,9 horas de trabalho, FL6-7):

| Quem | Entra | Sai |
|---|---|---|
| Classe Civil | Salários: cerca de **106 moedas** (12 empregados; 6 gerentes a × 1,5) | 15 Pães × 1,9 = **28,5**; 1,875 Móveis × 5,1 = **9,6** |
| Banco da Cidade | Os Pães: **28,5** | Os salários da Cidade (10 empregados): cerca de **84** |
| Jogador | Os Móveis: **9,6** | Os salários da Carpintaria (2, nível 2): cerca de **22**; a Tora comprada: cerca de **2** |

**O que isso mostra:** sem o gasto extra e sem o imposto (marco 7), o Banco perde cerca de **55 moedas por dia** e zera em uns **7 dias**; o jogador perde cerca de 14 por dia e zera em uns 7 dias; e a classe Civil junta cerca de 68 por dia. É exatamente o que o estabilizador, o gasto extra e o imposto existem para fechar, e o teste de 1 ano (marco 7) mede. O marco 6 só precisa que o dinheiro **ande certo**: ele vai mostrar o buraco no console, de propósito.

## 6. Conversa com outros sistemas

| Sistema | O que a Economia lê | O que entrega |
|---|---|---|
| Inventário | A comida e os Materiais | Tira o que se come |
| Produção | As horas trabalhadas | O dinheiro do jogador (para comprar insumos) |
| Construção | O dono, o nível, o gerente | — |
| Itens (marco 3) | O preço base, o tipo do item | — |
| Felicidade (marco 7) | — | Os dias em déficit; a renda |
| Mercado Local e estabilizador (marco 7) | — | O estoque, a riqueza média, o consumo não pago |
| Interface (marco 8) | — | O dinheiro no HUD; o painel da cidade |

## 7. Nomes no código

Em `Codigo/Economia/`:

| Nome | O que é |
|---|---|
| `DinheiroDaVila` | O Banco, o dinheiro do jogador, o da classe Civil, a renda e os dias em déficit |
| `RegrasDaEconomia` | A cesta mínima, a loja da cidade (110%), o bônus de nível, o gerente e a ordem do dia |
| `consumirACestaDoDia(...)` | A regra 4.1 |
| `pagarOsSalariosDoDia(...)` | A regra 4.2 |
| `calcularSalarioEmDecimos(ticks, centesimosPorHora, bonusDeNivel, ehGerente)` | A conta da parte 5 |
| `rodarAOrdemDoDia(...)` | Os passos do `ordem_do_dia`, na ordem |
| `montarResumoDoDia(...)` | A linha do console (FL6-8) |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Salário de 6.320 ticks, Manufaturado, nível 1 | 79 décimos |
| O mesmo, gerente | 119 décimos (118,5, a metade sobe) |
| O mesmo, nível 2 | 87 décimos (86,9) |
| 15 NPCs, 45 Pães da Cidade a 1,7 | 30 Pães sobram; a classe paga 15 × 19 = 285 décimos ao Banco (1,87 arredondado para 1,9) |
| 15 NPCs, só 10 Pães | Os 10 saem; 1 dia em déficit de comida |
| No dia seguinte, com Pão para todos | Os dias em déficit voltam a 0 |
| Materiais em 8 dias, 15 NPCs | 15 Móveis no total (1 ou 2 por dia) |
| A classe sem dinheiro | A comida sai do mesmo jeito; o consumo não pago é anotado |
| O Banco sem dinheiro para os salários | Os empregados da Cidade não recebem; 1 dia de atraso |
| Uma ordem do dia com um passo desconhecido | Erro no `regras.json` |
| Um dia inteiro sem tela | O dinheiro total (Banco + jogador + classe) só muda pelo consumo não pago: nada some e nada aparece do nada |

## 9. Fora da fatia (e fora do marco 6)

- **No marco 7:** tudo na parte 10.
- **No marco 8:** tudo na parte 11 (o preço do jogador, o nível de salário, a contratação, a compra e a venda).
- As outras classes e a cesta delas; a Bebida e a Taverna; a Tora como lenha no inverno; os Mercados da Facção e Mundial; os Ricos.

---

## 10. Marco 7: o Mercado Local, o gasto extra, o imposto e o estabilizador

> Parte escrita em 2026-10-18 para o **marco 7** (a simulação sem tela de 1 ano). Ela fecha o ciclo do dinheiro que o marco 6 deixou aberto: no marco 6, o Banco perde cerca de 54 moedas por dia (parte 5). Os pontos marcados com FL7 vêm da **rodada FL7**, aplicada em 2026-10-18, toda com "seguir sugestão".

### 10.1 O que muda

| Regra | No marco 6 | No marco 7 |
|---|---|---|
| Preço de cada item | O preço base | O **preço local**, que anda com o estoque (Mercado Local, V2-5) |
| O que a classe gasta | Só a cesta mínima | A cesta e o **gasto extra**, até 80% da renda (Classe Social) |
| Para onde vai o gasto extra | — | Os Materiais a mais e, o resto, a loja da cidade, para o Banco (FL7-1) |
| Imposto | Nenhum | 20% dos salários, retido a cada dia, todo para o Banco (FL7-2) |
| Estabilizador | Nenhum | O Banco ajuda a classe pobre (V2-7) |
| Salário | Nível e gerente | E a riqueza da cidade e o desemprego (FL7-3) |
| Salário atrasado | Só conta os dias | Depois de 16 dias, os empregados saem (Fatia Vertical, 4.5) |

**Por que o dinheiro fecha assim** (o modelo de 64 dias da FL7-1): com o imposto de 20% e o gasto de 80%, a classe gasta **toda** a renda bruta, e tudo o que ela gasta, menos os Móveis, volta ao Banco. A conta do Banco num dia fica: + o Pão + o gasto extra + o imposto − os salários da Cidade ≈ 0. O Banco fica perto de 400 moedas nos 64 dias, o jogador perde cerca de 0,4 moeda por dia, e a riqueza média dos Civis fica em 12.

### 10.2 Dados

**No `regras.json`** (novos):

```json
"mercado_local": { "dias_do_estoque_alvo": 5, "fator_minimo_em_porcento": 50, "fator_maximo_em_porcento": 200,
                   "passo_maximo_por_dia_em_porcento": 10 },
"loja_da_cidade_compra_em_porcento": 90,
"gasto_da_classe_em_porcento_da_renda": 80,
"imposto": { "taxa_em_porcento": 20, "parte_da_cidade_em_porcento": 100 },
"estabilizador": { "ajuda_abaixo_de_dias_da_cesta": 3, "ajuda_ate_dias_da_cesta": 5, "teto_por_estacao_em_porcento_do_banco": 10 },
"multiplicadores_do_salario": { "riqueza_minima_em_porcento": 80, "riqueza_maxima_em_porcento": 120, "dias_da_cesta_para_o_maximo": 10,
                                "desemprego_minimo_em_porcento": 90, "desemprego_maximo_em_porcento": 110 },
"dias_de_atraso_ate_os_empregados_sairem": 16,
"ordem_do_dia": ["consumo", "salarios", "mercado_local", "estabilizador", "felicidade", "eventos", "escolha_das_receitas"]
```

A `ordem_do_dia` ganha os 4 passos que faltavam (FT1-5), na ordem da Fatia Vertical (parte 3).

**Estado novo:**

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `precoLocalPorItem` | dinheiro, um por item | Pão: 19 | Sim |
| `consumoDosUltimosDiasPorItem` | 5 números por item, em centésimos de unidade (FL7-4) | Tora: 800, 800, 0, 1.600, 800 | Sim |
| `bancoNoComecoDaEstacao` | dinheiro | 4.000 | Sim |
| `ajudaDoEstabilizadorNaEstacao` | dinheiro | 0 | Sim |
| `gastoExtraDeHoje`, `gastoDaCestaDeHoje` | dinheiro | 240, 440 | Não: só para a Felicidade e o relatório |
| `impostoDeHoje` | dinheiro | 210 | Não |

No primeiro dia, o preço local de cada item é o preço base, e os 5 dias de consumo são a previsão do marco 6 (as receitas atuais).

### 10.3 Quando roda

À meia-noite, na ordem do `ordem_do_dia`:

1. **Consumo** (regra 4.1, com o gasto extra da regra 10.5).
2. **Salários** (regra 4.2, com os multiplicadores, o imposto e a saída por atraso: regras 10.6 a 10.8).
3. **Mercado Local** (regra 10.4).
4. **Estabilizador** (regra 10.9).
5. **Felicidade** ([[Ficha Logica/Felicidade|ficha da Felicidade]]).
6. **Eventos** ([[Ficha Logica/Eventos|ficha dos Eventos]]).
7. **Escolha das receitas** ([[Ficha Logica/Producao|Produção]], regra 4.3, agora com os preços locais).

### 10.4 O Mercado Local (a cada dia)

Para cada item:

1. **O estoque-alvo** = 5 × o consumo previsto de um dia (a média dos 5 últimos dias, FL7-4), em centésimos.
2. **O fator**, em porcentagem = 100 + (alvo − estoque) × 100 ÷ alvo, preso entre 50 e 200. O estoque conta os dois donos. Com alvo 0 (ninguém usa o item, como a Pedra), o fator é 100.
3. **O preço desejado** = preço base × fator ÷ 100.
4. **O passo:** o preço local anda na direção do desejado, no máximo 10% do preço local do dia (arredondado, a metade subindo, e no mínimo 1 décimo), sem passar do desejado.

**Onde o preço local vale** (no lugar do preço base):

- a loja da cidade vende a 110% dele e, a partir do marco 8, compra a 90%;
- os itens do jogador são pagos pelo preço local (até o marco 8, quando o jogador fixa a faixa de ±30%);
- a compra de insumos da Carpintaria do jogador (110% do preço local);
- a nota da escolha das receitas da cidade.

*Exemplo:* o Pão com 45 no estoque e alvo 75: fator = 100 + 30 × 100 ÷ 75 = 140; desejado = 17 × 140 ÷ 100 = 23,8 décimos. O passo máximo é 10% de 17 = 1,7, arredondado para 2: o preço vai de 1,7 a 1,9 no primeiro dia, a 2,1 no segundo, e chega a 2,4 no quarto dia.

### 10.5 O gasto extra (no passo do consumo)

Depois da cesta mínima (regra 4.1):

1. **O teto do dia** = 80% da renda do dia da classe (os salários brutos do dia anterior, antes do imposto).
2. **O que sobra para gastar** = o teto − o que a cesta custou hoje, sem passar do dinheiro da classe. Se der zero ou menos, não há gasto extra.
3. **Materiais a mais:** a classe compra até **o mesmo número de Materiais Básicos que a cesta tirou hoje** (o mais barato; no empate, o da Cidade), pagando ao dono, enquanto houver dinheiro para gastar (FL7-1).
4. **O resto vai para a loja da cidade** (o Banco): é o que a classe gastaria com a comida de Tier 2, a Bebida e os serviços que a fatia ainda não tem. Nenhum item sai do inventário (FL7-1).
5. Guarde o `gastoExtraDeHoje` (a soma dos passos 3 e 4) e o `gastoDaCestaDeHoje`, para a Felicidade (até +5).

*Exemplo:* a renda de ontem foi 85 moedas: o teto é 68. A cesta custou 15 Pães × 2,1 + 2 Móveis × 5,1 = 41,7. Sobram 26,3: 2 Móveis a mais (10,2, para o jogador) e 16,1 para o Banco.

### 10.6 Os multiplicadores do salário (no passo dos salários)

Os dois entram na conta da parte 5, como mais duas porcentagens (FL7-3):

- **riqueza da cidade** = 80 + 40 × riqueza média ÷ (10 dias da cesta de um Civil), presa entre 80 e 120. A cesta de um dia é o Pão na loja da cidade mais um oitavo do Móvel, nos preços locais. Riqueza de 5 dias da cesta dá 100;
- **desemprego** = 110 − a taxa de desemprego (em %), presa entre 90 e 110. Na fatia, 3 desempregados em 15 dão 20%, e o multiplicador é 90.

*Exemplo:* o Cozinheiro com 6.320 ticks (79 décimos no marco 6), com a riqueza média de 12,0 e a cesta de 2,54 por dia: riqueza = 80 + 40 × 12,0 ÷ 25,4 = 98 (arredondado para baixo); desemprego 90. O salário = 79 × 0,98 × 0,90 = 69,7, arredondado para **70 décimos**.

### 10.7 O imposto (no passo dos salários)

Cada salário pago tem 20% retido na hora, e esse dinheiro vai para o Banco da Cidade (na fatia, sem o Banco da Facção, a cidade fica com tudo) (FL7-2). O imposto é arredondado por salário, a metade subindo. O empregado recebe o resto. O lucro do jogador ainda não paga imposto (entra com a interface do marco 8).

*Exemplo:* os 70 décimos do Cozinheiro: 14 de imposto para o Banco, 56 para a classe. A renda do dia (o teto do gasto extra) conta os 70.

### 10.8 A saída por salário atrasado (no passo dos salários)

Quando uma construção chega a **16 dias seguidos** de salário atrasado (uma estação), os empregados dela saem: viram desempregados, e o `diasDeSalarioAtrasado` volta a 0 (Fatia Vertical, 4.5). Na fatia, ninguém é contratado de novo até a interface do marco 8.

### 10.9 O estabilizador (a cada dia)

1. No dia 1 de cada estação, guarde o `bancoNoComecoDaEstacao` e zere a `ajudaDoEstabilizadorNaEstacao`.
2. Se a riqueza média da classe está abaixo de **3 dias da cesta** de um Civil, o Banco paga à classe o que falta para chegar a **5 dias** (V2-7).
3. O total pago na estação nunca passa de **10% do `bancoNoComecoDaEstacao`**, nem do que o Banco tem.

*Exemplo:* a cesta de um dia custa 2,54; 3 dias são 7,6 por Civil. Com a riqueza média em 6,0 (90 para os 15), o Banco paga até a classe chegar a 5 dias: 12,7 × 15 − 90 = 100,5 moedas, se couber nos 10% da estação (40 moedas, com o Banco em 400 no começo dela): paga 40.

### 10.10 Contas

**O dinheiro de um dia, no marco 7** (o modelo da FL7-1, com a cidade inteira trabalhando):

| Quem | Entra | Sai |
|---|---|---|
| Classe Civil | Salários de cerca de 85 moedas (com os multiplicadores) | Imposto de 17; a cesta de cerca de 44; o gasto extra de cerca de 24 |
| Banco da Cidade | O Pão (cerca de 34), o gasto extra que não é Móvel (cerca de 14), o imposto (17), a Tora do jogador (cerca de 1) | Os salários da Cidade (cerca de 66) |
| Jogador | Os Móveis da cesta e os a mais (cerca de 19) | Os salários da Carpintaria (cerca de 20) |

**Os resultados do modelo, em 64 dias:**

| Opção | Banco no fim | Jogador no fim | Riqueza média |
|---|---|---|---|
| Hoje (marco 6) | 20 (vazio no dia 8) | 10 | 43 |
| **Sugestão** (gasto extra no Banco, imposto 20%, todo da cidade) | **428** (o menor: 398) | 72 | 12 |
| O mesmo, com a cidade ficando com 30% do imposto (a V2-7) | 22 | 10 | 5 |
| O gasto extra só em Móveis | 30 | 19 | 42 |
| Imposto de 10% | 24 | 10 | 43 |

### 10.11 Testes

| Entrada | Resultado esperado |
|---|---|
| Pão com 45 no estoque, alvo 75, preço 1,7 | Fator 140; desejado 2,38; o preço vai a 1,9 |
| Pão com 200 no estoque, alvo 75 | Fator 50 (preso); o preço desce 10% por dia até 0,85 |
| Pedra sem consumo (alvo 0) | Fator 100; o preço fica no base |
| Renda de 85, cesta de 41,7, 2 Móveis na cesta | 2 Móveis a mais (10,2 para o jogador) e 16,1 para o Banco |
| A classe sem dinheiro | Não há gasto extra |
| Salário do Cozinheiro com riqueza 12,0, cesta 2,54 e desemprego de 20% | 70 décimos; 14 de imposto |
| A riqueza média em 6,0 (abaixo de 3 dias) | O Banco paga até o teto da estação (10% do Banco do começo dela) |
| Uma construção com 16 dias de atraso | Os empregados viram desempregados |
| Uma ordem do dia com "mercado_local" antes de "consumo" | Roda nessa ordem (a ordem é do arquivo) |
| 64 dias sem tela | O Banco nunca fica abaixo de 250 e, no dia 64, no máximo 50 abaixo do dia 1 (FE4-3); o dinheiro total só muda pelo Mercador viajante e pela verba da Coroa (as entradas de fora) |

---

## 11. Marco 8: as decisões do jogador

> Escrita em 2026-10-19. Decidida na rodada **FL8** (2026-10-19, toda com "seguir sugestão"; ver a parte 8 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]). Os botões estão na parte 10 da [[Ficha Logica/Interface|Interface]]; o pagamento das obras, na parte 10 da [[Ficha Logica/Construcao|Construção]]. Todas as decisões chegam como **pedidos** (Interface, regra 10.8).

### 11.1 Dados

Novos no `regras.json`:

```json
"decisoes_do_jogador": {
  "faixa_do_preco_em_porcento": 30,
  "passo_do_preco_em_porcento": 5,
  "salario_baixo_em_porcento": 75, "salario_alto_em_porcento": 125,
  "velocidade_com_salario_baixo_em_porcento": 90, "velocidade_com_salario_alto_em_porcento": 110,
  "dias_de_salario_baixo_para_um_empregado_sair": 8,
  "loja_da_cidade_compra_em_porcento": 90
}
```

**Estado novo** (vai no save, marco 9): na vila, `porcentagemDoPrecoPorItem` (de −30 a +30, começa em 0; vale para o item em todas as construções do jogador); em cada construção do jogador, `nivelDeSalario` (Baixo, Normal, Alto; começa em Normal) e `diasSeguidosComSalarioBaixo`.

Com tudo no padrão (0% e Normal) e sem pedidos, o marco 8 dá o mesmo resultado do marco 7, a não ser pela manutenção.

### 11.2 O preço do jogador (FL8-6)

1. **O preço** de um item do jogador = o preço local × (100 + p) ÷ 100, arredondado para décimos (meio para cima), com p de −30 a +30, de 5 em 5.
2. **A cesta mínima** compra sempre, por esse preço (é o mínimo).
3. **O gasto extra em Materiais** (regra 10.5) compra uma fração do que compraria: com p ≤ 0, tudo; com p > 0, (30 − p) ÷ 30 (arredondado para baixo, em itens inteiros). O dinheiro que sobra vai para a loja da cidade, como o resto do gasto extra.
4. **A Cidade**, na manutenção, compra as Tábuas por esse preço, sem fração (Construção, regra 10.6).
5. O preço local continua andando pelo estoque (regra 10.4), igual para todos os donos.

*Exemplo:* os Móveis a 5,1 no local; o jogador põe +20%: 6,1. A cesta compra os 2 do dia por 6,1; o gasto extra queria 2 a mais e compra 2 × 10 ÷ 30 = 0,67 → 0. O jogador ganha 12,2 em vez de 20,4.

**Ajuste da FE3 (2026-10-19):** o `multiplicador_do_preco` dos Móveis de Madeira passou de 100 para **140** (`itens.csv`), para o jogador ganhar cerca de +3,5 a +4,5 moedas por dia (antes, +0,45). Os exemplos desta ficha continuam com o preço antigo (5,1) porque só ilustram a conta. O custo é do Banco: o dinheiro do jogador sai dele, e o Banco termina o ano 1 entre 110 e 175 (antes, perto de 400); ele **não fecha o ano 2** sem uma fonte de fora. **[Refinar depois]** (FE3-2): o Construtor comprar as Tábuas do jogador; uma fonte de dinheiro para o Banco no jogo completo (o Mercador, as exportações).

**A verba da Coroa (FE4-1 e FE4-2, 2026-10-19):** no dia 1 de cada estação, no passo "estabilizador" da ordem do dia e antes de o teto recomeçar, entram de fora no Banco **75 moedas** (`verba_da_coroa_por_estacao_em_decimos`: 750 no `regras.json`), fixas, contadas em `dinheiroQueEntrouDeFora`. Elas cobrem o que o jogador ganha (FE3): medido no `--simular 64`, o Banco termina o ano em 475 (semente 1) e 408 (semente 2), nunca abaixo de 295, e o jogador segue com +3,4 e +4,4 por dia. **[Refinar depois]** a exportação da sobra pelo Mercado (FE4-1 b) quando houver mais itens; a verba pela Felicidade (FE4-2 c); no jogo completo, a verba e a parte da Facção no imposto (V2-7) pensadas juntas.

**O preço do jogador (FE3-3):** na vila de hoje ninguém mais vende o que o jogador faz, então o preço 0% é sempre o melhor (+15% dá 16,6 por dia; −15% dá 16,1; 0% dá 19,1). A interface avisa com uma dica. **[Refinar depois]** o preço passa a valer com concorrência (outra Carpintaria, o Mercador) ou procura que cresce com o preço baixo.

### 11.3 O nível de salário (FL8-7)

| Nível | Salário | Velocidade dos empregados | Outro efeito |
|---|---|---|---|
| Baixo | × 0,75 | × 0,9 | Depois de 8 dias seguidos em Baixo, o último empregado da lista (o de maior número na vila) sai (vira desempregado), e a contagem recomeça |
| Normal | × 1 | × 1 | — |
| Alto | × 1,25 | × 1,1 | — |

- O multiplicador entra **junto** com os outros do salário (o bônus de nível, o gerente × 1,5, a riqueza e o desemprego), antes do arredondamento.
- A velocidade multiplica a do ciclo da produção (regra 4.2 da [[Ficha Logica/Producao|Produção]]).
- Trocar o nível vale a partir do próximo pagamento; trocar para Normal ou Alto zera a contagem dos dias.

### 11.4 Contratar e demitir (FL8-7)

1. **Contratar:** só um desempregado, e só se houver vaga no nível da construção. Ele passa a ser empregado na hora, e a rotina dele leva ao trabalho a partir do próximo horário de trabalho.
2. **Demitir:** o empregado vira desempregado na hora, sem indenização; as horas que ele já trabalhou no dia não são pagas.
3. O **gerente** (V3-8) continua sendo o primeiro da lista de empregados; se ele sai, o próximo vira gerente.
4. O desemprego da vila muda na hora (e com ele o multiplicador do salário do dia seguinte).

### 11.5 Comprar e vender na loja da cidade (FL8-8)

Pela aba Mercado, sem o personagem andar.

1. **Comprar:** o preço de cada item = o local × 110% (meio para cima, em décimos); o total = o preço × a quantidade. Pode se a Cidade tiver o item e o jogador tiver o dinheiro. O item passa da Cidade para o jogador, e o dinheiro, do jogador para o Banco.
2. **Vender:** o preço = o local × 90%; pode se o jogador tiver o item e a capacidade da Cidade couber. O item passa do jogador para a Cidade, e o dinheiro, do Banco para o jogador. Se o Banco não tiver o dinheiro, a venda é recusada.
3. Os itens que a Cidade compra entram no estoque dela e mudam o preço local no próximo dia, como qualquer estoque.

*Exemplo:* o jogador vende 10 Tábuas com o local em 1,5: 1,5 × 90% = 1,35 → 1,4; recebe 14,0.

### 11.6 Contas

| Conta | Valor |
|---|---|
| Móveis a 5,1, p = −30, −15, 0, +15, +30 | 3,6; 4,3; 5,1; 5,9; 6,6 (meio para cima) |
| Fração do gasto extra com p = +5, +15, +25 | 25/30, 15/30, 5/30 |
| Manufaturado da Carpintaria nível 2, Baixo / Alto | 1,0 × 1,1 (o bônus do nível) × 0,75 = 0,825 / × 1,25 = 1,375 por hora |
| Comprar 10 Blocos com o local em 1,8 | 1,8 × 110% = 1,98 → 2,0; total 20,0 |

### 11.7 Nomes no código

| Nome | O que é |
|---|---|
| `DecisoesDoJogador` | O estado da regra 11.1 em cada construção do jogador |
| `NivelDeSalario` (`Baixo`, `Normal`, `Alto`) | O nível |
| `precoDoItemDoJogador(precoLocal, porcentagem)` | Regra 11.2, passo 1 |
| `fracaoDoGastoExtraComOPreco(porcentagem)` | Regra 11.2, passo 3, em trigésimos |
| `comprarDaLojaDaCidade`, `venderParaALojaDaCidade` | Regra 11.5 |
| `RegrasDasDecisoesDoJogador` | O bloco `"decisoes_do_jogador"` do `regras.json` |

### 11.8 Testes

| Situação | Resultado esperado |
|---|---|
| `precoDoItemDoJogador(51, 20)` | 61 décimos |
| `precoDoItemDoJogador(51, -30)` | 36 décimos |
| A porcentagem +7 | Recusada (só de 5 em 5) |
| Os Móveis a +30% num dia com gasto extra | A cesta compra; o gasto extra não compra Móveis e vai todo para a loja da cidade |
| 8 dias em Baixo com 2 empregados | No dia 8, o último da lista vira desempregado |
| Contratar com as vagas cheias | Recusado |
| Demitir o gerente | O outro empregado vira gerente |
| Vender 10 Tábuas a 1,5 | +14,0 para o jogador, −14,0 do Banco, +10 Tábuas da Cidade |
| Comprar 200 Blocos com 150 na Cidade | Recusado |
| O dinheiro total depois de cada pedido | Igual |
