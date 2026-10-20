---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase D3 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com ES1 vêm da **rodada ES1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Escravidão

## 1. O que faz

Põe a **classe Escravo** nas cidades das facções que permitem a escravidão: o escravo trabalha sem salário, o dono paga a comida e a Senzala. O **Mercado de Escravos** compra e vende. A **Obediência** decide se ele trabalha e se foge; a **rebelião** acontece quando a Felicidade e o Controle caem demais. A **libertação**, a alforria e a **abolição** tiram gente da classe. O jogador pode **ser capturado e vendido** e então busca a liberdade.

Documento do sistema: [[Sistema de Escravidão/Descrição|Escravidão]].

**Esta versão:** a classe Escravo, o preço e o Mercado, a Obediência, a fuga, a rebelião (pelo cálculo rápido), a libertação, a alforria, a abolição, a captura do jogador e os 3 caminhos dele. **Prisioneiros de guerra, infantaria de escravos, tráfico no mercado negro, venda de prisioneiros entre facções e missões** ficam na parte 9.

## 2. Dados

### `data/escravidao.json` (arquivo novo)

```json
{
  "proporcao_inicial_em_porcento": { "escravista": 30, "guerreira": 0, "autoritaria": 0 },
  "preco": { "base": 200, "por_ponto_da_melhor_habilidade": 4, "mago": 200, "velho": 50, "cultura_hostil": 80 },
  "mercado_alvo": { "por_vaga_bracal_vazia": 2, "minimo": 2 },
  "venda_pelo_dono_em_porcento": 70,
  "comprar_a_liberdade_vezes_o_preco": 150,
  "felicidade_base_do_escravo": 30,
  "obediencia": { "base": 50, "medo_maximo": 30, "grilhoes": 15, "por_estacao_de_cativeiro": 1, "cativeiro_maximo": 10,
                  "felicidade_abaixo_de": 30, "cultura_guerreira": -10 },
  "fuga": { "obediencia_abaixo_de": 30, "detectado_em_porcento": 50 },
  "rebeliao": { "felicidade_abaixo_de": 10, "controle_abaixo_de": 10, "dias_seguidos": 4, "perdedores_executados_ou_vendidos_em_porcento": 20,
                "guardas_por_nivel_da_cidade": 2 },
  "alforria_por_estacao_em_porcento": { "tolerancia": 1, "escravista": 0 },
  "abolicao": { "ricos_felicidade": -15, "dias": 64 },
  "tratamentos": {
    "comida_refinada": { "felicidade": 5 },
    "grilhoes": { "obediencia": 15, "felicidade": -5, "velocidade_em_porcento": 80 },
    "castigo_publico": { "medo": 5, "dias": 8, "felicidade": -10, "reputacao_religiosa": -2 }
  },
  "captura_do_jogador_pelos_bandidos_em_porcento": 50,
  "peculio_por_hora_livre_em_decimos": 10,
  "peculio_maximo_por_dia_em_decimos": 40
}
```

### A classe Escravo (no bloco `"classes"` da [[Ficha Logica/ClasseSocial|Classe Social]])

- **Cesta:** 1 comida básica por dia, **paga pelo dono** (tirada do estoque do dono, sem dinheiro); sem Materiais nem Luxo.
- **Sem salário, sem imposto, sem gasto extra, sem dinheiro.**
- **Moradia:** a Senzala (como a Casa, só para Escravos).
- **Felicidade:** alvo = **30** + consumo + moradia + tratamento + eventos (sem o fator desemprego; Religião e Leis × cultura pela cultura dele).
- **Natalidade e mortalidade:** as da População; os filhos nascem Escravos.

### Cada NPC escravo

| Campo | Exemplo |
|---|---|
| `donoDoEscravo` | a Cidade, um Rico ou o jogador |
| `inicioDoCativeiro` | dia 120 |
| `grilhoes` | sim / não |
| `prazoDoTrabalhoForcado` | dia 200, ou nenhum |
| `obediencia` (recalculada todo dia) | 55 |

**Por cidade:** `diasSeguidosParaRebeliao`; o estoque do Mercado de Escravos (os NPCs à venda).

**O sorteio:** a fuga, a alforria e a rebelião usam a sequência de NPCs da cidade.

## 3. Quando roda

| Quando | O quê |
|---|---|
| **A cada dia, passo 1** (consumo) | A comida dos escravos sai do estoque do dono |
| **A cada dia, passo 5** (felicidade) | A Felicidade dos Escravos; a Obediência de cada um (4.3); a contagem da rebelião (4.5) |
| **Toda noite, às 0 h**, no passo 6 (eventos) | A fuga (4.4) |
| **Dia 1 de cada estação** | Alforria (4.6); compra da cidade e dos Ricos (4.2) |
| Na derrota do grupo para bandidos | A captura do jogador (4.8) |

## 4. Regras passo a passo

### 4.1 O começo

Nas cidades da **Escravista**, 30% dos NPCs começam Escravos (os de habilidade Braçal mais alta, nos empregos Braçais), da Cidade, com Senzala. A Guerreira (só prisioneiros) e a Autoritária (trabalho forçado como pena) começam sem escravos: eles chegam pela guerra e pelo Crime.

### 4.2 O Mercado de Escravos

1. **Preço base** = 200 + 4 × a melhor habilidade de Trabalho; × 2 se Mago; × 0,5 se velho; × 0,8 se a cultura dele é hostil à do comprador.
2. **Oferta e procura:** o mercado tem um estoque-alvo = 2 × as vagas Braçais vazias da cidade (mínimo 2). O fator é o do Mercado Local (de 50 a 200), sobre o preço base de cada escravo.
3. **Quem vende:** o dono, por 70% do preço; os cativos dos bandidos (4.8) e os fugitivos recapturados entram de graça no estoque (os bandidos são abstratos; ES1-3).
4. **Quem compra:**
   - o **jogador** (vira Escravo do jogador, Semi-Controlado, trabalha nas construções dele);
   - a **Cidade**, no dia 1 da estação, para vagas Braçais vazias, se o Banco tem a reserva de 250 mais o preço;
   - o **Rico**, para vagas Braçais vazias da empresa dele, se o dinheiro dele paga.
5. O dinheiro da venda vai para o dono do Mercado (a Cidade, ou o Rico dono).

### 4.3 A Obediência (passo 5)

Obediência = 50 + Medo + 15 (Grilhões) + 1 por estação de cativeiro (até 10) − (30 − Felicidade, se a Felicidade está abaixo de 30) − 10 se a cultura dele é Guerreira, entre 0 e 100.

- **Medo da cidade** = Controle × 30 ÷ 100, com o Controle do Crime (100 − Criminalidade, CJ1-4), + 5 durante um castigo público.
- **No trabalho:** a cada hora do turno, o escravo trabalha com chance igual à Obediência (LOD 0); em LOD 2, o trabalho presumido × Obediência ÷ 100 (ES1-1).

### 4.4 A fuga (toda noite)

1. Escravo com Obediência abaixo de 30, sem Grilhões: chance de tentar = (30 − Obediência)%.
2. Tentou: **50%** de ser visto (até a Percepção existir; ES1-2). Visto: volta, castigo (Felicidade −10 por 8 dias). Não visto: foge.
3. **Fugitivo:** sai da cidade; crime "Fuga" (gravidade 3) e Recompensa na facção (como a do jogador, Crime). Metade vai para uma Vila Livre (vira Civil lá); metade **soma 1 à Criminalidade** da região (vira bandido; a ameaça bandida do Crime).
4. A mesma regra em LOD 0 e LOD 2 (os escravos são registros em todo LOD; OC1-2). A regra "por estação" do documento fica no lugar da diária.

### 4.5 A rebelião

1. Felicidade dos Escravos **abaixo de 10** e Controle **abaixo de 10**, por **4 dias seguidos**.
2. **Combate:** os escravos (kit civil) contra os guardas (2 por nível da cidade, kit guarda) e os Civis adultos que defendem (kit civil). Em LOD 0, combate local até 40; acima disso, e em LOD 2, o **cálculo rápido** do [[Ficha Logica/Combate|Combate]].
3. **Vencem:** todos fogem (4.4, passo 3); a Felicidade dos Civis −5 por 1 estação.
4. **Perdem:** 20% são executados ou vendidos; o resto volta com Felicidade −10 por 1 estação.

### 4.6 A libertação, a alforria e a abolição

- **O dono liberta:** vira Civil (Plebeu); +20 de Opinião sobre o jogador; reputação +2 com a Religiosa e −5 com as facções que permitem a escravidão.
- **Alforria** (dia 1 da estação): com a lei de Tolerância, 1% dos escravos da cidade (fração acumulada, como a assimilação); na Escravista, 0%.
- **Abolição** (a assembleia muda a lei para "proibida"): todos viram Civis no dia 1 da estação seguinte; as Senzalas viram Casas; os Ricos −15 de Felicidade por 64 dias; na Escravista, o extremo é violado (Cultura).
- **Reputação do jogador:** vender −3 e comprar −1 com a Religiosa.

### 4.7 O escravo do jogador

- Trabalha nas construções do jogador pela Lista de Prioridades (Interface). O jogador escolhe o tratamento (comida refinada, Grilhões, castigo).
- Não conta nos vinculados (Companheiros).

### 4.8 O jogador capturado (ES1-4)

1. Quando o grupo do jogador perde para **bandidos** (Combate, 4.7): 50% de chance de **roubo** (75%, como antes) e 50% de **captura**.
2. **Captura:** o grupo é levado ao Mercado de Escravos mais perto que fica numa facção que permite a escravidão ((6,6), (5,6), ou outro), e cada um entra no estoque. O jogador é **comprado** pelo primeiro comprador (a Cidade, na próxima estação, se nenhum Rico comprar antes).
3. **Os 3 caminhos:**
   - **Comprar a liberdade:** 1,5 × o preço dele. O jogador junta o **pecúlio**: 1 moeda por hora livre que ele trabalha a mais, até 4 por dia;
   - **Fugir:** a regra 4.4 (o jogador decide tentar; ele não precisa de Obediência baixa); vira fugitivo com Recompensa;
   - **Servir:** o dono (se for um Rico ou um Importante) liberta depois de 4 estações com Opinião +40.
4. Os companheiros capturados podem ser comprados de volta pelo jogador livre.

## 5. Contas

| Conta | Valor |
|---|---|
| Escravo com Braçal 80 | 200 + 320 = **520**; velho: **260** (bate com o documento) |
| O documento: Medo 10, Felicidade 25, sem Grilhões | 50 + 10 − 5 = **55**; com Grilhões: **70** (bate) |
| Uma cidade tranquila (Criminalidade 11) | Controle 89 → Medo **26,7**; escravo novo com Felicidade 30: Obediência **76,7** |
| A mesma cidade em fome (Criminalidade 56, Felicidade 10) | Controle 44 → Medo 13,2; − 20: Obediência **43** (sem fuga) |
| Comprar a liberdade com pecúlio de 4 por dia, preço 400 | 600 ÷ 4 = **150 dias** (cerca de 2,3 anos) |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Classe Social | A classe Escravo (cesta paga pelo dono, sem salário) |
| Trabalho | O escravo ocupa vaga sem salário; a cidade e o Rico compram para vagas Braçais vazias |
| Produção | O trabalho × Obediência (ES1-1) |
| População | Os filhos de escravos nascem Escravos; a Senzala |
| Felicidade | A Felicidade dos Escravos (base 30) |
| Crime | O Controle no Medo; a Fuga e a Recompensa; o fugitivo vira bandido |
| Combate | A rebelião; a captura na derrota para bandidos (4.8; muda a 4.7 do Combate) |
| Facção | A lei de Escravidão; a abolição pela assembleia |
| Reputação | Comprar, vender, libertar |
| Companheiros | A Opinião do libertado; os escravos fora dos limites |
| Save | A classe Escravo, os dados de cada escravo, o Mercado; o formato passa para a **versão 16** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `precoDoEscravo(npc, comprador)` | Regra 4.2 |
| `calcularObediencia(npc)` | Regra 4.3 |
| `tentarFugaDosEscravos(cidade)` | Regra 4.4 |
| `conferirRebeliao(cidade)` | Regra 4.5 |
| `libertarEscravo(npc)`, `aplicarAlforria(cidade)`, `abolirEscravidao(faccao)` | Regra 4.6 |
| `capturarGrupoDoJogador()` | Regra 4.8 |

## 8. Testes

1. **Os números da parte 5.**
2. **O teste de 1 ano não muda:** a vila não tem facção nem escravos.
3. **Obediência e trabalho:** um escravo com Obediência 60 em LOD 2 produz 60% do trabalho presumido.
4. **Fuga:** Obediência 20 → 10% por noite de tentar; metade das tentativas é vista.
5. **Abolição:** 30 escravos viram 30 Civis no dia 1 da estação seguinte; as Senzalas, Casas.
6. **Captura:** em 1.000 derrotas para bandidos, cerca de 500 capturas.
7. **O dinheiro do mundo:** comprar e vender escravos só passa dinheiro entre donos (os cativos entram de graça; ES1-3).

## 9. Fora desta versão

- Prisioneiros de guerra (100 = 1 escravo), a venda de prisioneiros entre facções, a infantaria de escravos (Guerra e Batalha).
- O tráfico no mercado negro onde é proibido (Contrabando, × 1,5).
- A dívida como fonte; a grande revolta.
- As missões da Religiosa e da Escravista; os caçadores de escravos.
- O −10 de relação da Religiosa com quem tem Mercado de Escravos (Diplomacia, D5).

## 10. Simulação de mesa

### 10.1 Cenário

- **A Carpintaria do jogador** (FE3): 2 empregados Civis; lucro do jogador cerca de **3,4 por dia**.
- O jogador tem uma vila em território **Escravista** (a lei permite), e compra **2 escravos** Braçais 50 (preço 400 cada, fator 100).

### 10.2 O lucro com escravos

| Linha por dia | Com 2 Civis | Com 2 escravos |
|---|---|---|
| Salários (cerca de 8 por empregado) | −16 | 0 |
| Comida dos escravos (2 Pães a 1,87) | 0 | −3,7 |
| Produção (Obediência 77: 77% do trabalho) | 100% | 77% |
| Lucro do jogador | **+3,4** | cerca de **+11** (com a produção menor) |

O investimento de 800 se paga com os cerca de 7,8 a mais por dia em cerca de **100 dias** (1,6 ano). Os escravos vivem cerca de 16 anos de trabalho. **Para o dono particular, a escravidão rende muito**: é a base econômica da Escravista, como o documento pede.

**Para a cidade, nada muda no dinheiro:** com Civis, o Banco paga os salários e recebe de volta o imposto e o gasto (FA1-2); com escravos, não paga nem recebe. Só a comida sai do estoque nos dois casos.

### 10.3 O jogador capturado

| Momento | O que acontece |
|---|---|
| Perde para 6 bandidos (Combate, 10.4) | Sorteio 312 < 500 → **captura** |
| Levado ao porto (6,6) | Preço: 200 + 4 × 50 = 400 |
| Estação seguinte | A Cidade do porto compra (tem vaga Braçal vazia) |
| Como escravo | Obediência não importa (ele é o jogador); trabalha o turno e, nas horas livres, junta pecúlio: 4 por dia |
| 150 dias depois | Junta 600 = 1,5 × 400: **compra a liberdade**. Ou tenta fugir antes: 50% de ser visto |

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| Os exemplos do documento batem? | **Sim:** 520 e 260; Obediência 55 e 70 |
| Os escravos de uma cidade tranquila fogem? | **Não:** Obediência perto de 77 |
| A rebelião acontece fácil? | **Não:** pede Felicidade abaixo de 10 **e** Criminalidade acima de 90 por 4 dias: só numa cidade em colapso |
| O jogador fica preso como escravo para sempre? | **Não:** 3 caminhos; comprar leva cerca de 2,3 anos, fugir é na hora (com risco) |
| O dinheiro do mundo se conserva? | **Sim:** os cativos entram de graça; compra e venda só passam dinheiro entre donos (ES1-3) |
| O teste de 1 ano muda? | **Não** |
| **Achado:** o documento tinha duas regras de fuga (por noite em LOD 0, por estação em LOD 2) | **Resolvido (ES1-1, OC1-1):** uma só, por noite, para todos (os escravos existem como registros em todo LOD) |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| O dono não tem comida para o escravo | Ele fica sem comida (consumo −25 na Felicidade dele): a Obediência cai |
| A facção abole a escravidão com o jogador escravo | Ele vira Civil livre no dia 1 da estação seguinte |
| Ninguém compra o jogador | Ele fica no estoque do Mercado (trabalha para o dono do Mercado) até alguém comprar |
| O escravo com prazo (trabalho forçado) chega ao fim | Volta à classe anterior |
| Mercado sem estoque | Não vende |
| Escravo Mago, velho, de cultura hostil | 200 + 4 × hab × 2 × 0,5 × 0,8 |

### 10.6 O que muda em outras fichas

- **Classe Social:** a classe Escravo.
- **Combate (4.7):** a derrota para bandidos pode virar captura (ES1-4).
- **Produção:** o trabalho do escravo × Obediência.

### 10.7 Decidido em modo automático (rodada ES1, seguir sugestão)

1. **ES1-1:** a fuga é sorteada toda noite, em todo LOD; o trabalho do escravo × Obediência.
2. **ES1-2:** 50% de ser visto na fuga até a Percepção existir.
3. **ES1-3:** os cativos dos bandidos e os fugitivos recapturados entram de graça no estoque do Mercado.
4. **ES1-4:** a derrota do grupo para bandidos é 50% roubo e 50% captura e venda.
5. **ES1-5:** a Felicidade base do escravo é 30 (não 50).
6. **ES1-6:** a Escravista começa com 30% de escravos; a Guerreira e a Autoritária, com nenhum.
7. **ES1-7:** o escravo jogador junta pecúlio: 1 por hora livre trabalhada, até 4 por dia.
