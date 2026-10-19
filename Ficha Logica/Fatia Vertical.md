> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> **Ficha técnica lógica da fatia vertical**, primeira versão (2026-10-17), no formato decidido na rodada V5-9. Traduz os sistemas em **estruturas, dados e regras** que o código vai seguir, em linguagem simples e sem código de verdade. As escolhas novas foram decididas na rodada FT1 (parte 8).

# Ficha Lógica: Fatia Vertical

**Descrição geral**

A **fatia vertical** é a primeira versão jogável: uma vila de **nível 1**, com **15 NPCs reais Civis** e um personagem controlado pelo jogador, sem guerra, facções, crime nem magia ([[Arquitetura/Descrição|Arquitetura]]). Ela existe para provar a parte mais arriscada do projeto: **a simulação com LOD e o ciclo do dinheiro**.

Esta ficha é o mapa para programar. Cada parte responde a uma pergunta:

| Parte | Responde |
|---|---|
| 1. Entidades e componentes | O que existe no jogo e que dados cada coisa guarda? |
| 2. Arquivos de dados | O que o jogo lê ao abrir? |
| 3. O ciclo do tempo | O que acontece a cada tick, hora, dia e estação, e em que ordem? |
| 4. As regras | Como cada conta é feita, passo a passo? |
| 5. Save | O que é gravado? |
| 6. Testes | Como saber se funciona? |
| 7. Marcos | Em que ordem programar? |

### O que entra e o que fica de fora

| Entra | Fica de fora |
|---|---|
| 1 Mapa Local em LOD 0; 1 vila de nível 1 | Mapa Mundial, viagem, outras cidades |
| 15 NPCs Civis com rotina e Utilidade simples | Outras classes, crianças, migração |
| Fazenda, Posto de Coleta, Cozinheiro, Carpinteiro, Mercado, Construtor, Casas | Outras construções, Tecnologia, eras |
| Grão, Água, Tora, Pão, Tábua, Móveis de Madeira | Itens com Rank, armas, armaduras |
| Inventário da cidade, Mercado Local, salários, consumo, estabilizador | Mercado da Facção e Mundial (o preço mundial é só o preço base) |
| Felicidade (4 fatores) | Leis, Religião, Criminalidade, guerra |
| 12 eventos (V3-6), estações e clima do dia | Narradores (a fatia usa o sorteio simples) |
| 1 personagem do jogador, com 100 moedas e uma Carpintaria própria (FT1-1) | Companheiros, combate |
| Salvar e carregar; teste de 1 ano sem tela | Interface personalizável (só o HUD básico) |

---

## 1. Entidades e componentes

O jogo usa **entidades e componentes** ([[Arquitetura/Descrição|Arquitetura]], ECS):

- **Entidade:** um número que identifica uma coisa (um NPC, uma construção).
- **Componente:** uma ficha de dados presa a uma entidade. Um NPC é uma entidade com os componentes Posição, Rotina, Emprego e outros.
- **Sistema:** a regra que roda sobre todas as entidades que têm certos componentes. O sistema de Movimento, por exemplo, anda todos os NPCs que têm Posição e Caminho.

**Tipos dos campos:**

| Tipo | O que é | Exemplo |
|---|---|---|
| Inteiro | Número sem vírgula | Nível 1; quantidade 12 |
| Dinheiro | Inteiro em **décimos de moeda** (V2-1) | 17 = 1,7 moeda |
| Decimal | Número com vírgula, só para frações e fatores, nunca para dinheiro | Progresso 2,5 h; fator 1,5 |
| Texto | Nome ou ID de texto | `item.pao` |
| ID | Referência a outra coisa, por número interno | A construção onde o NPC trabalha |
| Lista | Vários valores | Os empregados de uma construção |
| Sim/não | Verdadeiro ou falso | Em obra |

### Relógio do mundo (um só)

| Campo | Tipo | Observação |
|---|---|---|
| Tick atual | Inteiro (grande) | Conta desde o início da partida |
| Ano, estação, dia, hora, minuto | Inteiros | Derivados do tick |
| Velocidade | Inteiro | 0 (pausa), 1, 2 ou 4 |
| Semente e estado do sorteio | Inteiros | Mesma semente = mesma partida (Eventos) |

### Mapa Local (um, na fatia)

| Campo | Tipo | Observação |
|---|---|---|
| Tamanho | Inteiros | 64 × 64 tiles (FT1-4) |
| Camadas de tiles | Listas | Lidas do Tiled, pelo nome da camada ([[Pipeline de Arte e Conteúdo/Descrição\|Pipeline]]) |
| Passagem | Lista de sim/não por tile | Onde dá para andar |
| Lotes | Lista: posição, tamanho, ocupado por | 8 pequenos e 3 médios no nível 1 (V3-8) |
| Recursos | Lista: recurso, produtividade | Grão (normal), Água, Madeira (Tora) |
| LOD | Inteiro | Sempre 0 na fatia |

### Cidade

| Campo | Tipo | Observação |
|---|---|---|
| Nível | Inteiro | 1 |
| Banco da Cidade | Dinheiro | Começa em 4000 (400 moedas) |
| Inventário | Tabela de pilhas (item, dono, quantidade) | Um vetor por item ([[Sistema de Inventário/Descrição\|Inventário]]) |
| Capacidade | Inteiro por categoria | Soma das construções: 100 × nível nas pequenas, 200 × nível nas médias (V5, IT-4) |
| População | Inteiro por classe | 15 Civis |
| Riqueza média | Dinheiro por classe | Dinheiro médio de um Civil |
| Felicidade e alvo | Decimais por classe | 0 a 100 |
| Dias em déficit | Inteiro por tipo de consumo | Comida e Materiais Básicos |
| Preço local e fator | Dinheiro e decimal por item | Mercado Local (V2-5) |
| Desemprego | Decimal | 0 a 1 |
| Eventos ativos | Lista | ID, dias restantes, efeitos |

### Construção

| Campo | Tipo | Observação |
|---|---|---|
| Tipo | ID de dado | `construcao.cozinheiro` |
| Nível | Inteiro | 1 a 5 |
| Dono | Cidade ou Jogador | |
| Lote | ID | Onde está |
| Vida | Inteiro | Cai com a tempestade forte |
| Vagas | Inteiro | Tabela geral: 2, 3, 5, 7, 10 |
| Empregados | Lista de NPCs | |
| Gerente | NPC | O de maior habilidade (V3-8) |
| Caixa | Dinheiro | Nas da Cidade, usa o Banco da Cidade |
| Receita atual | ID | |
| Progresso | Decimal (horas) | Horas já trabalhadas no ciclo |
| Fila de ordens | Lista: tipo, item, quantidade, prioridade | Só nas do jogador |
| Salário | Baixo, Normal ou Alto | Só nas do jogador |
| Em obra | Sim/não, mais as horas que faltam | |

### NPC

Cada NPC é uma entidade com estes componentes:

| Componente | Campos | Observação |
|---|---|---|
| Identidade | Nome, cultura, classe, idade | Classe Civil na fatia |
| Habilidades | 13 números de 0 a 100 | A fatia usa Trabalho Braçal, Manufaturado e Inteligência |
| Posição | Tile (x, y) e a posição dentro do tile | |
| Caminho | Lista de tiles até o destino | Calculado pela busca de caminho (FT1-3) |
| Rotina | Horários da classe | Tabela da V2-2 |
| Ação | Dormir, Trabalhar, Comer, Comprar ou Passear (FL5-3; o Lazer e o Fugir ficam fora da fatia) | Escolhida pela Utilidade (V2-10); passo a passo na [[Ficha Logica/IA\|ficha da IA]] |
| Emprego | Construção e Função | Vazio = desempregado |
| Moradia | A Casa onde mora | Vazio = dorme ao relento |

O **personagem do jogador** tem também os componentes **Controlado** (Fome de 0 a 100, Dinheiro e inventário próprio) e **Dono** (as construções dele) (FT1-1).

### Definições lidas dos dados (não mudam durante o jogo)

| Definição | Campos principais |
|---|---|
| Item | ID, nome (ID de texto), categoria, tipo, Tier, peso, **preço base** (calculado ao carregar) |
| Receita | Produto e lote, insumos, construção e nível mínimo, Função e habilidade, habilidade mínima, horas, era |
| Construção | Porte, custo por nível, horas de obra, vagas, capacidade, receitas |
| Evento | O molde do [[Sistema de Eventos/Descrição\|Eventos]] |

---

## 2. Arquivos de dados

Tudo o que é conteúdo fica fora do código, na pasta `Jogo/` ([[Arquitetura/Moldes de Conteúdo|Moldes de Conteúdo]]):

| Arquivo | Formato | O que tem na fatia |
|---|---|---|
| `data/itens.csv` | CSV (planilha) | Os 8 itens da fatia (FL3-1, e a Pedra e o Bloco de Pedra da FL6-5); os cerca de 180, numa tarefa de dados depois da fatia |
| `data/receitas.csv` | CSV | Pão, Tábua, Móveis de Madeira e as Braçais |
| `data/construcoes.json` | JSON | As 7 construções da fatia |
| `data/eventos.json` | JSON | Os 12 eventos da V3-6 |
| `data/regras.json` | JSON | Todos os números das rodadas: salários, margem, cesta mínima, ticks, estações |
| `data/vila_inicial.json` | JSON | O estado do primeiro dia: a semente, as construções nos lotes (a Carpintaria é do jogador) e os 15 NPCs com casa e emprego (marco 5, FL5-1 e FL5-2); desde o marco 6, o dinheiro (o Banco, as 100 moedas do jogador e o dos Civis), o estoque inicial, o nível de cada construção, as habilidades e as ordens da Carpintaria (FL6) |
| `data/mapas/vila.tmj` | JSON do Tiled (FL1-1) | O mapa, com as camadas, os lotes e as áreas de recurso ([[Ficha Logica/Mapa\|ficha do Mapa]]) |
| `textos/pt-BR.json` | JSON | Todos os textos da tela (V3-12) |

*Exemplo de linha do `receitas.csv`* (as colunas seguem a receita padrão da IP1-1):

```
id;produto;lote;insumos;construcao;nivel_min;trabalho;hab_min;horas;era
receita.pao;item.pao;4;item.grao:2|item.agua:1;construcao.cozinheiro;1;manufaturado;10;4;1
```

*Exemplo de trecho do `regras.json`:*

```json
{
  "ticks_por_segundo": 20,
  "salario_por_hora_em_centesimos": { "bracal": 75, "manufaturado": 100, "especialista": 150 },
  "margem_do_dono_em_porcento": 120,
  "cesta_minima_civil": { "comida_por_dia": 1, "materiais_a_cada_dias": 8 },
  "gasto_maximo_da_renda": 0.8
}
```

Os salários por hora ficam em **centésimos** de moeda (75 = 0,75 moeda), só para a conta do preço (FL3-3); o dinheiro do jogo continua em décimos.

---

## 3. O ciclo do tempo

**Conversões** (V2-11, com a correção feita nesta ficha):

| Medida | Valor |
|---|---|
| 1 segundo real a 1x | 90 segundos de jogo |
| Ticks por segundo real | 20 |
| **1 tick** | **4,5 segundos de jogo** |
| 1 hora de jogo | 800 ticks = 40 s reais |
| 1 dia | 19.200 ticks = 16 min reais |
| 1 estação (16 dias) | cerca de 4,3 h reais |

*Correção:* a V2-11 dizia "1 tick = 2 s de jogo". A conta certa é 90 ÷ 20 = 4,5 s, e o [[Sistema de Tempo/Descrição|Tempo]] foi corrigido.

### O que roda e quando

| Quando | O que roda |
|---|---|
| **A cada tick** | Entrada do jogador; movimento dos NPCs pelo caminho; animação |
| **A cada 5 minutos de jogo** (cerca de 67 ticks) | Reavaliar a Utilidade de cada NPC. Distribuída: em cada tick, só 1/67 dos NPCs reavalia, para não pesar num tick só |
| **A cada hora de jogo** | Produção (cada trabalhador presente soma horas no ciclo); obras; fase do dia (luz) |
| **A cada dia** (virada às **0h**, rodada FT1-5) | A sequência do dia, na ordem abaixo |
| **A cada estação** (no dia 1) | Produtividade da estação; manutenção das construções; redistribuição dos empregos; nascimentos e mortes; troca de paleta |

**A sequência do dia** (FT1-5):

1. **Consumo da cidade:** a cesta mínima e a regra do gasto (4.4).
2. **Salários:** cada construção paga os empregados (4.5).
3. **Mercado Local:** novo fator e novo preço de cada item (4.6).
4. **Estabilizador:** o Banco ajuda a classe sem dinheiro (4.7).
5. **Dias em déficit** e **Felicidade** (4.8).
6. **Eventos:** sorteio dos ocasionais e checagem dos condicionais (4.12).
7. **IA da produção:** cada construção da cidade escolhe a receita do dia seguinte (4.3).

*Por que essa ordem:*

- primeiro se consome, para o preço do dia seguinte refletir a falta;
- os salários vêm antes do mercado, para a riqueza já contar no estabilizador;
- a Felicidade vem depois de tudo que a afeta;
- os eventos vêm por último, para reagir ao estado final do dia.

---

## 4. As regras, passo a passo

Cada regra está escrita como uma receita de bolo. O dinheiro está sempre em décimos.

### 4.1 Preço base (ao carregar os dados)

1. Comece pelos itens que não têm receita com insumo (os brutos).
2. Para cada receita, quando todos os insumos já têm preço:
   1. some o preço dos insumos;
   2. some as horas × o salário por hora do Trabalho da receita;
   3. divida pelo lote;
   4. multiplique por 1,2 (margem do dono).
3. Repita até todos os itens terem preço.
4. Se sobrar item sem preço, a cadeia tem um laço: é um erro de dados, e o `jogo --validar` avisa.

*Exemplo:* Pão = (2 × 5 + 1 × 5 + 4 × 10) ÷ 4 × 1,2 = 16,5, arredondado para **17** (1,7 moeda).

### 4.2 Ciclo de produção (a cada hora)

1. Para cada construção com receita e empregados presentes (na Ação Trabalhar):
2. Se o ciclo não começou, tente começar:
   1. confira se o inventário tem os insumos;
   2. se tiver, tire-os;
   3. se não tiver, a construção fica parada (alerta, se for do jogador).
3. Some ao progresso, para cada empregado presente:

   > 1 h × velocidade × bônus de nível

   - velocidade = 0,5 + habilidade ÷ 100;
   - bônus de nível = 1 + 0,1 × os níveis acima do mínimo da receita.
4. Quando o progresso chegar às horas da receita:
   1. ponha o lote no inventário, com o dono da construção;
   2. zere o progresso.

   Se a capacidade da categoria estiver cheia, o produto espera e a construção para.
5. Nas Braçais não há insumo: o lote é "1 unidade a cada 0,5 h", vezes a produtividade e a estação.

### 4.3 Escolha da receita (IA, a cada dia)

1. Para cada receita que a construção pode fazer:

   > **nota = (preço local do produto − custo dos insumos) × (1 + déficit)**

   - o déficit vai de 0 (estoque cheio) a 1 (zerado).
2. Se o estoque de comida da cidade estiver abaixo de 3 dias de consumo, a comida ganha sempre.
3. Escolha a receita de maior nota.

As construções do jogador seguem a fila de ordens, não esta conta.

### 4.4 Consumo diário

1. Para cada classe, calcule a renda do dia (salários recebidos) e o teto de gasto (80% da renda).
2. **Cesta mínima:**
   1. tire do inventário 1 comida por NPC, a mais barata que a classe aceita;
   2. tire 1 Material Básico a cada 8 dias por NPC;
   3. pague o preço local ao dono de cada item: Banco, para os itens da Cidade; o jogador, para os itens dele.
3. Se faltou item, some 1 dia em déficit naquele tipo de consumo; se não faltou, zere a conta.
4. **Gasto extra:** enquanto sobrar teto, compre comida de Tier maior, Bebida (até 25% da comida) e Materiais a mais.
5. O que sobra da renda vai para a riqueza média da classe.

### 4.5 Salários (a cada dia)

1. Para cada empregado que trabalhou no dia:

   > salário = horas × salário por hora do Trabalho × multiplicadores (nível, gerente, riqueza, desemprego, nível de salário do jogador)

2. O dinheiro sai do caixa da construção (nas da Cidade, do Banco) e vai para a renda da classe do NPC.
3. Se não houver dinheiro, o empregado não recebe e a construção entra em atraso. Depois de 1 estação em atraso, os empregados saem.

### 4.6 Mercado Local (a cada dia)

1. Para cada item:

   > estoque-alvo = 5 dias do consumo do item (classes + receitas)
   >
   > fator = 1 + (alvo − estoque) ÷ alvo, limitado entre 0,5 e 2,0

2. Calcule o preço desejado = preço base × fator.
3. Mova o preço local na direção do desejado, no máximo 10%.

### 4.7 Estabilizador (a cada dia)

1. Se a riqueza média de uma classe estiver abaixo de 3 dias da cesta mínima, o Banco paga a diferença até chegar a 5 dias.
2. O Banco nunca paga mais de 10% do que tinha no começo da estação.

### 4.8 Felicidade (a cada dia)

Na fatia, só 4 fatores, dentro das faixas do [[Sistema de Felicidade e Estabilidade/Descrição|Felicidade]] (FT1-2):

| Fator | Conta |
|---|---|
| Consumo atendido | −25 × a fração de NPCs sem comida no dia; −5 se os Materiais Básicos estão em déficit; até +5 pelo gasto extra |
| Desemprego e renda | +10 − 20 × a taxa de desemprego, limitado entre −10 e +10 |
| Moradia | −10 × a fração de NPCs sem casa |
| Eventos duradouros | A soma dos eventos ativos, limitada entre −20 e +20 |

1. Alvo = 50 + a soma dos fatores, limitado entre 0 e 100.
2. Felicidade nova = Felicidade + 10% × (alvo − Felicidade).

### 4.9 Utilidade do NPC (a cada 5 minutos de jogo)

1. Para cada ação, a nota é 1,0 se ela é a do período da rotina, mais os bônus da V2-10 (Comer, Comprar, Lazer, Fugir).
2. O NPC faz a de maior nota. Se o destino mudou, peça um caminho novo (4.10).

### 4.10 Movimento (a cada tick)

- Busca de caminho **A\*** numa grade de **8 direções** (diagonal custa 1,4), calculada uma vez por destino (FT1-3).
- O NPC anda pelo caminho na velocidade de andar: **128 tiles por hora de jogo** (FL4-1). Se um tile ficar bloqueado (uma obra nova), ele pede outro caminho.
- O passo a passo está na [[Ficha Logica/NPC|ficha do NPC]] e na regra 4.4 da [[Ficha Logica/Mapa|ficha do Mapa]].
- Na fatia, os NPCs **não colidem** entre si (passam um pelo outro), para simplificar.

### 4.11 Obra

1. A obra começa quando a cidade tem os materiais (que saem do inventário) e um lote livre.
2. A cada hora, cada Operário presente soma horas × velocidade.
3. Quando chega às horas de obra, a construção fica pronta.

### 4.12 Eventos (a cada dia)

1. **Ocasionais:** para cada evento cujas condições valem (estação, cooldown), sorteie com a chance base. Se sair, aplique os efeitos da lista fechada.
2. **Condicionais:** confira cada condição (por exemplo, "comida abaixo de 10% por 3 dias"). Se vale e o evento não está ativo, dispare-o.
3. Os eventos ativos descontam 1 dia; com 0, os efeitos saem.

---

## 5. Save

| Grava | Não grava (recalcula ao carregar) |
|---|---|
| Relógio, semente e estado do sorteio | Definições de itens, receitas, construções e eventos (vêm dos dados) |
| Cidade: Banco, inventário, riqueza, Felicidade, preços locais, dias em déficit, eventos ativos | Preços base |
| Construções: tipo, nível, dono, Vida, empregados, caixa, receita, progresso, ordens, obra | Caminhos dos NPCs |
| NPCs: todos os componentes, menos o caminho | Capacidade (soma das construções) |
| Personagem do jogador: posição, Fome, dinheiro, inventário | |

O formato é um arquivo binário com número de versão ([[Arquitetura/Descrição|Arquitetura]]). Teste obrigatório: salvar, carregar e salvar de novo tem que dar o mesmo arquivo.

---

## 6. Testes

- **Teste de 1 ano sem tela** (V2-12):
  - **comando:** `jogo --simular 64 --semente 1` (FT1-6), que roda 64 dias na velocidade máxima, sem desenhar nada;
  - **relatório:** grava um CSV com uma linha por dia: estoque de cada item, preço do Pão, Banco, riqueza média, Felicidade, empregados, eventos do dia.
  - **Passa se:** ninguém passa fome por mais de 3 dias seguidos, o Banco nunca zera, e o preço do Pão fica entre 1,2 e 2,6.
- **Repetição:** a mesma semente tem que dar o mesmo relatório, linha por linha. Isso pega erros que aparecem "às vezes".
- **Testes das fórmulas:** pequenos testes automáticos de cada regra da parte 4. Exemplo: "Pão com os preços de partida custa 17".
- **Desempenho:** medir com o Tracy o tempo de um tick com 15 NPCs, e depois com 500 NPCs falsos, a meta da V2-11.

---

## 7. Marcos de programação

A ordem da [[Arquitetura/Descrição|Arquitetura]] (V4-6), com o que fica pronto em cada marco:

| Marco | Fica pronto quando |
|---|---|
| 0. Ambiente | O VS Code compila e roda um "olá, janela" com o SDL3; as bibliotecas do marco 1 estão aprovadas (FT1-7) |
| 1. Janela, loop, relógio fixo e mapa de tiles | A janela abre, o relógio conta os dias e o mapa do Tiled aparece |
| 2. Câmera e entrada | O jogador move a câmera e dá zoom |
| 3. Carregar os dados | O jogo lê os arquivos, calcula os preços base, e o `--validar` funciona |
| 4. NPC andando | Um NPC vai de um ponto a outro pela busca de caminho |
| 5. Rotina | Os 15 NPCs dormem, trabalham e passeiam nos horários |
| 6. Produção, inventário e consumo | O Pão sai do Cozinheiro, entra no estoque e é comido |
| 7. Simulação sem tela de 1 ano | O teste da parte 6 passa |
| 8. Interface básica | HUD com data, velocidade, dinheiro; painel da cidade e do inventário; as obras e a manutenção; o preço, o salário, a contratação e a compra e venda do jogador (rodada FL8) |
| 9. Salvar e carregar | O teste do save passa |

---

## 8. Decisões da rodada FT1

| Pergunta | Decisão |
|---|---|
| FT1-1 Papel do jogador | O jogador começa com **100 moedas** e uma **Carpintaria nível 2 própria** (3 vagas, 2 ocupadas no começo; era nível 1, e a FL6-2 subiu para o 2, porque os Móveis pedem a Carpintaria nível 2). Pode contratar desempregados, escolher o salário (Baixo, Normal, Alto), dar ordens (Fazer N, Manter estoque, Repetir), fixar o preço na faixa de ±30% e andar, comprar e vender no Mercado. Os Móveis de Madeira dele são os Materiais Básicos da vila |
| FT1-2 Felicidade | Os 4 fatores da regra 4.8 |
| FT1-3 Movimento | A\* em 8 direções (diagonal custa 1,4); um caminho por destino, com o cálculo distribuído entre os ticks; os NPCs não colidem entre si |
| FT1-4 Mapa e câmera | Mapa de **64 × 64 tiles**; zoom de 2x, 3x e 4x (padrão 3x, cerca de 33 × 19 tiles na tela); câmera com WASD ou setas, roda do mouse para o zoom, arrastar com o botão do meio; lotes de 4 × 4 (pequeno), 6 × 6 (médio) e 8 × 8 tiles (grande); um campo de Grão, um poço e um bosque |
| FT1-5 Ordem do dia | Virada às 0h; a ordem da parte 3 fica no `data/regras.json` (campo `ordem_do_dia`), para ser trocada nos testes sem mexer no código |
| FT1-6 Teste sem tela | `jogo --simular 64 --semente 1` grava o `relatorio.csv` (uma linha por dia) e termina com PASSOU ou FALHOU e o motivo ("fome por 4 dias seguidos no dia 37"); a mesma semente dá o mesmo relatório |
| FT1-7 Ambiente | VS Code com as extensões grátis "C/C++" e "CMake Tools"; depurador gdb (vem com o MinGW); primeiro código: um "olá, janela" com o SDL3, compilado pelo CMake; no mesmo repositório (depois organizado em `Codigo/` e `Jogo/`, ver [[Arquitetura/Descrição|Arquitetura]]) |

**Rodada FL1 (2026-10-17):** as fichas do marco 1 ([[Ficha Logica/Tempo|Tempo]] e [[Ficha Logica/Mapa|Mapa]]). O mapa é salvo em JSON (`vila.tmj`), lido pela nlohmann/json já no marco 1; a partida começa às **6h** (tick 4.800); o relógio usa no máximo 250 milissegundos por quadro e pausa quando a janela é minimizada; os testes rodam num programa próprio pelo `ctest`; as camadas do Tiled têm nomes sem acento; o marco 1 usa um conjunto de tiles de teste.

**Rodada FL3 (2026-10-18):** as fichas dos [[Ficha Logica/Itens|Itens]], dos [[Ficha Logica/Recursos|Recursos]] e das [[Ficha Logica/Receitas|Receitas]]. O marco 3 tem só os 6 itens da fatia; as receitas Braçais são 2 unidades em 1 hora; o salário por hora fica em centésimos só na conta do preço, com a metade arredondando para cima, item por item; o peso é uma unidade inteira com o Pão valendo 1; o `construcoes.json` do marco 3 é só a lista de IDs; os recursos ficam num `recursos.csv`, a estação no `regras.json` e a produtividade na área do Tiled.

**Rodada FL5 (2026-10-18):** a ficha da [[Ficha Logica/IA|IA]] (marco 5). O `vila_inicial.json` põe as 10 construções nos lotes (retângulos de cor provisórios, que já bloqueiam o lote, com a porta embaixo); 15 Civis, 12 empregados (a Carpintaria começa com 2) e 3 desempregados; a ação do Período Livre é o **Passear**, o Comprar ganha dela no dia de compra, e o Lazer fica fora da fatia; os números da rotina e das notas (em centésimos) ficam no `regras.json`, com a reavaliação a cada 67 ticks; quem está dentro da casa ou do trabalho come lá, e quem está na rua vai para casa; o passeio é a um tile sorteado a até 12 tiles, com o gerador PCG32 e a semente da partida; o teste usa a tecla F3, e o botão direito e o N do marco 4 saem.

**Rodada FL6 (2026-10-18):** as fichas da [[Ficha Logica/Producao|Produção]], do [[Ficha Logica/Inventario|Inventário]], da [[Ficha Logica/Economia|Economia]] e da [[Ficha Logica/Construcao|Construção]] (marco 6). O marco 6 tem o inventário, a produção, o consumo da cesta mínima e os salários, com os preços no preço base; o Mercado Local, o estabilizador, o gasto extra e o imposto ficam para o marco 7, e as obras, para o 8; a Carpintaria do jogador começa no **nível 2** (muda a FT1-1) e segue as ordens "manter 6 Móveis" e "manter 12 Tábuas", comprando da Cidade a Tora que faltar; a vila começa com o Banco em 400 moedas, o jogador com 100, 12 moedas por Civil e 3 dias de Pão; a Pedra, o Bloco de Pedra e a Pedreira entram nos dados, e a vila começa com 150 Blocos (a FL3-1 passa a 8 itens); todos os NPCs têm habilidade 50; o café passa para as **6h**, e o almoço conta como trabalho; o teste usa uma linha de resumo por dia no console e a tecla F4.

**Rodada FA1 (2026-10-18):** os critérios da arte, no [[Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]]: o quadro do boneco de 32 × 32, com os pés em (16, 28); a luz de cima, à esquerda, e o contorno colorido; a sombra no chão semitransparente (a cor 65) e no máximo 8 cores a mais; as bordas em conjuntos de cantos de 16 tiles; e as construções com 32 px a mais de altura que o lote.

**Rodada FL7 (2026-10-18):** a parte 10 da [[Ficha Logica/Economia|Economia]] e da [[Ficha Logica/Producao|Produção]] e as fichas da [[Ficha Logica/Felicidade|Felicidade]], dos [[Ficha Logica/Eventos|Eventos]] e da [[Ficha Logica/Simulacao|Simulação sem tela]] (marco 7). O gasto extra da classe compra primeiro Materiais a mais (até o número da cesta do dia) e, o resto, na loja da cidade, para o Banco; o imposto é de **20%**, retido a cada dia no salário e todo para o Banco da Cidade (na fatia, sem facção: muda a V2-7 só na fatia), e o lucro do jogador ainda não paga; os multiplicadores do salário são contínuos (a riqueza, de 80% a 120%; o desemprego, 110% − a taxa, de 90% a 110%); o consumo previsto é a média dos 5 últimos dias do que a vila gastou ou quis gastar; uma receita da Cidade para com 125% do estoque-alvo e volta abaixo do alvo, e a construção sem candidata fica de folga (os empregados passeiam e não recebem); 11 dos 12 eventos da V3-6 (sem a Tempestade, 4 adaptados), num `data/eventos.json`, sem o Narrador; a Felicidade começa em 50 e, no marco 7, só é medida e mostrada; o `relatorio.csv` sai com ponto e vírgula e vírgula decimal, o `ctest` repete 16 dias duas vezes e roda os 64 dias à parte, e o Tracy é uma opção do CMake, desligada.

**Rodada FE1 (2026-10-19):** o equilíbrio do teste de 1 ano (marco 7), toda com "seguir sugestão". A Carpintaria do jogador passa a "manter **20** Móveis" e "manter 12 Tábuas" (muda a FL6-3: o gasto extra e o consumo medido sobem o estoque-alvo, e com 6 o preço ia ao teto e o dinheiro drenava do Banco para o jogador); a vila começa com **75 Pães** (5 dias, o estoque-alvo; muda a FL6-4: com 45, uma Febre num Cozinheiro no primeiro mês fazia o Pão passar de 2,6). Medido: 10 sementes de 64 dias, todas passam; em 4 anos, o Banco não fica abaixo de 334.

**Rodada FL4 (2026-10-18):** a ficha do [[Ficha Logica/NPC|NPC]] e a busca de caminho na do [[Ficha Logica/Mapa|Mapa]] (marco 4). O NPC anda 128 tiles por hora de jogo (160 milésimos de tile por tick); a diagonal só vale com os dois tiles retos do lado livres; sem caminho, o NPC vai ao tile mais perto do destino; o boneco provisório (Mana Seed, quadros de 64 × 64) é descrito num arquivo de dados e tem um boneco de teste de reserva no Git; o desenho fica entre a posição de um tick e a do seguinte; o marco 4 se testa com o botão direito, o F2 (caminhos) e o N (mais um NPC).

**Rodada FL2 (2026-10-18):** a ficha da [[Ficha Logica/Interface|Interface]] (câmera do marco 2). A câmera anda 900 pixels da tela por segundo em qualquer zoom (Shift dobra, diagonal corrigida); o zoom deixa o ponto embaixo do mouse parado; a borda do mapa nunca sai da borda da tela; a câmera começa no centro do mapa e não vai no save; mover pela borda da tela fica como opção futura, desligada; uma tabela de ações para as teclas entra no marco 3.

**Rodada FT2:** o código foi liberado marco a marco. As bibliotecas entram por marco, e os nomes são em português, sem acento e descritivos ([[Arquitetura/Descrição|Arquitetura]]).

**Organização das pastas (2026-10-17):** esta ficha é o mapa geral da fatia. Cada sistema ganha a sua ficha nesta pasta, no modelo da [[Ficha Logica/Descrição|Ficha Lógica]], pouco antes do marco que o usa.

## Ligação com outros sistemas

| Sistema | O que a ficha usa |
|---|---|
| Arquitetura e Moldes | ECS, formatos de dados, marcos |
| Tempo | Ticks, horas, dias e estações |
| Produção e Receitas | Receita padrão, ciclo, Braçais |
| Econômico, Trabalho e Classe Social | Preço base, Mercado Local, salários, cesta, estabilizador |
| Felicidade | Os 4 fatores da fatia |
| IA | Rotina e Utilidade |
| Construção e População | Obras, portes, moradias, lotes |
| Eventos e Clima | 12 eventos; estações e clima do dia |
| Inventário e Itens | Pilhas, capacidade, os 8 itens |

## Perguntas em aberto

1. **Valores a testar:** todos os números da fatia, no teste sem tela.
2. O que cada marco revelar vira pergunta numa rodada.
