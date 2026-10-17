> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de IA

**Descrição geral**

Define como os NPCs decidem o que fazer: a **rotina diária** dos Não-Controlados, a **IA de combate local**, as **necessidades** (de forma individual para os Controlados e agregada para os outros) e a ligação com a IA das facções. Os pesos das decisões das facções estão na [[Sistema de Diplomacia/Descrição|Diplomacia]].

### Rotina

As ações de um NPC Não-Controlado seguem a **rotina**: um ciclo diário de 24 horas dividido em 3 partes, **dormir, trabalhar e Período Livre**. Cada classe social tem uma distribuição diferente desses períodos ao longo do dia, e algumas Funções também têm a sua.

| Período | Para o NPC | Para o jogo |
|---|---|---|
| **Dormir** | Normalmente de madrugada: os NPCs vão para casa dormir | É o melhor momento para o crime. Os guardas não têm esse período e ficam em atividade o dia todo |
| **Trabalho** | Os NPCs vão às construções ligadas à sua Função e produzem para a cidade; cada um procura a construção correspondente | Movimenta a produção |
| **Livre** | Os NPCs frequentam lojas, tavernas, o Templo e outras construções de entretenimento, e circulam pela cidade | Movimenta a economia e dá ambientação |

Conforme a Função, a rotina deixa de depender dos períodos do dia e se torna **contínua**, como a dos guardas, que trabalham 24 horas fazendo rondas.

**Horas da rotina** (rodada V2-2):

| Classe | Dormir | Trabalhar | Período Livre |
|---|---|---|---|
| Escravo | 22h às 5h (7 h) | 5h às 17h (12 h) | 17h às 22h (5 h) |
| Civil | 22h às 6h (8 h) | 7h às 15h (8 h) | 6h às 7h e 15h às 22h (8 h) |
| Rico | 23h às 7h (8 h) | 9h às 15h (6 h, gerência) | 10 h |
| Nobreza e Realeza | 0h às 8h (8 h) | 10h às 14h (4 h, política e corte) | 12 h |
| Guardas e Funções contínuas | Turnos de 8 h (6h às 14h, 14h às 22h, 22h às 6h); cada guarda faz 1 turno | | |

Um turno de 8 h dura 5 min e 20 s reais a 1x. A madrugada, com só o turno da noite acordado, é a hora do crime.

### IA de combate local

- **Início:** a agressividade da IA no Mapa Local depende de **gatilhos e ações do jogador**: atacar um NPC, roubar, insultar, entre outros. Esses gatilhos iniciam uma ação hostil do NPC afetado.
- Conforme a Função, o gatilho é diferente: os **guardas** podem ficar hostis mesmo sem ligação direta com a situação, agindo para pacificá-la ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).
- **Durante o combate:** a IA ataca alvos individuais, priorizando os **mais próximos**. Depois de incapacitar o alvo, procura outro hostil por perto, até as ameaças acabarem. Troca de arma conforme a distância do inimigo.
- **Dano:** usa a fórmula do [[Sistema de Combate/Descrição|Sistema de Combate]] (dano da arma × habilidade ÷ 50 − Defesa do alvo, com piso de 1), o mesmo para o jogador e para a IA.

### Sistema de decisão

O sistema de decisão é **híbrido**, ligado ao LOD do Mapa Local:

- **LOD 0** (visão do jogador): sistema de **Utilidade**, que pesa ao mesmo tempo Bens de Consumo, Felicidade, Medo e Personalidade para escolher a melhor ação.
- **LOD 1 e 2:** **Regras Fixas**, mais baratas de processar.

Isso se liga à agregação por cidade dos Não-Controlados: o LOD 2 é exatamente o caso em que essa agregação se aplica.

**Utilidade simples** (rodada V2-10, a da fatia vertical):

- **Ações:** Dormir, Trabalhar, Comer, Comprar, Lazer (Taverna) e Fugir.
- **Nota:** 1,0 se a ação é a do período da rotina (0 se não), mais os bônus:
  - **Comer:** +1,5 nas refeições (7h, 12h e 19h, por 30 min);
  - **Comprar:** +1,2 uma vez a cada 2 dias, no Período Livre. A compra de verdade é a da cidade; a visita mostra isso na tela;
  - **Lazer:** +0,5 × (1 − Felicidade ÷ 100);
  - **Fugir:** +3,0 com combate a menos de 10 tiles, o que passa por cima de tudo.
- O NPC faz a ação de maior nota e reavalia a cada **5 minutos de jogo** (cerca de 3 s reais) ou quando algo muda.
- **LOD 1:** a ação do período, sem as notas (Regras Fixas).

### Necessidades dos Controlados

Esses atributos são **individuais** para cada NPC do grupo do jogador ou com interação próxima com ele (Importantes):

- **Fome:** ao se aproximar de 50, a IA come do inventário (ver [[Sistema de NPC/Descrição|NPC]]).
- **Felicidade:** abaixo de 20, o jogador recebe um aviso; ao chegar a 0, o NPC pede para sair do grupo (só vai embora depois de alguns dias, o que dá chance de resolver por diálogo ou pagamento), migrar ou se revoltar ([[Sistema de Felicidade e Estabilidade/Descrição|Felicidade e Estabilidade]]).
- **Medo:** varia conforme a Função. Para os Escravos, valores altos ajudam a prevenir revoltas (ver a Obediência na [[Sistema de Escravidão/Descrição|Escravidão]]); para os outros NPCs, decide quando recuar, se render ou fugir.

### Necessidades dos Não-Controlados

Para a IA geral, as necessidades são usadas de forma **agregada, por cidade**, em vez de calculadas NPC por NPC, o que economiza processamento. Conforme o estado delas, eventos são disparados sem precisar calcular a necessidade de cada NPC.

- **Bens de Consumo:** Comida e os outros recursos exigidos pela classe social ([[Sistema de Classe Social/Descrição|Classe Social]]), calculados pela quantidade desses itens no inventário da cidade, em proporção ao número de NPCs. A falta causa eventos de reação, que variam conforme a classe. Substitui a antiga Fome como atributo separado, porque toda classe já exige Comida.
- **Felicidade:** guardada por classe social em cada cidade; a falta dispara principalmente eventos políticos.
- **Medo:** usado de formas variadas; de base, decide se o NPC foge ou continua lutando.

---

## Parte técnica

- **Frequência:** a Utilidade (LOD 0) reavalia a ação a cada poucos segundos de jogo ou quando algo muda; as Regras Fixas (LOD 1) só nas trocas de período da rotina; em LOD 2 não há decisão individual.
- **Dados:** horários da rotina por classe e Função, gatilhos de hostilidade e pesos da Utilidade em JSON.
- **Depuração:** painel (Dear ImGui) com a ação escolhida por um NPC e a nota de cada opção.
- **Save:** só a ação atual e o destino dos NPCs em LOD 0 e 1.
- **Fatia vertical:** rotina com os 3 períodos e uma Utilidade simples (trabalhar, comer, comprar, dormir).

## Ligação com outros sistemas

| Sistema | O que a IA fornece ou recebe |
|---|---|
| NPC | Personalidade, Rotina, Ação atual, Destino; Fome e Felicidade dos Controlados |
| Facção e Diplomacia | IA de facção em baixa frequência (a cada 4 dias para guerra e paz; por estação para tratados), pela Personalidade do líder ([[Sistema de Diplomacia/Descrição\|Diplomacia]]) |
| Crime e Justiça | Reação escalonada dos guardas (rendição, prisão não letal, força letal); detecção por Percepção e Destreza; suborno por Ganância e Lealdade |
| Combate | Fórmula de dano, posturas dos Controlados, Medo e rendição ([[Sistema de Combate/Descrição\|Combate]]) |
| Guerra e Batalha | Os Esquadrões não usam a IA individual; guerras de IA pelo cálculo rápido ([[Sistema de Guerra/Descrição\|Guerra]], [[Sistema de Batalha/Descrição\|Batalha]]) |
| Cura | Recuperação de Vida ([[Sistema de Cura/Descrição\|Cura]]) |
| Eventos | Nos eventos com escolha que afetam facções de IA, a decisão sai da Personalidade do líder ([[Sistema de Eventos/Descrição\|Eventos]]) |
| Clima e Estações | Tempestade: os NPCs ficam em casa no Período Livre ([[Sistema de Clima e Estações/Descrição\|Clima]]) |
| Felicidade e Escravidão | Medo agregado no Controle e na Obediência |

## Perguntas em aberto

1. **Valores a testar:** horas da rotina e notas da Utilidade (rodada V2).
2. **Utilidade:** a lista de ações e como os pesos se combinam (rodada V2, versão simples para a fatia vertical).
3. **Medo agregado** da cidade em LOD 2 (pendência da Felicidade).
