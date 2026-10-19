# Rodada pendente: FE1, o equilíbrio do teste de 1 ano (marco 7)

**Perguntas enviadas em 2026-10-19, aguardando respostas.** O código do marco 7 rodou o teste de 1 ano com 10 sementes e por 4 anos (256 dias), numa cópia compilada sem depuração (64 dias em 1,5 segundo). Com a semente 1, ele **passa**, mas apareceram 2 problemas de equilíbrio. Os dois mudam números já decididos (a FL6-3 e a FL6-4), por isso viram perguntas; os dados continuam como estão até a resposta. (A FL7 foi aplicada em 2026-10-18, toda com "seguir sugestão".)

Responder no `Respostas.txt` pelos códigos (FE1-1, FE1-2). "Seguir sugestão" aceita a proposta.

**FE1-1. Os Móveis caros (o dinheiro que vai do Banco para o jogador).**

- *Contexto:* a FL6-3 deu à Carpintaria do jogador as ordens "manter 6 Móveis" e "manter 12 Tábuas": 6 Móveis eram 3 dias da cesta (1,875 por dia). No marco 7, o gasto extra compra mais Móveis (até o dobro: cerca de 3,75 por dia), e o consumo medido sobe: o estoque-alvo dos Móveis passa a cerca de 19, mas a ordem para em 6. O estoque fica entre 2 e 5, e o preço local vai a 8 ou 9,6 (o teto é 10,2, o dobro do base de 5,1). A classe paga ao jogador quase o dobro, o lucro do jogador ainda não paga imposto (FL7-2), e o dinheiro sai do Banco para o jogador. Com a semente 1, no dia 64: Banco 272, jogador 237. Em 4 anos, o Banco fica quase vazio por volta do dia 190 (jogador com 572). É pior no Inverno: a Fazenda produz 25%, nunca folga, e o Banco paga mais salários.
- *Pergunta:* como segurar o preço dos Móveis? *Exemplo:* no dia 60, os Móveis estão a 9,5, com 2 no estoque, e a Carpintaria parada porque já tem os 6 que a ordem pede (ela tem Tábua e tempo para fazer mais).
- *Opções* (medidas):
  - (a) **as ordens passam a "manter 20 Móveis" e "manter 12 Tábuas".** Com a FE1-2 (a): em 10 sementes, o Banco fica entre 332 e 415 no dia 64; em 4 anos, nunca abaixo de 334; o jogador fica entre 56 e 160; os Móveis, entre 3,3 e 6,9. Ganha: o Mercado Local faz o trabalho dele, sem regra nova; no marco 8, quem escolhe as ordens é o jogador. Perde: o jogador lucra pouco, porque o preço base quase não cobre os salários da Carpintaria (o gerente ganha × 1,5);
  - (b) cobrar o imposto também do lucro do jogador (a V2-7 fala em "salários e lucros"). Ganha: igual ao documento. Perde: precisa da conta do lucro do dia (vendas − salários − insumos), e os Móveis continuam faltando e caros;
  - (c) limitar o preço dos itens do jogador a +30% do base (a faixa da FT1-1) até o marco 8. Medido: só atrasa; o Banco ainda fica quase vazio por volta do dia 190;
  - (d) deixar como está. O teste de 1 ano passa com a semente 1, mas o Banco cai, e o segundo ano já falharia.
- *Sugestão:* (a).
- *Conflitos:* muda a FL6-3 (manter 6 → 20 Móveis). O "manter 12 Tábuas" continua.

**FE1-2. O Pão no primeiro mês (a Febre no Cozinheiro).**

- *Contexto:* o Cozinheiro tem 2 empregados e faz 16 Pães por dia; a vila come 15 (FL6-7). A vila começa com 45 Pães (3 dias, FL6-4), e o estoque-alvo é 75 (5 dias): o Pão começa caro (2,3 no dia 3), e o estoque sobe só 1 Pão por dia. Se a Febre pega um dos Cozinheiros no primeiro mês, faltam 8 Pães por dia, por 4 dias. Na semente 3, no dia 27, a Febre pega o Fábio: o estoque cai de 63 para 32, e o preço vai a 2,7 no dia 31 (o teste falha acima de 2,6). Em 10 sementes, 2 falham (a 3 e a 4), as duas assim.
- *Pergunta:* como a vila aguenta uma Febre no Cozinheiro no começo do ano? *Exemplo:* a semente 3, acima.
- *Opções* (medidas):
  - (a) **a vila começa com 75 Pães (5 dias, o estoque-alvo), junto com a FE1-1 (a).** Em 10 sementes, todas passam; o Pão fica entre 1,3 e 2,1. Ganha: um número só; o preço já começa no base. Perde: sozinha, sem a FE1-1 (a), ela tira dinheiro do Banco (o Pão barato do começo: Banco entre 22 e 161 no dia 64), então as duas vão juntas. Uma Febre nos 2 Cozinheiros ao mesmo tempo ainda deixaria a vila sem Pão novo por 4 dias, mas os 75 Pães cobrem 5;
  - (b) o Cozinheiro no nível 2, com 3 empregados (o Nuno sai dos desempregados). Com a FE1-1 (a), todas passam, mas o Pão desce até 1,2 (o limite do teste), e o jogador fica quase sem dinheiro (de 9 a 33). Ganha: uma Febre não para o Pão. Perde: muda a vila inicial (FL5-1: 12 empregados e 3 desempregados) e o desemprego (de 20% para 13%);
  - (c) deixar como está: 2 em cada 10 sementes falham no primeiro ano.
- *Sugestão:* (a).
- *Conflitos:* muda a FL6-4 (3 → 5 dias de Pão: 45 → 75 no `vila_inicial.json`).

Depois das respostas:

- mudar o `vila_inicial.json`, rodar de novo as 10 sementes e anotar a rodada na parte 8 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]] e na [[Ficha Logica/Economia|Economia]] (os números da FL6-3 e da FL6-4);
- depois, as fichas do marco 8 (a interface do jogador, as obras e o Mercado).
