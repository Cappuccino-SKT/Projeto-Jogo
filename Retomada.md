> Ponto de partida de cada sessão. Curto de propósito: o detalhe de cada sistema está no documento dele (índice em [[Descrição do Projeto]]), e a situação geral no [[Estado do Projeto]]. Atualizado em 2026-10-17.

## 1. Onde paramos

- **Design completo:** todos os sistemas estão escritos, a revisão completa terminou, e a [[Ficha Logica/Fatia Vertical|Ficha Lógica: Fatia Vertical]] está pronta.
- **Rodada FT2 (2026-10-17):** o autor **liberou o código, marco a marco**.
- **Pastas reorganizadas (2026-10-17):** `Documentos/`, `Ficha Logica/`, `Codigo/` e `Jogo/` (seção 3).
- **Fichas do marco 1 (2026-10-17):** [[Ficha Logica/Tempo|Tempo]] e [[Ficha Logica/Mapa|Mapa]] (tiles e camadas) escritas. O marco 0 não tem ficha. A **rodada FL1** foi enviada (seção 4).
- **Próximo passo:** as respostas da FL1, que ajustam as duas fichas. O marco 0 (seção 3) não depende delas e pode começar antes.
- **Ao retomar:** rodar `git fetch` e conferir se o pull request mais recente foi mesclado no `main`. O último é o da branch `docs/fichas-marco-1`.

## 2. Como trabalhamos

- **Idioma e tom:** português do Brasil, em linguagem simples.
- **Rodadas de perguntas** quando há decisão a tomar:
  - códigos (FT2-1, FT2-2...); o autor responde em `C:\Users\Felipe\Desktop\Respostas.txt` (anexado com @);
  - "Seguir sugestão" aceita a proposta; a ideia própria do autor sempre prevalece;
  - antes de aplicar, conferir se o arquivo é mesmo da rodada atual.
- **Padrão de cada pergunta** (pedido do autor). Nada abstrato, como "Tom?" ou "Equilíbrio?" soltos.
  1. **Contexto:** o que já está decidido e o que a pergunta destrava.
  2. **Pergunta concreta:** com números, nomes e um exemplo de situação de jogo.
  3. **Opções:** de 2 a 4, com o que cada uma ganha e perde.
  4. **Sugestão completa e pronta para usar:** valores iniciais, exemplo, impacto nos outros sistemas, custo e o que fica para testar.
  5. **Conflitos:** o que bate com decisões anteriores, e a saída.
- **Marcações nos documentos:**
  - sem marca: decisão do autor;
  - **[Proposta]**: sugestão ainda não decidida;
  - **[Em aberto]**: pergunta que precisa de decisão;
  - **[Refinar depois]**: ideia aceita, com detalhes para depois.

  Quando um "seguir sugestão" deixa algo vago, o documento detalha com valores marcados como [Proposta].
- **Git:**
  - cada rodada ou marco é uma branch e um pull request;
  - sem o `gh`, o autor abre o pull request pelo link `https://github.com/FelipeSKT/Projeto-Jogo/pull/new/NOME-DA-BRANCH`;
  - se o pull request anterior ainda não foi mesclado, a branch nova sai da anterior, e o autor é avisado da ordem para aceitar.
- **Formato dos documentos:**
  - um por sistema (`Sistema de X/Descrição.md`), com legenda, descrição, seções, Parte técnica, Ligação com outros sistemas e Perguntas em aberto;
  - sem seção de "atualizações recentes": o histórico fica no Git.
- **Preferências do autor:**
  - interface personalizável (ele joga em 1600 × 900);
  - fontes legíveis;
  - arte toda à mão;
  - só teclado e mouse;
  - "por quê?" em tudo;
  - a simulação vem primeiro, e o evento explica.

## 3. Código (rodada FT2)

- **Pastas (organização de 2026-10-17):**
  - `Documentos/`: todo o design (os sistemas, o Glossário, o índice e o `Histórico/`);
  - `Ficha Logica/`: cada sistema virado lógica de programação (dados, regras passo a passo, testes). O modelo está na [[Ficha Logica/Descrição|Ficha Lógica]]. Um sistema é convertido **pouco antes do marco que o usa**, não todos de uma vez;
  - `Codigo/`: as regras em C++ (tempo, economia, produção, NPCs). **Nunca abre janela nem desenha**, para o teste sem tela rodar;
  - `Jogo/`: o programa de verdade (janela, desenho, mouse e teclado), que chama o `Codigo/`. Os dados (`data/`) e a arte (`assets/`) ficam aqui;
  - na raiz: `CLAUDE.md`, esta Retomada, o Estado e o `CMakeLists.txt` principal. A pasta `build/` (o que o compilador gera) fica fora do Git.
- **Marcos:** de 0 a 9, na ficha (parte 7). Cada marco tem uma branch própria (`codigo/marco-N-nome`) e um pull request.
- **Jeito de trabalhar (FT2-3):**
  - o assistente escreve em passos pequenos, e cada passo compila e roda;
  - ele dá os comandos prontos;
  - o autor compila, roda e conta o que viu;
  - cada marco termina com um resumo simples em `Jogo/LEIA-ME.md`.
- **Nomes (FT2-4):** português sem acento (`Cidade`, `Construcao`, `precoBase`), com comentários em português.
  - **Ideia do autor:** nada abstrato demais. Todo nome de variável, função ou classe diz o motivo dele: `horasTrabalhadasNoCiclo`, não `h` nem `valor`.
  - As palavras do C++ (`int`, `vector`, `for`) e os nomes das bibliotecas continuam como são.
- **Bibliotecas (FT2-2):** aprovadas. Cada uma entra no marco em que é usada e é baixada pelo CMake (`FetchContent`), em versão fixa:

| Marco | Bibliotecas |
|---|---|
| 0 e 1 | SDL3, glad (OpenGL 3.3), stb_image |
| 3 | nlohmann/json (o CSV é lido por código próprio) |
| 4 e 5 | EnTT |
| 7 | Tracy |
| 8 | Dear ImGui |
| Depois da fatia | miniaudio |

- **Ambiente (FT1-7):** g++ 15.2 (MinGW), CMake, Ninja e Git, já instalados; VS Code com as extensões "C/C++" e "CMake Tools"; depurador gdb; tudo neste repositório.
- **Marco 0, o próximo:**
  1. o autor instala as duas extensões do VS Code;
  2. o assistente cria o `CMakeLists.txt` da raiz e o `Jogo/main.cpp`, que abre uma janela vazia com o SDL3 (o marco 0 não tem regra, então a `Codigo/` ainda fica vazia);
  3. compilar e rodar;
  4. escrever o LEIA-ME do marco.

## 4. Rodada pendente: FL1, as fichas do marco 1

**Perguntas enviadas em 2026-10-17, aguardando respostas.** Ao escrever as fichas do [[Ficha Logica/Tempo|Tempo]] e do [[Ficha Logica/Mapa|Mapa]], apareceram 6 pontos que os documentos não decidem. As fichas já usam as sugestões, marcadas como [Proposta].

Responder no `Respostas.txt` pelos códigos (FL1-1, FL1-2...). "Seguir sugestão" aceita a proposta.

**FL1-1. Como o marco 1 lê o mapa do Tiled.**

- *Contexto:* o marco 1 termina quando "o mapa do Tiled aparece". A Arquitetura e o Pipeline dizem que o mapa é salvo em **JSON**, mas a Fatia Vertical cita `mapas/vila.tmx`, o formato XML do Tiled. E a biblioteca de JSON (nlohmann/json) só entra no **marco 3** (FT2-2).
- *Pergunta:* no marco 1, como o jogo lê um mapa de 64 × 64 tiles com 6 camadas, ou seja, cerca de 25.000 números?
- *Opções:*
  - (a) **antecipar a nlohmann/json para o marco 1** e salvar o mapa em JSON (`vila.tmj`). Ganha: um formato só para os mapas e os dados, a mesma biblioteca do marco 3, e cerca de 100 linhas de leitura. Perde: uma biblioteca a mais logo no início, mas ela já está aprovada, é só de cabeçalhos e o CMake baixa sozinho;
  - (b) manter o `.tmx` e trazer uma biblioteca de XML (por exemplo, a pugixml). Ganha: o formato padrão do Tiled. Perde: uma biblioteca nova, que não foi aprovada e só serve para isso, e dois formatos no projeto;
  - (c) no marco 1, um mapa de teste escrito no próprio código, e o Tiled só no marco 3. Ganha: nenhuma biblioteca antes da hora. Perde: o marco 1 não cumpre o que promete, e esse código vai para o lixo depois.
- *Sugestão:* (a). O Tiled salva direto em `Jogo/data/mapas/vila.tmj`: um arquivo só, sem passo de exportar para esquecer. A `Jogo/assets/` guarda o projeto do Tiled e as imagens-fonte.
- *Impacto:* Arquitetura e Retomada (a nlohmann/json passa a entrar no marco 1), Fatia Vertical (parte 2), Pipeline.
- *Conflitos:* a FT2-2 dizia "marco 3" para a nlohmann/json. Muda só a data de entrada, não a aprovação. O `vila.tmx` da Fatia Vertical vira `vila.tmj`.

**FL1-2. A hora em que a partida começa.**

- *Contexto:* a partida começa no 1º dia da Primavera do Ano 412 ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]), mas a hora não foi decidida. O dia vira às 0h (FT1-5), e os Civis dormem das 22h às 6h ([[Sistema de IA/Descrição|IA]]).
- *Pergunta:* a partida nova começa a que horas? *Exemplo:* começando às 0h, o jogador abre o jogo e vê a vila escura, com todos dormindo, por 6 horas de jogo, ou seja, 4 minutos reais a 1x.
- *Opções:*
  - (a) **0h** (tick 0). Ganha: a conta mais simples. Perde: um começo parado e escuro;
  - (b) **6h** (tick 4.800). Ganha: os Civis acordam, o amanhecer está no meio, e o trabalho começa 1 hora depois. Perde: o dia 1 tem só 18 horas, mas o turno de trabalho (7h às 15h) fica inteiro;
  - (c) **7h** (tick 5.600). Ganha: já começa de dia, com todos trabalhando. Perde: o jogador não vê a vila acordar.
- *Sugestão:* (b) 6h. O contador continua contando a partir da 0h do dia 1, então nenhuma conta muda: só o tick inicial é 4.800. O valor fica nas regras (`hora_inicial_da_partida: 6`). No teste sem tela, `--simular 64` conta 64 viradas de dia.
- *Conflitos:* nenhum.

**FL1-3. As proteções do relógio e o que o marco 1 mostra.**

- *Contexto:* o relógio anda pelo tempo real de cada quadro. O HUD só chega no marco 8, e as teclas da câmera no marco 2, mas o marco 1 já precisa mostrar que o relógio anda.
- *Pergunta 1:* o PC trava por 3 segundos, ou o autor arrasta a janela (o Windows para o programa enquanto ela é arrastada). O que o relógio faz com esses 3 segundos?
  - (a) **limite de 250 milissegundos por quadro:** o mundo anda só 250 milissegundos e o resto do tempo é perdido. Ganha: nunca trava em cadeia. Perde: o mundo "pula" um pedacinho do tempo, o que ninguém nota;
  - (b) sem limite: roda os 60 ticks atrasados de uma vez. Ganha: o tempo é exato. Perde: num PC lento, cada quadro demora mais e acumula mais atraso, até o jogo travar;
  - (c) além de (a), **pausar sozinho quando a janela é minimizada**.
- *Pergunta 2:* como ver a data e mudar a velocidade no marco 1?
- *Sugestão:* (a) e (c). Ao perder o foco, o jogo continua rodando, para o autor poder olhar outra janela. No marco 1:
  - a data e a velocidade aparecem **no título da janela**: "Primavera, dia 1, 06h00, Ano 412, 1x". Isso não precisa de fonte nem de desenho de texto;
  - cada virada de hora e de dia escreve uma linha no console;
  - **teclas provisórias:** Espaço pausa e despausa; 1, 2 e 3 escolhem 1x, 2x e 4x; F1 pinta a colisão de vermelho.
- *Conflitos:* a [[Interface/Descrição|Interface]] quer atalhos remapeáveis. As teclas do marco 1 são provisórias e entram na tabela de atalhos depois.

**FL1-4. Como rodar os testes automáticos.**

- *Contexto:* cada ficha tem uma tabela de testes (16 no Tempo, 15 no Mapa). A FT2-2 aprovou 8 bibliotecas, nenhuma de testes.
- *Pergunta:* como esses testes viram um programa? *Exemplo:* o teste "o tick 1.000.000 é 2h do dia 5 do Inverno" precisa rodar sozinho, sem janela, e dizer PASSOU ou FALHOU.
- *Opções:*
  - (a) **um programa de testes próprio** (`Codigo/Testes/`), com uma função `conferir(condicao, "o que está sendo conferido")` de cerca de 20 linhas. Roda com `ctest --test-dir build`, que já vem com o CMake. Ganha: nada novo para aprender e nenhuma biblioteca. Perde: mensagens mais simples;
  - (b) a biblioteca **doctest** (um arquivo só, muito usada). Ganha: mensagens melhores ("esperava 2, veio 3"). Perde: uma biblioteca nova e comandos (`TEST_CASE`, `CHECK`) que parecem mágica para quem está começando;
  - (c) testar à mão, rodando o jogo. Perde: a garantia de que uma mudança não quebrou o que já funcionava.
- *Sugestão:* (a). Se os testes crescerem muito (no marco 7), a doctest volta como pergunta.
- *Conflitos:* nenhum, porque não entra biblioteca nova.

**FL1-5. O padrão do arquivo do Tiled.**

- *Contexto:* o [[Pipeline de Arte e Conteúdo/Descrição|Pipeline]] define 7 camadas de tiles e as camadas de objetos, com nomes com acento e espaço ("Detalhes do chão"). O jogo procura cada camada pelo nome.
- *Pergunta:* como escrever os nomes e configurar o Tiled? *Exemplo:* se a camada se chamar "Colisão" no Tiled e "Colisao" no código, o jogo diz "falta a camada `colisao`", e o erro é difícil de enxergar.
- *Opções:*
  - (a) **nomes sem acento e em minúsculas** (`chao`, `detalhes_do_chao`, `colisao`...), como os IDs dos dados (`item.pao`). Ganha: igual no Tiled, no código e nos dados. Perde: um pouco menos bonito no Tiled;
  - (b) nomes com acento, como no Pipeline. Ganha: mais bonito no Tiled. Perde: é fácil errar ao digitar, e o erro é invisível.
- *Sugestão:* (a), com a tabela da parte 2 da [[Ficha Logica/Mapa|ficha do Mapa]], e também:
  - camadas em formato CSV, conjunto de tiles embutido no mapa, sem margem e sem tiles espelhados;
  - lotes como retângulos na camada `lotes`, com a propriedade `tipo_permitido` (na fatia, sempre `qualquer`);
  - **as construções não são pintadas no Tiled**: o jogo as desenha nos lotes, porque elas mudam durante a partida;
  - um lote não pode ter tile de colisão, porque a construção ainda vai nascer nele;
  - o jogo lista **todos** os erros do mapa de uma vez.
- *Conflitos:* o Pipeline passa a mostrar os nomes novos.

**FL1-6. A arte do marco 1.**

- *Contexto:* o [[Estado do Projeto]] diz que os pacotes de arte provisórios são escolhidos no início do código. O marco 1 precisa de um conjunto de tiles de 16 × 16 e de um mapa de 64 × 64 no Tiled.
- *Pergunta:* com que arte o marco 1 desenha o mapa?
- *Opções:*
  - (a) **um conjunto de teste feito pelo assistente:** quadrados de cor lisa (grama verde, água azul, terra marrom, parede cinza, árvore verde-escura, colisão vermelha) e um `vila.tmj` de teste com os 11 lotes e as 3 áreas de recurso. O autor abre no Tiled e mexe à vontade. Ganha: o marco 1 não espera pela arte, e os erros de leitura aparecem logo. Perde: é feio;
  - (b) **um pacote grátis de licença CC0** (livre para qualquer uso), por exemplo o "Tiny Town" da Kenney, de tiles de 16 × 16 (conferir a licença na página antes de baixar). O autor baixa e desenha o mapa. Ganha: já parece um jogo. Perde: o autor precisa aprender o Tiled antes de ver o marco 1, e o estilo pode não ser o 3/4 da Arquitetura;
  - (c) o autor desenha os primeiros tiles no Pixelorama. Perde: é lento para um marco de teste.
- *Sugestão:* (a) no marco 1. Depois, (b) com calma, numa tarefa à parte: o autor escolhe o pacote, guarda em `Jogo/assets/provisorio/` com o LEIA-ME da origem (Pipeline) e redesenha o `vila.tmj`. A ferramenta de atlas continua para depois: na fatia, um conjunto de tiles já é uma imagem só.
- *Conflitos:* nenhum.

Depois das respostas:

- ajustar as duas fichas, a Fatia Vertical, a Arquitetura e o Pipeline;
- tirar as marcas [Proposta] do que foi aceito.

## 5. Onde está cada coisa

| Preciso de | Onde |
|---|---|
| Índice dos sistemas | [[Descrição do Projeto]] |
| O que programar e como | [[Ficha Logica/Fatia Vertical\|Ficha Lógica: Fatia Vertical]] e as fichas de cada sistema em `Ficha Logica/` |
| Decisões técnicas, bibliotecas, marcos | [[Arquitetura/Descrição\|Arquitetura]] |
| Itens, receitas, preços | [[Sistema de Itens/Descrição\|Itens]], [[Sistema de Produção/Receitas\|Receitas]], [[Sistema Econômico/Descrição\|Econômico]] |
| Moldes de conteúdo | [[Arquitetura/Moldes de Conteúdo\|Moldes de Conteúdo]] |
| Termos | [[Glossário]] |
| Situação, números-chave e o que falta | [[Estado do Projeto]] |
| Histórico das revisões (não precisa ler) | `Documentos/Histórico/` |

## 6. Cuidados técnicos (para quem edita)

- **Ferramentas:** Python 3.11, sempre chamado com `-I`. Não há `gh` nem Visual Studio.
- **Edição em lote:**
  - um script Python troca trechos exatos e confere que cada trecho existe uma vez só;
  - rodar antes no modo de teste (`--dry`);
  - preservar as quebras de linha (CRLF ou LF) e o BOM de cada arquivo.
- **Links:** nenhum link interno do Obsidian pode ficar quebrado. Nas tabelas, o separador do texto do link é escapado (`\|`).
- **Console do Windows:** os acentos aparecem corrompidos só na tela; os arquivos estão em UTF-8.
- **Git:**
  - mensagem de commit com `git commit -F arquivo`;
  - `git fetch` antes de editar;
  - não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json`.
- **GitHub:** o navegador interno não está logado no GitHub, e o assistente nunca digita senhas.
- **Sessões na nuvem:** o `Respostas.txt` chega como anexo; trabalhar na branch indicada pela sessão.
