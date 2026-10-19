---
tags: [ficha-logica]
---

> O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 para o **marco 9** (salvar e carregar). A rodada **FL9** foi respondida em 2026-10-19: todas as sugestões foram aceitas (a FL9-2 com o aviso da parte 9).

# Ficha Lógica: Save

## 1. O que faz

Grava a partida num arquivo e a reconstrói depois, **exatamente** como estava: o mesmo tick, o mesmo sorteio, os mesmos NPCs no meio do mesmo passo. Uma partida carregada continua igual à que não foi salva (o "mesma semente = mesma partida" vale também depois de carregar).

Documento do sistema: [[Arquitetura/Descrição|Arquitetura]] ("Salvar e carregar": arquivo binário com número de versão). Mapa geral: [[Ficha Logica/Fatia Vertical|Fatia Vertical]], parte 5.

**Regra de ouro:** o save guarda só o **estado** (o que muda no jogo). As **definições** (itens, receitas, construções, eventos, regras) vêm sempre dos arquivos de `data/`, e o save guarda só os **IDs em texto** delas (`item.pao`, nunca "o item número 3").

## 2. Dados

### Definições (não mudam durante o jogo)

| Campo | Tipo | Unidade | Valor |
|---|---|---|---|
| `versaoDoFormatoDoSave` | inteiro | — | 1 (sobe a cada mudança no que é gravado) |
| `formatoDoArquivo` | — | — | MessagePack pela nlohmann/json (FL9-1); o comando `jogo --save-para-json arquivo.save` mostra o save em texto para investigar |
| `extensaoDoArquivo` | texto | — | `.save` |
| `pastaDosSaves` | texto | — | `SDL_GetPrefPath` do jogo + `saves/` (no Windows, `C:\Users\<usuário>\AppData\Roaming\Projeto-Jogo\saves\`); o `Jogo/LEIA-ME.md` diz o caminho (FL9-4) |
| `espacosDeSaveManual` | inteiro | espaços | 3 (`espaco1.save` a `espaco3.save`) (FL9-3) |
| Save automático | — | — | 1 espaço (`automatico.save`), todo dia às 0h (a cada 20 minutos reais a 1x), depois da ordem do dia (FL9-3) |
| `horasSemSalvarParaConfirmar` | inteiro | horas de jogo | 1 **[Proposta]** (FL9-6) |

### Estado (o que vai no arquivo)

O arquivo é uma árvore de seções com nome. Cada seção corresponde a uma parte da `SimulacaoDaVila` (`Codigo/Vila/SimulacaoDaVila.h`).

| Seção | O que grava | Exemplo |
|---|---|---|
| `cabecalho` | versão do formato, versão do jogo, data real em que salvou, data do jogo em texto (para a lista de saves), nome do espaço | `1`, `"0.9"`, `"2026-10-19 21:40"`, `"Dia 12 do Verão, ano 412, 14h"` |
| `relogio` | tick atual, velocidade | `529.600`, `0` |
| `sorteios` | estado e incremento dos dois geradores PCG32 (NPCs e eventos) e a semente da partida | `semente 1` |
| `vila` | a lista das construções **na ordem** (os índices são a posição nela): tipo, nível, dono, lote, Vida, caixa, receita | `construcao.padaria`, nível 1, dono `cidade` |
| `producao` | por construção: empregados, progresso da receita, ordens do jogador, trabalho do dia | |
| `obras` | as obras em andamento: construção, lote, horas feitas, materiais entregues | |
| `inventario` | por dono, a quantidade de cada item (ID em texto) | `cidade: item.pao 42` |
| `dinheiro` | Banco, dinheiro de cada dono | `Banco 1.830` |
| `mercado` | preços locais, dias em déficit, histórico usado pela previsão | |
| `felicidade` | valor e os fatores guardados | |
| `eventos` | eventos ativos (ID, dias que faltam) e os cooldowns | `evento.febre, 2 dias` |
| `decisoes` | preço, salário e escolhas do jogador em cada construção dele | |
| `npcs` | cada NPC **na ordem da vila inicial**: identidade, casa e trabalho, lugar (tile, próximo tile, progresso), olhar, decisão, doença, fome, dinheiro, habilidades, caminho (a lista de tiles e o índice) e os pedidos de caminho que esperam o cálculo (FL9-2) | |
| `jogador` | **[Refinar depois]** o personagem do jogador ainda não existe no código; o dinheiro e o estoque dele já vão em `dinheiro` e `inventario` | |
| `diario` | as 300 notificações do Diário | |
| `camera` | **[Refinar depois]** não foi gravada no marco 9: a câmera volta centralizada | |

**Não vai no arquivo** (recalculado ao carregar): as definições e os preços base; o mapa bloqueado pelos lotes (sai da lista das construções); a capacidade das construções; os campos só de desenho (`lugarNoTickAnteriorParaDesenho`, `distanciaNoCicloDeAndarEmMilesimos`); os NPCs falsos do teste de desempenho; as notificações na tela e a fila de pedidos (já aplicada antes de salvar).

## 3. Quando roda

- **Salvar:** só **entre dois ticks**, nunca no meio de um. Antes, aplica a fila de pedidos (`aplicarOsPedidosPendentes`).
  - pelo menu (Esc → Salvar) ou pelo salvamento rápido (**F5**, no último espaço manual usado; se nenhum foi usado, no espaço 1);
  - automático todo dia às 0h, depois da ordem do dia e da linha do relatório (FL9-3).
- **Carregar:** pela tela inicial (Continuar, Carregar), pelo menu (Esc → Carregar), pelo carregamento rápido (**F9**, o save mais novo, manual ou automático), ou pela linha de comando (`jogo --carregar arquivo.save`). A partida carregada começa **pausada** (FL9-6).

## 4. Regras passo a passo

### 4.1 Salvar

1. Aplique a fila de pedidos do jogador.
2. Monte a árvore: cabeçalho, depois cada seção da tabela da parte 2, **sempre na mesma ordem**. Listas sempre na mesma ordem (construções e NPCs pela posição; inventário pelo ID em texto, em ordem alfabética).
3. Converta a árvore em bytes (MessagePack).
4. Grave num arquivo temporário (`espaco1.save.tmp`); só depois de gravado por inteiro, troque pelo arquivo de verdade. Assim, se o jogo fechar no meio, o save antigo continua inteiro.
5. Mostre a notificação "Jogo salvo (Dia 12 do Verão, 14h)".

### 4.2 Carregar

1. Leia o arquivo. Se não existir ou estiver quebrado: mensagem "Não foi possível ler o save" e nada muda.
2. Confira a versão do formato. Diferente da atual: recuse com "Este save é de outra versão do jogo (versão 1; o jogo usa a 2)" e mantenha a partida atual (FL9-5).
3. Confira cada ID em texto contra os dados carregados. Um ID que não existe mais (por exemplo, `item.vinho` foi apagado do `itens.json`): recuse com o ID na mensagem e mantenha a partida atual (FL9-5).
4. Monte uma `SimulacaoDaVila` nova e vazia, com os mesmos dados e regras.
5. Preencha cada parte pela seção dela, na mesma ordem do salvamento. Os NPCs são criados no registro da EnTT **na ordem gravada**, para ganharem os mesmos números de entidade.
6. Recalcule o que não foi gravado (parte 2): o mapa bloqueado pelos lotes, as capacidades, o lugar de desenho igual ao lugar atual.
7. Só agora troque a partida antiga pela nova. Se algo falhou nos passos 1 a 6, a partida antiga continua.

### 4.3 Saves na tela

- **Tela inicial** (FL9-6): Continuar (o save mais novo, manual ou automático), Novo jogo, Carregar, Sair.
- **Lista de saves:** uma linha por espaço: nome, data do jogo, data real. Espaço vazio aparece como "(vazio)".
- **Menu do Esc** (FL9-6): Voltar, Salvar, Carregar, Sair para a tela inicial. Sair ou carregar pede confirmação se houver mais de 1 hora de jogo sem salvar. O Esc deixa de fechar o jogo (marco 1); fechar é pelo "Sair" da tela inicial ou pelo X da janela, que também pede a confirmação.

## 5. Contas

- **Tamanho do arquivo** (estimativa da fatia): 15 NPCs × cerca de 300 bytes + 20 construções × cerca de 200 bytes + 300 notificações × cerca de 80 bytes ≈ **33 KB**. Salvar e carregar levam bem menos de 1 segundo; o save automático diário não trava o jogo.
- **Tick do save:** dia 12 do Verão do ano 412, 14h → dia 16 (Primavera) + 12 = dia 28 da partida; tick = 27 × 19.200 + 14 × 800 = **529.600** (o tick 0 é a 0h do dia 1; a partida começa no 4.800, às 6h).

## 6. Conversa com outros sistemas

| Sistema | O que o save lê dele | O que entrega |
|---|---|---|
| [[Ficha Logica/Tempo\|Tempo]] | tick e velocidade | o relógio no tick gravado |
| [[Ficha Logica/IA\|IA]] e [[Ficha Logica/NPC\|NPC]] | os componentes de cada NPC | os NPCs recriados na mesma ordem |
| [[Ficha Logica/Producao\|Produção]], [[Ficha Logica/Inventario\|Inventário]], [[Ficha Logica/Economia\|Economia]], [[Ficha Logica/Felicidade\|Felicidade]], [[Ficha Logica/Eventos\|Eventos]] | o estado de cada um | o estado de volta |
| [[Ficha Logica/Construcao\|Construção]] | as construções e as obras | a lista, o mapa bloqueado recalculado |
| [[Ficha Logica/Interface\|Interface]] | o Diário e a câmera | a tela inicial, a lista de saves, o menu do Esc, a notificação |
| [[Ficha Logica/Simulacao\|Simulação sem tela]] | — | `--carregar` e `--salvar-no-dia` para os testes |

## 7. Nomes no código

- Pasta `Codigo/Save/` (como ficou no marco 9): `ArquivoDeSave.h/.cpp` (o ponto de entrada, os bytes, o `.tmp`, o cabeçalho), `GravarAPartida.cpp`, `CarregarAPartida.cpp` e `AcessoAoEstadoParaOSave.h` (a porta para os campos privados). As contas do dia (`consumoNaoPagoHoje` e as outras "de hoje") também vão no arquivo, para um save no meio do dia continuar igual.
- Funções: `montarASecaoDosNpcs`, `gravarAPartidaNoArquivo(const SimulacaoDaVila&, caminhoDoArquivo)`, `carregarAPartidaDoArquivo(dados, regras, caminhoDoArquivo)` → devolve a partida nova ou o motivo da recusa (`ResultadoDoCarregamento`).
- Constantes: `versaoDoFormatoDoSave`, `espacosDeSaveManual`.
- `Jogo/`: `TelaInicial.h/.cpp`, `MenuDoEsc.h/.cpp`, `ListaDeSaves.h/.cpp`.

## 8. Testes

1. **Ida e volta** (Fatia Vertical, parte 5): partida nova, 10 dias; salvar → carregar → salvar de novo → **os dois arquivos são iguais, byte a byte**.
2. **Continuar igual** (FL9-7, depende da FL9-2): `jogo --simular 64 --semente 1` dá um relatório; `jogo --simular 64 --semente 1 --salvar-e-carregar-no-dia 20` (salva às 0h do dia 20, carrega, segue) dá **o mesmo relatório, linha por linha**.
3. **Versão errada:** um save com a versão 0 é recusado com a mensagem, e a partida antiga continua.
4. **ID que sumiu:** um save com `item.vinho` é recusado com "item.vinho" na mensagem.
5. **Arquivo quebrado:** um save cortado pela metade é recusado, sem travar.
6. **Gravação segura:** o arquivo `.tmp` some depois de salvar; o save antigo só é trocado no fim.

## 9. Fora da fatia

- Salvar por **Mapa Local** e os mapas em LOD 2 só com os dados agregados (Arquitetura).
- Facções, diplomacia e relações em seções próprias.
- Conversores de saves antigos para a versão nova **[Refinar depois]**: até o lançamento, um save de outra versão é recusado (FL9-5).
- **[Refinar depois]** O caminho dos NPCs no save (FL9-2): na fatia são 15 NPCs (cerca de 2 KB). No jogo completo, com centenas de NPCs, gravar o caminho de todos pesa: só os NPCs importantes gravariam o caminho e os demais o recalculariam ao carregar (quem é importante vem dos dados; o autor define). Nesse caso, o teste "continuar igual" passaria a valer só para os importantes.
- Nomear o save, apagar save pela tela, miniatura da tela no save.
