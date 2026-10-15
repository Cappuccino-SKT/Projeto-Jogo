> Resumo compacto do projeto, atualizado ao aplicar as respostas da rodada P0-5 (2026-10-15). Serve para retomar o trabalho sem reler tudo. As descrições completas ficam em cada pasta de sistema (ver [[Descrição do Projeto]]). **Para saber por onde recomeçar, veja a [[Retomada]].**

## Resolvido

**Mapa e Cidade**
- 4 níveis de mapa; arte toda à mão. LOD 0/1/2 por Mapa Local; Mapa Interno começa em LOD 2.
- Cada Mapa Local tem no máximo **uma cidade**. Estados de território: Sem dono, Território de facção (vizinho de cidade), Em disputa (vizinho de cidades de facções diferentes) e Com cidade. Facção e estado são derivados.
- Mapa em disputa gera guerras constantes até uma facção dominar ou o jogador interferir. Guerras de IA são resolvidas por **cálculo rápido de forças**; o Mapa de Guerra só existe quando o jogador participa.
- **Fator de Escala** global (1 NPC real = N pessoas; exemplo 100; valor a testar).
- Cidade nível 1 a 4 (nível 4 só capitais, por conquista). Dono de vila: o jogador pode fundar assentamento em território de facção.

**Facção**
- 6 facções principais = 6 culturas = 6 regiões.
- **PIB** (soma do dinheiro das cidades, só para comparar) e **Banco da Facção** (impostos; paga soldo, obras e guerra).
- Fundar facção: cidade nível 3, sem vínculo com outra facção (senão revolta), com 2 nobres na dinastia.
- Cidade herda **todas** as leis da facção. Leis: itens proibidos, preços, escravidão, imposto, conscrição, magia (inclui "privilégio dos magos"), crimes e penas, Poder do líder, Sistema de votação.
- Assembleia: uma por ano e extraordinárias; votos por cultura/cidade conforme a lei de votação; veto do líder depende da personalidade e do Poder do líder.
- Revolta: a resposta sai de um valor calculado (força, personalidade do líder, valor da cidade, outras guerras).
- Rito de Admissão e sucessão vêm da Cultura.

**Exército**
- Soldados **não são NPCs**: formam **Esquadrões** (inicialmente 1 esquadrão = 1 NPC removido = Fator de Escala soldados). Sem classe social.
- Conscritos = população × lei de Conscrição ÷ Fator de Escala. Alistar reduz produtividade e Felicidade. Importantes e guardas nunca são alistados.
- Manutenção: dinheiro (Banco da Facção), comida (Ração Militar) e materiais de equipamento (consumo periódico, sem durabilidade).
- **Caravana Militar** carrega suprimento (uso exclusivo militar). Sem suprimento, a Moral cai e os esquadrões desertam aos poucos.
- Ao dissolver, nascem NPCs novos das classes Escravo e Civil.

**Cultura**
- Traços pré-feitos combináveis; leis desejadas (valor ideal e peso); Felicidade cai com a distância entre as leis da facção e o ideal da cultura; cada cultura tem pelo menos 1 pensamento extremo.
- 6 ideologias: Escravista (mercantil), Religiosa, Autoritária, Tecnocrata, Guerreira e Arcana (Mago gera discriminação). Religião faz parte da cultura.
- Cultura da cidade = do dono + proporção de outras; NPC nasce com a da cidade; afinidades em 3 níveis (afim, neutra, hostil). Arte própria de construção por cultura.

**Crime e Justiça**
- Lista de crimes com gravidade 1 a 5; só existe crime com testemunha (Percepção × Destreza, segurança, noite). Em LOD 2, vira Criminalidade da cidade.
- Recompensa por facção, decai com o tempo. Guardas: rendição, prisão não letal (Inconsciente), força letal.
- Punições (multa, confisco, prisão, trabalho forçado, banimento, execução), variáveis por lei e por classe. Suborno depende de Ganância e Lealdade.
- **Loja ilegal:** qualquer construção de produção pode ser uma, após missão de confiança. Itens proibidos mudam por lei.
- Caçadores de Recompensa: base inicial = crime de gravidade 5. Guardas por cidade: nível e Criminalidade, mínimo por lei.

**Itens, economia e outros**
- **Rank** (0 a 5) para armas, equipamentos e artefatos; **Tier** (0 a 4) para materiais e consumo. Armadura = 3 peças separadas (Leve/Média/Pesada). Nomes unificados e canvases atualizados.
- Classe Social: Realeza consome Artefatos; mapeamento de consumo para itens (provisório).
- **Sem meses:** manutenção e produção em LOD 2 por estação (16 dias). O Construtor fornece a manutenção.
- Fruta entrou nos recursos; Peixe descartado. Novas construções previstas: Mercado, Taverna, Quartel, Prisão, Casa da Moeda e Templo.
- Início de Jogo: 7 estilos, personalizado com orçamento de pontos, bônus só iniciais, grupo por estilo.
- Cura: regeneração por descanso, Medicamento acelera, Hospital cura o grupo.
- Arquitetura: C++20 com g++/CMake/Ninja já instalados; stack proposta (SDL3, OpenGL 3.3, EnTT, JSON); primeira meta = fatia vertical.

**Eventos e Narrador**
- Dois tipos pelo gatilho: **Ocasional** (sorteio com chance ajustada pelo estado) e **Condicional** (condição atendida). Modelo único em JSON: Gatilho, Condições, Efeitos, Peso, Cooldown.
- Escopos: Mundo, Facção, Cidade/Mapa Local, Grupo do jogador. Duração: Pontual ou Duradouro. Efeitos de uma lista fechada.
- Ocasionais sorteados por dia (LOD 0 e 1) ou por estação (LOD 2); chance base × modificadores; cooldown e limite de eventos negativos por cidade; eventos positivos incluídos.
- Condicionais avaliados por sinal ou varredura diária/sazonal, com duração mínima e histerese; chance 100% para consequências óbvias.
- Todo evento guarda as **causas** ("Por quê?"). A simulação vem primeiro: o evento só força o que as regras não produzem sozinhas.
- Bandidos: grupos soltos ligados à Criminalidade; em LOD 2 viram "ameaça bandida" por região.
- **Narrador** = dificuldade escolhida no início; cada um tem um **foco**, e nenhum é mais difícil que outro. Regula só os ocasionais que afetam o jogador (tensão); o mundo longe do jogador usa sorteio simples.
- Eventos com 0 a 3 escolhas; notificações Info/Aviso/Crítico (Crítico pausa); diário de eventos com data e causas; gerador aleatório próprio com semente.

**Combate**
- Tempo real com **pausa ativa**; o jogo volta a 1x ao iniciar combate. Ordens básicas e quatro posturas (Agressiva, Defensiva, Manter posição, Evitar combate). "Em combate" = hostil dentro do raio de alerta.
- Dano: sorteio entre Dano Mín. e Máx. da arma × (habilidade ÷ 50) − Defesa, piso 1. Corpo a corpo sempre acerta; longo alcance tem chance por distância; sem projéteis físicos. Novo atributo **Intervalo de Ataque**. Vantagem arma × armadura de até ±25%.
- Modo letal/não letal por NPC. Medo, fuga e rendição pela Coragem. Dois slots de arma + Escudo; munição consumida sem recolher; sem estamina; armadura Média e Pesada reduzem a velocidade; Atributo Mínimo dá penalidade, não trava.
- **Armas e equipamentos de Rank alto podem ser encantados** (bônus de atributos). Feitiços e detalhes do encantamento no Sistema de Magia.
- Evolução por uso. Criaturas (animais/monstros) são entidades leves, não NPCs; bandidos e guardas são NPCs.
- Combate local até cerca de 40 combatentes; acima ou com Esquadrões, Mapa de Guerra. Morte permanente; se o grupo todo cai, é **capturado** (sem Game Over). Duelo como modo especial. Montarias só fora de combate por ora.
- Arte em camadas com animações por tipo de arma, em 4 direções.

**Tecnologia e Pesquisa**
- Duas camadas: **Tecnologia Mundial** (vale para todas as facções; desbloqueada por prazo no tempo, simulando o conhecimento se espalhando, como no Victoria 3) e **Tecnologia de Facção** (só para a facção; desbloqueada por pesquisa).
- Pontos de Pesquisa gerados no **Laboratório** por NPCs, conforme a Inteligência; o Banco da Facção paga a manutenção; o imposto para pesquisa (Tecnocrata) aumenta o orçamento.
- Árvore com pré-requisitos em 5 ramos (Agricultura e Produção, Construção, Militar, Economia, Administração) e 4 eras ligadas aos Tiers; pode ganhar camadas no futuro.
- Desbloqueios concretos (construções, receitas, Tier/Rank, leis, níveis de cidade); bônus percentuais raros.
- IA escolhe pela cultura/ideologia; o jogador escolhe se for líder. Difusão por conquista, diplomacia e roubo; nunca se esquece.
- Cada cultura começa com 2 ou 3 tecnologias da era 1. Entre 40 e 60 tecnologias; progresso por estação; a fatia vertical não tem tecnologia.
- Marcos mundiais: janela no calendário com sorteio; a pesquisa do mundo adianta, mas nunca fora da janela. 3 marcos maiores abrem as eras 2 a 4, mais 5 a 8 menores. Difusão: −10% de custo por facção que já tem a tecnologia, até −50%.
- Laboratório: trabalhadores Civis (bônus por Inteligência), gerente Nobre. Vila sem facção recebe os marcos e pesquisa com penalidade de custo.
- Magia fica em sistema separado, com a mesma mecânica de pontos.

**Magia**
- Magos raros: 5% da população (15% na Arcana); filho de Mago tem chance maior. Sem falha aleatória: o custo da magia é social e político.
- 3 escolas: Combate (dano e controle), Suporte (cura e bônus) e Ofício (encantamento e produção). Cada Mago tem afinidade com uma.
- Feitiços liberados pela pesquisa arcana e aprendidos com Grimório ou mestre; quantidade pela Inteligência. Mana regenera com o tempo (mais rápido em descanso); Pedra de Mana recupera.
- Combate: efeito instantâneo ou em área, com Mana, conjuração interrompível por dano e recarga; feitiços de área acertam aliados.
- Novas construções: Torre Arcana (Pontos Arcanos), Forja Arcana (Aço Mágico), Encantaria. Mago de Suporte acelera o Hospital.
- Encantamento: Rank 3+, na Encantaria, por Mago de Ofício, com Mana e Pedra de Mana; um por item.
- Árvore arcana de 15 a 25 itens, eras pelos marcos mundiais. Esquadrão de Magos: caro, limitado pelos Magos, só se a lei permitir.

**Objetivo e Progressão**
- Sandbox sem fim obrigatório. **Ambições** opcionais (uma sugerida por estilo de início, trocável; mais as grandes: conquistar capital, unificar região); ao cumprir, epílogo e o jogador continua ou encerra.
- Sem nível nem XP geral. **Renome** 0 a 100 por feitos: atrai companheiros, libera títulos e missões maiores; a má fama é a Recompensa.
- Escada de títulos: Plebeu → Cidadão → Cavaleiro → Nobre → Senhor de cidade → Líder de facção, com Renome e um requisito concreto.
- Missões: eventos com escolha, contratos no quadro da Taverna (nascem do estado do mundo) e missões longas de 3 a 5 etapas.
- Masmorras: conjunto fixo desenhado à mão, dificuldade 1 a 5, inimigos e Loot sorteados de tabelas, repovoam após estações; lendárias são únicas e guardam Artefatos.
- Derrota para Criaturas: o grupo acorda ferido, perde o Loot carregado e mantém o equipamento. **Crônica** dos feitos, sem pontuação. A fatia vertical não tem este sistema.

**Comércio civil e caravanas**
- Caravanas civis reais no mapa mundial (podem ser vistas e atacadas); em LOD 2 o comércio vira fluxo por estação.
- Donos: a cidade (mercadores abstratos, dinheiro da cidade, leva o que sobra e traz o que falta) e o jogador. A cidade precisa de um **Posto Comercial**.
- Rota pelo lucro esperado (diferença de preço − distância, risco e tarifas); o comércio aproxima os preços. O jogador só conhece os preços das cidades visitadas, com data.
- Caravana = animais de carga (capacidade por peso) + guardas + manutenção. Lei de **Tarifa**; guerra bloqueia; embargo ou acordo comercial (zera a tarifa).
- Riscos: bandidos, guerra, clima; perda de carga; contratos de escolta. Rota fixa liberada por tecnologia de Economia. Dinheiro passa entre cidades, total do mundo constante.

**Revisão geral (2026-10-14)** — ver [[Revisão do Projeto]]
- Lista de sistemas novos aceita (P0, P1 e P2): Felicidade e Estabilidade, População e Habitação, Trabalho/Salários/Propriedade, Interface, Mundo e Estado Inicial e Pipeline de arte (P0); Viagem, Guerra, Batalha, Diplomacia, Reputação, Companheiros e Diálogo, Religião, Clima, Notícias, Efeitos de itens e Escravidão (P1); Áudio, tutorial, configurações, localização, modding (P2).
- **NPCs reais por nível de cidade** (provisório): 15, 40, 100 e 300; o Fator de Escala converte em população total.
- As 8 inconsistências foram corrigidas (8x só no Mapa Mundial; título × classe; layout da cidade; entidades em trânsito; Hospital e Boticário; Grimório e Pedra de Mana; Intervalo de Ataque por tipo; Inventário por componentes).
- Mapas desenhados no **Tiled**. Mundo fixo com estado inicial variável. Recursos não se esgotam, têm produtividade por estação.
- **Ordem de trabalho:** foco total na documentação, sem código. Depois, uma **ficha técnica lógica**; só então a programação.
- Documentos consolidados (sem "Atualizações recentes"); criado o [[Glossário]].

**População e Habitação**
- Capacidade de NPCs reais por nível (15, 40, 100 e 300) e ocupação limitada por moradias e comida; população total = NPCs reais × Fator de Escala.
- Nascimentos e mortes em pessoas por estação com fração acumulada; a cada Fator de Escala nasce ou morre 1 NPC real. Importantes e Controlados têm ciclo de vida completo; NPCs comuns são repostos; crianças comuns não são NPCs.
- Vida em anos de jogo (criança 0–3, adulto 4–20, velho 21+, morte natural entre 21 e 30); 1 ano ≈ 17 h reais a 1x.
- Natalidade, mortalidade (velhice, doença, fome, guerra, crime) e migração por atratividade; migrantes mantêm a cultura; guerra e fronteira fechada bloqueiam a migração.
- Moradia por classe (Senzala, Casa, Casarão, Mansão, Palácio); a cidade constrói sozinha em lotes fixos marcados no Tiled (3 tamanhos); poucos layouts por tipo e cultura.
- Subir de nível: ocupação acima de 90% por 2 estações, Felicidade, construções obrigatórias, tecnologia e custo. Queda de nível só em catástrofe grave. Mesmas regras para jogador e IA.

**Trabalho, Salários e Propriedade**
- Ciclo do dinheiro: consumidores → lojas → salários e insumos → trabalhadores. A distribuição rotativa do Banco da Cidade vira estabilizador. O jogo guarda a riqueza média por classe e por cidade; o Dinheiro individual só existe para Importantes, Controlados e Ricos donos de empresas.
- Emprego por vagas por nível da construção; desemprego afeta renda, Felicidade, Criminalidade e migração. Escravos sem salário; guardas pagos pelo Banco da Cidade; soldo pelo Banco da Facção.
- Atributo **Dono** nas construções: Cidade, Facção, Rico ou jogador. Empresas dos Ricos contadas por cidade em LOD 2. Comprar, vender e transferir; a conquista leva as construções da Cidade e da Facção.
- Salário por Função e nível; jogador escolhe Baixo, Normal ou Alto. Imposto único sobre receita, por estação. Preço do jogador dentro de ±30% do Mercado Local. Insumos comprados do inventário da cidade.
- Mobilidade Civil → Rico por vagas de empresa. Falência e leilão. A Universidade melhora a Inteligência e o Trabalho Especialista e dá bônus aos Pesquisadores.
- Tabela mestre [[Sistema de Produção/Funções e Construções|Funções e Construções]] criada a partir dos canvases.

**Felicidade e Estabilidade**
- **Felicidade** de 0 a 100 **por classe social** em cada cidade (a da cidade é a média ponderada). Alvo = 50 + contribuições de: consumo, leis × cultura, Criminalidade, desemprego e renda, moradia, conscrição, guerra, imposto, eventos, religião e repressão. Inércia: anda uma fração por dia (10%) em direção ao alvo.
- Faixas: Contente (>70), Estável (40–70), Inquieta (25–40), Protesto (10–25) e Revolta (<10). Reações por classe: Escravo foge/rebela, Civil faz greve e migra, Rico fecha empresas, Nobreza pressiona leis, Realeza golpe.
- **Estabilidade = Felicidade ponderada + Controle − 10**; a revolta ocorre abaixo de zero por alguns dias. Controle (0 a 30) vem de guardas, Esquadrões estacionados e Medo; custa dinheiro e reduz a Felicidade das classes baixas (menos nas culturas Autoritárias).
- A facção tem Estabilidade própria (cidades + legitimidade do líder). Controlados: 6 fatores, aviso abaixo de 20 e saída com atraso no 0. Painel "Por quê?" com os 5 maiores fatores.

**Interface e Controles**
- Mundo em pixel-art de escala inteira; **interface em resolução nativa e nítida**, com fontes comuns legíveis (letras não desenhadas à mão por ora), molduras e ícones à mão. Resolução de referência **1600 × 900**; escala ajustável de 100% a 200%. Tema único.
- **HUD bastante personalizável**: mover, redimensionar, esconder, escala, opacidade, ancoragem com encaixe, perfis de layout e restaurar padrão, num modo de edição. Layout guardado em perfil do jogador, fora do save.
- HUD padrão, **uma janela de gestão por vez** com abas, lentes do Mapa Mundial, modo construção por lotes, painel de trabalho (Função × prioridade), pausa ativa com ordens visíveis.
- Só teclado e mouse (sem controle de console), atalhos remapeáveis, zoom em níveis inteiros (1x a 4x).
- Notificações **personalizáveis** (categorias, limite, agrupamento, o que aparece em velocidade alta, se o Crítico pausa). "Por quê?" por passar o mouse e barra de alertas. Enciclopédia gerada dos dados. Menus: novo jogo em 3 passos, autosave por estação. Interface própria; Dear ImGui só para depuração.

**Mundo e Estado Inicial** — ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]
- Malha de **48 Mapas Locais (8 × 6)**, 6 regiões de 8 mapas (2 × 3 blocos): Guerreira e Arcana ao norte, Autoritária e Religiosa no centro, Tecnocrata e Escravista ao sul. Culturas hostis ficam vizinhas.
- Por região: 1 capital, 1 cidade e 1 vilarejo (a Escravista tem 2); mais 3 **Vilas Livres** sem facção. No primeiro dia: 16 mapas de território, 6 em disputa e só 4 sem dono (8%, contra 30% sugeridos: em aberto).
- Bioma e 2 a 3 recursos de destaque por região, com escassez de outros. **12 masmorras** (2 por região), dificuldade pela distância da capital; 3 lendárias no meio do mundo.
- **Tom:** baixa magia, chegada da pólvora, sombrio e político; criaturas míticas só no futuro, em masmorras. **Só humanos** nas primeiras versões.
- **Nomes significativos** (Facção Guerreira, Capital Arcana) e **personagens abstratos** até a história ser escrita; a história do mundo fica para depois.
- **Ano 412**, 1º dia da Primavera; todos na Era 1. PIB entre −19% e +21% da média; cada facção forte numa coisa (a Guerreira é a mais pobre e tem o maior exército).
- Matriz de afinidade 6 × 6; valor de relação = afinidade (+40, 0, −40) − 15 se dividem mapa em disputa ± 15 sorteado; 1 ou 2 guerras iniciais e 2 acordos comerciais sorteados. Pacotes de construções por nível e cultura; 5 Importantes fixos por papel; 2 ou 3 pontos de partida por estilo.

## Em aberto

- **Testes:** valor do Fator de Escala e tamanho final do esquadrão; gravidades e multas; fórmula da Criminalidade; recompensa mínima; limiar de controle de mapa em disputa; taxa de regeneração e custo do Hospital.
- **Cultura:** como reaproveitar partes da arte de construção (os nomes ficam para a história do jogo).
- **Mundo:** mapas sem dono (8% na proposta, contra 30% sugeridos); Dono de vila em mapa livre ou em território; história, nomes e personagens; listas de tecnologias por cultura; valores do estado inicial.
- **Facção:** detalhes da assembleia.
- **Crime:** loja ilegal do jogador; tipos de missão de confiança.
- **Cura:** quando o Inconsciente vira Morto; itens de Medicamento.
- **Itens:** Tier e Rank provisórios dos itens a cadastrar; peça de armadura Média; revisar o mapeamento Luxo/Exclusivo por Tier.
- **Início de Jogo:** custo em pontos; bônus exato do estilo Mercador.
- **Eventos:** valores a testar (chances, limite de negativos, tensão, histerese); nomes e foco final dos narradores e se há controle de intensidade; categorias e efeitos extras; doença em LOD 0; formato do diário; lista inicial de eventos.
- **Combate:** valores a testar (dano, acerto, Intervalo de Ataque, vantagens, raio de alerta, Medo, evolução, limite de combatentes); tabela de bônus e custo do encantamento; confirmar a regra de armadura mista.
- **Tecnologia:** valores a testar (janelas, adiantamento, pontos, custos, penalidade); lista dos marcos e das tecnologias iniciais; cultura Agrária.
- **Construção:** custos e níveis das construções novas (Mercado, Taverna, Quartel, Prisão, Casa da Moeda, Templo, Posto Comercial, Laboratório, Torre Arcana, Forja Arcana, Encantaria, Hospital, Boticário, Universidade); ainda não estão nos canvases.
- **Magia:** lista de feitiços e da árvore arcana; valores a testar; como obter Grimório; Esquadrão de Magos no Mapa de Guerra; feitiços de Suporte fora do combate.
- **Objetivo:** condições das Ambições e epílogos; Renome por feito e por título; repovoamento; lista de contratos, missões e masmorras; recrutamento de companheiros.
- **Comércio:** valores (capacidade, salário, Tarifa, peso do risco); animais de carga; guardas como NPCs ou valor abstrato; fórmula do fluxo em LOD 2.
- **População:** valores a testar (taxas, vida em anos de jogo, capacidade das moradias, custo de subir de nível); lotes por mapa e nível; pesos da migração; Senzala; moradia dos Controlados; crianças decorativas.
- **Trabalho:** valores a testar (vagas, salários, faixa de preço, estabilizador, imposto, empresas dos Ricos); nome e categoria das Funções; Engenheiro e Operário sem construção; Espada sem construção no canvas do Ferreiro; gerente das construções da Cidade; leilão; forma de estudo na Universidade.
- **Felicidade:** valores a testar (contribuições, inércia, Controle, dias para revolta, efeitos das faixas); Medo agregado; fuga e rebelião de Escravos; greves; layout do painel "Por quê?".
- **Interface:** elementos fixos do HUD; janelas de gestão móveis ou não; opções exatas de notificação; fonte do texto; painel "Por quê?"; telas do Mapa de Guerra; IDs de texto para outros idiomas.
- **Arquitetura:** PC alvo mínimo; aprovação das bibliotecas; tamanho do tile, perspectiva e mundo (provisórios).

## Próximos passos

**Lista de sistemas (da [[Revisão do Projeto]]):**

| Prioridade | Sistema | Estado |
|---|---|---|
| P0 | Felicidade e Estabilidade | Escrito |
| P0 | População e Habitação | Escrito |
| P0 | Trabalho, Salários e Propriedade (e Funções e Construções) | Escrito |
| P0 | Interface e Controles | Escrito |
| P0 | Mundo e Estado Inicial (a história fica para depois) | Escrito |
| P0 | **Pipeline de arte e conteúdo** | **Perguntas enviadas, aguardando respostas** |
| P1 | Viagem e Mundo; Reputação; Companheiros e Diálogo; Guerra e Campanha; Batalha; Diplomacia; Religião; Clima e Estações; Notícias; Efeitos de itens e Escravidão | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |

**Ordem:** responder a rodada P0-6 (Pipeline de arte) → sistemas P1 → quando o autor estiver satisfeito com a documentação, a **ficha técnica lógica** → só então a programação. Os detalhes da rodada pendente estão na [[Retomada]].

## Como estamos trabalhando

- Cada rodada: uma lista de perguntas com sugestão. As respostas vêm num arquivo `Respostas.txt` enviado na conversa (ou direto no chat); "seguir sua sugestão" aceita a proposta. A ideia própria do autor prevalece. As perguntas e sugestões seguem o **padrão detalhado** descrito na [[Retomada]] (pedido do autor).
- Marcações nos documentos: **[Proposta]**, **[Em aberto]**, **[Refinar depois]**. Os termos estão no [[Glossário]].
- Por ora **não se escreve código**: a documentação vem primeiro, depois a ficha técnica lógica, depois a programação.
- O projeto está no GitHub (`FelipeSKT/Projeto-Jogo`), com histórico de versões. Cada rodada vira um pull request para o autor aceitar. O jeito de trabalhar e os cuidados técnicos estão na [[Retomada]].
