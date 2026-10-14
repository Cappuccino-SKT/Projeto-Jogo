> Resumo compacto do projeto em 2026-10-14, depois da revisão geral e dos Sistemas de Eventos, Combate, Tecnologia e Magia. Serve para retomar o trabalho sem reler tudo. As descrições completas ficam em cada pasta de sistema (ver [[Descrição do Projeto]]).

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

## Em aberto

- **Testes:** valor do Fator de Escala e tamanho final do esquadrão; gravidades e multas; fórmula da Criminalidade; recompensa mínima; limiar de controle de mapa em disputa; taxa de regeneração e custo do Hospital.
- **Cultura:** nomes das culturas e regiões; como reaproveitar partes da arte de construção.
- **Facção:** detalhes da assembleia; caravanas civis (Sistema Econômico).
- **Crime:** loja ilegal do jogador; tipos de missão de confiança.
- **Cura:** quando o Inconsciente vira Morto; itens de Medicamento.
- **Itens:** Tier e Rank provisórios dos itens a cadastrar; peça de armadura Média; revisar o mapeamento Luxo/Exclusivo por Tier.
- **Construção:** custos e níveis das novas construções; ainda não estão nos canvases.
- **Início de Jogo:** custo em pontos; o estilo Mercador depende das caravanas civis.
- **Eventos:** valores a testar (chances, limite de negativos, tensão, histerese); nomes e foco final dos narradores e se há controle de intensidade; categorias e efeitos extras; doença em LOD 0; formato do diário; lista inicial de eventos.
- **Combate:** valores a testar (dano, acerto, Intervalo de Ataque, vantagens, raio de alerta, Medo, evolução, limite de combatentes); tabela de bônus e custo do encantamento; derrota para Criaturas; masmorras; confirmar a regra de armadura mista.
- **Tecnologia:** valores a testar (janelas, adiantamento, pontos, custos, penalidade); lista dos marcos e das tecnologias iniciais; cultura Agrária.
- **Construção:** custos e níveis do Laboratório, Torre Arcana, Forja Arcana e Encantaria.
- **Magia:** lista de feitiços e da árvore arcana; valores a testar; como obter Grimório; Esquadrão de Magos no Mapa de Guerra; feitiços de Suporte fora do combate.
- **Arquitetura:** PC alvo mínimo; aprovação das bibliotecas; tamanho do tile, perspectiva e mundo (provisórios).

## Sistemas ainda não escritos (ordem)

1. Objetivo do jogo e progressão
2. Comércio civil e caravanas

## Como estamos trabalhando

- Cada rodada: uma lista de perguntas com sugestão. As respostas vêm num arquivo `Respostas.txt` enviado na conversa (ou direto no chat); "seguir sua sugestão" aceita a proposta.
- Marcações nos documentos: **[Proposta]**, **[Em aberto]**, **[Refinar depois]**.
- O projeto está no GitHub (`FelipeSKT/Projeto-Jogo`), com histórico de versões. Cada rodada vira um pull request para revisão.
