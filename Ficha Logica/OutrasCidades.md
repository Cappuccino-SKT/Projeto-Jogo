---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase B1b do [[Ficha Logica/Plano das Fichas|Plano das Fichas]], achada na [[Ficha Logica/Viagem|Viagem]], VI1-7), com a **parte 10: Simulação de mesa**. Os pontos marcados com OC1 vêm da **rodada OC1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Outras cidades (LOD 1 e 2)

## 1. O que faz

Põe as **22 cidades do mundo** (19 de facção e 3 Vilas Livres), além da vila do jogador, para viver ao mesmo tempo que a vila, sem tela: comem, trabalham, pagam salários, mudam preços, crescem e sobem de nível. É o que o Comércio (B2) precisa para ter Mercados com preços diferentes.

**A ideia central (OC1-1):** a cidade longe do jogador roda **as mesmas regras do dia** da vila (consumo, salários, mercado, estabilizador, felicidade, eventos, IA da produção) e a **mesma regra de produção por hora**. Só uma coisa muda: o NPC não anda. Em vez de contar os ticks que ele trabalhou, o jogo **presume** o trabalho de cada empregado no turno. Assim, a produção em LOD 2 bate com a do LOD 0, e entrar ou sair de uma cidade não dá salto.

Documentos: [[Sistema de Mapa/Descrição|Mapa]] (a tabela de LOD), [[Sistema de Tempo/Descrição|Tempo]] (a frequência), [[Arquitetura/Descrição|Arquitetura]] (a meta de NPCs), [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]] (as cidades iniciais).

## 2. Dados

### Os níveis de simulação nesta versão

| LOD | Quem | O que roda |
|---|---|---|
| **0** | O mapa onde o jogador está | Tudo, tick a tick, com tela |
| **1** | A **vila do jogador** quando ele está fora (VI1-7) | Tudo, tick a tick, **sem tela** (como o `--simular`) |
| **2** | Todas as outras cidades | As regras do dia + a produção por hora com **trabalho presumido**; os NPCs não andam |

O LOD 1 "de verdade" do documento (mapas com NPC Controlado fora da câmera, sem busca de caminho fina) entra com os companheiros, na fase D.

### Definições (`data/regras.json`, bloco novo `"outras_cidades"`)

```json
"outras_cidades": {
  "trabalho_presumido_em_porcento": 98,
  "npcs_reais_por_nivel": [15, 40, 100, 300],
  "dinheiro_inicial_do_banco_por_nivel": [400, 1500, 6000, 20000],
  "verba_da_coroa_por_npc_por_estacao_em_decimos": 50,
  "construcoes_de_producao_a_cada_npcs": 15
}
```

| Campo | O que é |
|---|---|
| `trabalho_presumido_em_porcento` | Quanto de cada hora do turno conta como trabalho em LOD 2. A IA mede cerca de **7,9 horas** num turno de 8 na vila (FL6-7): 98%. **Medir de novo** no teste da parte 8 |
| `npcs_reais_por_nivel` | Vila 15, vilarejo 40, cidade 100, capital 300 (Mundo e Estado Inicial) |
| `dinheiro_inicial_do_banco_por_nivel` | Em moedas, × a Riqueza da cultura |
| `verba_da_coroa_por_npc_por_estacao_em_decimos` | A verba da vila (75 moedas para 15 NPCs) por NPC: **5 moedas**. Até a Facção pagar do Banco da Facção (fase D) |
| `construcoes_de_producao_a_cada_npcs` | O gerador repete o conjunto de produção da vila inicial a cada 15 NPCs (4.1) |

### Arquivo `data/cidades_iniciais.json` (do documento do Mundo)

Uma linha por cidade: mapa, nome, nível, facção (ou nenhuma), cultura, pacote. O dinheiro sai da tabela acima.

### Estado (vai no save)

Cada cidade guarda **o mesmo que a vila já guarda**: NPCs (todos, com idade, classe, habilidades, casa, emprego), construções (tipo, nível, dono, empregados, progresso, receita), estoques, preços locais, Banco, dinheiro das classes, Felicidade, eventos ativos, acumulado da População. A mais: `lodDoMapa` (0, 1 ou 2) e `posicoesJaDistribuidas` (sim ou não; 4.5).

**Os NPCs existem sempre** (OC1-2): os cerca de 2.700 do mundo são criados no começo da partida e ficam no save, mesmo em LOD 2. O documento dizia "LOD 2 guarda só os dados agregados"; a ficha troca por registros completos sem posição (cerca de 200 bytes cada, 540 KB no total), para o NPC que o jogador conheceu continuar o mesmo.

**Os sorteios de cada cidade** (OC1-3): a vila do jogador continua com as sequências 1 (NPCs), 2 (eventos) e 4 (População). Cada outra cidade usa as suas: **sequência = 100 + número do mapa × 10 + k**, com k = 1 NPCs, 2 eventos, 4 População. O clima é por região (Viagem, 4.8).

## 3. Quando roda

| Quando | LOD 2 | LOD 0 e 1 |
|---|---|---|
| A cada tick | Nada | Tudo (como hoje) |
| A cada hora | **Produção com trabalho presumido** (4.3) | Produção com os ticks trabalhados (Produção, 4.2) |
| A cada dia | A sequência do dia inteira, na mesma ordem (passos 0 a 7) | A mesma |
| A cada estação | Verba da Coroa, redistribuição, regras da Cidade | As mesmas |

**A ordem entre cidades:** a cada hora e a cada dia, a vila do jogador roda **primeiro**, depois as outras pelo **número do mapa**. Nada de uma cidade lê o estado de outra no mesmo passo (até o Comércio, que vai ter as suas regras).

## 4. Regras passo a passo

### 4.1 Criar as cidades (começo da partida)

Para cada linha do `cidades_iniciais.json`, menos a vila do jogador (que vem da `vila_inicial`):

1. **As construções:** o pacote do nível e da cultura (Mundo e Estado Inicial). As de **produção** (Fazenda, Padaria, Posto de Coleta, Serraria, Carpintaria...) são as da vila inicial, repetidas a cada 15 NPCs, arredondando para cima (OC1-4), **menos as Braçais de um recurso que o mapa não tem** (CO1-1, do [[Ficha Logica/Comercio|Comércio]]: sem isso todas as cidades produzem igual e não há comércio). Grão e Água existem em todo mapa com cidade. Uma capital de 300 tem 20 vezes as da vila. As construções sem receita (Quartel, Templo, Palácio) entram, mas só ocupam lugar até os seus sistemas chegarem.
2. **Os NPCs:** a quantidade do nível, como na População: idade de 5 a 19 anos, habilidades da faixa Civil (20 a 60), sorteados com a sequência da cidade. Todos Civis; os Ricos aparecem depois, pelas regras da Classe Social (só a partir do nível 2).
3. **As casas:** as moradias que a População pede para essa quantidade.
4. **O emprego:** a contratação diária da Cidade (Trabalho, 4.1) roda uma vez, antes do dia 1.
5. **Os estoques:** 5 dias do consumo previsto de cada item (o estoque-alvo do Mercado Local); os preços locais começam no preço base.
6. **O dinheiro:** o Banco com o valor do nível × a Riqueza; cada Civil com o mesmo dinheiro de um Civil da vila inicial.
7. A escolha de receita (Produção, 4.3) roda uma vez.

### 4.2 Quem trabalha numa hora em LOD 2

Um empregado conta como **presente** numa hora quando:

1. a rotina dele (IA) põe essa hora no turno de trabalho;
2. ele não está doente nem em casa por evento (Febre, Epidemia);
3. o clima permite (Tempestade: metade do trabalho ao ar livre, como no LOD 0; Clima, 4.2).

### 4.3 A produção por hora em LOD 2

É a regra 4.2 da [[Ficha Logica/Producao|Produção]] **sem mudar nada**, menos o passo 3:

- Em vez dos ticks trabalhados, cada empregado presente soma **800 × 98 ÷ 100 = 784 ticks** na hora, × velocidade × bônus de nível.
- Os insumos, o lote, o estoque cheio, a sobra para o próximo ciclo e as Braçais ficam iguais.

### 4.4 O dia em LOD 2

A sequência do dia roda inteira, na mesma ordem: clima, consumo, salários, mercado, estabilizador, felicidade, eventos, IA da produção. Ela já é feita por classe e por construção, não por NPC andando. O que no LOD 0 depende de onde o NPC foi:

- **Compras no Mercado:** já são do consumo do dia (Economia, 4.1).
- **Lazer e Felicidade por ação:** **[Refinar depois]** quando houver; até lá, nada.
- **População:** as mesmas regras e o mesmo acumulado; os nascimentos criam registros de NPC sem posição; as mortes tiram o registro.

### 4.5 Entrar numa cidade (LOD 2 → 0)

1. Só acontece se o mapa tem `arquivo_tiled` (VI1-8).
2. **A primeira vez:** as construções da cidade são postas nos lotes do mapa, na ordem dos lotes e pelo porte (`posicoesJaDistribuidas` = sim). **[Refinar depois]:** o mapa da cidade precisa ter lotes para o pacote dela.
3. Cada NPC aparece onde a rotina manda naquela hora: em casa de noite, no trabalho no turno, no caminho de um para o outro na hora de ir.
4. A produção não muda: o progresso de cada construção já está certo (o LOD 2 somou as horas inteiras até aqui) e, daí em diante, conta os ticks de verdade.
5. A troca acontece **depois** do passo da hora, nunca no meio dele.

### 4.6 Sair de uma cidade (LOD 0 → 2)

1. As posições e as ações dos NPCs são esquecidas; os registros ficam.
2. O progresso das construções fica como está.
3. Se é a vila do jogador, ela vai para o LOD 1, não o 2 (VI1-7).

### 4.7 A verba da Coroa e o Banco

Cada cidade recebe, no dia 1 de cada estação, **5 moedas por NPC real** (a vila continua com as 75 dela). A reserva de 250 do Banco (FE4-3) vale para todas. A cidade de nível 4 tem verba de 1.500 por estação.

## 5. Contas

| Conta | Valor |
|---|---|
| NPCs no mundo | 6 capitais × 300 + 6 cidades × 100 + 7 vilarejos × 40 + Vila Livre do Norte 40 + 2 Vilas Livres de 15 + a vila do jogador 15 = **2.765** |
| Construções de produção | cerca de 2.765 ÷ 15 × 5 (as da vila) = **cerca de 920** |
| Trabalho por hora em LOD 2 | cerca de 920 construções, uma conta cada: nada perto do custo do LOD 0 |
| Dia em LOD 2 | as regras do dia para 2.765 NPCs: cerca de 185 vezes a vila, uma vez por dia |
| Memória | 2.765 × cerca de 200 bytes = **cerca de 550 KB** no save |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Produção | A regra 4.2 com o trabalho presumido (4.3) |
| IA | A rotina diz as horas do turno; a Utilidade só roda em LOD 0 e 1 |
| Economia, Classe Social, Trabalho, População, Cidade, Clima, Eventos, Cura | As mesmas regras do dia em cada cidade |
| Viagem | Entrar e sair de uma cidade troca o LOD (4.5, 4.6) |
| Comércio (B2) | Lê os estoques e os preços locais de cada cidade |
| Save | Uma seção por cidade; o formato passa para a **versão 5** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `lodDoMapa` | 0, 1 ou 2 |
| `criarCidadesIniciais()` | Regra 4.1 |
| `empregadoPresenteNestaHora(npc, hora)` | Regra 4.2 |
| `ticksPresumidosPorHora` | 784 |
| `produzirNaHora(cidade)` | Produção 4.2; em LOD 2 usa os ticks presumidos |
| `hidratarCidade(cidade)` / `desidratarCidade(cidade)` | Regras 4.5 e 4.6 |
| `sequenciaDoSorteioDaCidade(numeroDoMapa, tipo)` | 100 + mapa × 10 + tipo |

## 8. Testes

1. **A vila em LOD 2 por 1 ano:** rodar o teste de 1 ano com a vila forçada em LOD 2. O estoque de comida, o Banco e a Felicidade do dia 64 ficam a **no máximo 10%** dos valores do LOD 0, nas 10 sementes. Se não, ajustar o `trabalho_presumido_em_porcento` pela média medida.
2. **O teste de 1 ano não muda** com as 22 outras cidades rodando (as sequências delas são outras; a vila roda primeiro).
3. **A Padaria da parte 10.2:** 12 Pães no dia em LOD 2 e em LOD 0.
4. **Entrar às 11 h:** os 12 Pães do dia continuam (parte 10.3).
5. **Criar a capital:** 300 NPCs, 20 vezes as construções de produção da vila, comida produzida ≥ 300 por dia na primeira semana.
6. **Mesma semente, mesmo mundo:** dois começos de partida dão as mesmas 22 cidades.
7. **Tempo:** um ano de jogo com as 22 cidades, sem tela, leva no máximo **3 vezes** o tempo do teste de hoje (só a vila).

## 9. Fora desta versão

- O LOD 1 do documento (companheiros fora da câmera); o LOD 0 de vários mapas ao mesmo tempo.
- Ricos, empresas e leilões já no começo (vêm com o tempo, pela Classe Social).
- As construções sem sistema (Quartel, Templo, Prisão, Palácio, Universidade, Casa da Moeda).
- O Banco da Facção pagando a verba (Facção, fase D); as diferenças de cultura nas receitas.
- A troca de mercadorias entre cidades (Comércio, B2).
- Os lotes e o desenho dos mapas das outras cidades (conteúdo).

## 10. Simulação de mesa

### 10.1 Cenário

Uma Padaria de nível 1 numa cidade em LOD 2, com 2 padeiros: habilidade 40 (velocidade 90%) e 60 (velocidade 110%). Turno das 7 h às 15 h. O Pão leva **4 horas** por ciclo, lote de **4**. Um ciclo = 4 × 800 × 100 = **320.000** pontos de progresso.

### 10.2 Um dia em LOD 2 e em LOD 0

**LOD 2**, a cada hora do turno: 784 × 90 + 784 × 110 = **156.800**.

| Hora que acabou | Progresso | Ciclos (Pães) | Sobra |
|---|---|---|---|
| 8 h | 156.800 | — | 156.800 |
| 9 h | 313.600 | — | 313.600 |
| 10 h | 470.400 | 1 (4) | 150.400 |
| 11 h | 307.200 | — | 307.200 |
| 12 h | 464.000 | 1 (8) | 144.000 |
| 13 h | 300.800 | — | 300.800 |
| 14 h | 457.600 | 1 (12) | 137.600 |
| 15 h | 294.400 | — | 294.400 |

Total: **12 Pães**, sobra 294.400 para o dia seguinte.

**LOD 0**, com os 7,9 horas medidas: 6.320 ticks × 90 + 6.320 × 110 = 1.264.000 → 3 ciclos = **12 Pães**, sobra 304.000. **Bate:** a diferença é de 9.600 pontos (3% de um ciclo).

### 10.3 O jogador entra às 11 h

| Momento | O que acontece |
|---|---|
| 8 h a 11 h | LOD 2: 4 horas = 627.200 → 1 ciclo (4 Pães), progresso 307.200 |
| 11 h | Passo da hora acaba; o jogador entra; os padeiros aparecem na Padaria (é turno) |
| 11 h a 15 h | LOD 0: cerca de 3,9 horas de verdade cada = 3.120 ticks × 200 = 624.000 |
| 15 h | 307.200 + 624.000 = 931.200 → mais 2 ciclos (8 Pães), sobra 291.200 |

Total do dia: **12 Pães**, como nos dois casos inteiros. Nenhum salto.

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| A produção bate entre LOD 0 e 2? | **OK:** 12 = 12 por dia na Padaria; a diferença é a sobra (3% de um ciclo) |
| A troca de LOD dá salto? | **Não:** a troca acontece entre horas, e o progresso é o mesmo número nos dois modos |
| O teste de 1 ano muda? | **Não:** a vila roda primeiro e as outras cidades sorteiam com as suas sequências (OC1-3). **Achado:** sem isso, as 22 cidades gastariam números da sequência 2 dos eventos, e a vila mudaria |
| As cidades novas passam fome? | **Não no começo:** o conjunto de produção da vila alimenta 15; repetido a cada 15 NPCs, alimenta todos (OC1-4). **Achado:** o pacote do documento (Fazenda, Posto de Coleta e moradias para o nível 1) não diz quantas; uma capital de 300 com 1 Fazenda morreria de fome na primeira estação |
| O dinheiro das cidades aguenta? | **Igual à vila:** a verba de 5 moedas por NPC por estação é a mesma proporção que segura o Banco da vila acima de 250 (FE4-3). A capital começa com 20.000 × Riqueza e recebe 1.500 por estação |
| O tempo aguenta? | O dia das 22 cidades é cerca de 185 vezes o da vila, uma vez por dia; a produção é 1 conta por construção por hora. O teste 7 mede |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Entra às 23 h | Todos em casa; nenhuma produção a mudar |
| Entra no meio do caminho para o trabalho (6 h 50) | O NPC aparece no caminho; o LOD 2 não somou essa hora (não era turno) |
| Uma cidade fica sem NPCs | As construções param ("sem empregados"); o Banco continua recebendo a verba (5 × 0 = 0) |
| Um NPC que o jogador conheceu morre em LOD 2 | O registro sai; ao voltar, ele não está. A notícia da morte de um conhecido fica para as Notícias (B3) |
| Uma cidade em LOD 2 sobe de nível | As regras da Cidade rodam igual; a obra nova é posta num lote só quando o jogador entrar (4.5) |
| Salvar com o jogador numa cidade | A cidade guarda as posições (LOD 0); as outras, só os registros |
| Mapa de cidade sem arquivo do Tiled | Fica em LOD 2 para sempre (VI1-8); o jogador a vê no Mapa Mundial e, com o Comércio, negocia pelas caravanas |

### 10.6 O que achou e como foi resolvido

1. **Os sorteios das outras cidades mudariam a vila.** **Resolvido (OC1-3):** uma sequência por cidade, e a vila roda primeiro.
2. **O pacote do documento não dá a quantidade de construções de produção.** **Resolvido (OC1-4):** repetir o conjunto da vila a cada 15 NPCs.
3. **O documento manda o LOD 2 guardar só agregados**, mas aí o NPC conhecido some. **Resolvido (OC1-2):** guardar todos os NPCs (cerca de 550 KB).
4. **O LOD 2 "por estação" do documento** faria a cidade andar aos saltos, e o Comércio precisa de preços que mudam a cada dia. **Resolvido (OC1-1):** as regras do dia todo dia e a produção por hora, com o trabalho presumido; o custo é pequeno (parte 5).
5. **A verba das outras cidades.** **Resolvido (OC1-5):** 5 moedas por NPC por estação até a Facção.

### 10.7 Decidido em modo automático (rodada OC1, seguir sugestão)

1. **OC1-1:** em LOD 2, as mesmas regras do dia, todo dia, e a produção por hora com o trabalho presumido (98% do turno, a medir no teste 1).
2. **OC1-2:** todos os NPCs do mundo existem sempre como registros, no save.
3. **OC1-3:** cada cidade sorteia com as suas sequências (100 + mapa × 10 + tipo); a vila do jogador roda primeiro e mantém as dela.
4. **OC1-4:** o conjunto de produção da vila inicial a cada 15 NPCs, arredondando para cima.
5. **OC1-5:** a verba da Coroa de 5 moedas por NPC por estação para as outras cidades.
6. **OC1-6:** a vila do jogador fica em LOD 1 (tudo, sem tela) quando ele sai (confirma a VI1-7).
