# O programa de testes do Codigo/ (rodada FL1-4), rodado pelo ctest.

add_executable(testes_do_codigo
    ExecutarTodosOsTestes.cpp
    TestesDoRelogio.cpp
)

target_link_libraries(testes_do_codigo PRIVATE regras_do_jogo)
target_compile_options(testes_do_codigo PRIVATE -Wall -Wextra)

if(MINGW)
    target_link_options(testes_do_codigo PRIVATE -static)
endif()

# Registra o programa no ctest. A pasta de trabalho é esta, para os testes
# acharem os arquivos de teste (por exemplo, os mapas pequenos).
add_test(NAME testes_do_codigo
         COMMAND testes_do_codigo
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
