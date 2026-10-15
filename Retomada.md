> Notas para **retomar o trabalho** depois de uma pausa ou de uma compactação da conversa. Guarda o que não cabe no [[Estado do Projeto]]: onde paramos, a pergunta que está pendente, o jeito de trabalhar e os cuidados técnicos. Atualizado ao fim da rodada P0-5 (Mundo e Estado Inicial), antes de uma pausa.

## 1. Por onde começar

1. Ler o [[Estado do Projeto]] (o que está resolvido e em aberto) e a [[Revisão do Projeto]] (lista de sistemas que faltam).
2. **Rodada pendente: P0-5, Mundo e Estado Inicial.** A lista de perguntas foi enviada ao autor e **ainda não foi respondida**. Ao retomar, pedir o `Respostas.txt` (ou as respostas no chat). Se o arquivo não tiver essa rodada, avisar (já aconteceu de o arquivo antigo ser anexado). A lista completa está na seção 4.
3. Depois da P0-5: **rodada P0-6, Pipeline de arte e conteúdo** (última do P0). Depois, os sistemas P1 (seção 3).
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

## 3. Lista de sistemas (da Revisão)

| Prioridade | Sistema | Estado |
|---|---|---|
| P0 | Felicidade e Estabilidade | Escrito |
| P0 | População e Habitação | Escrito |
| P0 | Trabalho, Salários e Propriedade (+ Funções e Construções) | Escrito |
| P0 | Interface e Controles | Escrito |
| P0 | **Mundo e Estado Inicial (Lore)** | **Perguntas enviadas, sem resposta** |
| P0 | Pipeline de arte e conteúdo (Tiled, orçamento de arte) | A fazer |
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

## 4. Rodada P0-5 pendente: Mundo e Estado Inicial

As decisões criativas (nomes, tom, história, personagens) são do autor; as sugestões abaixo são estrutura.

- **A1. Disposição:** 48 Mapas Locais (8 × 6) em 6 blocos de 8 mapas (4 × 2), 3 colunas por 2 linhas. Cada região: 1 capital (nível 4), 1 cidade grande, 1 ou 2 vilarejos, o resto território e mapas livres. Fronteiras entre regiões geram mapas em disputa.
- **A2. Bioma e recursos:** cada região tem um bioma ligado à cultura e 2 a 3 recursos de destaque (Guerreira: ferro e carvão; Arcana: Pedra Mágica e Flor Espiritual; Escravista: algodão e portos), e escassez de outros, para criar comércio.
- **A3. Mapas livres e masmorras:** cerca de 30% dos mapas sem dono; cerca de 12 masmorras (2 por região), dificuldade crescente com a distância das capitais; 3 lendárias em regiões diferentes.
- **B1. Nomes:** provisórios descritivos (Mercantil/Escravista, Sagrada/Religiosa, Imperial/Autoritária, Tecnocrata, Marcial/Guerreira, Arcana). **Pedir os nomes que o autor já tem.**
- **B2. Tom:** fantasia de baixa magia, tecnologia em transição para a pólvora, sem criaturas míticas além de animais e monstros; tom sombrio e político (mais Mount & Blade).
- **B3. Só humanos** na primeira versão, diferenciados pelas culturas.
- **C1. Calendário:** parte de um ano alto (por exemplo, Ano 412); os marcos de tecnologia contam anos desde o início da partida.
- **C2. Facções iniciais:** as 6 principais e algumas cidades independentes pequenas (níveis 1 e 2), sem facção, para o jogador e o estilo "Dono de vila".
- **C3. Relações:** matriz base 6 × 6 por afinidade cultural e fronteira, mais 1 ou 2 guerras iniciais sorteadas entre vizinhos hostis e alguns acordos comerciais.
- **C4. O que varia por partida:** relações (pequeno desvio), guerras iniciais, 2 a 3 tecnologias extras por cultura, Loot das masmorras, semente do Narrador, personalidade dos líderes, preços iniciais (±10%). Fixo: mapas, cidades, culturas, regiões e masmorras.
- **C5. Líderes e Importantes:** 6 líderes e nobres gerados no começo (nomes de lista por cultura e Personalidade de modelos); mais 3 a 5 NPCs Importantes fixos escritos à mão, ligados às Ambições (Arquimago, rei exilado, grande mercador). **Pedir os personagens que o autor imagina.**
- **D1. Equilíbrio:** PIB varia até ±30% entre facções, nenhuma com mais que o dobro de outra; todas na Era 1 de tecnologia.
- **D2. Construções iniciais:** pacote por nível e cultura em arquivo de dados (nível 3: Fazenda, Pasto, Cozinheiro, Pedreira, Mercado, Taverna, Quartel e Templo).
- **D3. Pontos de partida:** 2 ou 3 fixos por estilo de início (o Dono de vila em 3 mapas livres próximos de facções diferentes).
- **E1. Documentos:** um `Mundo e História/Descrição.md` (regiões, culturas, história, líderes e Importantes) e tabelas de dados iniciais.

Depois das respostas: escrever `Mundo e História/Descrição.md`; atualizar Mapa, Cidade, Cultura, Facção, Início de Jogo, Objetivo e Tecnologia; abrir a rodada P0-6 (Pipeline de arte e conteúdo).

## 5. Fatos que valem lembrar

- **Tempo:** 24 h de jogo = 16 min reais a 1x; 1 ano (64 dias) ≈ 17 h reais a 1x; 1 estação (16 dias) ≈ 4,3 h. Velocidade 8x só no Mapa Mundial; nos outros mapas, até 4x.
- **População:** NPCs reais por nível de cidade: 15, 40, 100 e 300; o Fator de Escala (exemplo 100) converte em população total.
- **Moradias** (canvas civil): Senzala, Casa, Casarão, Mansão e Palácio (níveis 1 a 5). A **Universidade** existe no canvas civil.
- **Canvas de produção:** 15 construções de produção (Armeiro, Artesão, Açougue, Carpinteiro, Costureiro, Cozinheiro, Destilaria, Fazenda, Ferreiro, Metalurgia, Misturador, Pasto, Pedreira, Posto de Coleta e Rancho). O Ferreiro não lista Espada (provável erro; ver [[Sistema de Produção/Funções e Construções|Funções e Construções]]).
- **Mapas:** serão desenhados no **Tiled** (editor gratuito); o autor ainda não conhece a ferramenta e pediu explicação simples quando chegar a hora.
- **Git:** o repositório é `FelipeSKT/Projeto-Jogo`. Cada rodada vira um pull request para o autor aceitar.

## 6. Cuidados técnicos (para quem edita)

- **Ferramentas do PC:** g++ 15.2, CMake, Ninja, Git e Python 3.11. **Não há** `gh` (GitHub CLI) nem Visual Studio. O Python precisa ser chamado com `-I` ao ler arquivos do vault.
- **Edição em lote:** usar um script Python que carrega cada arquivo, troca trechos exatos com **verificação de que o trecho existe uma única vez**, e só então grava (com modo de teste "dry-run" antes). Preservar as quebras de linha (CRLF ou LF) e o BOM de cada arquivo. Backup da pasta antes de edições grandes (a pasta é um repositório Git, mas o backup extra é barato).
- **Conferir links:** o vault não deve ter `[[link]]` quebrado. Em tabelas, o separador do alias precisa ser escapado: `[[Pasta/Descrição\|Texto]]`.
- **Console do Windows:** a saída do PowerShell mostra acentos corrompidos; é só na tela, os arquivos estão em UTF-8.
- **Git:** o primeiro commit desta sessão falhou por causa da mensagem em *here-string*; usar `git commit -F arquivo`. Rodar `git fetch` antes de editar, porque outras sessões podem ter mexido no repositório. Não commitar `.obsidian/appearance.json` nem `.obsidian/core-plugins.json` (o Obsidian os altera sozinho). O `.gitignore` já ignora o workspace do Obsidian.
- **Pull request:** sem o `gh`, o assistente envia a branch e o autor abre o PR pelo link `https://github.com/FelipeSKT/Projeto-Jogo/pull/new/NOME-DA-BRANCH`. O navegador interno não está logado no GitHub e o assistente nunca digita senhas. A extensão Claude in Chrome pode permitir criar o PR, se estiver conectada.
- **Nomes de itens:** a lista do [[Sistema de Itens/Descrição|Sistema de Itens]] manda sobre os canvases.
