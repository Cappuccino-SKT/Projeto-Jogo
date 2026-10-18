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
- **Arte (2026-10-18):** o autor testou gerar a arte com o Gemini e **abandonou a ideia**; ele vai **desenhar à mão, no Pixelorama**, com a paleta **Resurrect 64** (mais algumas cores, se precisar). O [[Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]] junta todos os critérios, na branch `docs/guia-da-arte` (feita em cima da `codigo/marco-5-rotina`), e abriu a **rodada FA1**, aplicada em 2026-10-18, toda com "seguir sugestão". Logo depois, o autor decidiu **não usar nenhuma arte por enquanto**: o jogo fica só com a arte de teste feita pelo assistente (o terreno de quadrados, o boneco de teste e os retângulos das construções). O guia fica pronto para quando o desenho voltar.
- **Marco 5 (2026-10-18):** escrito na branch `codigo/marco-5-rotina` (feita do `main`, que já tem a ficha do marco 5); **confirmado pelo autor** (seção 3).
- **Fichas do marco 6 (2026-10-18):** a [[Ficha Logica/Producao|Produção]], o [[Ficha Logica/Inventario|Inventário]], a [[Ficha Logica/Economia|Economia]] (consumo e salários) e a [[Ficha Logica/Construcao|Construção]], na branch `docs/fichas-marco-6` (feita em cima da `docs/guia-da-arte`). A **rodada FL6** foi aplicada (2026-10-18), toda com "seguir sugestão".
- **Marco 6 (2026-10-18):** escrito na branch `codigo/marco-6-producao`, com a FA1 e a FL6 aplicadas, e já mesclado no `main`; o autor mandou seguir (seção 3).
- **Fichas do marco 7 (2026-10-18):** a parte 10 da [[Ficha Logica/Economia|Economia]] e da [[Ficha Logica/Producao|Produção]], e as fichas da [[Ficha Logica/Felicidade|Felicidade]], dos [[Ficha Logica/Eventos|Eventos]] e da [[Ficha Logica/Simulacao|Simulação sem tela]], na branch `docs/fichas-marco-7` (feita do `main`). Abriram a **rodada FL7** (seção 4).
- **Próximo passo:** o autor responde a FL7; depois, aplicar e começar o marco 7.
- **Ao retomar:** rodar `git fetch` e conferir se os pull requests foram mesclados no `main`. Tudo até o marco 6 já entrou no `main`; falta o `docs/fichas-marco-7`.

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
  - arte toda à mão (por enquanto, só a arte de teste do assistente, decisão de 2026-10-18);
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
- **Marco 6 (pronto em 2026-10-18; o autor mandou seguir):** o inventário da cidade, a produção (o ciclo, a escolha da receita, as ordens da Carpintaria, os Braçais no campo), o consumo da cesta mínima e os salários (`Codigo/Inventario/`, `Producao/`, `Economia/`, `Construcao/`), juntos na `Codigo/Vila/SimulacaoDaVila.cpp`; o resumo do dia no console e o F4; as imagens das construções, quando existirem. 239 testes no `Codigo/` (com 3 dias sem tela) e 42 no `Jogo/`.
- **Marco 5 (pronto, confirmado pelo autor em 2026-10-18):** a vila inicial (`data/vila_inicial.json`: 10 construções em retângulos de cor, que bloqueiam o lote, e 15 Civis com casa e emprego) e a rotina com a Utilidade (`Codigo/Npc/RotinaDosNpcs.cpp`, `Codigo/Vila/`), com o sorteio PCG32 (`Codigo/Tempo/SorteioComSemente.cpp`); F3 mostra as ações. 193 testes no `Codigo/` (com um dia inteiro sem tela) e 37 no `Jogo/`.
- **Marco 4 (pronto, confirmado pelo autor em 2026-10-18):** um NPC no centro da vila anda até o tile do botão direito pela busca A\* (`Codigo/Mapa/BuscaDeCaminho.cpp`, `Codigo/Npc/`), com a EnTT 3.16.0; o boneco do Mana Seed (ou o de teste, sem a pasta) é desenhado entre os ticks; N cria mais NPCs e F2 mostra os caminhos. 167 testes no `Codigo/` e 36 no `Jogo/`.
- **Marco 3 (pronto, confirmado pelo autor em 2026-10-18):** os dados em `Jogo/data` (itens, receitas, recursos, construções, regras e textos), lidos e conferidos por `Codigo/Dados/`, com o preço base (Pão 1,7) e o `jogo --validar`; as teclas passam pela tabela de ações (FL2-4). 124 testes no `Codigo/` e 25 no `Jogo/`.

## 4. Rodada pendente: FL7, a simulação de 1 ano (marco 7)

**Perguntas enviadas em 2026-10-18, aguardando respostas.** Ao escrever a parte 10 da [[Ficha Logica/Economia|Economia]] e da [[Ficha Logica/Producao|Produção]] e as fichas da [[Ficha Logica/Felicidade|Felicidade]], dos [[Ficha Logica/Eventos|Eventos]] e da [[Ficha Logica/Simulacao|Simulação sem tela]], apareceram 8 pontos. A FL7-1 é a mais importante: decide se o Banco sobrevive ao ano. As fichas já usam as sugestões, marcadas como [Proposta]. (A FA1 e a FL6 foram aplicadas em 2026-10-18, todas com "seguir sugestão".)

Responder no `Respostas.txt` pelos códigos (FL7-1, FL7-2...). "Seguir sugestão" aceita a proposta.

**FL7-1. Para onde vai o gasto extra (como o dinheiro fecha).**

- *Contexto:* no marco 6, o Banco perde cerca de 54 moedas por dia: ele paga 84 de salários e só recebe o Pão (28,5). A Classe Social diz que cada classe gasta até 80% da renda: primeiro a cesta, e o resto "sobe o Tier da comida, compra mais materiais e paga a Bebida". Mas a fatia não tem comida de Tier 2 nem Bebida: hoje, a classe guarda o resto, e o dinheiro some do ciclo (os Civis chegam a 43 moedas cada no dia 30, e o Banco a 0).
- *Pergunta:* o que a classe compra com o que sobra da renda? *Exemplo:* os Civis ganharam 85 moedas ontem; a cesta custou 41,7; para onde vão os outros 26,3 do teto de 68?
- *Opções* (com um modelo de 64 dias, ficha da Economia, 10.10):
  - (a) **primeiro, Materiais a mais (até o mesmo número da cesta do dia, pagos ao dono); o resto vai para a loja da cidade, para o Banco, como a comida de Tier 2, a Bebida e os serviços que a fatia ainda não tem (nenhum item sai do estoque).** Ganha: o dinheiro fecha (o Banco fica perto de 400 o ano todo, o menor é 398), o jogador vende o dobro de Móveis, e a regra do gasto continua a mesma quando os itens de verdade entrarem (eles só substituem a "loja"). Perde: uma compra sem item, só na fatia;
  - (b) só os Materiais a mais; o resto vira poupança. Ganha: só compras de verdade. Perde: o Banco zera, e o teste de 1 ano falha (no modelo, o Banco termina com 30);
  - (c) pôr na fatia uma comida de Tier 2 e a Bebida (por exemplo, a Cerveja, com a Taverna). Ganha: o gasto de verdade. Perde: itens, receitas, uma construção e empregados novos, que a vila de 15 não tem, num marco que já é grande.
- *Sugestão:* (a).
- *Conflitos:* a Classe Social fala em itens; a "loja da cidade" vale só enquanto a fatia não tiver os itens de Tier 2 e a Bebida.

**FL7-2. O imposto na fatia.**

- *Contexto:* a V2-7 diz que o imposto é de 10%, 20% ou 30% (Baixo, Normal, Alto) sobre os salários e os lucros, cobrado por estação, e que a cidade fica com 30% (o resto vai para o Banco da Facção). A fatia não tem facção. No modelo da FL7-1, o dinheiro só fecha se a classe gastar toda a renda: 80% de gasto mais 20% de imposto.
- *Pergunta:* qual taxa, quando ela é cobrada e com quem ela fica? *Exemplo:* o Cozinheiro recebe 7,0 moedas no dia; quanto vai para o Banco, e quando?
- *Opções:*
  - (a) **20% (Normal), retido no próprio salário a cada dia, todo para o Banco da Cidade (na fatia, sem facção); o lucro do jogador ainda não paga (entra com a interface).** O Cozinheiro recebe 5,6, e 1,4 vai para o Banco na hora. Ganha: o dinheiro fecha (o modelo da FL7-1); sem um "dia do imposto" que tira um bloco de dinheiro de uma vez. Perde: diferente da V2-7 em dois pontos (a cada dia, e 100% da cidade);
  - (b) como a V2-7: por estação, 30% para a cidade, e os 70% somem (o Banco da Facção não existe). Ganha: igual ao documento. Perde: o Banco zera (no modelo, termina com 22);
  - (c) 10% (Baixo). Ganha: os Civis juntam dinheiro. Perde: o Banco zera (no modelo, termina com 24).
- *Sugestão:* (a).
- *Conflitos:* a V2-7 (por estação; a cidade com 30%) vale para o jogo completo, com facções; na fatia, sem facção, a cidade fica com tudo. O "imposto Normal dá −5 de Felicidade" não entra: a fatia tem só 4 fatores (FT1-2).

**FL7-3. Os multiplicadores do salário (riqueza e desemprego).**

- *Contexto:* o Trabalho e Propriedade dá as faixas (a riqueza da cidade de × 0,8 a × 1,2; o desemprego de × 0,9, com muito desemprego, a × 1,1, com falta de gente), mas não a conta. No marco 6, os dois ficaram em × 1 (FL6-1).
- *Pergunta:* como sai cada multiplicador? *Exemplo:* a riqueza média é 12 moedas, e 3 dos 15 Civis estão sem emprego; quanto ganha o Cozinheiro que ganharia 7,9?
- *Opções:*
  - (a) **riqueza = 0,8 + 0,4 × riqueza média ÷ (10 dias da cesta), presa entre 0,8 e 1,2 (5 dias da cesta dão × 1); desemprego = 1,1 − a taxa de desemprego, presa entre 0,9 e 1,1.** No exemplo: 0,98 × 0,90, e o Cozinheiro ganha 7,0. Ganha: os salários sobem quando a vila está rica e descem com o desemprego, sem saltos. Perde: na fatia, com 20% de desemprego, todos ganham 10% menos;
  - (b) faixas em degraus (por exemplo, riqueza abaixo de 3 dias × 0,8; de 3 a 7 × 1; acima × 1,2). Ganha: fácil de ler. Perde: o salário salta quando a riqueza cruza um degrau;
  - (c) deixar em × 1 até depois da fatia. Ganha: menos conta. Perde: o teste de 1 ano não mede o efeito dos multiplicadores.
- *Sugestão:* (a).
- *Conflitos:* nenhum.

**FL7-4. A previsão do consumo (a Tora que acaba).**

- *Contexto:* os 30 dias sem tela do marco 6 mostraram que a Tora acaba no dia 12: o estoque-alvo só conta os insumos das receitas atuais, a Carpintaria passa quase todo o tempo nos Móveis (que gastam Tábua), e o Posto de Coleta fica na Água. Sem Tábua, os Móveis acabam no dia 19. O mesmo estoque-alvo vai servir ao Mercado Local, que precisa do "consumo da cidade" (V2-5).
- *Pergunta:* como a vila prevê o consumo de cada item? *Exemplo:* a Carpintaria fica 8 horas parada "sem Tora"; o Posto de Coleta percebe que falta Tora?
- *Opções:*
  - (a) **a média dos 5 últimos dias do que a vila gastou ou quis gastar: o que saiu do estoque (a cesta, o gasto extra e os insumos) mais a demanda que faltou (cada hora parada por falta de um insumo, com gente trabalhando, conta a parte de um ciclo). A fila do jogador fica parada "sem Tora" (e não "sem ordem") quando alguma ordem quer produzir e só falta insumo.** No exemplo: 8 Toras de demanda no dia, e o Posto escolhe a Tora no dia seguinte. Ganha: vale para qualquer item e qualquer cadeia; a mesma conta serve ao Mercado Local. Perde: 5 números guardados por item;
  - (b) a receita atual, e, na Carpintaria do jogador, também as receitas de todas as ordens. Ganha: pouca mudança. Perde: só conserta este caso; outra cadeia pode travar do mesmo jeito;
  - (c) o Posto de Coleta alterna a Água e a Tora a cada dia. Ganha: simples. Perde: ignora o estoque e não serve ao Mercado Local.
- *Sugestão:* (a).
- *Conflitos:* muda o estoque-alvo da regra 4.3 da Produção (marco 6), que usava as receitas atuais.

**FL7-5. A folga (o estoque que enche e o Pão barato).**

- *Contexto:* com déficit 0, a nota da escolha continua positiva, e os Braçais nunca param: a Fazenda faz cerca de 31,6 Grãos por dia, e a vila gasta 8. Nos 30 dias do marco 6, o Grão e a Água enchem os 900 de matéria-prima no dia 17, e a produção trava ("o estoque encheu"), a Carpintaria por último. O Pão também sobe (15,8 feitos e 15 comidos por dia): no modelo, o preço local dele chega a 1,22 no dia 64 e passaria de 1,2, o limite do teste de 1 ano.
- *Pergunta:* o que uma construção da Cidade faz quando já tem estoque de sobra? *Exemplo:* a Fazenda com 250 Grãos e alvo 40.
- *Opções:*
  - (a) **uma receita deixa de ser candidata quando o produto tem 125% do estoque-alvo (6 dias e 6 horas) e só volta abaixo do alvo; sem candidata, a construção fica de folga no dia seguinte; a Carpintaria do jogador sem ordem também folga; de folga, os empregados passeiam e não recebem.** No exemplo: a Fazenda trabalha cerca de 1 dia em cada 4; o Pão fica entre cerca de 60 e 95 Pães, e o preço, entre cerca de 1,3 e 2,0. Ganha: a matéria-prima nunca enche, e o preço do Pão fica na faixa do teste. Perde: os empregados de folga ganham menos (a renda da classe cai um pouco);
  - (b) a construção continua produzindo, e a Cidade vende a sobra a um comprador de fora a 90%. Ganha: os empregados trabalham sempre; entra dinheiro. Perde: um comprador sem fim é dinheiro aparecendo do nada, e o Pão continua barato;
  - (c) deixar travar com "o estoque encheu". Ganha: nada a fazer. Perde: a Carpintaria para, os Móveis acabam, e o preço do Pão sai da faixa.
- *Sugestão:* (a).
- *Conflitos:* os números da V2-12 (32 Grãos por dia) viram a capacidade da Fazenda, não a produção de todo dia.

**FL7-6. Os eventos da fatia.**

- *Contexto:* a V3-6 escolheu 12 eventos para a fatia. Alguns precisam de coisas que a fatia não tem: a Tempestade forte danifica a Vida de uma construção (que entra com as obras, no marco 8); a Febre usa a mortalidade e o Remédio; o Mercador viajante vende itens de fora; o Achado dá Pedra de Mana; a Fome causa migração; a Festa gasta Cerveja. O Narrador regula a tensão do jogador, que ainda não age no teste de 1 ano.
- *Pergunta:* quais eventos entram, como, e com que chance? *Exemplo:* no Verão, quantas vezes vem a Seca?
- *Opções:*
  - (a) **11 dos 12, sem a Tempestade; 4 adaptados (a Febre deixa 2 Civis doentes em casa por 4 dias; o Mercador compra 20 unidades do item com mais dias de estoque, a 90%, e o dinheiro vai para o Banco; o Achado dá 20 Toras; a Fome e a Festa mexem só na Felicidade); as chances em por mil por dia (a Seca com 4% no Verão: cerca de 0,6 vez por verão); no máximo 2 negativos ativos; sem o Narrador.** A tabela completa está na ficha dos Eventos. Ganha: quase a lista toda, e cada evento mexe em algo que existe. Perde: 4 eventos diferentes do documento, até a fatia ter o que falta;
  - (b) só os 7 que entram sem adaptação. Ganha: igual ao documento. Perde: menos variedade no teste de 1 ano;
  - (c) os 12, pondo a Vida das construções, a mortalidade e a Pedra de Mana agora. Ganha: completo. Perde: 3 sistemas novos no marco 7.
- *Sugestão:* (a), com os eventos num `data/eventos.json` e um sorteio próprio (o PCG32 na sequência 2).
- *Conflitos:* a lista da V3-6 fica com 11 na fatia; a Tempestade volta com as obras.

**FL7-7. A Felicidade na fatia.**

- *Contexto:* os 4 fatores e o passo de 10% por dia já estão decididos (FT1-2). Faltam o valor inicial, a conta do bônus do gasto extra ("até +5") e se as faixas (Contente, Inquieta, Protesto...) já mudam alguma coisa.
- *Pergunta:* com quanto a Felicidade começa, e o que ela muda no marco 7? *Exemplo:* depois de uma semana de fome, a Felicidade chega a 38 (Inquieta); a produção cai?
- *Opções:*
  - (a) **começa em 50; o bônus é +5 × (gasto extra ÷ gasto da cesta), no máximo +5; guardada em décimos; no marco 7, ela só é medida e mostrada (no resumo do dia e no relatório) e serve de condição aos eventos; os efeitos das faixas ficam para depois da fatia.** Ganha: o teste de 1 ano mede a Felicidade sem que ela mude a economia que está sendo testada. Perde: a Felicidade ainda não tem consequência;
  - (b) as faixas já mudam a produtividade (Contente +5%, Inquieta −10%, Protesto −25%). Ganha: a Felicidade pesa. Perde: mais uma coisa mexendo na economia antes de ela estar equilibrada;
  - (c) começa no alvo do primeiro dia (56). Ganha: sem a subida dos primeiros dias. Perde: nada de importante; é só o ponto de partida.
- *Sugestão:* (a). Num dia comum, o alvo fica em cerca de 58,7 (Estável).
- *Conflitos:* nenhum.

**FL7-8. O teste de 1 ano e o desempenho.**

- *Contexto:* a FT1-6 decidiu o comando (`jogo --simular 64 --semente 1`), o relatório e o PASSOU/FALHOU; a parte 6 da Fatia Vertical dá os critérios (fome por mais de 3 dias seguidos, o Banco zerado, o Pão fora de 1,2 a 2,6) e pede medir o desempenho com o Tracy e 500 NPCs (V2-11). Faltam o formato do relatório, o que o `ctest` roda e a meta de tempo.
- *Pergunta:* como o relatório sai e como medir o desempenho? *Exemplo:* depois de rodar, como o autor vê o preço do Pão do dia 40?
- *Opções:*
  - (a) **o `relatorio.csv` na pasta de onde o jogo foi chamado, com ponto e vírgula e vírgula decimal (abre direto no Excel ou no LibreOffice), uma linha por dia com o estoque de cada item, o preço do Pão, o Banco, o jogador, a riqueza média, a Felicidade, os empregados, os dias em déficit, o consumo não pago e os eventos do dia; o `ctest` confere a repetição com 2 rodadas de 16 dias e roda os 64 dias como um teste à parte (cerca de 30 segundos); o tempo médio de um tick sai sempre no fim; o Tracy entra como uma opção do CMake, desligada por padrão; e o `--npcs-a-mais 485` mede os 500 NPCs, com a meta de 3 milissegundos por tick.** Ganha: o autor abre o relatório numa planilha e faz gráficos; o `ctest` continua rápido. Perde: o teste de 64 dias deixa o `ctest` cerca de 30 segundos mais lento;
  - (b) um relatório só no console. Ganha: nada a abrir. Perde: 64 linhas largas, ruins de ler;
  - (c) o Tracy sempre ligado. Ganha: medição a qualquer hora. Perde: o jogo fica um pouco mais lento e manda dados pela rede, mesmo sem o programa do Tracy aberto.
- *Sugestão:* (a).
- *Conflitos:* nenhum.

Depois das respostas:

- tirar as marcas [Proposta] da parte 10 da Economia e da Produção e das fichas da Felicidade, dos Eventos e da Simulação, e anotar a rodada na parte 8 da Fatia Vertical (e a mudança da V2-7 no Econômico, se a FL7-2 for aceita);
- começar o marco 7, na branch `codigo/marco-7-simulacao`.

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
