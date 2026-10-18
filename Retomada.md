> Ponto de partida de cada sessão. Curto de propósito: o detalhe de cada sistema está no documento dele (índice em [[Descrição do Projeto]]), e a situação geral no [[Estado do Projeto]]. Atualizado em 2026-10-17.

## 1. Onde paramos

- **Design completo:** todos os sistemas estão escritos, a revisão completa terminou, e a [[Ficha Logica/Fatia Vertical|Ficha Lógica: Fatia Vertical]] está pronta.
- **Rodada FT2 (2026-10-17):** o autor **liberou o código, marco a marco**.
- **Pastas reorganizadas (2026-10-17):** `Documentos/`, `Ficha Logica/`, `Codigo/` e `Jogo/` (seção 3).
- **Fichas do marco 1 (2026-10-17):** [[Ficha Logica/Tempo|Tempo]] e [[Ficha Logica/Mapa|Mapa]] (tiles e camadas) escritas, com a **rodada FL1 aplicada**. O marco 0 não tem ficha.
- **Marco 0 (2026-10-17):** na branch `codigo/marco-0-ambiente` (feita em cima da `docs/fichas-marco-1`). No primeiro teste do autor, a janela de 1600 × 900 não cabia na tela dele (1600 × 900, com 1600 × 860 livres) e o X ficou escondido; corrigido e **confirmado pelo autor** (seção 3).
- **Marco 1 (2026-10-17):** escrito na branch `codigo/marco-1-relogio-e-mapa` (feita em cima da `codigo/marco-0-ambiente`), em 5 passos; **confirmado pelo autor** (seção 3).
- **Ficha da Interface e marco 2 (2026-10-18):** na branch `codigo/marco-2-camera-e-entrada` (feita em cima da `codigo/marco-1-relogio-e-mapa`). A [[Ficha Logica/Interface|ficha da Interface]] (câmera, zoom e teclas) abriu a **rodada FL2** (seção 4), e o código já segue as sugestões dela. Falta o autor responder a FL2 e testar o marco 2.
- **Próximo passo:** as respostas da FL2 e o teste do marco 2; depois, as fichas de Itens, Recursos e Receitas e o marco 3.
- **Ao retomar:** rodar `git fetch` e conferir se os pull requests foram mesclados no `main`. As fichas do marco 1 e o marco 0 já entraram; falta o `codigo/marco-1-relogio-e-mapa`, e depois o do marco 2.

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
- **Marco 2 (escrito, falta o autor confirmar):** a câmera em `Jogo/CameraDoMapa.cpp` (WASD ou setas, Shift, roda do mouse para o zoom com o ponto embaixo do mouse parado, arrastar com o botão do meio, limite ao mapa), com 18 testes num segundo programa, o `testes_do_jogo`.

## 4. Rodada pendente: FL2, a câmera do marco 2

**Perguntas enviadas em 2026-10-18, aguardando respostas.** Ao escrever a [[Ficha Logica/Interface|ficha da Interface]] (câmera, zoom e teclas), apareceram 4 pontos que os documentos não decidem. A FT1-4 já decidiu o principal: WASD ou setas, a roda do mouse para o zoom em 2x, 3x e 4x (padrão 3x) e arrastar com o botão do meio. **O código do marco 2 já segue as sugestões abaixo**; se a resposta for outra, a mudança é pequena. (A FL1 foi aplicada em 2026-10-17, toda com "seguir sugestão".)

Responder no `Respostas.txt` pelos códigos (FL2-1, FL2-2...). "Seguir sugestão" aceita a proposta.

**FL2-1. A velocidade da câmera no teclado.**

- *Contexto:* a câmera anda com WASD ou com as setas (FT1-4), mas a velocidade não foi decidida. O mapa da vila tem 64 tiles de lado.
- *Pergunta:* quantos tiles por segundo a câmera anda, e o que muda no zoom, no Shift e na diagonal? *Exemplo:* a 3x, segurando D, quanto tempo leva para ir da Fazenda (à esquerda) até o lago (à direita), uns 40 tiles?
- *Opções:*
  - (a) **velocidade fixa na tela: 900 pixels da tela por segundo**, em qualquer zoom (a 3x, cerca de 19 tiles por segundo: 2 segundos da Fazenda ao lago; a 2x, 28 tiles por segundo). Shift dobra a velocidade. Na diagonal, a mesma velocidade das retas. Ganha: parece igual em todo zoom. Perde: no zoom 2x, os tiles passam mais rápido;
  - (b) velocidade fixa no mundo (por exemplo, 20 tiles por segundo em qualquer zoom). Ganha: o mundo anda sempre igual. Perde: no 4x a tela parece correr, e no 2x parece lenta;
  - (c) a câmera acelera devagar e freia devagar (com "inércia"). Ganha: movimento suave. Perde: menos preciso para quem quer parar num tile exato.
- *Sugestão:* (a), com Shift × 2 e a diagonal corrigida (× 0,7071). Os números ficam numa tabela do código e, no futuro, no perfil do jogador, para o jogador ajustar.
- *Conflitos:* nenhum.

**FL2-2. O zoom, o limite do mapa e onde a câmera começa.**

- *Contexto:* o zoom troca de nível pela roda (FT1-4). Falta decidir em que ponto ele se fixa, se a câmera pode sair do mapa e onde ela começa.
- *Pergunta:* *exemplo:* o mouse está em cima do poço e o jogador gira a roda para o 4x. O poço continua embaixo do mouse, ou vai para outro lugar da tela? E se o jogador segura A por muito tempo, a câmera mostra o vazio além da borda?
- *Opções do zoom:*
  - (a) **o ponto embaixo do mouse fica parado**, como em Stardew Valley e RimWorld. Ganha: dá para "mergulhar" no que se quer ver. Perde: nada relevante;
  - (b) o zoom é sempre no centro da tela. Ganha: mais simples. Perde: o jogador precisa mover a câmera depois de cada zoom.
- *Opções do limite:*
  - (a) **a borda do mapa nunca sai da borda da tela**; se o mapa ficar menor que a janela (janela enorme no 2x), ele fica centralizado. Ganha: nunca aparece fundo vazio. Perde: o tile da borda nunca fica no centro da tela;
  - (b) a câmera pode passar da borda até meia tela. Ganha: dá para centralizar a borda. Perde: aparece o fundo preto.
- *Sugestão:* zoom (a), limite (a), e a câmera **começa com o centro do mapa no centro da tela**, em 3x (no marco 6, pode começar na Carpintaria do jogador). A posição da câmera **não vai no save**: ao carregar, ela recomeça no centro. Gravar a posição é fácil depois, se o autor quiser.
- *Conflitos:* nenhum.

**FL2-3. Mover a câmera pela borda da tela.**

- *Contexto:* a [[Interface/Descrição|Interface]] diz, na seção "Controles", "Câmera: WASD, bordas da tela ou arrastar com o botão do meio". A FT1-4, que veio depois, só cita WASD, setas e o botão do meio.
- *Pergunta:* encostar o mouse na borda da tela move a câmera? *Exemplo:* o autor joga em janela, com outra janela ao lado. Levar o mouse até a outra janela passa pela borda direita do jogo, e a câmera anda sem ele querer.
- *Opções:*
  - (a) **não na fatia; no futuro, como opção nas configurações, desligada por padrão**. Ganha: nada acontece sem querer, em janela. Perde: quem gosta de jogar assim (estilo RTS clássico) espera um pouco;
  - (b) sim, ligado, com uma faixa de 8 pixels. Ganha: o jeito RTS. Perde: em janela, a câmera foge toda hora;
  - (c) sim, mas só em tela cheia (F11). Ganha: sem o problema da janela. Perde: um comportamento que muda sem o jogador saber por quê.
- *Sugestão:* (a). A linha da Interface passa a dizer "WASD ou setas, ou arrastar com o botão do meio; mover pela borda da tela é uma opção futura, desligada por padrão".
- *Conflitos:* a frase da Interface, corrigida como acima.

**FL2-4. As teclas e os atalhos remapeáveis.**

- *Contexto:* a Interface quer atalhos remapeáveis, guardados no perfil do jogador. O perfil só aparece no marco 8 ou depois. Até lá, as teclas provisórias do marco 1 (Espaço, 1, 2, 3, F1, F11) e as da câmera ficam no código.
- *Pergunta:* como guardar as teclas até o perfil existir, sem ter que procurar tecla por tecla no código depois?
- *Opções:*
  - (a) **uma tabela de ações no código** (Pausar → Espaço; Velocidade 1x → 1; Mover para cima → W ou ↑; e assim por diante), e o laço principal pergunta "a ação Pausar foi apertada?", nunca "o Espaço foi apertado?". No perfil, a tabela passa a ser lida do arquivo. Ganha: trocar uma tecla é mudar uma linha, e o remapear futuro já tem onde se encaixar. Perde: um pouco mais de código agora;
  - (b) as teclas direto no código, como no marco 1, e a tabela só no marco 8. Ganha: mais simples agora. Perde: refazer o laço principal depois.
- *Sugestão:* (a), começando no **marco 3**: o marco 2 ainda usa as teclas direto, porque são poucas, e a tabela entra com o próximo grupo de teclas. As letras usam a posição no teclado (o W é a tecla do W em qualquer layout).
- *Conflitos:* nenhum.

Depois das respostas:

- tirar as marcas [Proposta] da ficha da Interface e corrigir a seção "Controles" da Interface (FL2-3);
- ajustar o código do marco 2 se alguma resposta for diferente da sugestão.

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
