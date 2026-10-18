// O stb_image é uma biblioteca de um arquivo só: o código dela entra no jogo
// por este arquivo, que liga a "implementação" antes de incluir o cabeçalho.
// Só aceitamos PNG, para o jogo ficar menor.

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>
