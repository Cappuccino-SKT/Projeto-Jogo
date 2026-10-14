> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Magia

**Descrição geral**

A magia é rara e poderosa. Ela aparece no combate (feitiços, cajados), na produção (Forja Arcana, Encantaria), na cura e na guerra. O preço dela não é o risco de falhar, e sim o **custo social e político**: leis, discriminação, crime e a reação de culturas hostis.

Já existe nos outros sistemas:

- **NPC:** booleana **Mago**, **Mana** máxima/atual (só para Magos), habilidade **Magia** e **Inteligência** (influencia a Mana). Ver [[Sistema de NPC/Descrição|Sistema de NPC]].
- **Itens:** cajados que gastam Mana, Atributo Mágico, Pedra Mágica, Pedra de Mana Bruta e Aço Mágico. Ver [[Sistema de Itens/Descrição|Sistema de Itens]].
- **Combate:** cajado com Mana e itens encantados. Ver [[Sistema de Combate/Descrição|Sistema de Combate]].
- **Facção, Cultura e Crime:** lei de Magia (livre / restrita / proibida, com "privilégio dos magos"); Arcana dá privilégio aos Magos, Tecnocrata os bane, Religiosa restringe; magia é crime na Tecnocrata.

---

## Magos

- Magos são **raros**: cerca de **5%** da população nasce Mago, e **15%** na cultura Arcana.
- **Filho de Mago** tem chance maior de nascer Mago.
- Ser Mago não se aprende: é a booleana do NPC, definida no nascimento.
- **[Refinar depois]** chance exata para filhos de Mago e como o Fator de Escala afeta a contagem em LOD 2.

## Escolas

| Escola | Faz | Exemplos |
|---|---|---|
| **Combate** | Dano e controle | Dano em área, lentidão, atordoamento |
| **Suporte** | Cura e bônus | Cura em combate, bônus de atributo, acelerar a cura no Hospital |
| **Ofício** | Encantamento e magia na produção | Encantar itens, trabalhar na Forja Arcana e na Encantaria |

Cada Mago tem **afinidade com uma escola** (sorteada ao nascer). Ele pode aprender as outras, mas evolui nelas mais devagar.

## Feitiços

### Como se aprende

1. A **pesquisa arcana** da facção libera o feitiço (ver "Pesquisa arcana").
2. O Mago o aprende com um **Grimório** (item) ou com um **mestre** (outro Mago que já o conhece).
3. O **número de feitiços** que um Mago conhece depende da **Inteligência**.

### Mana

- Regenera com o tempo e **mais rápido em descanso**, como a Vida (ver [[Sistema de Cura/Descrição|Sistema de Cura]]).
- A **Pedra de Mana** pode ser consumida para recuperar Mana. Não há poção de mana separada.

### No combate

- Seguem a regra do combate de **não ter projéteis físicos**: o efeito é **instantâneo** no alvo ou numa **área**.
- Cada feitiço tem **custo de Mana**, **tempo de conjuração** e **recarga**.
- A conjuração é **interrompida** se o Mago levar dano.
- Feitiços de área **acertam aliados também**, o que dá peso à pausa ativa.
- **Sem falha aleatória:** se a conjuração termina, o feitiço funciona.
- O dano e a força dos feitiços usam a habilidade **Magia**, como o cajado.

## Magia fora do combate

| Onde | O que faz |
|---|---|
| **Forja Arcana** | Construção de produção onde Magos de Ofício produzem **Aço Mágico** |
| **Encantaria** | Construção onde Magos de Ofício encantam itens |
| **Hospital** | Um Mago de Suporte trabalhando acelera a cura |
| **Torre Arcana** | Gera Pontos Arcanos (pesquisa) |

## Encantamento

- Só itens de **Rank 3 ou maior**.
- Feito na **Encantaria** por um **Mago de Ofício**.
- Custa **Mana** e **Pedra de Mana**.
- O encantamento é um **bônus fixo** no item (Atributo Mágico, Dano, Defesa ou Buff de habilidade), sem gastar Mana no combate.
- **Um encantamento por item.** Um novo encantamento **substitui** o antigo.
- **[Refinar depois]** tabela de bônus por Rank e custo de cada encantamento.

## Pesquisa arcana

- A **Torre Arcana** gera **Pontos Arcanos**, com Magos trabalhando nela. Mesma mecânica dos Pontos de Pesquisa (ver [[Sistema de Tecnologia/Descrição|Sistema de Tecnologia]]).
- Árvore própria e menor: **15 a 25** feitiços e técnicas (novos encantamentos, receitas da Forja Arcana).
- As eras da árvore arcana também dependem dos **marcos mundiais**.
- A Tecnocrata não pesquisa magia; a Arcana pesquisa tecnologia e magia, mais devagar na tecnologia.

## Magos na guerra

- **Esquadrão de Magos:** tipo especial de Esquadrão, **caro** e limitado pela **quantidade de Magos** da facção (ver [[Sistema de Facção/Descrição|Sistema de Facção]]).
- Só existe se a **lei de Magia** permitir.
- **[Refinar depois]** como ele age no Mapa de Guerra e no cálculo rápido de forças.

## Risco e custo social

- Não existe falha ou acidente aleatório ao conjurar.
- O custo é social e político: a lei de Magia da facção, a discriminação na cultura Arcana, o crime de magia na Tecnocrata, a restrição na Religiosa e a reação de culturas hostis (ver [[Sistema de Cultura/Descrição|Sistema de Cultura]] e [[Sistema de Crime e Justiça/Descrição|Sistema de Crime e Justiça]]).

---

## Parte técnica

- **Dados:** feitiços, escolas e encantamentos em JSON na pasta `data/`, com validação ao carregar.
- **LOD:** em LOD 2, Magos só contam para produção (Forja Arcana, Encantaria, Torre Arcana) e para o tamanho possível dos Esquadrões de Magos.
- **Save:** feitiços conhecidos e afinidade de cada Mago, Mana atual, recargas ativas e pesquisa arcana de cada facção.
- A fatia vertical **não tem magia**.

### Ligação com outros sistemas

| Sistema | Ligação |
|---|---|
| NPC | Mago, Mana, habilidade Magia, Inteligência, afinidade de escola |
| Combate | Feitiços com conjuração, recarga e área; cajado; itens encantados |
| Itens | Grimório, Pedra de Mana, Aço Mágico, encantamentos |
| Construção / Produção | Torre Arcana, Forja Arcana, Encantaria |
| Cura | Regeneração de Mana; Mago de Suporte no Hospital |
| Tecnologia | Mesma mecânica de pontos; eras pelos marcos mundiais |
| Facção | Lei de Magia; Esquadrão de Magos |
| Cultura / Crime | Arcana, Tecnocrata, Religiosa; crime de magia |

### Perguntas em aberto

1. **Lista inicial de feitiços** por escola e o que a árvore arcana libera.
2. **Valores a testar:** custo de Mana, conjuração, recarga, regeneração, bônus de encantamento, quantos feitiços por Inteligência.
3. **Grimório:** como é obtido (comprado, produzido, achado em masmorras).
4. **Esquadrão de Magos:** quantos Magos por esquadrão e como age no Mapa de Guerra.
5. **Feitiços de Suporte fora do combate** além do Hospital (por exemplo, colheita, clima).
