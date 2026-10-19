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

# EnTT: guarda as entidades (cada NPC é um número) e os componentes presos a
# elas (a posição, o caminho). Entra no marco 4 (FT2-2). Só cabeçalhos.
# Versão 3.16.0 (2025-11-20). A 4.0 (2026-07) muda a interface; fica para
# quando precisarmos dela.
FetchContent_Declare(
    EnTT
    URL https://github.com/skypjack/entt/archive/refs/tags/v3.16.0.tar.gz
    URL_HASH SHA256=7d7b4037b737992342049ffab14f22fa10243e01664f8c3a0657aa247ac52f71
)
FetchContent_MakeAvailable(EnTT)

# Tracy: mede quanto tempo cada trecho do jogo gasta (marco 7, FL7-8). Fica
# DESLIGADO por padrão: ligado, o jogo manda as medições pela rede para o
# programa do Tracy, baixado à parte. Para ligar:
#   cmake --preset padrao -DUSAR_TRACY=ON
# Versão 0.11.1 (2024-08).
option(USAR_TRACY "Liga as medições do Tracy (ficha da Simulação, regra 4.4)" OFF)
if(USAR_TRACY)
    FetchContent_Declare(
        tracy
        URL https://github.com/wolfpld/tracy/archive/refs/tags/v0.11.1.tar.gz
        URL_HASH SHA256=2c11ca816f2b756be2730f86b0092920419f3dabc7a7173829ffd897d91888a1
    )
    FetchContent_MakeAvailable(tracy)
endif()

# ---------------------------------------------------------------------------
# As nossas pastas
# ---------------------------------------------------------------------------
# Liga o ctest, que roda os testes automáticos (rodada FL1-4).
enable_testing()

add_subdirectory(Codigo)
add_subdirectory(Jogo)
