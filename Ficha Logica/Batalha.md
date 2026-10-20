---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase D6 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com BA1 vêm da **rodada BA1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Batalha

## 1. O que faz

A batalha entre exércitos no **Mapa de Guerra**, por **Esquadrão** (100 soldados, 10 de Vida cada). Cada Esquadrão causa dano pelo Ataque, pela Defesa do alvo e pela vantagem; a **Moral** cai com as baixas e faz fugir. Formações, flanco, terreno e clima mudam os números. No fim, as baixas viram **mortos, feridos e prisioneiros**. A batalha sem o jogador é o **cálculo rápido**, com as mesmas grandezas.

Documento do sistema: [[Sistema de Batalha/Descrição|Batalha (Mapa de Guerra)]]; o combate local, no [[Ficha Logica/Combate|Combate]].

**Esta versão:** o Esquadrão, o dano, a Moral, as formações, o flanco, a carga, o terreno, o clima, o General e a Comitiva, os Magos, a artilharia, o fim da batalha, o cerco simples e o cálculo rápido de exércitos. **Os bonecos, os tamanhos visuais e as telas** são da Interface e da arte (parte 9).

## 2. Dados

### `data/esquadroes.json` e `data/batalha.json` (arquivos novos)

| Tipo | Ataque | Defesa | Alcance (tiles) | Velocidade | Intervalo (ticks) | Especial |
|---|---|---|---|---|---|---|
| Infantaria Leve | 8 | 4 | 1 | 1,0 | 20 | |
| Infantaria Pesada | 10 | 8 | 1 | 0,8 | 30 | +25% contra Lanceiros |
| Lanceiros | 8 | 6 | 2 | 0,9 | 20 | +50% contra Cavalaria; parados anulam a carga |
| Arqueiros | 6 | 2 | 12 | 1,0 | 30 | |
| Besteiros | 9 | 5 | 10 | 0,9 | 50 | |
| Cavalaria | 10 | 6 | 1 | 2,0 | 20 | +50% na carga; +25% contra tiro |
| Atiradores | 14 | 2 | 8 | 0,9 | 80 | Era 2 |
| Artilharia | 20 em área | 1 | 36 | 0,4 | 120 | Era 2; 30 tiros |
| Magos | 12 em área | 2 | 12 | 1,0 | 60 | Mana |
| Milícia | 5 | 3 | 1 | 1,0 | 20 | Só na defesa da própria cidade |

```json
"batalha": {
  "vida_por_soldado": 10,
  "soldados_em_contato_em_porcento": 30,
  "ritmo_do_dano_em_porcento": 10,
  "moral_inicial": 70,
  "moral_por_ponto_de_baixas": -1,
  "moral_flanco": -10, "moral_retaguarda": -20, "moral_general_derrotado": -25,
  "moral_vizinho_fugindo": -5, "moral_por_tiro_de_area": -3,
  "faixas_de_moral": { "firme": 50, "abalado": 25, "reagrupa_em": 40 },
  "disciplina_inicial": 50,
  "flanco_dano_em_porcento": 25, "retaguarda_dano_em_porcento": 50,
  "experiencia_por_nivel_em_porcento": 5, "batalhas_por_nivel": 2,
  "baixas": { "mortos": 40, "feridos": 40, "capturados": 20 },
  "perseguicao_com_cavalaria_em_porcento": 20,
  "esquadrao_se_desfaz_abaixo_de": 20,
  "tempo_limite_em_ticks": 36000,
  "o_mundo_avanca_depois_em_horas": 2,
  "maximo_por_lado_no_campo": 20
}
```

O **Rank** do kit muda o Ataque e a Defesa em +25% por Rank acima de 1 (como no Combate). A vantagem de arma × armadura (±25%) é a do Combate.

### Cada Esquadrão (vai no save, fora da batalha)

| Campo | Exemplo |
|---|---|
| `tipo`, `soldados`, `rankDoKit`, `experiencia` (0 a 5) | infantaria leve, 100, 1, 0 |
| `moral`, `disciplina` | 70, 50 |
| `cidadeDeOrigem`, `comandante` | (1,1); nenhum |
| `feridosQueVoltamNoDia` | 30 soldados no dia 210 |

O Mapa de Guerra e as posições **não vão no save** (a batalha não é salva no meio).

## 3. Quando roda

| Quando | O quê |
|---|---|
| **A cada tick**, no Mapa de Guerra | Movimento, golpes, Moral (4.1 a 4.6) |
| A cada 10 ticks | Cada Esquadrão reavalia o alvo |
| No fim da batalha | Baixas, prisioneiros, experiência (4.8) |
| Batalha sem o jogador | O cálculo rápido (4.10), uma vez |

**O mundo para** durante a batalha; no fim, avança **2 horas**. O sorteio usa a sequência da Viagem (5).

## 4. Regras passo a passo

### 4.1 O dano de um Esquadrão

Quando o intervalo acaba e o alvo está no alcance:

1. **Soldados que lutam:** no corpo a corpo, **30%** dos vivos (a frente da formação); no tiro e na área, todos.
2. **Dano por soldado** = o maior entre 1 e (Ataque × vantagem × bônus − Defesa do alvo). Bônus: flanco +25%, retaguarda +50%, carga +50%, Experiência +5% por nível, Comandante, terreno.
3. **Dano do golpe** = soldados que lutam × dano por soldado × 10% (o ritmo; BA1-2).
4. **Soldados derrubados** = dano ÷ 10 (Vida de cada um), com a fração guardada no Esquadrão para o próximo golpe.
5. **Área** (Artilharia, Magos): o dano vai em cada Esquadrão no raio de 3 tiles, **aliados também**.
6. **Tiro:** Arqueiros e Besteiros −15% de alcance na chuva; Atiradores −30% de dano na chuva e não atiram na tempestade; à noite, visão −30%.

### 4.2 A Moral

1. **Início:** 70 + General por perto (até +10) + Comandante + cultura (Religiosa +10) − 20 sem suprimento.
2. **Cai:**

| Causa | Moral |
|---|---|
| Soldados perdidos | **−1 a cada 1%** (BA1-1; o documento dizia 2%) |
| Atacado pelo flanco / pela retaguarda | −10 / −20 no impacto |
| General derrotado | −25 em todo o exército |
| Vizinho aliado fugindo | −5 |
| Atingido por área | −3 por golpe (−5 de Magos de Combate) |

3. **Disciplina:** cada queda × (1 − Disciplina ÷ 200).
4. **Sobe:** +5 com inimigo fugindo perto; +1 a cada 20 ticks com o General perto (até o inicial); +1 a cada 60 ticks vencendo o corpo a corpo.
5. **Faixas:** 50 ou mais firme; 25 a 49 abalado (−10% de Ataque); abaixo de 25 **foge** (para a borda; reagrupa com 40 sem perseguição); fugiu 2 vezes: **destroçado** (sai).

### 4.3 Formações

Linha (normal); Coluna (+30% de velocidade, −30% de Defesa); Muralha de Escudos (parado, anula a carga, −50% de velocidade); Dispersa (−50% do dano de tiro e área recebido, −20% no corpo a corpo); Cunha (Cavalaria, +25% na carga).

### 4.4 Terreno e clima

| Terreno | Efeito |
|---|---|
| Floresta | −30% do dano de tiro recebido; −30% de velocidade; sem carga |
| Rio | Só em ponte ou vau |
| Pântano | −50% de velocidade |
| Estrada | +20% de velocidade |
| Elevação (camada do Tiled) | De cima: +15% de alcance e dano de tiro; +10% no corpo a corpo morro abaixo |

Neve: −20% de velocidade; névoa: −40% de visão e alcance. O clima e a hora do começo valem a batalha toda.

### 4.5 General, Comitiva e Comandante

- **Comitiva:** se o General é o jogador, ela é o **grupo controlado**: cada companheiro luta com a Vida e o dano dele, pelas regras do [[Ficha Logica/Combate|Combate]] (o golpe dele num Esquadrão derruba soldados pelo dano ÷ 10).
- **Aura:** +10 de Moral num raio de 10 tiles. A Comitiva fugiu ou caiu: −25 em todo o exército.
- **Comandante** (companheiro ou nobre): +1% de Ataque e Defesa e +1 de Moral a cada 10 pontos da habilidade principal. Esquadrão destruído: 50% ferido, 30% capturado, 20% morto.

### 4.6 Magos e Artilharia

- **Esquadrão de Magos** (1 NPC Mago alistado; até 10% dos Magos da facção e 1 a cada 4 Esquadrões; 3× o soldo; 1 Pedra de Mana por batalha): Combate faz área (−5 de Moral); Suporte devolve 1 soldado a cada 200 ticks aos Esquadrões a 5 tiles e dá +10 de Moral em volta. Mana do Esquadrão: 100; cada feitiço gasta 20; volta 1 a cada 20 ticks. Só com a lei de Magia que permite ([[Ficha Logica/Magia|Magia]]).
- **Artilharia** (Era 2): área, 30 tiros, não anda enquanto atira; brecha na Paliçada em 400 ticks de tiro, Pedra em 1.200, Bastião em 2.400.

### 4.7 O cerco

O defensor fica atrás da **Muralha** (tiles com colisão). O atacante entra pelo Portão (o Aríete: infantaria empurrando por 1.200 ticks, sob fogo) ou por uma brecha. As tropas de tiro na Muralha: +30% de Defesa e +20% de alcance. **Vitória do atacante:** controlar a **praça central** (ponto no Tiled) por 1.200 ticks, ou afugentar a guarnição. A Milícia (2 por nível da cidade; os guardas da Escravidão) luta junto.

### 4.8 O fim da batalha

1. **Vitória:** todos os inimigos fugindo ou destroçados. **Tempo limite:** 36.000 ticks (30 min a 1x); vence o defensor.
2. **Perseguição:** se o vencedor tem Cavalaria, quem fugiu perde mais 20% dos soldados que restavam.
3. **Baixas** (os derrubados): **40% mortos, 40% feridos, 20% capturados** pelo vencedor.
   - Feridos do **vencedor** voltam em 16 dias (8 com Hospital na cidade de origem ou Magos de Suporte no exército).
   - Feridos do **perdedor** viram prisioneiros.
4. **Mortos** saem da população da cidade de origem: o número de mortos (em pessoas) entra como morte no acumulado da [[Ficha Logica/Populacao|População]] (BA1-3).
5. **Prisioneiros:** escravizar (se a lei permite; **100 = 1 NPC Escravo**, [[Ficha Logica/Escravidao|Escravidão]]), libertar (+2 de reputação com a facção deles) ou guardar para o tratado de paz (Guerra).
6. **Esquadrão com menos de 20 soldados** se desfaz; os soldados se juntam a outro do mesmo tipo ou voltam para casa.
7. **Experiência:** +1 nível a cada 2 batalhas que o Esquadrão sobreviveu.
8. O mundo avança 2 horas.

### 4.9 Reforços

Exércitos num mapa vizinho entram pela borda depois de 1.200 ticks (1 minuto de batalha) por hora de viagem até ali. Acima de 20 Esquadrões por lado, os extras entram quando um sai.

### 4.10 O cálculo rápido de exércitos

O mesmo do [[Ficha Logica/Combate|Combate]] (CB1-1), com as grandezas do Esquadrão:

1. **Dano por tick** de um Esquadrão = soldados que lutam × dano por soldado (contra a Defesa média e a armadura mais comum do outro lado) × 10% ÷ intervalo. Bônus de cultura, Experiência, Comandante; Magos de Combate × 2 (MG1-4).
2. **Poder do lado** = soma do dano por tick × soma da Vida (soldados × 10).
3. **Chance** = poder ÷ soma dos poderes; o defensor na Muralha × 1,3 no poder (BA1-4).
4. **O perdedor** foge com **45 ÷ (1 − Disciplina média ÷ 200) %** de baixas (o ponto em que a Moral chega a 25 com a BA1-1: 45% com Disciplina 0, **60% com 50**); mais 20% do resto se o vencedor tem Cavalaria.
5. **O vencedor** perde essa mesma porcentagem × poder do perdedor ÷ poder do vencedor.
6. As baixas seguem a 4.8.

## 5. Contas

**Infantaria Leve contra Infantaria Leve** (Rank 1, 100 soldados cada): dano por soldado = 8 − 4 = 4; no começo, 30 lutam: 30 × 4 × 10% = **12 por golpe** = 1,2 soldado a cada 20 ticks.

| Moral pelas baixas | Com 2% por ponto (documento) | Com 1% por ponto (BA1-1) |
|---|---|---|
| Foge (Moral abaixo de 25, de 70, Disciplina 50) | −45 ÷ 0,75 = 60 pontos → **120% de baixas: nunca foge**, luta até morrer | 60 pontos → **60% de baixas** |
| Com Disciplina 0 | 90% | **45%** |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Combate | A mesma vantagem; a Comitiva; o cálculo rápido (CB1-1) |
| Guerra (D7) | Quando há batalha; o Placar; o cerco; o suprimento; os prisioneiros no tratado |
| População | Os mortos saem pelo acumulado (BA1-3) |
| Escravidão | Os prisioneiros escravizados (100 = 1) |
| Magia | O Esquadrão de Magos |
| Companheiros | A Comitiva, o Comandante |
| Cultura | Os bônus por tipo; a Religiosa +10 de Moral |
| Tecnologia | A Era dos tipos |
| Clima, Tempo | O clima da batalha; o mundo parado; +2 horas |
| Save | Os Esquadrões (fora da batalha); o formato passa para a **versão 19** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Esquadrao` | O registro da parte 2 |
| `danoDoGolpeDoEsquadrao(atacante, alvo)` | Regra 4.1 |
| `atualizarMoral(esquadrao, causa)` | Regra 4.2 |
| `aplicarTerrenoEClima(esquadrao, tile)` | Regra 4.4 |
| `terminarBatalha(vencedor, perdedor)` | Regra 4.8 |
| `calculoRapidoDeExercitos(ladoA, ladoB)` | Regra 4.10 |

## 8. Testes

1. **A tabela da parte 5.**
2. **Infantaria Leve contra Infantaria Leve**, 1.000 vezes sem tela: quem foge tem entre 40% e 65% de baixas.
3. **Coerência:** 1.000 batalhas vistas (sem tela) e 1.000 rápidas dos mesmos exércitos: as vitórias diferem em no máximo 10 pontos percentuais; as baixas médias em no máximo 10 pontos.
4. **Lanceiros contra a carga:** a carga da Cavalaria contra Lanceiros parados não tem os +50%.
5. **Área acerta aliados.**
6. **Mortos na População:** uma batalha com 288 mortos de uma cidade tira 288 pessoas do acumulado dela.
7. **O teste de 1 ano não muda.**

## 9. Fora desta versão

- Os bonecos, os 3 tamanhos visuais, as telas (pré-batalha, posicionamento, resultado) e os controles (Interface e arte).
- O combate individual no tamanho Grande.
- As escadas e as torres de cerco.
- A Muralha no Tiled (tileset de conteúdo).

## 10. Simulação de mesa

### 10.1 Cenário

Dois Esquadrões de **Infantaria Leve**, Rank 1, Disciplina 50, sem General. Planície, Sol.

### 10.2 O choque, de 100 em 100 ticks

| Tick | Golpes até aqui (5 por 100 ticks) | Soldados de cada lado | Baixas | Moral (−1 por 1% × 0,75) |
|---|---|---|---|---|
| 0 | — | 100 | 0% | 70 |
| 100 | 5 × 1,2 = 6 | 94 | 6% | 65,5 |
| 200 | 5 × 1,13 | 88,4 | 11,6% | 61,3 |
| 400 | | cerca de 78 | 22% | 53,5 |
| 600 | | cerca de 69 | 31% | **46,8 (abalado, −10% de Ataque)** |
| 900 | | cerca de 57 | 43% | 37,8 |
| 1.250 | | cerca de 46 | 54% | 29,5 |
| 1.500 | | cerca de 40 | **60%** | **25: foge** |

O lado com a sorte melhor nos golpes foge um pouco depois e vence. O choque dura cerca de **1.500 ticks** (75 segundos a 1x), perto de uma luta de Total War.

**Com a regra do documento** (−1 a cada 2%), aos 1.500 ticks a Moral seria 47,5; ninguém fugiria e a luta seguiria até sobrar quase ninguém (1% de cada lado só por volta dos 7.000 ticks). Numa guerra, isso mataria cerca de 40% de **todos** os soldados de cada batalha.

### 10.3 Uma batalha de exércitos pelo cálculo rápido

**Guerreira** (20 Esquadrões de Infantaria Leve, 1 de Cavalaria entre eles) contra **Autoritária** (16), a guerra da Diplomacia (10.3):

- Dano por tick de cada Esquadrão: 30 × 4 × 10% ÷ 20 = 0,6.
- Guerreira: 20 × 0,6 = 12; Vida 20.000; poder **240.000**. Autoritária: 9,6; Vida 16.000; poder **153.600**.
- Chance da Guerreira: 240 ÷ 393,6 = **61%**.
- **Se vence** (Disciplina 50 nos dois lados): a Autoritária perde 60% (960 soldados) + 20% do resto pela Cavalaria (128) = **1.088**; a Guerreira perde 60% × 153,6 ÷ 240 = **38,4%** (768).
- **Baixas:** Autoritária: 435 mortos, 435 feridos (prisioneiros da Guerreira), 218 capturados → **653 prisioneiros** → 6 NPCs Escravos (a Guerreira permite com prisioneiros de guerra). Guerreira: 307 mortos, 307 feridos (voltam em 16 dias), 154 capturados pela Autoritária.
- **População:** a Autoritária perde 435 pessoas de 44.000 (**1,0%**); a Guerreira, 307 (0,7%).

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| As unidades fogem num ponto razoável? | **Só com a BA1-1:** fogem com 45% a 60% de baixas. **Achado:** com a regra do documento, um Esquadrão de Disciplina 50 nunca foge |
| A batalha dura o que deve? | Um choque de infantaria: cerca de 75 s a 1x; uma batalha de 20 Esquadrões fica dentro dos 30 minutos |
| O cálculo rápido e a batalha vista batem? | Os dois usam o mesmo dano e o mesmo ponto de fuga (60% com Disciplina 50); o teste 3 mede |
| As baixas pesam na população? | Pouco por batalha (0,7% a 1,0%); muitas batalhas seguidas pesam (a Guerra mede) |
| O teste de 1 ano muda? | **Não** |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Um lado sem nenhum Esquadrão | Não há batalha |
| Dano por soldado abaixo de 1 (Milícia contra Infantaria Pesada) | Vale 1 (como no Combate) |
| Os dois lados fogem ao mesmo tempo | Vence o defensor |
| O General é o jogador e ele cai | −25 em todo o exército; o jogador segue as regras do Combate (Inconsciente, captura) |
| Artilharia sem Bala de Canhão | Não atira; fica como um Esquadrão de Defesa 1 |
| Prisioneiros sem a lei que permite escravizar | Libertar ou guardar para o tratado |
| Um Esquadrão de 25 soldados perde 6 | Fica com 19: se desfaz |

### 10.6 Decidido em modo automático (rodada BA1, seguir sugestão)

1. **BA1-1:** a Moral cai 1 a cada 1% de baixas (não 2%): a unidade foge entre 45% e 60%.
2. **BA1-2:** no corpo a corpo lutam 30% dos soldados; o dano do golpe × 10% (o ritmo), para um choque durar cerca de 75 segundos.
3. **BA1-3:** os mortos saem da população pelo acumulado da População, em pessoas.
4. **BA1-4:** no cálculo rápido, o perdedor foge com 45 ÷ (1 − Disciplina ÷ 200) % de baixas (60% com Disciplina 50), o vencedor perde essa porcentagem × a razão dos poderes, e o defensor na Muralha tem × 1,3 no poder.
5. **BA1-5:** os tempos do documento em segundos viram ticks (1 s a 1x = 20 ticks).
