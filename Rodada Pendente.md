# Rodada pendente

Aqui fica só a rodada aberta. Quando o autor responde, as decisões vão para as fichas (ou para o [[Documentos/Mundo e História/Descrição|Mundo e História]]) e a rodada sai deste arquivo.

## Rodada RA1: as Raças (ficha D8)

Responda em `C:\Users\Felipe\Desktop\Respostas.txt` com o código (RA1-1...). "Seguir sugestão" aceita a sugestão.

**O que já está decidido:**
- As raças nasceram da mana da Grande Fusão misturada aos humanos (HI2-2). Antes da Fusão só existiam humanos.
- Os Clãs de Tian-Lang (Guerreira) são em boa parte dessas raças (HI2-4).
- Cada raça tem a sua idade de adulto e o seu tempo de vida.
- As raças entram no jogo depois dos marcos da fase D.
- Cada raça precisa do seu manequim no estilo de arte novo.

**As idades do jogo:** hoje o humano é adulto aos 5 e velho aos 31; na história, isso dá cerca de 12 e 77 anos (÷ 2,5).

---

### RA1-1. Quantas raças e quais?

**Contexto:** cada raça nova custa um manequim e as roupas redesenhadas para o corpo dela, além dos números (idades e habilidades). Poucas raças, bem diferentes, valem mais que muitas parecidas.

**Pergunta:** quantas raças, além dos humanos, e como elas são?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) 3 raças**, uma para cada jeito em que a mana mudou o corpo | Cada uma com um papel claro; a arte cabe | Menos variedade |
| B) 5 ou mais raças | Um mundo mais variado | Muita arte e números para equilibrar |
| C) 1 raça, com graus de mutação (leve, forte) | A arte mais barata | O mundo fica pouco diferente do humano |

**Sugestão (A):** os nomes são provisórios; troque à vontade.

| Raça | Como a mana a mudou | Adulto e velho (jogo) | Na história | Força e fraqueza |
|---|---|---|---|---|
| **Humanos** | Não mudou | 5 e 31 | 12 e 77 | Medianos em tudo |
| **Ferinos** | O corpo virou meio fera: garras, pelos e dentes | 4 e 21 | 10 e 52 | Fortes no combate; vida curta; ruins no Ofício fino |
| **Pétreos** | A pele virou pedra e cristal | 8 e 60 | 20 e 150 | Duros e longevos; lentos; demoram para ser adultos e quase não têm filhos |
| **Velados** | A mana ficou no sangue: pele pálida, olhos que brilham | 6 e 41 | 15 e 102 | Mais chance de nascer mago; frágeis no combate |

**Conflitos:** nenhum. Cassius Varro continua humano: a vida longa dele vem da Torre, não da raça.

---

### RA1-2. O que a raça muda no jogo?

**Contexto:** a cultura já muda as habilidades com pontos (Guerreira +5 no combate, por exemplo).

**Pergunta:** além da idade, o que mais a raça muda?

| Opção | O que ganha | O que perde |
|---|---|---|
| A) Só a aparência e a idade | Simples | A raça quase não importa na hora de jogar |
| **B) A idade, pontos de habilidade (como a cultura) e uma vantagem com um defeito** | A raça pesa na escolha de quem contratar e recrutar, sem regra nova pesada | Mais números para equilibrar |
| C) B, mais necessidades próprias (comida, casa, clima) | Raças bem diferentes | Muito trabalho em várias fichas |

**Sugestão (B):**
- **Ferinos:** Corpo a Corpo +10, Ofício −10. Vantagem: não sentem o frio do inverno. Defeito: comem 50% mais.
- **Pétreos:** Braçal +10, Velocidade −20%. Vantagem: −30% de dano recebido. Defeito: 1/4 da chance de ter filho.
- **Velados:** Especialista +5, Corpo a Corpo −10. Vantagem: 10 vezes mais chance de ser mago (com a Magia, D4). Defeito: a Religiosa os venera e a Tecnocrata os caça (RA1-4).

**Exemplo:** numa cidade fria, a Ferraria que contrata um Ferino ganha no inverno, mas o mercado vende mais comida.

**Conflitos:** os pontos da cultura e os da raça somam, sempre entre 0 e 100.

---

### RA1-3. Quantos de cada raça em cada povo?

**Contexto:** cada NPC já tem uma cultura. A raça é um campo separado, porque um Ferino pode ter nascido na Teocracia.

**Pergunta:** como as raças se distribuem pelos povos no começo do jogo?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) Uma tabela de % por povo** | Cada povo com o seu jeito, ligado à história | Mais um número no `culturas.json` |
| B) A raça presa ao povo (todo Guerreiro é Ferino) | Simples | Contraria a história: os Clãs são só "boa parte" das raças |

**Sugestão (A):** os NPCs livres. Os escravos de cada povo vêm à parte, com a Escravidão (D3).

| Povo | Humanos | Ferinos | Pétreos | Velados |
|---|---|---|---|---|
| Clãs de Tian-Lang (Guerreira) | 40% | 35% | 20% | 5% |
| Teocracia de Theodoria (Religiosa) | 70% | 10% | 5% | 15% |
| Ordem de Custodum (Arcana) | 90% | 0 | 0 | 10% (magos) |
| Sultanato de Zahran (Escravista) | 90% | 5% | 5% | 0 |
| Coroa de Valdren (Autoritária) | 90% | 5% | 5% | 0 |
| Comunidade de Ashmoor (Tecnocrata) | 100% | 0 | 0 | 0 |

**Conflitos:** nenhum.

---

### RA1-4. Como cada povo trata as outras raças?

**Contexto:** a história já dá o jeito de cada povo, menos o da Coroa ("a definir"). No jogo, isso precisa virar regra.

**Pergunta:** isso vira uma lei de cada facção ou fica fixo na cultura?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) Uma lei nova, "As outras raças"**, com 4 opções: iguais, segunda classe, escravizáveis e caçadas | A assembleia pode mudar a lei, as minorias reagem e o jogador vê isso no mapa | Mais uma lei para equilibrar |
| B) Fixo na cultura, sem lei | Mais simples | O mundo não muda com o tempo |

**Sugestão (A), com o começo de cada povo:**
- **Iguais:** Clãs e Teocracia (a Teocracia ainda dá +10 de Felicidade aos Velados, os "abençoados").
- **Segunda classe:** Coroa (pagam o dobro de imposto: são súditos do "imperador eterno", mas abaixo dos humanos) e Sultanato.
- **Escravizáveis:** Ordem de Custodum (cobaias).
- **Caçadas:** Comunidade de Ashmoor (o NPC de outra raça é preso ou foge).

O NPC de outra raça sob uma lei ruim tem a Felicidade mais baixa, como a minoria hostil da Cultura.

**Conflitos:** a Coroa estava "a definir" na história; esta sugestão a define. A lei entra com a ficha D8, e a escravidão de verdade, com a D3.

---

### RA1-5. Os filhos de pais de raças diferentes

**Pergunta:** o filho de um Humano com uma Ferina é de que raça?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) Da raça de um dos pais, sorteada (50% cada)** | Simples; não cria raça nova nem arte nova | Nenhum mestiço visível |
| B) Não podem ter filhos | Simples | Estranho para raças que vieram dos humanos |
| C) Uma raça mestiça | Mais variedade | Mais manequins e números |

**Sugestão (A).** A idade de adulto e de velho é a da raça do filho.

---

### RA1-6. O jogador pode ser de outra raça?

**Pergunta:** na criação do personagem, o jogador escolhe a raça?

| Opção | O que ganha | O que perde |
|---|---|---|
| **A) Escolhe** | Outro jeito de jogar: um Ferino na Comunidade de Ashmoor é caçado | A criação do personagem ganha um passo |
| B) Sempre humano | Simples | Perde a variedade |

**Sugestão (A).** A raça do jogador usa as mesmas leis da RA1-4. Se ele for caçado numa cidade, a revista dos guardas (Crime) o prende.

**Conflitos:** a criação do personagem ainda não existe no jogo (o estilo é escolhido no começo); a raça entra junto do estilo.

---

### RA1-7. E os monstros?

**Contexto:** a Fusão também mudou os animais (HI2-2).

**Pergunta:** os monstros entram nesta ficha?

| Opção | O que ganha | O que perde |
|---|---|---|
| A) Sim, nesta ficha | Tudo da mutação num lugar só | A ficha fica grande |
| **B) Numa ficha própria (Fauna e Monstros), com o mapa novo** | Os monstros dependem dos biomas e da cratera do mapa novo | Mais uma ficha no plano |

**Sugestão (B).**
