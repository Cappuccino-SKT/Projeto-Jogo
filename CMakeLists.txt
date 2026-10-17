# CMakeLists.txt principal: a "receita" que diz ao CMake como montar o projeto.
# Ele junta as duas pastas de código:
#   Codigo/ -> as regras do jogo, sem janela (testadas sozinhas pelo ctest)
#   Jogo/   -> o programa de verdade: janela, desenho, mouse e teclado

cmake_minimum_required(VERSION 3.24)

project(ProjetoJogo LANGUAGES C CXX)

# C++20, sem extensões próprias do g++, para o código valer em qualquer compilador.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Bibliotecas de fora (rodada FT2-2): o próprio CMake baixa cada uma, numa
# versão fixa, para dentro da pasta build/. Nada precisa ser instalado à mão.
# ---------------------------------------------------------------------------
include(FetchContent)

# SDL3: abre a janela e lê o teclado e o mouse.
# - versão 3.4.18 (2026-10-02);
# - o URL_HASH confere a "impressão digital" do arquivo baixado: se alguém
#   trocar o arquivo no servidor, o CMake recusa;
# - compilada como biblioteca estática: ela entra dentro do jogo.exe, e não
#   precisa de um SDL3.dll ao lado do programa.
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.18/SDL3-3.4.18.tar.gz
    URL_HASH SHA256=9c75cf16330322c217dedd2e0609f1124f1b54b8633e763467b4684d0f4334a3
)
FetchContent_MakeAvailable(SDL3)

# nlohmann/json: lê os arquivos JSON (o mapa do Tiled e, a partir do marco 3,
# os dados). Entrou no marco 1 por causa do mapa (rodada FL1-1).
# Versão 3.12.0; o pacote "json.tar.xz" é o pequeno, recomendado pelo autor.
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
)
FetchContent_MakeAvailable(nlohmann_json)

# stb_image: carrega as imagens PNG (os conjuntos de tiles). É um arquivo só,
# sem CMakeLists; o FetchContent só baixa, e o Jogo/ inclui a pasta.
# Versão fixa: o commit 2c980bb (2026-08-02).
FetchContent_Declare(
    stb
    URL https://github.com/nothings/stb/archive/2c980bb59875b0d32144a71867fbdebb2f77cd20.tar.gz
    URL_HASH SHA256=9a955b1b49a4410088a2e0ee2a9c057c3c907d0c1d75454144cb980aca0ba515
)
FetchContent_MakeAvailable(stb)

# ---------------------------------------------------------------------------
# As nossas pastas
# ---------------------------------------------------------------------------
# Liga o ctest, que roda os testes automáticos (rodada FL1-4).
enable_testing()

add_subdirectory(Codigo)
add_subdirectory(Jogo)
