# Rodada pendente

Aqui fica só a rodada aberta. Quando o autor responde, as decisões vão para as fichas (ou para o [[Documentos/Mundo e História/Descrição|Mundo e História]]) e a rodada sai deste arquivo.

## Rodada LE1: as leis dos 5 reinos

**De onde vem:** o próximo marco de código é o D3 (Escravidão). Ao abrir a ficha, achei um problema: as **leis de cada reino no jogo ainda são as das facções antigas**, e várias contradizem a história nova. Hoje, no `faccoes.json`, a escravidão é **proibida** na União, na Federação e no Clero, e a única facção com escravos no começo era a Escravista, que saiu. Sem decidir isso, o marco D3 começaria com **nenhum escravo no mundo**.

Responda em `Respostas.txt`, pelo código (LE1-1...). "Seguir sugestão" aceita.

### LE1-1. A lei de Escravidão e os escravos do primeiro dia

**Contexto.** A lei tem 5 opções: proibida, só prisioneiros, como pena, ampla, qualquer um é propriedade. A ficha da Escravidão põe 30% de escravos só na Escravista. Pela história nova, quase todos os reinos têm escravos.

**Pergunta.** Qual lei e quantos escravos cada reino tem no primeiro dia?

| Reino | Lei de hoje | Sugestão de lei | Escravos no começo (sugestão) |
|---|---|---|---|
| Império Mágico | Como pena | **Ampla** | **30%** dos NPCs |
| União Mágica | Proibida | **Ampla** | **25%** |
| Federação Humana | Proibida | **Ampla** | **20%** (os 20% de "outras raças" da história são escravos) |
| Clero de Rudeus | Proibida | **Proibida** (é a terra que acolhe a todos) | **0%** |
| Reino Bárbaro | Só prisioneiros | **Só prisioneiros** | **15%** (capturados nos saques) |

- **A.** A tabela acima.
- **B.** Todos os reinos, menos o Clero, com "ampla" e 30%. Ganha: simples. Perde: os reinos ficam iguais.
- **C.** Outros números (escreva).

**Sugestão:** A. Exemplo: numa cidade do Império com 300 NPCs, 90 começam Escravos, nos empregos Braçais, com Senzala. O Clero sem escravos vira o destino natural dos fugitivos (a tensão 5 da história).

**Conflitos:** a regra ES1-6 da Escravidão; o desejo de cada cultura no `culturas.json` (o desejo é a lei inicial); os números do Mundo (a comida e o dinheiro das cidades mudam com 20 a 30% de gente sem salário): o teste de equilíbrio do mundo precisa ser refeito no marco.

### LE1-2. A lei de Magia

**Contexto.** As opções: só magos governam, privilégio dos magos, livre, restrita, proibida, magos banidos. Hoje o Império está com "restrita", mas na história ele é governado por Mestiços, todos magos.

**Pergunta.** Qual é a lei de Magia de cada reino?

| Reino | Lei de hoje | Sugestão |
|---|---|---|
| Império Mágico | Restrita | **Privilégio dos magos** |
| União Mágica | Privilégio dos magos | **Só magos governam** (a assembleia dos 7 mais fortes) |
| Federação Humana | Proibida | **Proibida** (o "magos banidos" fica como o pensamento extremo, aonde a Grande História dela leva) |
| Clero de Rudeus | Restrita | **Livre** (todos, com ou sem magia) |
| Reino Bárbaro | Restrita | **Livre** (só a força importa) |

- **A.** A tabela acima.
- **B.** A Federação já começa com "magos banidos" (o extermínio desde o primeiro dia). Perde: a Grande História dela não tem para onde crescer.

**Sugestão:** A.

**Conflitos:** o pensamento extremo da cultura Arcana é "só magos governam": se a lei inicial da União já é o extremo, a assembleia nunca mexe nela (achado antigo da ficha da Facção). Se preferir evitar, a União fica com "privilégio dos magos" como hoje.

### LE1-3. O governo e a fé de cada reino

**Contexto.** Três leis descrevem o governo: o poder do líder, o sistema de votação e a tolerância religiosa. Algumas não batem com a parte 5.2 da história.

**Pergunta.** Trocamos as que não batem?

| Reino | Hoje (poder; votação; fé) | Sugestão |
|---|---|---|
| Império Mágico | Autoritário; conselho; fé oficial | Não muda |
| União Mágica | Moderado; conselho; fé oficial | Não muda (os 7 são o conselho) |
| Federação Humana | Fraco; por população; tolerância | **Moderado; conselho** (só "os escolhidos" votam); tolerância |
| Clero de Rudeus | Moderado; dois terços; perseguição | Moderado; **conselho** (os 12); **fé oficial** (um reino que abre as portas a todos não pode perseguir quem chega) |
| Reino Bárbaro | Moderado; maioria simples; tolerância | **Autoritário** ("o rei decide tudo"); maioria simples; tolerância |

- **A.** A tabela acima.
- **B.** Não mexer agora; deixar para quando a Diplomacia e a Guerra forem programadas.

**Sugestão:** A. São 5 valores num arquivo de dados.

**Conflitos:** o desejo das culturas no `culturas.json`; o teste da assembleia (no começo ninguém propõe lei nova, porque a lei já é o desejo).

### LE1-4. As 4 cidades sem dono (a antiga Escravista)

**Contexto.** O Grande porto, a Cidade dos mercados, a Plantação de algodão e o Porto de pesca ficaram sem facção. Uma cidade sem facção hoje não tem lei de escravidão nenhuma. A rodada HI13-1 dizia que a região fica "neutra, com um mercado de escravos".

**Pergunta.** Essas cidades têm escravos e Mercado de Escravos?

- **A.** Sim: a cidade sem facção segue a lei que a **cultura dela** deseja (a cultura Escravista deseja "ampla"). As 4 começam com 30% de escravos, e o Grande porto e a Cidade dos mercados têm Mercado de Escravos. Ganha: um polo neutro de tráfico, onde os 5 reinos compram. Perde: uma regra nova ("sem facção, vale o desejo da cultura").
- **B.** Não: cidade sem facção não tem escravos. Os Mercados ficam só nas capitais dos reinos que permitem.
- **C.** Só os Mercados, sem escravos próprios: as cidades só revendem.

**Sugestão:** A. Exemplo: o jogador capturado por bandidos perto de (6,6) é levado ao Mercado da Cidade dos mercados, e um comprador do Império o leva embora.

**Conflitos:** a regra 4.8 da Escravidão (para onde o jogador capturado é levado); as Vilas Livres do Norte, do Centro e do Sul continuam sem escravos (a cultura delas não deseja).
