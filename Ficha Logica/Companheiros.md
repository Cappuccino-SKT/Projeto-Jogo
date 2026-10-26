---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase D2 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com CP1 vêm da **rodada CP1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Companheiros e Diálogo

## 1. O que faz

Três partes:

1. **O grupo do jogador:** recrutar na Taverna, pagar por estação, a **Opinião** e a lealdade, os cargos fora do grupo e a morte.
2. **A família e as dinastias:** casamento, filhos, herdeiro; as dinastias da IA casam e têm filhos por estação, e o líder de cada facção passa a **envelhecer e morrer**, com sucessão.
3. **O diálogo:** as ações de conversa com a chance e a falha.

Documento do sistema: [[Sistema de Companheiros e Diálogo/Descrição|Companheiros, Relações familiares e Diálogo]].

**Esta versão** traz também o que os outros sistemas esperavam: a **Personalidade** (Coragem, Ganância, Agressividade, Sociabilidade, Lealdade) e o **Carisma** para os Importantes e os Controlados (CP1-1). O **Renome** vem do [[Ficha Logica/Objetivo|Objetivo e Progressão]] (4.1); sem ele (o teste da fatia), 0 (CP1-2).

## 2. Dados

### `data/companheiros.json` (arquivo novo)

```json
{
  "grupo_controlado": { "base": 4, "renome_por_um": 20 },
  "vinculados": { "base": 10, "renome_por_um": 5, "por_cidade_do_jogador": 3 },
  "recrutaveis_por_nivel_da_cidade": [0, 1, 2, 4],
  "qualidades": {
    "comum":     { "renome_minimo": 0,  "habilidade_principal": [20, 50], "entrada": 100,  "salario_por_estacao": 50 },
    "bom":       { "renome_minimo": 20, "habilidade_principal": [40, 70], "entrada": 300,  "salario_por_estacao": 150 },
    "excelente": { "renome_minimo": 50, "habilidade_principal": [60, 90], "entrada": 1000, "salario_por_estacao": 400 }
  },
  "estacoes_sem_salario_para_sair": 2,
  "opiniao": { "vitoria_junto": 2, "salario_em_dia": 1, "salario_atrasado": -10, "companheiro_morto": -5,
               "ordem_contra_a_personalidade": -5, "missao_pessoal": 10, "presente_por_moedas": 20 },
  "felicidade_do_controlado": { "salario_em_dia": 10, "salario_atrasado": -20, "sem_comida": -25, "por_companheiro_morto_na_estacao": -10, "minimo_mortos": -30 },
  "familia": { "gravidez_estacoes": 1, "chance_de_filho_por_estacao_em_porcento": 10,
               "chance_de_casar_por_estacao_em_porcento": 25, "adulto_aos": 4, "fertil_ate": 20 },
  "dialogo": { "base": 50, "habilidade_divide": 2, "opiniao_divide": 4, "traco_divide": 3, "limites": [5, 95],
               "falha_persuadir": -5, "falha_intimidar": -10, "barganha_em_porcento": 5 }
}
```

`presente_por_moedas`: +1 de Opinião a cada 20 moedas de valor, × (100 + Ganância − 50) ÷ 100, até +10 por presente.

### O NPC Importante e o Controlado ganham

| Campo | De 0 a 100 | Usado por |
|---|---|---|
| `coragem` | Medo e fuga | Combate (no lugar do 60 padrão) |
| `ganancia` | Barganhar, subornar, presentes | Diálogo; Crime (suborno de guarda, quando o guarda for Importante) |
| `agressividade`, `sociabilidade` | Veto do líder; reação à intimidação | Facção, Diálogo |
| `lealdade` | Velocidade da queda da Opinião | 4.3 |
| `carisma` (habilidade) | Persuadir, barganhar, líder de caravana | Diálogo, Comércio, Viagem (suborno) |
| `opiniaoSobreOJogador` | −100 a +100 | 4.3 |

Sorteados ao criar o NPC (de 20 a 80), com a sequência de NPCs da cidade. Os NPCs comuns não ganham nada: valem 50 quando alguém pergunta.

### Estado (vai no save)

- **Do jogador:** a lista dos vinculados (onde está, o cargo, estações sem salário), o grupo controlado, o cônjuge, os filhos (idade, foco de educação), o sucessor escolhido.
- **De cada facção:** a **dinastia** (o líder, o cônjuge, os filhos, 2 nobres), cada um um NPC Importante com idade.
- **De cada Taverna:** os recrutáveis da estação.

## 3. Quando roda

| Quando | O quê |
|---|---|
| **Dia 1 de cada estação** | Salários (4.2); recrutáveis novos (4.1); casamentos e filhos da IA (4.6); a Opinião pelo salário |
| **A cada dia, passo 5** | A Felicidade dos Controlados (4.4) |
| Ao morrer um líder | A sucessão (4.7) |
| Quando o jogador conversa | O diálogo (4.9) |

## 4. Regras passo a passo

### 4.1 Recrutar

1. No dia 1 de cada estação, cada Taverna gera os recrutáveis: 1 no nível 2, 2 no nível 3, 4 no nível 4 (o nível 1 não tem Taverna). Cada um sorteia o foco (Combate, Comércio, Ofício) e a qualidade: a melhor que o **Renome** do jogador deixa, com 50% de chance de cair um degrau.
2. São NPCs novos (não saem dos trabalhadores da cidade), da cultura da cidade.
3. **Recrutar:** paga a entrada; o NPC vira **Controlado** e entra no grupo (ou fica vinculado, se o grupo estiver cheio). Opinião inicial: +20.
4. **Limites** (Renome 0): grupo **4**, vinculados **10**. A família não conta.

### 4.2 O salário (dia 1 de cada estação)

1. Paga cada vinculado do dinheiro do jogador. O dinheiro vai para a **classe Civil da cidade de origem** dele (como o salário da caravana; o dinheiro fica no mundo; CP1-3).
2. Faltou dinheiro: o vinculado conta 1 estação sem salário (Opinião −10, Felicidade −20). Com **2**, ele sai.

### 4.3 A Opinião e a lealdade

1. As mudanças da tabela `opiniao`. As **quedas** × (150 − Lealdade) ÷ 100 (Lealdade 80: 70%; 20: 130%).
2. A lealdade pela faixa da Opinião:

| Opinião | Efeito |
|---|---|
| +60 ou mais | Com Felicidade 0, fica mais 1 estação antes de sair |
| +20 a +59 | Sai com Felicidade 0 |
| −19 a +19 | A Felicidade dele anda 25% mais rápido para baixo |
| −20 a −59 | Pede +25% de salário no dia 1 da estação; se o jogador recusa, −10 |
| −60 ou menos | Na noite seguinte, 25% de chance de desertar com o equipamento dele |

### 4.4 A Felicidade do Controlado (passo 5)

Alvo = 50 + salário (+10 em dia, −20 atrasado) + comida (−25 se o grupo ficou sem comida hoje) + companheiros mortos (−10 por morto na estação, até −30) + eventos. Passo de 10% ao dia, como a da cidade. Os outros fatores do documento (moradia, cargo) **[Refinar depois]**.

### 4.5 Os cargos

| Cargo | Efeito nesta versão |
|---|---|
| Gerente de construção do jogador | O "gerente" que o salário já prevê (Economia): +1% de produção a cada 5 de Especialista acima de 40, até +12% |
| Líder de caravana do jogador | −10% de risco e +5% de lucro a cada 20 de Carisma acima de 30 (Comércio, 4.3 e 4.5) |
| Governador, Professor, Comandante | Quando o jogador tiver cidade, Universidade e Esquadrões |

### 4.6 A família e as dinastias da IA (dia 1 de cada estação)

1. **Casar:** cada adulto solteiro da dinastia (e o jogador, se pedir; 4.8) tem 25% por estação de casar com um nobre gerado da mesma cultura.
2. **Filhos:** cada casal com a mulher adulta, entre 5 e 30 anos (rodada HI12; antes, 4 a 20): 10% por estação. A gravidez dura 1 estação; o filho nasce como Importante, criança até os 5 anos.
3. **Envelhecer e morrer:** os Importantes seguem a velhice da [[Ficha Logica/Populacao|População]] (a partir de 31 anos, rodada HI12). A ficha da Facção dizia "o líder não morre": **agora morre** (CP1-4). **Exceção (rodada HI11-1, 2026-10-26):** o Grão-Mestre da Ordem de Custodum tem o traço **Vida prolongada**: não envelhece enquanto a Ordem tiver uma Torre Arcana com Magos trabalhando ([[Ficha Logica/Magia|Magia]]). Se a Torre cair (guerra, crise), ele envelhece de uma vez até a idade de velho e segue a velhice normal. O traço é de Cassius Varro (131 anos na história), não do cargo.
4. **A dinastia do começo (CP1-5):** o líder tem de **8 a 30 anos** (rodada HI12: no jogo, adulto é de 5 a 30; antes, 8 a 18). O líder do começo é fixo (rodada HI7-1, [[Ficha Logica/Faccao|Facção]] 4.6). Se ele tem 12 ou mais, já começa casado e com 1 ou 2 filhos adultos.

### 4.7 A sucessão

Quando o líder morre, o herdeiro sai da regra de sucessão da cultura:

| Regra | Herdeiro nesta versão |
|---|---|
| Primogenitura (herança) | O filho adulto mais velho |
| Eleição entre nobres | O adulto da dinastia de maior Sociabilidade |
| Duelo ou mérito | O adulto da dinastia de maior Corpo a Corpo (Guerreira) ou maior Especialista (Tecnocrata) |
| Escolha do líder | O filho adulto de maior Opinião... do líder morto: o mais velho |
| Escolha do clero (Religiosa) | O adulto da dinastia de maior Sociabilidade (o Sumo Sacerdote escolhe) |

**Sem ninguém adulto:** crise. Nesta versão, um dos 2 nobres vira líder e a legitimidade cai para 30; a guerra civil entra com a Guerra (D7). Notícia **Mundial** "Morre o líder da Arcana; sobe X".

### 4.8 O casamento e a sucessão do jogador

1. **Pedir casamento** (diálogo): Opinião +60 ou mais, título compatível (com os títulos do Objetivo; até lá, o jogador só casa fora da Nobreza e da Realeza) e o **costume** da cultura do outro (dote, duelo, cerimônia no Templo, permissão, contrato de 50, Magos). Efeitos: o cônjuge vira Controlado; +20 de reputação com a facção da família dele.
2. **Filhos:** a regra 4.6; o jogador escolhe o foco de educação (Combate, Magia, Comércio, Governo); aos 5 anos, as habilidades do foco nascem no topo da faixa.
3. **Morte do jogador:** o filho adulto mais velho assume; sem filho, o companheiro com Opinião +60 ou mais que o jogador escolheu (ou o de maior Opinião). Sem ninguém: fim da partida, com o epílogo.

### 4.9 O diálogo

1. **Ações com teste:** chance = 50 + (habilidade do jogador − 50) ÷ 2 + Opinião ÷ 4 ± (traço do NPC − 50) ÷ 3, entre 5% e 95%. O traço e o sinal: Persuadir e Barganhar, −Ganância; Intimidar, −Coragem (a habilidade é Corpo a Corpo, até existir a Força); Subornar (fora dos guardas), +Ganância.
2. **Efeitos nesta versão:**
   - **Conversar:** uma notícia Local ou Regional que já chegou à cidade ([[Ficha Logica/Noticias|Notícias]]); o jogador passa a saber dela;
   - **Perguntar sobre preços:** atualiza os preços conhecidos daquela cidade;
   - **Barganhar** (com o dono ou o gerente de uma loja): +5% na venda ou −5% na compra, nessa transação;
   - **Persuadir:** um favor das Missões (fase E); nesta versão, −10% na próxima compra na loja dele;
   - **Elogiar ou presentear:** a Opinião;
   - **Intimidar:** como persuadir, mas a falha tira 10 e, com Agressividade acima de 70, o NPC ataca (Combate).
3. **Falha:** Persuadir e Barganhar, Opinião −5; Intimidar, −10.
4. **NPCs comuns:** só o menu curto (comprar e vender, um rumor, pedir caminho).

## 5. Contas

| Conta | Valor |
|---|---|
| O exemplo do documento: Carisma 70, Opinião +20, Ganância 80 | 50 + 10 + 5 − 10 = **55%** (bate) |
| Lucro do jogador na Carpintaria (FE3) | cerca de 3,4 a 4,4 por dia = **54 a 70 por estação** |
| 1 companheiro comum | 100 de entrada + **50 por estação** |
| Lucro de uma viagem de comércio ao vilarejo (Comércio, 10.4) | cerca de **36** |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Combate | A Coragem no Medo; a Comitiva; vitórias dão Opinião; o companheiro morto pesa nos outros |
| Facção | O líder envelhece e morre; a sucessão; a idade inicial do líder (CP1-5); a Sociabilidade e a Agressividade no veto |
| População | A velhice dos Importantes; os filhos |
| Reputação | +20 pelo casamento; a Opinião é o outro lado da reputação |
| Comércio | O líder de caravana (Carisma) |
| Economia | O gerente; o salário vai para a classe Civil |
| Notícias | Conversar dá notícias; a morte de um líder é Mundial |
| Viagem | O Carisma no suborno dos bandidos (já previsto) |
| Save | Personalidade, Carisma, Opinião, vinculados, família, dinastias; o formato passa para a **versão 15** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Personalidade` | Os 5 traços dos Importantes e Controlados |
| `gerarRecrutaveisDaTaverna(cidade)` | Regra 4.1 |
| `pagarVinculados()` | Regra 4.2 |
| `mudarOpiniao(npc, valor, motivo)` | Regra 4.3 |
| `felicidadeDoControlado(npc)` | Regra 4.4 |
| `casarEDarFilhosDasDinastias()` | Regra 4.6 |
| `escolherHerdeiro(faccao)` | Regra 4.7 |
| `chanceDaAcaoDeDialogo(acao, jogador, npc)` | Regra 4.9 |

## 8. Testes

1. **A chance de 55%** do exemplo do documento.
2. **Salário:** sem dinheiro 2 estações, o companheiro sai; com Lealdade 80, a Opinião cai 7 por estação atrasada (10 × 70%).
3. **Limites:** com Renome 0, o 5º recruta fica vinculado fora do grupo; o 11º não pode ser recrutado.
4. **Dinastia:** em 1.000 mundos sorteados, a proporção de mortes de líder **sem herdeiro adulto** fica abaixo de 10% nos primeiros 20 anos.
5. **Sucessão:** a Guerreira (duelo ou mérito) escolhe o adulto de maior Corpo a Corpo.
6. **O teste de 1 ano não muda:** a vila não tem Importantes nem companheiros.

## 9. Fora desta versão

- O Renome e os títulos (Objetivo e Progressão, fase E): até lá, Renome 0.
- As missões pessoais e os favores de verdade; as conversas escritas à mão; as frases modulares (conteúdo).
- Governador, Professor, Comandante (quando houver cidade do jogador, Universidade e Esquadrões).
- O casamento entre dinastias como evento diplomático (Diplomacia, D5).
- A guerra civil (Guerra, D7).
- A Força, a Inteligência e a Percepção como atributos.

## 10. Simulação de mesa

### 10.1 Cenário

O jogador é Aventureiro, com 300 moedas e Renome 0. A Cidade Religiosa (7,3), nível 3, tem 2 recrutáveis nesta estação: um lanceiro comum (Corpo a Corpo 42) e um arqueiro comum (Longo Alcance 35).

### 10.2 Um ano com 2 companheiros

| Quando | O que acontece | Dinheiro | Opinião (lanceiro, Lealdade 40) |
|---|---|---|---|
| Primavera, dia 1 | Recruta os dois: 200 | 100 | +20 |
| Primavera, dia 3 | Vence 3 bandidos (cálculo rápido: o grupo tem poder bem maior) | 100 | +22 |
| Verão, dia 1 | Salário: 100 | 0 | +23 |
| Verão | Ganha 80 no comércio | 80 | +23 |
| Outono, dia 1 | Salário: paga o lanceiro (50); sobram 30, que não pagam o arqueiro (o último recrutado fica por último) | 30 | +24; o arqueiro −10 × 1,1 = −11 |
| Inverno, dia 1 | 30 não pagam ninguém: os dois sem salário | 30 | lanceiro −11 → +13; arqueiro: 2ª estação → **sai** |

**Achado:** o lucro da Carpintaria (54 a 70 por estação) não paga nem 2 companheiros comuns (100 por estação), e o jogador sozinho tem 8,8% contra 6 bandidos (Combate, 10.4). O começo fica apertado de propósito: o primeiro companheiro vem com o comércio, não com a oficina. **Decisão (CP1-6):** fica assim, e a Interface mostra o custo por estação antes de recrutar. Os estilos com dinheiro (Mercador, 800) começam com companheiros mais cedo, como o documento quer.

### 10.3 A dinastia Arcana em 20 anos

| Ano | O que acontece |
|---|---|
| 1 | Líder com **16 anos**, casado, 1 filho adulto (6 anos) e 1 filha criança (2 anos) (CP1-5) |
| 3 | A filha faz 4 anos: 2 adultos na linha |
| 5 | Líder com 20; nasce um terceiro filho (10% por estação, cerca de 1 a cada 2,5 anos) |
| 6 a 10 | Velhice a partir dos 21: a chance de morrer cresce a cada estação (População) |
| cerca de 8 | **Morre o líder** (24 anos). Sucessão Arcana (poder mágico; nesta versão, o adulto de maior Especialista até a Magia): sobe a filha |

**Sem a CP1-5:** com a idade de 30 a 50 da ficha da Facção, todo líder já começaria "velho" (acima de 21) e morreria nas primeiras estações. E um líder de 18 sem filhos adultos morreria em cerca de 3 a 8 anos, enquanto o primeiro herdeiro levaria cerca de 2,5 anos para nascer e mais 4 para ser adulto: **crise de sucessão em quase metade das facções**.

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| A fórmula do diálogo bate com o documento? | **Sim:** 55% (parte 5), com o traço ÷ 3 |
| O dinheiro dos salários some? | **Não:** vai para a classe Civil da cidade de origem (CP1-3) |
| As dinastias têm herdeiro? | **Sim, com a CP1-5** (teste 4) |
| O jogador sem dinheiro perde o grupo? | Em 2 estações; o fiel (+60) segura mais 1 |
| O teste de 1 ano muda? | **Não** |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| O jogador morre sem filho nem companheiro +60 | Escolhe o companheiro de maior Opinião, se houver; sem ninguém, epílogo |
| Dois filhos adultos com a mesma idade (gêmeos) | O de menor número de NPC |
| O companheiro desertor leva a arma que o jogador comprou | Leva: o equipamento está com ele (como o documento manda) |
| A Taverna fecha (sem Taverna depois de cair de nível) | Sem recrutáveis |
| Casar com um companheiro já vinculado | Ele deixa de contar no limite dos vinculados (vira família) |
| O líder morre no meio de uma assembleia | A assembleia é no dia 1 do ano; a morte, na estação: o novo líder vale na próxima |

### 10.6 O que muda em outras fichas

- **Facção:** a idade do líder (8 a 18, CP1-5); o líder envelhece e morre; a sucessão (4.7).
- **Combate:** a Coragem vem da Personalidade para Importantes e Controlados (os comuns continuam com 60).

### 10.7 Decidido em modo automático (rodada CP1, seguir sugestão)

1. **CP1-1:** a Personalidade (5 traços) e o Carisma entram nesta ficha, só para Importantes e Controlados.
2. **CP1-2:** Renome 0 até o Objetivo e Progressão.
3. **CP1-3:** o salário dos companheiros vai para a classe Civil da cidade de origem deles.
4. **CP1-4:** o líder da facção envelhece e morre pela velhice da População.
5. **CP1-5:** a dinastia do começo: líder de 8 a 18 anos; com 12 ou mais, casado e com 1 ou 2 filhos adultos.
6. **CP1-6:** o começo do jogador sem dinheiro para companheiros fica assim; a interface mostra o custo antes de recrutar.
7. **CP1-7:** o traço do NPC no diálogo vale (traço − 50) ÷ 3.
