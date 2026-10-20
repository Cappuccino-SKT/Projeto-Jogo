---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase E2 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com IN1 vêm da **rodada IN1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Início de Jogo

## 1. O que faz

No **Novo Jogo**, o jogador escolhe um **estilo de início** (ou monta um personalizado com 100 pontos). O estilo põe o personagem num ponto de partida, com cultura, dinheiro, itens, companheiros, habilidades, reputação e o que ele já sabe do mapa. **Nada disso muda uma regra do jogo**: são só bônus do começo.

Documento: [[Sistema de Início de Jogo/Descrição|Início de Jogo]]; os pontos de partida, em [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]].

## 2. Dados

### `data/mundo/estilos.json` (arquivo novo)

```json
{
  "id": "estilo.mercador",
  "cultura": "do_ponto",
  "pontos_de_partida": [[7, 5], [2, 3], [5, 6]],
  "dinheiro": 800,
  "itens": [ ["item.cavalo", 2] ],
  "companheiros": [ { "foco": "comercio", "qualidade": "comum", "cargo": "lider_de_caravana", "familia": false } ],
  "habilidades": { "carisma": 15 },
  "reputacao": [],
  "conhecimento": { "cidades_com_posto_comercial": true, "precos_das_cidades_vizinhas": 3 }
}
```

| Estilo | Dinheiro | Itens e grupo | Habilidade e reputação | Sabe do mapa |
|---|---|---|---|---|
| **Dono de vila** | 300 | Assentamento nível 1 (1 Casa, 1 Fazenda, 1 Posto de Coleta **e 1 Padaria**; IN1-1), 40 Tábuas, 40 Blocos de Pedra, **40 Pães** (IN1-1), 4 colonos Civis; o Banco do assentamento com **400** (IN1-2) | +10 de Especialista (IN1-3) | Os vizinhos |
| **Aventureiro** | 150 | 2 companheiros comuns de combate; arma e armadura Leve Rank 2 | +10 em Corpo a Corpo ou Longo Alcance | As masmorras não lendárias da região |
| **Membro de facção** | 200 | Título Cidadão (Cavaleiro, se Nobre); uma Casa na cidade | **+30** com a facção (IN1-4) | O território da facção |
| **Mercador** | 800 | 2 Cavalos; 1 companheiro comum (líder de caravana) | +15 de Carisma | As cidades com Posto Comercial; os preços de 3 cidades vizinhas |
| **Nobre exilado** | 400 | 2 companheiros nobres **da família** (sem salário; IN1-5); Espada Longa Rank 2 | +20 com a facção da cultura dele; **−20** com a que tomou o trono (IN1-4) | Os vizinhos |
| **Escravo** | 0 | 2 escravos aliados (Vínculo alto); Grilhões | +20 de Braçal; −20 com o dono e a facção dele | O mapa onde está |
| **Mago** | 100 | Cajado Rank 2, 3 Pedras de Mana | Mago; +20 de Magia | Os vizinhos |
| **Artesão** (novo, a fatia; IN1-6) | 100 | Uma Carpintaria nível 2 com 2 empregados, na Vila Livre do Centro (4,3) | — | Os vizinhos |

**Personalizado:** 100 pontos, a partir de um Civil sem nada; a cultura é grátis. Custos do documento: 10 moedas = 1; companheiro 15; +10 numa habilidade 10; item 1 a cada 10 moedas; ser Mago 30; assentamento 40; título Cidadão 10.

### O que o jogo guarda

`estiloDoInicio` e `pontoDePartida` (no save, para o epílogo e a Crônica).

## 3. Quando roda

Uma vez, no passo 9 da criação do mundo ([[Ficha Logica/Mundo|Mundo]], 4), depois das facções e antes do clima do dia 1.

## 4. Regras passo a passo

### 4.1 O Novo Jogo (3 passos da Interface)

1. **Estilo** (ou personalizado) e **ponto de partida** (um da lista, ou sorteado).
2. **Narrador** (os Eventos; fora desta versão: o narrador padrão).
3. **Personagem:** nome, aparência (paleta); a **cultura** é a do estilo; com `"do_ponto"`, a da região do ponto (IN1-7). Idade: 6 anos (adulto jovem); habilidades da faixa Civil (20 a 60) + o bônus; Personalidade não (o jogador decide por ele).

### 4.2 Aplicar o estilo

1. Põe o personagem no ponto: no `entrada_do_mapa` do mapa (se tiver arquivo do Tiled) ou no Mapa Mundial.
2. Dinheiro, itens (no inventário do personagem), companheiros (gerados como os recrutáveis, 4.1 dos [[Ficha Logica/Companheiros|Companheiros]], com Opinião +40; os da **família** com +60 e sem salário).
3. **Reputação:** a base de cada facção = a afinidade da cultura (+20 mesma, +10 afim, 0, −15 hostil) + o bônus do estilo. Liga a regra que a [[Ficha Logica/Reputacao|Reputação]] deixou em 0 (RE1-6).
4. **Conhecimento:** marca como vistos (névoa da Viagem, 4.10) os mapas do estilo, com a data do dia 1; os preços conhecidos das cidades do estilo.

### 4.3 Os estilos com regra a mais

- **Dono de vila:** cria uma **cidade nova** de nível 1 no ponto (5,2), (2,5) ou (5,3), com as construções do estilo e os 4 colonos; o jogador é o **governador** dela: escolhe a obra da vez (Cidade) e o patrocínio dos festivais. Em (5,2) e (2,5), a vila nasce **vinculada** à facção do território (leis, verba da facção); em (5,3), **independente** (verba "de fora", FA1-3). O mapa passa a "com cidade" (o território em volta é recalculado).
- **Membro de facção:** o jogador começa como **membro** (o Rito de Admissão já cumprido), no estado "Membro" da Facção.
- **Escravo:** o jogador entra como Escravo de uma cidade do ponto ((7,5) ou (8,6)); os 3 caminhos da [[Ficha Logica/Escravidao|Escravidão]] (4.8).
- **Artesão:** o começo da fatia vertical: a vila de 15 NPCs da `vila_inicial` é a **Vila Livre do Centro** (4,3), e o jogador tem a Carpintaria nível 2 com 2 empregados e 100 moedas (IN1-6).

## 5. Contas

**Os pontos de cada estilo** (preços base da Economia; os itens sem preço ainda contam pela estimativa):

| Estilo | Conta | Pontos |
|---|---|---|
| Dono de vila | 300 moedas (30) + assentamento (40) + Tábuas 60 e Blocos 40 moedas (10) + Pães 68 (7) + Especialista (10) | **97** |
| Aventureiro | 15 + 2 companheiros (30) + 3 itens Rank 2 (cerca de 15) + habilidade (10) | **70** |
| Membro de facção | 20 + Cidadão (10) + Casa (cerca de 20) + reputação +30 (sem custo na tabela) | **50 + reputação** |
| Mercador | 80 + 2 Cavalos (cerca de 10) + companheiro (15) + Carisma (15) + preços conhecidos | **120** |
| Nobre exilado | 40 + 2 companheiros (30) + espada (cerca de 5) + reputação | **75 + reputação** |
| Mago | 10 + Mago (30) + Magia +20 (20) + cajado e Pedras (cerca de 10) | **70** |

Os estilos ficam entre 50 e 120 pontos, e a reputação não tem preço na tabela do personalizado. **Achado (IN1-8):** o documento diz que "cada estilo vale cerca de 100". A ficha deixa como está, porque os estilos mais baratos trazem mais risco (o Aventureiro e o Mago dependem de masmorras), e o personalizado ganha a reputação a 1 ponto a cada 2 de bônus.

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Mundo | O passo 9 da criação; os pontos de partida |
| Reputação | A base por afinidade + o bônus do estilo (liga a RE1-6) |
| Companheiros | Os companheiros iniciais; a família sem salário |
| Cidade, Trabalho | O assentamento do Dono de vila; o jogador governador |
| Escravidão | O estilo Escravo |
| Viagem | O conhecimento inicial (névoa) |
| Comércio | Os preços conhecidos do Mercador |
| Fatia vertical | O estilo Artesão é o começo dela, no mundo |
| Save | O estilo e o ponto; o formato passa para a **versão 22** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `EstiloDeInicio` | Uma linha do `estilos.json` |
| `aplicarEstiloDeInicio(estilo, ponto)` | Regra 4.2 |
| `criarAssentamentoDoJogador(ponto)` | Regra 4.3, Dono de vila |
| `custoEmPontos(escolhasDoPersonalizado)` | O personalizado |

## 8. Testes

1. **Dono de vila, 16 dias:** os 4 colonos comem todo dia, recebem o salário e não saem (parte 10.2).
2. **Nobre exilado, 2 estações:** os 2 companheiros da família ficam, com Opinião +60.
3. **Mercador:** os preços das 3 cidades vizinhas aparecem no Mapa Mundial com a data do dia 1.
4. **Reputação:** um Mago Arcano Membro da Arcana começa com +20 + 30 = **+50** (Amigável) na Arcana e −15 na Religiosa e na Tecnocrata.
5. **Personalizado:** 101 pontos é recusado.
6. **Artesão:** o começo é igual ao da fatia (o teste de 1 ano roda o mesmo).

## 9. Fora desta versão

- O Narrador (Eventos).
- Os títulos de verdade (Objetivo e Progressão, E3).
- A aparência do personagem (arte).
- Os estilos novos (dados).

## 10. Simulação de mesa

### 10.1 Cenário

O **Dono de vila** em (5,3) (independente). O assentamento do documento: 1 Casa, 1 Fazenda, 1 Posto de Coleta, 4 colonos.

### 10.2 Os primeiros dias do assentamento

**Como o documento escreveu:**

| Dia | O que acontece |
|---|---|
| 1 | Os 4 colonos trabalham na Fazenda (Grão) e no Posto de Coleta (Água, Tora). Ninguém faz Pão: o Grão é matéria-prima, não comida |
| 2 | Consumo: nenhum Pão no estoque. **Fome** (Felicidade −25) |
| 2 | Salários: o Banco do assentamento tem **0**. Ninguém recebe |
| 4 | Fome da População (3 dias): começam as mortes do acumulado |
| 17 | 1 estação sem salário: **os colonos saem** (Trabalho) |

**Com a IN1-1 e a IN1-2:** 1 Padaria e 40 Pães (10 dias para 4) dão tempo de a Padaria começar; o Banco com 400 paga 4 × cerca de 6 = 24 por dia, e o dinheiro volta pelo consumo e pelo imposto (FA1-2). A verba "de fora" (5 por NPC por estação = 20) cobre o pouco que o jogador tira.

| Dia | Estoque de Pão | Padaria (1 padeiro, habilidade 40) | Banco |
|---|---|---|---|
| 1 | 40 | Sem Grão ainda | 400 |
| 2 | 36 | O Grão da Fazenda chega; o 1º ciclo começa | cerca de 400 (paga 24, recebe cerca de 24) |
| 3 em diante | sobe cerca de 3 por dia | 1 padeiro a 90%, 7,9 horas: 6.320 × 90 = 568.800 → cerca de 1,8 ciclo = **7 Pães** por dia, para 4 que comem (o Pão gasta cerca de 4 Grãos e 2 Águas por dia) | estável |

O assentamento se sustenta, e a sobra de Pão (cerca de 3 por dia) é o começo da economia da vila do jogador: vender para as caravanas ou para os migrantes que chegam.

### 10.3 O Nobre exilado

400 moedas e 2 companheiros nobres "bons" (150 por estação cada) dariam **300 por estação**: o dinheiro acaba na 2ª estação e os dois saem na 4ª. **Resolvido (IN1-5):** os companheiros do Nobre exilado são da **família** (a dinastia caída), sem salário e com Opinião +60, como a família nos Companheiros.

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| O Dono de vila sobrevive? | **Só com a IN1-1 e a IN1-2** (10.2) |
| O Nobre exilado mantém o grupo? | **Só com a IN1-5** (10.3) |
| O Aventureiro paga os 2 companheiros? | 150 moedas = 1,5 estação de salário (100 por estação). Ele precisa de uma masmorra ou de um contrato nos primeiros 24 dias (Objetivo e Progressão). É o risco do estilo |
| A reputação do documento bate? | Havia duas versões: Reputação dizia Membro +20 e Nobre −30; o Início (V3-2, mais nova) diz +30 e −20. **Vale a V3-2** (IN1-4) |
| A fatia vertical cabe no jogo completo? | **Sim, com o estilo Artesão** (IN1-6): a vila da fatia é a Vila Livre do Centro |
| O teste de 1 ano muda? | **Não:** o `--simular` usa a `vila_inicial` como hoje |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Dono de vila num ponto já ocupado por uma cidade | Não acontece: os 3 pontos são mapas sem cidade |
| Escravo sem cidade com Mercado de Escravos no ponto | Os 2 pontos do estilo têm (7,5) e (8,6) |
| Personalizado com cultura Arcana e "ser Mago" | 30 pontos; a chance de nascer Mago não vale para o jogador (ele escolheu) |
| Ponto sorteado | Um dos pontos do estilo, com a sequência 8 |
| Membro de facção de uma facção em guerra no dia 1 | Começa em guerra (o mundo é igual para todos) |

### 10.6 Decidido em modo automático (rodada IN1, seguir sugestão)

1. **IN1-1:** o assentamento do Dono de vila ganha 1 Padaria e 40 Pães.
2. **IN1-2:** o Banco do assentamento começa com 400 (o dinheiro do nível 1).
3. **IN1-3:** o "+10 de Inteligência" do Dono de vila vira +10 de Especialista até existir a Inteligência.
4. **IN1-4:** valem os bônus de reputação da V3-2 (Membro +30; Nobre exilado +20 e −20).
5. **IN1-5:** os companheiros do Nobre exilado são da família: sem salário, Opinião +60.
6. **IN1-6:** um estilo novo, **Artesão**, é o começo da fatia vertical; a vila da fatia é a Vila Livre do Centro (4,3). **Para o autor conferir.**
7. **IN1-7:** a cultura do estilo "do ponto" é a da região do ponto de partida.
8. **IN1-8:** os estilos ficam entre 50 e 120 pontos; não se iguala a 100.
