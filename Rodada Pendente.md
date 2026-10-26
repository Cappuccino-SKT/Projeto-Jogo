# Rodada pendente

## Rodada AR1: o formato da arte no estilo RimWorld (2026-10-26)

**Contexto.** O autor trocou a pixel art pelo estilo do RimWorld: bonecos simples vistos de cima, manequim com as roupas em camadas, contorno grosso, cores lisas ([[Documentos/Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte]]; referências em `Documentos/Pipeline de Arte e Conteúdo/referencias/`). O assistente gerou um manequim de teste por código (frente, costas e lado, 4 tons de pele). Hoje o jogo é feito para a pixel art: o tile tem **16 pixels**, o zoom anda em **níveis inteiros** (2x, 3x e 4x) para os pixels não borrarem, e o boneco provisório tem quadros de 64 × 64 com 6 quadros de andar. Estas perguntas definem o formato novo.

**Pergunta AR1-1: de que tamanho são as imagens?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Como o RimWorld: o tile desenhado em **64 × 64** e o boneco em **128 × 128** (ocupa cerca de 1,5 tile na tela) | Detalhe para as golas, os bordados e as plumas das referências; boa nitidez de perto | Mais memória (16 vezes a do tile de 16); os mapas do Tiled, feitos em 16 px, precisam ser convertidos |
| **B** | Tile em **32 × 32** e boneco em **64 × 64** | Metade do caminho: leve | Perde detalhe no zoom de perto |
| **C** | Manter o mundo medido em tiles de 16 no Tiled e nas contas; só as **imagens** passam a ter 64 por tile (o jogo as reduz ao desenhar) | Os mapas e as contas não mudam | Duas medidas para lembrar (o tile do mapa e o da imagem) |

**Sugestão: C**, com as imagens no tamanho da A. Por dentro, o jogo já mede o mundo em tiles (a posição do NPC é em milésimos de tile), então só o desenho muda.

**Pergunta AR1-2: como o boneco se mexe?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Como o RimWorld: **uma imagem parada por direção** e por peça. O jogo faz o movimento: um balanço leve ao andar, a arma girando no ataque, o boneco deitado ao cair | Cerca de **3 desenhos por peça** no lugar de 162; a IA e o código conseguem fazer | Menos vida que uma animação desenhada |
| **B** | Poucos quadros: andar com 2 quadros, o resto como na A | Um pouco mais de vida | O dobro de desenhos no andar |
| **C** | Animação completa, como estava na pixel art (162 quadros por peça) | A mais bonita | Muito trabalho; difícil com IA |

**Sugestão: A.**

**Pergunta AR1-3: quantas direções?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | **3 desenhadas** (frente, costas e lado); o outro lado é o espelho | Como o RimWorld e o teste do assistente | Uma peça assimétrica (escudo numa mão só) troca de lado ao virar |
| **B** | **4 desenhadas** | Escudo e arma sempre na mão certa | Um terço a mais de desenho |

**Sugestão: A** (é o que a regra antiga já fazia).

**Pergunta AR1-4: como fica o zoom?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | **Livre e suave**, como o RimWorld: a roda do mouse vai de cerca de 0,5x a 4x, e a imagem é suavizada ao reduzir | Ver a vila inteira ou um boneco de perto | Muda a regra do "pixel inteiro" da [[Ficha Logica/Interface\|Interface]] (4.6) |
| **B** | Níveis inteiros, como hoje | Nada muda | Sem sentido fora da pixel art; pouca visão de longe |

**Sugestão: A.**

**Pergunta AR1-5: com que programa o autor corrige as imagens?**

| Opção | Programa | Ganha | Perde |
|---|---|---|---|
| **A** | **Krita** | Grátis, feito para pintura, camadas, muitos tutoriais em português | Pesado em PC fraco |
| **B** | **Inkscape** | Grátis, desenho em vetor (linhas sempre lisas, muda de tamanho sem perder qualidade) | Difícil de corrigir uma imagem que veio da IA |
| **C** | **GIMP** | Grátis e conhecido | Menos confortável para pintar |

**Sugestão: A.**

**Pergunta AR1-6: quando o código muda?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Agora, num marco curto antes do D2: o zoom livre e o manequim do assistente no lugar do boneco de teste | O jogo já mostra o estilo novo | Atrasa o D2 |
| **B** | Depois do D2 | O D2 sai antes | O jogo fica mais tempo com a arte velha |
| **C** | Quando o autor tiver a primeira leva de arte (o corpo e 2 ou 3 roupas) | Muda uma vez só, já com a arte de verdade | Sem data |

**Sugestão: C.** Até lá, o jogo segue com a arte de teste, e nada se perde.

**Conflitos:** a AR1-1 e a AR1-4 mudam a [[Ficha Logica/Interface|Interface]] (zoom, 4.6), o [[Ficha Logica/Mapa|Mapa]] (o desenho dos tiles) e o [[Ficha Logica/NPC|NPC]] (o quadro do boneco). A AR1-2 troca a tabela de animação do Pipeline. Depois da AR1, o [[Documentos/Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]] é reescrito.
