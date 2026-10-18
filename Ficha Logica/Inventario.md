---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 6** (produção, inventário e consumo). Cobre **o inventário da cidade** (as pilhas, o dono e a capacidade). O inventário do personagem do jogador entra com o personagem. Os pontos marcados com FL6 estão na **rodada FL6** ([[Retomada]], seção 4), ainda sem resposta: a ficha já usa as sugestões, marcadas como [Proposta].

# Ficha Lógica: Inventário

## 1. O que faz

Guarda **quanto há de cada item** na cidade e **de quem é** (da Cidade ou do jogador), num inventário só, sem levar nada de uma construção para outra. Diz também quanto **cabe** em cada categoria: quando enche, a produção daquela categoria para.

Documento do sistema: [[Sistema de Inventário/Descrição|Inventário]] (V1-5: um inventário por cidade; IT-4: a capacidade). Quem põe e tira itens: a [[Ficha Logica/Producao|Produção]] e a [[Ficha Logica/Economia|Economia]] (o consumo).

## 2. Dados

### Estado (muda durante o jogo)

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `quantidadePorItemDaCidade` | lista de inteiros, uma posição por item (pelo número interno) | Pão: 45 | Sim |
| `quantidadePorItemDoJogador` | outra lista igual | Tábua: 12 | Sim |
| `capacidadePorCategoria` | inteiro por categoria | matéria-prima: 900 | Não: soma das construções |

- **Por que duas listas, e não uma tabela de pilhas** (item, dono, quantidade): a fatia só tem dois donos, e uma lista por dono é uma passada direta pela memória, como pede o Inventário ("um vetor por item"). Os Ricos e os itens com Rank (depois da fatia) ganham uma lista pequena à parte, como o documento prevê.
- **A capacidade vale para os dois donos juntos** [Proposta] (FL6-4): os itens do jogador ocupam o mesmo espaço da cidade (o jogador guarda nas construções dele, que somam à capacidade).

### Definições

A categoria de cada item vem do `itens.csv` (`materia_prima` ou `bem_de_consumo` na fatia), e a capacidade de cada construção, da [[Ficha Logica/Construcao|ficha da Construção]].

### O estoque do primeiro dia

No `vila_inicial.json` [Proposta] (FL6-4):

```json
"estoque_inicial": [
  { "item": "item.pao", "dono": "cidade", "quantidade": 45 },
  { "item": "item.grao", "dono": "cidade", "quantidade": 32 },
  { "item": "item.agua", "dono": "cidade", "quantidade": 16 },
  { "item": "item.tora", "dono": "cidade", "quantidade": 16 },
  { "item": "item.tabua", "dono": "jogador", "quantidade": 12 },
  { "item": "item.moveis_madeira", "dono": "jogador", "quantidade": 6 },
  { "item": "item.bloco_de_pedra", "dono": "cidade", "quantidade": 150 }
]
```

## 3. Quando roda

O inventário não roda nada sozinho: ele é lido e mudado pela produção (a cada hora) e pelo consumo (a cada dia). A capacidade é recalculada quando uma construção muda de nível ou é construída.

## 4. Regras passo a passo

### 4.1 Pôr um item

1. Some a quantidade de todos os itens da categoria, dos dois donos.
2. Se o que vai entrar passa da capacidade da categoria, **não entra**: devolva "não coube" (a produção espera; regra 4.2 da Produção).
3. Se não, some na lista do dono.

### 4.2 Tirar um item

1. Confira se o dono tem a quantidade. Se não tiver, **não tire nada** e devolva "faltou".
2. Se tiver, subtraia.

Tirar e pôr são sempre de uma vez (tudo ou nada): um ciclo de Pão nunca gasta só o Grão e deixa a Água para depois.

### 4.3 Somar a capacidade

Para cada construção da vila e cada categoria do `guarda` dela: capacidade da categoria += capacidade do porte × nível.

## 5. Contas

**A capacidade da vila da fatia:** 900 de matéria-prima e 800 de bem de consumo (ficha da Construção, parte 5).

**O estoque do primeiro dia** (FL6-4), em dias de consumo:

| Item | Quantidade | Quanto dura |
|---|---|---|
| Pão | 45 | 3 dias de 15 NPCs (o limite da "comida em perigo" da regra 4.3 da Produção) |
| Grão, Água | 32 e 16 | 4 dias do Cozinheiro (8 Grão e 4 Água por dia) |
| Tora | 16 | 2 dias da Carpintaria |
| Tábua e Móveis (do jogador) | 12 e 6 | O começo da Carpintaria; 6 Móveis cobrem 3 dias de Materiais Básicos |
| Bloco de Pedra | 150 | A primeira obra e a manutenção, quando entrarem (FL6-5) |

**O peso** (FL3-4) não conta no inventário da cidade: só no do personagem do jogador.

## 6. Conversa com outros sistemas

| Sistema | O que lê | O que muda |
|---|---|---|
| Produção | Os insumos; o espaço livre | Tira os insumos, põe o produto |
| Economia (consumo) | A comida e os Materiais Básicos | Tira o que a vila consome |
| Construção | — | A capacidade |
| Mercado Local (marco 7) | O estoque de cada item | — |
| Save (marco 9) | As duas listas | — |
| Interface (marco 8) | O painel do inventário | — |

## 7. Nomes no código

Em `Codigo/Inventario/`:

| Nome | O que é |
|---|---|
| `DonoDoItem` | `Cidade` ou `Jogador` |
| `InventarioDaCidade` | As duas listas e a capacidade por categoria |
| `quantidadeDe(item, dono)` | Quanto o dono tem |
| `tentarPor(item, dono, quantidade)` e `tentarTirar(...)` | As regras 4.1 e 4.2. Devolvem se deu certo |
| `tentarTirarVarios(lista de (item, quantidade), dono)` | Tira todos ou nenhum (os insumos de um ciclo) |
| `espacoLivreNaCategoria(categoria)` | A capacidade menos o que já está lá |
| `recalcularCapacidade(vila, catalogo, regras)` | A regra 4.3 |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Pôr 10 Pães da Cidade num inventário vazio, com capacidade 800 | 10 Pães da Cidade |
| Pôr 10 Tábuas do jogador | 10 do jogador; 0 da Cidade |
| Capacidade de matéria-prima 100, com 95 dentro: pôr 6 | Não coube; continua com 95 |
| Tirar 3 Grãos com 2 no estoque | Faltou; continua com 2 |
| Tirar 2 Grão e 1 Água com 2 Grão e 0 Água | Faltou; os 2 Grãos continuam lá |
| A capacidade da vila da fatia | 900 e 800 |
| O estoque inicial da fatia | 45 Pães da Cidade, 12 Tábuas do jogador |

## 9. Fora da fatia (e fora do marco 6)

- Os itens com Rank e instâncias (armas, armaduras) e os donos Ricos.
- O inventário do personagem do jogador (com peso e equipamento), das caravanas e dos Importantes.
- A logística só visual (carroças), que o documento deixa para o futuro.
