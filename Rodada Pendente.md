# Rodada pendente: FE2, os Blocos de Pedra acabam no 4º ano (marco 8)

**Pergunta enviada em 2026-10-19, aguardando resposta.** O marco 8 está programado, e o teste de 1 ano (64 dias) passa nas 10 sementes. Numa simulação de 4 anos (256 dias, sementes 1 e 2), a vila **falha no verão do 4º ano** (dia 217 e 225): o preço do Pão chega a 2,7 e, depois, o Pão acaba. O motivo vem da FL8-5 (a manutenção) com a FL6-5 (a vila não tem Pedreira); a resposta muda um número já decidido, por isso vira pergunta. Os dados continuam como estão até lá.

Responder no `Respostas.txt` pelo código (FE2-1). "Seguir sugestão" aceita a proposta.

**FE2-1. Os Blocos de Pedra da vila.**

- *Contexto:* a manutenção de cada estação gasta cerca de **14 Blocos de Pedra**: 11 nas construções da Cidade e 3 na Carpintaria do jogador, que os compra da loja da cidade. A vila começa com **150 Blocos** (FL6-5) e não tem Pedreira, então eles acabam por volta da 13ª estação (dia 193). Sem Bloco, a manutenção fica esperando material, as construções passam a produzir **75%** (FL8-5), e o Cozinheiro faz 12 Pães por dia para uma vila que come 15. O Pão cai 3 por dia: de 87 (dia 208) para 0 (dia 240), e o preço sobe até 3,4. O Banco continua bem (de 459 a 585 moedas), então o problema é só de material. O jogador não consegue fazer Blocos: a Pedreira é uma construção média, e os 3 lotes médios já estão ocupados.
- *Pergunta:* de onde vêm os Blocos depois do 3º ano? *Exemplo:* no dia 209, a manutenção da Fazenda pede 1 Bloco, a Cidade tem 0, e a Fazenda produz 75% dali em diante.
- *Opções:*
  - (a) **a vila começa com 300 Blocos** (cerca de 5 anos de manutenção; muda a FL6-5, de 150 para 300). Ganha: um número só, nenhuma regra nova. Perde: só adia o problema para o 6º ano; a Pedreira continua para as rodadas de conteúdo;
  - (b) a loja da cidade compra Blocos do Mercador viajante (o evento já existe: quando ele vem, a Cidade compra 20 Blocos a 110% com o dinheiro do Banco). Ganha: a pedra volta ao jogo por um evento já previsto. Perde: o evento é ocasional (1 ou 2 visitas por ano) e entra uma regra nova nos Eventos;
  - (c) a manutenção da Cidade não gasta Blocos (só Tábuas e horas); as obras do jogador seguem gastando. Ganha: o Bloco fica só para as obras. Perde: muda a V2-8 ("5% dos materiais do nível atual");
  - (d) deixar como está: o teste de 1 ano (a fatia) continua passando, mas a vila falha a partir do 4º ano.
- *Sugestão:* (a).
- *Conflitos:* muda a FL6-5 (150 para 300 Blocos no `vila_inicial.json`).

Depois da resposta: mudar o `vila_inicial.json` (e a parte 10 da [[Ficha Logica/Construcao|Construção]], se for a (b) ou a (c)), rodar de novo 10 sementes de 64 dias e 2 de 256 dias, e anotar a rodada na parte 8 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]].
