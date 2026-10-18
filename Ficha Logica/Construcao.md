---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 6** (produção, inventário e consumo). Cobre **os dados das construções** (porte, vagas, capacidade, quem trabalha) e o desenho delas. As **obras** (construir e subir de nível) e a **manutenção** ficam para depois do marco 6 (FL6-1). Os pontos marcados com FL6 foram decididos na **rodada FL6** (2026-10-18, toda com "seguir sugestão").

# Ficha Lógica: Construção

## 1. O que faz

Diz, para cada **tipo** de construção, o porte, quantas vagas ela tem em cada nível, quanto ela guarda no inventário da cidade e que Trabalho as vagas usam. E guarda, para cada construção **da vila**, o nível, o dono, os empregados e o gerente.

Documento do sistema: [[Sistema de Construção/Descrição|Construção]], com as vagas e os donos do [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]] e as Funções de [[Sistema de Produção/Funções e Construções|Funções e Construções]]. A vila inicial (os lotes, as portas) está na [[Ficha Logica/IA|ficha da IA]]; a produção, na [[Ficha Logica/Producao|ficha da Produção]].

## 2. Dados

### Definições: o `data/construcoes.json` completo

No marco 3 (FL3-5), o arquivo era só a lista dos IDs. No marco 6 ele ganha os campos de cada tipo:

```json
[
  { "id": "construcao.fazenda", "porte": "pequeno", "guarda": ["materia_prima"],
    "funcao": "fazendeiro" },
  { "id": "construcao.cozinheiro", "porte": "medio", "guarda": ["materia_prima", "bem_de_consumo"],
    "funcao": "cozinheiro" },
  { "id": "construcao.mercado", "porte": "medio", "guarda": ["bem_de_consumo"],
    "funcao": "lojista", "trabalho_sem_receita": "manufaturado" },
  { "id": "construcao.construtor", "porte": "pequeno", "guarda": ["materia_prima"],
    "funcao": "operario", "trabalho_sem_receita": "bracal" },
  { "id": "construcao.casa", "porte": "pequeno", "guarda": [], "moradores": 4 }
]
```

| Campo | O que é | Exemplo |
|---|---|---|
| `porte` | Pequeno, médio ou grande ([[Sistema de Construção/Descrição\|Construção]], V2-8) | `medio` |
| `guarda` | As categorias de item que ela soma à capacidade da cidade (Inventário, IT-4) | `["bem_de_consumo"]` |
| `funcao` | O nome da Função das vagas | `cozinheiro` |
| `trabalho_sem_receita` | O Trabalho de quem trabalha numa construção sem receita (Lojista, Operário). Nas outras, o Trabalho é o da receita | `manufaturado` |
| `moradores` | Só nas moradias: quantos moram em cada uma | 4 |

**As 8 da fatia** (7 na vila e a Pedreira, só nos dados, FL6-5):

| Construção | Porte | Guarda | Função | Trabalho |
|---|---|---|---|---|
| Fazenda | Pequeno | Matéria-prima | Fazendeiro | Pela receita (Braçal) |
| Posto de Coleta | Pequeno | Matéria-prima | Coletor | Pela receita (Braçal) |
| Cozinheiro | Médio | Matéria-prima e bem de consumo | Cozinheiro | Pela receita (Manufaturado) |
| Carpinteiro | Médio | Matéria-prima e bem de consumo | Carpinteiro | Pela receita (Manufaturado) |
| Mercado | Médio | Bem de consumo | Lojista | Manufaturado |
| Construtor | Pequeno | Matéria-prima | Operário | Braçal |
| Pedreira (fora da vila) | Médio | Matéria-prima e bem de consumo | Pedreiro | Pela receita (Manufaturado) |
| Casa | Pequeno | — | (moradia, 4 moradores) | — |

**No `data/regras.json`**, os números iguais para todos os tipos:

```json
"vagas_por_nivel": [2, 3, 5, 7, 10],
"capacidade_por_nivel": { "pequeno": 100, "medio": 200, "grande": 400 },
"obra_por_porte": {
  "pequeno": { "tabua": 10, "bloco_de_pedra": 10, "barra_de_ferro": 0, "horas": 40 },
  "medio":   { "tabua": 20, "bloco_de_pedra": 30, "barra_de_ferro": 0, "horas": 120 },
  "grande":  { "tabua": 40, "bloco_de_pedra": 80, "barra_de_ferro": 10, "horas": 400 }
}
```

- As **vagas** são a tabela geral (IP3-11); a **capacidade** é 100, 200 ou 400 × o nível (IT-4).
- A `obra_por_porte` (V2-8) só é lida e conferida no marco 6; ela é usada quando as obras entrarem.

### Estado: cada construção da vila

Com o que a ficha da IA já tem (o tipo, o lote, a porta, o dono):

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `nivel` | inteiro, de 1 a 5 | 1 (a Carpintaria do jogador, 2: FL6-2) | Sim |
| Os empregados | os NPCs com aquele trabalho | Elisa e Fábio | Sim (no NPC) |
| `indiceDoGerente` | o NPC, ou nenhum | Elisa | Não: recalculado |
| `vagas` | inteiro | 2 | Não: sai do nível |

O `vila_inicial.json` ganha o campo `"nivel"` em cada construção.

## 3. Quando roda

- **Ao abrir:** a leitura e a conferência do `construcoes.json` e dos números do `regras.json` (o `--validar` confere tudo).
- **Ao montar a vila e a cada dia** (antes dos salários): o gerente de cada construção (regra 4.2).
- A construção em si não roda nada por tick: quem produz é a [[Ficha Logica/Producao|Produção]].

## 4. Regras passo a passo

### 4.1 Conferir os dados

1. O `porte` é `pequeno`, `medio` ou `grande`; as categorias do `guarda` existem nos itens; o `trabalho_sem_receita`, se houver, é `bracal`, `manufaturado` ou `especialista`.
2. Uma construção de produção (que tem receitas no `receitas.csv`) não precisa do `trabalho_sem_receita`; uma sem receitas e sem `moradores` precisa.
3. Na vila inicial, os empregados de cada construção não passam das vagas do nível dela; a moradia não tem empregados.

### 4.2 O gerente (V3-8)

O empregado de **maior habilidade** no Trabalho da construção. No empate, o de menor número na vila. Ele recebe o salário × 1,5 ([[Ficha Logica/Economia|ficha da Economia]]).

### 4.3 Desenhar (no `Jogo/`)

1. Se existir a imagem `data/imagens/construcoes/<tipo>_nivel_<n>.png` (por exemplo, `construcoes/casa_nivel_1.png`), desenhe-a com a linha de baixo na linha de baixo do lote, e o resto subindo para fora dele (FA1-5, o [[Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]]).
2. Se não existir, desenhe o retângulo de cor do marco 5 (FL5-1). Assim, cada construção que o autor desenhar aparece no jogo sem mexer no código.
3. **Por cima de quem passa atrás:** a parte da imagem acima do lote (o telhado) é desenhada **depois** dos NPCs que estão mais acima na tela, como a copa das árvores. Por isso, as construções entram na mesma ordem de desenho dos NPCs (pela linha de baixo).

## 5. Contas

**Vagas e capacidade da vila no primeiro dia:**

| Construção | Nível | Vagas | Capacidade |
|---|---|---|---|
| Fazenda, Posto de Coleta, Construtor | 1 | 2 cada | 100 de matéria-prima cada |
| Cozinheiro | 1 | 2 | 200 de matéria-prima e 200 de bem de consumo |
| Mercado | 1 | 2 | 200 de bem de consumo |
| Carpintaria do jogador | 2 (FL6-2) | 3 (2 ocupadas) | 400 de matéria-prima e 400 de bem de consumo |
| **Total** | | | **900 de matéria-prima e 800 de bem de consumo** |

## 6. Conversa com outros sistemas

| Sistema | O que a Construção entrega | O que recebe |
|---|---|---|
| Inventário | A capacidade por categoria | — |
| Produção | As vagas, o gerente, o nível (o bônus de 10% por nível) | — |
| Economia | O dono (quem paga), o gerente (× 1,5), o nível (+10% de salário) | — |
| IA (marco 5) | A porta e o lote | Quem está dentro |
| Obras (depois) | O custo e as horas por porte | — |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `DefinicaoDeConstrucao` | Os campos do `construcoes.json` (com `porte`, `categoriasQueGuarda`, `funcao`, `trabalhoSemReceita`, `moradores`) |
| `CatalogoDeConstrucoes` | Já existe (marco 3); ganha as definições completas |
| `RegrasDasConstrucoes` | As vagas, a capacidade e a obra por porte, do `regras.json` |
| `calcularVagas(nivel)` e `calcularCapacidade(porte, nivel)` | As contas da parte 5 |
| `ConstrucaoDaVila::nivel` e `escolherGerente(...)` | O nível (na vila) e a regra 4.2 |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Vagas no nível 1, 2 e 5 | 2, 3 e 10 |
| Capacidade de uma construção média no nível 2 | 400 |
| O `construcoes.json` da fatia | 8 tipos, sem erro |
| Um porte `enorme` | Erro |
| O Mercado sem `trabalho_sem_receita` | Erro |
| 3 empregados numa Fazenda nível 1 | Erro: 2 vagas |
| Dois empregados com habilidade 50 e 60 | O de 60 é o gerente |
| Empate de habilidade | O de menor número na vila |
| A capacidade da vila da fatia | 900 de matéria-prima e 800 de bem de consumo |

## 9. Fora da fatia (e fora do marco 6)

- **As obras** (construir num lote livre, subir de nível, as horas dos Operários, o Engenheiro) e a **manutenção por estação** (FL6-1). A **pedra** delas vem dos 150 Blocos de Pedra do estoque inicial (FL6-5).
- A Vida das construções e a tempestade; a demolição; a compra e a venda; o leilão; a loja ilegal.
- Os donos Facção e Rico (a fatia só tem a Cidade e o jogador).
- A aparência nos níveis 3 e 5.
