# Rodada pendente: FL9, salvar e carregar (marco 9)

**Perguntas enviadas em 2026-10-19, aguardando resposta.** A ficha nova é a [[Ficha Logica/Save|Save]]. Responder no `Respostas.txt` pelo código (FL9-1...). "Seguir sugestão" aceita a proposta.

**FL9-1. O formato do arquivo.**

- *Contexto:* a [[Arquitetura/Descrição|Arquitetura]] pede um "arquivo binário com número de versão". Binário é menor e mais rápido, mas não dá para abrir e ler num editor de texto.
- *Pergunta:* como os bytes são escritos? *Exemplo:* o save da fatia tem cerca de 33 KB; um erro "o Pão sumiu do estoque depois de carregar" precisa ser investigado.
- *Opções:*
  - (a) **MessagePack pela nlohmann/json**, que já usamos: o código monta uma árvore como a do JSON e a biblioteca a converte em binário. Ganha: nenhuma biblioteca nova, pouco código, e um comando `jogo --save-para-json arquivo.save` mostra o save em texto para investigar. Perde: um pouco maior e mais lento que um binário próprio (sem importância com 33 KB).
  - (b) binário próprio (número por número, em ordem fixa). Ganha: o menor e mais rápido. Perde: muito mais código, e qualquer erro de ordem estraga o arquivo sem aviso; ler para investigar exige uma ferramenta própria.
  - (c) JSON em texto. Ganha: abre no Bloco de Notas. Perde: muda a decisão da Arquitetura; o arquivo fica cerca de 2 vezes maior.
- *Sugestão:* (a).
- *Conflitos:* nenhum ((c) muda a Arquitetura).

**FL9-2. O caminho dos NPCs no save.**

- *Contexto:* a parte 5 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]] diz que os caminhos **não** vão no save (são recalculados). Mas o A\* tem empates: refeito a partir do meio do caminho, pode escolher outra diagonal de mesmo custo. Os pedidos de caminho que esperam o cálculo distribuído também mudariam de ordem.
- *Pergunta:* o save grava o caminho de cada NPC? *Exemplo:* às 7h12, a Padeira está no 5º tile de um caminho de 18 até a Padaria. Recalculado, o caminho novo passa por outro tile, ela chega 1 tick depois, o Pão sai 1 tick depois, e o relatório do dia já é diferente da partida sem save.
- *Opções:*
  - (a) **gravar o caminho** (a lista de tiles e o índice) e os pedidos que esperam. Ganha: a partida carregada é idêntica à que não foi salva (o teste da FL9-7 é possível). Perde: muda a parte 5 da Fatia Vertical; o arquivo cresce cerca de 2 KB (15 NPCs × até 40 tiles).
  - (b) recalcular, como está na Fatia Vertical. Ganha: arquivo menor, segue a decisão antiga. Perde: depois de carregar, a partida pode divergir aos poucos (só o teste "salvar, carregar e salvar dá o mesmo arquivo" continua valendo).
- *Sugestão:* (a).
- *Conflitos:* muda a parte 5 da Fatia Vertical (os caminhos passam para a coluna "Grava").

**FL9-3. Quantos espaços de save e o save automático.**

- *Contexto:* a fatia não diz quando nem onde o jogador salva.
- *Pergunta:* quantos espaços manuais e quando o jogo salva sozinho? *Exemplo:* o jogador joga 2 horas, esquece de salvar, e o jogo fecha por um erro.
- *Opções:*
  - (a) **3 espaços manuais + 1 automático, gravado todo dia do jogo às 0h** (a cada 20 minutos reais a 1x). F5 salva no último espaço usado, F9 carrega o último save. Ganha: perde-se no máximo 1 dia de jogo; dá para guardar 3 momentos. Perde: um pouco mais de tela (a lista de espaços).
  - (b) 1 espaço manual só (F5 e F9), sem automático. Ganha: o mais simples. Perde: o erro do exemplo apaga as 2 horas.
  - (c) 3 manuais + automático a cada estação (16 dias, 5 horas e 20 minutos reais a 1x). Ganha: menos gravações. Perde: pode perder horas de jogo.
- *Sugestão:* (a).
- *Conflitos:* nenhum. F5 e F9 estão livres (F1 a F4 são de depuração).

**FL9-4. A pasta dos saves.**

- *Contexto:* o jogo roda hoje da pasta `build/`, que pode ser apagada para compilar do zero.
- *Pergunta:* onde ficam os arquivos `.save`? *Exemplo:* você apaga a pasta `build/` para resolver um erro de compilação.
- *Opções:*
  - (a) **na pasta do usuário que o SDL indica** (`SDL_GetPrefPath`, no Windows `C:\Users\Felipe\AppData\Roaming\Projeto-Jogo\saves\`); o `LEIA-ME.md` diz o caminho. Ganha: sobrevive a apagar o `build/`; é o lugar padrão dos jogos no Windows, e um jogo instalado em "Arquivos de Programas" não pode gravar na própria pasta. Perde: a pasta fica escondida (AppData).
  - (b) numa pasta `saves/` ao lado do executável. Ganha: fácil de achar. Perde: some ao apagar o `build/`; não funciona num jogo instalado.
- *Sugestão:* (a).
- *Conflitos:* nenhum.

**FL9-5. Save de outra versão ou com dados que mudaram.**

- *Contexto:* durante o desenvolvimento, o formato do save e os arquivos de `data/` mudam muito.
- *Pergunta:* o que acontece ao carregar um save velho? *Exemplo:* no marco 10 a Felicidade ganha um fator novo (versão 2 do formato), e você abre um save da versão 1; ou o `item.vinho` foi apagado do `itens.json`.
- *Opções:*
  - (a) **recusar com uma mensagem clara** ("Este save é de outra versão do jogo"; "O save usa o item.vinho, que não existe mais") e manter a partida atual. Ganha: nenhum erro escondido; quase nenhum código. Perde: os saves de teste se perdem a cada mudança.
  - (b) conversores de versão (uma função que transforma a versão 1 na 2). Ganha: os saves duram. Perde: um conversor por mudança, trabalho que só vale depois do lançamento.
  - (c) carregar o que der e preencher o que falta com o valor de partida. Ganha: quase sempre abre. Perde: a partida pode ficar errada sem aviso (estoque sem o item, NPC sem fator).
- *Sugestão:* (a) até o lançamento; os conversores ficam como [Refinar depois].
- *Conflitos:* nenhum.

**FL9-6. A tela inicial e o menu do Esc.**

- *Contexto:* hoje o jogo abre direto numa partida nova. Para carregar, é preciso um lugar na tela.
- *Pergunta:* como o jogador chega aos saves? *Exemplo:* você abre o jogo para continuar a partida de ontem.
- *Opções:*
  - (a) **tela inicial** (Continuar, Novo jogo, Carregar, Sair) **e menu do Esc** durante o jogo (Voltar, Salvar, Carregar, Sair para a tela inicial), feitos com o Dear ImGui, como o resto da interface da fatia. Sair ou carregar pede confirmação se houver mais de 1 hora de jogo sem salvar. A partida carregada começa pausada. Ganha: o fluxo normal de um jogo. Perde: duas telas novas.
  - (b) só F5 e F9 e a opção `--carregar` na linha de comando, sem tela. Ganha: o mínimo de código. Perde: sem lista de saves; um jogador não saberia usar.
- *Sugestão:* (a).
- *Conflitos:* hoje o Esc **fecha o jogo** na hora (marco 1); com a (a), o Esc abre o menu, e fechar passa a ser pelo "Sair" (ou o X da janela, que também pede confirmação).

**FL9-7. O teste do marco 9.**

- *Contexto:* o marco 9 fica pronto quando "o teste do save passa" (Fatia Vertical, parte 7). Hoje esse teste é só "salvar, carregar e salvar de novo dá o mesmo arquivo".
- *Pergunta:* o teste também exige que a partida carregada continue igual? *Exemplo:* o mapa bloqueado pelos lotes não vai no arquivo (é recalculado ao carregar). Um erro que esquece de bloquear o lote de uma obra terminada passa no teste de hoje (os dois arquivos saem iguais), mas os NPCs da partida carregada passam a andar por dentro da construção.
- *Opções:*
  - (a) **os dois testes:** o de hoje e o "continuar igual": `jogo --simular 64 --semente 1` e o mesmo com `--salvar-e-carregar-no-dia 20` dão o mesmo relatório, linha por linha. Ganha: pega qualquer coisa esquecida no save. Perde: exige a FL9-2 (a); mais 45 s no `ctest`.
  - (b) só o de hoje. Ganha: segue a decisão antiga. Perde: deixa passar o erro do exemplo.
- *Sugestão:* (a).
- *Conflitos:* depende da FL9-2 (a); muda a linha do marco 9 na parte 7 da Fatia Vertical.
