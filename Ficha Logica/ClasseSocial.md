---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase A4 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com CS1 vêm da **rodada CS1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Classe Social

## 1. O que faz

Troca o "só existe o Civil" da fatia por **classes de dados**:

- Cada classe tem a **cesta mínima**, o **dinheiro** (a soma da classe), a **renda**, a **Felicidade** e os **dias em déficit** por tipo de consumo.
- O consumo, o gasto extra, o estabilizador e a Felicidade, que hoje rodam para o Civil, passam a rodar **para cada classe**, na ordem das classes.
- **Ricos:** cada Rico é dono de **pelo menos uma empresa** (uma construção de produção). O lucro é a renda dele.
- **Mobilidade:** um Civil sobe para Rico quando a cidade abre uma empresa nova; um Rico cai para Civil quando perde a empresa.

Documento do sistema: [[Sistema de Classe Social/Descrição|Classe Social]]; os donos e as empresas, no [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]].

**Esta versão:** só **Civil e Rico**. Os Escravos (fase D), a Nobreza e a Realeza (com a Facção, fase C) usam as mesmas regras quando chegarem. **(CS1-1)** Os Ricos só aparecem com a cidade no **nível 2** (40 NPCs): a vila de nível 1 continua só com Civis, e o teste de 1 ano não muda (parte 10.2).

## 2. Dados

### Definições (no `data/regras.json`, o bloco `"cesta_minima_civil"` vira `"classes"`)

```json
"classes": [
  { "id": "rico", "moradia": "construcao.casarao", "tier_minimo_da_comida": 2,
    "cesta": { "comida_por_dia": 1, "materiais_a_cada_dias": 4, "luxo_a_cada_dias": 4 },
    "habilidades_de_partida": { "especialista": [60, 90], "outras": [30, 60] } },
  { "id": "civil", "moradia": "construcao.casa", "tier_minimo_da_comida": 1,
    "cesta": { "comida_por_dia": 1, "materiais_a_cada_dias": 8 },
    "habilidades_de_partida": { "todas": [20, 60] } }
],
"empresas_dos_ricos_em_porcento_das_construcoes_de_producao": 30,
"nivel_da_cidade_para_os_ricos": 2,
"dias_de_deficit_de_luxo_para_perder_a_empresa": 16
```

- **A ordem da lista é a ordem do consumo:** a classe de cima come primeiro **(CS1-2)**.
- **Os grupos de itens** saem do tier e do tipo (IP2-11): comida = `alimento` com tier ≥ o mínimo da classe; Materiais = `mobilia` e `vestuario` de tier 1 e 2; **Luxo** = tier 3 que não é remédio.
- **A Casa** abriga 4; o **Casarão**, 2 por nível (V2-8). O Casarão é uma construção nova (média, como no documento da Construção) no `construcoes.json`.

### Estado (vai no save)

| Campo | Onde | Exemplo | No save? |
|---|---|---|---|
| `classe` | cada NPC | `rico` | Sim |
| `dinheiro`, `rendaDoDia` | cada classe (a soma) | Civil 4.200; Rico 380 | Sim |
| `dinheiroDoRico` | cada Rico (Dinheiro individual) | 95 | Sim |
| `empresaDoRico` | cada Rico: a construção de que ele é dono | `padaria_2` | Sim |
| `diasEmDeficit` | cada classe, cada tipo de consumo (comida, materiais, luxo) | Rico, luxo: 3 | Sim |
| `devidosEmFracoes` | cada classe, cada tipo (o `materiaisDevidosEmOitavos` de hoje, generalizado) | Rico, luxo: 2 de 4 | Sim |
| `felicidade` | cada classe | Civil 58; Rico 61 | Sim |

O `dinheiroDaClasseCivil` e o `rendaDoDiaDaClasseCivil` da Economia viram o Civil desta tabela; o save passa para a versão 3 (o carregar de um save antigo põe todo mundo no Civil).

## 3. Quando roda

| Quando | O que roda |
|---|---|
| **A cada dia**, passo 1 (consumo) | A cesta e o gasto extra de **cada classe**, na ordem das classes (regra 4.1) |
| **A cada dia**, passo 2 (salários) | A empresa do Rico paga os salários; o lucro dela vira a renda do Rico (regra 4.2) |
| **A cada dia**, passo 4 (estabilizador) | O estabilizador olha **cada classe** |
| **A cada dia**, passo 5 (Felicidade) | A Felicidade de **cada classe** (os mesmos 4 fatores, com a cesta dela) |
| **No dia 1 de cada estação** | Abrir empresas e subir Civis (regra 4.3); conferir a perda de empresas (regra 4.4) |

## 4. Regras passo a passo

### 4.1 O consumo por classe (todo dia)

Para cada classe, **na ordem da lista**:

1. **Comida:** para cada NPC da classe, tire 1 item de comida com tier ≥ o mínimo dela, **o mais barato** que existir. A classe paga ao dono do item (como hoje).
2. **Materiais e Luxo:** como os Materiais de hoje (Economia, regra 4.1, passo 2): some os NPCs aos devidos; a cada "N dias" completos, tire 1 item.
3. **Dias em déficit:** por tipo de consumo, como hoje.
4. **Gasto extra:** até 80% da renda da classe, como hoje (Economia, regra 10.5), com a cesta dela.

*Rico:* o dinheiro da classe Rico é a soma dos `dinheiroDoRico`. Cada compra da classe sai **dividida igualmente** entre os Ricos com dinheiro; quem não tem fica de fora da divisão. *Por que:* o Rico tem Dinheiro individual (Trabalho e Propriedade), mas o consumo continua agregado por classe (documento).

### 4.2 A empresa do Rico (todo dia)

A empresa é uma construção de produção com `dono` = o Rico. Ela funciona como as da Cidade:

1. A **receita** é escolhida pela IA da Cidade (Produção, regra 4.3). O Rico não tem fila de ordens.
2. **As vendas** (o que a cidade compra dela) vão para o `dinheiroDoRico`.
3. **Os salários e os insumos** saem do `dinheiroDoRico`. Sem dinheiro, o salário atrasa (Economia, regra 10.8), e a empresa pode ir a leilão (Trabalho, regra 4.4).
4. **O lucro do dia** = vendas − salários − insumos. Se for positivo, 20% é imposto (vai para o Banco), e o resto é a renda do Rico (a `rendaDoDia` da classe Rico soma os lucros depois do imposto).

### 4.3 Abrir uma empresa e subir um Civil (dia 1 de cada estação)

1. **Pode?** A cidade está no nível 2 ou mais **e** as empresas dos Ricos são menos de 30% das construções de produção (arredondado para baixo) **e** há cama livre num Casarão.
2. **Que construção:** a construção de produção **da Cidade** com o **maior lucro** nos últimos 16 dias **(CS1-3)**. Ela passa a ser do Rico novo. Os empregados continuam.
3. **Que Civil:** o de maior habilidade no Trabalho da construção (no empate, o de menor número), entre os que **não** são empregados do jogador.
4. **A troca:** ele vira Rico; deixa o emprego (a vaga abre, e a regra 4.1 do Trabalho a preenche); muda para o Casarão. O `dinheiroDoRico` começa com o dinheiro médio de um Civil × 5 **(CS1-4)**, tirado da classe Civil (dinheiro que já existia; nada nasce do nada).
5. No máximo **1 por estação**. Escreva no Diário.

### 4.4 Perder a empresa e cair para Civil

1. **Pelo leilão:** a empresa do Rico vai a leilão (Trabalho, regra 4.4). Sem lance, ela volta para a Cidade.
2. **Pelo Luxo:** se a classe Rico fica **16 dias seguidos** em déficit de Luxo, o Rico com **menos dinheiro** perde a empresa (evento "Perda de empresas": volta para a Cidade, sem pagamento).
3. **Ao perder a empresa**, o Rico cai para Civil: muda para uma Casa com cama livre, fica desempregado, e o `dinheiroDoRico` passa para a classe Civil. Sem cama livre numa Casa, ele **continua no Casarão** até abrir uma cama (sem ser Rico).

### 4.5 A Felicidade por classe

A regra 4.1 da ficha da [[Ficha Logica/Felicidade|Felicidade]] roda para cada classe, com:

- **Consumo atendido** da classe: os sem comida dela, o déficit dos Materiais dela e, **no Rico**, −5 se o Luxo está em déficit;
- **Desemprego e renda:** no Rico, que não tem emprego, conta só a renda: +10 se o lucro de ontem foi positivo, −10 se foi negativo **(CS1-5)**;
- **Moradia** e **Eventos:** iguais, com os eventos de cada classe.

### 4.6 As habilidades de partida

O NPC criado pela População (o nascido e o migrante, regra 4.5 da [[Ficha Logica/Populacao|ficha da População]]) sorteia as habilidades na faixa da classe dele. O Civil: 20 a 60 em todas. A reposição nasce Civil.

## 5. Contas

| Conta | Exemplo |
|---|---|
| Cesta do Rico por dia (Pão Doce 3,0; Móveis 5,1; um Luxo de 12) | 3,0 + 5,1 ÷ 4 + 12 ÷ 4 = **7,3 moedas** |
| Cesta do Civil por dia | 1,9 + 5,1 ÷ 8 = **2,5 moedas** |
| Lucro de uma Padaria que vende 36 Pães a 1,9 (preço = custo × 1,2) | 68,4 × 0,2 ÷ 1,2 = **11,4**; imposto 2,3; renda do Rico **9,1** |
| O Rico pode gastar | 80% de 9,1 = **7,3**: paga a cesta, sem sobra |
| Lucro de uma Fazenda que vende 20 Grãos a 0,5 | 10 × 0,2 ÷ 1,2 = **1,7**; renda 1,3: não paga a cesta |
| Empresas numa cidade com 10 construções de produção | 30% = **3** |

## 6. Conversa com outros sistemas

| Sistema | Lê | Entrega |
|---|---|---|
| Economia | O preço local, o dono do item | O consumo, o gasto extra e o estabilizador por classe; a renda do Rico |
| Trabalho | O dono, o leilão, as vagas | A empresa do Rico; o Rico que dá lance no leilão; o Civil que deixa o emprego |
| Produção | A IA da receita, o lucro de cada construção | A empresa sempre no modo automático |
| Felicidade | — | A Felicidade de cada classe |
| População | — | A faixa das habilidades por classe; a moradia por classe |
| Cidade | O nível da cidade, as construções de produção | — |
| Eventos | — | "Perda de empresas" |
| Construção | O Casarão | — |
| Interface | — | No painel da cidade: as classes, a Felicidade e a riqueza de cada uma |
| Save | — | A parte 2 |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `Codigo/Vila/ClasseSocial.h`: `DefinicaoDeClasse`, `EstadoDaClasse` | Os dados e o estado da parte 2 |
| `ClasseDoNpc`, `RicoDonoDeEmpresa` | Os componentes do NPC |
| `consumirACestaDeCadaClasse(...)` | A regra 4.1 (substitui a `consumirACestaDoDia`) |
| `fecharOLucroDaEmpresaDoRico(...)` | A regra 4.2 |
| `abrirEmpresaESubirUmCivil(...)`, `tirarAEmpresaDoRico(...)` | As regras 4.3 e 4.4 |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Vila de nível 1 | Nenhum Rico; os mesmos números do teste de 1 ano |
| Cidade de nível 2, 10 construções, 2 empresas, Casarão com cama | Abre a 3ª empresa: a de maior lucro; sobe o Civil de maior habilidade |
| O mesmo, sem cama em Casarão | Não abre |
| O mesmo, com 3 empresas | Não abre (30% de 10 = 3) |
| Civil com 12 de dinheiro médio vira Rico | O Rico começa com 60; a classe Civil perde 60 |
| A Padaria do Rico vende 68,4 com lucro 11,4 | Imposto 2,3; renda 9,1 |
| A empresa com prejuízo | Sem imposto; renda negativa; Felicidade do Rico com −10 na renda |
| 16 dias sem Luxo | O Rico mais pobre perde a empresa e vira Civil |
| Pouca comida: 3 Pães Doces e 2 Ricos; Pão para 30 Civis de 36 | Os Ricos comem primeiro (Pão Doce); 6 Civis ficam sem comida |
| Um save da versão 2 | Carrega com todos no Civil |

## 9. Fora desta versão

- **Escravo, Nobreza e Realeza**, os Bens Exclusivos e os Artefatos.
- **A Bebida e a Taverna** no gasto extra (até 25% da Comida): quando houver Taverna.
- **Os itens de Rico** (comida Tier 2 e Luxo Tier 3): vêm com o conteúdo (rodadas C1 a C3). **Sem eles, a cidade não abre empresas** (a regra 4.3 confere se existe pelo menos 1 item de cada grupo do Rico nos dados).
- **O jogador subir para Rico com o dinheiro dele** e o título: Objetivo e Progressão (fase E).
- **Penas por classe:** Crime e Justiça (fase C).
- **LOD 2:** o consumo por estação.

## 10. Simulação de mesa

### 10.1 Cenário

Uma cidade de **nível 2** (o próximo passo da vila; ficha da Cidade): 40 NPCs, 10 construções de produção, 1 Casarão (2 camas). Itens novos supostos: Pão Doce (tier 2, 3,0) e um Luxo de tier 3 (12,0). No começo, 40 Civis e nenhuma empresa.

### 10.2 A vila de nível 1 não muda

Nível 1: a regra 4.3 não abre empresas, e a lista das classes só tem gente no Civil. O consumo por classe com uma classe só é o consumo de hoje. **O teste de 1 ano dá o mesmo resultado.**

### 10.3 O primeiro Rico, estação a estação

| Estação | O que acontece |
|---|---|
| 1 | A regra 4.3 abre a 1ª empresa. A de maior lucro é a **Padaria** (11,4 por dia). Sobe a Rita (Manufaturado 58): Rico, no Casarão, com 60 de dinheiro (5 × 12) |
| 1, por dia | Lucro 11,4 → imposto 2,3 → renda 9,1. A cesta do Rico custa 7,3 e é paga. Sobra cerca de 1,8 por dia: o dinheiro da Rita sobe uns 29 por estação |
| 2 | 2ª empresa: o **Mercado** (o 2º maior lucro). Sobe o Tomás. O Casarão fica cheio (2 camas) |
| 3 | 3ª empresa: **não abre**, porque não há cama em Casarão. A cidade (ficha da Cidade) precisa construir outro |

**O dinheiro do Banco:** antes, o lucro da Padaria (11,4 por dia) ia para o Banco. Agora vai para a Rita, e o Banco recebe 2,3 de imposto e as compras dela nas lojas da Cidade (o Pão Doce, os Móveis e o Luxo, se forem da Cidade): cerca de 7 por dia. **O Banco perde cerca de 2 por dia por Rico** (a poupança do Rico), cerca de 130 por ano. Com a verba da Coroa (300 por ano), cabe.

### 10.4 Por que a empresa de maior lucro (CS1-3)

Se a regra pegasse uma construção qualquer, a **Fazenda** daria ao Rico uma renda de 1,3 por dia, e a cesta dele custa 7,3. Ele não pagaria a cesta, o estabilizador daria dinheiro do Banco a ele todo dia, e o Luxo entraria em déficit: em 16 dias, perderia a empresa. Um Rico que nasce para quebrar. Com "a de maior lucro", os 2 primeiros Ricos se sustentam. **O 3º e o 4º** (com empresas menores) ficam no limite: é a mobilidade "rara" do documento.

### 10.5 Verificações

| O que nunca pode acontecer | Resultado |
|---|---|
| Dinheiro surgindo do nada | **OK:** o Rico começa com dinheiro tirado da classe Civil; o lucro vem das vendas |
| Rico sem empresa | **OK:** perder a empresa = cair para Civil (regra 4.4) |
| Empresa sem dono | **OK:** volta para a Cidade |
| Mais Ricos que camas de Casarão | **OK:** regra 4.3, passo 1 |
| O jogador perdendo um empregado para a promoção | **OK:** a regra 4.3 pula os empregados do jogador |
| Um Rico com dinheiro negativo | **OK:** a compra só divide entre quem tem dinheiro; o salário atrasa em vez de ficar negativo |
| Os Civis comendo o Pão Doce dos Ricos | Pode: o Civil come tier 1 **ou 2**, e o mais barato primeiro. Como os Ricos comem antes (CS1-2), o Pão Doce só sobra para os Civis se houver demais |

### 10.6 Casos extremos

| Caso | O que acontece |
|---|---|
| Nenhum item de Luxo nos dados | A cidade não abre empresas (parte 9) |
| A cidade cai do nível 2 para o 1 | Os Ricos continuam (a regra 4.3 só impede **novas** empresas) |
| A empresa do Rico vai a leilão, e outro Rico dá lance | O comprador fica com 2 empresas: **(CS1-6)** pode, e a renda dele soma as duas |
| Fome geral | Os Ricos comem primeiro; a Felicidade dos Civis cai mais rápido |

### 10.7 Decidido em modo automático (rodada CS1, seguir sugestão)

1. **CS1-1:** os Ricos só a partir do nível 2 da cidade (a vila de 15 teria 1,5 Rico, e a renda de uma empresa pequena não paga a cesta dele).
2. **CS1-2:** a ordem do consumo é a da lista das classes: de cima para baixo (a classe alta come primeiro).
3. **CS1-3:** a empresa nova é a construção da Cidade de maior lucro nos últimos 16 dias (parte 10.4).
4. **CS1-4:** o Rico novo começa com 5 vezes o dinheiro médio de um Civil, tirado da classe Civil.
5. **CS1-5:** no Rico, o fator "Desemprego e renda" usa só o lucro de ontem: +10 ou −10.
6. **CS1-6:** um Rico pode ter mais de uma empresa, se comprar no leilão.
