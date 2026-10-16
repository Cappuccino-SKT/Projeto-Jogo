> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Crime e Justiça

**Descrição geral**

Define o que é crime, como é detectado, como é punido e quais as consequências para quem o comete. Une peças que já existem em outros sistemas: os atributos **Criminoso**, **Prisioneiro** e **Recompensa** do NPC, a **Função de guarda**, o **Nível de Segurança** do Mapa Interno, o período de dormir como melhor momento para o crime, e a lei **Crimes e penas** do Sistema de Facção.

Quase tudo aqui é configurável **por lei da facção**: cada facção decide o que é crime, qual a pena e como ela varia por classe.

### Crimes

- Existe uma **lista fixa de crimes**, em arquivo de dados.
- Cada crime tem uma **gravidade de 1 a 5**.
- Cada facção define, na lei **Crimes e penas**, quais crimes valem e qual a pena de cada um.

**[Proposta]** Lista inicial e gravidade (valores ajustáveis):

| Crime | Gravidade |
|---|---|
| Desacato (insulto, que já é gatilho de hostilidade) | 1 |
| Furto | 2 |
| Contrabando (item proibido) | 2 |
| Invasão de domicílio | 3 |
| Agressão | 3 |
| Fuga ou ajuda a escravo | 3 |
| Assassinato | 5 |
| Traição | 5 |

**Crimes culturais.** Alguns crimes vêm da **Cultura** da facção: heresia na Religiosa, magia na Tecnocrata, libertar escravo na Escravista, ser não Mago em certas funções na Arcana. Eles entram na lista da facção conforme as leis dela.

### Detecção

**Um crime só existe se houver testemunha.** Testemunhas são NPCs ou guardas dentro de um raio.

- A **Percepção** da testemunha é comparada à **Destreza (ocultação)** do criminoso.
- O **Nível de Segurança** do Mapa Interno modifica o resultado.
- O **período do dia** modifica o resultado: a noite reduz a detecção.
- Sem testemunha, nada é registrado.

### Criminalidade da cidade (LOD 2)

Em mapas em LOD 2, o crime é **agregado por cidade**: nenhum crime individual é simulado fora da visão do jogador.

- A cidade tem um valor de **Criminalidade**.
- **[Proposta]** Influenciado por Felicidade, pobreza e presença de guardas.
- A Criminalidade alimenta **eventos** e a **Felicidade** da cidade.

### Recompensa

- A recompensa é **por facção**, não global. Um criminoso procurado em uma facção é inocente em outra.
- **Cálculo:** valor base do crime × gravidade.
- **Diminui com o tempo.**
- Só NPCs Criminosos têm a lista de recompensas, para não gastar memória com NPCs comuns.

**Reputação.** Todo crime detectado também tira reputação do jogador na facção: −2 × a gravidade. Pagar a Recompensa limpa a ficha, mas a reputação volta só aos poucos (ver [[Sistema de Reputação/Descrição|Sistema de Reputação]]).

**Efeito no Sistema de NPC.** O atributo Recompensa deixa de ser um valor único e passa a ser uma **lista (facção, valor)**. A booleana Criminoso passa a significar "tem recompensa em alguma facção".

### Reação dos guardas

Resposta **escalonada**:

1. **Ordem de rendição.** O jogador pode aceitar, subornar ou ignorar.
2. **Prisão**, com **combate não letal**: o guarda tenta deixar o alvo **Inconsciente** em vez de Morto (usando o Estado Atual que já existe).
3. **Força letal** só se o criminoso resistir ou o crime for muito grave.

Guardas continuam sem o período de dormir, fazendo rondas 24 horas, como já definido na IA.

**Quantidade de guardas por cidade.** Depende do **nível da cidade** e da **Criminalidade**, com um **mínimo definido pela lei** da facção.

### Punições

Tipos de punição:

- **Multa**
- **Confisco de itens**
- **Prisão** por tempo
- **Trabalho forçado:** o criminoso vira escravo produtivo por um período
- **Banimento**
- **Execução**

**A punição de cada crime pode ser alterada pela lei da facção.** Exemplos de tendência cultural: a Escravista aplica escravização, a Religiosa aplica punições severas.

**[Proposta]** O trabalho forçado marca o NPC como Escravo com prazo. Ao fim, ele volta à classe anterior.

### Penas por classe social

As penas **variam por classe social**, e isso também é **modificável por lei**: cada facção tem sua própria variação.

Exemplo de tendência: um Nobre paga multa por um crime que levaria um Civil à prisão e um Escravo ao castigo. As culturas Autoritária e Escravista tendem a ser as mais desiguais.

### Prisão

A Prisão é um **Mapa Interno** do tipo Construção Pública.

- NPCs presos cumprem a pena em **LOD 2**, sem simulação.
- Um **jogador preso** pode:
  - esperar a pena passar (tempo acelerado);
  - pagar fiança;
  - tentar fugir (Destreza e Percepção);
  - ser resgatado por um aliado.

### Jogador criminoso

- A facção gera NPCs **Caçadores de Recompensa** (nova Função) que o procuram. **Como base inicial**, isso acontece quando o criminoso cometeu um crime do mesmo nível do Assassinato (gravidade 5). O valor mínimo de recompensa será definido em testes futuros. Em LOD 2 isso é agregado.
- O jogador **limpa a ficha** por:
  - **anistia** (paga);
  - **cumprir a pena**;
  - **sair da jurisdição** (a recompensa é por facção).

### Suborno

Existe suborno: pagar um guarda para ignorar o crime.

- A chance depende da **Ganância** e da **Lealdade** do guarda (Personalidade), do **valor oferecido** e da **gravidade do crime**.
- Se falhar, a **recompensa sobe** e o guarda tenta prender. O suborno também é uma ação do Diálogo (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).

### Mercado negro e itens ilegais

- Alguns itens **já foram pensados para serem ilegais**.
- Pela lei **Itens proibidos**, a facção pode **tornar ilegal um item comum** ou **tornar legal um item ilegal**.
- Itens proibidos só são vendidos em **lojas ilegais**, com **preço multiplicado** e **risco de flagrante** pelos guardas.
- Comprar, vender ou carregar um item proibido é o crime de **Contrabando**.

**Lojas ilegais.** **Qualquer construção de produção pode ser uma loja ilegal.** Ela se passa por outra coisa (uma ferraria, uma cozinha), mas na verdade compra e vende produtos ilegais. Isso torna o mercado negro imersivo: o jogador não descobre qual loja é ilegal só olhando.

- Para descobrir que uma loja é ilegal, e poder negociar produtos ilegais nela, o jogador precisa de um **evento ou missão**: é o NPC **ganhando confiança** no jogador para permitir a entrada no mercado negro.
- **[Proposta]** A construção ganha um atributo "Ilegal" (disfarce), e o NPC dono ou gerente guarda um nível de Confiança com o jogador. Sem confiança, a loja se comporta como uma loja comum.

### Prisioneiros de guerra

- Só **líderes e NPCs Importantes** viram prisioneiros individuais, com a opção de **resgate** ou **execução**.
- As **baixas comuns** de um esquadrão derrotado podem ser **escravizadas em massa**, se a lei da facção permitir, gerando NPCs Escravo conforme o Fator de Escala (100 prisioneiros = 1 NPC Escravo). Na batalha, 20% dos soldados derrubados são capturados, e os feridos do perdedor também (ver [[Sistema de Batalha/Descrição|Sistema de Batalha]]).

### Ligação com outros sistemas

| Sistema | O que Crime e Justiça fornece ou recebe |
|---|---|
| Facção | Lei Crimes e penas, Itens proibidos, Escravidão; recompensa por facção |
| Cultura | Crimes culturais e tendência de punição |
| NPC | Criminoso, Prisioneiro, Recompensa (agora uma lista); Função de guarda e de Caçador de Recompensa |
| IA | Reação dos guardas, suborno (Personalidade), detecção (Percepção e Destreza) |
| Mapa | Nível de Segurança do Mapa Interno; Prisão como Mapa Interno |
| Cidade | Criminalidade, que influencia Felicidade e eventos |
| Econômico | Lojas ilegais (qualquer construção de produção), preço multiplicado, multas e confiscos como dinheiro da facção |
| Itens | Itens proibidos; Grilhões (item do Ferreiro) para prisioneiros |
| Classe Social | Variação das penas por classe |
| Tempo | Duração das penas e decaimento da recompensa |
| Eventos | Bandidos surgem como evento condicional da Criminalidade alta: grupos soltos (não uma facção), com tamanho proporcional à Criminalidade da cidade mais próxima; em LOD 2 viram um número de "ameaça bandida" por região. A missão de confiança pode ser um evento com escolha (ver [[Sistema de Eventos/Descrição\|Sistema de Eventos]]) |
| Combate | O modo de ataque (letal ou não letal) é do NPC; atacar guarda ou cidadão gera crime (Agressão ou Assassinato); se todo o grupo do jogador cai diante dos guardas, ele é preso; duelo aceito não é crime (ver [[Sistema de Combate/Descrição\|Sistema de Combate]]) |

### Perguntas em aberto

1. **Valores de gravidade e das multas.** A tabela é um ponto de partida e precisa de testes.
2. **Fórmula da Criminalidade.** Os pesos de Felicidade, pobreza, desemprego e guardas (testes futuros).
3. **Valor mínimo da recompensa** para gerar Caçadores de Recompensa (testes futuros). Base inicial: crimes de gravidade 5.
4. **Loja ilegal do jogador.** O jogador também pode transformar uma construção de produção dele em loja ilegal?
5. **Missão de confiança.** Quais tipos de missão ou evento, e quanta Confiança é necessária.
