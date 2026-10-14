> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Tecnologia e Pesquisa

**Descrição geral**

A tecnologia é como o mundo avança: libera construções, receitas, itens melhores, leis e níveis de cidade. O sistema tem **duas camadas**, inspiradas no Victoria 3, onde o conhecimento se espalha pelo mundo:

| Camada | Afeta | Como é desbloqueada | Papel |
|---|---|---|---|
| **Tecnologia Mundial** | Todas as facções | Por **tempo**: cada uma tem um prazo e, quando ele chega, vale para o mundo todo | Simula o conhecimento se espalhando; puxa todas as facções para frente e impede que uma fique para trás para sempre |
| **Tecnologia de Facção** | Só a facção que pesquisou | Por **pesquisa** (Pontos de Pesquisa) | Diferencia as facções; é onde entram as escolhas, a cultura e a disputa |

Princípios:

1. **Desbloqueios concretos.** Uma tecnologia libera coisas (construção, receita, item, lei, nível de cidade). Bônus percentuais são raros e ficam para casos como a cunhagem.
2. **Uma tecnologia nunca é esquecida.**
3. **Tudo é dado.** Tecnologias ficam em JSON, como os eventos (ver [[Arquitetura/Descrição|Arquitetura]]).

---

## Tecnologia Mundial

- Lista fixa de **marcos mundiais**. Cada marco tem uma **janela** no calendário (por exemplo, entre o ano 3 e o ano 5); o momento exato é sorteado dentro dela. Isso varia as partidas sem perder o controle do ritmo.
- **Adiantar:** quanto mais facções já pesquisaram as tecnologias ligadas ao marco, mais cedo ele chega, mas **nunca fora da janela**. Guerras e eventos não mexem no prazo.
- Ao chegar, todas as facções recebem o marco ao mesmo tempo. Isso dispara o evento **"Descoberta de Tecnologia"** (escopo Mundo; ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]) e pode mexer no Mercado Mundial (ver [[Sistema Econômico/Descrição|Sistema Econômico]]).
- **O que um marco faz:** os marcos maiores **abrem a era seguinte** da Tecnologia de Facção; sem eles, nenhuma facção pesquisa a próxima era. Quem pesquisa muito avança dentro da era, mas o mundo inteiro passa de era junto. Alguns marcos também liberam algo direto para todos (por exemplo, uma receita básica).
- **Quantidade:** 3 marcos maiores (abrem as eras 2, 3 e 4) e de 5 a 8 marcos menores entre eles.
- **Difusão:** uma Tecnologia de Facção fica **10% mais barata para cada facção que já a tem**, até **−50%**. É o "conhecimento se espalhando" também dentro da camada de facção.

## Tecnologia de Facção

### Quem tem

A tecnologia de facção pertence à **Facção**: todas as cidades dela usam o que foi descoberto. O jogador usa a tecnologia da facção a que pertence; se fundar a própria facção, passa a pesquisar a dele.

**Vila do jogador sem facção:** recebe os marcos mundiais e, se tiver um Laboratório, pesquisa a própria árvore com uma **penalidade de custo**.

### Pontos de Pesquisa

- Gerados no **Laboratório**, uma construção de produção cujo produto é **não tangível** (ver [[Sistema de Construção/Descrição|Sistema de Construção]]).
- Produzidos pelos NPCs que trabalham nele: trabalhadores da classe **Civil**, com bônus para Inteligência alta; um **Nobre** pode ser o gerente (ver [[Sistema de Classe Social/Descrição|Sistema de Classe Social]] e [[Sistema de NPC/Descrição|Sistema de NPC]]). Níveis maiores de Laboratório produzem mais.
- O **Banco da Facção** paga a manutenção dos Laboratórios. A lei de **imposto para pesquisa** (ideologia Tecnocrata) aumenta esse orçamento (ver [[Sistema de Cultura/Descrição|Sistema de Cultura]] e [[Sistema de Facção/Descrição|Sistema de Facção]]).
- Os pontos de todas as cidades vão para um único projeto de pesquisa da facção.
- **[Proposta]** Pontos por trabalhador por dia = Inteligência ÷ 50 × multiplicador do nível do Laboratório. Valores a testar.

### Organização

Árvore com pré-requisitos, em **5 ramos** e **4 eras**. Pode ganhar novas camadas no futuro.

| Ramo | Exemplos de desbloqueio |
|---|---|
| **Agricultura e Produção** | Novas receitas, construções de produção, mais produtividade |
| **Construção** | Novas construções, níveis de construção, níveis de cidade |
| **Militar** | Armas e armaduras de Rank maior, Quartel, pólvora, tamanho e equipamento dos Esquadrões |
| **Economia** | Mercado, Casa da Moeda, cunhagem, caravanas |
| **Administração** | Leis novas, Prisão, Templo, controle de Criminalidade |

**Eras e Tier:** as eras acompanham os Tiers de material. A era 1 libera Tier 1, a era 2 libera Tier 2 e assim por diante; o Tier 0 está disponível desde o início. O Rank máximo de armas e equipamentos sobe junto (ver [[Sistema de Itens/Descrição|Sistema de Itens]]). **[Refinar depois]** tabela exata de Rank por era.

### Quem escolhe o que pesquisar

- **Facções de IA:** escolhem pelas prioridades da cultura e da ideologia (Guerreira prefere Militar, Escravista prefere Economia, Tecnocrata prefere Produção e Construção). Ver [[Sistema de IA/Descrição|Sistema de IA]].
- **Jogador:** escolhe quando é o líder da facção.

### Difusão entre facções

Além do desconto da difusão, uma facção pode obter uma tecnologia de outra de três formas:

1. **Conquista:** tomar uma cidade com Laboratório dá uma tecnologia que a facção perdedora tinha e a vencedora não.
2. **Diplomacia:** compra ou troca em acordo.
3. **Roubo:** missão de crime (ver [[Sistema de Crime e Justiça/Descrição|Sistema de Crime e Justiça]]).

### Começo de jogo

As facções **não começam iguais**: cada cultura começa com 2 ou 3 tecnologias próprias da era 1, ligadas à arte e aos traços dela.

### Tamanho e ritmo

- Entre **40 e 60** tecnologias de facção na primeira versão.
- O progresso conta **por estação** (16 dias); não existem meses.
- A primeira meta (fatia vertical) **não tem tecnologia**.

---

## Magia

A Magia **não entra na árvore**. Ela será um sistema próprio (o próximo da fila), mas usa a mesma mecânica de pontos, gerados num local próprio (por exemplo, uma Torre Arcana). A Tecnocrata não pesquisa magia; a Arcana pesquisa as duas, mais devagar.

## Parte técnica

- **Dados:** marcos mundiais e tecnologias de facção em JSON na pasta `data/`, com validação ao carregar (o jogo avisa se uma tecnologia citar construção, receita, item ou lei inexistente).
- **LOD:** a pesquisa é somada por estação para todas as facções; só a facção do jogador mostra o progresso diário na interface.
- **Save:** guarda os marcos já chegados, as tecnologias de cada facção, o projeto atual e os pontos acumulados.
- **Depuração:** painel (Dear ImGui) para liberar uma tecnologia ou adiantar um marco.

### Ligação com outros sistemas

| Sistema | Ligação |
|---|---|
| Construção | Laboratório produz pesquisa; tecnologias liberam construções e níveis |
| Produção / Itens | Tecnologias liberam receitas, Tiers e Ranks |
| Facção | Dona da tecnologia de facção; Banco da Facção paga os Laboratórios |
| Cultura | Prioridades de pesquisa por ideologia; tecnologias iniciais por cultura; Tecnocrata e imposto para pesquisa |
| Econômico | Cunhagem e tecnologias que aumentam o dinheiro do mundo; marcos mexem no Mercado Mundial |
| Eventos | "Descoberta de Tecnologia" como evento de escopo Mundo |
| Crime e Justiça | Roubo de tecnologia como missão |
| NPC | Inteligência define a produtividade no Laboratório |
| Tempo | Prazos dos marcos e progresso por estação |
| Magia | Mesma mecânica de pontos, árvore separada |

### Perguntas em aberto

1. **Valores a testar:** janelas dos marcos, quanto a pesquisa do mundo adianta um marco, pontos por trabalhador, custo de cada tecnologia, penalidade da vila sem facção.
2. **Lista dos marcos mundiais** (3 maiores e 5 a 8 menores) e o que os menores liberam.
3. **Lista inicial de tecnologias** e tecnologias iniciais de cada cultura.
4. **Cultura Agrária/Naturalista** (traço candidato, contra a tecnologia): como ela interage com a pesquisa.
