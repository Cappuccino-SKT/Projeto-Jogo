> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a rodada IP1 (Padronização da produção) aplicada e a IP2 (Nomes e lista final dos itens) enviada.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** todos os sistemas P0 e P1 estão escritos, e a **revisão completa** está em andamento (levantamento e plano na [[Revisão Completa]]). A V1 (faxina) foi mesclada no `main` (PR #9). Em 2026-10-17 foi aplicada a **IP1** (padronização da produção: receita padrão, família por construção, escada de níveis, Rank do produto, escala, tempo, ordens e limites; ver [[Sistema de Produção/Descrição|Produção]]), na branch `docs/ip1-producao`. A **rodada IP2 (Nomes e lista final dos itens) foi enviada e aguarda respostas** (seção 4). **Ao retomar, rodar `git fetch` e conferir se o pull request da `docs/ip1-producao` foi mesclado no `main`.**
3. **Próximo passo:** pedir o `Respostas.txt` da IP2 (conferir se é desta rodada), reescrever a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] com os nomes finais, trocar os nomes em todos os documentos e abrir a **IP3** (insumos de cada produto e a nova Funções e Construções).
4. Depois: V2 (economia numérica), V3 (pendências), V4 e V5 (propostas), como no plano da [[Revisão Completa]].
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
| Revisão | **Itens e Produção: IP2 (nomes), IP3 (insumos)** | **IP2 enviada, aguardando respostas** |
| Revisão | V2 (economia numérica), V3 (pendências), V4 e V5 (propostas) | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, modo observador, eventos de dinastia. As ordens de produção em lote foram decididas na IP1-7 ([[Sistema de Produção/Descrição|Produção]]). A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: IP2, Nomes e lista final dos itens

**Perguntas enviadas em 2026-10-17, aguardando respostas.** A IP1 decidiu a forma de produzir. Esta rodada decide **como cada item se chama e quais itens existem**; os insumos de cada um ficam para a IP3, e os preços para a V2.

Responder no `Respostas.txt` pelos códigos (IP2-1, IP2-2...). "Seguir sugestão" aceita a proposta. Nas tabelas, dá para aceitar e trocar só alguns nomes, por exemplo: "Seguir sugestão, mas Lenha em vez de Tora".

**Já decidido (base das perguntas):**

- **Tom do mundo:** baixa magia e tecnologia em transição para a **pólvora**; sombrio e político, perto de Mount & Blade. Só humanos, em 6 culturas. Os nomes próprios (lugares, pessoas, deuses) ficam para quando a história for escrita.
- A **cultura muda só a aparência** do item (troca de paleta agora, estilo próprio no futuro), nunca o nome nem os atributos ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]).
- A lista de itens manda sobre os canvases. O **ID interno** (`item.mosquete_aco`) não muda quando o nome muda.
- **IP1:**
  - uma família de produtos por construção;
  - Rank limitado pelo material (Ferro 3, Aço 4, Aço Mágico 5), pelo nível e pela habilidade;
  - no máximo 3 insumos por receita;
  - nenhum item órfão.
- **Animação:** 8 tipos de arma (Espada, Lança, Machado, Clava, Arco, Besta, Mosquete e Cajado), 4 quadros de ataque cada.
- **Sem mudança nesta rodada:** os animais vivos (Galinha, Porco, Ovelha, Vaca, Cavalo), os Livros, a Relíquia, o Grimório, os Grilhões, a Sela e os Dejetos Animais (nome já unificado antes).

**IP2-1. A regra geral dos nomes.**

- *Contexto:* hoje há nomes de épocas diferentes ("Terno", "Camiseta Moderna", "Capuz Ninja", "Rifle", "Cajado Moderno") e nomes montados de jeitos diferentes ("Fardo de Grão", "Ferro Bruto", "Barra de Ferro").
- *Pergunta:* que regra todo nome de item segue?
- *Opções:*
  - (a) **nomes descritivos em português de época**, mais ou menos dos séculos XV a XVII (quando a pólvora chega). São claros para o jogador e fáceis de traduzir;
  - (b) **nomes inventados de fantasia** ("Lâmina de Korr"): dão identidade, mas o jogador não sabe o que é o item sem ler a descrição, e eles dependem da história, que ainda não existe;
  - (c) **um nome por cultura** (o "Mosquete" da Tecnocrata e o "Trovão" da Guerreira): muito sabor, mas multiplica a lista de textos por 6 e confunde o comércio.
- *Sugestão:* (a), com 5 regras:
  1. **Época:** só palavras que caberiam num mundo de espadas e primeiras armas de fogo. Nada de "moderno", "terno", "camiseta", "ninja", "social" ou "rifle".
  2. **Padrão "objeto + de + material":** "Espada Curta de Aço". O material só aparece quando existe mais de uma versão do objeto.
  3. **Sem embalagem no nome:** o item é a coisa ("Grão"), não o saco (IP2-2).
  4. **Singular**, e a interface mostra "Pão × 12". Isso evita regras de plural no código e na tradução. A exceção são as coisas que só existem como monte (Ossos, Especiarias, Legumes, Joias).
  5. **Nomes de lugar e de cultura** só nos Artefatos lendários, e depois da história.
- *Impacto:*
  - Itens (a lista inteira);
  - Interface (formato "Nome × quantidade");
  - Pipeline (os nomes ficam num arquivo de textos, pronto para outros idiomas).

**IP2-2. Matérias-primas brutas: nomes sem embalagem.**

- *Contexto:* "Fardo de Grão", "Saco de Vegetal", "Balde de Leite". A IP1-5 decidiu que 1 unidade é o que 1 NPC real usa, e ele representa cerca de 100 pessoas. O "saco" ou o "balde" não diz a quantidade de verdade e ainda alonga os nomes na interface (o autor joga em 1600 × 900).
- *Opções:*
  - (a) tirar a embalagem e usar o nome natural;
  - (b) manter como a V1-6 deixou.
- *Sugestão:* (a):

| Antes | Depois | Por quê |
|---|---|---|
| Ferro Bruto, Cobre Bruto, Ouro Bruto | **Minério de Ferro**, **Minério de Cobre**, **Minério de Ouro** | Nome natural do que sai da mina |
| Pedra Bruta, Argila Bruta | **Pedra**, **Argila** | "Bruta" fica óbvio |
| Madeira Bruta | **Tora** | "Madeira" sozinha confunde com a Tábua; a lenha do inverno passa a ser Tora |
| Fardo de Grão | **Grão** | |
| Saco de Vegetal | **Legumes** | |
| Saco de Frutas | **Fruta** | |
| Fardo de Folha de Fumo | **Folha de Fumo** | |
| Saco de Flor Espiritual | **Flor Espiritual** | |
| Fardo de Especiaria | **Especiarias** | |
| Fardo de Algodão | **Algodão** | |
| Pedaço de Carne | **Carne Crua** | Diferencia dos pratos de carne |
| Couro Bruto | **Couro Cru** | Par com o Couro Refinado |
| Balde de Leite, Fardo de Lã, Saco de Ovos | **Leite**, **Lã**, **Ovo** | |
| Carvão, Água, Ossos, Peixe, Dejetos Animais | Sem mudança | |

  - **Regra extra:** o recurso do mapa e o item têm o mesmo nome sempre que possível (recurso "Grão" → item "Grão"). A exceção são os minérios: recurso "Ferro" → item "Minério de Ferro".
- *Conflito:* desfaz parte da V1-6, que tinha padronizado "Saco de Ovos", "Fardo de Lã" e "Balde de Leite". Se o autor preferir as embalagens, a opção (b) mantém tudo e muda só os minérios.
- *Impacto:*
  - [[Sistema de Itens/Descrição|Itens]] e [[Sistema de Recursos/Recursos Existentes|Recursos Existentes]];
  - Clima e Classe Social (a lenha vira Tora);
  - Cura e Efeitos de Itens (o Remédio usa Especiarias).

**IP2-3. A família da mana (Pedra Mágica × Pedra de Mana).**

- *Contexto:* hoje há nomes diferentes para coisas parecidas:
  - o recurso do mapa "Pedra Mágica";
  - o item bruto "Pedra de Mana Bruta", do Posto de Coleta;
  - a "Pedra de Mana" refinada, da Forja Arcana. Ela recupera Mana, é gasta no encantamento e faz parte do kit dos Magos;
  - a "Joia Mágica de Pedra Mágica" e o "Anel Mágico de Pedra Mágica", do Artesão;
  - a "Pedra Mágica" do loot das masmorras.
- *Pergunta:* quantos itens existem, e com que nomes?
- *Opções:*
  - (a) **tudo com "Mana"**, a palavra que o jogador já vê na barra do Mago;
  - (b) **"Cristal Arcano"** como nome base: mais fantasia e combina com Forja e Torre Arcana, mas muda cerca de 10 documentos e a palavra Mana fica só na barra;
  - (c) manter como está.
- *Sugestão:* (a):

| Coisa | Nome | Tier ou Rank | Onde |
|---|---|---|---|
| Recurso do mapa | **Pedra de Mana** | — | Mapas da região Arcana, sobretudo |
| Item bruto | **Pedra de Mana Bruta** | Tier 1 | Posto de Coleta |
| Item refinado | **Pedra de Mana** | Tier 2 | Forja Arcana (Mago de Ofício) |
| Joia do Mago | **Joia de Mana** (antes Joia Mágica) | Rank 1-5 | Artesão |
| Anel | **Anel de Mana** (antes Anel Mágico) | Rank 1-5 | Artesão |
| Loot das masmorras | Pedra de Mana (refinada) | Tier 2 | Masmorras |

  O nome "Pedra Mágica" some do jogo.
- *Impacto:* Itens, Recursos Existentes, Mundo (destaque Arcano), Magia, Efeitos de Itens, Objetivo (loot).

**IP2-4. Materiais intermediários.**

- *Contexto:* os intermediários são o que uma construção faz para outra usar. Hoje: Barra de Ferro (Tier 1), Barra de Cobre (2), Barra de Aço (2), Barra de Ouro (3), Barra de Aço Mágico (4), Tijolos de Pedra (1), Tecido (1), Couro Refinado (1), Fertilizante (1), Pó Explosivo (2) e Tábua (1, ainda a cadastrar). Há 4 problemas:
  - o "tijolo" é de argila; o que sai da pedra é bloco;
  - "Pó Explosivo" é a **pólvora**, a palavra que define a época do mundo;
  - "Fertilizante" é palavra moderna;
  - só existe um "Tecido", e a **Lã não tem nenhum uso**, o que a deixaria órfã pela IP1-8.
- *Sugestão:*

| Antes | Depois | Tier | Construção |
|---|---|---|---|
| Barra de Ferro | Barra de Ferro | 1 | Metalurgia |
| Barra de Cobre | Barra de Cobre (**Tier 1**: o cobre é mais fácil de trabalhar que o ferro) | 1 | Metalurgia |
| Barra de Aço | Barra de Aço | 2 | Metalurgia |
| Barra de Ouro | Barra de Ouro | 3 | Metalurgia |
| Barra de Aço Mágico | Barra de Aço Mágico | 4 | Forja Arcana |
| Tijolos de Pedra | **Bloco de Pedra** | 1 | Pedreira |
| — | **Tábua** (sai da lista "a cadastrar") | 1 | Carpinteiro |
| Tecido | **Tecido de Algodão** e **Tecido de Lã** | 1 | Costureiro |
| Couro Refinado | Couro Refinado | 1 | Costureiro |
| Fertilizante | **Adubo** | 1 | Misturador |
| Pó Explosivo | **Pólvora** | 2 | Misturador |

  - A Pedra de Mana refinada está na IP2-3.
  - **Regra do item órfão (IP1-8), esclarecida:** conta como uso ser insumo de receita, material de obra (custos da V2) ou consumo de classe. O Bloco de Pedra e a Tábua são usados principalmente nas obras.
- *Impacto:*
  - Itens;
  - Construção e Guerra (as obras e a Muralha de Pedra usam Bloco de Pedra e Tábua);
  - Cura (o Curativo usa Tecido de Algodão);
  - Tecnologia (a Pólvora entra na Era 2).

**IP2-5. A escada de materiais fora do metal.**

- *Contexto:* a IP1-4 limita o Rank pelo material (Ferro 3, Aço 4, Aço Mágico 5). Isso funciona para o metal, mas há dois problemas:
  - os arcos são "de Aço" e "de Aço Mágico". Um arco de aço não existia na época, e o Carpinteiro não trabalha metal;
  - o couro e o tecido não têm escada nenhuma.
- *Pergunta:* como os itens de madeira e de couro chegam aos Ranks 4 e 5?
- *Opções:*
  - (a) **três degraus por família:** o degrau 4 vem de um reforço que existia na época, e o 5 vem da Pedra de Mana, como o Aço Mágico no metal;
  - (b) madeira e couro param no Rank 3. É simples, mas arqueiros e tropas leves ficam fracos no fim do jogo;
  - (c) manter o metal em tudo (arco de aço).
- *Sugestão:* (a):

| Família | Rank até 3 (Era 1) | Rank até 4 (Era 2) | Rank até 5 (Era 4) |
|---|---|---|---|
| Metal | Ferro | Aço | Aço Mágico |
| Madeira (arcos, escudo de madeira, cajado de madeira) | **de Madeira** (Tábua) | **Composto** (Tábua + Ossos + Couro Refinado: o arco composto de madeira, chifre e tendão existia) | **Arcano** (Tábua + Pedra de Mana) |
| Couro (armadura Leve, IP2-7) | **de Couro** (Couro Refinado) | **Cravejado** (Couro Refinado + Barra de Aço) | **Arcano** (Couro Refinado + Pedra de Mana) |
| Tecido (roupas) | O teto vem da própria peça (IP2-8) | | |

  - Cada item é feito na construção da sua família: o Carpinteiro faz o Arco Composto e o Arco Arcano, e o Costureiro faz o Cravejado e o Arcano de couro. A Pedra de Mana entra só como insumo; fazê-la continua sendo da Forja Arcana.
  - As **bestas** continuam de metal (a besta de aço existia), e o Carpinteiro as monta com Barra de Aço como insumo.
  - *Exemplo:* a Religiosa (Ovelhas) tem Ossos e couro e faz Arcos Compostos; para o Arco Arcano, ela precisa comprar Pedra de Mana da Arcana, sua rival.
- *Impacto:*
  - Itens, Produção e a IP3 (insumos);
  - Magia (mais demanda de Pedra de Mana);
  - Comércio (a Arcana ganha um produto de exportação).

**IP2-6. Armas: a lista final.**

- *Contexto:* hoje há 51 armas:
  - corpo a corpo: 4 tipos × 2 tamanhos × 3 metais = 24;
  - arcos 9; bestas 7; armas de fogo 6; cajados 5.

  Fora isso, 4 munições e 2 canhões. Cada arma é 1 desenho com troca de paleta pelo material. Os problemas:
  - o "Mosquete de Cobre" não existiu (os primeiros canos eram de ferro ou bronze);
  - "Rifle" é palavra moderna;
  - o "Cajado Moderno" de Aço Mágico (1-4) é pior em tudo que o Cajado Arcano de Aço Mágico (1-5);
  - o Arco Recurvo fica entre o Curto e o Longo sem diferença clara;
  - a Besta Pesada de Aço Mágico é 3-5, fora do padrão 1-5.
- *Sugestão:*

| Tipo | Lista final (Rank) | Mudança |
|---|---|---|
| Espada, Lança, Machado, Clava | Curta e Longa (Machado: Curto e Longo), de Ferro (1-3), Aço (1-4) e Aço Mágico (1-5) | Sem mudança: "Curta" e "Longa" dizem o alcance, que é regra do Combate |
| Arco | Arco Curto e Arco Longo, de Madeira (1-3), Composto (1-4) e Arcano (1-5) | Sai o Recurvo; materiais da IP2-5 (de 9 para 6) |
| Besta | Besta Leve de Madeira (1-3), Aço (1-4) e Aço Mágico (1-5); Besta Pesada e Besta de Repetição de Aço (1-4) e Aço Mágico (1-5) | A Pesada de Aço Mágico passa a 1-5 |
| Arma de fogo | **Arcabuz** de Ferro (1-3, Era 2); Mosquete de Aço (1-4, Era 3) e de Aço Mágico (1-5, Era 4); Carabina de Aço (1-4) e de Aço Mágico (1-5); **Fuzil Raiado** de Aço Mágico (1-5, Era 4) | O Arcabuz substitui o Mosquete de Cobre; o Fuzil Raiado substitui o Rifle. O cobre continua como insumo das armas de fogo, nas peças de latão (IP3) |
| Cajado | Cajado Simples de Madeira (1-2, Carpinteiro) e de Cobre (1-3, Artesão); Cajado Arcano de Aço (1-4) e de Aço Mágico (1-5); **Cetro** de Aço Mágico (1-4) | O Cajado Moderno vira o Cetro: **[Proposta]** um cajado de uma mão, que deixa usar Escudo, com dano 25% menor |
| Munição | **Flecha** (1-3), **Virote** (1-3, da besta), **Cartucho** (1-3, bala com a pólvora em papel, que já existia no século XVI), **Bala de Canhão** (1-3) | Antes: "Munição de Arco", "de Besta", "de Arma de Fogo" e "de Canhão de Guerra" |
| Artilharia (Armamento de Exército) | **Bombarda** de Ferro (1-3, Era 2); **Canhão** de Aço (1-4, Era 3); Canhão de Aço Mágico (1-5, Era 4) | Antes: "Canhão de Guerra" (Rank 3) e "Canhão de Guerra Moderno" (Rank 5); agora segue a regra do material |

  - **Total:** 48 armas (antes 51), mais 4 munições e 3 peças de artilharia. Nenhuma animação nova: o Arcabuz, a Carabina e o Fuzil usam a do Mosquete, e o Cetro usa a do Cajado.
- *Impacto:*
  - Itens;
  - Batalha (kits: os Atiradores usam Arcabuz ou Mosquete; a Artilharia, Bombarda ou Canhão);
  - Combate (a linha "Besta, Mosquete" da tabela de vantagem vale para todas as armas de fogo);
  - Tecnologia (Arcabuz e Bombarda na Era 2);
  - Pipeline.

**IP2-7. Armaduras e a peça Média.**

- *Contexto:*
  - O Combate tem 3 pesos (Leve, Média e Pesada), com vantagens por arma. A Média e a Pesada deixam o NPC mais lento.
  - A IP1-2 pôs as Leves e as Pesadas no Ferreiro (metal) e a Média no Costureiro (couro). Só que o couro é mais leve que o metal: uma "Leve" de ferro e uma "Média" de couro ficam ao contrário do que o jogador espera.
  - O canvas original do autor já tinha a Leve no Costureiro (I-21).
- *Opções:*
  - (a) **pela ordem histórica:** Leve = couro (Costureiro); Média = cota de malha (Ferreiro); Pesada = placas (Ferreiro);
  - (b) manter como a IP1-2 deixou.
- *Sugestão:* (a). Cada armadura tem 3 peças (Capacete, Peitoral, Perneiras):

| Peso | Capacete | Peitoral | Perneiras | Materiais (Rank) | Construção |
|---|---|---|---|---|---|
| Leve | Capuz | Gibão | Perneiras | de Couro (1-3), Cravejado (1-4), Arcano (1-5) (IP2-5) | Costureiro |
| Média | Coifa de Malha | Cota de Malha | Perneiras de Malha | de Ferro (1-3), Aço (1-4), Aço Mágico (1-5) | Ferreiro |
| Pesada | Elmo | Couraça | Grevas | de Ferro (1-3), Aço (1-4), Aço Mágico (1-5) | Ferreiro |

  - *Exemplos de nome:* "Gibão Cravejado", "Cota de Malha de Aço", "Elmo de Aço Mágico".
  - **Total:** 27 peças (antes 18, mais 3 Médias a cadastrar = 21).
  - **Kits da Batalha:**
    - Infantaria Leve, Arqueiros e Atiradores usam a Leve;
    - Lanceiros, Besteiros e Cavalaria usam a Média;
    - Infantaria Pesada usa a Pesada.

    Isso já combina com a tabela de kits da [[Sistema de Batalha/Descrição|Batalha]].
- *Conflito:* muda a IP1-2 em um ponto: o Costureiro passa a fazer a armadura Leve (em vez da Média), e o Ferreiro faz a Média e a Pesada. Se o autor preferir (b), fica como a IP1-2.
- *Impacto:*
  - Itens e Funções e Construções;
  - Batalha (kits); o Combate não muda de regra;
  - Pipeline (9 desenhos de peça, com cor pelo material).

**IP2-8. Roupas, anéis e escudos.**

- *Contexto:* as roupas não militares (Chapéu, Camisa e Calças) dão pouca Defesa e algum bônus de habilidade. Há 17 peças, e várias têm nome moderno ("Capuz Ninja", "Camiseta Moderna", "Terno", "Calça Social").
- *Sugestão:* organizar as roupas em **conjuntos pelo papel social**, sem mudar a quantidade (17 peças):

| Conjunto | Chapéu | Camisa | Calças | Material | Rank | Antes |
|---|---|---|---|---|---|---|
| Pobre | — | Trapos | Calças Rasgadas | Tecido de Lã | 1-2 | Sem mudança |
| Trabalhador | Chapéu de Palha (de Grão) | Camisa de Couro | Calça de Couro | Couro Refinado | 1-2 | Camiseta e Calça Robusta |
| Mineiro | Chapéu de Mineiro (de Ferro) | — | — | Barra de Ferro | 1-2 | Sem mudança |
| Artífice | — | Camisa de Artífice | Calça de Artífice | Tecido de Algodão | 1-3 | Camiseta e Calça Moderna |
| Furtivo | Capuz Furtivo | Túnica Furtiva | Calça Furtiva | Tecido de Lã | 1-3 | Capuz, Camisa e Calça Ninja |
| Mercador | — | Casaca de Mercador | Calça de Mercador | Tecido de Algodão | 1-4 | Terno, Calça Social |
| Nobre | Chapéu Nobre | Túnica Nobre | Calça Nobre | Tecido de Algodão | 1-4 | Camiseta Nobre |
| Realeza | Coroa (de Ouro, Artesão) | — | — | Barra de Ouro | 1-5 | Sem mudança |

  - **[Proposta]** O conjunto Furtivo dá bônus de **Destreza** (ocultação), útil ao Ladrão; o Mercador e o Nobre dão bônus de **Carisma**. Os valores ficam para a V2.
  - **Anéis:** Anel de Cobre (1-3), Anel de Ouro (1-5) e Anel de Mana (1-5; IP2-3), todos do Artesão.
  - **Escudos:** Escudo de Madeira (1-3; antes "Escudo Primitivo", 1-2) e Escudo de Ferro (1-3), de Aço (1-4) e de Aço Mágico (1-5).
- *Impacto:* Itens; Costureiro (roupas de tecido e couro); Artesão (Coroa e anéis); Crime (roupa furtiva).

**IP2-9. Comida e bebida: a lista final.**

- *Contexto:* hoje há 11 itens:
  - Tier 1: Pão, Sopa, Legumes Cozidos e Ração Militar;
  - Tier 2: Torta de Fruta, Linguiça, Queijo e Cerveja;
  - Tier 4: Licor, Carne Envelhecida e Doce de Baunilha.

  Os problemas:
  - Sopa e Legumes Cozidos são quase a mesma coisa;
  - não há prato de Peixe, embora a pesca tenha voltado;
  - a Religiosa tem um "vilarejo de vinhedos", mas o Vinho não existe;
  - a baunilha não é recurso;
  - não há nada de Tier 3.
- *Sugestão:* 13 itens. A coluna "Feito de" é só uma ideia, para a IP3 confirmar:

| Tier | Item | Construção | Feito de (ideia) | Antes |
|---|---|---|---|---|
| 1 | Pão | Cozinheiro | Grão + Água | Sem mudança |
| 1 | Sopa | Cozinheiro | Legumes + Água | Absorve os Legumes Cozidos |
| 1 | **Peixe Defumado** | Cozinheiro | Peixe + Tora | Novo |
| 1 | Ração Militar | Cozinheiro | Grão + Carne Crua | Sem mudança |
| 2 | Torta de Fruta | Cozinheiro | Fruta + Grão + Ovo | Sem mudança |
| 2 | Linguiça | Cozinheiro | Carne Crua + Especiarias | Sem mudança |
| 2 | Queijo | Cozinheiro | Leite | Sem mudança |
| 2 | Cerveja | Destilaria | Grão + Água | Sem mudança |
| 3 | **Vinho** | Destilaria | Fruta | Novo (vinhedos da Religiosa) |
| 3 | **Assado Temperado** | Cozinheiro | Carne Crua + Especiarias + Tora | Novo |
| 4 | Licor | Destilaria | Vinho + Especiarias | Sem mudança |
| 4 | **Presunto Curado** | Cozinheiro | Carne Crua + Especiarias, com mais horas de cura | Antes Carne Envelhecida |
| 4 | **Doce de Especiarias** | Cozinheiro | Fruta + Especiarias + Leite | Antes Doce de Baunilha |

  - Os subgrupos "Básico", "Refinado" e "de Luxo" deixam de existir: vale o **Tier** (IP2-11).
  - Com isso, todo produto animal tem uso: Ovo (Torta), Leite (Queijo, Doce), Lã (Tecido de Lã), Ossos (Arco Composto), Dejetos (Adubo).
- *Impacto:*
  - Itens;
  - Cozinheiro e Destilaria (IP3);
  - Efeitos de Itens (Saciedade; **[Proposta]** Vinho +3 de Felicidade, entre a Cerveja (+2) e o Licor);
  - Religião (festivais com Vinho), Mundo (vinhedos), Recursos (o Peixe ganha uso).

**IP2-10. Outros bens de consumo: mobília, roupa, luxo, narcóticos e remédios.**

- *Contexto:* hoje há:
  - Mobília Básica: Peças de Argila (1) e Móveis de Madeira (2);
  - Mobília Refinada: Móveis Refinados (3) e Ornamento (4);
  - Luxo: Perfumes (3) e Joias (4);
  - Narcóticos: Fumo (2) e Pó Espiritual (3);
  - Medicamentos: Curativo (1) e Remédio (3).

  Os NPCs comuns **não consomem roupa**: as roupas da IP2-8 são equipamento com Rank, só para NPCs equipados. Por isso o Tecido e o Costureiro quase não têm mercado nas cidades.
- *Sugestão:*

| Grupo | Item (Tier) | Construção | Antes |
|---|---|---|---|
| Mobília | **Cerâmica** (1) | Pedreira | Peças de Argila |
| Mobília | Móveis de Madeira (2) | Carpinteiro | Sem mudança |
| Mobília | Móveis Refinados (3) | Carpinteiro | Sem mudança |
| Mobília | Ornamento (4) | Artesão | Sem mudança |
| **Vestuário** | **Roupa Comum** (1) | Costureiro | Novo |
| **Vestuário** | **Roupa Fina** (3) | Costureiro | Novo |
| Essência | **Perfume** (3) | Destilaria | Perfumes (singular, IP2-1) |
| Joia | Joias (4) | Artesão | Sem mudança |
| Narcótico | Fumo (2), Pó Espiritual (3) | Misturador | Sem mudança |
| Medicamento | Curativo (1), Remédio (3) | Boticário | Sem mudança |

  - A **Roupa Comum** e a **Roupa Fina** são bens de consumo: têm Tier e não têm Rank, e só precisam de ícone, sem arte de boneco. A cidade as gasta aos poucos, como gasta a Cerâmica. Elas dão ao Costureiro, ao Algodão e à Lã uma demanda de verdade.
- *Impacto:* Itens; Classe Social (IP2-11); Costureiro (IP3); Pipeline (2 ícones a mais).

**IP2-11. Luxo, Exclusivo e o que cada classe consome (I-13).**

- *Contexto:*
  - Na [[Sistema de Classe Social/Descrição|Classe Social]], cada classe consome um tipo a mais que a de baixo:
    - o Escravo consome Comida;
    - o Civil, também Materiais Básicos;
    - o Rico, também Luxo;
    - a Nobreza, também Exclusivos;
    - a Realeza, também Artefatos.
  - O mapeamento para os itens é provisório e se contradiz: o Licor é "Alimento de Luxo" e Bem Exclusivo ao mesmo tempo; a Bebida está dentro da Comida.
- *Opções:*
  - (a) **regra pelo Tier**: um item novo já cai no grupo certo, sem mexer na tabela;
  - (b) uma lista fixa de itens por classe: mais controle, mas cada item novo exige mexer na tabela.
- *Sugestão:* (a):

| Consumo | O que conta | Quem consome |
|---|---|---|
| Comida | Alimentos e bebidas. O Escravo come Tier 1; o Civil, Tier 1 e 2; o Rico e acima, Tier 2 ou mais (nobre não come Ração) | Todas as classes |
| Materiais Básicos | Mobília e Vestuário de Tier 1 e 2 (Cerâmica, Móveis de Madeira, Roupa Comum), mais a Tora no inverno | Civil e acima (a Tora, todas as classes) |
| Bens de Luxo | Tudo de Tier 3 que não é remédio (Móveis Refinados, Roupa Fina, Perfume, Vinho, Assado Temperado), mais os Narcóticos | Rico e acima |
| Bens Exclusivos | Tudo de Tier 4 (Licor, Presunto Curado, Doce de Especiarias, Joias, Ornamento) | Nobreza e Realeza |
| Artefatos | Livro, Joia de Mana e Relíquia, como posse (já decidido) | Realeza |

  - Resolve a I-13: o Licor é de Tier 4, então é Exclusivo, e o grupo "Alimento de Luxo" deixa de existir.
  - Os **remédios** ficam fora do consumo de classe: a cidade os gasta só nas epidemias (já decidido na Cura).
  - Onde a lei proíbe os Narcóticos (na Religiosa), o Luxo vem só dos outros itens de Tier 3.
  - **[Proposta]** Quanto da Comida pode ser Bebida (por exemplo, até 25%) e as quantidades de tudo ficam para a V2.
- *Impacto:*
  - Classe Social (a tabela de mapeamento);
  - Itens (os grupos passam a ser por Tier);
  - Felicidade (o fator de consumo atendido);
  - V2 (as quantidades).

**Se tudo for aceito:** cerca de 180 itens, quase o mesmo número de hoje (cerca de 175). Saem nomes modernos e itens repetidos; entram Vinho, Peixe Defumado, Assado Temperado, Roupa Comum, Roupa Fina, Tecido de Lã e a armadura Média completa.

Depois das respostas:

- reescrever a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] com os nomes finais;
- trocar os nomes em todos os documentos;
- atualizar os Recursos Existentes, a Classe Social e os kits da Batalha;
- abrir a **IP3** (insumos de cada produto e a nova Funções e Construções).

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). O Ferreiro não lista Espada (provável erro; ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]).
- **Arte:** o autor desenha por enquanto (sem experiência em pixel art), no **Pixelorama** (grátis), com pacotes de terceiros provisórios na fatia vertical. Mapas no **Tiled** (explicar em linguagem simples). Sem verba para programas pagos. Projeto **sem prazo**: o limite da arte é o peso no jogo, não o tempo (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
- **Estações na arte:** só mudança de cor por enquanto; **toda arte extra de estação fica para o futuro** (neve, rios congelados, folhas, flores).
- **Viagem:** 4 h por Mapa Local a pé, multiplicada por terreno, infraestrutura, tamanho do grupo, montaria e carga; caravanas sempre reais no Mapa Mundial (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).
- **Grupo:** controlado = 4 + Renome ÷ 20; vinculados = 10 + Renome ÷ 5 + 3 por cidade; **sem retratos** nas conversas (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).
- **Linguagem:** o foco é **C++**; ferramentas em outras linguagens só como ajuda, nunca essenciais.
- **Guerra e Batalha:** Placar de Guerra (−100 a +100) paga a paz; cidade ocupada só muda de dono no tratado; exército de até 20 Esquadrões com General; **o mundo para durante a batalha**; a simulação é sempre por Esquadrão de 100 soldados, e o jogador escolhe o **tamanho visual** (20, 50 ou 100 bonecos por Esquadrão; até 4.000 soldados na tela, pedido do autor). A 7ª camada do Tiled é a Elevação. Na programação, a guerra por cálculo rápido vem antes do Mapa de Guerra (ver [[Sistema de Guerra/Descrição|Guerra]], [[Sistema de Batalha/Descrição|Batalha]]).
- **Diplomacia:** estado (Paz, Guerra, Trégua) + tratados que se somam + atitude (faixa da relação); Confiabilidade; vassalo tem leis próprias (ver [[Sistema de Diplomacia/Descrição|Diplomacia]]).
- **Religião:** uma fé por cultura, com Devoção (Alta na Religiosa, nenhuma na Tecnocrata); cada Sacerdote atende 20 NPCs; fator Religião de −5 a +10; 2 festivais por ano (4 na Religiosa); lei de Tolerância religiosa; heresia é um ato; assimilação de 1% a 4% por estação; Bênção e Anátema do Sumo Sacerdote (ver [[Sistema de Religião/Descrição|Religião e Fé]]).
- **Clima:** Fazenda 75%, 100%, 150% e 25% nas quatro estações (base temperada); clima do dia sorteado por região, com 60% de repetir; lenha no inverno (1 Madeira Bruta por NPC real a cada 4 dias, × bioma); vontade de guerra −15 no inverno; noite com visão −30%; arte só com paleta, tom e partículas (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Notícias:** 3 h por mapa (o mundo em 1 dia); Local, Regional e Mundial; precisão cai com a distância; a IA sabe tudo na hora (simplificação) (ver [[Sistema de Notícias/Descrição|Notícias]]).
- **Efeitos de itens:** só Controlados e Importantes usam itens com efeito individual; Livro +3 (até 60) e Livro Antigo +6 (até 80), sem se gastar; Livro de Atributos +2 × Rank, se gasta; **Livro Sagrado obrigatório** na Religiosa (ideia do autor; sem ele na revista, heresia de gravidade 2 na primeira vez); Artefatos da Realeza como posse; estado Doente e traço Viciado (ver [[Sistema de Itens/Efeitos de Itens|Efeitos de Itens]]).
- **Escravidão:** 5 fontes; Mercado de Escravos com preço 200 + 4 × a melhor habilidade de Trabalho; Obediência com fórmula; fuga abaixo de 30 de Obediência; rebelião com Felicidade e Controle abaixo de 10 por 4 dias; abolição liberta tudo em 1 estação; 3 caminhos do estilo Escravo (ver [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]).
- **Revisão V1:** a pesca voltou (Pesqueiro); animais são itens (Cavalo = montaria e carga); um inventário por cidade, com pilhas; Mosquete (não Musket); Fome dos Controlados cai 100 em 36 horas; Templo com a tabela geral de vagas (2 nas capitais); crime de Sedição; os 13 documentos antigos estão no formato padrão.
- **Produção (IP1):** uma receita fixa por produto; cada item numa só construção (Aço Mágico só na Forja Arcana); escada de níveis 1 a 5 (Tier 1, 2, 2, 3, 4 e Rank 1 a 5) mais a trava da era (Rank até 3, 4, 4, 5); Rank = menor entre material, nível e 1 + habilidade ÷ 25; **1 NPC real = 1 trabalhador e 1 consumidor**; horas por unidade de 0,5 a 8 por Tier (armas 8, 12 e 24); fila de ordens do jogador (Fazer N, Manter estoque, Repetir); no máximo 3 insumos, 4 passos e nenhum item órfão.
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
