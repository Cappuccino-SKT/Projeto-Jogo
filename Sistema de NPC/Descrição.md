## Sistema de NPC

**Descrição geral**

Os NPCs são separados, a princípio, em 3 modelos — Controlados, Semi-Controlados e Não-Controlados — cada um com uma função dentro do jogo, e cada um derivando modelos mais específicos.

- **Controlados** — NPCs sobre os quais o jogador tem controle direto, indicando cada ação. Usado nos NPCs que fazem parte do grupo principal do jogador, interagindo com o mundo de acordo com sua vontade.
- **Semi-Controlados** — NPCs que o jogador possui, mas não controla diretamente. O jogador dá comandos pré-definidos, e a IA toma as ações automaticamente.
- **Não-Controlados** — a maioria dos NPCs do jogo. Interagem com o jogador, mas não fazem parte do seu controle; todas as suas ações são feitas pelo sistema de IA.

O atributo booleano **Escravo** (Atributos Gerais) e a categoria **Escravo do jogador** (Semi-Controlado) não são a mesma coisa: um NPC pode ser Escravo (booleano = verdadeiro) sem pertencer ao jogador — ele continua Não-Controlado, apenas com status legal de escravo dentro de uma cidade/facção qualquer. Só vira Semi-Controlado quando é especificamente adquirido pelo grupo do jogador.

### Modelos derivados

**Controlados** Até o momento, os Controlados são aqueles que fazem parte do grupo principal do jogador.

**Semi-Controlados** Atualmente separados em dois grupos:

- **Escravo do jogador** — grupo de escravos vinculados ao grupo principal. Não são controlados diretamente, mas recebem ordens pré-definidas do jogador, que a IA executa.
- **Exército do jogador** — tropas usadas no Mapa de Guerra. Como o sistema de guerra é diferente do sistema principal, é necessário um controle diferente: nesse mapa as unidades não tomam ações individuais, mas em grupo — de forma parecida com o Total War. **Soldados de guerra não são mais NPCs**: ao serem alistados, os NPCs saem da simulação e passam a existir só como parte de um **Esquadrão** (entidade própria, descrita no Sistema de Facção). Guardas continuam NPCs e nunca entram no sistema de soldados.

**Não-Controlados** Sendo a maioria dos NPCs e com ações feitas completamente pela IA, sua quantidade de atributos é consideravelmente reduzida, para facilitar o processamento do jogo. Englobam basicamente todos os NPCs que o jogador não consegue controlar de nenhuma forma.

### Atributos Gerais (todo NPC possui)

1. Nome
2. Idade
3. Estágio (estágio de vida: criança, adulto, velho; em anos de jogo, criança 0–3, adulto 4–20 e velho 21+; só Importantes e Controlados têm crianças como NPC, ver [[Sistema de População/Descrição|Sistema de População]])
4. Facção
5. Cultura (o NPC nasce com a cultura da cidade; migrantes e escravos capturados mantêm a cultura de origem; as minorias podem assimilar a cultura dominante, ver [[Sistema de Religião/Descrição|Religião e Fé]])
6. Função (tipo de trabalho: ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]; também guarda, comerciante, caçador de recompensa etc.)
7. Classe Social (a qual classe pertence dentro da facção)
8. Personalidade — conjunto de valores numéricos (0–100): Agressividade, Coragem, Lealdade, Ganância e Sociabilidade, organizados em "modelos" nomeados e reutilizáveis (ex: modelo "Covarde Ganancioso" = Coragem baixa + Ganância alta) para facilitar autoria. Esses valores alimentam diretamente as decisões da IA.
9. Sexo (masculino ou feminino)
10. Rotina
11. Dinheiro (individual só para Importantes, Controlados e Ricos donos de empresas; os demais usam a riqueza média por classe da cidade, ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]])
12. Posição atual (posição no mapa)
13. Mago (booleana)
14. Escravo (booleana — pode ser temporário, no caso de trabalho forçado)
15. Prisioneiro (booleana)
16. Criminoso (booleana — verdadeira se o NPC tem recompensa em alguma facção)
17. Recompensa (lista de pares facção/valor, só para Criminosos — a recompensa é por facção e diminui com o tempo; ver Sistema de Crime e Justiça)
18. Grávida (booleana, aplicável apenas ao sexo feminino; só para Importantes e Controlados)
19. Vida máxima / atual
20. Mana máxima / atual (só existe se o atributo Mago estiver habilitado)
21. Velocidade
22. Capacidade de peso
23. Equipamento
24. **Estado Atual** (Vivo → Ferido → Inconsciente → Morto) — derivado da Vida atual (ex: acima de 30% = Vivo, 1–30% = Ferido, 0 = Inconsciente). Sistema simples (sem dano por parte do corpo), pensado para suportar combates com muitos NPCs simultâneos sem custo alto de performance.
25. **Nível de Detalhe de Simulação (LOD)** — não calculado por NPC individualmente; herdado do Mapa Local onde o NPC está.
26. **Ação atual** — o que o NPC está fazendo agora (dormindo, trabalhando, lutando, viajando). Guia tanto a Rotina quanto ordens diretas.
27. **Destino/Posição-alvo** — usado tanto pela Rotina dos Não-Controlados (ir até a construção de trabalho) quanto por ordens de movimento dos Controlados.
28. **Geração de NPC** — ao criar um NPC novo, os Atributos de Habilidade nascem dentro de uma faixa aleatória definida pela combinação de Cultura e Classe Social. A Cultura aplica ainda modificadores pequenos (por exemplo, ±10 a 20) nas faixas de habilidade e uma tendência nos modelos de Personalidade. NPCs gerados na dissolução de um exército nascem como Escravo ou Civil.
29. **Relação** — lista de vínculos com outros NPCs (ex: filho, cônjuge), restrita a NPCs marcados como Importante, para não gastar memória com isso em NPCs comuns.
30. **Opinião** — o que um NPC Importante pensa do jogador, de −100 a +100; só existe para os Importantes que o jogador já encontrou. Não confundir com Relação (ver [[Sistema de Reputação/Descrição|Sistema de Reputação]]). Para os companheiros, a Opinião é a lealdade (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).

### Atributos de Habilidade (todo NPC possui)

1. Corpo a Corpo — influencia o dano e a capacidade de usar certas armas
2. Longo Alcance — influencia o dano e a capacidade de usar certas armas
3. Magia — influencia o dano e a capacidade de usar certas magias e armas
4. Defesa — influenciada por equipamento
5. Força — influencia a capacidade de carga e outras habilidades
6. Agilidade — influencia a velocidade de movimento e outras habilidades
7. Destreza — influencia a ocultação e outras habilidades
8. Percepção — influencia a detecção e outras habilidades
9. Carisma — influencia as conversas e outras habilidades
10. Inteligência — influencia a mana e outras habilidades
11. Trabalho Braçal — influencia certos trabalhos
12. Trabalho Manufaturado — influencia certos trabalhos
13. Trabalho Especialista — influencia certos trabalhos

### Atributos por categoria

**Controlados**

1. Inventário (sistema diferente do inventário dos Não-Controlados)
2. Evolução (serve para evoluir os Atributos de Habilidade; as de combate sobem por uso, ver [[Sistema de Combate/Descrição|Sistema de Combate]])
3. Postura (Agressiva, Defensiva, Manter posição ou Evitar combate; usada quando não há ordem)
4. Modo de ataque (letal ou não letal)
5. Livros lidos, traço Viciado e estado Doente (também nos Importantes; ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]])

**Semi-Controlados — Escravo**

1. Tarefa designada — a tarefa que o NPC está executando agora.
2. Lista de Prioridades (editável pelo jogador) — ordem de tarefas (ex: 1º Coleta, 2º Produção, 3º Descanso); o escravo sempre tenta executar a disponível de maior prioridade.
3. Obediência — probabilidade de seguir a prioridade definida vs. resistir (0 a 100; fórmula no [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]).
4. Revolta — risco de rebelião, calculável a partir de Obediência baixa + Felicidade baixa dos Escravos, reduzido pelo Controle da cidade (ver [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]]).
5. Vínculo — dono ou facção responsável por esse escravo.

**Esquadrão (não é NPC)**

Os atributos que antes pertenciam a "Semi-Controlados — Exército" migraram para o **Esquadrão**, entidade própria (ver Sistema de Facção): ID do Esquadrão, Formação de combate, Alvo de combate, Moral do grupo, Disciplina do grupo e Vínculo (jogador ou facção). Também ganhou: Tamanho (quantidade de soldados), Cidade de origem, Equipamento e Manutenção. Os tipos, o kit, a Experiência e os números de batalha estão no [[Sistema de Batalha/Descrição|Sistema de Batalha]].

**Criatura (não é NPC)**

Animais e monstros são entidades leves, sem personalidade, rotina nem inventário completo: Vida, Dano, Defesa, Velocidade, Comportamento e Loot (ver [[Sistema de Combate/Descrição|Sistema de Combate]]).

**Não-Controlados**

1. Inventário (sistema diferente do inventário dos Controlados)
2. Importante (booleana indicando se o NPC tem importância para o mundo — ex: um evento acontece se ele morrer). NPCs Importantes são sempre ignorados no alistamento e, se capturados em guerra, viram prisioneiros individuais (resgate ou execução)

---