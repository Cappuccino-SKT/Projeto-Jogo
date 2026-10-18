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
- **Fichas do marco 4 (2026-10-18):** a [[Ficha Logica/NPC|ficha do NPC]] (posição, movimento e o desenho do boneco) e a busca de caminho na [[Ficha Logica/Mapa|ficha do Mapa]] (regra 4.4), na branch `docs/fichas-marco-4` (feita em cima da `codigo/marco-3-dados`). Abriram a **rodada FL4** (seção 4).
- **Próximo passo:** o autor responde a FL4; depois, aplicar e começar o marco 4.
- **Ao retomar:** rodar `git fetch` e conferir se os pull requests foram mesclados no `main`. Os marcos 0 a 2 e as fichas até o marco 3 já entraram; faltam o `codigo/marco-3-dados` e, depois dele, o `docs/fichas-marco-4`.

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
- **Marco 3 (pronto, confirmado pelo autor em 2026-10-18):** os dados em `Jogo/data` (itens, receitas, recursos, construções, regras e textos), lidos e conferidos por `Codigo/Dados/`, com o preço base (Pão 1,7) e o `jogo --validar`; as teclas passam pela tabela de ações (FL2-4). 124 testes no `Codigo/` e 25 no `Jogo/`.

## 4. Rodada pendente: FL4, o NPC andando (marco 4)

**Perguntas enviadas em 2026-10-18, aguardando respostas.** Ao escrever a [[Ficha Logica/NPC|ficha do NPC]] (posição e movimento) e a busca de caminho na [[Ficha Logica/Mapa|ficha do Mapa]] (regra 4.4), apareceram 6 pontos que os documentos não decidem. As fichas já usam as sugestões, marcadas como [Proposta]. (A FL3 foi aplicada em 2026-10-18, toda com "seguir sugestão".)

Responder no `Respostas.txt` pelos códigos (FL4-1, FL4-2...). "Seguir sugestão" aceita a proposta.

**FL4-1. A velocidade de andar do NPC.**

- *Contexto:* a regra 4.10 da Fatia Vertical diz só "na velocidade de andar". A [[Sistema de Viagem/Descrição|Viagem]] diz que atravessar um Mapa Local a pé leva cerca de 1 hora de jogo, e o Mapa Local do jogo completo tem 128 tiles. O NPC anda pelos ticks: pausado ele para, e a 4x anda 4 vezes mais rápido.
- *Pergunta:* quantos tiles o NPC anda por hora de jogo? *Exemplo:* um Civil sai de casa às 6h e vai à Fazenda, a 20 tiles. Quanto tempo de jogo ele gasta, e quanto tempo isso leva na tela?
- *Opções:*
  - (a) **128 tiles por hora** (160 milésimos de tile por tick): 3,2 tiles por segundo a 1x; os 20 tiles levam 9 min de jogo, ou 6 s reais. Ganha: bate com a Viagem, e é um passo de caminhada fácil de ver. Perde: a 4x, são 12,8 tiles por segundo, rápido para seguir com os olhos;
  - (b) 64 tiles por hora: 1,6 tiles por segundo; os 20 tiles levam 19 min de jogo, ou 12,5 s reais. Ganha: calmo e fácil de seguir. Perde: contradiz a Viagem (2 horas por mapa), e o trajeto come mais da hora livre do Civil entre acordar (6h) e trabalhar (7h);
  - (c) 192 tiles por hora: 4,8 tiles por segundo; os 20 tiles levam 6 min de jogo. Ganha: ágil. Perde: numa vila de 64 tiles, os NPCs parecem correr, e o tempo de trajeto quase some.
- *Sugestão:* (a), no `regras.json` como `velocidade_de_andar_em_tiles_por_hora: 128`, para os testes poderem mudar. A animação troca de quadro a cada 0,4 tile andado (125 milissegundos por quadro a 1x). A Agilidade mudando a velocidade fica para depois da fatia.
- *Conflitos:* nenhum. A Viagem ("1 hora por mapa a pé") ganha o número exato.

**FL4-2. A diagonal na quina de um obstáculo.**

- *Contexto:* a FT1-3 decidiu a busca A\* em 8 direções, com a diagonal custando 1,4. Não decidiu se o NPC pode passar na diagonal rente a um tile bloqueado.
- *Pergunta:* com o NPC em (4, 6) e uma pedra em (5, 6), ele pode ir direto para (5, 5), raspando a quina da pedra? *Exemplo:* o tronco de uma árvore na esquina de uma rua.
- *Opções:*
  - (a) **a diagonal só vale com os dois tiles retos do lado livres.** Ganha: o boneco nunca atravessa a quina de uma árvore ou de uma parede desenhada. Perde: caminhos um pouco mais longos perto dos obstáculos (no teste da ficha, custo 104 em vez de 98);
  - (b) basta um dos dois tiles livre. Ganha: caminhos mais curtos. Perde: o boneco passa por cima de meio tronco ou da ponta da parede;
  - (c) a diagonal sempre vale. Ganha: o caminho mais curto. Perde: o NPC passa entre duas árvores encostadas na diagonal, por um "buraco" que não existe na tela.
- *Sugestão:* (a).
- *Conflitos:* nenhum.

**FL4-3. O destino sem caminho.**

- *Contexto:* o jogador (no marco 4) ou a rotina (no marco 5) pode pedir um destino bloqueado: um tile de água, uma árvore ou, no marco 6, o meio de uma construção.
- *Pergunta:* o que o NPC faz quando não há caminho até o destino? *Exemplo:* clicar no meio do lago da vila, em (50, 50).
- *Opções:*
  - (a) **vai até o tile mais perto do destino a que dá para chegar**, e o console avisa. No exemplo, até (56, 50), a margem leste. Ganha: o NPC sempre reage, e "ir até a construção" funciona mesmo sem marcar a porta. Perde: uma busca sem caminho examina o mapa quase todo (3.886 tiles na vila, menos de 1 milissegundo); e "o mais perto" é em linha reta, então às vezes fica do outro lado do obstáculo, como no exemplo;
  - (b) não vai: fica parado, com um aviso. Ganha: simples e claro. Perde: todo destino da rotina precisa ser um tile livre, escolhido com cuidado nos dados;
  - (c) primeiro procura o tile livre mais perto do destino, e só então busca o caminho até ele. Ganha: a margem fica do lado certo mais vezes. Perde: duas buscas; e, se esse tile estiver numa ilha, continua sem caminho.
- *Sugestão:* (a). No marco 6, cada construção pode ganhar um tile de entrada (a ficha da Construção decide), e a regra (a) fica como rede de segurança.
- *Conflitos:* nenhum.

**FL4-4. A arte do boneco no marco 4.**

- *Contexto:* a [[Arquitetura/Descrição|Arquitetura]] e o [[Pipeline de Arte e Conteúdo/Descrição|Pipeline]] dizem: boneco de 16 × 24 (1 tile de largura e 1,5 de altura), 3 direções desenhadas e a esquerda espelhada, andar com 4 quadros. A arte provisória que o autor baixou (Mana Seed, grátis) é diferente: folhas de 512 × 512 com quadros de 64 × 64, o boneco com cerca de **14 × 29** pixels dentro do quadro (quase 1 × 2 tiles), 4 direções desenhadas, andar com 6 quadros e camadas separadas (corpo, roupa, cabelo). E ela não pode entrar no Git, porque o repositório é público.
- *Pergunta:* o que o marco 4 desenha, e como o jogo acha os arquivos? *Exemplo:* alguém baixa o repositório em outro PC, sem a pasta `Assents Temporarios/`, e abre o jogo.
- *Opções:*
  - (a) **o boneco descrito num arquivo de dados** (`data/personagens/boneco_provisorio.json`: quadro de 64, pés em (32, 43), a linha de cada animação, 6 quadros de andar e as 3 camadas), lido direto da pasta `Assents Temporarios/`. Se faltar alguma camada, o jogo usa um **boneco de teste** de cores lisas, no mesmo formato, feito pelo script da arte de teste e guardado no Git, e mostra um aviso. Ganha: o autor já vê o Mana Seed andando no marco 4; a arte própria de 16 × 24 só troca os números do arquivo; e o jogo abre em qualquer PC. Perde: o boneco provisório fica com quase 2 tiles de altura, maior que o da Arquitetura; e um arquivo de dados a mais;
  - (b) só o boneco de teste no marco 4, e o Mana Seed num marco depois. Ganha: o marco 4 fica menor. Perde: o autor não vê a arte de verdade andando, que foi o motivo de baixar o pacote;
  - (c) recortar o Mana Seed para 16 × 24. Ganha: bate com a Arquitetura. Perde: o boneco tem 29 pixels de altura e não cabe; cortaria a cabeça ou os pés.
- *Sugestão:* (a). O provisório usa as 4 direções desenhadas (sem espelho); o espelhamento fica para a arte própria. Os quadros de 64 × 64 têm muito espaço vazio, mas isso só pesa na placa de vídeo: cada folha tem 1 MB, e 3 camadas, 3 MB.
- *Conflitos:* o boneco de 16 × 24 da Arquitetura e do Pipeline continua valendo para a arte própria; os dois documentos ganham uma nota sobre o tamanho do provisório.

**FL4-5. O movimento suave entre os ticks.**

- *Contexto:* o relógio roda 20 ticks por segundo a 1x, e a tela mostra 60 quadros por segundo. O NPC anda 0,16 tile (2,6 pixels do mundo) por tick.
- *Pergunta:* a tela mostra o NPC só onde ele estava no último tick, ou entre um tick e outro? *Exemplo:* a 3x, com o NPC andando para a direita e a câmera parada.
- *Opções:*
  - (a) **desenhar entre a posição do tick anterior e a do atual**, pela fração do tick que já passou (o relógio já guarda esse tempo, no acumulador). Ganha: o movimento fica liso, a 60 quadros. Perde: cada NPC guarda mais uma posição, e a imagem fica até 50 milissegundos atrás da simulação, o que não se percebe;
  - (b) desenhar só a posição do tick. Ganha: mais simples. Perde: a 3x, o boneco pula 7,7 pixels da tela 20 vezes por segundo e parece tremido, pior ainda com a câmera andando;
  - (c) subir para 60 ticks por segundo. Ganha: liso sem conta a mais. Perde: muda o "1 tick = 4,5 s" da Fatia Vertical e triplica o custo da simulação, contra a meta de 500 NPCs.
- *Sugestão:* (a). Vale também, depois, para o personagem do jogador.
- *Conflitos:* nenhum.

**FL4-6. Como testar o marco 4.**

- *Contexto:* o marco 4 está pronto quando "um NPC vai de um ponto a outro pela busca de caminho". Até a rotina (marco 5), alguém precisa dar o destino. A [[Interface/Descrição|Interface]] diz que o botão esquerdo seleciona e o direito dá ordem.
- *Pergunta:* como o autor manda o NPC andar e confere o caminho? *Exemplo:* clicar do outro lado do lago e ver o NPC contornar a margem.
- *Opções:*
  - (a) **o botão direito manda o NPC ao tile clicado; o F2 pinta o caminho de azul; a tecla N cria mais um NPC** (até 50), e o botão direito manda todos. Ganha: testa o caminho, a quina, o lago e vários NPCs passando um pelo outro. Perde: duas teclas de teste a mais na tabela de ações, que saem depois;
  - (b) só o botão direito, com 1 NPC. Ganha: o mínimo. Perde: não testa vários NPCs nem mostra o caminho;
  - (c) o NPC anda sozinho entre pontos sorteados, sem clique. Ganha: nada a fazer. Perde: o autor não escolhe o caso a testar.
- *Sugestão:* (a). Selecionar um NPC com o botão esquerdo fica para depois da fatia. A tecla N ainda serve, no marco 7, para encher a vila de NPCs no teste de desempenho.
- *Conflitos:* nenhum. O N e o F2 são provisórios, como o F1.

Depois das respostas:

- tirar as marcas [Proposta] das fichas do NPC e do Mapa, anotar a rodada na parte 8 da Fatia Vertical e pôr a nota do boneco provisório na Arquitetura e no Pipeline (FL4-4);
- começar o marco 4, na branch `codigo/marco-4-npc-andando`.

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
