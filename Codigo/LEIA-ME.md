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
| `Vila/` | A vila inicial: as construções nos lotes e os NPCs com casa e emprego (marco 5) | [[Ficha Logica/IA\|IA]] |
| `Dados/` | A leitura e a conferência dos dados (itens, construções, recursos, receitas, textos, regras), o preço base e o `--validar` | [[Ficha Logica/Itens\|Itens]], [[Ficha Logica/Recursos\|Recursos]], [[Ficha Logica/Receitas\|Receitas]] |
| `Testes/` | O programa de testes (rodada FL1-4): `ctest --test-dir build` | A parte 8 de cada ficha |
