## Sistema de Mapa

**Descrição geral**

Toda a parte gráfica será feita à mão — nada será procedural ou aleatório. O mapa é separado em 4 tipos: 3 mapas de escala (Mundial, Local e Interno) e 1 mapa de situação (Guerra), cada um com sua própria função e importância.

O Nível 4 (Guerra) serve a uma funcionalidade específica do jogo — situações de combate massivo, inspiradas na franquia _Total War_.

### Nível 1 — Mapa Mundial (RTS)

O Mapa Mundial consiste em quadrados conectados entre si, formando uma malha quadriculada. Cada quadrado dessa malha representa um Mapa Local (Nível 2). Ele não guarda dados de jogo das cidades nem dos NPCs — funciona como um contêiner leve (grafo + gráfico geral), o que o torna barato de manter sempre carregado, sem necessidade de LOD. A única exceção é a lista de **entidades em trânsito** (grupo do jogador, caravanas, exércitos e bandos em viagem entre Mapas Locais); as regras de viagem ficam para o sistema de **Viagem e Mundo** **[Em aberto]**.

O Mapa Mundial serve para fornecer informações globais sobre o estado do mundo: localização de cada cidade, domínio de cada facção sobre os Mapas Locais, e — junto com outros sistemas do jogo — gerenciamento em escala global (relevante para quando o jogador tiver sua própria facção). Também serve para viagens de longa distância, sem que o jogador precise ir manualmente até a borda de cada Mapa Local.

Inclui um **índice de busca** por Mapa Local (nome, facção, tipo — ex: "mostrar cidades da Facção X") e um **ponto de entrada de Viagem Rápida**, que define em qual Mapa Local (e em que posição) o grupo reaparece ao viajar diretamente pelo Mapa Mundial em vez de andar até a borda.

Essa é a função pensada até agora para o Mapa Mundial. Novas funções ainda serão adicionadas, e algumas das atuais podem ser removidas, se necessário.

**População no Mapa Mundial.** A população mostrada é calculada com o **Fator de Escala** (global): 1 NPC real representa N pessoas. Ver Sistema de Facção.

**Tamanho inicial (provisório).** O mundo é dividido em 6 regiões (uma por cultura inicial), com 6 a 8 Mapas Locais cada, de 36 a 48 mapas no total na primeira versão. Valores a ajustar depois de testes.

### Nível 2 — Mapa Local (RPG)

O Mapa Local é um dos quadrados dentro da malha do Mapa Mundial. É onde boa parte do jogo ocorre: acesso direto aos NPCs, interações, conversas, lutas, comércio, produção — praticamente tudo que interage com o mundo acontece nesse mapa.

O terreno (chão, água, paredes, recursos pontuais) é construído em uma **grade de tiles**, usada para colisão e renderização. NPCs e itens têm posição livre (contínua) por cima dessa grade. Cada Mapa Local também guarda uma **lista de Mapas Internos** (as construções presentes nele) e um **Nível de Detalhe de Simulação (LOD)**.

**Tamanho inicial (provisório).** 128 × 128 tiles por Mapa Local, de 16 px cada, a ajustar após testes de desempenho.

Existem diferentes métodos para acessar outros Mapas Locais a partir do atual:

- **Método simples** — ir até a borda do mapa para entrar no Mapa Local adjacente. A transição usa **alinhamento proporcional**: a posição de saída (0–100% ao longo da borda) é convertida para a mesma porcentagem na borda de entrada do Mapa Local vizinho, mesmo que os dois tenham tamanhos diferentes.
- **Viagem pelo Mapa Mundial** — usado com prioridade quando o destino é um Mapa Local distante, sem conexão adjacente ao mapa onde o jogador reside atualmente.
- Outros métodos ainda estão sendo pensados.

**Sistema de LOD por Mapa Local:** LOD 2 (Abstrato) quando não há NPC Controlado presente — necessidades e eventos avançam em blocos de tempo, sem simulação tile a tile; LOD 1 (Reduzido) quando há NPC Controlado mas o mapa está fora da visão da câmera — simulação roda, mas sem pathfinding fino nem combate detalhado; LOD 0 (Completo) quando o mapa está na visão atual — simulação completa, tick a tick. Todo NPC dentro do Mapa Local herda esse LOD — não é calculado por NPC individualmente, o que barateia bastante o processamento. Mais de um Mapa Local pode estar ativo ao mesmo tempo, cada um no seu próprio nível.

Dentro de cada Mapa Local existem os Mapas Internos (Nível 3), correspondentes às construções presentes nele.

#### Cidade e território

Cada Mapa Local comporta **apenas uma cidade**, mas nem todo Mapa Local tem uma. O mapa tem um de quatro **estados de território**:

- **Sem dono** — sem cidade e sem cidade vizinha. Existe para o jogador explorar e, se quiser, fundar um assentamento.
- **Território de facção** — sem cidade própria, mas **adjacente** (os 4 vizinhos na malha) a uma cidade de uma facção. É vinculado à facção dessa cidade, e pode ser tratado como vila ou entreposto em desenvolvimento.
- **Em disputa** — adjacente a cidades de **facções diferentes**. Causa guerras constantes até uma facção dominá-lo ou o jogador interferir.
- **Com cidade** — tem uma cidade. A facção do Mapa Local é a da cidade.

O estado e a facção de um Mapa Local são **derivados** das cidades vizinhas, não armazenados: se uma cidade muda de facção por conquista, os mapas adjacentes seguem. Detalhes em Sistema de Facção.

O jogador pode fundar um assentamento (cidade nível 1) em um mapa sem dono ou em território de facção. No segundo caso, o assentamento nasce vinculado à facção dominante.

### Nível 3 — Mapa Interno (RPG)

O Mapa Interno é o interior das construções que residem no Mapa Local, referenciando sempre o Mapa Local pai ao qual pertence. Existem 3 tipos:

- **Moradia** — tem um Dono definido; alvo de furto.
- **Masmorra** — sem dono; contém relíquias, itens e possivelmente monstros.
- **Construção Pública** — tavernas, mercados, ferrarias, prisões; necessária como destino físico de trabalho dentro da Rotina dos NPCs. A **Prisão** é uma Construção Pública onde NPCs presos cumprem pena (ver Sistema de Crime e Justiça).

Qualquer construção de produção pode funcionar como **loja ilegal** disfarçada (ver Sistema de Crime e Justiça).

Cada Mapa Interno tem sua própria grade de tiles (em escala menor) e um **Nível de Segurança**, que afeta a chance de detecção em furtos e a dificuldade de masmorras. A entrada/saída usa um **ponto fixo** (a porta), diferente da transição por borda proporcional usada entre Mapas Locais — ao sair, o NPC reaparece no "Endereço da volta" no Mapa Local pai.

Por padrão, todo Mapa Interno começa em **LOD 2 (Abstrato)**, mesmo que o Mapa Local pai esteja em LOD 0 — só sobe de nível quando alguém entra de fato, evitando simular o interior de toda casa de toda cidade o tempo todo.

### Nível 4 — Mapa de Guerra (RTS)

Mapa diferente dos demais: só existe em situações de combate entre duas facções ou exércitos. Seu acesso acontece normalmente pelo Mapa Mundial. Como o sistema de guerra é um sistema separado do sistema de combate principal do jogo, essa separação evita que os dois tipos de combate entrem em conflito entre si.

O Mapa de Guerra herda o terreno/bioma do Mapa Local onde o confronto foi disparado, para a batalha fazer sentido geograficamente.

**Exércitos e Esquadrões.** O jogador não controla soldados individuais, mas **Esquadrões** (por exemplo, 2000 soldados = 20 esquadrões). Soldados não são NPCs. As **Caravanas Militares** acompanham o exército e carregam suprimentos. Ver Sistema de Facção.

**Quando é gerado.** O Mapa de Guerra só é criado quando **o jogador participa**. Guerras entre facções de IA (por exemplo, por um mapa em disputa) são resolvidas **automaticamente**, por um cálculo rápido que compara a força dos dois exércitos.

O Mapa de Guerra guarda a lista de Facções/Exércitos participantes (com seus Esquadrões, Formação e Moral — ver Sistema de Facção), uma condição de vitória/derrota (ex: moral do exército inimigo chega a 0) e o resultado pós-batalha (mudança de Facção dominante do Mapa Local, baixas nos Exércitos, possível saque, prisioneiros). Por ser temporário, o Mapa de Guerra é gerado e destruído sob demanda — não fica "adormecido" como os outros mapas.

### Atributos

**Mapa Mundial (Nível 1)**

- Endereço de um grafo que carrega a malha de Mapas Locais
- Endereço do gráfico do Mapa Mundial
- Lista de entidades em trânsito (ver Viagem e Mundo, a escrever)

**Mapa Local (Nível 2)**

- Endereço da parte gráfica
- Endereço dele no grafo mundial
- Endereço das 4 outras regiões adjacentes (obtido pelo grafo)
- Lista de recursos
- Estado de território (Sem dono / Território de facção / Em disputa / Com cidade) — derivado
- Facção pertencente — derivada da cidade do mapa ou da cidade adjacente
- Cidade (no máximo uma)
- Cultura da região
- População total da região (NPCs reais × Fator de Escala)
- Tamanho do mapa (valor x, y)
- Nome do mapa
- Booleana indicando se essa região possui uma cidade
- Grade de Tiles
- Lista de Mapas Internos
- Nível de Detalhe de Simulação (LOD)

**Mapa Interno (Nível 3)**

- Endereço da parte gráfica
- Endereço da volta (posição para onde os NPCs devem reaparecer ao sair desse mapa)
- Mapa Local pai
- Tipo (Moradia / Masmorra / Construção Pública)
- Grade de Tiles própria
- Dono (se Moradia)
- Nível de Segurança

**Mapa de Guerra (Nível 4)**

- Endereço da parte gráfica
- Mapa Local de origem
- Facções/Exércitos participantes (Esquadrões e Caravanas)
- Condição de vitória/derrota
- Resultado pós-batalha

---

### Integração com outros sistemas

- **Eventos:** o sorteio e a avaliação de eventos respeitam o LOD de cada Mapa Local. Em LOD 2 os efeitos são agregados. Bandidos ocupam mapas Sem dono ou de fronteira (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]).
- **Combate local:** é para grupos pequenos e médios (cerca de 40 combatentes ativos, a testar). Acima disso, ou com Esquadrões, o confronto vai para o Mapa de Guerra (ver [[Sistema de Combate/Descrição|Sistema de Combate]]).
- **População:** a migração entre cidades, em LOD 0, aparece como NPCs que chegam pela borda do mapa; em LOD 2 é um fluxo calculado por estação (ver [[Sistema de População/Descrição|Sistema de População]]).
