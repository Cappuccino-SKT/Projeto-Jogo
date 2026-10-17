> **Refeita na rodada IP3 (2026-10-17)**, com as regras da IP1 (família por construção e escada de níveis; ver [[Sistema de Produção/Descrição|Produção]]), a lista final da IP2 (ver [[Sistema de Itens/Descrição|Itens]]) e as receitas da IP3 (ver [[Sistema de Produção/Receitas|Receitas]]).
>
> Rascunho em construção. **[Proposta]** = sugestão ainda não decidida. As Funções, as categorias e os produtos por nível foram decididos nas rodadas IP1 a IP3; as Funções dos serviços públicos ainda são propostas.

## Funções e Construções

**Descrição geral**

Tabela mestre que liga cada **Função** (o trabalho do NPC) à **construção** onde ela é exercida e ao que a construção produz. É a base do emprego (ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]]) e da validação dos dados: o jogo deve avisar se uma receita citar uma construção ou item que não exista.

Cada construção de produção tem níveis de 1 a 5. A coluna de produtos mostra o que **cada nível libera** de novo (os níveis seguintes mantêm o que já havia). Os itens seguem os nomes do [[Sistema de Itens/Descrição|Sistema de Itens]].

### Construções de produção

| Construção | Função | Categoria de produção | Produtos por nível |
|---|---|---|---|
| Armeiro | Armeiro | Especialista | **N1:** Arcabuz, Cartucho, Bombarda, Bala de Canhão (Era 2); **N2:** Mosquete, Carabina e Canhão de Aço (Era 3); **N5:** Mosquete, Carabina e Canhão de Aço Mágico, Fuzil Raiado (Era 4) |
| Artesão | Artesão | Especialista | **N1:** Anel de Cobre, Cajado Simples de Cobre; **N2:** Cajado Arcano de Aço (Era 2); **N3:** Joia de Mana; **N4:** Coroa, Anel de Ouro, Anel de Mana; **N5:** Joias, Ornamento; Cajado Arcano de Aço Mágico e Cetro (Era 4) |
| Açougue | Açougueiro | Manufaturada | **N1:** Carne Crua, Ossos e Couro Cru (uma receita por animal) |
| Boticário | Boticário | Especialista | **N1:** Curativo; **N4:** Remédio |
| Carpinteiro | Carpinteiro | Pela receita | **N1:** Tábua, Flecha, Virote; Arco Curto e Longo de Madeira, Besta Leve de Madeira, Escudo de Madeira, Cajado Simples de Madeira; **N2:** Móveis de Madeira; Arcos Compostos e Bestas de Aço (Era 2); **N4:** Móveis Refinados; **N5:** Arcos Arcanos e Bestas de Aço Mágico (Era 4) |
| Costureiro | Costureiro | Pela receita | **N1:** Tecido de Algodão, Tecido de Lã, Couro Refinado, Roupa Comum; roupas dos conjuntos Pobre, Trabalhador, Artífice e Furtivo; armadura Leve de Couro; **N2:** Sela; conjuntos Mercador e Nobre; armadura Leve Cravejada (Era 2); **N4:** Roupa Fina; **N5:** armadura Leve Arcana (Era 4) |
| Cozinheiro | Cozinheiro | Pela receita | **N1:** Pão, Sopa, Peixe Defumado, Ração Militar; **N2:** Torta de Fruta, Linguiça, Queijo; **N4:** Assado Temperado; **N5:** Presunto Curado, Doce de Especiarias |
| Destilaria | Destilador | Pela receita | **N2:** Cerveja; **N4:** Vinho, Perfume; **N5:** Licor. **[Proposta]** Sem produto no nível 1: é construída já no nível 2 (V3) |
| Fazenda | Fazendeiro | Braçal | **N1:** Grão, Legumes, Fruta, Algodão, Especiarias, Folha de Fumo, Flor Espiritual (com Adubo no estoque, +25%) |
| Ferreiro | Ferreiro | Pela receita | **N1:** Espada, Lança, Machado e Clava de Ferro (Curtos e Longos), Escudo de Ferro, armaduras Média e Pesada de Ferro, Chapéu de Mineiro; **N2:** Grilhões; as mesmas armas, escudo e armaduras de Aço (Era 2); **N5:** de Aço Mágico (Era 4) |
| Forja Arcana | Mago de Ofício | Especialista | **N1:** Barra de Aço Mágico (Era 4; exceção à escada: o nível dá velocidade e vagas) |
| Metalurgia | Metalúrgico | Pela receita | **N1:** Barra de Ferro, Barra de Cobre; **N2:** Barra de Aço (Era 2); **N4:** Barra de Ouro |
| Misturador | Misturador | Pela receita | **N1:** Adubo; **N2:** Fumo; Pólvora (Era 2); **N4:** Pó Espiritual |
| Pasto | Rancheiro | Braçal | **N1:** Galinha, Porco, Ovelha, Vaca, Cavalo (exige 2 animais da espécie no estoque) |
| Pedreira | Pedreiro | Manufaturada | **N1:** Bloco de Pedra, Cerâmica |
| Pesqueiro | Pescador | Braçal | **N1:** Peixe (mapas com rio, lago ou litoral) |
| Posto de Coleta | Coletor | Braçal | **N1:** Água, Pedra, Argila, Tora, Minério de Ferro, de Cobre e de Ouro, Carvão, Pedra de Mana |
| Rancho | Rancheiro | Braçal | **N1:** Ovo, Lã, Leite, Dejetos Animais (1 ciclo por animal por dia; não gasta os animais) |

**Categoria "pela receita"** (IP3-11): conta o Trabalho **Manufaturado** nos bens de Tier 1 e 2 e nos itens de degrau 3 (Ferro, Cobre, Madeira, Couro), e o **Especialista** nos bens de Tier 3 e 4 e nos itens de degrau 4 e 5. Isso generaliza o "Ferreiro e Costureiro mudam nos níveis altos". O Armeiro, o Artesão, o Boticário e a Forja Arcana são sempre Especialistas.

**Observações:**

- As receitas de cada produto (insumos, lote, horas) estão em [[Sistema de Produção/Receitas|Receitas]].
- Cada produto está no nível mínimo da escada: pelo Tier nos bens (Tier 1 no nível 1, 2 no 2, 3 no 4, 4 no 5) e pelo degrau do material nos itens com Rank (degrau 3 no nível 1, 4 no 2, 5 no 5). Os níveis seguintes mantêm o que já havia, sobem o teto de Rank e deixam a receita 10% mais rápida por nível.
- A era trava só os materiais de equipamento e os itens com Rank; os bens de consumo existem desde a Era 1 (IP3-9).
- **Fazenda, Pasto, Posto de Coleta, Pesqueiro, Rancho e Açougue** liberam tudo no nível 1: o nível aumenta a eficiência, não o catálogo.

### Construções civis e de serviço

| Construção | Função **[Proposta]** | O que faz |
|---|---|---|
| Construtor (níveis 1 a 3) | **Operário** (Trabalho Braçal) e **Engenheiro** (gerente da obra; Inteligência) | Constrói e mantém as construções da cidade. A Inteligência exigida numa obra é a do Engenheiro; sem ele, só obras de nível 1 (rodada IP3-11) |
| Universidade | Professor | Educação (ver Sistema de Trabalho e Propriedade) |
| Senzala, Casa, Casarão, Mansão, Palácio | (moradia) | Moradias por classe (ver [[Sistema de População/Descrição\|Sistema de População]]) |
| Mercado | Lojista | Loja geral |
| Taverna | Taverneiro | Entretenimento; quadro de contratos |
| Quartel | Oficial instrutor | Alistamento e formação de Esquadrões |
| Prisão | Carcereiro (Guarda) | Cumprimento de pena |
| Casa da Moeda | Moedeiro | Cunhagem |
| Templo | Sacerdote (Civil; gerente Nobre) | Culto da facção: cobertura (cada Sacerdote atende 20 NPCs), Felicidade, oferendas e festivais; na Religiosa, menos doença (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| Posto Comercial | Mercador | Envio de caravanas civis |
| Laboratório | Pesquisador | Pontos de Pesquisa |
| Torre Arcana | Mago | Pontos Arcanos |
| Encantaria | Mago de Ofício | Encantamento |
| Hospital | Curandeiro | Cura do grupo do jogador |

### Diferenças em relação aos exemplos do Sistema de Produção

O Sistema de Produção cita alguns nomes que **não aparecem** no canvas. Sugestão de equivalência:

| Citado no texto | No canvas |
|---|---|
| Lenhador, Mineiro, Coletor | **Coletor**, no Posto de Coleta (madeira, pedra, minérios, água e outros recursos brutos) |
| Rancheiro | **Rancheiro**, no Pasto (animais) e no Rancho (ovo, lã, leite, dejetos) |
| Oleiro | **Pedreiro**, na Pedreira (tijolo e bens de argila) |
| Alfaiate | **Costureiro** |
| Joalheiro | **Artesão** (Joias, Ornamento e anéis, inclusive o Anel de Mana) |
| Engenheiro | **Construtor**: o gerente da obra (rodada IP3-11) |
| Produtor Mágico | Forja Arcana e Encantaria (Mago de Ofício) |
| Operário | **Construtor**: faz a obra e a manutenção (rodada IP3-11) |
| Comerciante | Mercado (Lojista) e Posto Comercial (Mercador) |

### Perguntas em aberto

1. **Destilaria no nível 2:** confirmar a exceção à escada (rodada V3).
2. **Funções dos serviços públicos** (Lojista, Taverneiro, Carcereiro, Moedeiro...): confirmar nas rodadas de propostas (V4 e V5).
3. **Valores a testar:** vagas por nível (a tabela geral 2, 3, 5, 7 e 10 vale para todas as construções, IP3-11) e habilidade mínima de cada nível.
