# O programa de testes do Jogo/: só as partes sem janela (a câmera, as teclas e o boneco).
# Usa a mesma ferramenta de testes do Codigo/ (ConferenciaDosTestes.h).

add_executable(testes_do_jogo
    ExecutarTestesDoJogo.cpp
    TestesDaCamera.cpp
    TestesDasAcoesDoTeclado.cpp
    TestesDoBoneco.cpp
)

target_link_libraries(testes_do_jogo PRIVATE interface_do_jogo)
target_include_directories(testes_do_jogo PRIVATE ${PROJECT_SOURCE_DIR}/Codigo/Testes)
target_compile_options(testes_do_jogo PRIVATE -Wall -Wextra)
# Para o teste ler o boneco_provisorio.json da fatia.
target_compile_definitions(testes_do_jogo PRIVATE PASTA_DOS_DADOS_DO_JOGO="${PROJECT_SOURCE_DIR}/Jogo/data")

if(MINGW)
    target_link_options(testes_do_jogo PRIVATE -static)
endif()

add_test(NAME testes_do_jogo COMMAND testes_do_jogo)
