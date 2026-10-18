# glad (OpenGL 3.3)

O **glad** acha as funções do OpenGL na placa de vídeo. Os arquivos desta pasta foram **gerados uma vez** e guardados aqui, porque o gerador precisa do Python com a biblioteca jinja2. Assim, compilar o jogo não precisa de nada além do g++ e do CMake.

- Gerador: glad2 versão 2.0.8.
- Configuração: OpenGL 3.3 core, sem extensões, em C.
- Comando usado (num ambiente Python à parte, com `pip install glad2==2.0.8`):

```
python -m glad --api "gl:core=3.3" --extensions "" --reproducible --out-path Jogo/externo/glad c
```

Licença: WTFPL ou CC0, e Apache 2.0 nas especificações da Khronos (ver o topo de `src/gl.c`).

Não editar estes arquivos à mão: se precisar de outra versão do OpenGL, gerar de novo.
