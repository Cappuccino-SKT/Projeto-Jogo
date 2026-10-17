> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Efeitos de Itens

**Descrição geral**

Tabela mestre do que os itens especiais **fazem ao serem usados**: livros, joias, artefatos, medicamentos, comida, bebida e narcóticos. Complementa o [[Sistema de Itens/Descrição|Sistema de Itens]], que guarda a lista, o Rank e o Tier; aqui ficam só os efeitos. As armas e armaduras continuam no [[Sistema de Combate/Descrição|Sistema de Combate]], e os feitiços e o encantamento, no [[Sistema de Magia/Descrição|Sistema de Magia]].

Regra geral: **só Controlados e Importantes usam itens com efeito individual**. Os NPCs comuns consomem de forma agregada, pela cidade ([[Sistema de Classe Social/Descrição|Classe Social]]). Assim, os traços novos (livros lidos, Viciado, Doente) só existem para poucos NPCs e não pesam na memória.

Os valores numéricos são iniciais e ficam em dados (`efeitos_itens.json`) para ajustar nos testes.

---

## Tabela mestre

| Item | Como se usa | Efeito | Duração | De onde vem |
|---|---|---|---|---|
| **Livro** (Rank 2) | Ler: 8 h fora de combate | +3 na habilidade do assunto, até 60; uma vez por NPC; não se gasta | Permanente | Mercado (cidade nível 3+), masmorras |
| **Livro Antigo** (Rank 4) | Ler: 8 h | +6 na habilidade do assunto, até 80; uma vez por NPC; não se gasta | Permanente | Só masmorras |
| **Livro Sagrado** (Rank 1) | Carregar | Exigido na revista das facções com a lei "Livro Sagrado obrigatório" (ver abaixo) | — | Templo e Mercado das cidades Religiosas |
| **Livro de Atributos** (Rank 1-5) | Ler: 8 h; **se gasta** | +2 × Rank numa habilidade de combate, até 100 | Permanente | Masmorras e recompensas de missão |
| **Grimório** (Rank 2-4) | Estudar (só Magos) | Ensina um feitiço (ver [[Sistema de Magia/Descrição\|Magia]]) | Permanente | A definir na Magia |
| **Joia Mágica** (Rank 1-5) | Carregar no inventário (só a melhor conta) | Mago: +10 × Rank de Mana máxima e +5% × Rank de regeneração de Mana | Enquanto carregar | Masmorras, Artesão (futuro) |
| **Anel Mágico** (Rank 1-5) | Equipar no slot Anel | +3% × Rank numa habilidade escolhida na produção; encantável a partir do Rank 3 | Enquanto equipado | Artesão nível 5 |
| **Pedra de Mana** (Tier 2) | Consumir | Recupera Mana (já decidido) | Na hora | Produtor Mágico |
| **Relíquia** (Artefato Rank 1-3) | Guardar ou doar | Sem efeito em combate; doada a um Templo ou Palácio, +2 de Renome; conta na posse da Realeza | — | Masmorras, Mercado de capitais |
| **Artefato lendário** (Rank 4-5) | Equipar ou carregar | Efeito próprio e único (ver "Artefatos lendários") | Enquanto estiver com o NPC | Masmorras lendárias |
| **Curativo** (Tier 1) | Usar | Regeneração × 2; estanca o sangramento do Inconsciente | 8 h | Boticário nível 1 (Tecido + Água) |
| **Remédio** (Tier 3) | Usar | Regeneração × 3; cura o estado Doente | 24 h | Boticário nível 3 (Flor Espiritual + Fardo de Especiaria) |
| **Grilhões** (Tier 2) | Pôr num escravo ou prisioneiro | Obediência +15; Felicidade −5; velocidade −20%; não foge | Enquanto estiver com eles | Ferreiro nível 2 (ver [[Sistema de Escravidão/Descrição\|Escravidão]]) |

## Livros

- Cada Livro e Livro Antigo tem um **assunto**, sorteado quando é criado ou achado: Inteligência, Carisma, Percepção, Trabalho Braçal, Manufaturado ou Especialista. O nome mostra o assunto ("Tratado de Comércio", de Carisma).
- **Ler** é uma ação de 8 horas, fora de combate (no descanso ou no Período Livre). O NPC guarda a **lista dos livros que já leu**; ler o mesmo título de novo não dá nada.
- O livro **não se gasta**: pode ser emprestado aos companheiros, vendido ou guardado.
- **Universidade:** cada livro guardado nela dá +2% ao bônus de educação da cidade, até 10 livros ([[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]]).
- **Produção:** por enquanto, só se compra ou se acha. **[Refinar depois]** Uma tecnologia de "Imprensa" (Era 2) pode liberar a produção ([[Sistema de Tecnologia/Descrição|Tecnologia]]).
- *Exemplo:* o Mercador lê "Tratado de Comércio" e passa de 48 para 51 de Carisma; empresta ao companheiro, que passa de 30 para 33.

**Livro de Atributos:** o livro raro de **combate**. Assunto: Corpo a Corpo, Longo Alcance, Defesa, Força, Agilidade, Destreza ou Magia (só para Magos). **Se gasta** ao ser lido. Rank 4 e 5 só nas masmorras de dificuldade 4 e nas lendárias; não é vendido no Mercado comum. **[Proposta]** Preço de cerca de 300 × Rank. É um atalho raro; a regra continua sendo a evolução por uso do Combate.

### Livro Sagrado (ideia do autor)

Inspirado no Kenshi. Nas facções de cultura Religiosa, **todo NPC do grupo do jogador precisa carregar um Livro Sagrado**. Se um guarda revistar o grupo e alguém estiver sem o livro, isso é **crime de heresia**.

- **Lei:** nova lei de facção, **Livro Sagrado obrigatório** (sim ou não). A Religiosa deseja "sim", com peso alto, e começa com ela ([[Sistema de Facção/Descrição|Facção]], [[Sistema de Cultura/Descrição|Cultura]]).
- **Item:** Livro Sagrado, Artefato do tipo Livro, Rank 1, peso baixo. **[Proposta]** 20 moedas, vendido no Templo e no Mercado das cidades Religiosas. Não ensina habilidade; é um objeto de fé.
- **Quem precisa:** os **Controlados** do grupo. **[Proposta]** Os Escravos do jogador não precisam (são propriedade); os NPCs comuns da cidade não são revistados (a simulação considera que eles cumprem a lei).
- **Revista:** os guardas da cidade (na entrada e nas rondas) e as patrulhas da facção. **[Proposta]** Chance de revista conforme a reputação do jogador na facção ([[Sistema de Reputação/Descrição|Reputação]]):

| Reputação | Na entrada da cidade | Encontro com patrulha ou ronda |
|---|---|---|
| Amigável ou Aliado | 0% | 0% |
| Neutro | 10% | 10% |
| Hostil | Sempre (já decidido na Reputação) | 25% |
| Inimigo | Sempre | 50% |

- **Sem o livro:** crime de heresia ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]). **[Proposta]** Na primeira vez numa estação, é a forma leve: **gravidade 2**, multa, e o guarda obriga a comprar o livro ali mesmo pelo dobro do preço. Se alguém do grupo for pego de novo sem o livro na mesma estação, vira heresia comum (gravidade 3, ou 5 com a lei de Perseguição).
- *Conflito:* na Religião, a heresia é um **ato**, e um NPC não é preso só por ser de outra fé. Andar sem o livro numa facção que o exige é uma **omissão de um dever da lei**, que vale para qualquer cultura, por isso não contradiz a regra. A gravidade leve evita que esquecer um livro leve à execução, a pena da heresia na Religiosa ([[Sistema de Religião/Descrição|Religião e Fé]]).
- *Exemplo:* o Aventureiro entra na Cidade Religiosa (7,3) com 3 companheiros; a reputação é Neutra e sai a revista (10%). Um companheiro não tem o livro: o grupo paga a multa e 40 moedas pelo livro. Dois dias depois, outro guarda pega o mesmo companheiro sem o livro (ele o vendeu): agora é heresia de gravidade 5, e o guarda dá a ordem de rendição.

## Artefatos

**Dois tipos:**

- **Relíquias** (Rank 1 a 3): genéricas e sem efeito em combate. Valem muito, servem ao consumo da Realeza e, doadas a um Templo ou Palácio, dão +2 de Renome.
- **Artefatos lendários** (Rank 4 e 5): **únicos**, com nome e efeito próprios, escritos à mão. Cerca de 6 na primeira versão, de 1 a 2 por masmorra lendária ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).

**[Proposta] Artefatos lendários iniciais** (nomes provisórios, até a história ser escrita):

| Artefato | Efeito |
|---|---|
| Estandarte do Primeiro Rei | +10 de Moral no exército em que o portador é General |
| Elmo do Juramento | +15 de Coragem; imune ao Medo |
| Cajado da Primeira Chama | Magia +10; feitiços de Combate custam 20% menos Mana |
| Anel da Língua de Prata | Carisma +10; +10% de chance nas ações de diálogo |
| Mapa do Navegante | Revela todos os mapas da região onde está; viagem do grupo × 0,9 |
| Coroa Partida | Para um líder: +10 de legitimidade (Estabilidade da facção) |

**Consumo da Realeza como posse.** A cidade precisa ter **2 itens de Artefato** (Livro, Joia Mágica ou Relíquia) **por NPC real de Realeza**, guardados no Palácio. Eles **não somem**: 1 em cada 10 sai por estação (presentes, roubos), e isso gera a demanda. Os Artefatos lendários não entram nessa conta.

- *Conflito resolvido:* a Classe Social trata todo consumo como gasto do estoque. Só os Artefatos passam a ser posse com desgaste; o resto continua igual ([[Sistema de Classe Social/Descrição|Classe Social]]).

## Medicamentos e doença

- **Curativo** e **Remédio**, na tabela mestre.
- **Doença:** nos **NPCs comuns**, é só um modificador da cidade (o evento de Saúde). Nos **Controlados e Importantes**, existe o estado **Doente**: −20% nas habilidades e sem regeneração, até tomar Remédio, ir ao Hospital ou passar 8 dias. Isso fecha a pergunta em aberto da [[Sistema de Cura/Descrição|Cura]].
- **Na cidade:** durante uma epidemia, a cidade gasta **1 Remédio por 10 NPCs reais por dia**. Com estoque, a mortalidade da doença cai pela metade. Isso dá demanda real ao Boticário.
- *Exemplo:* uma peste na Cidade Autoritária (100 NPCs reais) gasta 10 Remédios por dia; com 80 no estoque, ela aguenta 8 dias protegida, e o preço do Remédio sobe pela simulação.

## Comida, bebida e narcóticos no grupo

| Item | Efeito no Controlado (no dia) |
|---|---|
| Alimento Básico (Pão, Sopa, Legumes Cozidos, Ração Militar) | Fator comida 0 |
| Alimento Refinado (Torta de Fruta, Linguiça, Queijo) | +3 |
| Alimento de Luxo (Licor, Carne Envelhecida, Doce de Baunilha) | +6 |
| Cerveja | +2 de Felicidade |
| Fumo | +3 de Felicidade por 1 dia |
| Pó Espiritual | +6 de Felicidade; Mago: +25% de regeneração de Mana por 1 dia |

- O "fator comida" é o fator "comida e descanso" da Felicidade dos Controlados ([[Sistema de Felicidade e Estabilidade/Descrição|Felicidade]]).
- O grupo come a **melhor comida do inventário**, a menos que o jogador mande "economizar".
- **Vício:** cada uso tem chance de criar o traço **Viciado** (5% no Fumo, 10% no Pó Espiritual). Viciado sem a droga por 2 dias: Felicidade −10. Some depois de 1 estação sem uso.
- **Nas cidades, nada muda:** os narcóticos continuam sendo consumo de Luxo agregado, sem vício.
- Os narcóticos são proibidos na Religiosa: carregar é Contrabando ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).

---

## Parte técnica

- **Dados:** `efeitos_itens.json` com o efeito de cada item ou tipo (habilidade, valor, duração, teto, se gasta), os assuntos dos livros, os Artefatos lendários, as chances de vício e a revista do Livro Sagrado.
- **Por NPC** (só Controlados e Importantes): lista de livros lidos (IDs), traço Viciado (com a data do último uso), estado Doente (com a data de início) e efeitos temporários ativos.
- **Frequência:** os efeitos temporários vencem por hora de jogo; nada roda por tick.
- **Save:** os dados por NPC acima e a posse de Artefatos de cada Palácio.
- **Fatia vertical:** só a qualidade da comida na Felicidade.

## Ligação com outros sistemas

| Sistema | O que os Efeitos de Itens fornecem ou recebem |
|---|---|
| Itens | Lista, Rank e Tier; assuntos dos livros; Livro Sagrado e Artefatos lendários como itens novos |
| NPC | Livros lidos, Viciado e Doente, só para Controlados e Importantes |
| Classe Social | Posse de Artefatos pela Realeza |
| Cura | Curativo, Remédio e estado Doente |
| Magia | Joia Mágica, Pó Espiritual e Grimório |
| Felicidade | Comida, bebida e narcóticos no fator "comida e descanso"; vício |
| Combate e Batalha | Livro de Atributos; Artefatos lendários (Moral, Coragem) |
| Religião e Crime | Livro Sagrado obrigatório; revista; heresia sem o livro |
| Reputação | Chance de revista pela faixa; Renome por doação de Relíquia |
| Trabalho e Propriedade | Livros guardados na Universidade |
| Produção | Boticário (Curativo e Remédio); Artesão (Anel Mágico) |
| Objetivo e Progressão | Loot das masmorras; Artefatos lendários |
| Interface | Botões "Usar" e "Ler"; ícone de Viciado e Doente no retrato |

## Perguntas em aberto

1. **Valores a testar:** pontos por livro e tetos, preços, duração dos efeitos, chances de vício, chance de revista.
2. **Artefatos lendários:** lista final e nomes (com a história do jogo).
3. **Produção de livros** (Imprensa) e quem produz a Joia Mágica.
4. **Livro Sagrado:** se outras culturas ganham uma exigência parecida (por exemplo, a Autoritária com um documento de identidade).
