---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase B2 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com CO1 vêm da **rodada CO1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Comércio

## 1. O que faz

Liga os Mercados Locais das cidades. As **caravanas das cidades** levam o que sobra numa cidade para outra onde falta e trazem de volta o que falta em casa. O **jogador** compra numa cidade e vende em outra, sozinho ou com a sua caravana. O comércio aproxima os preços e move dinheiro entre cidades, sem criar dinheiro.

Documento do sistema: [[Sistema de Comércio/Descrição|Comércio Civil e Caravanas]]. Usa a [[Ficha Logica/Viagem|Viagem]] (o movimento, os encontros) e as [[Ficha Logica/OutrasCidades|Outras cidades]] (os Mercados que existem longe do jogador).

**Esta versão:** as caravanas das cidades, a venda e a compra nas lojas com limite, a caravana do jogador viagem a viagem e os preços que o jogador conhece. **Tarifa, guerra, embargo, contrabando e rota fixa** ficam na parte 9.

## 2. Dados

### Definições (`data/regras.json`, bloco novo `"comercio"`)

```json
"comercio": {
  "caravanas_por_posto_comercial": 2,
  "caravana_da_cidade": { "condutores": 2, "guardas": 2, "cavalos": 4 },
  "capacidade_por_cavalo": 100,
  "salario_por_dia_da_caravana_em_decimos": 60,
  "loja_compra_em_porcento": 90,
  "loja_vende_em_porcento": 110,
  "acima_do_alvo_a_loja_paga_em_porcento_do_base": 45,
  "lucro_minimo_em_porcento_do_custo": 20,
  "risco_chance_de_perder_em_porcento": 60,
  "risco_perda_media_em_porcento": 60
}
```

| Campo | O que é |
|---|---|
| `caravana_da_cidade` | 4 pessoas (abstratas: não saem dos NPCs da cidade) e 4 Cavalos (abstratos nesta versão). Força 2 × 1 + 2 × 3 = **8** |
| `capacidade_por_cavalo` | 100 unidades de peso (Itens: Pão 1, Grão 1, Água 1, Tábua 2, Tora 3, Móveis 8) |
| `salario_por_dia_da_caravana_em_decimos` | 6 moedas por pessoa por dia: o turno de 8 horas do Braçal (75 centésimos por hora) |
| `acima_do_alvo_a_loja_paga_em_porcento_do_base` | O que a loja paga por unidade **além do estoque-alvo**: 90% × o preço mínimo do Mercado (50% do base) = **45% do preço base** (CO1-3) |
| `lucro_minimo_em_porcento_do_custo` | A caravana só sai se o lucro esperado passar de 20% do custo da viagem |
| `risco_*` | O risco da rota (4.3): a caravana atacada perde em 60% das vezes e perde 60% da carga em média. **Medir** com o teste da parte 8 |

### Estado (vai no save)

**Cada caravana** é uma `EntidadeEmTransito` da Viagem (tipo `"caravana"`), com a mais:

| Campo | Exemplo |
|---|---|
| `cidadeDona` (ou o jogador) | mapa 8 (Capital Arcana) |
| `situacao` | `"em_casa"`, `"indo"`, `"voltando"` |
| `destino` | mapa 22 |
| `cargaPorItem` | Tábua: 20 |
| `dinheiroLevado` | 0 (em décimos) |

**Por cidade:** `aCaminhoPorItem` (quanto as caravanas já levam para essa cidade, para outra caravana não mandar o mesmo).

**Do jogador:** `precosConhecidos`, uma tabela por cidade com o preço local de cada item e o **dia em que foi visto** (junto da névoa da Viagem).

## 3. Quando roda

| Quando | O quê |
|---|---|
| **A cada dia, passo 3b (novo)**, depois do Mercado Local (3) e antes do estabilizador (4) | Cada caravana **em casa** escolhe se sai e para onde (4.2 a 4.4) |
| Ao **chegar ao destino** (tick da chegada) | Vende, compra a volta e parte de volta (4.5) |
| Ao **chegar em casa** | Descarrega e devolve o dinheiro (4.6) |
| Quando o jogador **vende ou compra** numa loja | 4.7 |
| Ao **entrar num mapa com cidade** | Atualiza os `precosConhecidos` (4.8) |

As cidades fazem o passo 3b pelo número do mapa; as caravanas de uma cidade, pela ordem de criação.

## 4. Regras passo a passo

### 4.1 Quem tem caravana

Só a cidade com **Posto Comercial** manda caravanas: **2 por Posto**. No começo, os Postos estão nas 6 capitais (pacote do nível 4) e na Cidade Escravista (7,5): **14 caravanas** no mundo. As outras cidades só recebem. A caravana do jogador não precisa de Posto (CO1-5).

### 4.2 Sobra e falta

Para cada item numa cidade:

- **Sobra** = estoque da cidade − estoque-alvo (se positiva). É o que a cidade pode mandar sem ficar abaixo do alvo.
- **Falta** = estoque-alvo − estoque − `aCaminhoPorItem` (se positiva). É o que a cidade quer receber.

O estoque-alvo é o do Mercado Local (5 dias do consumo previsto). Só os itens **da Cidade** contam; os do jogador não entram na caravana da cidade.

### 4.3 A conta de uma viagem (passo 3b)

Para a caravana em casa (cidade C) e cada destino D (todas as outras cidades, menos as que não têm rota):

1. **A ida:** para cada item com sobra em C e falta em D, quantidade = a menor entre a sobra, a falta e o espaço que sobra (400 ÷ peso). Ganho por unidade = 90% do preço local em D − o preço local em C. Enche pela ordem do **maior ganho por unidade de peso**, só os itens com ganho positivo.
2. **A volta:** igual, com sobra em D e falta em C. Ganho por unidade = o preço local em C − 110% do preço local em D. A volta é paga com o dinheiro da venda da ida (se faltar, leva menos).
3. **Os dias:** os ticks da rota ida e volta (Viagem, 4.2, com o clima de hoje), mais as noites acampadas, arredondado para cima em dias.
4. **O custo** = dias × 4 pessoas × (6 moedas + o preço local do Pão em C).
5. **O risco**, em por mil = soma, nos mapas da ida e da volta, da chance de encontro × 75% (bandidos) × 60% (perde) × 60% (perda média), até 1.000.
6. **O lucro esperado** = (ganho da ida + ganho da volta) × (1.000 − risco) ÷ 1.000 − custo.

### 4.4 Sair ou ficar

1. Escolhe o destino de **maior lucro esperado**; empate: o mapa de menor número.
2. Se o lucro esperado for **menor que 20% do custo**, fica em casa até amanhã.
3. Se sai: tira a carga do estoque de C (itens da Cidade), soma a carga ida ao `aCaminhoPorItem` de D, tira a comida da viagem (4 Pães por dia) do estoque de C, e parte (Viagem, 4.1; acampa se for noite).
4. O **salário** da caravana é pago **pelo Banco de C à classe Civil de C** quando ela chega em casa: os dias fora, arredondados para cima, × 4 pessoas × 6 moedas (as pessoas são da cidade; o dinheiro não sai do mundo). Se o Banco não tem a reserva de 250 mais esse salário, a caravana não sai.

### 4.5 Chegar ao destino

1. **Vende** a carga à loja de D (4.7), tira do `aCaminhoPorItem` de D o que tinha marcado.
2. **Compra** a volta: os itens de 4.3 passo 2, recalculados com o estoque e o preço **de agora**, sem passar da sobra de D e do dinheiro da venda.
3. Parte de volta no mesmo tick (ou acampa, se for noite).
4. Se foi **atacada** no caminho (Viagem, 4.11), chega com o que sobrou da carga.

### 4.6 Chegar em casa

Põe a carga no estoque de C, devolve o `dinheiroLevado` ao Banco de C, paga o salário (4.4), e fica **em casa** até o próximo passo 3b.

### 4.7 Vender e comprar numa loja (para todos: caravana e jogador)

1. **A loja vende** a 110% do preço local (Economia).
2. **A loja compra** a 90% do preço local **até completar o estoque-alvo**. Cada unidade **além do alvo** é paga a **45% do preço base** (CO1-3). O preço do dia não muda com a venda; ele anda no passo 3 do dia seguinte, como sempre.
3. A loja paga do **Banco da cidade** e só enquanto o Banco tem acima da reserva de 250; depois disso, não compra mais nesse dia.
4. **A reserva da loja (RE1-4, da [[Ficha Logica/Reputacao|Reputação]]):** a loja não vende ao jogador o que deixaria o estoque abaixo de **2 dias do consumo previsto**. Sem isso, o jogador compra o estoque inteiro, a cidade passa fome e ele revende mais caro.
5. Para o jogador, o preço da compra e da venda ainda passa pela faixa da reputação (Reputação, 4.4). Cada preço por unidade é arredondado em décimos, a metade para cima.

*Exemplo:* a loja tem 0 Tábuas, alvo 20, preço local 3,0. O jogador vende 40: 20 × 2,7 = 54 e 20 × 0,7 (45% de 1,5 = 0,675, arredondado) = 14. Total **68 moedas**, não 108.

### 4.8 O que o jogador sabe

1. Ao entrar num mapa com cidade, os `precosConhecidos` dessa cidade viram os de hoje, com a data.
2. A tela do Mapa Mundial mostra o preço e "visto há N dias".
3. As caravanas do jogador trazem a tabela da cidade de destino ao voltar. Notícias de preço: Notícias (B3).

### 4.9 A caravana do jogador

1. **Montar:** o jogador precisa de Cavalos (100 de capacidade cada; item do conteúdo novo) e contrata guardas entre os desempregados da cidade, pelo salário de 6 moedas por dia (Trabalho; eles viram empregados do jogador).
2. **Ordem de viagem:** destino, o que levar e o que comprar lá (quantidade e preço máximo). Viagem a viagem; a rota fixa vem com a Tecnologia (B5).
3. Anda e é atacada como qualquer caravana (força: guardas × 3 + 1 do condutor). O jogador vê a chegada e o resultado (Interface).
4. Paga os guardas do dinheiro do jogador, a cada dia, como as oficinas dele.

## 5. Contas

| Conta | Valor |
|---|---|
| Caravana, 3 trechos de estrada na planície | 3.200 × 75 → 2.400 × 133 → **3.192 ticks** por trecho (3 h 59) |
| Custo de 2 dias | 2 × 4 × (6 + 1,7) = **61,6 moedas** |
| Risco, 6 mapas de território (50‰) | 6 × 50 × 0,75 × 0,6 × 0,6 = **81‰** (8,1%) |
| Ganho por Tábua (C a 0,8; D a 3,0) | 0,9 × 3,0 − 0,8 = **1,9** |
| Tábuas para pagar a viagem | 61,6 × 1,2 ÷ (1,9 × 0,919) = **cerca de 43** |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Viagem | A caravana é uma entidade em trânsito; tempo, acampamento, Tempestade parada, encontros e 4.11 |
| Outras cidades | Os estoques, os preços e os Bancos de todas as cidades, todo dia |
| Economia | O preço local, a reserva do Banco, a loja a 110% e 90%; o passo 3b novo na ordem do dia |
| Trabalho | Os guardas do jogador são empregados dele |
| Itens | O peso; o Cavalo (conteúdo novo) |
| Construção | O Posto Comercial (conteúdo novo, nível 4) |
| Notícias (B3) | O ataque à caravana e os preços viram notícia |
| Save | As caravanas, o `aCaminhoPorItem`, os `precosConhecidos`; o formato passa para a **versão 6** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `sobraDoItem(cidade, item)`, `faltaDoItem(cidade, item)` | Regra 4.2 |
| `calcularLucroEsperadoDaViagem(caravana, destino)` | Regra 4.3 |
| `decidirViagemDaCaravana(caravana)` | Regra 4.4, no passo 3b |
| `venderNaLoja(cidade, item, quantidade, vendedor)` | Regra 4.7 |
| `comprarNaLoja(cidade, item, quantidade, comprador)` | Regra 4.7 |
| `aCaminhoPorItem` | Por cidade |
| `precosConhecidos` | Do jogador, com a data |

## 8. Testes

1. **A venda de 40 Tábuas** do exemplo 4.7 paga 68 moedas.
2. **A loja para** de comprar quando o Banco chega a 250.
3. **Duas caravanas no mesmo dia** para a mesma cidade: a segunda vê a falta já descontada pelo `aCaminhoPorItem` e escolhe outro destino.
4. **O dinheiro do mundo:** a soma de todos os Bancos, classes e do jogador só muda pela verba da Coroa, pelo Mercador viajante dos Eventos e pelas perdas para bandidos. Rodar 1 ano com as 22 cidades.
5. **A caravana não tira abaixo do alvo** da cidade dona nem compra abaixo do alvo do destino.
6. **Os preços se aproximam:** com uma cidade sem floresta, o preço da Tábua nela fica, na média do ano, a no máximo 50% acima da média das cidades com floresta. (Sem o comércio, ficaria no teto, 200%.)
7. **O risco medido:** em 1 ano, a perda média das caravanas para bandidos fica perto dos 8% da conta da parte 5; se não, ajustar os `risco_*`.

## 9. Fora desta versão

- A Tarifa, o Banco da Facção, a guerra, a cidade sitiada, o embargo e o acordo comercial (Facção e Diplomacia).
- O contrabando de narcóticos (Crime e Escravidão).
- A rota fixa (Tecnologia, B5).
- Os Cavalos de verdade na caravana da cidade (comprados e gastos); a quantidade de caravanas pelo nível do Posto.
- O contrato de escolta (Objetivo e Progressão).
- A informação atrasada para as caravanas das cidades: nesta versão, elas veem os preços de hoje de todas as cidades **[Refinar depois]** com as Notícias.

## 10. Simulação de mesa

### 10.1 Cenário

- **C:** uma capital com floresta e Posto Comercial. Tábua: estoque 180, alvo 60, preço local 0,8 (fator no mínimo). Grão: estoque 60, alvo 90, preço 0,6.
- **D1:** um vilarejo sem floresta, a 3 trechos de estrada. Tábua: estoque 0, alvo 20, preço 3,0. Grão: estoque 200, alvo 150, preço 0,4.
- **D2:** uma capital sem floresta, a 3 trechos também. Tábua: estoque 0, alvo 80, preço 3,0.
- Clima de Sol. Tudo em território (50‰ por mapa).

### 10.2 A escolha no passo 3b do dia 1

| Destino | Ida | Volta | Bruto | × (1 − risco 8,1%) | − custo | Lucro |
|---|---|---|---|---|---|---|
| D1 | 20 Tábuas × 1,9 = 38,0 | 30 Grãos × (0,6 − 0,44) = 4,8 | 42,8 | 39,3 | − 61,6 | **−22,3** |
| D2 | 80 Tábuas × 1,9 = 152,0 | (D2 sem sobra que C queira) 0 | 152,0 | 139,7 | − 61,6 | **+78,1** |

D2 ganha (78,1 > 20% de 61,6 = 12,3). A **caravana 1** sai para D2 com 80 Tábuas (160 de peso); `aCaminhoPorItem` de D2: Tábua 80. A **caravana 2** refaz a conta: a falta de D2 agora é 0 e D1 dá prejuízo. **Fica em casa.**

### 10.3 A viagem, hora a hora

| Dia e hora | O que acontece |
|---|---|
| D1 0 h | Decide; tira 80 Tábuas e 8 Pães de C; acampada até as 6 h |
| D1 6 h a 17 h 58 | 3 trechos de 3.192 ticks; sorteios de encontro 412, 803, 95 (todos ≥ 50) → nada |
| D1 17 h 58 | Chega a D2. Vende 80 Tábuas: falta 80, todas a 2,7 = **216 moedas** do Banco de D2. Compra: nada. Parte de volta |
| D1 22 h | Acampa: andou cerca de 3.227 ticks (o trecho 1 e 35 ticks do 2) |
| D2 6 h a 13 h 56 | Termina os trechos 2 e 3 (6.349 ticks) |
| D2 13 h 56 | Chega em C: devolve 216 ao Banco de C e paga o salário de 2 dias, 2 × 4 × 6 = **48**, à classe Civil de C |
| D3 0 h | Passo 3b: decide de novo com os preços de agora |

**O dinheiro:** o Banco de C ganhou 216 − 48 = +168 e os Civis de C ganharam 48; D2 perdeu 216. A soma do mundo **não mudou**.

**Os preços:** D2 tem 80 Tábuas = o alvo → fator 100 → o preço desce 10% ao dia, de 3,0 até 1,5, em cerca de 7 dias. C fica no mínimo enquanto sobra. **O comércio aproxima o preço do destino**, como pede o documento.

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| O dinheiro do mundo se conserva? | **Sim:** a venda passa de um Banco a outro; o salário fica na cidade dona; só a comida da viagem some (é consumo) |
| Duas caravanas mandam o mesmo para o mesmo lugar? | **Não:** o `aCaminhoPorItem` desconta a falta (D2, caravana 2) |
| O jogador pode "despejar" carga e lucrar demais? | **Não:** acima do alvo, a loja paga 45% do base (exemplo 4.7: 68 em vez de 108). **Achado:** sem esse limite, o preço do dia ficava parado até a meia-noite e o jogador vendia 200 Tábuas a 2,7 |
| O vilarejo pequeno recebe Tábuas? | **Quase nunca** das caravanas: a falta dele (20) não paga a viagem (D1: −22,3). Só quando a falta passa de cerca de 43 Tábuas (parte 5). **Achado e decisão (CO1-4):** é um espaço para o jogador, que viaja barato (sozinho, sem salário) e ganha cerca de 36 numa ida a D1 (20 × (2,7 − 0,88), comprando na loja de C a 110%). As caravanas das cidades servem as cidades grandes |
| A caravana deixa C sem Tábuas? | **Não:** só leva a sobra (180 − 60 = 120; levou 80) |
| O risco da conta bate com o da Viagem? | Aproximado: 6 mapas × 3,75% de bandidos ≈ 21% de viagens com encontro; a caravana de força 8 perde para bandos de 5 ou mais (força 10+). A conta usa 60% × 60% (**medir**, teste 7) |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| D sem dinheiro acima de 250 | A loja não compra; a caravana volta com a carga e devolve ao estoque de C (perde a viagem). **Achado (CO1-6):** a conta 4.3 só considera destinos com Banco acima de 250 + o valor da venda |
| A caravana é atacada e perde tudo | Chega a D sem carga, não vende, volta. C perdeu as Tábuas e o salário |
| Tempestade na região no meio da viagem | Fica parada o dia todo (Viagem, 4.3); o salário continua |
| O destino passa a ter sobra enquanto a caravana anda | Ela vende o que couber na falta a 90% e o resto a 45% do base (4.7). A perda é do erro de previsão |
| O jogador vende antes da caravana chegar | A falta acabou: a caravana vende a 45% do base. O jogador "furou" a caravana; é jogo |
| Uma cidade perde o Posto Comercial | As caravanas que estão fora terminam a viagem e não saem mais |

### 10.6 O que achou e como foi resolvido

1. **Todas as cidades produziam igual** (OC1-4 copia a vila): sem diferença de preço, não há comércio. **Resolvido (CO1-1):** o gerador das Outras cidades só põe as construções Braçais dos **recursos que o mapa tem** (a Produção já pede a área do recurso, 4.3). A cidade sem floresta compra Tora e Tábua; Grão e Água existem em todo mapa com cidade. A ficha das Outras cidades ganha essa nota.
2. **O preço do dia não reagia à venda grande.** **Resolvido (CO1-3):** acima do alvo, 45% do base.
3. **Duas caravanas para o mesmo destino.** **Resolvido (CO1-2):** o `aCaminhoPorItem`.
4. **As caravanas não servem os vilarejos.** **Decidido (CO1-4):** fica assim; é o espaço do jogador.
5. **Destino sem dinheiro.** **Resolvido (CO1-6):** a conta só olha destinos com dinheiro para pagar.

### 10.7 Decidido em modo automático (rodada CO1, seguir sugestão)

1. **CO1-1:** as cidades só têm as Braçais dos recursos do próprio mapa; Grão e Água em todo mapa com cidade.
2. **CO1-2:** o `aCaminhoPorItem` desconta a falta do destino.
3. **CO1-3:** a loja paga 90% do preço local até o alvo e 45% do preço base além dele, para todos.
4. **CO1-4:** as caravanas das cidades só saem com lucro de 20% acima do custo; os vilarejos ficam para o jogador.
5. **CO1-5:** a caravana do jogador não precisa de Posto Comercial; só as das cidades.
6. **CO1-6:** um destino só entra na conta se o Banco dele tem a reserva de 250 mais o valor da venda.
7. **CO1-7:** nesta versão, as caravanas das cidades veem os preços de hoje de todas as cidades; com as Notícias, passam a ver os preços que chegaram até elas.

### 10.8 Ajustes do código (marco B2, 2026-10-21)

1. **O Posto Comercial** ainda não existe como construção: as cidades com Posto estão marcadas no `cidades_iniciais.json` (`posto_comercial`), as 6 capitais e a Cidade dos mercados.
2. **O passo 3b roda depois do dia de todas as cidades** (e não entre o Mercado e o estabilizador de cada uma): a caravana precisa ver as outras cidades, e as cidades rodam uma depois da outra.
3. **A caravana do jogador (4.9)** fica para quando houver Cavalos no conteúdo. O jogador comercia com o **grupo**: parado numa outra cidade, compra e vende na loja dela (o painel "Loja desta cidade" no Mapa Mundial), pela regra 4.7.
4. **A faixa da Reputação** (4.7, passo 5) entra com a Reputação.
5. **A loja da vila** passou a seguir a regra 4.7 também (CO1-3 é para todos): a compra tem a reserva de 2 dias (RE1-4); a venda paga 90% até o alvo e 45% do base além dele.
6. **O mercador viajante** continua só no console.
7. **Achado: o comércio quase não acontece** (de 4 a 11 viagens em 1 ano). Está na rodada CO2.

