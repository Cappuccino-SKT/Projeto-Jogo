# CMakeLists.txt principal: a "receita" que diz ao CMake como montar o projeto.
# Ele junta as duas pastas de código:
#   Codigo/ -> as regras do jogo, sem janela (começa no marco 1)
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

# ---------------------------------------------------------------------------
# As nossas pastas
# ---------------------------------------------------------------------------
# A pasta Codigo/ entra no marco 1, com o relógio do mundo.
add_subdirectory(Jogo)
