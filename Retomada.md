> Ponto de partida de cada sessão. Curta de propósito: só o que muda o trabalho de hoje. O detalhe fica nos documentos e no Git. Atualizado em 2026-10-19.

## 1. Onde paramos

- **Marcos 0 a 7:** prontos e no `main`.
- **Arte:** por enquanto, só a arte de teste do assistente (decisão de 2026-10-18).
- **Marco 8** (a interface do jogador, as obras e as decisões): escrito na branch `codigo/marco-8-interface`, com a FL8 aplicada (ver `Jogo/LEIA-ME.md`, "Marco 8", o que testar). Falta o teste do autor. A rodada FE2 (Blocos de Pedra) está aplicada: a vila começa com 300 Blocos.
- **Próximo passo:** o autor testa o marco 8; depois, as fichas do marco 9 (salvar e carregar) e a rodada FL9 (Opus, esforço médio).

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
