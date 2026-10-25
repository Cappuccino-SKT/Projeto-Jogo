> Ponto de partida de cada sessão. Curta de propósito: só o que muda o trabalho de hoje. O detalhe fica nos documentos e no Git. Atualizado em 2026-10-19.

## 1. Onde paramos

- **Marcos 0 a 7:** prontos e no `main`.
- **Arte:** por enquanto, só a arte de teste do assistente (decisão de 2026-10-18).
- **Marcos 8 e 9** (interface; salvar e carregar): prontos, testados e no `main`. A fatia vertical está completa. O que cada um trouxe e como testar: `Jogo/LEIA-ME.md`.
- **Próximo passo:** as rodadas FE3, FE4 e FE5 estão aplicadas (o jogador ganha +3,4 a +4,4 por dia; a verba da Coroa segura o Banco; a crise de comida dá prejuízo ao jogador de propósito). Fase nova: só fichas lógicas, sem código, na ordem do [[Ficha Logica/Plano das Fichas|Plano das Fichas]], cada uma com a Simulação de mesa. **Fase A pronta** (2026-10-19): Clima, Trabalho, População, Classe Social, Cidade e Cura. As rodadas PO1, CS1, CI1 e CU1 foram decididas em modo automático (pedido do autor: seguir a sugestão nas dúvidas); cada ficha lista as decisões na parte 10.7, para o autor revisar. Para a vila crescer, faltam **conteúdo** (o mapa com os lotes dos níveis 2 e 3; a Bebida; a comida Tier 2 e o Luxo Tier 3; o Remédio). Fase B: [[Ficha Logica/Viagem|Viagem]] (B1) e [[Ficha Logica/OutrasCidades|Outras cidades]] (B1b) e [[Ficha Logica/Comercio|Comércio]] (B2) e [[Ficha Logica/Noticias|Notícias]] (B3) , [[Ficha Logica/Reputacao|Reputação]] (B4) e [[Ficha Logica/Tecnologia|Tecnologia]] (B5) prontas: **fase B completa**, rodadas VI1, OC1, CO1, NO1, RE1 e TE1 automáticas. Conteúdo novo pedido: o Posto Comercial, o Cavalo, o Laboratório e as listas de tecnologias e marcos (rodada de conteúdo C1). Fase C: [[Ficha Logica/Faccao|Facção]] (C1) , [[Ficha Logica/Cultura|Cultura]] (C2), [[Ficha Logica/Crime|Crime e Justiça]] (C3) e [[Ficha Logica/Religiao|Religião]] (C4) prontas: **fase C completa**, rodadas FA1, CT1, CJ1 e RL1 automáticas (mudam o teto do gasto da Economia, o território da Viagem e dão o 5º fator da Felicidade). Fase D: [[Ficha Logica/Combate|Combate]] (D1) , [[Ficha Logica/Companheiros|Companheiros e Diálogo]] (D2) , [[Ficha Logica/Escravidao|Escravidão]] (D3) , [[Ficha Logica/Magia|Magia]] (D4) , [[Ficha Logica/Diplomacia|Diplomacia]] (D5) , [[Ficha Logica/Batalha|Batalha]] (D6) e [[Ficha Logica/Guerra|Guerra]] (D7) prontas: **fase D completa**, rodadas CB1, CP1, ES1, MG1, DP1, BA1 e GU1 automáticas (o cálculo rápido substitui a força simples da Viagem; a Personalidade e as dinastias). **Fase E completa: todas as fichas do plano estão escritas** ([[Ficha Logica/Mundo|Mundo]], [[Ficha Logica/Inicio|Início de Jogo]], [[Ficha Logica/Objetivo|Objetivo e Progressão]]; rodadas MU1, IN1 e OB1 automáticas). Para o autor conferir: a MU1-2 (a Guerreira sustentável), a IN1-6 (o estilo Artesão da fatia) e a OB1-1 (a "maior fortuna"). **Código depois da fatia** (o autor pediu para seguir no automático, com o Opus): **marcos A1 (Clima), A2 (Trabalho e Propriedade), A3 (População), A6 (a Epidemia, da Cura) e B1 (Viagem) prontos** (branches `codigo/clima`, `codigo/trabalho`, `codigo/populacao` e `codigo/cura`; o que entrou e como testar: `Jogo/LEIA-ME.md`). Rodadas EQ1 e EQ2 aplicadas (branch `codigo/rodada-eq1`). **Marcos B1 (Viagem) e B1b (Outras cidades) prontos** (branches `codigo/viagem` e `codigo/outras-cidades`). **Marco B2 (Comércio) pronto** (branch `codigo/comercio`). Rodada CO2 aplicada (opção A, branch `codigo/rodada-co2`). **Marcos B3 (Notícias) e B4 (Reputação) prontos** (branches `codigo/noticias` e `codigo/reputacao`). Rodada TE2 respondida (opção A). **Marco B5 (Tecnologia) pronto** (branch `codigo/tecnologia`), com conteúdo provisório até a rodada de conteúdo C1. **Marcos C1 (Facção) e C2 (Cultura) prontos** (branches `codigo/faccao` e `codigo/cultura`). **Marcos C3 (Crime e Justiça) e C4 (Religião) prontos** (branches `codigo/crime` e `codigo/religiao`): **fase C do código completa**. **Marco D1 (Combate) pronto** (branch `codigo/combate`). **Rodada CB2 aplicada** (opção C, branch `codigo/rodada-cb2`): o cálculo rápido roda o combate simulado. A4 (Classe Social) e A5 (Cidade) esperam conteúdo: o mapa com os lotes do nível 2, a comida Tier 2, o Luxo Tier 3 e o Casarão. **História do mundo iniciada** (2026-10-25, branch `historia/mundo`): [[Documentos/Mundo e História/Descrição|Mundo e História]], rodadas HI1, HI2 e HI3 respondidas pelo autor; falta o mapa novo do continente (o autor desenha) e a rodada dos nomes (HI4). Regra do autor: a história manda (mudanças nas fichas por rodada).

## 2. Como trabalhamos

- Português do Brasil, linguagem simples; o autor tem pouca experiência em C++.
- **Rodadas de perguntas:** códigos (FE1-1...), escritas em [[Rodada Pendente]]; o autor responde em `C:\Users\Felipe\Desktop\Respostas.txt` (anexado com @). "Seguir sugestão" aceita; a ideia do autor prevalece. Quando um "seguir sugestão" deixa algo vago, o documento detalha com valores marcados [Proposta].
- **Cada pergunta traz:** contexto; pergunta concreta com números e um exemplo de jogo; 2 a 4 opções com o que ganham e perdem; uma sugestão completa; os conflitos com decisões anteriores.
- **Marcações:** sem marca = decisão do autor; [Proposta]; [Em aberto]; [Refinar depois].
- **Git:** cada rodada ou marco = uma branch e um pull request, aberto pelo autor pelo link `https://github.com/FelipeSKT/Projeto-Jogo/pull/new/NOME-DA-BRANCH`. Se o anterior não foi mesclado, a branch nova sai dele.

## 3. Código

- **Pastas:** `Documentos/` (design) → `Ficha Logica/` (a lógica de cada sistema, escrita pouco antes do marco) → `Codigo/` (as regras em C++, nunca abre janela) → `Jogo/` (janela, desenho, `data/`, `assets/`).
- **Marcos:** de 0 a 9, na parte 7 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]. O que cada marco trouxe e como testar: `Jogo/LEIA-ME.md`.
- **Nomes:** português sem acento e sempre descritivos (`horasTrabalhadasNoCiclo`, não `h`).
- **Ferramentas:** g++ 15.2 (MinGW), CMake (preset `padrao`, Debug, pasta `build/`), Ninja, Python 3.11. Bibliotecas pelo `FetchContent` com versão e `URL_HASH` fixos: SDL3, nlohmann/json, stb, EnTT 3.16; glad em `Jogo/externo/`; Tracy só com `-DUSAR_TRACY=ON`; Dear ImGui 1.91.9 (só a interface da fatia, marco 8).
- **Testes:** `ctest --test-dir build` (cerca de 1 min e meio: o teste de 1 ano leva 45 s).

## 4. Cuidados técnicos

- No Git Bash, um heredoc com acentos quebra: escrever o script com a ferramenta de arquivo e rodar `python -I`.
- Preservar CRLF ou LF de cada arquivo; nenhum link do Obsidian quebrado (nas tabelas, `\|`).
- Não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json`; `Assents Temporarios/` fica fora do Git (licença do Mana Seed).
- O navegador interno não está logado no GitHub, e o assistente nunca digita senhas.

## 5. Onde está cada coisa

| Preciso de | Onde |
|---|---|
| O que programar | [[Ficha Logica/Fatia Vertical\|Fatia Vertical]] e a ficha do sistema em `Ficha Logica/` |
| Índice dos sistemas | [[Descrição do Projeto]] |
| Bibliotecas e decisões técnicas | [[Arquitetura/Descrição\|Arquitetura]] |
| Números-chave e o que falta | [[Estado do Projeto]] |
| Histórico (não ler sem necessidade) | `Documentos/Histórico/` e o Git |
