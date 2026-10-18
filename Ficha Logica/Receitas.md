---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 3** (carregar os dados e calcular os preços base). As decisões da rodada **FL3** (2026-10-18, todas "seguir sugestão") já estão aplicadas.

# Ficha Lógica: Receitas e preço base

## 1. O que faz

Lê as receitas do `data/receitas.csv`, confere cada uma e calcula o **preço base** de todos os itens, seguindo a cadeia: primeiro os brutos, depois o que usa os brutos, e assim por diante. Se a cadeia tiver um laço (A precisa de B, e B precisa de A), o `jogo --validar` avisa.

Documentos do sistema: [[Sistema de Produção/Receitas|Receitas]], a receita padrão do [[Sistema de Produção/Descrição|Produção]] e o preço base do [[Sistema Econômico/Descrição|Econômico]] (V2-4). Mapa geral: [[Ficha Logica/Fatia Vertical|Fatia Vertical]], regra 4.1.

**No marco 3, nada é produzido:** a ficha só lê, confere e calcula os preços. O ciclo de produção (as horas, os insumos saindo do estoque) é da Produção, no marco 6.

**Na fatia: 6 receitas**, uma por item (IP1-1): as 3 Braçais (Grão, Água, Tora), o Pão, a Tábua e os Móveis de Madeira.

## 2. Dados

### O arquivo `data/receitas.csv` (FL3-2)

As colunas são as da receita padrão (IP1-1), no formato que a Fatia Vertical já mostrava, com o Trabalho em letras minúsculas, como os outros IDs:

```
id;produto;lote;insumos;construcao;nivel_min;trabalho;hab_min;horas;era
receita.grao;item.grao;2;;construcao.fazenda;1;bracal;10;1;1
receita.agua;item.agua;2;;construcao.posto_de_coleta;1;bracal;10;1;1
receita.tora;item.tora;2;;construcao.posto_de_coleta;1;bracal;10;1;1
receita.pao;item.pao;4;item.grao:2|item.agua:1;construcao.cozinheiro;1;manufaturado;10;4;1
receita.tabua;item.tabua;4;item.tora:2;construcao.carpinteiro;1;manufaturado;10;4;1
receita.moveis_madeira;item.moveis_madeira;2;item.tabua:3;construcao.carpinteiro;2;manufaturado;25;4;1
```

- **Insumos:** `item:quantidade`, separados por `|`. Vazio = sem insumo (as Braçais).
- **As Braçais em horas inteiras (FL3-2):** a regra diz "1 unidade a cada 0,5 h" no Tier 0. Em vez de horas com vírgula, a receita diz **2 unidades em 1 hora**. É a mesma velocidade, e o ciclo fecha a cada hora, junto com a "hora nova" do relógio. No Tier 1 (Fruta, Minério de Ouro...) será 1 unidade em 1 hora; no Tier 2 (Especiarias), 1 em 2 horas.

### As regras do preço, no `data/regras.json` (FL3-3)

```json
"salario_por_hora_em_centesimos": { "bracal": 75, "manufaturado": 100, "especialista": 150 },
"margem_do_dono_em_porcento": 120
```

**Por que centésimos:** o salário Braçal é 0,75 moeda por hora (V2-3), que são 7,5 décimos: não cabe no dinheiro inteiro em décimos (V2-1). Em centésimos, ele é 75, inteiro. Os centésimos só aparecem **nestas regras e dentro da conta**; o preço e o dinheiro do jogo continuam em décimos. O salário que se paga por dia (6, 8 e 12 moedas, V2-3) é inteiro em décimos.

### Definições (não mudam durante o jogo)

| Campo | Tipo | Exemplo (Pão) | Regra |
|---|---|---|---|
| `identificadorDeTexto` | texto | `receita.pao` | Começa com `receita.`; único |
| `numeroDoProduto` | número de item | o de `item.pao` | Existe; **uma receita por produto** (IP1-1) |
| `loteProduzido` | inteiro | 4 | 1 ou mais |
| `insumos` | lista de (item, quantidade) | (Grão, 2), (Água, 1) | Até 3 tipos (IP3-1); cada item existe, uma vez só, e não é o próprio produto; quantidade 1 ou mais |
| `construcaoQueProduz` | ID de construção | `construcao.cozinheiro` | Existe |
| `nivelMinimoDaConstrucao` | inteiro | 1 | De 1 a 5 |
| `trabalhoUsado` | lista fechada | `manufaturado` | `bracal`, `manufaturado` ou `especialista` |
| `habilidadeMinima` | inteiro | 10 | De 0 a 100 |
| `horasPorCiclo` | inteiro | 4 | 1 ou mais |
| `eraMinima` | inteiro | 1 | De 1 a 4 |

O resultado da regra 4.2 fica no item: `precoBaseEmDecimos` ([[Ficha Logica/Itens|ficha dos Itens]]).

### Estado

Nenhum. A receita atual e o progresso de cada construção são da Produção (marco 6).

## 3. Quando roda

**Ao abrir o jogo** (e no `jogo --validar`), depois dos itens, das construções e dos recursos (ordem na [[Ficha Logica/Itens|ficha dos Itens]], parte 3):

1. a regra 4.1 lê e confere as receitas;
2. a regra 4.2 calcula os preços base;
3. a regra 4.3 confere a escada de níveis e dá **avisos** (não erros).

Os preços são calculados **uma vez**. Durante o jogo, quem muda é o preço local (Mercado Local, marco 7), que parte do preço base.

## 4. Regras passo a passo

### 4.1 Ler o `receitas.csv`

1. Leia o arquivo e o cabeçalho, como nos itens. As 10 colunas são obrigatórias.
2. Para cada linha, confira cada campo pela tabela da parte 2. Cada problema vira um erro com o arquivo, a linha e o motivo, por exemplo:
   - "receitas.csv, linha 5: o insumo item.farinha não existe";
   - "receitas.csv, linha 5: a receita tem 4 tipos de insumo; o máximo é 3";
   - "receitas.csv, linha 7: item.pao já tem a receita receita.pao (linha 5)".
3. Os insumos são lidos assim: separe por `|`; em cada pedaço, separe por `:`; à esquerda o ID do item, à direita a quantidade inteira.

### 4.2 Calcular os preços base

1. Marque todos os itens como "sem preço".
2. Repita em rodadas:
   1. para cada receita cujo produto ainda não tem preço, e cujos insumos **já têm** todos (as Braçais não têm insumo, então entram logo na primeira rodada), calcule o preço do produto (parte 5) e guarde-o no item;
   2. se numa rodada inteira nenhum item ganhou preço, pare.
3. Sobrou item sem preço?
   - se ele **não tem receita**: erro "item.x não tem receita, então não tem preço";
   - se ele tem receita, mas um insumo nunca ganhou preço: é um **laço** na cadeia. Erro "não foi possível calcular o preço de item.a e item.b: a cadeia tem um laço".

**Por que em rodadas, e não numa ordem fixa:** a ordem das linhas no arquivo não importa. O autor pode escrever os Móveis antes da Tábua, e o jogo acha a ordem sozinho. Com cerca de 180 itens e cadeias de até 4 passos, são no máximo 5 rodadas: nada pesa.

### 4.3 Conferir a escada de níveis (avisos)

Para cada receita, compare com as regras da IP3-1 e avise se não bater (pode ser uma exceção de propósito, como a Destilaria e a Forja Arcana):

- **nível mínimo pelo Tier** do produto, nos bens: Tier 0 e 1 no nível 1, Tier 2 no 2, Tier 3 no 4 e Tier 4 no 5;
- **habilidade mínima pelo nível:** 10, 25, 40, 55 e 70 nos níveis 1 a 5 (IP1-1);
- **lote e horas pelo Tier**, nos bens de Tier 1 a 4: 4 em 4 h, 2 em 4 h, 1 em 4 h, 1 em 8 h.

*Exemplo de aviso:* "receita.moveis_madeira: nível mínimo 1, mas o Tier 2 pede nível 2".

## 5. Contas

**A fórmula** (V2-4):

> preço base = (custo dos insumos + horas × salário por hora) ÷ lote × 1,2

**Em inteiros (FL3-3):**

1. Custo em centésimos = soma de (quantidade × preço do insumo em décimos × 10) + horas × salário por hora em centésimos.
2. Preço em décimos = custo em centésimos × margem (120) × multiplicador do item (100) ÷ (lote × 100 × 100 × 10).
3. **Arredondamento: a metade sobe** (16,5 vira 17; 4,5 vira 5). Em inteiros: (2 × numerador + denominador) ÷ (2 × denominador).
4. **Cada item é arredondado antes de entrar no seguinte:** o Pão usa o Grão a 5 décimos, não a 4,5. É o que a Fatia Vertical já fazia no exemplo do Pão.
5. Se o resultado der 0, o preço vira 1 décimo: nada é de graça.

**Os 8 itens da fatia** (a Pedra e o Bloco de Pedra desde a FL6-5):

| Item | Custo (centésimos) | Conta | Preço |
|---|---|---|---|
| Grão | 0 + 1 h × 75 = 75 | 75 × 1,2 ÷ 2 lote = 45 centésimos = 4,5 décimos | **5** (0,5 moeda) |
| Água | igual ao Grão | | **5** (0,5) |
| Tora | igual ao Grão | | **5** (0,5) |
| Pão | (2 × 5 + 1 × 5) × 10 = 150; + 4 h × 100 = 550 | 550 × 1,2 ÷ 4 = 165 centésimos = 16,5 décimos | **17** (1,7) |
| Tábua | 2 × 5 × 10 = 100; + 400 = 500 | 500 × 1,2 ÷ 4 = 150 centésimos | **15** (1,5) |
| Móveis de Madeira | 3 × 15 × 10 = 450; + 400 = 850 | 850 × 1,2 ÷ 2 = 510 centésimos | **51** (5,1) |
| Pedra | igual ao Grão (Posto de Coleta, 2 em 1 hora) | | **5** (0,5) |
| Bloco de Pedra | 4 × 5 × 10 = 200; + 400 = 600 | 600 × 1,2 ÷ 4 = 180 centésimos (Pedreira, 4 Pedras em 4 horas: IP3-3) | **18** (1,8) |

Os seis primeiros batem com a tabela de **preços de partida** do [[Sistema Econômico/Descrição|Econômico]] (0,5; 1,7; 1,5; 5,1). Isso vira teste: se alguém mudar a fórmula sem querer, o teste avisa.

**Ordem das rodadas na fatia:** na 1ª rodada, Grão, Água e Tora; na 2ª, Pão e Tábua; na 3ª, Móveis de Madeira; a 4ª não muda nada, e o cálculo para.

## 6. Conversa com outros sistemas

| Sistema | O que lê das receitas | O que entrega às receitas |
|---|---|---|
| Itens | — | Os números dos produtos e insumos; guarda o preço base |
| Construções (marco 3: só os IDs) | — | Se a construção existe |
| Regras (`regras.json`) | — | O salário por hora e a margem |
| Recursos | — | Nada direto: a Braçal do Grão e o recurso Grão apontam para o mesmo item |
| Produção (marco 6) | O lote, os insumos, as horas, a habilidade mínima, a construção | — |
| IA da produção (marco 6) | Quais receitas cada construção faz | — |
| Econômico (marco 7) | O preço base, ponto de partida do preço local | — |

## 7. Nomes no código

Em `Codigo/Dados/`:

| Nome | O que é |
|---|---|
| `DefinicaoDeReceita` | Os campos da parte 2 |
| `InsumoDaReceita` | `numeroDoItem` e `quantidade` |
| `TrabalhoDaReceita` | `Bracal`, `Manufaturado`, `Especialista` |
| `CatalogoDeReceitas` | A lista das receitas e o índice "receita de cada produto" |
| `lerArquivoDeReceitas(textoDoArquivo, catalogoDeItens, listaDeConstrucoes, listaDeProblemas)` | A regra 4.1 |
| `calcularPrecosBaseDosItens(catalogoDeItens, catalogoDeReceitas, regrasDoPreco, listaDeProblemas)` | A regra 4.2 |
| `calcularPrecoDeUmaReceitaEmDecimos(receita, catalogoDeItens, regrasDoPreco)` | A conta da parte 5 |
| `arredondarMetadeParaCima(numerador, denominador)` | O passo 3 da conta |
| `conferirEscadaDeNiveis(...)` | A regra 4.3 |
| `RegrasDoPreco` | Os salários em centésimos e a margem |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| As 6 receitas da fatia | Sem erro nem aviso |
| Preços da fatia | Grão 5, Água 5, Tora 5, Pão 17, Tábua 15, Móveis de Madeira 51 |
| As mesmas receitas em ordem contrária no arquivo | Os mesmos preços |
| Um insumo `item.farinha` | Erro: o item não existe |
| 4 tipos de insumo | Erro: o máximo é 3 |
| O mesmo insumo duas vezes (`item.grao:1\|item.grao:1`) | Erro |
| O Pão como insumo do Pão | Erro |
| Duas receitas para o Pão | Erro com as duas linhas |
| Trabalho `bracal ` (com espaço) ou `Braçal` | Erro: fora da lista fechada |
| Horas 0 ou lote 0 | Erro |
| Laço: Tábua precisa de Móveis, e Móveis precisa de Tábua | Erro de laço citando os dois; os outros 4 itens ganham preço |
| Um item sem receita | Erro "não tem receita" |
| Os Móveis de Madeira com nível mínimo 1 | Aviso da escada, não erro |
| `arredondarMetadeParaCima(33, 2)` | 17 (16,5 sobe) |
| `arredondarMetadeParaCima(9, 2)` | 5 (4,5 sobe) |
| Um item com multiplicador 300 (como o Cavalo) | O preço triplica |

## 9. Fora da fatia

- As outras cerca de 170 receitas (FL3-1), os **moldes** de armas e armaduras expandidos ao carregar (um molde × três materiais) e a receita por animal do Açougue.
- Os campos opcionais: **subprodutos**, **exige no estoque** (os animais do Pasto e do Rancho) e **insumo opcional** (o Adubo da Fazenda).
- O preço por **Rank** (× 1; 1,5; 2,2; 3,2; 4,5; Rank 0 vale metade).
- Os itens sem receita que só se compram ou se acham (livros, Grimório): um preço fixo no `itens.csv`, quando existirem.
- A conferência de era e tecnologia (as eras entram com a Tecnologia).
