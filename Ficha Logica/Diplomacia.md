---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase D5 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com DP1 vêm da **rodada DP1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Diplomacia

## 1. O que faz

Guarda a **relação** (−100 a +100) entre cada par de facções em contato, o **estado** (Paz, Guerra, Trégua), os **tratados** e a **Confiabilidade** de cada facção. A cada 4 dias, a IA de cada facção calcula a **vontade de guerra** contra as outras; a cada estação, propõe e aceita tratados e casamentos. O **freio** (Ameaça e Coalizão) segura quem cresce demais.

Documento do sistema: [[Sistema de Diplomacia/Descrição|Diplomacia]]; o primeiro dia, em [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]].

**Esta versão:** relação, estado, os tratados que já têm efeito (Acordo Comercial, Embargo, Não Agressão, Alianças, Casamento), a Confiabilidade, a vontade de guerra e a declaração, as propostas da IA, a fronteira fechada e a Coalizão. **O que acontece na guerra, a paz pelo Placar, a vassalagem, os reféns e a diplomacia do jogador** ficam na Guerra (D7) e na parte 9.

## 2. Dados

### `data/diplomacia.json` (arquivo novo)

```json
{
  "atitudes": [ ["hostil", -31], ["fria", -1], ["neutra", 29], ["amigavel", 69], ["leal", 100] ],
  "relacao_alvo": { "afim": 40, "neutra": 0, "hostil": -40, "mapa_em_disputa": -15,
                    "por_tratado_de_alianca_ou_casamento": 10, "mercado_de_escravos_para_a_religiosa": -10,
                    "passo_por_estacao": 2 },
  "tratados": {
    "acordo_comercial": { "relacao_minima": 10 },
    "embargo":          { "relacao_minima": null },
    "nao_agressao":     { "relacao_minima": 0, "dias": 64 },
    "alianca_defensiva": { "relacao_minima": 40 },
    "alianca_total":    { "relacao_minima": 60 },
    "casamento":        { "relacao_minima": 0, "relacao": 20 }
  },
  "assinar": 5, "cancelar_valido": -10,
  "confiabilidade": { "inicial": 70, "quebrar_tratado": -20, "guerra_sem_motivo": -20, "ignorar_chamado": -15,
                      "quebrar_tregua": -30, "estacao_sem_quebra": 5, "minima_para_tratado": 30 },
  "vontade_de_guerra": {
    "limiar": 60,
    "forca_por_ponto_da_razao": 50,
    "agressividade_maxima": 30, "ganancia_maxima": 20,
    "motivo_forte": 20, "sem_motivo": -20,
    "relacao_divide": 4, "cultura_guerreira": 15, "religiosa_contra_hostil": 15,
    "por_guerra_em_andamento": -20, "cansaco_divide": 2, "coalizao": 30, "inverno": -15
  },
  "ameaca": { "pib_vezes_a_media": 1.6, "cidades_conquistadas_em_2_anos": 3, "fim_abaixo_de": 40 },
  "nao_agressao_inicial_dos_pares_hostis_dias": 64
}
```

### Estado (vai no save)

| Campo | Por quê | Exemplo |
|---|---|---|
| `paresEmContato` | Só os pares com fronteira ou rota (o Mundo diz quais) | Guerreira × Autoritária |
| `relacaoDoPar` | −100 a +100 | −55 |
| `estadoDoPar`, `fimDaTregua` | Paz, Guerra ou Trégua | Paz |
| `tratadosDoPar` | Lista com início e fim | Não Agressão até o dia 65 |
| `confiabilidadePorFaccao` | 0 a 100 | 70 |
| `porQueDaUltimaDecisao` | Para as Notícias e a depuração | "Vontade de guerra 86: força 1,25..." |

A **força** de cada facção (para a IA) é a quantidade de **Esquadrões** dela (Mundo e Estado Inicial; com a Guerra, a força de verdade dos exércitos).

**O sorteio:** o desvio das relações iniciais e as guerras e acordos do primeiro dia usam a sequência 7 (a da Facção).

## 3. Quando roda

| Quando | O quê |
|---|---|
| Ao criar o mundo | Relações, guerras e acordos do primeiro dia (4.1) |
| **A cada 4 dias**, passo 6b (novo, depois dos eventos) | A vontade de guerra e a declaração (4.4) |
| **Dia 1 de cada estação**, passo 6b | A relação anda (4.2); propostas de tratados e casamentos (4.5); Confiabilidade (4.3); Coalizão (4.6) |
| Na hora de um acontecimento | As mudanças de relação dos eventos, tratados e quebras |

## 4. Regras passo a passo

### 4.1 O primeiro dia

1. Relação de cada par em contato = base da afinidade (+40, 0, −40) − 15 se dividem um mapa em disputa + desvio sorteado de −15 a +15.
2. **Guerras:** 1 (70%) ou 2 (30%) entre os 3 pares candidatos (Guerreira × Autoritária, Arcana × Religiosa, Religiosa × Escravista). O par em guerra começa com relação −70.
3. **Os pares candidatos que não foram sorteados** começam com uma **Não Agressão de 1 ano** (DP1-1). Sem ela, a IA declarava essas guerras no 4º dia (parte 10.2).
4. **Acordos:** 2 entre os pares candidatos a acordo com relação +25 ou mais depois do desvio.

### 4.2 A relação anda (dia 1 de cada estação)

1. **Relação-alvo** do par = a base da afinidade − 15 (disputa) + 10 por tratado de Aliança ou Casamento + (Sociabilidade − Agressividade dos dois líderes) ÷ 10 − 10 se um tem Mercado de Escravos e o outro é Religioso.
2. A relação anda **2 pontos** em direção ao alvo (como a Reputação).
3. **Na hora:** assinar +5; cancelar um tratado válido −10; casamento +20; Bênção +10 e Anátema −20 (Religião); declarar guerra: a relação vai para no máximo −70.

### 4.3 A Confiabilidade

Começa em 70. Quebrar tratado −20; declarar guerra sem motivo −20; ignorar o chamado do aliado −15; quebrar a trégua −30; +5 a cada estação sem quebras (até 100). A IA **não assina** nada com quem tem menos de 30.

### 4.4 A vontade de guerra (a cada 4 dias)

Para cada facção A e cada facção B em contato, em **Paz** e sem Não Agressão:

| Fator | Conta |
|---|---|
| Força | (Esquadrões de A ÷ Esquadrões de B − 1) × 50 (1,5 → +25; 0,8 → −10) |
| Agressividade do líder de A | Agressividade × 30 ÷ 100 |
| Ganância × riqueza de B | Ganância × 20 ÷ 100 × PIB de B ÷ PIB médio, até 20 |
| Motivo | **+20** se há motivo forte (um mapa em disputa entre os dois; Anátema; Reivindicação; Ofensa); **−20** se não há |
| Relação | −relação ÷ 4 |
| Cultura | Guerreira +15; Religiosa +15 contra cultura hostil |
| Guerras de A em andamento | −20 cada |
| Cansaço de guerra de A | −Cansaço ÷ 2 (0 até a Guerra) |
| Coalizão contra B | +30 (4.6) |
| Inverno | −15 |

- **Acima de 60:** A declara guerra a B. Se não há motivo, Confiabilidade −20. Notícia **Mundial**. Os aliados de B (Aliança Defensiva ou Total) recebem o **chamado** e entram, se a Lealdade do líder for 30 ou mais; se não, Confiabilidade −15.
- Uma facção declara **no máximo 1 guerra por avaliação** (a de maior vontade; DP1-2).
- O "Por quê?" guarda os fatores.

### 4.5 As propostas (dia 1 de cada estação)

Para cada par em Paz, pela ordem dos números das facções:

1. **A proposta:** a facção de maior Sociabilidade propõe o primeiro tratado da lista que ainda não existe e cuja relação mínima está cumprida: Acordo Comercial, Não Agressão (só se a vontade de guerra de um contra o outro passa de 40), Aliança Defensiva (só se um vizinho tem 1,5× a força dela e as culturas são afins), Aliança Total, Casamento (se as duas dinastias têm adultos solteiros).
2. **O aceite:** a outra aceita se a relação cumpre o mínimo, a Confiabilidade de quem propõe é 30 ou mais, e o tratado não a põe contra um aliado dela.
3. **Efeitos nesta versão:**
   - **Acordo Comercial:** tarifa 0 entre os dois (Facção, 4.3);
   - **Embargo** (de um lado só, a IA usa com atitude Hostil): as caravanas do outro não entram no território (a rota do Comércio desvia ou não sai);
   - **Não Agressão:** proíbe a declaração (4.4), por 64 dias;
   - **Alianças:** o chamado (4.4);
   - **Casamento:** um adulto de cada dinastia casa ([[Ficha Logica/Companheiros|Companheiros]], 4.6); +20; Não Agressão enquanto os dois viverem; o filho pode reclamar o trono do outro (motivo "Reivindicação").
4. **Fronteira fechada:** com atitude Hostil, a facção fecha a fronteira para a outra: as caravanas, os migrantes (População) e os viajantes dela não entram.

### 4.6 A Ameaça e a Coalizão

1. **Ameaçadora:** PIB acima de 1,6 × a média das facções, ou 3 cidades conquistadas em 2 anos.
2. As facções em contato com ela ganham **+30** na vontade de guerra contra ela e propõem entre si uma **Aliança Defensiva conjunta** (a Coalizão), sem a exigência de relação.
3. **Fim:** quando a Ameaça cai abaixo de 40 por 1 estação, ou quando o alvo faz paz com todos os membros. Ameaça = (PIB ÷ PIB médio) × 25 + cidades conquistadas em 2 anos × 15 (DP1-4).

## 5. Contas

**A vontade de guerra no 4º dia**, com líderes de Agressividade 50 e Ganância 50 (+15 e +10) e o PIB de B perto da média:

| A contra B | Esquadrões | Força | Motivo | Relação | Cultura | Total |
|---|---|---|---|---|---|---|
| Guerreira contra Autoritária | 20 / 16 | +12,5 | +20 | −55 → +13,75 | +15 | **86,25** |
| Autoritária contra Guerreira | 16 / 20 | −10 | +20 | +13,75 | 0 | **48,75** |
| Religiosa contra Arcana | 13 / 10 | +15 | +20 | +13,75 | +15 | **88,75** |
| Arcana contra Religiosa | 10 / 13 | −11,5 | +20 | +13,75 | 0 | **47,25** |
| Religiosa contra Escravista | 13 / 14 | −3,6 | +20 | +13,75 | +15 | **70,2** |
| Escravista contra Religiosa | 14 / 13 | +3,8 | +20 | +13,75 | 0 | **62,6** |

(+15 da Agressividade e +10 da Ganância em todas as linhas.) O exemplo do documento (Guerreira, 96) usa um líder mais agressivo: bate.

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Facção | O líder (Agressividade, Sociabilidade, Ganância, Lealdade); a tarifa |
| Companheiros | O casamento entre dinastias; a Reivindicação |
| Comércio | Acordo Comercial (tarifa 0), Embargo, fronteira fechada |
| População | A fronteira fechada e a guerra bloqueiam a migração |
| Religião | Bênção e Anátema mudam a relação |
| Escravidão | −10 da Religiosa com quem tem Mercado de Escravos |
| Notícias | Declarações, tratados e casamentos: Mundial ou Regional |
| Clima | −15 no inverno |
| Guerra (D7) | O que acontece na guerra; a paz; a Trégua; a vassalagem; o Cansaço |
| Save | Pares, relações, estados, tratados, Confiabilidade; o formato passa para a **versão 18** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `criarRelacoesDoPrimeiroDia()` | Regra 4.1 |
| `relacaoAlvoDoPar(a, b)`, `andarRelacoes()` | Regra 4.2 |
| `mudarConfiabilidade(faccao, valor, motivo)` | Regra 4.3 |
| `vontadeDeGuerra(a, b)` | Regra 4.4, com os fatores para o "Por quê?" |
| `declararGuerra(a, b, motivo)` | Regra 4.4 |
| `proporEAceitarTratados()` | Regra 4.5 |
| `ameacaDaFaccao(faccao)` | Regra 4.6 |

## 8. Testes

1. **A tabela da parte 5**, com os líderes médios.
2. **O começo:** em 1.000 mundos, nenhuma guerra nova nos primeiros 64 dias, além das sorteadas (a Não Agressão inicial, DP1-1).
3. **Uma guerra por vez:** uma facção com vontade acima de 60 contra 2 outras declara só contra a de maior vontade.
4. **Confiabilidade:** quebrar um tratado (70 → 50) e declarar sem motivo (50 → 30): ainda assina; mais uma quebra (10): ninguém assina com ela.
5. **Relação:** Autoritária × Tecnocrata começa em +40 e fica em +40 (alvo +40 com líderes médios).
6. **O teste de 1 ano não muda:** a vila não tem facção.

## 9. Fora desta versão

- O que acontece na guerra, o Placar, a paz, a Trégua, o Cansaço de guerra, a superextensão (Guerra, D7).
- A Passagem Militar, a vassalagem e o chamado às armas (Guerra).
- Os reféns e as missões de diplomacia (Objetivo e Progressão).
- A diplomacia do jogador (quando ele for dono de cidade ou líder): o painel e a chance com o Carisma e a Opinião.
- Mensageiros que levam as propostas; a IA que espera as notícias.

## 10. Simulação de mesa

### 10.1 Cenário

O primeiro dia de uma partida: o sorteio deu **1 guerra**, Arcana × Religiosa. Os líderes são médios (Agressividade, Ganância, Sociabilidade e Lealdade 50).

### 10.2 Os primeiros 64 dias sem a DP1-1

| Dia | O que acontece |
|---|---|
| 1 | Guerra: Arcana × Religiosa (sorteada) |
| 4 | Guerreira contra Autoritária: **86** → **declara**. Religiosa contra Escravista: 70,2 − 20 (já em guerra) = 50,2 → não. Escravista contra Religiosa: 62,6 → **declara** (a Religiosa já está em guerra com a Arcana) |
| 4 | A Escravista não tem aliança; a Autoritária chama ninguém (sem aliança) |
| 8 | A Religiosa está em 2 guerras; Tecnocrata e Arcana não se conhecem |
| Resultado | **3 guerras no 4º dia**, em quase toda partida, qualquer que fosse o sorteio |

**Achado:** o motivo "mapa em disputa" (+20) é permanente, e os pares hostis têm relação −55. Assim, a vontade passa de 60 sempre que o atacante é Guerreiro ou Religioso, ou um pouco mais forte. **Resolvido (DP1-1):** os pares candidatos não sorteados começam com Não Agressão de 1 ano. A primeira avaliação que conta é no dia 65.

### 10.3 O dia 65 com a DP1-1

| Par | Vontade | O que acontece |
|---|---|---|
| Guerreira contra Autoritária | 86,25 (o dia 65 é o 1º da Primavera do ano 2: sem o −15 do inverno) | **Declara** |
| Religiosa contra Escravista | 70,2 − 20 (em guerra com a Arcana) = 50,2 | Não |
| Escravista contra Religiosa | 62,6 | **Declara** |

Com a guerra do primeiro dia, a partida chega a 3 guerras **no começo do ano 2**, e não na primeira semana. **Atenção:** é muita guerra. A Guerra (D7) traz o Cansaço (que pesa −Cansaço ÷ 2) e a paz pelo Placar; a ficha da Guerra vai conferir se o mundo não fica em guerra o tempo todo.

### 10.4 Um casamento

Escravista × Autoritária (+40, Amigável). Na estação 2, a Escravista (mais sociável) propõe o Casamento: as duas dinastias têm adultos solteiros, relação 40 ≥ 0. Aceito: **+20** → 60; Não Agressão enquanto viverem; o alvo da relação sobe +10 → 50. Na estação 3, Aliança Total exige +60: proposta e aceita (+5 → 65).

### 10.5 Verificações

| Pergunta | Resultado |
|---|---|
| A tabela de relações iniciais do Mundo bate? | **Sim:** afim +40, hostil −40, disputa −15 (−55) |
| O sorteio das guerras iniciais tem sentido? | **Só com a DP1-1.** Sem ela, a IA declarava as outras no 4º dia (10.2) |
| Uma facção entra em várias guerras ao mesmo tempo? | Uma por avaliação (DP1-2) e −20 por guerra em andamento |
| A Religiosa contra a Escravista no começo | A vontade dela fica em 50 enquanto luta com a Arcana; mas a da Escravista (62,6) passa: a Religiosa pode levar 2 guerras. É o que o documento chama de "candidata a guerra" |
| O teste de 1 ano muda? | **Não** |

### 10.6 Casos extremos

| Caso | O que acontece |
|---|---|
| Dois pares declaram no mesmo dia, um contra o outro | Vale a declaração da facção de menor número; a outra já está em guerra com ela |
| O aliado está em Não Agressão com o agressor | O chamado não obriga: ele fica de fora sem perder Confiabilidade |
| Uma facção sem líder (crise de sucessão) | A vontade de guerra usa Agressividade 50 até o novo líder |
| Arcana e Tecnocrata sem contato | Não há par: nem relação nem guerra, até uma rota ou fronteira |
| A relação passa de +100 | Fica em +100 |

### 10.7 Decidido em modo automático (rodada DP1, seguir sugestão)

1. **DP1-1:** os pares candidatos a guerra não sorteados começam com Não Agressão de 1 ano.
2. **DP1-2:** uma facção declara no máximo 1 guerra por avaliação (a de maior vontade).
3. **DP1-3:** a relação anda 2 pontos por estação até um alvo (afinidade, disputa, tratados, líderes, escravidão).
4. **DP1-4:** a Ameaça = (PIB ÷ média) × 25 + cidades conquistadas em 2 anos × 15; a Coalizão acaba abaixo de 40.
5. **DP1-5:** a força para a IA é a quantidade de Esquadrões até a Guerra ter os exércitos de verdade.
6. **DP1-6:** a fronteira fechada com atitude Hostil bloqueia caravanas, migrantes e viajantes da outra facção.
