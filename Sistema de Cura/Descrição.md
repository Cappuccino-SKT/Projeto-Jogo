> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Cura

**Descrição geral**

Define como os NPCs se recuperam de ferimentos. Hoje existe o **Estado Atual** (Vivo → Ferido → Inconsciente → Morto, derivado da Vida atual), o tipo de item **Medicamento** e a função **Boticário**, mas nenhuma regra de recuperação. Este sistema é propositalmente curto e simples, coerente com o combate sem dano por parte do corpo, pensado para muitos NPCs ao mesmo tempo.

### Regras

1. **Regeneração por descanso.** A Vida regenera com o tempo quando o NPC descansa.
2. **Medicamento acelera.** Usar um Medicamento acelera a regeneração.
3. **Hospital cura o grupo.** O Hospital (construção futura) cura os NPCs do grupo do jogador.

### Regeneração por descanso

**[Proposta]**

- Taxa base de regeneração por hora de jogo, maior enquanto o NPC dorme (Período de Dormir) ou está na Ação "descansando".
- Em LOD 2, a regeneração é calculada em bloco (por hora de jogo), sem simular NPC a NPC.
- A regeneração só acontece fora de combate. "Em combate" é definido no [[Sistema de Combate/Descrição|Sistema de Combate]] (hostil dentro do raio de alerta).

### Medicamentos

- O tipo de item **Medicamento** já existe (Bem de Consumo), mas **ainda não há itens**.
- Quem produz é o **Boticário** (Produção Especialista), em uma construção Boticário (ver Sistema de Construção).
- Itens: **Curativo** (Tier 1; Tecido de Algodão + Água): regeneração × 2 por 8 horas e estanca o sangramento; **Remédio** (Tier 3; Flor Espiritual + Especiarias): regeneração × 3 por 24 horas e cura o estado Doente (ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).
- **Na cidade:** numa epidemia, a cidade gasta 1 Remédio por 10 NPCs reais por dia; com estoque, a mortalidade da doença cai pela metade.

### Hospital

Construção (ver Sistema de Construção). **[Proposta]** Cura os NPCs Controlados do grupo mediante pagamento, com velocidade que depende do nível da construção. Em LOD 2, funciona como a Taverna e o Mercado: serviço agregado para a cidade. Um Mago de Suporte trabalhando nele acelera a cura (ver Sistema de Magia).

### Templo da Religiosa

Nas cidades Religiosas, o Templo cuida dos feridos pobres: a cura dos NPCs comuns fica **25% mais rápida**, e a mortalidade por doença cai 25% (ver [[Sistema de Religião/Descrição|Religião e Fé]]).

### Estado Inconsciente e Morte

O guarda deixa o alvo **Inconsciente** em vez de Morto na prisão não letal (ver Sistema de Crime e Justiça), então o Inconsciente precisa ser um estado estável.

**Quando o Inconsciente vira Morto** (proposta confirmada na rodada V3-4, com números):

- O Inconsciente **não morre sozinho** em combate não letal. No modo não letal (padrão contra cidadãos e guardas), Vida 0 deixa o alvo Inconsciente. O modo letal e o não letal estão no [[Sistema de Combate/Descrição|Sistema de Combate]].
- Morre por **golpe de misericórdia** (um ataque letal intencional contra o alvo) ou por **sangramento**, se o combate for letal e ele ficar sem tratamento por um número de horas.
- O Inconsciente acorda depois de um tempo, com Vida baixa.
- **Números:**
  - **não letal:** o Inconsciente não sangra e acorda em **4 horas de jogo**, com 10% da Vida;
  - **letal:** ele **sangra** e morre em **2 horas de jogo** (80 s reais a 1x) sem Curativo;
  - **golpe de misericórdia:** mata na hora e é crime de Assassinato, se for contra um cidadão.
- **Hospital:** cura 5% da Vida por hora de jogo (10% com um Mago de Suporte) e custa 2 moedas por dia de internação.
- Se o grupo do jogador é derrotado por Criaturas, ele acorda ferido no mesmo lugar depois que elas vão embora (ver Objetivo e Progressão).

### Fora de escopo por enquanto

Ferimentos por parte do corpo.

**Doenças e epidemias** agora entram como eventos ocasionais da categoria Saúde, em geral Duradouros (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]). A chance aumenta com população densa, pouca comida e ausência de Hospital ou Boticário. Medicamento e Hospital funcionam como resposta a elas. **Doença em LOD 0:** nos NPCs comuns, é só um modificador da cidade; nos **Controlados e Importantes**, existe o estado **Doente** (−20% nas habilidades e sem regeneração, até tomar Remédio, ir ao Hospital ou passar 8 dias). Ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]].

### Ligação com outros sistemas

| Sistema | O que a Cura fornece ou recebe |
|---|---|
| NPC | Vida atual e Estado Atual; Ação atual "descansando" |
| Itens | Curativo e Remédio; estado Doente (ver [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]]) |
| Produção | Boticário (função e construção) |
| Construção | Hospital e Boticário |
| Crime e Justiça | Estado Inconsciente na prisão não letal |
| Tempo | Regeneração por hora; Período de Dormir |
| Eventos | Doenças e epidemias como eventos de Saúde |
| Religião | Templo da Religiosa acelera a cura e reduz as mortes por doença (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |

### Perguntas em aberto

1. **Valores a testar:** 4 h para acordar, 2 h de sangramento, cura e custo do Hospital (rodada V3-4).
2. Taxa de regeneração fora do Hospital (testes).
