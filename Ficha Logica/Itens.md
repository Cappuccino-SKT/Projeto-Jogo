---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 3** (carregar os dados). As decisões da rodada **FL3** (2026-10-18, todas "seguir sugestão") já estão aplicadas. Esta ficha também descreve **a ordem de leitura de todos os dados e o `jogo --validar`**, porque os itens são os primeiros a serem lidos.

# Ficha Lógica: Itens

## 1. O que faz

Lê a lista de itens do `data/itens.csv` e os nomes do `textos/pt-BR.json`, confere cada linha e dá a cada item um **número interno**, que o resto do jogo usa no lugar do texto. O **preço base** de cada item é calculado depois, pelas receitas ([[Ficha Logica/Receitas|ficha das Receitas]]).

Documento do sistema: [[Sistema de Itens/Descrição|Sistema de Itens]]. Formato dos dados: [[Arquitetura/Moldes de Conteúdo|Moldes de Conteúdo]]. Mapa geral: [[Ficha Logica/Fatia Vertical|Fatia Vertical]], partes 1 e 2.

**Por que um número interno:** comparar `item.pao` com `item.pao` é comparar 8 letras; comparar o número 3 com o 3 é uma conta só. No meio da simulação, o jogo faz isso milhões de vezes. O texto continua existindo para quem edita os dados e para as mensagens de erro.

**Na fatia: 8 itens:** Grão, Água, Tora, Pão, Tábua e Móveis de Madeira (FL3-1), e a Pedra e o Bloco de Pedra, para as obras (FL6-5). Os outros cerca de 172 ficam para uma tarefa de dados depois da fatia.

## 2. Dados

### O arquivo `data/itens.csv` (FL3-1 e FL3-4)

Uma planilha salva em CSV, com **ponto e vírgula** separando as colunas (o padrão do Excel e do LibreOffice em português, porque a vírgula é a dos decimais). Nenhum número tem vírgula: tudo é inteiro.

```
id;categoria;tipo;tier;rank_min;rank_max;peso;multiplicador_do_preco
item.grao;materia_prima;bruto_vegetal;0;;;1;100
item.agua;materia_prima;bruto_mineral;0;;;1;100
item.tora;materia_prima;bruto_vegetal;0;;;3;100
item.pao;bem_de_consumo;alimento;1;;;1;100
item.tabua;materia_prima;manufaturado;1;;;2;100
item.moveis_madeira;bem_de_consumo;mobilia;2;;;8;100
item.pedra;materia_prima;bruto_mineral;0;;;3;100
item.bloco_de_pedra;materia_prima;manufaturado;1;;;3;100
```

O peso da Pedra e do Bloco (3, como a Tora) é [Proposta].

- A primeira linha diz o nome de cada coluna. As colunas podem vir em qualquer ordem, e o jogo as acha pelo nome.
- Linha vazia, ou que começa com `#`, é ignorada: serve para comentários e para separar grupos na planilha.
- O **nome** que aparece na tela não fica aqui: fica no `textos/pt-BR.json` (Moldes, regra 1), por exemplo `"item.moveis_madeira": "Móveis de Madeira"`.

### Definições (lidas do arquivo, não mudam durante o jogo)

| Campo | Tipo | Unidade | Exemplo | Regra |
|---|---|---|---|---|
| `identificadorDeTexto` | texto | — | `item.pao` | Começa com `item.`; só letras minúsculas sem acento, números e `_`; único |
| `numeroInterno` | inteiro | — | 3 | A posição no arquivo, contando do 0 |
| `categoriaDoItem` | lista fechada | — | `bem_de_consumo` | `materia_prima`, `bem_de_consumo`, `arma`, `armamento_de_exercito`, `equipamento`, `artefato` |
| `tipoDoItem` | lista fechada | — | `alimento` | Precisa combinar com a categoria (tabela abaixo) |
| `tier` | inteiro ou vazio | 0 a 4 | 1 | Obrigatório em matéria-prima e bem de consumo; vazio no resto |
| `rankMinimo`, `rankMaximo` | inteiros ou vazios | 0 a 5 | vazio | Só em armas, equipamentos e artefatos (fora da fatia) |
| `pesoEmUnidades` | inteiro | unidades de peso | 1 | Maior que 0 (FL3-4) |
| `multiplicadorDoPrecoEmPorcento` | inteiro | % | 100 | De 1 a 1.000; o Cavalo terá 300 (Econômico) |
| `precoBaseEmDecimos` | dinheiro | décimos de moeda | 17 | **Calculado** pelas receitas, nunca escrito no arquivo |

**Os tipos de cada categoria** (do [[Sistema de Itens/Descrição|Sistema de Itens]]):

| Categoria | Tipos aceitos |
|---|---|
| `materia_prima` | `bruto_mineral`, `bruto_vegetal`, `bruto_animal`, `manufaturado` |
| `bem_de_consumo` | `alimento`, `bebida`, `mobilia`, `vestuario`, `essencia_e_joia`, `narcotico`, `medicamento` |
| `arma` | `espada`, `lanca`, `machado`, `clava`, `arco`, `besta`, `arma_de_fogo`, `cajado`, `municao` |
| `armamento_de_exercito` | `artilharia` |
| `equipamento` | `capacete`, `peitoral`, `perneiras`, `chapeu`, `camisa`, `calcas`, `anel`, `escudo` |
| `artefato` | `livro`, `joia_de_mana`, `artefato` |

### Estado

Nenhum: a lista de itens não muda durante o jogo. As pilhas ("Pão × 12" no estoque da cidade) são do [[Sistema de Inventário/Descrição|Inventário]], no marco 6, e guardam só o número interno e a quantidade.

## 3. Quando roda

**Uma vez, ao abrir o jogo**, antes de abrir a janela. A ordem de leitura de todos os dados (marco 3):

1. `textos/pt-BR.json`: os nomes.
2. `data/regras.json`: os números das rodadas (salários, margem, produção por estação).
3. `data/itens.csv`: esta ficha.
4. `data/construcoes.json`: no marco 3, só a lista de IDs (FL3-5).
5. `data/recursos.csv`: a [[Ficha Logica/Recursos|ficha dos Recursos]].
6. `data/receitas.csv`: a [[Ficha Logica/Receitas|ficha das Receitas]].
7. **Preços base:** a regra 4.2 da ficha das Receitas.
8. `data/mapas/vila.tmj`: a [[Ficha Logica/Mapa|ficha do Mapa]], agora com os tipos de área vindos dos recursos.

**Por que essa ordem:** cada arquivo só cita o que já foi lido. A receita cita itens e construções; o recurso cita o item que gera; o mapa cita os recursos.

**Todos os erros de uma vez:** como no mapa (FL1-5), um erro não para a leitura. O jogo lê tudo, junta todos os erros e só então decide: com erro, mostra a lista e não abre.

## 4. Regras passo a passo

### 4.1 Ler o `itens.csv`

1. Leia o arquivo. Se ele não existir, anote o erro e pare esta etapa.
2. Leia a linha de cabeçalho. Falta uma coluna obrigatória (`id`, `categoria`, `tipo`, `tier`, `peso`)? Erro, e pare esta etapa. As colunas `rank_min`, `rank_max` e `multiplicador_do_preco` são opcionais (vazias = sem Rank e 100%).
3. Para cada linha seguinte que não seja vazia nem comentário:
   1. confira o ID (prefixo, letras, único). Repetido: erro "item.pao aparece duas vezes (linhas 5 e 9)";
   2. confira a categoria e o tipo nas listas fechadas;
   3. confira o Tier: obrigatório e de 0 a 4 em matéria-prima e bem de consumo; vazio no resto;
   4. confira o peso (inteiro maior que 0) e o multiplicador (de 1 a 1.000);
   5. dê ao item o próximo número interno e guarde-o na lista e no "índice por texto".
4. Cada erro diz o arquivo, a linha e o motivo: "itens.csv, linha 4: o tipo "pão" não existe na categoria bem_de_consumo".

### 4.2 Conferir os nomes

Para cada item, procure o ID no `textos/pt-BR.json`. Faltou: erro "item.tabua não tem nome em textos/pt-BR.json". Sobrou um texto `item.` que não é de nenhum item: **aviso**, porque pode ser um item apagado de propósito.

### 4.3 Achar um item pelo ID

Quem lê os outros arquivos (recursos, receitas) pergunta "qual é o número de `item.grao`?". A resposta vem do índice por texto. Se o ID não existe, quem perguntou anota o erro com o arquivo e a linha dele.

### 4.4 O modo `jogo --validar`

1. `jogo --validar` lê todos os dados na ordem da parte 3, **sem abrir janela**.
2. Escreve no console cada aviso e cada erro, e no fim um resumo: "Dados conferidos: 6 itens, 3 recursos, 6 receitas, 1 mapa. 0 erros, 1 aviso."
3. Termina com o código 0 se não houve erro, e 1 se houve. Assim, no futuro, dá para rodá-lo antes de cada commit.

## 5. Contas

Não há fórmula nos itens: o preço base está na ficha das Receitas. As únicas contas são de posição:

- **Número interno:** a ordem das linhas válidas. *Exemplo:* com as 6 linhas do exemplo, `item.grao` = 0, `item.agua` = 1, `item.tora` = 2, `item.pao` = 3, `item.tabua` = 4, `item.moveis_madeira` = 5.
- **Número da linha no erro:** conta a linha de cabeçalho como 1, como a planilha mostra. O Pão está na linha **5**.

**Peso (FL3-4):** a "unidade de peso" tem o Pão como referência (1). O Grão e a Água pesam 1, a Tábua 2, a Tora 3 e os Móveis de Madeira 8. Na fatia, o peso só limita o que o personagem do jogador carrega (marco 6).

## 6. Conversa com outros sistemas

| Sistema | O que lê dos itens | O que entrega aos itens |
|---|---|---|
| Receitas | O número de cada produto e insumo | O preço base de cada item |
| Recursos | O número do item que cada recurso gera | — |
| Inventário (marco 6) | O número, a categoria e o tipo (para a capacidade por categoria) | — |
| Classe Social e consumo (marcos 6 e 7) | O tipo e o Tier (consumo por Tier) | — |
| Interface (marco 8) | O nome, para mostrar "Pão × 12" | — |
| Econômico (marco 7) | O preço base, ponto de partida do Mercado Local | — |

## 7. Nomes no código

Em `Codigo/Dados/` (sem SDL nem OpenGL):

| Nome | O que é |
|---|---|
| `DefinicaoDeItem` | Os campos da parte 2 |
| `CategoriaDoItem`, `TipoDoItem` | As listas fechadas, como `enum class` |
| `CatalogoDeItens` | A lista dos itens e o índice por texto |
| `procurarNumeroDoItem(identificadorDeTexto)` | A regra 4.3; devolve "não existe" quando falta |
| `lerArquivoDeItens(textoDoArquivo, tabelaDeTextos, listaDeProblemas)` | A regra 4.1 e a 4.2 |
| `TabelaDeTextos` | Os nomes do `textos/pt-BR.json` |
| `TabelaCsv`, `lerTabelaCsv(textoDoArquivo)` | O leitor de CSV próprio (FT2-2), usado pelos itens, recursos e receitas: separa as linhas e as colunas, guarda o número de cada linha |
| `ListaDeProblemasDosDados` | Os erros e os avisos, cada um com o arquivo, a linha e o motivo |
| `DadosDoJogo`, `carregarDadosDoJogo(pastaDosDados)` | Tudo junto, na ordem da parte 3 |
| `validarDadosSemJanela()` | O `jogo --validar` (no `Jogo/main.cpp`, que só chama o `Codigo/` e escreve no console) |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| O `itens.csv` da fatia | 8 itens, sem erro; `item.pao` é o número 3 |
| As colunas em outra ordem | O mesmo resultado |
| Uma linha `# comentário` e uma linha vazia | Ignoradas; os números internos não mudam |
| `item.pao` duas vezes | Erro com as duas linhas |
| ID `Item.Pão` | Erro: letra maiúscula e acento |
| Tipo `alimento` na categoria `materia_prima` | Erro: o tipo não combina com a categoria |
| Tier vazio num bem de consumo | Erro |
| Tier 5 | Erro: o Tier vai de 0 a 4 |
| Peso 0 | Erro |
| Falta a coluna `peso` | Erro de cabeçalho; nenhum item lido |
| `item.tabua` sem nome no `pt-BR.json` | Erro |
| Um texto `item.velho` sem item | Aviso, não erro |
| Procurar `item.grao` | Número 0 |
| Procurar `item.farinha` | "Não existe" |
| `jogo --validar` com os dados da fatia | Resumo com 0 erros; código de saída 0 |
| `jogo --validar` com um erro | O erro com arquivo e linha; código de saída 1 |

## 9. Fora da fatia

- Os outros cerca de 174 itens (FL3-1), com os atributos por tipo (dano, defesa, saciedade) em JSON.
- Rank, cultura, textura, encantamento e as instâncias de armas e equipamentos.
- A proibição por facção, o Valor da Facção e o Local (o Local entra no marco 7).
- Os efeitos dos itens especiais ([[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).
- Os nomes de fantasia e outros idiomas (o arquivo de textos já está pronto para eles).
