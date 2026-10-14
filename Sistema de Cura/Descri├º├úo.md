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
- A regeneração só acontece fora de combate.

### Medicamentos

- O tipo de item **Medicamento** já existe (Bem de Consumo), mas **ainda não há itens**.
- Quem produz é o **Boticário** (Produção Especialista), em uma construção Boticário que ainda será criada.
- **[Proposta]** Itens iniciais, a definir com Tier e receita: um curativo simples (Tier 1) e um medicamento mais forte (Tier 3), que usa Flor Espiritual.

### Hospital

Construção futura. **[Proposta]** Cura os NPCs Controlados do grupo mediante pagamento, com velocidade que depende do nível da construção. Em LOD 2, funciona como a Taverna e o Mercado: serviço agregado para a cidade.

### Estado Inconsciente e Morte

O guarda deixa o alvo **Inconsciente** em vez de Morto na prisão não letal (ver Sistema de Crime e Justiça), então o Inconsciente precisa ser um estado estável.

**[Em aberto]** Quando o Inconsciente vira Morto? **[Proposta]**

- O Inconsciente **não morre sozinho** em combate não letal.
- Morre por **golpe de misericórdia** (um ataque letal intencional contra o alvo) ou por **sangramento**, se o combate for letal e ele ficar sem tratamento por um número de horas.
- O Inconsciente acorda depois de um tempo, com Vida baixa.

### Fora de escopo por enquanto

Ferimentos por parte do corpo.

**Doenças e epidemias** agora entram como eventos ocasionais da categoria Saúde, em geral Duradouros (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]). A chance aumenta com população densa, pouca comida e ausência de Hospital ou Boticário. Medicamento e Hospital funcionam como resposta a elas. **[Em aberto]** Como a doença aparece em LOD 0: estado "Doente" no NPC ou só modificador da cidade.

### Ligação com outros sistemas

| Sistema | O que a Cura fornece ou recebe |
|---|---|
| NPC | Vida atual e Estado Atual; Ação atual "descansando" |
| Itens | Tipo Medicamento (itens a criar) |
| Produção | Boticário (função e construção a criar) |
| Construção | Hospital e Boticário (futuros) |
| Crime e Justiça | Estado Inconsciente na prisão não letal |
| Tempo | Regeneração por hora; Período de Dormir |
| Eventos | Doenças e epidemias como eventos de Saúde |

### Perguntas em aberto

1. Quando o Inconsciente vira Morto (proposta acima).
2. Itens de Medicamento e suas receitas.
3. Taxa de regeneração e custo do Hospital (testes).
4. Doença em LOD 0: estado "Doente" do NPC ou só modificador da cidade (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]).

### Atualizações de combate

- **Em combate:** a regeneração só acontece fora de combate; "em combate" é definido no [[Sistema de Combate/Descrição|Sistema de Combate]] (hostil dentro do raio de alerta).
- **Letal e não letal:** o Inconsciente só morre por golpe de misericórdia ou sangramento quando o combate é **letal**. No modo não letal, Vida 0 deixa o alvo Inconsciente e ele não morre sozinho.
- **Derrota para Criaturas:** **[Em aberto]**, ver o [[Sistema de Combate/Descrição|Sistema de Combate]].
