# Rodada pendente

## Rodada HI12: a vida até os 30 anos de jogo (2026-10-26)

**Contexto.** Na HI11, o autor pediu para **aumentar o limite de vida para 30 anos**. Hoje, pela [[Ficha Logica/Populacao|População]] (`regras.json`): o NPC vira adulto aos **4** anos de jogo e fica velho aos **21**. A partir daí, a chance de morrer de velhice é de 10% por estação para cada ano acima de 20: aos 21, 10% por estação; aos 22, 20%. Na prática, quase todos morrem entre 21 e 23. Por isso a idade da história é dividida por 4: 84 anos da história equivalem a 21 do jogo. A HI11-1 foi respondida (Cassius Varro ganha o traço Vida prolongada, com os Companheiros).

**Exemplo de jogo:** com a velhice aos 31, um ferreiro que chega adulto aos 4 anos trabalha uns 27 anos antes de ficar velho, não mais 17. A vila troca menos gente por morte, e os líderes e as Grandes Histórias duram mais.

**Pergunta HI12-1: como alongar a vida?**

| Opção | O que muda | Ganha | Perde |
|---|---|---|---|
| **A** | Só a velhice: velho aos **31** (a chance conta acima de 30); adulto continua aos 4. A idade da história passa a ser dividida por **2,5** (80 anos da história ≈ 32 do jogo) | A menor mudança que faz a vida chegar a cerca de 30; a infância continua curta, e a vila repõe trabalhadores rápido | A infância fica curta demais para a nova escala (o adulto de 4 anos equivale a 10 anos da história) |
| **B** | Velho aos **31** e adulto aos **6**; tudo na escala de 2,5 (adulto aos 15 anos da história) | A escala fica coerente do começo ao fim | Os filhos levam mais tempo para trabalhar; a vila cresce mais devagar |
| **C** | Velho aos 31 só para os **Importantes** (líderes, Companheiros); os comuns seguem como hoje | As histórias duram, e a população não muda | Duas velhices diferentes: estranho e difícil de explicar |

**Sugestão: A.**

**Com a A, os líderes ficariam assim** (a idade da história ÷ 2,5): Lívia 22, Yasmin 14, Tian Feng 20, Edmund Hale 24, Theodoros IV 29 e Cassius Varro 52 (só pela Vida prolongada). A faixa do líder no começo passa de **8 a 18** para **8 a 28**.

**Conflitos:** muda o `regras.json` (idade_velho de 21 para 31, a chance acima de 30), a [[Ficha Logica/Populacao|População]], a regra 4.6 da [[Ficha Logica/Faccao|Facção]] (a divisão e a faixa do líder) e a CP1-4 dos [[Ficha Logica/Companheiros|Companheiros]]. A população cresce mais (morre menos gente), então os testes da População e o teste de um ano podem mudar de número; isso é conferido no código.
