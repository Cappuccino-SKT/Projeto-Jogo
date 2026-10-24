---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase B3 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com NO1 vêm da **rodada NO1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Notícias

## 1. O que faz

Cada acontecimento importante do mundo vira uma **notícia**, que nasce num mapa e numa hora e chega aos outros mapas **3 horas por mapa de distância**. O jogador fica sabendo quando a notícia chega ao mapa onde ele está ou a uma caravana dele. A notícia perde detalhe com a distância. É o jeito de mostrar as 22 cidades vivendo sem tirar a névoa.

Documento do sistema: [[Sistema de Notícias/Descrição|Notícias e Informação]].

**Esta versão:** as notícias dos eventos, das cidades, do Comércio e da Viagem; a chegada; a precisão; as notícias de preço; a notificação, o quadro da Taverna, o Mapa Mundial e a Crônica do mundo. **Conversar, os companheiros na rede, a guerra e a diplomacia** ficam na parte 9.

## 2. Dados

### Definições: `data/noticias.json` (arquivo novo)

```json
{
  "horas_por_mapa": 3,
  "importancias": {
    "local":    { "alcance_em_mapas": 2,  "dias_no_quadro": 8 },
    "regional": { "alcance_em_mapas": 5,  "dias_no_quadro": 16 },
    "mundial":  { "alcance_em_mapas": 99, "dias_no_quadro": 64 }
  },
  "precisao_exata_ate_mapas": 2,
  "precisao_arredondada_ate_mapas": 5,
  "preco_fora_da_media_em_porcento": 30,
  "preco_mesma_noticia_depois_de_dias": 8,
  "preco_noticias_por_cidade_por_dia": 1,
  "limite_de_noticias_ativas": 500
}
```

### Cada evento ganha a importância da notícia

Uma coluna nova nos eventos (ficha dos [[Ficha Logica/Eventos|Eventos]]) e nos outros acontecimentos:

| De onde | Acontecimento | Importância |
|---|---|---|
| Eventos | Seca, Fome, Epidemia | Regional |
| Eventos | Geada precoce, Colheita farta, Festa da Colheita | Local |
| Eventos | Chuva boa, Ratos, Achado, Mercador viajante, Febre, Falta de móveis | Nenhuma (fica só no diário da cidade) |
| Clima | Inverno rigoroso | Mundial |
| Cidade | Subiu ou caiu de nível | Regional |
| Comércio | Caravana atacada | Local |
| Viagem | Bandidos atacaram alguém na estrada | Local |
| Comércio | Preço fora da média (4.6) | Local |

O texto vem do modelo de frase do próprio acontecimento, com o "Por quê?" e os números (4.4).

### Cada notícia (no save)

| Campo | Exemplo |
|---|---|
| `numeroDaNoticia` | 1.207 |
| `tipo`, `importancia` | `"evento.epidemia"`, regional |
| `mapaDeOrigem`, `tickDeOrigem` | 23 (7,3), dia 5 0 h |
| `numeros` | doentes: 1.880 |
| `porQue` | "a cidade está cheia e sem Boticário" |
| `diaEmQueSaiDoQuadro` | dia 21 |

A hora de chegada a cada mapa **não é guardada**: é calculada quando precisa (4.2).

### Do jogador (no save)

- `noticiasQueOJogadorSabe`: número da notícia, dia em que soube, de que distância soube (a melhor até agora).
- `cronicaDoMundo`: as Regionais e Mundiais que ele soube, para sempre (cerca de 100 bytes cada).

## 3. Quando roda

| Quando | O quê |
|---|---|
| Quando um acontecimento da tabela acontece | Cria a notícia (4.1) |
| **A cada hora** | Confere o que chegou ao jogador (4.3) e tira as que passaram do prazo (4.7) |
| Ao **entrar num mapa** | O jogador recebe o que já está ali (4.3) |
| **A cada dia, passo 3c (novo)**, depois do passo 3b do Comércio | As notícias de preço (4.6) |

Sem sorteio: as Notícias não usam o gerador.

## 4. Regras passo a passo

### 4.1 Nascer

1. Cria o registro com a origem (o mapa e o tick do acontecimento), os números e o "Por quê?".
2. Se a lista passar de 500, sai a **Local mais velha**; se não houver Local, a Regional mais velha (a Crônica do jogador não perde nada).

### 4.2 Quando chega a um mapa

1. **Distância** = o menor número de passos entre os dois mapas na malha (os 4 vizinhos). Sem mapas bloqueados nesta versão, é |coluna − coluna| + |linha − linha|.
2. Se a distância passa do **alcance** da importância, a notícia **nunca chega** àquele mapa.
3. **Chegada** = tick de origem + distância × 3 horas × 800 ticks.

### 4.3 O jogador fica sabendo

A cada hora, e ao entrar num mapa, para cada notícia ativa que o jogador ainda não sabe:

1. **Pelo mapa dele:** o mapa onde o jogador está (em viagem, o `mapaAtual` da Viagem). Se a notícia já chegou ali, ele sabe, com a distância da origem a esse mapa.
2. **Pela rede dele:** cada caravana do jogador ([[Ficha Logica/Comercio|Comércio]], 4.9), no `mapaAtual` dela. Se a notícia chegou ali, ele sabe com a distância da origem a esse mapa.
3. Vale a **menor** das distâncias. Se ele já sabia, mas agora soube de mais perto, a notícia **ganha a versão mais precisa**, sem aviso novo.
4. Ao entrar num mapa, as notícias que já estavam ali (no prazo) chegam **juntas**, num aviso só: "3 notícias novas em Vila Livre do Centro" (NO1-3).

### 4.4 A precisão

Pela distância com que o jogador soube:

| Distância | Os números | Exemplo (1.880 doentes) |
|---|---|---|
| 0 a 2 | Exatos | "1.880 doentes" |
| 3 a 5 | **1 algarismo**, arredondado (a metade sobe) | "cerca de 2.000 doentes" |
| 6 ou mais | **Uma palavra** pelo tamanho: menos de 10 "alguns"; 10 a 99 "dezenas de"; 100 a 999 "centenas de"; 1.000 ou mais "milhares de" | "milhares de doentes" |

Os números da notícia são **pessoas** (NPCs reais × Fator de Escala), não NPCs reais.

### 4.5 Onde o jogador vê

| Onde | O que mostra |
|---|---|
| **Notificação** | Por padrão, só as **Regionais e Mundiais**. As Locais vão para o quadro e para o Mapa Mundial, sem aviso; o jogador pode ligar (Interface) (NO1-2) |
| **Quadro da Taverna** | Na cidade com Taverna: as notícias que já chegaram àquele mapa e ainda estão no prazo, mesmo as que o jogador não sabia (ao abrir, ele passa a saber) |
| **Mapa Mundial** | A informação do mapa de origem ganha "soube há N dias" |
| **Crônica do mundo** | Uma aba com a data do acontecimento e a data em que o jogador soube |

### 4.6 As notícias de preço (passo 3c)

Para cada cidade, em ordem de mapa:

1. **A média** de um item = a média simples dos preços locais desse item nas cidades **da mesma região** (as 6 regiões fazem o papel das facções nesta versão).
2. Os itens com o preço local **30% ou mais** acima ou abaixo da média, e que não viraram notícia nessa cidade nos **últimos 8 dias**, são candidatos.
3. Vira notícia **só o candidato de maior diferença** (1 por cidade por dia; NO1-1). Texto: "Tábua cara em D2: 3,0 (+69% da região)".
4. Ao chegar ao jogador, além de virar notícia, ela atualiza o `precosConhecidos` daquele item naquela cidade, com a **data da origem**.

### 4.7 Sair

1. A notícia sai da lista no `diaEmQueSaiDoQuadro` (origem + dias no quadro).
2. A Crônica do mundo guarda as Regionais e Mundiais que o jogador soube, para sempre.

## 5. Contas

| Conta | Valor |
|---|---|
| Maior distância na malha 8 × 6 | (1,1) a (8,6): 7 + 5 = **12 mapas = 36 horas** |
| Leste a oeste | 7 passos = **21 horas** (o documento dizia "8 mapas, 1 dia": são 8 mapas contando o de origem) |
| Regional | até 5 mapas = 15 horas; fica 16 dias: sempre chega antes de sair |
| Notícias de preço | no máximo 22 por dia × 8 dias = **176 ativas** (sem o limite de 1 por cidade, com os cerca de 180 itens do jogo completo, seriam até 495 por dia) |
| Eventos que viram notícia | cerca de 4 por cidade por ano × 22 = cerca de 90 por ano |
| Conferência por hora | até 500 notícias × (1 + caravanas do jogador) contas de distância: quase nada |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Eventos, Clima, Cidade, Cura | Os acontecimentos e a importância de cada um |
| Comércio | As notícias de preço; caravana atacada; as caravanas do jogador são a rede |
| Viagem | O `mapaAtual` do jogador; a névoa com a data |
| Outras cidades | Os eventos das 22 cidades viram notícias |
| Interface | A notificação (categoria "Notícias"), o quadro da Taverna, a aba Crônica do mundo |
| Save | As notícias ativas, o que o jogador sabe, a Crônica; o formato passa para a **versão 7** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Noticia` | O registro da parte 2 |
| `criarNoticia(tipo, mapa, numeros, porQue)` | Regra 4.1 |
| `distanciaEmMapas(mapaA, mapaB)` | Regra 4.2 |
| `tickDeChegada(noticia, mapa)` | Regra 4.2; "nunca" se passar do alcance |
| `conferirNoticiasDoJogador()` | Regra 4.3, a cada hora e ao entrar num mapa |
| `textoComPrecisao(noticia, distancia)` | Regra 4.4 |
| `criarNoticiasDePreco()` | Regra 4.6, passo 3c |

## 8. Testes

1. **O exemplo do documento:** guerra em (1,1) às 10 h; jogador em (7,3) sabe às 10 h do dia seguinte; uma caravana dele em (4,1) faz ele saber às 19 h do mesmo dia.
2. **Alcance:** uma Regional em (7,3) nunca chega a (1,4) (7 mapas).
3. **Precisão:** 1.880 vira "1.880", "cerca de 2.000" e "milhares de" nas distâncias 2, 4 e 7; 1.450 vira "cerca de 1.000"; 1.500 vira "cerca de 2.000".
4. **Melhora sem aviso:** soube a 5 mapas; depois entra num mapa a 1 da origem: o texto fica exato e não há notificação nova.
5. **Preço:** uma cidade com Tábua a +70% e Grão a +40% cria só a notícia da Tábua; no dia seguinte, só a do Grão (a Tábua espera 8 dias).
6. **Limite:** com 500 ativas, a nova tira a Local mais velha.
7. **O teste de 1 ano não muda:** as Notícias não sorteiam nem mexem em estoque ou dinheiro.

## 9. Fora desta versão

- "Conversar" com NPCs e os companheiros fora do grupo na rede (Companheiros e Diálogo, fase D).
- As cidades do jogador na rede (quando ele governar uma cidade).
- Guerra, batalha, cerco, paz, tratados, morte de líder, Anátema, marcos de tecnologia (os sistemas que ainda não têm ficha).
- Boatos falsos; jornal impresso; a IA que espera as notícias.
- Notícias de preço com a média da **facção** (até a Facção, é a média da região).

## 10. Simulação de mesa

### 10.1 Cenário

- O jogador está na vila, em (5,3). Uma caravana dele está em (2,3).
- No dia 5, à 0 h (passo 6 do dia, eventos), começa uma **Epidemia** na Cidade Religiosa (7,3): 20% de 9.400 pessoas = **1.880 doentes**. Importância Regional (alcance 5, 16 dias).
- Vila Livre do Centro (4,3) tem Taverna? Não (nível 1). A Capital Autoritária fica em (1,4).

### 10.2 A notícia andando

| Mapa | Distância de (7,3) | Chega | Quem fica sabendo | Texto |
|---|---|---|---|---|
| (7,3), a origem | 0 | D5 0 h | — | — |
| (5,3), a vila | 2 | **D5 6 h** | O jogador, notificação | "Epidemia na Cidade Religiosa: 1.880 doentes" |
| (4,3) | 3 | D5 9 h | — | — |
| (2,3), a caravana | 5 | D5 15 h | Já sabia (a 2 mapas); a de 5 é pior: nada muda | — |
| (1,4) | 7 | **Nunca** (passa do alcance 5) | — | — |

Se o jogador estivesse viajando em (3,3) às 6 h, saberia às 12 h, com distância 4: "cerca de 2.000 doentes".

### 10.3 O jogador anda para perto

O jogador estava em (2,3) (distância 5) e soube às 15 h: "cerca de 2.000 doentes". No dia 6, ele chega a (6,3) (distância 1): a notícia fica "1.880 doentes" na Crônica, **sem aviso novo**.

### 10.4 As notícias de preço num dia

Uma região com 4 cidades. Preços da Tábua: 0,8 (C), 3,0 (D2), 1,5 e 1,8. Média = 7,1 ÷ 4 = **1,775**.

| Cidade | Tábua | Diferença | Grão (média 0,5) | Diferença | Vira notícia |
|---|---|---|---|---|---|
| C | 0,8 | **−55%** | 0,6 | +20% | Tábua barata (−55%) |
| D2 | 3,0 | **+69%** | 0,4 | −20% | Tábua cara (+69%) |
| as outras | 1,5 e 1,8 | −15% e +1% | 0,5 | 0 | nada |

O jogador em (5,3) a 2 mapas de D2 recebe "Tábua cara em D2" (Local, sem notificação; vai para o quadro e o Mapa Mundial). O `precosConhecidos` de D2 ganha a Tábua a 3,0 com a data de hoje. É a rota de comércio do vilarejo (Comércio, 10.4), que o jogador agora vê sem viajar.

### 10.5 Verificações

| Pergunta | Resultado |
|---|---|
| O exemplo do documento bate? | **OK:** (1,1) a (7,3) = 6 + 2 = 8 mapas = 24 h; (1,1) a (4,1) = 3 mapas = 9 h → 19 h |
| Uma notícia pode sair antes de chegar? | **Não:** a mais longe chega em 36 h, e o prazo mais curto é de 8 dias |
| As notícias enchem o quadro? | **Não com o limite:** no máximo 22 de preço por dia (NO1-1). **Achado:** sem o limite, com os 180 itens do jogo completo, seriam até 495 por dia, e a lista de 500 só teria preços |
| As notificações viram spam? | **Não:** as Locais não avisam por padrão (NO1-2). **Achado:** a 2 mapas de 4 a 6 cidades, o jogador receberia cerca de 5 avisos de preço por dia |
| O teste de 1 ano muda? | **Não:** as Notícias só leem; não sorteiam nem mudam o jogo |

### 10.6 Casos extremos

| Caso | O que acontece |
|---|---|
| A notícia nasce no mapa do jogador | Distância 0: ele sabe na mesma hora, exato |
| O jogador entra num mapa onde 12 notícias já chegaram | Um aviso só (as Regionais e Mundiais contadas): "12 notícias novas em X" (NO1-3) |
| Duas Epidemias seguidas na mesma cidade | Duas notícias; cada uma com a sua origem |
| A notícia de preço não é mais verdade quando chega | Chega com a data da origem; o jogador vê "visto há 1 dia" e decide. A notícia não muda depois de nascer |
| O jogo carrega um save no meio do caminho de uma notícia | A chegada é recalculada a partir da origem: igual |
| A caravana do jogador é destruída | Sai da rede; o que ela já trouxe fica |

### 10.7 Decidido em modo automático (rodada NO1, seguir sugestão)

1. **NO1-1:** só 1 notícia de preço por cidade por dia, a de maior diferença; o mesmo item na mesma cidade espera 8 dias.
2. **NO1-2:** a notificação, por padrão, só para as Regionais e Mundiais; as Locais vão para o quadro e o Mapa Mundial.
3. **NO1-3:** ao entrar num mapa, as notícias que já estavam lá chegam juntas, num aviso só.
4. **NO1-4:** a média do preço é a da **região** até a Facção existir.
5. **NO1-5:** a importância de cada acontecimento é a da tabela da parte 2; os eventos pequenos (Febre, Ratos, Mercador viajante) não viram notícia.
6. **NO1-6:** a distância é contada na malha (passos), e a chegada não é guardada: é calculada pela origem.

### 10.8 Ajustes do código (marco B3, 2026-10-24)

1. **A rede do jogador** é só o mapa do grupo: as caravanas do jogador ainda não existem (Comércio, 10.8).
2. **O quadro da Taverna** fica para quando houver Taverna. No lugar dele, o botão **Notícias** da barra de cima abre uma janela com as notícias que o jogador sabe (todas, também as Locais) e a aba **Crônica do mundo**.
3. **Subiu ou caiu de nível** fica para o marco A5 (as cidades ainda não mudam de nível).
4. **Caravana atacada e bandidos na estrada** viram uma notícia só: "Bandidos atacaram a caravana de X em (c,l)", com "perdeu N% da carga" ou "os guardas venceram". O ataque ao grupo do jogador não vira notícia (ele estava lá).
5. **O Inverno rigoroso** é sorteado em cada cidade: vira uma notícia Mundial em cada cidade onde acontece.
6. **Os eventos da própria vila** entram na lista e na Crônica, mas não avisam de novo (o console dos eventos já avisou).
7. **Os números:** a Epidemia leva os doentes da cidade e a Fome a população, em pessoas; os outros eventos vão sem número. O "Por quê?" só aparece nas notícias de preço e de ataque.
8. **A notícia de preço** ("Tábua em X: 3,0 (+69% da média da região)") só troca o preço daquele item no `precosConhecidos` se a cidade já foi vista e a visita é mais velha que a notícia; a data da visita não muda.

