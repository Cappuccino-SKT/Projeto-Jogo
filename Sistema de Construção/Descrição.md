> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Construção

**Descrição geral**

Existem diversos tipos de construção, cada uma com um objetivo. Há uma separação entre as construções que produzem **recursos tangíveis** e as que produzem **coisas não tangíveis**. *Exemplo:* uma mina produz minerais, que o jogador pode pegar; um laboratório produz pesquisa, que o jogador não pega, mas que afeta a cidade ou o mundo.

A tabela que liga cada construção de produção à Função e aos produtos de cada nível está em [[Sistema de Produção/Funções e Construções|Funções e Construções]].

### Atributos

- Nome da construção
- Vida máxima e atual
- Nível
- Recursos para construir
- Recursos que precisam existir no mapa para permitir a construção
- Nível de Inteligência exigido para a construção (como se as construções avançadas precisassem de um engenheiro para acompanhar a obra)
- Tempo de construção
- Trabalhadores construindo (afetam o tempo para terminar)
- Dono (Cidade, Facção, Rico ou jogador; ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]])
- Vagas (por nível; ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]])
- Gerente
- Empregados contratados
- Empregados trabalhando no momento
- Capacidade de armazenamento por categoria (somada ao inventário da cidade; ver [[Sistema de Inventário/Descrição|Inventário]])
- Lista do que pode ser produzido na construção
- Lista do que está desbloqueado para produzir
- Produção atual e, nas do jogador, a fila de ordens (Fazer N, Manter estoque de N, Repetir sempre)
- Tempo até terminar de produzir
- Manutenção por Estação: gasta uma quantidade de recursos a cada estação (16 dias) para manter a construção. O gasto é pago pela cidade e exige um **Construtor** na cidade, que fornece a manutenção.
- Nível de LOD
- Ilegal (booleana: loja ilegal disfarçada) e Confiança do gerente com o jogador

### LOD das construções

O LOD precisa funcionar de forma eficiente nas construções, para não haver grande perda de desempenho.

- **Todos os dados** só são usados nas construções que pertencem ao jogador e que estão na visão dele.
- **Nas outras** (de outros donos, ou fora da visão do jogador), só a produção é processada: o que a construção está produzindo vira um **cálculo por estação** (16 dias), que gera a quantidade de produtos que ela geraria normalmente. Isso economiza processamento.
- Nas construções que não são do jogador, a escolha do que produzir segue a **demanda da cidade** a que ela pertence, pela nota (lucro × déficit) do [[Sistema de Produção/Descrição|Produção]]. As do jogador seguem a fila de ordens.

### Níveis

- Quanto maior o nível, maior a **eficiência** da construção e, nas de produção, a **capacidade de produzir coisas novas**.
- Em troca, quanto maior o nível, maior o **custo para construir** e o **gasto de dinheiro com a manutenção**.
- **Escada padrão** (rodada IP1-3): nas construções de transformação, o nível 1 libera produtos de Tier 1 e Rank 1; o 2, Tier 2 e Rank 2; o 3, Tier 2 e Rank 3; o 4, Tier 3 e Rank 4; o 5, Tier 4 e Rank 5. A era da tecnologia também precisa permitir. Nas Braçais, o nível só aumenta a eficiência. Tabela no [[Sistema de Produção/Descrição|Produção]].
- Cada nível acima do mínimo de uma receita a deixa **10% mais rápida**. Nas capitais que começam com construções de nível 4 e 5 na Era 1, o nível alto dá eficiência e vagas, e as receitas maiores esperam a era.
- As construções podem ser feitas **nível por nível**, com desconto para construir o próximo nível. Quem quiser construir direto um nível alto paga o custo integral.

### Quem constrói

As construções podem ser feitas à mão pelos NPCs do jogador; mas, se houver uma **construtora** (o Construtor), ela pode construir sozinha os projetos do jogador ou da própria cidade.

### Loja ilegal

**Qualquer construção de produção pode ser uma loja ilegal.** Ela se passa por outra coisa, mas compra e vende produtos ilegais. O jogador só passa a negociar nela depois de um evento ou missão em que o NPC ganha confiança nele. Ver [[Sistema de Crime e Justiça/Descrição|Sistema de Crime e Justiça]].

### Construções a acrescentar (a detalhar nos canvases)

Construções que o resto do projeto já exige e ainda não existem nos canvases. Os custos e os níveis ainda não foram definidos (ver "Perguntas em aberto").

| Construção | Função |
|---|---|
| **Mercado** | Loja geral onde os NPCs compram no Período Livre (a "Loja" do [[Sistema Econômico/Descrição\|Sistema Econômico]]) |
| **Taverna** | Entretenimento no Período Livre; vende Bebida; quadro de contratos e de notícias ([[Objetivo e Progressão/Descrição\|Objetivo e Progressão]], [[Sistema de Notícias/Descrição\|Notícias]]) |
| **Quartel** | Onde os conscritos são alistados e os Esquadrões são formados (2 dias cada; tantos ao mesmo tempo quanto o nível) |
| **Prisão** | Mapa Interno onde NPCs presos cumprem pena ([[Sistema de Crime e Justiça/Descrição\|Crime e Justiça]]) |
| **Casa da Moeda** | Cunhagem: ganho anual de dinheiro ([[Sistema Econômico/Descrição\|Econômico]]) |
| **Templo** | Construção de culto da Facção, em todas as culturas (a Arcana usa a Torre Arcana e a Tecnocrata o Laboratório); Sacerdotes, cobertura, oferendas e festivais. Segue a tabela geral de vagas; as cidades de nível 4 podem ter **2 Templos** (revisão V1-10; ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| **Posto Comercial** | Necessário para a cidade enviar caravanas civis ([[Sistema de Comércio/Descrição\|Comércio]]) |
| **Laboratório** | Gera Pontos de Pesquisa ([[Sistema de Tecnologia/Descrição\|Tecnologia]]) |
| **Torre Arcana** | Gera Pontos Arcanos com Magos ([[Sistema de Magia/Descrição\|Magia]]) |
| **Forja Arcana** | Magos de Ofício produzem a Barra de Aço Mágico, com Pedra de Mana; é a única construção que faz Aço Mágico (rodadas IP1-2 e IP2-3; ver [[Sistema de Magia/Descrição\|Magia]]) |
| **Encantaria** | Magos de Ofício encantam itens de Rank 3 ou maior ([[Sistema de Magia/Descrição\|Magia]]) |
| **Hospital** | Cura os NPCs do grupo do jogador; um Mago de Suporte acelera a cura ([[Sistema de Cura/Descrição\|Cura]]) |
| **Boticário** | Produz Curativos e Remédios ([[Sistema de Cura/Descrição\|Cura]]) |
| **Pesqueiro** | Extrai Peixe nos mapas com rio, lago ou litoral; Função Pescador (revisão V1-4; ver [[Sistema de Recursos/Descrição\|Recursos]]) |
| **Moradias** | Senzala (Escravos), Casa (Civil), Casarão (Rico), Mansão (Nobreza) e Palácio (Realeza), cada uma com níveis de 1 a 5 no canvas de construções civis; construídas em lotes fixos ([[Sistema de População/Descrição\|População]]) |
| **Universidade** | Educação: melhora a Inteligência e o Trabalho Especialista dos NPCs da cidade e dá bônus aos Pesquisadores; construção da Facção ([[Sistema de Trabalho e Propriedade/Descrição\|Trabalho e Propriedade]]) |
| **Muralha** | Defesa da cidade em 3 níveis: Paliçada, Muralha de Pedra e Bastião ([[Sistema de Guerra/Descrição\|Guerra]]) |
| **Posto da Guarda** | Mais vagas de guarda (Controle) e ponto de reunião da milícia ([[Sistema de Guerra/Descrição\|Guerra]]) |
| **Mercado de Escravos** | Compra e venda de escravos (dono: Facção ou Rico), só onde a escravidão é permitida ([[Sistema de Escravidão/Descrição\|Escravidão]]) |

### Clima

A **tempestade forte** danifica as construções de madeira; o reparo sai pela manutenção. No inverno, as moradias gastam Tora como lenha (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).

### Arte

A aparência de cada construção muda nos níveis 1, 3 e 5, com peças reaproveitáveis por cultura. Interior próprio só nas públicas mais visitadas; as outras usam interiores genéricos (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).

### Modo construção (interface)

Ao entrar no modo construção, os lotes livres ficam destacados. Clicar num lote abre uma lista por categoria, com custo, vagas e materiais; há pré-visualização, fila de obras e demolição (ver [[Interface/Descrição|Interface e Controles]]).

---

## Parte técnica

- **Dados:** cada tipo de construção em JSON (níveis, custo por nível, tempo de obra, manutenção, vagas, capacidade por categoria, receitas liberadas, exigências de recurso e de tecnologia).
- **LOD:** dados completos só nas construções do jogador na visão dele; as outras produzem por estação, pela demanda da cidade.
- **Save:** por construção: tipo, nível, dono, Vida, produção atual e progresso, empregados, Ilegal e Confiança.
- **Fatia vertical:** Fazenda, Cozinheiro, Mercado e Casa, com custo, manutenção e vagas.

## Ligação com outros sistemas

| Sistema | O que a Construção fornece ou recebe |
|---|---|
| Produção | Receitas por nível; capacidade de armazenamento; Funções e Construções |
| Trabalho e Propriedade | Dono, vagas, gerente, salários, compra e venda |
| Inventário e Econômico | Capacidade somada ao inventário da cidade; lojas |
| Cidade e População | Lotes fixos; construções obrigatórias por nível; moradias |
| Mapa | Construções como Mapas Internos; interiores próprios ou genéricos |
| Tecnologia | Construções e níveis liberados por tecnologia |
| Crime e Justiça | Loja ilegal disfarçada |
| Guerra | Muralha, Posto da Guarda, Quartel; construções como obstáculo na batalha |
| Religião | Templo (2 nas cidades de nível 4) |
| Clima e Estações | Tempestade forte danifica a madeira |
| Pipeline | Aparência nos níveis 1, 3 e 5; peças por cultura |
| Interface | Modo construção, lotes, fila de obras |

## Perguntas em aberto

1. **Custos, tempos, manutenção e níveis** de cada construção, incluindo as novas (rodada V2 da [[Revisão Completa]]).
2. **"Nível de Inteligência para construir":** é a Inteligência do Construtor, a do gerente da obra ou da cidade?
