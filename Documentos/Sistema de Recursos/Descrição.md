> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Recursos

**Descrição geral**

Os recursos são atributos dos **Mapas Locais**. Cada Mapa Local tem uma variedade de recursos que podem ser explorados ali, e a extração só é possível por **construções específicas** (Fazenda, Pasto, Posto de Coleta e Pesqueiro). A lista completa, com o item que cada recurso gera, está em [[Sistema de Recursos/Recursos Existentes|Recursos Existentes]].

- Cada mapa tem de **3 a 6 recursos**; cada região tem 2 a 3 recursos de **destaque** (produtividade alta) e falta de outros, para obrigar o comércio ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- O que está disponível numa região é limitado pela lista de recursos do Mapa Local: uma mina só existe e produz onde há o recurso mineral ([[Sistema de Produção/Descrição|Produção]]).

### Produtividade por estação

Os recursos **não se esgotam**. Em vez de um sistema de esgotamento, cada recurso tem uma **produtividade por estação** (por exemplo, a floresta rende menos no inverno, a colheita é maior no outono). A tecnologia e as construções de nível maior aumentam a produtividade. Os valores por estação e por bioma, e os eventos de clima que os reduzem, estão no [[Sistema de Clima e Estações/Descrição|Clima e Estações]]. *Exemplo:* na tabela base, a Fazenda rende 75% na primavera, 100% no verão, 150% no outono e 25% no inverno.

### Produtividade base (rodada V2-9)

- **Produtividade:** recurso normal × 1,0; de destaque × 1,5; ausente, não existe.
- **Construções Braçais por recurso, em cada Mapa Local:** até 2 num recurso normal e até 4 num de destaque. A Água não tem limite (poço).
- A extração segue as [[Sistema de Produção/Receitas|Receitas]]: 1 unidade a cada 0,5 h (Tier 0), × produtividade × estação.
- **Conta de comida** de uma cidade de 100 NPCs (100 Pães por dia):

| Precisa de | Quanto | Quem faz | Trabalhadores |
|---|---|---|---|
| Grão | 50 por dia | 1 Fazendeiro (habilidade 50, 8 h) faz 16 por dia a 100% | 3 a 4 |
| Água | 25 por dia | Coletor | 1 a 2 |
| Pão | 100 por dia | 1 Cozinheiro faz 8 por dia | 12 a 13 |

  Isso dá cerca de 18% da cidade para a comida.
- **A testar:** o Cozinheiro é o gargalo (13 cozinheiros para 4 fazendeiros). Se incomodar, o lote do Pão sobe para 8.

### Pesca

Decisão do autor na revisão (V1-4): **a pesca volta ao jogo** (o Peixe tinha sido descartado), porque o mundo já tem portos.

- **Recurso Peixe** nos mapas com rio, lago ou litoral (marcados na camada "Recursos" do Tiled, área de pesca).
- **Construção nova: Pesqueiro** (Braçal; Função **Pescador**; níveis 1 a 5, que aumentam a eficiência, como a Fazenda).
- **Item:** Peixe (Recurso Bruto, Animal, Tier 0). As receitas que usam o Peixe ficam para a rodada de receitas.
- **Por estação:** 100% na primavera, no verão e no outono; 75% no inverno; 25% no inverno dos biomas frios (rios congelados).
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

1. **Valores a testar:** produtividade (× 1,0 e × 1,5), limite de construções por recurso e o gargalo do Cozinheiro (rodada V2-9).
