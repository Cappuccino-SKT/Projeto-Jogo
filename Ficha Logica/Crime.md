---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase C3 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com CJ1 vêm da **rodada CJ1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Crime e Justiça

## 1. O que faz

Duas partes:

1. **A Criminalidade de cada cidade** (de 0 a 100): sobe com a infelicidade, o desemprego e a pobreza. Ela tira Felicidade, causa furtos no armazém e põe mais **bandidos nas estradas** da região (a ameaça bandida da Viagem).
2. **O crime do jogador:** a **revista** na entrada de uma cidade acha os itens proibidos (Contrabando) ou a falta do Livro Sagrado. Depois vêm a multa, o suborno, a fuga, a **Recompensa** por facção, a prisão e os **Caçadores de Recompensa**. A **loja ilegal** do jogador entra também.

Documento do sistema: [[Sistema de Crime e Justiça/Descrição|Crime e Justiça]].

**Esta versão:** a Criminalidade agregada (a mesma regra em LOD 0 e 2), a revista, as penas do jogador, a Recompensa e a loja ilegal do jogador. **Testemunhas, guardas que andam, combate não letal, crimes de NPCs um a um, missões de confiança e prisioneiros de guerra** ficam na parte 9.

## 2. Dados

### `data/crimes.json` (arquivo novo)

```json
{
  "crimes": [
    { "id": "crime.desacato", "gravidade": 1 },
    { "id": "crime.furto", "gravidade": 2 },
    { "id": "crime.contrabando", "gravidade": 2 },
    { "id": "crime.heresia_sem_livro", "gravidade": 2 },
    { "id": "crime.invasao", "gravidade": 3 },
    { "id": "crime.agressao", "gravidade": 3 },
    { "id": "crime.sedicao", "gravidade": 3 },
    { "id": "crime.heresia", "gravidade": 3 },
    { "id": "crime.assassinato", "gravidade": 5 },
    { "id": "crime.traicao", "gravidade": 5 }
  ],
  "multa_por_gravidade_ao_quadrado": 25,
  "recompensa_vezes_a_multa": 2,
  "recompensa_cai_por_estacao_em_porcento": 10,
  "cacadores_a_partir_de": 500,
  "revista_por_faixa_em_porcento": { "inimigo": 100, "hostil": 50, "neutro": 10, "amigavel": 5, "aliado": 0 },
  "suborno_base_em_porcento": 20,
  "suborno_por_multa_oferecida_em_porcento": 40,
  "suborno_maximo_em_porcento": 90,
  "dias_de_prisao_por_gravidade": 2,
  "fianca_vezes_a_multa": 2,
  "criminalidade": {
    "base": 10, "por_ponto_de_felicidade_abaixo_de_50": 1, "por_ponto_da_taxa_de_desemprego": 20,
    "sem_dinheiro_para_2_dias_da_cesta": 15, "passo_por_dia_em_porcento": 10,
    "felicidade_a_partir_de": 20, "felicidade_por_ponto_acima": -0.2,
    "furto_a_partir_de": 25, "furto_por_mil_por_ponto": 1, "furto_tira_em_porcento": 10,
    "ameaca_bandida_a_partir_de": 10, "ameaca_bandida_por_mil_por_ponto": 3
  }
}
```

A lei **Crimes e penas** de cada facção (Facção) escolhe quais crimes valem; nesta versão, todos valem em todas, e a **Heresia** é gravidade 5 com a Perseguição.

### Estado (vai no save)

| Campo | Exemplo |
|---|---|
| `criminalidadeEmDecimos` | por cidade: 112 (11,2) |
| `recompensaDoJogadorPorFaccao` | Religiosa: 400 |
| `dinheiroDaRecompensaPago` | — |
| `jogadorPresoAte` | dia 38, ou ninguém |
| `lojaIlegalDoJogador` | por construção do jogador: sim ou não, e o estoque ilegal |

**O sorteio:** a revista, o suborno e a batida usam o gerador da **Viagem** (sequência 5), porque são ações do jogador. O furto no armazém é um evento da cidade (a sequência de eventos dela).

## 3. Quando roda

| Quando | O quê |
|---|---|
| **A cada dia, passo 5b (novo)**, depois da Felicidade | A Criminalidade de cada cidade (4.1) |
| A cada dia, passo 6 (eventos) | O furto no armazém (4.2) |
| O jogador **entra num mapa com cidade** de facção | A revista (4.4) |
| **Dia 1 de cada estação** | A Recompensa cai (4.6); a batida na loja ilegal (4.8) |
| Ao entrar num trecho da Viagem | Os Caçadores (4.7) |

## 4. Regras passo a passo

### 4.1 A Criminalidade (passo 5b)

1. **Alvo** = 10 + (50 − Felicidade dos Civis, se positivo) + 20 × a taxa de desemprego (0 a 1) + 15 se a classe Civil não tem dinheiro para 2 dias da cesta. Preso entre 0 e 100.
2. **Passo:** a Criminalidade anda 10% ao dia até o alvo, como a Felicidade (em décimos, arredondando na direção do alvo).
3. **Na Felicidade** (no dia seguinte, no fator Eventos): −0,2 por ponto **acima de 20** (CJ1-2).

### 4.2 O furto no armazém (evento)

Só entra no sorteio dos eventos quando a Criminalidade é **25 ou mais**: chance por dia = 1 por mil por ponto. Tira **10% de um item** da Cidade (o de maior valor em estoque), arredondado para baixo. Cooldown de 8 dias. Notícia: nenhuma (fica no diário da cidade).

### 4.3 A ameaça bandida (para a Viagem)

Ameaça de uma região, em por mil = 3 × (média da Criminalidade das cidades da região − 10), se positivo. Ela soma à chance de encontro de cada mapa da região (Viagem, 4.5), que até agora usava 0.

### 4.4 A revista (o jogador entra numa cidade de facção)

1. **Chance**, pela faixa da Reputação: Inimigo 100%, Hostil 50%, Neutro 10%, Amigável 5%, Aliado 0% (CJ1-1). Sorteia.
2. **O que a revista acha:**
   - **Contrabando:** cada item proibido pela facção (Facção, 4.7) no inventário do jogador e do grupo;
   - **Heresia sem o Livro Sagrado:** na facção com a lei, se o grupo não leva o Livro, uma vez por estação.
3. Se achou, o jogo pausa e oferece (o crime de maior gravidade conta):
   - **Pagar:** a multa (25 × gravidade²) e o **confisco** dos itens proibidos. A ficha fica limpa;
   - **Subornar:** o jogador oferece um valor; chance = 20% + 40% × oferta ÷ multa, até 90%. Deu certo: paga a oferta e entra. Falhou: perde a oferta, a Recompensa sobe (4.5) e ele é **preso** (4.6);
   - **Fugir:** volta para o Mapa Mundial sem entrar. Não perde os itens, mas a Recompensa sobe (4.5).
4. A cada crime (o pago também), a **reputação** cai 2 × a gravidade (Reputação).

### 4.5 A Recompensa

1. Cada crime não pago soma **2 × a multa** na Recompensa da facção.
2. Com Recompensa nessa facção, a revista é **certa** (100%) e não há opção de pagar só a multa: pagar = a Recompensa inteira + a multa do crime novo.

### 4.6 A prisão

1. Dias = 2 × a soma das gravidades dos crimes da ficha.
2. O jogador escolhe: **esperar** (o tempo passa a 8x; ele fica no mapa da cidade sem agir) ou pagar a **fiança** (2 × a multa de cada crime).
3. Ao sair: Recompensa da facção = 0, os itens proibidos confiscados.
4. Nesta versão, sem fuga nem resgate (Combate e Companheiros).

### 4.7 Os Caçadores de Recompensa

Com a Recompensa de uma facção **≥ 500**: ao entrar num trecho que chega a um mapa dessa facção (território ou cidade), a Viagem sorteia também um encontro de **Caçadores**: chance de 100 por mil, que sempre atacam. Força = Recompensa ÷ 100 × 3. É a luta rápida da Viagem (4.6): se o jogador perde, é **preso** (4.6). Se vence, segue (a Recompensa não muda). Não dá para subornar nem render-se de outro jeito: render-se = prisão.

### 4.8 A loja ilegal do jogador

1. O jogador marca uma construção de produção dele como **loja ilegal**. Ela compra e vende os itens proibidos da facção a **× 2 do preço local** (só com o jogador, nesta versão; a procura dos NPCs vem com o conteúdo).
2. **Batida** no dia 1 de cada estação: chance = Controle ÷ 200, com **Controle = 100 − Criminalidade** até a Felicidade e Estabilidade ter o Controle (CJ1-4).
3. **Se descoberta:** confisco do estoque ilegal, Contrabando (gravidade 3) para o gerente; para o jogador, se ele estiver no mapa (pela revista, 4.4).

### 4.9 A recompensa cai

No dia 1 de cada estação, cada Recompensa cai 10%, arredondada para baixo; abaixo de 10, zera.

## 5. Contas

| Gravidade | Multa | Recompensa por crime não pago | Dias de prisão | Fiança |
|---|---|---|---|---|
| 1 | 25 | 50 | 2 | 50 |
| 2 | 100 | 200 | 4 | 200 |
| 3 | 225 | 450 | 6 | 450 |
| 5 | 625 | 1.250 | 10 | 1.250 |

**Criminalidade na vila do teste de 1 ano:** Felicidade cerca de 58, desemprego 1 em 15 (0,07), Civis com dinheiro: alvo = 10 + 0 + 1,3 + 0 = **11,3**. Abaixo de 20 (sem efeito na Felicidade) e de 25 (sem furto). **O teste não muda** (CJ1-2, CJ1-3).

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Felicidade | A Felicidade e o desemprego entram no alvo; a Criminalidade acima de 20 tira Felicidade |
| Eventos | O furto no armazém (só com Criminalidade ≥ 25) |
| Viagem | A ameaça bandida (4.3); o encontro de Caçadores (4.7); a luta rápida |
| Facção | Os itens proibidos; a lei Crimes e penas; a Heresia com a Perseguição |
| Reputação | A faixa dá a chance de revista; cada crime tira 2 × a gravidade |
| Comércio | A loja ilegal; as caravanas das cidades não levam itens proibidos para onde são proibidos |
| Religião (C4) | A Heresia; o Livro Sagrado |
| Notícias | Recompensa ≥ 500: notícia Local "Procura-se" na facção [Refinar depois] |
| Save | A Criminalidade, as Recompensas, a prisão, a loja ilegal; o formato passa para a **versão 12** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `criminalidadeEmDecimos` | Por cidade |
| `calcularAlvoDaCriminalidade(cidade)` | Regra 4.1 |
| `ameacaBandidaDaRegiao(regiao)` | Regra 4.3 |
| `revistarJogador(cidade)` | Regra 4.4 |
| `tentarSubornar(oferta, multa)` | Regra 4.4 |
| `recompensaDoJogadorPorFaccao` | Regra 4.5 |
| `prenderJogador(faccao)` | Regra 4.6 |
| `fazerBatidaNaLojaIlegal(construcao)` | Regra 4.8 |

## 8. Testes

1. **A vila do teste:** Criminalidade perto de 11; nenhum furto e nenhum efeito na Felicidade em 64 dias, nas 10 sementes.
2. **O vilarejo em fome** da parte 10.3 chega a 56 e tira 7,2 de Felicidade.
3. **Ameaça bandida:** região com média 30: +60 por mil na chance de encontro.
4. **Revista:** Neutro, 1.000 entradas: cerca de 100 revistas.
5. **Suborno:** oferta = metade da multa: 40%; oferta = 2 multas: 90%.
6. **Recompensa:** 2 fugas com Contrabando (gravidade 2): 400; mais uma: 600 → Caçadores.
7. **Prisão:** 3 crimes de gravidade 2: 12 dias, fiança 600.
8. **A queda:** 600 vira 540, 486, 437... por estação.

## 9. Fora desta versão

- Testemunhas, Percepção e Destreza, o Nível de Segurança, a noite e a névoa na detecção (quando o jogador puder roubar e lutar no mapa).
- Os guardas que andam, a ordem de rendição, o combate não letal, a força letal; a quantidade de guardas por cidade e o Controle de verdade.
- Os crimes dos NPCs um a um; as penas por classe; trabalho forçado, banimento, execução.
- A fuga da prisão e o resgate; os Caçadores andando no mapa.
- As lojas ilegais dos NPCs, a Confiança e as missões de confiança (Objetivo e Progressão).
- Prisioneiros de guerra (Guerra, Escravidão).
- Os bandos que nascem da Criminalidade como entidades (com a Facção e as patrulhas; VI1-1).

## 10. Simulação de mesa

### 10.1 Cenário

O jogador é Mercador e leva **20 Fumos** (proibidos fora da Escravista) para vender na loja ilegal dele, numa vila em território Religioso. Reputação com a Religiosa: **Neutro (0)**; 300 moedas. A rota passa por 3 cidades Religiosas.

### 10.2 A viagem do contrabandista

| Momento | Sorteio | O que acontece | Recompensa | Reputação |
|---|---|---|---|---|
| Entra na cidade 1 | revista 10%: 612 → não | Entra e compra comida | 0 | 0 |
| Entra na cidade 2 | 41 → **revista** | Acha 20 Fumos (Contrabando, gravidade 2) e nenhum Livro Sagrado (lei da Religiosa; heresia sem livro, gravidade 2). Multa: 100. O jogador **foge** | **200** | −4 → **−4** |
| Entra na cidade 3 | Com Recompensa, revista certa | Acha de novo. **Pagar** custaria 200 + 100 = 300 (tudo o que ele tem) e os 20 Fumos. Ele foge de novo | **400** | **−8** |
| Viaja pelo território | — | Abaixo de 500: sem Caçadores | 400 | −8 |
| Dia 1 da estação seguinte | — | A Recompensa cai 10% | **360** | volta 2: −6 |

Se ele fugir de mais uma revista: 360 + 200 = **560** → Caçadores (100 por mil por trecho no território Religioso). Uma luta rápida com força 3 contra 560 ÷ 100 × 3 = 16 tem chance de **16%**: quase certo que ele vai preso, por 2 × (2 + 2 + 2) = **12 dias**, ou paga 600 de fiança.

**Para não ser pego:** subir a reputação com a Religiosa para Amigável (revista de 5%), levar um Livro Sagrado, ou evitar entrar nas cidades e vender só na própria vila.

### 10.3 Um vilarejo em fome

Vilarejo de 40 Civis, Felicidade **25**, desemprego **30%**, Civis sem dinheiro para 2 dias da cesta:

| Dia | Alvo | Criminalidade | Efeito na Felicidade | Furto (por mil) |
|---|---|---|---|---|
| 0 | 10 + 25 + 6 + 15 = **56** | 11,0 | 0 | — |
| 1 | 56 | 15,5 | 0 | — |
| 5 | 56 | cerca de 29 | −1,8 | 29 |
| 10 | 56 | cerca de 40 | −4,0 | 40 |
| 20 | 56 | cerca de 51 | −6,2 | 51 |

A Felicidade cai mais (−6,2), o alvo do crime sobe mais 6,2, e a conta se estabiliza: cada ponto de Felicidade perdido sobe 1 ponto de Criminalidade, que tira só 0,2 de Felicidade. **Não vira uma espiral sem fim** (o ganho da volta é 0,2, menor que 1). A ameaça bandida da região sobe: com essa cidade a 56 e as outras 3 a 11, a média é 22,25 → **+36,75 por mil** nas estradas.

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| O teste de 1 ano muda? | **Não:** a vila fica em cerca de 11, abaixo dos dois limites (CJ1-2, CJ1-3) |
| Crime e Felicidade viram espiral? | **Não:** a volta tem ganho 0,2 (10.3) |
| O contrabando tem risco no Neutro? | **Sim, com a CJ1-1.** **Achado:** pelo documento, só a faixa Hostil revista; no Neutro, o contrabando seria sem risco nenhum. Agora: 10% por entrada no Neutro |
| A ameaça bandida pesa demais? | Numa região tranquila (todas em cerca de 11): +3 por mil, quase nada. Numa região em crise: +37 a +60 |
| O jogador fica preso para sempre? | **Não:** no máximo 2 × a soma das gravidades; e a fiança |
| As caravanas da cidade levam itens proibidos? | **Não:** a loja legal não vende o item proibido (Facção, 4.7), e a caravana só leva da loja legal |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Sem dinheiro para a multa | "Pagar" não aparece; sobram o suborno (se tiver algo para oferecer) e a fuga |
| A revista acha um item que a facção proibiu hoje | Conta: a lei vale no dia seguinte à assembleia, e a revista é depois |
| Preso com uma caravana dele no caminho | A caravana segue; ela não depende do jogador |
| A vila do jogador sem facção | Sem leis, sem revista, sem crime (só a Criminalidade, que é de todas as cidades) |
| A Criminalidade em 100 | Felicidade −16; furto de 10% por dia de chance; a região com mais bandidos |
| Uma cidade sem Civis | Não há alvo; a Criminalidade fica como está |

### 10.6 O que muda em outras fichas

- **Viagem:** a ameaça bandida deixa de ser 0 e vem da regra 4.3; um tipo de encontro novo, os Caçadores (4.7).
- **Eventos:** um evento novo, o furto no armazém, só com Criminalidade ≥ 25.
- **Reputação:** a faixa também dá a chance de revista (CJ1-1).

### 10.7 Decidido em modo automático (rodada CJ1, seguir sugestão)

1. **CJ1-1:** revista em todas as faixas (Neutro 10%, Amigável 5%, Aliado 0%, Hostil 50%, Inimigo 100%).
2. **CJ1-2:** a Criminalidade só tira Felicidade acima de 20 (0,2 por ponto), para a vila tranquila não mudar.
3. **CJ1-3:** o furto no armazém só entra no sorteio com Criminalidade ≥ 25.
4. **CJ1-4:** Controle = 100 − Criminalidade até a Felicidade e Estabilidade ter o Controle de verdade.
5. **CJ1-5:** a revista oferece Pagar, Subornar e Fugir; a fuga sempre dá certo nesta versão (sem combate) e aumenta a Recompensa.
6. **CJ1-6:** com Recompensa na facção, a revista é certa; os Caçadores aparecem a partir de 500.
7. **CJ1-7:** prisão de 2 dias por ponto de gravidade, ou fiança de 2 multas.

### 10.8 Ajustes do código (marco C3, 2026-10-24)

1. **A loja ilegal do jogador (4.8)** fica para quando houver itens proibidos no conteúdo: hoje nenhum item do jogo é proibido (os "narcóticos", cajados e outros ainda não existem).
2. **O Livro Sagrado ainda não é um item:** numa cidade da Religiosa (a facção com a lei do Livro), a revista sempre acha a Heresia sem o Livro, uma vez por estação. O item vem com a Religião (C4).
3. **A revista acontece em cada cidade de facção em que o grupo entra**, também no meio da rota. Ao fugir, o grupo volta ao mapa de onde veio, e a viagem para ali.
4. **Preso,** o grupo para; se estava na estrada (Caçadores), é levado à capital da facção. Ele não parte, não comercia e não doa. O tempo segue na velocidade escolhida (sem o 8x automático).
5. **Cada revista conta só o crime de maior gravidade**: ele dá a multa, a Recompensa (2 × a multa), a gravidade da ficha (os dias de prisão) e a fiança, como na parte 10.2.
6. **A multa, o suborno e a fiança** vão para o Banco da cidade onde aconteceram.
7. **A Criminalidade** começa em 11,0 em todas as cidades e anda na ordem do dia, logo depois da Felicidade. O furto usa o sorteio dos eventos da cidade, só com 25 ou mais.
8. **A ameaça bandida** é a média das cidades da região (as Vilas Livres contam) e entra também no risco da rota das caravanas.
9. **A notícia "Procura-se"** fica para depois.

