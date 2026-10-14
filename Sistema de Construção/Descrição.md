Teremos diversos tipos de construção cada uma tem um objetivo. temos um separação entre construção que produz recursos tangiveis, e construção que produz coisas não tangiveis, exemplo.
Uma mina produz minerais recursos tangiveis pelo player, já uma laboratorio produz pesquisa, algo não tangivel pelo player mas que afeta a cidade ou o mundo.

Atributos
- Nome da construção
- Vida maxima/Atual
- Nivel
- Recursos para construção
- Recursos que precisam existir no mapa para permitir a contrução
- Nivel de inteligencia para permitir a construção (isso é como se construções avançadas precisasem de um engenheiro para monitorar a obra).
- Tempo de construção
- Trabalhadores construindo (Afeta o tempo para a construção terminar)
- Gerente
- Empregados contratados
- Empregados trabalhando no momentos
- Inventario da construção
- Lista do que pode ser produzido na construção
- Lista do que esta desbloqueado para produzir
- Produção atual
- Tempo até terminar de produzir
- Manutenção por Estação (Gasta uma quantidade de recursos a cada estação, 16 dias, para manter a construção. Esse gasto é pago pela cidade e exige um Construtor na cidade, que fornece a manutenção)
- Nivel de LOD
- Ilegal (booleana: loja ilegal disfarçada) e Confiança do gerente com o jogador

Temos que fazer com que o LOD nas construções funcione de forma eficiente, para não termos grande perda de desempenho na gameplay, a ideia seria apenas em construções que pertenção ao player e que a visão do player esteja sobre a construção que todos os dados devem ser usados, em situações aonde a construção não pertence ao player, ou aonde o player não esteja visualizando, apenas podemos processar as informação de produção, podemos processar oque o as construções estão construindo e fazer um calculo por estação (16 dias) aonde a cada estação a construção gera uma quantidade de produtos de acordo com o valor que ela geraria normalmente, assim podemos economizar o processamento, junto a isso para construção que não seja do player a decisão do que a construção vai produzir vai ficar de acordo com a demanda da cidade que ela pertença.

Para as construções com nivel a ideia é que quanto maior o nivel maior a eficiencia da construção juntamente com a capacidade de produzir novas coisas se for uma construção de produção. entretando quanto maior o nivel maior o custo para se construir e também o gasto de dinheiro para se manter a manutenção.

As construções podem ser construidas de nivel por nivel, com dessa forma você recebe um desconto para construir o proximo nivel, caso queira construir diretamente um nivel alto, você pagara o custo integral da construção.

As construções podem ser construidas manualmente pelos NPCs do jogador, mas caso haja uma construtora, ela pode construir automaticamente os projetos do jogador ou da propria cidade.

### Atualizações recentes

- **Sem meses no jogo:** a manutenção e o cálculo de produção das construções em LOD 2 acontecem **por estação** (16 dias).
- A manutenção é fornecida pelo **Construtor**: a cidade precisa ter um para manter as construções.
- Novos atributos: **Ilegal** (loja ilegal disfarçada) e **Confiança** do gerente com o jogador.

### Loja ilegal

**Qualquer construção de produção pode ser uma loja ilegal.** Ela se passa por outra coisa, mas compra e vende produtos ilegais. O jogador só passa a negociar nela depois de um evento ou missão em que o NPC ganha confiança nele. Ver Sistema de Crime e Justiça.

### Construções a acrescentar (a detalhar nos canvases)

Construções que o resto do projeto já exige e ainda não existem nos canvases. Custos e níveis ainda não foram definidos.

| Construção | Função |
|---|---|
| **Mercado** | Loja geral onde os NPCs compram no Período Livre (a "Loja" do Sistema Econômico) |
| **Taverna** | Entretenimento no Período Livre; vende Bebida |
| **Quartel** | Onde os conscritos são alistados e os Esquadrões são formados |
| **Prisão** | Mapa Interno onde NPCs presos cumprem pena (Sistema de Crime e Justiça) |
| **Casa da Moeda** | Cunhagem: ganho anual de dinheiro (Sistema Econômico) |
| **Templo** | Construção da cultura Religiosa; função detalhada com a Cultura |
| **Laboratório** | Gera Pontos de Pesquisa (Sistema de Tecnologia) |
| **Torre Arcana** | Gera Pontos Arcanos com Magos (Sistema de Magia) |
| **Forja Arcana** | Magos de Ofício produzem Aço Mágico (Sistema de Magia) |
| **Encantaria** | Magos de Ofício encantam itens de Rank 3 ou maior (Sistema de Magia) |

**Ficam para depois:** Posto da Guarda, Hospital, Muralha e Boticário (produção de Medicamentos, ver Sistema de Cura).
