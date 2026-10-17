# Jogo/: o programa que abre a janela (jogo.exe).

add_executable(jogo
    main.cpp
    TextoDaDataDoJogo.cpp
)

# O jogo usa as regras do Codigo/ e a SDL3 (baixada pelo CMakeLists.txt da raiz).
target_link_libraries(jogo PRIVATE regras_do_jogo SDL3::SDL3)

# Avisos do compilador ligados: ele aponta coisas suspeitas no nosso código.
target_compile_options(jogo PRIVATE -Wall -Wextra)

# No MinGW, coloca as bibliotecas do próprio g++ dentro do jogo.exe.
# Assim o programa abre com dois cliques, sem procurar DLLs do MinGW.
if(MINGW)
    target_link_options(jogo PRIVATE -static)
endif()
