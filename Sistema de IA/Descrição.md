
## Sistema de IA

### Rotina

Um NPC Não-Controlado tem suas ações influenciadas pelo sistema de rotina do jogo. A rotina é um ciclo diário de 24 horas, dividido em 3 partes: período de dormir, período de trabalhar e período livre. Cada classe social tem uma distribuição diferente desses períodos ao longo do dia; funções específicas também têm distribuições diferenciadas.

Cada período tem uma função tanto para o NPC quanto para a própria gameplay:

- **Período de Dormir** — normalmente ocorre durante a madrugada. Sua principal função é fazer os NPCs recolherem-se em suas casas para dormir — para o jogador, é o melhor momento para ações de crime. Quem exerce a função de guarda não possui esse período, permanecendo em atividade o dia inteiro para combater o crime.
- **Período de Trabalho** — os NPCs vão às construções relacionadas à sua função, para produzir recursos para a cidade. Cada cidadão tem um serviço diferente, e cada um procura uma construção correspondente.
- **Período Livre** — serve principalmente para movimentar a atividade econômica da cidade: os NPCs frequentam lojas. Também serve como ambientação, com os NPCs circulando tranquilamente pela cidade e frequentando tavernas e outras construções de entretenimento.

Dependendo da função, a rotina do NPC pode deixar de depender dos períodos do dia e se tornar contínua — como os guardas, que trabalham 24 horas fazendo rondas e mantendo a segurança das cidades.

### IA de Combate Local

O início da agressividade da IA no Mapa Local depende de gatilhos e ações do jogador — por exemplo: atacar um NPC, roubar, insultar, entre outros. Esses gatilhos iniciam uma ação hostil por parte do NPC afetado.

Durante o combate, a IA ataca alvos individuais, priorizando os mais próximos. Após incapacitar o alvo, procura um novo alvo hostil próximo, até que as ameaças cessem. O dano causado é calculado como `Atributo de ataque relevante (Corpo a Corpo / Longo Alcance / Magia) − Defesa do alvo`, com um piso mínimo (ex: sempre pelo menos 1) para evitar combates que nunca terminam entre dois NPCs com Defesa muito alta.

Dependendo da função do NPC, o gatilho de início de combate é diferente — por exemplo, guardas podem se tornar hostis mesmo sem relação direta com a situação, agindo para pacificá-la.

O sistema de decisão é híbrido, ligado ao LOD do Mapa Local: NPCs em LOD 0 (visão do jogador) usam um sistema de **Utilidade**, pesando Bens de Consumo, Felicidade, Medo e Personalidade ao mesmo tempo pra escolher a melhor ação. NPCs em LOD 1/2 usam **Regras Fixas**, mais baratas de processar. Isso conecta diretamente com a agregação por cidade usada nos Não-Controlados: LOD 2 é exatamente o caso onde essa agregação se aplica.

### Comportamento por Necessidade — Controlados

- **Fome** — ao se aproximar de 0, a IA busca ou consome alimento do inventário.
- **Felicidade** — abaixo de 20 o jogador recebe um aviso; ao chegar a 0, o NPC pede para sair do grupo (só vai embora depois de alguns dias, dando chance de resolver por diálogo ou pagamento), migrar ou se revoltar (ver [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]]).
- **Medo** — seu uso varia conforme a função: para Escravos, valores altos ajudam a prevenir revoltas; para NPCs normais, decide quando recuar, se render ou fugir.

Esses atributos são individuais para cada NPC que faça parte do grupo do jogador ou que tenha interação próxima com ele.

### Comportamento por Necessidade — Não-Controlados

Para a IA geral, esses atributos são usados de forma agregada, vinculados à cidade, em vez de calculados individualmente por NPC — o que economiza processamento. Dependendo da situação desses atributos, eventos são desencadeados sem necessidade de calcular a necessidade de cada NPC dentro da cidade.

- **Bens de Consumo** — inclui Comida e demais recursos exigidos pela classe social do NPC (ver Sistema de Classe Social). Calculado pela quantidade desses itens no inventário da cidade, em proporção ao número de NPCs. A falta ou diminuição do estoque causa eventos de reação dos NPCs, variando conforme a classe afetada. Substitui a antiga Fome como atributo separado, já que toda classe social já exige Comida como parte do seu consumo.
- **Felicidade** — valor influenciado por diversos fatores em conjunto, guardado por classe social em cada cidade. Sua falta desencadeia principalmente eventos políticos (ver [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]]).
- **Medo** — usado de forma variada conforme a situação; de base, decide se o NPC deve fugir ou continuar em combate.

---

### Ligações com outros sistemas

- **IA de Facção:** decide diplomacia, guerra, resposta a revoltas e disputas de mapa em baixa frequência (uma vez por dia ou por estação), usando a Personalidade do líder (ver Sistema de Facção).
- **Guardas:** reação escalonada ao crime (ordem de rendição, prisão não letal, força letal); a detecção usa Percepção e Destreza, e o suborno usa Ganância e Lealdade (ver Sistema de Crime e Justiça).
- **Esquadrões:** não usam a IA individual de NPC; no Mapa de Guerra são comandados por esquadrão. Guerras entre facções de IA são resolvidas por um cálculo rápido.
- **Recuperação de Vida:** ver Sistema de Cura.
- **Eventos:** nos eventos com escolha que afetam facções de IA, a decisão sai da Personalidade do líder (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]).
- **Combate:** o núcleo de combate, as posturas dos Controlados, o Medo e a rendição estão no [[Sistema de Combate/Descrição|Sistema de Combate]]. A IA troca a arma conforme a distância do inimigo.
