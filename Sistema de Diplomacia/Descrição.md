> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Diplomacia

**Descrição geral**

Define as **relações entre as facções**: o valor de relação, o estado (paz, guerra ou trégua), os tratados, a vassalagem, os casamentos e os reféns, como a IA decide, o que o jogador pode fazer e o freio contra quem cresce demais. Antes, isso vivia dentro do [[Sistema de Facção/Descrição|Sistema de Facção]]; a guerra em si está no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

Já estava decidido e vale como base:

- **Relação** de −100 a +100 para cada par de facções em contato; só os pares com contato (fronteira ou rota de comércio) são guardados ([[Sistema de Facção/Descrição|Facção]]).
- A relação muda com a fronteira, a afinidade das culturas, a Personalidade dos líderes (Agressividade, Ganância, Lealdade) e os eventos.
- Relações iniciais, afinidades e guerras e acordos sorteados no primeiro dia ([[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]]).
- Casamento entre dinastias dá +20 de relação ([[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]]).
- Acordo comercial zera a Tarifa; guerra e embargo bloqueiam o comércio ([[Sistema de Comércio/Descrição|Comércio]]).

---

## Três camadas

A antiga lista de 6 estados exclusivos (Guerra, Hostil, Neutro, Comércio, Aliança, Vassalagem) foi separada em três camadas, para que duas facções possam, por exemplo, ter acordo comercial **e** aliança ao mesmo tempo.

1. **Estado** (um só por par): **Paz**, **Guerra** ou **Trégua** (1 ano depois de um tratado de paz).
2. **Tratados** (vários ao mesmo tempo): Acordo Comercial, Embargo, Passagem Militar, Não Agressão, Aliança Defensiva, Aliança Total, Vassalagem e Casamento.
3. **Atitude** (faixa da relação, só para leitura):

| Atitude | Relação |
|---|---|
| Hostil | −100 a −31 |
| Fria | −30 a −1 |
| Neutra | 0 a 29 |
| Amigável | 30 a 69 |
| Leal | 70 ou mais |

Com atitude **Hostil**, a facção pode **fechar a fronteira** (bloqueia caravanas, migração e viajantes da outra) e a guerra sem motivo custa a metade.

## Tratados

| Tratado | Efeito | Exige | Duração |
|---|---|---|---|
| Acordo Comercial | Tarifa 0 entre os dois | Relação +10 | Até cancelar (aviso de 16 dias) |
| Embargo (de um lado só) | Proíbe as caravanas do outro no território | — | Até cancelar |
| Passagem Militar | Exércitos atravessam o território sem guerra | Relação +20 | 1 ano |
| Não Agressão | Proíbe declarar guerra | Relação 0 | 1 ano |
| Aliança Defensiva | Entra na guerra se o aliado for atacado | Relação +40 | Até cancelar |
| Aliança Total | Entra também nas guerras de ataque do aliado | Relação +60 | Até cancelar |
| Vassalagem | Ver "Vassalagem" | Tratado de paz ou pedido | Até a independência ou a integração |
| Casamento | Ver "Casamento e reféns" | Relação 0 | Enquanto o casal viver |

- **[Proposta]** Assinar um tratado dá +5 de relação; cancelar um tratado ainda válido dá −10.
- **Chamado de aliado:** quando um aliado entra em guerra, ele chama; quem atende entra na guerra com o motivo "Chamado de aliado" ([[Sistema de Guerra/Descrição|Guerra]]).

### Confiabilidade

Nota nova de cada facção, de **0 a 100**, que começa em **70**.

| Ação | Confiabilidade |
|---|---|
| Quebrar tratado | −20 |
| Declarar guerra sem motivo | −20 |
| Ignorar chamado de aliado | −15 |
| Quebrar trégua | −30 |
| Cada estação sem quebras | +5 |

A IA **não faz tratado** com quem tem Confiabilidade abaixo de 30.

## Vassalagem

| | Membro | Vassalo |
|---|---|---|
| O que é | Cidade dentro da facção | Facção ou cidade independente subordinada a um suserano |
| Leis | Todas as da facção (herança total) | Próprias |
| Imposto | Normal, para o Banco da Facção | Tributo de **15% do imposto** ao Banco do suserano |
| Guerra | Segue a facção | **Chamado às armas:** 30% dos Esquadrões ou o valor em dinheiro; não declara guerra nem faz aliança sozinho |
| Ganha | Proteção e cargos | Aliança Defensiva automática e Acordo Comercial com o suserano |

- **Como se vira vassalo:** por tratado de paz (custo 60 no Placar) ou por pedido do próprio vassalo.
- **Saída:** declarar independência (guerra com o motivo "Independência"), mais fácil se o suserano está em outra guerra ou com Cansaço alto.
- **Integração:** depois de **4 anos** com relação **+50 ou mais**, o suserano pode absorver o vassalo em paz; as cidades dele viram membros.
- **Para o jogador:** o dono de uma Vila Livre ou de uma cidade independente pode **pedir vassalagem** a uma facção e ganhar proteção sem perder as próprias leis. Recusar o chamado às armas dá −10 de reputação ([[Sistema de Reputação/Descrição|Reputação]]).

## Casamento e reféns

**Casamento entre dinastias**

- Proposto com relação 0 ou mais, se as duas dinastias têm adultos solteiros.
- **Efeitos:** +20 de relação (já decidido); **Não Agressão** enquanto os dois viverem; e **reivindicação**: se a outra dinastia fica sem herdeiro, o filho do casal pode reclamar o trono (motivo de guerra "Reivindicação").
- As dinastias da IA propõem casamentos por estação ([[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]]).

**Reféns** (versão simples)

- Um **nobre** entregue como garantia num tratado de paz ou de vassalagem. Vive **2 anos** na capital da outra facção e depois volta.
- Se quem entregou o refém **quebrar o tratado**, o refém pode ser preso ou executado. A execução dá à facção do refém o motivo de guerra "Ofensa".
- Abre missões para o jogador: resgatar ou escoltar um refém ([[Objetivo e Progressão/Descrição|Objetivo e Progressão]]).

## Como a IA decide

**Frequência:** a cada **4 dias**, avalia guerra e paz; a cada **estação**, tratados, casamentos e alianças. Funciona em qualquer LOD (são poucas facções; o custo é desprezível).

**Vontade de guerra** contra a facção X; declara guerra **acima de 60**:

| Fator | Pontos |
|---|---|
| Força própria ÷ força de X | 1,0 = 0; 1,5 = +25; 2,0 = +50 (abaixo de 1, negativo) |
| Agressividade do líder | até +30 |
| Ganância × riqueza de X | até +20 |
| Motivo | forte +20; nenhum −20 |
| Relação | −Relação ÷ 4 (relação −60 → +15) |
| Cultura | Guerreira +15; Religiosa +15 contra culturas hostis |
| Guerras em andamento | −20 cada |
| Cansaço de guerra | −Cansaço ÷ 2 |
| Coalizão contra facção ameaçadora | +30 (ver "Freio contra quem cresce demais") |
| Inverno | −15 (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |

*Exemplo:* Guerreira (20 Esquadrões) contra Autoritária (16): força 1,25 → +12; Agressividade alta +25; Ganância média e celeiro rico +10; motivo (mapa em disputa) +20; relação −55 → +14; cultura +15. Total **96**: declara guerra.

- **Paz:** aceita os termos se o Placar contra ela cobre o custo, ou se o Cansaço dela é alto (ver [[Sistema de Guerra/Descrição|Guerra]]).
- **Lealdade** do líder: respeita tratados e atende chamados; abaixo de 30, pode trair.
- **Sociabilidade:** mais propostas de tratado e casamento.
- **[Proposta]** **Alianças:** a IA procura Aliança Defensiva com facções afins quando um vizinho tem força 1,5× maior que a dela.
- Cada decisão guarda o **"Por quê?"**, para as [[Sistema de Notícias/Descrição|Notícias]] e para o painel de depuração.
- **Bênção e Anátema:** o Sumo Sacerdote muda as relações das facções Religiosas (+10 ou −20); o Anátema libera a "Guerra santa" (ver [[Sistema de Religião/Descrição|Religião e Fé]]).

## A diplomacia do jogador

| Papel | O que pode fazer |
|---|---|
| Líder de facção | Acesso total ao painel de Diplomacia |
| Dono de cidade independente | Acordo Comercial, Passagem Militar, Não Agressão e pedido de vassalagem |
| Membro (nobre) | Sem diplomacia de facção; influencia pela assembleia e pela Opinião do líder |

- **Painel de Diplomacia:** lista de facções com relação, atitude, tratados, Confiabilidade e o Placar das guerras. Cada proposta mostra a **chance de aceite e o porquê** ("+20 pela relação; −30 porque vocês estão em guerra com o aliado deles").
- **Bônus pessoal na chance:** (Carisma − 50) ÷ 5 + Opinião do líder ÷ 5.
- As propostas são **instantâneas** na primeira versão (sem mensageiro viajando); as **notícias** da guerra chegam com atraso (ver [[Sistema de Notícias/Descrição|Notícias]]). As facções da IA sabem tudo na hora, uma simplificação da primeira versão.

## Freio contra quem cresce demais

Com 48 mapas e 6 facções, uma facção (ou o jogador) poderia engolir o mundo depressa. Dois freios:

- **Ameaça e Coalizão:** uma facção é **ameaçadora** se tem PIB acima de **1,6× a média** ou conquistou **3 ou mais cidades em 2 anos**. As facções em contato ganham +30 na decisão de formar uma **Coalizão**: uma Aliança Defensiva conjunta contra ela.
- **Superextensão:** cada cidade conquistada há menos de 1 ano dá **−3 de Estabilidade** à facção ([[Sistema de Felicidade e Estabilidade/Descrição|Felicidade e Estabilidade]]).
- Vale também para o jogador líder.

---

## Parte técnica

- **Dados:** `diplomacia.json` com os tratados (exigências, duração, efeitos), as faixas de atitude, a Confiabilidade e os pesos da IA.
- **Por par em contato:** relação, estado, data da trégua, lista de tratados com data de início e de fim, reféns.
- **Por facção:** Confiabilidade, Ameaça e o "Por quê?" da última decisão.
- **Frequência:** relações e tratados a cada 4 dias e por estação; nada roda por tick.
- **Save:** pares em contato, tratados, tréguas, reféns, Confiabilidade e coalizões.
- **Depuração:** painel (Dear ImGui) que mostra a vontade de guerra de cada facção contra cada outra, com os fatores.
- **Fatia vertical:** não usa (1 cidade, sem facções).

## Ligação com outros sistemas

| Sistema | O que a Diplomacia fornece ou recebe |
|---|---|
| Facção | Relação, Personalidade do líder, dinastia; o estado "Vassalo" do jogador |
| Guerra | Estados de Guerra e Trégua; motivos; chamado de aliado; tratado de paz e vassalagem pelo Placar |
| Comércio | Acordo Comercial (Tarifa 0), Embargo, fronteira fechada |
| Viagem | Passagem Militar; fronteira fechada bloqueia rotas |
| População | Guerra e fronteira fechada bloqueiam a migração |
| Companheiros e Diálogo | Casamento entre dinastias, herdeiros e reivindicação |
| Reputação | Opinião do líder e reputação do jogador na chance de aceite; recusa do chamado às armas |
| Cultura | Afinidades; Guerreira e Religiosa mais propensas à guerra |
| Felicidade e Estabilidade | Superextensão; Cansaço de guerra |
| Objetivo e Progressão | Missões de reféns e de diplomacia |
| Eventos | Declarações, tratados, quebras e casamentos como eventos com causa registrada |
| Interface | Painel de Diplomacia, chance de aceite com o porquê, lente Política |
| Religião | Bênção e Anátema do Sumo Sacerdote (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| Clima e Estações | Vontade de guerra −15 no inverno (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Notícias | Tratados e guerras viram notícia; a IA sabe na hora (ver [[Sistema de Notícias/Descrição\|Notícias]]) |

## Perguntas em aberto

1. **Valores a testar:** pesos da vontade de guerra, limiar de declaração (60), Confiabilidade, exigências dos tratados.
2. **Mensageiros:** no futuro, as propostas podem levar tempo de viagem (ligado às Notícias).
3. **Coalizão:** quando ela se desfaz (por exemplo, quando a Ameaça cai).
