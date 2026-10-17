> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a rodada V3 (Pendências de regra) aplicada e a V4 (Confirmação das propostas, parte 1) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas P0 e P1 estão escritos, e a **revisão completa** está em andamento (levantamento e plano na [[Revisão Completa]]). A V1, a IP1 e a IP2 estão no `main` (PRs #9 a #11). A IP3, a V2 e a V3 estão em branches empilhadas, cada uma criada a partir da anterior: `docs/ip3-receitas` → `docs/v2-economia` → `docs/v3-pendencias`. A V3 foi aplicada em 2026-10-17 e criou o documento [[Arquitetura/Moldes de Conteúdo|Moldes de Conteúdo]] (ideia do autor). A **rodada V4 (Confirmação das propostas, parte 1) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se os pull requests foram mesclados no `main`, na ordem IP3 → V2 → V3.**
3. **Próximo passo:** pedir o `Respostas.txt` da V4 (conferir se é desta rodada), tirar as marcações confirmadas, aplicar as mudanças (F-14, A-4, A-7) e abrir a **V5** (propostas dos outros documentos).
4. Depois: a **ficha técnica lógica da fatia vertical**. As listas de conteúdo ficam para as rodadas C1 a C3, mais para frente (ideia do autor na V3-13).
5. Quando o autor estiver satisfeito com a documentação: **ficha técnica lógica**, e só então a programação.

## 2. Como trabalhamos

- **Idioma:** sempre português do Brasil.
- **Fluxo:** o assistente manda uma lista de perguntas numeradas, cada uma com uma **sugestão**. O autor responde no arquivo `C:\Users\Felipe\Desktop\Respostas.txt` (anexado com @) ou no chat. "Seguir sugestão" aceita a proposta; a ideia própria do autor sempre prevalece. Depois o assistente escreve o documento do sistema, atualiza os outros documentos, e abre a próxima rodada.
- **Antes de aplicar respostas:** conferir se o `Respostas.txt` é mesmo da rodada atual (olhar os itens e a data de gravação).
- **Decisões do autor:** o que não tem marcação nos documentos foi decidido por ele. As marcações são **[Proposta]**, **[Em aberto]** e **[Refinar depois]** (ver [[Glossário]]).
- **Sem código por enquanto.** Foco total em documentação. Depois, uma **ficha técnica lógica** (como cada sistema vira estruturas, dados e regras) e só então a programação. O autor tem experiência mínima em C++ e nenhuma com bibliotecas: explicar em linguagem simples.
- **Preferências do autor:**
  - interface e HUD **bem personalizáveis** (ele joga em 1600 × 900 e não gosta de interface fixa);
  - notificações personalizáveis;
  - letras da interface em fontes comuns legíveis (não desenhadas à mão);
  - arte do mundo toda à mão, nada procedural;
  - só teclado e mouse.
- **Estilo dos documentos:** primeira linha com a legenda das marcações; "Descrição geral"; seções do sistema; "Parte técnica" (LOD, dados em JSON, save, fatia vertical); tabela "Ligação com outros sistemas"; "Perguntas em aberto". Cada sistema novo vira uma pasta `Sistema de X/Descrição.md`, entra no [[Descrição do Projeto]] (índice) e no Estado, e as ligações são acrescentadas nos documentos dos sistemas vizinhos. Não usar seções de "Atualizações recentes": o histórico fica no Git.
- **Princípios do projeto:** a simulação vem primeiro e o evento explica; tudo agregado por cidade e por LOD; dados em JSON; "por quê?" em tudo.

### Padrão das perguntas e sugestões (obrigatório)

Pedido do autor em 2026-10-15: as perguntas da P0-5 saíram abstratas ("B2. Tom", "D1. Equilíbrio") e as sugestões, curtas demais. A partir da P0-6, **toda rodada segue este padrão**:

1. **Contexto** (1 a 3 linhas): o que já está decidido sobre o tema e o que a pergunta destrava.
2. **Pergunta concreta:** o que exatamente precisa ser decidido, com números, nomes e um exemplo de situação de jogo. Uma pergunta por decisão; um tema com várias decisões vira vários itens.
3. **Opções** (quando houver): 2 a 4, cada uma com o que ganha e o que perde (arte, programação, desempenho, diversão).
4. **Sugestão completa:** a opção recomendada e o porquê, pronta para virar documento: valores iniciais, tabela ou lista, exemplo prático, impacto nos outros sistemas e o que fica para testar.
5. **Conflitos:** se a sugestão ou uma resposta anterior bate com outra decisão, apontar e propor a saída.

Regras extras:

- Explicar termos e ferramentas em linguagem simples.
- Quando um "seguir sugestão" deixar algo vago, o documento detalha com valores concretos marcados como **[Proposta]**.
- Economizar palavras sem cortar informação útil: nada de introduções longas.
- Formato fácil de responder no `Respostas.txt`: códigos (A1, B2...) e títulos curtos.

**Exemplo.** Abstrato (evitar): "B2. Tom: fantasia de baixa magia?". Detalhado (seguir): "B2. Quanta magia aparece no dia a dia? *Contexto:* a cultura Arcana dá privilégios aos Magos, e o Combate tem Cajado. *Opções:* (a) magia rara, cerca de 1 Mago para cada 50 NPCs, feitiços caros; (b) magia comum, todo guarda de cidade Arcana conjura. *Sugestão:* (a), porque mantém o tom sombrio e político e reduz a arte de efeitos. *Impacto:* Magia (quantidade de Magos por cidade), Combate (Esquadrão de Magos raro), arte (poucos efeitos de feitiço)."

## 3. Lista de sistemas (da Revisão)

| Prioridade | Sistema | Estado |
|---|---|---|
| P0 | Felicidade e Estabilidade | Escrito |
| P0 | População e Habitação | Escrito |
| P0 | Trabalho, Salários e Propriedade (+ Funções e Construções) | Escrito |
| P0 | Interface e Controles | Escrito |
| P0 | Mundo e Estado Inicial | Escrito (a história fica para depois) |
| P0 | Pipeline de arte e conteúdo | Escrito |
| P1 | Viagem e Mundo | Escrito |
| P1 | Reputação e Relações do jogador | Escrito |
| P1 | Companheiros, Relações familiares e Diálogo | Escrito |
| P1 | Guerra e Campanha | Escrito |
| P1 | Batalha (Mapa de Guerra) | Escrito |
| P1 | Diplomacia | Escrito |
| P1 | Religião | Escrito |
| P1 | Clima e Estações | Escrito |
| P1 | Notícias e Informação | Escrito |
| P1 | Efeitos de itens e Escravidão | Escrito |
| Revisão | Revisão completa: V1 (inconsistências e faxina) | Aplicada |
| Revisão | Itens e Produção: IP1 (padronização) | Aplicada |
| Revisão | Itens e Produção: IP2 (nomes) | Aplicada |
| Revisão | Itens e Produção: IP3 (insumos) | Aplicada |
| Revisão | V2 (economia numérica) | Aplicada |
| Revisão | V3 (pendências de regra) | Aplicada |
| Revisão | **V4 (propostas, parte 1)** | **Enviada, aguardando respostas** |
| Revisão | V5 (propostas, parte 2) | A fazer |
| Depois | Ficha técnica da fatia vertical; rodadas de conteúdo C1 a C3 | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, modo observador, eventos de dinastia. As ordens de produção em lote foram decididas na IP1-7 ([[Sistema de Produção/Descrição|Produção]]). A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: V4, Confirmação das propostas (parte 1)

**Perguntas enviadas em 2026-10-17, aguardando respostas.** As marcações **[Proposta]** são detalhes que o assistente acrescentou às sugestões aceitas, ou sugestões que o autor ainda não viu uma por uma. Esta rodada confirma as de **7 documentos**, mais os moldes de conteúdo da V3-13. A V5 faz o mesmo com os outros documentos.

**Como responder:** "Confirmar todas" ou "Confirmar todas, exceto F-3 e F-7 (e o que mudar)". Uma proposta confirmada perde a marcação e vira decisão do autor. A recomendação de cada uma vem no fim da linha: **Confirmar**, **Mudar** (com o motivo) ou **Já decidida** (só limpar a marcação).

**V4-1. Facção (17 propostas).** [[Sistema de Facção/Descrição|Facção]]

- F-1. Cultura dominante = a de maior população somada nas cidades, recalculada a cada estação. **Confirmar.**
- F-2. Número de cidades, PIB e exército não são guardados: são somados das cidades, em cache. **Confirmar.**
- F-3. Alistamento e formação de Esquadrões no Quartel. **Confirmar.**
- F-4. A Caravana Militar usa o componente de Inventário (Carga), não luta, mas acompanha o exército. **Confirmar.**
- F-5. Caravana capturada ou destruída: o exército perde a carga (saque). **Confirmar.**
- F-6. Poder do líder em 3 níveis: Autoritário, Moderado e Fraco. **Já decidida** (V3-1 usa os três).
- F-7. Agressividade alta e Sociabilidade baixa aumentam a chance de veto. **Já decidida** (fórmula da V3-7).
- F-8. Dinastia = lista de nobres ligados ao líder (Líder, Herdeiro, Nobre), todos Importantes. **Confirmar.**
- F-9. O vínculo do mapa vizinho é derivado; 4 estados do Mapa Local. **Confirmar.**
- F-10. Vassalagem: obrigações (tributo e chamado às armas) e benefícios (proteção, preços da facção, cargos). **Confirmar.**
- F-11. A facção decide sobre a cidade que quer se desvincular pelo valor da cidade e pelas outras guerras. **Confirmar.**
- F-12. Ações diante da independência: aceitar, negociar tributo menor, declarar guerra ou bloquear o comércio. **Confirmar.**
- F-13. O jogador pertence a uma facção por vez: Independente, Membro, Vassalo ou Líder. **Confirmar.**
- F-14. Requisitos para fundar facção: dinheiro mínimo e escolha da cultura, do nome e das leis. **Mudar:** dar o número da V2, **2.000 moedas** no Banco da Facção novo.
- F-15. Facção sem cidades é dissolvida; o líder e a dinastia viram exilados e podem refundar. **Confirmar.**
- F-16. Atributo Título/Cargo nos NPCs Importantes. **Confirmar.**
- F-17. Legenda e textos que citam propostas já feitas. **Limpar.**

**V4-2. Mundo e Estado Inicial (14 propostas).** [[Mundo e Estado Inicial/Descrição|Mundo]]

- M-1. Campo "Raça" no NPC, com um só valor (Humano), para não fechar a porta a outras raças. **Confirmar.**
- M-2. Ordem das regiões: culturas hostis vizinhas; Tecnocrata e Arcana em cantos opostos. **Confirmar.**
- M-3. Bordas: montanhas ao norte, mar a leste e ao sul, estepe e deserto a oeste. **Confirmar.**
- M-4. O mapa inicial célula por célula (a malha 8 × 6). **Confirmar.**
- M-5. Cidades iniciais: 1 capital, 1 cidade e 1 vilarejo por região (2 vilarejos na Escravista). **Confirmar.**
- M-6. Biomas, recursos de destaque e escassos de cada região. **Confirmar.**
- M-7. Dificuldade das masmorras pela distância até a capital (1 a 4; lendárias 5). **Confirmar.**
- M-8. Valores iniciais das facções (NPCs, Riqueza, PIB, Esquadrões). **Confirmar.** O PIB já bate com os Bancos da V2.
- M-9. Pacotes de construções por nível e cultura. **Confirmar.**
- M-10. Afinidade entre as culturas (afim, neutra, hostil). **Confirmar.**
- M-11. 5 Importantes fixos: Arquimago, Rei Exilado, Grande Mercador, Sumo Sacerdote e Mestre Inventor. **Confirmar.**
- M-12. Limites do estado inicial variável (relações ±15, 1 ou 2 guerras, 2 acordos...). **Confirmar.**
- M-13. Pontos de partida de cada estilo (2 ou 3 por estilo). **Confirmar.**
- M-14. Frase "os detalhes marcados como [Proposta] foram acrescentados agora". **Limpar.**

**V4-3. Religião (11 propostas).** [[Sistema de Religião/Descrição|Religião e Fé]]

- R-1. Na Arcana, cada Mago da Torre Arcana conta como Sacerdote para a cobertura. **Confirmar.**
- R-2. Datas dos festivais de cada cultura (Festa da Colheita nos dias 1 e 2 do Outono...). **Confirmar.**
- R-3. Os dias santos da Religiosa duram 1 dia, com metade dos efeitos. **Confirmar.**
- R-4. O bônus de Felicidade do festival dura 8 dias. **Confirmar.**
- R-5. O evento Social positivo passa a ser a "festa espontânea" (vitória, casamento, visitante ilustre). **Confirmar.**
- R-6. A Felicidade −15 da Perseguição vale só na parte da população que é minoria. **Confirmar.**
- R-7. A IA do Sumo Sacerdote dá Bênção a quem tem +40 com a Religiosa e fez algo pela fé. **Confirmar.**
- R-8. Anátema contra o jogador sem facção: −20 de reputação com as facções Religiosas. **Confirmar.**
- R-9. Oferenda mínima de 50 moedas na Capital Religiosa. **Confirmar.** São cerca de 6 dias de salário de um Civil pela V2.
- R-10. Peregrinos: +10% de consumidores na Capital Religiosa e na cidade de peregrinação. **Confirmar.**
- R-11. Legenda. **Limpar.**

**V4-4. Pipeline de Arte e Conteúdo (11 propostas).** [[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]

- P-1. Pacotes provisórios de preferência CC0, com a origem anotada em `assets/provisorio/LEIA-ME.txt`. **Confirmar.**
- P-2. Pixelorama para desenhar. **Já decidida** (o autor escolheu).
- P-3. VS Code para editar os dados. **Confirmar.**
- P-4. O texto "por que o Pixelorama". **Já decidida.**
- P-5. Primeiros passos para aprender pixel art (tutoriais do Lospec, exercícios). **Confirmar.**
- P-6. Paleta mestre a partir de uma paleta pública do Lospec (Endesga 32 ou Resurrect 64). **Confirmar.**
- P-7. Estimativa do peso da arte na memória. **Confirmar** (é uma conta, não uma regra).
- P-8. Teto inicial de 256 MB de memória de imagens. **Confirmar.** Cabe na placa de 2 GB da V3-12.
- P-9. O jogo lê o CSV direto, em C++. **Confirmar.**
- P-10. Modo `jogo --validar`. **Confirmar** (já usado em vários documentos).
- P-11. Estrutura de pastas do projeto. **Confirmar.**

**V4-5. Clima e Estações (9 propostas).** [[Sistema de Clima e Estações/Descrição|Clima e Estações]]

- C-1. A Água rende 100% o ano todo. **Confirmar.**
- C-2. Tempestade: trabalho ao ar livre −50% naquele dia. **Confirmar.**
- C-3. Chances de cada clima por estação (bioma temperado). **Confirmar.**
- C-4. Ajustes por bioma (neve nas montanhas, pouca chuva nas terras áridas, tempestade no litoral). **Confirmar.**
- C-5. A tempestade forte danifica construções de madeira; o reparo sai pela manutenção. **Confirmar.** O evento da V3-6 usa isso.
- C-6. "Efeitos do inverno × 1,5" nos biomas frios. **Confirmar.**
- C-7. Horas das 4 fases do dia. **Confirmar.**
- C-8. Limite de 300 partículas de clima na tela. **Confirmar.**
- C-9. Legenda. **Limpar.**

**V4-6. Arquitetura (7 propostas).** [[Arquitetura/Descrição|Arquitetura]]

- A-1. g++ (MinGW) com CMake e Ninja, sem Visual Studio; C++20. **Confirmar.**
- A-2. As bibliotecas sugeridas (SDL3, OpenGL 3.3, EnTT, leitor de JSON, Dear ImGui, Tracy). **Adiar:** a aprovação fica para o primeiro dia de código, como já previsto.
- A-3. Composição em vez de herança (ECS). **Confirmar.**
- A-4. Dados em JSON numa pasta `data/`. **Mudar:** JSON para os dados com estrutura e **CSV** para as tabelas grandes (itens, receitas, preços), como o Pipeline e os Moldes já dizem.
- A-5. Memória por Mapa Local (arenas: tudo de um mapa é criado e apagado junto). **Confirmar.**
- A-6. Save em arquivo binário com número de versão. **Confirmar.**
- A-7. Os 8 marcos da fatia vertical. **Mudar:** acrescentar o marco "simulação sem tela de 1 ano" (o teste da V2-12) antes da interface.

**V4-7. Efeitos de Itens (8 propostas).** [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]

- E-1. Livro de Atributos de Rank 4 e 5 só nas masmorras difíceis. **Confirmar.**
- E-2. Livro Sagrado por 20 moedas, no Templo e no Mercado das cidades Religiosas. **Confirmar.**
- E-3. Quem precisa do Livro Sagrado: só os Controlados (não os Escravos do jogador nem os NPCs comuns). **Confirmar.**
- E-4. Chance de revista pela reputação do jogador. **Confirmar.**
- E-5. Heresia leve na primeira vez da estação: gravidade 2, multa (100 moedas pela V3-3) e compra obrigatória do livro. **Confirmar.**
- E-6. A lista dos Artefatos lendários iniciais (nomes provisórios). **Confirmar.**
- E-7. Alimento de Tier 3: +4 no fator comida. **Confirmar.**
- E-8. Vinho: +3 de Felicidade. **Confirmar.**

**V4-8. Os moldes de conteúdo (ideia do autor na V3-13).**

- *Contexto:* o autor pediu que as listas grandes virem estruturas modulares, prontas para copiar e modificar, e que agora só os moldes fiquem prontos. O documento [[Arquitetura/Moldes de Conteúdo|Moldes de Conteúdo]] foi criado com:
  - as regras comuns: ID legível, listas fechadas, referência por ID, JSON e CSV;
  - os moldes que já existiam: Item, Receita, Construção, Evento, Estilo de início e Traço de cultura;
  - os moldes novos: Tecnologia, Marco mundial, Lei, Feitiço, Contrato e missão, Ambição, Narrador e Masmorra.
- *Pergunta:* os moldes novos estão bons para começar? Falta algum tipo de conteúdo?
- *Sugestão:* **confirmar os 8 moldes novos.** As listas completas ficam para as rodadas de conteúdo, mais para frente:
  - **C1:** tecnologias e marcos;
  - **C2:** feitiços;
  - **C3:** contratos, missões, Ambições e eventos.

  Outros tipos que podem virar molde no futuro: Raça, Criatura e Título de Nobreza.
- *Impacto:* Arquitetura, Tecnologia, Facção, Magia, Objetivo, Eventos.

Depois das respostas:

- tirar as marcações confirmadas;
- aplicar as mudanças (F-14, A-4, A-7);
- abrir a **V5** (propostas dos outros documentos).

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). A família de cada uma foi fixada na IP1-2 (a Espada está no Ferreiro), e as receitas estão em [[Sistema de Produção/Receitas|Receitas]] (IP3).
- **Arte:** o autor desenha por enquanto (sem experiência em pixel art), no **Pixelorama** (grátis), com pacotes de terceiros provisórios na fatia vertical. Mapas no **Tiled** (explicar em linguagem simples). Sem verba para programas pagos. Projeto **sem prazo**: o limite da arte é o peso no jogo, não o tempo (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
- **Estações na arte:** só mudança de cor por enquanto; **toda arte extra de estação fica para o futuro** (neve, rios congelados, folhas, flores).
- **Viagem:** 4 h por Mapa Local a pé, multiplicada por terreno, infraestrutura, tamanho do grupo, montaria e carga; caravanas sempre reais no Mapa Mundial (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).
- **Grupo:** controlado = 4 + Renome ÷ 20; vinculados = 10 + Renome ÷ 5 + 3 por cidade; **sem retratos** nas conversas (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).
- **Linguagem:** o foco é **C++**; ferramentas em outras linguagens só como ajuda, nunca essenciais.
- **Guerra e Batalha:** Placar de Guerra (−100 a +100) paga a paz; cidade ocupada só muda de dono no tratado; exército de até 20 Esquadrões com General; **o mundo para durante a batalha**; a simulação é sempre por Esquadrão de 100 soldados, e o jogador escolhe o **tamanho visual** (20, 50 ou 100 bonecos por Esquadrão; até 4.000 soldados na tela, pedido do autor). A 7ª camada do Tiled é a Elevação. Na programação, a guerra por cálculo rápido vem antes do Mapa de Guerra (ver [[Sistema de Guerra/Descrição|Guerra]], [[Sistema de Batalha/Descrição|Batalha]]).
- **Diplomacia:** estado (Paz, Guerra, Trégua) + tratados que se somam + atitude (faixa da relação); Confiabilidade; vassalo tem leis próprias (ver [[Sistema de Diplomacia/Descrição|Diplomacia]]).
- **Religião:** uma fé por cultura, com Devoção (Alta na Religiosa, nenhuma na Tecnocrata); cada Sacerdote atende 20 NPCs; fator Religião de −5 a +10; 2 festivais por ano (4 na Religiosa); lei de Tolerância religiosa; heresia é um ato; assimilação de 1% a 4% por estação; Bênção e Anátema do Sumo Sacerdote (ver [[Sistema de Religião/Descrição|Religião e Fé]]).
- **Clima:** Fazenda 75%, 100%, 150% e 25% nas quatro estações (base temperada); clima do dia sorteado por região, com 60% de repetir; lenha no inverno (1 Tora por NPC real a cada 4 dias, × bioma); vontade de guerra −15 no inverno; noite com visão −30%; arte só com paleta, tom e partículas (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Notícias:** 3 h por mapa (o mundo em 1 dia); Local, Regional e Mundial; precisão cai com a distância; a IA sabe tudo na hora (simplificação) (ver [[Sistema de Notícias/Descrição|Notícias]]).
- **Efeitos de itens:** só Controlados e Importantes usam itens com efeito individual; Livro +3 (até 60) e Livro Antigo +6 (até 80), sem se gastar; Livro de Atributos +2 × Rank, se gasta; **Livro Sagrado obrigatório** na Religiosa (ideia do autor; sem ele na revista, heresia de gravidade 2 na primeira vez); Artefatos da Realeza como posse; estado Doente e traço Viciado (ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).
- **Escravidão:** 5 fontes; Mercado de Escravos com preço 200 + 4 × a melhor habilidade de Trabalho; Obediência com fórmula; fuga abaixo de 30 de Obediência; rebelião com Felicidade e Controle abaixo de 10 por 4 dias; abolição liberta tudo em 1 estação; 3 caminhos do estilo Escravo (ver [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]).
- **Revisão V1:** a pesca voltou (Pesqueiro); animais são itens (Cavalo = montaria e carga); um inventário por cidade, com pilhas; Mosquete (não Musket); Fome dos Controlados cai 100 em 36 horas; Templo com a tabela geral de vagas (2 nas capitais); crime de Sedição; os 13 documentos antigos estão no formato padrão.
- **Produção (IP1):** uma receita fixa por produto; cada item numa só construção (Aço Mágico só na Forja Arcana); escada de níveis 1 a 5 (Tier 1, 2, 2, 3, 4 e Rank 1 a 5) mais a trava da era (Rank até 3, 4, 4, 5); Rank = menor entre material, nível e 1 + habilidade ÷ 25; **1 NPC real = 1 trabalhador e 1 consumidor**; horas por unidade de 0,5 a 8 por Tier (armas 8, 12 e 24); fila de ordens do jogador (Fazer N, Manter estoque, Repetir); no máximo 3 insumos, 4 passos e nenhum item órfão.
- **Itens (IP2):** nomes de época, sem embalagem e no singular (Grão, Tora, Minério de Ferro, Carne Crua, Ovo); nomes de fantasia só no futuro (ideia do autor); **só a Pedra de Mana**, bruta e sem refino (ideia do autor); Pólvora, Adubo, Bloco de Pedra, Tecido de Algodão e de Lã; escada de materiais (de Madeira, Composto, Arcano; de Couro, Cravejado, Arcano); Arcabuz, Fuzil Raiado, Cetro, Flecha, Virote, Cartucho, Bombarda e Canhão; armadura **Leve de couro** (Costureiro), **Média de malha** e **Pesada de placas** (Ferreiro); roupas em conjuntos por papel social; Vinho, Peixe Defumado, Roupa Comum e Roupa Fina; **consumo pelo Tier** (Tier 3 = Luxo, Tier 4 = Exclusivo); cerca de 180 itens.
- **Receitas (IP3):** lote 4/2/1/1 pelo Tier; 8, 12 ou 24 h pelo degrau; armadura faz as 3 peças juntas; moldes no Ferreiro, Carpinteiro e Armeiro; munição em lote (20 Flechas); Pólvora = Carvão + Dejetos Animais; Aço Mágico = Barra de Aço + Pedra de Mana; **a era trava só equipamento**, os bens de luxo existem desde a Era 1; Operário e Engenheiro no Construtor; cadeia máxima de 4 passos e nenhum órfão.
- **Economia (V2):**
  - **dinheiro:** moeda com 1 casa decimal, guardada em décimos inteiros; preço = (insumos + horas × salário) ÷ lote × 1,2;
  - **salário por hora:** 0,75 Braçal, 1,0 Manufaturado, 1,5 Especialista; soldo 8 por Esquadrão por dia;
  - **preços:** Pão 1,7; Barra de Ferro 2,1; Espada Curta de Ferro 13,9;
  - **mercado:** Mercado Local de × 0,5 a × 2,0 pelo estoque (alvo de 5 dias), andando no máximo 10% por dia;
  - **consumo:** cesta mínima por classe e regra do gasto de 80%;
  - **leis:** imposto de 10, 20 ou 30%; cunhagem de 1 Barra de Ouro em 25 moedas;
  - **obras:** 3 portes de construção;
  - **rotina:** Civil trabalha das 7h às 15h; tick de 20 por segundo;
  - **fatia vertical:** vila de 15 NPCs.
- **Pendências (V3):**
  - **leis iniciais:** numa tabela única na Facção; **só a Escravista permite narcóticos** e é a única produtora (ideia do autor);
  - **crime:** multa = 25 × gravidade²; Caçadores a partir de 500 de Recompensa;
  - **narradores:** 4 (Cronista, Guerra, Terra, Corte), com intensidade de 50% a 150%;
  - **fatia vertical:** 12 eventos;
  - **interface:** fonte Noto Sans;
  - **PC mínimo:** 4 núcleos, 8 GB, OpenGL 3.3 e 2 GB de vídeo;
  - **conteúdo:** **moldes de conteúdo** prontos e listas nas rodadas C1 a C3 (ideia do autor).
- **Mundo:** 48 Mapas Locais (8 × 6), 6 regiões de 8 mapas; partida no Ano 412; nomes significativos até a história ser escrita (ver [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- **Git:** o repositório é `FelipeSKT/Projeto-Jogo`. Cada rodada vira um pull request para o autor aceitar.

## 6. Cuidados técnicos (para quem edita)

- **Ferramentas do PC:** g++ 15.2, CMake, Ninja, Git e Python 3.11. **Não há** `gh` (GitHub CLI) nem Visual Studio. O Python precisa ser chamado com `-I` ao ler arquivos do vault.
- **Edição em lote:** usar um script Python que carrega cada arquivo, troca trechos exatos com **verificação de que o trecho existe uma única vez**, e só então grava (com modo de teste "dry-run" antes). Preservar as quebras de linha (CRLF ou LF) e o BOM de cada arquivo. Backup da pasta antes de edições grandes (a pasta é um repositório Git, mas o backup extra é barato).
- **Conferir links:** o vault não deve ter `[[link]]` quebrado. Em tabelas, o separador do alias precisa ser escapado: `[[Pasta/Descrição\|Texto]]`.
- **Console do Windows:** a saída do PowerShell mostra acentos corrompidos; é só na tela, os arquivos estão em UTF-8.
- **Git:** o primeiro commit desta sessão falhou por causa da mensagem em *here-string*; usar `git commit -F arquivo`. Rodar `git fetch` antes de editar, porque outras sessões podem ter mexido no repositório. Não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json` (o Obsidian os altera sozinho). O `.gitignore` já ignora o workspace do Obsidian.
- **Sessões na nuvem (claude.ai/code):** o `Respostas.txt` chega como anexo na conversa; trabalhar na branch indicada pela sessão. O assistente só cria o pull request se o autor pedir.
- **Pull request (no PC):** sem o `gh`, o assistente envia a branch e o autor abre o PR pelo link `https://github.com/FelipeSKT/Projeto-Jogo/pull/new/NOME-DA-BRANCH`. O navegador interno não está logado no GitHub e o assistente nunca digita senhas. A extensão Claude in Chrome pode permitir criar o PR, se estiver conectada.
- **Nomes de itens:** a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] manda sobre os canvases.
