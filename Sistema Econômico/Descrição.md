> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema Econômico

**Descrição geral**

O preço é dividido em **3 camadas**, cada uma influenciando a menor. Isso permite preços diferentes conforme a região do mapa.

| Camada | O que faz | O que muda o preço | Quando é recalculada |
|---|---|---|---|
| **Mercado Mundial** | Dá o valor primário de todos os produtos; é o primeiro a definir um valor para cada item | Eventos climáticos, descoberta de tecnologia, eventos políticos | No início do jogo e a cada evento mundial que altere o preço de um produto |
| **Mercado da Facção** | Recebe o valor do Mercado Mundial e aplica uma porcentagem (para mais ou para menos). Vale só nos Mapas Locais dominados pela facção | Lei de Proibição (item proibido), ajustes de preço por categoria (leis), eventos exclusivos da facção | No início do jogo e a cada evento interno da facção que altere preços |
| **Mercado Local** | Recebe o preço da facção e o ajusta de novo. É o **valor final**, usado pelo jogador na compra e na venda | Oferta e demanda | A cada dia de jogo e quando o jogador compra ou vende |

### Loja

Cada construção comercial funciona como uma **Loja**, com atributos próprios:

- **Dono** (Cidade, Facção, Rico ou jogador; ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]]);
- **Categoria vendida** (liga com a Categoria dos itens);
- **Dinheiro próprio** (usado para pagar o jogador quando ele vende itens a ela);
- **Construção associada** (o Mapa Interno do tipo Construção Pública onde ela funciona).

### Armazenamento de itens no Mercado

Cada cidade tem um Mercado Local e **um inventário próprio**, que contabiliza todos os itens das lojas e construções da cidade. Esse inventário é **compartilhado** por todas as lojas, o que simplifica a logística: não há inventário separado por construção (decisão confirmada na revisão V1-5; ver [[Sistema de Inventário/Descrição|Inventário]]).

- Para evitar que os itens do jogador se misturem com os da cidade (por exemplo, o jogador usar por engano os materiais da cidade na própria produção), cada item tem o atributo **Dono**.
- O atributo **Categoria** permite mostrar cada item nas lojas certas, e o jogador frequenta lojas diferentes para comprar coisas diferentes.

### Compra e venda pelos NPCs Não-Controlados

Os NPCs Não-Controlados têm rotinas que incluem atender a demanda de consumo, frequentando as lojas da cidade, tanto as da cidade quanto as do jogador.

- **Comprando de uma loja do jogador:** o NPC muda o dono do item (do jogador para a cidade) dentro do inventário da cidade; em troca, o jogador recebe o valor do item.
- **Comprando de uma loja da cidade:** o valor vai para o dinheiro da loja, e nada muda no inventário (o item já era da cidade).
- **O jogador comprando:** além de mudar de dono, o item sai do inventário da cidade e vai para o inventário do NPC Controlado. Se o jogador depois o guardar na própria loja, ele volta ao inventário da cidade.

### Distribuição de dinheiro

Para que os NPCs Não-Controlados e as lojas tenham dinheiro, há uma **distribuição rotativa**: cada cidade tem um dinheiro próprio (o **Banco da Cidade**), dado no início do jogo. A cidade distribui uma porcentagem desse valor entre os NPCs, conforme a classe social (igual entre os NPCs da mesma classe); a outra parte é distribuída igualmente entre as lojas.

**Contra a economia fixa** (o total de dinheiro nunca muda, e o jogador poderia concentrar todo o dinheiro do mundo):

- **Fontes:** a **cunhagem** (ganho anual na Casa da Moeda) e **tecnologias** da facção que aumentam o dinheiro total das cidades.
- **Sumidouros:** **impostos** periódicos da cidade e da facção, e perdas em eventos negativos (saques, guerra), evitando inflação descontrolada numa partida longa.

### Ciclo do dinheiro

O dinheiro circula num ciclo: **os consumidores gastam nas lojas, as lojas pagam salários e compram insumos, e os trabalhadores gastam de novo**. A distribuição rotativa vira o **estabilizador** desse ciclo: a cidade só distribui dinheiro quando a média por classe cai abaixo de um limiar. O jogo guarda a **riqueza média por classe e por cidade**, não o dinheiro de cada NPC. Os salários, os donos das construções, o imposto único sobre receita e o preço nas lojas do jogador estão no [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]].

### Atributos exigidos nos itens

Valor Global, Valor Facção, Valor Local, Proibido, Categoria e Dono (ver [[Sistema de Itens/Descrição|Itens]]).

---

## Parte técnica

- **Preço:** o Mercado Mundial é uma tabela por item; o da Facção, uma porcentagem por categoria e por item proibido; o Local, um ajuste por oferta e demanda por item e cidade. O preço final é calculado na hora, não guardado em cada item.
- **Frequência:** o Mercado Local por dia e a cada compra do jogador; os outros só por evento.
- **LOD:** em LOD 2 o Mercado Local é atualizado por estação, junto com a produção e o consumo agregados.
- **Save:** Banco de cada cidade, dinheiro das lojas, riqueza média por classe, modificadores ativos dos Mercados Mundial e da Facção, e o ajuste local de cada item por cidade.
- **Fatia vertical:** um Mercado Local, uma loja da cidade, o Banco da Cidade e o ciclo do dinheiro.

## Ligação com outros sistemas

| Sistema | O que o Econômico fornece ou recebe |
|---|---|
| Facção | Impostos para o Banco da Facção; soldo e obras devolvem dinheiro às cidades; PIB = soma do dinheiro das cidades; ajustes de preço e proibições pelas leis |
| Trabalho e Propriedade | Salários, donos, imposto sobre receita, preço nas lojas do jogador |
| Inventário | Inventário único da cidade, com Dono e Categoria |
| Crime e Justiça | Mercado negro: itens proibidos só em lojas ilegais, com preço multiplicado; multas e confiscos viram dinheiro da facção |
| Reputação | Preço do jogador de −10% (Aliado) a +15% (Hostil); com Inimigo, as lojas não negociam ([[Sistema de Reputação/Descrição\|Reputação]]) |
| Construção | Casa da Moeda e Mercado |
| Comércio | Caravanas civis sempre reais; rota pelo lucro esperado; Tarifa; o dinheiro passa de uma cidade para outra ([[Sistema de Comércio/Descrição\|Comércio]]) |
| Eventos | O efeito "modificador de preço" recalcula o Mercado Mundial ou da Facção; quando a alta já nasce da simulação (guerra consumindo Ração Militar, comida no inverno), o evento só explica a causa ([[Sistema de Eventos/Descrição\|Eventos]]) |
| Notícias | Preços 30% fora da média viram notícia ([[Sistema de Notícias/Descrição\|Notícias]]) |
| Escravidão | Mercado de Escravos com oferta e procura ([[Sistema de Escravidão/Descrição\|Escravidão]]) |

## Perguntas em aberto

1. **Moeda e preços base** de cada item no Mercado Mundial (rodada V2 da [[Revisão Completa]]).
2. **Banco inicial** de cada cidade, limiar do estabilizador e valor da cunhagem (rodada V2).
3. **Fórmula da oferta e demanda** no Mercado Local (quanto o preço sobe com a falta).
