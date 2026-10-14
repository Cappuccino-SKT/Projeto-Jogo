> **Atualização recente (revisão geral).** O que mudou neste documento:
> - Cada Mapa Local comporta **uma única cidade**.
> - Novos atributos: Dinheiro (banco da cidade), Criminalidade, Guardas, Esquadrões.
> - **Derivados das leis da facção:** Itens proibidos. **Derivado do Fator de Escala:** Conscritos.
> - A Cultura passa a ser a do dono mais uma proporção de outras culturas.
> - Como uma cidade passa a fazer parte de uma facção (vontade, conquista ou fundação).

## Sistema de Cidade

A Cidade é uma área dentro de um mapa local que possua cidade. Cada Mapa Local comporta **no máximo uma cidade**. As cidades são locais de alta concentração de NPCs, com capacidade de construção tanto de produção quanto para comércio.

A princípio teremos níveis para as cidades, esse nível significa o desenvolvimento da cidade sendo: nível 1, assentamento; nível 2, vilarejo; nível 3, cidade; e nível 4, metrópole. Dentro do jogo algumas regiões já terão cidades devido à história do jogo e às facções já existentes. A ideia é que do nível 1 até o 3 os jogadores consigam criar por conta própria, porém o nível 4 vai ficar limitado apenas para as capitais das facções principais. A ideia é que as metrópoles sejam uma cidade que ocupe o tamanho inteiro do mapa local, sendo algo muito grande e limitado para ter poucos. A única forma de o jogador possuir uma metrópole é pela conquista ou por outros sistemas que iremos desenvolver.

### Facção da cidade

Uma cidade pertence a uma facção por **vontade** (o dono oferece lealdade), por **conquista** ou por ter sido **fundada** em território de facção. A cidade herda **todas as leis** da facção (não há leis locais). Ver Sistema de Facção.

O jogador pode fundar um assentamento (nível 1) em um mapa sem dono, ou em território de facção, onde ele nasce vinculado à facção dominante. Para fundar uma facção própria, a cidade precisa estar no nível 3 e fora de qualquer facção.

### Atributos

- Nome
- A qual Mapa Local ela pertence (um único)
- Quantidade de NPCs (NPCs reais; a população total é NPCs × Fator de Escala)
- Facção
- Cultura — a cultura do dono (base) mais uma proporção agregada de outras culturas
- Construções
- Esquema (caso seja uma cidade gerada pelo sistema)
- Quantidade de NPCs de cada classe social
- Dinheiro da cidade (o "banco da cidade" do Sistema Econômico)
- Inventário da cidade
- Felicidade da cidade — afetada pela distância entre as leis da facção e as leis desejadas pela cultura, pela Criminalidade e por convocações altas
- Criminalidade — valor agregado de crime da cidade (ver Sistema de Crime e Justiça)
- Bens de consumo de cada classe social (usado para causar eventos)
- Nível da cidade
- Itens proibidos — **derivado das leis da facção**
- Conscritos disponíveis — **derivado**: população × lei de Conscrição ÷ Fator de Escala, menos os já alistados (usado para o sistema de Batalha)
- Esquadrões da cidade (os que a cidade sustenta)
- Guardas — quantidade depende do nível da cidade e da Criminalidade, com um mínimo definido pela lei da facção
- Lotes vazios para construção
- Voto na assembleia — vem da cultura do dono (ver Sistema de Facção)
