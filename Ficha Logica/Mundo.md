---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase E1 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com MU1 vêm da **rodada MU1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Mundo e Estado Inicial

## 1. O que faz

Cria o **mundo do primeiro dia** a partir de uma **semente**: os 48 mapas, as 22 cidades com as construções, os NPCs, os estoques e o dinheiro, as 6 facções com as leis, os Bancos, os Esquadrões, as dinastias e as tecnologias, as relações, as guerras e os acordos iniciais, os marcos, as masmorras e os 5 Importantes fixos. A mesma semente gera o mesmo mundo.

Documento: [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]. Esta ficha **junta** o que as fichas das fases B a D já decidiram para o começo e põe tudo **numa ordem só**.

## 2. Dados

### `data/mundo/` (arquivos do documento)

| Arquivo | O que tem |
|---|---|
| `mapas.json` | Os 48 mapas: coluna, linha, região, bioma, terreno, infraestrutura, recursos com a produtividade, `arquivo_tiled`, masmorra. **Sem o estado do território** (derivado; FA1-6) |
| `cidades_iniciais.json` | Cidade, mapa, nível, facção (ou nenhuma), cultura, pacote |
| `faccoes_iniciais.json` | Riqueza, tradição militar, leis, tecnologias fixas e a lista das extras |
| `relacoes.json` | Afinidades, pares candidatos a guerra e a acordo, limites do sorteio |
| `masmorras.json` | Posição, dificuldade, tabela de Loot |
| `importantes_fixos.json` | Os 5 papéis e onde começam |
| `pontos_partida.json` | Os pontos de cada estilo de início (Início de Jogo, E2) |

### Os números do primeiro dia

| O quê | Conta | Onde estava |
|---|---|---|
| Dinheiro do Banco de cada cidade | 400 / 1.500 / 6.000 / 20.000 por nível × Riqueza | Outras cidades |
| PIB da facção | Soma do dinheiro das cidades dela | Facção |
| Banco da Facção | 25% do PIB | Facção |
| Esquadrões | Capital 8 + cidade 4 + vilarejo 1 (cada) × tradição (Gue 1,5; Aut 1,25; Esc e Rel 1; Tec 0,85; Arc 0,75), arredondado | Mundo |
| NPCs reais | 15 / 40 / 100 / 300 por nível | Outras cidades |
| **Produtividade dos recursos (MU1-1)** | Destaque 150%, normal 100%, escasso 50% | Nova |

**O sorteio do mundo:** uma sequência nova, a **8** ("mundo"), para os preços iniciais (±10%), as tecnologias extras e o Loot das masmorras. As outras usam as que as fichas já deram (parte 4).

## 3. Quando roda

Uma vez, no **Novo Jogo**, antes do primeiro tick (o dia 1 do Ano 412, Primavera, 6 h). Depois, só os resultados vão no save (com a semente).

## 4. Regras passo a passo (a ordem da criação)

1. **Ler e conferir os dados.** Erro e parar se: uma cidade num mapa que não existe; 2 cidades no mesmo mapa; uma região sem 8 mapas; um PIB fora de ±30% da média; um item, receita ou construção citado que não existe.
2. **Os mapas e o território:** o estado de cada mapa sai das cidades vizinhas ([[Ficha Logica/Faccao|Facção]], 4.1).
3. **As cidades** (pela ordem do número do mapa; a vila do jogador vem da `vila_inicial` e do Início de Jogo):
   1. As construções: o pacote do nível e da cultura, os Templos que bastam (RL1-1), o Laboratório das capitais (TE1-1), a Senzala e o Mercado de Escravos da Escravista, e as **de produção dimensionadas** (MU1-2, abaixo).
   2. Os NPCs: a quantidade do nível, cada um com idade (5 a 19), cultura da cidade, habilidades (faixa da classe + os pontos da cultura), Mago (15% na Arcana, 5% nas outras; MG1-1), e na Escravista 30% Escravos (ES1-6). Sequência: a de NPCs da cidade (100 + mapa × 10 + 1).
   3. As casas e as Senzalas; a contratação; os estoques (5 dias do consumo previsto); os preços locais = preço base × (90 a 110%) (sequência 8); o Banco pelo nível.
4. **As facções:** as leis da cultura (sem o extremo); o Banco da Facção (25% do PIB); os Esquadrões, como guarnição na capital (Infantaria Leve, kit Rank 1, nesta versão; MU1-3); a dinastia (líder de 8 a 18 anos, CP1-5) e os 2 a 3 nobres; as tecnologias fixas + 2 ou 3 extras (sequência 8).
5. **As relações, as guerras e os acordos** (Diplomacia, 4.1; sequência 7), com a Não Agressão inicial (DP1-1).
6. **Os marcos mundiais:** o dia de cada um (Tecnologia, 4.4; sequência 6).
7. **As masmorras:** o Loot e os inimigos pela dificuldade (sequência 8; Objetivo e Progressão).
8. **Os 5 Importantes fixos** (Arquimago, Rei Exilado, Grande Mercador, Sumo Sacerdote, Mestre Inventor), nas cidades deles, como NPCs Importantes com Personalidade.
9. **O jogador** (Início de Jogo, E2).
10. **O clima do dia 1** de cada região (Viagem, 4.8).

### MU1-2: as construções de produção pelos recursos

O gerador das Outras cidades (OC1-4 e CO1-1) repetia o conjunto da vila a cada 15 NPCs e tirava as Braçais dos recursos que o mapa não tem. Agora:

1. Cada Braçal da comida (Fazenda de Grão e as outras) entra com a quantidade da vila × **100 ÷ produtividade** do recurso no mapa, arredondada para cima: escasso (50%) = o **dobro** de Fazendas; destaque (150%) = dois terços.
2. As outras Braçais (Tora, Pedra, Minério): só nos mapas que têm o recurso, com a quantidade da vila (sem compensar a produtividade: a sobra ou a falta delas é o que o Comércio leva).
3. Água e Grão existem em todo mapa com cidade (CO1-1), mesmo escassos.

## 5. Contas

| Facção | Cidades (nível) | Dinheiro | PIB | Esquadrões | NPCs | Magos | Escravos |
|---|---|---|---|---|---|---|---|
| Guerreira | 4, 3, 2 | 27.500 × 0,85 | **23.375** | 13 × 1,5 = **20** | 440 | 22 | 0 |
| Arcana | 4, 3, 2 | × 0,95 | 26.125 | 13 × 0,75 = **10** | 440 | **66** | 0 |
| Autoritária | 4, 3, 2 | × 1,15 | 31.625 | × 1,25 = **16** | 440 | 22 | 0 |
| Religiosa | 4, 3, 2 | × 1,00 | 27.500 | **13** | 440 | 22 | 0 |
| Tecnocrata | 4, 3, 2 | × 1,05 | 28.875 | × 0,85 = **11** | 440 | 22 | 0 |
| Escravista | 4, 3, 2, 2 | 29.000 × 1,20 | **34.800** | 14 × 1 = **14** | 480 | 24 | **144** |

Todos os números batem com a tabela de equilíbrio do documento (PIB, Esquadrões, os cerca de 66 Magos da Arcana). Total do mundo: **2.750 NPCs** nas 22 cidades + os 15 da vila do jogador.

## 6. Conversa com outros sistemas

Esta ficha **chama** as regras de criação das outras: Outras cidades (4.1), Facção, Cultura, Religião, Tecnologia, Diplomacia, Companheiros (dinastias), Escravidão, Magia, Viagem (clima). O que muda nelas:

- **Outras cidades:** a MU1-2 (as Fazendas pela produtividade).
- **Recursos:** a produtividade de destaque e de escassez (150% e 50%).
- **Save:** a semente do mundo e os resultados dos sorteios; o formato passa para a **versão 21**.

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `criarMundo(semente)` | Os 10 passos da parte 4, nesta ordem |
| `conferirDadosDoMundo()` | Passo 1 |
| `dimensionarConstrucoesDeProducao(cidade)` | MU1-2 |
| `sortearPrecosIniciais(cidade)` | Passo 3.3 (sequência 8) |

## 8. Testes

1. **Mesma semente, mesmo mundo:** dois mundos com a semente 1 são iguais byte a byte no save.
2. **O território** dos 48 mapas bate com o mapa inicial do documento (parte 10.2).
3. **A tabela da parte 5.**
4. **Comida no primeiro ano:** nenhuma das 22 cidades passa fome nos primeiros 64 dias sem comércio nem guerra, nas 10 sementes.
5. **Validação:** um `cidades_iniciais.json` com 2 cidades no mesmo mapa dá erro.
6. **O teste de 1 ano** (a vila sozinha, `--simular`) não muda: ele não cria o mundo.

## 9. Fora desta versão

- A história, os nomes e os personagens (quando a história for escrita).
- As listas de tecnologias e o Loot (rodadas de conteúdo).
- A Semente do Narrador dos Eventos (quando o Narrador tiver ficha).
- Os tipos de Esquadrão do primeiro dia por cultura (nesta versão, todos Infantaria Leve; MU1-3).

## 10. Simulação de mesa

### 10.1 Cenário

A semente 1, o mapa inicial do documento e as regras de território da Facção.

### 10.2 O território, mapa a mapa

Conferi os 26 mapas sem cidade pela regra da Facção (4.1: as cidades vizinhas de facção, menos as Vilas Livres):

| Resultado | Mapas |
|---|---|
| Território, bate | (2,1), (3,1), (6,1), (7,1), (4,2), (5,2), (8,2), (8,3), (2,4), (4,4), (1,5), (2,5), (3,5), (1,6), (4,6), (7,6) |
| Em disputa, bate | (1,2) Gue/Aut, (3,3) Gue/Aut, (6,3) Arc/Rel, (6,4) Rel/Esc, (5,5) Tec/Esc, (8,5) Rel/Esc |
| Sem dono, bate | (5,1), (5,3), (3,4), (5,4) |

**Os 26 batem** com o mapa do documento (16 território, 6 em disputa, 4 sem dono, como na tabela de contagem).

### 10.3 A comida da Guerreira

A região Guerreira tem o Grão **escasso**. A vila alimenta 15 NPCs com as Fazendas dela (produtividade 100%); chame esse conjunto de "1 conjunto".

| | Capital Guerreira (300 NPCs) | Conjuntos de Fazendas | Grão produzido (vs. o necessário) |
|---|---|---|---|
| Com OC1-4 (copiar a vila) | 300 ÷ 15 = 20 | 20 | **50%**: fome desde a primeira semana |
| Com MU1-2 | 20 × 100 ÷ 50 = 40 | 40 | **100%** |

**Achado:** o documento quer a Guerreira "com pouca comida, que precisa comprar grão ou atacar o celeiro". Pela regra das Outras cidades, ela teria **metade** da comida e morreria de fome antes do primeiro comércio. **Decidido (MU1-2):** ela começa sustentável, mas com o dobro de Fazendas, gastando trabalhadores que as outras usam em outra coisa. O incentivo para a guerra fica mais fraco. **Para o autor conferir:** se quiser a Guerreira dependente de verdade, a sugestão alternativa é dimensionar as Fazendas para 80% e abrir uma **rota de grão inicial** da Escravista (um acordo comercial já sorteado entre as duas).

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| O território do primeiro dia bate? | **Sim:** os 26 mapas (10.2) |
| Os Esquadrões e o PIB batem? | **Sim:** as fórmulas dão os números da tabela do documento |
| As cidades passam fome no começo? | **Não, com a MU1-2** (10.3); o teste 4 confere |
| A mesma semente gera o mesmo mundo? | **Sim:** cada sorteio tem a sua sequência (1 a 8 e as das cidades), e a ordem dos passos é fixa |
| O PIB passa dos ±30%? | **Não:** a Escravista fica a +21% da média e a Guerreira a −19% (o documento) |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Um mapa sem `arquivo_tiled` com cidade | A cidade existe em LOD 2 para sempre (VI1-8) |
| Um recurso escasso de comida em todo o mapa e nenhum outro | O dobro de Fazendas; se a vaga não couber nos lotes (quando houver mapa), fica no estoque de vagas [Refinar depois] |
| Uma facção com 2 vilarejos (Escravista) | Esquadrões: 8 + 4 + 1 + 1 = 14 |
| O ponto de partida do jogador é um mapa com cidade | A vila do jogador não é criada; ele começa na cidade (Início de Jogo) |

### 10.6 Decidido em modo automático (rodada MU1, seguir sugestão)

1. **MU1-1:** a produtividade dos recursos: destaque 150%, normal 100%, escasso 50%.
2. **MU1-2:** as Fazendas e as outras Braçais de comida de cada cidade são dimensionadas pela produtividade (100 ÷ produtividade), para nenhuma cidade começar com fome. **Para o autor conferir** (10.3).
3. **MU1-3:** os Esquadrões do primeiro dia são Infantaria Leve, kit Rank 1, de guarnição na capital.
4. **MU1-4:** a sequência 8 ("mundo") para os preços iniciais, as tecnologias extras e o Loot.
5. **MU1-5:** a ordem da criação é a da parte 4, sempre a mesma.
