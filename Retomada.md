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
- **Ficha do marco 5 (2026-10-18):** a [[Ficha Logica/IA|ficha da IA]] (a vila inicial, a rotina e a Utilidade), na branch `docs/fichas-marco-5` (feita em cima da `codigo/marco-4-npc-andando`). A **rodada FL5** foi aplicada (2026-10-18), toda com "seguir sugestão".
- **Arte (2026-10-18):** o autor testou gerar a arte com o Gemini e **abandonou a ideia**; ele vai **desenhar à mão, no Pixelorama**, com a paleta **Resurrect 64** (mais algumas cores, se precisar). O [[Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]] junta todos os critérios, na branch `docs/guia-da-arte` (feita em cima da `codigo/marco-5-rotina`), e abriu a **rodada FA1** (seção 4).
- **Marco 5 (2026-10-18):** escrito na branch `codigo/marco-5-rotina` (feita do `main`, que já tem a ficha do marco 5); **confirmado pelo autor** (seção 3).
- **Próximo passo:** as fichas do marco 6 (Produção, Inventário, Trabalho e Propriedade, Econômico e Construção).
- **Ao retomar:** rodar `git fetch` e conferir se os pull requests foram mesclados no `main`. Tudo até a ficha do marco 5 já entrou no `main`; falta o `codigo/marco-5-rotina`.

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
- **Marco 5 (pronto, confirmado pelo autor em 2026-10-18):** a vila inicial (`data/vila_inicial.json`: 10 construções em retângulos de cor, que bloqueiam o lote, e 15 Civis com casa e emprego) e a rotina com a Utilidade (`Codigo/Npc/RotinaDosNpcs.cpp`, `Codigo/Vila/`), com o sorteio PCG32 (`Codigo/Tempo/SorteioComSemente.cpp`); F3 mostra as ações. 193 testes no `Codigo/` (com um dia inteiro sem tela) e 37 no `Jogo/`.
- **Marco 4 (pronto, confirmado pelo autor em 2026-10-18):** um NPC no centro da vila anda até o tile do botão direito pela busca A\* (`Codigo/Mapa/BuscaDeCaminho.cpp`, `Codigo/Npc/`), com a EnTT 3.16.0; o boneco do Mana Seed (ou o de teste, sem a pasta) é desenhado entre os ticks; N cria mais NPCs e F2 mostra os caminhos. 167 testes no `Codigo/` e 36 no `Jogo/`.
- **Marco 3 (pronto, confirmado pelo autor em 2026-10-18):** os dados em `Jogo/data` (itens, receitas, recursos, construções, regras e textos), lidos e conferidos por `Codigo/Dados/`, com o preço base (Pão 1,7) e o `jogo --validar`; as teclas passam pela tabela de ações (FL2-4). 124 testes no `Codigo/` e 25 no `Jogo/`.

## 4. Rodada pendente: FA1, os critérios da arte

**Perguntas enviadas em 2026-10-18, aguardando respostas.** Ao escrever o [[Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]], apareceram 5 critérios que os documentos não decidem. O guia já usa as sugestões, marcadas como [Proposta], então dá para começar a desenhar (o chão, primeiro) enquanto a rodada não volta. (A FL5 foi aplicada em 2026-10-18, toda com "seguir sugestão".)

Responder no `Respostas.txt` pelos códigos (FA1-1, FA1-2...). "Seguir sugestão" aceita a proposta.

**FA1-1. O quadro e as direções do boneco.**

- *Contexto:* o boneco tem 16 × 24 px, e o Pipeline diz "3 direções desenhadas e a esquerda espelhada". O jogo lê o tamanho do quadro e o ponto dos pés do `boneco_provisorio.json` (FL4-4), mas só aceita quadros quadrados, e hoje desenha as 4 direções da folha, sem espelhar.
- *Pergunta:* de que tamanho é cada quadro da folha, onde ficam os pés, e quem faz a esquerda? *Exemplo:* o NPC anda para a esquerda; de onde sai esse desenho?
- *Opções:*
  - (a) **quadro de 32 × 32, boneco nas colunas 8 a 23 e nas linhas 5 a 28, pés em (16, 28); 4 linhas de direção na folha, e a da esquerda é a da direita copiada e espelhada no Pixelorama.** Ganha: a folga para os braços, a sombra e as ferramentas do "trabalhar"; o jogo não muda. Perde: a folha tem um pouco de espaço vazio, e a linha da esquerda é um arquivo a mais para manter igual à da direita;
  - (b) quadro de 24 × 24, justo. Ganha: menos espaço vazio. Perde: as ferramentas e os braços abertos não cabem;
  - (c) o jogo espelha a esquerda sozinho (3 linhas na folha). Ganha: um desenho a menos. Perde: muda o código, e um boneco com algo só de um lado (uma bolsa, um escudo) troca de lado ao virar.
- *Sugestão:* (a). Se o espelhamento automático fizer falta mais tarde (com as armas e os escudos), ele vira pergunta junto com o combate.
- *Conflitos:* nenhum. O Pipeline ("a esquerda é a direita espelhada") continua valendo: o espelho é feito no Pixelorama.

**FA1-2. A luz, o contorno e o sombreado.**

- *Contexto:* o Pipeline decide a vista 3/4 e a paleta, mas não a direção da luz nem o contorno. Sem uma regra única, cada peça sai com uma luz diferente, e a cena fica estranha.
- *Pergunta:* de onde vem a luz, e como é o contorno? *Exemplo:* uma árvore ao lado de uma casa: de que lado fica a sombra das duas?
- *Opções:*
  - (a) **luz de cima, à esquerda; contorno colorido (o tom mais escuro da rampa do material, ou a cor 01 quando faltar); chão sem contorno; sem dithering; de 3 a 4 tons por material.** Ganha: o estilo de Stardew, a referência da vista; as peças se destacam do chão sem ficarem "duras". Perde: escolher a cor do contorno peça por peça;
  - (b) contorno preto puro. Ganha: simples. Perde: fica pesado em 16 px, e o preto puro nem está na Resurrect 64;
  - (c) sem contorno. Ganha: visual mais suave. Perde: os bonecos somem em cima da grama de cor parecida.
- *Sugestão:* (a).
- *Conflitos:* nenhum.

**FA1-3. As cores a mais e a sombra no chão.**

- *Contexto:* o autor decidiu a Resurrect 64 "com algumas cores a mais se necessário". A Resurrect 64 não tem nenhuma cor transparente, e as sombras no chão (embaixo dos bonecos, das árvores e das construções) ficam melhores um pouco transparentes, mostrando a grama por baixo.
- *Pergunta:* quantas cores a mais, e como fica a sombra? *Exemplo:* um NPC andando da grama para a terra: a sombra dele muda de cor?
- *Opções:*
  - (a) **a sombra no chão é a cor 01 com 50% de transparência (a cor 65, a única transparente do jogo); e no máximo 8 cores a mais (66 a 73), cada uma anotada no guia com o motivo e acrescentada no fim do arquivo da paleta.** Ganha: a sombra escurece qualquer chão do jeito certo; o limite segura a paleta. Perde: uma regra a mais para lembrar;
  - (b) sombras opacas, de cor escura. Ganha: nenhum pixel transparente. Perde: a sombra fica com a mesma cor em cima da grama, da terra e da água, e parece um buraco;
  - (c) cores a mais livres. Ganha: liberdade. Perde: a paleta se perde aos poucos, e a troca de paleta por cultura (Pipeline) fica mais difícil.
- *Sugestão:* (a).
- *Conflitos:* nenhum. O jogo já desenha com transparência (marco 4).

**FA1-4. As bordas entre dois chãos (grama e terra, grama e água).**

- *Contexto:* o Tiled tem os "terrenos" (Pipeline): pinta-se "terra", e ele escolhe a borda certa. Mas o número de tiles de borda depende do tipo de conjunto, e o projeto não espelha tiles (ficha do Mapa), então cada borda é desenhada.
- *Pergunta:* quantos tiles de borda desenhar para cada par de chãos? *Exemplo:* a rua de terra da vila cruza a grama e faz uma curva.
- *Opções:*
  - (a) **o conjunto de cantos de 16 tiles** (cada canto do tile é grama ou terra), que o Tiled usa direto no pincel de terreno. Ganha: o pincel funciona sozinho, inclusive nas diagonais; 16 tiles por par. Perde: os caminhos precisam de 2 tiles de largura, no mínimo (as ruas da vila já têm 2);
  - (b) 13 tiles (3 × 3 de bordas e 4 cantos de dentro). Ganha: 3 tiles a menos. Perde: faltam as duas diagonais, e o pincel do Tiled não monta sozinho;
  - (c) 47 tiles (o conjunto "blob"). Ganha: caminhos de 1 tile e curvas mais bonitas. Perde: quase 3 vezes mais desenho, muito para quem está começando.
- *Sugestão:* (a), para os dois pares da fatia (grama e terra; grama e água): 32 tiles de borda no total.
- *Conflitos:* nenhum.

**FA1-5. O tamanho das construções.**

- *Contexto:* a construção ocupa o lote no chão (pequeno 4 × 4 tiles, médio 6 × 6), mas, na vista 3/4, o telhado e a parede da frente "sobem" na tela. O jogo ainda não desenha imagens nos lotes (são os retângulos da FL5-1); isso entra com a ficha da Construção, no marco 6.
- *Pergunta:* qual é o tamanho da imagem de cada construção, e como ela se encaixa no lote? *Exemplo:* a Casa no lote 2, com a porta em (19, 25).
- *Opções:*
  - (a) **a imagem tem a largura do lote e 32 px a mais de altura (pequena 64 × 96, média 96 × 128); a linha de baixo da imagem fica na linha de baixo do lote, e o telhado sobe para fora dele; a porta cobre a coluna do tile da porta.** Ganha: as construções têm altura e parecem de verdade, como em Stardew. Perde: o telhado cobre os tiles acima do lote (e quem passar por trás), o que o desenho do marco 6 precisa tratar;
  - (b) a construção cabe inteira no chão do lote (64 × 64), vista mais de cima. Ganha: simples de encaixar. Perde: casas achatadas, sem parede da frente;
  - (c) uma altura livre por construção, anotada nos dados. Ganha: torres e casas baixas. Perde: mais um número por construção, sem necessidade na fatia.
- *Sugestão:* (a). A ordem de desenho (o telhado na frente de quem passa atrás) entra na ficha da Construção.
- *Conflitos:* a ficha do Mapa já dizia que as construções são desenhadas "por cima" dos lotes a partir do marco 6; a altura extra é a novidade.

Depois das respostas:

- tirar as marcas [Proposta] do Guia da Arte e anotar a rodada no Pipeline;
- as fichas do marco 6, na branch `docs/fichas-marco-6`.

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
