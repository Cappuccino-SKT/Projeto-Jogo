---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase B1 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com VI1 vêm da **rodada VI1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Viagem

## 1. O que faz

Move o **grupo do jogador** e, a partir do Comércio (B2), as **caravanas** pelo **Mapa Mundial**, de Mapa Local em Mapa Local, como num grafo. Diz quanto tempo cada trecho leva, quando o grupo acampa e come, sorteia os **encontros** no caminho e guarda o que o jogador já viu do mundo (**névoa**).

Documento do sistema: [[Sistema de Viagem/Descrição|Viagem e Mundo]]; a malha, em [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]].

**Esta versão:** o grupo do jogador (o jogador sozinho; os companheiros chegam na fase D), as regras de movimento que as caravanas vão usar, os encontros por sorteio e a névoa. **Exércitos, patrulhas e bandos que rondam** ficam para a Facção e a Guerra (parte 9).

## 2. Dados

### Definições: `data/viagem.json` (arquivo novo)

```json
{
  "ticks_base_por_mapa": 3200,
  "ticks_minimos_por_mapa": 800,
  "terreno_em_porcento": { "planicie": 100, "litoral": 100, "floresta": 125, "colinas": 125, "arido": 125, "floresta_densa": 150, "montanha": 150 },
  "infraestrutura_em_porcento": { "selvagem": 100, "trilha": 90, "estrada": 75, "cidade_grande": 65 },
  "tamanho_do_grupo": [ [5, 100], [20, 110], [100, 125], [500, 150], [2000, 175], [999999, 200] ],
  "montados_em_porcento": 67,
  "carga_acima_do_limite_em_porcento": 125,
  "caravana_em_porcento": 133,
  "clima_em_porcento": { "sol": 100, "nublado": 100, "chuva": 125, "neve": 150, "tempestade": 150, "calor_forte": 110 },
  "estacao_em_porcento": { "lama_na_primavera_sem_estrada": 110, "inverno_bioma_frio": 150, "inverno_outros": 110 },
  "teto_clima_vezes_estacao_em_porcento": 200,
  "acampar_das_horas": [22, 6],
  "refeicoes_por_pessoa_por_dia": 1,
  "chance_de_encontro_por_mil": { "territorio": 50, "em_disputa": 150, "sem_dono": 200 },
  "marcha_forcada_encontro_em_porcento": 150,
  "pesos_dos_encontros": { "bandidos": 75, "mercador_viajante": 25 },
  "bando_bandidos_minimo": 3,
  "bando_bandidos_sorteio_ate": 9,
  "forca_por_pessoa": { "jogador": 3, "civil": 1, "guarda": 3, "soldado": 4, "bandido": 2 },
  "bandidos_nao_atacam_se_o_grupo_for_vezes_mais_forte": 2,
  "suborno_por_bandido_em_decimos": 250,
  "fuga_base_em_porcento": 50,
  "fuga_limites_em_porcento": [10, 90],
  "perda_ao_se_render_em_porcento": 50,
  "perda_ao_perder_a_luta_em_porcento": 75,
  "luta_rapida_dura_ticks": 800,
  "perda_de_carga_da_caravana_minima_em_porcento": 30
}
```

| Campo | O que é |
|---|---|
| `ticks_base_por_mapa` | 4 horas a pé = 3.200 ticks |
| `tamanho_do_grupo` | Pares [até tantas pessoas, multiplicador]. Um Esquadrão conta como 100 |
| `pesos_dos_encontros` | Só os dois tipos desta versão. Patrulha (Crime), peregrinos (Religião), refugiados (Guerra) e caravana em apuros (Comércio) entram com os seus sistemas |
| `forca_por_pessoa` | A **força simples** do cálculo rápido (VI1-4). O Combate troca pelos atributos de verdade |

### O `mapas.json` ganha, em cada mapa

| Campo | Exemplo | Uso |
|---|---|---|
| `coluna`, `linha` | 3, 2 | Posição na malha 8 × 6; os vizinhos são os 4 lados |
| `numero_do_mapa` | (linha − 1) × 8 + coluna | Ordem fixa para desempates |
| `regiao`, `bioma` | 1, `"temperado"` | Clima do dia e estação |
| `terreno`, `infraestrutura` | `"floresta"`, `"trilha"` | Tempo do trecho |
| `estado_do_territorio` | `"territorio"`, `"em_disputa"`, `"sem_dono"` | Chance de encontro |
| `arquivo_tiled` | `"vila_inicial.tmj"` ou `""` | Vazio = sem desenho: só se atravessa pelo Mapa Mundial (VI1-8) |
| `capital` | `true` / `false` | Conhecida desde o início |

### Estado (vai no save)

**Cada entidade em trânsito** (o grupo do jogador é a entidade 0):

| Campo | Tipo | Exemplo |
|---|---|---|
| `tipoDeEntidade` | texto | `"grupo_do_jogador"`, `"caravana"` |
| `mapaAtual`, `proximoMapa` | inteiros | 10, 11 |
| `rota` | lista de mapas até o destino | [11, 19, 20, 28] |
| `ticksDoTrecho` | inteiro, calculado ao entrar no trecho e **fixo** até o fim dele (VI1-3) | 4000 |
| `ticksAndadosNoTrecho` | inteiro | 1800 |
| `pessoas`, `montados`, `cargaAcimaDoLimite` | inteiro, sim/não, sim/não | 1, não, não |
| `marchaForcada`, `noitesSemDormir` | sim/não, inteiro | não, 0 |
| `encontroMarcado` | o encontro sorteado ao entrar no trecho, ou nenhum | bandidos, 6 |
| `inventarioDoGrupo` | itens e dinheiro levados | 3 Pães, 1.500 décimos |

**A névoa**, um registro por mapa: `diaEmQueFoiVisto` (−1 = nunca) e o que foi visto naquele dia (dono, nível da cidade, estado do território).

**O clima de cada região** (`climaDeHojePorRegiao`, 6 valores): a [[Ficha Logica/Clima|ficha do Clima]] tinha um clima só; agora cada região sorteia o seu (VI1-6).

**O sorteio:** um gerador só da viagem, o PCG32 com a semente da partida e a **sequência 5** (NPCs 1, eventos 2, clima 3, população 4).

## 3. Quando roda

| Quando | O quê |
|---|---|
| **A cada tick**, para cada entidade em trânsito que não está acampada | Avança 1 tick no trecho (regra 4.3) |
| Ao **entrar num trecho novo** | Calcula `ticksDoTrecho` (4.2) e sorteia o encontro (4.5) |
| **A cada dia, passo 0** (clima) | Sorteia o clima de cada região, a da vila primeiro (4.8) |
| **A cada dia, passo 1** (consumo) | Cada grupo em trânsito come do próprio inventário (4.4) |
| Às **22 h** e às **6 h** | Acampa e retoma (4.4) |

## 4. Regras passo a passo

### 4.1 Partir

1. O jogador abre o Mapa Mundial e escolhe um destino. Só pode partir se não estiver em combate (fase D) e o destino for outro mapa.
2. Busca a **rota mais rápida** (Dijkstra) no grafo de 48 mapas, usando o tempo do trecho **sem** clima e estação (assim a rota não muda todo dia). Empate: o caminho que passa pelo mapa de **menor número** primeiro. Refaz a rota só se o destino mudar ou um mapa ficar bloqueado (fase D).
3. O grupo sai do Mapa Local e vira o ícone no Mapa Mundial, no centro do mapa atual. O primeiro trecho começa (4.2).
4. **A vila não para:** enquanto o jogador viaja, a vila continua rodando com as mesmas regras, sem tela, como no `--simular` (VI1-7). As oficinas do jogador seguem a [[Ficha Logica/Trabalho|ficha do Trabalho]].

### 4.2 O tempo de um trecho

Um trecho vai do **centro de um mapa ao centro do vizinho** e usa os valores do **mapa de chegada** (VI1-3). Ao entrar no trecho:

`ticksDoTrecho` = 3.200 × terreno ÷ 100 × infraestrutura ÷ 100 × tamanho ÷ 100 × montaria ÷ 100 × carga ÷ 100 × caravana ÷ 100 × clima e estação ÷ 100

- Cada divisão é **arredondada para baixo**, nesta ordem.
- **Clima e estação** = clima de hoje da região do mapa de chegada × estação ÷ 100, com **teto de 200**.
- Infraestrutura: vale o melhor valor do mapa; `cidade_grande` = cidade de nível 3 ou 4.
- Mínimo de **800 ticks** (1 hora).
- O valor fica **fixo** até o fim do trecho, mesmo se o clima mudar à meia-noite.

### 4.3 Andar

1. A cada tick, se o grupo não está acampado, `ticksAndadosNoTrecho` sobe 1.
2. Se há `encontroMarcado` e o andado chega à **metade** do trecho, o encontro acontece (4.6).
3. Quando o andado chega a `ticksDoTrecho`: o grupo chega ao centro do próximo mapa. Se é o destino, a viagem acaba (4.7); se não, começa o trecho seguinte (4.2).
4. **Caravanas na Tempestade:** se o clima de hoje da região do `proximoMapa` é Tempestade, a caravana não anda o dia todo. O grupo do jogador anda (com o × 150 do trecho, se entrou nele hoje).

### 4.4 Noite e comida

1. Às **22 h**, o grupo acampa e para de andar; às **6 h**, retoma do ponto onde parou. Com **marcha forçada**, não acampa: `noitesSemDormir` sobe 1 a cada 6 h passadas sem acampar.
2. No **consumo** (passo 1 do dia), cada pessoa do grupo come **1 refeição** do `inventarioDoGrupo` (a comida de maior Tier primeiro, como na vila).
3. **Sem comida:** a Felicidade do grupo cai 10 por dia e a Vida cai aos poucos (ligado com os companheiros e o Combate, fase D). Nesta versão, o jogador recebe só o aviso "O grupo está sem comida".
4. **Marcha forçada:** a chance de encontro × 150% e a Felicidade do grupo −5 por noite (fase D).

### 4.5 Sortear o encontro (ao entrar no trecho)

1. Chance = a do `estado_do_territorio` do mapa de chegada (50, 150 ou 200 por mil) + a ameaça bandida da região (0 nesta versão; Eventos) × 150% se for marcha forçada.
2. Sorteia de 0 a 999. Se o número for **menor** que a chance, há encontro.
3. Sorteia o tipo pelos pesos: 75 bandidos, 25 mercador viajante.
4. **Bandidos:** o bando tem 3 + sorteio de 0 a 9 bandidos.
5. Guarda tudo em `encontroMarcado`. O encontro acontece na metade do trecho (4.3). Encontros sorteados, sem bandos que rondam o mapa (VI1-1).

### 4.6 O encontro

1. A velocidade volta para 1x e o jogo pausa.
2. **Mercador viajante:** abre a troca com os preços de outra região (Comércio, B2; nesta versão, os preços da vila × 110%). Depois o grupo segue.
3. **Bandidos:** força do bando = bandidos × 2; força do grupo = soma de `forca_por_pessoa` de cada membro. Se a força do grupo for **mais de 2 vezes** a do bando, eles não atacam ("Um bando observa vocês de longe e vai embora"). Se não, o jogador escolhe:
   - **Subornar:** preço = bandidos × 25 moedas × (150 − Carisma do jogador) ÷ 100, arredondado para baixo. Só aparece se o grupo tiver o dinheiro. Paga e segue.
   - **Fugir:** chance = 50% + (tempo do bando − tempo do grupo) × 50 ÷ tempo do bando, entre 10% e 90%. O tempo de cada lado é o tamanho × montaria (sem terreno, que é o mesmo). Se der certo, segue; se falhar, vira **Lutar**.
   - **Lutar:** até o Combate (fase D), é o **cálculo rápido** (VI1-2): chance de vencer = força do grupo ÷ (força do grupo + força do bando). Leva 1 hora (800 ticks parados). Vencer: segue. Perder: o grupo perde **75%** do dinheiro e de cada item carregado (a perda arredondada para baixo) e segue.
   - **Render-se:** perde **50%** do dinheiro e de cada item, sem luta, e segue. A captura do Combate entra na fase D.

### 4.7 Chegar

1. A velocidade volta para 1x.
2. Se o mapa tem `arquivo_tiled`, o grupo aparece no ponto **`entrada_do_mapa`** (o mesmo ponto em que os migrantes da População chegam; VI1-9). Se não tem, o grupo fica no Mapa Mundial e só pode escolher outro destino (VI1-8).

### 4.8 O clima por região

No passo 0 do dia, o gerador do clima (sequência 3) sorteia **primeiro a região da vila** e depois as outras, da região 1 à 6, pulando a da vila. Assim, os sorteios da vila são **os mesmos da fatia** e o teste de 1 ano não muda. Todas as regiões usam a tabela temperada até os outros biomas terem a sua (Clima, parte 9).

### 4.9 Andar até a borda

Quando o jogador anda até a borda de um Mapa Local (sem abrir o Mapa Mundial), o mapa vizinho é carregado e o jogador aparece na borda oposta. Só vale se o vizinho tiver `arquivo_tiled`; se não tiver, a borda fica fechada com o aviso "Para seguir por aqui, use o Mapa Mundial". Um encontro não é sorteado nesse caminho (o jogador está vendo o mapa).

### 4.10 Névoa

1. Ao **entrar** num mapa (no centro ou pela borda), `diaEmQueFoiVisto` = hoje, e o registro guarda dono, nível e estado.
2. A **visão atual** é o mapa do grupo e os 4 vizinhos: mostram as entidades em trânsito ao vivo e atualizam a data.
3. **Desde o início:** as 6 capitais e os vizinhos do mapa de partida. Os estilos de início (Mercador, Membro de facção, Aventureiro) entram com o Início de Jogo.

### 4.11 Cálculo rápido entre a IA (a partir do Comércio)

Caravana × bandidos longe do jogador: no trecho com encontro de bandidos, a caravana vence com a chance força ÷ (força + força do bando). Se perde, fica com uma perda de carga = 30% + 70% × (força do vencedor − força do perdedor) ÷ força do vencedor. O resultado vira notícia (Notícias, B3).

## 5. Contas

| Quem | Conta (em ticks) | Tempo |
|---|---|---|
| Mensageiro a cavalo, cidade nível 4, planície | 3.200 × 100 × 65 × 100 × 67 → 2.080 → **1.393** | 1 h 44 |
| Jogador a pé, floresta sem estrada | 3.200 × 125 → **4.000** | 5 h |
| Caravana de 15, estrada na planície | 3.200 × 75 → 2.400 × 110 → 2.640 × 133 → **3.511** | 4 h 23 |
| Exército de 20 Esquadrões, montanha | 3.200 × 150 → 4.800 × 175 → **8.400** | 10 h 30 |

Os quatro batem com os exemplos do documento. Marcha de um dia: das 6 h às 22 h = **16 horas**. Cruzar o mundo a pé (8 mapas de planície sem estrada) = 32 h = 2 dias.

**Suborno:** 8 bandidos, Carisma 50: 8 × 25 × 100 ÷ 100 = **200 moedas** (o exemplo do documento).

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Tempo | O tick move as entidades; a velocidade 8x só no Mapa Mundial; volta a 1x no encontro e na chegada |
| Clima | O clima por região (4.8); os multiplicadores de clima e estação |
| Mapa | O Mapa Mundial, o `mapas.json`, o ponto `entrada_do_mapa`, a borda |
| Inventário | O `inventarioDoGrupo` e as perdas do encontro |
| Economia | O dinheiro do jogador no suborno e nas perdas |
| População | O mesmo ponto de chegada dos migrantes |
| Comércio (B2) | As caravanas usam 4.2 a 4.5 e 4.11 |
| Notícias (B3) | A notícia anda no passo do mensageiro a cavalo |
| Save | A seção `viagem`; o formato passa para a **versão 4** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `EntidadeEmTransito` | O registro da parte 2 |
| `buscarRotaMaisRapida(origem, destino, entidade)` | Dijkstra, regra 4.1 |
| `calcularTicksDoTrecho(entidade, mapaDeChegada)` | Regra 4.2 |
| `avancarEntidadesEmTransito()` | Regra 4.3, a cada tick |
| `acamparOuRetomar(hora)` | Regra 4.4 |
| `sortearEncontroAoEntrarNoTrecho(entidade)` | Regra 4.5 |
| `resolverEncontroComBandidos(escolha)` | Regra 4.6 |
| `resolverPorCalculoRapidoDeForcas(forcaA, forcaB)` | Regras 4.6 e 4.11 |
| `marcarMapaComoVisto(numeroDoMapa)` | Regra 4.10 |
| `climaDeHojePorRegiao` | Regra 4.8 |

## 8. Testes

1. **Os quatro exemplos** da parte 5 dão os ticks exatos.
2. **Rota:** de (1,1) a (3,1) em planície igual passa por (2,1); com floresta densa em (2,1) e estrada em (2,2) e (3,2), a rota desvia se for mais rápida; empate escolhe o menor número.
3. **Acampar:** trecho de 2.080 ticks começado às 20 h 45 termina às 7 h 21 do dia seguinte (parte 10.2).
4. **Tempo fixo:** um trecho começado num dia de Sol não muda quando o dia seguinte sorteia Chuva.
5. **Encontro na metade:** o encontro marcado acontece com o andado = metade do trecho.
6. **Bandidos não atacam** um grupo com força 25 contra um bando de 6 (força 12): 25 > 24.
7. **Suborno escondido** quando falta dinheiro.
8. **Perdas arredondadas para baixo:** 3 Pães e 75% → perde 2, fica 1.
9. **Clima da vila igual:** o teste de 1 ano com as 10 sementes dá os mesmos resultados com as 6 regiões sorteando (4.8).
10. **Mesma semente, mesma viagem:** dois saves iguais, mesma viagem, mesmos encontros.
11. **Save no meio do trecho:** carregar continua do mesmo tick do trecho e com o mesmo encontro marcado.
12. **Mapa sem arquivo:** chegar a ele deixa o grupo no Mapa Mundial; a borda para ele fica fechada.

## 9. Fora desta versão

- Exércitos, patrulhas e **bandos que rondam** o mapa (Facção e Guerra, fase D); a ameaça bandida dos Eventos.
- A luta de verdade no Mapa Local e a captura (Combate, fase D); os companheiros, a Vida e a Felicidade do grupo.
- Encontros de patrulha, peregrinos, refugiados e caravana em apuros.
- Hidratação: a caravana que aparece na borda do mapa do jogador e o atravessa pela estrada (entra com o Comércio, B2).
- Estradas construídas, dano de guerra, mapas bloqueados, Passagem Militar.
- Conhecimento inicial por estilo e mapas comprados.
- As outras cidades simuladas (ver 10.6: ficha nova antes do Comércio).

## 10. Simulação de mesa

### 10.1 Cenário

O jogador sai da vila (mapa M1) para uma cidade de nível 3 (M5), a 4 trechos. Ele vai a pé e sozinho (força 3, Carisma 40), com 150 moedas e 3 Pães. É Outono (sem multiplicador de estação). No dia 1 faz Sol; no dia 2, Chuva.

| Trecho | Mapa de chegada | Terreno, infraestrutura | Estado | Ticks |
|---|---|---|---|---|
| 1 | M2 | floresta, selvagem | em disputa (150‰) | 3.200 × 125 = **4.000** |
| 2 | M3 | floresta, estrada | território (50‰) | 4.000 × 75 = **3.000** |
| 3 | M4 | planície, estrada | território (50‰) | **2.400** |
| 4 | M5 | planície, cidade nível 3 | território (50‰) | **2.080** |

### 10.2 A viagem, hora a hora

| Dia e hora | O que acontece |
|---|---|
| D1 8 h 00 | Parte. Trecho 1: 4.000 ticks. Sorteio de encontro: 87 < 150 → **encontro**; tipo 412 < 750 → bandidos; tamanho 3 + 3 = **6 bandidos** (força 12) |
| D1 10 h 30 | Metade do trecho (2.000 ticks): o encontro. Força do grupo 3 < 24 → atacam. Suborno: 6 × 25 × 110 ÷ 100 = 165 moedas > 150 → **não aparece**. O jogador escolhe Fugir: 50 + (110 − 100) × 50 ÷ 110 = **54%**; sorteio 612 → falha → Lutar |
| D1 10 h 30 | Luta rápida: chance 3 ÷ 15 = **20%**; sorteio 734 → perde. Perde 75% de 1.500 décimos = 1.125 → fica com **37,5 moedas**; perde 2 Pães → fica com **1** |
| D1 11 h 30 | A luta levou 1 hora; volta a andar |
| D1 14 h 00 | Chega a M2. Trecho 2: 3.000 ticks; sorteio 503 > 50 → nada |
| D1 17 h 45 | Chega a M3. Trecho 3: 2.400; sorteio 220 → nada |
| D1 20 h 45 | Chega a M4. Trecho 4: 2.080; sorteio 911 → nada |
| D1 22 h 00 | Acampa: andou 1.000 de 2.080 |
| D2 0 h | Clima da região: Chuva (o trecho 4 continua com 2.080). Consumo: come o último Pão |
| D2 6 h 00 | Retoma: faltam 1.080 ticks |
| D2 7 h 21 | **Chega a M5** e aparece na `entrada_do_mapa` |

### 10.3 Uma caravana no inverno (a regra que o Comércio vai usar)

Caravana de 15 pessoas, estrada na planície, inverno no bioma temperado, dia de Neve: clima e estação = 150 × 110 ÷ 100 = 165 (abaixo do teto de 200). Ticks: 2.400 × 110 → 2.640 × 133 → 3.511 × 165 → **5.793** (7 h 14). No dia seguinte, Tempestade: a caravana fica parada o dia todo, no ponto onde estava do trecho.

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| Os exemplos do documento batem? | **OK:** os 4 da parte 5 e o suborno de 200 moedas |
| A chegada depois do acampamento está certa? | **OK:** 1.000 ticks antes das 22 h + 1.080 depois das 6 h = 2.080 |
| O teste de 1 ano muda? | **Não:** a vila sorteia o clima primeiro (4.8), a viagem tem sequência própria (5), e o teste não viaja |
| A comida da viagem bate? | **OK:** 1 Pão por dia; quem sai com 3 Pães e é assaltado fica com 1 e come no dia 2 |
| Fugir vale a pena contra Render-se? | **Sim, na média:** fugir perde 46% × 80% × 75% ≈ **28%** do que leva; render-se perde 50%. A escolha tem risco e faz sentido |
| E o jogador sozinho? | Risco de encontro com bandidos nesta rota: 1 − (1 − 0,1125) × (1 − 0,0375)³ ≈ **21%** por viagem. Sem guardas, ele quase sempre perde a luta. É o perigo esperado para quem viaja sozinho; os guardas vêm com os companheiros e o Comércio |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Sai às 21 h 59 | Anda 1 minuto e acampa; segue às 6 h |
| O encontro cai na noite | O grupo está acampado e não anda: o encontro acontece quando o andado chegar à metade, de manhã |
| Destino = o mapa atual | Não parte |
| Mapa na borda da malha | Só tem 2 ou 3 vizinhos; a rota nunca sai da malha |
| Sem dinheiro e sem itens | Render-se e perder não tiram nada; o grupo segue |
| Salvar acampado | Carrega acampado, com o mesmo andado |
| Tempestade com o jogador a 8x | O jogador não para (só as caravanas); a velocidade só volta a 1x no encontro e na chegada |
| Um evento grave na vila enquanto o jogador viaja (incêndio) | **[Em aberto, fica para a Interface]:** hoje nada pausa a 8x; a sugestão é o aviso de evento grave também voltar para 1x |

### 10.6 O que achou e como foi resolvido

1. **Não há outras cidades simuladas.** As fichas de hoje só simulam a vila; o destino da viagem é um mapa sem vida. A Simulação em LOD 1 e 2 não tem ficha. **Resolvido (VI1-7):** a vila segue rodando sem tela enquanto o jogador viaja, e uma ficha nova, **B1b "Outras cidades (LOD 1 e 2)"**, entra no plano antes do Comércio. O Comércio não funciona sem os Mercados das outras cidades.
2. **Faltam 47 mapas desenhados.** **Resolvido (VI1-8):** um mapa sem arquivo do Tiled só se atravessa pelo Mapa Mundial. O mundo inteiro já pode ser testado com só a vila desenhada.
3. **O clima era um só.** Com várias regiões, cada uma precisa do seu. **Resolvido (VI1-6):** a região da vila sorteia primeiro e o teste de 1 ano não muda.
4. **O trecho atravessa dois mapas** (de centro a centro). **Resolvido (VI1-3):** vale o mapa de chegada, e o tempo fica fixo para não mudar no meio do trecho.
5. **Lutar precisa do Combate** (fase D). **Resolvido (VI1-2):** até lá, cálculo rápido de forças, com perda maior que a de se render.

### 10.7 Decidido em modo automático (rodada VI1, seguir sugestão)

1. **VI1-1:** os encontros são sorteados ao entrar no trecho; os bandos que rondam o mapa entram com a Facção.
2. **VI1-2:** Lutar, até o Combate, é o cálculo rápido (força ÷ soma). Perder a luta tira 75%; render-se tira 50%.
3. **VI1-3:** o trecho usa os valores do mapa de chegada e o tempo fica fixo desde a entrada.
4. **VI1-4:** a força simples por pessoa (jogador 3, civil 1, guarda 3, soldado 4, bandido 2) fica no `viagem.json`.
5. **VI1-5:** suborno de 25 moedas por bandido × (150 − Carisma) ÷ 100; fuga de 50% ajustada pela velocidade, entre 10% e 90%.
6. **VI1-6:** o clima é por região, e a região da vila sorteia primeiro.
7. **VI1-7:** a vila continua rodando sem tela enquanto o jogador viaja; a ficha nova B1b (Outras cidades, LOD 1 e 2) vem antes do Comércio.
8. **VI1-8:** um mapa sem arquivo do Tiled só se atravessa pelo Mapa Mundial.
9. **VI1-9:** um ponto só, `entrada_do_mapa`, para a chegada da viagem e dos migrantes (a População chamava de `entrada_da_vila`).
