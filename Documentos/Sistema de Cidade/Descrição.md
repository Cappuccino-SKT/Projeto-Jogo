> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Cidade

**Descrição geral**

A Cidade é uma área dentro de um Mapa Local. Cada Mapa Local comporta **no máximo uma cidade**. As cidades são locais de alta concentração de NPCs, com capacidade de construção tanto de produção quanto de comércio.

Este é o **documento de entrada** da cidade: ele guarda os níveis e os atributos, e aponta para os sistemas onde está cada regra (ver "Onde está cada regra").

### Níveis

| Nível | Nome | Quem pode ter |
|---|---|---|
| 1 | Assentamento | Qualquer um; o jogador pode fundar |
| 2 | Vilarejo | Qualquer um; sobe pelas regras da População |
| 3 | Cidade | Qualquer um; necessária para fundar uma facção |
| 4 | Metrópole | **Só as capitais** das facções principais |

- Algumas regiões já começam com cidades, por causa da história do jogo e das facções que já existem.
- Do nível 1 ao 3, o jogador consegue criar por conta própria. O **nível 4** fica limitado às capitais: a metrópole ocupa o **Mapa Local inteiro**, algo muito grande e raro. A única forma de o jogador ter uma metrópole é pela **conquista** (ou por sistemas futuros).

### População real por nível

O Fator de Escala converte NPCs reais em população total, mas a quantidade de **NPCs reais** de cada cidade vem do nível dela. Assim as cidades não ficam vazias e o custo de simulação é previsível.

Valores iniciais, a ajustar nos testes:

| Nível | NPCs reais | População total (Fator 100, exemplo) |
|---|---|---|
| 1 | 15 | 1.500 |
| 2 | 40 | 4.000 |
| 3 | 100 | 10.000 |
| 4 | 300 | 30.000 |

As cidades do primeiro dia (19 de facção e 3 Vilas Livres) estão no [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]].

### Facção da cidade

Uma cidade pertence a uma facção por **vontade** (o dono oferece lealdade), por **conquista** ou por ter sido **fundada** em território de facção. A cidade herda **todas as leis** da facção (não há leis locais).

O jogador pode fundar um assentamento (nível 1) num mapa sem dono, ou em território de facção, onde ele nasce vinculado à facção dominante. Para fundar uma facção própria, a cidade precisa estar no nível 3 e fora de qualquer facção ([[Sistema de Facção/Descrição|Facção]]).

### Atributos

- Nome
- Mapa Local a que pertence (um único)
- Quantidade de NPCs reais (a população total é NPCs × Fator de Escala)
- Facção
- Cultura: a cultura do dono (base) mais uma proporção agregada de outras culturas
- Construções e lotes de construção (fixos, marcados no mapa; vazios ou ocupados)
- Layout (desenhado à mão, com os lotes pré-definidos)
- Quantidade de NPCs de cada classe social e bens de consumo de cada classe
- Banco da Cidade (o dinheiro da cidade)
- Taxa de desemprego e riqueza média por classe
- Inventário da cidade (tabela única, com a capacidade somada das construções)
- Felicidade por classe social (de 0 a 100) e Estabilidade
- Criminalidade
- Nível da cidade, capacidade de NPCs e ocupação atual
- Itens proibidos (**derivado** das leis da facção)
- Conscritos disponíveis (**derivado**: população × lei de Conscrição ÷ Fator de Escala, menos os já alistados)
- Esquadrões da cidade e guarnição
- Muralha (nenhuma, Paliçada, Pedra ou Bastião) e situação de guerra (sitiada ou ocupada, e por quem)
- Guardas (pelo nível e pela Criminalidade, com um mínimo pela lei da facção)
- Cobertura do Templo e festivais
- Voto na assembleia (vem da cultura do dono)

### Onde está cada regra

| Assunto | Documento |
|---|---|
| Crescimento, migração, moradias e subida de nível | [[Sistema de População/Descrição\|População e Habitação]] |
| Felicidade por classe, Controle, Estabilidade e revolta | [[Sistema de Felicidade e Estabilidade/Descrição\|Felicidade e Estabilidade]] |
| Emprego, salários, donos, impostos e empresas | [[Sistema de Trabalho e Propriedade/Descrição\|Trabalho e Propriedade]] |
| Consumo por classe | [[Sistema de Classe Social/Descrição\|Classe Social]] |
| Mercado, lojas e Banco da Cidade | [[Sistema Econômico/Descrição\|Econômico]] e [[Sistema de Inventário/Descrição\|Inventário]] |
| Construções, níveis e manutenção | [[Sistema de Construção/Descrição\|Construção]] |
| Crime, guardas e Criminalidade | [[Sistema de Crime e Justiça/Descrição\|Crime e Justiça]] |
| Cerco, ocupação, saque e Muralha | [[Sistema de Guerra/Descrição\|Guerra]] |
| Templo, festivais e minorias | [[Sistema de Religião/Descrição\|Religião e Fé]] |
| Escravos e Senzala | [[Sistema de Escravidão/Descrição\|Escravidão]] |
| Cultura da cidade | [[Sistema de Cultura/Descrição\|Cultura]] |

---

## Parte técnica

- **LOD:** a cidade segue o LOD do Mapa Local; em LOD 2, tudo é agregado e atualizado por estação.
- **Valores derivados:** itens proibidos, conscritos e estado do território são calculados, não guardados.
- **Save:** os atributos guardados acima (os derivados não).
- **Fatia vertical:** 1 cidade, com nível, NPCs reais, Banco, inventário, Felicidade por classe e lotes.

## Ligação com outros sistemas

Os sistemas da tabela "Onde está cada regra", mais:

| Sistema | O que a Cidade fornece ou recebe |
|---|---|
| Mapa | Uma cidade por Mapa Local; estado do território dos vizinhos |
| Facção | Leis herdadas; impostos; voto na assembleia |
| Mundo e Estado Inicial | Cidades e pacotes de construções do primeiro dia |

## Perguntas em aberto

1. Nenhuma própria: as pendências de cada assunto estão nos documentos da tabela "Onde está cada regra".
