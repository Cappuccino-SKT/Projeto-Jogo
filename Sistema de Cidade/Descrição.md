## Sistema de Cidade

A Cidade é uma área dentro de um mapa local que possua cidade. Cada Mapa Local comporta **no máximo uma cidade**. As cidades são locais de alta concentração de NPCs, com capacidade de construção tanto de produção quanto para comércio.

A princípio teremos níveis para as cidades, esse nível significa o desenvolvimento da cidade sendo: nível 1, assentamento; nível 2, vilarejo; nível 3, cidade; e nível 4, metrópole. Dentro do jogo algumas regiões já terão cidades devido à história do jogo e às facções já existentes. A ideia é que do nível 1 até o 3 os jogadores consigam criar por conta própria, porém o nível 4 vai ficar limitado apenas para as capitais das facções principais. A ideia é que as metrópoles sejam uma cidade que ocupe o tamanho inteiro do mapa local, sendo algo muito grande e limitado para ter poucos. A única forma de o jogador possuir uma metrópole é pela conquista ou por outros sistemas que iremos desenvolver.

### População real por nível

O Fator de Escala converte NPCs reais em população total, mas a quantidade de **NPCs reais** de cada cidade vem do nível dela. Assim as cidades não ficam vazias e o custo de simulação é previsível.

**[Proposta]** valores iniciais, a ajustar em testes:

| Nível | Nome | NPCs reais | População total (Fator 100, exemplo) |
|---|---|---|---|
| 1 | Assentamento | 15 | 1.500 |
| 2 | Vilarejo | 40 | 4.000 |
| 3 | Cidade | 100 | 10.000 |
| 4 | Metrópole | 300 | 30.000 |

As condições para subir de nível, o crescimento da população, a migração e as moradias estão no [[Sistema de População/Descrição|Sistema de População]].

As cidades que existem no primeiro dia (19 de facção e 3 Vilas Livres independentes) estão no [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]].

### Facção da cidade

Uma cidade pertence a uma facção por **vontade** (o dono oferece lealdade), por **conquista** ou por ter sido **fundada** em território de facção. A cidade herda **todas as leis** da facção (não há leis locais). Ver Sistema de Facção.

O jogador pode fundar um assentamento (nível 1) em um mapa sem dono, ou em território de facção, onde ele nasce vinculado à facção dominante. Para fundar uma facção própria, a cidade precisa estar no nível 3 e fora de qualquer facção.

### Atributos

- Nome
- A qual Mapa Local ela pertence (um único)
- Quantidade de NPCs (NPCs reais; a população total é NPCs × Fator de Escala; o alvo por nível está em "População real por nível")
- Facção
- Cultura — a cultura do dono (base) mais uma proporção agregada de outras culturas
- Construções
- Layout (desenhado à mão, com lotes de construção pré-definidos)
- Quantidade de NPCs de cada classe social
- Dinheiro da cidade (o "banco da cidade" do Sistema Econômico)
- Taxa de desemprego e riqueza média por classe (ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]])
- Inventário da cidade
- Felicidade da cidade — um valor de 0 a 100 por classe social (a da cidade é a média ponderada), afetada pela distância entre as leis da facção e as leis desejadas pela cultura, pela Criminalidade, por convocações altas e outros fatores
- Estabilidade — Felicidade ponderada + Controle − 10; decide a revolta (ver [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]])
- Criminalidade — valor agregado de crime da cidade (ver Sistema de Crime e Justiça)
- Bens de consumo de cada classe social (usado para causar eventos)
- Nível da cidade
- Itens proibidos — **derivado das leis da facção**
- Conscritos disponíveis — **derivado**: população × lei de Conscrição ÷ Fator de Escala, menos os já alistados (usado para o sistema de Batalha)
- Esquadrões da cidade (os que a cidade sustenta) e guarnição (Esquadrões estacionados)
- Muralha (nenhuma, Paliçada, Pedra ou Bastião) e situação de guerra: sitiada ou ocupada, e por quem (ver [[Sistema de Guerra/Descrição|Sistema de Guerra]])
- Guardas — quantidade depende do nível da cidade e da Criminalidade, com um mínimo definido pela lei da facção
- Lotes de construção (fixos, marcados no mapa; vazios ou ocupados)
- Capacidade de NPCs (pelo nível) e ocupação atual (ver [[Sistema de População/Descrição|Sistema de População]])
- Voto na assembleia — vem da cultura do dono (ver Sistema de Facção)
