> Revisão completa do projeto em 2026-10-14, feita depois que todos os sistemas planejados ganharam uma primeira descrição. O autor respondeu as perguntas da seção 10 no mesmo dia (ver "Decisões" abaixo). A lista de sistemas das seções 4 e 5 foi **aceita**; cada sistema novo será escrito em rodadas curtas de perguntas (ver [[Estado do Projeto]] e [[Descrição do Projeto]]).
>
> **Decisões (2026-10-14).** (1) Lista de sistemas P0, P1 e P2 aceita. (2) NPCs reais por nível de cidade: 15, 40, 100 e 300. (3) As 8 correções da seção 3 foram aplicadas. (4) Mapas no Tiled. (5) Mundo fixo com estado inicial variável. (6) Recursos não esgotam, têm produtividade por estação. (7) **Sem código por enquanto**: documentação, depois uma ficha técnica lógica, depois programação. (8) Documentos consolidados e [[Glossário]] criado.

## 1. Resumo

**O que está bom.** Os 27 documentos formam um conjunto coerente: um mundo em 4 escalas de mapa, simulação por LOD, economia em 3 camadas, facções com leis e cultura, crime, eventos, combate, tecnologia, magia, objetivos e comércio. O princípio "a simulação vem primeiro, o evento explica" está presente em quase tudo, e o projeto é todo orientado a dados (JSON), o que combina com a meta de desempenho. Os links entre sistemas estão consistentes e não há link quebrado.

**O que mais preocupa.** Os sistemas de "cima" (facção, cultura, tecnologia, magia, comércio) estão bem avançados, mas **a base da simulação do dia a dia está fina**: quem trabalha onde, como a população cresce, onde as pessoas moram, de onde vem o salário, como a Felicidade é calculada. É justamente isso que a fatia vertical precisa, então a base deve ser a próxima prioridade.

**Os 6 achados principais**

1. **Faltam 6 sistemas de base** que a fatia vertical já exige: Felicidade e Estabilidade, População e Habitação, Trabalho/Salários/Propriedade, Interface, Estado Inicial do Mundo e Ferramentas de conteúdo (ver seção 4).
2. **O Mapa Mundial é descrito como "contêiner leve sem dados de jogo"**, mas Comércio, Tempo e Combate já colocam caravanas, exércitos e o grupo do jogador **em trânsito** nele. Falta definir viagem e entidades em trânsito.
3. **O Fator de Escala (1 NPC = 100 pessoas) pode deixar as cidades vazias.** Um assentamento de 500 pessoas teria só 5 NPCs reais. É preciso decidir a população real por nível de cidade antes do código.
4. **A guerra só existe como "Mapa de Guerra" (a batalha).** Falta a camada de campanha (movimento de exércitos, cerco, ocupação, paz) e as regras da batalha em si.
5. **A transição de LOD (acordar um mapa que estava abstrato) não está especificada.** É o ponto técnico mais arriscado, e a fatia vertical foi pensada justamente para testá-lo.
6. **Há 8 inconsistências pequenas** entre documentos (seção 3), todas de fácil correção.

---

## 2. Estado de cada documento

| Documento | Estado | Observação |
|---|---|---|
| Mapa | Bom | Falta definir viagem e entidades em trânsito |
| Cidade | Fino | Sem condições para subir de nível; "Esquema" gerado conflita com "nada procedural" |
| NPC | Bom | Falta lista de Funções e ligação com emprego |
| Classe Social | Bom | Títulos do Objetivo não batem com as classes |
| IA | Fino | Antigo; só rotina e combate; sem Felicidade/Trabalho |
| Facção | Bom, grande | Diplomacia e guerra merecem documentos próprios |
| Cultura | Bom | Religião sem mecânica; nomes em aberto |
| Crime e Justiça | Bom | Tipos de missão de confiança em aberto |
| Cura | Curto, ok | Hospital e Medicamentos sem itens |
| Eventos | Bom | Lista inicial de eventos para escrever |
| Combate | Bom | Falta a batalha de Esquadrões |
| Tecnologia | Bom | Listas de marcos e tecnologias |
| Magia | Bom | Lista de feitiços; Grimório fora da lista de itens |
| Objetivo e Progressão | Bom | Ambições e missões sem condições exatas |
| Comércio | Bom | Guardas da caravana: NPC ou abstrato |
| Econômico | Fino | Sem salários, impostos coletados nem preço das lojas do jogador |
| Itens | Grande, bom | Itens novos só em notas; efeitos de uso não definidos |
| Inventário | Antigo | Usa herança; a Arquitetura propõe composição |
| Produção | Fino | Sem lista de Funções nem tabela de receitas |
| Construção | Médio | Hospital e vários prédios ainda "para depois"; faltam Dono e custos |
| Recursos | Muito fino | Uma frase; sem esgotamento/regeneração |
| Tempo | Fino | "8x só no Mapa Mundial" conflita com Combate |
| Início de Jogo | Bom | Custos em pontos em aberto |
| Arquitetura | Bom | Faltam navegação, transição de LOD, ferramentas |

---

## 3. Inconsistências para corrigir

| #   | Onde                            | Problema                                                                                                                              | Sugestão                                                                                          |
| --- | ------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| 1   | Tempo × Combate                 | Tempo diz que a velocidade 8x é exclusiva do Mapa Mundial; Combate diz que 1x, 2x, 4x e 8x continuam valendo                          | Valer só até 4x nos mapas locais e de guerra; 8x no Mapa Mundial                                  |
| 2   | Objetivo × Classe Social        | A escada de títulos (Plebeu, Cidadão, Cavaleiro, Nobre...) não existe como classe; "Cavaleiro" e "Cidadão" não estão em Classe Social | Tabela título × classe: título é um rótulo político e a classe só muda pelas regras de mobilidade |
| 3   | Cidade                          | "Esquema (cidade gerada pelo sistema)" contradiz "nada procedural"                                                                    | Trocar por "layout desenhado à mão, com lotes pré-definidos"                                      |
| 4   | Mapa Mundial                    | É um "contêiner sem dados", mas caravanas, exércitos e grupos viajam nele                                                             | Criar o conceito de **entidade em trânsito** (ver Viagem, seção 4)                                |
| 5   | Construção × Cura/Magia/Eventos | Hospital e Boticário ficam "para depois", mas Cura, Magia e a doença dependem deles                                                   | Promover Hospital e Boticário para a tabela de construções                                        |
| 6   | Itens × Magia                   | Grimório e Pedra de Mana (consumível) aparecem só em nota; só a Pedra de Mana Bruta está na lista                                     | Cadastrar os dois na lista de itens                                                               |
| 7   | Combate × Itens                 | Intervalo de Ataque existe nos atributos, mas nenhuma arma da lista tem valor                                                         | Preencher quando houver a primeira tabela de armas                                                |
| 8   | Inventário                      | Descreve herança de classes, enquanto a Arquitetura propõe composição                                                                 | Reescrever como "componentes de inventário"                                                       |

Também há "Atualizações recentes" espalhadas no fim de vários documentos. **[Proposta]** consolidar tudo no corpo do texto em uma passada única.

---

## 4. Sistemas que faltam

Prioridade: **P0** = necessário para a fatia vertical; **P1** = necessário para o jogo completo; **P2** = pode esperar.

### P0 — Base da simulação

**4.1 Felicidade e Estabilidade.** *(Escrito: ver [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]].)* A Felicidade é citada em 11 documentos (Cidade, Cultura, Crime, IA, Eventos...), mas nenhum define a conta. Sugestão: um documento que reúna os fatores e transforme em um número.
- Fatores: consumo atendido (por classe), distância entre leis e cultura, Criminalidade, conscrição, guerra, impostos, religião, eventos duradouros.
- Limiares: **inquietação**, **protesto** e **revolta**, ligados aos eventos condicionais já previstos (revolta, assembleia extraordinária).
- Individual só para Controlados; agregada na cidade para o resto (como a IA já prevê).

**4.2 População e Habitação.** *(Escrito: ver [[Sistema de População/Descrição|Sistema de População]].)* Antes não existia: nascimento, morte por idade, migração, quantas casas a cidade tem, crescimento. Sugestão:
- Capacidade de população = casas (Moradias) e comida.
- Nascimentos e mortes **agregados por estação**, sem simular cada NPC (o NPC real é sorteado para nascer ou morrer).
- Migração entre cidades pela diferença de Felicidade e oferta de emprego.
- **Condições para subir o nível da cidade** (1 → 2 → 3): população, construções necessárias e tecnologia.
- **População real por nível**: decidir quantos NPCs reais cada nível tem (por exemplo, 15, 40, 100 e 300), em vez de um único Fator de Escala (ver seção 7).

**4.3 Trabalho, Salários e Propriedade.** *(Escrito: ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]].)* O Econômico tem a distribuição rotativa de dinheiro, mas não diz como alguém consegue um emprego, quem paga salário, nem quem é o dono de uma construção.
- **Função ↔ Construção:** tabela mestra (Lenhador ↔ Serraria, Ferreiro ↔ Ferraria...).
- **Emprego:** a cidade atribui NPCs a vagas; desemprego aumenta a Criminalidade e reduz a Felicidade.
- **Salários:** pagos pelo dono da construção; vêm do caixa da loja ou da cidade.
- **Propriedade:** atributo **Dono** nas construções (cidade, jogador, NPC Rico, facção). É o que dá sentido ao evento "perda de empresas dos Ricos".
- **Impostos:** definir sobre o quê incidem (renda, comércio, propriedade) e como chegam ao Banco da Facção.
- **Lojas do jogador:** o jogador pode fixar o preço dentro de uma faixa em torno do Mercado Local.

**4.4 Interface e Controles.** *(Escrito: ver [[Interface/Descrição|Interface e Controles]].)* Nenhum documento trata da interface, e um jogo de gerenciamento com tantos sistemas vive disso. Sugestão de um documento curto:
- Telas: HUD do mapa local, mapa mundial, painel da cidade, inventário, produção, pesquisa, facção, diário de eventos.
- Controles: câmera, seleção, ordens (já definidas no Combate), pausa ativa.
- Princípio: **informação em camadas**, com resumos e detalhes sob demanda; tooltips com o "por quê" (já previsto nos Eventos).
- Comece com wireframes simples antes de qualquer arte.

**4.5 Estado Inicial do Mundo e Lore.** *(Escrito: ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]; a história fica para depois.)* Há 6 regiões, 6 culturas e 6 facções, mas nenhum nome, mapa nem história. Sugestão:
- Documento de **Mundo e História** (nomes das culturas e regiões, relações iniciais entre facções, guerras em andamento, NPCs Importantes e líderes).
- Tabela do **estado inicial**: cidades, população, dinheiro, tecnologia, leis e relações de cada facção.
- **Ano de início** ("história do mundo") e primeiros NPCs Importantes.

**4.6 Pipeline de arte e ferramentas.** *(Escrito: ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]].)* Toda a arte é à mão, inclusive 36 a 48 mapas de 128 × 128 tiles e todos os Mapas Internos. Isso exige ferramentas.
- **Editor de mapas:** sugestão de usar o **Tiled** (gratuito; exporta JSON) para desenhar mapas com camadas, lotes e pontos de entrada, em vez de escrever um editor próprio agora.
- **Orçamento de arte:** estimar o volume (tiles por estação, construções × níveis × 6 culturas, bonecos em camadas, animações por tipo de arma) e escolher o que entra na primeira versão.
- Pasta `data/` com esquemas e validação (já prevista).

### P1 — Jogo completo

**4.7 Viagem e Mundo.** Resolve a inconsistência #4. Define:
- Entidades em trânsito no Mapa Mundial (grupo do jogador, caravanas, exércitos, bandos): posição, velocidade e rota.
- Tempo de viagem entre Mapas Locais, viagem rápida, encontros no caminho (viram evento do "Grupo do jogador", com Criaturas ou bandidos).
- **Névoa de exploração** e mapa descoberto.
- O que acontece quando uma entidade passa por um mapa em LOD 2.

**4.8 Guerra e Campanha.** Hoje só existe o Mapa de Guerra (a batalha). Falta:
- Declaração de guerra, objetivos, **cerco e ocupação** de cidades, saque, cansaço de guerra e **tratado de paz**.
- Movimento de exércitos no Mapa Mundial (usa a Viagem), linhas de suprimento e Caravana Militar.
- **Defesa de cidade** (Muralha, Posto da Guarda).
- A fórmula do **cálculo rápido de forças**, que deve ser coerente com a batalha real.

**4.9 Batalha (Mapa de Guerra).** Regras do combate de Esquadrões, inspiradas em Total War: formação, Moral, terreno, flanco, artilharia (Canhão de Guerra), Esquadrão de Magos, condições de vitória e derrota. O Mapa de Guerra tem atributos, mas nenhuma regra.

**4.10 Diplomacia.** Hoje vive dentro de Facção. Merece documento próprio:
- Estados diplomáticos, tratados (comércio, aliança, vassalagem, passagem), motivos de guerra (casus belli), casamentos entre dinastias, reféns.
- Como a IA decide (Personalidade do líder, Agressividade e Ganância).

**4.11 Reputação e Relações do jogador.** Existe a Recompensa (má fama) e o Renome (feitos), mas não a **relação do jogador com cada facção e cidade**, que o Início de Jogo já usa ("relação inicial com as facções"). Sugestão: valor de −100 a +100 por facção, que afeta preços, acesso a contratos, Rito de Admissão e ao tratamento pelos guardas (como em Mount & Blade).

**4.12 Companheiros, Relações e Diálogo.**
- **Companheiros:** recrutamento (pergunta em aberto do Objetivo), salário, lealdade, saída por Felicidade 0, limite do grupo.
- **Relações familiares:** casamento, filhos e herdeiros (os atributos Grávida e Relação existem, sem regras).
- **Diálogo:** um sistema de opções por menu (persuadir, barganhar, intimidar, subornar, aceitar missão), usando Carisma. Sugestão: **diálogos modulares por modelos de texto**, não árvores escritas à mão, para o volume de texto ficar viável para uma pessoa só.

**4.13 Religião.** Está dentro da Cultura, mas só existe o nome. Sugestão: Templo gera **Fé**; clérigo como Função; festivais (eventos); heresia como crime cultural; conversão; efeito na Felicidade. Religião também define o conteúdo da cultura Religiosa.

**4.14 Clima e Estações.** Há 4 estações e eventos climáticos, mas as estações não afetam nada de forma regular. Sugestão: modificadores por estação (colheita no outono, produção agrícola menor no inverno, viagem mais lenta com neve) e clima do dia como evento duradouro. **Atenção à arte:** 4 estações multiplicam os tiles; começar com variação apenas de cor.

**4.15 Notícias e Informação.** O Comércio já fala de "notícias" e de preços que envelhecem. Sugestão: rumores que se espalham pelas tavernas e rotas (guerras, recompensas, preços, contratos), com atraso por distância. Une Comércio, Objetivo, Crime e Eventos, e dá vida à Crônica do mundo.

**4.16 Efeitos de itens e Escravidão.**
- **Efeitos de itens:** o que fazem Livro, Livro de Atributos, Joia Mágica, Artefato e Medicamento ao serem usados.
- **Escravidão e tráfico:** compra e venda de escravos (Mercado de Escravos, preço, captura de prisioneiros, Grilhões), hoje espalhada entre NPC, Crime e Cultura.

### P2 — Depois

| Sistema | Observação |
|---|---|
| Áudio e música | Música por cultura e ambiente; a biblioteca (miniaudio) já está proposta |
| Tutorial e onboarding | Importante em um jogo com tantos sistemas; depois da interface |
| Configurações, acessibilidade e controles | Resolução, teclas, legendas, daltonismo |
| Localização | Usar IDs de texto desde o início, mesmo que só haja português |
| Modding | Os dados em JSON já preparam o caminho; decidir se é objetivo |
| Conquistas e estatísticas | Opcional |

---

## 5. Abstrato que pode virar sistema

| Abstrato hoje | Onde aparece | Como virar sistema |
|---|---|---|
| **Felicidade** | 11 documentos | 4.1 Felicidade e Estabilidade |
| **"Eventos políticos"** | Classe Social, IA | Parte de 4.1 (limiares de revolta) |
| **Fator de Escala** | Mapa, Cidade, Facção | 4.2 (população real por nível) |
| **Funções dos NPCs** | NPC, Produção | 4.3 (tabela Função ↔ Construção ↔ Receita) |
| **"Empresas dos Ricos"** | Classe Social | 4.3 (Dono nas construções) |
| **Relação / Importante** | NPC | 4.5 (NPCs Importantes) e 4.12 |
| **"Notícias"** | Comércio | 4.15 |
| **"Contratos" e "missões"** | Objetivo | Lista inicial em dados (já prevista) |
| **Religião** | Cultura | 4.13 |
| **Estado Guerra** | Facção | 4.8 e 4.9 |
| **Relação inicial com facções** | Início de Jogo | 4.11 |
| **"Armadura mista", "Escudo"** | Itens, Combate | Só decisão de regra, já proposta |
| **Artefatos** | Classe Social, Objetivo | 4.16 (efeitos de itens) |
| **Esgotamento de recursos** | Recursos | Decidir: recursos esgotam ou regeneram (floresta, mina)? |

---

## 6. Em aberto, por tipo

**Dependem de teste (não decidir agora).** Fator de Escala, tamanho do esquadrão, gravidade e multas, fórmula da Criminalidade, limiar de mapa em disputa, taxa de regeneração, chances dos eventos, dano e acerto do combate, valores da tecnologia e da magia, capacidade e Tarifa do comércio.
**Dica:** como a simulação é separada do desenho, rodar a simulação **sem gráfico (modo headless)** por muitos anos de jogo e gravar números (dinheiro total, população, Felicidade, eventos por ano). Dá para ajustar a maioria desses valores antes de qualquer arte.

**Dependem de você (decisão criativa).**
- Nomes das culturas e regiões; história do mundo.
- Foco e nomes dos narradores; categorias extras de eventos.
- Condições das Ambições, Renome por feito, tipos de missão de confiança.
- Guardas da caravana: NPC ou valor abstrato; animais de carga.
- Hospital e Medicamentos; quando o Inconsciente vira Morto.
- Custos em pontos do início personalizado; bônus do Mercador.
- Recursos esgotam ou regeneram.

**Dependem de produção de conteúdo.**
- Lista de itens a cadastrar (Tábua, Grilhões, Sela, munições, Canhões, Anel Mágico, Armadura Média, Grimório, Pedra de Mana).
- Novas construções nos canvases: Mercado, Taverna, Quartel, Prisão, Casa da Moeda, Templo, Posto Comercial, Laboratório, Torre Arcana, Forja Arcana, Encantaria, Hospital, Boticário.
- Lista inicial de eventos (10 a 15 para a fatia vertical), marcos e tecnologias, feitiços, contratos, missões e masmorras.

**Dependem de decisão técnica.** PC alvo mínimo; aprovação das bibliotecas; tamanho do tile, perspectiva e mundo; navegação (pathfinding); transição de LOD.

---

## 7. Riscos do projeto e como reduzi-los

| Risco | Por quê | Sugestão |
|---|---|---|
| **Escopo** | Mais de 20 sistemas para uma pessoa com pouca experiência em C++ | Construir em **camadas jogáveis**: fatia vertical → cidade completa → mundo → política e guerra. Cada camada deve ser divertida sozinha |
| **Volume de arte** | Tudo à mão: 36 a 48 mapas, construções × níveis × 6 culturas, bonecos e animações | Orçamento de arte (4.6); começar com **1 cultura completa** e reaproveitar peças |
| **População real baixa** | Com Fator 100, uma cidade média tem poucas dezenas de NPCs reais | Definir NPCs reais por nível de cidade (4.2). Cidades que parecem vazias são pior que um Fator "mais honesto" |
| **Transição de LOD** | Acordar um mapa abstrato exige posicionar NPCs, itens e produção coerentes com a hora do dia | Especificar "hidratação" e "desidratação" no Arquitetura e testar já na fatia vertical |
| **Equilíbrio da economia** | Três camadas de preço, dinheiro que circula entre cidades, impostos e salários | Simulação headless (seção 6) e métricas desde o início |
| **Pathfinding** | NPCs com posição livre sobre grade, em mapas de 128 × 128, com 40 combatentes | Escolher cedo: A* com movimento suave, sem colisão entre NPCs |
| **Docs desatualizados** | Várias atualizações em notas | Consolidar e manter um **Glossário** (Rank, Tier, LOD, Esquadrão...) |

---

## 8. Ideias para acrescentar

Opcionais, em ordem de custo-benefício:

1. **Mundo fixo, estado inicial variável.** O mapa é desenhado à mão, mas relações iniciais, tecnologia inicial, localização do Loot e a "semente" do Narrador variam. Dá rejogabilidade sem arte nova.
2. **Crônica do mundo visível.** Um "jornal" na Taverna com os eventos recentes do mundo (Notícias). Mostra a simulação funcionando.
3. **Especialização das cidades** (como Songs of Syx): cada cidade fica conhecida por um produto, o que dá motivo para o comércio.
4. **Ordens de produção do jogador** em lote (fila por construção) e prioridades por Função, no estilo RimWorld.
5. **Guarnição e defesa de cidade:** a cidade do jogador pode ter Guardas pagos, uma Muralha e uma milícia (conscritos da cidade) em caso de ataque.
6. **Modo observador:** assistir à simulação do mundo sem jogador (também serve para teste).
7. **Eventos de dinastia:** casamentos, herdeiros, intrigas, ligados à Facção, que dão personalidade às 6 facções.
8. **Seleção de tema por cultura** (cores, ícones e fonte da interface), reaproveitando a arte de cada cultura.

---

## 9. Ordem sugerida

1. **Documentação P0** (em rodadas curtas de perguntas, como estamos fazendo): Felicidade, População e Habitação, Trabalho/Salários/Propriedade, Interface, Estado Inicial e Mundo, Pipeline e ferramentas. Em paralelo, corrigir as 8 inconsistências.
2. **Código da fatia vertical**, marcos 1 a 3 da Arquitetura (janela, câmera, carregar dados), enquanto a documentação P0 termina. Aprovar as bibliotecas e criar o repositório do código.
3. **Fatia vertical completa** (marcos 4 a 8), mais o **modo headless** para testar a economia.
4. **P1 em ordem de dependência:** Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias.
5. **P2** perto do lançamento.

---

## 10. Perguntas para você

1. **Sistemas novos.** Aceita a lista da seção 4 e as prioridades P0, P1 e P2? Quer acrescentar ou tirar algum?
2. **População real por nível de cidade** em vez de um único Fator de Escala? Sugestão: manter o Fator para a população total e definir o NPC real por nível (15, 40, 100 e 300).
3. **Correções da seção 3.** Aplico as 8 sugestões?
4. **Tiled para os mapas.** Usar o Tiled (gratuito) em vez de escrever um editor próprio?
5. **Mundo fixo com estado inicial variável** (ideia 1) está de acordo com a ideia de arte à mão?
6. **Recursos esgotam?** Sugestão: não esgotam, mas têm **produtividade por estação** (a floresta se recupera, a mina rende menos no nível baixo e mais com tecnologia), para evitar um sistema de esgotamento.
7. **Ordem.** Quer fazer a documentação P0 antes do código ou em paralelo?
8. **Faxina nos documentos.** Posso consolidar as "Atualizações recentes" no corpo dos textos e criar um **Glossário**?
