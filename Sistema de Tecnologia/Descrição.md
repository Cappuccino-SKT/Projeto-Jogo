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

- Lista fixa de **marcos mundiais**. Cada marco tem um **prazo** no calendário do jogo; ao chegar, todas as facções o recebem ao mesmo tempo.
- A chegada de um marco dispara o evento **"Descoberta de Tecnologia"** (escopo Mundo; ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]) e pode mexer no Mercado Mundial (ver [[Sistema Econômico/Descrição|Sistema Econômico]]).
- **[Proposta]** Os marcos mundiais **abrem as eras**: a Tecnologia de Facção de uma era só pode ser pesquisada depois que o marco que abre essa era chegou. Assim, quem pesquisa muito avança dentro da era, mas o mundo inteiro anda junto de uma era para a outra.
- **[Proposta]** **Difusão:** uma Tecnologia de Facção fica mais barata quanto mais facções já a conhecem (por exemplo, −10% por facção que a tem, até −50%). É o "conhecimento se espalhando" também dentro da camada de facção.
- **[Em aberto]** O prazo é uma data fixa ou uma janela com sorteio (por exemplo, entre os anos 3 e 5)? Algo pode adiantar ou atrasar um marco (pesquisa total do mundo, eventos, guerras)?

## Tecnologia de Facção

### Quem tem

A tecnologia de facção pertence à **Facção**: todas as cidades dela usam o que foi descoberto. O jogador usa a tecnologia da facção a que pertence; se fundar a própria facção, passa a pesquisar a dele.

### Pontos de Pesquisa

- Gerados no **Laboratório**, uma construção de produção cujo produto é **não tangível** (ver [[Sistema de Construção/Descrição|Sistema de Construção]]).
- Produzidos pelos NPCs que trabalham nele; a produtividade depende da **Inteligência** do NPC (ver [[Sistema de NPC/Descrição|Sistema de NPC]]). Níveis maiores de Laboratório produzem mais.
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

Além do desconto da difusão (ver Tecnologia Mundial), uma facção pode obter uma tecnologia de outra de três formas:

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

1. **Prazo dos marcos mundiais:** data fixa ou janela com sorteio? Algo adianta ou atrasa?
2. **Quantos marcos mundiais** e se eles só abrem eras ou também liberam coisas diretamente.
3. **Difusão:** confirmar o desconto por facção que já conhece a tecnologia e o limite.
4. **Vila do jogador sem facção:** recebe só os marcos mundiais ou tem alguma pesquisa própria?
5. **Quem trabalha no Laboratório:** qual Classe Social (ver [[Sistema de Classe Social/Descrição|Sistema de Classe Social]]).
6. **Valores a testar:** pontos por trabalhador, custo de cada tecnologia, descontos.
7. **Lista inicial de tecnologias** e tecnologias iniciais de cada cultura.
8. **Cultura Agrária/Naturalista** (traço candidato, contra a tecnologia): como ela interage com a pesquisa.
