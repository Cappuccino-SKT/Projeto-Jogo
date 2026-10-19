---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 7** (a simulação sem tela de 1 ano). Os pontos marcados com FL7 vêm da **rodada FL7**, aplicada em 2026-10-18, toda com "seguir sugestão" (as decisões estão na parte 8 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]).

# Ficha Lógica: Simulação sem tela (o teste de 1 ano)

## 1. O que faz

`jogo --simular 64 --semente 1` roda **64 dias** da vila (1 ano) o mais rápido possível, sem abrir janela, grava o **relatório** (uma linha por dia) e termina com **PASSOU** ou **FALHOU** e o motivo (FT1-6). É o teste que diz se a economia da fatia se sustenta sozinha: o marco 7 está pronto quando ele passa (Fatia Vertical, parte 7).

Documentos: a parte 6 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]] (o teste, o relatório, os critérios, a repetição e o desempenho) e a [[Arquitetura/Descrição|Arquitetura]] (o Tracy e a meta de 500 NPCs, V2-11).

**A divisão das pastas:** o `--simular` fica no `Jogo/main.cpp`, como o `--validar`, mas só chama o `Codigo/`: a `SimulacaoDaVila` do marco 6 (a mesma do jogo com janela) e o relatório em `Codigo/Simulacao/`.

## 2. Dados

### A linha de comando

| Comando | O que faz |
|---|---|
| `jogo --simular 64 --semente 1` | 64 dias com a semente 1 (no lugar da `semente_da_partida` do `vila_inicial.json`) |
| `jogo --simular 64` | 64 dias com a semente do arquivo |
| `jogo --simular 8 --npcs-a-mais 485` | O teste de desempenho: 8 dias com 500 NPCs; só mede o tempo, sem os critérios (FL7-8) |

### O relatório: `relatorio.csv` (FL7-8)

Gravado na pasta de onde o jogo foi chamado, com ponto e vírgula entre as colunas e vírgula nos decimais (abre direto no Excel e no LibreOffice em português). Uma linha por dia, escrita na meia-noite, depois da ordem do dia:

| Coluna | Exemplo |
|---|---|
| `dia`, `estacao`, `dia_da_estacao` | 37; Outono; 5 |
| Um estoque por item (os dois donos juntos): `pao`, `grao`, `agua`, `tora`, `tabua`, `moveis_madeira`, `pedra`, `bloco_de_pedra` | 52; 240; ... |
| `preco_do_pao` (o preço local) | 1,9 |
| `banco`, `jogador`, `riqueza_media` | 402,3; 81,0; 12,1 |
| `felicidade` | 58,4 |
| `empregados`, `desempregados` | 12; 3 |
| `deficit_comida`, `deficit_materiais` (os dias seguidos) | 0; 0 |
| `consumo_nao_pago` | 0,0 |
| `eventos_do_dia` (os que começaram ou terminaram) | "Seca começou" |

## 3. Quando roda

Só pela linha de comando. O jogo com janela não muda.

## 4. Regras passo a passo

### 4.1 O `--simular`

1. Lê os dados como o `--validar`. Com erro, escreve os erros e termina com 1.
2. Monta a `SimulacaoDaVila` (marco 6) com a semente pedida.
3. Avança tick a tick, sem esperar o tempo real, até a meia-noite do último dia: 64 dias dão 64 linhas.
4. Em cada meia-noite, depois da ordem do dia: escreve a linha do relatório e confere os critérios (4.2).
5. No fim, escreve no console "PASSOU" ou "FALHOU: " com o **primeiro** problema, o tempo que levou e o tempo médio de um tick, e termina com 0 (passou) ou 1 (falhou).

### 4.2 Os critérios (Fatia Vertical, parte 6)

| Critério | Falha quando | A mensagem |
|---|---|---|
| Fome | 4 ou mais dias seguidos em déficit de comida | "fome por 4 dias seguidos no dia 37" |
| Banco | O Banco da Cidade chega a 0 ou menos | "o Banco zerou no dia 12" |
| Preço do Pão | O preço local sai da faixa de 1,2 a 2,6 | "o preço do Pão foi a 2,7 no dia 20" |

### 4.3 A repetição

A mesma semente dá o mesmo relatório, linha por linha. O `ctest` confere isso com 2 rodadas de 16 dias (FL7-8), e roda o teste de 64 dias uma vez, como um teste à parte.

### 4.4 O desempenho

1. **O tempo médio de um tick**, medido pelo relógio do C++ (`std::chrono`), sai sempre no fim do `--simular`.
2. **O Tracy** entra como uma opção do CMake, `USAR_TRACY`, desligada por padrão: ligada, o jogo manda as medições para o programa do Tracy (baixado à parte), que mostra quanto tempo cada regra gasta (FL7-8).
3. **Os 500 NPCs** (a meta da V2-11): o `--npcs-a-mais 485` cria 485 Civis a mais, sem casa nem emprego, que só passeiam. A meta: a 4x (80 ticks por segundo), cada tick gasta no máximo **3 milissegundos** em média, para sobrar tempo para o desenho a 60 quadros por segundo (FL7-8).

## 5. Contas

**Quantos ticks:** 64 dias × 19.200 = 1.228.800 ticks (a partida começa às 6h; o último dia termina na meia-noite do dia 65).

**O tempo do teste:** no marco 6, 3 dias sem tela levam cerca de 1,5 segundo (a medir): 64 dias, cerca de 30 segundos. Por isso o `ctest` confere a repetição com 16 dias, e o teste de 64 dias é um teste à parte.

**A meta dos 500 NPCs:** a 4x, um segundo real tem 80 ticks. A 60 quadros por segundo, um quadro tem 16,7 milissegundos e cerca de 1,3 tick. Com 3 milissegundos por tick, a simulação usa cerca de 4 milissegundos por quadro, e o resto fica para o desenho.

## 6. Conversa com outros sistemas

| Sistema | O que a simulação lê |
|---|---|
| A `SimulacaoDaVila` (marco 6) | Tudo: avança a vila tick a tick |
| Inventário, Economia, Felicidade, Eventos | As colunas do relatório e os critérios |
| Dados | Os mesmos do jogo, lidos como o `--validar` |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Codigo/Simulacao/RelatorioDaSimulacao.h`: `LinhaDoRelatorio`, `montarLinhaDoRelatorio(...)`, `escreverCabecalhoDoRelatorio()` | Uma linha do CSV |
| `ConferenciaDoTesteDeUmAno`, `conferirOsCriteriosDoDia(...)` | A regra 4.2 |
| `rodarASimulacaoSemTela(dados, dias, semente, npcsAMais)` | A regra 4.1, chamada pelo `main.cpp` |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| `jogo --simular 64 --semente 1` | 64 linhas no relatório; PASSOU (o teste do marco 7) |
| A mesma semente, 16 dias, duas vezes | Os mesmos relatórios, linha por linha |
| Semente 1 e semente 2 | Relatórios diferentes (os passeios e os eventos mudam) |
| 4 dias seguidos sem comida (um caso de teste) | FALHOU: "fome por 4 dias seguidos no dia ..." |
| O Banco em 0 (um caso de teste) | FALHOU: "o Banco zerou no dia ..." |
| `--simular` com um erro nos dados | Os erros no console; termina com 1, sem relatório |
| `--simular 8 --npcs-a-mais 485` | Escreve o tempo médio de um tick; sem PASSOU nem FALHOU |

## 9. Fora da fatia (e fora do marco 7)

- O LOD 1 e o LOD 2 (as outras cidades e a simulação por estação), e a meta de 5.000 NPCs em LOD 1.
- Vários núcleos (Arquitetura: objetivo futuro).
