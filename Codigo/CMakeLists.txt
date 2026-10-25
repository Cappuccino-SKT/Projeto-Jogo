# Codigo/: as regras do jogo, numa biblioteca que nunca abre janela nem desenha.
# O Jogo/ e o programa de testes usam esta biblioteca.

add_library(regras_do_jogo STATIC
    Tempo/RelogioDoMundo.cpp
    Tempo/SorteioComSemente.cpp
    Mapa/MapaLocal.cpp
    Mapa/BuscaDeCaminho.cpp
    Npc/MovimentoDosNpcs.cpp
    Npc/RotinaDosNpcs.cpp
    Vila/VilaInicial.cpp
    Inventario/InventarioDaCidade.cpp
    Construcao/ContasDasConstrucoes.cpp
    Producao/ProducaoDaVila.cpp
    Economia/ConsumoESalarios.cpp
    Economia/MercadoLocal.cpp
    Economia/FelicidadeDaClasse.cpp
    Eventos/DefinicaoDosEventos.cpp
    Eventos/EventosDaVila.cpp
    Producao/ConsumoMedidoDaVila.cpp
    Simulacao/RelatorioDaSimulacao.cpp
    Jogador/DecisoesDoJogador.cpp
    Jogador/ObrasDaVila.cpp
    Jogador/PedidosDoJogador.cpp
    Jogador/NotificacoesEAlertas.cpp
    Clima/Clima.cpp
    Clima/ClimaDaVila.cpp
    Vila/Trabalho.cpp
    Vila/Populacao.cpp
    Viagem/MundoDoJogo.cpp
    Viagem/RegrasDaViagem.cpp
    Viagem/Viagem.cpp
    Viagem/ViagemDaVila.cpp
    Cidades/OutrasCidades.cpp
    Comercio/Comercio.cpp
    Noticias/Noticias.cpp
    Reputacao/Reputacao.cpp
    Tecnologia/Tecnologia.cpp
    Faccao/Faccao.cpp
    Cultura/Cultura.cpp
    Crime/Crime.cpp
    Religiao/Religiao.cpp
    Vila/SimulacaoDaVila.cpp
    Save/ArquivoDeSave.cpp
    Save/GravarAPartida.cpp
    Save/CarregarAPartida.cpp
    Dados/ProblemasDosDados.cpp
    Dados/TabelaCsv.cpp
    Dados/TextosERegras.cpp
    Dados/Itens.cpp
    Dados/Construcoes.cpp
    Dados/Recursos.cpp
    Dados/Receitas.cpp
    Dados/DadosDoJogo.cpp
)

# Quem usa a biblioteca escreve, por exemplo, #include "Tempo/RelogioDoMundo.h".
target_include_directories(regras_do_jogo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(regras_do_jogo PRIVATE -Wall -Wextra)

# A leitura do mapa e dos dados usa a nlohmann/json, mas só por dentro (PRIVATE).
target_link_libraries(regras_do_jogo PRIVATE nlohmann_json::nlohmann_json)

# Os NPCs usam a EnTT, e quem usa a biblioteca também a enxerga (PUBLIC): o
# Jogo/ lê as posições para desenhar.
target_link_libraries(regras_do_jogo PUBLIC EnTT::EnTT)

# O Tracy (marco 7), só com -DUSAR_TRACY=ON: as linhas MEDIR_ESTE_TRECHO do
# Desempenho/MedicaoDoDesempenho.h passam a medir.
if(USAR_TRACY)
    target_link_libraries(regras_do_jogo PUBLIC Tracy::TracyClient)
    target_compile_definitions(regras_do_jogo PUBLIC USAR_TRACY)
endif()

# Os testes automáticos (rodada FL1-4).
add_subdirectory(Testes)
