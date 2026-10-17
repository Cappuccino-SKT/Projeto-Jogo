> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de NPC

**Descrição geral**

Os NPCs são separados, a princípio, em **3 modelos**, cada um com uma função dentro do jogo e cada um dando origem a modelos mais específicos:

| Modelo | O que é |
|---|---|
| **Controlados** | NPCs sobre os quais o jogador tem controle direto, indicando cada ação: os NPCs do grupo principal do jogador, que interagem com o mundo conforme a vontade dele |
| **Semi-Controlados** | NPCs que o jogador possui, mas não controla diretamente: ele dá comandos pré-definidos, e a IA age sozinha |
| **Não-Controlados** | A maioria dos NPCs do jogo. Interagem com o jogador, mas não fazem parte do controle dele; todas as ações vêm da [[Sistema de IA/Descrição\|IA]] |

O atributo booleano **Escravo** (Atributos Gerais) e a categoria **Escravo do jogador** (Semi-Controlado) não são a mesma coisa: um NPC pode ser Escravo sem pertencer ao jogador. Ele continua Não-Controlado, só com o status legal de escravo numa cidade ou facção qualquer, e só vira Semi-Controlado quando é adquirido pelo grupo do jogador ([[Sistema de Escravidão/Descrição|Escravidão]]).

### Modelos derivados

- **Controlados:** até o momento, são os que fazem parte do grupo principal do jogador ([[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]]).
- **Semi-Controlados:**
  - **Escravo do jogador:** escravos vinculados ao grupo principal. Não são controlados diretamente, mas recebem ordens pré-definidas do jogador, que a IA executa.
  - **Exército do jogador:** as tropas do Mapa de Guerra. Nesse mapa as unidades não agem uma a uma, mas em grupo, de forma parecida com o Total War. **Soldados não são NPCs:** ao serem alistados, os NPCs saem da simulação e passam a existir só como parte de um **Esquadrão** (ver abaixo). Guardas continuam NPCs e nunca entram no sistema de soldados.
- **Não-Controlados:** todos os outros. Entre eles, os **Importantes** têm vida individual completa ([[Sistema de População/Descrição|População]]).

### Atributos gerais (todo NPC tem)

1. Nome
2. Idade
3. Estágio de vida (criança 0–3, adulto 4–20 e velho 21+, em anos de jogo; só Importantes e Controlados têm crianças como NPC; ver [[Sistema de População/Descrição|População]])
4. Facção
5. Cultura (o NPC nasce com a cultura da cidade; migrantes e escravos capturados mantêm a cultura de origem; as minorias podem assimilar a cultura dominante, ver [[Sistema de Religião/Descrição|Religião e Fé]])
6. Função (tipo de trabalho: ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]; também guarda, comerciante, caçador de recompensa etc.)
7. Classe Social (a classe a que pertence dentro da facção)
8. **Personalidade**: valores de 0 a 100 de Agressividade, Coragem, Lealdade, Ganância e Sociabilidade, organizados em "modelos" com nome, reutilizáveis (por exemplo, o modelo "Covarde Ganancioso" = Coragem baixa + Ganância alta), para facilitar a criação. Esses valores alimentam direto as decisões da IA
9. Sexo (masculino ou feminino)
10. Rotina
11. Dinheiro (individual só para Importantes, Controlados e Ricos donos de empresas; os demais usam a riqueza média por classe da cidade, ver [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]])
12. Posição atual no mapa
13. Mago (booleana)
14. Escravo (booleana; pode ser temporário, no caso de trabalho forçado)
15. Prisioneiro (booleana)
16. Criminoso (booleana: verdadeira se o NPC tem recompensa em alguma facção)
17. Recompensa (lista de pares facção e valor, só para Criminosos; é por facção e diminui com o tempo; ver [[Sistema de Crime e Justiça/Descrição|Crime e Justiça]])
18. Grávida (booleana; só para Importantes e Controlados do sexo feminino)
19. Vida máxima e atual
20. Mana máxima e atual (só se o atributo Mago estiver ligado)
21. Velocidade
22. Capacidade de peso
23. Equipamento
24. **Estado Atual** (Vivo → Ferido → Inconsciente → Morto), derivado da Vida atual (por exemplo, acima de 30% = Vivo, 1% a 30% = Ferido, 0 = Inconsciente). Sistema simples, sem dano por parte do corpo, pensado para combates com muitos NPCs sem custo alto de desempenho
25. **Nível de Detalhe de Simulação (LOD):** não é calculado por NPC; é herdado do Mapa Local onde o NPC está
26. **Ação atual:** o que o NPC está fazendo agora (dormindo, trabalhando, lutando, viajando). Guia a Rotina e as ordens diretas
27. **Destino ou posição-alvo:** usado pela Rotina dos Não-Controlados (ir à construção de trabalho) e pelas ordens de movimento dos Controlados
28. **Geração de NPC:** ao criar um NPC, os Atributos de Habilidade nascem numa faixa aleatória definida pela combinação de Cultura e Classe Social. A Cultura aplica ainda modificadores pequenos (por exemplo, ±10 a 20) e uma tendência nos modelos de Personalidade. NPCs gerados na dissolução de um exército nascem como Escravo ou Civil
29. **Relação:** lista de vínculos com outros NPCs (por exemplo, filho, cônjuge), só para Importantes, para não gastar memória com NPCs comuns
30. **Opinião:** o que um NPC Importante pensa do jogador, de −100 a +100; só para os Importantes que o jogador já encontrou. Não confundir com Relação ([[Sistema de Reputação/Descrição|Reputação]]). Para os companheiros, a Opinião é a lealdade ([[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]])

### Atributos de habilidade (todo NPC tem)

| Habilidade | Influencia |
|---|---|
| Corpo a Corpo | Dano e capacidade de usar certas armas |
| Longo Alcance | Dano e capacidade de usar certas armas |
| Magia | Dano e capacidade de usar certas magias e armas |
| Defesa | Influenciada pelo equipamento |
| Força | Capacidade de carga e outras habilidades |
| Agilidade | Velocidade de movimento e outras habilidades |
| Destreza | Ocultação e outras habilidades |
| Percepção | Detecção e outras habilidades |
| Carisma | Conversas e outras habilidades |
| Inteligência | Mana e outras habilidades |
| Trabalho Braçal | Certos trabalhos |
| Trabalho Manufaturado | Certos trabalhos |
| Trabalho Especialista | Certos trabalhos |

### Atributos por categoria

**Controlados**

1. Inventário (diferente do dos Não-Controlados; ver [[Sistema de Inventário/Descrição|Inventário]])
2. Evolução (para evoluir os Atributos de Habilidade; as de combate sobem por uso, ver [[Sistema de Combate/Descrição|Combate]])
3. Postura (Agressiva, Defensiva, Manter posição ou Evitar combate; usada quando não há ordem)
4. Modo de ataque (letal ou não letal)
5. Livros lidos, traço Viciado e estado Doente (também nos Importantes; ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]])
6. **Fome** (0 a 100; 100 = satisfeito; também nos Importantes). Cai **100 em 36 horas** de jogo. O NPC come sozinho quando passa de 50, do inventário dele ou do grupo, pela melhor comida. A **Saciedade** da comida devolve, pelo Tier (rodada V5, IT-5): Tier 1 = 60, Tier 2 = 80, Tier 3 = 90 e Tier 4 = 100. Com Fome 0, a Vida cai 2% por hora e a Felicidade cai rápido. Na prática, 1 a 2 refeições por dia (decisão do autor na revisão V1-9)
7. **Felicidade individual** (0 a 100; também nos Importantes), pelos 6 fatores do [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]]
8. **Noites sem dormir** (contador, usado pela marcha forçada da [[Sistema de Viagem/Descrição|Viagem]]). Não há estamina nem atributo de descanso

**Semi-Controlados — Escravo**

1. Tarefa designada: a tarefa que o NPC está executando agora
2. Lista de Prioridades (editável pelo jogador): a ordem das tarefas (por exemplo, 1º Coleta, 2º Produção, 3º Descanso); o escravo sempre tenta a tarefa disponível de maior prioridade
3. Obediência: a chance de seguir a prioridade em vez de resistir (0 a 100; fórmula no [[Sistema de Escravidão/Descrição|Sistema de Escravidão]])
4. Revolta: risco de rebelião, calculado pela Obediência baixa e pela Felicidade baixa dos Escravos, reduzido pelo Controle da cidade
5. Vínculo: o dono ou a facção responsável pelo escravo

**Esquadrão (não é NPC)**

Os atributos que antes eram de "Semi-Controlados — Exército" migraram para o **Esquadrão**, entidade própria ([[Sistema de Facção/Descrição|Facção]]): ID, Formação, Alvo de combate, Moral, Disciplina e Vínculo (jogador ou facção), mais Tamanho (quantidade de soldados), Cidade de origem, Equipamento e Manutenção. Os tipos, o kit, a Experiência e os números de batalha estão no [[Sistema de Batalha/Descrição|Sistema de Batalha]].

**Criatura (não é NPC)**

Animais e monstros são entidades leves, sem personalidade, rotina nem inventário completo: Vida, Dano, Defesa, Velocidade, Comportamento e Loot ([[Sistema de Combate/Descrição|Combate]]).

**Não-Controlados**

1. Inventário (diferente do dos Controlados)
2. Importante (booleana: o NPC tem importância para o mundo; por exemplo, um evento acontece se ele morrer). Os Importantes são sempre ignorados no alistamento e, se capturados na guerra, viram prisioneiros individuais (resgate ou execução)

---

## Parte técnica

- **Memória:** os atributos individuais caros (Relação, Opinião, Fome, Felicidade individual, livros lidos, Viciado, Doente, Dinheiro) só existem para **Controlados e Importantes**; os NPCs comuns usam os valores agregados da cidade.
- **ECS:** cada grupo de atributos é um componente; um NPC comum não carrega os componentes dos Controlados ([[Arquitetura/Descrição|Arquitetura]]).
- **LOD:** herdado do Mapa Local; em LOD 2 os NPCs comuns não se movem nem decidem, só entram nas contas agregadas.
- **Save:** NPCs reais de cada cidade com os atributos que têm; Importantes e Controlados completos.
- **Fatia vertical:** NPCs comuns com rotina, Função, Classe e habilidades de Trabalho; 1 Controlado com inventário, Fome e Felicidade.

## Ligação com outros sistemas

| Sistema | O que o NPC fornece ou recebe |
|---|---|
| IA | Rotina, Personalidade, Medo; decisões por Utilidade ou Regras Fixas |
| Classe Social e Cultura | Faixas de habilidade na Geração de NPC; tendência de Personalidade |
| População | Estágio de vida, nascimentos e mortes; Importantes com vida completa |
| Trabalho e Produção | Função e habilidades de Trabalho |
| Combate e Cura | Vida, Estado Atual, Postura, Modo de ataque, regeneração |
| Facção e Guerra | Alistamento cria Esquadrões; guardas ficam de fora |
| Crime e Justiça | Criminoso, Prisioneiro, Recompensa |
| Escravidão | Escravo, Obediência, Revolta, Vínculo |
| Companheiros e Reputação | Opinião, Relação, casamento e herdeiros |
| Felicidade | Felicidade individual dos Controlados e Importantes |
| Efeitos de Itens | Fome e Saciedade; livros lidos; Viciado; Doente |
| Magia | Mago, Mana |

## Perguntas em aberto

1. **Vida e Mana base** e quanto cada habilidade as muda (rodada V2 ou testes do Combate).
2. **Saciedade** de cada comida e a velocidade da Fome (valores a testar).
