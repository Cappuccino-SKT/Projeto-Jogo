
## Sistema Econômico

**Descrição geral**

Dividido em 3 partes, cada uma influenciando a parte menor — isso permite preços variados dependendo da região do mapa.

- **Mercado Mundial** — atribui o valor primário de todos os produtos; é o primeiro a definir um valor para os itens.
- **Mercado da Facção** — recebe o valor do Mercado Mundial e atribui um novo valor em porcentagem (aumentando ou diminuindo o valor base). Válido apenas dentro dos Mapas Locais dominados por essa facção.
- **Mercado Local** — recebe o preço alterado pelo Mercado da Facção e o ajusta novamente. Esse é o valor final, usado pelo jogador na compra ou venda de um item.

### Critérios de alteração de preço

- **Mercado Mundial** — Eventos Climáticos, Descoberta de Tecnologia, Eventos Políticos.
- **Mercado da Facção** — Lei de Proibição (se o item for proibido na facção), Eventos Exclusivos da Facção.
- **Mercado Local** — Oferta e Demanda.

O Mercado Mundial é recalculado no início do jogo e sempre que ocorre um evento mundial capaz de alterar o preço de um produto. O Mercado da Facção é recalculado no início do jogo e sempre que ocorre um evento interno à facção capaz de alterar preços. O Mercado Local é atualizado a cada dia do jogo, para acompanhar o preço dinâmico, e também é recalculado quando o jogador compra ou vende um produto.

### Loja

Cada construção comercial funciona como uma **Loja**, com atributos próprios: **Dono** (cidade ou jogador), **Categoria vendida** (liga com a Categoria dos Itens), **Dinheiro próprio** (usado pra pagar o jogador ao vender itens a ela) e **Construção associada** (referência ao Mapa Interno do tipo Construção Pública onde ela opera).

### Armazenamento de itens no Mercado

Cada cidade possui um Mercado Local e um inventário próprio, que contabiliza todos os itens dentro das lojas presentes na cidade. Esse inventário é compartilhado por todas as lojas da cidade — o que simplifica o sistema de logística, evitando um inventário separado por construção.

Todos os itens armazenados dentro das construções estão, na prática, dentro de um único inventário compartilhado — o inventário da cidade. Para evitar que itens do jogador entrem em conflito com os da cidade (por exemplo, o jogador usar por engano materiais da cidade em sua própria produção), cada item possui um atributo de **Dono**. Além disso, os itens possuem um atributo de **Categoria**, que permite exibi-los nas lojas correspondentes — permitindo que o jogador frequente lojas diferentes para comprar materiais diferentes.

### Compra e venda por NPCs Não-Controlados

Os NPCs Não-Controlados têm rotinas que incluem atender sua demanda de consumo, frequentando lojas dentro da cidade — tanto lojas da própria cidade quanto do jogador.

- Ao comprar de uma loja do jogador, o NPC muda o dono do item (do jogador para a cidade) dentro do inventário da cidade; em troca, o jogador recebe o valor do item.
- Ao comprar de uma loja da cidade, o valor do item vai para o dinheiro da loja, e nada muda no inventário (o item já pertencia à cidade).
- Quando o jogador compra um item de uma loja, além de o item mudar de dono, ele sai do inventário da cidade e vai para o inventário do NPC Controlado. Se o jogador posteriormente guardar esse item em sua própria loja, ele retorna ao inventário da cidade.

### Distribuição de dinheiro

Para que NPCs Não-Controlados e lojas possuam dinheiro, o jogo terá um sistema de distribuição rotativa: cada cidade possui um dinheiro próprio (o "banco da cidade"), atribuído no início do jogo. A cidade distribui uma porcentagem desse valor entre todos os NPCs, variando conforme a classe social — a distribuição é igual entre NPCs da mesma classe. A outra parte é reservada e distribuída igualmente entre as lojas da cidade.

Para evitar o problema de uma economia fixa (onde a quantidade total de dinheiro nunca muda, e o jogador poderia acabar concentrando todo o dinheiro do mundo), haverá métodos para aumentar a quantia total de dinheiro das cidades — principalmente a **cunhagem** (um ganho anual de dinheiro) e **tecnologias** desbloqueadas pela facção que aumentam o dinheiro total das cidades. Como contrapeso, impostos periódicos cobrados pela cidade/facção — ou perda de dinheiro em eventos negativos (saques, guerra) — funcionam como sumidouro, evitando inflação descontrolada ao longo de uma partida longa.

### Atributos exigidos pelo sistema econômico (nos itens)

- Valor Global
- Valor Facção
- Valor Local
- Proibido
- Categoria
- Dono

### Atualizações recentes

- **Banco da Facção e PIB:** os impostos vão para o Banco da Facção, e o soldo dos Esquadrões e as obras devolvem dinheiro às cidades. O PIB da facção é a soma do dinheiro das cidades (ver Sistema de Facção).
- **Proibição:** deriva das leis da facção. Os ajustes de preço por categoria do Mercado da Facção também vêm das leis.
- **Mercado negro:** itens proibidos só são vendidos em lojas ilegais (qualquer construção de produção), com preço multiplicado (ver Sistema de Crime e Justiça).
- **Multas e confiscos:** viram dinheiro da facção.
- **Casa da Moeda e Mercado:** novas construções (ver Sistema de Construção).
- **Caravanas civis:** o comércio entre cidades será tratado aqui, não necessariamente com o mesmo sistema da Caravana Militar. A definir.
- **Eventos:** o efeito "modificador de preço" de um evento (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]) dispara o recálculo do Mercado Mundial ou da Facção. Quando a alta de preço já nasce da simulação (por exemplo, guerra consumindo Ração Militar), o evento só registra e explica a causa.
