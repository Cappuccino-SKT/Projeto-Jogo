# Codigo/: as regras do jogo, numa biblioteca que nunca abre janela nem desenha.
# O Jogo/ e o programa de testes usam esta biblioteca.

add_library(regras_do_jogo STATIC
    Tempo/RelogioDoMundo.cpp
    Mapa/MapaLocal.cpp
    Dados/ProblemasDosDados.cpp
    Dados/TabelaCsv.cpp
    Dados/TextosERegras.cpp
    Dados/Itens.cpp
    Dados/Recursos.cpp
    Dados/Receitas.cpp
    Dados/DadosDoJogo.cpp
)

# Quem usa a biblioteca escreve, por exemplo, #include "Tempo/RelogioDoMundo.h".
target_include_directories(regras_do_jogo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(regras_do_jogo PRIVATE -Wall -Wextra)

# A leitura do mapa e dos dados usa a nlohmann/json, mas só por dentro (PRIVATE).
target_link_libraries(regras_do_jogo PRIVATE nlohmann_json::nlohmann_json)

# Os testes automáticos (rodada FL1-4).
add_subdirectory(Testes)
