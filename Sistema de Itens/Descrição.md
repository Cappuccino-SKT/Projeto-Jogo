## Sistema de Itens

**Descrição geral**

Os itens são divididos em categorias, cada uma com uma função dentro do jogo. Os itens têm duas escalas distintas: o **Rank** (0 a 5, seis níveis), usado em armas, equipamentos e artefatos, e o **Tier** (0 a 4), usado em materiais e bens de consumo, ligado à complexidade de produção e ao tipo de Trabalho exigido (Braçal, Manufaturado ou Especialista). Materiais e bens de consumo não têm Rank, e armas, equipamentos e artefatos não têm Tier. Cada rank acima de 0 concede um bônus percentual no Atributo de Habilidade relevante ao equipar (ex: uma espada rank 3 aumenta um pouco mais o Corpo a Corpo do que uma rank 1), e/ou passa a exigir uma Habilidade mínima pra ser usado com eficácia total.

### Cultura

A cultura é um atributo das armas e equipamento, ele foi inspirado no mesmo sistema do jogo rimworld aonde temos os Estilos em certos itens, aonde um unico item pode ter mais de uma aparencia dependendo da cultura que o produziu, a ideia é a mesma, invés de criar diversas armas e equipamentos e depois ter que balancear todos eles, vamos criar os itens padrão, e depois apenas atribuir uma textura diferente dependendo a cultura que o produziu.


### Atributos (Geral)

- **Valor Global** — valor fornecido pelo mercado global
- **Valor Facção** — valor fornecido pelo mercado da facção
- **Valor Local** — valor fornecido pelo mercado local
- **Descrição** — descrição do item
- **Proibição** — estrutura que marca em quais facções esse item é proibido
- **Dono** — indica quem tem a posse legal do item
- **Categoria** — indicativo para agrupar itens parecidos
- **Nome** — nome do item
- **Rank** — nível de armas, equipamentos e artefatos, de 0 a 5
- **Tier** — nível de materiais e bens de consumo, de 0 a 4
- **Peso** — usado pela "Capacidade de peso" do NPC
- **Tipo** — derivado da Categoria (ver Tipos, abaixo)
- Textura

### Atributos (Arma Corpo a Corpo)
- Dano Maximo/Minimo
- Intervalo de Ataque
- Alcance
- Atributo Magico
- Atributo Minimo
- Cultura

### Atributos (Arma Longo Alcance)
- Intervalo de Ataque
- Alcance Maximo
- Munição
- Atributo Minimo
- Cultura

### Atributos (Municação)
- Dano Maximo/Minimo
- Atributo Magico

### Atributos (Cajado Magico)
- Dano Maximo/Minimo
- Alcance Maximo
- Mana (Quanto de mana gasta para atirar)
- Intervalo de Ataque
- Atributo Magico
- Atributo Minimo
- Cultura

### Atributos (Comida)
- Saciedade (Quanto da fome ela enche)

### Atributos (Equipamento)
- Defesa
- Lugar (Aonde esse equipamento fica no NPC, tipo capacete, torax, calça, anel, Escudo)
- Atributo Magico
- Buff de habilidade
- Cultura
### Categorias
1. Materia Prima
2. Bem de Consumo
3. Arma
4. Equipamento
5. Artefato

### Tipos (Derivado de Categoria)
- Materia Prima:
	1. Recurso Bruto
	2. Recurso Manufaturado
- Bem de Consumo:
	1. Alimento
	2. Bebida
	3. Medicamento
	4. Luxo
	5. Exclusivo
- Arma:
	1. Espada
	2. Lança
	3. Machado
	4. Clava
	5. Arco
	6. Cajado
	7. Musquete
	8. Besta
	9. Munição
- Equipamento
	 1. Capacete (Militar)
	 2. Peitoral (Militar)
	 3. Perneiras (Militar)
	 4. Chapeu (Não Militar)
	 5. Camisa (Não Militar)
	 6. Calças (Não Militar)
	 7. Anel
	 8. Escudo
- Artefato
	1. Livro
	2. Joia Magica
	3. Artefato
---

### Regras de armadura

Cada receita de armadura produz as **três peças separadas** (Capacete, Peitoral e Perneiras). **Leve, Média e Pesada** indicam o peso de cada peça, e o NPC pode combinar peças de pesos diferentes. A lista abaixo só tem peças Leves e Pesadas de metal; a peça **Média** (couro e tecido reforçado, feita pelo Costureiro) ainda será cadastrada. Em combate vale a classe de peso **mais frequente** entre as três peças (empate: Média) **[Proposta]**; ver [[Sistema de Combate/Descrição|Sistema de Combate]].

### Nomes unificados

A lista de itens manda sobre os canvases: **Couro Refinado** (não "Couro Tratado"), **Móveis Refinados** (não "Móveis de Luxo"), **Cajado Simples** (não "Cajado Básico"), **Clava** (não "Maça") e **Dejetos Animais** (não "Esterco"). Os canvases foram atualizados.

### Itens Existentes:

Materia-Prima:
- Recurso Bruto:
	- Mineral:
		1. Ferro Bruto (Tier 0)
		2. Cobre Bruto (Tier 0)
		3. Ouro Bruto (Tier 1)
		4. Pedra Bruta (Tier 0)
		5. Argila Bruta (Tier 0)
		6. Carvão (Tier 0)
		7. Pedra de Mana Bruta (Tier 1)
		8. Agua (Tier 0)
	- Vegetal:
		1. Fardo de Grão (Tier 0)
		2. Saco de Vegetal (Tier 0) 
		3. Saco de Frutas (Tier 1)
		4. Fardo de Folha de Fumo (Tier 0)
		5. Saco de Flor Espiritual (Tier 1)
		6. Fardo de Especiaria (Tier 2)
		7. Fardo de Algodão (Tier 0)
		8. Madeira Bruta (Tier 0)
	- Animal:
		1. Pedaço de Carne (Tier 0)
		2. Couro Bruto (Tier 0)
		3. Balde de Leite (Tier 0)
		4. Fardo de Lã (Tier 0)
		5. Ossos (Tier 0)
		6. Saco de Ovos (Tier 0) 
		7. Dejetos Animais (Tier 0)
- Recurso Manufaturado:
	 - Produção:
		1. Barra de Ferro (Tier 1)
		2. Barra de Aço (Tier 2)
		3. Barra de Aço Magico (Tier 4)
		4. Barra de Cobre (Tier 2)
		5. Barra de Ouro (Tier 3)
		6. Tijolos de Pedra (Tier 1)
	 - Manufaturado:
		1. Tecido (Tier 1)
		2. Fertilizante (Tier 1)
		3. Couro Refinado (Tier 1)
		4. Pó Explosivo (Tier 2)
- Bens de Consumo:
	- Alimento - Basico:
		1. Pão (Tier 1)
		2. Sopa (Tier 1)
		3. Legumes Cozidos (Tier 1)
		4. Ração Militar (Tier 1)
	- Mobilia - Basica:
		1. Materias de Argila (Tier 1)
		2. Moveis de Madeira (Tier 2)
	- Alimento - Refinado:
		1. Torta de Fruta (Tier 2)
		2. Linguiça (Tier 2)
		3. Queijo (Tier 2)
		4. Cerveja (Tier 2)
	- Mobilia - Refinada:
		1. Moveis Refinados (Tier 3)
		2. Ornamento (Tier 4)
	- Alimento de Luxo:
		1. Licor (Tier 4)
		2. Carne Envelhecida (Tier 4)
		3. Doce de Baunilha (Tier 4)
	 - Bens de Luxo:
		 1. Joias (Tier 4)
		 2. Perfumes (Tier 3)
	- Narcotico:
		1. Fumo (Tier 2)
		2. Pó Espiritual (Tier 3)
- Armas Corpo a Corpo:
	- Espada:
		Espada Curta - Ferro (Rank 1-3)
		Espada Curta - Aço (Rank 1-4)
		Espada Curta - Aço Magico (Rank 1-5)
		Espada Longa - Ferro (Rank 1-3)
		Espada Longa - Aço (Rank 1-4)
		Espada Longa - Aço Magico (Rank 1-5)
	- Lança:
		Lança Curta - Ferro (Rank 1-3)
		Lança Curta - Aço (Rank 1-4)
		Lança Curta - Aço Magico (Rank 1-5)
		Lança Longa - Ferro (Rank 1-3)
		Lança Longa - Aço (Rank 1-4)
		Lança Longa - Aço Magico (Rank 1-5)
	- Machado:
		Machado Curta - Ferro (Rank 1-3)
		Machado Curta - Aço (Rank 1-4)
		Machado Curta - Aço Magico (Rank 1-5)
		Machado Longa - Ferro (Rank 1-3)
		Machado Longa - Aço (Rank 1-4)
		Machado Longa - Aço Magico (Rank 1-5)
	- Clava:
		Clava Curta - Ferro (Rank 1-3)
		Clava Curta - Aço (Rank 1-4)
		Clava Curta - Aço Magico (Rank 1-5)
		Clava Longa - Ferro (Rank 1-3)
		Clava Longa - Aço (Rank 1-4)
		Clava Longa - Aço Magico (Rank 1-5)
- Arma Longo Alcance:
	- Arco:
		Arco Curto - Madeira (Rank 1-3)
		Arco Curto - Aço (Rank 1-4)
		Arco Curto - Aço Magico (Rank 1-5)
		Arco Recurvo - Madeira (Rank 1-3)
		Arco Recurvo - Aço (Rank 1-4)
		Arco Recurvo - Aço Magico (Rank 1-5)
		Arco Longo - Madeira (Rank 1-3)
		Arco Longo - Aço (Rank 1-4)
		Arco Longo - Aço Magico (Rank 1-5)
	- Besta:
		Besta Leve - Madeira (Rank 1-3)
		Besta Leve - Aço (Rank 1-4)
		Besta Leve - Aço Magico (Rank 1-5)
		Besta Pesada - Aço (Rank 1-4)
		Besta Pesada - Aço Magico (Rank 3-5)
		Besta Repetição - Aço (Rank 1-4)
		Besta Repetição - Aço Magico (Rank 1-5)
	 Canhão de mão:
		Musket - Cobre (Rank 1-3)
		Musket - Aço (Rank 1-4)
		Musket - Aço Magico (Rank 1-5)
		Carabina - Aço (Rank 1-4)
		Carabina - Aço Magico (Rank 1-5)
		Rifle - Aço Magico (Rank 1-5)
- Arma Magica:
	Cajado:
		Cajado Simples - Madeira (Rank 1-2)
		Cajado Simples - Cobre (Rank 1-3)
		Cajado Arcano - Aço (Rank 1-4)
		Cajado Arcano - Aço Magico (Rank 1-5)
		Cajado Moderno - Aço Magico (Rank 1-4)
- Equipamento
	-  Capacete (Militar):
		Capacete Leve - Ferro (Rank 1-3)
		Capacete Leve - Aço (Rank 1-4)
		Capacete Leve - Aço Magico (Rank 1-5)
		Capacete Pesado - Ferro (Rank 1-3)
		Capacete Pesado - Aço (Rank 1-4)
		Capacete Pesado - Aço Magico (Rank 1-5)
	- Peitoral (Militar)
		Peitoral Leve - Ferro (Rank 1-3)
		Peitoral Leve - Aço (Rank 1-4)
		Peitoral Leve - Aço Magico (Rank 1-5)
		Peitoral Pesado - Ferro (Rank 1-3)
		Peitoral Pesado - Aço (Rank 1-4)
		Peitoral Pesado - Aço Magico (Rank 1-5)  
	- Perneiras (Militar)
		Perneiras Leve - Ferro (Rank 1-3)
		Perneiras Leve - Aço (Rank 1-4)
		Perneiras Leve - Aço Magico (Rank 1-5)
		Perneiras Pesado - Ferro (Rank 1-3)
		Perneiras Pesado - Aço (Rank 1-4)
		Perneiras Pesado - Aço Magico (Rank 1-5)    
	- Chapeu (Não Militar)
		Chapeu de Palha - Grão (Rank 1-2)
		Chapeu de Mineiro - Ferro (Rank 1-2)
		Capuz Ninja - Tecido - (Rank 1-3)
		Chapeu Nobre - Tecido (Rank 1-4)
		Coroa - Ouro (Rank 1-5)
	- Camisa (Não Militar)
		Trapos - Tecido (Rank 1-2)
		Camiseta Robusta - Couro (Rank 1-2)
		Camisa Ninja - Tecido (Rank 1-3)
		Camiseta Moderna - Tecido (Rank 1-3)
		Camiseta Nobre - Tecido (Rank 1-4)
		Terno - Tecido (Rank 1-4)
	- Calças (Não Militar)
		Calça rasgadas - Tecido (Rank 1-2)
		Calça Robusta - Couro (Rank 1-2)
		Calça Ninjas - Tecido (Rank 1-3)
		Calça Modernas - Tecido (Rank 1-3)
		Calça Nobre - Tecido (Rank 1-4)
		Calça social - (Rank 1-4)
	- Anel
		Anel de Cobre - Cobre (Rank 1-3)
		Anel de Ouro - Ouro (Rank 1-5)
	- Escudo
		Escudo Primitivo - Madeira (Rank 1-2)
		Escudo - Ferro (Rank 1-3)
		Escudo - Aço (Rank 1-4)
		Escudo - Aço Magico (Rank 1-5)
- Artefato:
	Livros:
		Livro (Rank 2)
		Livro Antigo (Rank 4)
		Livro de Atributos (Rank 1-5)
	Joias Magicas:
		Joias Magicas- Pedra Magica (Rank 1-5)
	Artefato:
		Artefato (Rank 1-5)

---

### Itens a cadastrar (provisório)

Itens que aparecem nos canvases mas ainda não estão na lista. Tier e Rank são provisórios.

| Item | Categoria / Tipo | Tier ou Rank | Feito por |
|---|---|---|---|
| Tábua | Matéria-Prima / Recurso Manufaturado (Produção) | Tier 1 | Carpinteiro, a partir de Madeira Bruta |
| Grilhões | Matéria-Prima / Recurso Manufaturado (Produção) | Tier 2 | Ferreiro (nível 2+); usados em prisioneiros e escravos |
| Sela | Matéria-Prima / Recurso Manufaturado (Manufaturado) | Tier 2 | Costureiro (nível 5); usada com Cavalos |
| Munição de Arco | Arma / Munição | Rank 1-3 | Carpinteiro |
| Munição de Besta | Arma / Munição | Rank 1-3 | Carpinteiro (nível 3+) |
| Munição de Canhão de Mão | Arma / Munição | Rank 1-3 | Armeiro |
| Munição de Canhão de Guerra | Arma / Munição | Rank 1-3 | Armeiro (nível 3+) |
| Canhão de Guerra | Armamento de Exército | Rank 3 | Armeiro (nível 3+) |
| Canhão de Guerra Moderno | Armamento de Exército | Rank 5 | Armeiro (nível 5) |
| Anel Mágico - Pedra Mágica | Equipamento / Anel | Rank 1-5 | Artesão (nível 5) |
| Armadura Média (3 peças) | Equipamento / Capacete, Peitoral, Perneiras | Rank 1-3 | Costureiro (nível 4+) |
| Grimório | Artefato / Livro (ensina feitiços) | Rank 2-4 | A definir (comprado, produzido ou achado em masmorras; ver Sistema de Magia) |
| Pedra de Mana | Bem de Consumo (recupera Mana) | Tier 2 | Produtor Mágico, a partir da Pedra de Mana Bruta |

**Armamento de Exército** não é equipado por NPCs: faz parte do Equipamento de um Esquadrão (ver Sistema de Facção). Cada tipo de Esquadrão recebe um **kit** (1 item de cada peça equipa os 100 soldados), e o Rank dos itens define a qualidade (ver [[Sistema de Batalha/Descrição|Sistema de Batalha]]).

**Medicamentos.** O tipo Medicamento existe, mas ainda não há itens. Ver Sistema de Cura.

**Intervalo de Ataque.** Cada arma da lista precisa de um valor. **[Proposta]** definir por tipo de arma (Espada e Lança rápidas, Machado e Clava lentas, Besta e Musquete muito lentas) e ajustar por Rank em testes.

### Encantamento

Armas e equipamentos de Rank alto podem ser encantados, o que aumenta atributos como Atributo Mágico, Dano, Defesa e Buff de habilidade. A partir do Rank 3, na Encantaria, por um Mago de Ofício, com Mana e Pedra de Mana; um encantamento por item. Ver [[Sistema de Magia/Descrição|Sistema de Magia]] e [[Sistema de Combate/Descrição|Sistema de Combate]].
