---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase B4 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com RE1 vêm da **rodada RE1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Reputação

## 1. O que faz

Guarda **o que cada facção e cada Vila Livre pensa do jogador**, de −100 a +100, e muda os **preços das lojas** pela faixa. Sobe e desce com as ações do jogador e volta devagar para a base.

Documento do sistema: [[Sistema de Reputação/Descrição|Reputação e Relações do jogador]].

**Esta versão:** o valor por facção e Vila Livre, a base, as faixas, os preços nas lojas, a volta com o tempo, o "Por quê?" e as duas ações que já existem: **vencer bandidos** e **doar dinheiro**. Os crimes, as missões, os títulos, os guardas, a guerra e a **Opinião** dos NPCs Importantes chegam com os seus sistemas (parte 9). A tabela de ações fica pronta nos dados.

## 2. Dados

### Definições: `data/reputacao.json` (arquivo novo)

```json
{
  "faixas": [
    { "nome": "inimigo",  "ate": -60, "compra_em_porcento": null, "venda_em_porcento": null },
    { "nome": "hostil",   "ate": -20, "compra_em_porcento": 115,  "venda_em_porcento": 85 },
    { "nome": "neutro",   "ate": 19,  "compra_em_porcento": 100,  "venda_em_porcento": 100 },
    { "nome": "amigavel", "ate": 59,  "compra_em_porcento": 95,   "venda_em_porcento": 105 },
    { "nome": "aliado",   "ate": 100, "compra_em_porcento": 90,   "venda_em_porcento": 110 }
  ],
  "volta_por_estacao": 2,
  "acoes": {
    "venceu_bando_no_territorio": 1,
    "doou_a_cada_100_moedas": 1,
    "doacao_ate_acima_da_base": 20,
    "contrato_da_taverna": 2,
    "missao_de_nobre": 5,
    "missao_longa": 10,
    "ajudou_caravana_em_apuros": 2,
    "lutou_ao_lado_na_batalha": 5,
    "crime_detectado_por_gravidade": -2,
    "atacou_caravana_ou_patrulha": -10,
    "lutou_contra_na_batalha": -15
  },
  "base_por_afinidade": { "mesma": 20, "afim": 10, "neutra": 0, "hostil": -15 },
  "mudancas_no_por_que": 5
}
```

`null` = a loja não negocia (Inimigo). As faixas vão "até" o valor escrito, começando logo depois da anterior (Inimigo: −100 a −60).

### Estado (vai no save)

| Campo | Por quê | Exemplo |
|---|---|---|
| `reputacaoPorFaccao` | 6 facções + 3 Vilas Livres = **9 números**, inteiros de −100 a +100 | Escravista: +18 |
| `basePorFaccao` | 9 números, fixados no começo da partida | Escravista: 0 |
| `ultimasMudancas` | por facção, as 5 últimas: valor, motivo, dia | "+1: venceu um bando em (6,4), dia 12" |

As cidades usam o valor da facção dona. A **vila do jogador** não tem reputação: ela é sempre Neutro (RE1-5).

## 3. Quando roda

| Quando | O quê |
|---|---|
| Quando acontece uma ação da tabela | Muda o valor (4.2) |
| **Dia 1 de cada estação, passo 4** (estabilizador) | A volta para a base (4.3) |
| Quando o jogador compra ou vende numa loja | O preço pela faixa (4.4) |

## 4. Regras passo a passo

### 4.1 A base (começo da partida)

Base de cada facção = a afinidade da cultura do personagem com ela + o bônus do estilo de início ([[Ficha Logica/Inicio|Início de Jogo]], 4.2: mesma cultura +20, afim +10, neutra 0, hostil −15; Membro +30, Nobre exilado +20 e −20, Escravo −20). Sem estilo (o teste da fatia), a base é 0 (RE1-6). As Vilas Livres começam em 0. O valor começa igual à base.

### 4.2 Mudar o valor

1. Soma o valor da ação; prende entre −100 e +100.
2. Guarda a mudança em `ultimasMudancas` (as 5 últimas).
3. Se a faixa mudou, notificação: "Sua reputação com a Escravista agora é **Amigável**".

**As ações desta versão:**

- **Venceu um bando** (luta rápida vencida, Viagem 4.6) num mapa de território de uma facção: **+1** com ela. Em mapa em disputa ou sem dono: nada.
- **Doou** numa cidade da facção: +1 a cada 100 moedas inteiras, mas **só até a base + 20** (RE1-2). O dinheiro vai para o **Banco daquela cidade** (até a Facção ter Banco). A sobra que não daria ponto não é aceita: o jogo mostra "Com 300 moedas você chega ao máximo por doação".

### 4.3 A volta com o tempo

No dia 1 de cada estação, cada valor anda **2 pontos** em direção à base. Se está a menos de 2 da base, fica na base.

### 4.4 Os preços nas lojas

Só para o **jogador** (as caravanas das cidades não têm reputação):

1. **Inimigo:** a loja não compra nem vende ("A loja se recusa a negociar com você").
2. **Compra do jogador:** o preço da loja (110% do local, Comércio 4.7) × `compra_em_porcento` ÷ 100.
3. **Venda do jogador:** o que a loja paga (90% do local até o alvo; 45% do base além do alvo) × `venda_em_porcento` ÷ 100.
4. Cada preço por unidade é arredondado em décimos, a metade para cima (como na Economia).

## 5. Contas

| Faixa | Compra (× preço local) | Venda até o alvo (× preço local) | Diferença entre comprar e vender |
|---|---|---|---|
| Hostil | 110 × 115 = **126,5%** | 90 × 85 = **76,5%** | 50 pontos |
| Neutro | 110% | 90% | 20 pontos |
| Amigável | 104,5% | 94,5% | 10 pontos |
| Aliado | **99%** | **99%** | **0** |

No Aliado, comprar e vender na mesma loja no mesmo dia empata: não dá lucro, mas também não custa. Ver 10.4.

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Comércio | A regra 4.7 do Comércio ganha o ajuste da faixa (4.4) e a reserva de 2 dias (RE1-4) |
| Viagem | Vencer bandidos no território dá +1 |
| Outras cidades | A facção dona de cada cidade (`cidades_iniciais.json`); o Banco que recebe a doação |
| Notícias | A mudança de faixa pode virar notícia Local na própria facção [Refinar depois] |
| Interface | O painel de Reputação, o "Por quê?" e a notificação de faixa |
| Save | Os 9 valores, as bases e as últimas mudanças; o formato passa para a **versão 8** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `reputacaoPorFaccao`, `basePorFaccao` | Parte 2 |
| `mudarReputacao(faccao, valor, motivo)` | Regra 4.2 |
| `faixaDaReputacao(valor)` | Inimigo a Aliado |
| `voltarReputacaoParaABase()` | Regra 4.3 |
| `precoDeCompraDoJogador(cidade, item)` / `precoDeVendaDoJogador(cidade, item)` | Regra 4.4 |
| `doarParaAFaccao(cidade, moedas)` | Regra 4.2 |

## 8. Testes

1. **Faixas:** −60 é Inimigo, −59 Hostil, 19 Neutro, 20 Amigável, 60 Aliado.
2. **Preço:** Pão local 1,7 com Hostil: o jogador compra a 1,7 × 110% × 115% = 2,15 → **2,2**; vende a 1,7 × 90% × 85% = 1,30 → **1,3**.
3. **Inimigo** não compra nem vende.
4. **Doação:** base 0, valor 15: doar 1.000 aceita só 500 e chega a +20.
5. **Volta:** +20 vira 18, 16, 14, 12 nas 4 estações; +1 vira 0 (base).
6. **Bando em mapa sem dono** não muda nada.
7. **O teste de 1 ano não muda:** a reputação não mexe em nada sem o jogador agir.

## 9. Fora desta versão

- A **Opinião** dos NPCs Importantes (Companheiros e Diálogo, fase D).
- Os crimes (Crime e Justiça, fase C), as missões e os contratos (Objetivo e Progressão), as batalhas e a cadeia de metade (Guerra), a escravidão.
- Os guardas por faixa (revista, expulsão, ajuda em brigas) e os títulos.
- A base por cultura e estilo (Início de Jogo).

## 10. Simulação de mesa

### 10.1 Cenário

O jogador comercia Tábuas e Pães com as cidades da **Escravista** (base 0, valor 0). Ele tem 2.500 moedas. Começa no dia 1 da Primavera.

### 10.2 Um ano de reputação

| Quando | O que faz | Valor | Faixa |
|---|---|---|---|
| Primavera, dia 3 | Doa 2.000 moedas na Cidade Escravista | 0 → **+20** | Amigável (notificação) |
| Primavera, dia 9 | Vence um bando em (7,6), território Escravista | +20 → **+21** | Amigável |
| Verão, dia 1 | Volta: −2 | **+19** | **Neutro** (notificação) |
| Verão, dia 2 | Doa 100 | +20 | Amigável |
| Outono, dia 1 | Volta | +18 | Neutro |
| Outono, dia 2 | Doa 200 | +20 | Amigável |
| Inverno, dia 1 | Volta | +18 | Neutro |

**O custo de manter Amigável:** 200 moedas por estação, cerca de **12,5 moedas por dia**. A faixa dá cerca de 10 pontos a menos de diferença entre comprar e vender (parte 5). Vale a pena se o jogador compra e vende mais de cerca de **125 moedas por dia** nas cidades dessa facção.

### 10.3 Vender Tábuas na faixa Amigável

D2 (Escravista): falta 20 Tábuas, preço local 3,0, preço base 1,5. O jogador vende 30.

| Parte | Conta | Neutro | Amigável |
|---|---|---|---|
| 20 até o alvo | 3,0 × 90% × faixa | 20 × 2,7 = 54,0 | 20 × 2,8 (2,835 arredondado) = 56,0 |
| 10 além do alvo | 1,5 × 45% × faixa | 10 × 0,7 (0,675) = 7,0 | 10 × 0,7 (0,709) = 7,0 |
| Total | | **61,0** | **63,0** |

### 10.4 Comprar o estoque e revender mais caro

A vila tem 75 Pães (o alvo) a 1,7. Um jogador Aliado compra todos a 99% (1,683, arredondado para 1,7; 127,5 moedas). Sem Pão, o fator vai a 200 e o preço sobe 10% por dia; a vila passa fome. Em 7 dias o preço chega a cerca de 3,0. A Padaria repôs 30 (sobra para o consumo); a falta é 45. Ele vende 45 a 3,0 (99% de 3,0 = 2,97, arredondado) e 30 a 0,8 (110% de 45% do base de 1,7, arredondado): **159 moedas**, um lucro de 31,5 (25%) em uma semana, **com a vila em fome**.

**Achado:** com a faixa Aliado, comprar o estoque inteiro de uma cidade e revender dá lucro e mata a cidade de fome. **Resolvido (RE1-4):** a loja **não vende ao jogador** o que deixaria o estoque abaixo de **2 dias do consumo previsto** (a vila: 30 Pães). O jogador compra no máximo 45, a vila nunca fica sem comida por causa dele, e o preço sobe menos. A regra vale em todas as faixas e entra na regra 4.7 do [[Ficha Logica/Comercio|Comércio]].

### 10.5 Verificações

| Pergunta | Resultado |
|---|---|
| As faixas fecham sem buraco? | **OK:** −100 a −60, −59 a −20, −19 a 19, 20 a 59, 60 a 100 |
| A doação pode ser repetida sem fim? | **Não passa de base + 20** (RE1-2). **Achado:** o documento dizia "até +20 no total"; lido como "para sempre", a doação de 2.000 se perdia na primeira estação (+20 → +18) e nunca mais podia ser feita |
| O Aliado cria dinheiro na mesma loja? | **Não:** compra a 99% e vende a 99% no mesmo dia: empata. Com o preço subindo, sim (10.4): resolvido pela reserva |
| A volta leva o Inimigo a Neutro? | De −60 (base 0), em 2 pontos por estação, **21 estações** (5 anos). É de propósito: um inimigo dura |
| O teste de 1 ano muda? | **Não** |

### 10.6 Casos extremos

| Caso | O que acontece |
|---|---|
| Reputação +100 e mais uma ação +5 | Fica +100 |
| Base −15 (cultura hostil) e o valor é −15 | A volta não muda nada |
| Doar 50 moedas | Não dá ponto: "Doe pelo menos 100 moedas" |
| A cidade muda de facção (Guerra, fase D) | Os preços passam a usar a nova facção na hora |
| Inimigo e a única loja de comida por perto | O jogador fica sem comprar ali; é o peso de ser Inimigo |

### 10.7 Decidido em modo automático (rodada RE1, seguir sugestão)

1. **RE1-1:** a reputação é um inteiro de −100 a +100, por facção e Vila Livre; a cadeia de metade da guerra arredonda para baixo, em módulo, quando a Guerra chegar.
2. **RE1-2:** a doação sobe só até a base + 20 e pode ser repetida depois da volta.
3. **RE1-3:** a doação vai para o Banco da cidade onde foi feita, até a Facção ter Banco.
4. **RE1-4:** a loja não vende ao jogador abaixo de 2 dias do consumo previsto (vale também para o Comércio).
5. **RE1-5:** a vila do jogador não tem reputação; é sempre Neutro.
6. **RE1-6:** até o Início de Jogo, a base é 0 para todas as facções.

### 10.8 Ajustes do código (marco B4, 2026-10-24)

1. **Os 9 donos** são as 6 facções e as 3 Vilas Livres, na ordem do `cidades_iniciais.json`. A vila do jogador (Vila Livre do Centro) fica na lista, mas é sempre Neutro e não aparece na janela (RE1-5).
2. **O bando vencido** conta no território de uma facção só e também no mapa de uma cidade dela.
3. **A volta** roda no tick em que a estação começa, depois das outras cidades, e entra no "Por quê?" ("a volta com o tempo"); se ela muda a faixa, também avisa.
4. **A faixa vale nas lojas das outras cidades**, quando o grupo compra ou vende lá. A loja da vila do jogador não muda (RE1-5).
5. **A doação:** os botões "Doar 100" e "Doar 500" ficam no painel "Loja desta cidade" do Mapa Mundial; o grupo precisa levar as moedas.
6. **O `reputacao.json`** ganhou o `texto` de cada faixa ("Amigável") e os `nomes_das_faccoes` para a tela. A `base_por_afinidade` fica no arquivo, sem uso até o Início de Jogo.

