# Rodada pendente

## Rodada HI8: os líderes do Ano 412 (2026-10-26)

**Contexto.** A HI7 foi respondida: o líder de cada facção é **fixo** (o herdeiro e os nobres continuam sorteados); cada povo tem o seu mito; Kassian Dravel morreu, e a espada dele está nas ruínas; o jogador só conhece o mito da própria cultura ([[Documentos/Mundo e História/Descrição|Mundo e História]], partes 4.3, 4.6 e 8). Também ficaram os nomes Teocracia de Theodoria e Ordem de Custodum. Agora, os 6 líderes. Cada um tem a Personalidade de 5 traços dos Importantes ([[Ficha Logica/Companheiros|Companheiros]]: Coragem, Ganância, Agressividade, Sociabilidade e Lealdade), mostrada aqui como alta ou baixa, e **um segredo**. Na Coroa, o "imperador" é Aurelian, que dorme; quem governa é um regente.

**Pergunta HI8-1: quem governa cada facção?** (uma letra por linha, ou uma ideia sua)

| Facção | Opção A | Opção B | Sugestão |
|---|---|---|---|
| **Coroa de Valdren** (romano) | **Regente Lívia Valdren**, 54 anos, da linhagem direta. Fria e paciente, governa "em nome do Eterno". Lealdade e Coragem altas; Sociabilidade baixa. **Segredo:** para Aurelian não definhar, a Coroa leva escravos com o dom mágico ao quarto selado, e eles nunca voltam | **Cônsul Marcus Severan**, 47 anos, um general que entrou na família pelo casamento. Quer acordar o Eterno com um novo ritual. Agressividade e Ganância altas. **Segredo:** não tem o sangue Valdren e teme ser descoberto | **A:** o segredo dela é a crueldade que o tema pede, e liga a Coroa ao Sultanato (de onde vêm os escravos) |
| **Sultanato de Zahran** (árabe) | **Sultão Harun al-Zahran**, 68 anos, dono do maior mercado de escravos do continente. Ganância altíssima; Coragem baixa. **Segredo:** está doente, e os emires já disputam quem fica no lugar dele | **Sultana Yasmin bint Qadir**, 35 anos, que subiu ao trono envenenando o irmão. Sociabilidade e Ganância altas; Lealdade baixa. **Segredo:** compra pólvora de Ashmoor às escondidas | **B:** uma governante que trai e negocia com todos dá mais história ao comércio e à pólvora |
| **Clãs de Tian-Lang** (chinês) | **Grande Lobo Tian Feng**, 51 anos, mudado pela mana, venceu o título num duelo, como manda a doutrina. Coragem e Agressividade altíssimas. **Segredo:** está envelhecendo e sabe que vai perder o próximo duelo | **Grande Loba Lang Mei**, 29 anos, mudada pela mana, matou o anterior em duelo. Coragem alta; Lealdade baixa. **Segredo:** sabe o caminho até a espada de Kassian nas ruínas | **A:** a velhice de um guerreiro numa cultura do mais forte é um drama pronto (a sucessão da parte 5) |
| **Comunidade de Ashmoor** (vitoriano) | **Lorde-Engenheiro Edmund Hale**, 60 anos, o presidente do Conselho da Comunidade. Lealdade e Coragem altas. **Segredo:** o filho dele nasceu com o dom mágico, e ele o esconde da lei que manda executá-lo | **Chanceler Victoria Ashcombe**, 44 anos, dona das maiores minas. Ganância altíssima; Sociabilidade alta. **Segredo:** as minas usam mais mutantes escravizados do que a lei permite, e ela vende parte deles ao Sultanato | **A:** o líder da lei contra a magia escondendo um filho mago é o dilema bom-ou-cruel que o jogador pode usar |
| **Teocracia de Theodoria** (bizantino) | **Sumo Sacerdote Theodoros IV**, 72 anos, o único que sabe o nome do Doador. Sociabilidade alta; Coragem baixa. **Segredo:** perdeu a fé; nunca ouviu nada do Doador | **Arquimandrita Irene Kallistos**, 38 anos, mudada pela mana e chamada de "santa viva". Coragem e Lealdade altas. **Segredo:** a mutação a está matando devagar | **A:** um chefe de fé sem fé, guardando um nome que talvez nem exista |
| **Ordem de Custodum** (latim erudito) | **Grão-Mestre Cassius Varro**, 131 anos, mantido vivo pelos próprios experimentos. Ganância e Agressividade altas. **Segredo:** sabe que Aurelian vive e tem um espião dentro da Coroa | **Magistra Octavia Lenn**, 40 anos, a maga mais forte da Ordem. Coragem alta; Lealdade baixa. **Segredo:** quer terminar o Ritual em si mesma, não para a Ordem | **A:** um homem de 131 anos prova que a imortalidade é possível e explica por que a Ordem não para |

**Pergunta HI8-2: os segredos dos líderes viram jogo?**

| Opção | O que é | Ganha | Perde |
|---|---|---|---|
| **A** | Só história: aparecem em livros, falas e notícias | Nada a programar | O jogador não faz nada com eles |
| **B** | O jogador pode **descobrir** um segredo (espionar, conversar, roubar cartas) e **usá-lo**: chantagear o líder, vendê-lo a uma facção inimiga ou revelá-lo ao povo (a Estabilidade cai). Entra como sistema numa ficha futura | É o "mal que dá mais caminhos" do tema; liga Crime, Diplomacia e Notícias | Uma ficha nova (Segredos) |
| **C** | B, mas só para o segredo da Coroa (o mais importante) | Menos trabalho | Os outros cinco ficam só na história |

**Sugestão: B**, com a ficha de Segredos entrando no Plano das Fichas depois da fase D.

**Conflitos:** a HI8-1 entra no `faccoes.json` e muda o código que hoje sorteia o líder (`Faccao.cpp`, regra 4.6 da Facção: idade de 8 a 18 anos no poder, Agressividade e Sociabilidade de 20 a 80); isso vira uma tarefa de código depois desta rodada. A HI8-2 B cria uma ficha nova.
