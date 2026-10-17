> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> **Receitas de todos os produtos**, decididas na rodada IP3 (2026-10-17). As regras gerais estão no [[Sistema de Produção/Descrição|Sistema de Produção]] e a lista dos itens no [[Sistema de Itens/Descrição|Sistema de Itens]]. As quantidades são iniciais: a rodada V2 ajusta os números ao calcular os preços.

## Receitas

**Descrição geral**

Cada produto tem **uma receita fixa** (IP1-1), feita numa única construção (IP1-2). Este documento lista todas, construção por construção. A tabela que mostra o que cada nível de cada construção libera está em [[Sistema de Produção/Funções e Construções|Funções e Construções]].

## Regras de quantidade (IP3-1)

1. **Lote pelo Tier:**

| Tier do produto | Lote (unidades por ciclo) | Horas do ciclo |
|---|---|---|
| 1 | 4 | 4 |
| 2 | 2 | 4 |
| 3 | 1 | 4 |
| 4 | 1 | 8 |

2. **Itens com Rank:** 1 por ciclo; as horas seguem o degrau do material:
   - 8 h para Ferro, Cobre, Madeira e Couro;
   - 12 h para Aço, Composto e Cravejado;
   - 24 h para Aço Mágico e Arcano.

   A armadura faz as 3 peças juntas e leva 3 vezes isso (24, 36 ou 72 h). A artilharia leva 3 vezes o tempo de uma arma.
3. **"3 insumos" são 3 tipos de item**, em qualquer quantidade.
4. **Nível mínimo:**
   - nos bens, pelo Tier do produto: Tier 1 no nível 1, Tier 2 no 2, Tier 3 no 4 e Tier 4 no 5;
   - nos itens com Rank, pelo degrau do material: degrau 3 no nível 1; degrau 4 no nível 2; degrau 5 no nível 5.

   Nas joias de ouro conta o Tier da Barra de Ouro (nível 4); a Joia de Mana fica no nível 3.
5. **Era (IP3-9):**
   - a era trava só os **materiais de equipamento e os itens com Rank**: Aço e degrau 4 na Era 2; Pólvora, Arcabuz e Bombarda na Era 2; Mosquete, Carabina e Canhão de Aço na Era 3; Aço Mágico, Arcano e Fuzil Raiado na Era 4;
   - os **bens de consumo** de qualquer Tier (Vinho, Joias, Licor...) e a Barra de Ouro existem desde a Era 1, travados só pelo nível da construção;
   - o teto de Rank por era continua (3, 4, 4, 5).
6. **Munição em lote:** 20 Flechas, 10 Virotes ou 20 Cartuchos por ciclo de 4 h. A Bala de Canhão sai 1 por ciclo, porque 1 unidade é o suprimento de uma batalha (30 tiros).
7. **Exceção da Forja Arcana:** faz a Barra de Aço Mágico desde o nível 1; a trava é só a era (Era 4).
8. **Habilidade que conta (IP3-11):**
   - Trabalho **Manufaturado** nos bens de Tier 1 e 2 e nos itens de degrau 3;
   - Trabalho **Especialista** nos bens de Tier 3 e 4 e nos itens de degrau 4 e 5;
   - Trabalho **Braçal** nas construções que tiram do mapa.

   O Armeiro, o Artesão, o Boticário e a Forja Arcana são sempre Especialistas.

**Habilidade mínima (IP1-1):** 10 no nível 1, 25 no nível 2, 40 no nível 3, 55 no nível 4 e 70 no nível 5 da receita.

---

## Construções que tiram do mapa (IP3-2)

### Fazenda, Posto de Coleta e Pesqueiro

- **Sem insumos.** 1 unidade a cada 0,5 h de trabalho (Tier 0), 1 h (Tier 1: Minério de Ouro, Fruta, Flor Espiritual, Pedra de Mana) ou 2 h (Tier 2: Especiarias).
- O resultado é multiplicado pela produtividade do recurso no mapa e pela estação ([[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Adubo:** insumo **opcional** da Fazenda. Com Adubo no estoque, ela gasta 1 por ciclo e rende +25%.

### Pasto

Sem insumo (o pasto é o capim do mapa). Precisa de **2 animais da mesma espécie no estoque**, que não são gastos, e faz 1 animal novo por ciclo:

| Animal | Horas por ciclo |
|---|---|
| Galinha | 4 |
| Porco | 8 |
| Ovelha | 8 |
| Vaca | 16 |
| Cavalo | 24 |

### Rancho

Cada animal no estoque permite **1 ciclo por dia**, de 2 h. Os animais não são gastos.

| Animal | Produto | Subproduto |
|---|---|---|
| Galinha | 4 Ovo | — |
| Ovelha | 2 Lã | 1 Dejetos Animais |
| Vaca | 4 Leite | 2 Dejetos Animais |
| Porco | — | 2 Dejetos Animais |

### Açougue

Gasta o animal; cada ciclo leva 4 h. É a **única exceção** à "uma receita por produto": há uma receita por animal.

| Animal | Saídas |
|---|---|
| 1 Galinha | 2 Carne Crua |
| 1 Porco | 6 Carne Crua, 1 Couro Cru, 1 Ossos |
| 1 Ovelha | 4 Carne Crua, 1 Couro Cru, 1 Ossos |
| 1 Vaca | 8 Carne Crua, 2 Couro Cru, 2 Ossos |

O Cavalo não vai para o Açougue: é montaria e carga.

---

## Metalurgia, Forja Arcana e Pedreira (IP3-3)

| Construção | Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|---|
| Metalurgia | Barra de Ferro | 4 | 4 Minério de Ferro + 2 Carvão | 1 | 1 | 4 |
| Metalurgia | Barra de Cobre | 4 | 4 Minério de Cobre + 1 Carvão | 1 | 1 | 4 |
| Metalurgia | Barra de Aço | 2 | 2 Barra de Ferro + 2 Carvão | 2 | 2 | 4 |
| Metalurgia | Barra de Ouro | 1 | 3 Minério de Ouro + 1 Carvão | 4 | 1 | 4 |
| Forja Arcana | Barra de Aço Mágico | 1 | 1 Barra de Aço + 1 Pedra de Mana | 1 (exceção) | 4 | 8 |
| Pedreira | Bloco de Pedra | 4 | 4 Pedra | 1 | 1 | 4 |
| Pedreira | Cerâmica | 4 | 3 Argila + 1 Tora (lenha do forno) | 1 | 1 | 4 |

**Cadeia mais longa do jogo**, exatamente no limite de 4 passos: Minério de Ferro → Barra de Ferro → Barra de Aço → Barra de Aço Mágico → Espada.

## Carpinteiro (IP3-4)

| Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|
| Tábua | 4 | 2 Tora | 1 | 1 | 4 |
| Móveis de Madeira | 2 | 3 Tábua | 2 | 1 | 4 |
| Móveis Refinados | 1 | 2 Tábua + 1 Tecido de Algodão + 1 Barra de Cobre | 4 | 1 | 4 |
| Flecha | 20 | 2 Tábua + 1 Barra de Ferro | 1 | 1 | 4 |
| Virote | 10 | 2 Tábua + 1 Barra de Ferro | 1 | 1 | 4 |

**Moldes** (1 por ciclo; horas, nível e era pelo degrau):

| Molde | de Madeira (degrau 3) | Composto (degrau 4) | Arcano (degrau 5) |
|---|---|---|---|
| Arco Curto | 1 Tábua + 1 Tecido de Lã (corda) | 1 Tábua + 1 Ossos + 1 Couro Refinado | 1 Tábua + 1 Pedra de Mana + 1 Tecido de Lã |
| Arco Longo | 2 Tábua + 1 Tecido de Lã | 2 Tábua + 1 Ossos + 1 Couro Refinado | 2 Tábua + 1 Pedra de Mana + 1 Tecido de Lã |
| Escudo de Madeira | 2 Tábua + 1 Couro Refinado | — | — |
| Cajado Simples de Madeira | 2 Tábua + 1 Pedra de Mana | — | — |

| Molde (besta) | de Madeira | de Aço | de Aço Mágico |
|---|---|---|---|
| Besta Leve | 2 Tábua + 1 Barra de Ferro | 1 Tábua + 2 Barra de Aço | 1 Tábua + 2 Barra de Aço Mágico |
| Besta Pesada | — | 2 Tábua + 3 Barra de Aço | 2 Tábua + 3 Barra de Aço Mágico |
| Besta de Repetição | — | 2 Tábua + 2 Barra de Aço + 1 Barra de Cobre | 2 Tábua + 2 Barra de Aço Mágico + 1 Barra de Cobre |

## Ferreiro (IP3-5)

**Moldes.** "B" é a barra do material: Barra de Ferro (nível 1, 8 h), Barra de Aço (nível 2, Era 2, 12 h) ou Barra de Aço Mágico (nível 5, Era 4, 24 h).

| Molde | Insumos |
|---|---|
| Espada Curta, Lança Curta, Machado Curto, Clava Curta | 1 B + 1 Tábua (cabo) |
| Espada Longa, Machado Longo, Clava Longa | 2 B + 1 Tábua |
| Lança Longa | 1 B + 2 Tábua (haste) |
| Escudo de metal | 2 B + 1 Tábua |
| Armadura Média (Coifa, Cota e Perneiras de Malha) | 6 B + 1 Tecido de Lã (acolchoado) |
| Armadura Pesada (Elmo, Couraça e Grevas) | 8 B + 2 Couro Refinado (forro e correias) |

| Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|
| Grilhões (Tier 2) | 2 | 2 Barra de Ferro | 2 | 1 | 4 |
| Chapéu de Mineiro (1-2) | 1 | 1 Barra de Ferro + 1 Couro Refinado | 1 | 1 | 8 |

*Exemplo:* um kit de Infantaria Pesada de Aço leva 12 Barras de Aço:

- Espada Longa: 2;
- armadura Pesada: 8;
- Escudo: 2.

Essas 12 Barras de Aço saem de 24 Barras de Ferro + 24 Carvão.

## Armeiro (IP3-6)

O cobre entra nas peças de latão (gatilho e guarnições). "B" é a barra do material.

| Produto | Insumos | Nível | Era | Horas |
|---|---|---|---|---|
| Arcabuz de Ferro | 2 Barra de Ferro + 1 Tábua + 1 Barra de Cobre | 1 | 2 | 8 |
| Mosquete (Aço ou Aço Mágico) | 2 B + 1 Tábua + 1 Barra de Cobre | 2 ou 5 | 3 ou 4 | 12 ou 24 |
| Carabina (Aço ou Aço Mágico) | 1 B + 1 Tábua + 1 Barra de Cobre | 2 ou 5 | 3 ou 4 | 12 ou 24 |
| Fuzil Raiado de Aço Mágico | 3 Barra de Aço Mágico + 1 Tábua + 1 Barra de Cobre | 5 | 4 | 24 |
| Cartucho (lote 20) | 1 Barra de Ferro + 2 Pólvora + 1 Tecido de Algodão (o papel do cartucho) | 1 | 2 | 4 |
| Bombarda de Ferro | 8 Barra de Ferro + 2 Tábua | 1 | 2 | 24 |
| Canhão (Aço ou Aço Mágico) | 8 B + 2 Tábua + 2 Barra de Cobre | 2 ou 5 | 3 ou 4 | 36 ou 72 |
| Bala de Canhão (1 por batalha) | 2 Barra de Ferro + 3 Pólvora | 1 | 2 | 4 |

## Costureiro (IP3-7)

| Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|
| Tecido de Algodão | 4 | 4 Algodão | 1 | 1 | 4 |
| Tecido de Lã | 4 | 4 Lã | 1 | 1 | 4 |
| Couro Refinado | 4 | 4 Couro Cru + 2 Água | 1 | 1 | 4 |
| Roupa Comum | 4 | 2 Tecido de Lã | 1 | 1 | 4 |
| Roupa Fina | 1 | 2 Tecido de Algodão + 1 Especiarias (açafrão e outras tinturas) | 4 | 1 | 4 |
| Sela | 2 | 3 Couro Refinado + 1 Barra de Ferro (fivelas) | 2 | 1 | 4 |

**Roupas equipáveis** (1 peça por ciclo, de 4 h; 2 h no conjunto Pobre):

- **Molde:** chapéu ou capuz = 1 tecido; camisa ou túnica = 2; calças = 2. O tecido é o do conjunto ([[Sistema de Itens/Descrição|Itens]]).
- **Mercador e Nobre:** gastam também 1 Especiarias (a tintura) e saem no nível 2. Os outros conjuntos saem no nível 1.

**Armadura Leve** (3 peças por ciclo):

| Material | Insumos | Nível | Era | Horas |
|---|---|---|---|---|
| de Couro | 6 Couro Refinado | 1 | 1 | 24 |
| Cravejado | 5 Couro Refinado + 2 Barra de Aço | 2 | 2 | 36 |
| Arcano | 5 Couro Refinado + 1 Pedra de Mana | 5 | 4 | 72 |

## Artesão (IP3-8)

| Produto | Insumos | Nível | Era | Horas |
|---|---|---|---|---|
| Joias (Tier 4) | 2 Barra de Ouro | 5 | 1 | 8 |
| Ornamento (Tier 4) | 1 Barra de Ouro + 2 Barra de Cobre | 5 | 1 | 8 |
| Coroa (1-5) | 3 Barra de Ouro | 4 | 1 | 24 |
| Anel de Cobre (1-3) | 1 Barra de Cobre | 1 | 1 | 8 |
| Anel de Ouro (1-5) | 1 Barra de Ouro | 4 | 1 | 8 |
| Anel de Mana (1-5) | 1 Barra de Ouro + 1 Pedra de Mana | 4 | 1 | 12 |
| Joia de Mana (1-5) | 1 Barra de Cobre + 2 Pedra de Mana | 3 | 1 | 12 |
| Cajado Simples de Cobre (1-3) | 2 Barra de Cobre + 1 Pedra de Mana | 1 | 1 | 8 |
| Cajado Arcano (Aço ou Aço Mágico) | 2 B + 1 Tábua + 1 Pedra de Mana | 2 ou 5 | 2 ou 4 | 12 ou 24 |
| Cetro de Aço Mágico (1-4) | 1 Barra de Aço Mágico + 1 Pedra de Mana + 1 Barra de Ouro | 5 | 4 | 24 |

O Rank dos anéis, da Coroa e das joias de mana continua limitado pelo nível e pela era (até 3 na Era 1).

## Cozinheiro e Destilaria (IP3-9)

| Construção | Produto | Lote | Insumos | Nível | Horas |
|---|---|---|---|---|---|
| Cozinheiro | Pão | 4 | 2 Grão + 1 Água | 1 | 4 |
| Cozinheiro | Sopa | 4 | 2 Legumes + 1 Água | 1 | 4 |
| Cozinheiro | Peixe Defumado | 4 | 3 Peixe + 1 Tora | 1 | 4 |
| Cozinheiro | Ração Militar | 4 | 2 Grão + 1 Carne Crua | 1 | 4 |
| Cozinheiro | Torta de Fruta | 2 | 1 Fruta + 1 Grão + 1 Ovo | 2 | 4 |
| Cozinheiro | Linguiça | 2 | 2 Carne Crua + 1 Especiarias | 2 | 4 |
| Cozinheiro | Queijo | 2 | 3 Leite | 2 | 4 |
| Cozinheiro | Assado Temperado | 1 | 2 Carne Crua + 1 Especiarias + 1 Tora | 4 | 4 |
| Cozinheiro | Presunto Curado | 1 | 3 Carne Crua + 1 Especiarias | 5 | 8 |
| Cozinheiro | Doce de Especiarias | 1 | 2 Fruta + 1 Especiarias + 1 Leite | 5 | 8 |
| Destilaria | Cerveja | 2 | 2 Grão + 1 Água | 2 | 4 |
| Destilaria | Vinho | 1 | 3 Fruta | 4 | 4 |
| Destilaria | Licor | 1 | 1 Vinho + 1 Especiarias | 5 | 8 |
| Destilaria | Perfume | 1 | 1 Flor Espiritual + 1 Especiarias + 1 Água | 4 | 4 |

Todos são da Era 1 (IP3-9).

## Misturador e Boticário (IP3-10)

| Construção | Produto | Lote | Insumos | Nível | Era | Horas |
|---|---|---|---|---|---|---|
| Misturador | Adubo | 4 | 3 Dejetos Animais + 1 Ossos (farinha de osso) | 1 | 1 | 4 |
| Misturador | Pólvora | 2 | 2 Carvão + 2 Dejetos Animais (o salitre vinha do esterco curtido) | 2 | 2 | 4 |
| Misturador | Fumo | 2 | 2 Folha de Fumo | 2 | 1 | 4 |
| Misturador | Pó Espiritual | 1 | 3 Flor Espiritual | 4 | 1 | 4 |
| Boticário | Curativo | 4 | 1 Tecido de Algodão + 1 Água | 1 | 1 | 4 |
| Boticário | Remédio | 1 | 1 Flor Espiritual + 1 Especiarias | 4 | 1 | 4 |

---

## Conferência

**Cadeias:** a mais longa tem 4 passos (Espada, armadura e Cajado Arcano de Aço Mágico). As outras ficam em 1 a 3 passos, por exemplo Fruta → Vinho → Licor.

**Nenhum item órfão.** Cada intermediário tem pelo menos um uso:

| Item | Usado em |
|---|---|
| Ossos | Arco Composto, Adubo |
| Dejetos Animais | Adubo, Pólvora |
| Ovo | Torta de Fruta |
| Leite | Queijo, Doce de Especiarias |
| Lã | Tecido de Lã |
| Couro Cru | Couro Refinado |
| Carvão | Metalurgia, Pólvora |
| Pedra de Mana | Aço Mágico, joias e cajados de mana, itens Arcanos, kit dos Magos, encantamento |
| Adubo | Fazenda (opcional) |
| Pólvora | Cartucho, Bala de Canhão |
| Tábua e Bloco de Pedra | Obras (custos na V2), móveis, armas |
| Barra de Cobre | Armas de fogo, Móveis Refinados, Ornamento, anéis, bestas de repetição |
| Tecido de Lã | Roupa Comum, Cota de Malha, arcos, roupas Pobre e Furtiva |
| Tecido de Algodão | Curativo, Cartucho, Móveis Refinados, Roupa Fina, roupas |

**A Destilaria não tem produto no nível 1:** a Cerveja é de Tier 2. Por isso a Destilaria é construída **já no nível 2**, uma exceção à escada como a da Forja Arcana (confirmado na rodada V3-13).

---

## Parte técnica

- **Dados:** uma linha por receita no CSV de receitas ([[Sistema de Produção/Descrição|Produção]]). Os moldes são escritos uma vez e expandidos ao carregar (um molde × três materiais = três receitas), para o autor não repetir linhas.
- **Campos novos:**
  - **"Exige no estoque (não gasta)"**, opcional: os animais do Pasto e do Rancho (IP3-2);
  - **"Insumo opcional"**, opcional: o Adubo da Fazenda, com o bônus.
- **Validação:** o `jogo --validar` confere os limites (3 tipos de insumo, 3 saídas, 4 passos), os órfãos, o nível pela escada e a era.
- **Fatia vertical:** Pão (2 Grão + 1 Água → 4 Pão, 4 h, Cozinheiro nível 1) e, se couber, Cerâmica ou Móveis de Madeira.

## Ligação com outros sistemas

| Sistema | O que as Receitas fornecem ou recebem |
|---|---|
| Produção | Regras da receita, escada de níveis, Rank do produto, tempo |
| Funções e Construções | O que cada nível de cada construção libera |
| Itens | Os produtos e insumos da lista final |
| Tecnologia | A era de cada material de equipamento |
| Batalha e Guerra | Custo dos kits, da munição e da Ração Militar |
| Econômico | Preço base ≈ custo dos insumos + horas × salário (V2) |
| Trabalho | Habilidade usada em cada receita (Braçal, Manufaturado ou Especialista) |

## Perguntas em aberto

1. **Números finais:** a V2 ajusta lotes, horas e quantidades junto com os preços.
2. **Destilaria no nível 2:** confirmada na V3-13.
3. **Valores a testar:** habilidade mínima por nível, rendimento do Adubo, ciclos do Pasto e do Rancho.
