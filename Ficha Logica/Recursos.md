---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 3** (carregar os dados). As decisões da rodada **FL3** (2026-10-18, todas "seguir sugestão") já estão aplicadas.

# Ficha Lógica: Recursos

## 1. O que faz

Lê a lista de recursos do `data/recursos.csv`: cada recurso diz que item ele gera, que construção o tira do mapa e qual área do Tiled o marca. Junta isso às **áreas de recurso** do mapa da vila e calcula o **fator de produção** de cada recurso: a produtividade do mapa (normal ou destaque) vezes a estação.

Documentos do sistema: [[Sistema de Recursos/Descrição|Sistema de Recursos]], [[Sistema de Recursos/Recursos Existentes|Recursos Existentes]] e a produção por estação do [[Sistema de Clima e Estações/Descrição|Clima e Estações]].

**No marco 3, nada é produzido:** a ficha só lê, confere e sabe responder "quanto rende o Grão no outono?". Quem usa a resposta é a Produção, no marco 6.

**Na fatia: 3 recursos**, os do mapa da vila: Grão (o campo), Água (o poço) e Madeira (o bosque, que gera Tora).

## 2. Dados

### O arquivo `data/recursos.csv` (FL3-6)

```
id;item_gerado;construcao;area_no_tiled;grupo_da_estacao
recurso.grao;item.grao;construcao.fazenda;campo_de_grao;fazenda
recurso.agua;item.agua;construcao.posto_de_coleta;poco;agua
recurso.madeira;item.tora;construcao.posto_de_coleta;bosque;madeira
```

- `area_no_tiled` é o valor da propriedade `tipo_de_recurso` das áreas no Tiled (FL1-5). Por isso o poço se chama `poco` no mapa e o recurso é a Água.
- `grupo_da_estacao` diz que linha da tabela de estações vale para o recurso: o Posto de Coleta tira Madeira e Pedra, que rendem diferente no inverno, então a estação vai **por recurso**, e não por construção (Clima e Estações).

### A produção por estação, no `data/regras.json` (FL3-6)

```json
"producao_por_estacao_em_porcento": {
  "fazenda":  [75, 100, 150, 25],
  "animais":  [100, 100, 100, 75],
  "madeira":  [100, 100, 100, 75],
  "minerais": [100, 100, 100, 90],
  "agua":     [100, 100, 100, 100]
},
"produtividade_do_recurso_em_porcento": { "normal": 100, "destaque": 150 }
```

A ordem das estações é sempre Primavera, Verão, Outono e Inverno. Os números são os da tabela base do [[Sistema de Clima e Estações/Descrição|Clima e Estações]] (bioma temperado) e da V2-9.

### A produtividade da vila: no próprio mapa (FL3-6)

Cada área de recurso do Tiled ganha uma propriedade opcional `produtividade`, com `normal` (o padrão, se faltar) ou `destaque`. Na vila, as três são normais. No jogo completo, ela virá do `mapas.json` ([[Sistema de Recursos/Descrição|Recursos]]).

### Definições (não mudam durante o jogo)

| Campo | Tipo | Exemplo | Regra |
|---|---|---|---|
| `identificadorDeTexto` | texto | `recurso.grao` | Começa com `recurso.`; único |
| `numeroDoItemGerado` | número interno | o de `item.grao` | O item precisa existir |
| `construcaoQueExtrai` | ID de construção | `construcao.fazenda` | A construção precisa existir |
| `nomeDaAreaNoTiled` | texto | `campo_de_grao` | Único entre os recursos |
| `grupoDaEstacao` | lista fechada | `fazenda` | Um dos grupos do `regras.json` |

Na vila, cada área de recurso do mapa guarda também o **número do recurso** e a **produtividade** (100 ou 150).

### Estado

Nenhum. A produtividade não muda durante o jogo, e a estação vem do relógio. O save não guarda nada dos recursos ([[Sistema de Recursos/Descrição|Recursos]], "Save").

## 3. Quando roda

- **Ao abrir o jogo**, depois dos itens e das construções e antes das receitas (ordem na [[Ficha Logica/Itens|ficha dos Itens]], parte 3): a regra 4.1.
- **Ao ler o mapa**, logo depois: a regra 4.2 liga cada área do Tiled ao seu recurso.
- **Quando a Produção pedir** (marco 6, a cada ciclo Braçal): a regra 4.3. A conta é tão barata que não precisa ser guardada.

## 4. Regras passo a passo

### 4.1 Ler o `recursos.csv`

1. Leia o arquivo e o cabeçalho, como nos itens. As 5 colunas são obrigatórias.
2. Para cada linha:
   1. confira o ID (prefixo `recurso.`, letras, único);
   2. procure o item gerado. Não existe: erro "recursos.csv, linha 3: o item item.tora não existe";
   3. procure a construção. Não existe: erro;
   4. confira o nome da área (letras minúsculas, único entre os recursos);
   5. confira o grupo no `regras.json`. Não existe: erro.

### 4.2 Ligar as áreas do mapa aos recursos

1. Para cada área de recurso lida do Tiled, procure o recurso cujo `area_no_tiled` é igual ao `tipo_de_recurso` da área. Não achou: erro "a área de recurso 21 tem o tipo "pocoo", que nenhum recurso usa".
2. Leia a propriedade `produtividade`: vazia = `normal`; `normal` = 100; `destaque` = 150; outro valor = erro.

**Isso muda uma regra do mapa:** na ficha do Mapa (regra 4.1, passo 8), os tipos conhecidos eram uma lista fixa no código (`campo_de_grao`, `poco`, `bosque`). Agora eles vêm do `recursos.csv`, e um recurso novo não precisa de código novo (Moldes, regra 2).

### 4.3 O fator de produção

**Fator** = produtividade do recurso no mapa (%) × produção da estação do grupo (%) ÷ 100, em **por cento**, com uma casa decimal guardada como decimal (é um fator, não dinheiro: a regra do dinheiro inteiro não vale aqui).

A Produção, no marco 6, multiplica as unidades de cada ciclo Braçal por esse fator ÷ 100 e guarda a sobra com vírgula, para nada se perder ([[Sistema de Produção/Descrição|Produção]]).

## 5. Contas

| Recurso | Produtividade | Estação | Conta | Fator |
|---|---|---|---|---|
| Grão (normal) | 100 | Outono (150) | 100 × 150 ÷ 100 | **150%** |
| Grão (normal) | 100 | Inverno (25) | 100 × 25 ÷ 100 | **25%** |
| Grão (destaque) | 150 | Inverno (25) | 150 × 25 ÷ 100 | **37,5%** |
| Madeira (normal) | 100 | Inverno (75) | 100 × 75 ÷ 100 | **75%** |
| Água (normal) | 100 | qualquer (100) | 100 × 100 ÷ 100 | **100%** |

*Exemplo de jogo (marco 6):* a Fazenda da vila tem 2 Fazendeiros, que juntos fazem 32 Grão por dia a 100% (V2-12). No inverno, com 25%, fazem **8** (bate com a Arquitetura: "32 Grão, 8 no inverno").

## 6. Conversa com outros sistemas

| Sistema | O que lê dos recursos | O que entrega aos recursos |
|---|---|---|
| Itens | — | O número do item gerado |
| Construções (marco 3: só os IDs) | — | Se a construção existe |
| Mapa | — | As áreas, com o tipo e a produtividade |
| Relógio do mundo | — | A estação atual |
| Produção (marco 6) | O fator de cada recurso; qual construção tira qual recurso; onde fica a área (os Braçais vão trabalhar lá) | — |
| Receitas | — | As receitas Braçais dizem as horas e o lote da extração ([[Ficha Logica/Receitas\|Receitas]]) |

## 7. Nomes no código

Em `Codigo/Dados/`:

| Nome | O que é |
|---|---|
| `DefinicaoDeRecurso` | Os campos da parte 2 |
| `GrupoDaEstacao` | O nome do grupo e os 4 valores por estação, em % |
| `CatalogoDeRecursos` | A lista dos recursos e o índice por texto e por nome de área |
| `lerArquivoDeRecursos(textoDoArquivo, catalogoDeItens, listaDeConstrucoes, regras, listaDeProblemas)` | A regra 4.1 |
| `ligarAreasDoMapaAosRecursos(mapaLocal, catalogoDeRecursos, listaDeProblemas)` | A regra 4.2 |
| `calcularFatorDeProducaoEmPorcento(produtividadeEmPorcento, grupoDaEstacao, estacaoAtual)` | A regra 4.3 |

Na `AreaDeRecurso` do mapa ([[Ficha Logica/Mapa|Mapa]]) entram dois campos: `numeroDoRecurso` e `produtividadeEmPorcento`.

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| O `recursos.csv` da fatia | 3 recursos, sem erro |
| `item_gerado` = `item.carvao` (não existe na fatia) | Erro com o arquivo e a linha |
| Construção `construcao.mina` | Erro: a construção não existe |
| Dois recursos com a área `poco` | Erro |
| Grupo `pantano` | Erro: não existe no `regras.json` |
| O mapa da vila depois da leitura | As 3 áreas ligadas: campo → Grão, poço → Água, bosque → Madeira; todas com 100% |
| Uma área com o tipo `pocoo` | Erro: nenhum recurso usa esse tipo |
| Uma área com `produtividade` = `destaque` | 150% |
| Uma área com `produtividade` = `muita` | Erro |
| Fator do Grão normal no outono | 150 |
| Fator do Grão de destaque no inverno | 37,5 |
| Fator da Água em qualquer estação | 100 |

## 9. Fora da fatia

- Os outros 19 recursos (Ferro, Pedra, os animais, o Peixe...) e o `mapas.json` com os 48 Mapas Locais.
- Os ajustes por **bioma** (montanha, árido, litoral, floresta enevoada) e os eventos de clima que reduzem a produção.
- O **limite de construções por recurso** (2 num normal, 4 num de destaque; a Água sem limite), que entra com a Construção (marco 6).
- A tecnologia que aumenta a produtividade.
- O Adubo (insumo opcional da Fazenda, +25%).
