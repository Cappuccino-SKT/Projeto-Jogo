> Ponto de partida de cada sessão. Curta de propósito: só o que muda o trabalho de hoje. O detalhe fica nos documentos e no Git. Atualizado em 2026-10-25.

## 1. Onde paramos

- **Fichas:** todas as do [[Ficha Logica/Plano das Fichas|Plano das Fichas]] estão escritas (fases A a E), menos a D8 (Raças), que entra depois dos marcos da fase D.
- **Código, no `main`:** a fatia vertical (marcos 0 a 9) e os marcos A1, A2, A3, A6, B1, B1b, B2, B3, B4, B5 (com conteúdo provisório), C1, C2, C3, C4 e D1, mais as rodadas EQ1, EQ2, CO2, TE2 e CB2. O que cada um trouxe e como testar: `Jogo/LEIA-ME.md`.
- **Esperam conteúdo:** A4 (Classe Social) e A5 (Cidade) precisam do mapa com os lotes do nível 2, da comida Tier 2, do Luxo Tier 3 e do Casarão. Também faltam a Bebida, o Remédio, o Posto Comercial, o Cavalo e as listas de tecnologias e marcos (rodada de conteúdo C1).
- **História:** [[Documentos/Mundo e História/Descrição|Mundo e História]], rodadas HI1 a HI10 respondidas (até as Grandes Histórias; fichas novas D8 a D10 no plano). Os 6 líderes fixos já estão no código (branch `codigo/lideres-fixos`); Rodadas HI11 e HI12 aplicadas (Vida prolongada de Cassius; adulto aos 5 e velho aos 31). Regra do autor: **a história manda** (quando ela bate numa ficha, a ficha muda por rodada). O autor vai desenhar um **mapa novo**, de um continente inteiro; o mapa 8 × 6 de hoje é provisório.
- **Próximo passo:** na história, os mitos de cada povo e os personagens de hoje; no código, o marco D2 (Companheiros e Diálogo).
- **Para o autor conferir** (decididas em modo automático): a MU1-2 (a Guerreira sustentável), a IN1-6 (o estilo Artesão da fatia) e a OB1-1 (a "maior fortuna").
- **Arte:** **estilo RimWorld** no lugar da pixel art (decisão de 2026-10-26; [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte]]). O jogo segue com a arte de teste do assistente; esperando a rodada AR1 (o formato da arte).

## 2. Como trabalhamos

- Português do Brasil, linguagem simples; o autor tem pouca experiência em C++.
- **Rodadas de perguntas:** códigos (FE1-1...), escritas em [[Rodada Pendente]]; o autor responde em `C:\Users\Felipe\Desktop\Respostas.txt` (anexado com @). "Seguir sugestão" aceita; a ideia do autor prevalece. Quando um "seguir sugestão" deixa algo vago, o documento detalha com valores marcados [Proposta].
- **Cada pergunta traz:** contexto; pergunta concreta com números e um exemplo de jogo; 2 a 4 opções com o que ganham e perdem; uma sugestão completa; os conflitos com decisões anteriores.
- **Marcações:** sem marca = decisão do autor; [Proposta]; [Em aberto]; [Refinar depois].
- **Git:** cada rodada ou marco = uma branch e um pull request. O assistente abre o pull request com o GitHub CLI (`C:\Program Files\GitHub CLI\gh.exe`, já logado; o texto vai por `--body-file`) e faz o merge quando o autor pede. Se o anterior não foi mesclado, a branch nova sai dele.

## 3. Código

- **Pastas:** `Documentos/` (design) → `Ficha Logica/` (a lógica de cada sistema, escrita pouco antes do marco) → `Codigo/` (as regras em C++, nunca abre janela) → `Jogo/` (janela, desenho, `data/`, `assets/`).
- **Marcos:** de 0 a 9, na parte 7 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]. O que cada marco trouxe e como testar: `Jogo/LEIA-ME.md`.
- **Nomes:** português sem acento e sempre descritivos (`horasTrabalhadasNoCiclo`, não `h`).
- **Ferramentas:** g++ 15.2 (MinGW), CMake (preset `padrao`, Debug, pasta `build/`), Ninja, Python 3.11. Bibliotecas pelo `FetchContent` com versão e `URL_HASH` fixos: SDL3, nlohmann/json, stb, EnTT 3.16; glad em `Jogo/externo/`; Tracy só com `-DUSAR_TRACY=ON`; Dear ImGui 1.91.9 (só a interface da fatia, marco 8).
- **Testes:** `ctest --test-dir build` (cerca de 10 minutos: rodar em segundo plano e só no fim do marco).

## 4. Cuidados técnicos

- No Git Bash, um heredoc com acentos quebra: escrever o script com a ferramenta de arquivo e rodar `python -I`.
- Preservar CRLF ou LF de cada arquivo; nenhum link do Obsidian quebrado (nas tabelas, `\|`).
- Não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json`; `Assents Temporarios/` fica fora do Git (licença do Mana Seed).
- O assistente nunca digita senhas; o GitHub é pelo `gh`.

## 5. Onde está cada coisa

| Preciso de | Onde |
|---|---|
| O que programar | [[Ficha Logica/Fatia Vertical\|Fatia Vertical]] e a ficha do sistema em `Ficha Logica/` |
| Índice dos sistemas | [[Descrição do Projeto]] |
| Bibliotecas e decisões técnicas | [[Arquitetura/Descrição\|Arquitetura]] |
| Números-chave e o que falta | [[Estado do Projeto]] |
| Histórico (não ler sem necessidade) | `Documentos/Histórico/` e o Git |
