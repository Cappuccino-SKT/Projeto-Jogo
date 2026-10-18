# Codigo

Aqui ficam **as regras do jogo em C++**: tempo, economia, produção, NPCs. Cada regra segue a ficha do sistema em `Ficha Logica/`.

**A regra desta pasta:** nada aqui abre janela, desenha ou lê o mouse. Por isso:

- o teste sem tela (`jogo --simular 64 --semente 1`) roda só com esta pasta;
- cada regra pode ser testada sozinha, em segundos;
- a parte da tela (`Jogo/`) só pergunta para cá "o que aconteceu?" e desenha.

**Nomes:** português sem acento e sempre descritivos (`horasTrabalhadasNoCiclo`, não `h`).

**O que já tem:**

| Pasta | O que faz | Ficha |
|---|---|---|
| `Tempo/` | O relógio do mundo: ticks, data, fase do dia, viradas, velocidade e pausa; e o sorteio com semente (PCG32, marco 5) | [[Ficha Logica/Tempo\|Tempo]] |
| `Mapa/` | A leitura e a conferência do mapa do Tiled: camadas, passagem, lotes e áreas de recurso; e a busca de caminho A* (marco 4) | [[Ficha Logica/Mapa\|Mapa]] |
| `Npc/` | Os componentes do NPC e o movimento: criar, pedir caminho, atender os pedidos e andar a cada tick (marco 4). A rotina e a Utilidade: cada NPC escolhe a ação e vai até o lugar dela (marco 5). Usa a EnTT | [[Ficha Logica/NPC\|NPC]], [[Ficha Logica/IA\|IA]] |
| `Vila/` | A vila inicial: as construções nos lotes e os NPCs com casa e emprego (marco 5); e a `SimulacaoDaVila`, a vila inteira num tick, usada pelo jogo e pelo teste sem tela (marco 6) | [[Ficha Logica/IA\|IA]] |
| `Inventario/` | O inventário da cidade: quanto há de cada item, de quem é (Cidade ou jogador) e quanto cabe (marco 6) | [[Ficha Logica/Inventario\|Inventário]] |
| `Producao/` | O ciclo de produção, a escolha da receita da cidade, a fila de ordens do jogador e o campo dos Braçais (marco 6) | [[Ficha Logica/Producao\|Produção]] |
| `Economia/` | O dinheiro, o consumo da cesta mínima, os salários e a ordem do dia (marco 6) | [[Ficha Logica/Economia\|Economia]] |
| `Construcao/` | As vagas, a capacidade e o gerente de cada construção (marco 6) | [[Ficha Logica/Construcao\|Construção]] |
| `Dados/` | A leitura e a conferência dos dados (itens, construções, recursos, receitas, textos, regras), o preço base e o `--validar` | [[Ficha Logica/Itens\|Itens]], [[Ficha Logica/Recursos\|Recursos]], [[Ficha Logica/Receitas\|Receitas]], [[Ficha Logica/Construcao\|Construção]] |
| `Testes/` | O programa de testes (rodada FL1-4): `ctest --test-dir build` | A parte 8 de cada ficha |
