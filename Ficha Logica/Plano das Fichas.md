> Plano para escrever as fichas lógicas depois da fatia vertical. É só documentação: nada de código até o autor decidir. Proposta de 2026-10-19, para o autor aprovar ou mudar.

## 1. Como medi

- **Documentos completos:** quase todos os sistemas já estão decididos. As marcações [Em aberto] e [Refinar depois] só aparecem na legenda do topo; a População tem duas de verdade (os valores da vida e das condições de nível).
- **O critério que sobra é a dependência:** um sistema entra antes quando conversa com o que já tem ficha (Tempo, Produção, Economia, NPC, IA...) e quase não precisa de sistemas sem ficha.
- **A fatia como base:** cada ficha nova começa pela parte 9 (**Fora da fatia**) das fichas atuais e cresce a partir da vila de 15 Civis.

## 2. Simulação de mesa (nova parte das fichas)

Para achar erros antes do código, cada ficha nova ganha uma **parte 10: Simulação de mesa**:

1. **Cenário:** a vila da fatia com o sistema novo ligado (quantas pessoas, construções e dinheiro no começo).
2. **Passo a passo de alguns dias** numa tabela: o que cada regra lê, calcula e grava, com os números.
3. **Verificações:** o que nunca pode acontecer. Exemplos: dinheiro surgindo do nada, um número negativo, gente sem casa sem motivo, duas regras disputando a mesma coisa.
4. **Casos extremos:** vila vazia, estoque zerado, valor no limite.
5. **O que achou:** cada problema vira pergunta numa rodada antes do código.

## 3. Ordem proposta

### Fase A — completar a vila (prioridade alta)

São os sistemas que mais se ligam ao que já existe e menos dependem do que falta.

| # | Ficha | Por que agora | Precisa de |
|---|---|---|---|
| A1 | **[[Ficha Logica/Clima\|Clima e Estações]]** (escrita em 2026-10-19; rodada CL1) | O Tempo já conta as estações; o clima mexe na Produção, nos Recursos (colheita) e na Felicidade, que já têm ficha | Tempo, Produção, Recursos, Felicidade |
| A2 | **Trabalho e Propriedade** (completo) | A Economia já usa parte dele (salário e emprego); falta contratar, demitir, dono de lote e aluguel | Economia, Produção, NPC |
| A3 | **População** | Nascer, morrer, migrar e onde morar: a vila deixa de ter 15 pessoas fixas | Construção, Trabalho (A2), Clima (A1) |
| A4 | **Classe Social** | Moradia, salário e consumo por classe; liga a População ao dinheiro | População (A3), Trabalho (A2), Felicidade |
| A5 | **Cidade** | Níveis da cidade e as condições para subir; fecha a vila como um todo | População (A3), Classe (A4), Construção |
| A6 | **Cura** | Documento curto; doença e morte por causas da População | População (A3), Itens |

### Fase B — várias vilas (prioridade média)

| # | Ficha | Precisa de |
|---|---|---|
| B1 | **Viagem** | Mapa, Tempo, Clima (A1) |
| B2 | **Comércio** | Economia, Viagem (B1) |
| B3 | **Notícias** | Eventos, Viagem (B1) |
| B4 | **Reputação** | Comércio (B2), Crime (C1) em parte |
| B5 | **Tecnologia** | Produção, Construção |

### Fase C — sociedade (prioridade média e baixa)

Crime e Justiça; Religião; Cultura; Facção. São documentos bem completos, mas cada um se liga a cinco ou mais sistemas sem ficha. Ficam depois das fases A e B.

### Fase D — conflito (prioridade baixa)

Combate; Batalha; Guerra; Diplomacia; Magia; Escravidão; Companheiros e Diálogo. São os mais longos e os que mais dependem dos outros (Guerra se liga a 18 sistemas).

### Fase E — a partida completa (por último)

Mundo e Estado Inicial; Início de Jogo; Objetivo e Progressão. Juntam tudo, então só fazem sentido no fim.

### Fora das fichas

Pipeline de Arte e Conteúdo (não é regra de jogo) e Interface (cresce junto com cada sistema).

## 4. Primeiro passo sugerido

Escrever a ficha do **Clima e Estações (A1)** já com a Simulação de mesa. Ela é curta, só depende de fichas prontas e serve para testar o formato novo antes das fichas maiores (População e Trabalho).
