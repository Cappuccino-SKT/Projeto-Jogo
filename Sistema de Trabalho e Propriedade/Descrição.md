> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Trabalho, Salários e Propriedade

**Descrição geral**

Define como um NPC consegue emprego, quem é dono de cada construção, quem paga os salários, como o dinheiro circula e como os impostos funcionam. Antes disso, o Econômico tinha a distribuição rotativa do Banco da Cidade e as lojas, mas não dizia quem trabalha onde nem de onde sai o salário.

O dinheiro circula em um ciclo que se sustenta sozinho:

> **consumidores gastam nas lojas → as lojas pagam salários e compram insumos → os trabalhadores gastam de novo**

A distribuição rotativa do Banco da Cidade vira só o **estabilizador** desse ciclo (ver "Salários"). Tudo é **agregado por cidade**, sem rastrear o dinheiro de cada NPC.

---

## Emprego

### Vagas

- As construções têm **vagas por nível**. Os atributos de construção "empregados contratados" e "empregados trabalhando" já existem.
- O NPC recebe a vaga que combina com a **classe** e com a **habilidade** dele, com preferência pela melhor habilidade.
- A cada **estação**, a cidade redistribui os NPCs sem vaga ou com vaga ruim.
- A Função de cada vaga vem da tabela [[Sistema de Produção/Funções e Construções|Funções e Construções]].

Vagas por nível da construção, iguais para todas (confirmado na rodada IP3-11; valores a testar):

| Nível | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|
| Vagas | 2 | 3 | 5 | 7 | 10 |

### Desemprego

- A cidade guarda uma **taxa de desemprego** (vagas de menos ou de sobra).
- Desemprego alto reduz a **renda média** e a **Felicidade**, aumenta a **Criminalidade** e empurra a **migração** para outras cidades.
- Vagas sobrando atraem migrantes (ver [[Sistema de População/Descrição|Sistema de População]]).

### Escravos

- **Não recebem salário.** Produzem para o dono (cidade, Rico ou jogador), que paga a comida e a Senzala.
- O Escravo do jogador segue a **Lista de Prioridades** que já existe ([[Sistema de NPC/Descrição|Sistema de NPC]]).
- Vagas Braçais sem trabalhador podem ser preenchidas com escravos comprados no Mercado de Escravos (preço base 200 + 4 × a melhor habilidade de Trabalho), pela cidade, pelos Ricos ou pelo jogador (ver [[Sistema de Escravidão/Descrição|Sistema de Escravidão]]).

### Guardas e soldados

- Os **guardas** são pagos pelo **Banco da Cidade**, com o mínimo definido pela lei da facção ([[Sistema de Crime e Justiça/Descrição|Crime e Justiça]]). Salário: 1,0 moeda por hora (rodada V2-3).
- Os **Esquadrões** recebem **soldo** do **Banco da Facção**: **8 moedas por dia** por Esquadrão, o salário de 1 trabalhador, porque 1 Esquadrão = 1 NPC alistado. O Esquadrão de Magos custa 3 vezes; o mercenário, 2 vezes por estação, adiantado (rodada V2-3).

---

## Propriedade

### Quem pode ser dono de uma construção

Novo atributo **Dono** nas construções, com quatro valores:

| Dono | Exemplos |
|---|---|
| **Cidade** | Mercado, Taverna, Hospital e produção básica |
| **Facção** | Quartel, Prisão, Casa da Moeda, Laboratório, Templo e Universidade |
| **Rico (NPC)** | Parte das construções de produção e de comércio: as "empresas" |
| **Jogador** | O que ele constrói ou compra |

### Empresas dos Ricos

- **30%** das construções de produção de cada cidade pertencem a Ricos (rodada V2-7).
- Em **LOD 2** só existe a **contagem de empresas** por cidade, sem rastrear cada Rico.
- Em **LOD 0**, o Rico dono é um NPC de verdade, com **Dinheiro individual**.
- O evento "perda de empresas" (ver [[Sistema de Classe Social/Descrição|Classe Social]]) tira a construção do Rico e a **devolve à cidade**.

### Comprar, vender e transferir

- O jogador pode **comprar** construções da Cidade ou de Ricos a um preço de mercado e **vender** as dele.
- A **conquista** de uma cidade transfere as construções da Cidade e da Facção. As dos Ricos **continuam deles**.

---

## Salários

### Quem paga e quanto

- O **dono** da construção paga os trabalhadores: **por dia** de jogo em LOD 0 e 1, **por estação** em LOD 2.
- O valor vem de uma **tabela por Função e nível**, multiplicada por um fator da **riqueza média** da cidade e do **desemprego**.
- O dinheiro sai do **caixa da construção** (lucro das vendas). Nas construções da **Cidade**, sai do **Banco da Cidade**.

**Tabela de salários** (rodada V2-3), por hora, pela habilidade que a receita usa:

| Trabalho | Moedas por hora | Por dia (turno de 8 h) |
|---|---|---|
| Braçal | 0,75 | 6 |
| Manufaturado | 1,0 | 8 |
| Especialista | 1,5 | 12 |
| Mago de Ofício | 2,5 | 20 |

- **Multiplicadores:**
  - +10% por nível da construção acima do 1;
  - gerente × 1,5;
  - riqueza da cidade de × 0,8 a × 1,2;
  - desemprego de × 0,9 (muito) a × 1,1 (falta gente).
- *Exemplo:* um Cozinheiro Civil num Cozinheiro nível 1 ganha 8 por dia, 128 por estação.

### Salários do jogador

O jogador escolhe entre três níveis de salário:

| Nível | Efeito |
|---|---|
| **Baixo** (× 0,75) | Reduz a Felicidade e a lealdade; aumenta a rotatividade |
| **Normal** (× 1) | Padrão |
| **Alto** (× 1,25) | Aumenta a produtividade e atrai trabalhadores melhores |

### A distribuição rotativa do Banco da Cidade

- Deixa de ser a fonte principal de dinheiro e vira **estabilizador**.
- A cidade só distribui dinheiro quando a **média por classe** cai abaixo de **3 dias da cesta mínima**, e paga até voltar a 5 dias, com teto de 10% do Banco por estação (rodada V2-7).
- O ciclo normal é: consumo → lojas → salários → consumo.

### Dinheiro dos NPCs

- O jogo guarda **só a riqueza média por classe e por cidade**.
- O **Dinheiro individual** existe apenas para **Importantes, Controlados e Ricos donos de empresas** (poucos). Isso mantém o desempenho.

### Mobilidade social (Civil → Rico)

- Em vez de "Dinheiro acima de um limiar", existem **vagas de Rico**, ligadas ao número de empresas.
- Quando a cidade prospera e abre uma **empresa nova**, um Civil de alta habilidade sobe para Rico.
- A queda é inversa: empresa perdida.
- O **jogador** sobe com o dinheiro próprio.

---

## Impostos e lojas

### Imposto

- Um **imposto único sobre receita** (salários e lucros), na taxa da lei de **Imposto** da facção, cobrado **por estação**: Baixo 10%, Normal 20% ou Alto 30% (rodada V2-7).
- A **cidade fica com 30%** para obras; o resto vai para o **Banco da Facção**.
- **Escravos não pagam.**
- A Tarifa do comércio continua separada ([[Sistema de Comércio/Descrição|Sistema de Comércio]]).

### Preço nas lojas do jogador

- O jogador fixa o preço dentro de uma **faixa de ±30%** em torno do **Mercado Local**.
- Fora da faixa, vende pouco ou nada (se for mais caro) ou perde lucro (se for mais barato).
- As lojas da Cidade compram a **90%** e vendem a **110%** do preço local (rodada V2-5).

### Insumos

A construção **compra** os insumos do **inventário compartilhado da cidade** pelo preço do Mercado Local, e paga do caixa dela. Assim o dinheiro entra no ciclo e o preço dos insumos influencia o lucro.

---

## Empresas do jogador

### Contratar

- O jogador contrata **NPCs desempregados** da cidade pelo painel da construção, ou pelo quadro da Taverna, até o limite de **vagas** do nível.
- **Controlados** podem ser **gerentes**.

### Falência

- Se o caixa não paga os salários, a **produção diminui** por estação (os trabalhadores saem aos poucos) e depois **para**.
- O jogador pode **injetar dinheiro**.
- Uma construção parada por muito tempo **volta à cidade** (leilão) e vira evento.

---

## Educação

A **Universidade** é uma construção da **Facção**, presente no canvas civil.

- **Melhora a Inteligência e o Trabalho Especialista** dos NPCs da cidade ao longo do tempo (educação).
- Dá **bônus aos Pesquisadores** do Laboratório ([[Sistema de Tecnologia/Descrição|Sistema de Tecnologia]]).
- Exige a tecnologia do ramo **Administração**.
- **[Refinar depois]** como um Controlado estuda nela e o tamanho do bônus.

---

## Parte técnica

### LOD

Em LOD 2, o cálculo é **por cidade, por Função e por estação**:

- trabalhadores × produtividade × tempo → **produção**;
- **massa salarial**;
- **impostos**;
- **desemprego**;
- nascimento de empresas e **vagas de Rico**.

Sem NPC a NPC. Em LOD 0 e 1, os salários são pagos por dia.

### Dados e save

- **Dados:** tabela de salários por Função e nível, vagas por nível, taxa de imposto e fração de empresas dos Ricos, em JSON.
- **Save:** Dono de cada construção, caixa das construções, riqueza média por classe, taxa de desemprego, número de empresas por cidade e Dinheiro dos Ricos individuais.
- **Fatia vertical:** uma cidade, poucas Funções (Fazendeiro, Cozinheiro, Lojista), salário fixo e sem Ricos.

### Ligação com outros sistemas

| Sistema | Ligação |
|---|---|
| Econômico | Ciclo do dinheiro, Mercado Local, estabilizador, imposto |
| Construção | Dono, vagas por nível, Gerente, Universidade |
| Produção | Tabela de Funções e Construções |
| NPC | Função, Dinheiro (individual só para alguns), Lista de Prioridades do Escravo |
| Classe Social | Mobilidade Civil → Rico por vagas; perda de empresas |
| Cidade | Desemprego, riqueza média, Banco da Cidade |
| Facção | Lei de Imposto, Banco da Facção, soldo dos Esquadrões |
| População | Emprego livre atrai migrantes |
| Crime e Justiça | Desemprego aumenta a Criminalidade; guardas pagos pela cidade |
| Eventos | Perda de empresas, falência, leilão |
| Tecnologia | Universidade e Pesquisadores |
| Comércio | Tarifa e mercadores |

### Perguntas em aberto

1. **Valores a testar:** vagas por nível, tabela de salários, faixa de preço (±30%), limiar do estabilizador, taxa de imposto e parte da cidade, fração de empresas dos Ricos.
2. **Funções:** nomes e categorias decididos na IP3-11; a habilidade que conta é a da receita ([[Sistema de Produção/Receitas|Receitas]]).
3. **Quem gerencia** as construções da Cidade em LOD 0 (NPC gerente próprio ou a cidade).
4. **Leilão:** como funciona quando uma construção volta à cidade.
5. **Universidade:** forma exata de estudo dos Controlados e tamanho do bônus.
