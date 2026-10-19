---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 6** (produção, inventário e consumo). Cobre **os dados das construções** (porte, vagas, capacidade, quem trabalha) e o desenho delas. As **obras** (construir e subir de nível) e a **manutenção** ficam para depois do marco 6 (FL6-1). Os pontos marcados com FL6 foram decididos na **rodada FL6** (2026-10-18, toda com "seguir sugestão").

# Ficha Lógica: Construção

## 1. O que faz

Diz, para cada **tipo** de construção, o porte, quantas vagas ela tem em cada nível, quanto ela guarda no inventário da cidade e que Trabalho as vagas usam. E guarda, para cada construção **da vila**, o nível, o dono, os empregados e o gerente.

Documento do sistema: [[Sistema de Construção/Descrição|Construção]], com as vagas e os donos do [[Sistema de Trabalho e Propriedade/Descrição|Trabalho e Propriedade]] e as Funções de [[Sistema de Produção/Funções e Construções|Funções e Construções]]. A vila inicial (os lotes, as portas) está na [[Ficha Logica/IA|ficha da IA]]; a produção, na [[Ficha Logica/Producao|ficha da Produção]].

## 2. Dados

### Definições: o `data/construcoes.json` completo

No marco 3 (FL3-5), o arquivo era só a lista dos IDs. No marco 6 ele ganha os campos de cada tipo:

```json
[
  { "id": "construcao.fazenda", "porte": "pequeno", "guarda": ["materia_prima"],
    "funcao": "fazendeiro" },
  { "id": "construcao.cozinheiro", "porte": "medio", "guarda": ["materia_prima", "bem_de_consumo"],
    "funcao": "cozinheiro" },
  { "id": "construcao.mercado", "porte": "medio", "guarda": ["bem_de_consumo"],
    "funcao": "lojista", "trabalho_sem_receita": "manufaturado" },
  { "id": "construcao.construtor", "porte": "pequeno", "guarda": ["materia_prima"],
    "funcao": "operario", "trabalho_sem_receita": "bracal" },
  { "id": "construcao.casa", "porte": "pequeno", "guarda": [], "moradores": 4 }
]
```

| Campo | O que é | Exemplo |
|---|---|---|
| `porte` | Pequeno, médio ou grande ([[Sistema de Construção/Descrição\|Construção]], V2-8) | `medio` |
| `guarda` | As categorias de item que ela soma à capacidade da cidade (Inventário, IT-4) | `["bem_de_consumo"]` |
| `funcao` | O nome da Função das vagas | `cozinheiro` |
| `trabalho_sem_receita` | O Trabalho de quem trabalha numa construção sem receita (Lojista, Operário). Nas outras, o Trabalho é o da receita | `manufaturado` |
| `moradores` | Só nas moradias: quantos moram em cada uma | 4 |

**As 8 da fatia** (7 na vila e a Pedreira, só nos dados, FL6-5):

| Construção | Porte | Guarda | Função | Trabalho |
|---|---|---|---|---|
| Fazenda | Pequeno | Matéria-prima | Fazendeiro | Pela receita (Braçal) |
| Posto de Coleta | Pequeno | Matéria-prima | Coletor | Pela receita (Braçal) |
| Cozinheiro | Médio | Matéria-prima e bem de consumo | Cozinheiro | Pela receita (Manufaturado) |
| Carpinteiro | Médio | Matéria-prima e bem de consumo | Carpinteiro | Pela receita (Manufaturado) |
| Mercado | Médio | Bem de consumo | Lojista | Manufaturado |
| Construtor | Pequeno | Matéria-prima | Operário | Braçal |
| Pedreira (fora da vila) | Médio | Matéria-prima e bem de consumo | Pedreiro | Pela receita (Manufaturado) |
| Casa | Pequeno | — | (moradia, 4 moradores) | — |

**No `data/regras.json`**, os números iguais para todos os tipos:

```json
"vagas_por_nivel": [2, 3, 5, 7, 10],
"capacidade_por_nivel": { "pequeno": 100, "medio": 200, "grande": 400 },
"obra_por_porte": {
  "pequeno": { "tabua": 10, "bloco_de_pedra": 10, "barra_de_ferro": 0, "horas": 40 },
  "medio":   { "tabua": 20, "bloco_de_pedra": 30, "barra_de_ferro": 0, "horas": 120 },
  "grande":  { "tabua": 40, "bloco_de_pedra": 80, "barra_de_ferro": 10, "horas": 400 }
}
```

- As **vagas** são a tabela geral (IP3-11); a **capacidade** é 100, 200 ou 400 × o nível (IT-4).
- A `obra_por_porte` (V2-8) só é lida e conferida no marco 6; ela é usada quando as obras entrarem.

### Estado: cada construção da vila

Com o que a ficha da IA já tem (o tipo, o lote, a porta, o dono):

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `nivel` | inteiro, de 1 a 5 | 1 (a Carpintaria do jogador, 2: FL6-2) | Sim |
| Os empregados | os NPCs com aquele trabalho | Elisa e Fábio | Sim (no NPC) |
| `indiceDoGerente` | o NPC, ou nenhum | Elisa | Não: recalculado |
| `vagas` | inteiro | 2 | Não: sai do nível |

O `vila_inicial.json` ganha o campo `"nivel"` em cada construção.

## 3. Quando roda

- **Ao abrir:** a leitura e a conferência do `construcoes.json` e dos números do `regras.json` (o `--validar` confere tudo).
- **Ao montar a vila e a cada dia** (antes dos salários): o gerente de cada construção (regra 4.2).
- A construção em si não roda nada por tick: quem produz é a [[Ficha Logica/Producao|Produção]].

## 4. Regras passo a passo

### 4.1 Conferir os dados

1. O `porte` é `pequeno`, `medio` ou `grande`; as categorias do `guarda` existem nos itens; o `trabalho_sem_receita`, se houver, é `bracal`, `manufaturado` ou `especialista`.
2. Uma construção de produção (que tem receitas no `receitas.csv`) não precisa do `trabalho_sem_receita`; uma sem receitas e sem `moradores` precisa.
3. Na vila inicial, os empregados de cada construção não passam das vagas do nível dela; a moradia não tem empregados.

### 4.2 O gerente (V3-8)

O empregado de **maior habilidade** no Trabalho da construção. No empate, o de menor número na vila. Ele recebe o salário × 1,5 ([[Ficha Logica/Economia|ficha da Economia]]).

### 4.3 Desenhar (no `Jogo/`)

1. Se existir a imagem `data/imagens/construcoes/<tipo>_nivel_<n>.png` (por exemplo, `construcoes/casa_nivel_1.png`), desenhe-a com a linha de baixo na linha de baixo do lote, e o resto subindo para fora dele (FA1-5, o [[Pipeline de Arte e Conteúdo/Guia da Arte|Guia da Arte]]).
2. Se não existir, desenhe o retângulo de cor do marco 5 (FL5-1). Assim, cada construção que o autor desenhar aparece no jogo sem mexer no código.
3. **Por cima de quem passa atrás:** a parte da imagem acima do lote (o telhado) é desenhada **depois** dos NPCs que estão mais acima na tela, como a copa das árvores. Por isso, as construções entram na mesma ordem de desenho dos NPCs (pela linha de baixo).

## 5. Contas

**Vagas e capacidade da vila no primeiro dia:**

| Construção | Nível | Vagas | Capacidade |
|---|---|---|---|
| Fazenda, Posto de Coleta, Construtor | 1 | 2 cada | 100 de matéria-prima cada |
| Cozinheiro | 1 | 2 | 200 de matéria-prima e 200 de bem de consumo |
| Mercado | 1 | 2 | 200 de bem de consumo |
| Carpintaria do jogador | 2 (FL6-2) | 3 (2 ocupadas) | 400 de matéria-prima e 400 de bem de consumo |
| **Total** | | | **900 de matéria-prima e 800 de bem de consumo** |

## 6. Conversa com outros sistemas

| Sistema | O que a Construção entrega | O que recebe |
|---|---|---|
| Inventário | A capacidade por categoria | — |
| Produção | As vagas, o gerente, o nível (o bônus de 10% por nível) | — |
| Economia | O dono (quem paga), o gerente (× 1,5), o nível (+10% de salário) | — |
| IA (marco 5) | A porta e o lote | Quem está dentro |
| Obras (depois) | O custo e as horas por porte | — |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `DefinicaoDeConstrucao` | Os campos do `construcoes.json` (com `porte`, `categoriasQueGuarda`, `funcao`, `trabalhoSemReceita`, `moradores`) |
| `CatalogoDeConstrucoes` | Já existe (marco 3); ganha as definições completas |
| `RegrasDasConstrucoes` | As vagas, a capacidade e a obra por porte, do `regras.json` |
| `calcularVagas(nivel)` e `calcularCapacidade(porte, nivel)` | As contas da parte 5 |
| `ConstrucaoDaVila::nivel` e `escolherGerente(...)` | O nível (na vila) e a regra 4.2 |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Vagas no nível 1, 2 e 5 | 2, 3 e 10 |
| Capacidade de uma construção média no nível 2 | 400 |
| O `construcoes.json` da fatia | 8 tipos, sem erro |
| Um porte `enorme` | Erro |
| O Mercado sem `trabalho_sem_receita` | Erro |
| 3 empregados numa Fazenda nível 1 | Erro: 2 vagas |
| Dois empregados com habilidade 50 e 60 | O de 60 é o gerente |
| Empate de habilidade | O de menor número na vila |
| A capacidade da vila da fatia | 900 de matéria-prima e 800 de bem de consumo |

## 9. Fora da fatia (e fora do marco 6)

- **No marco 8 (parte 10):** as obras e a manutenção.
- Antes da parte 10: **as obras** (construir num lote livre, subir de nível, as horas dos Operários, o Engenheiro) e a **manutenção por estação** (FL6-1). A **pedra** delas vem dos 150 Blocos de Pedra do estoque inicial (FL6-5).
- A Vida das construções e a tempestade; a demolição; a compra e a venda; o leilão; a loja ilegal.
- Os donos Facção e Rico (a fatia só tem a Cidade e o jogador).
- A aparência nos níveis 3 e 5.

---

## 10. Marco 8: as obras e a manutenção

> Escrita em 2026-10-19. Decidida na rodada **FL8** (2026-10-19, toda com "seguir sugestão"; ver a parte 8 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]). O painel e o botão estão na parte 10 da [[Ficha Logica/Interface|Interface]].

### 10.1 O que muda

| | Até o marco 7 | No marco 8 |
|---|---|---|
| Os 2 Operários do Construtor | Recebem e ficam na construção | Fazem a manutenção e as obras |
| O lote 5 (pequeno, o único livre) | Vazio | O jogador pode encomendar uma construção nele |
| Subir de nível | — | Só as construções do jogador, até o nível 3 (FL8-4) |
| Os 150 Blocos de Pedra | Parados no estoque | Gastos pela manutenção e pelas obras |

**Quem encomenda** (FL8-3): só o jogador. A Cidade não faz obras novas na fatia (a população não cresce, então nunca falta moradia); ela só faz a manutenção.

### 10.2 Dados

Nada novo nos arquivos: a `obra_por_porte` do `regras.json` (marco 6) já tem as Tábuas, os Blocos, o Ferro e as horas. Novos no `regras.json`:

```json
"obras": {
  "nivel_maximo_com_o_engenheiro": 3,
  "manutencao_em_porcento": 5,
  "producao_com_manutencao_atrasada_em_porcento": 75
}
```

A hora de obra custa o salário Braçal do `salario_por_hora_em_centesimos` (75 centésimos = 0,75); o total é arredondado para décimos (meio para cima), como os salários.

**Estado novo** (vai no save, marco 9):

| Campo | Onde | Exemplo |
|---|---|---|
| `filaDeObras` | Na vila | A manutenção do Cozinheiro, depois a Fazenda do lote 5 |
| Cada obra: o tipo (nova, subir de nível, manutenção), o lote, o tipo de construção, o nível de destino | Na fila | Nova, lote 5, Fazenda, nível 1 |
| Cada obra: `horasDeObraQueFaltam` (em centésimos de hora, para a velocidade não arredondar) | Na fila | 4000 = 40 h |
| `manutencaoAtrasada` | Em cada construção | Sim / não |

### 10.3 Quando roda

- **A cada hora** (junto com o ciclo da produção): os Operários presentes na obra da vez fazem horas (regra 10.5).
- **No primeiro dia de cada estação, à meia-noite** (um passo novo na ordem do dia, `"manutencao"`, antes do `"consumo"`): entra a manutenção de cada construção (regra 10.6).
- **Ao aplicar o pedido "Encomendar uma obra"** (parte 10.8 da Interface): a regra 10.4.

### 10.4 Encomendar uma obra (o jogador)

1. **Quanto custa** (V2-8): o custo do porte no nível 1 × o nível de destino. Uma construção nova nasce no nível 1.
2. **Pode?** O lote está livre (ou a construção é do jogador e não tem obra na fila); o nível de destino ≤ o máximo do Engenheiro (FL8-4: o gerente do Construtor conta com Inteligência 50, até o nível 3); o jogador tem o dinheiro.
3. **Os materiais:** saem do estoque do jogador; o que faltar, ele compra da loja da cidade a 110% do preço local (parte 11 da [[Ficha Logica/Economia|Economia]]). Tudo sai do inventário na hora (Fatia Vertical, regra 4.11).
4. **As horas:** o jogador paga ao Banco as horas de obra × o salário da hora de obra, na hora (os Operários já recebem do Banco).
5. A obra entra no **fim da fila**. No lote livre, aparece a obra (a imagem de teste: um quadrado marrom com o contorno do lote), e ele já bloqueia a passagem (regra 4.2 do [[Ficha Logica/Mapa|Mapa]]).

Uma obra não se cancela na fatia.

### 10.5 A obra (a cada hora)

1. **A obra da vez** é a primeira da fila. Manutenções vêm antes das obras novas e das subidas (FL8-5).
2. **Os Operários** trabalham no Construtor, como hoje, e as horas deles valem para a obra da vez (no marco 8 eles **não andam até o lote**; ir à obra fica para quando houver arte e animação de obra).
3. Cada Operário **presente** soma 1 hora × a velocidade dele (a mesma da produção, regra 4.2 da [[Ficha Logica/Producao|Produção]]). O gerente do Construtor (um dos 2) é o Engenheiro e também soma horas.
4. Com `horasDeObraQueFaltam` ≤ 0, a obra fica **pronta**:
   - nova: a construção nasce no lote, do jogador, no nível 1, sem empregados e sem ordens; a passagem é atualizada;
   - subir de nível: o nível sobe, e com ele as vagas e a capacidade; a construção produziu normalmente durante a obra;
   - manutenção: `manutencaoAtrasada` volta a não.
5. Uma notificação: "Obra pronta: Fazenda (lote 5)."

### 10.6 A manutenção (no começo de cada estação) (FL8-5)

1. Para cada construção, a manutenção é **5% dos materiais do nível atual** (o nível 1 × o nível; cada material arredondado para cima, e no mínimo 1 Tábua) e **5% das horas** (arredondado para cima).
2. **Se a anterior não ficou pronta**, a construção fica com `manutencaoAtrasada` = sim, e a nova não entra (a velha continua na fila).
3. **Da Cidade:** os Blocos saem do estoque da Cidade; as Tábuas, a Cidade compra do estoque do jogador pelo preço do jogador (o dinheiro sai do Banco para o jogador). Se o jogador não tiver Tábuas suficientes, a manutenção entra na fila **esperando material** e tenta de novo a cada dia, à meia-noite; enquanto espera, os Operários passam para a próxima obra.
4. **Do jogador:** os materiais saem do estoque dele (o que faltar, ele compra a 110%), e ele paga as horas ao Banco, como numa obra. Sem dinheiro, ela espera como no passo 3.
5. **Com a manutenção atrasada**, a construção produz **75%** (o ciclo da produção dura 4/3 das horas) até a manutenção ficar pronta. Uma Casa atrasada não muda nada na fatia.

### 10.7 Contas

| Conta | Valor |
|---|---|
| Fazenda no lote 5 (pequeno, nível 1) | 10 Tábuas + 10 Blocos + 40 h. Com as Tábuas do jogador, 10 Blocos a 2,0 (o local de 1,8 × 110%) = 20,0 e 40 h × 0,75 = 30,0: **50 moedas**. Com os 2 Operários do Construtor (um deles é o gerente), 2 × 8 h por dia, velocidade 1: 40 ÷ 16 = **2,5 dias** |
| Carpintaria do nível 2 para o 3 (média) | 60 Tábuas + 90 Blocos + 360 h. Comprando os Blocos: 180,0 + 270,0 = **450 moedas** e as 60 Tábuas; 360 ÷ 16 = **22,5 dias** |
| Manutenção de uma construção pequena (nível 1) | 1 Tábua + 1 Bloco (5% de 10 = 0,5, para cima) + 2 h |
| Manutenção de uma média (nível 1) | 1 Tábua + 2 Blocos + 6 h |
| Manutenção da Carpintaria (média, nível 2) | 2 Tábuas + 3 Blocos + 12 h, pagas pelo jogador (9,0 moedas das horas) |
| A Cidade, por estação | 7 pequenas + 2 médias: **9 Tábuas, 11 Blocos e 26 h** (1,6 dia dos Operários). Os 150 Blocos duram cerca de 13 estações (3 anos) |

### 10.8 Nomes no código

| Nome | O que é |
|---|---|
| `ObraNaFila`, `TipoDeObra` (`ConstrucaoNova`, `SubirDeNivel`, `Manutencao`) | Uma obra da fila |
| `ObrasDaVila` | A fila e as regras 10.4 a 10.6 |
| `custoDaObra(porte, nivelDeDestino)` | Os materiais e as horas (regra 10.4, passo 1) |
| `custoDaManutencao(porte, nivelAtual)` | Regra 10.6, passo 1 |
| `RegrasDasObras` | O bloco `"obras"` do `regras.json` |

### 10.9 Testes

| Situação | Resultado esperado |
|---|---|
| `custoDaObra(medio, 3)` | 60 Tábuas, 90 Blocos, 360 h |
| `custoDaManutencao(pequeno, 1)` | 1 Tábua, 1 Bloco, 2 h |
| `custoDaManutencao(medio, 2)` | 2 Tábuas, 3 Blocos, 12 h |
| Encomendar a Fazenda no lote 5 com 50 moedas e 10 Tábuas | Paga; a obra entra na fila; o lote fica ocupado |
| A mesma obra com 49,9 moedas | Descartada; nada muda |
| Subir a Carpintaria para o nível 4 | Recusado (o Engenheiro só vai até o 3) |
| 2 Operários presentes por 8 horas, velocidade 1, numa obra de 40 h | Faltam 24 h |
| A obra chega a 0 h | A construção nasce, a passagem do lote muda, a notificação sai |
| O começo do Verão com o jogador sem Tábuas | A manutenção da Cidade espera material; as obras do jogador andam |
| Uma estação inteira sem a manutenção pronta | `manutencaoAtrasada`; o ciclo da construção dura 4/3 |
| 64 dias sem tela, sem nenhum pedido | Passa nas 10 sementes (a manutenção muda o equilíbrio: conferir de novo) |
| O dinheiro total num dia com obra | Igual: o que o jogador paga, o Banco recebe |
