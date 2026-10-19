---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase A5 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com CI1 vêm da **rodada CI1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Cidade

## 1. O que faz

A vila passa a **crescer sozinha**:

- **A Cidade encomenda obras** ao Construtor: as construções que o próximo nível exige, Casas quando faltam camas, Casarões para os Ricos e níveis a mais nas construções que não dão conta.
- **A cidade sobe de nível** quando cumpre as condições (ocupação, Felicidade, construções obrigatórias, dinheiro). Cada nível traz mais capacidade de NPCs e mais lotes.
- **A cidade cai de nível** numa catástrofe longa, com aviso antes.

Documentos do sistema: [[Sistema de Cidade/Descrição|Cidade]] (níveis e atributos) e os "Níveis de cidade" da [[Sistema de População/Descrição|População]]. As obras são as da ficha da [[Ficha Logica/Construcao|Construção]] (parte 10).

**Esta versão:** os níveis 1 a 3 (o 4 é só das capitais, por conquista). Sem Tecnologia ainda (fase B5): a condição de tecnologia fica desligada.

## 2. Dados

### Definições (no `data/regras.json`, bloco novo `"cidade"`)

```json
"cidade": {
  "ocupacao_minima_para_subir_em_porcento": 90,
  "estacoes_com_ocupacao_para_subir": 2,
  "felicidade_minima_para_subir": 55,
  "obrigatorias_por_nivel": {
    "2": ["construcao.mercado", "construcao.taverna"],
    "3": ["um_de:construcao.quartel,construcao.templo,construcao.laboratorio"]
  },
  "custo_para_subir_em_decimos": { "2": 1000, "3": 3000 },
  "reserva_do_banco_em_decimos": 2500,
  "migrantes_a_mais_com_ocupacao_abaixo_de_em_porcento": 50,
  "queda": { "dias_seguidos_de_fome": 16, "ocupacao_abaixo_de_em_porcento_do_nivel_anterior": 50,
             "estacoes": 2 }
}
```

| Campo | O que é |
|---|---|
| `ocupacao_minima...`, `estacoes_com_ocupacao...` | NPCs reais ≥ 90% da capacidade no dia 1 de 2 estações seguidas |
| `felicidade_minima_para_subir` | A Felicidade dos Civis (CI1-1) |
| `obrigatorias_por_nivel` | As do documento. No nível 3, uma das três, conforme a cultura (fase C); nesta versão, qualquer uma |
| `custo_para_subir_em_decimos` | 100 moedas para o 2; 300 para o 3, do Banco (CI1-2) |
| `reserva_do_banco_em_decimos` | A Cidade não gasta em obras nem em subir de nível se o Banco ficaria abaixo de 250 (o mínimo do teste de 1 ano, FE4-3) |
| `queda` | As condições da regra 4.5 |

**No mapa (Tiled):** cada lote ganha a propriedade `nivel_da_cidade` (1, 2, 3 ou 4): o lote só existe a partir desse nível. O mapa da vila precisa dos lotes dos níveis 2 e 3 (16 e 30 pequenos, 6 e 12 médios, 1 e 3 grandes, V3-8). **Isso pede um mapa novo, maior:** conteúdo do autor.

**Construções novas** no `construcoes.json`: a **Taverna** e o **Casarão** (médios, documento da Construção). A Taverna não tem receita até existir a Bebida (conteúdo): ela conta como obrigatória, mas os empregados ficam de folga **(CI1-3)**.

### Estado (vai no save)

| Campo | Exemplo | No save? |
|---|---|---|
| `nivelDaCidade` | 1 | Sim |
| `estacoesSeguidasComOcupacao` | 1 | Sim |
| `estacoesSeguidasEmQueda`, `avisoDeQuedaDado` | 0, falso | Sim |
| As obras da Cidade na fila do Construtor (com o dono "cidade") | Taverna no lote 14 | Sim (na seção `obras`) |

## 3. Quando roda

| Quando | O que roda |
|---|---|
| **No dia 1 de cada estação**, depois da População | Conferir a subida (regra 4.3) e a queda (regra 4.5) |
| **A cada dia**, depois da escolha das receitas (passo 7) | Encomendar no máximo **1 obra** da Cidade, se não houver outra obra da Cidade na fila (regra 4.1) |
| **Quando uma obra da Cidade termina** | Ela entra na vila com o dono "cidade"; a regra 4.1 do Trabalho preenche as vagas |

## 4. Regras passo a passo

### 4.1 A Cidade encomenda obras (todo dia)

Só se **não há obra da Cidade na fila** e o Banco, depois de pagar, fica acima da reserva (250). Escolha a primeira desta lista que der:

1. **Obrigatória do próximo nível** que falta (a Taverna, para o nível 2), num lote livre do porte dela, **mesmo que o lote seja do próximo nível** (CI1-4).
2. **Casa**, se as camas livres são menos que 2 e os NPCs reais estão abaixo da capacidade.
3. **Casarão**, se a cidade abre empresas (ficha da [[Ficha Logica/ClasseSocial|Classe Social]], regra 4.3) e não há cama livre num Casarão.
4. **Subir de nível uma construção de produção da Cidade** cujo produto passou 4 dias ou mais em déficit na estação (o produto mais em falta primeiro), se o nível dela for menor que o da cidade + 1 e o Engenheiro puder (Construção, regra 10.4, passo 2).
5. **Nada:** espere o próximo dia.

**Como a Cidade paga** (igual à manutenção, Construção, regra 10.6): os Blocos saem do estoque da Cidade; as **Tábuas**, a Cidade compra do estoque do jogador pelo preço dele (o dinheiro sai do Banco para o jogador). As horas, os Operários já recebem do Banco. **Se faltar material**, a obra fica na fila esperando, e o Diário avisa: "A vila precisa de 20 Tábuas para a Taverna".

O lote: o primeiro lote livre do porte, na ordem dos lotes do mapa.

### 4.2 O que é uma "obra da Cidade" na fila

A fila do Construtor é uma só, por ordem de chegada (as do jogador e as da Cidade). A obra da Cidade usa o mesmo andamento (horas dos Operários, entrega dos materiais).

### 4.3 Subir de nível (dia 1 de cada estação)

1. **Ocupação:** se NPCs reais ≥ 90% da capacidade, some 1 a `estacoesSeguidasComOcupacao`; senão, zere.
2. **Pode subir** se tudo vale:
   - `estacoesSeguidasComOcupacao` ≥ 2;
   - Felicidade dos Civis ≥ 55;
   - as obrigatórias do próximo nível existem e estão prontas;
   - o Banco tem o custo + a reserva (100 + 250 para o nível 2);
   - o próximo nível é 2 ou 3 (o 4 só por conquista).
3. **Sobe:** pague o custo do Banco (o dinheiro sai do jogo: são as festas e os papéis da nova vila **(CI1-2)**); `nivelDaCidade` + 1; a capacidade passa para a do nível (40 ou 100, ficha da [[Ficha Logica/Populacao|População]]); os lotes do novo nível aparecem; zere a contagem. Diário: "A vila virou Vilarejo!".

### 4.4 Depois de subir: o crescimento

- A População (regra 4.4) traz **1 migrante por estação**, e **mais 1** se a ocupação está abaixo de 50% **(CI1-5)**.
- Os nascimentos continuam (+54 pessoas por estação numa vila de 15).
- A regra 4.1 constrói Casas antes de faltar cama.

### 4.5 Cair de nível (dia 1 de cada estação)

1. **Em queda** se, na estação que terminou: houve **16 dias seguidos de fome**, **ou** os NPCs reais estão abaixo de 50% da capacidade do nível **anterior** (no nível 2: abaixo de 8).
2. Na 1ª estação em queda: **aviso** no Diário ("Se a fome continuar, a vila volta a ser Assentamento").
3. Na 2ª estação seguida: **cai** 1 nível. As construções acima do novo nível (e as dos lotes que somem) ficam **inativas**: sem empregados (viram desempregados), sem produzir, sem ser destruídas. Voltam a funcionar se a cidade subir de novo.
4. Uma estação sem queda zera a contagem e o aviso.

## 5. Contas

| Conta | Exemplo |
|---|---|
| Custo da Taverna (média, nível 1) | 20 Tábuas + 30 Blocos + 120 horas |
| Tempo da Taverna | 2 Operários × cerca de 8 h por dia = 16 h por dia: **cerca de 8 dias** |
| Custo da Casa (pequena) | 10 Tábuas + 10 Blocos + 40 horas (2,5 dias) |
| O Banco para subir ao nível 2 | 100 + 250 de reserva = **350** |
| Casas para 40 NPCs | 16 camas hoje; faltam 24: **6 Casas** (60 Tábuas) |
| Crescimento no nível 2 | Nascimentos cerca de 0,5 NPC por estação + migração 2 (abaixo de 50%) = **2,5 por estação**: de 15 a 20 em 2 estações; de 20 a 36 (90%) em mais cerca de 10 estações (a migração cai para 1 acima de 50%) |

## 6. Conversa com outros sistemas

| Sistema | Lê | Entrega |
|---|---|---|
| População | Os NPCs reais, as camas | A capacidade; o migrante a mais |
| Felicidade | A Felicidade dos Civis | — |
| Construção | A fila, os custos, o Engenheiro | As obras da Cidade |
| Economia | O Banco; o estoque e o preço da Tábua do jogador | O pagamento ao jogador pelas Tábuas; o custo de subir |
| Produção | Os dias em déficit de cada produto | — |
| Classe Social | Se a cidade abre empresas | O nível (os Ricos a partir do 2) |
| Trabalho | — | As vagas novas |
| Mapa | Os lotes com `nivel_da_cidade` | Os lotes que aparecem e somem |
| Interface | — | O nível e as condições no painel da cidade; os avisos no Diário |
| Simulação sem tela | — | Colunas novas: `nivel`, `obra_da_cidade`, `lotes_livres` |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Codigo/Vila/Cidade.h`: `RegrasDaCidade`, `EstadoDaCidade` | A parte 2 |
| `escolherObraDaCidade(...)` | A regra 4.1 |
| `conferirSubidaDeNivel(...)`, `subirNivelDaCidade(...)` | A regra 4.3 |
| `conferirQuedaDeNivel(...)` | A regra 4.5 |
| `lotesDoNivel(mapa, nivel)` | Os lotes que existem no nível |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Nível 1, sem Taverna, lote médio livre só no nível 2 | Encomenda a Taverna no lote do nível 2 |
| Banco em 290; as 20 Tábuas da Taverna custam 30 | Encomenda (o Banco fica em 260); com o Banco em 270, não |
| Taverna na fila esperando Tábuas, jogador com 0 | A obra espera; o Diário avisa |
| 15 de 15 NPCs por 2 estações, Felicidade 58, Mercado e Taverna, Banco 400 | Sobe ao nível 2; Banco 300; capacidade 40 |
| O mesmo, Felicidade 50 | Não sobe |
| Nível 2 com 7 NPCs por 2 estações | Aviso na 1ª; cai na 2ª; as construções dos lotes do nível 2 ficam inativas |
| Teste de 1 ano, 10 sementes | O mesmo resultado de hoje (parte 10.2) |

## 9. Fora desta versão

- **Tecnologia** do ramo Construção como condição: fase B5.
- **Obrigatórias por cultura** no nível 3: fase C.
- **Nível 4** (capitais, conquista): fase D.
- **Saque e cerco** como causa de queda: fase D.
- **A cidade do jogador** (fundar um assentamento e decidir as obras dele): Objetivo e Progressão (fase E).
- **Construções de produção novas** pela Cidade (uma segunda Fazenda): nesta versão, a Cidade só sobe o nível das que existem (regra 4.1, passo 4).

## 10. Simulação de mesa

### 10.1 Cenário

A vila do teste de 1 ano: nível 1, 15 de 15 NPCs, 16 camas, 11 lotes (o 5 livre, pequeno), Banco perto de 475, Felicidade perto de 58. O jogador faz Móveis e quase não guarda Tábuas (0 a 4 no relatório).

### 10.2 O teste de 1 ano (o jogador sem mexer)

| Dia | O que acontece |
|---|---|
| 1 | Regra 4.1: falta a Taverna. Lote médio livre só no nível 2 → **precisa do mapa novo**. No mapa da fatia (sem lotes do nível 2), não há lote: **nada acontece**. Casas: 1 cama livre (< 2), mas a vila está cheia (15 = capacidade): nada |
| Estações 1 e 2 | Ocupação 100%: a contagem chega a 2. Mas falta a Taverna: **não sobe** |

**Resultado:** no mapa da fatia, a vila não cresce, e o teste de 1 ano dá o mesmo resultado. **OK.**

### 10.3 Com o mapa novo e o jogador vendendo Tábuas

| Dia | O que acontece |
|---|---|
| 1 | A Cidade encomenda a Taverna no 1º lote médio do nível 2. Blocos: 30 do estoque (havia 244). Tábuas: o jogador tem 2. **A obra espera**; Diário: "A vila precisa de 20 Tábuas para a Taverna" |
| 1 a 6 | O preço da Tábua sobe (a obra conta como demanda). O jogador muda a fila para fazer Tábuas (a Carpintaria faz cerca de 10 por dia) |
| 3 | 20 Tábuas: a Cidade compra (cerca de 30 moedas do Banco para o jogador). A obra começa |
| 11 | A Taverna fica pronta (8 dias). Sem Bebida, os 2 Taverneiros contratados folgam (CI1-3) |
| Estação seguinte | Ocupação 2 estações ✓, Felicidade 58 ✓, Mercado e Taverna ✓, Banco cerca de 440 ≥ 350 ✓: **sobe ao nível 2**. Banco cerca de 340 |
| As 6 estações seguintes | A regra 4.1 faz Casas (10 Tábuas cada, do jogador), uma de cada vez. A população chega a cerca de 30 |

**O que mostra:**

1. **O jogador é o fornecedor de Tábuas da vila.** Sem ele, a vila não cresce. É um bom gancho de jogo (a vila "pede" e paga), mas é uma dependência forte: registrado na rodada (CI1-6).
2. **A Taverna é obrigatória e não faz nada** até existir a Bebida.
3. **O Banco aguenta**: Taverna (cerca de 30 de Tábuas) + subir (100) + 6 Casas (cerca de 90 de Tábuas) = cerca de 220 em 2 anos, contra 600 da verba da Coroa no mesmo tempo.

### 10.4 Verificações

| O que nunca pode acontecer | Resultado |
|---|---|
| O Banco abaixo de 250 por causa de obra ou de nível | **OK:** a reserva (regra 4.1 e 4.3) |
| Duas obras da Cidade ao mesmo tempo | **OK:** uma de cada vez |
| A Cidade usando um lote do jogador | **OK:** só lote livre |
| A capacidade abaixo dos NPCs reais | **OK:** a capacidade só cai na queda, e aí a População não cria ninguém até voltar a caber (ninguém é expulso) |
| A obrigatória sem lote (o impasse do nível 1) | **OK:** CI1-4 deixa usar o lote do próximo nível |
| Dinheiro surgindo do nada | **OK:** as Tábuas passam do Banco para o jogador; o custo de subir **sai** do jogo (como a manutenção) |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| A fome derruba a vila de 40 para 7 | Aviso, depois queda para o nível 1; as construções dos lotes do nível 2 inativas; os empregados delas viram desempregados |
| O jogador ocupa todos os lotes médios livres | A Taverna espera um lote; a vila não sobe |
| O jogador vende Tábuas mais caras (o preço dele) | A Cidade paga o preço do jogador, até a reserva do Banco |
| A vila sobe e cai várias vezes | Cada subida custa 100; as construções voltam a funcionar |

### 10.6 Ligações refeitas

- A **regra 4.1 da Classe Social** (abrir empresas) depende do Casarão, que esta ficha manda construir (passo 3).
- A **ficha da População** usa a capacidade desta ficha e o migrante a mais (regra 4.4).

### 10.7 Decidido em modo automático (rodada CI1, seguir sugestão)

1. **CI1-1:** a Felicidade mínima para subir é 55 (a vila de hoje fica perto de 58).
2. **CI1-2:** subir custa 100 moedas (nível 2) e 300 (nível 3), do Banco, que saem do jogo; o Banco nunca fica abaixo de 250.
3. **CI1-3:** a Taverna existe sem receita até a Bebida; conta como obrigatória, e os empregados folgam.
4. **CI1-4:** a obrigatória do próximo nível pode usar um lote do próximo nível (no nível 1 não há lote médio livre).
5. **CI1-5:** abaixo de 50% de ocupação, chega 1 migrante a mais por estação.
6. **CI1-6:** a Cidade compra as Tábuas do jogador e espera por elas, sem outra fonte. Uma Carpintaria da Cidade fica para quando houver construções de produção novas (parte 9).
