> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Comércio Civil e Caravanas

**Descrição geral**

O comércio entre cidades é feito por **caravanas civis**. Elas ligam os Mercados Locais (ver [[Sistema Econômico/Descrição|Sistema Econômico]]): levam o que sobra numa cidade para onde falta, aproximam os preços e movem dinheiro de uma cidade para outra. A Caravana Militar é outra coisa e continua no [[Sistema de Facção/Descrição|Sistema de Facção]].

---

## Caravanas no mapa

- A caravana civil **existe de verdade no mapa mundial**: é uma entidade simples que se move, pode ser vista e pode ser atacada.
- **Sempre real no Mapa Mundial**, inclusive longe do jogador: o ícone anda de mapa em mapa, o que custa quase nada. Só a simulação tile a tile fica para os Mapas Locais em LOD 0 e 1. Isso substitui o fluxo calculado por estação previsto antes (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).

## Donos

| Dono | Como funciona |
|---|---|
| **Cidade** | Os mercadores da cidade (NPCs abstratos) usam o **dinheiro da cidade**: levam o que sobra e trazem o que falta |
| **Jogador** | Caravanas próprias, montadas por ele |

A cidade precisa de um **Posto Comercial** (construção nova; ver [[Sistema de Construção/Descrição|Sistema de Construção]]) para enviar caravanas.

## Escolha da rota

- Pelo **lucro esperado**: diferença de preço entre os Mercados Locais **menos o custo da viagem** (distância, risco de bandidos e tarifas).
- Com o tempo, o comércio **aproxima os preços** das cidades.

## Informação de preços

- O jogador só vê os preços das **cidades que visitou**, com a **data da última visita**. A informação envelhece.
- **Notícias e contratos** podem trazer preços de outras cidades.

## Composição da caravana

| Parte | Função |
|---|---|
| **Animais de carga** | Capacidade, por peso |
| **Guardas contratados** | Defesa |
| **Manutenção** | Comida e salário dos guardas |

O jogador monta a dele. A da cidade vem pronta, conforme o **nível da cidade**.

## Tarifas e fronteiras

- Nova lei de facção: **Tarifa**. Cobra imposto sobre caravanas que **entram no território** da facção; o dinheiro vai para o **Banco da Facção**.
- **Facções em guerra não comerciam** entre si.
- A diplomacia pode criar um **embargo** ou um **acordo comercial** (estado diplomático "Comércio"), que **zera a tarifa**.

## Riscos

- **Bandidos** (ligados à Criminalidade; ver [[Sistema de Eventos/Descrição|Sistema de Eventos]] e [[Sistema de Crime e Justiça/Descrição|Sistema de Crime e Justiça]]), **guerra no caminho** e **clima** que atrasa a viagem.
- Uma caravana atacada perde **parte ou toda a carga**.
- **Contratos de escolta** nascem quando o risco de uma rota é alto (ver [[Objetivo e Progressão/Descrição|Objetivo e Progressão]]).

## Automação

- **Rota fixa:** comprar X na cidade A e vender na B, repetindo. Liberada por uma tecnologia do ramo **Economia** (ver [[Sistema de Tecnologia/Descrição|Sistema de Tecnologia]]).
- Antes disso, o jogador comercia pessoalmente ou dá ordens **viagem a viagem**.

## Dinheiro

- O comércio **move dinheiro entre cidades**: o total do mundo não muda. Cidades ricas e pobres surgem naturalmente.
- A **cunhagem** (fonte) e os **impostos** (sumidouro) continuam como estão no Sistema Econômico.

---

## Parte técnica

- **Fatia vertical:** não tem este sistema (só uma cidade).
- **Save:** caravanas ativas (posição, dono, carga, guardas, rota), rotas fixas do jogador e os preços conhecidos pelo jogador com a data.
- **Desempenho:** todas as caravanas andam de mapa em mapa no Mapa Mundial; só nos Mapas Locais em LOD 0 e 1 elas viram NPCs e se movem tile a tile.

### Ligação com outros sistemas

| Sistema | Ligação |
|---|---|
| Econômico | Diferença entre Mercados Locais; dinheiro move entre cidades |
| Facção | Lei de Tarifa; Banco da Facção; guerra, embargo e acordo comercial |
| Mapa e Viagem | Caravana sempre real no Mapa Mundial; tempo de viagem, encontros e escolta (ver [[Sistema de Viagem/Descrição\|Sistema de Viagem]]) |
| Construção | Posto Comercial |
| Inventário / Itens | Carga por peso |
| Eventos / Crime | Bandidos, clima, ataques |
| Objetivo e Progressão | Contratos de escolta; contratos e notícias trazem preços |
| Tecnologia | Rota fixa no ramo Economia |
| Início de Jogo | Estilo Mercador |

### Perguntas em aberto

1. **Valores a testar:** capacidade por animal, salário dos guardas, valor da Tarifa, quanto o risco pesa na rota.
2. **Animais de carga:** quais são e de onde vêm (recursos, criação).
3. **Guardas da caravana:** são NPCs reais ou valor de defesa abstrato?
4. **Fluxo em LOD 2:** fórmula do volume trocado por estação.
5. **Bônus do estilo Mercador** (ver [[Sistema de Início de Jogo/Descrição|Sistema de Início de Jogo]]).
