> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> **Regras padronizadas na rodada IP1 (2026-10-17).** Os nomes dos itens foram fechados na rodada IP2; os insumos de cada produto vêm na IP3. Os valores numéricos são iniciais, a testar.

## Sistema de Produção

**Descrição geral**

A produção é baseada em **3 categorias** — Produto Braçal, Produto Manufaturado e Produto Especializado — que correspondem às habilidades de Trabalho dos NPCs. A produção ocorre **em sequência**: cada produto mais complexo exige produtos anteriores, formando um sistema de demanda.

| Categoria | O que é | Exemplos de Função |
|---|---|---|
| **Braçal** | Toda produção feita direto do mapa, gerando um recurso básico. O que existe numa região é limitado pela lista de recursos do Mapa Local (uma mina só existe onde há recurso mineral) | Fazendeiro, Coletor, Rancheiro, Pescador, Construtor |
| **Manufaturada** | Toda produção que transforma um produto básico num produto manufaturado | Ferreiro (níveis básico e médio), Costureiro (básico e médio), Cozinheiro, Pedreiro, Metalúrgico |
| **Especialista** | Toda produção que resulta num item de alto nível | Ferreiro (nível alto), Costureiro (nível alto), Armeiro, Artesão, Boticário, Mago de Ofício |

Toda produção segue a mesma forma: uma **receita** fixa, feita numa única construção, liberada pelo nível da construção e pela era da tecnologia, com o tempo e a qualidade dados pela habilidade de quem trabalha.

### Estruturas

A produção é baseada em **construções**, desenvolvidas junto com o [[Sistema de Construção/Descrição|Sistema de Construção]]. Cada construção tem:

- uma **família** de produtos (ver abaixo);
- uma **capacidade de armazenamento**, que soma ao inventário da cidade (limite de estoque, para evitar acúmulo infinito; ver [[Sistema de Inventário/Descrição|Inventário]]);
- uma quantidade de trabalhadores designados pela Função do NPC.

*Exemplo de cadeia:* o Posto de Coleta extrai minérios (conforme a região), que a Metalurgia transforma em barras, que o Ferreiro transforma em armas e armaduras.

**Logística simplificada:** não é preciso "entregar" fisicamente a produção entre construções; tudo fica no inventário único da cidade ([[Sistema Econômico/Descrição|Econômico]]).

---

## Receita padrão

**Uma receita fixa por produto**, sempre com os mesmos campos. Receitas alternativas para o mesmo produto (Pão de Grão ou de Legumes) só entram depois, como exceção, se a rodada IP3 pedir.

| Campo | O que é | Exemplo (Pão) |
|---|---|---|
| Produto e quantidade | O que sai de um ciclo | 4 Pão |
| Insumos | Itens e quantidades gastos | 2 Grão + 1 Água |
| Subprodutos | Saídas extras, se houver | — |
| Construção e nível mínimo | Onde se faz | Cozinheiro, nível 1 |
| Função e Trabalho | Quem faz e qual habilidade conta | Cozinheiro; Trabalho Manufaturado |
| Habilidade mínima | Abaixo dela, o NPC não consegue fazer | 10 |
| Horas de trabalho | Quanto trabalho um ciclo leva | 4 horas |
| Era | Tecnologia mínima | Era 1 |

- **Ciclo:** os insumos saem do inventário da cidade quando o ciclo começa, e o produto entra quando ele termina. Sem insumos, o ciclo não começa.
- **[Proposta]** Um ciclo interrompido (fim do turno, falta de trabalhador) guarda o progresso em horas e continua depois; os insumos já gastos não voltam.

## Uma família de produtos por construção

**Regra:** cada item é feito em **uma única construção**. Assim a IA sabe onde produzir, e o jogador sabe onde procurar. A família segue o **ofício e o material**, como nos ofícios reais.

| Construção | Família (tudo o que é dela) |
|---|---|
| Fazenda | Vegetais |
| Pasto | Animais vivos |
| Rancho | Produtos de animais vivos (ovos, lã, leite, dejetos) |
| Pesqueiro | Peixe |
| Posto de Coleta | Tora, Pedra, Argila, minérios, Carvão, Água e Pedra de Mana |
| Açougue | Carne Crua, Ossos e Couro Cru (gasta o animal) |
| Metalurgia | Barras de Ferro, Cobre, Aço e Ouro |
| **Forja Arcana** | **Barra de Aço Mágico** (precisa de Mago de Ofício). A Pedra de Mana é bruta e não passa por refino (IP2-3) |
| Pedreira | Bloco de Pedra e Cerâmica |
| Carpinteiro | Tudo de madeira: Tábua, móveis, arcos (de Madeira, Compostos e Arcanos), bestas, Flecha e Virote, Escudo de Madeira, Cajado Simples de Madeira |
| Ferreiro | Tudo de metal batido: lâminas, hastes, clavas, escudos de metal, **armaduras Médias (malha) e Pesadas (placas)**, Grilhões, Chapéu de Mineiro |
| Armeiro | Armas de fogo, Cartucho, artilharia e Bala de Canhão |
| Costureiro | Tecido de Algodão e de Lã, Couro Refinado, roupas (as equipáveis e as de consumo), **armadura Leve** (couro), Sela |
| Artesão | Joias, anéis, Ornamento, Coroa, cajados de metal e Cetro, Joia de Mana |
| Cozinheiro | Comida (do Pão ao Doce de Especiarias) |
| Destilaria | Bebidas (Cerveja, Vinho, Licor) e Perfume |
| Misturador | Misturas secas: Adubo, Pólvora, **Fumo** e Pó Espiritual |
| Boticário | Medicamentos |

- **Mudanças em relação ao canvas:** a Metalurgia deixa de fazer Aço Mágico (só a Forja Arcana faz); o Fumo sai do Artesão e vai para o Misturador; o Pó Espiritual sai da Destilaria e vai para o Misturador; as armaduras de metal (Média de malha e Pesada de placas) ficam no Ferreiro, e a Leve, de couro, no Costureiro (a IP2-7 mudou este ponto da IP1-2).
- A tabela completa, produto por produto e nível por nível, será refeita na rodada IP3 em [[Sistema de Produção/Funções e Construções|Funções e Construções]].
- A **Encantaria** não tem família: ela muda um item que já existe (ver [[Sistema de Magia/Descrição|Magia]]).

## O que cada nível libera

Uma **escada padrão** para todas as construções de transformação (Manufaturadas e Especialistas):

| Nível | Produtos de Tier até | Rank máximo do que sai | Era típica |
|---|---|---|---|
| 1 | 1 | 1 | Era 1 |
| 2 | 2 | 2 | Era 1 |
| 3 | 2 | 3 | Era 2 |
| 4 | 3 | 4 | Era 3 |
| 5 | 4 | 5 | Era 4 |

- **Duas travas ao mesmo tempo:** o **nível** da construção libera a receita na construção, e a **era** da tecnologia libera o Tier e o Rank no mundo. O material também limita o Rank (ver abaixo).
- **Rank por era:** Era 1, Rank até 3; Era 2, até 4 (Aço); Era 3, até 4; Era 4, até 5 (Aço Mágico). O Tier 0 existe desde o início; a Era N libera o Tier N ([[Sistema de Tecnologia/Descrição|Tecnologia]]).
- As construções **Braçais** (Fazenda, Pasto, Rancho, Pesqueiro, Posto de Coleta) não seguem a escada: liberam tudo no nível 1, e o nível só aumenta a eficiência.
- **Capitais na Era 1:** as construções de nível 4 e 5 das capitais (pacotes do [[Mundo e Estado Inicial/Descrição|Mundo]]) continuam dando **eficiência e mais vagas**, mas as receitas de Tier e Rank maiores só liberam com a era.
- *Exemplo:* um Ferreiro nível 3, na Era 2, faz Espada de Aço até Rank 3; com nível 4 e Era 3, até Rank 4; a Espada de Aço Mágico só no nível 5, na Era 4.

## De onde sai o Rank do item produzido

O Rank de uma arma, equipamento ou artefato produzido é o **menor** de três valores:

1. o teto do **material** (Ferro 3, Aço 4, Aço Mágico 5). **[Proposta]** Na prática, é o máximo da faixa do item na lista do [[Sistema de Itens/Descrição|Sistema de Itens]] (a "Espada Curta de Ferro (Rank 1-3)" tem teto 3), o que também vale para itens sem metal, como as roupas;
2. o teto do **nível** da construção (tabela acima);
3. o Rank pela **habilidade** do trabalhador: **1 + habilidade de Trabalho ÷ 25**, arredondado para baixo (0 a 24 = 1; 25 a 49 = 2; 50 a 74 = 3; 75 a 99 = 4; 100 = 5). Conta a habilidade de Trabalho da receita (Manufaturado ou Especialista).

- **Sorte:** 20% de chance de sair 1 Rank acima (se os tetos deixarem) e 10% de sair 1 abaixo (nunca abaixo de 1).
- **Em LOD 2:** usa a habilidade média dos trabalhadores, sem sorte.
- *Exemplo:* um Ferreiro com Trabalho Especialista 60, num Ferreiro nível 4, fazendo Espada de Ferro: habilidade dá 3, nível dá 4, material dá 3 → **Rank 3**. Com Aço, o material deixa 4, mas a habilidade continua dando 3.
- Itens com **Tier** (materiais, comida) não têm qualidade: a habilidade só acelera.
- O Rank 0 não sai da produção: fica para itens quebrados ou improvisados **[Proposta]**.

## Escala: o que 1 NPC real produz e consome

**1 NPC real = 1 trabalhador e 1 consumidor nas contas da economia.** O Fator de Escala (exemplo 100) só muda a **população exibida**, os conscritos e o tamanho visual dos soldados ([[Sistema de Facção/Descrição|Facção]]).

- É coerente com o que já estava decidido: 1 Ração por Esquadrão por dia = 1 Ração por NPC alistado; o escravo é comprado como 1 NPC real que trabalha como 1 trabalhador; o Controlado come do mesmo item que a cidade vende.
- *Exemplo:* uma cidade de 100 NPCs reais come cerca de 100 a 150 unidades de comida por dia; a interface mostra "População: 10.000" (com o Fator 100), mas as contas usam 100.
- Assim todos os números da economia (rodada V2) ficam pequenos e fáceis de balancear.

## Tempo e quantidade por ciclo

**Horas de trabalho por unidade**, por Tier, como guia para as receitas da rodada IP3:

| Produto | Horas por unidade |
|---|---|
| Tier 0 (Braçal) | 0,5 |
| Tier 1 | 1 |
| Tier 2 | 2 |
| Tier 3 | 4 |
| Tier 4 | 8 |
| Arma ou peça de Ferro e Cobre | 8 |
| Arma ou peça de Aço | 12 |
| Arma ou peça de Aço Mágico | 24 |

- **Velocidade do trabalhador** = 0,5 + habilidade ÷ 100 (habilidade 0 = metade; 50 = normal; 100 = 1,5 vez).
- **Nível acima do mínimo da receita:** +10% por nível.
- **[Proposta]** Horas reais de um ciclo = horas da receita ÷ (velocidade × (1 + 0,1 × níveis acima do mínimo)). Vários trabalhadores na mesma construção fazem ciclos separados, ao mesmo tempo.
- Na produção **Braçal**, a estação e o bioma multiplicam o resultado ([[Sistema de Clima e Estações/Descrição|Clima e Estações]]); a produtividade base de cada recurso fica para a rodada V2.
- *Exemplo:* um Cozinheiro com habilidade 50 trabalha 8 horas e faz 8 Pães (Tier 1) por dia; uma cidade de 100 NPCs precisaria de cerca de 12 a 18 cozinheiros, ou de comidas que rendem mais por hora (a IP3 equilibra). As horas do turno vêm da rotina (rodada V2).

## Quem decide o que produzir

- **Construções do jogador:** uma fila de **ordens**, como no RimWorld, cada uma com uma prioridade:
  - **Fazer N** (por exemplo, 20 Espadas);
  - **Manter estoque de N** (produz até ter N; volta a produzir quando cai);
  - **Repetir sempre**.
- **Construções da cidade e dos Ricos (IA):** a cada dia (LOD 0 e 1) ou estação (LOD 2), escolhem a receita liberada com a maior nota:

> **nota = (preço local do produto − custo dos insumos) × (1 + déficit da cidade naquele item)**

- A **comida tem prioridade** quando o estoque cai abaixo de 3 dias de consumo.
- **[Proposta]** O déficit vai de 0 (estoque cobre a demanda) a 1 (estoque zerado), pelo mesmo contador de dias em déficit da [[Sistema de Classe Social/Descrição|Classe Social]].
- *Exemplo:* um Ferreiro de um Rico pode fazer Lança de Ferro (lucro 20) ou Grilhões (lucro 12). A cidade não tem Grilhões (déficit 1): nota da Lança = 20 × 1 = 20; dos Grilhões = 12 × 2 = 24. Ele faz Grilhões.

## Limites de complexidade

- No máximo **3 insumos** e **3 saídas** (produto e subprodutos) por receita.
- Cadeia de no máximo **4 passos** do mapa ao produto final (por exemplo, Minério de Ferro → Barra de Ferro → Espada).
- **Nenhum item órfão:** todo item intermediário precisa ser usado por pelo menos uma receita ou consumido por alguma classe. O `jogo --validar` avisa os órfãos.
- **Sem ferramentas** como insumo na primeira versão; o desgaste das construções já é a manutenção.

## Regras já decididas

- **Estações e clima:** a produção Braçal muda com a estação e o bioma (na tabela base, a Fazenda rende 150% no outono e 25% no inverno); a Manufaturada e a Especialista não mudam ([[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Animais** (revisão V1-3): o **Pasto** cria Galinha, Porco, Ovelha, Vaca e Cavalo, que são itens do tipo Animal. O **Rancho** precisa dos animais no estoque, mas não os gasta (ovos, lã, leite e dejetos). O **Açougue** gasta o animal e faz Carne Crua, Ossos e Couro Cru.
- **Pesca** (revisão V1-4): o **Pesqueiro** extrai Peixe nos mapas com rio, lago ou litoral ([[Sistema de Recursos/Descrição|Recursos]]).
- **Cadeia militar:** Ração Militar e materiais de manutenção de equipamento abastecem os exércitos ([[Sistema de Guerra/Descrição|Guerra]]).
- **Loja ilegal:** qualquer construção de produção pode funcionar como loja ilegal disfarçada ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).
- **Funções e construções:** a tabela mestre que liga cada Função à construção e ao que ela produz está em [[Sistema de Produção/Funções e Construções|Funções e Construções]]. Os salários, as vagas e os donos estão no [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]].

---

## Parte técnica

- **Dados:** as receitas num CSV (uma linha por receita, editada em planilha), com as colunas da receita padrão; IDs de texto legíveis (`receita.pao`), convertidos em números ao carregar ([[Arquitetura/Descrição|Arquitetura]]).
- **Validação (`jogo --validar`):** item, construção ou Função inexistente; mais de 3 insumos ou 3 saídas; item feito em duas construções; item órfão; cadeia com mais de 4 passos; receita acima da escada de níveis.
- **LOD:** em LOD 0 e 1, cada trabalhador faz ciclos de verdade, e a IA escolhe a receita por dia; em LOD 2, a produção de cada construção é um cálculo por estação (trabalhadores × horas do turno × velocidade média ÷ horas por unidade), com a escolha feita uma vez por estação.
- **Save:** por construção: fila de ordens (tipo, item, quantidade, prioridade), receita atual e progresso do ciclo em horas.
- **Fatia vertical:** Fazenda → Cozinheiro → Mercado, com a receita do Pão do exemplo acima e uma ordem "Manter estoque" na construção do jogador.

## Ligação com outros sistemas

| Sistema | O que a Produção fornece ou recebe |
|---|---|
| Recursos | Recursos de cada Mapa Local e produtividade por estação |
| Construção | Construções, níveis (escada padrão) e capacidade de armazenamento |
| Itens | Produtos e insumos; Tier e Rank; teto de Rank pelo material |
| Trabalho e Propriedade | Funções, vagas, salários e donos; habilidade dos empregados |
| NPC | Habilidades de Trabalho (velocidade e Rank); Geração de NPC |
| Inventário e Econômico | Inventário único da cidade; preço local na nota da IA; preço base pelo custo da receita (V2) |
| Tecnologia | Eras liberam os Tiers, os Ranks e as receitas |
| Guerra e Batalha | Ração Militar, munição e manutenção dos exércitos; Rank do kit |
| Clima e Estações | Produção Braçal por estação |
| Interface | Fila de ordens no painel da construção |
| IA | Escolha da receita pela nota nas construções da cidade e dos Ricos |
| Classe Social | Déficit da cidade por item; prioridade da comida |

## Perguntas em aberto

1. **Nomes e lista final dos itens:** fechados na rodada IP2 (ver [[Sistema de Itens/Descrição|Itens]]).
2. **Insumos de cada produto** e a nova tabela de Funções e Construções: rodada IP3.
3. **Números:** produtividade base dos recursos, horas do turno e preço base (≈ custo dos insumos + horas de trabalho × salário): rodada V2.
4. **Valores a testar:** horas por unidade, chances de sorte no Rank, bônus de +10% por nível, limiar de 3 dias de comida.
