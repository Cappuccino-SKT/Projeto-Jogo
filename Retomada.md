> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado ao aplicar a rodada P0-5 e abrir a P0-6 (2026-10-15).

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Rodada pendente: P0-6, Pipeline de arte e conteúdo** (última do P0). A lista de perguntas (seção 4) foi enviada ao autor e **ainda não foi respondida**. Ao retomar, pedir o `Respostas.txt` (ou as respostas no chat) e conferir se ele é desta rodada: os itens vão de A0a a E3.
3. Depois da P0-6: os sistemas P1 (seção 3).
4. Quando o autor estiver satisfeito com a documentação: **ficha técnica lógica**, e só então a programação.

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
| P0 | **Pipeline de arte e conteúdo** (Tiled, orçamento de arte) | **Perguntas enviadas, sem resposta** |
| P1 | Viagem e Mundo (entidades em trânsito, encontros, névoa) | A fazer |
| P1 | Reputação e Relações do jogador | A fazer |
| P1 | Companheiros, Relações familiares e Diálogo | A fazer |
| P1 | Guerra e Campanha | A fazer |
| P1 | Batalha (Mapa de Guerra) | A fazer |
| P1 | Diplomacia | A fazer |
| P1 | Religião | A fazer |
| P1 | Clima e Estações | A fazer |
| P1 | Notícias e Informação | A fazer |
| P1 | Efeitos de itens e Escravidão | A fazer |
| P2 | Áudio, tutorial, configurações, localização, modding, conquistas | Depois |

Ordem sugerida do P1: Viagem → Reputação → Companheiros e Diálogo → Guerra, Batalha e Diplomacia → Religião, Clima, Notícias → Efeitos de itens e Escravidão. **Ideias da seção 8 da Revisão** ainda sem decisão do autor, exceto "mundo fixo com estado inicial variável" (aceita): crônica do mundo visível, especialização de cidades, ordens de produção em lote, guarnição e defesa, modo observador, eventos de dinastia.

## 4. Rodada P0-6 pendente: Pipeline de arte e conteúdo

Objetivo: decidir **como a arte e os dados do jogo serão produzidos** e **quanto** cabe na primeira versão. Tudo é feito à mão, e é aqui que mora o maior risco do projeto (ver a seção 7 da [[Revisão do Projeto]]). Inclui duas pendências da P0-5 (A0a e A0b). Primeira rodada no padrão detalhado (seção 2).

### A. Pendências da P0-5

**A0a. Quantos mapas sem dono?**
- *Contexto:* foi aceito "cerca de 30% dos mapas sem dono" e também "1 capital, 1 cidade e 1 ou 2 vilarejos por região". Numa malha de 48 mapas, cada cidade de facção transforma até 4 vizinhos em território. Na disposição proposta no [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]], sobram só **4 mapas sem dono (8%)**.
- *Opções:*
  - (a) **Aceitar cerca de 8%.** Os territórios de facção continuam exploráveis (masmorras, recursos, bandidos) e permitem fundar assentamento vinculado; as 3 Vilas Livres completam o espaço sem facção. Não custa arte.
  - (b) **Malha de 10 × 6 (60 mapas)**, com uma faixa central de 12 mapas selvagens entre o oeste e o leste: cerca de 27% sem dono, mas são 12 mapas de 128 × 128 a mais para desenhar (25% a mais de arte de mapa).
  - (c) **Manter 48 e tirar os vilarejos** (só capital e cidade por região): 13 mapas sem dono (27%), mas sobram 12 cidades de facção e só 2 mapas em disputa, com menos guerra de fronteira.
- *Sugestão:* **(a)**. O volume de arte é o maior risco do projeto, e o que importa para o jogador (espaço para explorar e fundar) já existe nos territórios e nas Vilas Livres. Se nos testes o mundo parecer apertado, a malha cresce depois (você já previu que o tamanho pode mudar).

**A0b. Onde o Dono de vila começa?**
- *Contexto:* o [[Sistema de Início de Jogo/Descrição|Início de Jogo]] diz que o Dono de vila começa num assentamento **em território de facção** (vinculado a ela) e tenta torná-lo independente. A proposta do Mundo colocou os pontos em **mapas livres** (já independente).
- *Opções:*
  - (a) Mapa livre: começa independente, sem impostos nem leis de facção; mais tranquilo, com menos história.
  - (b) Território de facção: começa vinculado; para cumprir a Ambição "Fundar uma facção", precisa crescer até o nível 3 e romper com a facção, o que pode virar revolta. Mais conflito.
  - (c) **Os dois:** 3 pontos, 2 em territórios de facções diferentes e 1 livre; o jogador escolhe.
- *Sugestão:* **(c)**, com os pontos: território Arcano (5,2), território Tecnocrata (2,5) e o mapa livre (5,3). Mantém a ideia original (ascender e se libertar) e dá uma opção pacífica.

### B. Quem desenha e com qual ferramenta

**B1. Quem vai desenhar, e com quanta experiência?**
- *Contexto:* tudo é à mão: 48 mapas, construções de 6 culturas, bonecos em camadas, animações, ícones e a interface. A [[Interface/Descrição|Interface]] diz "nosso próprio desenhista", mas não está definido quem é.
- *Pergunta:* quem desenha: você, um amigo ou um artista contratado? Alguém já fez pixel art ou animação? Quantas horas por semana podem ir para a arte?
- *Opções:* (a) você sozinho; (b) você e um artista parceiro (por exemplo: você monta os mapas no Tiled; o parceiro desenha tiles, bonecos e construções); (c) artista contratado por encomenda.
- *Sugestão:* a escolha depende da sua resposta, mas em qualquer caso usar **arte provisória** na fatia vertical e nos protótipos: quadrados coloridos e bonecos simples de 16 px, feitos por você em minutos. Assim a programação não espera a arte, e a arte final entra depois, trocando só os arquivos de imagem. Arte provisória não é procedural; é um rascunho à mão, descartado depois. Também é possível usar pacotes gratuitos de terceiros **só como provisório**, nunca no jogo final.

**B2. Programa de desenho**
- *Contexto:* o Tiled monta os mapas, mas não desenha os tiles nem os bonecos. Falta um programa de pixel art.
- *Opções:*
  - (a) **Aseprite**: cerca de US$ 20 (Steam ou itch.io), ou grátis se compilado a partir do código aberto. Tem camadas, animação, paleta travada, e exporta a folha de sprites (uma imagem com todos os quadros) com um arquivo JSON que o jogo lê. É o padrão dos jogos indie de pixel art.
  - (b) **LibreSprite**: grátis, uma versão antiga do Aseprite, sem atualizações recentes.
  - (c) **Pixelorama**: grátis e em desenvolvimento ativo, um pouco menos completo.
- *Sugestão:* **(a) Aseprite**. Os arquivos `.aseprite` ficam guardados como fonte e o jogo usa só o PNG exportado. Se o custo pesar, começar no Pixelorama, que exporta no mesmo formato.

**B3. Padrão de camadas dos mapas no Tiled**
- *Contexto:* o Tiled funciona como um "Paint de azulejos": você escolhe um tile e pinta no mapa. Cada mapa tem **camadas** (folhas transparentes empilhadas) e **objetos** (pontos e retângulos com nome). O jogo lê as camadas pelo nome, então o padrão precisa estar fixo antes do primeiro mapa.
- *Pergunta:* aceita este padrão para todos os Mapas Locais?
- *Sugestão:*
  - **Camadas de tiles:** 1. Chão (grama, terra, água); 2. Detalhes do chão (flores, caminhos); 3. Objetos baixos (pedras, cercas, lavouras); 4. Construções e paredes; 5. Acima do personagem (copas de árvore, telhados); 6. Colisão (invisível no jogo; marca onde não se anda).
  - **Camadas de objetos:** Lotes (pequeno, médio e grande, com o tipo permitido); Portas (ligam ao Mapa Interno); Bordas (saída para o mapa vizinho); Recursos (áreas de floresta, mina, pesca); Criaturas e bandidos (pontos de aparição); Pontos de interesse (quadro de contratos, entrada de masmorra).
  - **Ajudas do editor:** o recurso de **terrenos** do Tiled (você pinta "grama" e ele escolhe sozinho o tile de borda certo) e os **carimbos** (pedaços prontos, como "bosque pequeno" ou "lavoura", colados várias vezes). São só ajudas na hora de desenhar: o mapa salvo é fixo e feito à mão, não é gerado pelo jogo.

### C. Especificação da arte

**C1. Tamanho do boneco e quanto aparece na tela**
- *Contexto:* tile de 16 px e vista de cima em 3/4 (como Stardew) estão como provisórios. Você joga em 1600 × 900.
- *Conta:* com zoom 2x, cada tile ocupa 32 px na tela, e 1600 × 900 mostra **50 × 28 tiles**; com 3x, 33 × 19; com 4x, 25 × 14. Um Mapa Local de 128 × 128 tem cerca de 2,5 telas de largura e 4,5 de altura em 2x.
- *Opções do boneco:* (a) **16 × 24 px** (1 tile de largura e 1,5 de altura): multidões legíveis, estilo próximo de Songs of Syx; (b) **16 × 32 px** (estilo Stardew): mais detalhe de roupa e armadura, mas cada peça custa mais pixels em cada quadro.
- *Sugestão:* confirmar o tile de 16 px e a vista 3/4, com boneco de **16 × 24** e zoom padrão de 2x. O jogo mostra muita gente (100 a 300 NPCs reais por cidade, combates de 40), e cada peça de armadura é desenhada em todos os quadros de animação; o boneco menor corta esse custo em cerca de 25%.

**C2. Paleta de cores**
- *Contexto:* arte feita por mais de uma pessoa, ou ao longo de anos, fica desencontrada sem uma paleta fixa.
- *Opções:* (a) **paleta mestre** fixa para o jogo todo; (b) paleta livre em cada desenho.
- *Sugestão:* **(a)**. Uma paleta de cerca de 48 cores (partindo de uma paleta pública conhecida de pixel art, como a Endesga 32 ou a Resurrect 64, ajustada) e **2 cores de destaque por cultura**, usadas em bandeiras, roupas e telhados: Guerreira (vermelho-sangue e ferro), Arcana (roxo e azul-noite), Autoritária (dourado e preto), Religiosa (branco e azul-celeste), Tecnocrata (cobre e verde-oliva), Escravista (púrpura e areia). As mesmas cores pintam a lente Política do Mapa Mundial.

**C3. Troca de paleta nas variações por cultura** (decisão que mais mexe no volume de arte)
- *Contexto:* cada item tem textura por cultura, e cada boneco usa camadas (corpo, 3 peças de armadura, arma e escudo). Se cada variação for um desenho novo, o número de quadros explode (ver D1).
- *Como funciona:* desenha-se a peça uma vez, à mão (por exemplo, a Armadura Pesada), e o jogo troca cada cor por outra, de uma tabela. O "metal" vira ferro escuro na Guerreira e bronze polido na Escravista; o "tecido" vira vermelho ou púrpura. O desenho continua feito à mão; só as cores mudam.
- *Opções:* (a) troca de paleta para cultura, material (Tier) e cor de roupa; desenho novo só quando a forma muda; (b) tudo desenhado separado: máximo de identidade, custo multiplicado por 6; (c) misto: corpo e roupas por troca de paleta; armaduras, armas e escudos com forma própria por cultura.
- *Sugestão:* **(c)** como meta e **(a)** na primeira versão. Cada cultura ganha silhueta própria nas peças que mais aparecem (um elmo com chifres na Guerreira, um capuz na Arcana) e o resto vem das cores.

**C4. Quantos quadros de animação**
- *Contexto:* as animações são compartilhadas por tipo de arma (8 tipos), em 4 direções ([[Sistema de Combate/Descrição|Combate]]).
- *Sugestão (quadros por direção):* parado 2; andar 4 (correr usa o andar acelerado); ataque 4 para cada um dos 8 tipos de arma (32); ferido 1; cair 3; trabalhar 4 em cada um de 3 gestos (golpear: machado, picareta, martelo; abaixar e colher; carregar) (12). Total: 54 por direção, cerca de **216 quadros por camada**. Desenhando só 3 direções e espelhando a esquerda a partir da direita: cerca de **162**.
- *Pergunta:* aceita esse orçamento e o espelhamento? Com o espelhamento, o escudo troca de mão quando o boneco vira; muitos jogos aceitam isso.

### D. Orçamento e escopo da primeira versão

**D1. Volume de arte e por onde começar**
- *Contexto:* estimativa grosseira, para dimensionar (valores a refinar):

| Item | Conta completa | Primeira cultura, com os cortes sugeridos |
|---|---|---|
| Tiles de terreno | cerca de 200 por bioma × 6 = 1.200 | 1 bioma: cerca de 200 |
| Construções | 22 tipos × 5 níveis × 6 culturas = 660 desenhos | aparência muda só nos níveis 1, 3 e 5, com peças reaproveitáveis: 22 × 3 = 66 |
| Bonecos | cerca de 20 camadas × 162 quadros = 3.200 quadros | troca de paleta (C3): cerca de 1.500 |
| Mapas Locais | 48 mapas de 128 × 128 | 1 mapa na fatia vertical; 8 na primeira região |
| Mapas Internos | ver D3 | moradias genéricas + 4 públicas |
| Criaturas | cerca de 10 tipos × 40 quadros | 3 tipos |
| Interface | molduras, botões e cerca de 150 ícones de itens | ícones da fatia vertical |

- *Pergunta:* aceita produzir a arte em camadas, igual à programação, e qual cultura vem primeiro?
- *Sugestão:* (1) **fatia vertical:** 1 bioma (planície), 1 cultura, 1 Mapa Local, Fazenda, Cozinheiro, Mercado e Casa, 1 boneco com parado, andar e trabalhar; (2) **cidade completa** da mesma cultura; (3) **a primeira região** inteira; (4) as outras culturas e biomas. Primeira cultura: **Autoritária**, porque a fatia vertical (Fazenda → Cozinha → Mercado) combina com a planície do grão, e a cultura dela é a mais "neutra" visualmente, boa base para as outras.

**D2. As estações mudam a aparência do mapa?**
- *Contexto:* o ano tem 4 estações de 16 dias (cerca de 4,3 horas reais cada). O sistema de Clima e Estações ainda será escrito.
- *Opções:* (a) sem mudança visual; (b) **tingimento** do mapa por estação (mais amarelado no outono, azulado no inverno) e algumas peças trocadas à mão (neve nos telhados, árvores sem folhas, lavoura colhida), cerca de 15% de tiles a mais; (c) um conjunto completo de tiles por estação, como Stardew: tiles de terreno × 4.
- *Sugestão:* **(b)**: o jogador sente a estação, e o custo de arte é pequeno.

**D3. Quais construções têm interior desenhado?**
- *Contexto:* cada construção é um Mapa Interno com grade própria. Desenhar interiores para 22 tipos × níveis × culturas é enorme.
- *Opções:* (a) todas; (b) só as que o jogador visita com frequência (Moradias, Taverna, Mercado, Templo, Prisão, Palácio, Quartel e as 12 masmorras); nas de produção, os trabalhadores entram pela porta e o interior existe só como dado, sem arte; (c) interiores genéricos por tamanho (pequeno, médio e grande), decorados com móveis do tipo da construção.
- *Sugestão:* **(b) + (c)**: as públicas da lista com interior próprio; Moradias e construções de produção com interior genérico por tamanho e cultura. *Conflito resolvido:* o furto em loja ilegal disfarçada ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]) acontece numa construção de produção, que assim também tem interior (o genérico).

### E. Dados e ferramentas

**E1. Como editar os dados (itens, receitas, construções)**
- *Contexto:* está decidido JSON na pasta `data/`, com validação. Algumas tabelas terão centenas de linhas (itens, receitas, preços).
- *Opções:* (a) JSON direto num editor de texto comum; (b) **VS Code** (grátis) com **esquemas JSON**: o editor completa os campos e sublinha os erros enquanto você digita; (c) planilhas (LibreOffice, Excel ou Google Sheets) convertidas em JSON por um script Python.
- *Sugestão:* **(b)** para tudo e **(c)** só nas tabelas grandes e simples (itens, receitas, preços). Dois scripts em Python, que você já tem instalado: `converter_planilhas.py` (gera os JSON) e `validar_dados.py`, que roda antes do jogo e antes de cada commit e lista erros como "a receita Pão cita o item Farinha de Trigo, que não existe".

**E2. Quem manda nas listas depois que os dados existirem?**
- *Contexto:* hoje as listas estão nos documentos e nos canvases do Obsidian, e já houve nomes diferentes entre eles.
- *Sugestão:* quando a pasta `data/` for criada (na ficha técnica), **os dados passam a mandar nas listas** (itens, receitas, construções, preços) e os documentos ficam com as regras. Os canvases viram arquivo histórico ou são gerados a partir dos dados. Até lá, a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] continua mandando.

**E3. Onde guardar a arte**
- *Contexto:* a arte terá arquivos-fonte (`.aseprite`, mapas do Tiled) e arquivos exportados (PNG). Pixel art é leve (poucos KB por imagem).
- *Sugestão:* guardar tudo no **repositório do código**, que será criado quando a programação começar: `assets/` para as fontes e `data/` para o que o jogo lê. Sem Git LFS (um complemento do Git para arquivos grandes), que não é necessário com arquivos tão pequenos. Este repositório continua só com documentos.

Depois das respostas: escrever `Pipeline de Arte e Conteúdo/Descrição.md`; atualizar Arquitetura, Interface, Combate (animações), Construção (níveis visuais), Mapa (camadas do Tiled), Cultura (arte) e o Mundo (A0a e A0b); fechar o P0 e abrir o P1, começando por Viagem e Mundo.

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). O Ferreiro não lista Espada (provável erro; ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]).
- **Mapas:** serão desenhados no **Tiled** (editor gratuito); o autor ainda não conhece a ferramenta e pediu explicação simples quando chegar a hora.
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
