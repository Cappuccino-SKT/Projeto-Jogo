---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 5** (rotina). Cobre **a vila inicial, a rotina e a Utilidade simples**: os 15 NPCs dormem, trabalham, comem, compram e passeiam nos horários. As decisões da rodada **FL5** (2026-10-18, todas "seguir sugestão") já estão aplicadas.

# Ficha Lógica: IA (rotina e Utilidade)

## 1. O que faz

Cada NPC **escolhe o que fazer** (a ação) pela **Utilidade**: uma nota para cada ação, e ele faz a de maior nota. A ação diz **para onde ir** (a casa, o trabalho, o Mercado ou um passeio); o NPC anda até lá pelo caminho do marco 4 e, numa construção, **entra** e some da tela até sair.

Para isso, o marco 5 também monta a **vila inicial**: as 10 construções nos lotes, as 4 Casas e os 15 NPCs Civis, cada um com a casa e o emprego dele.

Documentos do sistema: [[Sistema de IA/Descrição|IA]] (a rotina, as horas da V2-2 e a Utilidade simples da V2-10) e as regras 4.9 e 4.10 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]. O movimento está na [[Ficha Logica/NPC|ficha do NPC]].

**A divisão das pastas:** a vila, a rotina, as notas e o sorteio ficam em `Codigo/` (o teste sem tela do marco 7 roda tudo isso); o `Jogo/` só desenha as construções e a depuração.

**O que fica de fora do marco 5:** a produção, o inventário, o dinheiro e o consumo de verdade (marco 6). No marco 5, **Comer** e **Comprar** são só idas e vindas, para a vila parecer viva; o que se come e o que se compra entra no marco 6.

## 2. Dados

### Definições (lidas dos dados, não mudam durante o jogo)

**As rotinas e as notas,** no `data/regras.json` (FL5-3). As notas ficam em **centésimos** (1,0 = 100), inteiras, pelo mesmo motivo do milésimo de tile: a mesma partida dá sempre o mesmo resultado.

```json
"rotinas": {
  "civil": { "dormir": [22, 6], "trabalhar": [7, 15] }
},
"utilidade": {
  "nota_da_acao_do_periodo": 100,
  "bonus_de_comer": 150,
  "horas_das_refeicoes": [7, 12, 19],
  "minutos_da_refeicao": 30,
  "bonus_de_comprar": 120,
  "dias_entre_compras": 2,
  "minutos_da_compra": 30,
  "minutos_entre_reavaliacoes": 5
},
"passeio": {
  "distancia_maxima_em_tiles": 12,
  "minutos_de_espera": [15, 45]
}
```

- Cada período vai da hora de início (inclusive) até a de fim (exclusive): `[22, 6]` é das 22h às 6h, passando pela meia-noite. O que não é dormir nem trabalhar é **Livre** (para o Civil, das 6h às 7h e das 15h às 22h, como na V2-2).
- As outras classes (Escravo, Rico, Nobreza, Guardas) entram depois da fatia, com uma linha cada.

**A vila inicial,** num arquivo novo, o `data/vila_inicial.json` (FL5-1 e FL5-2). No marco 5, só as construções, os NPCs e a semente; o Banco, o estoque e as 100 moedas do jogador entram no marco 6.

```json
{
  "semente_da_partida": 1,
  "construcoes": [
    { "id": "fazenda", "tipo": "construcao.fazenda", "lote": 1, "dono": "cidade" },
    { "id": "casa_1", "tipo": "construcao.casa", "lote": 2, "dono": "cidade" },
    { "id": "carpintaria_do_jogador", "tipo": "construcao.carpinteiro", "lote": 11, "dono": "jogador" }
  ],
  "npcs": [
    { "nome": "Ana", "classe": "civil", "casa": "casa_1", "trabalho": "fazenda" },
    { "nome": "Nuno", "classe": "civil", "casa": "casa_4", "trabalho": "" }
  ]
}
```

**A vila da fatia** (FL5-1 e FL5-2), nos 11 lotes do mapa (8 pequenos e 3 médios), pelos portes do [[Sistema de Construção/Descrição|Construção]] (as oficinas e o Mercado são médios):

| Lote | Tamanho | Construção (`id`) | Porta | Moradores ou empregados |
|---|---|---|---|---|
| 1 | Pequeno, em (12, 21) | Fazenda (`fazenda`) | (14, 25) | Ana, Bento |
| 6 | Pequeno, em (12, 29) | Posto de Coleta (`posto_de_coleta`) | (14, 33) | Clara, Davi |
| 10 | Médio, em (35, 29) | Cozinheiro (`cozinheiro`) | (38, 35) | Elisa, Fábio |
| 9 | Médio, em (45, 19) | Mercado (`mercado`) | (48, 25) | Gil, Helena (Lojistas) |
| 3 | Pequeno, em (22, 21) | Construtor (`construtor`) | (24, 25) | Iago, Joana (Operários) |
| 11 | Médio, em (42, 29) | Carpintaria do jogador (`carpintaria_do_jogador`) | (45, 35) | Lauro, Marta |
| 2 | Pequeno, em (17, 21) | Casa (`casa_1`) | (19, 25) | Ana, Bento, Clara, Davi |
| 7 | Pequeno, em (17, 29) | Casa (`casa_2`) | (19, 33) | Elisa, Fábio, Gil, Helena |
| 8 | Pequeno, em (22, 29) | Casa (`casa_3`) | (24, 33) | Iago, Joana, Lauro, Marta |
| 4 | Pequeno, em (35, 21) | Casa (`casa_4`) | (37, 25) | Nuno, Olga, Pedro (desempregados) |
| 5 | Pequeno, em (40, 21) | Livre (para a primeira obra) | — | — |

- **Os nomes** são provisórios: os nomes de verdade esperam a história do mundo ([[Estado do Projeto]]).
- **A porta** é o tile logo abaixo do meio do lote: x = canto x + largura ÷ 2; y = canto y + altura. É por ela que o NPC entra e sai.
- **12 empregados e 3 desempregados:** com os 2 da Carpintaria, os salários do dia dão cerca de 85, a conta da Arquitetura (V2-12).

### Estado (muda durante o jogo)

**A vila** (uma lista simples, sem EnTT: são poucas construções, como diz a [[Arquitetura/Descrição|Arquitetura]]):

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| Cada construção: `identificador`, `tipo`, `numeroDoLote`, `donoEhOJogador` | texto, ID, inteiro, sim/não | `casa_1`, `construcao.casa`, 2, não | Sim |
| Cada construção: `tileDaPorta` | 2 inteiros | (19, 25) | Não: sai do lote |
| `sorteioDosNpcs` | o estado do sorteio | — | Sim (marco 9) |

**Os componentes novos do NPC** (com os do marco 4):

| Componente | Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|---|
| `IdentidadeDoNpc` | `nome`, `classe` | texto, Civil | Ana, Civil | Sim |
| `CasaETrabalhoDoNpc` | `indiceDaCasa`, `indiceDoTrabalho` | posição na lista da vila, ou −1 | 1, 0 | Sim |
| `DecisaoDoNpc` | `acaoAtual` | Dormir, Trabalhar, Comer, Comprar ou Passear | Trabalhar | Sim |
| | `indiceDaConstrucaoOndeEsta` | posição na lista, ou −1 (na rua) | 0 (dentro da Fazenda) | Sim |
| | `indiceDaConstrucaoDoDestino` | posição na lista, ou −1 | 0 | Sim |
| | `estaACaminho` | sim/não | pediu um caminho e ainda não chegou (acrescentado no código do marco 5) | Sim |
| | `tickEmQueAEsperaAcaba` | tick, ou −1 | o fim da compra ou da parada do passeio | Sim |
| | `diaDaUltimaCompra` | dia da partida, ou −1 | 0 | Sim |

## 3. Quando roda

**Ao abrir o jogo:** a regra 4.1 monta a vila e cria os 15 NPCs dentro das casas; cada um já escolhe a ação (regra 4.2).

**A cada tick**, nesta ordem:

1. **Reavaliar** os NPCs da vez (regra 4.2): cada NPC reavalia a cada **67 ticks** (5 minutos de jogo), no tick em que (tick + número do NPC) resto 67 = 0. Com 15 NPCs, no máximo 1 por tick, como pede a Fatia Vertical ("só 1/67 dos NPCs reavalia em cada tick").
2. **Terminar as esperas** (regra 4.5): quem acabou de comprar ou de parar no passeio reavalia na hora.
3. **Atender os pedidos de caminho e andar** (marco 4).
4. **Chegar** (regra 4.4): quem chegou à porta entra; quem chegou ao ponto do passeio começa a esperar.

Pausado, nada disso roda.

## 4. Regras passo a passo

### 4.1 Montar a vila (ao abrir)

1. Leia o `vila_inicial.json`. Cada problema vira um erro com o motivo, como nos outros dados, e o `--validar` também confere este arquivo.
2. Para cada construção:
   - o `tipo` precisa existir no `construcoes.json`, e o `id` não pode repetir;
   - o `lote` precisa existir no mapa e estar livre (duas construções no mesmo lote é erro);
   - calcule a porta (parte 2). Erro se ela estiver fora do mapa ou bloqueada.
3. **Bloqueie os tiles do lote** na passagem do mapa (FL5-1): é a regra 4.2 da [[Ficha Logica/Mapa|ficha do Mapa]], que viria no marco 6 e entra agora, porque o NPC precisa contornar as construções.
4. Para cada NPC:
   - a `casa` precisa ser uma construção do tipo `construcao.casa`; cada Casa tem no máximo **4 moradores** (16 lugares nas 4 Casas, V2-12);
   - o `trabalho` precisa ser uma construção que não é Casa, ou vazio (desempregado). No marco 5 não há limite de vagas: ele entra com a Construção (marco 6).
5. Crie cada NPC **dentro da casa**: a `PosicaoDoNpc` na porta, `indiceDaConstrucaoOndeEsta` = a casa.
6. Ligue o sorteio dos NPCs com a `semente_da_partida` (regra 4.6).

### 4.2 Escolher a ação (a Utilidade, a cada 5 minutos de jogo)

1. **O período da rotina** pela hora atual: Dormir, Trabalhar ou Livre.
2. **As notas,** em centésimos (V2-10):

   | Ação | Nota |
   |---|---|
   | Dormir | 100 no período Dormir |
   | Trabalhar | 100 no período Trabalhar, **se tem emprego** |
   | Passear | 100 no período Livre; e no período Trabalhar, para quem não tem emprego (FL5-3) |
   | Comer | +150 da hora da refeição (6h, 12h, 19h; o café passou das 7h para as 6h na FL6-7) até 30 minutos depois |
   | Comprar | +120 no período Livre, se é o dia de compra dele e ainda não comprou hoje |

3. **Escolha a de maior nota.** No empate, vale esta ordem: Comer, Comprar, Trabalhar, Dormir, Passear (só para o resultado nunca depender da sorte).
4. **Se a ação mudou,** escolha o destino (regra 4.3). Se não mudou, o NPC continua o que fazia.

**O dia de compra** de cada NPC: o dia em que (dia da partida + número do NPC) resto 2 = 0. Assim, metade da vila compra num dia, e a outra metade no outro (uma vez a cada 2 dias, V2-10).

**O Lazer e o Fugir ficam fora do marco 5** (FL5-3): o Lazer precisa da Taverna, que não está na fatia, e o Fugir precisa de combate. A nota deles entra com cada um.

### 4.3 O destino de cada ação

| Ação | Para onde vai | O que faz lá |
|---|---|---|
| Dormir | A porta da casa | Entra e fica |
| Trabalhar | A porta do trabalho. **Marco 6:** os Braçais vão para um tile da área do recurso ([[Ficha Logica/Producao\|Produção]], regra 4.1) | Entra e fica (o Braçal fica no campo, visível) |
| Comer (FL5-4) | Se está **dentro** da casa ou do trabalho, fica lá; o Braçal parado no tile dele no campo também fica (FL6-7). Se não, vai para casa | Come até o fim da meia hora da refeição. No trabalho, a meia hora conta como trabalhada (FL6-7) |
| Comprar | A porta do Mercado | Entra e fica 30 minutos; depois anota `diaDaUltimaCompra` = hoje |
| Passear (FL5-5) | Um tile livre sorteado, a até 12 tiles de onde está (regra 4.6) | Fica de 15 a 45 minutos (sorteados) e sorteia outro |

**Sair de uma construção:** se o NPC está dentro de uma e o destino novo é outro lugar, ele reaparece na porta e anda. Se o destino é a própria construção onde está, nada muda.

**Sem caminho** (não deve acontecer na vila, mas o código cuida): o NPC vai até o tile mais perto, como no marco 4 (FL4-3), e reavalia no próximo turno dele.

### 4.4 Chegar

Quando o caminho do NPC acaba:

1. se o destino é uma construção e ele está na porta dela, ele **entra**: `indiceDaConstrucaoOndeEsta` = a construção. Para Comprar, a espera começa: `tickEmQueAEsperaAcaba` = agora + 30 minutos (400 ticks);
2. se a ação é Passear, a espera começa: agora + os minutos sorteados.

### 4.5 Terminar uma espera

Quando o tick chega a `tickEmQueAEsperaAcaba`:

- **Comprar:** anote a compra (`diaDaUltimaCompra` = hoje) e reavalie na hora (regra 4.2). O NPC sai do Mercado se a ação nova pedir outro lugar.
- **Passear:** sorteie outro tile (regra 4.6) e vá.

### 4.6 O sorteio com semente

O passeio precisa de números sorteados, e a mesma semente tem que dar a mesma partida (Arquitetura e [[Sistema de Eventos/Descrição|Eventos]]):

1. **O gerador** (FL5-5): um gerador próprio pequeno, o **PCG32** (cerca de 15 linhas), e não o do C++. *Por quê:* as funções de sorteio do C++ (`std::uniform_int_distribution`) podem dar números diferentes em outro compilador; o PCG32 dá os mesmos em qualquer lugar.
2. **Um gerador por sistema:** o dos NPCs agora; o dos eventos (marco 7) é outro, ligado à mesma semente da partida. Assim, um NPC a mais não muda os eventos sorteados.
3. **O tile do passeio:** sorteie x e y a até 12 tiles do tile atual. Aceite se dá para andar e não é a porta de uma construção. Se não, tente de novo, no máximo 20 vezes; sem sucesso, o NPC fica onde está até a próxima reavaliação.
4. **A espera:** sorteie um número inteiro de 15 a 45 minutos.

Como os NPCs reavaliam sempre na mesma ordem (pelo tick e pelo número), a ordem dos sorteios é sempre a mesma.

### 4.7 Desenhar (no `Jogo/`)

1. **As construções** (FL5-1): cada lote ocupado vira um retângulo de cor lisa, uma cor por tipo (Casa marrom, Fazenda amarela, Posto de Coleta verde, Cozinheiro laranja, Carpintaria castanha, Mercado azul, Construtor cinza), com a porta pintada mais escura. A arte de verdade das construções entra depois, com o pacote de arte provisório.
2. **Os NPCs dentro de uma construção não são desenhados.** Os outros, como no marco 4.
3. **A depuração** (FL5-6): a tecla **F3** liga um quadradinho colorido em cima da cabeça de cada NPC, com a cor da ação (Comer laranja, Comprar azul, Passear verde, Trabalhar e Dormir cinza, a caminho do trabalho ou da casa), e escreve no console uma tabela com cada NPC: nome, ação, onde está e as notas.
4. **As teclas de teste do marco 4 saem** (FL5-6): o botão direito não manda mais os NPCs (quem manda é a rotina), e o N não cria NPCs fora da vila. O F2 (os caminhos) continua.

## 5. Contas

**A reavaliação:** 5 minutos de jogo = 300 s ÷ 4,5 s por tick = 66,7 ticks, arredondado para **67**. A 1x, isso é 3,35 segundos reais. Uma mudança de período (7h, 15h, 22h) é notada, no pior caso, 5 minutos de jogo depois.

**As esperas em ticks:** 30 minutos = 1.800 s ÷ 4,5 = **400 ticks**; 15 a 45 minutos = 200 a 600 ticks.

**O dia de um Civil empregado** (Ana, da Fazenda, que mora na `casa_1`, num dia sem compra; com o café às 6h da FL6-7):

| Hora | Notas que ganham | Ação | Onde |
|---|---|---|---|
| 6h00 | Comer 150 > Passear 100 | Comer | Acordou em casa: come lá até 6h30 |
| 6h30 | Passear 100 | Passear | Sai de casa e passeia |
| 7h00 | Trabalhar 100 | Trabalhar | Anda até um tile do campo de Grão (Braçal, marco 6) |
| 12h00 | Comer 150 > Trabalhar 100 | Comer | Come no campo, e a meia hora conta como trabalhada (FL6-7) |
| 15h00 | Passear 100 | Passear | Passeia |
| 19h00 | Comer 150 > Passear 100 | Comer | Volta para casa e come até 19h30 |
| 19h30 | Passear 100 | Passear | Passeia |
| 22h00 | Dormir 100 | Dormir | Volta para casa |

No **dia de compra**, às 6h30 o Comprar (120) ganha do Passear: ela vai ao Mercado. Se a reavaliação das 7h chega antes do fim da meia hora, o Trabalhar ganha, e ela sai sem terminar; nesse caso, compra às 15h.

*O dia de trabalho:* ela chega ao campo por volta das 7h05 e fica até as 15h, com o almoço contando: cerca de **7,9 horas** (FL6-7). Com o café às 7h (FL5-4), seriam 6,9 horas, e a vila passaria fome (ficha da [[Ficha Logica/Producao|Produção]], parte 5).

**Quantos andam juntos:** às **6h30** do dia de compra (depois do café), os NPCs da vez vão ao Mercado quase juntos: no dia 0, os 8 de número par (visto no código do marco 5). Um Lojista que compra no próprio Mercado não sai: a espera começa lá dentro.

## 6. Conversa com outros sistemas

| Sistema | O que a IA lê | O que a IA entrega |
|---|---|---|
| Tempo (marco 1) | A hora, o minuto, o dia e o tick | — |
| Mapa (marcos 1 e 4) | Os lotes; a passagem | Os tiles dos lotes ocupados, bloqueados (FL5-1) |
| Dados (marco 3) | Os tipos de construção (`construcoes.json`) e o `regras.json` | — |
| NPC e movimento (marco 4) | Quem chegou | O destino de cada NPC |
| Produção (marco 6) | — | Quem está **dentro** do trabalho (os empregados presentes) |
| Consumo e Mercado (marco 6) | — | As idas para comer e comprar, que ganham o efeito de verdade |
| Felicidade (marco 7) | — | (o Lazer usa a Felicidade, depois da fatia) |
| Save (marco 9) | — | A ação, onde está, o destino, as esperas e o estado do sorteio |

## 7. Nomes no código

Em `Codigo/` (sem SDL nem OpenGL):

| Nome | O que é |
|---|---|
| `Vila/VilaInicial.cpp`: `ConstrucaoDaVila`, `VilaDoJogo`, `lerVilaInicial(texto, mapa, catalogoDeConstrucoes, problemas)` | A regra 4.1 |
| `bloquearOsLotesOcupados(mapa, vila)` | A regra 4.1, passo 3 |
| `Npc/IdentidadeDoNpc`, `Npc/CasaETrabalhoDoNpc`, `Npc/DecisaoDoNpc` | Os componentes da parte 2 |
| `AcaoDoNpc` | `Dormir`, `Trabalhar`, `Comer`, `Comprar`, `Passear`, e `Nenhuma` (ao nascer, ou quando o NPC não chegou e precisa decidir de novo) |
| `PeriodoDaRotina` e `descobrirPeriodoDaRotina(horaDoDia, rotina)` | O passo 1 da regra 4.2 |
| `Npc/RegrasDaRotina.h`: `RegrasDaRotina`, `RegrasDaUtilidade`, `RegrasDoPasseio`, juntas em `RegrasDaVidaDosNpcs` | Os números do `regras.json` |
| `NotasDasAcoes` e `calcularNotasDasAcoes(...)` | O passo 2 da regra 4.2 |
| `escolherAcaoPelaUtilidade(notas)` | O passo 3 da regra 4.2 |
| `Npc/RotinaDosNpcs.cpp`: `ContextoDaRotina` | O registro, o mapa, a vila, as regras e o sorteio, juntos |
| `criarOsNpcsDaVila(contexto, relogio)` | A regra 4.1, passos 5 e 6 |
| `rodarARotinaAntesDeAndar(contexto, relogio)` e `cuidarDasChegadas(contexto, relogio)` | As regras 4.2 a 4.5, a cada tick (antes e depois do movimento) |
| `montarTabelaDasAcoesDosNpcs(contexto, relogio)` | A tabela da tecla F3 |
| `Tempo/SorteioComSemente.h`: `SorteioComSemente`, `sortearNumeroInteiroEntre(minimo, maximo)` | O PCG32 da regra 4.6 |
| `sortearTileDoPasseio(mapa, vila, tileAtual, regras, sorteio)` | A regra 4.6, passo 3 |

Em `Jogo/`:

| Nome | O que é |
|---|---|
| `DesenhistaDoMapa::desenharConstrucoesProvisorias(vila, ...)` | A regra 4.7, passo 1 |
| `AcaoDoJogador::MostrarAcoesDosNpcsParaDepuracao` | A tecla F3 (FL5-6) |
| `DesenhistaDosPersonagens::desenharMarcadoresDasAcoes(...)` | Os quadradinhos coloridos do F3 |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Hora 23h, 3h, 6h, 7h, 14h, 15h e 22h, rotina Civil | Dormir, Dormir, Livre, Trabalhar, Trabalhar, Livre, Dormir |
| Empregado às 12h10 | Comer (150) ganha de Trabalhar (100) |
| Desempregado às 10h | Passear (100) |
| Empregado às 15h, no dia de compra, sem ter comprado | Comprar (120) |
| O mesmo, depois de comprar | Passear (100) |
| NPC de número 3 nos dias 0, 1, 2 e 3 | Dia de compra nos dias 1 e 3 |
| Empate de notas (Trabalhar 100 e Passear 100, um caso de teste) | Trabalhar, pela ordem fixa |
| `vila_inicial.json` da fatia | Sem erros; 10 construções, 15 NPCs, 12 empregados, o lote 5 livre |
| Duas construções no lote 2 | Erro |
| Um NPC morando na Fazenda | Erro: a casa precisa ser Casa |
| 5 NPCs na mesma Casa | Erro: no máximo 4 |
| Os tiles do lote 2 depois de montar a vila | Bloqueados; a porta (19, 25) livre |
| PCG32 com a semente 1, os 3 primeiros números | Sempre os mesmos (os valores ficam no teste) |
| Dois sorteios com a mesma semente | As mesmas listas |
| Um dia inteiro sem tela (19.200 ticks), semente 1 | Às 3h, todos dentro de casa; às 10h, os 12 empregados dentro do trabalho; ninguém fica parado no meio de uma construção |
| O mesmo dia duas vezes | As mesmas posições, NPC por NPC, no fim do dia |

## 9. Fora da fatia (e fora do marco 5)

- As outras classes e as Funções contínuas (os turnos dos guardas).
- O Lazer (a Taverna), o Fugir (o combate) e o Medo; a Personalidade nas notas.
- A IA em LOD 1 (Regras Fixas, sem notas) e LOD 2 (sem decisão individual).
- O efeito de comer e de comprar (marco 6): o consumo é da cidade, e a ida ao Mercado só mostra isso na tela.
- A tempestade que deixa os NPCs em casa no Período Livre ([[Sistema de Clima e Estações/Descrição|Clima]]).
- O painel do Dear ImGui com as notas de cada NPC (marco 8 ou depois); no marco 5, o console.
