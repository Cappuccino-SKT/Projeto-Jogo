> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Notícias e Informação

**Descrição geral**

Define **como o jogador fica sabendo** do que acontece no mundo: cada acontecimento vira uma **notícia**, que nasce num lugar e numa hora e chega aos outros mapas com atraso. A notícia aparece no quadro da Taverna, nas conversas, nas notificações e no Mapa Mundial, e alimenta a **Crônica do mundo**. É o jeito de mostrar a simulação funcionando sem quebrar a névoa.

Já estava decidido e vale como base:

- O jogador só conhece o mundo com **informações datadas** (névoa); conhece os preços das cidades visitadas, com a data ([[Sistema de Viagem/Descrição|Viagem]], [[Sistema de Comércio/Descrição|Comércio]]).
- As propostas diplomáticas são instantâneas, mas as **notícias da guerra chegam com atraso** ([[Sistema de Diplomacia/Descrição|Diplomacia]]).
- "Conversar" dá rumores e notícias; peregrinos trazem rumores ([[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]]).
- Todo evento e toda decisão da IA guardam o **"Por quê?"** ([[Sistema de Eventos/Descrição|Eventos]]).

Os valores numéricos são iniciais e ficam em dados (`noticias.json`) para ajustar nos testes.

---

## Como as notícias andam

Toda notícia **nasce num Mapa Local, numa hora**. Ela chega a cada outro mapa depois de **3 horas por mapa de distância**, o passo de um mensageiro a cavalo. Atravessar o mundo (8 mapas) leva **1 dia**.

- **Distância:** número de passos no grafo de 48 mapas, sem contar o terreno.
- O jogador recebe a notícia quando ela chega:
  - ao mapa onde ele está; **ou**
  - a um lugar da **rede dele**: as cidades dele (os governadores informam), os companheiros fora do grupo e as caravanas dele.
- **Custo:** ao nascer, a notícia calcula uma vez a distância até cada um dos 48 mapas; depois basta comparar horas. Quase nada.

*Exemplo:* a Guerreira declara guerra à Autoritária na sua capital (1,1) às 10 h. O jogador está na Cidade Religiosa (7,3), a 8 mapas: sabe 24 horas depois, às 10 h do dia seguinte. Se um companheiro dele estiver na Vila Livre do Norte (4,1), a 3 mapas, ele sabe às 19 h do mesmo dia.

## Tipos e alcance

Cada notícia tem uma **importância**, que define **até onde ela vai** e **quanto tempo fica** no quadro da Taverna:

| Importância | Alcance | Fica no quadro | Exemplos |
|---|---|---|---|
| **Local** | 2 mapas | 8 dias | Bandidos numa estrada, festival, preço alto numa cidade, contrato |
| **Regional** | 5 mapas | 16 dias | Batalha, cerco, revolta, epidemia, seca, companhia de mercenários |
| **Mundial** | O mundo todo | 64 dias | Guerra declarada, paz, morte de líder, sucessão, marco de tecnologia, Anátema |

- As notícias **nascem dos eventos e das decisões** que já guardam o "Por quê?": Eventos, Guerra, Diplomacia, Comércio, Religião, Clima, Viagem e os contratos do Objetivo.
- **Não há texto novo para escrever:** o modelo de frase vem do próprio evento ("A Facção Guerreira declarou guerra à Autoritária. Motivo: mapa em disputa (2,2).").
- Uma notícia além do alcance não chega àquele mapa; o jogador só a vê se alguém da rede dele estiver dentro do alcance.

## Onde o jogador vê as notícias

| Onde | O que mostra |
|---|---|
| **Notificação** | Categoria "Notícias", personalizável como as outras (já decidido; ver [[Interface/Descrição\|Interface]]) |
| **Quadro da Taverna** | As notícias que chegaram àquela cidade e ainda estão no prazo. É a "Crônica do mundo visível" da [[Revisão do Projeto]] |
| **Conversa** | "Conversar" com um NPC dá uma notícia Local ou Regional que já chegou à cidade dele |
| **Mapa Mundial** | A notícia atualiza a informação datada do mapa ("Cidade Autoritária ocupada pela Guerreira, soube há 2 dias") |
| **Diário e Crônica do mundo** | Uma aba com a data do acontecimento e a data em que o jogador soube |

- **Crônica do mundo:** guarda as notícias **Regionais e Mundiais** que o jogador soube. No **epílogo**, ela aparece junto com a Crônica do jogador ([[Objetivo e Progressão/Descrição|Objetivo e Progressão]]).
- O **diário de eventos** continua com o que acontece com o jogador e as cidades dele; a Crônica do mundo é o que ele soube do resto.

## Precisão

A notícia perde detalhe com a distância **entre a origem e onde o jogador a recebeu**:

| Distância | Como chega | Exemplo |
|---|---|---|
| Até 2 mapas | Números exatos | "1.800 soldados" |
| 3 a 5 mapas | Arredondados | "cerca de 2.000 soldados" |
| 6 mapas ou mais | Vagos | "um grande exército" |

- A notícia ganha a versão mais precisa quando o jogador chega mais perto ou quando alguém da rede dele, mais perto, a recebe.
- **Boatos falsos** ficam para o futuro, para não frustrar na primeira versão. **[Refinar depois]**

## Preços como notícia

- Quando um item fica **30% acima ou abaixo** da média da facção numa cidade, nasce uma notícia **Local** ("Grão caro na Capital Guerreira, +45%").
- Ao chegar, ela atualiza a **tabela de preços conhecidos** do jogador para aquele item, com a data.
- **Caravanas e companheiros comerciantes** do jogador trazem a **tabela completa** da cidade de onde vêm.
- Verificado uma vez por dia; o mesmo item na mesma cidade só vira notícia de novo depois de 8 dias, para não encher o quadro.
- Dá ao jogador rotas de comércio sem visitar tudo ([[Sistema de Comércio/Descrição|Comércio]], [[Sistema Econômico/Descrição|Econômico]]).

## A IA e as notícias

- **As facções da IA sabem tudo na hora.** É uma simplificação comum em jogos de estratégia: barata e simples. **[Refinar depois]**
- Os **NPCs comuns** só "sabem" (para os rumores da conversa) o que já chegou à cidade deles.
- **Futuro:** mensageiros para as propostas diplomáticas e IA que também espera as notícias ([[Sistema de Diplomacia/Descrição|Diplomacia]]).

---

## Parte técnica

- **Dados:** `noticias.json` com o tempo por mapa, as importâncias (alcance e prazo no quadro), as faixas de precisão, o limiar dos preços e o modelo de frase de cada tipo de notícia que não venha de um evento.
- **Registro da notícia:** ID, tipo, importância, mapa e hora de origem, causas (o "Por quê?"), números (para a precisão), hora de chegada em cada mapa (48 valores) e prazo.
- **Frequência:** a chegada é conferida por hora de jogo, comparando horas; nada roda por tick.
- **Memória:** as notícias saem da lista ao fim do prazo; as Regionais e Mundiais que o jogador soube passam para a Crônica do mundo. Limite de 500 notícias ativas no mundo.
- **Save:** notícias ativas, a Crônica do mundo e a tabela de preços conhecidos do jogador com as datas.
- **Depuração:** painel (Dear ImGui) com as notícias ativas, a origem e a hora de chegada em cada mapa.
- **Fatia vertical:** não usa (uma cidade só).

## Ligação com outros sistemas

| Sistema | O que as Notícias fornecem ou recebem |
|---|---|
| Eventos | Os eventos geram as notícias, com o modelo de frase e o "Por quê?"; a Crônica do mundo complementa o diário |
| Viagem | Névoa e informações datadas; a notícia anda 3 h por mapa; peregrinos e caravanas trazem notícias |
| Guerra e Diplomacia | Declarações, batalhas, cercos, paz e tratados viram notícia; a IA sabe na hora |
| Comércio e Econômico | Preços 30% fora da média viram notícia; caravanas trazem a tabela completa |
| Companheiros e Diálogo | "Conversar" dá notícias da cidade; companheiros fora do grupo fazem parte da rede |
| Religião | Festivais (Local) e Anátema (Mundial) |
| Clima e Estações | Eventos extremos viram notícia Regional; o inverno rigoroso, Mundial |
| Objetivo e Progressão | Contratos aparecem como notícia Local; Crônica do mundo no epílogo |
| Tecnologia | Marcos mundiais viram notícia Mundial |
| Interface | Categoria "Notícias" nas notificações; quadro da Taverna; aba Crônica do mundo; mapa atualizado |
| Construção | O quadro da Taverna mostra as notícias junto dos contratos |

## Perguntas em aberto

1. **Valores a testar:** horas por mapa, alcance e prazo de cada importância, faixas de precisão, limiar dos preços.
2. **Boatos falsos:** quando entram e como o jogador os desmente.
3. **IA que espera as notícias** e mensageiros para as propostas (futuro).
4. **Jornal impresso** com a tecnologia (uma notícia mais rápida e precisa nas cidades com imprensa)?
