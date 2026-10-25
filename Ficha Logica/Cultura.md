---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase C2 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com CT1 vêm da **rodada CT1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Cultura

## 1. O que faz

Dá a cada NPC e a cada cidade uma das **6 culturas**. A cultura diz quais **leis** o povo quer (e o quanto se importa), qual é o **pensamento extremo** dele, com quais culturas ele se dá bem ou mal, e muda um pouco as **habilidades** de quem nasce nela. A distância entre as leis da facção e o desejo da cultura entra na **Felicidade**.

Documento do sistema: [[Sistema de Cultura/Descrição|Cultura]]; a conta da Felicidade, em [[Sistema de Felicidade e Estabilidade/Descrição|Felicidade e Estabilidade]].

**Esta versão:** as 6 culturas em dados, a cultura do NPC e da cidade, o fator **Leis × cultura** na Felicidade, as minorias hostis, os modificadores de habilidade e as afinidades. **Rito de Admissão, sucessão, personalidade, arte por cultura e assimilação** ficam na parte 9 (a assimilação vem na [[Ficha Logica/Plano das Fichas|Religião, C4]]).

## 2. Dados

### `data/culturas.json` (arquivo novo)

```json
{
  "id": "cultura.tecnocrata", "regiao": 5,
  "leis_desejadas": {
    "imposto": { "valor": "normal", "peso": 2 },
    "magia": { "valor": "proibida", "peso": 4 },
    "sistema_de_votacao": { "valor": "por_populacao", "peso": 2 },
    "poder_do_lider": { "valor": "fraco", "peso": 3 },
    "tolerancia_religiosa": { "valor": "tolerancia", "peso": 1 }
  },
  "extremo": { "lei": "magia", "valor": "magos_banidos" },
  "afinidades": { "arcana": "hostil", "religiosa": "hostil", "guerreira": "neutra", "autoritaria": "neutra", "escravista": "afim" },
  "habilidades_em_pontos": { "especialista": 10, "bracal": -5 },
  "rito_de_admissao": "servico", "sucessao": "merito"
}
```

Os valores das 6 culturas seguem a tabela de traços do documento e as leis iniciais da Facção (o desejo de cada cultura é a lei inicial da facção dela). As afinidades vêm do Mundo e Estado Inicial. Os pesos e os pontos de habilidade são **[Proposta]**, a ajustar.

### As leis em fila (CT1-1)

A distância é contada em **passos** entre as opções, então as opções de cada lei ficam **em fila, de um extremo ao outro**. O valor extremo de uma cultura é uma opção a mais, numa das pontas, que só existe para ser o extremo:

| Lei | Opções em fila (as dos extremos entre parênteses) |
|---|---|
| Magia | (só_magos_governam, Arcana) · privilégio_dos_magos · livre · restrita · proibida · (magos_banidos, Tecnocrata) |
| Escravidão | proibida · só_prisioneiros · como_pena · ampla · (qualquer_um_é_propriedade, Escravista) |
| Poder do líder | fraco · moderado · autoritário · (pedir_lei_é_crime, Autoritária) |
| Imposto | baixo · normal · alto |
| Tolerância religiosa | tolerância · fé_oficial · perseguição · (heresia_é_crime_capital, Religiosa) |
| Sistema de votação | por_população · maioria_simples · por_nível · dois_terços · conselho |
| Livro Sagrado obrigatório | não · sim |

A Guerreira tem o extremo na Conscrição ("só vale quem luta"), que entra com a Guerra. A [[Ficha Logica/Faccao|Facção]] passa a usar esta ordem.

### Estado

- **Cada NPC** ganha `cultura` (no save, com o registro do NPC).
- **A cultura da cidade** não é guardada: a **base** é a do dono (a da facção, ou a da região para a cidade sem facção; CT1-3); a **proporção** é a contagem dos NPCs da cidade por cultura (CT1-2).

## 3. Quando roda

| Quando | O quê |
|---|---|
| Ao criar um NPC (mundo, nascimento, migração) | Escolhe a cultura (4.1) e aplica os pontos de habilidade (4.4) |
| **A cada dia, passo 5** (felicidade) | O fator Leis × cultura e as minorias (4.2, 4.3) |
| Fim de cada estação | A cultura dominante da facção (4.5) |

## 4. Regras passo a passo

### 4.1 A cultura de quem nasce ou chega

1. **No começo do mundo:** os NPCs de cada cidade têm a cultura base dela.
2. **Nascido:** a cultura base da cidade.
3. **Migrante** (População, 4.5): a cultura da **região do mapa** com 75% de chance; com 25%, a de uma **região vizinha** sorteada (as que tocam a região na malha de blocos). Sorteio com a sequência de População da cidade.
4. **A vila do jogador:** os 15 NPCs da `vila_inicial` têm a cultura da região onde ela fica. A cultura do **jogador** é a dele (Início de Jogo) e não muda a vila (CT1-3). O teste de 1 ano roda **sem cultura** (vila sem facção, sem lei): o fator é 0 e os pontos de habilidade não se aplicam.

### 4.2 O fator Leis × cultura (5º fator da Felicidade)

Para cada classe da cidade, para cada cultura presente nela:

1. **Soma da distância** = Σ, nas leis desejadas, peso × |passos entre a lei da facção e o valor desejado|.
2. **Extremo:** se a lei do extremo está **no valor extremo**: +10. Se ela está **do outro lado do valor desejado** (longe do extremo) e a **2 passos ou mais** do desejado: −15 a mais (o extremo violado). No resto: nada.
3. **Fator da cultura** = −soma + extremo, preso entre **−30 e +10**.
4. **Fator da classe** = a média dos fatores das culturas, pesada pela quantidade de NPCs de cada cultura na classe, arredondada para baixo (em décimos).

Cidade **sem facção**: o fator é 0 (não há leis). Ele entra no alvo da Felicidade junto dos 4 de hoje (Felicidade, 4.1).

### 4.3 As minorias hostis

Na mesma classe: −10 × (NPCs de cultura **hostil** à cultura base da cidade ÷ NPCs da classe), em décimos. Entra no mesmo fator (o total continua preso entre −30 e +10).

### 4.4 As habilidades

Ao criar o NPC, depois do sorteio na faixa da classe (Classe Social, 4.6): soma os `habilidades_em_pontos` da cultura, preso entre 0 e 100.

### 4.5 A cultura dominante da facção

No fim de cada estação: a cultura com **mais NPCs** somados nas cidades da facção; empate: a da facção. Hoje é sempre a da facção; muda com a conquista (fase D).

## 5. Contas

| Caso | Conta | Fator |
|---|---|---|
| Cidade da cultura da facção, leis = desejo | 0 + 0 | **0** |
| Mesma cidade, depois de o Imposto ir de Normal a Alto (peso 2) | −2 | **−2** (e mais −5 da própria lei de imposto) |
| Tecnocratas numa facção Arcana (Magia: privilégio; desejo: proibida, 3 passos × 4) | −12; o privilégio fica do outro lado do desejo, a 3 passos: extremo violado, −15 | **−27** |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Felicidade | O 5º fator (4.2 e 4.3), de −30 a +10 |
| Facção | As leis em fila (CT1-1); o desejo usado na assembleia; o extremo fora da assembleia (FA1-5) |
| População | A cultura do nascido e do migrante |
| Classe Social | Os pontos de habilidade depois da faixa da classe |
| Outras cidades | Os NPCs do mundo nascem com a cultura da cidade |
| Reputação | A base por afinidade (a cultura do jogador) |
| Religião (C4) | A assimilação muda a cultura das minorias |
| Save | A cultura de cada NPC; o formato passa para a **versão 11** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `culturaDoNpc` | No registro do NPC |
| `culturaBaseDaCidade(cidade)` | Regra 4.1 e CT1-3 |
| `escolherCulturaDoMigrante(cidade)` | Regra 4.1 |
| `fatorLeisECultura(cidade, classe)` | Regras 4.2 e 4.3 |
| `passosEntre(lei, valorA, valorB)` | A distância na fila |
| `culturaDominante(faccao)` | Regra 4.5 |

## 8. Testes

1. **Fator 0** numa cidade com as leis iniciais da sua facção.
2. **O exemplo de −27** da parte 5.
3. **Mistura:** 80 Arcanos (0) e 20 Religiosos (−14) e a minoria hostil: fator da classe **−4,8** (parte 10.2).
4. **Teto:** uma cultura com soma 50 fica em −30.
5. **O teste de 1 ano não muda:** a vila sem facção tem fator 0 e não aplica pontos de habilidade.
6. **Migrante:** em 1.000 migrantes de uma cidade, cerca de 750 da cultura da região.

## 9. Fora desta versão

- Rito de Admissão e sucessão (guardados nos dados; usados pela Facção na fase D).
- As tendências de Personalidade por cultura (quando o NPC tiver Personalidade).
- A assimilação das minorias (Religião, C4).
- A arte por cultura (troca de paleta; Pipeline de Arte).
- O efeito do extremo na Diplomacia.
- A discriminação dos não Magos na Arcana (quando existir o atributo Mago).

## 10. Simulação de mesa

### 10.1 Cenário

A **Cidade Arcana** (7,2), nível 3, com 100 Civis: 80 Arcanos e **20 Religiosos** (migrantes). A Arcana e a Religiosa são **hostis**. As leis da Arcana são as iniciais.

### 10.2 O fator num dia

**Arcanos (80):** leis = desejo → soma 0; extremo (só_magos_governam) não está no valor, e a lei atual (privilégio) fica do lado dele: nada. **Fator 0.**

**Religiosos (20)**, contra as leis Arcanas:

| Lei | Lei Arcana | Desejo Religioso | Passos | Peso | Distância |
|---|---|---|---|---|---|
| Magia | privilégio | restrita | 2 | 3 | 6 |
| Tolerância | fé_oficial | perseguição | 1 | 3 | 3 |
| Livro Sagrado | não | sim | 1 | 4 | 4 |
| Votação | conselho | dois_terços | 1 | 1 | 1 |
| Poder do líder | moderado | moderado | 0 | — | 0 |
| **Soma** | | | | | **14** |

Extremo Religioso (heresia_é_crime_capital, depois de perseguição): a lei está em fé_oficial, do outro lado do desejo, mas a só **1 passo** dele: nada. **Fator −14.**

**A classe:** (80 × 0 + 20 × −14) ÷ 100 = **−2,8**; minoria hostil: −10 × 20 ÷ 100 = **−2,0**. **Total −4,8.**

O alvo da Felicidade da classe Civil cai de, por exemplo, 58 para **53,2**. A Felicidade anda 10% ao dia até lá. Na Cidade, a condição para subir de nível pede Felicidade ≥ 55: **a minoria Religiosa pode travar o nível da cidade.**

### 10.3 Verificações

| Pergunta | Resultado |
|---|---|
| As leis iniciais deixam alguém infeliz? | **Não:** em cada facção, as cidades são da cultura dela, e as leis iniciais são o desejo (fator 0) |
| A conta da distância faz sentido? | **Só com as leis em fila.** **Achado:** na Facção, a Magia estava como "livre, restrita, privilégio, proibida". Assim, o "privilégio" ficava a 1 passo do "proibida", como se o Arcano e o Tecnocrata quase concordassem. **Resolvido (CT1-1):** as opções em fila, de um extremo ao outro |
| Uma minoria pequena pesa demais? | **Não:** 20% de Religiosos tira 4,8 pontos; uma minoria de 5% tiraria 1,2 |
| O 5º fator muda o teste de 1 ano? | **Não:** a vila do teste não tem facção (fator 0) |
| A cultura da vila do jogador | **A da região** (CT1-3). **Achado:** a ficha da Facção (10.4) tinha a vila como Tecnocrata, a cultura do jogador. Os NPCs da vila são da região; o jogador propõe e vota na assembleia pelo que ele quiser (Facção, 4.5) |

### 10.4 Casos extremos

| Caso | O que acontece |
|---|---|
| Uma classe com 0 NPCs | Não há fator (a Felicidade dessa classe não é calculada) |
| Uma cidade só de minorias (todas hostis) | O fator das culturas + −10 da minoria, preso em −30 |
| A facção adota o extremo de uma cultura | +10 para essa cultura; para as hostis a ele, a distância cresce |
| Um migrante de cultura que não existe na facção | Conta normalmente: a cultura dele tem desejos para todas as leis |
| Um NPC com habilidade 95 e cultura +10 | Fica em 100 |

### 10.5 O que muda em outras fichas

- **Facção:** a ordem das opções da Magia, da Escravidão, do Poder do líder, da Tolerância e da Votação (CT1-1); na simulação 10.4, a vila do jogador é da cultura da região, e o jogador é quem propõe a lei.
- **Felicidade:** um 5º fator, Leis × cultura, de −30 a +10.

### 10.6 Decidido em modo automático (rodada CT1, seguir sugestão)

1. **CT1-1:** as opções de cada lei ficam em fila, e os extremos são opções a mais nas pontas.
2. **CT1-2:** a proporção de culturas da cidade é a contagem dos NPCs (eles já existem como registros; OC1-2).
3. **CT1-3:** a cultura base da cidade sem facção é a da região; a vila do jogador é da região, não do jogador.
4. **CT1-4:** o extremo dá +10 quando a lei está no valor extremo e −15 quando ela está do outro lado do desejo, a 2 passos ou mais dele.
5. **CT1-5:** a minoria hostil tira 10 × a proporção dela na classe.
6. **CT1-6:** o migrante tem 75% de chance de ser da região e 25% de uma região vizinha.

### 10.7 Ajustes do código (marco C2, 2026-10-24)

1. **O `culturas.json`** tem as 6 culturas com a região (1 Guerreira, 2 Arcana, 3 Autoritária, 4 Religiosa, 5 Tecnocrata, 6 Escravista), o desejo das leis, o extremo, as afinidades (a tabela do Mundo e Estado Inicial), os pontos de habilidade, o rito e a sucessão. Os pesos da Religiosa e da Tecnocrata seguem as partes 2 e 10.2; os das outras, os pontos de habilidade, o rito e a sucessão são **provisórios**, para o autor revisar. A Guerreira ainda não tem extremo (a Conscrição entra com a Guerra).
2. **Os pontos de Especialista** ficam guardados: o NPC ainda não tem essa habilidade (TE2-1). Os de Braçal e Manufaturado valem.
3. **Os pontos de habilidade só valem nas outras cidades:** a vila do jogador fica como no teste de 1 ano (4.1, passo 4).
4. **O sorteio da cultura do migrante** usa a sequência 8, a partir da semente, do mapa e do número do NPC; não muda os sorteios da População e não precisa de nada a mais no save.
5. **A vila do jogador** fica na região 3: os moradores são Autoritários. O fator dela é 0 (sem facção).
6. **O fator é calculado todo dia** com as leis de agora, antes do dia das cidades (junto das leis da Facção), e só para a classe Civil (a única desta versão).

