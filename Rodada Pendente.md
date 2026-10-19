# Rodada pendente: FE4, o dinheiro do Banco da Cidade

**Perguntas enviadas em 2026-10-19, aguardando resposta.** Segunda rodada de ajuste pelo teste de 1 ano (`jogo --simular 64 --semente N`). Responder no `Respostas.txt` pelo código (FE4-1...). "Seguir sugestão" aceita a proposta. Os números das opções são **estimativas** pelo modelo da [[Ficha Logica/Economia|ficha da Economia]] (partes 10 e 11); depois da resposta, o `--simular` confere o número final.

**O que o teste mostrou:** depois da FE3 (Móveis com `multiplicador_do_preco` 140), o jogador ganha **+3,4 por dia** (semente 1) e **+4,4** (semente 2), como pedido. Mas a economia da vila é **quase fechada**: o dinheiro só passa de mão em mão (Banco → salários → Civis → Pão, imposto e Móveis → Banco e jogador). Então tudo o que o jogador ganha sai do Banco. O Banco começa com 400 e termina o ano 1 em **175** (semente 1) e **108** (semente 2), caindo uns **3,6 a 4,6 por dia**; no ritmo de hoje, ele zera no meio do ano 2. A única entrada de fora é o **Mercador viajante** (3% por dia, uma pausa de 16 dias; cerca de 2 visitas por ano, de 9 a 20 moedas cada: uns **0,5 por dia**). O estabilizador (regra 10.9) não resolve: ele tira do Banco para ajudar a classe, não põe.

**A conta que importa:** para o Banco ficar parado (perto de 0 por dia em média) sem tirar os +4 do jogador, precisam entrar de fora uns **4 a 5 moedas por dia**, ou seja, **65 a 75 por estação** (a estação tem 16 dias) e **260 a 300 por ano**.

**FE4-1. De onde entra o dinheiro novo na cidade.**

- *Contexto:* hoje o dinheiro total da vila (Banco + jogador + Civis) só cresce pelo Mercador; qualquer lucro de um lado é perda do outro. Cobrar do jogador (imposto sobre o lucro, aluguel do lote) **não cria dinheiro**: só devolve ao Banco o que o jogador ganhou.
- *Pergunta:* qual fonte traz os cerca de 4,5 moedas por dia que faltam? *Exemplo:* no dia 32 da semente 2 o Banco está perto de 250 e cai 4,6 por dia; com a fonte nova ele deveria ficar entre 350 e 450 o ano inteiro, e o jogador continuar com os +4,4.
- *Opções:*
  - (a) **Verba da Coroa por estação:** no dia 1 de cada estação, entram **75 moedas** de fora no Banco (cerca de 4,7 por dia; 300 por ano). Um número no `regras.json`, contado em `dinheiroQueEntrouDeFora`. Ganha: simples, previsível, fácil de acertar no `--simular`; dá um "pagamento" visível 4 vezes por ano. Perde: é dinheiro que não vem de nada que o jogador ou a vila fazem; no jogo completo, com o Banco da Facção (V2-7), a direção do dinheiro precisa ser revista.
  - (b) **Exportação pelo Mercado:** a cada dia, a Cidade vende para fora a sobra dos seus itens acima do estoque-alvo, a **80% do preço local**, até **5 moedas por dia**. Com o Pão a 1,5, são uns 4 Pães por dia (1,2 cada). Ganha: o dinheiro vem da produção; uma vila que produz mais ganha mais (dá sentido às Fazendas novas). Perde: regra nova no Mercado Local (regra 10.4); depende de haver sobra (na Seca ou na Geada, nada entra); tirar Pão da vila pode subir o preço e a fome. O número precisa ser medido, não dá para garantir pela conta.
  - (c) **Mercador mais frequente e comprando mais:** chance de 3% para **20%** por dia, a pausa de 16 para **4 dias**, e até **60 unidades** em vez de 20 (com a Água a 0,45, cerca de 27 por visita; umas 11 visitas por ano, cerca de 300). Ganha: usa um evento que já existe; nenhuma regra nova. Perde: o dinheiro chega aos saltos e depende da sorte (a semente muda muito o resultado); um "evento" que acontece toda semana deixa de ser especial.
  - (d) **Imposto sobre o lucro do jogador ou aluguel do lote.** Ganha: o jogador sente um custo fixo. Perde: não cria dinheiro; para o Banco ficar parado, teria de tirar **quase todo** o lucro (os +4), o que desfaz a FE3-1.
- *Sugestão:* **(a), 75 moedas por estação**, valor final escolhido no `--simular` (sementes 1 e 2) para o Banco terminar o ano entre 350 e 500 sem nunca ficar abaixo de 250. A (b) fica como [Refinar depois] para o jogo completo, quando houver mais itens e sobras de verdade.
- *Conflitos:* a [[Ficha Logica/Eventos|ficha dos Eventos]] diz que o Mercador é "a única entrada de dinheiro de fora" e a ficha da Economia tem o teste "o dinheiro total só muda pelo Mercador" (parte 10.11): os dois textos ganham a fonte nova. Na V2-7 a cidade fica só com 30% do imposto e a Facção com o resto; na fatia não há Facção (FL7-2), então não há conflito agora, mas no jogo completo a verba e a parte da Facção precisam ser pensadas juntas.

**FE4-2. A verba é fixa ou acompanha o Banco.**

- *Contexto:* vale se a FE4-1 for (a). As sementes perdem de 3,6 a 4,6 por dia; um valor fixo serve para as duas, mas uma semente pior (ou o jogo completo, com mais construções do jogador) pode precisar de mais.
- *Pergunta:* como se calcula a verba de cada estação? *Exemplo:* no começo do Verão o Banco está em 330. Com a verba fixa entram 75 (vai a 405). Com "completar até 400" entram 70. Com a verba pela Felicidade (média 58), entram 75 × 58 ÷ 60 = 72,5.
- *Opções:*
  - (a) **fixa: 75 por estação.** Ganha: o jogador e o teste sabem o que esperar; se a economia ficar desequilibrada, o `--simular` mostra. Perde: se o jogador lucrar muito mais (no jogo completo), o Banco volta a cair e é preciso reajustar o número.
  - (b) **completar o Banco até 400, no máximo 100 por estação.** Ganha: o Banco nunca fica muito baixo, qualquer que seja o lucro do jogador. Perde: esconde os desequilíbrios (o teste sempre passa); um Banco que já está em 400 não ganha nada, e o jogador não tem por que cuidar da vila.
  - (c) **pela Felicidade:** 75 × a Felicidade média ÷ 60 (com 58, cerca de 72; com 40, 50). Ganha: liga o dinheiro da cidade ao bem-estar dos Civis; cuidar da vila vale a pena. Perde: uma vila infeliz perde dinheiro e fica mais infeliz (uma espiral para baixo); mais uma ligação entre sistemas para testar.
- *Sugestão:* **(a)** na fatia; a (c) fica como [Refinar depois] para quando a Felicidade tiver mais peso no jogo.
- *Conflitos:* nenhum com (a); (c) acrescenta uma ligação na parte 6 da ficha da Economia (a Felicidade já é lida pelos multiplicadores do salário, regra 10.6).

**FE4-3. Como o teste confere que o Banco fecha.**

- *Contexto:* o `--simular 64` mostra 1 ano. O problema da FE3 só apareceria de verdade no ano 2 (o Banco zerando). O teste de 1 ano leva 45 s; um de 2 anos levaria uns 90 s.
- *Pergunta:* o que o `--simular` deve exigir do Banco para dizer **PASSOU**? *Exemplo:* hoje a semente 2 passa com o Banco em 108 no dia 64, mas ele não sobreviveria ao ano 2.
- *Opções:*
  - (a) **1 ano, com uma regra nova:** o Banco no dia 64 não pode estar mais de **50 abaixo** do dia 1 (com 400 no início, pelo menos 350), e nunca abaixo de **250**. Ganha: pega a queda lenta sem gastar mais tempo de teste. Perde: não vê problemas que só aparecem depois de um ano (por exemplo, a classe ficando rica aos poucos).
  - (b) rodar 2 anos (`--simular 128`) no `ctest`, exigindo só que o Banco nunca zere. Ganha: confere de verdade o ano 2. Perde: o `ctest` passa de 1 min e meio para uns 2 min e meio; e uma queda lenta ainda passa se não zerar.
  - (c) as duas: a regra de (a) no teste de 1 ano, e o de 2 anos só à mão, quando mexer na economia. Ganha: o `ctest` continua rápido. Perde: depende de lembrar de rodar o de 2 anos.
- *Sugestão:* **(c)**: a regra de (a) no teste de 1 ano (sementes 1 e 2), e a linha "rodar `--simular 128` depois de mexer na economia" no `Jogo/LEIA-ME.md`.
- *Conflitos:* muda o critério do teste de 1 ano da Fatia Vertical (marco 7), que hoje só exige que o Banco nunca zere.
