> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-16, com a rodada P1-4 aplicada e a P1-5 enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** a **rodada P1-5 (Religião, Clima e Estações, Notícias) foi enviada e aguarda respostas** (seção 4). Em 2026-10-16 foi aplicada a P1-4 (Guerra, Batalha e Diplomacia: três documentos novos). Antes dela, a sessão de 2026-10-15 aplicou cinco rodadas (P0-5 Mundo, P0-6 Pipeline de arte, P1-1 Viagem, P1-2 Reputação e P1-3 Companheiros e Diálogo), fechou o P0, criou o `CLAUDE.md` e o padrão de perguntas detalhadas (seção 2) e registrou a ideia futura dos Títulos de Nobreza.
3. **Próximo passo:** pedir o `Respostas.txt` da P1-5 (conferir se é desta rodada; o arquivo pode ainda ser o da P1-4), escrever os documentos de Religião, Clima e Estações e Notícias, e abrir a P1-6 (Efeitos de itens e Escravidão), a última do P1.
4. Depois: os outros sistemas P1, na ordem da seção 3.
5. Quando o autor estiver satisfeito com a documentação: **ficha técnica lógica**, e só então a programação.

## 2. Como trabalhamos

- **Idioma:** sempre português do Brasil.
- **Fluxo:** o assistente manda uma lista de perguntas numeradas, cada uma com uma **sugestão**. O autor responde no arquivo `C:\Users\Felipe\Desktop\Respostas.txt` (anexado com @) ou no chat. "Seguir sugestão" aceita a proposta; a ideia própria do autor sempre prevalece. Depois o assistente escreve o documento do sistema, atualiza os outros documentos, e abre a próxima rodada.
- **Antes de aplicar respostas:** conferir se o `Respostas.txt` é mesmo da rodada atual (olhar os itens e a data de gravação).
- **Decisões do autor:** o que não tem marcação nos documentos foi decidido por ele. As marcações são **[Proposta]**, **[Em aberto]** e **[Refinar depois]** (ver [[Glossário]]).
- **Sem código por enquanto.** Foco total em documentação. Depois, uma **ficha técnica lógica** (como cada sistema vira estruturas, dados e regras) e só então a programação. O autor tem experiência mínima em C++ e nenhuma com bibliotecas: explicar em linguagem simples.
- **Preferências do autor:**
  - interface e HUD **bem personalizáveis** (ele joga em 1600 × 900 e não gosta de interface fixa);
  - notificações personalizáveis;
  - letras da interface em fontes comuns legíveis (não desenhadas à mão);
  - arte do mundo toda à mão, nada procedural;
  - só teclado e mouse.
- **Estilo dos documentos:** primeira linha com a legenda das marcações; "Descrição geral"; seções do sistema; "Parte técnica" (LOD, dados em JSON, save, fatia vertical); tabela "Ligação com outros sistemas"; "Perguntas em aberto". Cada sistema novo vira uma pasta `Sistema de X/Descrição.md`, entra no [[Descrição do Projeto]] (índice) e no Estado, e as ligações são acrescentadas nos documentos dos sistemas vizinhos. Não usar seções de "Atualizações recentes": o histórico fica no Git.
- **Princípios do projeto:** a simulação vem primeiro e o evento explica; tudo agregado por cidade e por LOD; dados em JSON; "por quê?" em tudo.

### Padrão das perguntas e sugestões (obrigatório)

Pedido do autor em 2026-10-15: as perguntas da P0-5 saíram abstratas ("B2. Tom", "D1. Equilíbrio") e as sugestões, curtas demais. A partir da P0-6, **toda rodada segue este padrão**:

1. **Contexto** (1 a 3 linhas): o que já está decidido sobre o tema e o que a pergunta destrava.
2. **Pergunta concreta:** o que exatamente precisa ser decidido, com números, nomes e um exemplo de situação de jogo. Uma pergunta por decisão; um tema com várias decisões vira vários itens.
3. **Opções** (quando houver): 2 a 4, cada uma com o que ganha e o que perde (arte, programação, desempenho, diversão).
4. **Sugestão completa:** a opção recomendada e o porquê, pronta para virar documento: valores iniciais, tabela ou lista, exemplo prático, impacto nos outros sistemas e o que fica para testar.
5. **Conflitos:** se a sugestão ou uma resposta anterior bate com outra decisão, apontar e propor a saída.

Regras extras:

- Explicar termos e ferramentas em linguagem simples.
- Quando um "seguir sugestão" deixar algo vago, o documento detalha com valores concretos marcados como **[Proposta]**.
- Economizar palavras sem cortar informação útil: nada de introduções longas.
- Formato fácil de responder no `Respostas.txt`: códigos (A1, B2...) e títulos curtos.

**Exemplo.** Abstrato (evitar): "B2. Tom: fantasia de baixa magia?". Detalhado (seguir): "B2. Quanta magia aparece no dia a dia? *Contexto:* a cultura Arcana dá privilégios aos Magos, e o Combate tem Cajado. *Opções:* (a) magia rara, cerca de 1 Mago para cada 50 NPCs, feitiços caros; (b) magia comum, todo guarda de cidade Arcana conjura. *Sugestão:* (a), porque mantém o tom sombrio e político e reduz a arte de efeitos. *Impacto:* Magia (quantidade de Magos por cidade), Combate (Esquadrão de Magos raro), arte (poucos efeitos de feitiço)."

## 3. Lista de sistemas (da Revisão)

| Prioridade | Sistema | Estado |
|---|---|---|
| P0 | Felicidade e Estabilidade | Escrito |
| P0 | População e Habitação | Escrito |
| P0 | Trabalho, Salários e Propriedade (+ Funções e Construções) | Escrito |
| P0 | Interface e Controles | Escrito |
| P0 | Mundo e Estado Inicial | Escrito (a história fica para depois) |
| P0 | Pipeline de arte e conteúdo | Escrito |
| P1 | Viagem e Mundo | Escrito |
| P1 | Reputação e Relações do jogador | Escrito |
| P1 | Companheiros, Relações familiares e Diálogo | Escrito |
| P1 | Guerra e Campanha | Escrito |
| P1 | Batalha (Mapa de Guerra) | Escrito |
| P1 | Diplomacia | Escrito |
| P1 | **Religião** | **Rodada P1-5 enviada, aguardando respostas** |
| P1 | **Clima e Estações** | **Rodada P1-5 enviada, aguardando respostas** |
| P1 | **Notícias e Informação** | **Rodada P1-5 enviada, aguardando respostas** |
| P1 | Efeitos de itens e Escravidão | A fazer (rodada P1-6) |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (entra na P1-5, em Notícias), especialização de cidades, ordens de produção em lote, modo observador, eventos de dinastia. A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: P1-5, Religião, Clima e Estações, Notícias

**Perguntas enviadas em 2026-10-16, aguardando respostas.** Junta três sistemas da [[Revisão do Projeto]] (4.13, 4.14 e 4.15), que são menores e se tocam (festivais nas estações, peregrinos que trazem notícias, clima que vira notícia). Responder no `Respostas.txt` pelos códigos (R1, E2...). "Seguir sugestão" aceita a proposta. Valores numéricos são iniciais, a testar.

**Já decidido (base das perguntas):**

- **Religião faz parte da Cultura**, não é um sistema à parte; a cultura Religiosa busca ideais religiosos; heresia é crime capital nela, e ela faz guerra santa contra culturas hostis ([[Sistema de Cultura/Descrição|Cultura]], [[Sistema de Guerra/Descrição|Guerra]]).
- O **Templo** é uma construção da Facção, com a Função Sacerdote; é obrigatório no nível 3 (ou Laboratório, conforme a cultura); a Arcana troca o Templo pela Torre Arcana e a Tecnocrata pelo Laboratório ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]], [[Sistema de População/Descrição|População]]).
- Felicidade tem o fator **Religião, de 0 a +10** (Templo e festivais) ([[Sistema de Felicidade e Estabilidade/Descrição|Felicidade]]).
- Recursos **não se esgotam**: têm **produtividade por estação**, com modificadores do futuro Clima ([[Sistema de Recursos/Descrição|Recursos]]).
- Estações na arte: **só mudança de cor** por enquanto; neve no chão, rios congelados, folhas e flores ficam para o futuro ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]).
- Eventos de clima já existem como categoria (seca, tempestade, geada, enchente) e o Narrador regula os que afetam o jogador ([[Sistema de Eventos/Descrição|Eventos]]).
- O jogador só conhece o mundo com **informações datadas** (névoa); conhece os preços das cidades visitadas, com data; as propostas diplomáticas são instantâneas, mas as **notícias da guerra chegam com atraso** ([[Sistema de Viagem/Descrição|Viagem]], [[Sistema de Comércio/Descrição|Comércio]], [[Sistema de Diplomacia/Descrição|Diplomacia]]).
- Diálogo: "Conversar" dá rumores e notícias; peregrinos trazem rumores ([[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]]).

### R. Religião

**R1. Religiões separadas ou a fé de cada cultura?**

- *Contexto:* o autor decidiu que a religião faz parte da Cultura. A Revisão listou "Religião" como sistema do P1 para dar mecânica ao Templo, aos festivais e à heresia.
- *Pergunta:* existem religiões próprias, que atravessam as culturas, ou cada cultura tem a sua fé?
- *Opções:* (a) **uma fé por cultura**: respeita a decisão anterior, sem dados novos de "religião" no NPC; (b) 2 ou 3 religiões que atravessam as culturas (estilo Crusader Kings): mais política, mas cada NPC ganha um segundo atributo, e conversão e conflito dobram; (c) uma fé única com seitas.
- *Sugestão:* (a). A "religião" de um NPC é a cultura dele; o novo documento só dá **mecânica** à fé. Cada cultura tem uma **Devoção**, que define quanto a fé pesa:

| Cultura | Fé (nome provisório) | Devoção | Construção de culto |
|---|---|---|---|
| Religiosa | Fé da Cidade Santa | Alta | Templo (1 nível acima, já decidido) |
| Autoritária | Culto ao Imperador | Média | Templo |
| Guerreira | Culto aos Ancestrais | Média | Templo |
| Escravista | Culto da Fortuna | Baixa | Templo |
| Arcana | Culto da Magia | Baixa | Torre Arcana faz o papel |
| Tecnocrata | Secular, sem fé organizada | Nenhuma | Laboratório e Universidade fazem o papel |

  Os nomes ficam para a história do jogo.
- *Conflito:* nenhum com a Cultura; o documento novo se chama "Religião e Fé" e diz, logo no começo, que é um complemento da Cultura.

**R2. O que o Templo faz?**

- *Contexto:* o Templo existe como nome, com a Função Sacerdote, sem efeito. O ponto forte da Religiosa no Mundo é "Felicidade estável e cura".
- *Sugestão:*
  - **Cobertura:** cada Sacerdote (NPC real) atende 20 NPCs reais. Cobertura = (Sacerdotes × 20) ÷ NPCs da cidade, até 100%. *Exemplo:* cidade nível 3 (100 NPCs) com Templo nível 3 (5 Sacerdotes) = 100%.
  - **Felicidade (fator Religião):** cobertura × máximo da Devoção: Alta +10, Média +7, Baixa +4, Nenhuma 0. Com Devoção Alta e cobertura abaixo de 50%: −5 em vez de bônus.
  - **Oferendas:** no Período Livre, os NPCs visitam o Templo como visitam a Taverna e doam 2% da renda. Isso paga os Sacerdotes e entra no ciclo do dinheiro.
  - **Templo da Religiosa:** reduz a mortalidade por doença na cidade em 25% e cuida dos feridos pobres (a cura de NPCs comuns fica 25% mais rápida). É o "ponto forte em cura" dela.
  - Casamentos da Religiosa acontecem no Templo (já decidido).
- *Impacto:* Felicidade, Trabalho (vagas de Sacerdote), Econômico (oferendas), Cura, Construção.

**R3. Festivais.**

- *Contexto:* "festival" já é um evento positivo e um evento duradouro na Felicidade, sem calendário.
- *Sugestão:*
  - **2 festivais por ano** em cada cultura: a **Festa da Colheita** (dias 1 e 2 do Outono, para todas) e um **festival próprio** da cultura, numa estação fixa. A Religiosa tem mais 2 dias santos (4 no total).
  - **Duração:** 2 dias. **Efeitos:** Felicidade +8 (evento duradouro); consumo de Comida e Bebida × 1,5 nesses dias; produção −50% no 1º dia (folga).
  - **Quem paga:** o dono da cidade, 2% do Banco da Cidade. O jogador pode **patrocinar** um festival maior (o dobro do custo, +4 a mais) ou **cancelar**. Cancelar com Devoção Alta dá −5 por 1 estação.
  - O calendário da interface mostra os festivais.
- *Impacto:* Felicidade, Econômico (demanda de Bebida e Comida), Eventos, Interface.

**R4. Heresia e tolerância.**

- *Contexto:* a heresia é crime capital na Religiosa, mas não há definição do que é heresia. A Felicidade já pune minorias pela distância entre as leis e a cultura.
- *Pergunta:* o que é heresia, e como a facção trata as minorias de outra cultura?
- *Sugestão:* nova lei de facção, **Tolerância religiosa**, com 3 valores:

| Valor | Efeito nas minorias | Quem deseja |
|---|---|---|
| Tolerância | Nenhum | Escravista, Guerreira, Tecnocrata |
| Fé oficial | Minorias pagam +5% de imposto; não podem ter construção de culto própria | Autoritária, Arcana |
| Perseguição | Minorias com Felicidade −15 e migração 2× mais rápida para fora; Controle +5 | Religiosa (pensamento extremo) |

  - **Heresia** (crime cultural, gravidade 3; gravidade 5 com Perseguição) são **atos**, não a simples existência: construir culto de outra fé, blasfemar (opção de diálogo), profanar ou roubar um Templo. Na primeira versão, os NPCs de minoria não são presos só por existirem.
- *Impacto:* Facção (nova lei), Crime (heresia), Felicidade, População (migração), Cultura (leis desejadas).

**R5. Conversão.**

- *Contexto:* o NPC nasce com a cultura da cidade e o migrante mantém a dele; a Cultura dominante de uma facção muda por estação, pela população.
- *Opções:* (a) **sem conversão**: a cultura só muda por nascimento e migração, o que leva gerações; (b) **assimilação lenta**: uma parte das minorias adota a cultura dominante da cidade a cada estação; (c) missionários ativos (NPCs que convertem): mais conteúdo e mais simulação.
- *Sugestão:* (b). A cada estação, uma parte de cada minoria da cidade adota a cultura dominante, se houver construção de culto dela: **1% com Tolerância, 2% com Fé oficial, 4% com Perseguição**. Funciona como a População: uma fração acumulada; a cada Fator de Escala, 1 NPC real muda de cultura. NPCs Importantes e Controlados nunca mudam sozinhos, só por evento ou diálogo.
  - *Exemplo:* Cidade Autoritária conquistada pela Religiosa, com 40 NPCs Autoritários, Fé oficial: 2% por estação = 0,8 NPC por estação; em 5 estações, cerca de 4 NPCs mudaram de cultura.
- *Conflito:* a Cultura dizia que a cultura vem do nascimento; passa a ter também a assimilação.

**R6. Clero, Sumo Sacerdote e peregrinação.**

- *Contexto:* o Sumo Sacerdote é um Importante fixo na Capital Religiosa (8,4); na Religiosa, a sucessão é "escolha do clero"; peregrinos já são encontros da Viagem; o Rito de Admissão da Religiosa é "Serviço (peregrinação, missões)"; há a masmorra "Templo profanado" em (5,4).
- *Sugestão:*
  - **Sacerdote** é uma Função da classe Civil; o gerente do Templo é Nobre. Sem classe "clero".
  - **Sumo Sacerdote:** escolhe o herdeiro na sucessão da Religiosa. Uma vez por estação, a IA dele pode dar uma **Bênção** (a uma facção ou ao jogador: +10 de relação ou reputação com as facções Religiosas e +5 de Felicidade nas cidades Religiosas da abençoada, por 1 estação) ou um **Anátema** (contra um líder: −20 de relação das facções Religiosas com ele e o motivo de guerra "Guerra santa" contra ele).
  - **Peregrinação:** ir à Capital Religiosa e fazer uma oferenda dá +10 de reputação com a Religiosa (1 vez por ano) e conta para o Rito de Admissão. A cidade santa recebe peregrinos: mais demanda na Taverna e no Mercado dela.
  - **Missões do Sumo Sacerdote:** caçar hereges, escoltar peregrinos, purificar o Templo profanado (5,4).
- *Impacto:* Facção (sucessão), Diplomacia e Guerra (Anátema), Reputação, Objetivo (missões), Viagem (peregrinos).

### E. Clima e Estações

**E1. Produção por estação.**

- *Contexto:* os recursos têm produtividade por estação, mas sem números. Planejar estoques para o inverno é o ritmo de jogos como RimWorld.
- *Sugestão:* tabela base (bioma temperado), em `clima.json`:

| Construção | Primavera | Verão | Outono | Inverno |
|---|---|---|---|---|
| Fazenda (grãos, legumes, fruta, algodão, fumo, especiaria) | 75% | 100% | 150% (colheita) | 25% |
| Pasto, Rancho | 100% | 100% | 100% | 75% |
| Posto de Coleta (madeira, coleta) | 100% | 100% | 100% | 75% |
| Pedreira e minas | 100% | 100% | 100% | 90% |

  Ajustes por bioma (só o que muda):

| Bioma (região) | Mudança |
|---|---|
| Montanhas frias e taiga (Guerreira) | Inverno: Fazenda 0%, Pasto 50% |
| Terras áridas (Tecnocrata) | Verão: Fazenda 75% (calor); Inverno: Fazenda 50% |
| Litoral quente (Escravista) | Inverno: Fazenda 75% |
| Planície, colinas, floresta (Autoritária, Religiosa, Arcana) | Tabela base |

  - Média anual da Fazenda na tabela base: 87,5%. Quem não guarda comida no outono passa fome no inverno, e a Guerreira, que já tem pouca comida, depende ainda mais do comércio ou da guerra pelo celeiro.
- *Impacto:* Produção, Recursos, Comércio (preço da comida sobe no inverno pela própria simulação), População (fome).

**E2. Clima do dia.**

- *Contexto:* hoje só existem estações e eventos climáticos raros. A Viagem já espera multiplicadores de "neve e chuva forte".
- *Opções:* (a) só estações, sem clima diário; (b) **clima do dia sorteado por região**, com efeito em viagem e combate e um efeito visual simples; (c) clima por Mapa Local, com frentes que se movem: bonito, mais caro e difícil de ler.
- *Sugestão:* (b). Sorteado **1 vez por dia por região** (6 sorteios por dia; barato e coerente entre mapas vizinhos), com 60% de chance de repetir o do dia anterior. As chances por bioma e estação ficam em dados.

| Clima | Viagem | Combate e Batalha | Outros |
|---|---|---|---|
| Limpo | × 1,0 | — | — |
| Nublado | × 1,0 | — | — |
| Chuva | × 1,25 | Alcance de tiro −15%; Atiradores −30% de dano (pólvora molhada) | — |
| Tempestade | × 1,5; caravanas param | Alcance de tiro −30%; Atiradores não atiram | NPCs ficam em casa |
| Neve | × 1,5 | Velocidade −20% | Lenha × 1,5 (ver E4) |
| Névoa | × 1,0 | Alcance de visão e de tiro −40% | Detecção de crime −20% |
| Calor forte | × 1,1 | — | Felicidade −2 |

  - **Visual:** uma camada por cima do mapa, com partículas (pingos, flocos), névoa translúcida e um tom de cor. São poucos sprites pequenos, sem tiles novos.
- *Conflito:* o Pipeline decidiu "estações só com cor". A camada de clima não é arte de tile: neve **caindo** sim, neve **no chão** continua para o futuro.

**E3. Eventos climáticos extremos.**

- *Contexto:* seca, tempestade, geada e enchente já são eventos; falta dizer onde e quando acontecem e o que fazem.
- *Sugestão:*

| Evento | Quando e onde | Efeito | Duração |
|---|---|---|---|
| Seca | Verão; terras áridas e planície | Fazenda −50% | 1 estação |
| Enchente | Primavera; mapas com rio | Fazenda −30%; perde 10% do estoque de comida da cidade | 4 dias |
| Geada fora de hora | Primavera ou outono; montanhas e colinas | Fazenda perde 50% da produção da estação | Pontual |
| Tempestade forte | Qualquer estação; litoral | Caravanas paradas; danifica construções de madeira (custo de reparo pela manutenção) **[Proposta]** | 2 dias |
| Onda de calor | Verão; terras áridas e litoral | Felicidade −5; chance de doença maior | 4 dias |
| Inverno rigoroso | Inverno; o mundo todo (cerca de 1 vez a cada 5 anos) | Efeitos do inverno × 1,5; Lenha × 2; mortalidade de velhos maior | A estação inteira |

  O Narrador continua regulando os que afetam o jogador.

**E4. Inverno e aquecimento.**

- *Contexto:* o consumo das classes não muda com a estação.
- *Opções:* (a) nada muda no inverno; (b) **as moradias gastam Lenha no inverno**; (c) Lenha e roupas de inverno.
- *Sugestão:* (b). No inverno, cada NPC real consome **1 Madeira Bruta a cada 4 dias** como "aquecimento", dentro do consumo de Materiais da Classe Social. Por bioma: frio × 2; temperado × 1; litoral quente e terras áridas × 0,25. Falta de Lenha: Felicidade −10 (pelo fator Consumo atendido) e mortalidade de velhos e crianças +50% na estação.
  - *Exemplo:* cidade Guerreira de 100 NPCs no inverno de 16 dias: 4 × 100 × 2 = 800 de Madeira Bruta, que precisa estar estocada no outono.
- *Impacto:* Classe Social (consumo), Produção (Posto de Coleta), Comércio, População.

**E5. Estações na viagem e na guerra.**

- *Contexto:* uma campanha cabe numa estação; nada impede a guerra no inverno.
- *Sugestão:*
  - **Viagem:** primavera com lama, × 1,1 em mapas sem estrada; inverno × 1,5 nos biomas frios e × 1,1 nos outros (somando com o clima do dia, até × 2,0 no máximo).
  - **Campanha de inverno:** exército fora de cidade amiga gasta 1,25 Ração por Esquadrão por dia; nos biomas frios, Moral −1 por dia; Cansaço +5 por estação de campanha no inverno.
  - **IA:** vontade de guerra −15 no inverno. As guerras ganham um ritmo: campanhas na primavera e no verão, paz e reconstrução no inverno.
- *Impacto:* Viagem, Guerra, Diplomacia (IA).

**E6. Noite e estações na tela.**

- *Contexto:* o dia tem 24 horas e os grupos acampam à noite; o crime já usa a noite na detecção; a arte das estações é só cor.
- *Sugestão:*
  - **Estações:** troca de paleta na placa de vídeo, como a das culturas (sem memória extra): primavera verde vivo, verão quente, outono alaranjado, inverno frio e dessaturado.
  - **Dia e noite:** um tom de cor sobre o mapa em 4 fases (amanhecer, dia, entardecer, noite), sem luzes dinâmicas na primeira versão. À noite, visão e detecção −30% (vale para crime e combate).
  - **Futuro:** tochas e janelas acesas, e a arte extra das estações (já decidido para depois).
- *Impacto:* Pipeline, Interface, Crime, Combate. Entra na fatia vertical (barato e dá vida).

### N. Notícias

**N1. Como as notícias andam pelo mundo.**

- *Contexto:* o jogador só conhece o mundo com informações datadas; as notícias da guerra devem chegar com atraso.
- *Opções:* (a) o jogador sabe tudo na hora: simples, mas quebra a névoa; (b) **atraso pela distância**, calculado; (c) notícias como objetos levados por caravanas e peregrinos: realista, mas caro e imprevisível.
- *Sugestão:* (b). Toda notícia nasce num Mapa Local, numa hora. Ela chega a cada outro mapa depois de **3 horas por mapa de distância** (o passo de um mensageiro a cavalo): atravessar o mundo (8 mapas) leva 1 dia.
  - O jogador recebe a notícia quando ela chega ao mapa onde ele está **ou** a um lugar da rede dele: as cidades dele (governadores informam), os companheiros fora do grupo e as caravanas dele.
  - **Custo:** ao nascer, a notícia calcula a distância até cada mapa (48 mapas); depois basta comparar horas. Quase nada.
- *Impacto:* Viagem (névoa), Interface (notificações), Companheiros (rede de informação).

**N2. Tipos e alcance.**

- *Sugestão:* cada notícia tem uma **importância**, que define até onde vai e quanto tempo fica no quadro:

| Importância | Alcance | Fica no quadro da Taverna | Exemplos |
|---|---|---|---|
| Local | 2 mapas | 8 dias | Bandidos numa estrada, festival, preço alto numa cidade, contrato |
| Regional | 5 mapas | 16 dias | Batalha, cerco, revolta, epidemia, seca, companhia de mercenários |
| Mundial | O mundo todo | 64 dias | Guerra declarada, paz, morte de líder, sucessão, marco de tecnologia, Anátema |

  As notícias nascem dos eventos e das decisões que já guardam o "Por quê?" (Eventos, Guerra, Diplomacia, Comércio). Não há texto novo para escrever: o modelo de frase vem do próprio evento.

**N3. Onde o jogador vê as notícias.**

- *Sugestão:*
  - **Notificação**, numa categoria "Notícias", personalizável como as outras (já decidido).
  - **Quadro da Taverna:** as notícias que chegaram àquela cidade. É a "Crônica do mundo visível" da Revisão.
  - **Conversa:** "Conversar" com um NPC dá uma notícia local ou regional (já decidido no Diálogo).
  - **Mapa Mundial:** a notícia atualiza a informação datada do mapa ("Cidade Autoritária ocupada pela Guerreira, soube há 2 dias").
  - **Diário e Crônica do mundo:** uma aba com a data do acontecimento e a data em que o jogador soube. No epílogo, a Crônica do mundo aparece junto com a do jogador.

**N4. Precisão.**

- *Opções:* (a) sempre exata; (b) **menos precisa com a distância**; (c) com boatos falsos.
- *Sugestão:* (b). Até 2 mapas: números exatos ("1.800 soldados"). De 3 a 5: arredondados ("cerca de 2.000"). 6 ou mais: vagos ("um grande exército"). **Boatos falsos** ficam para o futuro, para não frustrar na primeira versão.

**N5. Preços como notícia.**

- *Contexto:* o jogador só sabe os preços das cidades visitadas, com data.
- *Sugestão:* quando um item fica 30% acima ou abaixo da média da facção numa cidade, nasce uma notícia Local ("Grão caro na Capital Guerreira, +45%"). Ao chegar, ela atualiza a tabela de preços conhecidos do jogador para aquele item, com a data. Caravanas e companheiros comerciantes do jogador trazem a tabela completa da cidade de onde vêm.
- *Impacto:* Comércio (rotas do jogador), Econômico.

**N6. A IA também espera as notícias?**

- *Opções:* (a) **as facções da IA sabem tudo na hora**: barato e simples, uma simplificação comum em jogos de estratégia; (b) a IA também espera: justo, mas mais dados e decisões mais lentas.
- *Sugestão:* (a) na primeira versão, marcada como simplificação **[Refinar depois]**. Os NPCs comuns só "sabem" (para rumores) o que já chegou à cidade deles.

### D. Escopo

**D1. Documentos.**

- *Sugestão:*
  - **Três documentos novos:** `Sistema de Religião/Descrição.md` ("Religião e Fé", complemento da Cultura), `Sistema de Clima e Estações/Descrição.md` e `Sistema de Notícias/Descrição.md`.
  - **Atualizar:** Cultura, Construção (Templo), Felicidade (fator Religião e Lenha), Crime (heresia), Facção (lei de Tolerância), Classe Social (Lenha), Recursos e Produção (tabela por estação), Viagem, Combate e Batalha (clima), Guerra (campanha de inverno), Eventos (clima e festivais), Comércio (preços como notícia), Interface (camada de clima, quadro da Taverna, calendário), Pipeline (partículas e tom da noite), Objetivo (missões do Sumo Sacerdote e Crônica), Glossário.
  - **Dados:** `religiao.json`, `clima.json` e `noticias.json`.
  - **Fatia vertical:** entram a produção por estação (testa os estoques) e o tom de dia e noite; o clima do dia, a religião e as notícias ficam para depois.

Depois das respostas: escrever os três documentos, atualizar os vizinhos e abrir a última rodada do P1 (P1-6: Efeitos de itens e Escravidão).

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). O Ferreiro não lista Espada (provável erro; ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]).
- **Arte:** o autor desenha por enquanto (sem experiência em pixel art), no **Pixelorama** (grátis), com pacotes de terceiros provisórios na fatia vertical. Mapas no **Tiled** (explicar em linguagem simples). Sem verba para programas pagos. Projeto **sem prazo**: o limite da arte é o peso no jogo, não o tempo (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
- **Estações na arte:** só mudança de cor por enquanto; **toda arte extra de estação fica para o futuro** (neve, rios congelados, folhas, flores).
- **Viagem:** 4 h por Mapa Local a pé, multiplicada por terreno, infraestrutura, tamanho do grupo, montaria e carga; caravanas sempre reais no Mapa Mundial (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).
- **Grupo:** controlado = 4 + Renome ÷ 20; vinculados = 10 + Renome ÷ 5 + 3 por cidade; **sem retratos** nas conversas (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).
- **Linguagem:** o foco é **C++**; ferramentas em outras linguagens só como ajuda, nunca essenciais.
- **Guerra e Batalha:** Placar de Guerra (−100 a +100) paga a paz; cidade ocupada só muda de dono no tratado; exército de até 20 Esquadrões com General; **o mundo para durante a batalha**; a simulação é sempre por Esquadrão de 100 soldados, e o jogador escolhe o **tamanho visual** (20, 50 ou 100 bonecos por Esquadrão; até 4.000 soldados na tela, pedido do autor). A 7ª camada do Tiled é a Elevação. Na programação, a guerra por cálculo rápido vem antes do Mapa de Guerra (ver [[Sistema de Guerra/Descrição|Guerra]], [[Sistema de Batalha/Descrição|Batalha]]).
- **Diplomacia:** estado (Paz, Guerra, Trégua) + tratados que se somam + atitude (faixa da relação); Confiabilidade; vassalo tem leis próprias (ver [[Sistema de Diplomacia/Descrição|Diplomacia]]).
- **Mundo:** 48 Mapas Locais (8 × 6), 6 regiões de 8 mapas; partida no Ano 412; nomes significativos até a história ser escrita (ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- **Git:** o repositório é `FelipeSKT/Projeto-Jogo`. Cada rodada vira um pull request para o autor aceitar.

## 6. Cuidados técnicos (para quem edita)

- **Ferramentas do PC:** g++ 15.2, CMake, Ninja, Git e Python 3.11. **Não há** `gh` (GitHub CLI) nem Visual Studio. O Python precisa ser chamado com `-I` ao ler arquivos do vault.
- **Edição em lote:** usar um script Python que carrega cada arquivo, troca trechos exatos com **verificação de que o trecho existe uma única vez**, e só então grava (com modo de teste "dry-run" antes). Preservar as quebras de linha (CRLF ou LF) e o BOM de cada arquivo. Backup da pasta antes de edições grandes (a pasta é um repositório Git, mas o backup extra é barato).
- **Conferir links:** o vault não deve ter `[[link]]` quebrado. Em tabelas, o separador do alias precisa ser escapado: `[[Pasta/Descrição\|Texto]]`.
- **Console do Windows:** a saída do PowerShell mostra acentos corrompidos; é só na tela, os arquivos estão em UTF-8.
- **Git:** o primeiro commit desta sessão falhou por causa da mensagem em *here-string*; usar `git commit -F arquivo`. Rodar `git fetch` antes de editar, porque outras sessões podem ter mexido no repositório. Não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json` (o Obsidian os altera sozinho). O `.gitignore` já ignora o workspace do Obsidian.
- **Sessões na nuvem (claude.ai/code):** o `Respostas.txt` chega como anexo na conversa; trabalhar na branch indicada pela sessão. O assistente só cria o pull request se o autor pedir.
- **Pull request (no PC):** sem o `gh`, o assistente envia a branch e o autor abre o PR pelo link `https://github.com/FelipeSKT/Projeto-Jogo/pull/new/NOME-DA-BRANCH`. O navegador interno não está logado no GitHub e o assistente nunca digita senhas. A extensão Claude in Chrome pode permitir criar o PR, se estiver conectada.
- **Nomes de itens:** a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] manda sobre os canvases.
