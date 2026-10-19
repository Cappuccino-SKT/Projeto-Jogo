# Jogo/: o programa que abre a janela (jogo.exe).

# glad: acha as funções do OpenGL 3.3 na placa de vídeo. Os arquivos foram
# gerados uma vez e ficam em externo/glad (ver o LEIA-ME de lá).
add_library(glad STATIC externo/glad/src/gl.c)
target_include_directories(glad PUBLIC externo/glad/include)

# A parte da interface que não abre janela (a câmera, a tabela de ações do
# teclado e as contas do boneco), separada para poder ser testada pelo
# testes_do_jogo. Usa só os cabeçalhos da SDL (os nomes das teclas), sem
# ligar a SDL; as regras do Codigo/ (os NPCs); e a nlohmann/json, para ler o
# arquivo do boneco.
add_library(interface_do_jogo STATIC CameraDoMapa.cpp AcoesDoTeclado.cpp BonecoDoNpc.cpp ImagemDasConstrucoes.cpp)
target_include_directories(interface_do_jogo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(interface_do_jogo PUBLIC SDL3::Headers regras_do_jogo PRIVATE nlohmann_json::nlohmann_json)
target_compile_options(interface_do_jogo PRIVATE -Wall -Wextra)

# A Dear ImGui (marco 8, FL8-1): os arquivos dela com o apoio da SDL3 e do OpenGL 3.
add_library(imgui_do_jogo STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_do_jogo PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui_do_jogo PUBLIC SDL3::SDL3)
target_compile_options(imgui_do_jogo PRIVATE -w)

add_executable(jogo
    main.cpp
    InterfaceDoJogador.cpp
    TextoDaDataDoJogo.cpp
    DesenhoDeQuadrados.cpp
    DesenhoDoMapa.cpp
    DesenhoDosPersonagens.cpp
    ImplementacaoDoStbImage.cpp
)

# O jogo usa as regras do Codigo/, a câmera, a SDL3 e o glad; o stb_image é só um cabeçalho.
target_link_libraries(jogo PRIVATE regras_do_jogo interface_do_jogo SDL3::SDL3 glad imgui_do_jogo)
target_include_directories(jogo PRIVATE ${stb_SOURCE_DIR})

# Onde ficam os dados (o mapa, as imagens). Por enquanto, a pasta Jogo/data
# deste repositório: o jogo roda de dentro da pasta build/.
target_compile_definitions(jogo PRIVATE PASTA_DOS_DADOS_DO_JOGO="${CMAKE_CURRENT_SOURCE_DIR}/data"
    PASTA_DOS_ASSETS_DO_JOGO="${CMAKE_CURRENT_SOURCE_DIR}/assets")

# Avisos do compilador ligados: ele aponta coisas suspeitas no nosso código.
target_compile_options(jogo PRIVATE -Wall -Wextra)
# O código do stb_image é de fora: os avisos dele não são nossos.
set_source_files_properties(ImplementacaoDoStbImage.cpp PROPERTIES COMPILE_OPTIONS "-w")

# No MinGW, coloca as bibliotecas do próprio g++ dentro do jogo.exe.
# Assim o programa abre com dois cliques, sem procurar DLLs do MinGW.
if(MINGW)
    target_link_options(jogo PRIVATE -static)
endif()

# Os testes da câmera.
add_subdirectory(Testes)

# O teste de 1 ano (marco 7, FL7-8): "jogo --simular 64 --semente 1", rodado
# pelo ctest como um teste à parte. Ele grava o relatorio.csv na pasta build/.
add_test(NAME teste_de_um_ano COMMAND jogo --simular 64 --semente 1 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
