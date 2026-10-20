---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase E3 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]], a última do plano), com a **parte 10: Simulação de mesa**. Os pontos marcados com OB1 vêm da **rodada OB1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Objetivo e Progressão

## 1. O que faz

O jogo não tem fim obrigatório. O jogador escolhe uma **Ambição** e progride por **Renome** (0 a 100), **títulos**, dinheiro, terras e habilidades. O mundo oferece **contratos** que nascem dos problemas dele (bandidos, caravanas em risco, falta de um item, masmorra povoada). As **masmorras** dão Loot e Renome. A **Crônica** guarda os grandes feitos para o epílogo.

Documento: [[Objetivo e Progressão/Descrição|Objetivo do Jogo e Progressão]].

**Esta versão:** o Renome, os títulos com os requisitos, as condições das Ambições, os 4 tipos de contrato, as masmorras (com o mapa desenhado ou pelo cálculo rápido), o Loot e a Crônica. **As missões longas, os epílogos escritos e as listas de Loot e de Criaturas** são conteúdo (rodada C3).

## 2. Dados

### `data/objetivo.json` (arquivo novo)

```json
{
  "renome": { "contrato": 2, "masmorra_por_dificuldade": 3, "masmorra_lendaria": 20, "batalha_vencida": 2,
              "missao_longa": 5, "ambicao": 20, "maximo": 100 },
  "titulos": [
    { "id": "plebeu",   "renome": 0 },
    { "id": "cidadao",  "renome": 5,  "requisito": "casa_na_cidade_ou_membro" },
    { "id": "cavaleiro", "renome": 20, "reputacao": 20, "requisito": "servico_3_contratos_ou_1_batalha" },
    { "id": "nobre",    "renome": 40, "reputacao": 40, "requisito": "rito_de_admissao" },
    { "id": "senhor_de_cidade", "renome": 60, "reputacao": 60, "requisito": "governar_uma_cidade" },
    { "id": "lider_de_faccao", "requisito": "ser_lider" }
  ],
  "ritos": { "pagar_titulo": 2000, "servico_contratos": 5, "indicacao_opiniao_de_nobre": 40, "tributo_em_moedas": 1000 },
  "contratos": {
    "maximo_por_taverna": 3, "dias_no_quadro": 16,
    "cacar_bandidos": { "ameaca_minima_por_mil": 30, "recompensa_por_bandido": 20 },
    "escolta": { "risco_minimo_por_mil": 100, "recompensa_em_porcento_da_carga": 10 },
    "entrega": { "falta_minima": 20, "bonus_em_porcento": 10 },
    "limpar_masmorra": { "recompensa_por_dificuldade_ao_quadrado": 50 }
  },
  "masmorra": { "inimigos_por_dificuldade": 3, "etapas_sem_mapa": 3, "repovoa_em_estacoes": 4,
                "loot_moedas_por_dificuldade_ao_quadrado": 50, "grimorio_por_dificuldade_em_porcento": 5 }
}
```

Todos os números são **[Proposta]** (o documento deixa os valores para testes).

### As Ambições (condições, OB1-1)

| Ambição | Cumprida quando |
|---|---|
| Fundar uma facção | O jogador cria uma facção (Facção: cidade nível 3, 2 nobres, 2.000 moedas) |
| Conquistar a liberdade | O jogador deixa de ser Escravo (Escravidão, 4.8) |
| Retomar o trono | O jogador é líder da facção que tomou o trono da dinastia dele |
| Ter a maior fortuna do mundo | O dinheiro + o valor das construções do jogador passam da **metade do maior PIB** de facção |
| Limpar uma masmorra lendária | Uma das 3 |
| Tornar-se Arquimago | Magia 90 ou mais e a missão longa do Arquimago (Importante fixo) cumprida |
| Liderar a facção | O jogador é líder da facção de que é membro |
| Conquistar uma capital | Uma capital passa à facção do jogador |
| Unificar uma região | Todas as cidades de uma região são da facção do jogador |

### Estado (vai no save)

`ambicaoAtual`, `renome`, `tituloPorFaccao` (o título é por facção), `contratosAtivos`, `masmorras` (povoada ou limpa, e quando repovoa), `cronica`.

**O sorteio:** os contratos não sorteiam (nascem do estado); o Loot e os inimigos usam a sequência 8 (Mundo); a luta, a da Viagem (5).

## 3. Quando roda

| Quando | O quê |
|---|---|
| **A cada dia, passo 6c (novo)**, depois dos eventos | A varredura dos contratos (4.3) |
| Ao cumprir um feito | O Renome (4.1); a Crônica (4.6) |
| Quando o jogador pede | O título (4.2) |
| **Dia 1 de cada estação** | As masmorras repovoam (4.4) |
| A cada dia | Confere a Ambição (4.5) |

## 4. Regras passo a passo

### 4.1 O Renome

Sobe com os feitos (contrato +2, masmorra +3 × dificuldade, lendária +20, batalha vencida na Comitiva +2, missão longa +5, Ambição +20), até 100. **Não cai.** A má fama é a Recompensa (Crime). O Renome libera a qualidade dos recrutas e o tamanho do grupo (Companheiros: grupo 4 + Renome ÷ 20).

### 4.2 Os títulos (por facção)

1. **Pedir** um título a uma facção: precisa do **Renome**, da **reputação** com ela e do **requisito** do degrau.
2. **Nobre: o Rito de Admissão** da cultura da facção:

| Cultura | Rito | Nesta versão |
|---|---|---|
| Escravista | Pagar título | 2.000 moedas |
| Guerreira | Provar-se em combate | Vencer uma batalha (na Comitiva) ou um duelo |
| Religiosa | Serviço | A peregrinação (Religião) e 5 contratos na facção |
| Autoritária | Indicação | Opinião +40 de um nobre da dinastia |
| Tecnocrata | Serviço | 5 contratos na facção |
| Arcana | Provar-se na magia | Ser Mago com Magia 60 ou mais |

3. **Efeito:** o título é um rótulo político; abre as missões de nobres e o casamento com Nobreza (Companheiros). A **classe** não muda (só pelo Rito, que leva a Rico no Cavaleiro e a Nobreza no Nobre, como o documento permite).
4. **Perde o título** quando a reputação cai para Inimigo (Reputação).

### 4.3 Os contratos (passo 6c)

Cada Taverna tem até 3. Um contrato só nasce se o problema existe, e **some** se o problema acaba ou depois de 16 dias:

| Tipo | Nasce quando | Recompensa (paga pelo Banco da cidade) | Cumprido quando |
|---|---|---|---|
| **Caçar bandidos** | A ameaça bandida da região (Crime, 4.3) ≥ 30 por mil | 20 × bandidos do bando (sorteado ao nascer) | O jogador vence o bando num mapa da região (o encontro é certo ao entrar no mapa marcado) |
| **Escolta** | Uma caravana da cidade vai sair com risco ≥ 100 por mil (Comércio, 4.3) | 10% do valor da carga | A caravana chega; o grupo do jogador viaja com ela e soma no cálculo rápido |
| **Entrega** | A falta de um item na cidade ≥ 20 e nenhuma caravana a caminho (Comércio, 4.2) | A loja paga o preço local + 10% até a falta | A entrega |
| **Limpar masmorra** | Uma masmorra povoada a 2 mapas ou menos | 50 × dificuldade² | A masmorra limpa (4.4) |

O dinheiro sai do **Banco da cidade** (só se ele tem a reserva de 250 mais a recompensa): fica no mundo. Cada contrato cumprido: Renome +2 e reputação +1 com a facção da cidade (Reputação). Os contratos aparecem como notícia Local (Notícias).

### 4.4 As masmorras

1. **Povoada:** 3 × dificuldade inimigos (bandidos com o kit de bandido; Criaturas quando houver), mais um chefe com o dobro da Vida.
2. **Entrar:** com o Mapa Interno desenhado, o combate local (Combate). **Sem o mapa** (até o conteúdo): **3 etapas** pelo cálculo rápido, cada uma com 1/3 dos inimigos; a Vida do grupo não volta entre elas (OB1-2).
3. **Limpa:** o Loot = 50 × dificuldade² moedas + itens de Rank = dificuldade (das tabelas) + Grimório com 5% × dificuldade + Pedras de Mana (1 a dificuldade). Renome +3 × dificuldade.
4. **Repovoa** depois de 4 estações; as **lendárias** não repovoam e guardam um Artefato.
5. **Derrota:** como no Combate (bandidos: a Escravidão, 50% de captura; Criaturas: acorda no lugar, perde o Loot carregado).

### 4.5 A Ambição

O jogador escolhe uma (o estilo sugere; pode trocar quando quiser). Quando a condição da tabela vale: Renome +20, o **epílogo** (a Crônica e a Crônica do mundo) e a escolha: continuar ou encerrar.

### 4.6 A Crônica

Entram: Ambições cumpridas, títulos ganhos, masmorras lendárias, batalhas vencidas na Comitiva, cidades fundadas ou tomadas, e as **gerações** da dinastia do jogador (casamento, filhos, morte e herdeiro). Sem pontuação. No epílogo, junto com a Crônica do mundo (Notícias).

## 5. Contas

| Masmorra | Inimigos | Poder dos inimigos (cálculo rápido) | Recompensa do contrato | Loot em moedas | Renome |
|---|---|---|---|---|---|
| Dificuldade 1 | 3 bandidos | 3 × 0,18 × 300 = 162 (contra a Leve Rank 1, Defesa 3) | 50 | 50 | +3 (+2 do contrato) |
| Dificuldade 2 | 6 | 648 | 200 | 200 | +6 (+2) |
| Dificuldade 4 | 12 | 2.592 | 800 | 800 | +12 (+2) |
| Lendária (5) | 15 + chefe | 16 × 0,18 × 1.700 = cerca de 4.900 | 1.250 | 1.250 + Artefato | +20 (+2) |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Companheiros | O Renome abre a qualidade e o tamanho do grupo (CP1-2 deixava 0) |
| Reputação | A faixa para os títulos; +1 por contrato |
| Crime | A ameaça bandida gera os contratos de caçar bandidos |
| Comércio | O risco da rota gera a escolta; a falta gera a entrega |
| Combate, Viagem | A luta na masmorra e no encontro marcado |
| Facção, Cultura | O Rito de Admissão; fundar e liderar |
| Guerra | A batalha vencida; conquistar a capital, unificar a região |
| Escravidão | A liberdade; a derrota para bandidos na masmorra |
| Magia | O Arquimago; o Grimório no Loot |
| Notícias | Os contratos (Local); a Crônica do mundo |
| Save | Renome, títulos, contratos, masmorras, Crônica; o formato passa para a **versão 23** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `ganharRenome(valor, feito)` | Regra 4.1 |
| `pedirTitulo(faccao, titulo)` | Regra 4.2 |
| `varrerContratos()` | Regra 4.3, passo 6c |
| `entrarNaMasmorra(masmorra)`, `resolverMasmorraSemMapa(masmorra, grupo)` | Regra 4.4 |
| `conferirAmbicao()` | Regra 4.5 |
| `registrarNaCronica(feito)` | Regra 4.6 |

## 8. Testes

1. **A tabela da parte 5.**
2. **Contrato nasce e some:** a ameaça da região passa de 30: o contrato aparece; ela cai para 20: o contrato some.
3. **O dinheiro do contrato** sai do Banco da cidade (a soma do mundo não muda); o do Loot entra de fora (conta em `dinheiroQueEntrouDeFora`).
4. **Masmorra sem mapa:** 3 etapas; a Vida do grupo continua entre elas.
5. **Título:** Renome 40 e reputação 40 com a Escravista e 2.000 moedas: Nobre; com 1.999, não.
6. **O teste de 1 ano não muda:** a vila do teste não tem Taverna (nível 1), e a varredura não muda nada sem o jogador agir.

## 9. Fora desta versão

- As missões longas, as missões dos Importantes fixos e os epílogos escritos (conteúdo C3).
- As Criaturas e as tabelas de Loot de verdade (conteúdo).
- Os Mapas Internos das masmorras (arte).
- O Sistema de Títulos de Nobreza por cultura (versão futura do documento).

## 10. Simulação de mesa

### 10.1 Cenário

O **Aventureiro** (Início de Jogo): o jogador (Espada 60, Leve Rank 2) e 2 companheiros comuns (Lança 45 e Arco 40), 150 moedas. Ele começa na Vila Livre do Norte (4,1), região Guerreira, perto da masmorra (3,1), dificuldade 2.

### 10.2 O primeiro ano

Todos com o kit Rank 2 do estilo (+25%) e armadura Leve (Defesa 3 × 1,25 = 3,75):

- Jogador: 10 × 60 ÷ 50 = 12 × 1,25 (corte contra o bandido sem armadura) × 1,25 (Rank) = 18,75 ÷ 20 = **0,94** por tick.
- Lanceiro: 9 × 45 ÷ 50 = 8,1 × 1,25 × 1,25 = 12,7 ÷ 20 = **0,63**. Arqueiro: 8 × 40 ÷ 50 = 6,4 × 1,25 = 8 ÷ 30 = **0,27**.
- Grupo: 1,84 × Vida 300 = poder **552**. Cada bandido contra o grupo: 11,2 × 0,75 − 3,75 = 4,65 ÷ 30 = **0,155** por tick.

| Quando | O que faz | Chance | Dinheiro | Renome |
|---|---|---|---|---|
| Primavera, dia 1 | 150 moedas; o salário da estação já está pago | | 150 | 0 |
| Primavera, dia 3 | Contrato: caçar 5 bandidos (ameaça da região 34 por mil): 100 moedas. Bando: 5 × 0,155 × 500 = 388 | 552 ÷ 940 = **59%** | | |
| | Vence (41% de chance de perder: captura ou roubo); perde 388 ÷ 552 = 70% da Vida | | 250 | 2 |
| Verão, dia 1 | Salário: 100 | | 150 | 2 |
| Verão | Masmorra (3,1), dificuldade 2: 3 etapas de 2 bandidos (poder 2 × 0,155 × 200 = 62 cada) | **90%** cada; as 3, cerca de **73%** | | |
| | Limpa: contrato 200 + Loot 200 + itens Rank 2 | | 550 | 2 + 6 + 2 = 10 |
| Outono e Inverno | 2 entregas e 1 caçada | | cerca de 650 | cerca de 16 |

**Achado:** o **bando inteiro de uma vez** (59%, e o grupo sai com 30% da Vida) é mais perigoso do que a **masmorra em etapas** (73%). O primeiro contrato de caçar bandidos é a parte mais arriscada do começo do Aventureiro. **Decidido (OB1-3):** entre as etapas da masmorra sem mapa, o grupo pode **acampar 8 horas** (a Vida volta 16%); e a Interface mostra a comparação de forças ("superior", "equilibrado") antes de aceitar um contrato de caçar bandidos (a Batalha já tem esse texto).

### 10.3 O ritmo do Renome

| Estilo de jogo | Renome por estação | Renome 20 (bons recrutas, Cavaleiro) | Renome 50 (excelentes) |
|---|---|---|---|
| Aventureiro (contratos e masmorras) | cerca de 6 | cerca de 1 ano | cerca de 2 anos (com masmorras de 3 e 4) |
| Mercador (só comércio) | cerca de 1 (escoltas e entregas) | cerca de 5 anos | raramente |
| Dono de vila | 0 a 2 | depende das batalhas | — |

O Mercador fica sem Renome se só comercia: ele cresce pela **fortuna**, não pela fama. É a separação que o documento quer (a Ambição dele é a fortuna).

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| Os contratos só nascem de um problema real? | **Sim:** os 4 tipos olham o estado (ameaça, risco, falta, masmorra) e somem quando ele acaba |
| O dinheiro dos contratos cria dinheiro? | **Não:** sai do Banco da cidade. O **Loot cria** (50 × dif² por masmorra; 4 por ano de dificuldade 2 = 800), pouco perto da cunhagem (cerca de 54 mil por ano no mundo) |
| O Aventureiro sobrevive? | **Apertado, mas sim:** a masmorra de dificuldade 1 em 3 etapas de 1 bandido (poder 15,5 contra 552) é quase certa (97% cada) e paga 100; o risco está no bando inteiro (10.2) |
| A Ambição "maior fortuna" é alcançável? | Metade do maior PIB = 34.800 ÷ 2 = **17.400**: um Mercador que lucra cerca de 150 por estação leva décadas; com caravanas próprias e loja ilegal, anos. **Para o autor conferir (OB1-1)** |
| O teste de 1 ano muda? | **Não** |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| O jogador pega um contrato e o problema some (a ameaça cai) | O contrato some do quadro; o aceito fica até cumprir ou vencer o prazo, e paga se cumprir |
| O Banco da cidade não tem a recompensa | O contrato não nasce |
| Duas Tavernas na mesma região com o mesmo bando | O bando é um só: o primeiro contrato aceito marca o bando; o outro some |
| Renome 100 | Fica em 100 |
| Pedir Nobre na Arcana sem ser Mago | Não pode (o Rito pede Mago) |
| A masmorra lendária já limpa por outro? | Só o jogador limpa masmorras nesta versão |

### 10.6 Decidido em modo automático (rodada OB1, seguir sugestão)

1. **OB1-1:** as condições das Ambições da tabela da parte 2; a "maior fortuna" é a metade do maior PIB (**para o autor conferir**: é longa).
2. **OB1-2:** a masmorra sem Mapa Interno é resolvida em 3 etapas pelo cálculo rápido.
3. **OB1-3:** entre as etapas, o grupo pode acampar 8 horas (a Vida volta 16%); a comparação de forças aparece antes de aceitar a caçada.
4. **OB1-4:** os 4 contratos (caçar bandidos, escolta, entrega, limpar masmorra) nascem do estado e são pagos pelo Banco da cidade.
5. **OB1-5:** Renome por feito e por título como na parte 2; o Renome não cai.
6. **OB1-6:** o título é por facção, e o Rito de Admissão de cada cultura é o da tabela 4.2.
