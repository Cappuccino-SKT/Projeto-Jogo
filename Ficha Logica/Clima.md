---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19, a primeira depois da fatia (fase A1 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]). É a primeira com a **parte 10: Simulação de mesa**. Os pontos marcados com CL1 vêm da **rodada CL1**, aplicada em 2026-10-19, toda com "seguir sugestão".

# Ficha Lógica: Clima e Estações

## 1. O que faz

Todo dia, à meia-noite, a vila sorteia o **clima do dia** (limpo, nublado, chuva, tempestade, neve, névoa ou calor forte). O clima muda o trabalho ao ar livre, o Período Livre dos NPCs, a Felicidade e a lenha. No **inverno**, cada morador gasta **lenha** (Tora) para se aquecer. Uma vez a cada cerca de 5 anos, o inverno é **rigoroso**.

Já existe e esta ficha só usa: a **produção por estação** (ficha dos [[Ficha Logica/Recursos|Recursos]], regra 4.3), as **fases do dia** (ficha do [[Ficha Logica/Tempo|Tempo]]) e os eventos de clima da fatia (Chuva boa, Seca, Geada precoce, Colheita farta, na ficha dos [[Ficha Logica/Eventos|Eventos]]).

Documento do sistema: [[Sistema de Clima e Estações/Descrição|Clima e Estações]].

**Esta versão vale para a vila:** uma região, bioma **temperado** (planície). Os outros biomas, a viagem, o combate e a guerra ficam na parte 9.

## 2. Dados

### Definições: o `data/clima.json` (arquivo novo)

Lido depois do `regras.json` e conferido pelo `--validar`: as chances de cada estação somam 1.000, e um clima que não existe é erro. O nome de cada clima fica no `textos/pt-BR.json` (`"clima.neve": "Neve"`).

```json
{
  "chance_de_repetir_o_clima_em_por_mil": 600,
  "chances_por_estacao_em_por_mil": {
    "primavera": { "limpo": 350, "nublado": 250, "chuva": 300, "tempestade": 50, "nevoa": 50 },
    "verao":     { "limpo": 500, "nublado": 200, "chuva": 150, "tempestade": 50, "calor_forte": 100 },
    "outono":    { "limpo": 300, "nublado": 300, "chuva": 250, "tempestade": 50, "nevoa": 100 },
    "inverno":   { "limpo": 250, "nublado": 300, "tempestade": 50, "neve": 300, "nevoa": 100 }
  },
  "efeitos_do_clima": {
    "tempestade":  { "trabalho_ao_ar_livre_em_porcento": 50, "periodo_livre_em_casa": true },
    "neve":        { "lenha_em_porcento": 150 },
    "calor_forte": { "felicidade": -2 }
  },
  "aquecimento": {
    "lenha_por_morador_por_dia_em_centesimos": 25,
    "lenha_do_bioma_em_porcento": 100
  },
  "clima_do_primeiro_dia": "limpo"
}
```

| Campo | O que é |
|---|---|
| `chance_de_repetir_o_clima_em_por_mil` | 600 = 60% de o clima de hoje ser o de ontem |
| `chances_por_estacao_em_por_mil` | A tabela do documento, em por mil (inteiros, para o sorteio dar igual em qualquer computador). Um clima que não aparece na estação tem chance 0 |
| `efeitos_do_clima` | Só os efeitos que a vila tem. Os de viagem, tiro e visão ficam para depois (parte 9) |
| `lenha_por_morador_por_dia_em_centesimos` | 25 = 1 Tora a cada 4 dias |
| `lenha_do_bioma_em_porcento` | Temperado = 100. Frio = 200; litoral e árido = 25 (fora desta versão) |
| `clima_do_primeiro_dia` | O dia 1 da partida não tem "ontem": começa limpo |

O **Inverno rigoroso** não fica aqui: é um evento do `eventos.json` (regra 4.5).

### Estado (vai no save)

| Campo | Tipo | Exemplo | No save? |
|---|---|---|---|
| `climaDeHoje` | `TipoDeClima` | `Neve` | Sim |
| `climaDeOntem` | `TipoDeClima` | `Nublado` | Não: é o `climaDeHoje` antes do sorteio |
| `lenhaDevidaEmCentesimos` | inteiro, de 0 a 99 | 75 | Sim |
| `diasEmDeficitDeLenha` | inteiro | 0 | Sim |
| O sorteio do clima | o estado do PCG32 | — | Sim |

**O sorteio** (mesma regra dos Eventos): um gerador só do clima, o PCG32 com a semente da partida e a **sequência 3** (os NPCs usam a 1 e os eventos a 2). Assim, um evento a mais não muda o clima sorteado.

No save, entra uma seção nova, `clima`, com os três campos e o gerador (ficha do [[Ficha Logica/Save|Save]]). A versão do formato sobe para 2.

## 3. Quando roda

| Quando | O que roda |
|---|---|
| **A cada dia**, à 0h, **antes** do consumo (passo 0 novo da sequência do dia) | Sorteio do clima do dia (regra 4.1) |
| **A cada dia**, no passo 1 (consumo da cidade), depois da cesta | A lenha do inverno (regra 4.3) |
| **A cada dia**, no passo 5 (Felicidade) | O calor forte e a falta de lenha entram nos fatores (regra 4.4) |
| **A cada hora** (produção) | O trabalho ao ar livre na tempestade (regra 4.2) |
| **A cada 5 minutos** (reavaliação da IA) | O Período Livre em casa na tempestade (regra 4.2) |
| **No dia 1 do Inverno**, no passo 6 (Eventos) | O sorteio do Inverno rigoroso (regra 4.5) |

*Por que o clima vem antes do consumo:* a lenha de hoje depende de nevar hoje.

## 4. Regras passo a passo

### 4.1 Sortear o clima do dia

1. **Dia 1 da partida:** o clima é o `clima_do_primeiro_dia`. Não sorteia nada.
2. **Repetir?** Sorteie um número de 0 a 999. Se for menor que 600, **e** o clima de ontem tiver chance maior que 0 na estação de hoje, hoje repete o de ontem. Fim.
   - *Por que a segunda condição:* no dia 1 do Inverno, a chuva de ontem não pode continuar (a chuva não existe no inverno); vale o mesmo para a neve que vem do inverno para a Primavera.
3. **Sortear da tabela:** sorteie outro número de 0 a 999 e ande pela tabela da estação **na ordem do arquivo**, somando as chances; o clima é aquele em que a soma passa do número.
   - *Exemplo (Inverno):* número 610. Limpo 0 a 249, Nublado 250 a 549, Tempestade 550 a 599, **Neve 600 a 899**, Névoa 900 a 999 → Neve.
4. Grave o `climaDeHoje` e escreva no Diário quando mudar ("Começou a nevar").

### 4.2 Os efeitos do clima no dia

1. **Tempestade, trabalho ao ar livre:** nas construções Braçais (Fazenda e Posto de Coleta), o lote do ciclo é multiplicado por 50%, **por cima** do fator da estação e dos eventos (regra 4.7).
2. **Tempestade, Período Livre em casa:** no período Livre, o **Passear** e o **Comprar** valem 0. **(CL1-1)** Quem está na rua volta para casa: o Passear ganha o destino "a própria casa" até o clima mudar, e o NPC fica lá dentro. Nenhuma ação nova. O Comprar só é a caminhada até o Mercado (a cesta já sai do estoque à 0h), então perdê-lo não muda o dinheiro.
3. **Neve:** a lenha do dia × 150% (regra 4.3).
4. **Calor forte:** −2 de Felicidade **hoje**, no fator dos eventos (regra 4.4).
5. **Chuva, Nublado, Limpo, Névoa:** nada na vila. A névoa só mexe no crime e no combate (parte 9).

### 4.3 A lenha do inverno

Só no Inverno. Roda depois da cesta mínima, com o mesmo jeito de pagar: **a classe paga ao dono da Tora** o preço local (ficha da [[Ficha Logica/Economia|Economia]], regra 4.1).

1. **A lenha de hoje, em centésimos de Tora:** moradores × 25 × bioma (100%) ÷ 100 × neve (150% se nevar hoje) ÷ 100 × Inverno rigoroso (200% se ativo) ÷ 100, cada divisão **arredondada para baixo**.
2. Some à `lenhaDevidaEmCentesimos`.
3. **Enquanto** ela for 100 ou mais e a Cidade tiver Tora: tire 1 Tora, a classe paga, subtraia 100.
4. **Faltou Tora** (ainda 100 ou mais e a Cidade sem Tora): os moradores passaram frio. Zere a dívida, ficando só o resto (`lenhaDevidaEmCentesimos` resto 100), e some 1 a `diasEmDeficitDeLenha`. A lenha que faltou **não é cobrada depois**: o frio de ontem não volta.
5. **Não faltou:** zere o `diasEmDeficitDeLenha`.
6. **Fora do Inverno**, no dia 1 da Primavera, zere a dívida e os dias em déficit.

*Prioridade:* a lenha sai à 0h, antes de qualquer compra do dia. Então, com pouca Tora, a **Carpintaria do jogador** é quem fica sem; a casa aquecida vem primeiro.

### 4.4 Na Felicidade

Na regra 4.1 da ficha da [[Ficha Logica/Felicidade|Felicidade]]:

1. **Consumo atendido:** −10 se a lenha faltou hoje (`diasEmDeficitDeLenha` > 0), somado ao −5 dos Materiais.
2. **Eventos duradouros:** o calor forte soma −2 no dia em que acontece, com os modificadores dos eventos, ainda preso entre −20 e +20.

### 4.5 O Inverno rigoroso

Um evento novo no `eventos.json`, que já cuida do sorteio, do limite de 2 negativos e do cooldown:

```json
{ "id": "evento.inverno_rigoroso", "tipo": "ocasional", "categoria": "clima", "negativo": true,
  "estacoes": ["inverno"], "dias_da_estacao": [1], "chance_por_dia_em_por_mil": 250,
  "cooldown_dias": 64, "duracao_dias": 16,
  "efeitos": [ { "producao_do_inverno_dividida_em_porcento": 150 }, { "lenha_em_porcento": 200 } ] }
```

- **[Proposta]** 25% no dia 1 de cada Inverno, com cooldown de 64 dias: o inverno seguinte nunca é rigoroso, e a média fica em 1 a cada 5 anos (1 ano de pausa + 4 anos de espera, em média).
- **Produção do inverno ÷ 1,5:** o percentual de inverno de cada grupo é dividido por 1,5, arredondado para baixo. **(CL1-3)** Só a Fazenda e os animais: Fazenda 25% → **16%**; animais 75% → 50%. A madeira e os minerais ficam como estão.
- **Lenha × 2** (regra 4.3).
- A mortalidade dos velhos fica para a [[Sistema de População/Descrição|População]] (fase A3).

### 4.6 Estocar para o inverno (CL1-2 e CL1-3)

Nos **últimos 4 dias do Outono** (dias 13 a 16), na escolha das receitas (ficha da [[Ficha Logica/Producao|Produção]], regra 4.3 e parte 10), o estoque-alvo de dois itens cresce:

1. **Tora:** soma a lenha de 5 dias de inverno comum: moradores × 25 × 5 ÷ 100 (15 Civis: 18 Toras, arredondado para baixo).
2. **Grão:** soma 5 dias do consumo previsto do próprio Grão (a vila gasta cerca de 8 por dia: +40).

- Com o alvo maior, a folga (125% do alvo) sobe junto, e o Posto e a Fazenda trabalham mais. A Fazenda, a 150% no Outono, faz cerca de 47 Grãos num dia: 1 dia já cobre os 40 a mais. Mesmo com a Geada precoce (dias 13 e 14), sobram 2 dias.
- No dia 1 do Inverno, o alvo volta ao normal. O estoque fica acima de 125%, e a construção folga até cair abaixo do alvo (a regra da folga como está).
- O **Mercado Local** usa o mesmo alvo: nos 4 dias, o preço da Tora e do Grão sobe um pouco (o estoque fica abaixo do alvo maior). É o "estocar para o inverno" aparecendo no preço.

### 4.7 A ordem dos multiplicadores na produção Braçal

O lote de um ciclo Braçal (ficha da [[Ficha Logica/Producao|Produção]], regra 4.2, passo 1) passa a ser:

**lote × fator do recurso (produtividade × estação, já com o rigoroso) × eventos × clima do dia**, cada multiplicação em porcento, com o resto guardado no `restoDaProducaoBracal` como hoje.

## 5. Contas

| Conta | Exemplo |
|---|---|
| Lenha de um dia comum (15 moradores) | 15 × 25 = **375 centésimos** = 3 Toras e 75 de resto |
| Lenha num dia de neve | 375 × 150 ÷ 100 = **562** |
| Lenha no Inverno rigoroso, com neve | 375 × 150 ÷ 100 = 562; × 200 ÷ 100 = **1.124** (11 Toras) |
| Lenha num inverno inteiro (16 dias, cerca de 5 de neve) | 11 × 375 + 5 × 562 = **6.935** ≈ 69 Toras |
| O que isso custa aos Civis | 69 Toras × 0,5 (preço base) ≈ **35 moedas** no inverno; o dinheiro vai para a Cidade, dona do Posto de Coleta, e volta em salários |
| Fazenda no inverno com tempestade | 2 Grãos × 25% × 50% = 0,25 Grão por ciclo (o resto fica guardado) |
| Fazenda no verão, com Seca e tempestade | 100% × 50% × 50% = **25%** |
| Dias de tempestade por estação | 5% × 16 dias ≈ **0,8 dia** (com a repetição de 60%, a média do ano não muda; os dias só vêm mais juntos) |

## 6. Conversa com outros sistemas

| Sistema | Lê | Entrega |
|---|---|---|
| Tempo | O dia, a estação | — |
| Recursos e Produção | — | O clima do dia (−50% ao ar livre); a produção do inverno no rigoroso |
| Economia | O preço local da Tora, o dono | A lenha que sai do estoque e o dinheiro da classe |
| Inventário | A Tora da Cidade | Tira a lenha |
| IA | — | O Período Livre em casa na tempestade |
| Felicidade | — | −10 pela falta de lenha; −2 pelo calor forte |
| Eventos | O Inverno rigoroso ativo | O evento novo |
| Interface | — | O clima de hoje no HUD (ícone e nome); a camada de partículas (chuva, neve) e a névoa |
| Save | — | A seção `clima` |
| Simulação sem tela | — | Colunas novas no `relatorio.csv`: `clima`, `lenha_do_dia`, `deficit_lenha` |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Codigo/Clima/Clima.h`: `enum class TipoDeClima` | `Limpo`, `Nublado`, `Chuva`, `Tempestade`, `Neve`, `Nevoa`, `CalorForte` |
| `RegrasDoClima` | O `clima.json` lido |
| `EstadoDoClima` | `climaDeHoje`, `lenhaDevidaEmCentesimos`, `diasEmDeficitDeLenha`, o sorteio |
| `sortearClimaDoDia(estado, regras, estacao, sorteio)` | A regra 4.1 |
| `porcentagemDoTrabalhoAoArLivre(climaDeHoje, regras)` | A regra 4.2, passo 1 |
| `consumirLenhaDoDia(estado, regras, moradores, inventario, ...)` | A regra 4.3 |
| `TipoDeClima` em texto: `textoDoClima(...)` | Para o HUD e o Diário |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Dia 1 da partida | Limpo, sem sortear |
| Inverno, número de repetir 300, ontem Neve | Neve (repetiu) |
| Dia 1 do Inverno, ontem Chuva, número de repetir 100 | Não repete (a chuva não existe no inverno); sorteia da tabela |
| Inverno, número da tabela 610 | Neve |
| Inverno, número da tabela 999 | Névoa (o último da lista) |
| A mesma semente, 64 dias, duas vezes | O mesmo clima nos mesmos dias |
| A mesma semente, com um evento a mais | O mesmo clima (geradores separados) |
| 15 moradores, Inverno, Nublado, dívida 0 | 3 Toras; dívida 75 |
| 15 moradores, Inverno, Neve, dívida 75 | 562 + 75 = 637: 6 Toras; dívida 37 |
| 15 moradores, Primavera | Nenhuma lenha |
| Neve, dívida 637, a Cidade com 1 Tora | Tira 1; faltou; dívida 37; 1 dia em déficit; Felicidade −10 no consumo |
| Tempestade, Fazenda no verão, lote 2 | 1 Grão por ciclo |
| Tempestade, um NPC passeando às 19h | Volta para casa |
| Calor forte | Fator dos eventos −2 |
| 100 Invernos sorteados (só o rigoroso) | Cerca de 1 a cada 5 anos (entre 12 e 28), nunca dois seguidos |
| Dia 13 do Outono, 15 Civis, consumo de Grão 8 por dia | Alvo da Tora + 18; alvo do Grão + 40 |
| Dia 1 do Inverno | Os alvos voltam ao normal |
| Teste de 1 ano, 10 sementes | Continua passando, com e sem Inverno rigoroso |

## 9. Fora desta versão

- **Biomas:** frio, árido, litoral e floresta (chances e produção diferentes); a vila é temperada.
- **Viagem:** os multiplicadores do clima e da estação, com teto de × 2,0, e as caravanas paradas na tempestade (fase B1).
- **Combate e Batalha:** tiro, visão, velocidade; a noite com −30% de visão (fase D).
- **Crime:** a névoa e a noite reduzem a detecção (fase C).
- **Guerra e Diplomacia:** a campanha de inverno e a vontade de guerra −15 (fase D).
- **Eventos extremos de outros biomas:** Enchente (mapas com rio), Tempestade forte (litoral), Onda de calor (árido e litoral), Geada fora de hora da montanha.
- **Mortalidade** pela falta de lenha e no Inverno rigoroso: com a População (fase A3).
- **Arte extra** das estações (neve no chão, rios congelados) e luzes dinâmicas: para o futuro.

## 10. Simulação de mesa

### 10.1 Cenário

A vila do fim do teste de 1 ano (`relatorio.csv`, semente 1): **15 Civis**, a Carpintaria do jogador com 2 empregados e o Posto de Coleta com 2. No começo do Inverno (dia 49): **Tora 10**, Água 33, Grão 63.

Hipóteses tiradas das fichas:

- O **Posto de Coleta** faz, num dia inteiro, 31,6 de Água ou de Tora a 100% (ficha da Produção, parte 5). No inverno, a madeira rende 75%: cerca de **23 Toras** num dia de Tora.
- A **Carpintaria** gasta cerca de **6 Toras por dia** (média do teste).
- O Posto escolhe, à noite, a receita do dia seguinte pela **nota do déficit**: alvo = consumo médio de 5 dias × 5 (ficha da Produção, regra 4.3 e parte 10).
- O clima abaixo foi **escolhido para testar o pior caso**, não sorteado: o Posto fez Água no dia 49, e neva nos dias 50 e 51.

### 10.2 Inverno comum, dia a dia

| Dia | Clima | Lenha devida (antes → depois) | Tora à 0h | Lenha tirada | Tora no fim do dia | O Posto amanhã |
|---|---|---|---|---|---|---|
| 49 | Nublado | 0 + 375 = 375 → 75 | 10 | 3 | 10 − 3 − 6 (Carpintaria) = **1** | Tora (déficit 97%) |
| 50 | **Neve** | 75 + 562 = 637 → 37 | 1 | **1 de 6: faltou** | 0 + 23 − 6 = **17** | Tora (déficit cerca de 50%) |
| 51 | Neve | 37 + 562 = 599 → 99 | 17 | 5 | 17 − 5 + 23 − 6 = **29** | Água ou Tora |
| 52 | Limpo | 99 + 375 = 474 → 74 | 29 | 4 | 29 − 4 − 6 = **19** | — |

**O que a tabela mostra:**

1. **Um dia de frio no começo do inverno (dia 50):** a lenha cai de repente de 0 para 3 a 6 Toras por dia, e a previsão do Posto (média dos últimos 5 dias) ainda não sabe disso. Com o estoque baixo e um dia de neve, falta lenha. Felicidade −10 no consumo naquele dia; com o passo de 10%, a Felicidade real cai cerca de 1 ponto.
2. **Depois, o Posto alcança:** 23 Toras num dia de Tora cobrem a lenha (cerca de 4,3 por dia) e a Carpintaria (6). O Posto precisa de Tora em cerca de 45% dos dias e de Água em 13%: sobra folga.
3. **No dia 50, a Carpintaria do jogador também fica sem Tora** de manhã (a lenha saiu primeiro), até o Posto produzir.

### 10.3 Inverno rigoroso

- **Lenha:** 375 × 2 = 750 por dia (7 a 8 Toras), 1.124 nos dias de neve: 11 × 750 + 5 × 1.124 = 13.870: cerca de **139 Toras** no inverno (8,7 por dia).
- **Tora:** com a madeira em 75% (CL1-3), o Posto faz 23 num dia de Tora. A demanda é 8,7 (lenha) + 6 (Carpintaria) = 14,7 por dia: Tora em 64% dos dias e Água em 13%. **Cabe.**
  - Se a madeira também caísse para 50% (16 por dia de Tora): Tora em 92% dos dias + Água 13% = **105%. Não cabe**: falta lenha ou Água quase todo dia. Por isso a proposta deixa a madeira de fora.
- **Comida:** a Fazenda cai de 8 para cerca de 5 Grãos por dia: cerca de **46 Grãos a menos** no inverno. No teste, o inverno comum termina com Grão 32 e Pão 83. No rigoroso, o Grão acaba perto do dia 60; a Padaria para, e os 15 Civis comem o Pão do estoque nos últimos 4 a 5 dias. O inverno termina com cerca de **20 Pães**, acima do limite da fome (10% do alvo de 75 = 7 Pães).
  - Na semente 1, **passa raspando**. Com um Ratos no celeiro no inverno, ou uma Seca no verão que deixe menos Grão no outono, a fome chega e o teste de 1 ano dá **FALHOU** (fome de 4 dias seguidos). A IA da produção não guarda comida a mais no outono. É a pergunta CL1-3.

### 10.4 Verificações

| O que nunca pode acontecer | Resultado |
|---|---|
| Dinheiro surgindo do nada | **OK:** a lenha é paga pela classe ao dono da Tora; nada é criado |
| Tora negativa | **OK:** o passo 3 só tira com estoque; o passo 4 trata o que faltou |
| A lenha somando dívida sem fim | **OK:** o passo 4 zera a dívida no dia em que falta |
| A dívida passar de um inverno para o outro | **OK:** o passo 6 zera no dia 1 da Primavera |
| Chuva no inverno ou neve na primavera pela repetição | **OK:** a repetição exige chance maior que 0 na estação de hoje |
| Mesma semente, clima diferente | **OK:** gerador próprio, sequência 3 |
| Fazenda abaixo de 0% com Seca, tempestade e estação | **OK:** são multiplicações em porcento, nunca subtrações |
| Calor forte e Festa da Colheita no mesmo dia | **OK:** o fator dos eventos soma e continua preso entre −20 e +20 |
| Felicidade descontada duas vezes pela lenha | **OK:** só o fator Consumo atendido conta a lenha; não há evento de "frio" |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Vila sem Posto de Coleta (ou sem bosque) | Falta lenha o inverno inteiro: −10 todo dia. A regra não quebra |
| Tora 0 e Inverno rigoroso com neve | Falta tudo (11 Toras); dívida volta para 24; um dia em déficit |
| 16 dias seguidos de tempestade (chance quase zero, mas possível) | A Fazenda e o Posto a 50%; ninguém passeia. Nenhum número negativo |
| 0 moradores | 0 de lenha. Nada a dividir |
| Carregar um save no meio do inverno | A dívida e o clima voltam do save; o gerador continua do mesmo ponto |

### 10.6 O que achou e como foi resolvido (rodada CL1)

1. **CL1-1:** a IA não tinha "ficar em casa" na tempestade → o Passear vai para a própria casa (regra 4.2).
2. **CL1-2:** faltava lenha no primeiro dia frio → o Posto estoca Tora nos últimos 4 dias do Outono (regra 4.6). Refeita a parte 10.2: entrando no inverno com cerca de 30 Toras ou mais em vez de 10, o dia 50 tira as 6 Toras da neve e não falta.
3. **CL1-3:** o Inverno rigoroso corta só a comida (Fazenda 16%, animais 50%), e a vila estoca 40 Grãos a mais no Outono (regra 4.6). Refeita a parte 10.3: o inverno rigoroso começa com cerca de 100 Grãos, perde cerca de 77 e termina com cerca de 20 Grãos e o Pão do estoque: sem fome, mesmo com um Ratos no celeiro (−10% do Grão). São estimativas de mesa: o teste de 1 ano confirma quando for programado.
