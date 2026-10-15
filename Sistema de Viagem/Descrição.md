> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Viagem e Mundo

**Descrição geral**

Define como o **grupo do jogador, as caravanas, os exércitos, as patrulhas e os bandos** se movem pelo Mapa Mundial: quanto tempo leva, o que acontece no caminho, o que ocorre quando passam por um Mapa Local e o que o jogador sabe do mundo (névoa). É a base da Guerra, do Comércio e das Notícias.

Lembretes: 1 hora de jogo = 40 s reais a 1x; a velocidade 8x só existe no Mapa Mundial; a malha tem 48 Mapas Locais (ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).

### Movimento de mapa em mapa

- Toda entidade anda **de Mapa Local em Mapa Local**, como num grafo. A posição é **mapa atual + próximo mapa + progresso (0 a 100%)**. Na tela, o ícone desliza do centro de um quadrado ao centro do vizinho.
- A rota escolhe o caminho **mais rápido**, não o mais curto (ver "Tempo de travessia"). Com 48 mapas, a busca é instantânea e centenas de entidades custam quase nada.
- *Exemplo:* uma caravana da Cidade Escravista (7,5) para a Capital Autoritária (1,4) passa por 6 ou 7 mapas, quadrado a quadrado.

### Começar e terminar uma viagem

- O jogador pode abrir o Mapa Mundial e escolher o destino **de qualquer lugar**, desde que não esteja em combate e não haja inimigo dentro do raio de alerta.
- Ao partir, o grupo some do Mapa Local e vira um ícone no Mapa Mundial. Ao chegar, aparece no **ponto de entrada de Viagem Rápida** do destino (ver [[Sistema de Mapa/Descrição|Sistema de Mapa]]). Vale também para um mapa vizinho.
- Andar até a borda continua sendo o outro jeito: é mais rápido (cerca de 1 hora para atravessar um Mapa Local a pé), mas exige atenção do jogador. A viagem pelo Mapa Mundial é automática e pode correr a 8x.

### Tempo de travessia

O tempo base para atravessar um Mapa Local é de **4 horas a pé**. Ele é multiplicado pelo terreno, pela infraestrutura do mapa, pelo tamanho do grupo, pela montaria e pela carga:

**Tempo (horas) = 4 × Terreno × Infraestrutura × Tamanho × Montaria × Carga**

**Terreno** (vem do mapa, definido nos dados junto com o desenho no Tiled):

| Terreno | × |
|---|---|
| Planície, litoral | 1,0 |
| Floresta, colinas, terras áridas | 1,25 |
| Floresta densa | 1,5 |
| Montanha | 1,5 |

**Infraestrutura** (o autor pediu que mapas mais desenvolvidos sejam mais fáceis de atravessar). **[Proposta]** Valores:

| Infraestrutura do mapa | × |
|---|---|
| Selvagem (sem estrada) | 1,0 |
| Trilha | 0,9 |
| Estrada ligando cidades | 0,75 |
| Mapa com cidade de nível 3 ou 4 (ruas e pontes) | 0,65 |

Vale o melhor valor que o mapa tiver. **[Refinar depois]** Construir estradas (pela facção ou pelo jogador), ligado à Construção e à Tecnologia.

**Tamanho do grupo** (o autor pediu que um exército ande muito mais devagar que uma pessoa sozinha). Conta cada pessoa; um Esquadrão conta como 100. **[Proposta]**

| Pessoas no grupo | Exemplo | × |
|---|---|---|
| 1 a 5 | Jogador e alguns companheiros | 1,0 |
| 6 a 20 | Grupo grande, caravana pequena | 1,1 |
| 21 a 100 | Bando de bandidos, caravana grande | 1,25 |
| 101 a 500 | Exército de 1 a 5 Esquadrões | 1,5 |
| 501 a 2.000 | Exército de 6 a 20 Esquadrões | 1,75 |
| Mais de 2.000 | Grande exército | 2,0 |

**Montaria, carga e tipo:**

| Situação | × |
|---|---|
| A pé | 1,0 |
| Todos montados (Cavalo e Sela) | 0,67 (50% mais rápido) |
| Carga acima do limite | 1,25 |
| Caravana (anda no passo dos animais de carga) | 1,33 |
| Neve, chuva forte | definidos no futuro sistema de Clima e Estações |

O grupo anda na velocidade do **membro mais lento**. **[Proposta]** O tempo mínimo é de 1 hora por mapa.

**Exemplos:**

| Quem | Onde | Conta | Tempo |
|---|---|---|---|
| Mensageiro a cavalo | Mapa da Capital Autoritária (planície, cidade nível 4) | 4 × 1,0 × 0,65 × 1,0 × 0,67 | cerca de 1,7 h |
| Jogador e 3 companheiros a pé | Mapa livre de floresta, sem estrada | 4 × 1,25 × 1,0 × 1,0 | 5 h |
| Caravana de 15 pessoas | Estrada na planície | 4 × 1,0 × 0,75 × 1,1 × 1,33 | cerca de 4,4 h |
| Exército de 20 Esquadrões | Montanha sem estrada | 4 × 1,5 × 1,0 × 1,75 | 10,5 h |

Cruzar o mundo de leste a oeste (8 mapas) em terreno comum leva cerca de 32 horas de marcha para um viajante a pé (2 dias, com as noites de acampamento) e cerca de 56 horas para um grande exército (3,5 dias). Assim uma campanha de guerra cabe em uma estação (16 dias), mas a distância pesa.

### Noite, comida e marcha forçada

- Por padrão, o grupo **acampa à noite**, das 22 h às 6 h (8 horas paradas), e come **uma refeição por pessoa por dia**, tirada do inventário do grupo.
- **Marcha forçada** (opção do jogador): viaja sem parar. Cada noite sem dormir reduz a Felicidade do grupo e aumenta a chance de encontro (cansaço).
- **Sem comida:** a Felicidade cai rápido e a Vida cai aos poucos.
- *Exemplo:* 5 mapas a pé em terreno comum (20 horas de marcha) terminam no segundo dia, com uma noite de acampamento.
- Durante a viagem a 8x, qualquer encontro ou chegada **volta a velocidade para 1x**, como no combate.

### Encontros no caminho

**Encontros do jogador.** A cada Mapa Local atravessado, sorteia-se um encontro:

| Estado do mapa | Chance base |
|---|---|
| Território de facção | 5% |
| Em disputa | 15% |
| Sem dono | 20% |

A chance sobe com a **ameaça bandida** da região (ver [[Sistema de Eventos/Descrição|Eventos]]) e com a marcha forçada.

- **Os bandidos avaliam a força:** se o grupo do jogador for mais de 2 vezes mais forte, eles não atacam.
- **No encontro:** o jogo pausa e oferece **Lutar**, **Fugir**, **Negociar ou subornar** e **Render-se**.
  - **Lutar:** carrega aquele Mapa Local em LOD 0, com o grupo no ponto onde estava e os inimigos por perto; é o combate local normal (ver [[Sistema de Combate/Descrição|Combate]]).
  - **Fugir:** a chance depende da diferença de velocidade (a montaria ajuda). Se falhar, vira luta.
  - **Negociar ou subornar:** usa o Carisma; o preço sobe com a força dos bandidos.
  - **Render-se:** é a captura que já existe no Combate.
- *Exemplo:* o Mercador atravessa o mapa em disputa (6,4) com 4 guardas; aparecem 8 bandidos, que se acham mais fortes e atacam; o jogador suborna com 200 moedas e segue.

**Encontros que não são de luta** (sorteados junto com os hostis):

| Encontro | O que acontece |
|---|---|
| Mercador viajante | Compra e venda ali mesmo, com preços de outra região |
| Patrulha da facção | Se o jogador tiver Recompensa nessa facção, tenta prender; se não, segue |
| Refugiados | Fogem de guerra ou fome; podem pedir comida ou virar recrutas |
| Peregrinos | Indo à Capital Religiosa; trazem rumores |
| Caravana em apuros | Sendo atacada; ajudar dá dinheiro e relação com o dono |

**Encontros da IA, longe do jogador.** Caravana × bandidos e patrulha × bandidos usam o mesmo **cálculo rápido de forças** das guerras de IA (ver [[Sistema de Facção/Descrição|Facção]]), com um pouco de sorte. A caravana que perde fica com 30% a 100% de perda da carga (quanto maior a diferença de força, maior a perda). O resultado vira notícia e pode criar um **contrato de escolta**.

### Entidades em trânsito

| Entidade | Dono | O que faz |
|---|---|---|
| Grupo do jogador | Jogador | Vai aonde o jogador mandar |
| Caravana civil | Cidade, Rico ou jogador | Leva mercadorias entre Mercados (ver [[Sistema de Comércio/Descrição|Comércio]]) |
| Exército | Facção | Esquadrões com a Caravana Militar (ver Facção) |
| Bando de bandidos | Nenhum | Ronda mapas sem dono e de fronteira; ataca caravanas e viajantes |
| Patrulha | Facção | Anda pelo próprio território, caça bandidos e cobra a Tarifa; o soldo sai do Banco da Facção |

**Todas as entidades existem de verdade no Mapa Mundial, o tempo todo**, inclusive as caravanas da IA. O que pesava era a simulação tile a tile, não o ícone no mapa; com o movimento de mapa em mapa, 50 a 100 caravanas custam quase nada. Toda caravana pode ser seguida, emboscada e protegida, e os preços mudam quando ela chega. (Isso substitui o "fluxo calculado por estação" que o Comércio previa para o LOD 2.)

**Dados de cada entidade:** tipo, dono, posição (mapa atual, próximo mapa, progresso), destino e rota, velocidade calculada, força (para o cálculo rápido), quantidade de pessoas, carga e os NPCs que ela representa.

### Passagem por um Mapa Local

- **Mapa em LOD 2:** a entidade só passa; o mapa registra o efeito (Tarifa cobrada, comércio feito, ataque de bandidos).
- **Mapa em LOD 0 ou 1** (com o jogador presente): a entidade **aparece de verdade** na borda, na posição proporcional, com seus NPCs (a caravana com os animais e os guardas), atravessa o mapa pela estrada e sai pela borda oposta. Enquanto isso, o progresso dela no Mapa Mundial fica parado. O jogador pode conversar, comerciar ou atacar.
- *Exemplo:* o jogador está na Cidade Religiosa (7,3) e vê chegar pela borda leste a caravana da Escravista; pode comprar dela antes que ela chegue ao Mercado.

### Névoa e exploração

**Mapa Mundial:**

- **Não explorado:** só o contorno do quadrado e o bioma aproximado.
- **Explorado** (visitado ou atravessado): terreno, cidade, nível, dono e recursos, guardados com a **data em que foram vistos** ("visto há 12 dias").
- **Visão atual:** o mapa onde o grupo está e os 4 vizinhos mostram as entidades em trânsito ao vivo; o resto mostra a última informação.
- **Conhecido desde o início:** as 6 capitais (são famosas) e os mapas vizinhos ao ponto de partida.

**Dentro dos mapas:** **sem névoa nos Mapas Locais**; **névoa só nas masmorras**, onde as salas ainda não visitadas ficam escuras.

**Conhecimento extra:**

| Fonte | O que revela |
|---|---|
| Estilo Membro de facção | Todo o território da própria facção |
| Estilo Mercador | As cidades com Posto Comercial e os preços delas no primeiro dia |
| Estilo Aventureiro | As masmorras não lendárias da região de partida |
| Mapa comprado (Mercado ou mercador viajante) | Uma região inteira, com as informações datadas do dia da compra |

### Parte técnica

- **LOD:** as entidades vivem no Mapa Mundial, que não tem LOD e fica sempre carregado. Elas só viram NPCs de verdade quando entram num Mapa Local em LOD 0 ou 1 (hidratação) e voltam a ser só dados quando saem (desidratação).
- **Atualização:** o progresso de cada entidade avança a cada hora de jogo (ou a cada tick a 8x, para a animação ficar suave). **[Proposta]** Os sorteios de encontro acontecem ao entrar num novo mapa.
- **Rota:** busca do caminho mais rápido no grafo de 48 mapas, refeita só quando o destino muda ou um mapa fica bloqueado (guerra, fronteira fechada).
- **Dados:** `mapas.json` ganha terreno, infraestrutura e estradas de cada mapa; `viagem.json` guarda as tabelas de multiplicadores e as chances de encontro, para ajustar sem mexer no código.
- **Névoa:** um registro por Mapa Local com "explorado" e a data de cada informação vista; cabe em poucos KB no save.
- **Save:** entidades em trânsito (posição, rota, carga, força), névoa e informações datadas.
- **Fatia vertical:** não usa (1 cidade, sem Mapa Mundial). Entra na camada "mundo".

### Ligação com outros sistemas

| Sistema | O que a Viagem fornece ou recebe |
|---|---|
| Mapa | Entidades em trânsito no Mapa Mundial; ponto de entrada de Viagem Rápida; terreno e infraestrutura por mapa |
| Mundo e Estado Inicial | Malha, biomas, estradas e estados de território que definem tempos e chances |
| Comércio | Caravanas sempre reais no Mapa Mundial; risco de bandidos; contratos de escolta |
| Facção | Exércitos e patrulhas; cálculo rápido de forças; soldo das patrulhas; Tarifa |
| Eventos | Ameaça bandida; encontros como eventos do grupo; "criar grupo" (bandidos, mercador viajante) |
| Combate | Lutar carrega o Mapa Local em LOD 0; render-se é a captura; montaria acelera a viagem |
| Crime e Justiça | Patrulhas prendem quem tem Recompensa |
| Reputação | Ajudar caravana em apuros dá reputação com o dono (ver [[Sistema de Reputação/Descrição\|Sistema de Reputação]]) |
| Tempo | 8x só no Mapa Mundial; volta a 1x em encontros e chegadas; acampamento à noite |
| Interface | Névoa, informações datadas, ícones das entidades, janela de encontro |
| Início de Jogo | Conhecimento inicial do mapa por estilo |
| Guerra e Notícias (futuros) | Tempo de marcha dos exércitos; atraso das informações pela distância |
| Clima e Estações (futuro) | Multiplicadores de neve e chuva |

### Perguntas em aberto

1. **Valores a testar:** tempo base, multiplicadores, chances de encontro, perda de carga, preço do suborno.
2. **Construção de estradas:** quem constrói, quanto custa e se precisa de tecnologia.
3. **Clima:** multiplicadores de neve e chuva (futuro sistema de Clima e Estações).
4. **Quantas patrulhas** cada facção mantém e quanto custam.
