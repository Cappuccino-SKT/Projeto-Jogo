# Rodada pendente: FE5, o lucro do jogador numa crise de comida

**Perguntas enviadas em 2026-10-19.** Terceira rodada de ajuste, pela medida do ano 2 (`jogo --simular 128 --semente 1`; detalhes no `Jogo/LEIA-ME.md`, seção "Rodada FE4"). Responder no `Respostas.txt` pelo código. "Seguir sugestão" aceita a proposta.

**O que a medida mostrou:** nos dias 81 a 112 a vila tem uma crise de comida (Água baixa e a Febre do dia 85): o Pão cai de 92 para 43 e o preço sobe de 1,3 para 2,4. Os Civis ficam mais pobres (riqueza média 14 → 10,5), sobra menos para o gasto extra, a venda de Móveis cai de 3,3 para 2,7 por dia e o preço local deles cai até 4,3. A receita do jogador vai de 22,7 para 17,2 por dia; o salário da Carpintaria quase não muda (18,3 → 17,5). O saldo do jogador passa de **+3,4 para −0,9 por dia**, e volta a +0,2 quando a crise passa.

**FE5-1. O lucro do jogador deve aguentar uma crise de comida?**

- *Contexto:* o jogador só tem a Carpintaria. Os custos dela (salários) são fixos; a receita depende do dinheiro dos Civis. Ele já tem duas alavancas: o **salário baixo** (x0,75 no salário, x0,9 na produção; depois de 8 dias seguidos, um empregado sai) e as **ordens** (parar de produzir Móveis).
- *Pergunta:* o que acontece com o lucro do jogador quando a vila passa fome? *Exemplo:* no dia 90, o jogador perde 0,9 por dia; em 32 dias de crise, uns 30 moedas (de 305 para 275).
- *Opções:*
  - (a) **aceitar que a crise dá prejuízo**, sem regra nova. O jogador que percebe pode baixar o salário por até 7 dias, ou parar os Móveis. Ganha: a crise da vila pesa também no bolso do jogador, e cuidar da comida da vila passa a ser do interesse dele. Perde: o jogador que não acompanha perde um pouco (30 moedas em 32 dias, menos de 10 dias do lucro normal).
  - (b) piso para o preço dos Móveis: o preço local nunca abaixo de 80% do base (5,7 com o base de 7,1). Ganha: a receita cai menos (uns 19,5 por dia no pior momento). Perde: regra nova no Mercado Local (regra 10.4), só para os itens do jogador; os Civis, já sem dinheiro, compram ainda menos.
  - (c) salário que acompanha a venda: o salário da Carpintaria cai junto com a receita. Ganha: o jogador nunca perde. Perde: os empregados do jogador ficam mais pobres justo na crise; desfaz a escolha do nível de salário (FL8-7), que passa a ser automática.
- *Sugestão:* **(a)**. A perda é pequena e curta, o jogador já tem como reagir, e ela liga o interesse dele à comida da vila. Anotar como [Refinar depois] um alerta "a venda de Móveis caiu" quando a interface tiver o histórico de vendas.
- *Conflitos:* nenhum com (a); (b) muda a regra 10.4 da Economia; (c) muda a FL8-7.
