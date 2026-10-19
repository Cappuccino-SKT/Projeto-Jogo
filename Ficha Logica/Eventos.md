---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 7** (a simulação sem tela de 1 ano). Os pontos marcados com FL7 estão na **rodada FL7** ([[Retomada]], seção 4), ainda sem resposta: a ficha já usa as sugestões, marcadas como [Proposta].

# Ficha Lógica: Eventos

## 1. O que faz

Todo dia, a vila sorteia os **eventos ocasionais** (a seca, a colheita farta, os ratos...) e confere os **condicionais** (a fome, a falta de móveis). Um evento ativo muda a produção, a Felicidade ou o estoque por alguns dias. Cada um que começa ou termina vira uma linha no console e no relatório do teste de 1 ano.

Documento do sistema: [[Sistema de Eventos/Descrição|Eventos e Narrador]] (os tipos, a lista fechada de efeitos, o cooldown, a histerese e os 12 eventos da fatia, V3-6). A regra 4.12 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]] dá a ordem.

**Na fatia, sem o Narrador** [Proposta] (FL7-6): ele regula a tensão do jogador, e o jogador ainda não age no marco 7 (o teste de 1 ano roda sozinho). Fica o sorteio simples, que o documento já prevê para "o resto do mundo".

**A divisão das pastas:** `Codigo/Eventos/`, sem janela.

## 2. Dados

### Definições: o `data/eventos.json` [Proposta]

Um arquivo novo, lido depois do `vila_inicial.json`, conferido pelo `--validar` (um item, uma construção ou uma estação que não existe é erro). O nome de cada evento fica no `textos/pt-BR.json` (`"evento.seca": "Seca"`).

```json
[
  { "id": "evento.seca", "tipo": "ocasional", "categoria": "clima", "negativo": true,
    "estacoes": ["verao"], "chance_por_dia_em_por_mil": 40, "cooldown_dias": 32, "duracao_dias": 8,
    "efeitos": [ { "producao_da_construcao": "construcao.fazenda", "porcentagem": 50 } ] },
  { "id": "evento.fome", "tipo": "condicional", "negativo": true,
    "condicao": { "comida_abaixo_do_alvo_em_porcento": 10, "dias_seguidos": 3, "sai_acima_do_alvo_em_porcento": 30 },
    "efeitos": [ { "felicidade": -10 } ] }
]
```

| Campo | O que é |
|---|---|
| `tipo` | `ocasional`, `condicional` ou `data_fixa` |
| `negativo` | Conta no limite de 2 negativos ativos ao mesmo tempo (Eventos) |
| `estacoes`, `dias_da_estacao` | Quando o ocasional pode sair (vazio = sempre) |
| `chance_por_dia_em_por_mil` | 40 = 4% por dia. Inteiro, para o sorteio ser igual em qualquer computador |
| `cooldown_dias` | Dias, depois do fim, até poder sair de novo |
| `duracao_dias` | 0 = pontual (o efeito acontece uma vez) |
| `efeitos` | Da lista da parte 4.4 |

### Os 11 eventos da fatia [Proposta] (FL7-6)

Dos 12 da V3-6, a **Tempestade forte** fica de fora: ela danifica a Vida de uma construção, que só existe com as obras e a manutenção (marco 8). Quatro são adaptados ao que a fatia tem:

| Evento | Tipo | Quando | Chance por dia | Efeito | Duração | Cooldown |
|---|---|---|---|---|---|---|
| Chuva boa | Ocasional | Primavera | 6% | Fazenda produz 120% | 4 dias | 16 |
| Seca | Ocasional (negativo) | Verão | 4% | Fazenda produz 50% | 8 dias | 32 |
| Geada precoce | Ocasional (negativo) | Outono, dias 13 e 14 | 20% | Fazenda produz 0% (a colheita para 2 dias antes) | 2 dias | 64 |
| Colheita farta | Ocasional | Outono | 6% | Fazenda produz 130% | 4 dias | 16 |
| Ratos no celeiro | Ocasional (negativo) | Sempre | 2% | −10% do Grão da Cidade (arredondado para baixo) | Pontual | 16 |
| Achado na floresta | Ocasional | Sempre | 2% | +20 Toras para a Cidade (se couber). *Adaptado:* a Pedra de Mana não existe na fatia | Pontual | 16 |
| Mercador viajante | Ocasional | Sempre | 3% | Compra até 20 unidades do item da Cidade com mais dias de estoque, a 90% do preço local; o dinheiro vai para o Banco. *Adaptado:* não há itens de fora para ele vender | Pontual | 16 |
| Febre | Ocasional (negativo) | Sempre | 1,5% | 2 Civis sorteados ficam doentes em casa e não trabalham. *Adaptado:* sem mortalidade nem Remédio, que não existem na fatia | 4 dias | 32 |
| Fome | Condicional (negativo) | A comida abaixo de 10% do estoque-alvo por 3 dias seguidos | — | Felicidade −10. *Adaptado:* sem migração (a população é fixa na fatia). Sai com a comida acima de 30% do alvo | Enquanto valer | — |
| Falta de móveis | Condicional (negativo) | Materiais Básicos em déficit por 8 dias seguidos | — | Felicidade −5. Sai quando o déficit volta a 0 | Enquanto valer | — |
| Festa da Colheita | Data fixa | Dia 12 do Outono | — | Felicidade +10. *Adaptado:* sem o consumo de Cerveja, que não existe | 4 dias | — |

**Por que essas chances:** 6% por dia numa estação de 16 dias dá cerca de 1 vez por estação (0,96); 2% dá cerca de 1,3 vez por ano; a Geada, com 20% em 2 dias, sai em cerca de 1 ano a cada 3.

### Estado

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `eventosAtivos` | lista: o evento, os dias que faltam, as causas | Seca, 5 dias | Sim |
| `diaEmQuePodeSairDeNovo` | um por evento | Seca: dia 40 | Sim |
| `diasSeguidosNaCondicao` | um por condicional | Fome: 2 | Sim |
| O sorteio dos eventos | o estado do PCG32 | — | Sim |
| Nos NPCs doentes: `DoencaDoNpc` | dias que faltam | 3 | Sim |

**O sorteio** (ficha da IA, regra 4.6): um gerador só dos eventos, o PCG32 com a semente da partida e a **sequência 2** (os NPCs usam a 1). Assim, um NPC a mais não muda os eventos sorteados.

## 3. Quando roda

À meia-noite, no passo **"eventos"** do `ordem_do_dia` (o 6º, depois da Felicidade), para reagir ao estado final do dia (Fatia Vertical, parte 3). A Felicidade do dia seguinte já conta os eventos novos.

## 4. Regras passo a passo

### 4.1 Os eventos ativos

1. Cada evento ativo desconta 1 dia. Com 0, os efeitos saem, e o cooldown começa: `diaEmQuePodeSairDeNovo` = hoje + o cooldown.
2. Os NPCs doentes descontam 1 dia; com 0, voltam à rotina.

### 4.2 Os condicionais e as datas fixas

1. Para cada condicional: se a condição vale hoje, some 1 a `diasSeguidosNaCondicao`; se não, zere. Com os dias pedidos, e o evento não ativo, ele começa.
2. Um condicional ativo termina quando a condição de saída vale (a **histerese**: a Fome entra abaixo de 10% e só sai acima de 30%).
3. Uma data fixa começa no dia dela.

### 4.3 Os ocasionais

Na ordem do `eventos.json`, para cada ocasional:

1. Ele **pode sair** se: a estação e o dia batem, ele não está ativo, o cooldown acabou e, se é negativo, há menos de 2 negativos ativos.
2. Se pode, sorteie um número de 1 a 1.000. Se o número for menor ou igual à chance por mil, o evento começa.
3. Só os que podem sair gastam um número do sorteio (assim, a ordem dos sorteios é sempre a mesma para a mesma partida).

### 4.4 Os efeitos (parte da lista fechada do documento)

| Efeito | O que faz |
|---|---|
| `producao_da_construcao` | Enquanto ativo, o lote das receitas daquele tipo de construção é multiplicado pela porcentagem (com o resto guardado, como a estação: [[Ficha Logica/Producao\|Produção]], regra 4.2). Dois ativos se multiplicam |
| `felicidade` | Enquanto ativo, soma no fator "Eventos duradouros" ([[Ficha Logica/Felicidade\|Felicidade]]) |
| `mudar_estoque` | Pontual: tira uma porcentagem ou põe uma quantidade de um item da Cidade |
| `npcs_doentes` | Sorteia N Civis com emprego; cada um fica em casa (a ação Dormir ganha sempre) pelos dias do evento [Proposta] |
| `mercador_compra` | Pontual: compra do item da Cidade com mais dias de estoque (estoque ÷ consumo previsto), a 90% do preço local; o dinheiro entra no Banco. É a única entrada de dinheiro de fora da vila |

### 4.5 O aviso

Cada evento que começa ou termina escreve uma linha no console ("Evento: Seca (a Fazenda produz 50% por 8 dias)." e "Fim do evento: Seca.") e entra na coluna "eventos do dia" do relatório do teste de 1 ano. O diário de 300 entradas e as notificações na tela vêm com a interface (marco 8).

## 5. Contas

**Quanto a Seca custa:** a Fazenda faz cerca de 31,6 Grãos por dia; com 50% por 8 dias, faltam cerca de 126 Grãos. A vila gasta 8 por dia no Pão, e o estoque de Grão costuma estar acima de 100 no verão: a Seca não causa fome sozinha. Uma Seca com a Geada e os Ratos no mesmo ano é o que o teste de 1 ano quer ver.

**O Mercador viajante:** com Água a 300 no estoque e consumo de 4 por dia (75 dias de estoque), ele leva 20 Águas a 90% de 0,5 = 0,45, ou 9 moedas para o Banco.

## 6. Conversa com outros sistemas

| Sistema | O que os Eventos leem | O que entregam |
|---|---|---|
| Tempo | A estação, o dia da estação, o dia da partida | — |
| Inventário | A comida, o Grão, o espaço | Tira e põe itens |
| Economia | Os dias em déficit, o consumo previsto, o preço local | O dinheiro do Mercador |
| Produção | — | O multiplicador da produção |
| Felicidade | A Felicidade | O fator "Eventos duradouros" |
| IA (marco 5) | — | Os NPCs doentes ficam em casa |
| Simulação sem tela | — | Os eventos do dia no relatório |

## 7. Nomes no código

Em `Codigo/Eventos/`:

| Nome | O que é |
|---|---|
| `DefinicaoDeEvento`, `CatalogoDeEventos`, `lerArquivoDeEventos(...)` | O `eventos.json` |
| `EfeitoDoEvento` | Um efeito da lista da parte 4.4 |
| `EventoAtivo`, `EstadoDosEventos` | O estado da parte 2 |
| `DoencaDoNpc` | O componente do NPC doente |
| `rodarOsEventosDoDia(...)` | As regras 4.1 a 4.5 |
| `multiplicadorDaProducaoPorEventos(estado, tipoDaConstrucao)` | O que a Produção pergunta |
| `felicidadeDosEventosAtivos(estado)` | O que a Felicidade pergunta |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| O `eventos.json` da fatia | 11 eventos, sem erro |
| Um evento com o item `item.ouro` | Erro: o item não existe |
| A Seca no Inverno | Não pode sair (a estação não bate) |
| A Seca ativa, num ciclo de 2 Grãos | 1 Grão (50%) |
| A Seca e a Chuva boa juntas (um caso de teste) | 2 × 50% × 120% = 1,2 Grão: 1, com 20 de resto |
| Dois negativos ativos | Um terceiro negativo não pode sair; um positivo pode |
| A comida abaixo de 10% do alvo por 3 dias | A Fome começa; com 25% do alvo, continua; com 31%, termina |
| Os Ratos com 95 Grãos | Ficam 86 (9 tirados, 9,5 arredondado para baixo) |
| A Febre | 2 Civis com emprego ficam em casa 4 dias e não somam horas |
| A mesma semente, 64 dias, duas vezes | Os mesmos eventos, nos mesmos dias |

## 9. Fora da fatia (e fora do marco 7)

- O Narrador (a tensão e os 4 focos), as escolhas dos eventos e os eventos de outros escopos (Mundo, Facção, Grupo do jogador).
- A Tempestade forte (com a Vida das construções), a mortalidade e o Remédio da Febre, a migração da Fome e a Cerveja da Festa.
- O diário de 300 entradas e as notificações (marco 8).
