# O programa de testes do Codigo/ (rodada FL1-4), rodado pelo ctest.

add_executable(testes_do_codigo
    ExecutarTodosOsTestes.cpp
    TestesDoRelogio.cpp
    TestesDoMapa.cpp
    TestesDosDados.cpp
    TestesDaBuscaDeCaminho.cpp
    TestesDoMovimento.cpp
    TestesDaRotina.cpp
    TestesDaProducao.cpp
    TestesDaSimulacao.cpp
    TestesDoMarco8.cpp
    TestesDoSave.cpp
    TestesDoClima.cpp
    TestesDoTrabalho.cpp
    TestesDaPopulacao.cpp
    TestesDaCura.cpp
)

# Os testes do mapa usam a nlohmann/json para montar mapas com defeito.
target_link_libraries(testes_do_codigo PRIVATE regras_do_jogo nlohmann_json::nlohmann_json)

# A pasta desta CMakeLists, para os testes acharem os mapas de teste
# de qualquer pasta de onde forem rodados.
target_compile_definitions(testes_do_codigo PRIVATE PASTA_DOS_TESTES="${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(testes_do_codigo PRIVATE -Wall -Wextra)

if(MINGW)
    target_link_options(testes_do_codigo PRIVATE -static)
endif()

# Registra o programa no ctest. A pasta de trabalho é esta, para os testes
# acharem os arquivos de teste (por exemplo, os mapas pequenos).
add_test(NAME testes_do_codigo
         COMMAND testes_do_codigo
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
