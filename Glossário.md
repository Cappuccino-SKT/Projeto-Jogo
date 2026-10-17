> Definições curtas dos termos que se repetem nos documentos. A definição completa fica no sistema indicado. Se um termo mudar, atualize aqui também.

## Marcações dos documentos

| Termo | Significado |
|---|---|
| (sem marca) | Decisão do autor |
| **[Proposta]** | Sugestão ainda não decidida |
| **[Em aberto]** | Pergunta que precisa de decisão |
| **[Refinar depois]** | Ideia aceita em linhas gerais, com detalhes a definir mais adiante |

## Mundo e tempo

| Termo | Significado |
|---|---|
| **Mapa Mundial** | Nível 1. Malha de quadrados, cada um um Mapa Local. Contêiner leve; 8x de velocidade só aqui. [[Sistema de Mapa/Descrição\|Mapa]] |
| **Mapa Local** | Nível 2. Mapa em grade de tiles onde o jogo acontece; tem no máximo uma cidade |
| **Mapa Interno** | Nível 3. Interior de uma construção: Moradia, Masmorra ou Construção Pública |
| **Mapa de Guerra** | Nível 4. Batalha de Esquadrões, só criada quando o jogador participa; cópia do Mapa Local; o mundo para durante ela. [[Sistema de Batalha/Descrição\|Batalha]] |
| **LOD** | Nível de detalhe da simulação por Mapa Local: 0 completo, 1 reduzido, 2 abstrato |
| **Fator de Escala** | Quantas pessoas um NPC real representa (exemplo 100). Valor a testar |
| **Entidade em trânsito** | Grupo, caravana, exército ou bando viajando entre Mapas Locais |
| **Dia, Estação, Ano** | 24 h de jogo = 16 min reais a 1x; estação = 16 dias; ano = 4 estações = 64 dias. Não existem meses. [[Sistema de Tempo/Descrição\|Tempo]] |
| **Pausa ativa** | Pausar o jogo para dar ordens, que são executadas ao despausar |

## Pessoas

| Termo | Significado |
|---|---|
| **Controlado** | NPC do grupo do jogador, comandado diretamente. [[Sistema de NPC/Descrição\|NPC]] |
| **Semi-Controlado** | NPC do jogador que recebe ordens pré-definidas (por exemplo, o Escravo do jogador) |
| **Não-Controlado** | A maioria dos NPCs; agido pela IA |
| **Importante** | NPC com importância para o mundo; nunca é alistado |
| **Estado Atual** | Vivo, Ferido, Inconsciente ou Morto, derivado da Vida |
| **Fome** | 0 a 100, só para Controlados e Importantes; cai 100 em 36 horas; a comida devolve pela Saciedade |
| **Classe Social** | Realeza, Nobreza, Rico, Civil ou Escravo. [[Sistema de Classe Social/Descrição\|Classe Social]] |
| **Esquadrão** | Unidade de soldados; entidade própria, **não** um NPC. Sem classe social |
| **Criatura** | Animal ou monstro; entidade leve, **não** um NPC. [[Sistema de Combate/Descrição\|Combate]] |
| **Mago** | NPC que nasce com a habilidade mágica (booleana). [[Sistema de Magia/Descrição\|Magia]] |
| **Postura** | Comportamento do Controlado sem ordem: Agressiva, Defensiva, Manter posição ou Evitar combate |
| **Capacidade de NPCs** | Máximo de NPCs reais de uma cidade, definido pelo nível (15, 40, 100 e 300). [[Sistema de População/Descrição\|População]] |
| **Ocupação** | Número atual de NPCs reais de uma cidade em relação à capacidade |
| **Lote** | Terreno fixo de construção marcado no mapa, em 3 tamanhos |

## Política e cultura

| Termo | Significado |
|---|---|
| **Facção** | Entidade política com líder, leis e cidades. 6 principais. [[Sistema de Facção/Descrição\|Facção]] |
| **Cultura** | Conjunto de traços, ideologia e leis desejadas. 6 iniciais. [[Sistema de Cultura/Descrição\|Cultura]] |
| **Lei** | Regra da facção herdada por todas as cidades dela |
| **Assembleia** | Votação anual (ou extraordinária) para mudar leis |
| **Dinastia** | Família do líder; sucessão e herdeiros |
| **Rito de Admissão** | Forma de entrar na facção, definida pela cultura |
| **Território** | Estado de um Mapa Local: Sem dono, Território de facção, Em disputa ou Com cidade |
| **PIB** | Soma do dinheiro das cidades; só para comparar facções |
| **Banco da Facção** | Caixa da facção, alimentado por impostos |
| **Banco da Cidade** | Dinheiro próprio da cidade, distribuído a NPCs e lojas |
| **Felicidade** | Satisfação, de 0 a 100, guardada por classe social em cada cidade. [[Sistema de Felicidade e Estabilidade/Descrição\|Felicidade e Estabilidade]] |
| **Controle** | Força de contenção da cidade: guardas, Esquadrões estacionados e Medo |
| **Estabilidade** | Felicidade ponderada + Controle − 10; abaixo de zero por alguns dias, ocorre a revolta |
| **Criminalidade** | Valor agregado de crime da cidade. [[Sistema de Crime e Justiça/Descrição\|Crime e Justiça]] |
| **Recompensa** | Valor pago por capturar um criminoso; é por facção e decai com o tempo |
| **Renome** | Fama do personagem por feitos, de 0 a 100. [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]] |
| **Ambição** | Objetivo de longo prazo opcional do jogador |

## Itens e economia

| Termo | Significado |
|---|---|
| **Rank** | Nível de armas, equipamentos e artefatos, de 0 a 5. [[Sistema de Itens/Descrição\|Itens]] |
| **Tier** | Nível de materiais e bens de consumo, de 0 a 4 |
| **Mercado Mundial, da Facção, Local** | Três camadas de preço. [[Sistema Econômico/Descrição\|Econômico]] |
| **Caravana Militar** | Carrega suprimento do exército; uso exclusivo militar |
| **Caravana Civil** | Faz o comércio entre cidades. [[Sistema de Comércio/Descrição\|Comércio]] |
| **Tarifa** | Imposto sobre caravanas civis que entram no território da facção |
| **Ração Militar** | Comida do exército |
| **Posto Comercial** | Construção necessária para enviar caravanas civis |
| **Vaga** | Posto de trabalho de uma construção, por nível. [[Sistema de Trabalho e Propriedade/Descrição\|Trabalho e Propriedade]] |
| **Dono** | Quem possui uma construção: Cidade, Facção, Rico ou jogador |
| **Empresa** | Construção de produção ou comércio pertencente a um Rico |
| **Animal (item)** | Tipo de Matéria-Prima: Galinha, Porco, Ovelha, Vaca e Cavalo; o Pasto cria, o Rancho usa, o Açougue gasta |
| **Pesqueiro** | Construção Braçal que extrai Peixe nos mapas com rio, lago ou litoral |
| **Pilha** | Item + dono + quantidade; o jeito como o inventário guarda materiais e bens. [[Sistema de Inventário/Descrição\|Inventário]] |
| **Riqueza média** | Dinheiro médio por classe em uma cidade; substitui o dinheiro individual de cada NPC |
| **Relíquia / Artefato lendário** | Artefato genérico (Rank 1 a 3) / único, com efeito próprio (Rank 4 e 5). [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]] |
| **Livro Sagrado** | Livro exigido nas revistas das facções com a lei "Livro Sagrado obrigatório" (Religiosa) |
| **Doente / Viciado** | Estado e traço que só existem em Controlados e Importantes |
| **Obediência** | Chance de o escravo cumprir a prioridade, de 0 a 100. [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]] |
| **Mercado de Escravos** | Construção onde se compram e vendem escravos, só onde a lei permite |
| **Alforria / Abolição** | Escravo que compra a liberdade / lei que liberta todos os escravos da facção |

## Eventos, tecnologia e magia

| Termo | Significado |
|---|---|
| **Evento Ocasional** | Disparado por sorteio, com chance ajustada pelo estado do mundo. [[Sistema de Eventos/Descrição\|Eventos]] |
| **Evento Condicional** | Disparado quando uma condição do mundo é atendida |
| **Pontual / Duradouro** | Evento que acontece e acaba / evento que fica ativo por um tempo |
| **Escopo** | Onde o evento acontece: Mundo, Facção, Cidade/Mapa Local ou Grupo do jogador |
| **Narrador** | Regulador de ritmo dos eventos ocasionais; escolhido no início e funciona como dificuldade |
| **Marco mundial** | Tecnologia que chega para o mundo todo numa janela de tempo. [[Sistema de Tecnologia/Descrição\|Tecnologia]] |
| **Pontos de Pesquisa / Pontos Arcanos** | Gerados no Laboratório / na Torre Arcana |
| **Encantamento** | Bônus fixo em item de Rank 3 ou maior, feito na Encantaria |

## Combate

| Termo | Significado |
|---|---|
| **Em combate** | Ter um hostil dentro do raio de alerta |
| **Intervalo de Ataque** | Tempo entre ataques de uma arma |
| **Modo letal / não letal** | Se Vida 0 mata ou deixa Inconsciente |
| **Duelo** | Combate entre dois, sem interferência |

## Guerra e diplomacia

| Termo | Significado |
|---|---|
| **Exército** | 1 a 20 Esquadrões, Caravanas Militares e 1 General. [[Sistema de Guerra/Descrição\|Guerra]] |
| **General** | NPC Importante que comanda um exército; o Comando (Carisma e Inteligência) dá Moral |
| **Motivo de guerra** | Razão para declarar guerra; sem motivo, a guerra custa relação e Confiabilidade |
| **Placar de Guerra** | Pontos de −100 a +100 que medem quem está ganhando; pagam os termos da paz |
| **Cansaço de guerra** | 0 a 100 por facção; tira Felicidade e empurra para a paz |
| **Ocupada** | Cidade tomada que só muda de facção no tratado de paz (ou depois de 2 anos) |
| **Guarnição** | Esquadrões parados numa cidade; somam ao Controle e defendem no cerco |
| **Milícia** | Guardas e civis armados só para defender a própria cidade |
| **Cálculo rápido** | Resolução de uma batalha sem o Mapa de Guerra, com as mesmas grandezas |
| **Kit** | Itens que equipam um tipo de Esquadrão; o Rank define a qualidade. [[Sistema de Batalha/Descrição\|Batalha]] |
| **Moral** | 0 a 100 por Esquadrão; abaixo de 25 ele foge |
| **Comitiva** | Guarda do General na batalha; com o jogador, é o próprio grupo controlado |
| **Tamanho das unidades** | Opção do jogador: 20, 50 ou 100 bonecos por Esquadrão; não muda a simulação |
| **Trégua** | 1 ano sem guerra entre dois lados depois de um tratado de paz. [[Sistema de Diplomacia/Descrição\|Diplomacia]] |
| **Atitude** | Faixa da relação entre facções: Hostil, Fria, Neutra, Amigável ou Leal |
| **Tratado** | Acordo entre facções; vários ao mesmo tempo (Comercial, Passagem, Não Agressão, Alianças, Vassalagem, Casamento) |
| **Confiabilidade** | Nota de 0 a 100 de cada facção; cai ao quebrar tratados |
| **Vassalo** | Subordinado com leis próprias, que paga tributo e atende ao chamado às armas |
| **Coalizão** | Aliança defensiva conjunta contra uma facção ameaçadora |

## Religião, clima e notícias

| Termo | Significado |
|---|---|
| **Devoção** | Quanto a fé de uma cultura pesa: Alta, Média, Baixa ou Nenhuma. [[Sistema de Religião/Descrição\|Religião e Fé]] |
| **Cobertura do Templo** | Parte da cidade atendida pelos Sacerdotes (cada um atende 20 NPCs reais) |
| **Festival** | Data fixa do calendário de cada cultura (2 por ano; 4 na Religiosa) |
| **Tolerância religiosa** | Lei de facção: Tolerância, Fé oficial ou Perseguição das minorias |
| **Heresia** | Crime cultural: um ato contra a fé oficial, não a simples existência de uma minoria |
| **Assimilação** | Minorias adotando aos poucos a cultura dominante da cidade, por estação |
| **Bênção / Anátema** | Atos do Sumo Sacerdote que mudam relações; o Anátema libera a Guerra santa |
| **Clima do dia** | Limpo, Nublado, Chuva, Tempestade, Neve, Névoa ou Calor forte; sorteado por região. [[Sistema de Clima e Estações/Descrição\|Clima e Estações]] |
| **Lenha** | Madeira Bruta gasta pelas moradias no inverno |
| **Fase do dia** | Amanhecer, dia, entardecer ou noite; à noite, visão e detecção −30% |
| **Notícia** | Acontecimento que anda 3 h por mapa até o jogador. [[Sistema de Notícias/Descrição\|Notícias]] |
| **Importância** | Alcance da notícia: Local (2 mapas), Regional (5) ou Mundial |
| **Rede do jogador** | Cidades, companheiros fora do grupo e caravanas do jogador, que recebem notícias por ele |
| **Crônica do mundo** | Notícias Regionais e Mundiais que o jogador soube, com as duas datas |

## Interface

| Termo | Significado |
|---|---|
| **HUD** | Elementos fixos na tela durante o jogo (barra superior, retratos, minimapa...). Personalizável pelo jogador. [[Interface/Descrição\|Interface]] |
| **Modo de edição da interface** | Modo em que o jogador move, redimensiona e esconde os elementos do HUD |
| **Lente de mapa** | Modo de visualização do Mapa Mundial (Política, Cultura, Felicidade, Comércio, Perigo, Tecnologia) |
| **Barra de alertas** | Faixa com os problemas mais urgentes da cidade do jogador |
| **Por quê?** | Detalhamento dos fatores por trás de um número ou evento |
| **Perfil do jogador** | Arquivo com layout do HUD, escala, atalhos e notificações, separado do save |

## Desenvolvimento

| Termo | Significado |
|---|---|
| **Fatia vertical** | Versão mínima jogável: 1 cidade, Fazenda → Cozinha → Mercado. [[Arquitetura/Descrição\|Arquitetura]] |
| **Ficha técnica lógica** | Documento futuro que traduz os sistemas em estruturas, dados e regras de código; vem antes da programação |
| **Tiled** | Editor gratuito de mapas de tiles, usado para desenhar os mapas |
| **ECS** | Forma de organizar o código em entidades e componentes, para muitos NPCs rodarem rápido |
| **JSON** | Formato de arquivo de texto usado para os dados do jogo (itens, eventos, tecnologias...) |
