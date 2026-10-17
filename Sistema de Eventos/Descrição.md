> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Eventos e Narrador

**Descrição geral**

Os eventos são a camada que **registra, explica e dramatiza** o que acontece no mundo. Vários sistemas já dependem deles: déficit de consumo e revolta (Classe Social), assembleia extraordinária e crise de sucessão (Facção), Criminalidade (Crime e Justiça), preços (Econômico), doenças (Cura).

Dois tipos de evento, separados pelo **gatilho**:

| Tipo | Gatilho | Exemplos |
|---|---|---|
| **Ocasional** | Sorteio ao longo do tempo, com a chance ajustada pelo estado do mundo | Doença, seca, tempestade, incêndio, colheita farta, mercador viajante |
| **Condicional** | Estado do mundo: só acontece se a condição for atendida | Escassez por guerra, revolta de uma cidade, bandidos por alta Criminalidade, mudança de governo |

Princípios:

1. **Tudo tem explicação.** Todo evento guarda as suas causas e o jogador pode ver "por quê?" (ver "Registro de causas").
2. **A simulação vem primeiro.** Se uma consequência já nasce das regras (guerra consome Ração Militar e tira conscritos da produção, então o preço sobe pela oferta e demanda), o evento **não força** o resultado: ele marca, avisa e explica. O evento só altera diretamente o que a simulação não produz sozinha.
3. **Condições nunca são conferidas a cada tick.** Só por sinal ou em varredura diária ou sazonal (ver "Condicionais").
4. **O Narrador só mexe nos ocasionais.** Os condicionais nunca são alterados por ele, para preservar a causalidade.

### Modelo único do evento

Ocasionais e condicionais têm a mesma estrutura. Todo evento é um **dado** (JSON), não código.

| Campo | O que é |
|---|---|
| **ID** | Identificador de texto legível (por exemplo, `evento.seca`) |
| **Tipo** | Ocasional ou Condicional |
| **Categoria** | Clima, Saúde, Natureza, Fauna, Sorte, Social, Política, Economia, Ameaça... |
| **Escopo** | Onde acontece (ver abaixo) |
| **Gatilho** | Sorteio com chance base e modificadores (ocasional) ou sinais e condições (condicional) |
| **Condições** | Requisitos mínimos (estação, escopo, estado do mundo) |
| **Duração** | Pontual ou Duradouro, com tempo ou condição de término |
| **Efeitos** | Lista de efeitos da lista fechada |
| **Peso** | Prioridade/frequência relativa entre os ocasionais |
| **Cooldown** | Dias de jogo até poder acontecer de novo no mesmo escopo |
| **Aviso** | Info, Aviso ou Crítico |
| **Escolhas** | De 0 a 3 opções, opcional |

Exemplo ilustrativo (não é o formato final):

```json
{
  "id": "evento.seca",
  "tipo": "ocasional",
  "categoria": "clima",
  "escopo": "mapa_local",
  "chance_base_dia": 0.004,
  "modificadores": [ { "estacao": "verao", "mult": 3.0 } ],
  "duracao": { "tipo": "duradouro", "dias": 12 },
  "efeitos": [ { "mod_producao": { "recurso_tag": "agricola", "mult": 0.6 } } ],
  "cooldown_dias": 64,
  "aviso": "aviso"
}
```

### Escopo

Cada evento declara onde acontece:

- **Mundo:** afeta todo o mapa mundial (por exemplo, inverno rigoroso geral, descoberta de tecnologia).
- **Facção:** afeta uma facção e suas cidades (por exemplo, escassez da facção em guerra, assembleia extraordinária).
- **Cidade / Mapa Local:** afeta uma cidade e o Mapa Local dela (a maioria dos eventos).
- **Grupo do jogador:** afeta os NPCs controlados e as rotas do jogador (por exemplo, emboscada, visitante).

Eventos que atingem NPCs individuais (doença em um NPC, nascimento, morte natural) usam o mesmo mecanismo para o efeito visível, mas o ciclo de vida do NPC continua no Sistema de NPC. **[Refinar depois]**

### Duração

- **Pontual:** acontece, aplica o efeito e acaba (um incêndio, uma colheita farta).
- **Duradouro:** fica **ativo** com início, duração ou condição de término, e aplica modificadores enquanto dura (peste, escassez, guerra de preços, estado de revolta).

Eventos duradouros ativos ficam numa lista por escopo, com a data de início, a causa e o tempo restante.

### Efeitos possíveis (lista fechada)

Para o código ficar simples e seguro, o evento só pode usar efeitos desta lista:

1. **Modificador temporário** (produção, Felicidade, consumo, preço, velocidade de viagem), com valor e duração.
2. **Adicionar ou remover itens e recursos** do inventário da cidade ou do grupo.
3. **Ferir ou adoecer NPCs** (em LOD 2, vira uma redução proporcional da população).
4. **Criar um grupo** (bandidos, animais selvagens, mercador viajante, visitante).
5. **Mudar relação diplomática** entre facções.
6. **Propor mudança de lei ou revolta** (aciona o processo da Facção: assembleia extraordinária, revolta).
7. **Destruir ou danificar construções.**

Novos efeitos entram na lista só quando o código ganhar suporte a eles. **[Refinar depois]**

---

## Eventos Ocasionais

### Quando sorteia

- Em mapas **LOD 0 e 1**, uma vez por **dia** do jogo.
- Em mapas **LOD 2**, uma vez por **estação** (16 dias), com a chance acumulada do período.
- Cada escopo sorteia separadamente. Mundo e Facção são poucos, então rodam por dia.

Em LOD 2, os efeitos são **agregados** (por exemplo, uma peste reduz a população por uma porcentagem, sem escolher NPC a NPC).

### Como a chance muda

Chance final = **chance base × modificadores** declarados no próprio JSON. O modificador vem do estado do mundo, o que mantém a explicação do evento:

- **Doença:** população densa, pouca comida, sem Hospital ou Boticário.
- **Seca:** verão.
- **Incêndio:** muitas construções de madeira.
- **Praga de lavoura:** monocultura (só um tipo de Fazenda).

### Evitando sequências injustas

- **Cooldown** por evento e por escopo.
- **Limite de eventos negativos ativos** ao mesmo tempo por cidade. **[Proposta]** valor inicial 2, a ajustar em testes.
- O Narrador reforça isso para o jogador (ver abaixo).

### Eventos positivos

Entram, para o jogo não virar só punição: colheita farta, festa espontânea, jazida descoberta, mercador viajante, visitante ilustre. Os **festivais** deixaram de ser sorteados: vêm do calendário de cada cultura (ver [[Sistema de Religião/Descrição|Religião e Fé]]).

### Categorias iniciais

| Categoria | Exemplos |
|---|---|
| Clima | seca, enchente, geada fora de hora, tempestade forte, onda de calor, inverno rigoroso (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Saúde | doença, peste |
| Natureza | incêndio, praga de lavoura, terremoto |
| Fauna | ataque de animais, migração |
| Sorte | colheita farta, jazida |
| Social | festa espontânea, visitante, mercador |

Acréscimos serão pensados ao longo do desenvolvimento. **[Refinar depois]** O Narrador (ver abaixo) pode precisar de categorias de política e de ameaça para os focos dele.

---

## Eventos Condicionais

### Quando as condições são avaliadas

Dois modos, para nunca varrer o mundo a cada tick:

1. **Por sinal:** quando algo muda (guerra declarada, cidade em revolta, líder morto, lei alterada), o sistema publica um sinal e só os eventos interessados nele são avaliados.
2. **Varredura diária ou sazonal:** para condições contínuas, como Criminalidade alta há muito tempo. Diária em LOD 0 e 1 e por estação em LOD 2.

### Condições com duração

Uma condição pode exigir **duração mínima** e ter **margem de saída** (histerese). Exemplo: "Criminalidade acima de 60 por 8 dias seguidos" liga o evento, e só desliga abaixo de 40. Isso evita o evento piscando por uma flutuação momentânea.

Isso é a generalização do contador de **dias em déficit** que já existe na Classe Social: o estoque abaixo de um limiar por um número mínimo de dias, com chance crescente quanto mais tempo durar.

### Chance depois da condição

A condição cumprida **habilita** o evento. Cada evento define a chance:

- **100%** para consequências óbvias (guerra → escassez).
- **Chance diária menor** para as incertas (Criminalidade alta → surgem bandidos).

### Registro de causas ("Por quê?")

Todo evento guarda as suas **causas**: IDs da cidade, da guerra, da lei ou da facção que o originou. O jogador vê isso no aviso e no diário (exemplo: "Escassez de grãos em Porto Velho. Causa: guerra contra a Facção do Norte, 9 dias."). Para os ocasionais, a causa é o estado que aumentou a chance (por exemplo, "população densa, pouca comida").

### Exemplos de condicionais já previstos nos outros sistemas

| Origem | Condição | Evento |
|---|---|---|
| Classe Social | Dias em déficit de consumo | Revolta de Escravos e Civis, perda de empresas dos Ricos, mudança de lei por Nobres e Realeza |
| Cultura | Lei da facção violada em um pensamento extremo | Penalidade duradoura de Felicidade, com a causa registrada |
| Cultura / IA | Felicidade baixa por muito tempo | Evento político, revolta |
| Facção | Pressão de Nobres e Realeza, ou lei muito distante do ideal | Assembleia extraordinária |
| Facção | Morte do líder sem sucessor claro | Crise de sucessão |
| Facção | Guerra em andamento | Escassez e alta de preços na facção |
| Crime e Justiça | Criminalidade alta | Bandidos nas redondezas |
| Mapa | Mapa em disputa | Guerras constantes até uma facção dominar |

### Bandidos

Ligação com Crime e Justiça e com o Mapa:

- São **grupos soltos**, não uma facção. Têm tamanho proporcional à Criminalidade da cidade mais próxima.
- Ocupam mapas **Sem dono** ou de fronteira, atacam caravanas e viajantes, e podem ser combatidos ou subornados.
- Em **LOD 2**, viram um número de "ameaça bandida" por região, sem simular o grupo.

---

## Narrador

O Narrador é o **regulador de ritmo** do jogo (como o de RimWorld), sem criar eventos do nada.

### O que ele faz

- Mantém um nível de **tensão** do jogador. Quando está alto, reduz a chance dos ocasionais negativos. Quando está baixo há muito tempo, aumenta.
- Mexe **só nos ocasionais**. Os condicionais nunca são alterados por ele.
- Atua só nos eventos que afetam o **jogador** (as cidades dele, o grupo e as rotas). O resto do mundo usa o sorteio simples, sem Narrador, então o mundo é igual para todos e o jogador não é especial na simulação.

### Narradores selecionáveis

O jogador escolhe um narrador no início do jogo. **O narrador é a dificuldade do jogo, mas um não é mais difícil que o outro:** cada um tem um **foco**, e o jogador combina o foco com o estilo de jogo que deseja (ver Sistema de Início de Jogo).

- O foco muda **quais categorias de ocasionais** o Narrador favorece e o ritmo da tensão. Ele não muda as regras do mundo.

**Os 4 narradores** (rodada V3-5; nomes significativos até a história):

| Narrador | Foco | Combina com |
|---|---|---|
| O Cronista | Equilíbrio entre todas as categorias | Qualquer estilo |
| A Guerra | Ameaça, guerra e bandidos | Aventureiro, Membro de facção |
| A Terra | Clima, colheita, saúde e economia | Dono de vila, Mercador |
| A Corte | Política, intrigas e sucessão | Nobre exilado, Membro de facção |

- **Intensidade:** um controle separado, de 50% a 150% (padrão 100%), que multiplica a tensão. O foco muda **o quê**; a intensidade muda **quanto**.
- **[Proposta]** Quatro focos iniciais, com os nomes a definir:
  - **Natureza:** clima, saúde, fauna e sorte.
  - **Política:** visitantes, embaixadas, pedidos e rumores, mais pressão social.
  - **Ameaça:** saques, bandos e perigos de fora.
  - **Comércio:** mercadores, ofertas e oportunidades de negócio.
- **[Em aberto]** Se existe também um controle separado de intensidade.

### Ofertas e pedidos

Pedido de ajuda, contrato, caravana atacada e similares entram como **eventos com escolha**. Missões longas e objetivos ficam para o sistema de **Objetivo do jogo e progressão**.

---

## Apresentação ao jogador

### Eventos com escolha

Um evento pode ter de **0 a 3 opções**, cada uma com consequências próprias (pagar resgate, ajudar, ignorar). Se o evento afeta uma facção de IA, a decisão sai da **personalidade do líder**, como já está na Facção.

### Notificações

Com as velocidades altas (até 8x no Mapa Mundial), o jogador perde coisas. Três níveis:

| Nível | Efeito |
|---|---|
| **Info** | Só vai para o diário |
| **Aviso** | Ícone e som |
| **Crítico** | Pausa o jogo automaticamente |

Há um filtro por distância: eventos longe do jogador vão só para o diário. O jogador também pode **personalizar** as notificações (categorias, limite, agrupamento, o que aparece em velocidade alta e se o Crítico pausa); ver [[Interface/Descrição|Interface e Controles]].

### Diário de eventos

Histórico com **data** (ano, estação, dia), **escopo** e **causas**. Tem limite de tamanho e é gravado no save. A **Crônica do mundo** (o que o jogador soube do resto do mundo) fica nas [[Sistema de Notícias/Descrição|Notícias]].

### Diário e eventos da fatia vertical (rodadas V3-5 e V3-6)

- **Diário:** guarda as últimas **300 entradas**, e as mais antigas vão para um resumo por ano; filtros por categoria, lugar e Aviso; busca por texto.
- **12 eventos da fatia vertical:**

| Evento | Tipo | Gatilho e efeito |
|---|---|---|
| Chuva boa | Ocasional (Clima) | Primavera; Fazenda +20% por 4 dias |
| Seca | Ocasional (Clima) | Verão; Fazenda −50% por 8 dias |
| Geada precoce | Ocasional (Clima) | Fim do outono; a colheita para 2 dias antes |
| Tempestade forte | Ocasional (Clima) | Danifica 1 construção de madeira (−20% de Vida) |
| Febre | Ocasional (Saúde) | Mortalidade +50% por 8 dias; gasta Remédio se houver |
| Colheita farta | Ocasional (Sorte) | Outono; Fazenda +30% por 4 dias |
| Mercador viajante | Ocasional (Sorte) | Uma caravana pequena vende 3 itens de fora |
| Achado na floresta | Ocasional (Sorte) | +20 Tora ou +5 Pedra de Mana no estoque |
| Ratos no celeiro | Ocasional (Natureza) | −10% do Grão estocado |
| Fome | Condicional | Comida abaixo de 10% por 3 dias: Felicidade −10 e migração para fora |
| Falta de móveis | Condicional | Materiais Básicos em déficit por 8 dias: Felicidade dos Civis −5 |
| Festa da Colheita | Data fixa (festival) | Felicidade +10 por 4 dias; consumo de Cerveja × 1,5 |

- Mais eventos vêm na rodada de conteúdo C3, usando o molde de [[Arquitetura/Moldes de Conteúdo|Moldes de Conteúdo]].

---

## Parte técnica

- **Dados:** eventos e narradores em JSON na pasta `data/`, com validação ao carregar (o jogo avisa se um evento citar item, lei, cultura ou categoria inexistente). Ver [[Arquitetura/Descrição|Arquitetura]].
- **Depuração:** um painel (Dear ImGui) mostra as chances atuais, os eventos ativos e permite **forçar um evento** para testar.
- **Aleatoriedade:** o Narrador e os eventos usam um gerador próprio, com semente. Mesma semente e mesmas ações geram os mesmos eventos, o que facilita achar bugs.
- **Save:** guarda os eventos ativos, os cooldowns, a tensão do Narrador, o narrador escolhido e o diário.
- **LOD:** sorteio por dia em LOD 0 e 1 e por estação em LOD 2. Eventos em LOD 2 têm efeito agregado.

### Ligação com outros sistemas

| Sistema | O que o Sistema de Eventos fornece ou recebe |
|---|---|
| Tempo | Sorteio por dia e por estação; durações em dias |
| Cidade / Inventário | Estado e estoques que alimentam as condições (déficit de consumo) |
| Classe Social | Dias em déficit como condição; reações por classe |
| Cultura | Pensamentos extremos violados e Felicidade como condições |
| Facção | Assembleia extraordinária, revolta, sucessão; escolhas da IA pela personalidade do líder |
| Crime e Justiça | Criminalidade gera bandidos; missão de confiança como evento |
| Econômico | Modificadores de preço recalculam o Mercado Mundial ou da Facção |
| Cura | Doenças e epidemias como eventos de Saúde |
| Mapa e Viagem | Mapas em disputa, grupos em mapas Sem dono, eventos por LOD; encontros durante a viagem usam a ameaça bandida (ver [[Sistema de Viagem/Descrição\|Sistema de Viagem]]) |
| NPC | Efeitos visíveis em NPCs; NPCs Importantes podem disparar eventos ao morrer |
| Combate | Criaturas e bandidos criados por eventos; evento Crítico pausa; combate reduz a velocidade para 1x |
| População | Doença, fome, guerra e crime como causas de mortalidade; nascimentos e migração por estação |
| Felicidade | As faixas de Felicidade e a Estabilidade disparam protestos, greves, rebeliões, saída de empresas e revolta; eventos mudam a Felicidade por modificadores com causa (ver [[Sistema de Felicidade e Estabilidade/Descrição\|Sistema de Felicidade e Estabilidade]]) |
| IA | Decisões de facções de IA nos eventos com escolha |
| Início de Jogo | Escolha do narrador na criação |
| Religião | Festivais do calendário; atos do Sumo Sacerdote (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| Clima e Estações | Eventos climáticos extremos (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Notícias | Cada evento pode virar notícia, com o modelo de frase e o "Por quê?" (ver [[Sistema de Notícias/Descrição\|Notícias]]) |

### Perguntas em aberto

1. **Valores a testar:** chance base de cada ocasional, limite de eventos negativos por cidade, escala da tensão do Narrador, limiares e histerese dos condicionais.
2. **Narradores:** os 4 e a intensidade foram decididos na V3-5; os pesos de cada foco ficam a testar.
3. **Categorias extras** de ocasionais (política, ameaça) e novos efeitos na lista fechada.
4. **Doença em LOD 0:** estado "Doente" no NPC ou só modificador da cidade? Depende do Sistema de Cura.
5. **Diário:** 300 entradas, filtros e busca (rodada V3-5).
6. **Lista inicial de eventos:** os 12 da fatia vertical foram decididos na V3-6; mais eventos na rodada de conteúdo C3.
