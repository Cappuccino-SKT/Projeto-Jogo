> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Recursos

**Descrição geral**

Os recursos são atributos dos **Mapas Locais**. Cada Mapa Local tem uma variedade de recursos que podem ser explorados ali, e a extração só é possível por **construções específicas** (Fazenda, Pasto, Posto de Coleta e Pesqueiro). A lista completa, com o item que cada recurso gera, está em [[Sistema de Recursos/Recursos Existentes|Recursos Existentes]].

- Cada mapa tem de **3 a 6 recursos**; cada região tem 2 a 3 recursos de **destaque** (produtividade alta) e falta de outros, para obrigar o comércio ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- O que está disponível numa região é limitado pela lista de recursos do Mapa Local: uma mina só existe e produz onde há o recurso mineral ([[Sistema de Produção/Descrição|Produção]]).

### Produtividade por estação

Os recursos **não se esgotam**. Em vez de um sistema de esgotamento, cada recurso tem uma **produtividade por estação** (por exemplo, a floresta rende menos no inverno, a colheita é maior no outono). A tecnologia e as construções de nível maior aumentam a produtividade. Os valores por estação e por bioma, e os eventos de clima que os reduzem, estão no [[Sistema de Clima e Estações/Descrição|Clima e Estações]]. *Exemplo:* na tabela base, a Fazenda rende 75% na primavera, 100% no verão, 150% no outono e 25% no inverno.

### Pesca

Decisão do autor na revisão (V1-4): **a pesca volta ao jogo** (o Peixe tinha sido descartado), porque o mundo já tem portos.

- **Recurso Peixe** nos mapas com rio, lago ou litoral (marcados na camada "Recursos" do Tiled, área de pesca).
- **Construção nova: Pesqueiro** (Braçal; Função **Pescador**; níveis 1 a 5, que aumentam a eficiência, como a Fazenda).
- **Item:** Peixe (Recurso Bruto, Animal, Tier 0). As receitas que usam o Peixe ficam para a rodada de receitas.
- **[Proposta] Por estação:** 100% na primavera, no verão e no outono; 75% no inverno; 25% no inverno dos biomas frios (rios congelados).
- **Onde se destaca:** o litoral Escravista, que tem o porto de pesca (6,6), e o grande rio Autoritário.

---

## Parte técnica

- **Dados:** a lista de recursos de cada Mapa Local fica em `mapas.json`, com a produtividade (normal ou destaque); a tabela por estação e bioma fica em `clima.json`.
- **Frequência:** a produtividade muda no 1º dia de cada estação; a produção Braçal a usa a cada ciclo de produção.
- **Save:** nada próprio (a lista de recursos é fixa; a produtividade sai da estação).
- **Fatia vertical:** usa Grão e Água no mapa da cidade.

## Ligação com outros sistemas

| Sistema | O que os Recursos fornecem ou recebem |
|---|---|
| Mapa | Lista de recursos por Mapa Local |
| Mundo e Estado Inicial | Recursos de destaque e escassos por região |
| Produção | Fazenda, Pasto, Posto de Coleta e Pesqueiro extraem os recursos |
| Itens | Cada recurso gera um item de Matéria-Prima (ver [[Sistema de Recursos/Recursos Existentes\|Recursos Existentes]]) |
| Clima e Estações | Produtividade por estação e por bioma; eventos extremos |
| Tecnologia | Tecnologias de Agricultura e Produção aumentam a produtividade |
| Comércio | A escassez regional cria as rotas |

## Perguntas em aberto

1. **Produtividade base** de cada recurso (quanto um trabalhador extrai por dia): fica para as rodadas de Itens e Produção e para a V2.
