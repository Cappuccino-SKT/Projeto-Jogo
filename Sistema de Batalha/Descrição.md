> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Batalha (Mapa de Guerra)

**Descrição geral**

Define a **batalha em tempo real entre exércitos** no Mapa de Guerra, inspirada no Total War: o jogador comanda **Esquadrões**, não soldados. A batalha só acontece quando o jogador participa; as outras são resolvidas pelo cálculo rápido, que usa as mesmas grandezas (ver [[Sistema de Guerra/Descrição|Sistema de Guerra]]).

Já estava decidido e vale como base:

- O Mapa de Guerra é temporário (criado e destruído sob demanda) e herda o terreno do Mapa Local onde a batalha começou ([[Sistema de Mapa/Descrição|Mapa]]).
- 1 Esquadrão = 100 soldados; soldados não são NPCs; Moral e Disciplina são do Esquadrão ([[Sistema de Facção/Descrição|Facção]]).
- Arma × armadura com até ±25% e boneco por camadas com animações por tipo de arma ([[Sistema de Combate/Descrição|Combate]]).
- No Mapa de Guerra, a velocidade máxima é 4x ([[Sistema de Tempo/Descrição|Tempo]]).

Os valores são iniciais, ficam em `esquadroes.json` e `batalha.json`, e serão ajustados nos testes.

---

## Tipos de Esquadrão

São **9 tipos fixos**. A qualidade vem do **Rank** do equipamento entregue no kit, não de unidades diferentes.

| Tipo | Kit | Papel | Era |
|---|---|---|---|
| Infantaria Leve | Espada ou Lança Curta, armadura Leve (couro), escudo | Barata e versátil | 1 |
| Infantaria Pesada | Espada Longa, Machado ou Clava, armadura Pesada (placas), escudo | Segura a linha | 1 |
| Lanceiros | Lança Longa, armadura Média (malha) | Contra cavalaria | 1 |
| Arqueiros | Arco, armadura Leve, Flecha | Tiro de longe, barato | 1 |
| Besteiros | Besta, armadura Média, Virote | Tiro forte contra pesados | 1 |
| Cavalaria | Cavalo, Sela, Lança ou Espada, armadura Média | Carga e perseguição | 1 |
| Atiradores | Arcabuz ou Mosquete, armadura Leve, Cartucho | Tiro devastador e lento | 2 |
| Artilharia | Bombarda ou Canhão, Bala de Canhão | Brecha e dano em área | 2 |
| Magos | Cajado, Pedra de Mana | Dano em área ou suporte | 1 |

Mais um tipo especial, só de defesa: a **Milícia** (guardas e civis armados no cerco, equipamento leve; ver [[Sistema de Guerra/Descrição|Guerra]]).

- **Kit:** 1 item de cada peça equipa o Esquadrão inteiro (escala do Fator de Escala: 1 item para 100 soldados). O Rank dos itens define Ataque e Defesa ([[Sistema de Itens/Descrição|Itens]]).
- **Bônus de cultura** (um tipo cada):

| Cultura | Bônus |
|---|---|
| Guerreira | Cavalaria +10% de Ataque e mais barata |
| Autoritária | Lanceiros com +10 de Disciplina |
| Tecnocrata | Atiradores e Artilharia uma era antes (Era 1) |
| Arcana | Magos (ver "Esquadrão de Magos") |
| Religiosa | +10 de Moral em todos os tipos |
| Escravista | Infantaria de escravos: metade do soldo, −15 de Moral |

- **Arte:** nenhuma unidade nova. Usa o boneco por camadas e as animações por arma que já existem; a cultura muda só a paleta ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]).
- **Cavalaria:** existe só no Mapa de Guerra; no combate local a montaria continua fora da luta.

## Números do Esquadrão

**Atributos:** Tipo, Soldados (0 a 100), Ataque, Defesa, Alcance, Velocidade, Moral (0 a 100), Disciplina (0 a 100), Experiência (0 a 5), Munição, Mana (só Magos), Formação, Comandante e Cidade de origem.

- Ataque e Defesa saem do kit (dano médio da arma e Defesa somada da armadura), como no Combate local, com a mesma tabela de ±25% de arma × armadura.
- Cada soldado tem **10 de Vida**; o dano num Esquadrão derruba soldados.
- **Experiência:** cada nível dá +5% de Ataque e Defesa e +5 de Disciplina; sobe 1 nível a cada 2 batalhas sobrevividas.

Valores iniciais com equipamento Rank 1 (Alcance em relação ao Arco = 1):

| Tipo | Ataque | Defesa | Alcance | Velocidade | Observação |
|---|---|---|---|---|---|
| Infantaria Leve | 8 | 4 | — | 1,0 | |
| Infantaria Pesada | 10 | 8 | — | 0,8 | |
| Lanceiros | 8 | 6 | — | 0,9 | +50% contra Cavalaria |
| Arqueiros | 6 | 2 | 1,0 | 1,0 | |
| Besteiros | 9 | 5 | 0,8 | 0,9 | |
| Cavalaria | 10 | 6 | — | 2,0 | +50% na carga |
| Atiradores | 14 | 2 | 0,7 | 0,9 | Recarga longa |
| Artilharia | 20 em área | 1 | 3,0 | 0,4 | Não anda enquanto atira |
| Magos | 12 em área | 2 | 1,0 | 1,0 | Gasta Mana |
| Milícia **[Proposta]** | 5 | 3 | — | 1,0 | Só na defesa da própria cidade |

## Escala da batalha e tamanho visual

**Decisão do autor:** o objetivo é ter batalhas grandes de verdade, com até **4.000 soldados** no Mapa de Guerra (2.000 por lado), como no Total War, e deixar o **jogador escolher o tamanho** das unidades.

- **A simulação é sempre por Esquadrão de 100 soldados.** É isso que mantém a economia, a População e o Fator de Escala iguais para todos (1 Esquadrão = 1 NPC alistado), e o cálculo rápido coerente com a batalha.
- **O que o jogador escolhe é quantos bonecos representam cada Esquadrão:**

| Tamanho das unidades | Bonecos por Esquadrão | 1 boneco = | Máximo na tela (20 Esquadrões × 2 lados) |
|---|---|---|---|
| Pequeno | 20 | 5 soldados | 800 |
| Médio | 50 | 2 soldados | 2.000 |
| Grande | 100 | 1 soldado | **4.000** |

- **Onde se escolhe:** na criação do Novo Jogo, como no Total War. **[Proposta]** Como a escolha só muda o que aparece na batalha, ela pode ser trocada depois nas Configurações (fora de uma batalha), sem afetar o save.
- **Os bonecos:** ocupam lugares na formação, animam ataque e queda; cai 1 boneco a cada "1 boneco =" soldados perdidos. O caminho é calculado por Esquadrão, e cada boneco só segue o seu lugar na formação.
- **Primeira versão:** o tamanho Pequeno é o garantido em qualquer PC; o Médio e o Grande entram conforme os **testes de desempenho**. O PC mínimo roda o Pequeno.
- **[Refinar depois]** No tamanho Grande, cada soldado pode ganhar o **próprio combate** (Vida individual e golpes um a um, como no Total War), desde que o resultado continue igual ao do cálculo por Esquadrão. Decidir depois dos testes de desempenho.
- **Limite no campo:** 20 Esquadrões por lado ao mesmo tempo; o resto entra como reforço (ver "Reforços").

**Por que não mudar o número real de soldados por Esquadrão?** Porque 1 Esquadrão = 1 NPC alistado = Fator de Escala soldados. Mudar isso por partida mudaria o tamanho dos exércitos, o soldo, a Ração e o equilíbrio entre facções. O tamanho visual dá o espetáculo sem mexer na economia.

## Moral

- **Início:** 70 + General (até +10) + Comandante + cultura; −20 se o exército está sem suprimento.
- **Cai:**

| Causa | Moral |
|---|---|
| Soldados perdidos | −1 a cada 2% |
| Atacado pelo flanco | −10 no impacto |
| Atacado pela retaguarda | −20 no impacto |
| General derrotado (Comitiva fugiu ou caiu) | −25 em todo o exército |
| Esquadrão aliado vizinho fugindo | −5 |
| Atingido por artilharia ou magia | −3 por golpe (−5 por Magos de Combate) |

- **Sobe:** inimigo fugindo perto (+5); General por perto (+1 por segundo, até a Moral inicial); vencendo o corpo a corpo (+1 a cada poucos segundos).
- **Disciplina** reduz todas as quedas: queda × (1 − Disciplina ÷ 200). Disciplina 100 = metade da queda.
- **Faixas:**

| Faixa | Moral | Efeito |
|---|---|---|
| Firme | 50+ | Normal |
| Abalado | 25 a 49 | −10% de Ataque |
| Fugindo | abaixo de 25 | Corre para a borda; reagrupa se a Moral voltar a 40 sem perseguição |
| Destroçado | fugiu 2 vezes | Sai da batalha |

## Formações, flanco e vantagens

| Formação | Efeito |
|---|---|
| Linha (padrão) | Normal |
| Coluna | +30% de velocidade, −30% de Defesa |
| Muralha de Escudos | Parado; anula a carga; −50% de velocidade |
| Dispersa | −50% do dano de tiro e artilharia recebido; −20% no corpo a corpo |
| Cunha (só Cavalaria) | +25% na carga |

- **Flanco:** +25% de dano e −10 de Moral no alvo. **Retaguarda:** +50% e −20.
- **Carga:** a Cavalaria tem +50% no primeiro impacto; Lanceiros parados anulam a carga.
- **Ciclo de vantagens:** Lanceiros vencem Cavalaria; Cavalaria vence as tropas de tiro (Arqueiros, Besteiros, Atiradores, Artilharia); tiro vence a infantaria lenta; Infantaria Pesada vence Lanceiros.
- A tabela de ±25% de arma × armadura do Combate também vale aqui.

## Terreno e campo de batalha

A batalha acontece numa **cópia simplificada do Mapa Local** onde ela começou (128 × 128 tiles): sem NPCs e sem itens, com as construções como obstáculos. Não exige arte nova, e a geografia é exatamente a do mundo.

| Terreno (lido dos tiles) | Efeito |
|---|---|
| Floresta | −30% do dano de tiro recebido; −30% de velocidade; sem carga de Cavalaria |
| Rio | Só se atravessa em ponte ou vau |
| Pântano | −50% de velocidade |
| Estrada | +20% de velocidade |
| Construções | Obstáculo e cobertura |
| **Elevação** (colina) | De cima: +15% de alcance e de dano no tiro; +10% no corpo a corpo morro abaixo |

- **Elevação:** o autor incluiu a **7ª camada do Tiled**, "Elevação", pintada só nas colinas. Um mapa sem ela é plano (ver [[Pipeline de Arte e Conteúdo/Descrição|Pipeline de Arte e Conteúdo]]).
- **Clima e hora:** o clima do dia e a fase do dia no início valem a batalha toda (o mundo está parado). Chuva: −15% de alcance de tiro e −30% de dano dos Atiradores; Tempestade: −30% de alcance e os Atiradores não atiram; Neve: −20% de velocidade; Névoa: −40% de visão e de alcance; à noite, visão −30% (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Posicionamento:** o atacante entra pela borda de onde veio (a direção da viagem); o defensor se arma no centro ou no lado oposto.

## Artilharia e pólvora

- **Esquadrão de Artilharia** = 100 artilheiros com a Bombarda (Era 2) ou o Canhão (Era 3) no kit; o Canhão de Aço Mágico (Era 4) é a versão melhor.
- Alcance 3× o do Arco; **dano em área**; −3 de Moral por tiro nos atingidos.
- **Brecha na Muralha:** Paliçada em 20 s de tiro, Pedra em 60 s, Bastião em 120 s.
- Não anda enquanto atira; muito lenta; quase sem defesa no corpo a corpo.
- 1 Bala de Canhão por batalha, com limite de **30 tiros**.
- Artilharia e Atiradores a partir da **Era 2**; a Tecnocrata pode tê-los na Era 1 por tecnologia inicial ([[Sistema de Tecnologia/Descrição|Tecnologia]]). O Bastião (Era 3) é a resposta das muralhas.

## Esquadrão de Magos

- **1 Esquadrão de Magos = 1 NPC Mago alistado** (100 magos, pela escala).
- **Limite:** até 10% dos NPCs Magos da facção, e no máximo 1 para cada 4 Esquadrões comuns no exército. *Exemplo:* a Arcana tem cerca de 66 NPCs Magos, logo até 6 Esquadrões de Magos.
- **Custo:** 3× o soldo; 1 Pedra de Mana por batalha.
- **Pela escola do Mago alistado** ([[Sistema de Magia/Descrição|Magia]]):

| Escola | No Mapa de Guerra |
|---|---|
| Combate | Dano em área de médio alcance (artilharia leve que anda); −5 de Moral nos atingidos; abre brecha devagar |
| Suporte | Devolve 1 soldado a cada 10 s aos Esquadrões próximos (feridos voltam); +10 de Moral em volta |
| Ofício | Não vai à guerra |

- **Mana do Esquadrão:** gasta por feitiço e volta devagar; a Pedra de Mana da Caravana enche.
- **Feitiços de área acertam aliados** (regra do Combate), o que pede pausa e cuidado.
- **Leis:** só existe se a lei de Magia permitir; a Tecnocrata (magia proibida) não forma. **[Proposta]** Com a lei "restrita" (Religiosa), só Magos de Suporte.

## General, Comitiva e Comandantes

- **Comitiva:** o General entra com a guarda pessoal. Se o General é o jogador, a Comitiva é o **grupo controlado de verdade**: cada boneco é um companheiro, com a Vida e o dano reais do Combate local. É o Esquadrão "herói", como a guarda do general no Total War.
- **Aura do General:** +10 de Moral num raio. Se a Comitiva foge ou cai, −25 em todo o exército.
- Companheiros da Comitiva que caem seguem as regras do Combate: Inconsciente, morte permanente ou captura ([[Sistema de Combate/Descrição|Combate]]).
- **Comandante de Esquadrão** (companheiro ou nobre): **+1% de Ataque e Defesa e +1 de Moral a cada 10 pontos** da habilidade principal (Corpo a Corpo para infantaria e cavalaria; Longo Alcance para tiro). *Exemplo:* Corpo a Corpo 60 → +6% e +6 de Moral. Se o Esquadrão é destruído: 50% de chance de o Comandante ficar ferido, 30% de ser capturado e 20% de morrer. Isso resolve o bônus do cargo que estava em aberto em [[Sistema de Companheiros e Diálogo/Descrição|Companheiros e Diálogo]].
- **Jogador sem ser General:** controla só a Comitiva e os Esquadrões dele (próprios ou contratados); a IA comanda o resto do exército aliado.

## Controles, tempo e telas

**Controles** no estilo Total War:

- clique ou caixa para selecionar; Ctrl+1 a 9 para grupos;
- botão direito para mover ou atacar; arrastar com o botão direito define a largura e a frente da linha;
- botões de formação;
- **pausa ativa** (espaço) com ordens enfileiradas; velocidades 1x, 2x e 4x.

**O mundo para durante a batalha.** Uma batalha de 15 minutos reais a 1x seria quase um dia de jogo (16 min = 24 h). Por isso o relógio do mundo **congela** no Mapa de Guerra e, no fim da batalha, avança **2 horas fixas**.

**Telas** (ver [[Interface/Descrição|Interface e Controles]]):

1. **Pré-batalha:** forças comparadas, terreno e previsão; botões **Lutar**, **Resolver automaticamente** (cálculo rápido) e **Recuar** (o exército perde 10% e foge 1 mapa).
2. **Posicionamento:** zona de deploy, formações e ordem inicial.
3. **Batalha:** cartões dos Esquadrões (Soldados, Moral, Munição), minimapa, barra de força dos lados; o HUD é personalizável como o resto do jogo.
4. **Resultado:** baixas, prisioneiros, saque, Placar de Guerra e Reputação.

## Fim da batalha

- **Vitória:** todos os Esquadrões inimigos fugindo ou destruídos. Quem manda **recuar** sai pela borda.
- **Tempo limite:** 30 minutos de batalha; se acabar, vence o defensor.
- **Perseguição:** se o vencedor tem Cavalaria, quem fugiu perde mais 20% dos soldados que restavam.
- **Baixas:** dos soldados derrubados, **40% morrem, 40% ficam feridos e 20% são capturados** pelo vencedor.
  - Feridos do vencedor voltam em 16 dias (8 com Hospital na cidade de origem ou com Magos de Suporte no exército).
  - Feridos do perdedor viram prisioneiros.
- **Prisioneiros comuns:** escravizar (se a lei permite; **100 prisioneiros = 1 NPC Escravo**), libertar (+Reputação) ou trocar no tratado de paz. Líderes e NPCs Importantes viram prisioneiros individuais ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]).
- **Esquadrão com menos de 20 soldados** se desfaz; os restantes voltam à cidade de origem ou se juntam a outro do mesmo tipo.
- **Mortos** reduzem a população da cidade de origem (baixas permanentes, já decidido).
- A batalha vencida dá pontos no Placar de Guerra ([[Sistema de Guerra/Descrição|Guerra]]).

## Batalha de cerco

Versão simples na primeira versão:

- O defensor fica atrás da **Muralha** (tiles com colisão), que tem **Portão** e trechos.
- O atacante entra pelo **Portão**, derrubado com o Aríete (a infantaria empurra por 60 s, sob fogo), ou pelas **brechas** abertas pela Artilharia ou pelos Magos de Combate.
- **Tropas de tiro defensoras** ficam em posições marcadas na Muralha: +30% de Defesa e +20% de alcance.
- **Vitória do atacante:** controlar a **praça central** por 60 s (ponto marcado no Tiled) ou afugentar a guarnição.
- A **Milícia** da cidade luta junto com a guarnição.
- **Escadas e torres de cerco** ficam para o futuro.
- **Arte:** tileset de Muralha com Portão (aberto, fechado, destruído) e trecho (inteiro, em brecha).

## Reforços e vários exércitos

- Exércitos amigos ou inimigos num **Mapa Local vizinho** entram como **reforço** pela borda correspondente, com atraso de **1 minuto de batalha por hora de viagem** até ali.
- A **guarnição** da cidade também luta.
- Acima de 20 Esquadrões por lado, os extras entram quando algum sai do campo.
- **Aliados de outras facções** lutam do mesmo lado, controlados pela IA.

---

## Parte técnica

- **Dados:** `esquadroes.json` (tipos, kits, valores base e bônus de cultura) e `batalha.json` (Moral, formações, terreno, flanco, tempos de brecha, divisão das baixas e tamanhos visuais).
- **Simulação por Esquadrão** no ECS, no passo fixo: alvo, distância, dano, Moral. Cada Esquadrão reavalia o alvo a cada poucos ticks; grade espacial para achar vizinhos.
- **Bonecos visuais:** posições de formação calculadas a partir do Esquadrão; desenho em lote (todos os bonecos com a mesma folha de sprites num só comando de desenho), o que permite milhares na tela.
- **Caminho:** por Esquadrão, na grade do Mapa Local copiado; os bonecos só seguem os lugares da formação.
- **Mapa de Guerra:** gerado a partir do arquivo do Tiled do Mapa Local (camadas de colisão, terreno e Elevação) e destruído no fim. Não fica no save; só o resultado volta para o mundo.
- **Simulação sem tela:** a mesma batalha pode rodar sem desenhar nada, para calibrar o cálculo rápido.
- **Aleatoriedade com semente.**
- **Depuração:** painel (Dear ImGui) com Moral, dano e alvo de cada Esquadrão.
- **Fatia vertical:** não usa. Entra depois da guerra por cálculo rápido, como um marco próprio.

## Ligação com outros sistemas

| Sistema | O que a Batalha fornece ou recebe |
|---|---|
| Guerra | Resultado da batalha (baixas, prisioneiros, Placar); cálculo rápido com as mesmas grandezas; cerco e reforços |
| Facção | Esquadrões, Quartel, soldo; baixas reduzem a população |
| Mapa | Mapa de Guerra a partir do Mapa Local; Elevação; praça central e Muralha marcadas no Tiled |
| Combate | Fórmula de dano, arma × armadura, Comitiva com as regras do combate local |
| Itens | Kits por tipo; Rank do equipamento; artilharia (Bombarda e Canhão) e munições |
| Magia | Esquadrão de Magos (Combate e Suporte); lei de Magia |
| Companheiros | Comitiva e Comandante de Esquadrão |
| Cultura | Bônus de um tipo de Esquadrão por cultura |
| Tecnologia | Era dos tipos (pólvora na Era 2); Bastião na Era 3 |
| Crime e Justiça | Prisioneiros comuns e Importantes; escravização em massa pela lei; venda de prisioneiros (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) |
| Tempo | Mundo parado durante a batalha; até 4x |
| Interface | Pré-batalha, posicionamento, cartões de Esquadrão, resultado; HUD personalizável |
| Pipeline | Bonecos por camadas e paletas por cultura; 7ª camada Elevação; tileset de Muralha |
| Clima e Estações | Clima e fase do dia no início valem a batalha toda (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |

## Perguntas em aberto

1. **Valores a testar:** atributos dos tipos, Moral, flanco, terreno, divisão das baixas, tempo limite.
2. **Desempenho:** até onde o PC alvo aguenta os tamanhos Médio e Grande.
3. **Combate individual no tamanho Grande** (ver "Escala da batalha").
4. **Escadas e torres de cerco**, para uma versão futura.
