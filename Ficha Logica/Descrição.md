---
tags: [ficha-logica]
---

# Ficha Lógica

Esta pasta traduz o design de `Documentos/` em **lógica de programação**: que dados existem, quando cada regra roda, o passo a passo de cada regra e como testar. É a ponte entre o documento do sistema e o código em `Codigo/`.

**O caminho de cada sistema:** documento em `Documentos/` → ficha nesta pasta → regras em `Codigo/` → uso no programa em `Jogo/`.

**Quando fazer:** um sistema vira ficha **pouco antes do marco que o usa** (decisão de 2026-10-17), não todos de uma vez. Assim a ficha já nasce com o que o código anterior ensinou. Um problema de design achado aqui vira pergunta numa rodada, e o documento do sistema é atualizado.

O mapa geral da fatia (entidades, arquivos de dados, ordem do dia, marcos) está em [[Ficha Logica/Fatia Vertical|Ficha Lógica: Fatia Vertical]].

## Ordem das fichas, por marco

| Marco | Fichas a escrever antes |
|---|---|
| 0. Ambiente | Nenhuma (só abre uma janela) |
| 1. Janela, loop, relógio e mapa | [[Ficha Logica/Tempo\|Tempo]] (relógio fixo); [[Ficha Logica/Mapa\|Mapa]] (tiles e camadas). Escritas em 2026-10-17 |
| 2. Câmera e entrada | [[Ficha Logica/Interface\|Interface]] (só câmera, zoom e teclas). Escrita em 2026-10-18 |
| 3. Carregar os dados | [[Ficha Logica/Itens\|Itens]] (com a ordem de leitura e o `--validar`), [[Ficha Logica/Recursos\|Recursos]] e [[Ficha Logica/Receitas\|Receitas]] (com o preço base). Escritas em 2026-10-18 |
| 4. NPC andando | [[Ficha Logica/Mapa\|Mapa]] (busca de caminho, regra 4.4); [[Ficha Logica/NPC\|NPC]] (só posição e movimento). Escritas em 2026-10-18 |
| 5. Rotina | [[Ficha Logica/IA\|IA]] (a vila inicial, a rotina e a Utilidade). Escrita em 2026-10-18 |
| 6. Produção, inventário e consumo | [[Ficha Logica/Producao\|Produção]], [[Ficha Logica/Inventario\|Inventário]], [[Ficha Logica/Economia\|Economia]] (consumo e salários, com o Trabalho e Propriedade) e [[Ficha Logica/Construcao\|Construção]] (os dados). Escritas em 2026-10-18 |
| 7. Simulação sem tela de 1 ano | [[Ficha Logica/Economia\|Economia]] (parte 10: Mercado Local, gasto extra, imposto e estabilizador), [[Ficha Logica/Producao\|Produção]] (parte 10: a previsão do consumo e a folga), [[Ficha Logica/Felicidade\|Felicidade]], [[Ficha Logica/Eventos\|Eventos]] e [[Ficha Logica/Simulacao\|Simulação sem tela]]. Escritas em 2026-10-18 |
| 8. Interface básica | Interface (HUD e painéis) |
| 9. Salvar e carregar | Save (a partir da Arquitetura) |

## Modelo de cada ficha

Cada ficha se chama pelo sistema (`Ficha Logica/Tempo.md`, `Ficha Logica/Producao.md`...) e tem estas partes, nesta ordem:

1. **O que faz:** duas ou três linhas e o link para o documento do sistema.
2. **Dados:** uma tabela com campo, tipo, unidade e exemplo, separada em:
   - **Definições:** lidas dos arquivos de dados e que não mudam durante o jogo;
   - **Estado:** o que muda durante o jogo e vai no save.
3. **Quando roda:** a cada tick, hora, dia ou estação, e a posição na ordem do dia.
4. **Regras passo a passo:** em português simples e numerado, como uma receita de bolo. Cada passo diz o que lê, o que calcula e o que grava.
5. **Contas:** cada fórmula com um exemplo de números do jogo.
6. **Conversa com outros sistemas:** o que lê de cada um e o que entrega para cada um.
7. **Nomes no código:** os nomes das estruturas e funções que o código vai usar, já descritivos (rodada FT2-4).
8. **Testes:** entrada → resultado esperado, para virar teste automático.
9. **Fora da fatia:** o que o documento do sistema tem e esta versão ainda não faz.

### Exemplo curto (trecho da futura ficha do Tempo)

**Dados (estado):**

| Campo | Tipo | Unidade | Exemplo |
|---|---|---|---|
| `tickAtualDaPartida` | inteiro de 64 bits | ticks | 19.200 (início do dia 2) |
| `velocidadeDoJogo` | inteiro | 0, 1, 2 ou 4 | 1 |

**Regras passo a passo (a cada quadro da tela):**

1. Somar ao acumulador o tempo real que passou × `velocidadeDoJogo`.
2. Enquanto o acumulador tiver 1/20 de segundo ou mais: avançar 1 tick e tirar 1/20 de segundo do acumulador.
3. A cada tick: se `tickAtualDaPartida` for múltiplo de 800, rodar o que é "a cada hora"; se for múltiplo de 19.200, rodar a sequência do dia.

**Contas:** hora do dia = (`tickAtualDaPartida` ÷ 800) resto 24. *Exemplo:* tick 27.200 → 27.200 ÷ 800 = 34 → 34 resto 24 = **10h** do dia 2.

**Nomes no código:** `RelogioDoMundo`, `calcularTicksParaAvancarPeloTempoReal`, `horaDoDiaAtual`.

**Testes:** com a velocidade 1, 40 segundos reais avançam 800 ticks (1 hora); com a velocidade 0, nada avança.
