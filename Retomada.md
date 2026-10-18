> Ponto de partida de cada sessão. Curto de propósito: o detalhe de cada sistema está no documento dele (índice em [[Descrição do Projeto]]), e a situação geral no [[Estado do Projeto]]. Atualizado em 2026-10-18.

## 1. Onde paramos

- **Design completo:** todos os sistemas estão escritos, a revisão completa terminou, e a [[Ficha Logica/Fatia Vertical|Ficha Lógica: Fatia Vertical]] está pronta.
- **Rodada FT2 (2026-10-17):** o autor **liberou o código, marco a marco**.
- **Pastas reorganizadas (2026-10-17):** `Documentos/`, `Ficha Logica/`, `Codigo/` e `Jogo/` (seção 3).
- **Fichas do marco 1 (2026-10-17):** [[Ficha Logica/Tempo|Tempo]] e [[Ficha Logica/Mapa|Mapa]] (tiles e camadas) escritas, com a **rodada FL1 aplicada**. O marco 0 não tem ficha.
- **Marco 0 (2026-10-17):** na branch `codigo/marco-0-ambiente` (feita em cima da `docs/fichas-marco-1`). No primeiro teste do autor, a janela de 1600 × 900 não cabia na tela dele (1600 × 900, com 1600 × 860 livres) e o X ficou escondido; corrigido e **confirmado pelo autor** (seção 3).
- **Marco 1 (2026-10-17):** escrito na branch `codigo/marco-1-relogio-e-mapa` (feita em cima da `codigo/marco-0-ambiente`), em 5 passos; **confirmado pelo autor** (seção 3).
- **Ficha da Interface e marco 2 (2026-10-18):** na branch `codigo/marco-2-camera-e-entrada` (feita em cima da `codigo/marco-1-relogio-e-mapa`). A [[Ficha Logica/Interface|ficha da Interface]] (câmera, zoom e teclas) abriu a **rodada FL2** (seção 4), e o código já segue as sugestões dela. O marco 2 foi **testado e confirmado pelo autor**, e a **FL2 foi aplicada** (2026-10-18).
- **Arte provisória dos personagens (2026-10-18):** o autor baixou a versão grátis do pacote Mana Seed (pasta `Assents Temporarios/`, na raiz), só para os testes da fatia; a arte definitiva será própria. O repositório no GitHub é **público**, e o pacote proíbe, em geral, redistribuir os arquivos: por isso a pasta fica **fora do Git** (`.gitignore`). As folhas têm quadros de 64 × 64, e não o boneco de 16 × 24 da Arquitetura: isso é a pergunta FL4-4.
- **Fichas do marco 3 (2026-10-18):** [[Ficha Logica/Itens|Itens]] (com a ordem de leitura dos dados e o `--validar`), [[Ficha Logica/Recursos|Recursos]] e [[Ficha Logica/Receitas|Receitas]] (com o preço base), na branch `docs/fichas-marco-3` (feita em cima da `codigo/marco-2-camera-e-entrada`, para levar ao `main` os dois commits do marco 2 que ficaram de fora). A **rodada FL3** foi aplicada (2026-10-18).
- **Marco 3 (2026-10-18):** na branch `codigo/marco-3-dados` (feita do `main`, que já tem as fichas do marco 3); **confirmado pelo autor** (seção 3).
- **Fichas do marco 4 (2026-10-18):** a [[Ficha Logica/NPC|ficha do NPC]] (posição, movimento e o desenho do boneco) e a busca de caminho na [[Ficha Logica/Mapa|ficha do Mapa]] (regra 4.4), na branch `docs/fichas-marco-4` (feita em cima da `codigo/marco-3-dados`). A **rodada FL4** foi aplicada (2026-10-18), toda com "seguir sugestão".
- **Marco 4 (2026-10-18):** escrito na branch `codigo/marco-4-npc-andando` (feita do `main`, que já tem as fichas do marco 4); **confirmado pelo autor** (seção 3).
- **Ficha do marco 5 (2026-10-18):** a [[Ficha Logica/IA|ficha da IA]] (a vila inicial, a rotina e a Utilidade), na branch `docs/fichas-marco-5` (feita em cima da `codigo/marco-4-npc-andando`). Abriu a **rodada FL5** (seção 4).
- **Próximo passo:** o autor responde a FL5; depois, aplicar e começar o marco 5.
- **Ao retomar:** rodar `git fetch` e conferir se os pull requests foram mesclados no `main`. Tudo até as fichas do marco 4 já entrou no `main`; faltam o `codigo/marco-4-npc-andando` e, depois dele, o `docs/fichas-marco-5`.

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
  - interface personalizável (ele joga em 1600 × 900, que é o tamanho do monitor dele: só 1600 × 860 ficam livres acima da barra de tarefas);
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
- **Bibliotecas (FT2-2):** aprovadas. Cada uma entra no marco em que é usada e é baixada pelo CMake (`FetchContent`), em versão fixa. A exceção é o glad, gerado uma vez e guardado em `Jogo/externo/glad/` ([[Arquitetura/Descrição|Arquitetura]]):

| Marco | Bibliotecas |
|---|---|
| 0 e 1 | SDL3, glad (OpenGL 3.3), stb_image |
| 1 | nlohmann/json, antecipada do marco 3 para ler o mapa do Tiled (FL1-1); o CSV é lido por código próprio |
| 4 e 5 | EnTT |
| 7 | Tracy |
| 8 | Dear ImGui |
| Depois da fatia | miniaudio |

- **Ambiente (FT1-7):** g++ 15.2 (MinGW), CMake, Ninja e Git, já instalados; VS Code com as extensões "C/C++" e "CMake Tools"; depurador gdb; tudo neste repositório.
- **Marco 0 (pronto, confirmado pelo autor em 2026-10-17):** as extensões do VS Code já estavam instaladas; o `CMakeLists.txt` da raiz, o `CMakePresets.json` (configuração "padrao") e o `Jogo/main.cpp` abrem uma janela vazia com a SDL3 3.4.18, baixada pelo CMake e embutida no `jogo.exe`. Se 1600 × 900 não cabe na tela com a barra de título, a janela abre maximizada; F11 liga a tela cheia. Os comandos e o que testar estão no `Jogo/LEIA-ME.md`.
- **Marco 1 (pronto, confirmado pelo autor em 2026-10-18):** o relógio do mundo e a leitura do mapa no `Codigo/`, com 63 testes no `ctest`; o mapa de teste desenhado com o OpenGL; data, fase e velocidade no título da janela; teclas provisórias (Espaço, 1, 2, 3, F1, F11). O resumo está no `Jogo/LEIA-ME.md`.
- **Marco 2 (pronto, confirmado pelo autor em 2026-10-18):** a câmera em `Jogo/CameraDoMapa.cpp` (WASD ou setas, Shift, roda do mouse para o zoom com o ponto embaixo do mouse parado, arrastar com o botão do meio, limite ao mapa), com 18 testes num segundo programa, o `testes_do_jogo`.
- **Marco 4 (pronto, confirmado pelo autor em 2026-10-18):** um NPC no centro da vila anda até o tile do botão direito pela busca A\* (`Codigo/Mapa/BuscaDeCaminho.cpp`, `Codigo/Npc/`), com a EnTT 3.16.0; o boneco do Mana Seed (ou o de teste, sem a pasta) é desenhado entre os ticks; N cria mais NPCs e F2 mostra os caminhos. 167 testes no `Codigo/` e 36 no `Jogo/`.
- **Marco 3 (pronto, confirmado pelo autor em 2026-10-18):** os dados em `Jogo/data` (itens, receitas, recursos, construções, regras e textos), lidos e conferidos por `Codigo/Dados/`, com o preço base (Pão 1,7) e o `jogo --validar`; as teclas passam pela tabela de ações (FL2-4). 124 testes no `Codigo/` e 25 no `Jogo/`.

## 4. Rodada pendente: FL5, a rotina (marco 5)

**Perguntas enviadas em 2026-10-18, aguardando respostas.** Ao escrever a [[Ficha Logica/IA|ficha da IA]] (a vila inicial, a rotina e a Utilidade), apareceram 6 pontos que os documentos não decidem. A ficha já usa as sugestões, marcadas como [Proposta]. (A FL4 foi aplicada em 2026-10-18, toda com "seguir sugestão".)

Responder no `Respostas.txt` pelos códigos (FL5-1, FL5-2...). "Seguir sugestão" aceita a proposta.

**FL5-1. As construções no marco 5.**

- *Contexto:* a rotina manda o NPC dormir em casa e trabalhar na construção do emprego. Mas as construções só "funcionam" no marco 6, com a ficha da Construção (custo, obra, vagas). O mapa tem 11 lotes vazios (8 pequenos e 3 médios), e a ficha do Mapa dizia que as construções bloqueiam os tiles do lote só a partir do marco 6.
- *Pergunta:* onde o NPC dorme e trabalha no marco 5, e o que aparece na tela? *Exemplo:* às 22h, a Ana volta para casa. Para onde ela anda, e o que se vê?
- *Opções:*
  - (a) **um `vila_inicial.json` que já põe as 10 construções nos lotes** (4 Casas, Fazenda, Posto de Coleta, Cozinheiro, Mercado, Construtor e a Carpintaria do jogador; o lote 5 fica livre), cada uma desenhada como um **retângulo de cor lisa** por tipo, com a porta mais escura. Os tiles do lote passam a **bloquear** já no marco 5, e a **porta** é o tile logo abaixo do meio do lote. O NPC anda até a porta, entra e some da tela até sair. Ganha: a vila já fica com a cara final, e o marco 6 só acrescenta o que a construção faz. Perde: a regra do bloqueio, que era do marco 6, vem antes; e retângulos no lugar da arte;
  - (b) sem construções: o NPC anda até o meio do lote vazio e fica parado lá. Ganha: o marco 5 fica menor. Perde: 4 NPCs parados em cima de cada lote de Casa, e o marco 6 refaz tudo;
  - (c) esperar a ficha da Construção e fazer o marco 6 antes do 5. Ganha: as construções nascem completas. Perde: muda a ordem dos marcos da V4-6, e o marco 6 fica enorme.
- *Sugestão:* (a), com a vila da tabela da ficha da IA (parte 2): as oficinas e o Mercado nos 3 lotes médios, o resto nos pequenos, a Fazenda e o Posto de Coleta do lado esquerdo, perto do campo de Grão e do bosque.
- *Conflitos:* a regra 4.2 da ficha do Mapa (o lote ocupado bloqueia) passa do marco 6 para o marco 5. A arte das construções continua sem escolha ([[Estado do Projeto]], "O que falta").

**FL5-2. Os 15 NPCs da vila inicial.**

- *Contexto:* a fatia tem 15 Civis (V2-12), 4 Casas de 4 lugares, e as vagas do nível 1: Fazenda 2, Posto de Coleta 2, Cozinheiro 2, Mercado 2 Lojistas, Construtor 2 Operários, e a Carpintaria do jogador com 2 vagas (FT1-1). A FT1-1 diz que o jogador "pode contratar desempregados", mas não diz se a Carpintaria já começa com empregados.
- *Pergunta:* quem mora onde, quem trabalha onde, e quantos ficam sem emprego? *Exemplo:* no primeiro dia, às 10h, quantos NPCs passeiam na rua?
- *Opções:*
  - (a) **12 empregados e 3 desempregados**: as 10 vagas da cidade cheias e **a Carpintaria começa com 2 empregados**; 4, 4, 4 e 3 moradores nas Casas; nomes provisórios (Ana, Bento, Clara...). Ganha: os salários do dia dão cerca de 85, a conta da Arquitetura (V2-12), e 3 desempregados fazem o desemprego aparecer na Felicidade (20%: +6 no fator). Perde: o jogador não começa contratando;
  - (b) 10 empregados e 5 desempregados (a Carpintaria vazia). Ganha: o jogador contrata os 2 primeiros, um bom começo de tutorial. Perde: os salários caem para cerca de 69 por dia, e o desemprego de 33% pesa na Felicidade desde o primeiro dia;
  - (c) 15 empregados (mais vagas). Ganha: ninguém parado. Perde: precisa de vagas que o nível 1 não tem.
- *Sugestão:* (a). Às 10h do primeiro dia, só os 3 desempregados passeiam. Os nomes de verdade esperam a história do mundo.
- *Conflitos:* nenhum. A FT1-1 ("pode contratar desempregados") continua valendo: os 3 desempregados são os que o jogador pode contratar quando a Carpintaria subir de nível.

**FL5-3. O Período Livre, o Comprar e onde ficam os números.**

- *Contexto:* a Utilidade da V2-10 dá 1,0 à ação do período da rotina, +1,2 ao Comprar (uma vez a cada 2 dias, no Período Livre) e +0,5 × (1 − Felicidade ÷ 100) ao Lazer. Mas ela não diz **qual ação** é a do Período Livre. Se for o Lazer, ele tem 1,0 + 0,25 (com a Felicidade 50) = 1,25, e o Comprar, com 1,2, **nunca ganha**.
- *Pergunta:* qual é a ação do Período Livre, para o Comprar acontecer? *Exemplo:* às 15h, a Ana sai do trabalho no dia de compra. Ela vai ao Mercado ou passeia?
- *Opções:*
  - (a) **uma ação nova, Passear, é a do Período Livre (1,0)**; o Comprar (1,2) ganha dela no dia de compra; o **Lazer fica fora da fatia** (ele precisa da Taverna, que a fatia não tem) e volta com a Taverna, com a nota revista. E o desempregado, no período de trabalho, também passeia. Ganha: o Comprar acontece, e cada ação tem um lugar claro. Perde: uma ação a mais na lista da V2-10;
  - (b) o Lazer é a ação do Período Livre, e o Comprar sobe para +1,5. Ganha: fica perto da V2-10. Perde: o Lazer sem Taverna vira só um passeio com outro nome, e o número do Comprar muda;
  - (c) o Comprar ganha sempre no dia de compra, fora das notas. Ganha: simples. Perde: uma exceção à Utilidade.
- *Sugestão:* (a). E todos os números da rotina, das notas e do passeio vão para o `regras.json`, com as notas em centésimos (1,0 = 100), inteiras, como o resto. A reavaliação "a cada 5 minutos de jogo" vira a cada **67 ticks** (66,7 arredondado), um NPC por tick.
- *Conflitos:* a lista de ações da Fatia Vertical (parte 1) ganha o Passear e perde o Lazer na fatia; o [[Sistema de IA/Descrição|IA]] ganha uma nota.

**FL5-4. Onde o NPC come.**

- *Contexto:* a V2-10 dá +1,5 ao Comer nas refeições (7h, 12h e 19h, por 30 minutos). O que se come entra no marco 6 (o consumo é da cidade); no marco 5, comer é só um lugar e um tempo.
- *Pergunta:* para onde o NPC vai na hora de comer? *Exemplo:* às 19h, a Ana está passeando a 10 tiles de casa. E às 12h, ela está dentro da Fazenda.
- *Opções:*
  - (a) **quem está dentro da casa ou do trabalho come lá; quem está na rua vai para casa** e come até o fim da meia hora. No exemplo: às 12h ela come na Fazenda; às 19h, anda uns 5 minutos de jogo até em casa e come lá. Ganha: o almoço não tira ninguém do trabalho, e a tela mostra a vila voltando para casa no jantar. Perde: quem está longe come menos tempo;
  - (b) come onde estiver, mesmo no meio da rua. Ganha: o mais simples. Perde: NPCs parados meia hora no meio do caminho, sem motivo visível;
  - (c) todos comem no Mercado. Ganha: movimenta o Mercado. Perde: 15 NPCs no Mercado três vezes por dia, e o almoço tira todos do trabalho.
- *Sugestão:* (a).
- *Conflitos:* com o café às 7h, o empregado chega ao trabalho por volta das 7h35 e trabalha cerca de **7,4 horas**, e não 8. A produção do marco 6 é por hora presente, então isso aparece no teste de 1 ano, e a rodada do marco 7 decide se ajusta.

**FL5-5. O passeio e o sorteio.**

- *Contexto:* "passear" (o marco 5 é "dormem, trabalham e passeiam") precisa de lugares sorteados. A Arquitetura e o Eventos pedem números sorteados **com semente**: a mesma semente dá a mesma partida.
- *Pergunta:* para onde o NPC passeia, por quanto tempo, e com que sorteio? *Exemplo:* às 15h30, depois de comprar, a Ana sai do Mercado para passear.
- *Opções:*
  - (a) **um tile livre sorteado a até 12 tiles de onde ele está, com uma parada de 15 a 45 minutos sorteados, e depois outro**; o sorteio é um **gerador próprio pequeno, o PCG32**, ligado à semente da partida (a 1, no `vila_inicial.json`), com **um gerador para os NPCs** e outro, depois, para os eventos. Ganha: a vila parece viva; e o PCG32 dá os mesmos números em qualquer compilador, o que o sorteio do C++ não garante. Perde: cerca de 15 linhas de código a mais;
  - (b) pontos de passeio marcados no Tiled (a praça, o poço). Ganha: os NPCs se juntam em lugares que fazem sentido. Perde: uma camada de objetos nova, e o mapa de teste ainda não tem praça;
  - (c) sem passeio: no Período Livre, o NPC fica em casa. Ganha: simples. Perde: a vila fica vazia das 15h às 22h.
- *Sugestão:* (a). Os pontos de passeio do Tiled (b) podem entrar depois, com a arte do mapa, junto da camada `pontos_de_interesse` que o Pipeline já prevê.
- *Conflitos:* nenhum. O gerador separado por sistema é o que o [[Sistema de Eventos/Descrição|Eventos]] pede ("um gerador próprio").

**FL5-6. Como testar o marco 5.**

- *Contexto:* o marco 5 está pronto quando "os 15 NPCs dormem, trabalham e passeiam nos horários". Ainda não há texto na tela (o HUD é do marco 8). As teclas do marco 4 (o botão direito manda os NPCs; o N cria mais) brigam com a rotina.
- *Pergunta:* como o autor confere o que cada NPC está fazendo, e o que acontece com as teclas do marco 4? *Exemplo:* às 15h05, como saber se a Ana foi comprar ou passear?
- *Opções:*
  - (a) **a tecla F3 liga um quadradinho colorido em cima de cada NPC, com a cor da ação, e escreve no console a tabela dos 15** (nome, ação, onde está, as notas); **o botão direito e o N saem**, e o F2 (os caminhos) fica. Ganha: dá para ver a vila inteira de uma vez e conferir os números no console. Perde: as cores precisam de uma legenda (no LEIA-ME);
  - (b) só o console, escrevendo cada troca de ação. Ganha: nada na tela. Perde: cerca de 100 linhas por dia de jogo no console, difícil de acompanhar;
  - (c) manter o botão direito como uma ordem que vale até a próxima reavaliação. Ganha: o teste do marco 4 continua. Perde: a ordem dura no máximo 5 minutos de jogo e confunde o teste da rotina.
- *Sugestão:* (a). Para ver um dia inteiro, a tecla 3 (4x) faz um dia passar em 4 minutos reais.
- *Conflitos:* nenhum; as teclas de teste são provisórias (FL4-6).

Depois das respostas:

- tirar as marcas [Proposta] das fichas da IA, do NPC e do Mapa, e anotar a rodada na parte 8 da Fatia Vertical (com o Passear na lista de ações, FL5-3);
- começar o marco 5, na branch `codigo/marco-5-rotina`.

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
  - preservar as quebras de linha (CRLF ou LF) e o BOM de cada arquivo. Vários documentos estão em CRLF: num trecho de várias linhas, trocar o `
` pela quebra do arquivo antes de procurar.
- **Links:** nenhum link interno do Obsidian pode ficar quebrado. Nas tabelas, o separador do texto do link é escapado (`\|`).
- **Console do Windows:** os acentos aparecem corrompidos só na tela; os arquivos estão em UTF-8.
- **Git:**
  - mensagem de commit com `git commit -F arquivo`;
  - `git fetch` antes de editar;
  - não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json`.
- **GitHub:** o navegador interno não está logado no GitHub, e o assistente nunca digita senhas.
- **Sessões na nuvem:** o `Respostas.txt` chega como anexo; trabalhar na branch indicada pela sessão.
