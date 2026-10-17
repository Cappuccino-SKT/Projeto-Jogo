> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Tempo (Horário)

**Descrição geral**

O jogo tem um ciclo de tempo anual. Cada ano tem **64 dias**, divididos em **4 estações de 16 dias** cada, representando as estações reais do ano: primavera, verão, outono e inverno.

Um dia tem 24 horas, e **24 horas no jogo equivalem a 16 minutos reais** na velocidade 1x. O jogo tem 4 velocidades (1x, 2x, 4x e 8x); a **8x é exclusiva do Mapa Mundial**. Nos Mapas Locais, Internos e de Guerra, o máximo é 4x. **Durante uma batalha no Mapa de Guerra, o relógio do mundo fica parado**; no fim, ele avança 2 horas fixas (ver [[Sistema de Batalha/Descrição|Sistema de Batalha]]).

### Atributos

- Ano (começa num valor ligado à história do mundo: Ano 412; ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]])
- Estação
- Dia
- Hora
- Fase do dia (amanhecer, dia, entardecer, noite; derivada da hora)

### Períodos longos

Não existem meses no jogo. Períodos longos usam a **Estação** (16 dias) ou o **Ano** (64 dias). Por exemplo, a manutenção e a produção de construções em LOD 2 acontecem por estação, e a assembleia de leis das facções acontece uma vez por ano.

| Período | Duração em jogo | Tempo real a 1x |
|---|---|---|
| Hora | 1 h | 40 segundos |
| Dia | 24 h | 16 minutos |
| Estação | 16 dias | cerca de 4,3 horas |
| Ano | 64 dias | cerca de 17 horas |

### Estações

As estações mudam a produção Braçal, o clima do dia, a viagem, a guerra e a cor do mapa; o inverno pede lenha. O dia tem 4 fases de luz (amanhecer, dia, entardecer e noite), e à noite a visão e a detecção caem 30%. Ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]. Os festivais de cada cultura têm datas fixas no calendário (ver [[Sistema de Religião/Descrição|Religião e Fé]]).

### Frequência da simulação por LOD

A frequência do tick é amarrada ao **LOD do Mapa Local**, em vez de um valor fixo global:

- **LOD 0:** **20 ticks por segundo real** a 1x, em passo fixo (1 tick = 4,5 s de jogo: a 1x, 1 s real = 90 s de jogo, dividido por 20 ticks). A 4x, 80 ticks por segundo.
- **LOD 1:** 1 vez a cada **10 ticks** (2 por segundo).
- **LOD 2:** 1 vez por **hora de jogo** (a cada 40 s reais a 1x); a economia é somada por dia e por estação.

Valores da rodada V2-11, a medir na fatia vertical.

Isso também define o que pausa na velocidade 8x: a busca de caminho fina e a decisão individual dos Não-Controlados pausam; a posição dos exércitos em trânsito e os agregados por cidade (Bens de Consumo, Felicidade) continuam avançando. Assim, 1 dia dura 2 minutos reais nessa velocidade.

---

## Parte técnica

- **Relógio fixo:** a simulação anda em passos fixos, separados do desenho (ver [[Arquitetura/Descrição|Arquitetura]]). A velocidade (1x a 8x) muda quantos passos rodam por segundo, não o tamanho do passo.
- **Calendário:** ano, estação, dia e hora num único contador de horas de jogo; o resto é derivado.
- **Save:** o contador de horas de jogo e a velocidade atual.
- **Fatia vertical:** usa o relógio, as velocidades até 4x, as estações (produção) e as fases do dia (tom de cor).

## Ligação com outros sistemas

| Sistema | O que o Tempo fornece ou recebe |
|---|---|
| Eventos | Ocasionais sorteados uma vez por **dia** em LOD 0 e 1 e uma vez por **estação** em LOD 2; condicionais por sinal ou varredura diária ou sazonal (ver [[Sistema de Eventos/Descrição\|Sistema de Eventos]]) |
| Combate | Ao começar um combate, a velocidade volta sozinha para 1x; pausa ativa (ver [[Sistema de Combate/Descrição\|Sistema de Combate]]) |
| Batalha | O relógio do mundo para durante a batalha e avança 2 horas no fim |
| Viagem | 4 horas por Mapa Local a pé, com multiplicadores; acampamento à noite; encontros e chegadas voltam a velocidade para 1x (ver [[Sistema de Viagem/Descrição\|Sistema de Viagem]]) |
| Clima e Estações | Produção por estação, clima sorteado 1 vez por dia por região, fases do dia (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Notícias | As notícias andam 3 horas de jogo por mapa (ver [[Sistema de Notícias/Descrição\|Notícias]]) |
| População | Nascimentos, mortes e migração por estação; idade em anos de jogo |
| Facção | Assembleia uma vez por ano; decisões da IA a cada 4 dias e por estação |
| IA | Rotina diária de 24 horas (dormir, trabalhar, livre) |
| Interface | Data e hora na barra superior; calendário com estações e festivais |

## Perguntas em aberto

1. **Valores a testar:** 20 ticks por segundo e N = 10 no LOD 1 (rodada V2-11), medidos na fatia vertical.
