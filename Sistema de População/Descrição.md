> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de População e Habitação

**Descrição geral**

Define quantas pessoas uma cidade tem, como esse número muda (nascimento, morte, migração), onde as pessoas moram e quando a cidade sobe de nível. Antes disso, o projeto tinha o Fator de Escala e os atributos de idade e gravidez dos NPCs, mas nenhuma regra de crescimento.

Princípios:

1. **Poucos NPCs reais, muita população.** O que o jogador vê são NPCs reais; o resto da população é representado pelo Fator de Escala (ver [[Sistema de Facção/Descrição|Sistema de Facção]]).
2. **Agregado por padrão.** Nascimentos, mortes e migração são calculados por cidade e por estação, não NPC a NPC.
3. **Vida individual só para quem importa.** Importantes e Controlados têm ciclo de vida completo.

---

## Modelo de população

### Capacidade e ocupação

- Cada nível de cidade tem uma **capacidade de NPCs reais** (ver [[Sistema de Cidade/Descrição|Sistema de Cidade]]): 15, 40, 100 e 300 (valores provisórios).
- A **ocupação** é o número atual de NPCs reais, entre 0 e a capacidade. Ela é limitada pelas **moradias** e pela **comida**.
- A **população total** é NPCs reais × Fator de Escala.
- A cidade **sobe de nível** quando está quase cheia e cumpre as outras condições (ver "Níveis de cidade").

### Nascer e morrer em números grandes

- Nascimentos e mortes são calculados **em pessoas, por estação**, e acumulam uma **fração**.
- Quando a fração chega a **um Fator de Escala**, nasce ou morre **1 NPC real**.
- Se um NPC real morre em combate, crime ou evento, **só ele sai**. A cidade repõe a vaga aos poucos, por nascimento ou migração.

### Quem tem vida individual

| Grupo | Ciclo de vida |
|---|---|
| **Importantes e Controlados** | Completo: idade, família, gravidez, morte por velhice, herdeiros |
| **NPCs comuns** | **Reposição:** quando um morre de velhice, um novo NPC jovem ocupa a vaga, sem simular gravidez nem família |

### Crianças

- As crianças **comuns não são NPCs**. Contam só como uma fração da população (e, se a arte permitir, como sprites decorativos).
- Só crianças de **Importantes e Controlados** são NPCs.
- Isso economiza simulação e arte.

### Tempo de vida

Com 24 h de jogo = 16 min reais, **1 ano = cerca de 17 horas reais a 1x** (cerca de 4 horas a 4x), e 1 estação = cerca de 4,3 horas. Por isso a vida é medida em anos de jogo, curtos:

| Estágio | Idade (anos de jogo) |
|---|---|
| Criança | 0 a 3 |
| Adulto | 4 a 20 |
| Velho | 21 em diante |

- A **morte natural** acontece entre 21 e 30 anos, com chance crescente por estação.
- Assim uma dinastia de 2 ou 3 gerações cabe em uma partida longa.
- **[Refinar depois]** valores exatos, ou envelhecer mais rápido que o calendário, ajustados em testes.

---

## Natalidade, mortalidade e migração

### Natalidade

- Taxa base **por estação** sobre os adultos, multiplicada por modificadores: **comida suficiente, moradia livre, Felicidade e paz**.
- O teto é a **capacidade** da cidade.
- Gravidez individual (atributos Grávida e Estágio) só para Importantes e Controlados: dura 1 estação, e a infância, 3 anos (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).

### Mortalidade

Velhice, mais causas que vêm de outros sistemas:

- **doença** (evento de Saúde; ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]);
- **fome** (déficit de comida por muitos dias; efeito **gradual**, não instantâneo);
- **guerra** e **crime**;
- **frio** (falta de lenha no inverno: velhos e crianças +50%) e eventos de clima, como a onda de calor e o inverno rigoroso (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).

Na Religiosa, o Templo reduz a mortalidade por doença em 25% (ver [[Sistema de Religião/Descrição|Religião e Fé]]).

### Migração

- Cada estação, as pessoas vão para cidades mais **atraentes**: Felicidade alta, emprego livre, moradia livre e comida.
- Os **migrantes mantêm a cultura de origem** (ver [[Sistema de Cultura/Descrição|Sistema de Cultura]]); depois, podem assimilar a cultura dominante aos poucos (ver [[Sistema de Religião/Descrição|Religião e Fé]]).
- Com a lei de **Perseguição**, as minorias migram para fora 2× mais rápido.
- Em LOD 0 aparecem como NPCs que **chegam pela borda do mapa**. Em LOD 2 é só um fluxo calculado.
- Facções em **guerra** ou com **fronteira fechada** bloqueiam a migração entre elas.

---

## Habitação

### Moradias por classe

| Classe | Moradia |
|---|---|
| Escravo | Senzala |
| Civil | Casa |
| Rico | Casarão |
| Nobreza | Mansão |
| Realeza | Palácio |

- Cada moradia abriga um número de **NPCs reais** por tipo e nível (rodada V2-8):

| Moradia | Nível 1 | Por nível a mais |
|---|---|---|
| Senzala | 8 | +4 |
| Casa | 4 | +2 |
| Casarão | 2 | +1 |
| Mansão | 2 | +1 |
| Palácio | 3 | +1 |
- A moradia é um **Mapa Interno do tipo Moradia** (ver [[Sistema de Mapa/Descrição|Sistema de Mapa]]).
- O NPC **sem casa** dorme ao relento: a Felicidade cai e a chance de doença sobe.

### Quem constrói as casas

- A **cidade constrói sozinha** enquanto houver **lote livre** e materiais, usando o Construtor.
- Na cidade do jogador, ele decide o que construir.
- A **falta de lote** limita o crescimento e dá motivo para **subir de nível**, que libera mais lotes.

### Lotes e arte

- **Lotes fixos**, marcados no **Tiled** (ver [[Arquitetura/Descrição|Arquitetura]]) em 3 tamanhos: pequeno, médio e grande.
- Os interiores têm **poucos layouts por tipo de moradia e por cultura**, reaproveitados entre casas.
- Em **LOD 2** a casa não tem interior até alguém entrar.
- **[Refinar depois]** quantidade de lotes por Mapa Local e por nível.

---

## Níveis de cidade

### Condições para subir

**[Refinar depois]** valores e lista exata. Para subir de 1 para 2 e de 2 para 3, a cidade precisa de:

| Condição | Valor inicial |
|---|---|
| **Ocupação** | Acima de 90% da capacidade por 2 estações |
| **Felicidade** | Acima de um limiar |
| **Construções obrigatórias** | Nível 2: Mercado e Taverna. Nível 3: Quartel, Templo ou Laboratório, conforme a cultura |
| **Tecnologia** | Do ramo Construção (ver [[Sistema de Tecnologia/Descrição\|Sistema de Tecnologia]]) |
| **Custo** | Materiais e dinheiro |

O nível 4 continua restrito às capitais, por conquista (ver Sistema de Cidade).

### Queda de nível

- A cidade só **cai de nível** em catástrofe grave (peste, fome longa ou saque), com **aviso** antes. O saque e a fome no cerco estão no [[Sistema de Guerra/Descrição|Sistema de Guerra]].
- As construções acima do novo nível ficam **inativas**, sem serem destruídas.

### Jogador e IA

As **mesmas regras e a mesma fórmula** valem para o jogador e para a IA. As cidades da IA em LOD 2 crescem pelo cálculo por estação.

---

## Parte técnica

- **LOD:** em LOD 2, nascimento, morte e migração são somados por estação por cidade. Em LOD 0 e 1, a reposição de NPCs acontece quando o acumulador chega ao Fator de Escala.
- **Save:** população total, frações acumuladas de natalidade e mortalidade, ocupação, lotes ocupados e moradias por cidade.
- **Dados:** taxas de natalidade e mortalidade, capacidade por nível, capacidade por tipo de moradia e condições de nível em JSON.
- **Fatia vertical:** usa só uma cidade, com capacidade fixa e moradias simples; sem migração.

### Ligação com outros sistemas

| Sistema | Ligação |
|---|---|
| Cidade | Capacidade por nível, condições para subir, lotes |
| NPC | Estágio e idade, Grávida só para Importantes e Controlados, crianças comuns não são NPCs |
| Mapa | Moradia como Mapa Interno; chegada de migrantes pela borda |
| Construção | Moradias, lotes e construções obrigatórias por nível; Construtor |
| Classe Social | Moradia por classe |
| Cultura | Migrantes mantêm a cultura; construções obrigatórias dependem da cultura |
| Escravidão | Filhos de Escravos nascem Escravos onde a lei permite; fuga; Senzalas viram Casas na abolição (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) |
| Facção | Fator de Escala, guerra e fronteira bloqueiam a migração |
| Eventos | Doença, fome, guerra e crime como causas de mortalidade |
| Religião | Assimilação das minorias; migração das perseguidas; menos doença na Religiosa (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| Clima e Estações | Frio e eventos de clima como causas de morte; fome no inverno sem estoque (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Tecnologia | Ramo Construção libera níveis de cidade |
| Econômico | Emprego livre atrai migrantes; custo de subir de nível |
| Tempo | Taxas por estação; vida em anos de jogo |

### Perguntas em aberto

1. **Valores a testar:** taxas de natalidade e mortalidade, vida em anos de jogo, capacidade por tipo de moradia, limiares de Felicidade, custo de subir de nível.
2. **Quantidade de lotes** por Mapa Local e por nível, e tamanho de cada lote.
3. **Atratividade da migração:** pesos de Felicidade, emprego, moradia e comida (o emprego vem do [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]]).
4. **Senzala:** quantos Escravos abriga e quem o possui (cidade, dono ou facção).
5. **Moradia dos Controlados do jogador:** casa própria, quarto de Taverna ou alojamento.
6. **Crianças decorativas:** vale a pena o custo de arte?
