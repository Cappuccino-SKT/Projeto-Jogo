# Rodada pendente

Aqui fica só a rodada aberta. Quando o autor responde, as decisões vão para as fichas (ou para o [[Documentos/Mundo e História/Descrição|Mundo e História]]) e a rodada sai deste arquivo.

## Rodada HI13: a história nova (a Colisão dos Mundos)

**De onde vem:** o autor refez a história em 2026-11-11. O [[Documentos/Mundo e História/Descrição|Mundo e História]] já está com a versão nova. Esta rodada decide o que o assistente propôs por conta própria e o que muda nas fichas e no código, que ainda estão com a história antiga (6 facções, as raças antigas, os 6 líderes).

Responda em `Respostas.txt`, pelo código (HI13-1...). "Seguir sugestão" aceita.

### HI13-1. De 6 facções para 5 reinos

**Contexto.** As fichas e o código têm 6 facções com nomes de trabalho (Guerreira, Arcana, Autoritária, Religiosa, Tecnocrata, Escravista); o `Jogo/data/faccoes.json` e os testes usam esses nomes. A história nova tem 5 reinos, e todos têm escravos.

**Pergunta.** Como os 5 reinos entram no lugar das 6 facções?

- **A.** Manter os nomes de trabalho e só trocar o nome que o jogador vê: Autoritária = Império Mágico; Arcana = União Mágica; Tecnocrata = Federação Humana; Religiosa = Clero de Rudeus; Guerreira = Reino Bárbaro. A Escravista é apagada. Ganha: muda pouco código. Perde: "Autoritária" e "Tecnocrata" descrevem mal os reinos novos.
- **B.** Trocar também os nomes de trabalho (Imperial, Uniao, Federacao, Clero, Barbaro). Ganha: o código fica igual à história. Perde: mexe em quase todos os arquivos de dados e testes.
- **C.** Como A, mas a região da Escravista no mapa provisório vira "terra sem dono" até o mapa novo.

**Sugestão:** C. Exemplo: a cidade que hoje é do Sultanato de Zahran fica neutra, com um mercado de escravos, e os 5 reinos disputam a região. Os ódios entre os reinos passam a ser os da parte 5.6 da história.

**Conflitos:** a tabela de ódios do Mundo e Estado Inicial; a cultura Escravista nas fichas de Cultura, Escravidão e Facção; os 6 líderes fixos no código.

### HI13-2. Os nomes: Rudeus e Rudeanos

**Contexto.** O seu texto chama a raça de "magos de outro mundo" e também fala de "magos humanos". No jogo, "mago" vai aparecer o tempo todo e os dois vão se confundir. E o nome "Clero de Rudeus" não era explicado.

**Pergunta.** Que nome próprio a raça e o mundo mágico recebem?

- **A.** O mundo mágico é **Rudeus**; a raça é **Rudeanos** (como eles se chamam); os humanos os chamavam de "Magos" e, no Antigo Império, de "deuses". O Clero prega a volta a Rudeus como um paraíso.
- **B.** Rudeus é o nome de um **deus inventado** pelo Clero; a raça ganha outro nome (por exemplo, "Celestes", porque vieram do céu).
- **C.** Sem nome próprio: continua "Magos do outro mundo".

**Sugestão:** A (já escrita no documento como [Proposta]). Exemplo: um fiel diz "os Santos vão nos levar a Rudeus", e o jogador descobre que Rudeus existe mesmo, mas a viagem não é para ele.

**Conflitos:** nenhum; o deus antigo ("o Doador") sai junto com a história antiga.

### HI13-3. O calendário e as datas

**Contexto.** O código começa o jogo no Ano 412 (antes, "depois da Fusão"). O seu texto não traz datas.

**Pergunta.** As datas da parte 4.1 servem?

- **A.** Sim: o jogo continua no **Ano 412**, contado da Grande Colisão e escrito "Ano 412 E.C." (Era da Colisão). Antigo Império de cerca de 75 a 330; os 5 reinos nascem de 330 a 370; a Estabilidade tem uns 40 anos.
- **B.** Começar mais tarde (por exemplo, Ano 600), para a Estabilidade ser mais longa e os reinos mais velhos. Muda um número no código e nos testes.
- **C.** Escrever "d.C." (depois da Colisão). Perde: confunde com "depois de Cristo".

**Sugestão:** A. Com 40 anos de paz, os avós ainda lembram a Queda, e isso deixa os ódios vivos.

**Conflitos:** os Magos do outro mundo precisam viver mais de 412 anos para os 12 do conselho serem os que vieram na Colisão (HI13-4).

### HI13-4. As raças na ficha

**Contexto.** A ficha de Raças tem Humanos, Langren, Khadim, Ascendidos, Quitinos e o Eterno, com idades e regras. Três das raças novas são quase iguais a raças antigas.

**Pergunta.** Aproveitamos os números das raças antigas?

- **A.** Sim: **Cinzento** herda os números do Langren (adulto aos 4, velho aos 24); **Humano-Cinzento** herda os do Khadim (fabricado, velho aos 10); **Mago do outro mundo** herda os do Ascendido (fertilidade de 2%), com a vida bem mais longa; **Mestiço** usa os do Humano, sempre com o dom mágico; **Fungoide** aproveita a regra de tribo dos Quitinos (a mente coletiva no lugar da rainha); **Humano-Peludo** é novo (sugestão: adulto aos 2, velho aos 12, fertilidade dobrada). O Eterno sai.
- **B.** Refazer a ficha do zero numa rodada só de raças.

**Sugestão:** A.

**Segunda parte: os nomes.** "Cinzento" e "Humano-Cinzento" são raças muito diferentes com nomes quase iguais. Sugestão: o Humano-Cinzento seria a tentativa do Antigo Império de copiar a força dos Cinzentos em laboratório, e ganharia um nome próprio (por exemplo, **Forjados**). Se preferir manter os dois nomes, fica como está.

**Conflitos:** a regra do dom mágico (Magia, RV1-3), a Câmara de Moldagem dos Khadim, as tribos Quitinas, o traço "Vida prolongada" de Cassius.

### HI13-5. Os líderes

**Contexto.** O código tem 6 líderes fixos com nome, idade e segredo. Nenhum cabe inteiro na história nova, mas quatro ideias ainda servem.

**Pergunta.** O que fazer com eles?

- **A.** Aproveitar as ideias e trocar os nomes depois: **Federação**, o líder cujo filho nasceu com o dom mágico (era Edmund Hale); **Reino Bárbaro**, o rei que está envelhecendo e sabe que vai perder o próximo duelo (era Tian Feng); **União**, o mais forte dos 7, mantido vivo por experimentos com um Mago do outro mundo preso (era Cassius Varro); **Império**, a imperatriz Mestiça da família que venceu a guerra (era Lívia Valdren); **Clero**, o chefe do conselho dos 12, que finge ser um sacerdote humano (novo).
- **B.** O autor escreve os 5 líderes novos.
- **C.** Deixar os líderes para depois do mapa novo; até lá o código fica com os antigos.

**Sugestão:** A agora, com os nomes definitivos depois do mapa.

**Conflitos:** os líderes e as dinastias no código (rodadas HI8, HI11 e HI12).

### HI13-6. Como as Grandes Histórias acordam

**Contexto.** A regra antiga (HI10) sorteava **uma** Grande História por partida. O seu texto novo dá a cada história um gatilho (a descoberta do pó explosivo, os recursos do Clero, a escassez dos bárbaros).

**Pergunta.** Qual regra vale?

- **A.** Só os gatilhos: qualquer história acorda quando a condição dela acontece, e mais de uma pode andar ao mesmo tempo. Ganha: o mundo decide. Perde: partidas caóticas, com 3 fins do mundo juntos.
- **B.** Sorteio de uma por partida, como antes; o gatilho só diz **quando** ela começa.
- **C.** Gatilhos, mas uma por vez: a primeira que acordar trava as outras até terminar (com vitória ou falha).

**Sugestão:** C. Exemplo: a Federação descobre o pó explosivo no ano 3 da partida; enquanto a campanha dela durar, o Clero continua juntando recursos, mas o ritual só pode começar depois.

**Também nesta pergunta:** as falhas que o assistente escreveu (parte 8) servem? E o nome "Perigo Nuclear" fica, ou vira um nome do mundo (por exemplo, "A Magia do Cataclismo")?

**Conflitos:** HI10-1 a HI10-4; a ficha D10 ainda não foi escrita.

### HI13-7. A Federação e a magia

**Contexto.** Dois buracos: (1) humanos podem nascer com o dom mágico, e a Federação é feita de humanos; (2) o estilo dela termina em "steampunk arcano", mas ela quer acabar com a magia.

**Pergunta.** Como a Federação lida com isso?

- **A.** (1) A lei manda exterminar quem nasce com o dom, e há famílias que escondem os filhos. (2) O pó explosivo e as máquinas dependem de um mineral mudado pela mana; o governo sabe e esconde. É o grande segredo da Federação.
- **B.** (1) Quem nasce com o dom é expulso, não morto. (2) O estilo vira só "steampunk", sem nada arcano.
- **C.** (1) Como A. (2) A Federação admite que usa minerais de mana e diz que "matéria não é magia".

**Sugestão:** A. Dá um segredo que o jogador pode descobrir e usar para rachar o reino.

**Conflitos:** nenhum com as fichas; a lei antiga de Ashmoor já executava magos.

### HI13-8. O exército do Reino Bárbaro

**Contexto.** A frase veio cortada: "...enquanto a vanguarda, um exército de escravos, cerca o inimigo por trás utilizando."

**Pergunta.** Utilizando o quê?

- **A.** O número: uma massa de escravos mal armados que fecha o cerco.
- **B.** Montarias ou monstros domados (os Humanos-Peludos não são atacados por alguns monstros).
- **C.** Outra coisa (escreva).

**Sugestão:** B. Dá uma cara própria ao Reino Bárbaro e usa a capacidade dos Peludos.

**Aviso:** o texto usa "vanguarda" para quem fica atrás dando apoio; no documento ficou "retaguarda". Se a ideia era mesmo a tropa da frente, me avise.

### HI13-9. O que sobra da história antiga

**Contexto.** Algumas peças antigas já têm regra em ficha.

**Pergunta.** O que fica?

- **Marés de Mana** (a mana de uma região fica intensa por uma estação e causa mutações): **sugestão: fica**, como resto da Transformação.
- **A cratera da capital e a espada de Kassian:** **sugestão: saem.** No lugar, as masmorras lendárias viram os laboratórios do Antigo Império e os esconderijos dos Magos do outro mundo.
- **O tema "Tudo tem dono":** **sugestão: fica**, com a segunda pergunta "quem é gente?" (parte 1 da história).
