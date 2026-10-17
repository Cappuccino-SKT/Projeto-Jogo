> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> **Lista final dos itens:** decidida na rodada IP2 (2026-10-17). A forma de produzir foi padronizada na IP1 (ver [[Sistema de Produção/Descrição|Produção]]); os insumos de cada produto estão nas [[Sistema de Produção/Receitas|Receitas]] (IP3), e os preços vêm na V2.

## Sistema de Itens

**Descrição geral**

Os itens são divididos em **categorias**, cada uma com uma função dentro do jogo. Os itens têm duas escalas distintas:

- **Rank** (0 a 5, seis níveis): usado em armas, equipamentos e artefatos. Cada Rank acima de 0 dá um bônus percentual no Atributo de Habilidade relevante ao equipar (por exemplo, uma espada Rank 3 aumenta um pouco mais o Corpo a Corpo do que uma Rank 1) e/ou passa a exigir uma habilidade mínima para ser usado com eficácia total.
- **Tier** (0 a 4): usado em materiais e bens de consumo, ligado à complexidade da produção e ao tipo de Trabalho exigido (Braçal, Manufaturado ou Especialista). O Tier também define **qual classe consome** o bem (ver "Consumo por Tier").

Materiais e bens de consumo não têm Rank, e armas, equipamentos e artefatos não têm Tier.

O que os itens especiais fazem ao serem usados (livros, joias, artefatos, medicamentos, comida, bebida e narcóticos) está em [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]].

### Cultura

A cultura é um atributo das armas e dos equipamentos, inspirado nos **Estilos** do RimWorld: um único item pode ter mais de uma aparência, conforme a cultura que o produziu. Em vez de criar diversas armas e equipamentos e depois ter que balancear todos, o jogo cria os itens padrão e só atribui uma **textura diferente** conforme a cultura que o produziu ([[Sistema de Cultura/Descrição|Cultura]], [[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]). A cultura nunca muda o nome nem os atributos.

### Atributos (todos os itens)

- **Valor Global** — valor dado pelo Mercado Mundial: o preço base, calculado pela receita (rodada V2-4; ver [[Sistema Econômico/Descrição|Econômico]])
- **Valor Facção** — valor dado pelo Mercado da Facção
- **Valor Local** — valor dado pelo Mercado Local
- **Descrição**
- **Proibição** — em quais facções o item é proibido
- **Dono** — quem tem a posse legal do item
- **Categoria** — agrupa itens parecidos
- **Nome**
- **Rank** (armas, equipamentos e artefatos, de 0 a 5)
- **Tier** (materiais e bens de consumo, de 0 a 4)
- **Peso** — usado pela Capacidade de peso do NPC
- **Tipo** — derivado da Categoria (ver abaixo)
- **Textura**

### Atributos por tipo

| Tipo | Atributos |
|---|---|
| Arma corpo a corpo | Dano mínimo e máximo, Intervalo de Ataque, Alcance, Atributo Mágico, Atributo Mínimo, Cultura |
| Arma de longo alcance | Intervalo de Ataque, Alcance máximo, Munição, Atributo Mínimo, Cultura |
| Munição | Dano mínimo e máximo, Atributo Mágico |
| Cajado mágico | Dano mínimo e máximo, Alcance máximo, Mana (quanto gasta por disparo), Intervalo de Ataque, Atributo Mágico, Atributo Mínimo, Cultura |
| Comida e Bebida | Saciedade (quanto da Fome ela enche; ver [[Sistema de NPC/Descrição\|NPC]]) |
| Equipamento | Defesa, Lugar (capacete, peitoral, perneiras, chapéu, camisa, calças, anel, escudo), Atributo Mágico, Buff de habilidade, Cultura |

### Categorias e tipos

| Categoria | Tipos |
|---|---|
| Matéria-Prima | Recurso Bruto (Mineral, Vegetal, Animal), Recurso Manufaturado |
| Bem de Consumo | Alimento, Bebida, Mobília, Vestuário, Essência e Joia, Narcótico, Medicamento |
| Arma | Espada, Lança, Machado, Clava, Arco, Besta, Arma de fogo, Cajado, Munição |
| Armamento de Exército | Artilharia |
| Equipamento | Capacete, Peitoral e Perneiras (militares); Chapéu, Camisa e Calças (não militares); Anel; Escudo |
| Artefato | Livro, Joia de Mana, Artefato |

**Animais** (revisão V1-3): Galinha, Porco, Ovelha, Vaca e Cavalo são itens do tipo **Animal**. O Pasto os cria; o Rancho os usa sem gastar; o Açougue os transforma em carne, ossos e couro. O **Cavalo** é montaria (com Sela) e animal de carga das caravanas ([[Sistema de Produção/Descrição|Produção]], [[Sistema de Comércio/Descrição|Comércio]]).

---

## Regras

### Regra dos nomes (rodada IP2-1)

1. **Época:** nomes descritivos em português, com palavras que caberiam num mundo de espadas e primeiras armas de fogo (mais ou menos os séculos XV a XVII). Nada de "moderno", "terno", "camiseta", "ninja", "social" ou "rifle".
2. **Padrão "objeto + de + material":** "Espada Curta de Aço". O material só aparece quando existe mais de uma versão do objeto.
3. **Sem embalagem no nome:** o item é a coisa ("Grão"), não o saco.
4. **Singular**, e a interface mostra "Pão × 12". As exceções são as coisas que só existem como monte (Ossos, Especiarias, Legumes, Joias).
5. **Nomes de lugar e de cultura** só nos Artefatos lendários, depois da história.
6. O recurso do mapa e o item têm o mesmo nome sempre que possível (recurso "Grão" → item "Grão"); a exceção são os minérios (recurso "Ferro" → item "Minério de Ferro").

**Nomes de fantasia** (ideia do autor): podem entrar no futuro, quando a história existir, sem mudar nenhuma regra, porque os IDs internos (`item.mosquete_aco`) não mudam quando o nome muda.

### Rank do item produzido

O Rank de uma arma, equipamento ou artefato produzido é o **menor** de três valores (rodada IP1-4): o teto do **material** (o máximo da faixa do item nesta lista), o teto do **nível** da construção e o Rank pela **habilidade** do trabalhador (1 + habilidade de Trabalho ÷ 25). Há 20% de chance de sair 1 Rank acima e 10% de sair 1 abaixo. Os itens de **Tier** não têm qualidade: a habilidade só acelera. Detalhes e exemplo no [[Sistema de Produção/Descrição|Produção]].

### Escada de materiais (rodada IP2-5)

Cada família de material tem três degraus. O degrau 4 vem de um reforço que existia na época, e o 5 vem da **Pedra de Mana**, como o Aço Mágico no metal.

| Família | Rank até 3 (Era 1) | Rank até 4 (Era 2) | Rank até 5 (Era 4) |
|---|---|---|---|
| Metal | Ferro | Aço | Aço Mágico |
| Madeira (arcos, escudo e cajado de madeira) | de Madeira (Tábua) | Composto (Tábua + Ossos + Couro Refinado) | Arcano (Tábua + Pedra de Mana) |
| Couro (armadura Leve) | de Couro (Couro Refinado) | Cravejado (Couro Refinado + Barra de Aço) | Arcano (Couro Refinado + Pedra de Mana) |
| Tecido (roupas) | O teto vem da própria peça | | |

- Cada item é feito na construção da sua família: o Carpinteiro faz o Arco Composto e o Arcano; o Costureiro faz o Cravejado e o Arcano de couro.
- As **bestas** continuam de metal (a besta de aço existia); o Carpinteiro as monta com Barra de Aço como insumo.
- Os insumos estão nas [[Sistema de Produção/Receitas|Receitas]].

### Armadura

Cada receita de armadura produz as **três peças separadas** (Capacete, Peitoral e Perneiras). **Leve, Média e Pesada** indicam o peso de cada peça, e o NPC pode combinar peças de pesos diferentes. A regra de combate da armadura mista está no [[Sistema de Combate/Descrição|Sistema de Combate]].

Pela ordem histórica (rodada IP2-7, que mudou um ponto da IP1-2):

- **Leve** = couro, feita pelo **Costureiro**;
- **Média** = cota de malha, feita pelo **Ferreiro**;
- **Pesada** = placas, feita pelo **Ferreiro**.

### A Pedra de Mana (rodada IP2-3, ideia do autor)

Existe **um único item de mana**, a **Pedra de Mana**: o recurso bruto, tirado do mapa pelo Posto de Coleta e usado direto no kit dos Magos, sem etapa de refino. O nome "Pedra Mágica" saiu do jogo.

O que sai dela (confirmado na IP3; ver [[Sistema de Produção/Receitas|Receitas]]):

| Uso | Onde | Como |
|---|---|---|
| Recuperar Mana | Consumida pelo Mago | Na hora (já decidido na [[Sistema de Magia/Descrição\|Magia]]) |
| Kit do Esquadrão de Magos | Batalha | 1 por batalha (já decidido) |
| Encantamento | Encantaria | Gasta junto com a Mana do Mago de Ofício (já decidido) |
| Barra de Aço Mágico | Forja Arcana | Barra de Aço + Pedra de Mana |
| Joia de Mana e Anel de Mana | Artesão | Pedra de Mana + Barra de Ouro (ou de Cobre) |
| Itens Arcanos de madeira e couro | Carpinteiro, Costureiro | Degrau 5 da escada de materiais |

### Consumo por Tier (rodada IP2-11)

O Tier do bem define em qual consumo da [[Sistema de Classe Social/Descrição|Classe Social]] ele entra. Um item novo já cai no grupo certo, sem mexer na tabela.

| Consumo | O que conta | Quem consome |
|---|---|---|
| Comida | Alimentos e bebidas. O Escravo come Tier 1; o Civil, Tier 1 e 2; o Rico e acima, Tier 2 ou mais | Todas as classes |
| Materiais Básicos | Mobília e Vestuário de Tier 1 e 2, mais a Tora no inverno | Civil e acima (a Tora, todas as classes) |
| Bens de Luxo | Tudo de Tier 3 que não é remédio, mais os Narcóticos | Rico e acima |
| Bens Exclusivos | Tudo de Tier 4 | Nobreza e Realeza |
| Artefatos | Livro, Joia de Mana e Relíquia, como posse | Realeza |

Os **remédios** ficam fora do consumo de classe: a cidade os gasta só nas epidemias ([[Sistema de Cura/Descrição|Cura]]).

### Nomes unificados

A lista de itens **manda sobre os canvases**. Unificados antes da IP2: **Couro Refinado** (não "Couro Tratado"), **Móveis Refinados** (não "Móveis de Luxo"), **Cajado Simples** (não "Cajado Básico"), **Clava** (não "Maça"), **Dejetos Animais** (não "Esterco"), **Mosquete** (não "Musket"), **Machado Curto e Longo** e **Calças Rasgadas**.

**Trocas da IP2** (nome antigo → nome final):

| Grupo | Trocas |
|---|---|
| Matérias-primas | Ferro, Cobre e Ouro Bruto → **Minério de** Ferro, Cobre e Ouro; Pedra Bruta → **Pedra**; Argila Bruta → **Argila**; Madeira Bruta → **Tora**; Fardo de Grão → **Grão**; Saco de Vegetal → **Legumes**; Saco de Frutas → **Fruta**; Fardo de Folha de Fumo → **Folha de Fumo**; Saco de Flor Espiritual → **Flor Espiritual**; Fardo de Especiaria → **Especiarias**; Fardo de Algodão → **Algodão**; Pedaço de Carne → **Carne Crua**; Couro Bruto → **Couro Cru**; Balde de Leite → **Leite**; Fardo de Lã → **Lã**; Saco de Ovos → **Ovo** |
| Mana | Pedra Mágica e Pedra de Mana Bruta → **Pedra de Mana**; Joia Mágica → **Joia de Mana**; Anel Mágico → **Anel de Mana** |
| Intermediários | Tijolos de Pedra → **Bloco de Pedra**; Tecido → **Tecido de Algodão** e **Tecido de Lã**; Fertilizante → **Adubo**; Pó Explosivo → **Pólvora** |
| Armas | Mosquete de Cobre → **Arcabuz**; Rifle → **Fuzil Raiado**; Cajado Moderno → **Cetro**; Munição de Arco, de Besta, de Arma de Fogo e de Canhão → **Flecha**, **Virote**, **Cartucho** e **Bala de Canhão**; Canhão de Guerra e Canhão de Guerra Moderno → **Bombarda**, **Canhão** e Canhão de Aço Mágico; sai o Arco Recurvo |
| Roupas | Camiseta e Calça Robusta → **Camisa** e **Calça de Couro**; Camiseta e Calça Moderna → **de Artífice**; Capuz, Camisa e Calça Ninja → **Furtivos**; Terno e Calça Social → **Casaca** e **Calça de Mercador**; Camiseta Nobre → **Túnica Nobre**; Escudo Primitivo → **Escudo de Madeira** |
| Consumo | Peças de Argila → **Cerâmica**; Perfumes → **Perfume**; Carne Envelhecida → **Presunto Curado**; Doce de Baunilha → **Doce de Especiarias**; os Legumes Cozidos saem (a Sopa os absorve) |

### Armamento de Exército

Não é equipado por NPCs: faz parte do Equipamento de um Esquadrão. Cada tipo de Esquadrão recebe um **kit** (1 item de cada peça equipa os 100 soldados), e o Rank dos itens define a qualidade ([[Sistema de Batalha/Descrição|Sistema de Batalha]]).

### Intervalo de Ataque

Cada arma da lista precisa de um valor. Definir por tipo de arma (Espada e Lança rápidas; Machado e Clava lentas; Besta e armas de fogo muito lentas) e ajustar por Rank nos testes.

### Encantamento

Armas e equipamentos de Rank alto podem ser encantados, o que aumenta atributos como Atributo Mágico, Dano, Defesa e Buff de habilidade. A partir do Rank 3, na Encantaria, por um Mago de Ofício, com Mana e Pedra de Mana; um encantamento por item. Ver [[Sistema de Magia/Descrição|Sistema de Magia]] e [[Sistema de Combate/Descrição|Sistema de Combate]].

---

## Itens existentes

### Matéria-Prima: Recurso Bruto

| Tipo | Itens (Tier) |
|---|---|
| Mineral | Minério de Ferro (0), Minério de Cobre (0), Minério de Ouro (1), Pedra (0), Argila (0), Carvão (0), Água (0), **Pedra de Mana** (1) |
| Vegetal | Grão (0), Legumes (0), Fruta (1), Folha de Fumo (0), Flor Espiritual (1), Especiarias (2), Algodão (0), Tora (0) |
| Animal | Carne Crua (0), Couro Cru (0), Leite (0), Lã (0), Ossos (0), Ovo (0), Dejetos Animais (0), Peixe (0); animais vivos: Galinha (0), Porco (0), Ovelha (0), Vaca (0), Cavalo (1) |

### Matéria-Prima: Recurso Manufaturado

| Tipo | Itens (Tier) | Construção |
|---|---|---|
| Metal | Barra de Ferro (1), Barra de Cobre (1), Barra de Aço (2), Barra de Ouro (3) | Metalurgia |
| Metal mágico | Barra de Aço Mágico (4) | Forja Arcana |
| Obra | Bloco de Pedra (1) | Pedreira |
| Obra | Tábua (1) | Carpinteiro |
| Tecido e couro | Tecido de Algodão (1), Tecido de Lã (1), Couro Refinado (1) | Costureiro |
| Mistura | Adubo (1), Pólvora (2) | Misturador |
| Peças | Grilhões (2; para prisioneiros e escravos) | Ferreiro |
| Peças | Sela (2; usada com o Cavalo) | Costureiro |

**Item órfão (IP1-8, esclarecido na IP2-4):** conta como uso ser insumo de receita, material de obra (custos da V2) ou consumo de classe. O Bloco de Pedra e a Tábua são usados principalmente nas obras.

### Bens de Consumo

| Tipo | Itens (Tier) | Construção |
|---|---|---|
| Alimento | Pão (1), Sopa (1), Peixe Defumado (1), Ração Militar (1), Torta de Fruta (2), Linguiça (2), Queijo (2), Assado Temperado (3), Presunto Curado (4), Doce de Especiarias (4) | Cozinheiro |
| Bebida | Cerveja (2), Vinho (3), Licor (4) | Destilaria |
| Mobília | Cerâmica (1) | Pedreira |
| Mobília | Móveis de Madeira (2), Móveis Refinados (3) | Carpinteiro |
| Mobília | Ornamento (4) | Artesão |
| Vestuário | Roupa Comum (1), Roupa Fina (3) | Costureiro |
| Essência e Joia | Perfume (3) | Destilaria |
| Essência e Joia | Joias (4) | Artesão |
| Narcótico | Fumo (2), Pó Espiritual (3) | Misturador |
| Medicamento | Curativo (1), Remédio (3) (ver [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]]) | Boticário |

- A **Ração Militar** é a comida dos Esquadrões ([[Sistema de Guerra/Descrição|Guerra]]).
- A **Roupa Comum** e a **Roupa Fina** são bens de consumo: têm Tier e não têm Rank, e só precisam de ícone, sem arte de boneco. A cidade as gasta aos poucos, como gasta a Cerâmica.

### Armas corpo a corpo

Cada arma existe em três materiais: **Ferro (Rank 1-3)**, **Aço (Rank 1-4)** e **Aço Mágico (Rank 1-5)**. "Curta" e "Longa" dizem o alcance, que é regra do Combate.

| Tipo | Versões |
|---|---|
| Espada | Espada Curta, Espada Longa |
| Lança | Lança Curta, Lança Longa |
| Machado | Machado Curto, Machado Longo |
| Clava | Clava Curta, Clava Longa |

### Armas de longo alcance

| Tipo | Itens (Rank) |
|---|---|
| Arco | Arco Curto e Arco Longo, cada um de Madeira (1-3), Composto (1-4) e Arcano (1-5) |
| Besta | Besta Leve de Madeira (1-3), de Aço (1-4) e de Aço Mágico (1-5); Besta Pesada e Besta de Repetição, de Aço (1-4) e de Aço Mágico (1-5) |
| Arma de fogo | **Arcabuz** de Ferro (1-3, Era 2); Mosquete de Aço (1-4, Era 3) e de Aço Mágico (1-5, Era 4); Carabina de Aço (1-4) e de Aço Mágico (1-5); **Fuzil Raiado** de Aço Mágico (1-5, Era 4) |
| Cajado (arma mágica) | Cajado Simples de Madeira (1-2) e de Cobre (1-3); Cajado Arcano de Aço (1-4) e de Aço Mágico (1-5); **Cetro** de Aço Mágico (1-4) |
| Munição | **Flecha** (1-3), **Virote** (1-3, da besta), **Cartucho** (1-3, bala com a pólvora em papel), **Bala de Canhão** (1-3) |

- O **Cetro** é um cajado de uma mão, que deixa usar Escudo, com dano 25% menor.
- O cobre continua como insumo das armas de fogo, nas peças de latão (IP3).
- Nenhuma animação nova: o Arcabuz, a Carabina e o Fuzil Raiado usam a do Mosquete, e o Cetro usa a do Cajado.

### Armamento de Exército

| Tipo | Itens (Rank) |
|---|---|
| Artilharia | **Bombarda** de Ferro (1-3, Era 2); **Canhão** de Aço (1-4, Era 3); Canhão de Aço Mágico (1-5, Era 4) |

### Equipamento militar (armaduras)

| Peso | Capacete | Peitoral | Perneiras | Materiais (Rank) | Construção |
|---|---|---|---|---|---|
| Leve | Capuz | Gibão | Perneiras | de Couro (1-3), Cravejado (1-4), Arcano (1-5) | Costureiro |
| Média | Coifa de Malha | Cota de Malha | Perneiras de Malha | de Ferro (1-3), de Aço (1-4), de Aço Mágico (1-5) | Ferreiro |
| Pesada | Elmo | Couraça | Grevas | de Ferro (1-3), de Aço (1-4), de Aço Mágico (1-5) | Ferreiro |

*Exemplos de nome:* "Gibão Cravejado", "Cota de Malha de Aço", "Elmo de Aço Mágico". São 27 peças.

### Equipamento não militar (roupas, anéis e escudos)

As roupas são organizadas em **conjuntos pelo papel social**:

| Conjunto | Chapéu | Camisa | Calças | Material | Rank |
|---|---|---|---|---|---|
| Pobre | — | Trapos | Calças Rasgadas | Tecido de Lã | 1-2 |
| Trabalhador | Chapéu de Palha (de Grão) | Camisa de Couro | Calça de Couro | Couro Refinado | 1-2 |
| Mineiro | Chapéu de Mineiro (de Ferro) | — | — | Barra de Ferro | 1-2 |
| Artífice | — | Camisa de Artífice | Calça de Artífice | Tecido de Algodão | 1-3 |
| Furtivo | Capuz Furtivo | Túnica Furtiva | Calça Furtiva | Tecido de Lã | 1-3 |
| Mercador | — | Casaca de Mercador | Calça de Mercador | Tecido de Algodão | 1-4 |
| Nobre | Chapéu Nobre | Túnica Nobre | Calça Nobre | Tecido de Algodão | 1-4 |
| Realeza | Coroa (de Ouro, Artesão) | — | — | Barra de Ouro | 1-5 |

- O conjunto Furtivo dá bônus de **Destreza** (ocultação), útil ao Ladrão; o Mercador e o Nobre dão bônus de **Carisma**. Os valores ficam a testar.

| Peça | Itens (Rank) | Construção |
|---|---|---|
| Anel | Anel de Cobre (1-3), Anel de Ouro (1-5), **Anel de Mana** (1-5) | Artesão |
| Escudo | **Escudo de Madeira** (1-3) | Carpinteiro |
| Escudo | Escudo de Ferro (1-3), de Aço (1-4) e de Aço Mágico (1-5) | Ferreiro |

### Artefatos

| Tipo | Itens (Rank) |
|---|---|
| Livro | Livro (2), Livro Antigo (4), Livro de Atributos (1-5), Livro Sagrado (1), Grimório (2-4; ensina feitiços, ver [[Sistema de Magia/Descrição\|Magia]]) |
| Joia de Mana | Joia de Mana (1-5; aumenta a Mana do Mago) |
| Artefato | Relíquia (1-3); Artefatos lendários, únicos (4-5) |

### Contagem

Cerca de **180 itens**: 29 matérias-primas brutas (com os 5 animais), 14 manufaturadas, 25 bens de consumo, 48 armas, 4 munições, 3 peças de artilharia, 27 peças de armadura, 17 roupas, 3 anéis, 4 escudos e cerca de 8 artefatos (fora os lendários).

---

## Parte técnica

- **Dados:** a lista de itens em CSV (editada em planilha) e os atributos por tipo em JSON, com IDs de texto legíveis (por exemplo, `item.mosquete_aco`), convertidos em números ao carregar ([[Arquitetura/Descrição|Arquitetura]]). Os **nomes** ficam num arquivo de textos separado, pronto para outros idiomas e para os nomes de fantasia do futuro.
- **Validação:** o jogo confere, ao carregar, se toda receita, kit e loot cita um item que existe e se nenhum item intermediário está órfão (`jogo --validar`).
- **Instâncias:** materiais e bens de consumo são **pilhas** (item + quantidade + dono), sem instância individual; armas e equipamentos guardam também o Rank, a cultura e o encantamento (ver [[Sistema de Inventário/Descrição|Inventário]]).
- **Interface:** o nome aparece no formato "Pão × 12", sem plural.
- **Save:** só as pilhas e as instâncias que existem nos inventários; a lista de itens vem dos dados.
- **Fatia vertical:** Grão, Água, Pão e Móveis de Madeira (ou Cerâmica).

## Ligação com outros sistemas

| Sistema | O que os Itens fornecem ou recebem |
|---|---|
| Efeitos de Itens | O que cada item especial faz ao ser usado |
| Produção e Funções | Quem produz cada item (uma construção por item); Rank pelo material, pelo nível e pela habilidade |
| Recursos | Itens de Matéria-Prima gerados por cada recurso (mesmo nome do recurso) |
| Econômico e Comércio | Valor Global, da Facção e Local; peso na carga das caravanas |
| Inventário | Pilhas e instâncias; Dono; peso |
| Classe Social | Consumo pelo Tier do bem |
| Combate e Batalha | Atributos das armas e armaduras; kits dos Esquadrões |
| Cultura e Pipeline | Textura por cultura; troca de paleta pelo material |
| Crime e Justiça | Itens proibidos; Grilhões; roupa Furtiva |
| Magia | Cajados e Cetro, Pedra de Mana, Aço Mágico, encantamento, Grimório, Joia e Anel de Mana |
| Tecnologia | Eras liberam Tiers e Ranks; Pólvora, Arcabuz e Bombarda na Era 2 |

## Perguntas em aberto

1. **Insumos de cada produto:** decididos na IP3 ([[Sistema de Produção/Receitas|Receitas]]).
2. **Valores a testar:** preços base (fórmula da V2-4, no [[Sistema Econômico/Descrição|Econômico]]) e os multiplicadores manuais, como o do Cavalo.
3. **Grimório e livros:** quem produz (hoje só se compram ou se acham; ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).
4. **Nomes de fantasia:** para o futuro, quando a história existir (ideia do autor).
