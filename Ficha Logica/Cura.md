---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase A6 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com CU1 vêm da **rodada CU1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Cura

## 1. O que faz

Duas partes:

1. **A doença na cidade** (entra agora): a **Epidemia**, um evento de Saúde que deixa parte dos Civis doentes e **mata pessoas** pelo acumulado da [[Ficha Logica/Populacao|População]]. O **Remédio** em estoque corta as mortes pela metade; o **Boticário** e o **Hospital** deixam a epidemia mais rara.
2. **A Vida e a recuperação dos NPCs** (regras prontas, ligadas só quando o Combate chegar, fase D): regenerar descansando, Curativo, Remédio, Hospital, Inconsciente.

Documento do sistema: [[Sistema de Cura/Descrição|Cura]]; os efeitos dos itens, em [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]].

A **Febre** da fatia (2 Civis em casa por 4 dias, ficha dos [[Ficha Logica/Eventos|Eventos]]) continua como está: é a doença leve, sem mortes.

## 2. Dados

### A Epidemia, no `data/eventos.json`

```json
{ "id": "evento.epidemia", "tipo": "ocasional", "categoria": "saude", "negativo": true,
  "chance_por_dia_em_por_mil": 5, "cooldown_dias": 64, "duracao_dias": 8,
  "modificadores_da_chance": [
    { "ocupacao_minima_em_porcento": 90, "porcentagem": 200 },
    { "comida_em_deficit": true, "porcentagem": 200 },
    { "existe_construcao": ["construcao.boticario", "construcao.hospital"], "porcentagem": 50 } ],
  "efeitos": [ { "npcs_doentes_em_porcento_dos_civis": 20 }, { "felicidade": -10 },
               { "mortes_por_dia_em_por_mil_da_populacao": 10 },
               { "remedio_por_dia_a_cada_npcs": 10, "mortes_com_remedio_em_porcento": 50 } ] }
```

| Campo novo | O que é |
|---|---|
| `modificadores_da_chance` | Multiplicam a chance base, um depois do outro (CU1-1) |
| `npcs_doentes_em_porcento_dos_civis` | Como o `npcs_doentes` da Febre, mas em porcento (arredondado para cima) e sorteando **qualquer** Civil |
| `mortes_por_dia_em_por_mil_da_populacao` | 10 = 1% da população por dia, tirado do acumulado da População |
| `remedio_por_dia_a_cada_npcs` | 1 Remédio por 10 NPCs reais por dia (arredondado para cima); com ele, as mortes × 50%. Na cidade Religiosa com Templo, mais × 75% ([[Ficha Logica/Religiao|Religião]], 4.7) |

### A Vida (para o Combate; no `data/regras.json`, bloco `"cura"`)

```json
"cura": {
  "regeneracao_por_hora_em_milesimos_da_vida": 10,
  "regeneracao_dormindo_em_porcento": 200,
  "curativo": { "regeneracao_em_porcento": 200, "horas": 8 },
  "remedio":  { "regeneracao_em_porcento": 300, "horas": 24, "cura_doente": true },
  "hospital_por_hora_em_milesimos_da_vida": 50, "hospital_com_mago_em_milesimos": 100,
  "hospital_custo_por_dia_em_decimos": 20,
  "inconsciente_acorda_em_horas": 4, "inconsciente_acorda_com_vida_em_porcento": 10,
  "sangramento_mata_em_horas": 2
}
```

- **(CU1-2)** A regeneração base é 1% da Vida máxima por hora (de 0 a 100% em cerca de 4 dias acordado, 2 dias dormindo), × 2 dormindo. Curativo e Remédio **não se somam**: vale o maior.

### Estado (vai no save)

| Campo | Onde | No save? |
|---|---|---|
| A Epidemia ativa e os doentes | Os eventos ativos e o `DoencaDoNpc` (já existem) | Sim |
| `vidaAtual`, `estadoAtual`, `efeitoDeCuraAtivo` (item e horas que faltam) | cada NPC, **só com o Combate** | Sim |

## 3. Quando roda

| Quando | O que roda |
|---|---|
| **A cada dia**, no passo 6 (Eventos) | O sorteio da Epidemia, com os modificadores (regra 4.1) |
| **A cada dia** de Epidemia, no passo 1 (consumo) | Gastar o Remédio (regra 4.2) |
| **A cada dia** de Epidemia, no passo 5 (com a fome e o frio da População) | As mortes (regra 4.2) |
| **A cada hora** (só com o Combate) | A regeneração (regra 4.3) |

## 4. Regras passo a passo

### 4.1 O sorteio da Epidemia

1. **A chance de hoje** = 5 por mil × cada modificador que vale ÷ 100, um depois do outro (arredondado para baixo):
   - a ocupação da cidade (NPCs reais ÷ capacidade) ≥ 90%: × 200%;
   - a comida em déficit hoje: × 200%;
   - existe um Boticário ou um Hospital na cidade: × 50%.
2. O resto é o sorteio dos ocasionais de hoje (limite de 2 negativos, cooldown).
3. **Ao começar:** sorteie 20% dos Civis (arredondado para cima), de qualquer emprego ou sem emprego, e ponha `DoencaDoNpc` = 8 dias. Eles ficam em casa como na Febre. Diário: "Epidemia na vila: 3 doentes".

### 4.2 Cada dia de Epidemia

1. **Remédio** (no consumo): a cidade precisa de NPCs reais ÷ 10 Remédios (arredondado para cima). Se houver no estoque, tire e pague ao dono (como a cesta). Se não houver todos, não tire nenhum (**CU1-3**: meia dose não protege).
2. **Mortes** (na População): população × 10 ÷ 1.000; × 50% se o Remédio foi tomado hoje. Tire do acumulado da População. Se o acumulado passar de −100, morre 1 NPC: **um dos doentes, o mais velho** (CU1-4).
3. **No fim do evento**, os doentes voltam à rotina (o `DoencaDoNpc` já conta os dias).

### 4.3 A Vida (só com o Combate)

1. **A cada hora**, fora de combate e não Morto nem sangrando: Vida + Vida máxima × 10 ÷ 1.000 × (200% se está dormindo ou descansando) × (o maior efeito de item ativo: Curativo 200%, Remédio 300%) ÷ 10.000. Até a Vida máxima.
2. **Hospital:** internado, a Vida sobe 5% por hora (10% com um Mago de Suporte trabalhando lá), no lugar do passo 1. Custa 2 moedas por dia ao Controlado.
3. **Inconsciente:** no modo não letal, acorda em 4 horas com 10% da Vida; no modo letal, sangra e morre em 2 horas sem Curativo (o Curativo estanca: vira Inconsciente não letal).
4. **Doente** (Controlados e Importantes): habilidades −20% e sem regeneração, até tomar Remédio, ir ao Hospital ou passar 8 dias.

## 5. Contas

| Conta | Exemplo |
|---|---|
| Chance da Epidemia na vila cheia (15 de 15) | 5 × 200% = **10 por mil** (1% por dia) |
| Com fome também | 10 × 200% = 20 por mil |
| Com Boticário, vila cheia | 10 × 50% = 5 por mil |
| Epidemias por ano na vila cheia | 64 dias × 1% ≈ 0,64; com o cooldown de 64 dias depois do fim, cerca de **0,5 por ano** |
| Doentes numa vila de 15 | 20% = 3 (arredondado para cima) |
| Mortes por dia, vila de 15 | 1.500 × 10 ÷ 1.000 = **15 pessoas**; em 8 dias, 120 |
| Com Remédio | 7 por dia (arredondado para baixo), 56 em 8 dias; gasta 2 Remédios por dia, 16 no total |
| Numa cidade de 40 | 40 por dia, **320 em 8 dias** (cerca de 3 NPCs); com Remédio, 160 (1 ou 2) |

## 6. Conversa com outros sistemas

| Sistema | Lê | Entrega |
|---|---|---|
| Eventos | O sorteio, o limite de negativos | O evento novo; os modificadores da chance (campo novo, que serve também a outros eventos) |
| População | A população, o acumulado | As mortes da doença |
| Cidade | A ocupação | — |
| Economia | O déficit de comida; o preço do Remédio | A compra do Remédio |
| Construção | Se existe Boticário ou Hospital | — |
| IA | — | Os doentes ficam em casa |
| Combate (fase D) | — | A Vida, a regeneração, o Inconsciente |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `ModificadorDaChanceDoEvento` | Um item de `modificadores_da_chance` |
| `calcularChanceDoEventoHoje(evento, vila)` | A regra 4.1, passo 1 |
| `gastarRemedioDaEpidemia(...)`, `somarMortesDaEpidemia(...)` | A regra 4.2 |
| `RegrasDaCura`, `regenerarVidaNaHora(...)` | A regra 4.3 (com o Combate) |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Vila de 15 de 15, sem fome, sem Boticário | Chance 10 por mil |
| Vila de 10 de 15 (67%) | Chance 5 por mil |
| Epidemia começa numa vila de 15 | 3 doentes, 8 dias, Felicidade −10 |
| Dia de Epidemia, 15 NPCs, sem Remédio | Acumulado − 15 |
| O mesmo, 2 Remédios no estoque | Tira 2; acumulado − 7 |
| 1 Remédio no estoque (precisa de 2) | Não tira; acumulado − 15 |
| Acumulado passa de −100 durante a Epidemia | Morre o doente mais velho |
| Teste de 1 ano, 10 sementes | **Muda** (um evento novo muda os sorteios dos eventos): conferir que continua PASSOU |

## 9. Fora desta versão

- **A Vida e a regeneração** (regra 4.3): ligadas com o Combate (fase D). Os NPCs da vila ainda não têm Vida.
- **O Boticário, o Hospital, o Curativo e o Remédio:** a regra está pronta, mas os itens e as construções vêm com o conteúdo (o Remédio pede Flor Espiritual e Especiarias). Até lá, a Epidemia roda sem Remédio e sem o desconto do Boticário.
- **O Templo da Religiosa** (−25% nas mortes da doença, cura 25% mais rápida): fase C.
- **Ferimentos por parte do corpo:** fora do escopo do documento.

## 10. Simulação de mesa

### 10.1 Cenário

A vila do teste de 1 ano (15 de 15, acumulado da População em +100, vila cheia) e uma cidade de nível 2 com 40 NPCs (acumulado em +20).

### 10.2 Uma Epidemia na vila cheia

| Dia | O que acontece | Acumulado |
|---|---|---|
| 1 | Começa: Ana, Elisa e Nuno doentes (a Fazenda e o Cozinheiro com 1 empregado cada). Felicidade −10 | 100 − 15 = 85 |
| 2 a 8 | Os 3 em casa. A Fazenda faz metade do Grão; o Cozinheiro, metade do Pão (cerca de 8 por dia, a vila come 15) | 70, 55, ..., **−20** no dia 8 |
| 9 | Fim. Ninguém morreu: o acumulado não chegou a −100 | −20 |
| Estação seguinte | A vila continua cheia, então a PO1-4 não zera. Nascimentos +54: o acumulado volta a +34 | +34 |

**O que mostra:**

1. **Na vila cheia, a Epidemia não mata nenhum NPC**: as 120 mortes saem da "folga" de +100 (as pessoas da população que não são NPCs). É o certo: a vila tem 1.500 pessoas, e 120 morrem, mas os 15 NPCs representam o resto.
2. **O Pão cai:** 7 Pães por dia a menos por 8 dias = cerca de **56 Pães**. Com cerca de 80 no estoque, passa raspando. Se a Epidemia pega o inverno, com o Grão baixo, pode chegar a **fome**, e a fome dobra a chance de outra Epidemia (depois do cooldown). Fica assim: é a crise em cadeia que o documento quer, e o cooldown de 64 dias impede duas seguidas.

### 10.3 Uma Epidemia na cidade de 40

| Dia | Acumulado (começa em +20) | O que acontece |
|---|---|---|
| 1 a 3 | −20, −60, **−100** | Morre o doente mais velho (dia 3); acumulado 0 |
| 4 a 8 | −39, −78, **−117** | Morre outro no dia 6; −17; dias 7 e 8: −55, −93 |
| Fim | −93 | 2 mortos; 8 doentes (20% de 40) por 8 dias |
| Estação seguinte | A PO1-4 zera (a cidade está abaixo da capacidade) | — |

Com Remédio (4 por dia, 32 no total): −20 por dia, cerca de 1 morto. **O Remédio vale a pena** numa cidade grande.

### 10.4 Verificações

| O que nunca pode acontecer | Resultado |
|---|---|
| Duas Epidemias ao mesmo tempo | **OK:** cooldown e o evento é um só |
| Morrer quem não está doente | **OK:** a regra 4.2 escolhe entre os doentes; se nenhum doente sobrou, o mais velho (regra da População) |
| Gastar meia dose | **OK:** CU1-3 |
| Dinheiro surgindo do nada | **OK:** o Remédio é pago ao dono, como a cesta |
| A chance passando de 1.000 | **OK:** o máximo é 5 × 2 × 2 = 20 por mil |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Vila de 1 NPC | 1 doente (20% arredondado para cima); 1 morte por dia em pessoas (100 × 1%); não chega a −100 em 8 dias |
| Epidemia + Fome + Inverno rigoroso | Possível (o limite de 2 negativos vale só para os ocasionais; a Fome é condicional). As mortes somam no mesmo acumulado: a vila pode perder 1 ou 2 NPCs |
| Todos os empregados de uma construção doentes | Ela para por 8 dias |

### 10.6 O que muda no teste de 1 ano

A Epidemia entra no sorteio e muda a sequência dos eventos. Na vila cheia, ela não mata (parte 10.2), mas tira cerca de 56 Pães. **Quando for programada, rodar as 10 sementes de novo.** Se alguma falhar por fome, a primeira saída é a CU1-1 (diminuir a chance base de 5 para 3 por mil).

### 10.7 Decidido em modo automático (rodada CU1, seguir sugestão)

1. **CU1-1:** a chance base é 5 por mil por dia, com os modificadores do documento (densidade × 2, fome × 2, Boticário ou Hospital × 0,5).
2. **CU1-2:** a regeneração base é 1% da Vida por hora, × 2 dormindo; o Curativo e o Remédio não se somam.
3. **CU1-3:** o Remédio só protege com a dose inteira do dia.
4. **CU1-4:** quem morre na Epidemia é o doente mais velho.
