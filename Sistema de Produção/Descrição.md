> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> **Em reformulação:** o autor decidiu (revisão V1-2, V1-7 e V1-8) fazer rodadas próprias para **padronizar a forma de produção**, **reformular os nomes dos itens** e definir **os insumos de cada produto**. Este documento guarda o que já estava decidido até elas.

## Sistema de Produção

**Descrição geral**

A produção é baseada em **3 categorias** — Produto Braçal, Produto Manufaturado e Produto Especializado — que correspondem às habilidades de Trabalho dos NPCs. A produção ocorre **em sequência**: cada produto mais complexo exige produtos anteriores, formando um sistema de demanda.

| Categoria | O que é | Exemplos de Função |
|---|---|---|
| **Braçal** | Toda produção feita direto do mapa, gerando um recurso básico. O que existe numa região é limitado pela lista de recursos do Mapa Local (uma mina só existe onde há recurso mineral) | Fazendeiro, Coletor, Rancheiro, Pescador, Construtor |
| **Manufaturada** | Toda produção que transforma um produto básico num produto manufaturado | Ferreiro (níveis básico e médio), Costureiro (básico e médio), Cozinheiro, Pedreiro, Metalúrgico |
| **Especialista** | Toda produção que resulta num item de alto nível | Ferreiro (nível alto), Costureiro (nível alto), Armeiro, Artesão, Boticário, Mago de Ofício |

O sistema de produção será bastante completo, com exigências de habilidades e de matérias-primas combinadas. Quanto maior o grau do item produzido, maior a exigência de habilidade e de material. Cada receita tem um **tempo de produção base** (ligado ao [[Sistema de Tempo/Descrição|Tempo]]), reduzido pela habilidade do NPC responsável (Trabalho Especialista mais alto = produção mais rápida) e pelo nível da construção.

### Estruturas

A produção é baseada em **construções**, desenvolvidas junto com o [[Sistema de Construção/Descrição|Sistema de Construção]]. Cada construção tem:

- uma função de produção e acesso a um tipo de produção específico;
- uma **capacidade de armazenamento**, que soma ao inventário da cidade (limite de estoque, para evitar acúmulo infinito; ver [[Sistema de Inventário/Descrição|Inventário]]);
- uma quantidade de trabalhadores designados pela Função do NPC.

*Exemplo de cadeia:* o Posto de Coleta extrai minérios (conforme a região), que a Metalurgia transforma em barras, que o Ferreiro transforma em armas e armaduras.

**Logística simplificada:** não é preciso "entregar" fisicamente a produção entre construções; tudo fica no inventário único da cidade ([[Sistema Econômico/Descrição|Econômico]]).

### Regras já decididas

- **Estações e clima:** a produção Braçal muda com a estação e o bioma (na tabela base, a Fazenda rende 150% no outono e 25% no inverno); a Manufaturada e a Especialista não mudam ([[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Animais** (revisão V1-3): o **Pasto** cria Galinha, Porco, Ovelha, Vaca e Cavalo, que são itens do tipo Animal. O **Rancho** precisa dos animais no estoque, mas não os gasta (ovos, lã, leite e dejetos). O **Açougue** gasta o animal e faz Pedaço de Carne, Ossos e Couro Bruto.
- **Pesca** (revisão V1-4): o **Pesqueiro** extrai Peixe nos mapas com rio, lago ou litoral ([[Sistema de Recursos/Descrição|Recursos]]).
- **Cadeia militar:** Ração Militar e materiais de manutenção de equipamento abastecem os exércitos ([[Sistema de Guerra/Descrição|Guerra]]).
- **Loja ilegal:** qualquer construção de produção pode funcionar como loja ilegal disfarçada ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).
- **Funções e construções:** a tabela mestre que liga cada Função à construção e ao que ela produz está em [[Sistema de Produção/Funções e Construções|Funções e Construções]]. Os salários, as vagas e os donos estão no [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]].
- **Quem decide o que produzir:** nas construções que não são do jogador, a demanda da cidade ([[Sistema de Construção/Descrição|Construção]]).

---

## Parte técnica

- **Dados:** receitas em JSON ou CSV (a estrutura padrão da receita será decidida na rodada de padronização).
- **LOD:** produção completa nas construções do jogador na visão dele; nas outras, cálculo por estação.
- **Save:** produção atual e progresso de cada construção.
- **Fatia vertical:** Fazenda → Cozinheiro → Mercado (Grão e Água → Pão).

## Ligação com outros sistemas

| Sistema | O que a Produção fornece ou recebe |
|---|---|
| Recursos | Recursos de cada Mapa Local e produtividade por estação |
| Construção | Construções, níveis e capacidade de armazenamento |
| Itens | Produtos e insumos; Tier e Rank |
| Trabalho e Propriedade | Funções, vagas, salários e donos |
| NPC | Habilidades de Trabalho; Geração de NPC |
| Inventário e Econômico | Inventário único da cidade; insumos comprados pelo preço do Mercado Local |
| Tecnologia | Eras liberam os Tiers e as receitas |
| Guerra | Ração Militar, munição e manutenção dos exércitos |
| Clima e Estações | Produção Braçal por estação |

## Perguntas em aberto

1. **Padronização da produção** (estrutura da receita, família de cada construção, níveis, Rank do produto): rodada IP1.
2. **Nomes e lista dos itens:** rodada IP2.
3. **Insumos de cada produto:** rodada IP3.
