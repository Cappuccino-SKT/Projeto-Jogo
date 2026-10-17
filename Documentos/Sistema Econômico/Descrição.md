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

### Moeda (rodada V2-1)

- Uma unidade só, a **moeda**, com **1 casa decimal** na interface (o Pão custa 1,7).
- Por dentro, o dinheiro é guardado em **décimos inteiros** (1,7 vira 17). Isso evita os erros de arredondamento que fariam o dinheiro "vazar" numa partida longa.
- O nome próprio da moeda de cada cultura fica para a história.

### Preço base (rodada V2-4)

O Valor Global de cada item, no Mercado Mundial, sai de uma **fórmula** aplicada à receita:

> **preço base = (custo dos insumos + horas × salário por hora da receita) ÷ lote × 1,2**

- O 1,2 é a **margem do dono** (20%). Os salários por hora estão no [[Sistema de Trabalho e Propriedade/Descrição|Trabalho]].
- O jogo calcula os preços ao carregar os dados, na ordem da cadeia: primeiro os brutos, depois o que usa os brutos. Mudar uma receita muda o preço sozinho.
- **Rank:** o preço do Rank 1 é multiplicado por 1 (Rank 1), 1,5 (2), 2,2 (3), 3,2 (4) e 4,5 (5); o Rank 0 vale metade.
- **Multiplicador manual** opcional por item, para os raros: o Cavalo tem × 3.

**Preços de partida** (em moedas):

| Grupo | Item | Preço |
|---|---|---|
| Bruto Tier 0 | Grão, Legumes, Água, Pedra, Argila, Carvão, Tora, Minérios de Ferro e Cobre, Algodão, Folha de Fumo, Ovo, Leite | 0,5 |
| Bruto Tier 1 e 2 | Fruta, Flor Espiritual, Pedra de Mana, Minério de Ouro / Especiarias | 0,9 / 1,8 |
| Açougue | Carne Crua, Couro Cru, Ossos | 1,7 |
| Animais | Galinha / Porco e Ovelha / Vaca / Cavalo (× 3) | 3,6 / 7,2 / 14,4 / 65 |
| Intermediários | Tábua 1,5; Bloco de Pedra 1,8; Barra de Ferro 2,1; Barra de Cobre 2,0; Couro Refinado 3,5; Pólvora 3,6; Barra de Aço 5,5; Barra de Ouro 11,0; Barra de Aço Mágico 22,1 | — |
| Comida Tier 1 | Pão, Sopa 1,7; Peixe Defumado 1,8; Ração Militar 2,0 | — |
| Comida e bebida Tier 2 | Queijo, Cerveja 3,3; Torta de Fruta 3,5; Linguiça 5,5 | — |
| Tier 3 | Vinho, Pó Espiritual, Remédio 10,4; Perfume 11,0; Roupa Fina 13,7; Assado Temperado 14,0; Móveis Refinados 15,4 | — |
| Tier 4 | Doce de Especiarias 19,3; Presunto Curado 22,7; Licor 29,0; Ornamento 32,4; Joias 40,8 | — |
| Materiais básicos | Cerâmica 1,8; Roupa Comum 2,3; Móveis de Madeira 5,1 | — |
| Armas (Rank 1) | Flecha 0,5; Cartucho 1,0; Espada Curta de Ferro 13,9; Arco Longo de Madeira 15,5; Espada Longa de Ferro 16,4; Arcabuz 23,6; Espada Curta de Aço 30; de Aço Mágico 71,5 | — |
| Armaduras (Rank 1, 3 peças) | Leve de Couro 54; Pesada de Ferro 57; Pesada de Aço 126 | — |
| Artilharia | Bala de Canhão 25; Bombarda 67 | — |

*Exemplo:* um kit de Infantaria Pesada de Aço de Rank 3 custa cerca de 200 × 2,2 = 440 moedas, o mesmo que um escravo bom ou 55 dias de soldo.

### Mercado da Facção e Mercado Local (rodada V2-5)

- **Mercado da Facção:** preço mundial × ajuste da lei por categoria (−20%, 0 ou +20%). Os itens proibidos só aparecem nas lojas ilegais, a × 2.
- **Mercado Local:**
  - **estoque-alvo** = 5 dias do consumo da cidade, somando as classes e as receitas que usam o item;
  - **fator** = 1 + (alvo − estoque) ÷ alvo, limitado entre 0,5 e 2,0;
  - o preço anda no máximo **10% por dia** na direção do fator.
- **Lojas da Cidade:** compram a 90% e vendem a 110% do preço local.
- *Exemplo:* uma cidade de 100 NPCs come 100 Pães por dia e tem alvo de 500. Com 250 no estoque, o fator é 1,5, e o Pão vai de 1,7 a 2,6 em cerca de 5 dias. Uma caravana com 400 Pães derruba o preço.

### Estabilizador, imposto e cunhagem (rodada V2-7)

- **Estabilizador:** quando a riqueza média de uma classe cai abaixo de **3 dias da cesta mínima** ([[Sistema de Classe Social/Descrição|Classe Social]]), o Banco da Cidade paga até ela voltar a 5 dias. O teto é 10% do Banco por estação.
- **Imposto** (lei da facção, sobre salários e lucros, por estação):

| Nível | Taxa | Felicidade |
|---|---|---|
| Baixo | 10% | 0 |
| Normal | 20% | −5 |
| Alto | 30% | −10 |

  A cidade fica com 30% do imposto, e o Banco da Facção com 70%.
- **Cunhagem:** a Casa da Moeda transforma **1 Barra de Ouro em 25 moedas** (a barra vale 11). Ela cunha até 4 × o nível da Casa barras por estação. Uma Casa nível 2 põe 200 moedas novas por estação no mundo.

### Atributos exigidos nos itens

Valor Global, Valor Facção, Valor Local, Proibido, Categoria e Dono (ver [[Sistema de Itens/Descrição|Itens]]).

---

## Parte técnica

- **Dinheiro:** inteiros em décimos de moeda (V2-1).
- **Preço:** o Mercado Mundial é uma tabela por item, calculada pela fórmula da V2-4 ao carregar os dados; o da Facção, uma porcentagem por categoria e por item proibido; o Local, um ajuste por oferta e demanda por item e cidade. O preço final é calculado na hora, não guardado em cada item.
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

1. **Valores a testar:** todos os números da V2 (preços, fator do Mercado Local, estabilizador, imposto, cunhagem), no modo sem tela da fatia vertical.
