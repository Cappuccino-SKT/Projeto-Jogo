> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado em 2026-10-17, com a P1-6 aplicada (o P1 está completo) e a revisão completa aberta (rodada V1 enviada).

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Onde paramos:** **todos os sistemas P0 e P1 estão escritos.** Em 2026-10-17 foi aplicada a P1-6 (Efeitos de Itens e Escravidão) e, a pedido do autor (D2), começou a **revisão completa** do projeto: o levantamento está na [[Revisão Completa]], e a **rodada V1 (Inconsistências e faxina dos documentos) foi enviada e aguarda respostas** (seção 4). Branches: `docs/p1-5-religiao-clima-noticias` (P1-5) e `docs/p1-6-itens-escravidao` (P1-6 e a revisão, criada a partir da P1-5). **Ao retomar, rodar `git fetch` e conferir o que já foi mesclado no `main`.** Antes disso: a P1-4 (Guerra, Batalha e Diplomacia; pull request #7) e a sessão de 2026-10-15 (P0-5 a P1-3, o `CLAUDE.md` e o padrão de perguntas).
3. **Próximo passo:** pedir o `Respostas.txt` da V1 (conferir se é desta rodada), aplicar as correções e a faxina dos 13 documentos antigos e abrir a **V2** (base numérica da economia).
4. Depois: V3 (pendências e listas), V4 e V5 (confirmação das propostas), como no plano da [[Revisão Completa]].
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
| Revisão | **Revisão completa (rodadas V1 a V5; ver [[Revisão Completa\|Revisão Completa]])** | **V1 enviada, aguardando respostas** |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |
| Futuro | **Títulos de Nobreza**: mesma estrutura para todas as culturas; requisitos, privilégios e obrigações por cultura, modeláveis por facção e alteráveis pelas leis (ideia registrada no [[Objetivo e Progressão/Descrição\|Objetivo e Progressão]]) | Ideia do autor, para uma versão futura |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível (resolvida no [[Sistema de Notícias/Descrição|Sistema de Notícias]]), especialização de cidades, ordens de produção em lote, modo observador, eventos de dinastia. A "guarnição e defesa" foi resolvida no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

## 4. Rodada pendente: V1, Inconsistências e faxina dos documentos

**Perguntas enviadas em 2026-10-17, aguardando respostas.** É a primeira das 5 rodadas da **revisão completa** pedida pelo autor (D2 da P1-6). O levantamento inteiro, com o estado de cada documento, as 21 inconsistências, as 12 partes abstratas, as pendências e as propostas, está na [[Revisão Completa]]. Esta rodada resolve as inconsistências que pedem escolha e a faxina dos documentos antigos. Responder no `Respostas.txt` pelos códigos (V1-1, V1-2...). "Seguir sugestão" aceita a proposta.

**Já decidido (base das perguntas):**

- A lista do [[Sistema de Itens/Descrição|Sistema de Itens]] **manda** sobre os canvases, até a pasta `data/` existir.
- Armaduras: 3 peças separadas, de peso Leve, Média ou Pesada; o Costureiro faz a Média.
- O **Peixe foi descartado** dos recursos.
- Tom do mundo: baixa magia, chegada da pólvora, sombrio e político; 4 eras de tecnologia.
- Arquitetura: **composição**, não herança; o inventário é um componente.

**V1-1. Faxina dos documentos antigos e correções óbvias.**

- *Contexto:* 13 documentos são de antes do padrão: Mapa, Cidade, NPC, IA, Inventário, Econômico, Itens, Produção, Recursos, Recursos Existentes, Construção, Classe Social e Tempo. Faltam neles a legenda, a parte técnica, a tabela de ligações e as perguntas. Além disso, há 9 inconsistências com correção óbvia: I-1, I-6, I-8, I-12, I-15, I-17, I-18, I-20 (I-19 já foi corrigida).
- *Pergunta:* posso padronizar os 13 documentos e aplicar as correções óbvias sem mudar nenhuma decisão?
- *Sugestão:* sim, numa rodada só, sem mudar nenhuma regra:
  - **Formato:** legenda no topo, "Descrição geral", as seções que já existem, "Parte técnica" (LOD, dados, save, fatia vertical), "Ligação com outros sistemas" e "Perguntas em aberto", como nos documentos novos.
  - **Texto do autor:** o conteúdo continua o mesmo; só corrijo a ortografia e reorganizo. O que foi decisão do autor continua sem marcação.
  - **Cidade vira o "documento de entrada" da cidade:** os atributos ficam, mais uma tabela "onde está cada regra" (Felicidade, População, Trabalho, Crime, Guerra, Religião).
  - **Correções óbvias:**
    - I-1: a IA aponta para a fórmula de dano do Combate.
    - I-6: a Caravana usa o componente de Inventário.
    - I-8: o Mapa usa a condição de vitória da Batalha.
    - I-12: "Nobreza" em todo lugar.
    - I-15: fecho as duas perguntas obsoletas do Comércio.
    - I-17: limpo as duas propostas que já foram feitas.
    - I-18: a armadura mista fica só no Combate.
    - I-20: crime de **Sedição** (gravidade 3), só nas facções cuja lei proíbe pedir mudança de lei.
- *Custo:* nenhum para o jogo; só deixa o vault mais fácil de ler e de transformar em ficha técnica.

**V1-2. Armas sem produtor e produtos que não existem.**

- *Contexto:* I-2 e I-3. O Ferreiro repete "Machado de Ferro" e não faz Espada. O Armeiro nível 4 faz "Rifle de Aço", que não existe na lista. Carabina de Aço Mágico, Besta de Repetição, Mosquete de Aço Mágico e os arcos e bestas de metal não têm quem os faça.
- *Sugestão:* seguir os mesmos metais e níveis das outras armas:

| Construção | Nível | Acrescenta ou corrige |
|---|---|---|
| Ferreiro | 1 | **Espada Curta e Espada Longa de Ferro** (no lugar do Machado repetido) |
| Ferreiro | 3 | Espada Curta e Longa de Aço |
| Ferreiro | 5 | Espada Curta e Longa de Aço Mágico |
| Armeiro | 4 | **Carabina de Aço** (no lugar do "Rifle de Aço") |
| Armeiro | 5 | Rifle, Carabina e Mosquete de Aço Mágico; Canhão de Guerra Moderno |
| Carpinteiro | 4 | Arcos de Aço (Curto, Recurvo e Longo); Besta Leve de Aço; Besta de Repetição de Aço |
| Carpinteiro | 5 | Arcos e Bestas de Aço Mágico, incluindo a Besta Pesada e a de Repetição |

  Os arcos e as bestas de metal usam Barra de Aço ou de Aço Mágico como insumo.
- *Impacto:* Funções e Construções, Itens. Nenhuma arte nova: as armas já estão na lista.

**V1-3. Animais como itens (o Cavalo).**

- *Contexto:* I-4. Galinha, Porco, Ovelha, Vaca e Cavalo são recursos e produtos do Pasto, mas não são itens. A Viagem usa "Cavalo e Sela" para a montaria, a Cavalaria tem Cavalo no kit, e o Comércio ainda não decidiu os animais de carga.
- *Opções:*
  - (a) **animais viram itens** de um tipo novo, "Animal", sem mudar o resto;
  - (b) animais só como número no Pasto, e o Cavalo como um item especial;
  - (c) animais como Criaturas (entidades vivas no mapa): mais bonito, mas cada animal custaria simulação.
- *Sugestão:* (a).
  - **Novo tipo** em Matéria-Prima, Recurso Bruto: **Animal**. Itens: Galinha, Porco, Ovelha e Vaca (Tier 0) e **Cavalo** (Tier 1).
  - O **Pasto** cria os animais.
  - O **Rancho** precisa dos animais no estoque, mas **não os gasta**: Galinha dá ovos, Ovelha dá lã e Vaca dá leite, e todos dão Dejetos Animais.
  - O **Açougue** gasta o animal e faz Pedaço de Carne, Ossos e Couro Bruto.
  - **Cavalo:** montaria (com Sela, já decidido) e **animal de carga** das caravanas. Isso fecha a pendência do Comércio: **[Proposta]** +100 de capacidade de peso por Cavalo.
  - **No mapa:** os animais do Pasto aparecem como decoração, desenhados com os sprites das Criaturas, mas não são simulados um a um.
- *Impacto:* Itens, Produção, Comércio, Viagem, Batalha (kit da Cavalaria). Arte: os animais já existiam como Criaturas e decoração.

**V1-4. Peixe e o "porto de pesca".**

- *Contexto:* I-5. O autor descartou o Peixe, mas o vilarejo Escravista (6,6) é um "porto de pesca e de escravos", e o padrão do Tiled tem uma área de "pesca".
- *Opções:*
  - (a) **tirar a pesca**: o vilarejo (6,6) vira "porto de escravos e de carga", e a área de pesca sai do Tiled;
  - (b) voltar com o Peixe: recurso nos mapas com rio ou litoral, mais uma construção ("Posto de Pesca") e uma receita. São mais arte, mais dados e mais um alimento no consumo.
- *Sugestão:* (a), para respeitar a decisão de descartar o Peixe. Se a pesca fizer falta nos testes, ela volta como acréscimo pequeno.
- *Impacto:* Mundo e Estado Inicial, Pipeline (camada de objetos), Escravidão (o porto continua sendo de escravos).

**V1-5. Um inventário por cidade ou um por construção?**

- *Contexto:* I-7. O Econômico decidiu **um único inventário por cidade**, compartilhado por todas as lojas, para não haver logística de entrega. O Inventário fala em "N inventários de construção" com limite, e a Produção dá a cada construção uma **capacidade de armazenamento**.
- *Opções:*
  - (a) **um inventário lógico por cidade, com a capacidade somada das construções**;
  - (b) um inventário por construção, com transporte entre elas: mais realista, mas cria a logística que o Econômico quis evitar.
- *Sugestão:* (a).
  - A cidade tem **uma tabela única** (item, dono, quantidade), que é o que o Econômico já descreve.
  - Cada construção **acrescenta capacidade** às categorias que produz ou vende. *Exemplo:* a Fazenda nível 2 dá +200 de capacidade de matéria-prima vegetal.
  - Quando a capacidade de uma categoria enche, a produção dela **para** até alguém consumir ou vender. É o "limite de estoque contra acúmulo infinito" que a Produção pedia.
  - O jogador continua tendo os itens dele separados pelo atributo **Dono**.
  - O documento do Inventário é reescrito com os componentes: Itens (lista), Equipamento (NPC), Capacidade de peso (Controlado), Capacidade por categoria (cidade) e Carga (caravana).
- *Impacto:* Inventário, Produção, Econômico, Construção. Desempenho: uma tabela por cidade é o mais barato possível.

**V1-6. Nomes dos itens.**

- *Contexto:* I-9. Há nomes em inglês ("Musket") e erros de concordância.
- *Sugestão:* padronizar em português, nesta lista:

| Hoje | Fica |
|---|---|
| Musket (itens) e Musquete (tipo) | **Mosquete** (item e tipo); o grupo "Canhão de mão" vira "Arma de fogo" |
| Machado Curta / Longa | Machado Curto / Longo |
| Perneiras Leve / Pesado; Capacete Leve / Pesado | Perneiras Leves / Pesadas; Capacete Leve / Pesado (já certo) |
| Calça rasgadas; Calça Ninjas; Calça Modernas | Calças Rasgadas e as outras ajustadas na V1-8 |
| Materias de Argila (lista) e Bens de Argila (Pedreira) | **Peças de Argila** nos dois |
| Osso (Açougue) e Ossos (lista) | **Ossos** |
| Carne (Açougue) e Pedaço de Carne (lista) | **Pedaço de Carne** |
| Ovo, Lã, Leite (Rancho) | Saco de Ovos, Fardo de Lã, Balde de Leite (os nomes da lista) |
| Roupa de Couro, Roupas de Tecido (Costureiro) | As peças com nome da lista (ver V1-7) |
| Moveis de Madeira, Moveis Refinados | Móveis de Madeira, Móveis Refinados (acentos) |

- *Impacto:* Itens, Funções e Construções, Glossário. Os IDs de dados (por exemplo, `item.mosquete_aco`) já nascem certos na ficha técnica.

**V1-7. Itens sem produtor e a Armadura Leve.**

- *Contexto:* I-10 e I-21. Vários itens da lista não são feitos por nenhuma construção. A Armadura Leve de metal está no Costureiro.
- *Sugestão:*

| Construção | Acrescenta |
|---|---|
| Cozinheiro | N1: Legumes Cozidos; N4: Torta de Fruta; N5: Carne Envelhecida e **Doce de Especiaria** (no lugar do Doce de Baunilha, porque a baunilha não é recurso; usa o Fardo de Especiaria) |
| Artesão | N1: Anel de Cobre; N3: Anel de Ouro; N4: Coroa; N5: **Joia Mágica** (Ouro + Pedra Mágica) |
| Ferreiro | **Armadura Leve de metal**: N2 de Ferro, N3 de Aço, N5 de Aço Mágico (sai do Costureiro); Chapéu de Mineiro (N1) |
| Costureiro | N1: Couro Refinado, Trapos, Calças Rasgadas, Chapéu de Palha, Camiseta e Calça Robustas; N2: Tecido e as roupas de tecido simples; N3: **Armadura Média** (couro e tecido; sai do N4); N4: roupas nobres; N5: Sela |
| Forja Arcana | Pedra de Mana (o "Produtor Mágico" vira o Mago de Ofício da Forja Arcana) |

  Os livros continuam só comprados ou achados (decidido na P1-6).
- *Impacto:* Funções e Construções, Itens, Combate (a Média vem um nível antes, o que dá ao Costureiro um papel militar).

**V1-8. Nomes com cara moderna.**

- *Contexto:* I-11. "Capuz, Camisa e Calça Ninja", "Terno", "Camiseta Moderna", "Calça social" e "Cajado Moderno" destoam de um mundo sombrio e político na chegada da pólvora.
- *Opções:*
  - (a) **renomear** para o tom do mundo;
  - (b) manter, como roupas da Era 4 (a era mais moderna da tecnologia).
- *Sugestão:* (a):

| Hoje | Fica | Ideia |
|---|---|---|
| Capuz, Camisa e Calça Ninja | Capuz, Gibão e Calça **de Sombra** | Roupa de ladrão: bônus de Destreza (ocultação) |
| Camiseta Moderna, Calça Modernas | Casaca e Calça **de Oficial** | Era 3 e 4; bônus de Carisma com militares |
| Terno, Calça social | **Gibão e Calça de Gala** | Roupa nobre da Era 4 |
| Cajado Moderno | **Cajado Rúnico** | O mais alto da Era 4, mistura de magia e técnica |

  O Rifle e a Carabina continuam: são da Era 4 da pólvora.
- *Impacto:* Itens, Pipeline (só nomes; a arte é a mesma).

**V1-9. Fome e Felicidade dos Controlados.**

- *Contexto:* I-14. A IA usa **Fome** ("ao se aproximar de 0, busca alimento") e a Felicidade tem um valor individual para Controlados e Importantes, mas nenhum dos dois está na lista de atributos do NPC. A Viagem diz "uma refeição por pessoa por dia", e a comida tem o atributo **Saciedade**.
- *Sugestão:* acrescentar dois atributos só para Controlados e Importantes:
  - **Fome** (0 a 100; 100 = satisfeito): cai **100 em 36 horas** de jogo. O NPC come sozinho quando passa de 50, do inventário dele ou do grupo, pela melhor comida (decidido na P1-6). A **Saciedade** de cada comida devolve: Alimento Básico 60, Refinado 80, Luxo 100 **[Proposta]**. Com Fome 0, a Vida cai 2% por hora e a Felicidade cai rápido (como a Viagem já previa). Na prática, 1 a 2 refeições por dia.
  - **Felicidade individual** (0 a 100): os 6 fatores já decididos na Felicidade.
  - **Descanso:** não vira atributo (o jogo não tem estamina). Só um contador de **noites sem dormir**, usado pela marcha forçada.
  - Os NPCs comuns continuam com tudo agregado na cidade.
- *Impacto:* NPC, IA, Felicidade, Viagem, Efeitos de Itens.

**V1-10. Vagas do Templo.**

- *Contexto:* I-16. A Religião propôs 1, 2, 5, 10 e 15 Sacerdotes por nível; a tabela geral de vagas é 2, 3, 5, 7 e 10.
- *Opções:*
  - (a) **o Templo segue a tabela geral** (2, 3, 5, 7 e 10), e as capitais podem ter **2 Templos**;
  - (b) o Templo é uma exceção, com a própria tabela.
- *Sugestão:* (a), para manter uma só regra de vagas.
  - Cobertura com 1 Templo: 40, 60, 100, 140 e 200 NPCs reais.
  - Uma capital (300 NPCs) com 1 Templo nível 5 tem 67% de cobertura, acima do limite de 50% da Devoção Alta. Com o 2º Templo, chega a 100%.
  - O 2º Templo só existe nas cidades de nível 4.
- *Impacto:* Religião, Construção, Mundo (pacote de construções das capitais).

**V1-11. Ordem e ritmo da revisão.**

- *Contexto:* o plano da [[Revisão Completa]] tem 5 rodadas: V1 (esta), V2 (economia numérica), V3 (pendências e listas), V4 e V5 (propostas).
- *Opções:*
  - (a) **seguir o plano** na ordem;
  - (b) juntar V4 e V5 numa rodada só, com uma tabela grande respondida por "confirmar todas, exceto...";
  - (c) fazer a V2 logo depois desta, porque ela destrava a ficha técnica, e deixar V3 a V5 para depois da ficha.
- *Sugestão:* (a), com uma regra: nas rodadas V4 e V5, o autor pode responder **"confirmar todas, exceto..."**, e eu só detalho as que ele quiser mudar.

Depois das respostas: aplicar as correções, fazer a faxina dos 13 documentos e abrir a **V2** (base numérica da economia).

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
