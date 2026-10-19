# Rodada pendente

## Rodada TP1: o Trabalho e Propriedade (2026-10-19)

Quatro dúvidas que a **Simulação de mesa** da [[Ficha Logica/Trabalho|ficha do Trabalho]] (parte 10) achou. Responda em `C:\Users\Felipe\Desktop\Respostas.txt` com o código e a letra, ou "seguir sugestão".

---

### TP1-1: Quando a Cidade contrata, e com que trava

**Contexto:** hoje, uma vaga vazia numa construção da Cidade fica vazia para sempre (só o jogador contrata). O documento fala só da redistribuição a cada estação. A simulação (Caso C) mostrou um furo: se a Cidade recontratar no dia seguinte quem saiu por salário atrasado, o atraso zera, e a saída por atraso não serve para nada.

**Pergunta:** o Coletor Bruno sai do Posto de Coleta no dia 20, por 16 dias de salário atrasado. Há 3 desempregados. Quando a vaga é preenchida, e quem pode pegar?

| Opção | Como funciona | Ganha | Perde |
|---|---|---|---|
| **A** | Todo dia, pelo desempregado de maior habilidade. Trava: o Banco precisa ter 3 dias do salário, e quem saiu por atraso não volta para a mesma construção na mesma estação | A vila se recupera rápido; o furo fica fechado | Duas regras pequenas a mais |
| **B** | Só no dia 1 de cada estação, junto da redistribuição | Mais simples; nada de "sai e volta" | A vaga pode ficar até 16 dias vazia (a Fazenda produz pela metade) |
| **C** | Todo dia, sem trava | O mais simples | O "sai e volta" sem fim |

**Sugestão:** **A**.

**Conflitos:** nenhum. O teste de 1 ano não muda: hoje nenhuma vaga da Cidade fica vazia.

---

### TP1-2: A redistribuição pode tirar o emprego de alguém?

**Contexto:** a cada estação, a Cidade redistribui "os NPCs sem vaga ou com vaga ruim" (documento). Trocar dois empregados que ficam melhores no Trabalho um do outro é fácil (exemplo: Bruno, bom cozinheiro na Fazenda, troca com a Carla, boa fazendeira no Cozinheiro: a velocidade dos dois sobe de cerca de 90% para cerca de 117%).

**Pergunta:** a Fazenda tem a Ana (Braçal 40). O Davi está desempregado e tem Braçal 70. No dia 1 da estação, a Cidade troca os dois (a Ana fica desempregada)?

| Opção | Como funciona | Ganha | Perde |
|---|---|---|---|
| **A** | Só troca entre empregados; ninguém perde o emprego | Nenhum NPC é "demitido" pela Cidade; a Felicidade não sofre | Um desempregado bom espera uma vaga |
| **B** | Também troca desempregado por empregado, se for 10 ou mais melhor | A Cidade produz mais | A Ana perde o emprego sem ter feito nada; o desemprego roda entre as pessoas |

**Sugestão:** **A**. Na vila de hoje, todos têm 50, então nada muda; a escolha só pesa quando as habilidades variarem.

**Conflitos:** nenhum.

---

### TP1-3: O valor das construções e o modo automático

**Contexto:** o documento diz que o jogador compra da Cidade "a preço de mercado", sem dizer quanto. E a simulação (Caso A) mostrou que, se o jogador comprar o Posto de Coleta, o Posto passa a seguir a fila de ordens do jogador. Com a fila vazia, ele para: a Tora acaba em cerca de 5 dias, a Água em 9, e a vila passa fome em cerca de 14.

**Pergunta 1 (o valor):** quanto custa o Posto de Coleta (pequeno, nível 1)?

| Opção | Como funciona | Exemplo | Ganha | Perde |
|---|---|---|---|---|
| **A** | O custo da obra a preço de hoje × o nível; a Cidade compra de volta a 75% | Posto 60; Carpintaria nível 2: 330 (venda a 247,5) | Usa o que já existe; acompanha o preço da Tábua | Construções baratas; o jogador pode comprar a vila toda cedo |
| **B** | O mesmo × 2 (paga também o "ponto" e os empregados) | Posto 120; Carpintaria 660 | Comprar é uma decisão maior | Um número inventado a mais |
| **C** | Pelo lucro: 64 dias do lucro médio da construção | Depende do teste | Mais "real" | Uma construção com prejuízo valeria 0 ou menos |

**Pergunta 2 (o modo automático):** a construção comprada começa como?

| Opção | Como funciona | Ganha | Perde |
|---|---|---|---|
| **A** | Com o **modo automático** ligado: a IA da Cidade escolhe a receita, até o jogador desligar | Um clique não derruba a vila | Uma opção a mais no painel |
| **B** | Na fila de ordens, vazia, com um aviso no Diário | Nada novo | Fácil de esquecer; fome em cerca de 14 dias |

**Sugestão:** pergunta 1 **A**; pergunta 2 **A**.

**Conflitos:** nenhum. A Simulação também mostrou que comprar o Posto dá **prejuízo** (cerca de −2 a −5 por dia): ele é barato e vende barato. Fica assim, sem regra nova.

---

### TP1-4: Para onde vai o dinheiro do leilão

**Contexto:** uma construção do jogador parada (sem empregados) por 2 estações vai a leilão por 3 dias, com lance mínimo de 50% do valor (V3-8). O documento não diz quem recebe o lance. Hoje só o jogador pode dar lance (os Ricos chegam na fase A4), então o leilão quase sempre termina sem lance, e a construção volta para a Cidade de graça.

**Pergunta:** quando os Ricos existirem, um deles compra a Carpintaria quebrada do jogador por 165 (50% de 330). Para onde vão as 165 moedas?

| Opção | Como funciona | Ganha | Perde |
|---|---|---|---|
| **A** | Todas para o Banco | Simples; o jogador falido perde tudo (punição forte) | O jogador não recupera nada |
| **B** | Primeiro pagam os salários atrasados (aos Civis); o resto vai para o jogador | Justo, como um leilão de verdade; o jogador recomeça com algo | Uma conta a mais (guardar quanto deve) |
| **C** | Para o jogador, todas | O jogador sempre recupera | Os Civis que ficaram sem salário não recebem |

**Sugestão:** **B**. Sem lance (o caso de hoje), a construção volta para a Cidade e o jogador não recebe nada.

**Conflitos:** a ficha propunha A; muda a regra 4.4.
