---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase D7 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]], a última da fase D), com a **parte 10: Simulação de mesa**. Os pontos marcados com GU1 vêm da **rodada GU1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Guerra

## 1. O que faz

A guerra no **Mapa Mundial**: a declaração vem da [[Ficha Logica/Diplomacia|Diplomacia]]; a facção forma **exércitos** (Esquadrões no Quartel, um General, Caravanas Militares), paga o **soldo**, abastece, marcha (pela [[Ficha Logica/Viagem|Viagem]]), luta (pela [[Ficha Logica/Batalha|Batalha]]: cálculo rápido, ou o Mapa de Guerra com o jogador), **cerca**, **ocupa** e **saqueia** cidades. O **Placar** mede quem ganha; o **Cansaço** empurra para a paz; o **tratado** troca o Placar por termos e põe uma **Trégua** de 1 ano.

Documento do sistema: [[Sistema de Guerra/Descrição|Guerra e Campanha]].

**Esta versão:** a guerra entre as facções da IA de ponta a ponta, o dinheiro da guerra (o soldo e a parte da facção no imposto), o suprimento, o cerco, a ocupação, o saque, o Placar, o Cansaço, a paz, a dissolução e a entrada do jogador numa batalha que passa perto. **Mercenários, o jogador como general ou líder, o pedido de cidade conquistada e a guerra civil** ficam na parte 9.

## 2. Dados

### `data/guerra.json` (arquivo novo)

```json
{
  "soldo_por_esquadrao_por_dia_em_decimos": 80,
  "parte_da_faccao_no_imposto_em_porcento": 70,
  "reserva_alvo_do_banco_em_porcento_do_pib": 25,
  "guarnicao_em_porcento": 25,
  "esquadroes_por_exercito": 20,
  "quartel_dias": 2,
  "racao_por_esquadrao_por_dia": 1,
  "caravana_militar_carrega": 60,
  "sem_caravana_dias_de_racao": 3,
  "forrageio_em_porcento": 50,
  "inverno_racao_em_porcento": 125,
  "sem_suprimento": { "moral_por_dia": -5, "desercao_abaixo_de_moral": 30, "dias_por_desercao": 2 },
  "placar": { "batalha_minimo": 2, "batalha_maximo": 15, "vilarejo": 10, "cidade": 20, "capital": 35,
              "mapa_dominado": 10, "objetivo_a_cada_4_dias": 1, "saque_cidade": 5, "saque_mapa": 2, "caravana_capturada": 3 },
  "dominio_dias_sem_oposicao": 4,
  "cansaco": { "a_cada_4_dias": 1, "a_cada_2_esquadroes_perdidos": 1, "cidade_ocupada_por_estacao": 5,
               "comercio_bloqueado_por_estacao": 5, "inverno_por_estacao": 5, "sem_motivo": 20,
               "felicidade_por_ponto": -0.15, "cai_em_paz_por_estacao": 10, "guerreira_em_porcento": 50 },
  "paz": { "custos": { "paz_branca": 0, "prisioneiros": 5, "mapa_em_disputa": 10, "romper_alianca": 10,
                       "vilarejo": 15, "cidade": 30, "capital": 50, "tributo": 15, "indenizacao_maxima": 30,
                       "independencia": 30, "vassalagem": 60 },
           "aceita_pior_com_cansaco": 60, "paz_branca_com_cansaco": 100, "tregua_dias": 64 },
  "cerco": { "moral_da_guarnicao_por_dia_de_fome": -10, "dias_de_fome_para_render": 8, "ariete_dias": 2 },
  "muralha_no_poder": { "palicada": 1.3, "pedra": 1.6, "bastiao": 2.0 },
  "saque": { "dinheiro": 50, "estoque": 30, "populacao": 10, "felicidade": -30, "dias_da_felicidade": 64,
             "moral": 10, "reputacao": -10, "religiosa_relacao": -5 },
  "ocupacao": { "felicidade": -20, "posse_de_fato_em_dias": 128 },
  "conquista": { "felicidade": -15, "dias": 64 }
}
```

### A guerra (no save)

| Campo | Exemplo |
|---|---|
| `ladosDaGuerra` | Guerreira (e aliados) × Autoritária (e aliados) |
| `motivo`, `objetivo` | mapa em disputa (1,2); dominar o mapa |
| `placar` | +18 (visto pelo atacante) |
| `inicio`, `cidadesOcupadas`, `prisioneiros` | dia 65; nenhuma; 653 |
| `dominadorDoMapaEmDisputa` | Guerreira desde o dia 70 |

**O exército** é uma `EntidadeEmTransito` da Viagem (tipo `"exercito"`) com o General, a lista de Esquadrões, as Caravanas Militares (carga de Ração), a ordem e a Moral média. **Por facção:** `cansacoDeGuerra` (0 a 100).

**O sorteio:** a batalha usa a sequência 5 (Batalha); a IA da guerra não sorteia.

## 3. Quando roda

| Quando | O quê |
|---|---|
| Na declaração (Diplomacia) | A guerra nasce; os exércitos se formam (4.2) |
| **A cada 4 dias**, passo 6b | A IA da guerra (4.4), o Placar do objetivo, o Cansaço, a paz (4.9) |
| **A cada dia**, passo 2 | O soldo (4.1); passo 1: a Ração e o forrageio (4.3); o cerco (4.6) |
| A cada tick | Os exércitos andam (Viagem) |
| Dois exércitos hostis no mesmo mapa | A batalha (4.5) |
| **Dia 1 de cada estação** | A volta do dinheiro acima da reserva (4.1); o Cansaço cai em paz |

## 4. Regras passo a passo

### 4.1 O dinheiro da guerra (GU1-1)

1. **O soldo:** 8 moedas por Esquadrão por dia, pago pelo **Banco da Facção** no passo 2, para a **classe Civil da cidade de origem** de cada Esquadrão (as famílias; o dinheiro fica no mundo, como o salário da caravana).
2. **A parte da facção no imposto liga agora:** 70% do imposto das cidades vai para o Banco da Facção (o documento; a FA1-1 esperava a Guerra).
3. **A volta acima da reserva:** no dia 1 de cada estação, o que o Banco da Facção tem acima de **25% do PIB** volta às cidades, junto com a verba, na proporção dos NPCs (GU1-2). Assim, o imposto alto com exército pequeno não seca as cidades (parte 10.4).
4. Sem dinheiro para o soldo: o Esquadrão fica sem pagar; com 16 dias sem soldo, ele se desfaz (os soldados voltam como NPCs, 4.10).

### 4.2 Os exércitos

1. **Na declaração**, cada facção do lado em guerra junta os Esquadrões dela: **25% ficam de guarnição** na capital e nas cidades da fronteira; o resto forma exércitos de até 20, cada um com um **General** (um nobre ou o herdeiro da dinastia; Companheiros) e as Caravanas Militares que o Banco paga (1 a cada 5 Esquadrões).
2. **Formar Esquadrões (Quartel):** 1 conscrito (um NPC adulto Civil ou, com a lei, Escravo; nunca guarda nem Importante) + o kit (1 arma e 3 peças de armadura do tipo, tiradas do estoque da cidade) + 2 dias. **Sem o kit:** sai como Milícia (Ataque 5, Defesa 3) (GU1-3). O Quartel forma tantos ao mesmo tempo quanto o nível dele.
3. **A IA repõe** os Esquadrões perdidos até o número do primeiro dia, enquanto a conscrição da lei deixa (população × % ÷ 100 ÷ Fator, menos os alistados).
4. **Comando do General:** a média de Carisma e Corpo a Corpo (até a Inteligência): +10 de Moral inicial com Comando 100.

### 4.3 O suprimento (passo 1)

1. Cada Esquadrão come **1 Ração Militar por dia** (sem Ração no conteúdo: 1 comida de Tier 1; GU1-4), da Caravana; × 125% no inverno fora de cidade amiga.
2. **Em território próprio ou aliado:** a Caravana enche ao passar por cidade com Mercado (o Banco da Facção paga a loja).
3. **Em território inimigo:** o **forrageio** tira até 50% do consumo do estoque da cidade do mapa (ou da produção do mapa sem cidade, −50% por 1 estação).
4. **Sem suprimento:** Moral −5 por dia; abaixo de 30, 1 Esquadrão deserta a cada 2 dias (o de menor Moral; os soldados voltam para casa como NPCs). No poder do cálculo rápido: × 0,7.

### 4.4 A IA da guerra (a cada 4 dias)

Para cada exército, pela ordem:

1. **Objetivo do motivo:** mapa em disputa → marchar até ele e ficar; tomar cidade → marchar e cercar; tributo, honra, ofensa → atacar o exército inimigo mais perto ou cercar a cidade mais perto.
2. **Defesa:** se um exército inimigo está a 2 mapas ou menos de uma cidade própria e o exército próprio tem poder ≥ 80% do dele, vai interceptar; se não, recua para a cidade (guarnição).
3. **Recuar:** se a Moral média está abaixo de 40 ou o suprimento acaba em 2 dias, volta para a cidade amiga mais perto.
4. **Depois do objetivo:** se o Placar ainda não paga a paz que a facção quer, o exército escolhe a cidade inimiga mais perto (a de menor nível primeiro).

### 4.5 A batalha

1. Dois exércitos hostis no mesmo mapa: batalha. Sem o jogador: o **cálculo rápido de exércitos** da [[Ficha Logica/Batalha|Batalha]] (4.10). O cálculo em rodadas que o documento da Guerra descrevia é **trocado** por ele (GU1-5): uma fórmula só.
2. **O jogador passando perto** (no mapa ou num vizinho): pode entrar por um lado; abre o Mapa de Guerra (Batalha), com a Comitiva.
3. **Placar da batalha** = 2 + 13 × Esquadrões destruídos ÷ Esquadrões do exército inimigo na guerra, arredondado para baixo.
4. **Domínio do mapa em disputa:** vencer a última batalha ali e ficar 4 dias sem oposição: +10 e `dominadorDoMapaEmDisputa`.

### 4.6 O cerco (a cada dia)

1. Um exército hostil parado no mapa de uma cidade: o comércio, a migração e as caravanas param.
2. A cidade come do estoque sem repor de fora (a produção dela continua, menos o forrageio).
3. **Fome:** quando a comida acaba, a Moral da guarnição −10 por dia; a fome da População vale.
4. **Rendição:** Moral da guarnição 0, ou 8 dias de fome.
5. **Assalto:** sem Muralha, desde o 1º dia; com Muralha, depois do Aríete (2 dias) ou de uma brecha. O defensor soma a guarnição, a Milícia (2 por nível da cidade; Escravidão) e × a Muralha no poder (1,3, 1,6, 2,0). A IA ataca se o poder dela é 1,5× o da defesa; se não, espera a rendição.
6. Um exército amigo que chega e vence os sitiantes levanta o cerco.

### 4.7 A ocupação e a conquista

- **Tomada:** a cidade fica **ocupada**: o imposto vai para o ocupante; as leis e o dono legal continuam; Felicidade −20; Placar +10, +20 ou +35 enquanto ocupada.
- **Muda de dono** só no tratado (ou na posse de fato, 128 dias ocupada sem paz): adota as leis da nova facção; a cultura dos NPCs fica (a Cultura e a Religião cuidam); Felicidade −15 por 64 dias; os nobres antigos viram exilados. O território em volta segue (Facção, 4.1).

### 4.8 O saque

A IA escolhe saquear quando a cultura não se ofende (Guerreira, Escravista) e o Banco dela está abaixo da reserva-alvo; senão, ocupa.

- **Saquear:** leva 50% do Banco e 30% de cada estoque da cidade; mata 10% da população (pelo acumulado da População); Felicidade −30 por 64 dias; a cidade pode cair 1 nível (pelas regras da Cidade); Moral do exército +10; Reputação −10 com o dono; Placar +5; as facções Religiosas −5 de relação com o saqueador.
- **Mapa sem cidade:** produção −50% por 1 estação; Placar +2.

### 4.9 O Cansaço e a paz (a cada 4 dias)

1. **Cansaço sobe:** +1 a cada 4 dias de guerra; +1 a cada 2 Esquadrões perdidos; +5 por estação por cidade própria ocupada, por comércio bloqueado e por campanha no inverno; 20 de início numa guerra sem motivo. A Guerreira sobe pela metade. **Cai** 10 por estação em paz.
2. **Na Felicidade** (o fator "Guerra e ameaça", nos eventos): −0,15 por ponto.
3. **A proposta:** o lado com o Placar a favor exige os termos mais caros que cabem no Placar, pela ordem: o objetivo da guerra primeiro (o mapa, a cidade), depois tributo, indenização, prisioneiros.
4. **O aceite:** o outro aceita se o Placar contra ele cobre o custo; com Cansaço 60 ou mais, aceita até o dobro do Placar; com 100, aceita a paz branca de quem propuser.
5. **Com a paz:** os termos valem; os prisioneiros que não foram trocados ficam (Escravidão); **Trégua de 64 dias** (Diplomacia); o mapa em disputa cedido vira território do vencedor (a disputa acaba).

### 4.10 A dissolução

Na paz, a facção dissolve os Esquadrões acima do número do primeiro dia: cada um volta como **1 NPC** na cidade de origem (Civil, ou Escravo se era escravo), criado pela População. Os outros ficam como guarnição.

## 5. Contas

**O soldo e o imposto num ano** (64 dias), salários de cerca de 4,7 por NPC por dia:

| Facção | Imposto | Parte da facção (70%) | Esquadrões | Soldo | Diferença |
|---|---|---|---|---|---|
| Guerreira (440 NPCs) | 10% | 440 × 0,47 × 0,7 × 64 = **9.265** | 20 | 20 × 8 × 64 = **10.240** | −975 (paga do Banco: 7 anos com 5.844 e a cunhagem) |
| Escravista (480 NPCs) | 30% | 480 × 1,41 × 0,7 × 64 = **30.320** | 14 | **7.168** | **+23.150** (volta às cidades, GU1-2) |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Diplomacia | A declaração, os aliados, a Trégua, o Cansaço na vontade de guerra |
| Batalha | O cálculo rápido de exércitos; o Mapa de Guerra; o cerco no campo |
| Viagem | O exército como entidade em trânsito; o tempo pelo tamanho (× 1,5 a 2,0) |
| Facção | O soldo pelo Banco; os 70% do imposto ligados (GU1-1); a volta acima da reserva (GU1-2); a conscrição |
| Economia | O soldo para a classe Civil; a Caravana compra na loja |
| População | A conscrição tira NPCs; os mortos e o saque pelo acumulado; a dissolução devolve NPCs; a fome do cerco |
| Escravidão | Os prisioneiros escravizados; a Milícia |
| Cidade | A cidade saqueada pode cair de nível |
| Comércio | A guerra bloqueia o comércio entre os dois e a cidade sitiada |
| Felicidade | Ocupação −20, conquista −15, saque −30, Cansaço × −0,15 |
| Notícias | Declarações, batalhas, cercos, saques e a paz (Regional e Mundial) |
| Save | As guerras, os exércitos, o Cansaço; o formato passa para a **versão 20** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Guerra`, `Exercito` | Os registros da parte 2 |
| `pagarSoldo()`, `devolverAcimaDaReserva()` | Regra 4.1 |
| `formarExercitos(faccao, guerra)`, `formarEsquadraoNoQuartel(cidade, tipo)` | Regra 4.2 |
| `abastecerExercito(exercito)` | Regra 4.3 |
| `decidirOrdensDaGuerra(faccao)` | Regra 4.4 |
| `avancarCerco(cidade)` | Regra 4.6 |
| `ocupar(cidade)`, `saquear(cidade)` | Regras 4.7 e 4.8 |
| `proporEAceitarPaz(guerra)` | Regra 4.9 |

## 8. Testes

1. **A guerra da parte 10.2:** acaba em cerca de 1 semana com o mapa (1,2) cedido.
2. **O dinheiro das cidades da Escravista** num ano de guerra: o Banco de cada cidade fica na regra FE4-3 (nunca abaixo de 250, no máximo 50 abaixo do começo) com a GU1-2.
3. **Cerco:** um vilarejo com 5 dias de comida se rende no dia 12 a 13 (5 de estoque + 7 de Moral caindo).
4. **20 anos de mundo sem jogador**, 10 sementes: cada facção passa **menos de 40% do tempo em guerra**, e nenhuma some.
5. **Dissolução:** 6 Esquadrões a mais na paz viram 6 NPCs nas cidades de origem.
6. **O teste de 1 ano não muda:** a vila não tem facção.

## 9. Fora desta versão

- As companhias de **mercenários** (com o conteúdo das companhias e os contratos).
- O jogador como **mercenário com contrato**, membro com exército, vassalo com chamado às armas, líder que delega.
- O pedido de cidade conquistada pelo jogador (Renome, fase E).
- A **guerra civil** na crise de sucessão.
- A Passagem Militar e a vassalagem como termo de paz (a vassalagem tem custo 60 no Placar, mas o efeito é da Diplomacia na fase E).
- As patrulhas da facção (1 Esquadrão leve a cada 4 mapas).

## 10. Simulação de mesa

### 10.1 Cenário

O dia 65 da Diplomacia (10.3): a **Guerreira** (20 Esquadrões, 1 de Cavalaria) declara guerra à **Autoritária** (16) pelo mapa em disputa **(1,2)**, entre a capital Guerreira (1,1) e o vilarejo Autoritário (1,3). Os dois em Primavera; sem Muralha no mapa.

### 10.2 A guerra, de 4 em 4 dias

| Dia | O que acontece | Placar (Guerreira) | Cansaço Gue / Aut |
|---|---|---|---|
| 65 | Declaração. Guarnição 25%: a Guerreira sai com **15**, a Autoritária com **12**. Os dois marcham para (1,2): exército de 6 a 20 Esquadrões × 1,75; 1 mapa = cerca de 7 h | 0 | 0 / 0 |
| 65 | Batalha em (1,2) (cálculo rápido): poder 9 × 15.000 = **135.000** contra 7,2 × 12.000 = **86.400**: chance da Guerreira **61%**. Sorteio: vence | | |
| 65 | Autoritária perde 60% + 20% do resto (Cavalaria) = **816** soldados (8 Esquadrões); a Guerreira perde 60% × 0,64 = **38%** = 576 (5,8) | 2 + 13 × 8 ÷ 16 = **+8** | |
| 69 | A Autoritária recua (Moral abaixo de 40). Cansaço: +1 (4 dias), +4 (8 Esquadrões) para ela; a Guerreira +0,5 e +1,5 (metade) | +8 | 2 / 5 |
| 69 | **Domínio** de (1,2) depois de 4 dias sem oposição: **+10** | **+18** | |
| 69 | **Paz:** a Guerreira exige "Ceder (1,2)" (10) + "Prisioneiros" (5) = 15 ≤ 18. A Autoritária: o Placar contra (−18) cobre 15: **aceita** | | |
| 70 | Paz. (1,2) vira **território Guerreiro**; Trégua de 64 dias; 490 prisioneiros (feridos e capturados) libertados pelo termo | | |

A guerra durou **5 dias**. O motivo (o mapa em disputa) **acabou**: a vontade de guerra da Guerreira contra a Autoritária cai de 86 para cerca de 46 (−20 sem motivo, em vez de +20).

### 10.3 O mundo fica em guerra para sempre? (o alerta da Diplomacia)

| Guerra do começo do ano 2 | O que acontece | Depois |
|---|---|---|
| Guerreira × Autoritária | A de 10.2: 5 dias, o mapa cedido | Sem motivo: paz longa |
| Religiosa × Arcana (desde o dia 1) | Igual, por um mapa em disputa entre elas: quem vence leva o mapa | Sem motivo: paz longa |
| Escravista × Religiosa | Se a Religiosa vence a batalha do mapa, a Escravista paga e a disputa acaba; se o defensor ganha, o atacante aceita a indenização e a disputa **continua** | Nova guerra depois da Trégua (64 dias) |

**Resultado:** as disputas se resolvem uma a uma com o mapa cedido. Só a que o atacante perde volta depois da Trégua. O mundo **não fica em guerra o tempo todo**: as guerras são curtas (dias a semanas) e ficam mais raras com o tempo. O teste 4 mede.

**Atenção (achado):** as guerras curtas vêm de a paz ser aceita assim que o Placar cobre o objetivo. Por isso a guerra de conquista de cidade (cerco, 12 dias ou mais) é o caso longo.

### 10.4 O dinheiro da Escravista em guerra

Sem a GU1-2: a Escravista recebe 30.320 por ano da parte do imposto e gasta 7.168 de soldo. As cidades perdem **+23.150 por ano** para o Banco da Facção (cerca de 48 por NPC por ano): uma vila de 15 NPCs perderia 720 por ano e quebraria. **Com a GU1-2:** o Banco da Facção fica na reserva (25% do PIB, cerca de 8.700) e devolve o resto às cidades a cada estação. As cidades ficam como estavam.

### 10.5 Um cerco

A Guerreira cerca o vilarejo Autoritário (1,3), 40 NPCs, 200 Pães (5 dias), Paliçada, guarnição de 2 Esquadrões + 4 de Milícia:

| Dia do cerco | O que acontece |
|---|---|
| 1 a 2 | Monta o Aríete. Poder da defesa: (2 × 0,6 + 4 × Milícia (5 − 4) × 30 × 10% ÷ 20 = 0,15 cada → 0,6) = 1,8 × 6.000 × 1,3 = 14.040. O exército de 9 Esquadrões: 5,4 × 9.000 = 48.600: **3,5×**: ataca |
| 3 | Assalto: chance de 48.600 ÷ 62.640 = **78%** |
| Se esperar | Dias 1 a 5 com comida; 6 a 12 com fome, guarnição −10 por dia (de 70 a 0 em 7 dias): **rendição no dia 12** |

### 10.6 Verificações

| Pergunta | Resultado |
|---|---|
| O mundo fica em guerra o tempo todo? | **Não** (10.3): as disputas acabam com a paz |
| O dinheiro fecha? | **Sim, com a GU1-1 e a GU1-2:** o soldo volta às cidades; o excesso do Banco da Facção também |
| As baixas pesam? | A Autoritária perde 326 mortos (0,7%) e recupera os prisioneiros no termo. Uma guerra por ano é pouco; o saque (10% da população) é o que pesa |
| Os Esquadrões perdidos voltam? | A IA repõe pela conscrição (1 NPC por Esquadrão) e pelo kit; sem kit, Milícia |
| O teste de 1 ano muda? | **Não** |

### 10.7 Decidido em modo automático (rodada GU1, seguir sugestão)

1. **GU1-1:** com a Guerra, liga a parte da facção no imposto (70%) e o soldo (8 por Esquadrão por dia) vai para a classe Civil da cidade de origem.
2. **GU1-2:** no dia 1 de cada estação, o Banco da Facção acima de 25% do PIB volta às cidades com a verba.
3. **GU1-3:** o Esquadrão sem kit no estoque sai como Milícia.
4. **GU1-4:** sem a Ração Militar no conteúdo, o exército come 1 comida de Tier 1 por Esquadrão por dia.
5. **GU1-5:** o cálculo em rodadas do documento da Guerra é trocado pelo cálculo rápido de exércitos da Batalha (uma fórmula só).
6. **GU1-6:** a IA guarda 25% dos Esquadrões de guarnição e repõe as baixas até o número do primeiro dia.
