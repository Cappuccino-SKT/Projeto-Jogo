# O programa de testes do Jogo/: só as partes sem janela (a câmera).
# Usa a mesma ferramenta de testes do Codigo/ (ConferenciaDosTestes.h).

add_executable(testes_do_jogo
    ExecutarTestesDoJogo.cpp
    TestesDaCamera.cpp
    TestesDasAcoesDoTeclado.cpp
)

target_link_libraries(testes_do_jogo PRIVATE interface_do_jogo)
target_include_directories(testes_do_jogo PRIVATE ${PROJECT_SOURCE_DIR}/Codigo/Testes)
target_compile_options(testes_do_jogo PRIVATE -Wall -Wextra)

if(MINGW)
    target_link_options(testes_do_jogo PRIVATE -static)
endif()

add_test(NAME testes_do_jogo COMMAND testes_do_jogo)
