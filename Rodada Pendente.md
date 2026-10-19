# Rodada pendente: FE3, o ritmo do dinheiro do jogador

**Perguntas enviadas em 2026-10-19, aguardando resposta.** Primeira rodada de ajuste pelo teste de 1 ano (`jogo --simular 64 --semente 1`). Responder no `Respostas.txt` pelo código (FE3-1...). "Seguir sugestão" aceita a proposta. Os números das opções são **estimativas** pelo modelo da [[Ficha Logica/Economia|ficha da Economia]] (partes 10 e 11); depois da resposta, o `--simular` confere o número de verdade.

**O que o teste mostrou:** a vila vai bem (ninguém passa fome, o Pão fica entre 1,3 e 1,9, o Banco entre 360 e 410). O problema é o jogador: ele começa com 100 moedas e termina o ano com 114,6, ou seja, **cerca de +0,45 moeda por dia**, e chega a cair para 70 no começo do Inverno. É assim por desenho: no modelo da FL7-1, a Carpintaria recebe uns 19 por dia pelos Móveis e paga uns 20 de salário, mais a Tora.

**FE3-1. Quanto o jogador deve ganhar por dia.**

- *Contexto:* as expansões custam (regra 10.4 da Construção, com os Blocos a 2,0 na loja da cidade e a hora de obra a 0,75): uma **Fazenda nova** (porte pequeno) custa cerca de **65 moedas** (10 Tábuas, 10 Blocos e 40 h); **subir a Carpintaria ao nível 3** (porte médio) custa cerca de **550** (60 Tábuas, 90 Blocos e 360 h).
- *Pergunta:* qual ritmo de crescimento o jogador deve ter na fatia? *Exemplo:* hoje, com +0,45 por dia, subir a Carpintaria ao nível 3 leva cerca de **1.200 dias, uns 19 anos de jogo** (o ano tem 64 dias). A Fazenda dá para fazer logo no começo, com o dinheiro inicial, mas a segunda leva uns 5 meses de jogo.
- *Opções:*
  - (a) **cerca de +5 moedas por dia** (uma Carpintaria saudável). Uma Fazenda a cada 13 dias; o nível 3 em cerca de 110 dias (menos de 2 anos). Ganha: o jogador sente o progresso dentro de uma partida. Perde: no jogo completo, com várias construções, o dinheiro pode crescer rápido demais (a conferir quando houver mais construções).
  - (b) cerca de +2 por dia. O nível 3 em uns 275 dias (pouco mais de 4 anos). Ganha: crescimento lento, cada decisão pesa. Perde: na fatia, com uma construção só, o jogador fica meses sem fazer nada novo.
  - (c) deixar como está (perto de 0). Ganha: nada muda. Perde: a fatia não tem progresso para o jogador; a única expansão é a do dinheiro inicial.
- *Sugestão:* (a), medido no `--simular` como "o dinheiro do jogador no dia 64, menos o do dia 1, dividido por 63", entre +4 e +6.
- *Conflitos:* nenhum com decisões anteriores; o caminho para chegar lá é a FE3-2.

**FE3-2. Como o jogador passa a ganhar mais.**

- *Contexto:* a Carpintaria vende só Móveis de Madeira, e a procura é fixa: a cesta tira 1,875 por dia (15 NPCs, 1 a cada 8 dias), e o gasto extra compra no máximo o mesmo número (regra 10.5 da Economia). Então mais produção não vende mais: o que muda o lucro é o **preço** dos Móveis ou o **custo** da Carpintaria.
- *Pergunta:* qual número mudar para chegar ao ritmo da FE3-1? *Exemplo:* com o preço base dos Móveis a 5,1, a Carpintaria recebe uns 19 por dia (3,75 Móveis × 5,1).
- *Opções:*
  - (a) **Móveis mais caros:** o `multiplicador_do_preco` dos Móveis no `itens.csv` sobe de 100 para 130 (o preço base vai de 5,1 para cerca de 6,6). A receita estimada sobe para uns 24 por dia, cerca de +5 para o jogador. Ganha: um número só, nos dados; o resto da economia não muda de regra. Perde: os Civis gastam mais com Móveis e sobra menos para o Banco (cerca de 5 por dia a menos, o Banco cai devagar; o estabilizador da regra 10.9 segura). Conferir no `--simular` se o Banco continua acima de 0 no ano.
  - (b) salário Manufaturado mais baixo (de 1,00 para 0,85 por hora). Ganha: a Carpintaria gasta uns 3 por dia a menos. Perde: vale para a cidade inteira (o Cozinheiro também), os Civis ganham menos, a Felicidade cai um pouco; e sozinho não chega a +5.
  - (c) uma segunda fonte de renda: o Construtor da Cidade **compra as Tábuas do jogador** para as obras e a manutenção (hoje ele só usa as da Cidade). Ganha: dá sentido à fila de Tábuas da Carpintaria. Perde: regra nova (ficha da Construção, regra 10.4), mais código, e a renda depende de quantas obras a cidade faz (pouca na fatia).
- *Sugestão:* (a), com o valor final escolhido pelo `--simular` entre 120 e 140 para dar os +4 a +6 da FE3-1; a (c) fica como [Refinar depois] para o jogo completo.
- *Conflitos:* muda o preço base dos Móveis (5,1), citado nos exemplos das fichas da Economia e da Interface; os exemplos são atualizados com o número novo.

**FE3-3. O preço do jogador nunca compensa.**

- *Contexto:* a regra 11.2 (FL8-6) deixa o jogador mudar o preço dos Móveis de −30% a +30%. A cesta compra de qualquer jeito; o gasto extra compra menos quando o preço sobe (a 30%, nada) e não compra mais quando o preço desce (ele já compra o máximo).
- *Pergunta:* o preço deve ser uma decisão de verdade? *Exemplo:* com os Móveis a 5,1, a receita do dia é: a 0%, 3,75 × 5,1 = **19,1**; a +15%, 1,875 × 5,9 + 0,94 × 5,9 = **16,6**; a +30%, 1,875 × 6,6 = **12,4**; a −15%, 3,75 × 4,3 = **16,1**. **O 0% é sempre o melhor**: o jogador que mexe no preço só perde.
- *Opções:*
  - (a) **deixar assim na fatia e anotar como [Refinar depois]**: o preço passa a valer quando houver concorrência (outra Carpintaria, o Mercador) ou uma procura que cresce com o preço baixo. Ganha: nenhuma regra nova agora. Perde: um controle da interface que não serve para nada por enquanto.
  - (b) o preço baixo atrai mais compra: abaixo de 0%, o gasto extra pode comprar até o dobro da cesta (a −30%, 1,875 × 2 a mais). A −30%: 5,6 × 3,6 = 20,2, um pouco acima do 0%. Ganha: o preço vira uma escolha (vender mais barato e mais). Perde: os Civis tiram mais dinheiro do Banco; muda a regra 10.5 da Economia.
  - (c) tirar o controle do preço da interface até a concorrência existir. Ganha: a tela não engana o jogador. Perde: desfaz uma parte do marco 8.
- *Sugestão:* (a), com uma linha na dica do controle: "Na vila de hoje, ninguém vende Móveis além de você: mudar o preço reduz a venda."
- *Conflitos:* nenhum com (a); (b) muda a regra 10.5 da Economia; (c) muda a FL8-6.
