> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Combate

**Descrição geral**

Define o combate no **Mapa Local** e no **Mapa Interno**, onde o jogador controla o grupo dele e luta contra NPCs e Criaturas. O combate em massa de Esquadrões continua no **Mapa de Guerra** (ver [[Sistema de Mapa/Descrição|Sistema de Mapa]] e [[Sistema de Facção/Descrição|Sistema de Facção]]).

Já estava decidido em outros documentos e vale como base:

- Dano = `Atributo de ataque relevante − Defesa do alvo`, com piso mínimo de 1 ([[Sistema de IA/Descrição|Sistema de IA]]).
- Sem dano por parte do corpo; o **Estado Atual** (Vivo, Ferido, Inconsciente, Morto) deriva da Vida ([[Sistema de NPC/Descrição|Sistema de NPC]]).
- Habilidades de combate: Corpo a Corpo, Longo Alcance, Magia e Defesa.

Princípios:

1. **Um único núcleo de combate.** Jogador, guardas, bandidos e Criaturas usam as mesmas regras. O cálculo rápido de LOD 1 e 2 usa a mesma fórmula, então um combate fora da tela tem resultado coerente com um combate visto.
2. **Profundidade vem do equipamento, do tipo de arma e do preparo**, não de mecânicas pesadas (partes do corpo, projéteis físicos, estamina).
3. **Derrota não é fim de jogo.** O grupo pode ser capturado (ver "Morte e derrota").

---

## Fluxo e controle

### Tempo real com pausa ativa

- O combate é em tempo real, com **pausa ativa** (como RimWorld e Kenshi). A barra de espaço pausa.
- Com o jogo pausado, o jogador dá ordens aos Controlados e elas são executadas ao despausar.
- As velocidades 1x, 2x e 4x continuam valendo (a 8x é exclusiva do Mapa Mundial). Quando um combate começa, o jogo **reduz sozinho para 1x**.

### Ordens

Seleção individual ou em caixa. Ordens básicas:

- mover;
- atacar um alvo;
- manter posição;
- recuar;
- usar item;
- trocar arma.

Habilidades especiais ficam para o sistema de Magia.

### Postura dos Controlados

Cada Controlado tem uma **postura**, usada quando ele não recebeu ordem (estilo Kenshi):

| Postura | Comportamento |
|---|---|
| **Agressiva** | Persegue e ataca |
| **Defensiva** (padrão) | Reage a quem ataca ou chega perto; foge por Medo e Coragem |
| **Manter posição** | Só ataca o que entra no alcance; nunca foge sozinho |
| **Evitar combate** | Foge |

### Estar em combate

O NPC fica **em combate** enquanto tem um hostil dentro de um **raio de alerta**. Sai do estado depois de alguns segundos sem ameaça. Esse estado:

- liga e desliga a regeneração do [[Sistema de Cura/Descrição|Sistema de Cura]] (só regenera fora de combate);
- reduz a velocidade do jogo para 1x ao começar;
- pausa automaticamente em eventos Críticos (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]).

O raio de alerta e o tempo de saída serão definidos em testes. **[Refinar depois]**

---

## Resolução do dano

### Fórmula

Valores iniciais, ajustados em testes:

1. **Dano bruto** = valor sorteado entre o Dano Mínimo e o Máximo da arma × (habilidade ÷ 50). Habilidade 50 dá 100% do dano da arma.
2. **Dano final** = o maior valor entre 1 e (dano bruto − Defesa do alvo).

A habilidade usada é Corpo a Corpo, Longo Alcance ou Magia, conforme a arma. A munição soma o dano dela ao da arma de longo alcance. No cajado vale o atributo Magia.

### Acerto e esquiva

- **Corpo a corpo:** acerta sempre que o alvo está no alcance. O tempo de recarga limita a frequência.
- **Longo alcance:** chance de acerto que **cai com a distância** e sobe com Longo Alcance e Percepção.
- **Sem projéteis físicos.** O acerto é calculado na hora e a animação do projétil é só visual. Isso é barato e evita um sistema de colisão.

### Intervalo de Ataque

A arma ganha o atributo **Intervalo de Ataque** (ver [[Sistema de Itens/Descrição|Sistema de Itens]]). Armas pesadas (Machado, Clava) são lentas e fortes. Lança e Espada são mais rápidas. A Agilidade reduz um pouco o intervalo.

### Vantagem entre arma e armadura

Uma tabela pequena em JSON, com multiplicadores de **no máximo ±25%**, dá identidade a cada tipo de arma sem complexidade.

Valores iniciais:

| Arma | Contra Leve | Contra Média | Contra Pesada |
|---|---|---|---|
| Espada, Lança | +25% | 0 | −25% |
| Clava, Machado | −25% | 0 | +25% |
| Arco | 0 | 0 | 0 |
| Besta e armas de fogo | 0 | 0 | +25% |

A armadura tem **três peças** de pesos que podem ser diferentes. Como não há partes do corpo, vale a classe de peso **mais frequente** entre as três peças (empate: Média). A **Defesa** é a soma das peças.

### Modo letal e não letal

- Um **interruptor por NPC**. Padrão: **letal** contra animais e bandidos, **não letal** contra cidadãos e guardas.
- Atacar guarda ou cidadão gera **crime** ([[Sistema de Crime e Justiça/Descrição|Sistema de Crime e Justiça]]).
- No modo não letal, Vida 0 deixa o alvo **Inconsciente**.
- No modo letal, vale a regra da Cura: o Inconsciente morre por golpe de misericórdia ou por sangramento sem tratamento.

### Medo, fuga e rendição

- O **Medo** sobe com Vida baixa, aliados caindo e inimigo muito mais forte. A **Coragem** (Personalidade) define o limiar.
- Acima do limiar, o NPC **foge**, respeitando a postura (ver acima).
- O NPC que não consegue fugir e tem Medo muito alto **se rende**: vira Prisioneiro ou fica Inconsciente.

---

## Equipamento e preparo

### Slots de arma

- Dois slots de arma: **Principal** e **Secundária** (por exemplo, espada e arco). O **Escudo** é um slot de equipamento.
- Trocar de arma leva cerca de **1 segundo**.
- A IA troca para a arma de longo alcance quando o inimigo está longe e para a corpo a corpo quando ele se aproxima.

### Munição

- Cada tiro gasta **1 unidade** de Munição do inventário, **sem recolher** depois.
- Sem munição, o NPC troca para a arma secundária.
- Quem produz a munição é o Carpinteiro (Flecha e Virote) e o Armeiro (Cartucho e Bala de Canhão), o que cria demanda econômica.

### Peso e armadura

- **Sem estamina ou fadiga.**
- A armadura **Média e a Pesada reduzem a velocidade de movimento**, usando o Peso do item e a Capacidade de peso do NPC.

### Atributo Mínimo

O NPC pode equipar uma arma acima da habilidade dele, mas leva **penalidade proporcional** no dano e no Intervalo de Ataque. Não há trava total.

### Armas e equipamentos encantados

Uma arma (ou equipamento) de **Rank alto** pode ser **encantada**, o que **aumenta os atributos dela**, como o Atributo Mágico, o Dano, a Defesa e o Buff de habilidade (atributos que os itens já têm).

- O encantamento é um **bônus fixo** aplicado ao item, sem gastar Mana na hora do combate.
- Só itens de **Rank 3 ou maior**. Feito na **Encantaria** por um **Mago de Ofício**, gastando Mana e Pedra de Mana.
- Um encantamento por item; um novo substitui o antigo.

Detalhes no [[Sistema de Magia/Descrição|Sistema de Magia]]. **[Refinar depois]** tabela de bônus por Rank e custo.

### Fronteira com Magia

No combate, entra agora só o que já existe: **cajado com Mana** (dano pelo atributo Magia) e **armas e equipamentos encantados**. Os feitiços (instantâneos ou em área, com Mana, conjuração interrompível e recarga; os de área acertam aliados) estão no [[Sistema de Magia/Descrição|Sistema de Magia]].

---

## Evolução

As habilidades de combate dos Controlados evoluem **por uso**, no estilo Kenshi:

- **Corpo a Corpo** e **Longo Alcance** sobem ao acertar golpes.
- **Defesa** sobe ao ser atingido.
- O ganho **diminui** conforme o valor sobe.
- Treino com manequim ou no Quartel (futuro) é um bônus. **[Refinar depois]**

Ver o atributo Evolução em [[Sistema de NPC/Descrição|Sistema de NPC]].

---

## Inimigos

- **NPCs de verdade:** guardas e **bandidos** (função própria; bandidos são Criminosos). Seguem o Sistema de NPC e a IA de combate local.
- **Criaturas (animais e monstros):** entidades **leves**, só com Vida, Dano, Defesa, Velocidade, Comportamento e Loot. Não têm personalidade, rotina nem inventário completo. Não são NPCs.
- Criaturas surgem por eventos (Fauna, ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]) e em masmorras (Mapa Interno).

Masmorras são povoadas por Criaturas e bandidos, com dificuldade de 1 a 5 e Loot de Rank compatível (ver [[Objetivo e Progressão/Descrição|Objetivo e Progressão]]).

---

## Escala do combate

- O combate local é para **grupos pequenos e médios**: cerca de **40 combatentes ativos**, a testar.
- Acima disso, ou quando há **Esquadrões**, o confronto vai para o **Mapa de Guerra**.
- Em **LOD 1 e 2**, os combates entre NPCs usam o **cálculo rápido** com a mesma fórmula de dano.

---

## Morte e derrota

- A **morte** de um Controlado é **permanente**. O jogo continua com os outros.
- Se o **personagem principal** morre, o herdeiro (ver [[Sistema de Facção/Descrição|Sistema de Facção]]) ou outro Controlado assume.
- Se **todo o grupo** cai (Inconsciente), o jogador é **capturado** em vez de ter Game Over. O destino depende de quem venceu:
  - **guardas:** prisão ([[Sistema de Crime e Justiça/Descrição|Sistema de Crime e Justiça]]);
  - **bandidos ou facção:** escravidão, resgate ou morte;
  - **Criaturas:** o grupo acorda ferido no mesmo lugar depois que elas vão embora; perde o Loot carregado e mantém o equipamento.
- **Game Over** só acontece se ninguém puder ser capturado.

---

## Casos especiais

### Duelo

Um modo entre **dois combatentes**: os outros NPCs não intervêm. Termina por **rendição, Inconsciente ou morte**, conforme o acordo (letal ou não). Vencer um duelo vale como "provar-se em combate" no Rito de Admissão ([[Sistema de Cultura/Descrição|Sistema de Cultura]]). Duelo aceito não é crime.

### Montarias

**[Refinar depois]** Fora do combate na primeira versão. A montaria serve para viagem e carga (velocidade e peso): com todos montados, a viagem pelo Mapa Mundial fica 50% mais rápida (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]). A cavalaria em combate existe só no Mapa de Guerra, como tipo de Esquadrão (ver [[Sistema de Batalha/Descrição|Sistema de Batalha]]). O item Sela já existe.

---

## Arte e animação

Toda a arte é feita à mão, então a escolha é feita para o volume de arte não multiplicar por item:

- **Boneco por camadas:** corpo base, 3 peças de armadura, arma e escudo, sobrepostos no mesmo conjunto de animações.
- **Animações compartilhadas por tipo de arma:** Espada, Lança, Machado, Clava, Arco, Cajado, Mosquete e Besta, em **4 direções**.
- Criaturas têm sprites e animações próprios, em número reduzido.
- Boneco de 16 × 24 px, 54 quadros por direção, 3 direções desenhadas e a esquerda espelhada (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).

---

## Desempenho

- O combate roda no **passo fixo** da simulação.
- **Grade espacial** para achar alvos próximos.
- Cada NPC **reavalia o alvo a cada poucos ticks**, não a cada tick.
- Cálculos em lote no ECS.
- O **painel de depuração** (Dear ImGui) mostra o dano calculado e o alvo de cada combatente.
- Aleatoriedade com semente, como no resto do jogo (ver [[Arquitetura/Descrição|Arquitetura]]).

---

## Ligação com outros sistemas

| Sistema | O que o Combate fornece ou recebe |
|---|---|
| NPC | Vida, Estado Atual, habilidades, Postura e Modo de ataque (Controlados); Criaturas fora do modelo de NPC |
| Itens | Intervalo de Ataque; Dano, Alcance e Atributo Mínimo das armas; Defesa e peso das peças; encantamento |
| IA | IA de combate local, Medo e Coragem, troca de arma |
| Cura | Regeneração só fora de combate; Inconsciente e Morto |
| Crime e Justiça | Agressão e assassinato; prisão não letal; captura pelos guardas |
| Facção | Herdeiro assume; Esquadrões vão ao Mapa de Guerra; prisioneiros |
| Batalha | A Comitiva do jogador usa a Vida e o dano reais do combate local; mesma tabela de arma × armadura (ver [[Sistema de Batalha/Descrição\|Sistema de Batalha]]) |
| Cultura | Duelo como rito |
| Mapa | Combate local e Mapa Interno; limite de combatentes; Mapa de Guerra |
| Eventos | Criaturas e bandidos por evento; eventos Críticos pausam |
| Tempo | Velocidade volta a 1x ao iniciar combate |
| Clima e Estações | Chuva, tempestade e névoa reduzem o tiro e a visão; à noite, visão −30% (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Início de Jogo | Derrota vira captura (estilos Escravo e Aventureiro) |
| Magia | Cajado, encantamento e, depois, feitiços |
| Interface | Seleção individual ou em caixa, barras de Vida, indicadores EM COMBATE e PAUSADO, ordens enfileiradas visíveis (ver [[Interface/Descrição\|Interface e Controles]]) |

### Perguntas em aberto

1. **Valores a testar:** fórmula de dano e escala da habilidade, chance de acerto por distância, Intervalo de Ataque, multiplicadores de vantagem, raio de alerta, limiares de Medo, ganho de evolução, limite de combatentes.
2. **Encantamento:** tabela de bônus por Rank e custo (ver Sistema de Magia).
3. **Armadura mista:** confirmar a regra da classe de peso mais frequente.
4. **Itens a cadastrar:** o atributo Intervalo de Ataque nas armas já existentes.
