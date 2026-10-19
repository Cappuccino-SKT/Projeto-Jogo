# Rodada pendente

## Rodada CL1 — o Clima e Estações (2026-10-19)

Três dúvidas que a **Simulação de mesa** da [[Ficha Logica/Clima|ficha do Clima]] (parte 10) achou. Responda em `C:\Users\Felipe\Desktop\Respostas.txt` com o código e a letra, ou "seguir sugestão".

---

### CL1-1 — O que o NPC faz em casa na tempestade

**Contexto:** o documento diz que, na tempestade, "NPCs ficam em casa" no Período Livre. A IA da fatia só tem 5 ações (Dormir, Trabalhar, Comer, Comprar, Passear) e nenhuma é "ficar em casa".

**Pergunta:** numa tempestade às 19h, a Ana está passeando no centro da vila. O que ela faz até o clima mudar?

| Opção | Como funciona | Ganha | Perde |
|---|---|---|---|
| **A** | O Passear ganha o destino "a própria casa": ela volta e fica lá dentro | Nenhuma ação nova; a vila fica vazia na tempestade, como o documento quer | Dentro de casa, ela fica parada (invisível) até dormir |
| **B** | Ação nova "Ficar em casa" (nota 100 na tempestade), com animação de entrar | Mais clara no código e no painel do NPC | Uma ação a mais na IA, nos testes e no save |
| **C** | Ela antecipa o Dormir | Nada novo | Dormir às 19h muda a rotina e o horário de acordar |

**Sugestão:** **A**. É o menor custo, e o resultado na tela é o mesmo.

**Conflitos:** nenhum. O Comprar perdido não muda o dinheiro, porque a cesta sai do estoque à 0h.

---

### CL1-2 — Falta lenha no primeiro dia frio do inverno

**Contexto:** a lenha começa de repente no dia 1 do Inverno (de 0 para 3 a 6 Toras por dia). O Posto de Coleta escolhe a receita pela média dos **últimos** 5 dias, então ainda não sabe disso.

**Pergunta:** na simulação de mesa (parte 10.2), a vila entra no inverno com 10 Toras, o Posto faz Água no dia 49 e neva no dia 50: faltam 5 Toras de lenha. Felicidade −10 no consumo naquele dia (a real cai cerca de 1 ponto), e a Carpintaria do jogador fica sem Tora de manhã. Isso deve ser evitado?

| Opção | Como funciona | Ganha | Perde |
|---|---|---|---|
| **A** | Deixar como está | Nenhuma regra nova; um "susto" pequeno e realista no começo do inverno | Acontece quase todo ano; o jogador perde um pouco de produção no dia |
| **B** | Nos **últimos 4 dias do Outono**, o alvo de estoque da Tora soma a lenha prevista de 5 dias (moradores × 0,25 × 5 = cerca de 19 Toras para 15 Civis) | O Posto estoca antes; quase nunca falta | Uma regra a mais na previsão da Produção (ficha da Produção, parte 10) |
| **C** | O Posto faz Tora sempre que for inverno e o estoque estiver abaixo de 10 | Simples | Regra fixa, que não cresce com a vila |

**Sugestão:** **B**. É o "estocar para o inverno" do documento, e serve depois para a comida (CL1-3).

**Conflitos:** nenhum. Muda só o alvo da Tora no fim do Outono.

---

### CL1-3 — O Inverno rigoroso: o quanto corta e a fome

**Contexto:** o documento diz "efeitos do inverno × 1,5" e dá o exemplo da Fazenda de 25% para cerca de 17%. Isso não diz o que acontece com a madeira. A simulação (parte 10.3) mostra:

- Se a **madeira** também cair (75% → 50%), o Posto não dá conta da lenha em dobro + Carpintaria + Água (precisaria de 105% dos dias). Falta lenha quase todo dia.
- Com a Fazenda em 16%, a vila perde cerca de 46 Grãos no inverno. Na semente 1, o Pão do estoque segura (o inverno termina com cerca de 20 Pães, e a fome começa em 7). Com Ratos no celeiro ou uma Seca no verão anterior, **a fome chega** e o teste de 1 ano dá FALHOU.

**Pergunta 1 (o corte):** em quais produções o Inverno rigoroso divide o percentual de inverno por 1,5?

| Opção | Fazenda | Animais | Madeira | Minerais | Ganha | Perde |
|---|---|---|---|---|---|---|
| **A** | 16% | 50% | 75% | 90% | A lenha cabe; o rigoroso pesa na comida | A madeira não sente o inverno pior |
| **B** | 16% | 50% | 50% | 60% | Fiel ao "× 1,5 em tudo" | Falta lenha o inverno inteiro na vila |

**Pergunta 2 (a fome):** o teste de 1 ano pode falhar por causa de um Inverno rigoroso?

| Opção | Como funciona | Ganha | Perde |
|---|---|---|---|
| **A** | Usar a regra da CL1-2 também para a comida: nos últimos 4 dias do Outono, o alvo do Grão soma 5 dias de consumo | A vila estoca e o teste passa; o rigoroso dói, mas não mata | Uma regra a mais (a mesma da CL1-2) |
| **B** | O teste de 1 ano desliga o Inverno rigoroso | Nada novo | O teste deixa de medir o pior inverno |
| **C** | Aceitar a fome como parte do jogo; o teste aceita até 4 dias de fome num inverno rigoroso | Fiel ao documento | O teste fica menos rígido justo onde mais importa |

**Sugestão:** pergunta 1 **A**; pergunta 2 **A**.

**Conflitos:** a Pergunta 1 A diverge do texto "efeitos do inverno × 1,5" do documento, que será ajustado. A chance (25% no dia 1 do Inverno, cooldown de 64 dias, cerca de 1 a cada 5 anos) também é [Proposta].
