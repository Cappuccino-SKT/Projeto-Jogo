> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Religião e Fé

**Descrição geral**

Este documento é um **complemento do [[Sistema de Cultura/Descrição|Sistema de Cultura]]**, não um sistema à parte: a religião de um NPC é a cultura dele. Aqui ficam só as **mecânicas da fé**: a Devoção de cada cultura, o que o Templo faz, os festivais, a heresia e a tolerância, a assimilação das minorias e o Sumo Sacerdote.

Já estava decidido e vale como base:

- Religião faz parte da Cultura; a cultura Religiosa busca ideais religiosos; heresia é crime capital nela, e ela faz guerra santa contra culturas hostis ([[Sistema de Cultura/Descrição|Cultura]], [[Sistema de Guerra/Descrição|Guerra]]).
- O **Templo** é uma construção da Facção, com a Função Sacerdote, obrigatória no nível 3 da cidade; a Arcana troca o Templo pela Torre Arcana e a Tecnocrata pelo Laboratório; na Religiosa ele vem 1 nível acima ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]], [[Sistema de População/Descrição|População]]).
- A Felicidade tem o fator **Religião** ([[Sistema de Felicidade e Estabilidade/Descrição|Felicidade]]).
- O **Sumo Sacerdote** é um Importante fixo na Capital Religiosa (8,4); a sucessão da Religiosa é "escolha do clero"; o Rito de Admissão dela é "Serviço (peregrinação, missões)"; peregrinos já são encontros da Viagem; há a masmorra "Templo profanado" em (5,4).
- Casamentos da Religiosa acontecem no Templo ([[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]]).

Os valores numéricos são iniciais e ficam em dados (`religiao.json`) para ajustar nos testes.

---

## Uma fé por cultura

Não existem religiões que atravessam as culturas: cada cultura tem **a sua fé**, e o NPC não ganha um atributo novo. A fé tem uma **Devoção**, que define quanto ela pesa na vida da cidade:

| Cultura | Fé (nome provisório) | Devoção | Construção de culto |
|---|---|---|---|
| Religiosa | Fé da Cidade Santa | Alta | Templo (1 nível acima, já decidido) |
| Autoritária | Culto ao Imperador | Média | Templo |
| Guerreira | Culto aos Ancestrais | Média | Templo |
| Escravista | Culto da Fortuna | Baixa | Templo |
| Arcana | Culto da Magia | Baixa | Torre Arcana faz o papel |
| Tecnocrata | Secular, sem fé organizada | Nenhuma | Laboratório e Universidade fazem o papel |

Os nomes ficam para a história do jogo.

## O Templo

**Cobertura.** Cada **Sacerdote** (NPC real) atende **20 NPCs reais** da cidade.

Cobertura = (Sacerdotes × 20) ÷ NPCs reais da cidade, até 100%.

Vagas de Sacerdote por nível do Templo: a **tabela geral de vagas** do [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]] (decisão do autor na revisão V1-10):

| Nível do Templo | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|
| Sacerdotes | 2 | 3 | 5 | 7 | 10 |
| Atende (NPCs reais) | 40 | 60 | 100 | 140 | 200 |

- As cidades de **nível 4** (as capitais) podem ter **2 Templos**; as coberturas se somam.
- *Exemplo:* cidade nível 3 (100 NPCs reais) com Templo nível 3 (5 Sacerdotes) = 100% de cobertura; com Templo nível 2, 60%. Uma capital (300 NPCs) com 1 Templo nível 5 tem 67% (acima do limite de 50% da Devoção Alta); com o 2º Templo, 100%.

- **Arcana:** na Torre Arcana, cada Mago que trabalha nela conta como um Sacerdote para a cobertura (o culto é a própria magia).
- **Tecnocrata:** Devoção Nenhuma; não há cobertura nem fator Religião.

**Felicidade (fator Religião).** Cobertura × máximo da Devoção:

| Devoção | Máximo | Com cobertura abaixo de 50% |
|---|---|---|
| Alta | +10 | **−5** em vez do bônus (o povo devoto se sente abandonado) |
| Média | +7 | Cobertura × 7 |
| Baixa | +4 | Cobertura × 4 |
| Nenhuma | 0 | 0 |

O fator Religião vai, portanto, de **−5 a +10** ([[Sistema de Felicidade e Estabilidade/Descrição|Felicidade]]). A conta usa a cultura de cada parte da população: uma minoria de outra fé não ganha o bônus do Templo da facção.

**Oferendas.** No Período Livre, os NPCs visitam o Templo como visitam a Taverna e doam **2% da renda**. O dinheiro paga os Sacerdotes e entra no ciclo do dinheiro da cidade ([[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]]). A sobra vai para o Banco da Facção, dona do Templo.

**Templo da Religiosa** (o "ponto forte em cura" dela):

- reduz em **25%** a mortalidade por doença na cidade;
- cuida dos feridos pobres: a cura dos NPCs comuns fica **25% mais rápida** ([[Sistema de Cura/Descrição|Cura]]).

**Função.** O **Sacerdote** é uma Função da classe **Civil**; o gerente do Templo é **Nobre**. Não existe uma classe "clero".

## Festivais

Cada cultura tem **2 festivais por ano**; a Religiosa tem mais 2 dias santos (4 no total).

| Cultura | Festival próprio (nome provisório) | Quando |
|---|---|---|
| Todas | **Festa da Colheita** | Dias 1 e 2 do Outono |
| Religiosa | Festa da Cidade Santa; e 2 dias santos | Dias 8 e 9 da Primavera; dias santos no dia 8 do Verão e no dia 8 do Inverno |
| Autoritária | Dia do Imperador | Dias 8 e 9 do Verão |
| Guerreira | Festa dos Ancestrais | Dias 8 e 9 do Inverno |
| Escravista | Grande Feira | Dias 8 e 9 da Primavera |
| Arcana | Noite das Estrelas | Dias 8 e 9 do Outono |
| Tecnocrata | Feira das Invenções | Dias 8 e 9 do Verão |

A Tecnocrata não tem fé organizada, mas tem festivais: eles são da cultura, não só da fé.

**Como funciona um festival:**

- **Duração:** 2 dias. Os dias santos da Religiosa duram 1 dia, com metade dos efeitos.
- **Felicidade +8**, como evento duradouro. O bônus dura 8 dias (meia estação).
- **Consumo** de Comida e Bebida × 1,5 nos dias do festival: a Taverna e o Mercado vendem mais.
- **Produção −50% no 1º dia** (folga).
- **Quem paga:** o dono da cidade, **2% do Banco da Cidade**.
- **O jogador**, dono da cidade, pode **patrocinar** (o dobro do custo; +4 a mais, ou seja, +12) ou **cancelar** (sem custo). Cancelar numa cultura de Devoção Alta dá **−5 por 1 estação**.
- O **calendário** da interface mostra os festivais ([[Interface/Descrição|Interface]]).

*Exemplo:* na Festa da Colheita, a cidade Autoritária nível 3 (Banco de 5.000) paga 100 moedas; o jogador patrocina por 200, e a Felicidade sobe 12 por 8 dias, enquanto a Taverna vende uma vez e meia mais Cerveja.

Os festivais deixam de ser um evento sorteado: agora vêm do **calendário**. O evento positivo da categoria Social passa a ser a **festa espontânea** (vitória numa guerra, casamento na dinastia, visitante ilustre). ([[Sistema de Eventos/Descrição|Eventos]])

## Heresia e tolerância

**Nova lei de facção: Tolerância religiosa.** Define como a facção trata as **minorias**, os NPCs de cultura diferente da cultura da facção:

| Valor | Efeito nas minorias | Quem deseja |
|---|---|---|
| **Tolerância** | Nenhum | Escravista, Guerreira, Tecnocrata |
| **Fé oficial** | Minorias pagam **+5% de imposto**; não podem ter construção de culto própria | Autoritária, Arcana |
| **Perseguição** | Minorias com **Felicidade −15** e **migração 2× mais rápida** para fora; **Controle +5** | Religiosa (pensamento extremo) |

- A lei entra nas **leis desejadas** da Cultura e, portanto, na conta "Leis × cultura" da Felicidade. As leis iniciais de cada facção seguem o desejo da cultura dela: a Religiosa começa com Perseguição.
- A Felicidade −15 da Perseguição é aplicada à parte da população que é minoria: numa cidade com 20% de minorias, a classe perde 15 × 0,2 = 3 pontos na média.

**Heresia** é um **crime cultural** de **gravidade 3** (**5** com Perseguição). São **atos**, não a simples existência de uma minoria:

- construir culto de outra fé;
- blasfemar (opção de diálogo);
- profanar ou roubar um Templo.

Na primeira versão, os NPCs de minoria **não são presos só por existirem**. **Livro Sagrado** (ideia do autor): com a lei "Livro Sagrado obrigatório", o grupo do jogador precisa carregar o livro nas revistas; sem ele, é heresia (gravidade 2 na primeira vez na estação; ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]). Na Religiosa, a pena da heresia é a execução (o pensamento extremo "heresia é crime capital"), pela lei Crimes e penas ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).

## Assimilação (conversão)

As minorias adotam **devagar** a cultura dominante da cidade. Não há missionários na primeira versão.

- A cada **estação**, uma parte de cada minoria muda para a cultura dominante, **se a cidade tiver a construção de culto dela** (Templo, Torre Arcana ou, na Tecnocrata, Universidade ou Laboratório):

| Lei de Tolerância | Assimilação por estação |
|---|---|
| Tolerância | 1% |
| Fé oficial | 2% |
| Perseguição | 4% |

- Funciona como a População: uma **fração acumulada** em pessoas; a cada **Fator de Escala**, **1 NPC real** muda de cultura ([[Sistema de População/Descrição|População]]).
- **NPCs Importantes e Controlados nunca mudam sozinhos**, só por evento ou diálogo.
- *Exemplo:* cidade Autoritária conquistada pela Religiosa, com 40 NPCs Autoritários e a lei de Fé oficial (2% por estação) = 0,8 NPC por estação; em 5 estações, cerca de 4 NPCs mudaram de cultura.
- **Mudança na Cultura:** antes, a cultura do NPC vinha só do nascimento; agora vem também da assimilação. É ela que faz a "proporção de outras culturas" da cidade mudar devagar depois de uma conquista ([[Sistema de Cultura/Descrição|Cultura]]).

## Sumo Sacerdote, Bênção e Anátema

O **Sumo Sacerdote** é o Importante fixo da Capital Religiosa (8,4).

- **Sucessão:** escolhe o herdeiro na sucessão da Religiosa ("escolha do clero"; ver [[Sistema de Facção/Descrição|Facção]]).
- **Uma vez por estação**, a IA dele pode dar:

| Ato | Alvo | Efeito |
|---|---|---|
| **Bênção** | Uma facção ou o jogador | +10 de relação (ou de reputação) com as facções Religiosas; +5 de Felicidade nas cidades Religiosas do abençoado, por 1 estação |
| **Anátema** | Um líder | −20 de relação das facções Religiosas com ele; o motivo de guerra **"Guerra santa"** contra ele ([[Sistema de Guerra/Descrição\|Guerra]]) |

- "Facções Religiosas" são as de cultura Religiosa: a facção principal e, se houver, vassalos e cidades independentes dessa cultura.
- Como a IA dele escolhe: Bênção para quem tem relação ou reputação de **+40 ou mais** com a Religiosa e fez algo pela fé na última estação (peregrinação, doação, guerra santa, Templo purificado); Anátema para um líder com relação de **−40 ou menos** que saqueou cidade Religiosa, profanou um Templo ou persegue a fé da Religiosa. Sem candidato, não faz nada.
- Anátema contra o jogador sem facção: −20 de reputação com as facções Religiosas.

## Peregrinação

- **Do jogador:** ir à Capital Religiosa (8,4) e fazer uma **oferenda** no Templo dá **+10 de reputação** com a Religiosa (1 vez por ano) e conta para o **Rito de Admissão** dela (Serviço). Oferenda mínima de 50 moedas ([[Sistema de Reputação/Descrição|Reputação]]).
- **Peregrinos do mundo:** a cidade santa recebe peregrinos, que aumentam a demanda da Taverna e do Mercado dela. +10% de consumidores na Capital Religiosa (8,4) e na cidade de peregrinação (7,3), por estação. Os peregrinos também são encontros na estrada e trazem notícias ([[Sistema de Viagem/Descrição|Viagem]], [[Sistema de Notícias/Descrição|Notícias]]).
- **Missões do Sumo Sacerdote:** caçar hereges, escoltar peregrinos e **purificar o Templo profanado** (5,4) ([[Objetivo e Progressão/Descrição|Objetivo e Progressão]]).

---

## Parte técnica

- **Dados:** `religiao.json` com a fé de cada cultura (nome, Devoção, construção de culto), as vagas de Sacerdote por nível, a tabela de Felicidade por Devoção, os festivais (cultura, estação, dias, efeitos e custo), os efeitos da lei de Tolerância, as taxas de assimilação e as regras da Bênção e do Anátema.
- **Frequência:** a cobertura e o fator Religião entram na conta da Felicidade (por dia em LOD 0 e 1, por estação em LOD 2); a assimilação roda **por estação**; o Sumo Sacerdote decide **por estação**; os festivais são datas fixas do calendário.
- **Por cidade:** cobertura, frações acumuladas de assimilação por cultura e o festival em andamento (patrocinado ou cancelado).
- **Save:** frações de assimilação, festivais patrocinados ou cancelados, Bênçãos e Anátemas ativos (com a data de fim) e a data da última peregrinação do jogador.
- **Depuração:** painel (Dear ImGui) com a cobertura e o fator Religião de cada cidade e um botão para forçar Bênção ou Anátema.
- **Fatia vertical:** não usa (a religião fica para depois).

## Ligação com outros sistemas

| Sistema | O que a Religião fornece ou recebe |
|---|---|
| Cultura | Uma fé por cultura e a Devoção; festivais próprios; lei de Tolerância desejada; assimilação das minorias |
| Felicidade e Estabilidade | Fator Religião (−5 a +10); festivais como eventos duradouros; Perseguição: −15 nas minorias e Controle +5 |
| Construção | Templo (Torre Arcana e Laboratório fazem o papel); vagas da tabela geral; 2 Templos nas cidades de nível 4 |
| Trabalho e Propriedade | Função Sacerdote (Civil) e gerente Nobre; oferendas no ciclo do dinheiro |
| Facção | Lei de Tolerância religiosa; escolha do herdeiro pelo Sumo Sacerdote na Religiosa |
| Crime e Justiça | Heresia como crime cultural (gravidade 3 ou 5) |
| População | Assimilação por estação; migração das minorias perseguidas; menos mortes por doença na Religiosa |
| Cura | Templo da Religiosa acelera a cura dos NPCs comuns |
| Diplomacia e Guerra | Bênção e Anátema mudam relações; Anátema libera a "Guerra santa" |
| Reputação | Peregrinação (+10); Bênção e Anátema do jogador |
| Viagem | Peregrinos como encontro; demanda extra na cidade santa |
| Objetivo e Progressão | Missões do Sumo Sacerdote; peregrinação conta para o Rito de Admissão |
| Eventos | Festivais do calendário; festa espontânea; atos do Sumo Sacerdote com causa registrada |
| Interface | Calendário com os festivais; botões de patrocinar e cancelar; cobertura no painel da cidade |
| Notícias | Festivais (Local) e Anátema (Mundial) viram notícia |

## Perguntas em aberto

1. **Valores a testar:** cobertura por Sacerdote, Felicidade por Devoção, custo e efeito dos festivais, taxas de assimilação, oferenda da peregrinação.
2. **Nomes** das fés e dos festivais (ficam para a história do jogo).
3. **IA do Sumo Sacerdote:** critérios finais da Bênção e do Anátema e o que acontece quando ele morre (quem escolhe o próximo).
4. **Missionários** e outras formas de conversão ativa (futuro).
5. **Magos na Religiosa:** como a lei de Magia "restrita" conversa com a heresia (um Mago é herege?).
