Inventario Abstrato, esse inventario é apenas um inventario generico que vai servir como herança para os inventarios especializados, vamos usa-lo apenas para criar a parte de armazenamente de informações de itens, como qual item esta sendo armazenado, a quantidade dele também o dinheiro, e as outras informações relevantes.

Inventario NPC Geral, O inventario do NPC geral vai herdar as funções do Inventario abstrato, sua especialização vai ser a a implementação do sistema de equipamente que vai permitir usar equipamento do inventario do NPC.

Inventario NPC jogador, esse é o inventario com mais funcionalidade por ser o principal inventario com interação do jogador, iremos herdar as funções do inventario do NPC, junto a isso iremos colocar uma parte grafica, alem de um limitante, que vai ser o peso aonde dependendo do capacidade de peso ele vai ficar limitado pela quantidade de itens ele vai poder carregar, a capacidade de peso vai ser influenciado pela força do NPC.

Inventario Cidade, as cidades iram herdar o inventario Abstrato, o inventario da cidade vai passar informações para outros sistemas como o sistema de eventos para causar eventos nas cidades e nas regiões, o inventario da cidade vinculado ao inventario de construção, sendo uma relação aonde cada cidade vai ser vinculada a N inventarios de construções que estejam dentro da cidade.

Inventario de Construção, o inventario de construção vai herdar do inventario Abstrato, ele vai possuir um sistema de limitação, aonde dependendo do tipo de construção sua capacidade pode ser diferente, além disso cada inventario de construção deve estar vinculado a apenas um inventario de cidade, se essa construção estiver dentro de uma cidade, esse inventario deve passar as informações do que tem dentro dela para o inventario da Cidade, para conseguirmos fazer o sistema de eventos funcionar.

## Atualizações recentes

Inventário da Caravana Militar: herda do Inventário abstrato e guarda Ração Militar, materiais de manutenção de equipamento e munição do exército (ver Sistema de Facção). O Esquadrão tem Equipamento próprio, que não é um inventário de NPC.
Os estoques do Inventário da Cidade alimentam as condições do [[Sistema de Eventos/Descrição|Sistema de Eventos]] (por exemplo, dias em déficit de consumo).
