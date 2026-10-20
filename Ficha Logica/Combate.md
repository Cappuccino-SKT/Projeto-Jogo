---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase D1 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com CB1 vêm da **rodada CB1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Combate

## 1. O que faz

O combate no Mapa Local e no Mapa Interno, em **tempo real com pausa ativa**. Cada golpe tira Vida pela fórmula do documento (dano da arma × habilidade − Defesa), com a vantagem entre arma e armadura. O **Medo** faz fugir e render-se. As habilidades sobem com o uso. Fora da tela, o **cálculo rápido** usa a **mesma fórmula** para dar um resultado coerente.

Documento do sistema: [[Sistema de Combate/Descrição|Combate]]; a Vida e o Inconsciente, na [[Ficha Logica/Cura|Cura]] (regra 4.3, ligada agora).

**Esta versão:** a fórmula, o acerto, o intervalo, a vantagem, os modos letal e não letal, o Medo, a fuga e a rendição, a evolução, o cálculo rápido (que substitui a força simples da Viagem), a derrota e o crime de atacar. **Feitiços, encantamento, montaria, Criaturas e masmorras, e o boneco em camadas** ficam na parte 9.

## 2. Dados

### `data/combate.json` (arquivo novo)

```json
{
  "vida_maxima": 100,
  "armas": {
    "soco":    { "dano": [2, 4],   "intervalo_em_ticks": 20, "alcance_em_tiles": 1,  "habilidade": "corpo_a_corpo" },
    "espada":  { "dano": [8, 12],  "intervalo_em_ticks": 20, "alcance_em_tiles": 1,  "habilidade": "corpo_a_corpo", "classe": "corte" },
    "lanca":   { "dano": [7, 11],  "intervalo_em_ticks": 20, "alcance_em_tiles": 2,  "habilidade": "corpo_a_corpo", "classe": "corte" },
    "machado": { "dano": [12, 18], "intervalo_em_ticks": 30, "alcance_em_tiles": 1,  "habilidade": "corpo_a_corpo", "classe": "impacto" },
    "clava":   { "dano": [11, 17], "intervalo_em_ticks": 30, "alcance_em_tiles": 1,  "habilidade": "corpo_a_corpo", "classe": "impacto" },
    "arco":    { "dano": [6, 10],  "intervalo_em_ticks": 30, "alcance_em_tiles": 12, "habilidade": "longo_alcance", "classe": "arco" },
    "besta":   { "dano": [12, 16], "intervalo_em_ticks": 50, "alcance_em_tiles": 14, "habilidade": "longo_alcance", "classe": "perfurante" }
  },
  "dano_por_rank_em_porcento": 25,
  "defesa_por_peca_rank_1": { "leve": 1, "media": 2, "pesada": 3 },
  "vantagem_em_porcento": {
    "corte":      { "leve": 25,  "media": 0, "pesada": -25 },
    "impacto":    { "leve": -25, "media": 0, "pesada": 25 },
    "arco":       { "leve": 0,   "media": 0, "pesada": 0 },
    "perfurante": { "leve": 0,   "media": 0, "pesada": 25 }
  },
  "acerto_a_distancia": { "base": 90, "por_tile_acima_de_3": -5, "por_ponto_de_habilidade_acima_de_50": 0.5, "limites": [10, 95],
                          "chuva": -10, "tempestade": -20, "noite_alcance_em_porcento": 70 },
  "medo": { "por_vida_perdida_em_porcento": 0.5, "por_aliado_caido": 10, "inimigo_2_vezes_mais_forte": 20,
            "coragem_padrao": 60, "rendicao_acima_da_coragem": 30 },
  "evolucao_por_golpe_em_centesimos": 10,
  "raio_de_alerta_em_tiles": 10,
  "ticks_para_sair_de_combate": 100,
  "reavaliar_alvo_a_cada_ticks": 10,
  "limite_de_combatentes": 40,
  "kits": {
    "civil":   { "arma": "soco",   "armadura": [],                          "habilidade": 30 },
    "bandido": { "arma": "clava",  "armadura": [],                          "habilidade": 40 },
    "guarda":  { "arma": "lanca",  "armadura": ["media", "media", "leve"],  "habilidade": 50 },
    "soldado": { "arma": "espada", "armadura": ["media", "media", "media"], "habilidade": 55 }
  }
}
```

Todos os números são **[Proposta]** (as armas reais vêm do conteúdo, com o Rank). Os **kits** servem para quem não tem inventário de verdade (bandidos sorteados, guardas da caravana, Caçadores): é o que substitui a força simples da Viagem (VI1-4).

### Estado de cada combatente

| Campo | Vai no save? | Exemplo |
|---|---|---|
| `vidaAtual` (o NPC ganha) | Sim | 73 |
| `habilidadesEmCentesimos` (Corpo a Corpo, Longo Alcance, Defesa) | Sim | corpo a corpo 5.040 (50,4) |
| `postura`, `modoLetal` | Sim (Controlados) | defensiva, não letal |
| `emCombate`, `ticksSemAmeaca`, `alvo`, `ticksAteOProximoGolpe`, `medo` | Não (recomeça ao carregar) | — |

**O sorteio:** o dano e o acerto usam o gerador dos NPCs da cidade (sequência 1 na vila); o cálculo rápido fora da tela usa o da Viagem (5).

## 3. Quando roda

| Quando | O quê |
|---|---|
| **A cada tick**, em LOD 0 e 1 | Os golpes, a fuga, o estado de combate (4.1 a 4.6) |
| A cada 10 ticks | Cada combatente reavalia o alvo e o Medo |
| Na Viagem, encontros e Caçadores | O cálculo rápido (4.8) |
| A cada hora | A Vida da Cura (4.3 da Cura), fora de combate |

## 4. Regras passo a passo

### 4.1 Entrar e sair de combate

1. Um NPC entra em combate quando um **hostil** está a 10 tiles ou menos. A velocidade do jogo vai para 1x.
2. Sai depois de **100 ticks** sem hostil no raio.
3. Em combate, não há regeneração (Cura).

### 4.2 O golpe

Quando `ticksAteOProximoGolpe` chega a 0 e o alvo está no alcance:

1. **Dano bruto** = sorteio entre o dano mínimo e o máximo da arma × (100 + 25 × (Rank − 1)) ÷ 100 × habilidade ÷ 50.
2. **Vantagem:** × (100 + vantagem da classe da arma contra a classe de peso da armadura do alvo) ÷ 100. **Sem armadura conta como Leve** (CB1-2).
3. **Dano final** = o maior entre 1 e (dano com vantagem − Defesa do alvo), arredondado para baixo.
4. A classe de peso da armadura é a **mais frequente** das 3 peças; empate: Média. A Defesa é a soma das peças (por peça: leve 1, média 2, pesada 3 no Rank 1, + 25% por Rank).
5. `ticksAteOProximoGolpe` = o intervalo da arma.

**Longo alcance:** antes do dano, a chance de acerto = 90% − 5% por tile acima de 3 + 0,5% por ponto de habilidade acima de 50 (menos se abaixo), −10% na chuva, −20% na tempestade, entre 10% e 95%. À noite, o alcance cai para 70%. Cada tiro gasta 1 Munição; sem ela, troca para a arma secundária.

**Arma acima do Atributo Mínimo:** dano e intervalo × (habilidade ÷ mínimo), quando a habilidade é menor.

### 4.3 Vida 0

- **Não letal:** Inconsciente (acorda em 4 horas com 10%; Cura, 4.3).
- **Letal:** Inconsciente sangrando; morre em 2 horas sem Curativo, ou na hora, se um inimigo der o golpe de misericórdia (a IA dá em quem está no modo letal contra ele).
- Padrão: letal contra bandidos e Criaturas; não letal contra cidadãos e guardas (CB1-3: os guardas usam não letal contra o jogador, a menos que ele tenha matado alguém).

### 4.4 O Medo

A cada 10 ticks: Medo = (100 − Vida%) × 0,5 + 10 por aliado caído neste combate + 20 se o poder do lado inimigo (4.8) é mais de 2 vezes o do lado dele.

- **Acima da Coragem** (padrão 60 até a Personalidade existir): **foge** (pela postura: "manter posição" nunca foge sozinho).
- **Acima da Coragem + 30** e sem caminho para fugir: **se rende** (fica parado, sai do combate; o vencedor decide: prender, Prisioneiro ou deixar ir).

### 4.5 A evolução (só Controlados)

- Cada golpe que acerta: a habilidade da arma sobe 10 × (100 − habilidade) ÷ 100 centésimos.
- Cada golpe recebido: a Defesa sobe o mesmo.
- *Exemplo:* habilidade 20 sobe 8 centésimos por golpe (um ponto a cada 12 golpes); 80 sobe 2 (um ponto a cada 50).

### 4.6 O crime de atacar

- Atacar um cidadão ou guarda: **Agressão** (gravidade 3); matar: **Assassinato** (5).
- **Testemunha:** qualquer NPC acordado a 10 tiles ou menos vê; à noite, cada testemunha vê com 70% de chance. Sem testemunha, nada (Crime). Percepção e Destreza entram quando o NPC tiver esses atributos (CB1-4).

### 4.7 A derrota

- **Um Controlado morre:** é permanente.
- **O jogador morre:** outro Controlado assume (a dinastia vem com os Companheiros, D2).
- **Todo o grupo Inconsciente:**
  - pelos **guardas**: prisão (Crime, 4.6);
  - por **bandidos**: perdem 75% do dinheiro e dos itens carregados (não o equipamento) e acordam no lugar. A escravidão vem com a Escravidão (D3);
  - por **Criaturas**: perdem o que carregam (não o equipamento) e acordam no lugar.
- Game Over só se ninguém puder ser capturado (sem Controlados vivos).

### 4.8 O cálculo rápido (fora da tela)

Para cada lado, com os kits ou o equipamento de verdade:

1. **Dano por tick** de um combatente contra o lado oposto = (dano médio da arma × Rank × habilidade ÷ 50 × vantagem contra a armadura **mais comum** do outro lado − a Defesa média do outro lado, mínimo 1) ÷ intervalo.
2. **Poder do lado** = (soma do dano por tick) × (soma da Vida) (CB1-1).
3. **Chance de A vencer** = poder A ÷ (poder A + poder B). Sorteio de 0 a 999.
4. **O vencedor** perde, em cada membro, Vida = 100 × poder do perdedor ÷ poder do vencedor por cento, até 90%. **O perdedor** fica todo Inconsciente (e segue a 4.7).

É o que a Viagem (4.6 e 4.11), o Comércio (caravana × bandidos) e o Crime (Caçadores) usam no lugar da força simples (**muda a VI1-4**).

## 5. Contas

**O dano médio por golpe, Rank 1:**

| Atacante | Arma × habilidade | Contra sem armadura (Leve, Defesa 0) | Contra guarda (Média, Defesa 5) |
|---|---|---|---|
| Jogador, Espada, 50 | 10 | 10 × 1,25 = **12,5** | 10 × 1,0 − 5 = **5** |
| Bandido, Clava, 40 | 14 × 0,8 = 11,2 | 11,2 × 0,75 = **8,4** | 11,2 − 5 = **6,2** |
| Guarda, Lança, 50 | 9 | 9 × 1,25 = **11,25** | 9 − 5 = **4** |
| Civil, soco, 30 | 3 × 0,6 = 1,8 | 1,8 × 1,25 = 2,25 | 1 (mínimo) |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Cura | A Vida, a regeneração e o Inconsciente (regra 4.3 dela) passam a valer |
| Viagem | A luta rápida e o cálculo da IA usam o 4.8; os kits no lugar da força simples (VI1-4) |
| Comércio | A caravana: 2 guardas e 2 civis (kits) |
| Crime | Agressão e Assassinato com testemunhas; os Caçadores com o kit de guarda; a prisão depois da derrota |
| Itens | Dano, intervalo, alcance, Rank, peças de armadura, Munição |
| Clima, Tempo | Chuva e noite no acerto; 1x ao entrar em combate |
| Interface | Pausa ativa, ordens, postura, barras de Vida |
| Save | A Vida, as habilidades em centésimos, a postura e o modo; o formato passa para a **versão 14** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `calcularDanoDoGolpe(atacante, alvo)` | Regra 4.2 |
| `chanceDeAcertoADistancia(atacante, alvo, distancia)` | Regra 4.2 |
| `classeDePesoDaArmadura(npc)`, `defesaDoNpc(npc)` | Regra 4.2, passo 4 |
| `atualizarMedo(npc)` | Regra 4.4 |
| `evoluirHabilidade(npc, habilidade)` | Regra 4.5 |
| `poderDoLado(combatentes, ladoOposto)` | Regra 4.8 |
| `resolverPorCalculoRapido(ladoA, ladoB)` | Regra 4.8 (substitui o `resolverPorCalculoRapidoDeForcas` da Viagem) |

## 8. Testes

1. **A tabela da parte 5.**
2. **Duelo 1 contra 1** da parte 10.2: o jogador vence em 8 golpes (160 ticks), com cerca de 73 de Vida.
3. **O cálculo rápido do mesmo duelo:** chance de cerca de 78% e perda de cerca de 29% de Vida, perto do combate visto.
4. **Coerência:** 1.000 duelos vistos e 1.000 rápidos dos mesmos lados: as vitórias diferem em no máximo 10 pontos percentuais.
5. **Mínimo de 1:** o civil contra o guarda tira 1 por golpe.
6. **Evolução:** 12 acertos com habilidade 20 dão cerca de +1 ponto.
7. **Testemunha à noite:** com 3 testemunhas, a chance de ser visto é 1 − 0,3³ = 97,3%.

## 9. Fora desta versão

- Os feitiços e o encantamento (Magia, D4).
- As Criaturas, as masmorras e o Loot (Objetivo e Progressão, conteúdo).
- A montaria no combate; o duelo como rito (Companheiros e Cultura).
- A Personalidade (Coragem, Agressividade) e os atributos Percepção, Destreza e Agilidade.
- A escravidão do grupo derrotado (Escravidão, D3).
- O boneco em camadas e as animações (arte).

## 10. Simulação de mesa

### 10.1 Cenário

O **jogador**: Espada Rank 1, habilidade 50, 3 peças Leves (Defesa 3), Vida 100, não letal desligado (letal). Um **bandido**: Clava, habilidade 40, sem armadura, Vida 100, Coragem 60.

### 10.2 O duelo, golpe a golpe

Dano do jogador: 8 a 12 × 1 × 1,25 (corte contra "sem armadura" = Leve) − 0 → 10 a 15, média 12,5, a cada 20 ticks. Dano do bandido: 11 a 17 × 0,8 × 0,75 (impacto contra Leve) − 3 → 3,6 a 7,2 → **3 a 7**, média cerca de 5, a cada 30 ticks.

| Tick | Golpe | Vida do jogador | Vida do bandido | Medo do bandido |
|---|---|---|---|---|
| 0 | Jogador (sorteio 10 × 1,25 = 12) | 100 | 88 | 6 + 20 = 26 (o jogador tem mais de 2× o poder, 10.3) |
| 0 | Bandido (sorteio 14 → 5) | 95 | 88 | |
| 20 | Jogador 15 | 95 | 73 | 33 |
| 30 | Bandido 6 | 89 | 73 | |
| 40 | Jogador 13 | 89 | 60 | 40 |
| 60 | Jogador 11; bandido 4 | 85 | 49 | 45 |
| 80 | Jogador 12 | 85 | 37 | 51 |
| 90 | Bandido 7 | 78 | 37 | |
| 100 | Jogador 14 | 78 | 23 | **58** |
| 120 | Jogador 12; bandido 5 | 73 | 11 | **64 > 60: foge** |
| 140 | O bandido ainda está ao lado (acabou de virar); o jogador acerta 12 | 73 | 0 | Inconsciente (letal: sangra) |

O jogador vence com **73 de Vida** em 8 golpes. Evolução: 8 acertos × 5 centésimos = +0,40 em Corpo a Corpo; 5 golpes recebidos × 5 = +0,25 em Defesa.

### 10.3 O cálculo rápido do mesmo duelo

- Jogador: 12,5 ÷ 20 = 0,625 por tick; Vida 100; poder **62,5**.
- Bandido: 5,4 ÷ 30 = 0,18 por tick; Vida 100; poder **18**.
- Chance do jogador: 62,5 ÷ 80,5 = **78%**. Se vence, perde 18 ÷ 62,5 = **29%** de Vida (o combate visto: 27%). **Bate.**

### 10.4 O jogador contra 6 bandidos (o encontro da Viagem, 10.2)

- Bandidos: 6 × 0,18 = 1,08 por tick; Vida 600; poder **648**.
- Jogador: poder 62,5.
- Chance do jogador: 62,5 ÷ 710,5 = **8,8%** (a força simples da Viagem dava 20%).

**Achado:** com o poder = dano × Vida, o número pesa ao quadrado: 6 bandidos fracos valem muito mais que 1 jogador bom. É o que acontece no combate visto (cada bandido golpeia ao mesmo tempo), então a regra é coerente (CB1-1). A viagem sozinho fica **mais perigosa** do que a ficha da Viagem previa; fugir (54%) e render-se (perde 50%) ficam mais importantes.

### 10.5 A caravana da cidade

2 guardas (Lança, 50, Média) e 2 civis contra um bando de 6:

- Caravana: guardas 2 × 11,25 ÷ 20 = 1,125; civis 2 × 2,25 ÷ 20 = 0,225; soma 1,35 por tick; Vida 400; poder **540**.
- Bandidos contra a Média (a mais comum: 2 Médias e 2 "sem armadura" = Leve, empate → Média), com a Defesa média da caravana (5 + 5 + 0 + 0) ÷ 4 = 2,5: 11,2 − 2,5 = 8,7 ÷ 30 = 0,29 cada; 1,74 por tick; Vida 600; poder **1.044**.
- Chance da caravana: 540 ÷ 1.584 = **34%**. A força simples (8 contra 12) dava 40%. **Perto**, um pouco pior para a caravana (o risco do Comércio sobe um pouco: medir no teste 7 dele).

### 10.6 Verificações

| Pergunta | Resultado |
|---|---|
| O cálculo rápido bate com o combate visto? | **Sim** no duelo (78% e 29% contra o visto 73 de Vida) e na caravana (34% contra 40%). Medir no teste 4 |
| O jogador sozinho sobrevive na estrada? | Contra 1 a 2 bandidos, sim; contra 6, 8,8%. **Muda a Viagem:** a luta fica pior que os 20% de antes; o Comércio muda pouco (a caravana vence 34% em vez de 40% contra 6 bandidos) |
| O dano mínimo deixa alguém imortal? | **Não:** sempre pelo menos 1 por golpe |
| A evolução sobe rápido demais? | **Não:** +0,40 num duelo; de 50 a 60 leva cerca de 25 duelos |
| O teste de 1 ano muda? | **Não:** a vila não tem combate; a Vida dos NPCs da vila só cai em combate |

### 10.7 Casos extremos

| Caso | O que acontece |
|---|---|
| Dois golpes no mesmo tick | Os dois acontecem, na ordem do número do NPC; quem cai no primeiro não dá o segundo |
| Um lado sem nenhum combatente | Não há luta: o outro vence sem perda |
| Poder 0 dos dois lados (só civis desarmados contra civis) | Dano mínimo 1: o poder nunca é 0 |
| Atacar alguém Inconsciente no modo não letal | Não dá dano (já está fora); no letal, é o golpe de misericórdia |
| Fugir no Mapa Interno sem saída | Rendição, se o Medo passar de 90 |
| Mais de 40 combatentes | Os que passam do limite ficam esperando fora do raio (Mapa de Guerra para Esquadrões, Batalha) |

### 10.8 Decidido em modo automático (rodada CB1, seguir sugestão)

1. **CB1-1:** o poder de um lado = soma do dano por tick × soma da Vida; a chance = poder ÷ soma dos poderes. Substitui a força simples da Viagem (VI1-4).
2. **CB1-2:** sem armadura conta como Leve na vantagem.
3. **CB1-3:** os guardas usam não letal contra o jogador, a menos que ele tenha matado alguém.
4. **CB1-4:** a testemunha é qualquer NPC acordado a 10 tiles, com 70% à noite, até o NPC ter Percepção.
5. **CB1-5:** a Coragem padrão é 60 até a Personalidade existir.
6. **CB1-6:** os números das armas e armaduras são uma proposta em dados (`combate.json`), a ajustar com o conteúdo.
