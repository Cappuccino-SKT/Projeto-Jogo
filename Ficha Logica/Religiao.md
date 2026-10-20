---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase C4 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com RL1 vêm da **rodada RL1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Religião

## 1. O que faz

Complemento da [[Ficha Logica/Cultura|Cultura]]: cada cultura tem **a sua fé**, com uma **Devoção**. O **Templo** cobre parte da cidade e dá (ou tira) Felicidade. Os **festivais** do calendário dão Felicidade e custam comida, trabalho e dinheiro. A lei de **Tolerância** pesa nas minorias, e as minorias mudam de cultura devagar (**assimilação**). O Sumo Sacerdote **abençoa** o jogador, e a **peregrinação** dá reputação.

Documento do sistema: [[Sistema de Religião/Descrição|Religião e Fé]].

**Esta versão:** a cobertura e o fator Religião na Felicidade, as oferendas, os festivais, a Tolerância, a assimilação, o Templo da Religiosa na Epidemia, a Bênção ao jogador e a peregrinação do jogador. **Heresia como ato, Anátema, Guerra santa, peregrinos na estrada, missões e a escolha do herdeiro** ficam na parte 9.

## 2. Dados

### `data/religiao.json` (arquivo novo)

```json
{
  "fes": {
    "religiosa":   { "devocao": "alta",   "culto": "templo" },
    "autoritaria": { "devocao": "media",  "culto": "templo" },
    "guerreira":   { "devocao": "media",  "culto": "templo" },
    "escravista":  { "devocao": "baixa",  "culto": "templo" },
    "arcana":      { "devocao": "baixa",  "culto": "torre_arcana" },
    "tecnocrata":  { "devocao": "nenhuma", "culto": "laboratorio" }
  },
  "npcs_por_sacerdote": 20,
  "felicidade_por_devocao": { "alta": [10, -5, 50], "media": [7], "baixa": [4], "nenhuma": [0] },
  "oferenda_em_porcento_da_renda": 2,
  "festival": { "felicidade": 8, "dias_do_bonus": 8, "comida_extra_em_porcento": 50,
                "producao_no_primeiro_dia_em_porcento": 50, "custo_em_porcento_do_banco": 2,
                "patrocinio_felicidade_a_mais": 4, "cancelar_na_devocao_alta": -5 },
  "festivais": [
    { "cultura": "todas", "nome": "Festa da Colheita", "estacao": "outono", "dias": [1, 2] },
    { "cultura": "religiosa", "nome": "Festa da Cidade Santa", "estacao": "primavera", "dias": [8, 9] },
    { "cultura": "religiosa", "nome": "Dia santo", "estacao": "verao", "dias": [8], "metade": true }
  ],
  "tolerancia": {
    "fe_oficial": { "imposto_a_mais_das_minorias_em_porcento": 5, "assimilacao_por_estacao_em_porcento": 2 },
    "tolerancia": { "assimilacao_por_estacao_em_porcento": 1 },
    "perseguicao": { "felicidade_das_minorias": -15, "migracao_das_minorias_vezes": 2, "controle": 5, "assimilacao_por_estacao_em_porcento": 4 }
  },
  "templo_da_religiosa_mortes_na_epidemia_em_porcento": 75,
  "peregrinacao": { "oferenda_minima": 50, "reputacao": 10, "vezes_por_ano": 1 },
  "bencao": { "reputacao_minima": 40, "reputacao": 10 }
}
```

`felicidade_por_devocao` da Alta: o máximo (+10), o valor com cobertura baixa (−5) e o limite da cobertura (50%). A lista completa de festivais segue a tabela do documento.

### Estado (vai no save)

| Campo | Exemplo |
|---|---|
| `fracaoDeAssimilacaoEmPessoas` | por cidade e por cultura de minoria: 80 |
| `festivalDeHoje` | por cidade: nenhum, ou o festival com "patrocinado" ou "cancelado" |
| `ultimaPeregrinacaoDoJogador` | ano 2 |
| `ultimaBencaoDoJogador` | ano 2, estação 3 |

**Derivados:** a cobertura de cada cidade (pelos Sacerdotes empregados); a minoria (os NPCs de cultura diferente da base da cidade).

## 3. Quando roda

| Quando | O quê |
|---|---|
| **A cada dia, passo 1** (consumo) | As oferendas (4.3); a comida extra do festival (4.4) |
| **A cada dia, passo 2** (salários) | O imposto a mais das minorias na Fé oficial (4.5) |
| **A cada dia, passo 5** (felicidade) | O fator Religião (4.2); a Perseguição nas minorias (4.5) |
| **A cada dia, passo 6** (eventos) | Começa o festival do calendário (4.4) |
| **Dia 1 de cada estação** | A assimilação (4.6); a Bênção (4.8) |
| Quando o jogador faz a oferenda em (8,4) | A peregrinação (4.9) |

## 4. Regras passo a passo

### 4.1 A cobertura

1. **Sacerdotes** = os empregados presentes dos Templos da cidade (Função Sacerdote, Civil). Na **Arcana**, os empregados da Torre Arcana; na **Tecnocrata**, não há cobertura.
2. **Cobertura** = Sacerdotes × 20 ÷ NPCs reais da cidade, até 100%.
3. **O pacote dá Templos que bastam (RL1-1):** cada cidade de facção com fé (menos a Tecnocrata) começa com Templos de cobertura de **pelo menos 60%**. As capitais têm 2 Templos (o documento permite); a Religiosa ganha um Templo já no vilarejo (7,4).

### 4.2 O fator Religião (6º fator da Felicidade)

Para cada classe, para cada cultura presente:

1. A cobertura conta **só se o culto da cidade é o dessa cultura** (a minoria de outra fé tem cobertura 0).
2. **Alta:** cobertura × 10, mas **−5** se a cobertura está abaixo de 50%. **Média:** cobertura × 7. **Baixa:** cobertura × 4. **Nenhuma:** 0.
3. O fator da classe = a média pesada pela quantidade de NPCs de cada cultura, em décimos (arredondado para baixo).

A cidade **sem facção** (a vila do teste) tem fator 0: não tem Templo nem cultura ativa.

### 4.3 As oferendas (passo 1)

Os Civis de cultura com fé dão **2% da renda do dia** ao Templo da cidade. O valor sai do **gasto extra** (dentro do teto, FA1-2) e vai para o **Banco da cidade** (até a Facção ter onde gastar; FA1-1). Não cria nem some com dinheiro: muda só para onde vai o gasto.

### 4.4 Os festivais

1. **No dia do festival** (passo 6): começa o evento duradouro "Festival": **Felicidade +8 por 8 dias** (os dias santos: +4).
2. **Custo:** 2% do Banco da cidade vão para a **classe Civil** (a festa é paga a ela: comida e bebida; o dinheiro fica no mundo).
3. **Produção:** no 1º dia, o turno vale metade (LOD 0: os NPCs saem às 11 h; LOD 2: as horas presumidas × 50%).
4. **Comida extra (RL1-2):** nos dias do festival, a classe compra até **50% a mais** da cesta de comida, **só da sobra** (o estoque acima do alvo) e com o dinheiro dela. Sem sobra, não há comida extra.
5. **O jogador**, na vila dele, pode **patrocinar** (paga do bolso o dobro do custo; Felicidade +12) ou **cancelar** (sem custo; −5 por 1 estação se a Devoção é Alta).
6. Notícia **Local** (Notícias).
7. **A vila sem facção** continua com a Festa da Colheita da fatia (Eventos: dia 12 do Outono, +10), para o teste de 1 ano não mudar (RL1-3).

### 4.5 A Tolerância

Para as **minorias** (os NPCs de cultura diferente da base da cidade):

- **Fé oficial:** +5 pontos no imposto do salário delas (passo 2).
- **Perseguição:** −15 na Felicidade, pesado pela proporção delas na classe (passo 5); na migração da População, as minorias saem primeiro e **2 por estação** em vez de 1; Controle +5 (Crime, CJ1-4).
- **Tolerância:** nada.

### 4.6 A assimilação (dia 1 de cada estação)

1. Só onde a cidade tem o **culto da cultura base** (Templo, Torre Arcana; na Tecnocrata, Laboratório ou Universidade).
2. Para cada cultura de minoria: fração += NPCs dela × 100 × taxa (1, 2 ou 4%).
3. A cada 100 na fração: **1 NPC** dessa cultura muda para a base: o **mais novo** (as crianças aprendem primeiro; RL1-4). Importantes e Controlados nunca.

### 4.7 O Templo da Religiosa na Epidemia

Na cidade Religiosa com Templo, as mortes da Epidemia ([[Ficha Logica/Cura|Cura]]) × 75%, arredondado para baixo. Soma com o Remédio (que corta pela metade): × 50% × 75%.

### 4.8 A Bênção ao jogador (dia 1 de cada estação)

Se a reputação do jogador com a Religiosa é **+40 ou mais** e ele fez algo pela fé na última estação (peregrinação ou doação à Religiosa), o Sumo Sacerdote o abençoa: **+10 de reputação** com a Religiosa. Uma por estação, no máximo.

### 4.9 A peregrinação do jogador

Na Capital Religiosa (8,4), a oferenda de **50 moedas ou mais** no Templo dá **+10 de reputação** com a Religiosa, **1 vez por ano**. O dinheiro vai para o Banco da cidade. Fica guardado para o Rito de Admissão (fase D).

## 5. Contas

| Cidade | NPCs | Templo (Sacerdotes) | Cobertura | Fator (Devoção) |
|---|---|---|---|---|
| Cidade Autoritária, nível 3 | 100 | 1 Templo nível 3 (5) | 100% | **+7** |
| Capital Religiosa, nível 4, só 1 Templo nível 4 | 300 | 7 | 47% | **−5** (Alta abaixo de 50%) |
| A mesma, com 2 Templos nível 4 (RL1-1) | 300 | 14 | 93% | **+9,3** |
| Vilarejo Religioso (7,4), nível 2, sem Templo (pacote do documento) | 40 | 0 | 0% | **−5** |
| O mesmo, com Templo nível 2 (RL1-1) | 40 | 3 | 100% | **+10** |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Felicidade | O 6º fator, Religião (−5 a +10); o festival no fator Eventos; a Perseguição nas minorias |
| Cultura | A fé e a Devoção de cada cultura; a assimilação muda a cultura do NPC |
| Outras cidades | O pacote dá os Templos (RL1-1) |
| Economia | As oferendas saem do gasto extra; o imposto a mais das minorias |
| População | A migração das minorias na Perseguição |
| Cura | As mortes da Epidemia × 75% com o Templo da Religiosa |
| Eventos | Os festivais do calendário; a Festa da Colheita da fatia fica só na vila sem facção |
| Crime | O Controle +5 na Perseguição; o Livro Sagrado (CJ1) |
| Reputação | A peregrinação e a Bênção |
| Notícias | O festival: Local |
| Save | Assimilação, festivais, peregrinação, Bênção; o formato passa para a **versão 13** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `coberturaDoCulto(cidade)` | Regra 4.1 |
| `fatorReligiao(cidade, classe)` | Regra 4.2 |
| `darOferendas(cidade)` | Regra 4.3 |
| `comecarFestivalDoCalendario(cidade)` | Regra 4.4 |
| `aplicarTolerancia(cidade)` | Regra 4.5 |
| `assimilarMinorias(cidade)` | Regra 4.6 |
| `abencoarJogador()` / `fazerPeregrinacao(oferenda)` | Regras 4.8 e 4.9 |

## 8. Testes

1. **A tabela da parte 5**, linha a linha.
2. **A minoria sem culto:** 80 Arcanos (Torre Arcana com 100%: +4) e 20 Religiosos (sem Templo: −5): fator **+2,2**.
3. **Festival sem sobra:** estoque no alvo, nenhuma comida extra; com 60 acima do alvo e 40 Civis, 20 a mais.
4. **Assimilação:** 40 Autoritários com Fé oficial: 1 NPC na 2ª estação (fração 80, depois 160 − 100 = 60), e **3 em 5 estações** (o documento dizia cerca de 4, sem descontar quem já mudou).
5. **Mortes:** 4 mortes de Epidemia viram 3 com o Templo da Religiosa; com o Remédio também: 4 × 50% = 2 × 75% = 1.
6. **O teste de 1 ano não muda:** a vila sem facção tem fator 0 e a mesma Festa da Colheita.

## 9. Fora desta versão

- A heresia como ato (construir culto, blasfemar, profanar) e o crime dela (Crime, quando o jogador puder agir assim).
- O Anátema, a Guerra santa e a Bênção a facções (Diplomacia e Guerra).
- A escolha do herdeiro pelo Sumo Sacerdote (Facção, fase D).
- Os peregrinos na estrada e a demanda extra na cidade santa (com os encontros da Viagem e o dinheiro que eles trazem; RL1-5).
- As missões do Sumo Sacerdote; a cura mais rápida dos pobres (Combate e Cura).
- Os missionários; os Magos na Religiosa.

## 10. Simulação de mesa

### 10.1 Cenário

- **Capital Religiosa (8,4):** 300 NPCs, 2 Templos nível 4 (RL1-1), Perseguição. Estoque de Pão 1.500 (alvo, 5 dias de 300).
- **Cidade Arcana (7,2):** 100 NPCs, 80 Arcanos e 20 Religiosos (o exemplo da Cultura), Torre Arcana com 5 Magos (100%), Fé oficial.

### 10.2 A Festa da Cidade Santa (Primavera, dias 8 e 9) na capital

| Dia | O que acontece |
|---|---|
| 8, passo 1 | Cesta: 300 Pães. Sobra acima do alvo: 0 → **sem comida extra** |
| 8, passo 6 | Começa o festival: +8 por 8 dias. Custo: 2% de um Banco de 20.000 = **400** para a classe Civil |
| 8, turno | Metade da produção: cerca de 150 Pães a menos que um dia comum (o estoque fica abaixo do alvo) |
| 9, passo 1 | Sem sobra: sem comida extra. O preço do Pão sobe (o estoque está abaixo do alvo) |
| 9 a 15 | Felicidade com +8 nos eventos; a produção volta ao normal e o estoque se recupera em poucos dias |

**Sem a RL1-2:** a capital compraria 150 Pães a mais em cada dia (300 no festival) e perderia 150 na produção: **450 Pães**, 1,5 dia de comida, a cada festival. São 4 festas por ano na Religiosa (Colheita, Cidade Santa e 2 dias santos), cerca de **4 a 5 dias de comida por ano**, numa cidade que produz o que come (OC1-4). Seria fome todo ano.

### 10.3 O fator Religião na Cidade Arcana

| Cultura | NPCs | Culto da cidade é o dela? | Fator | Peso |
|---|---|---|---|---|
| Arcanos | 80 | Sim (Torre Arcana, 100%) | 100% × 4 = +4 | +3,2 |
| Religiosos | 20 | Não (cobertura 0) | Alta abaixo de 50%: −5 | −1,0 |
| **Classe** | | | | **+2,2** |

Com a Cultura (−4,8, Cultura 10.2), a classe fica com **−2,6** desses dois fatores. Na Fé oficial, os 20 Religiosos pagam 25% de imposto (20 + 5). A assimilação (2% por estação): 20 × 100 × 2% = 40 por estação → **1 NPC a cada 2,5 estações**, cerca de 6 NPCs em 4 anos.

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| A Religiosa começa feliz? | **Só com a RL1-1.** **Achado:** pelo pacote do documento, a capital (1 Templo, 47%) e o vilarejo (sem Templo) teriam −5 desde o dia 1: a cultura mais devota seria a mais infeliz |
| Os festivais dão fome? | **Não, com a RL1-2:** a comida extra só sai da sobra (10.2) |
| O dinheiro do festival some? | **Não:** os 2% do Banco vão para a classe Civil, que gasta de volta |
| O 6º fator muda o teste de 1 ano? | **Não:** a vila sem facção tem fator 0, e a Festa da Colheita dela fica a mesma (RL1-3) |
| As facções ficam mais felizes que a vila? | **Sim:** com cobertura cheia, de +4 a +10. **Atenção:** a Cidade pede Felicidade ≥ 55 para subir de nível; as cidades de fé sobem mais fácil que a Tecnocrata (0). É uma diferença de cultura, de propósito |
| A assimilação é rápida demais? | **Não:** 1% a 4% por estação; 40 NPCs com 4% → 1,6 por estação |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Os Sacerdotes saem (salário atrasado) | Cobertura cai; na Religiosa abaixo de 50%: −5 até contratar de novo |
| Dois festivais no mesmo dia (Colheita e Arcana no Outono?) | Não acontece com o calendário do documento; se acontecer, só o primeiro da lista vale |
| Uma cidade só de minorias | Sem cobertura para elas; com Perseguição, −15 em todos |
| A minoria acaba pela assimilação | A fração dela zera e sai do save |
| O jogador peregrina duas vezes no ano | A segunda oferenda é aceita (o dinheiro vai para o Banco), mas não dá reputação |
| A Tecnocrata e a Festa da Colheita | Tem festivais (são da cultura), mas não tem cobertura |

### 10.6 O que muda em outras fichas

- **Felicidade:** o 6º fator, Religião (−5 a +10).
- **Outras cidades:** o pacote ganha os Templos que bastam (RL1-1).
- **Cura:** as mortes da Epidemia × 75% com o Templo da Religiosa.
- **Eventos:** a Festa da Colheita vira festival do calendário nas cidades de facção; na vila sem facção, fica como na fatia.

### 10.7 Decidido em modo automático (rodada RL1, seguir sugestão)

1. **RL1-1:** o pacote dá a cada cidade com fé Templos com pelo menos 60% de cobertura; as capitais com 2 Templos; a Religiosa com Templo no vilarejo.
2. **RL1-2:** a comida extra do festival sai só da sobra acima do alvo.
3. **RL1-3:** a vila sem facção fica com a Festa da Colheita da fatia.
4. **RL1-4:** na assimilação, muda primeiro o NPC mais novo da minoria.
5. **RL1-5:** os peregrinos do mundo (demanda e dinheiro na cidade santa) ficam para quando forem encontros de verdade na Viagem.
6. **RL1-6:** as oferendas saem do gasto extra e vão para o Banco da cidade.
