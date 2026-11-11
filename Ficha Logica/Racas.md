---
tags: [ficha-logica]
---

> O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-31 (fase D8 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]], rodada RA1) e **refeita em 2026-11-11 pela rodada HI13**, com as raças da história nova. A história das raças está no [[Documentos/Mundo e História/Descrição|Mundo e História]], parte 6. **O marco vem depois dos marcos da fase D** (HI3-2); até lá, o jogo tem só humanos.

# Ficha Lógica: Raças

## 1. O que faz

Dá a cada NPC uma **raça**, ao lado da cultura. A raça decide:

1. **A idade de adulto e de velho** (decisão do autor, 2026-10-26).
2. **As habilidades, a Personalidade, uma vantagem e um defeito** (RA1-2).
3. **Como ela tem filhos:** só com a mesma raça (RA1-5).
4. **Como cada reino a trata:** a lei "As outras raças" (RA1-4).

Três coisas do mundo nascem desta ficha: as **Marés de Mana** (RA1-7; mantidas pela HI13-9), a **Câmara de Forja** dos Forjados e as **colônias Fungoides**. Os monstros ficam numa ficha própria, **Fauna e Monstros** (RA1-10).

**O sistema precisa aceitar raças novas com facilidade** (decisão do autor, 2026-10-31). Por isso, cada raça é uma entrada no `racas.json`, e as leis e as culturas reagem às **marcas** da raça ("mutante", "magica", "monstruosa", "artificial", "mestica", "de_outro_mundo"), não ao nome dela. Uma raça nova com marcas conhecidas já funciona sem regra nova.

### De onde vieram os números (HI13-4)

| Raça nova | Herdou os números de | O que mudou |
|---|---|---|
| Humanos | Humanos | Nada |
| **Rudeanos** (os Magos do outro mundo) | Ascendidos | A vida bem mais longa: velho aos 400 [Proposta] (1.000 anos da história), para os que vieram na Colisão ainda estarem vivos no Ano 412 |
| **Mestiços** | Humanos | Todos nascem com o dom mágico |
| **Forjados** (o nome novo dos Humanos-Cinzentos) | Khadim | Feitos para a guerra, não para o trabalho: os pontos vão para o combate |
| **Cinzentos** | Langren | Nada |
| **Fungoides** | Quitinos | A mente coletiva no lugar da rainha |
| **Humanos-Peludos** | Raça nova | Adulto aos 2, velho aos 12, fertilidade dobrada |

O Eterno saiu. Os Forjados são a tentativa do Antigo Império Mágico de **copiar em laboratório a força dos Cinzentos**.

## 2. Dados

### `data/racas.json` (arquivo novo)

```json
{
  "racas": [
    { "id": "humano", "nome": "Humanos", "adulto": 5, "velho": 31, "reproducao": "normal", "fertilidade_em_porcento": 100,
      "habilidades_em_pontos": {}, "personalidade_em_pontos": {}, "dom_magico": "normal", "marcas": [] },
    { "id": "rudeano", "nome": "Rudeanos", "adulto": 10, "velho": 400, "reproducao": "normal", "fertilidade_em_porcento": 2,
      "habilidades_em_pontos": { "especialista": 15 }, "personalidade_em_pontos": { "sociabilidade": -20 },
      "dom_magico": "sempre", "marcas": ["magica", "de_outro_mundo"] },
    { "id": "mestico", "nome": "Mestiços", "adulto": 5, "velho": 31, "reproducao": "normal", "fertilidade_em_porcento": 100,
      "habilidades_em_pontos": {}, "personalidade_em_pontos": { "sociabilidade": -10 },
      "dom_magico": "sempre", "marcas": ["magica", "mestica"] },
    { "id": "forjado", "nome": "Forjados", "adulto": 0, "velho": 10, "reproducao": "fabricada",
      "habilidades_em_pontos": { "corpo_a_corpo": 20, "oficio": -15, "comercio": -15 },
      "personalidade_fixa": { "lealdade": 100, "coragem": 80 },
      "sobe_de_habilidade": false, "pode_liderar": false, "faz_greve_e_foge": false, "dom_magico": "nunca", "marcas": ["artificial"] },
    { "id": "cinzento", "nome": "Cinzentos", "adulto": 4, "velho": 24, "reproducao": "normal", "fertilidade_em_porcento": 100,
      "habilidades_em_pontos": { "corpo_a_corpo": 20, "oficio": -15, "comercio": -15 }, "personalidade_em_pontos": { "agressividade": 30 },
      "dano_recebido_no_corpo_a_corpo_em_porcento": 80, "producao_em_porcento": 70, "dom_magico": "nunca", "marcas": ["mutante"] },
    { "id": "fungoide", "nome": "Fungoides", "adulto": 2, "velho": 15, "reproducao": "colonia",
      "habilidades_em_pontos": { "bracal": 10 }, "personalidade_em_pontos": { "coragem": 20 },
      "dano_recebido_em_porcento": 70, "dom_magico": "nunca", "marcas": ["mutante", "monstruosa"] },
    { "id": "peludo", "nome": "Humanos-Peludos", "adulto": 2, "velho": 12, "reproducao": "normal", "fertilidade_em_porcento": 200,
      "habilidades_em_pontos": {}, "personalidade_em_pontos": {},
      "dano_recebido_em_porcento": 80, "alguns_monstros_nao_atacam": true, "dom_magico": "normal", "marcas": ["mutante"] }
  ],
  "lei_das_outras_racas": {
    "opcoes": ["iguais", "segunda_classe", "escravizaveis", "cacadas"],
    "felicidade": [0, -10, -25, -40],
    "imposto_da_segunda_classe_em_porcento": 200
  },
  "mare_de_mana": { "por_ano_no_mundo": 1, "dura_estacoes": 1, "dom_magico_dos_nascidos_vezes": 5 },
  "camara_de_forja": { "dias_por_forjado": 10 },
  "colonia_fungoide": { "importantes": 3, "anos_por_colonia_nova": 5, "chance_de_ser_hostil_em_porcento": 50,
                        "chance_de_nascer_com_mente_propria_em_porcento": 1 }
}
```

- **`reproducao`:** "normal" (casal da mesma raça), "colonia" (os Fungoides não têm gênero: nascem da colônia), "fabricada" (só pela Câmara de Forja) ou "nenhuma".
- **`dom_magico` (rodada RV1-3):** "normal" (a regra de nascer Mago da [[Ficha Logica/Magia|Magia]], 4.1), "nunca" ou "sempre". O número fica só na ficha da Magia.
- **Os números dos Forjados, dos Peludos e a mente própria dos Fungoides são [Proposta]:** a história só diz "altos atributos de combate", "alta resistência", "se reproduzem muito rápido" e "alguns poucos nascem com intelecto".
- **`alguns_monstros_nao_atacam`:** quais monstros, decide a ficha Fauna e Monstros.

### No `culturas.json`, cada cultura ganha

```json
"racas_da_populacao": { "humano": 80, "outras": 20 },
"racas_do_povo": ["humano"],
"lei_das_outras_racas": "escravizaveis",
"lei_por_marca": { "magica": "cacadas", "com_o_dom": "cacadas" }
```

(O exemplo é o da Federação Humana.)

- **`racas_da_populacao`:** a porcentagem de cada raça no reino (as da história, parte 5.4). **"outras"** [Proposta] é dividido por igual entre as raças que a linha não cita e que a lei do reino não caça; nunca Rudeanos nem Fungoides de colônia.
- **`racas_do_povo`:** as raças que a lei "As outras raças" **não** atinge.
- **`lei_das_outras_racas`:** a opção para quem não é do povo.
- **`lei_por_marca`:** exceções que valem **mais** do que as duas regras acima. Além das marcas da raça, existem duas marcas do NPC: **"com_o_dom"** e **"sem_o_dom"**. É assim que a Federação caça até o humano que nasce mago (HI13-7) e que a União rebaixa quem não tem magia.

| Reino (cultura) | A população (história, 5.4) | O povo | Os outros | Exceções por marca |
|---|---|---|---|---|
| Império Mágico (Autoritária) | Mestiços 40%, Humanos 30%, Forjados 10%, outras 18%, Rudeanos 2% (presos) | Mestiços | Escravizáveis | Nenhuma |
| União Mágica (Arcana) | Humanos 60%, Forjados 10%, outras 28%, Rudeanos 2% (presos) | Qualquer raça, se tiver o dom | (não se aplica) | "sem_o_dom": segunda classe; "mestica": caçadas |
| Federação Humana (Tecnocrata) | Humanos 80%, outras 20% | Humanos | Escravizáveis | "magica" e "com_o_dom": caçadas |
| Clero de Rudeus (Religiosa) | Rudeanos 10%, Humanos 50%, outras 40% | Todas | Iguais | Nenhuma (mas só Rudeanos entram no conselho) |
| Reino Bárbaro (Guerreira) | Cinzentos 30%, Peludos 30%, Humanos 20%, outras 20% | Todas | Iguais (vale a lei do mais forte, não a raça) | Nenhuma |

**Na União,** 30% dos Humanos e 10% das outras raças têm o dom (história, 5.4): no primeiro dia, esses números trocam a regra de nascer Mago da Magia. **Os Rudeanos presos** do Império e da União são escravos de laboratório: não trabalham e não andam pela cidade [Refinar depois, com a ficha de Segredos]. **Quantos de cada raça começam escravos** em cada reino sai com o marco, numa rodada da [[Ficha Logica/Escravidao|Escravidão]].

### O NPC ganha

| Campo | O que é |
|---|---|
| `raca` | O id da raça ("cinzento"). Até o marco, todos "humano" |
| `temDomMagico` | Sorteado ao nascer pela regra da [[Ficha Logica/Magia\|Magia]] (4.1), se a raça deixa; 5 vezes maior na Maré |
| `temMentePropria` | Só nos Fungoides: o raro que não está preso à mente coletiva. Só ele vive numa cidade ou vira companheiro |

### Estado (vai no save)

- A raça, o dom e a mente própria de cada NPC.
- As Marés de Mana ativas (a região e a estação em que acaba).
- Cada Câmara de Forja (de pé ou destruída; os dias até o próximo Forjado).
- Cada colônia Fungoide (o mapa, a população, se é hostil).

## 3. Quando roda

| Quando | O quê |
|---|---|
| Ao criar o NPC | A raça (pela cultura), os pontos, a Personalidade e o dom (4.1) |
| No dia 1 de cada estação | A idade de velho da raça na velhice; os filhos (só da mesma raça); a Maré nova (4.4); as colônias (4.6) |
| A cada dia | As Câmaras de Forja (4.5) |
| Na revista dos guardas e no imposto | A lei "As outras raças" (4.3) |

## 4. Regras passo a passo

### 4.1 Criar o NPC

1. **A raça:** sorteada pela tabela `racas_da_populacao` da cultura dele. O filho é **sempre da raça dos pais** (RA1-5: só a mesma raça tem filhos).
2. **As habilidades:** os pontos da raça somam com os da cultura; o total fica entre 0 e 100.
3. **A Personalidade:** os pontos da raça somam ao sorteio (de 20 a 80), entre 0 e 100; os traços fixos (Forjado) trocam o sorteio.
4. **O dom mágico (rodada RV1-3):** vale a regra 4.1 da [[Ficha Logica/Magia|Magia]]. A raça só diz se ela se aplica: "normal" usa a regra; "nunca", nunca nasce com o dom; "sempre", todos nascem. Numa região com Maré de Mana, a chance base é multiplicada por 5.

### 4.2 A vida e os filhos

1. **A velhice:** a regra da [[Ficha Logica/Populacao|População]] com o `velho` da raça (Cinzento a partir de 24; Peludo, de 12; Rudeano, de 400).
2. **Os filhos (RA1-5):** só um casal da mesma raça; a chance da [[Ficha Logica/Companheiros|família]] (10% por estação) × `fertilidade_em_porcento` (Rudeanos: 2%, ou 0,2% por estação; Peludos: 200%, ou 20%). A mulher é fértil da idade de adulto da raça até o velho dela − 1 [Proposta]. Raças diferentes podem casar, mas não têm filhos. **Um Mestiço só nasce de dois Mestiços:** a mistura original foi feita em laboratório.
3. **Os Forjados** não têm filhos: só nascem na Câmara (4.5). **Os Fungoides** só nascem da colônia (4.6).

### 4.3 A lei "As outras raças" (RA1-4)

Uma lei da [[Ficha Logica/Faccao|Facção]], com 4 opções. Para cada NPC, a ordem é:

1. Se ele tem uma marca que está em `lei_por_marca`, vale a opção dela (a pior, se tiver mais de uma).
2. Senão, se a raça dele está em `racas_do_povo`, nada acontece.
3. Senão, vale a `lei_das_outras_racas` do reino.

| Opção | O que acontece com ele |
|---|---|
| Iguais | Nada |
| Segunda classe | Felicidade −10; paga o dobro de imposto |
| Escravizáveis | Felicidade −25; pode ser capturado como escravo ([[Ficha Logica/Escravidao\|Escravidão]]) |
| Caçadas | Felicidade −40; a revista dos guardas o prende ([[Ficha Logica/Crime\|Crime]]); o NPC livre foge da cidade |

As culturas desejam a opção do começo (peso 4, como as outras leis do `culturas.json`); a assembleia pode mudá-la.

**A criança com o dom na Federação (HI13-7):** quem nasce com o dom ganha a marca "com_o_dom" e passa a ser caçado. [Proposta] Os pais podem **esconder** a criança: ela só é presa se a revista dos guardas a pegar ou se alguém a denunciar. É um segredo para a ficha D9.

### 4.4 As Marés de Mana (RA1-7, mantidas pela HI13-9)

1. Cerca de **1 Maré por ano** no mundo: no dia 1 de cada estação, 25% de chance de começar uma, numa região sorteada; dura 1 estação.
2. Na região: os magos ficam mais fortes ([[Ficha Logica/Magia|Magia]]); quem nasce ali tem o dom mágico 5 vezes mais; os animais podem virar monstros (Fauna e Monstros).
3. Não cria raças novas: a Transformação (do Ano 0 ao 40) já passou. As Marés são o que resta dela.
4. Notícia **Regional:** "Maré de Mana na região X". A União manda magos para lá; a Federação fecha a fronteira com ela ([Refinar depois] com a Diplomacia).

### 4.5 A Câmara de Forja (era a Câmara de Moldagem, RA1-8)

1. O **Império Mágico** e a **União Mágica** têm, cada um, uma Câmara de Forja na capital [Proposta]: é a herança dos laboratórios do Antigo Império. Não é segredo.
2. A cada **10 dias**, cada Câmara transforma 1 escravo humano do reino em 1 Forjado (com a [[Ficha Logica/Escravidao|Escravidão]]); sem escravo humano, para.
3. O jogador pode destruir uma Câmara. Destruída, aquele reino deixa de ganhar Forjados e, com a vida de 10 anos, perde os que tem em cerca de 10 anos de jogo: sem a elite pesada, o exército dele enfraquece.

### 4.6 As colônias Fungoides (eram as tribos Quitinas, RA1-9)

1. Cada colônia é um **lugar sem facção** na Grande Floresta de Esporos (o mapa novo), como uma cidade pequena.
2. **A população** é um número, como a das [[Ficha Logica/OutrasCidades|outras cidades]]: cresce rápido, mas para na comida da região. Só **3 guardiões** são NPCs Importantes (pelo desempenho).
3. **A mente coletiva:** a colônia age como um só. Sorteia-se ao nascer se ela é hostil (50%); a hostil ataca as caravanas como os bandidos. Por isso quase todos os reinos tratam os Fungoides como monstros.
4. **A mente própria:** 1 em cada 100 Fungoides nasce com intelecto próprio [Proposta]. Ele pode sair da colônia, morar numa cidade e ser companheiro do jogador.
5. **A colônia nova:** cerca de 1 a cada 5 anos, num mapa vizinho da floresta (notícia Regional). **A colônia acaba** se a população chegar a zero.

### 4.7 O jogador (RA1-6)

A raça do jogador **depende do início escolhido** (decisão do autor). [Proposta] Cada estilo do [[Ficha Logica/Inicio|Início de Jogo]] diz as raças que aceita:

| Estilo | Raças |
|---|---|
| Dono de vila, Mercador, Nobre exilado, Artesão | Humano |
| Aventureiro | Humano, Cinzento ou Humano-Peludo |
| Membro de facção | Uma raça do povo do reino (Império: Mestiço; União: quem tem o dom; Federação: Humano) |
| Escravo | Humano ou Humano-Peludo |
| Mago | Humano com o dom ou Mestiço |
| Rudeano, Forjado, Fungoide | Nunca no começo (um início próprio para o Fungoide de mente própria [Refinar depois]) |

## 5. Contas

| Conta | Valor |
|---|---|
| Humano com Corpo a Corpo 40 da cultura Guerreira (+5) vs. Cinzento da mesma cultura | 45 vs. 65 |
| Cinzento na Carpintaria, com habilidade igual à de um Humano | Produz 70% |
| Filhos de um casal Rudeano | 10% × 2% = 0,2% por estação: 1 filho a cada 500 estações (125 anos de jogo) |
| Filhos de um casal Peludo | 10% × 200% = 20% por estação: cerca de 1 filho a cada 5 estações, em 9 anos férteis (dos 2 aos 11) |
| Forjados de uma Câmara em 10 anos de jogo (40 estações de 16 dias, 1 a cada 10 dias) | Cerca de **64** (se houver escravos humanos) |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| NPC e População | O campo da raça; a velhice da raça; os filhos só da mesma raça |
| Cultura | `racas_da_populacao`, `racas_do_povo`, `lei_por_marca`; os pontos somam |
| Facção | A lei "As outras raças"; o líder tem raça (o do Império é Mestiço; o do Clero, Rudeano) |
| Companheiros | O recrutável tem a raça da Taverna; o Fungoide de mente própria como companheiro |
| Combate | Os danos recebidos dos Cinzentos, dos Fungoides e dos Peludos |
| Produção | Os 70% dos Cinzentos |
| Crime | A revista prende quem a lei caça |
| Escravidão | Os Forjados; a captura das raças escravizáveis |
| Magia | O dom mágico; as Marés deixam os magos mais fortes |
| Segredos (D9) | A criança com o dom escondida; os Rudeanos presos |
| Fauna e Monstros (nova) | Os animais que a Maré transforma; os monstros que não atacam os Peludos |
| Arte | Um manequim por raça ([[Pipeline de Arte e Conteúdo/Guia da Arte\|Guia da Arte]]) |
| Save | Tudo da parte 2 |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `DefinicaoDaRaca`, `CatalogoDasRacas` | O `racas.json` |
| `sortearARacaDoNpc(cultura)` | Regra 4.1, passo 1 |
| `idadeDeVelhoDaRaca(raca)` | Regra 4.2, passo 1 |
| `podemTerFilhos(umaRaca, outraRaca)` | Regra 4.2, passo 2 |
| `opcaoDaLeiParaONpc(faccao, npc)` | Regra 4.3 |
| `rodarAsMaresDeMana()` | Regra 4.4 |
| `rodarAsCamarasDeForja()` | Regra 4.5 |
| `rodarAsColoniasFungoides()` | Regra 4.6 |

## 8. Testes

1. **As idades:** um Cinzento fica velho aos 24; um Peludo, aos 12; um Rudeano, aos 400.
2. **Os filhos:** um casal Humano e Cinzento nunca tem filho; um casal Rudeano tem cerca de 1/50 da chance dos Humanos, e um casal Peludo, o dobro; nenhum Cinzento, Forjado ou Fungoide nasce com o dom; todo Mestiço e todo Rudeano nasce.
3. **A cultura:** em 1.000 NPCs do Império, cerca de 400 Mestiços, 300 Humanos e 100 Forjados; no Reino Bárbaro, cerca de 300 Cinzentos e 300 Peludos.
4. **A lei:** um Mestiço na Federação e na União é preso na revista; um Humano com o dom é preso na Federação e é do povo na União; um Humano sem o dom paga o dobro de imposto na União; um Cinzento no Reino Bárbaro e no Clero, nada.
5. **A ordem da lei:** a exceção por marca vale mais do que a raça do povo (o Humano com o dom na Federação).
6. **A Câmara:** com escravos humanos, 1 Forjado a cada 10 dias; destruída, nenhum.
7. **A colônia:** com a população em zero, acaba; a população para na comida; cerca de 1 em 100 nasce com mente própria.
8. **O teste de 1 ano não muda** antes do marco (todos humanos).

## 9. Fora desta versão

- Os monstros (Fauna e Monstros, com o mapa novo).
- O lugar das colônias e da Floresta de Esporos (o mapa novo).
- Quantos de cada raça começam escravos em cada reino (rodada da Escravidão, com o marco).
- Os Rudeanos presos como parte das Grandes Histórias (D10).
- O início próprio do Fungoide de mente própria.

## 10. Simulação de mesa

### 10.1 Um companheiro Cinzento em 3 reinos

O jogador recruta Bjorn, um Cinzento de Combate, numa cidade do Clero (a lei: iguais). Corpo a Corpo 45 + 20 = **65**.

| Onde | O que acontece |
|---|---|
| Clero de Rudeus | Nada; ele luta no grupo com 65 e −20% de dano recebido no corpo a corpo |
| Império Mágico | A lei é "escravizáveis": a Felicidade dele cai 25 enquanto o grupo fica, e ele pode ser capturado como escravo |
| Federação Humana | Ele não tem magia, então a lei também é "escravizáveis". Se o companheiro fosse um Mestiço, a revista o prenderia: o jogador o deixaria do lado de fora (a ação "Deixar aqui" dos Companheiros) |

### 10.2 Os Rudeanos em 50 anos

O Clero começa com cerca de 30 Rudeanos entre 300 NPCs (10%). Com 0,2% de filho por estação e por casal, mesmo que formem 15 casais, nascem cerca de **6 Rudeanos em 50 anos** (15 × 200 estações × 0,2%); nenhum morre de velho. **O conselho quase não muda**, e cada Rudeano morto é uma perda que o Clero não repõe. Isso combina com a história: foi pelo número que eles perderam a Morte dos Deuses.

### 10.3 Verificações

| Pergunta | Resultado |
|---|---|
| Uma raça nova precisa de código novo? | **Não**, se usar as reproduções e as marcas que já existem |
| A população Fungoide pesa no desempenho? | **Não:** é um número, com 3 Importantes por colônia |
| Os Forjados somem se a Câmara cair? | **Sim**, em cerca de 10 anos de jogo, e só naquele reino |
| O teste de 1 ano muda antes do marco? | **Não** |

### 10.4 Casos extremos

| Caso | O que acontece |
|---|---|
| O jogador casa com alguém de outra raça | Casa, mas não tem filhos; o sucessor sai dos companheiros (Companheiros 4.8) |
| O filho do jogador nasce com o dom numa vila da Federação | Ganha a marca "com_o_dom": o jogador esconde a criança, foge do reino ou a entrega |
| A Maré começa na região da vila do jogador | Os nascidos têm mais chance do dom; os animais podem virar monstros |
| O Clero muda a lei para "caçadas" | As raças que não são do povo ficam com Felicidade −40 e fogem |

### 10.5 O que muda em outras fichas

- **Facção:** a lei com exceções por marca; a raça do líder.
- **Cultura:** os 3 campos novos por cultura.
- **Magia:** os 30% e 10% de magos da União no primeiro dia.
- **Escravidão:** a Câmara de Forja; os Forjados como escravos militares.
- **Companheiros:** a Vida prolongada passa ao líder da União (história, parte 5.8).
