> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> **Em reformulação:** o autor decidiu (revisão V1-2, V1-7 e V1-8) fazer rodadas próprias para **padronizar a produção**, **reformular os nomes dos itens** para o contexto do mundo e definir **os insumos de cada produto**. A lista abaixo é a atual, com os nomes já padronizados na revisão V1-6.

## Sistema de Itens

**Descrição geral**

Os itens são divididos em **categorias**, cada uma com uma função dentro do jogo. Os itens têm duas escalas distintas:

- **Rank** (0 a 5, seis níveis): usado em armas, equipamentos e artefatos. Cada Rank acima de 0 dá um bônus percentual no Atributo de Habilidade relevante ao equipar (por exemplo, uma espada Rank 3 aumenta um pouco mais o Corpo a Corpo do que uma Rank 1) e/ou passa a exigir uma habilidade mínima para ser usado com eficácia total.
- **Tier** (0 a 4): usado em materiais e bens de consumo, ligado à complexidade da produção e ao tipo de Trabalho exigido (Braçal, Manufaturado ou Especialista).

Materiais e bens de consumo não têm Rank, e armas, equipamentos e artefatos não têm Tier.

O que os itens especiais fazem ao serem usados (livros, joias, artefatos, medicamentos, comida, bebida e narcóticos) está em [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]].

### Cultura

A cultura é um atributo das armas e dos equipamentos, inspirado nos **Estilos** do RimWorld: um único item pode ter mais de uma aparência, conforme a cultura que o produziu. Em vez de criar diversas armas e equipamentos e depois ter que balancear todos, o jogo cria os itens padrão e só atribui uma **textura diferente** conforme a cultura que o produziu ([[Sistema de Cultura/Descrição|Cultura]], [[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]).

### Atributos (todos os itens)

- **Valor Global** — valor dado pelo Mercado Mundial
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
| Comida | Saciedade (quanto da Fome ela enche; ver [[Sistema de NPC/Descrição\|NPC]]) |
| Equipamento | Defesa, Lugar (capacete, peitoral, perneiras, chapéu, camisa, calças, anel, escudo), Atributo Mágico, Buff de habilidade, Cultura |

### Categorias e tipos

| Categoria | Tipos |
|---|---|
| Matéria-Prima | Recurso Bruto (Mineral, Vegetal, **Animal**), Recurso Manufaturado |
| Bem de Consumo | Alimento, Bebida, Medicamento, Luxo, Exclusivo |
| Arma | Espada, Lança, Machado, Clava, Arco, Cajado, **Mosquete**, Besta, Munição |
| Equipamento | Capacete, Peitoral e Perneiras (militares); Chapéu, Camisa e Calças (não militares); Anel; Escudo |
| Artefato | Livro, Joia Mágica, Artefato |

**Animais** (revisão V1-3): Galinha, Porco, Ovelha, Vaca e Cavalo são itens do tipo **Animal**. O Pasto os cria; o Rancho os usa sem gastar; o Açougue os transforma em carne, ossos e couro. O **Cavalo** é montaria (com Sela) e animal de carga das caravanas ([[Sistema de Produção/Descrição|Produção]], [[Sistema de Comércio/Descrição|Comércio]]).

---

## Regras

### Armadura

Cada receita de armadura produz as **três peças separadas** (Capacete, Peitoral e Perneiras). **Leve, Média e Pesada** indicam o peso de cada peça, e o NPC pode combinar peças de pesos diferentes. A regra de combate da armadura mista está no [[Sistema de Combate/Descrição|Sistema de Combate]].

### Nomes unificados

A lista de itens **manda sobre os canvases**. Já unificados: **Couro Refinado** (não "Couro Tratado"), **Móveis Refinados** (não "Móveis de Luxo"), **Cajado Simples** (não "Cajado Básico"), **Clava** (não "Maça") e **Dejetos Animais** (não "Esterco"). Na revisão V1-6: **Mosquete** (não "Musket"), "Arma de fogo" como grupo (não "Canhão de mão"), **Machado Curto e Longo**, **Perneiras Leves e Pesadas**, **Calças Rasgadas**, **Peças de Argila** (não "Materias" ou "Bens de Argila"), **Ossos**, **Pedaço de Carne**, **Saco de Ovos**, **Fardo de Lã** e **Balde de Leite**.

### Armamento de Exército

Não é equipado por NPCs: faz parte do Equipamento de um Esquadrão. Cada tipo de Esquadrão recebe um **kit** (1 item de cada peça equipa os 100 soldados), e o Rank dos itens define a qualidade ([[Sistema de Batalha/Descrição|Sistema de Batalha]]).

### Intervalo de Ataque

Cada arma da lista precisa de um valor. **[Proposta]** Definir por tipo de arma (Espada e Lança rápidas; Machado e Clava lentas; Besta e Mosquete muito lentas) e ajustar por Rank nos testes.

### Encantamento

Armas e equipamentos de Rank alto podem ser encantados, o que aumenta atributos como Atributo Mágico, Dano, Defesa e Buff de habilidade. A partir do Rank 3, na Encantaria, por um Mago de Ofício, com Mana e Pedra de Mana; um encantamento por item. Ver [[Sistema de Magia/Descrição|Sistema de Magia]] e [[Sistema de Combate/Descrição|Sistema de Combate]].

---

## Itens existentes

### Matéria-Prima: Recurso Bruto

| Tipo | Itens (Tier) |
|---|---|
| Mineral | Ferro Bruto (0), Cobre Bruto (0), Ouro Bruto (1), Pedra Bruta (0), Argila Bruta (0), Carvão (0), Pedra de Mana Bruta (1), Água (0) |
| Vegetal | Fardo de Grão (0), Saco de Vegetal (0), Saco de Frutas (1), Fardo de Folha de Fumo (0), Saco de Flor Espiritual (1), Fardo de Especiaria (2), Fardo de Algodão (0), Madeira Bruta (0) |
| Animal | Pedaço de Carne (0), Couro Bruto (0), Balde de Leite (0), Fardo de Lã (0), Ossos (0), Saco de Ovos (0), Dejetos Animais (0), **Peixe (0)**; animais vivos: Galinha (0), Porco (0), Ovelha (0), Vaca (0), **Cavalo (1)** |

### Matéria-Prima: Recurso Manufaturado

| Tipo | Itens (Tier) |
|---|---|
| Produção | Barra de Ferro (1), Barra de Aço (2), Barra de Aço Mágico (4), Barra de Cobre (2), Barra de Ouro (3), Tijolos de Pedra (1) |
| Manufaturado | Tecido (1), Fertilizante (1), Couro Refinado (1), Pó Explosivo (2) |

### Bens de Consumo

| Grupo | Itens (Tier) |
|---|---|
| Alimento Básico | Pão (1), Sopa (1), Legumes Cozidos (1), Ração Militar (1) |
| Mobília Básica | Peças de Argila (1), Móveis de Madeira (2) |
| Alimento Refinado | Torta de Fruta (2), Linguiça (2), Queijo (2), Cerveja (2) |
| Mobília Refinada | Móveis Refinados (3), Ornamento (4) |
| Alimento de Luxo | Licor (4), Carne Envelhecida (4), Doce de Baunilha (4) |
| Bens de Luxo | Joias (4), Perfumes (3) |
| Narcótico | Fumo (2), Pó Espiritual (3) |
| Medicamento | Curativo (1), Remédio (3) (ver [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]]) |

### Armas corpo a corpo

Cada arma existe em três materiais: **Ferro (Rank 1-3)**, **Aço (Rank 1-4)** e **Aço Mágico (Rank 1-5)**.

| Tipo | Versões |
|---|---|
| Espada | Espada Curta, Espada Longa |
| Lança | Lança Curta, Lança Longa |
| Machado | Machado Curto, Machado Longo |
| Clava | Clava Curta, Clava Longa |

### Armas de longo alcance

| Tipo | Itens (Rank) |
|---|---|
| Arco | Arco Curto, Arco Recurvo e Arco Longo, cada um de Madeira (1-3), Aço (1-4) e Aço Mágico (1-5) |
| Besta | Besta Leve de Madeira (1-3), de Aço (1-4) e de Aço Mágico (1-5); Besta Pesada de Aço (1-4) e de Aço Mágico (3-5); Besta de Repetição de Aço (1-4) e de Aço Mágico (1-5) |
| Arma de fogo | Mosquete de Cobre (1-3), de Aço (1-4) e de Aço Mágico (1-5); Carabina de Aço (1-4) e de Aço Mágico (1-5); Rifle de Aço Mágico (1-5) |
| Cajado (arma mágica) | Cajado Simples de Madeira (1-2) e de Cobre (1-3); Cajado Arcano de Aço (1-4) e de Aço Mágico (1-5); Cajado Moderno de Aço Mágico (1-4) |

### Equipamento

| Peça | Itens (Rank) |
|---|---|
| Capacete (militar) | Capacete Leve e Capacete Pesado, de Ferro (1-3), Aço (1-4) e Aço Mágico (1-5) |
| Peitoral (militar) | Peitoral Leve e Peitoral Pesado, de Ferro (1-3), Aço (1-4) e Aço Mágico (1-5) |
| Perneiras (militar) | Perneiras Leves e Perneiras Pesadas, de Ferro (1-3), Aço (1-4) e Aço Mágico (1-5) |
| Chapéu (não militar) | Chapéu de Palha, de Grão (1-2); Chapéu de Mineiro, de Ferro (1-2); Capuz Ninja, de Tecido (1-3); Chapéu Nobre, de Tecido (1-4); Coroa, de Ouro (1-5) |
| Camisa (não militar) | Trapos, de Tecido (1-2); Camiseta Robusta, de Couro (1-2); Camisa Ninja, de Tecido (1-3); Camiseta Moderna, de Tecido (1-3); Camiseta Nobre, de Tecido (1-4); Terno, de Tecido (1-4) |
| Calças (não militar) | Calças Rasgadas, de Tecido (1-2); Calça Robusta, de Couro (1-2); Calça Ninja, de Tecido (1-3); Calça Moderna, de Tecido (1-3); Calça Nobre, de Tecido (1-4); Calça Social (1-4) |
| Anel | Anel de Cobre (1-3), Anel de Ouro (1-5) |
| Escudo | Escudo Primitivo, de Madeira (1-2); Escudo de Ferro (1-3), de Aço (1-4) e de Aço Mágico (1-5) |

Os nomes com cara moderna (Ninja, Moderna, Terno, Social, Cajado Moderno) serão reformulados na rodada de nomes.

### Artefatos

| Tipo | Itens (Rank) |
|---|---|
| Livro | Livro (2), Livro Antigo (4), Livro de Atributos (1-5), Livro Sagrado (1) |
| Joia Mágica | Joia Mágica de Pedra Mágica (1-5) |
| Artefato | Relíquia (1-3); Artefatos lendários, únicos (4-5) |

### Itens a cadastrar (provisório)

Itens que aparecem nos canvases ou em outros sistemas mas ainda não estão na lista principal. Tier e Rank são provisórios.

| Item | Categoria / Tipo | Tier ou Rank | Feito por |
|---|---|---|---|
| Tábua | Matéria-Prima / Recurso Manufaturado (Produção) | Tier 1 | Carpinteiro, a partir de Madeira Bruta |
| Grilhões | Matéria-Prima / Recurso Manufaturado (Produção) | Tier 2 | Ferreiro (nível 2+); usados em prisioneiros e escravos |
| Sela | Matéria-Prima / Recurso Manufaturado (Manufaturado) | Tier 2 | Costureiro (nível 5); usada com Cavalos |
| Munição de Arco | Arma / Munição | Rank 1-3 | Carpinteiro |
| Munição de Besta | Arma / Munição | Rank 1-3 | Carpinteiro (nível 3+) |
| Munição de Arma de Fogo | Arma / Munição | Rank 1-3 | Armeiro |
| Munição de Canhão de Guerra | Arma / Munição | Rank 1-3 | Armeiro (nível 3+) |
| Canhão de Guerra | Armamento de Exército | Rank 3 | Armeiro (nível 3+) |
| Canhão de Guerra Moderno | Armamento de Exército | Rank 5 | Armeiro (nível 5) |
| Anel Mágico de Pedra Mágica | Equipamento / Anel | Rank 1-5 | Artesão (nível 5) |
| Armadura Média (3 peças) | Equipamento / Capacete, Peitoral, Perneiras | Rank 1-3 | Costureiro (nível 4+) |
| Grimório | Artefato / Livro (ensina feitiços) | Rank 2-4 | A definir (comprado, produzido ou achado em masmorras; ver [[Sistema de Magia/Descrição\|Magia]]) |
| Pedra de Mana | Bem de Consumo (recupera Mana) | Tier 2 | Mago de Ofício, a partir da Pedra de Mana Bruta |

---

## Parte técnica

- **Dados:** a lista de itens em CSV (editada em planilha) e os atributos por tipo em JSON, com IDs de texto legíveis (por exemplo, `item.mosquete_aco`), convertidos em números ao carregar ([[Arquitetura/Descrição|Arquitetura]]).
- **Validação:** o jogo confere, ao carregar, se toda receita, kit e loot cita um item que existe (`jogo --validar`).
- **Instâncias:** materiais e bens de consumo são **pilhas** (item + quantidade + dono), sem instância individual; armas e equipamentos guardam também o Rank, a cultura e o encantamento (ver [[Sistema de Inventário/Descrição|Inventário]]).
- **Save:** só as pilhas e as instâncias que existem nos inventários; a lista de itens vem dos dados.
- **Fatia vertical:** Fardo de Grão, Água, Pão e Móveis de Madeira (ou Peças de Argila).

## Ligação com outros sistemas

| Sistema | O que os Itens fornecem ou recebem |
|---|---|
| Efeitos de Itens | O que cada item especial faz ao ser usado |
| Produção e Funções | Quem produz cada item; Tier e Rank pelo nível da construção |
| Recursos | Itens de Matéria-Prima gerados por cada recurso |
| Econômico e Comércio | Valor Global, da Facção e Local; peso na carga das caravanas |
| Inventário | Pilhas e instâncias; Dono; peso |
| Classe Social | Grupos de itens de cada tipo de consumo |
| Combate e Batalha | Atributos das armas e armaduras; kits dos Esquadrões |
| Cultura e Pipeline | Textura por cultura |
| Crime e Justiça | Itens proibidos; Grilhões |
| Magia | Cajados, Pedra de Mana, Aço Mágico, encantamento, Grimório |
| Tecnologia | Eras liberam Tiers e Ranks |

## Perguntas em aberto

1. **Padronização da produção, nomes e insumos:** rodadas IP1, IP2 e IP3 (ver a [[Revisão Completa]]).
2. **Peça de armadura Média:** materiais e quem faz (rodada IP1).
3. **Preço base** de cada item (rodada V2).
4. **Mapeamento Luxo × Exclusivo** por Tier (rodada IP2).
