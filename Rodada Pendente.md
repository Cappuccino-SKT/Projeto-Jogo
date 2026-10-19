# Rodada pendente: FL8, as fichas do marco 8 (a interface, as obras e as decisões do jogador)

**Perguntas enviadas em 2026-10-19, aguardando respostas.** As fichas do marco 8 estão escritas: a parte 10 da [[Ficha Logica/Interface|Interface]] (o HUD, os painéis e os pedidos), a parte 10 da [[Ficha Logica/Construcao|Construção]] (as obras e a manutenção) e a parte 11 da [[Ficha Logica/Economia|Economia]] (o preço, o salário, a contratação, a compra e a venda). Os detalhes de cada sugestão estão nelas, marcados [Proposta]. (A FE1 foi aplicada em 2026-10-19.)

Responder no `Respostas.txt` pelos códigos (FL8-1 a FL8-9). "Seguir sugestão" aceita a proposta.

**FL8-1. Como desenhar a interface na fatia.**

- *Contexto:* a parte técnica da [[Interface/Descrição|Interface]] diz que a interface do jogador é feita pelo **nosso desenhista**, com layouts em JSON, e que o Dear ImGui fica **só para a depuração**. Hoje o nosso desenhista só desenha quadrados e imagens: não escreve texto, não tem botão, lista, tabela nem campo de número.
- *Pergunta:* no marco 8, o HUD e os painéis usam o Dear ImGui ou o nosso desenhista? *Exemplo:* o painel da Carpintaria, com as 2 ordens, um campo "N: 20", os botões ↑ ↓ × e os 3 botões do salário.
- *Opções:*
  - (a) **o Dear ImGui, só na fatia**, com um tema de cores do jogo. As contas e os textos (o dinheiro formatado, os alertas, as notificações) ficam no `Codigo/`, testados; o ImGui só desenha e lê os cliques. Depois da fatia, o nosso desenhista troca só a parte do desenho. Ganha: botões, tabelas, campos e janelas prontos; o marco 8 fica bem menor. Perde: cara de ferramenta, não de jogo; o desenho dos painéis é feito duas vezes (a parte menor);
  - (b) o nosso desenhista já: texto com o `stb_truetype` (o stb já está no projeto), molduras de cor lisa, botões e listas nossos, layouts em JSON. Ganha: igual ao documento, nada jogado fora. Perde: o marco 8 fica 2 a 3 vezes maior (texto, clique, rolagem, campo de número, o foco do teclado), muito código novo, e o visual continua provisório até a arte;
  - (c) misto: a barra de cima, os alertas e as notificações pelo nosso desenhista; os painéis no ImGui. Ganha: o que fica sempre na tela já é o definitivo. Perde: duas formas de interface ao mesmo tempo, e o texto nosso precisa ser feito mesmo assim.
- *Sugestão:* (a).
- *Conflitos:* muda a parte técnica da Interface só na fatia.

**FL8-2. A fonte do texto.**

- *Contexto:* a Interface pede fontes comuns e legíveis, a resolução de 1600 × 900 e a escala de 100% a 200%. O ImGui traz uma fonte de pixel pequena (13 px), com os acentos do português. O repositório é público, então a fonte precisa poder ser distribuída.
- *Pergunta:* qual fonte e qual tamanho? *Exemplo:* "Ano 1, Primavera, dia 3 · 07:40 · Manhã" na barra de cima.
- *Opções:*
  - (a) **a Noto Sans** (licença SIL OFL, livre para distribuir), 18 px, em `Jogo/assets/fontes/` com o arquivo da licença; a escala fica em 100% até o perfil do jogador. Ganha: legível, todos os acentos, licença simples. Perde: um arquivo de cerca de 500 KB no repositório;
  - (b) a fonte do ImGui. Ganha: nada a baixar. Perde: pequena em 1600 × 900; cara de depuração;
  - (c) outra fonte livre que o autor escolher (dizer o nome).
- *Sugestão:* (a).
- *Conflitos:* nenhum.

**FL8-3. Quem encomenda as obras e quem paga.**

- *Contexto:* pela V2-8, a construtora (o Construtor) faz as obras do jogador e da cidade, com o custo por porte. A vila tem 11 lotes, e só o **lote 5** (pequeno) está livre. Os 2 Operários do Construtor (um é o gerente) recebem do Banco e hoje não fazem nada. A Cidade não tem uma IA que decida obras, e a população não cresce na fatia (sem migração), então nunca falta moradia.
- *Pergunta:* na fatia, quem encomenda uma obra e como paga? *Exemplo:* o jogador quer uma Fazenda no lote 5 (pequena: 10 Tábuas + 10 Blocos e 40 h de obra).
- *Opções:*
  - (a) **só o jogador encomenda** (num lote livre ou para subir o nível de uma construção dele); a Cidade só faz a manutenção (FL8-5). Ele paga na hora: as Tábuas e os Blocos saem do estoque dele, e o que faltar ele compra da loja da cidade a 110%; e paga ao Banco as horas × 0,75 (o salário Braçal), porque os Operários já recebem do Banco. No exemplo: as 10 Tábuas dele, 10 Blocos a 2,0 (20,0) e 40 h × 0,75 (30,0): **50 moedas**; pronta em 2,5 dias. Ganha: simples; o dinheiro do jogador volta para o Banco (ajuda o que a FE1-1 corrigiu); os Operários ganham trabalho. Perde: a vila não cresce sozinha;
  - (b) como (a), mais a Cidade construindo uma Casa quando faltar moradia. Perde: na fatia nunca falta, então a regra ficaria sem teste;
  - (c) o jogador constrói "à mão", com os empregados dele, sem o Construtor: paga só os materiais, e a construção dele para enquanto eles constroem. Ganha: mais barato. Perde: outra forma de obra e regras novas na rotina.
- *Sugestão:* (a).
- *Conflitos:* nenhum; a obra "à mão" fica para depois da fatia.

**FL8-4. O Engenheiro e o nível máximo na fatia.**

- *Contexto:* pela IP3-11 e a V2-8, sem Engenheiro só há obras de nível 1, e o Engenheiro precisa de Inteligência 30, 45, 60 e 75 para os níveis 2 a 5. Os NPCs da fatia não têm Inteligência (só a habilidade 50 no Braçal e no Manufaturado, FL6-6). A Carpintaria do jogador já está no nível 2; subir para o 3 (5 vagas) custa 60 Tábuas + 90 Blocos e 360 h: cerca de **450 moedas**, e 22,5 dias com os 2 Operários.
- *Pergunta:* até que nível dá para subir na fatia? *Exemplo:* no segundo ano, o jogador junta 450 moedas e quer a Carpintaria no nível 3.
- *Opções:*
  - (a) **o gerente do Construtor conta como Engenheiro com Inteligência 50: obras até o nível 3** (o 4 e o 5 pedem 60 e 75). Ganha: uma meta para o dinheiro do jogador, sem atributo novo. Perde: um número fixo até os atributos entrarem;
  - (b) só obras de nível 1 (nenhuma subida). Ganha: menos regras. Perde: o jogador só tem o lote 5 para investir;
  - (c) pôr a Inteligência nos NPCs agora (no `vila_inicial.json`). Ganha: a regra inteira. Perde: um atributo que só serve para isso na fatia.
- *Sugestão:* (a).
- *Conflitos:* nenhum (a regra da V2-8 continua; só a Inteligência é fixa na fatia).

**FL8-5. A manutenção por estação.**

- *Contexto:* pela V2-8, a cada estação, cada construção gasta 5% dos materiais do nível atual (no mínimo 1 Tábua) e 5% das horas, feitas pelos Operários. Na vila, por estação: a Cidade gasta **9 Tábuas, 11 Blocos e 26 h** (os 150 Blocos duram cerca de 3 anos), e a Carpintaria do jogador, 2 Tábuas, 3 Blocos e 12 h. **A Cidade não faz Tábua: só o jogador.**
- *Pergunta:* de onde vêm as Tábuas da Cidade, e o que acontece sem a manutenção? *Exemplo:* no primeiro dia do Verão, o jogador tem 0 Tábuas.
- *Opções:*
  - (a) **a Cidade compra as Tábuas do jogador pelo preço dele** e usa os Blocos dela; o jogador paga a da Carpintaria (os materiais + as horas ao Banco). A manutenção vem antes das obras novas. Sem Tábuas, ela espera e tenta de novo a cada dia. Uma construção que passa a estação inteira sem a manutenção fica "atrasada" e **produz 75%** até ela ficar pronta. Ganha: as Tábuas do jogador ganham um comprador, e a manutenção importa. Perde: mexe no equilíbrio da FE1 (conferir as 10 sementes no código do marco 8);
  - (b) como (a), mas sem efeito na produção: só o alerta, até a Vida das construções entrar. Ganha: não mexe no equilíbrio. Perde: a manutenção não importa;
  - (c) a manutenção fica para depois da fatia. Ganha: menos código. Perde: os Blocos da Cidade ficam parados.
- *Sugestão:* (a).
- *Conflitos:* nenhum (a FL6-1 já punha a manutenção no marco 8).

**FL8-6. O preço do jogador (a faixa de ±30%).**

- *Contexto:* pela FT1-1 e o [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]], o jogador fixa o preço numa faixa de ±30% em torno do preço local; "mais caro, vende pouco ou nada". Hoje a classe paga o preço local. Ela compra os Móveis da cesta (cerca de 2 por dia) e, com o gasto extra, até o dobro (FL7-1). Só o jogador faz Móveis e Tábuas, então ninguém concorre com ele.
- *Pergunta:* como a venda muda com o preço? *Exemplo:* os Móveis a 5,1 no local; o jogador põe +20% (6,1).
- *Opções:*
  - (a) **de −30% a +30%, de 5 em 5; a cesta compra sempre; o gasto extra compra uma fração**: tudo com 0% ou menos e (30 − p) ÷ 30 acima (com +15%, metade; com +30%, nada); o resto do gasto extra vai para a loja da cidade, como hoje. No exemplo, com +20%: a cesta paga 6,1 por Móvel e o gasto extra compra 1/3 do que compraria. Ganha: o jogador escolhe entre preço e volume (a +30% ele ganha menos que a 0%). Perde: a cesta paga caro sem reagir;
  - (b) a cesta também compra menos com o preço alto (sem Móveis, a Felicidade cai). Ganha: mais real. Perde: mexe na cesta mínima, e a Felicidade da fatia só é medida (FL7-7);
  - (c) o preço só fica na faixa, e a classe compra tudo igual. Perde: +30% é sempre melhor, e o dinheiro volta a sair do Banco (o problema da FE1-1).
- *Sugestão:* (a).
- *Conflitos:* nenhum; o padrão é 0%, e o teste de 1 ano não muda.

**FL8-7. O nível de salário, a contratação e a demissão.**

- *Contexto:* pelo [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]]: Baixo × 0,75 (menos Felicidade e lealdade, mais rotatividade), Normal × 1, Alto × 1,25 (mais produtividade, trabalhadores melhores); o jogador contrata desempregados pelo painel, até as vagas. Na fatia não há lealdade, e todos têm habilidade 50. A Carpintaria tem 3 vagas (2 ocupadas) e há 3 desempregados.
- *Pergunta:* o que cada nível faz na fatia, e como contratar e demitir? *Exemplo:* com Baixo, o Manufaturado da Carpintaria ganha 0,825 por hora em vez de 1,1.
- *Opções:*
  - (a) **Baixo: salário × 0,75 e velocidade × 0,9; depois de 8 dias seguidos em Baixo, um empregado sai (o mais novo na construção). Alto: × 1,25 e velocidade × 1,1.** Contratar: um desempregado, na hora, se houver vaga. Demitir: na hora, sem indenização, com as horas do dia pagas. Ganha: uma escolha de verdade (custo contra produção), com regras curtas. Perde: números fixos até a lealdade entrar;
  - (b) só o salário muda. Ganha: o mínimo. Perde: Baixo é sempre melhor;
  - (c) como (a), e o nível também entra na Felicidade da classe. Perde: a Felicidade da fatia só é medida (FL7-7), então não muda nada ainda.
- *Sugestão:* (a).
- *Conflitos:* nenhum.

**FL8-8. Comprar e vender sem o personagem.**

- *Contexto:* pela FT1-1, o jogador "anda, compra e vende no Mercado". O personagem do jogador ainda não existe no mapa (a parte 9 do [[Ficha Logica/NPC|NPC]] deixa para depois). A loja da cidade vende a 110% e, a partir do marco 8, compra a 90% do preço local (V2-5).
- *Pergunta:* como o jogador compra e vende na fatia? *Exemplo:* vender 10 Tábuas com o local a 1,5: 1,5 × 90% = 1,4 cada, 14,0 no total.
- *Opções:*
  - (a) **pela aba Mercado, sem andar**: o item e a quantidade; comprar a 110% (o item sai da Cidade, o dinheiro vai para o Banco) e vender a 90% (o contrário; recusado se o Banco não tiver o dinheiro). O personagem andando fica para depois da fatia. Ganha: o jogador mexe no estoque já no marco 8; pouco código. Perde: não tem o "andar até o Mercado";
  - (b) fazer o personagem agora: um NPC Controlado que anda com o botão direito, e a compra só abre perto do Mercado. Ganha: igual à FT1-1. Perde: o marco 8 cresce muito (seleção, ordem de andar, o Controlado, a Fome do jogador);
  - (c) sem compra e venda na fatia. Perde: o jogador não consegue os Blocos para as obras (só a Cidade tem).
- *Sugestão:* (a).
- *Conflitos:* muda a FT1-1 só na fatia ("andar" fica para depois).

**FL8-9. As notificações e os alertas.**

- *Contexto:* pela Interface, as notificações têm 3 níveis (Info, Aviso e Crítico, que pausa), ficam empilhadas à direita e o jogador personaliza tudo; há uma barra de alertas e um diário de 300 entradas (a ficha dos Eventos deixou o diário para o marco 8). Hoje os eventos só vão para o console.
- *Pergunta:* o que entra no marco 8? *Exemplo:* no dia 27, a Febre pega o Fábio.
- *Opções:*
  - (a) **notificações de texto à direita** (os eventos, as obras prontas, um empregado que sai): Info para as boas, Aviso para as ruins; até 5 na tela, 20 segundos reais cada; sem pausa automática; a aba Diário guarda as 300 últimas. **Uma barra com 4 alertas**: Pão para menos de 2 dias; o Banco abaixo de 10% do começo da estação; uma construção do jogador parada por falta de insumo; a manutenção atrasada. Ganha: o jogador vê o que acontece sem o console. Perde: sem personalização (fica para o perfil do jogador);
  - (b) como (a), sem o Diário. Ganha: um pouco menos de código. Perde: o que sumiu da tela se perde;
  - (c) só o console, como hoje.
- *Sugestão:* (a).
- *Conflitos:* nenhum; o Crítico com pausa fica para depois (nenhum evento da fatia é tão grave).

Depois das respostas: aplicar nas fichas (tirar as marcas [Proposta] decididas), anotar a rodada na parte 8 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]] e programar o marco 8.
