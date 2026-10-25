---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase B5 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com TE1 vêm da **rodada TE1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Tecnologia

## 1. O que faz

Duas camadas:

1. **Marcos mundiais:** chegam para o mundo todo numa data sorteada dentro de uma janela. A data fica mais cedo quando as facções pesquisam as tecnologias ligadas ao marco. Os marcos maiores abrem a era seguinte.
2. **Tecnologia de facção:** cada facção junta **Pontos de Pesquisa** nos seus Laboratórios e completa uma tecnologia por vez. Cada tecnologia **libera** construções, receitas, itens, leis ou níveis de cidade.

Documento do sistema: [[Sistema de Tecnologia/Descrição|Tecnologia e Pesquisa]].

**Esta versão:** as duas camadas, o Laboratório como construção de produção, a difusão (−10% por facção que já tem), a IA que escolhe pela cultura, a vila do jogador sem facção e as travas (era e tecnologia) na Produção e na Construção. **As listas de tecnologias e marcos são conteúdo** (rodada de conteúdo C1); a ficha usa listas de exemplo.

## 2. Dados

### `data/tecnologias.json` (arquivo novo, conteúdo)

```json
{ "id": "tec.rotas_de_caravana", "ramo": "economia", "era": 1, "custo": 100,
  "pre_requisitos": ["tec.mercado_organizado"],
  "libera": [ { "tipo": "construcao", "id": "construcao.posto_comercial" } ],
  "culturas_que_comecam_com": ["escravista"] }
```

### `data/marcos.json` (arquivo novo, conteúdo)

```json
{ "id": "marco.era_2", "janela_em_anos": [3, 5], "tecnologias_ligadas": ["tec.a", "tec.b", "tec.c"],
  "abre_era": 2, "libera": [] }
```

### Definições (`data/regras.json`, bloco novo `"tecnologia"`)

```json
"tecnologia": {
  "custo_por_era": [100, 200, 400, 800],
  "difusao_por_faccao_em_porcento": 10,
  "difusao_maxima_em_porcento": 50,
  "vila_sem_faccao_custo_em_porcento": 150,
  "prioridade_dos_ramos_por_cultura": {
    "guerreira":   ["militar", "construcao", "producao", "administracao", "economia"],
    "arcana":      ["administracao", "producao", "construcao", "economia", "militar"],
    "autoritaria": ["administracao", "militar", "producao", "construcao", "economia"],
    "religiosa":   ["administracao", "producao", "construcao", "economia", "militar"],
    "tecnocrata":  ["producao", "construcao", "economia", "administracao", "militar"],
    "escravista":  ["economia", "producao", "militar", "administracao", "construcao"]
  }
}
```

O `custo` de cada tecnologia no JSON pode ser escrito; se faltar, vale o `custo_por_era`.

### O Laboratório (Construção e Receitas)

- Construção **grande**, de produção, da Cidade (até a Facção).
- **Uma receita:** "Pontos de Pesquisa", Trabalho **Especialista**, 8 horas, lote 1, sem insumo (TE1-2). O produto **não vai ao inventário**: vai para os pontos da facção.
- A velocidade é a da Produção: 50 + a habilidade Especialista do empregado (a Inteligência do documento entra quando o NPC tiver o atributo).

### Estado (vai no save)

| Campo | Exemplo |
|---|---|
| `tecnologiasPorFaccao` | Escravista: {rotas_de_caravana, ...} |
| `projetoAtualPorFaccao`, `pontosAcumuladosPorFaccao` | Escravista: tec.moeda_de_prata, 64 |
| `eraAtualDoMundo` | 1 |
| `diaSorteadoPorMarco`, `marcosQueChegaram` | marco.era_2: dia 280; nenhum |
| O mesmo para a **vila do jogador** (como uma "facção" a mais) | — |

**O sorteio dos marcos:** uma vez, ao criar o mundo, com o PCG32 da **sequência 6**.

## 3. Quando roda

| Quando | O quê |
|---|---|
| A cada hora | O Laboratório produz como qualquer construção (Produção, 4.2; em LOD 2, com o trabalho presumido) |
| **A cada dia, passo 7b (novo)**, depois da escolha das receitas | Soma os pontos, completa tecnologias, escolhe o próximo projeto, confere os marcos (4.2 a 4.5) |
| Ao criar o mundo | Sorteia o dia de cada marco; dá as tecnologias iniciais de cada cultura |

## 4. Regras passo a passo

### 4.1 Quem pesquisa

- Cada **facção** soma os pontos de todos os Laboratórios das suas cidades num projeto só.
- A **vila do jogador** (sem facção) pesquisa a própria árvore se tiver um Laboratório, com o custo **× 150%**. As Vilas Livres não têm Laboratório: só recebem os marcos.
- **Onde há Laboratório no começo (TE1-1):** em **todas as capitais** (o pacote do nível 4 ganha o Laboratório) e, na Tecnocrata, também na Cidade (3,6), no lugar do Templo. O documento só punha o Laboratório na Tecnocrata; sem a mudança, as outras 5 facções nunca pesquisariam.

### 4.2 Somar os pontos (passo 7b)

1. Os ciclos completos dos Laboratórios no dia viram pontos (1 por ciclo).
2. Somam no `pontosAcumuladosPorFaccao`.
3. **Custo do projeto** = custo da tecnologia × (100 − 10 × facções que já a têm, até −50) ÷ 100, arredondado para baixo; × 150% se for a vila do jogador.
4. Se os pontos chegam ao custo: a tecnologia é da facção, os pontos descontam o custo (a sobra fica), e nasce a notícia **Regional** "A Tecnocrata descobriu X" (Notícias). Escolhe o próximo projeto (4.3) e confere de novo (no máximo 3 tecnologias por dia).

### 4.3 Escolher o projeto

**IA:** entre as tecnologias **disponíveis** (pré-requisitos completos e era ≤ a era do mundo), a do ramo de **maior prioridade** da cultura; empate: a de **menor custo** atual; empate: a primeira do arquivo. Sem sorteio.

**Jogador** (na vila dele, com Laboratório): escolhe na tela da Tecnologia; se não escolher, a regra da IA com a ordem da Tecnocrata.

### 4.4 Os marcos mundiais

1. **Ao criar o mundo:** para cada marco, sorteia um dia dentro da janela: do primeiro dia do ano inicial ao último dia do ano final (anos de 64 dias, contando o ano 1 como o primeiro da partida).
2. **Todo dia (passo 7b):** a data do marco = dia sorteado − (dia sorteado − primeiro dia da janela) × facções que têm **todas** as tecnologias ligadas ÷ 6, arredondado para baixo. Nunca antes do primeiro dia da janela.
3. Se hoje ≥ a data do marco e ele não chegou: chega. Se abre uma era, `eraAtualDoMundo` sobe. O que ele libera vale para todos (as facções, a vila do jogador, as Vilas Livres). Notícia **Mundial** (Notícias).

### 4.5 As travas

1. **Uma receita, uma construção ou um nível de cidade** que aparece em algum `libera` só pode ser usado por quem tem a tecnologia ou o marco. O que **não aparece** em nenhum `libera` é livre desde o começo (assim, tudo o que já existe na vila continua livre).
2. **A era:** o Rank máximo das armas e equipamentos (3, 4, 4, 5 pelas eras), quando o Combate chegar.
3. A Produção (4.3, as candidatas) e a Construção (a lista do que se pode construir) olham essas travas para a cidade, pela facção dona.
4. A **rota fixa** do Comércio vem de um **marco menor** de Economia (TE1-4), para o jogador sem facção não ficar sem ela.

### 4.6 O começo

Cada facção começa com as tecnologias de `culturas_que_comecam_com` da sua cultura (2 ou 3 da era 1). A vila do jogador começa sem nenhuma.

## 5. Contas

**Pontos por dia num Laboratório nível 1 com 6 Especialistas de habilidade 40 (velocidade 90%), em LOD 2:**

- por hora: 6 × 784 × 90 = 423.360; um ciclo = 8 × 800 × 100 = 640.000;
- por dia (8 horas): 3.386.880 → **5 pontos** (sobra 186.880).

| Facção | Laboratórios | Pontos por ano (64 dias) | Tecnologias da era 1 por ano (custo 100) |
|---|---|---|---|
| As 5 com 1 Laboratório | 1 | cerca de 340 | cerca de **3,4** |
| Tecnocrata | 2 | cerca de 680 | cerca de **6,8** |

**O ritmo das eras** (com cerca de 15 tecnologias por era, das 40 a 60 do documento): uma facção comum completa a era 1 (15 − 2 iniciais = 13 × 100, menos a difusão) em cerca de **3 a 4 anos**; a janela do marco da era 2 vai do ano 3 ao 5. **Bate.**

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Produção | O Laboratório é uma construção de produção; as travas nas candidatas (4.5) |
| Construção | O Laboratório (grande); as travas na lista de construções |
| Outras cidades | O pacote do nível 4 ganha o Laboratório; a cidade usa a tecnologia da facção dona |
| Cidade | Um nível de cidade pode pedir uma tecnologia |
| Comércio | O Posto Comercial e a rota fixa podem vir da tecnologia e de um marco |
| Notícias | Tecnologia de facção: Regional; marco: Mundial |
| Save | Tecnologias, projetos, pontos, marcos; o formato passa para a **versão 9** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `tecnologiasPorFaccao`, `pontosAcumuladosPorFaccao`, `projetoAtualPorFaccao` | Parte 2 |
| `custoAtualDaTecnologia(faccao, tecnologia)` | Regra 4.2, com a difusão |
| `somarPontosEConcluirTecnologias()` | Regra 4.2, passo 7b |
| `escolherProjetoDaFaccao(faccao)` | Regra 4.3 |
| `sortearDiasDosMarcos()` / `dataAtualDoMarco(marco)` | Regra 4.4 |
| `estaLiberado(dono, tipo, id)` | Regra 4.5 |

## 8. Testes

1. **Os 5 pontos por dia** do Laboratório da parte 5.
2. **Difusão:** custo 100; com 1 facção que já tem, 90; com 5, 50; nunca menos de 50.
3. **A vila do jogador:** custo 100 com 0 facções vira 150.
4. **O marco adianta:** dia sorteado 280, janela a partir do dia 129, 3 facções com todas as ligadas: dia **204**. Com as 6: dia 129.
5. **Nada fora da janela:** o marco nunca chega antes do primeiro dia nem depois do último.
6. **Trava:** uma receita no `libera` de uma tecnologia não é candidata para a cidade sem ela; uma receita fora de todo `libera` é.
7. **Validação:** uma tecnologia que libera uma construção que não existe dá erro ao carregar.
8. **O teste de 1 ano não muda:** a vila não tem Laboratório, e nada do que ela usa está num `libera`.

## 9. Fora desta versão

- O Banco da Facção pagando os Laboratórios, o imposto para pesquisa, a Universidade (bônus), o Nobre gerente.
- A conquista, a compra e o roubo de tecnologia (Guerra, Diplomacia, Crime).
- O jogador como líder de facção.
- A Inteligência do NPC (usa a habilidade Especialista).
- A Magia e os Pontos Arcanos (fase D).
- O Mercado Mundial.
- As listas de verdade (rodada de conteúdo C1).

## 10. Simulação de mesa

### 10.1 Cenário

- Era 1, 3 tecnologias de exemplo, todas de custo 100: **A** (economia), **B** (produção), **C** (construção, pré-requisito B).
- O marco da era 2 tem as ligadas {A, B, C}, janela dos anos 3 a 5 (dias 129 a 320), dia sorteado **280**.
- A **Tecnocrata** faz 10 pontos por dia (2 Laboratórios); a **Escravista**, 5. Nenhuma começa com A, B ou C.

### 10.2 A pesquisa, dia a dia (em blocos)

| Dia | Tecnocrata (ordem: produção, construção, economia) | Escravista (ordem: economia, produção, ...) | Data do marco |
|---|---|---|---|
| 1 | Projeto B (produção), custo 100 | Projeto A (economia), custo 100 | 280 |
| 10 | **B completa** (100). Notícia Regional. Projeto C (construção), custo 100 | 50 de 100 | 280 |
| 20 | **C completa**. Projeto A: a Escravista não tem ainda: custo 100 | **A completa** (100). Projeto B: a Tecnocrata tem: custo **90** | 280 |
| 30 | **A completa** (a Escravista já tinha: custo 90; dia 29). A Tecnocrata tem A, B e C | 50 de 90 | 280 − 151 × 1 ÷ 6 = 254,8 → **254** |
| 38 | (outras) | **B completa** (dia 38). Projeto C: custo 90 | 254 |
| 56 | | **C completa**. A Escravista tem A, B e C | 280 − 151 × 2 ÷ 6 = 229,7 → **229** |

Com 2 facções completas, o marco da era 2 chega no dia **229** (ano 4), em vez do 280. Se as 6 completarem antes do dia 129, ele chega no dia 129, o primeiro da janela.

### 10.3 Verificações

| Pergunta | Resultado |
|---|---|
| As 6 facções pesquisam? | **Só com a TE1-1.** **Achado:** pelo pacote do documento, só a Tecnocrata tem Laboratório (no lugar do Templo); as outras 5 ficariam paradas para sempre e só receberiam os marcos |
| O ritmo bate com a janela? | **OK:** cerca de 3,4 tecnologias por ano numa facção comum; a era 1 acaba em 3 a 4 anos, e a janela da era 2 é do ano 3 ao 5 (parte 5) |
| O marco sai da janela? | **Não:** a fórmula nunca passa do primeiro dia, e o dia sorteado já está dentro |
| A difusão favorece quem chega depois? | **Sim, até 50%:** a Escravista pagou 90 por B e C. É o "conhecimento se espalhando" |
| O teste de 1 ano muda? | **Não:** sem Laboratório na vila e sem trava no que ela usa. A sequência 6 é nova e só é usada na criação do mundo |
| O jogador sem facção fica sem a rota fixa? | **Não com a TE1-4:** ela vem de um marco menor de Economia. **Achado:** a vila nível 1 não tem lote grande para o Laboratório; sem o marco, o jogador sem facção nunca teria a rota fixa |

### 10.4 Casos extremos

| Caso | O que acontece |
|---|---|
| Uma facção sem Laboratório (perdeu a capital, fase D) | Não soma pontos; o projeto fica parado; recebe os marcos |
| Nenhuma tecnologia disponível (era completa) | Sem projeto; os pontos ficam guardados até o marco abrir a era seguinte |
| Duas tecnologias completam no mesmo dia (sobra grande) | No máximo 3 por dia; a sobra fica |
| O marco chega quando a facção está no meio de um projeto da era antiga | Nada muda; a era nova só aumenta as disponíveis |
| A tecnologia fica mais barata (difusão) no meio do projeto | O custo é recalculado todo dia; se os pontos já passam do novo custo, completa no passo 7b |
| Save no meio do ano | Os dias sorteados dos marcos estão no save; nada é sorteado de novo |

### 10.5 Decidido em modo automático (rodada TE1, seguir sugestão)

1. **TE1-1:** todas as capitais têm Laboratório no pacote do nível 4; a Tecnocrata também na Cidade, no lugar do Templo.
2. **TE1-2:** o Laboratório é uma construção de produção com a receita "Pontos de Pesquisa" (Especialista, 8 horas, lote 1, sem insumo); a velocidade usa a habilidade Especialista até o NPC ter a Inteligência.
3. **TE1-3:** custo por era 100, 200, 400 e 800; a vila do jogador paga 150%.
4. **TE1-4:** a rota fixa do Comércio vem de um marco menor de Economia, para todos.
5. **TE1-5:** os pontos contam todo dia (o documento dizia "por estação"); a pesquisa usa a mesma produção por hora das Outras cidades.
6. **TE1-6:** a data do marco desce na proporção das facções que têm todas as ligadas (÷ 6), nunca antes do primeiro dia da janela.
7. **TE1-7:** o que não aparece em nenhum `libera` é livre desde o começo.

### 10.8 Ajustes do código (marco B5, 2026-10-24; rodada TE2, opção A)

1. **TE2-1 (provisório):** até os Ricos chegarem (marco A4), a habilidade Especialista de um NPC é a Manufaturado dele.
2. **O Laboratório é uma construção sem receita** (`"trabalho_sem_receita": "especialista"` e `"pontos_de_pesquisa": true` no `construcoes.json`). O trabalho de cada hora soma como numa receita de 8 horas, lote 1, e cada ciclo completo vira 1 ponto. O ponto não é um item: assim ele não aparece nas lojas, no inventário nem no relatório.
3. **O Laboratório começa no nível 3** (5 vagas; a tabela `vagas_por_nivel` não tem 6). Dá cerca de 4,5 pontos por dia. Ele é o primeiro da fila da contratação: nas capitais Guerreira e Tecnocrata não há desempregados no começo, e ele ficaria vazio.
4. **As listas são provisórias** (até a rodada de conteúdo C1): as 3 tecnologias da parte 10 (Contabilidade, Ferramentas melhores e Andaimes) e o marco "A era das guildas" (era 2, anos 3 a 5). Nenhuma libera nada.
5. **As travas** entram nas receitas das cidades (a Produção não escolhe uma receita travada). A lista de construções da vila e os níveis de cidade ganham a trava quando alguma coisa desse tipo estiver num `libera`.
6. **A escolha do jogador** na tela da Tecnologia fica para quando a vila puder ter um Laboratório (o lote grande). Até lá, a janela **Tecnologia** só mostra o que cada facção pesquisa e os marcos.
7. **A notícia do marco** nasce no mapa do grupo do jogador, para chegar a todos na hora.
8. **Os dias contam do 0 no código:** a janela dos anos 3 a 5 vai do dia 128 ao 319 (129 a 320 na ficha).

