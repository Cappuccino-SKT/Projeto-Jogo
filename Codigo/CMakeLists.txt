# Codigo/: as regras do jogo, numa biblioteca que nunca abre janela nem desenha.
# O Jogo/ e o programa de testes usam esta biblioteca.

add_library(regras_do_jogo STATIC
    Tempo/RelogioDoMundo.cpp
)

# Quem usa a biblioteca escreve, por exemplo, #include "Tempo/RelogioDoMundo.h".
target_include_directories(regras_do_jogo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(regras_do_jogo PRIVATE -Wall -Wextra)

# Os testes automáticos (rodada FL1-4).
add_subdirectory(Testes)
