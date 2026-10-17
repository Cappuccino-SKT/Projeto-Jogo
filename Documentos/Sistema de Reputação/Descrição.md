> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Reputação e Relações do jogador

**Descrição geral**

Define **o que cada facção e cada personagem importante pensa do jogador**, como isso muda e o que isso muda no jogo: preços, guardas, contratos e títulos.

Três medidas convivem, cada uma com seu papel:

| Medida | Escala | O que diz | Onde está |
|---|---|---|---|
| **Reputação** | −100 a +100, por facção e por Vila Livre | O que a facção pensa do jogador | Este documento |
| **Opinião** | −100 a +100, por NPC Importante conhecido | O que aquela pessoa pensa do jogador | Este documento |
| **Recompensa** | Valor por facção | A lei quer o jogador | [[Sistema de Crime e Justiça/Descrição\|Crime e Justiça]] |
| **Renome** | 0 a 100, global | Fama pelos feitos | [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]] |

### Com quem o jogador tem reputação

- **Cada facção** e **cada Vila Livre** tem um valor de **−100 a +100**. As cidades usam o valor da sua facção (não há reputação por cidade).
- **Cada NPC Importante** que o jogador já encontrou tem uma **Opinião** pessoal (ver abaixo). Assim cabem histórias como "a facção me odeia, mas o herdeiro é meu amigo".

### Faixas

| Faixa | Valor |
|---|---|
| Inimigo | −100 a −60 |
| Hostil | −59 a −20 |
| Neutro | −19 a +19 |
| Amigável | +20 a +59 |
| Aliado | +60 a +100 |

### O que cada faixa muda

| Faixa | Preços no Mercado | Guardas | Contratos e missões | Títulos da facção |
|---|---|---|---|---|
| Inimigo | Lojas se recusam a negociar | Mandam sair da cidade; quem fica vira alvo | Nenhum | Nenhum; perde o título que tiver |
| Hostil | +15% na compra, −15% na venda | Revistam na entrada (chance de achar itens proibidos) | Só os do quadro da Taverna | Nenhum |
| Neutro | Normal | Normal | Quadro da Taverna | Plebeu e Cidadão |
| Amigável | −5% na compra, +5% na venda | Normal | Missões de nobres | Cavaleiro (a partir de +20) e Nobre (a partir de +40) |
| Aliado | −10% na compra, +10% na venda | Ajudam o jogador em brigas | Missões do líder | Senhor de cidade (a partir de +60) |

- Os títulos continuam pedindo também **Renome** e o requisito concreto do [[Objetivo e Progressão/Descrição|Objetivo e Progressão]]; a reputação é um requisito a mais. (O futuro sistema de Títulos de Nobreza pode mudar esses requisitos por cultura, facção e lei; ver o Objetivo.)
- *Exemplo:* o Mercador com Amigável na Escravista compra algodão 5% mais barato lá; se cair para Hostil por contrabando, passa a pagar 15% a mais e a ser revistado na entrada.

### Reputação e Recompensa

São **valores separados, mas ligados**. A Recompensa diz "a lei quer você"; a reputação diz "o que pensam de você".

- Todo crime **detectado** também tira reputação: **−2 × a gravidade do crime** (um crime de gravidade 2 tira 4; um assassinato, de gravidade 5, tira 10).
- Pagar a Recompensa limpa a ficha, mas a reputação só volta aos poucos (ver "Volta com o tempo").

### Como sobe e desce

Valores iniciais, a testar:

| Ação | Efeito na facção |
|---|---|
| Contrato da Taverna cumprido na cidade da facção | +2 |
| Missão de nobre cumprida | +5 |
| Missão longa (3 a 5 etapas) cumprida | +10 |
| Derrotar bandidos no território da facção | +1 por bando |
| Ajudar uma caravana em apuros | +2 com o dono |
| Lutar ao lado da facção numa batalha | +5 |
| Doar dinheiro à facção | +1 a cada 100 moedas, até +20 no total |
| Crime detectado | −2 × gravidade |
| Atacar caravana ou patrulha da facção | −10 |
| Lutar contra a facção numa batalha | −15 |
| Recusar o chamado às armas sendo vassalo | −10 |
| Libertar escravos (só nas facções que permitem escravidão) | −5 por grupo |
| Vender / comprar / libertar escravo (com a Religiosa) | −3 / −1 / +2 (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) |

**Efeito em cadeia, só na guerra.** Nas ações de guerra (batalha, saque, escolta de exército), o inimigo da facção ajudada perde **metade** do que ela ganhou. Nas outras ações, não há cadeia. *Exemplo:* lutar ao lado da Guerreira contra a Autoritária dá +5 na Guerreira e −2,5 na Autoritária.

**Volta com o tempo.** A reputação anda **2 pontos por estação** em direção ao valor base. Quem sumiu por um ano (4 estações) volta 8 pontos: um erro antigo não dura para sempre, e uma amizade precisa de cuidado.

### Valor base

**Base = afinidade da cultura do personagem com a facção + bônus do estilo de início.**

| Origem | Efeito na base |
|---|---|
| Facção da mesma cultura | +20 |
| Cultura afim | +10 |
| Cultura neutra | 0 |
| Cultura hostil | −15 |
| Estilo Membro de facção | +20 na própria facção |
| Estilo Nobre exilado | −30 na facção que derrubou a dinastia dele |
| Estilo Escravo | −20 com o dono e a facção dele |

As afinidades estão no [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]. *Exemplo:* um Mago de cultura Arcana começa com +20 na Arcana, 0 na Guerreira e −15 na Religiosa e na Tecnocrata. As Vilas Livres começam em 0 para todos.

### Opinião dos NPCs Importantes

O nome **Opinião** foi escolhido para não confundir com o atributo **Relação** do NPC, que é a lista de vínculos de família.

- Vai de **−100 a +100** e existe só para os Importantes que o jogador já encontrou: líder, herdeiro, nobres, os 5 Importantes fixos e os companheiros.
- **Muda por:**
  - **conversa** (persuadir, elogiar, ameaçar), usando o Carisma (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]);
  - **presentes**: +1 a +10, conforme o valor do presente e a Ganância do NPC;
  - **missões** pedidas por ele: +10;
  - **ações contra a família ou a cidade dele**: −10 a −50.
- **O que muda:**
  - **Rito de Admissão por Indicação:** exige Opinião de pelo menos +40 de um nobre da facção.
  - **Decisões do líder sobre o jogador** (pedido de título, resposta a uma revolta, vassalagem): a Opinião do líder entra no cálculo junto com a reputação da facção.
  - **Herdeiro:** quando o líder morre, a Opinião do herdeiro passa a pesar nas decisões da facção.
  - **Companheiros:** a Opinião vira a lealdade deles (detalhada no sistema de Companheiros).

### Interface

- Painel de **Reputação**: lista das facções e das Vilas Livres, com valor, faixa e a cor da faixa.
- Para cada uma, o **"Por quê?"** com as 5 últimas mudanças ("+5: lutou ao lado da facção na batalha de (2,3), há 3 dias").
- **Notificação** quando a faixa muda (por exemplo, de Neutro para Amigável).
- A Opinião aparece no painel do Importante e ao passar o mouse sobre ele.

### Parte técnica

- **Dados:** `reputacao.json` com as faixas, os efeitos de cada faixa, a tabela de ações e as bases por afinidade e estilo, para ajustar sem mexer no código.
- **Save:** o valor por facção e Vila Livre, a Opinião de cada Importante conhecido e o histórico das últimas mudanças (alimenta o "Por quê?" e o diário).
- **Custo:** dezenas de números; a volta com o tempo é calculada uma vez por estação.
- **Fatia vertical:** não usa (sem facções).

### Ligação com outros sistemas

| Sistema | O que a Reputação fornece ou recebe |
|---|---|
| Facção | Reputação e Opinião do líder entram na resposta à revolta, na vassalagem e nos pedidos de título |
| Guerra | Batalha, saque e escolta de exército são ações de guerra (cadeia de metade); lutar como mercenário conta como batalha; pedido de cidade conquistada usa a Opinião (ver [[Sistema de Guerra/Descrição\|Sistema de Guerra]]) |
| Crime e Justiça | Crime detectado tira reputação; Recompensa continua separada |
| Itens | A faixa define a chance de revista do Livro Sagrado (ver [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]]) |
| Objetivo e Progressão | Faixa mínima para cada título; missões de nobres e do líder |
| Início de Jogo | Base por cultura e estilo de início |
| NPC | Opinião dos Importantes (diferente do atributo Relação) |
| Econômico | Preço de compra e venda por faixa |
| Interface | Painel de Reputação, "Por quê?" e notificação de mudança de faixa |
| Viagem | Patrulhas prendem quem tem Recompensa; ajudar caravana em apuros dá reputação |
| Cultura | Afinidade entre culturas define a base; Rito de Admissão por Indicação usa a Opinião |
| Companheiros e Diálogo | Opinião vira lealdade; casamento dá +20 na facção da família (ver [[Sistema de Companheiros e Diálogo/Descrição\|Sistema de Companheiros e Diálogo]]) |

### Perguntas em aberto

1. **Valores a testar:** faixas, efeitos de preço, ações, volta por estação.
2. **Títulos de Nobreza** (sistema futuro): requisitos por cultura, facção e lei podem substituir a tabela de faixas por título.
