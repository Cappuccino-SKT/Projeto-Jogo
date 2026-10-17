# Codigo

Aqui ficam **as regras do jogo em C++**: tempo, economia, produção, NPCs. Cada regra segue a ficha do sistema em `Ficha Logica/`.

**A regra desta pasta:** nada aqui abre janela, desenha ou lê o mouse. Por isso:

- o teste sem tela (`jogo --simular 64 --semente 1`) roda só com esta pasta;
- cada regra pode ser testada sozinha, em segundos;
- a parte da tela (`Jogo/`) só pergunta para cá "o que aconteceu?" e desenha.

**Nomes:** português sem acento e sempre descritivos (`horasTrabalhadasNoCiclo`, não `h`).

Começa vazia: a primeira regra entra no marco 1 (o relógio do mundo).
