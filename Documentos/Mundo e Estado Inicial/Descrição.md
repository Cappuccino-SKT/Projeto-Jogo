> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Mundo e Estado Inicial

**Descrição geral**

Este documento define **como o mundo está no primeiro dia da partida**: a disposição das regiões e cidades, os biomas e recursos, as masmorras, as relações entre as facções, o calendário, o que varia de uma partida para outra e os pontos de partida do jogador.

Ele **não** traz a história do mundo. O autor decidiu deixar a história, os nomes e os personagens principais para depois, quando a história do jogo for escrita. Até lá:

- tudo usa **nomes significativos**, que dizem o que a coisa representa: "Facção Guerreira", "Capital Arcana", "Vila Livre do Norte";
- os personagens são **abstratos** (papéis como "o Arquimago", sem nome nem biografia);
- os nomes definitivos entram aos poucos, sem mudar nenhuma regra (os IDs internos dos dados não mudam quando o nome muda).

### Tom e raças

- **Tom:** fantasia de **baixa magia**, com a tecnologia em transição para a **pólvora**. Sombrio e político, mais perto de Mount & Blade do que de um conto de fadas.
- **Criaturas:** na primeira versão, só **animais e monstros comuns** (lobos, ursos, bandidos, aranhas gigantes). **Criaturas míticas** (dragões, mortos-vivos) podem aparecer no futuro, **dentro das masmorras**.
- **Raças:** só **humanos** nas primeiras versões, diferenciados pelas 6 culturas. Outras raças podem entrar no futuro. Para não fechar essa porta, os dados do NPC já guardam um campo "Raça" (com um único valor, Humano), assim acrescentar raças depois não exige mudar o formato do save.

### Disposição do mundo

O mundo tem **48 Mapas Locais**, numa malha de **8 colunas × 6 linhas**. O tamanho pode mudar no futuro; por enquanto fica assim.

São **6 regiões de 8 mapas** (blocos de 4 × 2), arrumadas em **2 colunas × 3 linhas** de blocos. (A sugestão original dizia "3 colunas por 2 linhas", mas essa conta dá 12 × 4; para manter a malha 8 × 6, o arranjo certo é 2 × 3.)

| | Oeste (colunas 1 a 4) | Leste (colunas 5 a 8) |
|---|---|---|
| **Norte** (linhas 1 e 2) | Região Guerreira | Região Arcana |
| **Centro** (linhas 3 e 4) | Região Autoritária | Região Religiosa |
| **Sul** (linhas 5 e 6) | Região Tecnocrata | Região Escravista |

Por que essa ordem: as culturas **hostis** entre si ficam vizinhas, para as fronteiras gerarem conflito desde o início (Guerreira × Autoritária, Arcana × Religiosa, Religiosa × Escravista). Tecnocrata e Arcana, as mais opostas, ficam em cantos distantes: o conflito entre elas é ideológico e comercial, não de fronteira.

Bordas do mundo: ao **norte**, montanhas intransponíveis; a **leste** e ao **sul**, o mar; a **oeste**, estepe e deserto. Ninguém atravessa as bordas.

#### Mapa inicial

Cada célula é um Mapa Local, na posição (coluna, linha). Abreviações: Gue = Guerreira, Arc = Arcana, Aut = Autoritária, Rel = Religiosa, Tec = Tecnocrata, Esc = Escravista. **M1** a **M4** = masmorra com essa dificuldade; **ML** = masmorra lendária.

| | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| **1** | **Capital Gue** | Terr. Gue | Terr. Gue · M2 | Vila livre | Livre · ML | Terr. Arc · M2 | Terr. Arc | **Capital Arc** |
| **2** | Disputa Gue/Aut | Cidade Gue | Vilarejo Gue | Terr. Gue · M4 | Terr. Arc | Vilarejo Arc | Cidade Arc | Terr. Arc |
| **3** | Vilarejo Aut | Cidade Aut | Disputa Gue/Aut | Vila livre | Livre | Disputa Arc/Rel | Cidade Rel | Terr. Rel · M1 |
| **4** | **Capital Aut** | Terr. Aut · M1 | Livre · ML | Terr. Tec | Livre · M3 | Disputa Rel/Esc | Vilarejo Rel | **Capital Rel** |
| **5** | Terr. Aut | Terr. Tec | Terr. Tec | Vilarejo Tec | Disputa Tec/Esc · ML | Vilarejo Esc | Cidade Esc | Disputa Rel/Esc |
| **6** | Terr. Tec · M1 | **Capital Tec** | Cidade Tec | Terr. Tec · M2 | Vila livre | Vilarejo Esc | Terr. Esc · M1 | **Capital Esc** |

Os estados de território (Terr., Disputa, Livre) **não são guardados**: saem das cidades vizinhas, como manda o [[Sistema de Mapa/Descrição|Sistema de Mapa]]. A tabela mostra o resultado no primeiro dia. Note que um mapa pode pertencer a uma região (cultura do terreno) e a outra facção: o mapa (4,4) fica na Região Autoritária, mas é território da Facção Tecnocrata, porque encosta no Vilarejo Tecnocrata.

**Contagem no primeiro dia:**

| Estado | Mapas | % |
|---|---|---|
| Com cidade de facção (6 capitais, 6 cidades, 7 vilarejos) | 19 | 40% |
| Com cidade independente (Vila livre) | 3 | 6% |
| Território de facção | 16 | 33% |
| Em disputa | 6 | 13% |
| Sem dono (Livre) | 4 | 8% |

**Mapas sem dono: cerca de 8%.** A sugestão inicial pedia cerca de 30%, mas isso não cabe em 48 mapas com 3 a 4 cidades por região (cada cidade de facção transforma até 4 vizinhos em território). O autor decidiu ficar com os 48 mapas e os 8%: os territórios de facção continuam exploráveis (masmorras, bandidos, recursos) e permitem fundar assentamento vinculado, e as 3 Vilas Livres completam o espaço sem facção. O mundo pode crescer depois dos testes; por isso o tamanho da malha vem dos dados, não do código (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).

#### Cidades iniciais

Cada região tem 1 capital (nível 4), 1 cidade (nível 3) e 1 vilarejo (nível 2); a Escravista tem 2 vilarejos (o segundo é um porto de pesca e de escravos). Os nomes são provisórios.

| Facção | Capital (nível 4) | Cidade (nível 3) | Vilarejo(s) (nível 2) |
|---|---|---|---|
| Guerreira | (1,1), fortaleza na montanha | (2,2), cidade mineira | (3,2), fronteira leste |
| Arcana | (8,1), cidade das torres | (7,2), cidade da floresta | (6,2), posto da Flor Espiritual |
| Autoritária | (1,4), sede do império, no rio | (2,3), cidade-fortaleza da fronteira norte | (1,3), celeiro |
| Religiosa | (8,4), cidade santa | (7,3), cidade de peregrinação | (7,4), vilarejo de vinhedos |
| Tecnocrata | (2,6), cidade das oficinas | (3,6), cidade das minas de cobre | (4,5), entreposto do leste |
| Escravista | (8,6), grande porto | (7,5), cidade dos mercados | (6,5), plantação de algodão; (6,6), porto de pesca |

**Cidades independentes (sem facção):** 3 Vilas Livres pequenas, para o jogador e o estilo Dono de vila. Elas não geram território (os mapas vizinhos continuam livres).

| Cidade | Posição | Nível | Ideia |
|---|---|---|---|
| Vila Livre do Norte | (4,1) | 2 | Entre a Guerreira e a Arcana; vive de mercenários e caça |
| Vila Livre do Centro | (4,3) | 1 | No meio do mundo, encruzilhada de caravanas; refúgio de exilados |
| Vila Livre do Sul | (5,6) | 1 | Vila de contrabandistas, colada nos domínios da Escravista |

### Biomas e recursos

Cada região tem um bioma ligado à sua cultura, 2 a 3 recursos de destaque e falta de outros, para obrigar o comércio. Distribuição concreta, usando a lista de [[Sistema de Recursos/Recursos Existentes|Recursos Existentes]]:

| Região | Bioma | Recursos de destaque (produtividade alta) | Escassos (pouco ou nenhum) |
|---|---|---|---|
| Guerreira | Montanhas frias e taiga | Ferro, Carvão, Cavalos | Grão, Fruta, Algodão, Especiaria |
| Arcana | Floresta antiga e enevoada | Pedra de Mana, Flor Espiritual, Madeira | Ferro, Cobre, Carvão |
| Autoritária | Planície fértil cortada por um grande rio | Grão, Pedra, Vacas | Pedra de Mana, Especiaria, Ouro |
| Religiosa | Colinas e vales temperados | Fruta, Ovelhas, Argila | Carvão, Ferro, Folha de Fumo |
| Tecnocrata | Terras áridas e cânions | Cobre, Ouro, Carvão | Grão, Madeira, Flor Espiritual |
| Escravista | Litoral quente | Algodão, Especiaria, Folha de Fumo | Ferro, Pedra de Mana, Pedra |

- **Peixe** (revisão V1-4): destaque no litoral Escravista, com o porto de pesca (6,6), e no grande rio Autoritário (ver [[Sistema de Recursos/Descrição|Recursos]]).
- **Bioma e clima:** o bioma muda a produção por estação, as chances de clima e a lenha no inverno (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Recursos básicos** (Água, Madeira, Pedra, Legumes, Galinha, Porcos) existem em quase todos os mapas, com produtividade normal; só o destaque e a escassez mudam por região.
- **Exemplo de comércio que nasce disso:** a Guerreira tem ferro e cavalos, mas pouca comida; o maior celeiro é a Autoritária, sua inimiga. Ela precisa comprar grão por intermédio da Escravista ou atacar o celeiro (1,3), o que dá motivo real para a guerra na fronteira.
- **Recursos por Mapa Local:** cada mapa tem de 3 a 6 recursos; os de destaque aparecem em mais mapas da região e com produtividade maior. A lista exata fica em dados, desenhada junto com o mapa no Tiled.

### Masmorras

São **12 masmorras**, 2 por região, das quais **3 lendárias**, em regiões diferentes. A dificuldade cresce com a distância das capitais.

Regra: dificuldade = distância (em Mapas Locais, contando só passos na horizontal e na vertical) até a capital mais próxima, de 1 a 4. As lendárias têm sempre dificuldade 5. O tipo de cada uma é só descritivo (os nomes vêm com a história).

| # | Posição | Região | Estado do mapa | Dificuldade | Tipo (provisório) |
|---|---|---|---|---|---|
| 1 | (3,1) | Guerreira | Território | 2 | Mina abandonada |
| 2 | (4,2) | Guerreira | Território | 4 | Fortaleza em ruínas |
| 3 | (6,1) | Arcana | Território | 2 | Torre de mago caída |
| 4 | (5,1) | Arcana | Sem dono | **Lendária** | Santuário arcano selado |
| 5 | (2,4) | Autoritária | Território | 1 | Catacumbas sob o rio |
| 6 | (3,4) | Autoritária | Sem dono | **Lendária** | Cidade antiga soterrada |
| 7 | (8,3) | Religiosa | Território | 1 | Cripta de um mosteiro |
| 8 | (5,4) | Religiosa | Sem dono | 3 | Templo profanado |
| 9 | (1,6) | Tecnocrata | Território | 1 | Oficina abandonada |
| 10 | (4,6) | Tecnocrata | Território | 2 | Cânion dos bandidos |
| 11 | (7,6) | Escravista | Território | 1 | Gruta de contrabandistas |
| 12 | (5,5) | Escravista | Em disputa | **Lendária** | Navio encalhado e cavernas |

Resultado: 4 masmorras de dificuldade 1, 3 de 2, 1 de 3, 1 de 4 e 3 lendárias. As fáceis ficam perto das capitais (boas para o começo do jogo); as lendárias, no meio do mundo, longe de todos. Inimigos e Loot são sorteados a cada partida (ver [[Objetivo e Progressão/Descrição|Objetivo e Progressão]]).

### Calendário

- A partida começa no **Ano 412**, no **1º dia da Primavera**.
- Os **marcos de tecnologia** contam os anos desde o início da partida: o ano 1 da partida é o Ano 412. Um marco com janela "do ano 3 ao ano 5" chega entre os Anos 414 e 416.
- Lembrete: 1 ano = 64 dias de jogo (4 estações de 16 dias), cerca de 17 horas reais a 1x (ver [[Sistema de Tempo/Descrição|Sistema de Tempo]]).

### Facções no início

Começam as **6 facções principais** e as **3 Vilas Livres** (sem facção).

#### Equilíbrio

Todas começam na **Era 1** de tecnologia. O PIB pode variar até ±30% entre as facções, e nenhuma pode ter mais que o dobro de outra.

Valores iniciais (a testar), com a ideia de que **cada facção é forte numa coisa e fraca em outra**:

| Facção | NPCs reais | População (Fator 100) | Riqueza | PIB inicial | Esquadrões (1 = 100 soldados) | Ponto forte |
|---|---|---|---|---|---|---|
| Guerreira | 440 | 44.000 | × 0,85 | 23.375 | 20 | Exército grande e cavalos |
| Arcana | 440 | 44.000 | × 0,95 | 26.125 | 10 | Magos e Pedra de Mana |
| Autoritária | 440 | 44.000 | × 1,15 | 31.625 | 16 | Comida e ordem (Controle alto) |
| Religiosa | 440 | 44.000 | × 1,00 | 27.500 | 13 | Felicidade estável e cura |
| Tecnocrata | 440 | 44.000 | × 1,05 | 28.875 | 11 | Pesquisa e cobre/ouro |
| Escravista | 480 | 48.000 | × 1,20 | 34.800 | 14 | Dinheiro e comércio |

Como foi calculado:

- **NPCs reais:** capital 300 + cidade 100 + vilarejo 40 (a Escravista tem 2 vilarejos).
- **Dinheiro inicial de cada cidade:** nível 4 = 20.000; nível 3 = 6.000; nível 2 = 1.500; nível 1 = 400 moedas, multiplicado pela "Riqueza" da cultura. O PIB é a soma (ver [[Sistema de Facção/Descrição|Sistema de Facção]]).
- **Banco da Facção inicial:** 25% do PIB.
- **Esquadrões iniciais:** capital 8, cidade 4, vilarejo 1, multiplicado pela tradição militar (Guerreira × 1,5; Autoritária × 1,25; Escravista e Religiosa × 1; Tecnocrata × 0,85; Arcana × 0,75).
- **Conferência:** a mais rica (Escravista, +21% da média) e a mais pobre (Guerreira, −19%) ficam dentro dos ±30%, e a diferença entre elas é de 1,5 vez.

#### Leis e tecnologia iniciais

- **Leis:** cada facção começa com as leis desejadas da própria cultura (ver [[Sistema de Cultura/Descrição|Sistema de Cultura]]).
- **Tecnologia:** todas na Era 1. Cada cultura tem as suas tecnologias iniciais fixas e recebe mais **2 ou 3 tecnologias extras sorteadas** de uma lista própria. As listas ficam no [[Sistema de Tecnologia/Descrição|Sistema de Tecnologia]] **[Em aberto]**.

#### Construções iniciais

Cada cidade começa com um **pacote de construções** por nível e cultura, lido de arquivo de dados. Pacotes:

| Nível | Construções do pacote base |
|---|---|
| 1 (Assentamento) | Fazenda, Posto de Coleta e moradias |
| 2 (Vilarejo) | Nível 1 + Pasto, Cozinheiro, Carpinteiro, Mercado e Taverna |
| 3 (Cidade) | Nível 2 + Pedreira, Ferreiro, Quartel, Templo e Prisão |
| 4 (Metrópole) | Nível 3 + Palácio, Universidade, Casa da Moeda, Hospital, Armeiro, Costureiro, Metalurgia e Posto Comercial |

| Cultura | Troca ou acréscimo nas cidades de nível 3 e 4 |
|---|---|
| Guerreira | Quartel 1 nível acima; + Armeiro já no nível 3 |
| Arcana | Templo trocado pela Torre Arcana; + Encantaria no nível 4 |
| Autoritária | Quartel e Prisão 1 nível acima |
| Religiosa | Templo 1 nível acima; + Boticário |
| Tecnocrata | Templo trocado pelo Laboratório |
| Escravista | + Senzala grande; + Posto Comercial já no nível 3; + Mercado de Escravos na Capital (8,6), na Cidade (7,5) e no porto (6,6) (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) |

As **moradias** não entram no pacote: a quantidade sai da população inicial (ver [[Sistema de População/Descrição|Sistema de População]]). Os níveis exatos de cada construção ficam para a lista de construções novas **[Em aberto]**.

### Relações iniciais

Afinidade entre as culturas (afim, neutra ou hostil, ver [[Sistema de Cultura/Descrição|Sistema de Cultura]]):

| | Gue | Arc | Aut | Rel | Tec | Esc |
|---|---|---|---|---|---|---|
| **Guerreira** | — | neutra | **hostil** | neutra | neutra | afim |
| **Arcana** | neutra | — | neutra | **hostil** | **hostil** | neutra |
| **Autoritária** | **hostil** | neutra | — | neutra | afim | afim |
| **Religiosa** | neutra | **hostil** | neutra | — | neutra | **hostil** |
| **Tecnocrata** | neutra | **hostil** | afim | neutra | — | neutra |
| **Escravista** | afim | neutra | afim | **hostil** | neutra | — |

Por quê: a Guerreira despreza a obediência imperial da Autoritária, mas vende prisioneiros de guerra à Escravista; a Religiosa condena a magia da Arcana e o tráfico de pessoas e de Fumo da Escravista; a Tecnocrata proíbe a magia; a Autoritária valoriza a ordem da Tecnocrata e o trabalho barato da Escravista.

**Valor inicial de relação** (−100 a +100) de cada par em contato:

- base pela afinidade: afim **+40**, neutra **0**, hostil **−40**;
- **−15** se as duas dividem um mapa em disputa;
- desvio sorteado de **−15 a +15** a cada partida.

**Contato:** as facções vizinhas se conhecem desde o início. A **Escravista**, por ser mercantil, tem contato com todas (rede de comércio). A Arcana e a Tecnocrata não se conhecem no primeiro dia.

| Par | Valor base | Atitude inicial |
|---|---|---|
| Guerreira × Autoritária | −55 | Hostil, candidata a guerra |
| Arcana × Religiosa | −55 | Hostil, candidata a guerra |
| Religiosa × Escravista | −55 | Hostil, candidata a guerra |
| Tecnocrata × Escravista | −15 | Fria (disputa um mapa) |
| Guerreira × Arcana | 0 | Neutra |
| Autoritária × Religiosa | 0 | Neutra |
| Escravista × Arcana | 0 | Neutra |
| Autoritária × Tecnocrata | +40 | Amigável, candidata a acordo |
| Escravista × Guerreira | +40 | Amigável, candidata a acordo |
| Escravista × Autoritária | +40 | Amigável, candidata a acordo |

**Guerras iniciais:** sorteia-se **1 guerra (70% das partidas) ou 2 (30%)** entre os três pares candidatos. O par em guerra começa com relação −70. Os outros pares começam em Paz; a atitude é a faixa da relação (ver [[Sistema de Diplomacia/Descrição|Sistema de Diplomacia]]).

**Acordos comerciais iniciais:** sorteiam-se **2** entre os pares candidatos que, depois do desvio, ficaram com relação de +25 ou mais. Acordo comercial zera a Tarifa entre os dois (ver [[Sistema de Comércio/Descrição|Sistema de Comércio]]).

### Líderes e NPCs Importantes

Nas primeiras versões os personagens são **abstratos**. Os personagens principais do mundo entram aos poucos, quando a história for escrita.

- **Gerados no início de cada partida** (por facção): 1 líder (Realeza), 1 herdeiro e 2 ou 3 nobres da dinastia; o governador da cidade de nível 3 é um desses nobres. Os nomes saem de uma lista por cultura e a Personalidade, dos modelos, com a tendência da cultura.
- 5 **Importantes fixos**, por papel, cada um ligado a uma Ambição:

| Papel | Onde começa | Ligação |
|---|---|---|
| O Arquimago | Capital Arcana (8,1) | Ambição "Tornar-se Arquimago" |
| O Rei Exilado | Vila Livre do Centro (4,3) | Ambição "Retomar o trono"; dono de uma dinastia caída |
| O Grande Mercador | Cidade Escravista (7,5) | Ambição "Ter a maior fortuna do mundo" |
| O Sumo Sacerdote | Capital Religiosa (8,4) | Missões de peregrinação e de heresia; Bênção e Anátema; escolhe o herdeiro da Religiosa (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| O Mestre Inventor | Capital Tecnocrata (2,6) | Missões de pesquisa e da pólvora |

### O que varia por partida

O mundo é fixo; o estado inicial varia. Limites:

| Varia | Como |
|---|---|
| Relações | Desvio de −15 a +15 em cada par |
| Guerras iniciais | 1 ou 2, sorteadas entre os pares candidatos |
| Acordos comerciais | 2, sorteados entre os pares candidatos |
| Tecnologias extras | 2 ou 3 por cultura, de uma lista própria |
| Loot e inimigos das masmorras | Sorteados das tabelas, pela dificuldade |
| Semente do Narrador | Nova a cada partida |
| Personalidade dos líderes e nobres | Sorteada dos modelos, com tendência da cultura |
| Preços iniciais | ±10% por item em cada cidade |

**Fixo:** mapas, posição e nível das cidades, culturas, regiões, recursos de cada mapa, masmorras (posição e dificuldade) e os Importantes fixos.

### Pontos de partida

Cada estilo de início tem 2 ou 3 pontos fixos; o jogador escolhe um, ou deixa sortear.

| Estilo | Pontos de partida |
|---|---|
| Dono de vila | Território Arcano (5,2), território Tecnocrata (2,5) ou o mapa livre (5,3) |
| Aventureiro | Vila Livre do Norte (4,1), Vila Livre do Centro (4,3), Cidade Religiosa (7,3) |
| Membro de facção | A capital da cultura escolhida |
| Mercador | Cidade Escravista (7,5), Cidade Autoritária (2,3), Vila Livre do Sul (5,6) |
| Nobre exilado | Vila Livre do Centro (4,3), Vila Livre do Sul (5,6) |
| Escravo | Cidade Escravista (7,5), Capital Escravista (8,6) |
| Mago | Capital Arcana (8,1), Cidade Arcana (7,2), Vila Livre do Norte (4,1) |

O estilo Dono de vila começa com o assentamento **já fundado** no ponto escolhido, e o jogador escolhe entre dois jeitos de jogar:

- **Em território de facção** (5,2) ou (2,5): nasce **vinculado** à facção, paga impostos e segue as leis dela. Para cumprir a Ambição "Fundar uma facção", precisa chegar ao nível 3 e romper com a facção, o que pode virar revolta.
- **No mapa livre** (5,3): nasce **independente**, mais tranquilo, entre a Arcana e a Religiosa.

### História do mundo

**[Em aberto]** Fica para depois, quando o autor começar a escrever a história do jogo: nomes das regiões, culturas, facções, cidades e personagens, a história antes do Ano 412 e o porquê das guerras. Quando for escrita, vira o documento **Mundo e História**.

### Parte técnica

- **Dados** (pasta `data/mundo/`), em JSON:
  - `mapas.json`: os 48 Mapas Locais (posição, região, bioma, recursos, arquivo do Tiled);
  - `cidades_iniciais.json`: cidade, posição, nível, facção, dinheiro e pacote de construções;
  - `faccoes_iniciais.json`: riqueza, Esquadrões, leis e tecnologias fixas;
  - `relacoes.json`: afinidades, pares candidatos a guerra e a acordo, e os limites do sorteio;
  - `masmorras.json`: posição, dificuldade e tabela de Loot;
  - `pontos_partida.json`: pontos por estilo de início.
- **Exemplo de entrada:**

```json
{ "id": "cidade.capital_guerreira", "nome": "Capital Guerreira",
  "mapa": [1, 1], "nivel": 4, "faccao": "faccao.guerreira",
  "dinheiro": 17000, "pacote": "pacote.nivel4_guerreira" }
```

- **Geração do estado inicial:** a partir de uma **semente**, o jogo aplica os sorteios (relações, guerras, acordos, tecnologias, Loot, Personalidade, preços) e grava o resultado no save. A mesma semente gera o mesmo mundo, o que ajuda a reproduzir bugs.
- **Save:** guarda a semente e o resultado dos sorteios; os dados fixos vêm dos arquivos.
- **Validação ao carregar:** cada cidade num mapa existente, no máximo uma cidade por mapa, regiões com 8 mapas, nenhum PIB fora dos ±30%.
- **Fatia vertical:** não usa este documento (1 cidade, sem facções). Ele entra na camada "mundo".

### Ligação com outros sistemas

| Sistema | O que o Mundo e Estado Inicial fornece ou recebe |
|---|---|
| Mapa | Malha 8 × 6, regiões, biomas e bordas; estados de território do primeiro dia |
| Cidade | Cidades iniciais, níveis, dinheiro e pacotes de construções |
| Cultura | Região de cada cultura; afinidades entre culturas |
| Facção | Facções iniciais, PIB, Banco, Esquadrões, relações, guerras e acordos iniciais |
| Recursos | Recursos de destaque e escassos por região |
| Tecnologia | Era 1 para todos; tecnologias extras sorteadas; calendário dos marcos |
| Tempo | Ano 412, 1º dia da Primavera |
| Objetivo e Progressão | 12 masmorras (3 lendárias); Importantes fixos ligados às Ambições |
| Início de Jogo | Pontos de partida por estilo |
| Comércio | Escassez que cria rotas; acordos comerciais iniciais |
| Eventos | A semente do Narrador é sorteada aqui |

### Perguntas em aberto

1. **História, nomes e personagens:** depois, com a história do jogo.
2. **Listas de tecnologias** iniciais e extras por cultura (Sistema de Tecnologia).
3. **Valores a testar:** dinheiro inicial por nível, Esquadrões, desvio das relações, chance de 1 ou 2 guerras.
