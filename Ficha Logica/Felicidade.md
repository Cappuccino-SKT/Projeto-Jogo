---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-18 para o **marco 7** (a simulação sem tela de 1 ano). Os pontos marcados com FL7 vêm da **rodada FL7**, aplicada em 2026-10-18, toda com "seguir sugestão" (as decisões estão na parte 8 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]).

# Ficha Lógica: Felicidade

## 1. O que faz

Todo dia, a vila calcula a **Felicidade dos Civis** (de 0 a 100): um alvo, que sai de 4 fatores, e um passo de 10% em direção a ele. No marco 7, a Felicidade é **medida e mostrada** (no resumo do dia e no relatório do teste de 1 ano) e entra nos eventos condicionais; ela ainda não muda a produção (FL7-7).

Documento do sistema: [[Sistema de Felicidade e Estabilidade/Descrição|Felicidade e Estabilidade]] (os fatores, a inércia, as faixas). Na fatia, só 4 fatores (FT1-2), com as contas da regra 4.8 da [[Ficha Logica/Fatia Vertical|Fatia Vertical]]. Sem revolta nem Controle.

**A divisão das pastas:** em `Codigo/Economia/` (junto da classe e do dinheiro), sem janela.

## 2. Dados

### Definições

No `regras.json`:

```json
"felicidade": {
  "inicial": 50,
  "base_do_alvo": 50,
  "passo_por_dia_em_porcento": 10,
  "consumo": { "sem_comida": -25, "materiais_em_deficit": -5, "gasto_extra_maximo": 5 },
  "desemprego": { "sem_desemprego": 10, "por_ponto_da_taxa": -20, "minimo": -10, "maximo": 10 },
  "moradia": { "sem_casa": -10 },
  "eventos": { "minimo": -20, "maximo": 20 }
}
```

### Estado

| Campo | Tipo | Exemplo | Vai no save? |
|---|---|---|---|
| `felicidadeDosCivisEmDecimos` | inteiro, de 0 a 1.000 (décimos de ponto) | 587 (58,7) | Sim |
| `alvoDaFelicidadeEmDecimos` | inteiro | 587 | Não: recalculado |
| Os 4 fatores do dia | inteiros, em décimos | consumo 27, desemprego 60, moradia 0, eventos 0 | Não: só para o relatório |

**Por que em décimos:** o passo de 10% de uma diferença pequena (por exemplo, 3 pontos) daria 0,3, que um inteiro em pontos perderia. Em décimos, tudo fica inteiro e igual em qualquer computador.

## 3. Quando roda

À meia-noite, no passo **"felicidade"** do `ordem_do_dia` (o 5º): depois do consumo, dos salários, do Mercado Local e do estabilizador, porque a Felicidade vem depois de tudo que a afeta (Fatia Vertical, parte 3). Os eventos (o 6º passo) usam a Felicidade nova.

## 4. Regras passo a passo

### 4.1 Os 4 fatores

1. **Consumo atendido** = −25 × (Civis que ficaram sem comida hoje ÷ Civis) − 5 se os Materiais Básicos estão em déficit + 5 × (gasto extra de hoje ÷ gasto da cesta de hoje), com o último pedaço no máximo +5.
2. **Desemprego e renda** = +10 − 20 × a taxa de desemprego (de 0 a 1), preso entre −10 e +10.
3. **Moradia** = −10 × (Civis sem casa ÷ Civis). Na fatia, os 15 têm casa: 0.
4. **Leis × cultura** (5º fator, com as facções): de −30 a +10, pela [[Ficha Logica/Cultura|Cultura]] (regras 4.2 e 4.3). Na vila sem facção, 0.
5. **Eventos duradouros** = a soma dos modificadores de Felicidade dos eventos ativos ([[Ficha Logica/Eventos|ficha dos Eventos]]), presa entre −20 e +20.

### 4.2 O alvo e o passo

1. **Alvo** = 50 + a soma dos 4 fatores, preso entre 0 e 100.
2. **Felicidade nova** = Felicidade + 10% × (alvo − Felicidade), em décimos, arredondando na direção do alvo (assim ela sempre chega ao alvo, e não para a 0,1 dele).

### 4.3 As faixas (só para mostrar, no marco 7)

| Faixa | Valor | No marco 7 |
|---|---|---|
| Contente | acima de 70 | Escrita no resumo do dia |
| Estável | 40 a 70 | Escrita no resumo do dia |
| Inquieta | 25 a 40 | Escrita no resumo do dia |
| Protesto | 10 a 25 | Escrita no resumo do dia |
| Revolta | abaixo de 10 | Escrita no resumo do dia |

Os efeitos das faixas (a produtividade, os migrantes, as greves, a revolta) ficam para depois da fatia (FL7-7).

## 5. Contas

**O primeiro dia da fatia:** ninguém ficou sem comida, os Materiais não estão em déficit, e o gasto extra é 0 (a renda do dia anterior é 0): consumo = 0. Desemprego: 3 em 15 = 0,2, então +10 − 4 = +6. Moradia 0, eventos 0. Alvo = 56. Felicidade = 50 + 10% × 6 = 50,6.

**Um dia comum, depois:** o gasto extra de 24 sobre uma cesta de 44 dá +2,7. Alvo = 50 + 2,7 + 6 = 58,7. Em uns 20 dias, a Felicidade chega perto de 58,7 (Estável).

**Uma semana de fome:** com 5 dos 15 sem comida, o consumo é −8,3, e o evento Fome (Felicidade −10) entra no terceiro dia. Alvo = 50 − 8,3 + 6 − 10 = 37,7 (Inquieta).

## 6. Conversa com outros sistemas

| Sistema | O que a Felicidade lê | O que entrega |
|---|---|---|
| Economia | Quem ficou sem comida, os dias em déficit de Materiais, o gasto extra e o da cesta | — |
| IA (marco 5) | Os desempregados, quem não tem casa | — |
| Eventos | Os modificadores dos eventos ativos | A Felicidade (condição de eventos) |
| Simulação sem tela | — | A Felicidade e os 4 fatores no relatório |
| Interface (marco 8) | — | O valor e a faixa no painel da cidade |

## 7. Nomes no código

Em `Codigo/Economia/`:

| Nome | O que é |
|---|---|
| `RegrasDaFelicidade` | Os números do `regras.json` |
| `FelicidadeDaClasse` | O valor, o alvo e os 4 fatores do dia |
| `calcularFatoresDaFelicidade(...)` | A regra 4.1 |
| `darOPassoDaFelicidade(felicidade, alvo, regras)` | A regra 4.2 |
| `nomeDaFaixaDaFelicidade(valor)` | A regra 4.3 |

## 8. Testes

| Entrada | Resultado esperado |
|---|---|
| Os fatores do primeiro dia da fatia | Consumo 0, desemprego +6, moradia 0, eventos 0; alvo 56 |
| Felicidade 50,0 e alvo 56,0 | 50,6 |
| Felicidade 58,6 e alvo 58,7 | 58,7 (chega ao alvo) |
| 5 de 15 sem comida | Consumo −8,3 |
| Gasto extra 3 vezes a cesta | O pedaço do gasto extra fica em +5 |
| Eventos somando +30 | +20 (preso) |
| Alvo abaixo de 0 | 0 |
| 64 dias sem tela | A Felicidade fica entre 40 e 70 (Estável) se não houver fome |

## 9. Fora da fatia (e fora do marco 7)

- Os outros 7 fatores (leis × cultura, criminalidade, conscrição, guerra, imposto, religião, repressão) e as outras 4 classes.
- Os efeitos das faixas, a Estabilidade, o Controle e a revolta.
- A Felicidade individual dos Controlados e Importantes.
