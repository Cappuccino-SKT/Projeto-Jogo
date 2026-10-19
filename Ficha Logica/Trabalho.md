---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase A2 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com TP1 vêm da **rodada TP1**, aplicada em 2026-10-19, toda com "seguir sugestão".

# Ficha Lógica: Trabalho e Propriedade

## 1. O que faz

Completa o mercado de trabalho e a propriedade da vila:

- **A Cidade contrata sozinha:** uma vaga vazia numa construção da Cidade é preenchida por um desempregado.
- **A cada estação, a Cidade redistribui** quem está no Trabalho errado (um bom cozinheiro na Fazenda).
- **O jogador compra e vende construções** da Cidade.
- **Falência e leilão:** uma construção parada há 2 estações vai a leilão; sem lance, volta para a Cidade.

**Já existe e esta ficha só usa** (ficha da [[Ficha Logica/Economia|Economia]]): o dono de cada construção; o salário por hora com o nível, o gerente, a riqueza e o desemprego; o imposto de 20%; a saída depois de 16 dias de salário atrasado; o nível de salário do jogador; o contratar e o demitir do jogador; o gerente (ficha da [[Ficha Logica/Construcao|Construção]], regra 4.2).

Documento do sistema: [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]].

## 2. Dados

### Definições (no `data/regras.json`, bloco novo `"trabalho"`)

```json
"trabalho": {
  "contratar_se_o_banco_tem_dias_de_salario": 3,
  "diferenca_minima_para_trocar": 10,
  "valor_da_construcao_em_porcento_da_obra": 100,
  "cidade_compra_de_volta_em_porcento": 75,
  "estacoes_parada_para_o_leilao": 2,
  "dias_de_leilao": 3,
  "lance_minimo_em_porcento_do_valor": 50
}
```

| Campo | O que é |
|---|---|
| `contratar_se_o_banco_tem_dias_de_salario` | A Cidade só contrata se o Banco paga 3 dias do novo salário (regra 4.1) |
| `diferenca_minima_para_trocar` | Na redistribuição, a troca só vale se a soma das habilidades usadas sobe 10 ou mais (regra 4.2) |
| `valor_da_construcao_em_porcento_da_obra` | O valor de uma construção = 100% do custo da obra dela (regra 4.3) |
| `cidade_compra_de_volta_em_porcento` | O jogador vende à Cidade por 75% do valor |
| `estacoes_parada_para_o_leilao`, `dias_de_leilao`, `lance_minimo_em_porcento_do_valor` | V3-8 |

### Estado (vai no save)

| Campo | Onde | Exemplo | No save? |
|---|---|---|---|
| `saiuPorAtrasoDe` | cada NPC: a construção que ele deixou por salário atrasado nesta estação, ou nenhuma | Posto de Coleta | Sim |
| `diasSemEmpregados` | cada construção | 0 | Sim |
| `modoAutomatico` | cada construção do jogador: verdadeiro ou falso | verdadeiro | Sim |
| `salarioDevidoEmDecimos` | cada construção: a soma dos salários brutos não pagos (regra 4.4) | 0 | Sim |
| `leilao` | a construção em leilão, os dias que faltam, o maior lance e quem deu | Carpintaria, 2 dias, 0 | Sim |

## 3. Quando roda

| Quando | O que roda |
|---|---|
| **A cada dia**, no passo 2 (salários), **depois** dos pagamentos e da saída por atraso | Preencher as vagas da Cidade (regra 4.1); contar os dias sem empregados (regra 4.4) |
| **No dia 1 de cada estação**, no passo da redistribuição dos empregos | A redistribuição (regra 4.2); zerar o `saiuPorAtrasoDe` de todos |
| **A qualquer hora** (pelo painel da construção) | Comprar e vender construções (regra 4.3); dar lance (regra 4.4) |
| **A cada dia**, no passo 6 (Eventos) | Abrir, contar e fechar o leilão (regra 4.4) |

## 4. Regras passo a passo

### 4.1 A Cidade preenche as vagas vazias (todo dia, TP1-1)

Para cada construção **da Cidade**, na ordem da vila, enquanto houver vaga vazia:

1. **O Banco aguenta?** Se o Banco não tem 3 dias do salário da vaga (salário por hora × 8 h × 3), pare: a Cidade não contrata sem dinheiro.
2. **Quem:** entre os desempregados, o de **maior habilidade** no Trabalho da construção. No empate, o de menor número na vila.
3. **Quem não pode:** quem saiu desta mesma construção por salário atrasado nesta estação (`saiuPorAtrasoDe`). Assim não há o "sai e volta no dia seguinte".
4. Ele vira empregado, como no contratar do jogador (Economia, regra 11.4): vai trabalhar a partir do próximo horário de trabalho. O gerente é escolhido de novo (Construção, regra 4.2).
5. Escreva no Diário ("Bruno foi contratado na Fazenda").

**As construções do jogador** não são preenchidas sozinhas: quem contrata é o jogador.

### 4.2 A redistribuição (dia 1 de cada estação, TP1-2)

Só entre **empregados da Cidade**. Os do jogador nunca são movidos.

1. Para cada par de empregados da Cidade (A e B) em construções de **Trabalhos diferentes**, na ordem da vila: some a habilidade de A no Trabalho de B e a de B no Trabalho de A.
2. Se a soma **depois** da troca for **10 ou mais** maior que a soma de agora, A e B trocam de emprego.
3. Repita até nenhum par trocar (no máximo uma volta por NPC, para nunca ficar em laço).
4. **Os desempregados não entram (TP1-2):** a redistribuição nunca tira o emprego de alguém. Um desempregado melhor espera uma vaga (regra 4.1).
5. Escolha os gerentes de novo e escreva as trocas no Diário.

*Na fatia, todos têm habilidade 50 em tudo:* nada muda. A regra só faz diferença quando as habilidades variarem (vila inicial nova ou a educação).

### 4.3 Comprar e vender construções (TP1-3)

**O valor de uma construção** = o custo da obra do porte dela ([[Ficha Logica/Construcao|Construção]], `obra_por_porte`) a preço de hoje × o nível:

> (Tábuas × preço local + Blocos de Pedra × preço local + horas da obra × 0,75) × nível

1. **Comprar da Cidade:** o jogador escolhe uma construção da Cidade de **produção** (não o Mercado nem as casas). Pode se tiver o dinheiro e se ela não estiver em leilão. O dinheiro vai para o Banco; o dono passa a ser o jogador. **Os empregados continuam**, agora pagos pelo jogador.
2. **O modo automático (TP1-3):** a construção comprada começa com `modoAutomatico` ligado: a receita do dia é escolhida pela IA da Cidade (Produção, regra 4.3), como antes. O jogador pode desligar e usar a fila de ordens. As construções que o jogador já tinha começam com o modo desligado.
3. **Vender à Cidade:** pelo valor × 75%, se o Banco tiver o dinheiro. O dono passa a ser a Cidade, os empregados continuam, e o modo volta para a IA da Cidade. O que estiver na fila de ordens é apagado.
4. **O estoque:** o inventário é da Cidade (compartilhado); só o que está no nome do jogador continua dele.

### 4.4 Falência e leilão (V3-8)

0. **A dívida:** cada dia em que o salário não é pago (Economia, regra 4.2, passo 3), o salário bruto de cada empregado soma ao `salarioDevidoEmDecimos` da construção. Ela só zera quando paga (no leilão) ou quando a construção muda de dono.
1. **Parada:** uma construção **sem nenhum empregado** soma 1 a `diasSemEmpregados` por dia; com algum empregado, zera.
2. **Ao chegar a 32 dias** (2 estações), abre o leilão por 3 dias e escreve no Diário. Só construções **do jogador** vão a leilão; uma da Cidade parada fica esperando a regra 4.1.
3. **Lance:** quem pode dar lance são os Ricos da cidade (não existem ainda: Classe Social, fase A4) e o jogador. O lance mínimo é 50% do valor (regra 4.3). O dinheiro do lance fica preso até o fim.
4. **No fim:**
   - **Com lance (TP1-4):** a construção vai para quem deu o maior lance. O dinheiro paga primeiro o `salarioDevidoEmDecimos` à classe dos empregados (na vila, os Civis), e o resto vai para o jogador. Se o lance não cobre a dívida, paga o que dá, e o resto da dívida some.
   - **Sem lance:** a construção volta para a Cidade, sem pagamento. A regra 4.1 a preenche no dia seguinte.
5. **O jogador dono** pode, durante a parada (antes do leilão), contratar e voltar a produzir: a contagem zera.

*Injetar dinheiro* (documento): na vila, o dinheiro do jogador é um só, sem caixa por construção. Pagar os salários já é "injetar". Fica para quando existir o caixa de cada construção.

## 5. Contas

| Conta | Exemplo |
|---|---|
| Valor do Posto de Coleta (pequeno, nível 1), Tábua 1,5 e Bloco 1,5 | (10 × 1,5 + 10 × 1,5 + 40 × 0,75) × 1 = **60 moedas** |
| Valor da Carpintaria do jogador (média, nível 2) | (20 × 1,5 + 30 × 1,5 + 120 × 0,75) × 2 = **330 moedas** |
| Vender a Carpintaria à Cidade | 330 × 75% = **247,5** |
| Lance mínimo no leilão da Carpintaria | 330 × 50% = **165** |
| O Banco aguenta contratar um Coletor? | 0,75 × 8 × 3 = **18 moedas** no Banco, no mínimo |
| Troca na redistribuição: Bruno (Braçal 40, Manufaturado 70) na Fazenda; Carla (Braçal 65, Manufaturado 45) no Cozinheiro | Agora 40 + 45 = 85; depois 70 + 65 = 135; +50 ≥ 10: **trocam** |
| A velocidade deles depois da troca | Bruno no Cozinheiro: 50 + 70 = 120% (era 95% da Carla); Carla na Fazenda: 115% (era 90% do Bruno) |

## 6. Conversa com outros sistemas

| Sistema | Lê | Entrega |
|---|---|---|
| Economia | O dinheiro do jogador e do Banco; o salário por hora; o preço local da Tábua e do Bloco | O pagamento das compras, vendas e lances |
| Construção | O dono, o nível, o porte, as vagas, a obra por porte | O dono novo; o gerente escolhido de novo |
| Produção | — | O modo automático (a IA escolhe a receita de uma construção do jogador) |
| NPC e IA | A habilidade, o emprego | O emprego novo (a rotina leva ao trabalho novo) |
| Felicidade | — | O desemprego muda (fator Desemprego e renda) |
| Interface | — | No painel da construção: comprar, vender, o modo automático, o leilão; o Diário |
| Save | — | Os campos da parte 2 |
| Simulação sem tela | — | Colunas novas: `contratacoes`, `trocas`, `leilao` |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Codigo/Vila/Trabalho.h`: `RegrasDoTrabalho` | O bloco `"trabalho"` do `regras.json` |
| `preencherVagasDaCidade(vila, regras, banco)` | A regra 4.1 |
| `redistribuirEmpregosDaCidade(vila, regras)` | A regra 4.2 |
| `calcularValorDaConstrucao(construcao, regrasDasConstrucoes, mercado)` | A conta da regra 4.3 |
| `comprarConstrucaoDaCidade(...)`, `venderConstrucaoParaACidade(...)` | A regra 4.3 |
| `LeilaoDeConstrucao`, `avancarLeilaoDoDia(...)`, `darLanceNoLeilao(...)` | A regra 4.4 |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Vaga vazia na Fazenda; desempregados com Braçal 40 e 60 | Contrata o de 60 |
| Empate de habilidade | O de menor número na vila |
| Banco com 10 moedas; vaga de Coletor (18 para 3 dias) | Não contrata |
| O NPC que saiu do Posto por atraso hoje; única vaga é no Posto | Não volta nesta estação; volta no dia 1 da próxima, se a vaga continuar |
| Vaga vazia na Carpintaria do jogador | A Cidade não preenche |
| Redistribuição com todos em 50 | Nada muda |
| Bruno e Carla do exemplo da parte 5 | Trocam; os gerentes são escolhidos de novo |
| Comprar o Posto com 50 moedas (valor 60) | Recusado |
| Comprar o Posto com 100 moedas | Jogador com 40; Banco + 60; empregados continuam; modo automático ligado |
| Vender a Carpintaria com o Banco em 200 (preço 247,5) | Recusado |
| Carpintaria sem empregados por 32 dias | Abre o leilão |
| Leilão sem lance | Volta para a Cidade; no dia seguinte, os desempregados são contratados |
| Lance de 165; dívida de salários de 64 | 64 para os Civis; 101 para o jogador |
| Lance de 165; dívida de 200 | 165 para os Civis; a dívida zera |
| Teste de 1 ano, 10 sementes, sem o jogador mexer | O mesmo resultado de hoje (a fatia não tem vaga vazia na Cidade) |

## 9. Fora desta versão

- **Ricos e empresas** (30% das construções de produção), a mobilidade Civil → Rico e o lance dos Ricos: com a Classe Social (fase A4).
- **Escravos** e o Mercado de Escravos: fase D.
- **Guardas e soldo dos Esquadrões:** fases C e D.
- **Imposto por estação, 30% para a cidade e o resto para a Facção:** a vila retém por dia e fica com tudo (FL7-2) até existir a Facção.
- **Universidade e estudo:** com a Tecnologia (fase B5).
- **O quadro de empregos da Taverna:** quando houver Taverna.
- **Caixa por construção** e "injetar dinheiro".
- **Migração** por desemprego ou vagas sobrando: com a População (fase A3).
- **LOD 2** (o cálculo por cidade e por estação): quando houver mais de uma cidade.

## 10. Simulação de mesa

### 10.1 Cenário

A vila do teste de 1 ano (semente 1): **15 Civis**, 12 empregados (10 da Cidade e 2 do jogador) e 3 desempregados (20%). As 5 construções de produção da Cidade (Fazenda, Posto de Coleta, Construtor, Cozinheiro e Mercado) têm 2 vagas cada, todas cheias. A Carpintaria do jogador tem 3 vagas e 2 empregados. No dia 64, o jogador tem cerca de **305 moedas**, e o Banco, cerca de **475**.

### 10.2 Caso A: o jogador compra o Posto de Coleta

O jogador paga 60 e fica dono do Posto.

**Sem o modo automático** (o Posto só segue a fila de ordens do jogador, que está vazia):

| Dia | O que acontece |
|---|---|
| 1 | Sem ordem: o Posto **folga** (Produção, regra 10.2). Os 2 Coletores passeiam e não recebem |
| 2 a 4 | Nem Água nem Tora. A Água (cerca de 35) cai 4 por dia; a Tora cai 6 por dia (a Carpintaria) |
| cerca de 5 | **A Tora acaba**: a Carpintaria do próprio jogador para |
| cerca de 9 | **A Água acaba**: o Cozinheiro para de fazer Pão |
| cerca de 14 | **O Pão acaba** (cerca de 80 no estoque, 15 comidos por dia, menos o que ainda foi feito): **fome** em toda a vila |

**O que mostra:** um único clique do jogador derruba a vila, e ele nem percebe que o Posto precisa de ordens. Daí o **modo automático ligado** na compra (regra 4.3, passo 2; TP1-3).

**Com o modo automático:** nada muda na vila. O jogador paga cerca de **11 moedas por dia** de salário (2 × 7,9 h × 0,75 × os multiplicadores) e recebe a Água e a Tora que a vila compra (cerca de 10 por dia × cerca de 0,6 = **6 moedas**). No dia de folga, não paga. **Resultado: cerca de −2 a −5 moedas por dia** para o jogador; o Banco ganha os mesmos 2 a 5. Comprar o Posto não dá lucro, porque ele é uma construção barata e de preço baixo. Isso é aceitável: é a escolha do jogador, e o painel mostra o lucro do dia.

### 10.3 Caso B: o jogador quebra

O jogador fica sem dinheiro e não paga a Carpintaria:

| Dia | O que acontece |
|---|---|
| 1 a 16 | Salário atrasado; os 2 Carpinteiros continuam trabalhando |
| 16 | Eles saem (Economia, regra 10.8): **5 desempregados** (33%). O multiplicador do desemprego fica no mínimo (90%, já estava). Felicidade: Desemprego e renda = 10 − 20 × 0,33 = **+3,3** (era +6) |
| 17 | A regra 4.1 não acha vaga na Cidade (todas cheias): os 5 continuam desempregados |
| 17 a 48 | A Carpintaria sem ninguém: `diasSemEmpregados` sobe até 32. **Sem Móveis**, os Materiais Básicos entram em déficit: −5 na Felicidade; com 8 dias, o evento "Falta de móveis" (−5) |
| 48 | Abre o leilão (3 dias). Sem Ricos, só o jogador poderia dar lance, e ele está quebrado |
| 51 | **Sem lance: a Carpintaria volta para a Cidade.** |
| 52 | A regra 4.1 contrata 3 desempregados (3 vagas no nível 2). Desemprego: 2 de 15 (13%). Felicidade +7,3. A Carpintaria volta a fazer Móveis pela IA da Cidade |

**O que mostra:**

1. A vila **se recupera sozinha**, mas passa cerca de 35 dias sem Móveis.
2. **O jogador perdeu tudo** sem nenhuma volta: nem os 330 da Carpintaria. Na vida real, o leilão pagaria as dívidas e devolveria o resto. Com um Rico dando lance (fase A4), o leilão paga os salários atrasados e devolve o resto ao jogador (TP1-4).

### 10.4 Caso C: o Banco fica sem dinheiro (o "sai e volta")

Sem a verba da Coroa (só em teste), o Banco chega a 0, e as construções da Cidade atrasam:

- **Sem o passo 3 da regra 4.1:** no dia 16, os 10 da Cidade saem; no dia 17, a Cidade os contrataria de novo, e o atraso zeraria. O salário atrasado viraria uma volta sem fim, e a saída por atraso não teria efeito.
- **Com os passos 1 e 3:** a Cidade não contrata sem 3 dias de salário no Banco, e quem saiu não volta na mesma estação. A vila para de verdade, e o problema aparece no teste. **OK.**

### 10.5 Verificações

| O que nunca pode acontecer | Resultado |
|---|---|
| Dinheiro surgindo do nada | **OK:** compra, venda e lance passam dinheiro entre o jogador e o Banco; a volta sem lance não paga nada |
| Um NPC em dois empregos | **OK:** a regra 4.1 só pega desempregados; a 4.2 troca os dois de uma vez |
| Mais empregados que vagas | **OK:** a regra 4.1 para quando não há vaga; a venda e a compra não mudam o nível |
| Laço sem fim na redistribuição | **OK:** cada troca soma 10 ou mais; a soma tem teto (100 × empregados), então as trocas acabam; e há o limite de uma volta |
| A Cidade contratando o "sai e volta" | **OK:** passo 3 da regra 4.1 |
| O jogador comprando uma construção em leilão | **OK:** recusado (regra 4.3, passo 1) |
| A vila sem Pão porque o jogador comprou o Posto | **OK com o modo automático** (Caso A) |
| Teste de 1 ano diferente do de hoje | **OK:** sem o jogador, nenhuma vaga da Cidade fica vazia, e todos têm 50: nenhuma regra nova dispara |

### 10.6 Casos extremos

| Caso | O que acontece |
|---|---|
| 0 desempregados e vaga vazia | A vaga fica vazia; a construção produz com quem tem |
| O jogador compra todas as 5 construções da Cidade (custa cerca de 60 a 330 cada) | Com o modo automático, a vila continua; o Banco fica só com o Mercado, o imposto e a cesta: a verba da Coroa segura |
| O jogador vende a Carpintaria e compra de volta no mesmo dia | Perde 25% (vende a 75%, compra a 100%). Não dá para lucrar com isso |
| O preço da Tábua sobe muito | O valor das construções sobe junto (o valor usa o preço de hoje) |

### 10.7 O que achou e como foi resolvido (rodada TP1)

1. **TP1-1:** a Cidade preenche as vagas todo dia, com as travas do Banco e do "sai e volta" (regra 4.1).
2. **TP1-2:** a redistribuição só troca empregados; ninguém perde o emprego (regra 4.2).
3. **TP1-3:** o valor é o custo da obra × o nível, a venda à Cidade é a 75%, e a construção comprada começa no modo automático (regra 4.3).
4. **TP1-4:** o lance paga primeiro os salários atrasados, e o resto vai para o jogador (regra 4.4).
