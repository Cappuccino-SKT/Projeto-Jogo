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
- Dono (Cidade, Facção, Rico ou jogador; ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]])
- Vagas (por nível; ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]])
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

### Loja ilegal

**Qualquer construção de produção pode ser uma loja ilegal.** Ela se passa por outra coisa, mas compra e vende produtos ilegais. O jogador só passa a negociar nela depois de um evento ou missão em que o NPC ganha confiança nele. Ver Sistema de Crime e Justiça.

### Construções a acrescentar (a detalhar nos canvases)

Construções que o resto do projeto já exige e ainda não existem nos canvases. Custos e níveis ainda não foram definidos.

| Construção | Função |
|---|---|
| **Mercado** | Loja geral onde os NPCs compram no Período Livre (a "Loja" do Sistema Econômico) |
| **Taverna** | Entretenimento no Período Livre; vende Bebida; quadro de contratos (Objetivo e Progressão) |
| **Quartel** | Onde os conscritos são alistados e os Esquadrões são formados (2 dias cada; tantos ao mesmo tempo quanto o nível) |
| **Prisão** | Mapa Interno onde NPCs presos cumprem pena (Sistema de Crime e Justiça) |
| **Casa da Moeda** | Cunhagem: ganho anual de dinheiro (Sistema Econômico) |
| **Templo** | Construção de culto da Facção, em todas as culturas (a Arcana usa a Torre Arcana e a Tecnocrata o Laboratório); Sacerdotes, cobertura, oferendas e festivais (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| **Posto Comercial** | Necessário para a cidade enviar caravanas civis (Sistema de Comércio) |
| **Laboratório** | Gera Pontos de Pesquisa (Sistema de Tecnologia) |
| **Torre Arcana** | Gera Pontos Arcanos com Magos (Sistema de Magia) |
| **Forja Arcana** | Magos de Ofício produzem Aço Mágico (Sistema de Magia) |
| **Encantaria** | Magos de Ofício encantam itens de Rank 3 ou maior (Sistema de Magia) |
| **Hospital** | Cura os NPCs do grupo do jogador; um Mago de Suporte acelera a cura (Sistema de Cura e Sistema de Magia) |
| **Boticário** | Produz Medicamentos (Sistema de Cura) |
| **Moradias** | Senzala (Escravos), Casa (Civil), Casarão (Rico), Mansão (Nobreza) e Palácio (Realeza), cada uma com níveis de 1 a 5 no canvas de construções civis; construídas em lotes fixos (ver [[Sistema de População/Descrição\|Sistema de População]]) |
| **Universidade** | Educação: melhora a Inteligência e o Trabalho Especialista dos NPCs da cidade e dá bônus aos Pesquisadores; construção da Facção (ver [[Sistema de Trabalho e Propriedade/Descrição\|Sistema de Trabalho e Propriedade]]) |
| **Muralha** | Defesa da cidade em 3 níveis: Paliçada, Muralha de Pedra e Bastião (ver [[Sistema de Guerra/Descrição\|Sistema de Guerra]]) |
| **Posto da Guarda** | Mais vagas de guarda (Controle) e ponto de reunião da milícia (ver [[Sistema de Guerra/Descrição\|Sistema de Guerra]]) |
| **Mercado de Escravos** | Compra e venda de escravos (dono: Facção ou Rico), só onde a escravidão é permitida (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) |

### Clima

A **tempestade forte** danifica as construções de madeira; o reparo sai pela manutenção. No inverno, as moradias gastam Madeira Bruta como lenha (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).

### Arte

A aparência de cada construção muda nos níveis 1, 3 e 5, com peças reaproveitáveis por cultura. Interior próprio só nas públicas mais visitadas; as outras usam interiores genéricos (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).

### Modo construção (interface)

Ao entrar no modo construção, os lotes livres ficam destacados. Clicar num lote abre uma lista por categoria, com custo, vagas e materiais; há pré-visualização, fila de obras e demolição (ver [[Interface/Descrição|Interface e Controles]]).
