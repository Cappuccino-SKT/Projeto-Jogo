---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 6** (produção, inventário e consumo). Cobre **o dinheiro, o consumo diário e os salários**. O Mercado Local, o estabilizador e o imposto entram nesta mesma ficha antes do marco 7. Os pontos marcados com FL6 estão na **rodada FL6** ([[Retomada]], seção 4), ainda sem resposta: a ficha já usa as sugestões, marcadas como [Proposta].

# Ficha Lógica: Economia (consumo e salários)

## 1. O que faz

Todo dia, à meia-noite, a vila **come** (a cesta mínima sai do inventário, e a classe paga ao dono do item) e os empregados **recebem** pelas horas trabalhadas (o dono da construção paga). É o ciclo do dinheiro: **os consumidores gastam nas lojas, as lojas pagam os salários, e os trabalhadores gastam de novo**.

Documentos do sistema: [[Sistema Econômico/Descrição|Econômico]] (a moeda, o preço base, as lojas), [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]] (os salários, os donos) e [[Sistema de Classe Social/Descrição|Classe Social]] (a cesta mínima, a regra do gasto); as regras 4.4 e 4.5 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]].

**O dinheiro é sempre inteiro, em décimos de moeda** (V2-1): 17 = 1,7 moeda.

## 2. Dados

### Definições

Já no `regras.json`: o salário por hora em centésimos (75, 100, 150; FL3-3). Novos [Proposta]:

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
- O dinheiro do primeiro dia vem do `vila_inicial.json` [Proposta] (FL6-4): `"banco_da_cidade": 4000`, `"dinheiro_do_jogador": 1000`, `"dinheiro_inicial_por_civil": 120`.

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
   - **sem dinheiro**, o item sai do mesmo jeito, e o dono não recebe a parte que faltou [Proposta] (FL6-1): a conta vai para o `consumoNaoPagoHoje`, que o estabilizador do marco 7 usa.
4. **Dias em déficit:** se faltou comida para alguém, some 1 a `diasEmDeficitDeComida`; se não, zere. O mesmo para os Materiais.
5. **O gasto extra** (a regra do gasto, até 80% da renda) fica para o marco 7 [Proposta] (FL6-1): na fatia, não há comida de Tier 2 nem Bebida para comprar, e para onde vai esse dinheiro é o que o teste de 1 ano vai medir.

### 4.2 Salários (a cada dia)

Para cada NPC empregado, com as horas que ele trabalhou no dia (`ticksTrabalhadosHoje`, da Produção):

1. **O salário:**
   > horas trabalhadas × salário por hora do Trabalho × bônus de nível × gerente
   - o Trabalho é o da receita da construção (ou o `trabalho_sem_receita`: o Lojista é Manufaturado, o Operário é Braçal);
   - bônus de nível = +10% por nível acima do 1 (Trabalho e Propriedade);
   - gerente = × 1,5 (ficha da Construção, regra 4.2);
   - os multiplicadores da riqueza da cidade e do desemprego ficam em × 1 no marco 6 [Proposta] (FL6-1); as fórmulas deles entram no marco 7;
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

- **No marco 7:** o Mercado Local (o preço que anda com o estoque), o estabilizador, o gasto extra, o imposto, os multiplicadores da riqueza e do desemprego e a saída dos empregados depois de 1 estação em atraso.
- **No marco 8:** o preço do jogador (±30%), o nível de salário (Baixo, Normal, Alto), a contratação.
- As outras classes e a cesta delas; a Bebida e a Taverna; a Tora como lenha no inverno; os Mercados da Facção e Mundial; os Ricos.
