# Jogo/: o programa que abre a janela (jogo.exe).

add_executable(jogo
    main.cpp
)

# O jogo usa a SDL3 (baixada pelo CMakeLists.txt da raiz).
target_link_libraries(jogo PRIVATE SDL3::SDL3)

# Avisos do compilador ligados: ele aponta coisas suspeitas no nosso código.
target_compile_options(jogo PRIVATE -Wall -Wextra)

# No MinGW, coloca as bibliotecas do próprio g++ dentro do jogo.exe.
# Assim o programa abre com dois cliques, sem procurar DLLs do MinGW.
if(MINGW)
    target_link_options(jogo PRIVATE -static)
endif()
