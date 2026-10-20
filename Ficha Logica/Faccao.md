---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase C do [[Ficha Logica/Plano das Fichas|Plano das Fichas]], a primeira da fase porque o Crime, a Religião e a Cultura dependem dela), com a **parte 10: Simulação de mesa**. Os pontos marcados com FA1 vêm da **rodada FA1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Facção

## 1. O que faz

Agrupa as cidades em **6 facções**, com um líder, as **leis**, um **Banco da Facção** e a **assembleia** anual que muda as leis. Diz também de quem é cada mapa sem cidade (o **território**).

Documento do sistema: [[Sistema de Facção/Descrição|Facção]]; as leis desejadas vêm da [[Sistema de Cultura/Descrição|Cultura]].

**Esta versão:** as facções e as cidades delas, o território derivado, as leis (com efeito nas que já têm sistema: imposto, tarifa, itens proibidos, ajuste de preço), o Banco da Facção (cunhagem, tarifa, verba às cidades) e a assembleia. **Exército, conscrição, guerra, diplomacia, dinastia, sucessão, revolta e a facção do jogador** ficam na parte 9.

## 2. Dados

### Definições: `data/faccoes.json` (arquivo novo)

```json
{
  "leis": {
    "imposto":     { "opcoes": ["baixo", "normal", "alto"], "valores_em_porcento": [10, 20, 30], "felicidade": [0, -5, -10] },
    "tarifa":      { "opcoes": ["baixa", "media", "alta"], "valores_em_porcento": [5, 10, 20] },
    "magia":       { "opcoes": ["livre", "restrita", "privilegio_dos_magos", "proibida"] },
    "escravidao":  { "opcoes": ["proibida", "so_prisioneiros", "como_pena", "ampla"] },
    "poder_do_lider": { "opcoes": ["fraco", "moderado", "autoritario"] },
    "sistema_de_votacao": { "opcoes": ["maioria_simples", "dois_tercos", "por_populacao", "por_nivel", "conselho"] },
    "tolerancia_religiosa": { "opcoes": ["tolerancia", "fe_oficial", "perseguicao"] },
    "livro_sagrado_obrigatorio": { "opcoes": ["nao", "sim"] }
  },
  "faccoes": [
    { "id": "faccao.guerreira", "cultura": "guerreira", "capital": [1, 1],
      "leis": { "imposto": "baixo", "tarifa": "media", "magia": "restrita", "escravidao": "so_prisioneiros",
                "poder_do_lider": "moderado", "sistema_de_votacao": "maioria_simples",
                "tolerancia_religiosa": "tolerancia", "livro_sagrado_obrigatorio": "nao" },
      "itens_proibidos": ["narcotico"], "ajuste_de_preco": { "arma": -20 } }
  ],
  "cunhagem_por_casa_da_moeda_por_dia_em_decimos": 1400,
  "verba_por_npc_por_estacao_em_decimos": 50,
  "banco_inicial_em_porcento_do_pib": 25,
  "legitimidade_inicial": 70,
  "veto_custa_legitimidade": 5,
  "leis_por_assembleia": 3,
  "felicidade_de_quem_perdeu_a_votacao": -5
}
```

As 6 facções seguem a tabela de leis iniciais do documento (V3-1). A Conscrição e os Crimes e penas entram com a Guerra e o Crime.

### Na Cultura (`data/culturas.json`, o que a Facção usa)

Para cada cultura e cada lei: o **valor desejado** (uma das opções) e o **peso** (1 a 5). O pensamento extremo é a lei com o valor no limite e peso 5, guardado à parte (`extremo`), e **não entra no desejo da assembleia** (FA1-5).

### Estado (vai no save)

| Campo | Exemplo |
|---|---|
| `leisAtuais` | por facção, o valor de cada lei |
| `bancoDaFaccao` | Guerreira: 5.844 moedas |
| `liderDaFaccao` | idade, Agressividade e Sociabilidade (0 a 100), sorteados ao criar o mundo |
| `legitimidadeDoLider` | 70 |
| `perdeuAVotacaoAte` | por cidade, o dia em que acaba o −5 de Felicidade |

**Derivados, não guardados:** as cidades da facção (pela `cidades_iniciais.json`), o PIB (soma dos Bancos e do dinheiro das classes das cidades), a cultura dominante e o estado de cada mapa (4.1).

**O sorteio** (só do líder, ao criar o mundo): o PCG32 da **sequência 7**. A assembleia sorteia o veto com a mesma sequência.

## 3. Quando roda

| Quando | O quê |
|---|---|
| Ao criar o mundo | Banco inicial, líder (4.6) |
| **A cada dia, passo 2** (salários) | O imposto pela lei da facção (4.2) |
| Quando uma caravana **entra no território** | A tarifa (4.3) |
| **A cada dia, passo 4** (estabilizador) | A cunhagem (4.4) |
| **Dia 1 de cada estação, passo 4** | A verba às cidades (4.4) |
| **Dia 1 de cada ano, passo 4** | A assembleia (4.5) |

## 4. Regras passo a passo

### 4.1 O território (derivado)

Para cada mapa, olhando os 4 vizinhos na malha:

1. **Com cidade:** tem cidade. É da facção da cidade, ou de ninguém (Vila Livre, vila do jogador independente).
2. **Território de facção:** sem cidade, e as cidades **de facção** vizinhas são todas da **mesma** facção.
3. **Em disputa:** sem cidade, com cidades vizinhas de **facções diferentes**. Quem domina fica guardado (exceção do documento; nesta versão, ninguém domina).
4. **Sem dono:** sem cidade vizinha de facção. As Vilas Livres não geram território.

Recalculado só quando uma cidade muda de dono. **Muda a Viagem (FA1-6):** o `estado_do_territorio` sai do `mapas.json` e passa a vir desta regra, como o documento manda.

### 4.2 O imposto (passo 2)

1. A taxa vem da lei **Imposto** da facção da cidade (10, 20 ou 30%); a cidade sem facção usa 20%.
2. A taxa é retida de cada salário, como hoje (Economia, 10.7).
3. **Todo o imposto fica com o Banco da cidade** nesta versão (FA1-1). O documento manda 70% para o Banco da Facção; isso entra com a Guerra, quando a facção tiver o que gastar (soldo). Antes disso, a facção só guardaria dinheiro, e as cidades secariam (parte 10.3).
4. **O teto do gasto extra muda (FA1-2):** passa a ser **(100 − taxa)% da renda** do dia da classe, não 80% fixo. Com 20%, nada muda.
5. A Felicidade da lei (0, −5, −10) entra na Felicidade da cidade (Felicidade).

### 4.3 A tarifa

1. Quando uma caravana entra num mapa de **território** ou **com cidade** de uma facção, vindo de um mapa que não é dessa facção, paga a tarifa: **valor da carga** (quantidade × preço local na cidade de onde saiu) × taxa ÷ 100, arredondado para baixo.
2. Quem paga: o Banco da cidade dona da caravana, ou o jogador. O dinheiro vai para o **Banco da Facção**.
3. As caravanas de uma facção não pagam tarifa no próprio território.
4. **No Comércio:** a conta do lucro esperado (Comércio, 4.3) soma as tarifas da rota ao custo.

### 4.4 O Banco da Facção

1. **Começa** com 25% do PIB da facção.
2. **Cunhagem:** cada **Casa da Moeda** da facção (pacote do nível 4) põe **140 moedas por dia** no Banco da Facção. É a fonte de dinheiro novo do mundo (Econômico).
3. **Verba às cidades:** no dia 1 de cada estação, o Banco da Facção paga a cada cidade **5 moedas por NPC real** (a verba que a Coroa pagava; OC1-5). Se o Banco não tem o total, paga a todas na mesma proporção.
4. **As cidades sem facção** (Vilas Livres e a vila do jogador independente) continuam recebendo os 5 por NPC "de fora" (FA1-3): é a renda delas (pedágios e contribuições). A vila do jogador **vinculada** a uma facção recebe da facção.
5. O `dinheiroQueEntrouDeFora` da Economia passa a contar a cunhagem e a verba das cidades sem facção.

### 4.5 A assembleia (dia 1 de cada ano)

1. **Propostas:** cada cidade calcula, para cada lei, a distância = |posição do valor atual − posição do valor desejado pela cultura dela| × peso. Propõe a lei de **maior distância**, se for maior que 0: mudar **um passo** na direção do desejo. Sem distância, não propõe.
2. **Pauta:** as 3 propostas com mais cidades; empate: a ordem das leis no arquivo.
3. **Voto de cada cidade:** sim se a mudança aproxima a lei do desejo da cultura dela; não se afasta; se não muda nada para ela, não vota. Com Felicidade abaixo de 30, vota **contra a posição do líder** (a do desejo da cultura do líder).
4. **Contagem**, pela lei Sistema de votação:
   - maioria simples: 1 voto por cidade, passa com mais da metade dos votos dados;
   - dois terços: passa com 2/3 dos votos dados;
   - por população: o voto pesa os NPCs reais da cidade;
   - por nível: o voto pesa o nível da cidade;
   - conselho: só a **capital** vota, até a dinastia e os nobres existirem (FA1-4).
5. **O líder:** se a lei passou e afasta do desejo da cultura dele:
   - Poder **autoritário**: veta sempre;
   - **moderado**: veta com a chance 50% + (Agressividade − Sociabilidade) ÷ 2, entre 0 e 100%;
   - **fraco**: não veta.
   Cada veto: legitimidade −5.
6. **Efeito:** a lei muda **no dia seguinte**; as cidades que votaram e perderam têm Felicidade −5 por 1 estação. Nasce a notícia **Regional** "A Arcana mudou a lei X".
7. A vila do jogador vinculada vota **como o jogador escolher** (a tela mostra a pauta; sem escolha, vota pela cultura dele).

### 4.6 O líder (nesta versão)

Ao criar o mundo, cada facção ganha um líder: idade 30 a 50, Agressividade e Sociabilidade de 20 a 80. **Não morre nesta versão** (a sucessão vem com a dinastia, fase D).

### 4.7 As leis com efeito nesta versão

| Lei | Efeito |
|---|---|
| Imposto | 4.2 |
| Tarifa | 4.3 |
| Itens proibidos | A loja legal **não vende nem compra** o item; a receita dele não é candidata na Produção. O contrabando vem com o Crime |
| Ajuste de preço | Vale só para o **jogador membro** da facção e as **caravanas das cidades dela** (é o benefício de membro do documento; FA1-7). O desconto sai do Banco da cidade |
| Sistema de votação, Poder do líder | 4.5 |
| Magia, Escravidão, Tolerância, Livro Sagrado, Conscrição | Guardadas; o efeito chega com os seus sistemas |

## 5. Contas

**O dinheiro de uma facção por estação** (Guerreira: 440 NPCs, 1 Casa da Moeda):

| Linha | Valor |
|---|---|
| Cunhagem | 140 × 16 = **+2.240** |
| Verba às cidades | 5 × 440 = **−2.200** |
| Tarifas (cerca de 4 caravanas de fora, carga de 64 moedas, 10%) | cerca de **+26** |
| Saldo | cerca de **+66 por estação** |

| Facção | NPCs | Verba | Saldo por estação (sem tarifas) | Banco inicial (25% do PIB) | Anos até zerar |
|---|---|---|---|---|---|
| Guerreira, Arcana, Autoritária, Religiosa, Tecnocrata | 440 | 2.200 | +40 | 5.800 a 7.900 | nunca |
| Escravista | 480 | 2.400 | −160 | 8.700 | cerca de 13 |

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| Economia | O imposto pela lei, o teto do gasto (FA1-2), a cunhagem e a verba no lugar da Coroa |
| Outras cidades | A facção de cada cidade; a verba (OC1-5) agora vem do Banco da Facção |
| Viagem | O estado do território passa a ser derivado (FA1-6) |
| Comércio | A tarifa no custo da rota; os itens proibidos fora da loja |
| Reputação | A faixa por facção; a doação pode ir para o Banco da Facção quando ela tiver (RE1-3) |
| Felicidade | A Felicidade da lei de imposto; o −5 de quem perdeu a votação |
| Notícias | A lei mudada: Regional |
| Cultura (próxima ficha) | As leis desejadas e o peso |
| Save | Leis, Banco, líder, legitimidade; o formato passa para a **versão 10** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `estadoDoTerritorio(numeroDoMapa)` | Regra 4.1 |
| `taxaDoImposto(cidade)` | Regra 4.2 |
| `cobrarTarifa(caravana, mapa)` | Regra 4.3 |
| `cunharEPagarVerba()` | Regra 4.4 |
| `fazerAssembleia(faccao)` | Regra 4.5 |
| `distanciaDaLei(lei, valorAtual, cultura)` | Regra 4.5, passo 1 |
| `leisAtuais`, `bancoDaFaccao`, `legitimidadeDoLider` | Parte 2 |

## 8. Testes

1. **Território:** (5,2) é Arcano; (5,3) é sem dono; (3,3) em disputa (parte 10.2).
2. **Imposto Baixo com o teto novo:** a vila com 10% por 64 dias: o Banco fica dentro da regra FE4-3 (nunca abaixo de 250; no máximo 50 abaixo do dia 1).
3. **O teste de 1 ano não muda** com 20% (o teto é o mesmo 80%) e a verba de 75 "de fora".
4. **Tarifa:** 80 Tábuas a 0,8 entrando na Autoritária (20%): 12,8 moedas.
5. **Verba sem dinheiro:** Banco da Facção com 1.100 e verba de 2.200: cada cidade recebe a metade.
6. **Assembleia:** o exemplo da parte 10.4 dá o mesmo resultado.
7. **Itens proibidos:** a loja da Religiosa não vende Fumo.

## 9. Fora desta versão

- Os 70% do imposto para o Banco da Facção (com a Guerra e o soldo).
- Exército, conscritos, Esquadrões, Caravana Militar (Guerra e Batalha).
- Dinastia, herdeiro, sucessão, guerra civil (Companheiros e Diálogo, Guerra).
- Relações diplomáticas (Diplomacia).
- O domínio do mapa em disputa (Guerra).
- O jogador membro por Rito de Admissão, vassalo, líder; a revolta; criar e acabar uma facção.
- A assembleia extraordinária por evento; o pensamento extremo entrando na pauta (FA1-5).
- A Estabilidade da facção (Felicidade e Estabilidade).

## 10. Simulação de mesa

### 10.1 Cenário

As 6 facções do primeiro dia, com as cidades do [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]. O jogador começa como Dono de vila em (5,2), território Arcano, com a cultura Tecnocrata.

### 10.2 O território

| Mapa | Vizinhos com cidade | Estado |
|---|---|---|
| (5,2) | (6,2) vilarejo Arcano | **Território Arcano** (bate com o documento) |
| (5,3) | (4,3) Vila Livre (não conta) | **Sem dono** (bate com o documento) |
| (3,3) | (3,2) vilarejo Guerreiro; (2,3) cidade Autoritária; (4,3) Vila Livre | **Em disputa** |
| (5,2), depois de o jogador fundar a vila nele | — | **Com cidade**, vinculada à Arcana |

### 10.3 O imposto Baixo e o teto do gasto

Uma cidade com renda bruta de **70 moedas por dia** (os salários):

| | Imposto 20%, teto 80% (hoje) | Imposto 10%, teto 80% (sem a FA1-2) | Imposto 10%, teto 90% (com a FA1-2) |
|---|---|---|---|
| Imposto para o Banco | 14 | 7 | 7 |
| Renda líquida da classe | 56 | 63 | 63 |
| Gasto da classe (volta ao Banco) | 56 | 56 | 63 |
| O Banco recebe de volta | 70 | **63** | 70 |
| Saldo do Banco por dia | 0 | **−7** | 0 |
| Em 1 ano | 0 | **−448** | 0 |

**Achado:** a conta da Economia só fecha com 20%. A Guerreira (imposto Baixo) teria as cidades com o Banco secando em 448 moedas por ano, e a classe juntando sem gastar. Com o **Alto** (30%), a classe teria teto de 56 e só 49 de renda: gastaria a poupança até zerar e ficaria sem reserva numa crise. **Resolvido (FA1-2):** o teto é 100% menos a taxa.

**E os 70% para a facção?** Com eles, o Banco da cidade receberia de volta só 70 − 0,7 × 14 = 60,2 por dia na taxa de 20%: −9,8 por dia, **−627 por ano** na vila, contra 300 de verba. **Resolvido (FA1-1):** o imposto fica todo com a cidade até a Guerra dar à facção um gasto que devolva o dinheiro (o soldo).

### 10.4 Uma assembleia

A Arcana tem 3 cidades (capital nível 4, cidade nível 3, vilarejo nível 2), todas da cultura Arcana, e a vila do jogador (Tecnocrata, nível 1). Votação: **Conselho (só Magos)**, que nesta versão é **só a capital** (FA1-4).

| Passo | O que acontece |
|---|---|
| Propostas | As 3 Arcanas têm as leis iguais ao desejo (distância 0): não propõem. A vila Tecnocrata: a maior distância é **Magia** (atual "privilégio dos magos", desejo "proibida", 1 passo × peso 5 = 5): propõe "privilégio → proibida" |
| Pauta | 1 lei: Magia |
| Votos | Capital Arcana: não (afasta do desejo). Só ela vota no Conselho |
| Resultado | **Não passa**. A vila perdeu: Felicidade −5 por 1 estação |

Com **maioria simples**, seriam 3 não contra 1 sim: também não passaria. Com uma facção de 3 cidades Tecnocratas conquistadas (fase D), passaria, e o líder Arcano (moderado, Agressividade 60, Sociabilidade 40) vetaria com 60% de chance.

### 10.5 Verificações

| Pergunta | Resultado |
|---|---|
| A assembleia muda alguma lei no começo? | **Não:** as cidades de cada facção são da mesma cultura, e as leis já são o desejo dela. **Achado:** com o extremo no desejo, as 6 facções votariam o **pensamento extremo** na primeira assembleia (dia 64), todas juntas, sem veto (o líder é da mesma cultura). **Resolvido (FA1-5):** o extremo não entra no desejo da assembleia; ele entra por evento (radicalização), como o documento pede ("o extremo fica para o jogo produzir") |
| O dinheiro das facções aguenta? | **Sim**, com a cunhagem de 140 por dia: 5 facções ganham cerca de 40 por estação; a Escravista perde 160 e zera em cerca de 13 anos. **Medir** no teste de 1 ano do mundo e ajustar a cunhagem |
| O teste de 1 ano muda? | **Não:** a vila do teste não tem facção, recebe os 75 "de fora", e o imposto é 20% |
| Duas cidades de facções diferentes na vizinhança | O mapa fica em disputa (10.2) e entra na chance de encontro da Viagem (150‰) |

### 10.6 Casos extremos

| Caso | O que acontece |
|---|---|
| Uma cidade proíbe o item que está no estoque dela | A loja para de vender; o estoque fica (o Crime decide o que fazer com ele) |
| A lei de imposto muda de Normal para Alto no meio do ano | Vale no dia seguinte; o teto muda junto (70%) |
| Banco da Facção a 0 | A verba não é paga; as cidades ficam só com a renda delas (a Felicidade cuida do resto quando a Estabilidade existir) |
| A vila do jogador muda de independente para vinculada | Passa a usar as leis e a receber a verba da facção; deixa de receber a verba "de fora" |
| Duas propostas com o mesmo número de cidades | A ordem das leis no arquivo |
| Um líder com Agressividade 100 e Sociabilidade 0 | Veto de 100% no moderado |

### 10.7 Decidido em modo automático (rodada FA1, seguir sugestão)

1. **FA1-1:** o imposto fica todo com a cidade até a Guerra; a parte da facção (70%) entra com o soldo.
2. **FA1-2:** o teto do gasto extra da classe é (100 − taxa do imposto)% da renda, e não 80% fixo.
3. **FA1-3:** as cidades sem facção recebem a verba de 5 por NPC "de fora"; as de facção, do Banco da Facção, que vive da cunhagem (140 por dia por Casa da Moeda) e da tarifa.
4. **FA1-4:** a votação por Conselho é só da capital até a dinastia existir.
5. **FA1-5:** o pensamento extremo não entra no desejo da assembleia; ele chega por evento.
6. **FA1-6:** o estado do território é derivado das cidades vizinhas e sai do `mapas.json` (muda a Viagem).
7. **FA1-7:** o ajuste de preço da facção vale para o jogador membro e as caravanas da facção.
