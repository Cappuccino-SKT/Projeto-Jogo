> A situação geral em poucas linhas. Onde paramos e como trabalhamos: [[Retomada]]. Atualizado em 2026-10-19.

## Situação

- **Design:** completo e revisado (cerca de 45 documentos; índice em [[Descrição do Projeto]]).
- **Código:** marcos 0 a 9 prontos e testados pelo autor: a fatia vertical está completa (2026-10-19).
- **Fatia vertical:** uma vila de 15 Civis; o jogador com uma Carpintaria; o teste de 1 ano sem tela.

## Números-chave

| Tema | Valor |
|---|---|
| Tempo | 20 ticks por segundo; 800 ticks por hora de jogo (40 s reais a 1x); dia = 16 min; estação = 16 dias; ano = 64 dias |
| Dinheiro | Inteiro, em décimos de moeda; preço = (insumos + horas × salário) ÷ lote × 1,2 |
| Salário por hora | Braçal 0,75; Manufaturado 1,0; Especialista 1,5 |
| Mapa | Tile de 16 × 16 px; mapa da fatia de 64 × 64 tiles; zoom de 2x, 3x e 4x |

## O que falta

- Ajustar os números pelo teste de 1 ano; decidir o que vem depois da fatia.
- A arte (por enquanto, só a de teste), a história do mundo e as rodadas de conteúdo C1 a C3.
- Cerca de 80 marcações [Em aberto] e [Refinar depois], sem pressa.
