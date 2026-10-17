---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-17 para o **marco 1** (janela, loop, relógio fixo e mapa de tiles). As propostas desta ficha estão na rodada **FL1** ([[Retomada]], seção 4).

# Ficha Lógica: Tempo

## 1. O que faz

O **relógio do mundo** conta o tempo do jogo em **ticks**, passos fixos de 4,5 segundos de jogo. Dele saem a data (ano, estação, dia, hora e minuto) e a fase do dia, e é ele que avisa quando começa uma hora, um dia ou uma estação nova. A velocidade (pausa, 1x, 2x, 4x) muda **quantos** ticks rodam por segundo, nunca o tamanho de cada um.

Documento do sistema: [[Sistema de Tempo/Descrição|Sistema de Tempo]]. Mapa geral: [[Ficha Logica/Fatia Vertical|Fatia Vertical]], parte 3.

**Por que um passo fixo:** com o mesmo número de ticks, o resultado é sempre o mesmo, em qualquer PC e em qualquer velocidade. É isso que permite o teste sem tela (`--simular`) e o "mesma semente = mesma partida".

## 2. Dados

### Definições (não mudam durante o jogo)

No marco 1, ficam juntas numa estrutura do código. A partir do marco 3, são lidas do `data/regras.json` **[Proposta]**.

| Campo | Tipo | Unidade | Valor |
|---|---|---|---|
| `ticksPorSegundoReal` | inteiro | ticks | 20 (a 1x) |
| `ticksPorHoraDeJogo` | inteiro | ticks | 800 |
| `horasPorDia` | inteiro | horas | 24 |
| `diasPorEstacao` | inteiro | dias | 16 |
| `estacoesPorAno` | inteiro | estações | 4 |
| `anoInicialDaPartida` | inteiro | ano | 412 ([[Mundo e Estado Inicial/Descrição\|Mundo e Estado Inicial]]) |
| `horaInicialDaPartida` | inteiro | hora | 6 **[Proposta]** (FL1-2) |
| `velocidadesPermitidas` | lista de inteiros | multiplicador | 0 (pausa), 1, 2, 4 |
| `limiteDeTempoRealPorQuadro` | inteiro | milissegundos | 250 **[Proposta]** (FL1-3) |
| Horas das fases do dia | inteiros | hora | amanhecer 5h, dia 7h, entardecer 18h, noite 20h ([[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |

**Valores derivados** (calculados, não escritos no arquivo):

| Valor | Conta | Resultado |
|---|---|---|
| Ticks por dia | 800 × 24 | 19.200 |
| Ticks por estação | 19.200 × 16 | 307.200 |
| Ticks por ano | 307.200 × 4 | 1.228.800 |
| Tempo real de 1 tick a 1x | 1 s ÷ 20 | 50 milissegundos = 50.000.000 nanossegundos |

### Estado (muda durante o jogo)

| Campo | Tipo | Unidade | Exemplo | Vai no save? |
|---|---|---|---|---|
| `tickAtualDaPartida` | inteiro de 64 bits | ticks | 27.200 (10h do dia 2) | Sim |
| `velocidadeDoJogo` | inteiro | 0, 1, 2 ou 4 | 1 | Sim |
| `velocidadeAntesDaPausa` | inteiro | 1, 2 ou 4 | 2 | Sim |
| `nanossegundosAcumuladosParaOProximoTick` | inteiro de 64 bits | nanossegundos | 12.000.000 | Não (é menos de 1 tick) |

- **Um só contador:** a data inteira sai do `tickAtualDaPartida`. Nada de guardar o dia e a hora separados, porque dois contadores podem discordar.
- **Por que inteiro de 64 bits:** um ano tem 1,2 milhão de ticks. Um inteiro comum (32 bits) estoura em cerca de 1.700 anos de jogo, e o de 64 bits nunca estoura na prática. O custo é de 4 bytes a mais.
- **Por que nanossegundos inteiros, e não segundos com vírgula:** um número com vírgula perde um pouquinho a cada soma, e em horas de jogo o relógio atrasaria. Com inteiros, nenhum tempo se perde. O SDL3 já entrega o tempo em nanossegundos.
- **O tick 0** é a meia-noite (0h) do dia 1 da Primavera do Ano 412. A partida começa no tick da `horaInicialDaPartida`: às 6h, isso dá 6 × 800 = **4.800**.
- **Semente e estado do sorteio:** ficam no relógio, como diz a Fatia Vertical, mas só entram no marco 7, com a ficha dos Eventos.

## 3. Quando roda

- **A cada quadro da tela** (cerca de 60 por segundo): o `Jogo/` mede o tempo real que passou e pede ao relógio para avançar (regra 4.1).
- **A cada tick:** o relógio soma 1 e diz se começou uma hora, um dia, uma estação ou um ano (regra 4.2).
- **No teste sem tela** (`--simular`, marco 7): não há quadros nem tempo real. O teste chama "avançar 1 tick" direto, o mais rápido possível. Por isso o relógio **nunca pergunta as horas ao Windows**: quem mede o tempo real é o `Jogo/`, e o `Codigo/` só recebe o número.

## 4. Regras passo a passo

### 4.1 Avançar pelo tempo real (a cada quadro)

1. O `Jogo/` mede quantos nanossegundos reais passaram desde o quadro anterior e entrega o número ao relógio.
2. **Limite de segurança [Proposta]:** se passou mais de 250 milissegundos, use só 250. Isso acontece quando o PC trava ou quando a janela é arrastada (o Windows para o programa enquanto ela é arrastada). Sem o limite, o jogo tentaria rodar todos os ticks atrasados de uma vez e travaria de novo.
3. Multiplique pela `velocidadeDoJogo` e some ao acumulador. Na pausa (velocidade 0), não soma nada.
4. **Enquanto** o acumulador tiver 50.000.000 nanossegundos (1 tick) ou mais:
   - rode a regra 4.2 (avançar 1 tick);
   - tire 50.000.000 do acumulador.
5. O que sobrar no acumulador fica para o próximo quadro. Nenhum tempo se perde.
6. Devolva quantos ticks avançaram neste quadro. O número serve para a depuração e para os testes.

### 4.2 Avançar 1 tick

1. Some 1 ao `tickAtualDaPartida`.
2. Rode o que é "a cada tick". No marco 1 não há nada; o movimento dos NPCs entra no marco 4.
3. Confira as viradas, **nesta ordem**:
   1. **hora nova:** o tick é múltiplo de 800;
   2. **dia novo:** é múltiplo de 19.200 (sempre às 0h);
   3. **estação nova:** é múltiplo de 307.200;
   4. **ano novo:** é múltiplo de 1.228.800.

**Por que essa ordem:** às 0h, a hora das 23h às 0h ainda é do dia velho. A produção dessa hora precisa entrar antes da sequência do dia, que fecha o dia (consumo, salários, mercado). Do mesmo modo, a sequência do dia fecha o último dia da estação antes de a estação nova começar (produtividade nova, manutenção).

**No marco 1**, cada virada só escreve uma linha no console, por exemplo `Começou o dia 2 da Primavera, Ano 412`. As regras de verdade se penduram nessas viradas a partir do marco 5.

**Só conta como virada o tick que acabou de chegar.** A partida começa no tick 4.800, que é múltiplo de 800, mas ele não dispara "hora nova", porque ninguém avançou até ele.

### 4.3 Mudar a velocidade

1. **Escolher uma velocidade:** aceite só 0, 1, 2 ou 4. Outro valor é ignorado, com um aviso no console.
2. **Pausar e despausar (a mesma tecla):**
   - se o jogo está andando, guarde a velocidade em `velocidadeAntesDaPausa` e ponha 0;
   - se está pausado, volte para a `velocidadeAntesDaPausa`.
   - *Exemplo:* o jogador está em 2x e pausa. Ao despausar, volta para 2x, não para 1x.
3. **Na troca de velocidade, o acumulador não é zerado.** O que sobrou continua valendo.

### 4.4 Ler a data e a fase do dia

São só contas sobre o `tickAtualDaPartida` (parte 5). Nada é guardado, e a mesma pergunta sempre dá a mesma resposta.

## 5. Contas

Todas as divisões são **inteiras** (sem vírgula, a sobra é jogada fora), e "resto" é a sobra da divisão.

| O quê | Conta | Exemplo com o tick 1.000.000 |
|---|---|---|
| Dia da partida (contando do 0) | tick ÷ 19.200 | 52 |
| Hora do dia | (tick ÷ 800) resto 24 | 1.250 resto 24 = **2h** |
| Minuto da hora | (tick resto 800) × 60 ÷ 800 | 0 × 60 ÷ 800 = **0** |
| Dia do ano (do 0 ao 63) | dia da partida resto 64 | 52 |
| Estação (0 a 3) | dia do ano ÷ 16 | 3 = **Inverno** |
| Dia da estação (1 a 16) | (dia do ano resto 16) + 1 | **5** |
| Ano | 412 + dia da partida ÷ 64 | **412** |

O tick 1.000.000 é, então, **2h00 do dia 5 do Inverno, Ano 412**.

**Outros exemplos:**

- **Tick 27.200:** 27.200 ÷ 800 = 34, e 34 resto 24 = **10h**. 27.200 ÷ 19.200 = 1, então é o **dia 2** da Primavera.
- **Tick 19.199** (o último antes da virada): (19.199 resto 800) = 799, e 799 × 60 ÷ 800 = 59. É **23h59** do dia 1.
- **O minuto não é exato:** um minuto de jogo dura 13,3 ticks. O relógio da tela anda de minuto em minuto, mas às vezes um minuto leva 13 ticks e às vezes 14. Ninguém percebe, e a hora cheia é sempre exata.

**Fase do dia** (pela hora):

| Hora | Fase |
|---|---|
| 5h e 6h | Amanhecer |
| 7h às 17h | Dia |
| 18h e 19h | Entardecer |
| 20h às 4h | Noite |

**Ticks por quadro**, numa tela de 60 quadros por segundo:

| Velocidade | Ticks por segundo | Ticks por quadro | 1 dia de jogo leva |
|---|---|---|---|
| 1x | 20 | 0 ou 1 (1 tick a cada 3 quadros) | 16 min |
| 2x | 40 | 0 ou 1 (média 0,67) | 8 min |
| 4x | 80 | 1 ou 2 (média 1,33) | 4 min |
| Teste sem tela | sem limite | — | uma fração de segundo |

Com o limite de 250 milissegundos, um quadro roda no máximo 250 ÷ 50 × 4 = **20 ticks** (na 4x).

## 6. Conversa com outros sistemas

| Sistema | O que lê do relógio | O que entrega ao relógio |
|---|---|---|
| Jogo (o laço principal) | Quantos ticks rodaram | Os nanossegundos reais de cada quadro; as teclas de velocidade |
| Interface (marco 8; no marco 1, o título da janela) | Data, hora, fase e velocidade | — |
| Produção e obras (marco 6) | A virada de hora | — |
| Sequência do dia (marcos 6 e 7) | A virada de dia, sempre às 0h | — |
| Clima e Estações | A estação (produtividade) e a fase do dia (tom de cor) | — |
| IA e rotina (marco 5) | A hora do dia (dormir, trabalhar, livre) | — |
| Eventos (marco 7) | A virada de dia | A semente e o estado do sorteio ficam guardados no relógio |
| Save (marco 9) | Tick, velocidade e velocidade antes da pausa | — |

## 7. Nomes no código

Ficam em `Codigo/Tempo/`, sem nada do SDL nem do OpenGL.

| Nome | O que é |
|---|---|
| `RegrasDoTempo` | A estrutura com as definições da parte 2 |
| `RelogioDoMundo` | A classe com o estado da parte 2 e as regras da parte 4 |
| `avancarTicksPeloTempoReal(nanossegundosReaisDesdeOUltimoQuadro)` | A regra 4.1. Devolve `quantidadeDeTicksAvancados` |
| `avancarUmTick()` | A regra 4.2 |
| `comecouUmaHoraNova()`, `comecouUmDiaNovo()`, `comecouUmaEstacaoNova()`, `comecouUmAnoNovo()` | As viradas do tick que acabou de chegar |
| `definirVelocidadeDoJogo(novaVelocidade)` e `alternarPausa()` | A regra 4.3 |
| `horaDoDiaAtual()`, `minutoDaHoraAtual()`, `diaDaPartida()`, `diaDoAnoAtual()`, `diaDaEstacaoAtual()`, `anoAtual()` | As contas da parte 5 |
| `estacaoAtual()`, que devolve `Estacao` | `Primavera`, `Verao`, `Outono`, `Inverno` |
| `faseDoDiaAtual()`, que devolve `FaseDoDia` | `Amanhecer`, `Dia`, `Entardecer`, `Noite` |

No `Jogo/`, a função `escreverDataNoTituloDaJanela` monta o texto da tela (por exemplo, "Primavera, dia 1, 06h00, Ano 412, 1x"), porque escrever na tela não é regra do jogo **[Proposta]** (FL1-3).

## 8. Testes

Cada linha vira um teste automático (o jeito de rodar está na FL1-4).

| Entrada | Resultado esperado |
|---|---|
| Velocidade 1, 4.000 quadros de 10 milissegundos (40 s reais) a partir do tick 0 | 800 ticks; hora 1h |
| Velocidade 0, 10 s reais | 0 ticks |
| Velocidade 4, 10 s reais | 800 ticks |
| Velocidade 1, um quadro de 2 s | Limitado a 250 milissegundos: 5 ticks |
| Velocidade 1, quadros de 30 milissegundos | 0, 1, 0, 1, 1 tick, e repete: 3 ticks a cada 150 milissegundos, sem perder tempo |
| Tick 27.200 | Dia 2 da Primavera, 10h00, fase Dia |
| Tick 19.199 | Dia 1, 23h59, fase Noite |
| Tick 1.000.000 | Inverno, dia 5, 2h00, Ano 412 |
| Tick 307.200 | Verão, dia 1, 0h00 |
| Tick 1.228.800 | Primavera, dia 1, Ano 413 |
| Horas 4, 5, 7, 18 e 20 | Noite, Amanhecer, Dia, Entardecer, Noite |
| Avançar 1 tick a partir do 19.199 | `comecouUmaHoraNova` e `comecouUmDiaNovo` verdadeiros; `comecouUmaEstacaoNova` falso |
| Avançar 1 tick a partir do 19.200 | Nenhuma virada |
| Partida nova | Tick 4.800, 6h00 do dia 1 da Primavera, Ano 412; nenhuma virada no começo |
| Em 2x, pausar e despausar | Volta para 2x |
| Pedir a velocidade 3 | Ignorada; continua na anterior |

## 9. Fora da fatia

O documento do sistema tem, e esta versão ainda não faz:

- a velocidade 8x, só do Mapa Mundial;
- a frequência por LOD (1 a cada 10 ticks no LOD 1, 1 por hora no LOD 2), porque a fatia está sempre em LOD 0;
- o relógio parado durante a batalha e as 2 horas somadas no fim;
- a volta automática para 1x no combate, nos encontros e nas chegadas da Viagem;
- o calendário na interface, com as estações e os festivais.
