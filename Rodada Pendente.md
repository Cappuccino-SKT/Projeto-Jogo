# Rodada pendente

Aqui fica só a rodada aberta. Quando o autor responde, as decisões vão para as fichas (ou para o [[Documentos/Mundo e História/Descrição|Mundo e História]]) e a rodada sai deste arquivo.

## Rodada RV1: o que a revisão geral achou (2026-11-09)

Responda em `C:\Users\Felipe\Desktop\Respostas.txt` com o código (RV1-1...). "Seguir sugestão" aceita a sugestão.

**De onde vem:** a revisão geral do projeto comparou as decisões mais novas (a arte no estilo RimWorld, a história, as Raças, a rodada HI12) com os documentos e o código mais antigos. O que era só texto desatualizado já foi corrigido. Sobraram 5 pontos em que duas decisões não combinam, e só o autor pode escolher.

---

### RV1-1. O peso da arte com imagens de 64 por tile

**Contexto:** a conta do peso da arte no [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte]] era da pixel art (tile de 16 × 16) e dava menos de 100 MB para o jogo inteiro. Com a rodada AR1-1 (imagens de 64 × 64 por tile, boneco de 128 × 128), cada imagem pesa 16 vezes mais. A conta refeita:

| O que está na memória | Peso |
|---|---|
| O jogo inteiro carregado (6 culturas com forma própria) | cerca de 520 MB |
| Só o necessário (o terreno e 2 culturas dos mapas ativos) | cerca de 175 MB |
| O teto decidido (rodada V3-12; PC mínimo com placa de 2 GB) | 256 MB |

O tile de 16 aparece na tela com 32 a 64 pixels (zoom de 2x a 4x). Então a imagem de 64 é mostrada no tamanho real só no zoom máximo; nos outros, o jogo a reduz.

**Pergunta:** o que fazer para a arte caber no teto?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) Manter 64 por tile** e carregar só as culturas dos mapas ativos (a regra que já existe); medir na primeira leva de arte; se passar do teto, ligar a compressão da placa de vídeo (4 vezes menos memória) | A arte fica nítida no zoom máximo; nada muda para o autor | Folga pequena (175 de 256 MB); a compressão, se precisar, é trabalho de código |
| B) Baixar para 32 por tile (boneco de 64) | 4 vezes menos memória (cerca de 130 MB com tudo carregado) | No zoom de 4x a imagem é esticada e fica borrada; o manequim oficial precisa ser reduzido |
| C) 64 para os bonecos, 32 para o chão e as construções | Os personagens nítidos; cerca de metade da memória | Dois tamanhos para lembrar; as construções borradas de perto |

**Sugestão: A.** O teto de 256 MB continua. O jogo guarda também cópias menores de cada imagem para o zoom de longe (para a imagem reduzida não "piscar"), o que soma um terço ao peso: cerca de 235 MB no pior caso, ainda dentro do teto. Se a medição da primeira leva passar disso, a compressão entra.

**Conflitos:** nenhum com a AR1-1 (ela continua valendo). A opção B muda a AR1-1 e o Guia da Arte.

---

### RV1-2. A cor por cultura: troca de paleta ou tingimento

**Contexto:** o Pipeline diz que cada peça é desenhada uma vez e o jogo **troca as cores por uma tabela** (o metal vira ferro escuro na Guerreira e bronze na Escravista). Isso foi decidido para a pixel art, que tem poucas cores exatas. No estilo RimWorld, a imagem tem bordas suaves e sombras: são centenas de tons, e a tabela não acha as cores para trocar. A mesma regra aparece nas fichas da Cultura, dos Itens (a cor do material) e do Clima.

**Pergunta:** como o jogo dá a cor da cultura e do material a uma peça?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) Tingimento, como no RimWorld:** a peça é pintada em tons claros de cinza, e o jogo multiplica pela cor da cultura ou do material. Uma segunda imagem (a máscara) diz quais partes recebem a cor principal e quais a secundária | Funciona com borda suave e sombra; uma peça serve para as 6 culturas e para todos os materiais; não gasta memória a mais | O autor pinta as partes coloridas em cinza e marca a máscara; cada peça muda no máximo 2 cores |
| B) Uma imagem pronta para cada cor | A arte mais bonita e livre | 6 vezes mais imagens para desenhar e 6 vezes mais memória (piora a RV1-1) |
| C) Manter a tabela de paleta | Nada muda nos documentos | Exige arte sem suavização, com poucas cores exatas: não combina com o estilo novo nem com o que a IA gera |

**Sugestão: A,** com 2 cores por peça (principal e secundária). Exemplo: a túnica de camponês é pintada em cinza-claro; na Guerreira sai vermelho-sangue com detalhes cor de ferro, na Religiosa, branca com azul-celeste. A pele, o rosto e o cabelo usam o mesmo tingimento (os 5 tons de pele viram 5 cores, não 5 manequins). As estações continuam como já estão: um tom de cor sobre o mapa inteiro.

**Conflitos:** muda o trecho "Variações por cultura" do Pipeline e o passo a passo do Guia da Arte (o prompt do ChatGPT passa a pedir a roupa em tons de cinza). Não muda nenhuma regra de jogo.

---

### RV1-3. Quem nasce com o dom mágico: a ficha da Magia e a das Raças não combinam

**Contexto:** as duas fichas dão números diferentes para a mesma coisa.

| Ficha | O que diz | Numa cidade de 300 NPCs |
|---|---|---|
| [[Ficha Logica/Magia\|Magia]] (regra 4.1, rodada MG1-1) | 5% dos NPCs são Magos (15% na Arcana); nasce Mago com 3% de chance (10% na Arcana), 25% com um pai Mago e 60% com os dois | 15 Magos (45 na Arcana) |
| [[Ficha Logica/Racas\|Raças]] (parte 2) | O Humano nasce com o dom em 5 por mil (0,5%) | 1 ou 2 |
| Raças (a tabela dos povos) | Na Ordem de Custodum, 5% são Ascendidos e os outros 95% são "Humanos com o dom" | 300 |

A história diz que só "raros humanos" manipulam a mana e que, na Ordem, "quem não usa magia é inferior".

**Pergunta:** qual regra vale?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) Vale a Magia** (3% e 10%, com a herança dos pais). A raça só multiplica: Humano × 1; Langren, Khadim e Quitino nunca; Ascendido e Eterno sempre. Na Ordem: 5% Ascendidos, cerca de 15% Humanos com o dom (os membros da Ordem) e 80% Humanos sem o dom, que são o povo comum, de **segunda classe** | As contas da Magia (Torre Arcana, Magos no exército) continuam certas; a Ordem ganha um povo comum explorado, o que combina com a história | 1 em cada 20 pessoas com o dom é "raro", mas não "raríssimo"; Ashmoor executa cerca de 3% das crianças que nascem |
| B) Vale a ficha das Raças (0,5%) | O dom é raríssimo, como um milagre | Quase não há Magos: a Torre Arcana, a Encantaria e os Magos de batalha ficam sem gente; a ficha da Magia inteira precisa ser refeita |
| C) Na Ordem, só quem tem o dom é livre (cerca de 15%); os outros 85% são escravos dela | A Ordem fica a mais cruel das facções | Uma facção de escravos a mais; pesa na Escravidão (marco D3) e muda o equilíbrio com o Sultanato |

**Sugestão: A.** No `racas.json`, o campo `dom_magico_em_por_mil` vira `dom_magico`: "normal" (a regra da Magia), "nunca" ou "sempre". A lei "As outras raças" da Ordem no começo: as outras raças são escravizáveis (como já está); os Humanos sem o dom pagam o dobro de imposto (segunda classe). A Maré de Mana continua multiplicando a chance por 5 (3% vira 15%).

**Conflitos:** corrige a resposta "seguir sugestão" das RA1-3 e RA1-4 só no trecho da Ordem. A História (parte 4.4, "raros humanos") continua valendo.

---

### RV1-4. Quantos Ascendidos nascem

**Contexto:** a ficha das Raças quer que a elite da Ordem "quase não mude": cerca de 2 Ascendidos novos em 50 anos, para um total de 15. Mas a conta com os números da própria ficha dá mais: cada casal tem 0,5% de chance por estação, ou cerca de 1 filho em 50 anos (200 estações). Se os 15 formarem 7 casais (a chance de casar é de 25% por estação, como a de todos), nascem cerca de **7**, e a elite cresce quase 50%.

**Pergunta:** como chegar aos cerca de 2 em 50 anos?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) Baixar a fertilidade dos Ascendidos de 5% para 2%** | Muda um número só; 7 casais × 200 estações × 0,2% = cerca de 3 filhos | Nenhuma |
| B) Baixar a chance de casar dos Ascendidos (de 25% para 2% por estação) | Combina com "o instinto de ter filhos é quase nulo"; poucos casais | Uma regra nova por raça (a chance de casar) |
| C) Deixar como está (cerca de 7 em 50 anos) | Nada muda | A Ordem repõe os Ascendidos mortos com certa facilidade; a "raça mais rara" cresce |

**Sugestão: A.** A simulação de mesa 10.2 da ficha passa a dizer "cerca de 3 em 50 anos".

**Conflitos:** nenhum.

---

### RV1-5. A idade máxima do migrante

**Contexto:** o migrante chegava com idade sorteada de 4 a 12, quando o adulto ia dos 4 aos 20 (a primeira metade da vida adulta). A rodada HI12 mudou o adulto para 5 a 30. O mínimo já foi corrigido para 5 nesta revisão (um migrante de 4 anos seria uma criança trabalhando). O máximo continuou em 12: hoje todo migrante chega jovem, e a vila demora mais a ter velhos.

**Pergunta:** qual a idade máxima do migrante?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) 5 a 17** (a mesma proporção de antes: a primeira metade da vida adulta) | As idades da vila ficam variadas como antes da HI12 | Os sorteios mudam: os números do teste de 1 ano mudam um pouco |
| B) 5 a 12 (como está) | Nada muda | Migrantes sempre jovens |

**Sugestão: A.**

**Conflitos:** nenhum.
