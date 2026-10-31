---
tags: [ficha-logica]
---

> O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-31 (fase D8 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **rodada RA1** (as respostas do autor; quase todas "seguir a sugestão"). A história das raças está no [[Documentos/Mundo e História/Descrição|Mundo e História]], partes 4.4 e 7.1. **O marco vem depois dos marcos da fase D** (HI3-2); até lá, o jogo tem só humanos.

# Ficha Lógica: Raças

## 1. O que faz

Dá a cada NPC uma **raça**, ao lado da cultura. A raça decide:

1. **A idade de adulto e de velho** (decisão do autor, 2026-10-26).
2. **As habilidades, a Personalidade, uma vantagem e um defeito** (RA1-2).
3. **Como ela tem filhos:** só com a mesma raça (RA1-5).
4. **Como cada povo a trata:** a lei nova "As outras raças" (RA1-4).

Três coisas do mundo nascem desta ficha: as **Marés de Mana** (RA1-7), a **Câmara de Moldagem** dos Khadim (RA1-8) e as **tribos Quitinas** (RA1-9). Os monstros ficam numa ficha própria, **Fauna e Monstros** (RA1-10).

**O sistema precisa aceitar raças novas com facilidade** (decisão do autor, 2026-10-31). Por isso, cada raça é uma entrada no `racas.json`, e as leis e as culturas reagem às **marcas** da raça ("mutante", "mágica", "monstruosa", "artificial", "única"), não ao nome dela. Uma raça nova com marcas conhecidas já funciona sem regra nova.

## 2. Dados

### `data/racas.json` (arquivo novo)

```json
{
  "racas": [
    { "id": "humano", "nome": "Humanos", "adulto": 5, "velho": 31, "reproducao": "normal", "fertilidade_em_porcento": 100,
      "habilidades_em_pontos": {}, "personalidade_em_pontos": {}, "dom_magico_em_por_mil": 5, "marcas": [] },
    { "id": "langren", "nome": "Langren", "adulto": 4, "velho": 24, "reproducao": "normal", "fertilidade_em_porcento": 100,
      "habilidades_em_pontos": { "corpo_a_corpo": 20, "oficio": -15, "comercio": -15 }, "personalidade_em_pontos": { "agressividade": 30 },
      "dano_recebido_no_corpo_a_corpo_em_porcento": 80, "producao_em_porcento": 70, "dom_magico_em_por_mil": 0, "marcas": ["mutante"] },
    { "id": "khadim", "nome": "Khadim", "adulto": 0, "velho": 10, "reproducao": "fabricada",
      "habilidades_em_pontos": { "bracal": 15 }, "personalidade_fixa": { "lealdade": 100, "coragem": 20 },
      "sobe_de_habilidade": false, "pode_liderar": false, "faz_greve_e_foge": false, "dom_magico_em_por_mil": 0, "marcas": ["artificial"] },
    { "id": "ascendido", "nome": "Ascendidos", "adulto": 10, "velho": 120, "reproducao": "normal", "fertilidade_em_porcento": 5,
      "habilidades_em_pontos": { "especialista": 15 }, "personalidade_em_pontos": { "sociabilidade": -20 },
      "dom_magico_em_por_mil": 1000, "marcas": ["mutante", "magica"] },
    { "id": "quitino", "nome": "Quitinos", "adulto": 2, "velho": 15, "velho_da_rainha": 40, "reproducao": "rainha",
      "habilidades_em_pontos": { "bracal": 10 }, "personalidade_em_pontos": { "coragem": 20 },
      "dano_recebido_em_porcento": 70, "come_qualquer_carne": true, "dom_magico_em_por_mil": 0, "marcas": ["mutante", "monstruosa"] },
    { "id": "eterno", "nome": "Eterno", "adulto": 0, "velho": null, "reproducao": "nenhuma", "populacao_maxima": 1,
      "habilidades_fixas": 100, "dom_magico_em_por_mil": 1000, "marcas": ["magica", "unica"] }
  ],
  "lei_das_outras_racas": {
    "opcoes": ["iguais", "segunda_classe", "escravizaveis", "cacadas"],
    "felicidade": [0, -10, -25, -40],
    "imposto_da_segunda_classe_em_porcento": 200
  },
  "mare_de_mana": { "por_ano_no_mundo": 1, "dura_estacoes": 1, "dom_magico_dos_nascidos_vezes": 5 },
  "camara_de_moldagem": { "dias_por_khadim": 10 },
  "tribo_quitina": { "importantes": 3, "anos_por_rainha_nova": 5, "chance_de_ser_hostil_em_porcento": 50 }
}
```

- **`reproducao`:** "normal" (casal da mesma raça), "rainha" (só a rainha põe ovos), "fabricada" (só pela Câmara de Moldagem) ou "nenhuma".
- **`dom_magico_em_por_mil`:** a chance de nascer com o dom de manipular a mana ([[Ficha Logica/Magia|Magia]]). [Proposta] 5 por mil nos Humanos ("raros").
- **`habilidades_fixas`:** o Eterno tem tudo em 100.

### No `culturas.json`, cada cultura ganha

```json
"racas_dos_livres": { "humano": 85, "langren": 10, "quitino": 5 },
"racas_do_povo": ["humano", "langren", "quitino"]
```

- **`racas_dos_livres`:** a porcentagem de cada raça entre os NPCs livres (RA1-3).
- **`racas_do_povo`:** as raças que a lei "As outras raças" **não** atinge. [Proposta] Na Ordem de Custodum, também não atinge quem tem o dom mágico.

| Povo | Livres (RA1-3) | Escravos ([[Ficha Logica/Escravidao\|Escravidão]]) | A lei no começo (RA1-4) |
|---|---|---|---|
| Clãs de Tian-Lang | 100% Langren | Os que eles capturam | Iguais (vale a lei do mais forte, não a raça) |
| Ordem de Custodum | 5% Ascendidos (a elite), 95% Humanos com o dom | Humanos e outras raças, para experimentos | Escravizáveis (todos os sem magia) |
| Sultanato de Zahran | 100% Humanos | Muitos Khadim, também nos exércitos | Escravizáveis |
| Comunidade de Ashmoor | 100% Humanos | Khadim (sem saber da origem) | Caçadas: quem tem a marca "mutante" ou "mágica" |
| Coroa de Valdren | 95% Humanos, 5% Langren | Poucos | Segunda classe (o dobro de imposto) |
| Teocracia de Theodoria | 85% Humanos, 10% Langren, 5% Quitinos | Nenhum | Iguais (todas abençoadas pelo Doador) |

### O NPC ganha

| Campo | O que é |
|---|---|
| `raca` | O id da raça ("langren"). Até o marco, todos "humano" |
| `temDomMagico` | Sorteado ao nascer (a chance da raça; 5 vezes maior na Maré) |
| `ehRainha` | Só nos Quitinos |
| `estaDesperto` | Só nos Khadim: o raro que ganhou consciência (um companheiro, o estilo Escravo) |

### Estado (vai no save)

- A raça, o dom, a rainha e o desperto de cada NPC.
- As Marés de Mana ativas (a região e a estação em que acaba).
- A Câmara de Moldagem (de pé ou destruída; os dias até o próximo Khadim).
- Cada tribo Quitina (o mapa, a população, a rainha, se é hostil).

## 3. Quando roda

| Quando | O quê |
|---|---|
| Ao criar o NPC | A raça (pela cultura), os pontos, a Personalidade e o dom (4.1) |
| No dia 1 de cada estação | A idade de velho da raça na velhice; os filhos (só da mesma raça); a Maré nova (4.4); as tribos (4.6) |
| A cada dia | A Câmara de Moldagem (4.5) |
| Na revista dos guardas e no imposto | A lei "As outras raças" (4.3) |

## 4. Regras passo a passo

### 4.1 Criar o NPC

1. **A raça:** sorteada pela tabela `racas_dos_livres` da cultura dele. O filho é **sempre da raça dos pais** (RA1-5: só a mesma raça tem filhos).
2. **As habilidades:** os pontos da raça somam com os da cultura; o total fica entre 0 e 100.
3. **A Personalidade:** os pontos da raça somam ao sorteio (de 20 a 80), entre 0 e 100; os traços fixos (Khadim) trocam o sorteio.
4. **O dom mágico:** chance da raça em por mil (vezes 5 se nasceu numa região com Maré de Mana).

### 4.2 A vida e os filhos

1. **A velhice:** a regra da [[Ficha Logica/Populacao|População]] com o `velho` da raça (Langren a partir de 24; Ascendido a partir de 120). O Eterno nunca envelhece.
2. **Os filhos (RA1-5):** só um casal da mesma raça; a chance da [[Ficha Logica/Companheiros|família]] (10% por estação) × `fertilidade_em_porcento` (Ascendidos: 0,5%). A mulher é fértil da idade de adulto da raça até o velho dela − 1 [Proposta]. Raças diferentes podem casar, mas não têm filhos.
3. **Os Khadim** não têm filhos: só nascem na Câmara (4.5). **Os Quitinos** só nascem da rainha (4.6).

### 4.3 A lei "As outras raças" (RA1-4)

Uma lei nova da [[Ficha Logica/Faccao|Facção]], com 4 opções; vale para o NPC cuja raça não está nas `racas_do_povo` da cultura da facção (e, em Ashmoor, para quem tem as marcas caçadas).

| Opção | O que acontece com ele |
|---|---|
| Iguais | Nada |
| Segunda classe | Felicidade −10; paga o dobro de imposto |
| Escravizáveis | Felicidade −25; pode ser capturado como escravo ([[Ficha Logica/Escravidao\|Escravidão]]) |
| Caçadas | Felicidade −40; a revista dos guardas o prende ([[Ficha Logica/Crime\|Crime]]); o NPC livre foge da cidade |

As culturas desejam a opção do começo (peso 4, como as outras leis do `culturas.json`); a assembleia pode mudá-la.

**O segredo dos Khadim:** Ashmoor caça as marcas "mutante" e "mágica", e os Khadim têm só "artificial". Se o segredo da origem for revelado (Segredos, D9), [Proposta] os Khadim passam a contar como "mágicos" em Ashmoor.

### 4.4 As Marés de Mana (RA1-7)

1. Cerca de **1 Maré por ano** no mundo: no dia 1 de cada estação, 25% de chance de começar uma, numa região sorteada; dura 1 estação.
2. Na região: os magos ficam mais fortes ([[Ficha Logica/Magia|Magia]]); quem nasce ali tem o dom mágico 5 vezes mais; os animais podem virar monstros (Fauna e Monstros).
3. Não cria raças novas: a Estabilização já passou (por volta do Ano 120 d.F.).
4. Notícia **Regional:** "Maré de Mana na região X". A Ordem manda magos para lá; Ashmoor fecha a fronteira com ela ([Refinar depois] com a Diplomacia).

### 4.5 A Câmara de Moldagem (RA1-8)

1. O Sultanato tem uma Câmara **escondida** na região da Maré original, mantida pela mana da Ordem.
2. A cada **10 dias**, ela transforma 1 escravo humano do Sultanato em 1 Khadim (com a [[Ficha Logica/Escravidao|Escravidão]]); sem escravo humano, para.
3. O jogador pode achá-la (como uma masmorra secreta), roubar as provas (um segredo para a D9) ou destruí-la. Destruída, os Khadim param de nascer e, com a vida de 10 anos, somem do mundo em cerca de 10 anos de jogo.

### 4.6 As tribos Quitinas (RA1-9)

1. Cada tribo é um **lugar sem facção** no mapa novo (na floresta da explosão), como uma cidade pequena.
2. **A população** é um número, como a das [[Ficha Logica/OutrasCidades|outras cidades]]: cresce rápido, mas para na comida da região. Só a **rainha e 2 guerreiros** são NPCs Importantes (pelo desempenho, o mesmo cuidado do limite das dinastias).
3. **Hostil ou neutra:** sorteado ao nascer a tribo (50%). A hostil ataca as caravanas como os bandidos.
4. **A rainha nova:** cerca de 1 a cada 5 anos. Se a rainha velha vive, a nova sai e funda outra tribo num mapa vizinho (notícia Regional); se a rainha morreu, a nova assume. **Sem rainha nem herdeira, a tribo acaba.**

### 4.7 O jogador (RA1-6)

A raça do jogador **depende do início escolhido** (decisão do autor). [Proposta] Cada estilo do [[Ficha Logica/Inicio|Início de Jogo]] diz as raças que aceita:

| Estilo | Raças |
|---|---|
| Dono de vila, Mercador, Nobre exilado, Artesão | Humano |
| Aventureiro | Humano ou Langren |
| Membro de facção | A raça do povo da facção (Clãs: Langren) |
| Escravo | Humano ou **Khadim desperto** (o modo mais difícil) |
| Mago | Humano com o dom ou Ascendido |
| Eterno, Quitino | Nunca no começo (um início próprio para Quitino [Refinar depois]) |

## 5. Contas

| Conta | Valor |
|---|---|
| Humano com Corpo a Corpo 40 da cultura Guerreira (+5) vs. Langren da mesma cultura | 45 vs. 65 |
| Langren na Carpintaria, com habilidade igual à de um Humano | Produz 70% |
| Filhos de um casal Ascendido em 100 estações férteis | 10% × 5% = 0,5% por estação: **cerca de 0,4 filho** |
| Khadim da Câmara em 10 anos de jogo (40 estações de 16 dias, 1 a cada 10 dias) | Cerca de **64** (se houver escravos humanos) |
| Humanos com o dom numa cidade de 300 NPCs | 5 por mil: cerca de **1 ou 2** |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| NPC e População | O campo da raça; a velhice da raça; os filhos só da mesma raça |
| Cultura | `racas_dos_livres`, `racas_do_povo`; os pontos somam |
| Facção | A lei "As outras raças"; o líder tem raça (Tian Feng é Langren; Cassius, Ascendido) |
| Companheiros | O recrutável tem a raça da Taverna; o Khadim desperto como companheiro |
| Combate | Os danos recebidos dos Langren e dos Quitinos |
| Produção | Os 70% dos Langren |
| Crime | A revista prende quem a lei caça |
| Escravidão | Os Khadim; a captura das raças escravizáveis |
| Magia | O dom mágico; as Marés deixam os magos mais fortes |
| Segredos (D9) | A origem dos Khadim; o segundo Eterno |
| Fauna e Monstros (nova) | Os animais que a Maré transforma |
| Arte | Um manequim por raça ([[Pipeline de Arte e Conteúdo/Guia da Arte\|Guia da Arte]]); o Eterno e a rainha Quitina, únicos |
| Save | Tudo da parte 2 |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `DefinicaoDaRaca`, `CatalogoDasRacas` | O `racas.json` |
| `sortearARacaDoNpc(cultura)` | Regra 4.1, passo 1 |
| `idadeDeVelhoDaRaca(raca)` | Regra 4.2, passo 1 |
| `podemTerFilhos(umaRaca, outraRaca)` | Regra 4.2, passo 2 |
| `opcaoDaLeiParaARaca(faccao, npc)` | Regra 4.3 |
| `rodarAsMaresDeMana()` | Regra 4.4 |
| `rodarACamaraDeMoldagem()` | Regra 4.5 |
| `rodarAsTribosQuitinas()` | Regra 4.6 |

## 8. Testes

1. **As idades:** um Langren fica velho aos 24; um Ascendido, aos 120; o Eterno nunca.
2. **Os filhos:** um casal Humano e Langren nunca tem filho; um casal Ascendido tem cerca de 1/20 da chance dos Humanos.
3. **A cultura:** em 1.000 NPCs da Teocracia, cerca de 850 Humanos, 100 Langren e 50 Quitinos; nos Clãs, todos Langren.
4. **A lei:** um Langren em Ashmoor é preso na revista; na Coroa, paga o dobro de imposto; nos Clãs, nada.
5. **O segredo:** antes da revelação, o Khadim não é caçado em Ashmoor.
6. **A Câmara:** com escravos humanos, 1 Khadim a cada 10 dias; destruída, nenhum.
7. **A tribo:** sem rainha nem herdeira, a tribo acaba; a população para na comida.
8. **O teste de 1 ano não muda** antes do marco (todos humanos).

## 9. Fora desta versão

- Os monstros (Fauna e Monstros, com o mapa novo).
- O lugar das tribos, da Câmara e da Maré original (o mapa novo).
- A Grande História dos Khadim despertos (Correntes Quebradas, D10).
- O início próprio de Quitino.

## 10. Simulação de mesa

### 10.1 Um companheiro Langren em 3 povos

O jogador recruta Hao, um Langren de Combate, numa cidade da Teocracia (a lei: iguais). Corpo a Corpo 45 + 20 = **65**.

| Onde | O que acontece |
|---|---|
| Teocracia | Nada; ele luta no grupo com 65 e −20% de dano recebido no corpo a corpo |
| Coroa de Valdren | A lei é "segunda classe": ele não paga imposto (não mora lá), mas a Felicidade dele cai 10 enquanto o grupo fica |
| Comunidade de Ashmoor | A lei é "caçadas": na revista, os guardas o prendem. O jogador deixa Hao do lado de fora (a ação "Deixar aqui" dos Companheiros) |

### 10.2 A Ordem em 50 anos

A Ordem começa com cerca de 15 Ascendidos entre 300 NPCs. Com 0,5% de filho por estação e por casal, nascem cerca de **2 Ascendidos em 50 anos**; nenhum morre de velho (o velho é aos 120). **A elite quase não muda**, e cada Ascendido morto numa guerra é uma perda que a Ordem não repõe. Isso combina com a história ("a raça mais rara").

### 10.3 Verificações

| Pergunta | Resultado |
|---|---|
| Uma raça nova precisa de código novo? | **Não**, se usar as reproduções e as marcas que já existem |
| A população Quitina pesa no desempenho? | **Não:** é um número, com 3 Importantes por tribo |
| Os Khadim somem se a Câmara cair? | **Sim**, em cerca de 10 anos de jogo |
| O teste de 1 ano muda antes do marco? | **Não** |

### 10.4 Casos extremos

| Caso | O que acontece |
|---|---|
| O jogador casa com alguém de outra raça | Casa, mas não tem filhos; o sucessor sai dos companheiros (Companheiros 4.8) |
| A rainha Quitina é companheira do jogador | Não pode: a rainha não sai da tribo |
| A Maré começa na região da vila do jogador | Os nascidos têm mais chance do dom; os animais podem virar monstros |
| A Teocracia muda a lei para "caçadas" | Os Langren e os Quitinos dela ficam com Felicidade −40 e fogem |

### 10.5 O que muda em outras fichas

- **Companheiros:** Cassius tem 432 anos na história (RA1-1, opção B); a Vida prolongada continua.
- **Facção:** a lei nova; a raça do líder.
- **Plano das Fichas:** a ficha nova **Fauna e Monstros**.
