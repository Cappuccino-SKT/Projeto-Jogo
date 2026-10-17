> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Clima e Estações

**Descrição geral**

Define o que as **estações** e o **clima** mudam no jogo: a produção ao longo do ano, o clima de cada dia, os eventos climáticos extremos, o inverno e a lenha, a viagem e a guerra em cada estação, e como o dia, a noite e as estações aparecem na tela. O objetivo é dar ao ano um **ritmo**, como em RimWorld: plantar, colher, estocar para o inverno e guerrear na estação certa.

Já estava decidido e vale como base:

- O ano tem **4 estações de 16 dias** (64 dias); o dia tem 24 horas ([[Sistema de Tempo/Descrição|Tempo]]).
- Os recursos **não se esgotam**: têm **produtividade por estação** ([[Sistema de Recursos/Descrição|Recursos]]).
- Estações na arte: **só mudança de cor**; toda arte extra de estação (neve no chão, rios congelados, folhas, flores) fica para o futuro ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]).
- Seca, tempestade, geada e enchente já são eventos de Clima; o Narrador regula os que afetam o jogador ([[Sistema de Eventos/Descrição|Eventos]]).
- Cada região tem um bioma ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).

Os valores numéricos são iniciais e ficam em dados (`clima.json`) para ajustar nos testes.

---

## Produção por estação

A produção **Braçal** (a que sai direto do mapa) muda com a estação. A Manufaturada e a Especialista não mudam.

**Tabela base** (bioma temperado):

| Produção | Primavera | Verão | Outono | Inverno |
|---|---|---|---|---|
| Fazenda (grãos, legumes, fruta, algodão, fumo, especiaria, flor espiritual) | 75% | 100% | 150% (colheita) | 25% |
| Pasto, Rancho | 100% | 100% | 100% | 75% |
| Posto de Coleta: madeira | 100% | 100% | 100% | 75% |
| Posto de Coleta: pedra, argila, minérios, carvão e pedra mágica | 100% | 100% | 100% | 90% |

- O Posto de Coleta junta madeira e minérios; por isso a tabela vale **por produto**, não por construção. **[Proposta]** A Água rende 100% o ano todo.
- **Média anual da Fazenda** na tabela base: 87,5%. Quem não guarda comida no outono passa fome no inverno.

**Ajustes por bioma** (só o que muda):

| Bioma (região) | Mudança |
|---|---|
| Montanhas frias e taiga (Guerreira) | Inverno: Fazenda 0%, Pasto 50% |
| Terras áridas e cânions (Tecnocrata) | Verão: Fazenda 75% (calor); Inverno: Fazenda 50% |
| Litoral quente (Escravista) | Inverno: Fazenda 75% |
| Planície, colinas e floresta (Autoritária, Religiosa, Arcana) | Tabela base |

*Exemplo:* a Guerreira, que já tem pouca comida, não colhe nada no inverno. Ela depende ainda mais do comércio ou da guerra pelo celeiro Autoritário (1,3), o que reforça o motivo de guerra que o [[Mundo e Estado Inicial/Descrição|Mundo]] já previa.

- **Preços:** a comida fica mais cara no inverno **pela própria simulação** (menos oferta, a mesma demanda); nenhum evento força isso ([[Sistema Econômico/Descrição|Econômico]]).
- A tecnologia e o nível da construção continuam aumentando a produtividade, por cima da estação.

## Clima do dia

O clima é sorteado **1 vez por dia por região** (6 sorteios por dia, baratos e coerentes entre mapas vizinhos), com **60% de chance de repetir** o do dia anterior.

| Clima | Viagem | Combate e Batalha | Outros |
|---|---|---|---|
| Limpo | × 1,0 | — | — |
| Nublado | × 1,0 | — | — |
| Chuva | × 1,25 | Alcance de tiro −15%; Atiradores −30% de dano (pólvora molhada) | — |
| Tempestade | × 1,5; caravanas param | Alcance de tiro −30%; Atiradores não atiram | NPCs ficam em casa |
| Neve | × 1,5 | Velocidade −20% | Lenha × 1,5 (ver "Inverno e aquecimento") |
| Névoa | × 1,0 | Alcance de visão e de tiro −40% | Detecção de crime −20% |
| Calor forte | × 1,1 | — | Felicidade −2 |

- **Combate local** usa os mesmos efeitos de tiro e visão ([[Sistema de Combate/Descrição|Combate]]). **Batalha:** como o mundo para durante ela, o clima do início **vale a batalha toda** ([[Sistema de Batalha/Descrição|Batalha]]).
- **Tempestade, "NPCs ficam em casa":** no Período Livre ninguém vai à Taverna nem ao Mercado. **[Proposta]** O trabalho ao ar livre (Fazenda, Pasto, Rancho, Posto de Coleta) rende −50% naquele dia.
- **Calor forte:** −2 de Felicidade enquanto durar, como modificador com causa.

**[Proposta] Chances por estação** (bioma temperado, em %):

| Estação | Limpo | Nublado | Chuva | Tempestade | Neve | Névoa | Calor forte |
|---|---|---|---|---|---|---|---|
| Primavera | 35 | 25 | 30 | 5 | — | 5 | — |
| Verão | 50 | 20 | 15 | 5 | — | — | 10 |
| Outono | 30 | 30 | 25 | 5 | — | 10 | — |
| Inverno | 25 | 30 | — | 5 | 30 | 10 | — |

**[Proposta] Ajustes por bioma:** montanhas frias com neve também no outono (15%) e na primavera (10%) e neve 50% no inverno; terras áridas com um terço da chuva e calor forte 30% no verão; litoral com o dobro de tempestades e chuva no lugar da neve; floresta enevoada (Arcana) com o dobro de névoa.

**Visual:** uma **camada por cima do mapa**, com partículas (pingos, flocos), **névoa translúcida** e um **tom de cor**. São poucos sprites pequenos, sem tiles novos.

- *Conflito resolvido:* o Pipeline decidiu "estações só com cor". A camada de clima não é arte de tile: **neve caindo sim**, **neve no chão continua para o futuro**.

## Eventos climáticos extremos

| Evento | Quando e onde | Efeito | Duração |
|---|---|---|---|
| **Seca** | Verão; terras áridas e planície | Fazenda −50% | 1 estação |
| **Enchente** | Primavera; mapas com rio | Fazenda −30%; perde 10% do estoque de comida da cidade | 4 dias |
| **Geada fora de hora** | Primavera ou outono; montanhas e colinas | Fazenda perde 50% da produção da estação | Pontual |
| **Tempestade forte** | Qualquer estação; litoral | Caravanas paradas; danifica construções de madeira (custo de reparo pela manutenção) **[Proposta]** | 2 dias |
| **Onda de calor** | Verão; terras áridas e litoral | Felicidade −5; chance de doença maior | 4 dias |
| **Inverno rigoroso** | Inverno; o mundo todo (cerca de 1 vez a cada 5 anos) | Efeitos do inverno × 1,5; Lenha × 2; mortalidade de velhos maior | A estação inteira |

- São **eventos ocasionais** da categoria Clima, com a chance base e os modificadores no JSON de eventos. O Narrador continua regulando os que afetam o jogador ([[Sistema de Eventos/Descrição|Eventos]]).
- O inverno rigoroso tem escopo **Mundo**; os outros, escopo **Cidade/Mapa Local** ou região.
- **[Proposta]** "Efeitos do inverno × 1,5" multiplica as perdas: a Fazenda temperada cai de 25% para cerca de 17%, e o multiplicador de viagem do inverno sobe 50%.

## Inverno e aquecimento

No inverno, as moradias gastam **lenha** (Madeira Bruta):

- Cada **NPC real** consome **1 Madeira Bruta a cada 4 dias** como "aquecimento", dentro do consumo de Materiais da Classe Social. Vale para **todas as classes**, inclusive os Escravos na Senzala ([[Sistema de Classe Social/Descrição|Classe Social]]).
- **Por bioma:** frio × 2; temperado × 1; litoral quente e terras áridas × 0,25.
- **Neve** no dia: × 1,5. **Inverno rigoroso:** × 2.
- **Falta de lenha:** Felicidade −10 (pelo fator Consumo atendido) e mortalidade de velhos e crianças **+50%** na estação ([[Sistema de População/Descrição|População]]).

*Exemplo:* uma cidade Guerreira de 100 NPCs reais no inverno de 16 dias gasta 4 × 100 × 2 = **800 de Madeira Bruta**, que precisa estar estocada no outono.

## Estações na viagem e na guerra

**Viagem** ([[Sistema de Viagem/Descrição|Viagem]]):

| Estação | Multiplicador |
|---|---|
| Primavera | × 1,1 em mapas sem estrada (lama) |
| Verão e Outono | × 1,0 |
| Inverno | × 1,5 nos biomas frios; × 1,1 nos outros |

- A estação e o clima do dia **se multiplicam**, com teto de **× 2,0**. *Exemplo:* neve (× 1,5) no inverno da montanha (× 1,5) daria × 2,25, mas fica em × 2,0.

**Campanha de inverno** ([[Sistema de Guerra/Descrição|Guerra]]):

- Um exército **fora de cidade amiga** gasta **1,25 Ração Militar por Esquadrão por dia**.
- Nos biomas frios, **Moral −1 por dia**.
- **Cansaço de guerra +5** por estação de campanha no inverno.

**IA:** a vontade de guerra cai **15 pontos no inverno** ([[Sistema de Diplomacia/Descrição|Diplomacia]]). As guerras ganham um ritmo: campanhas na primavera e no verão, paz e reconstrução no inverno.

## Dia, noite e estações na tela

- **Estações:** troca de paleta na placa de vídeo, como a das culturas (sem memória extra): primavera verde vivo, verão quente, outono alaranjado, inverno frio e dessaturado. A troca é gradual nos 2 primeiros dias da estação. **[Proposta]**
- **Dia e noite:** um tom de cor sobre o mapa em 4 fases, sem luzes dinâmicas na primeira versão:

| Fase **[Proposta]** | Horas |
|---|---|
| Amanhecer | 5 h às 7 h |
| Dia | 7 h às 18 h |
| Entardecer | 18 h às 20 h |
| Noite | 20 h às 5 h |

- **À noite:** visão e detecção **−30%**, no crime e no combate ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).
- **Futuro:** tochas e janelas acesas, e a arte extra das estações (já decidido para depois).
- Entra na **fatia vertical**: é barato e dá vida ao mapa.

---

## Parte técnica

- **Dados:** `clima.json` com a produção por estação e por bioma, as chances de clima por bioma e estação, os efeitos de cada clima, o aquecimento, os multiplicadores de viagem e de campanha, as paletas das estações e as fases do dia. Os eventos extremos ficam no JSON de eventos, na categoria Clima.
- **Frequência:** 1 sorteio de clima por dia por região; a produtividade muda no 1º dia de cada estação; a lenha é descontada junto com o consumo da cidade (por dia em LOD 0 e 1, por estação em LOD 2).
- **Aleatoriedade com semente**, como o resto do jogo: a mesma semente dá o mesmo clima.
- **Desempenho:** a camada de clima é desenhada em lote, com um limite de partículas na tela (**[Proposta]** 300); o tom do dia e a paleta das estações são feitos na placa de vídeo, sem custo de memória.
- **Save:** clima atual de cada região, eventos extremos ativos e o ano do último inverno rigoroso.
- **Depuração:** painel (Dear ImGui) para forçar um clima ou uma estação.
- **Fatia vertical:** entram a **produção por estação** (testa os estoques e a fome) e o **tom de dia e noite**. O clima do dia fica para depois.

## Ligação com outros sistemas

| Sistema | O que o Clima e Estações fornece ou recebe |
|---|---|
| Tempo | Estações de 16 dias; fases do dia pelas horas |
| Recursos e Produção | Produtividade Braçal por estação e por bioma; perdas dos eventos extremos |
| Classe Social | Lenha no inverno, dentro do consumo de Materiais |
| População | Mortalidade por frio e eventos de clima; fome no inverno sem estoque |
| Felicidade | Falta de lenha −10; calor forte −2; onda de calor −5 |
| Comércio e Econômico | Preço da comida sobe no inverno pela simulação; tempestade para as caravanas |
| Viagem | Multiplicadores do clima do dia e da estação, com teto de × 2,0 |
| Combate e Batalha | Tiro, visão e velocidade pelo clima; à noite, visão −30%; o clima do início vale a batalha toda |
| Guerra e Diplomacia | Campanha de inverno mais cara; IA com −15 de vontade de guerra no inverno |
| Crime e Justiça | Noite (−30%) e névoa (−20%) reduzem a detecção |
| Eventos | Eventos climáticos extremos, regulados pelo Narrador |
| Construção | Tempestade forte danifica construções de madeira |
| Mundo e Estado Inicial | Bioma de cada região define os ajustes |
| Pipeline e Interface | Paleta por estação, tom do dia, partículas e névoa; clima do dia no HUD |
| Notícias | Eventos extremos viram notícia Regional (inverno rigoroso, Mundial) |

## Perguntas em aberto

1. **Valores a testar:** produção por estação e bioma, chances de clima, efeitos no combate, lenha por NPC, multiplicadores de viagem e de campanha.
2. **Arte extra das estações** (neve no chão, rios congelados, folhas e flores): quando entra.
3. **Magia e clima:** feitiços de Suporte que mudam o clima ou protegem a colheita ([[Sistema de Magia/Descrição|Magia]]).
4. **Roupas de inverno:** se, no futuro, a falta de roupa também pesa no frio.
5. **Luzes dinâmicas** (tochas, janelas) e o custo delas no desempenho.
