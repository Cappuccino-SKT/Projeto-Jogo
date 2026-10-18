#pragma once

// A vila inicial (ficha da IA, regra 4.1, FL5-1 e FL5-2): as construções nos
// lotes do mapa e os NPCs com casa e emprego, lidas do data/vila_inicial.json.
// No marco 5, a construção é só um lugar (a porta, quem mora, quem trabalha);
// o que ela produz entra no marco 6.

#include <cstdint>
#include <string>
#include <vector>

#include "Dados/Itens.h"
#include "Dados/ProblemasDosDados.h"
#include "Mapa/MapaLocal.h"

constexpr const char* tipoDaCasa = "construcao.casa";
constexpr const char* tipoDoMercado = "construcao.mercado";
constexpr int moradoresPorCasa = 4;  // 16 lugares nas 4 Casas (V2-12)

struct ConstrucaoDaVila {
    std::string identificador;    // "casa_1"
    std::string tipo;             // "construcao.casa"
    int numeroDoLote = 0;         // o "id" do lote no Tiled
    bool donoEhOJogador = false;
    RetanguloEmTiles areaDoLote;  // os tiles que a construção ocupa (e bloqueia)
    PosicaoDeTile tileDaPorta;    // logo abaixo do meio do lote

    bool ehCasa() const { return tipo == tipoDaCasa; }
};

struct NpcDaVilaInicial {
    std::string nome;
    std::string classe;        // "civil" na fatia
    int indiceDaCasa = -1;     // posição na lista de construções
    int indiceDoTrabalho = -1; // -1 = desempregado
};

struct VilaDoJogo {
    uint64_t sementeDaPartida = 1;
    std::vector<ConstrucaoDaVila> construcoes;
    std::vector<NpcDaVilaInicial> npcsIniciais;

    // A primeira construção do tipo Mercado, ou -1 se a vila não tem Mercado.
    int indiceDoMercado() const;
    int quantidadeDeEmpregados() const;
};

// A porta de um lote: x = canto x + largura ÷ 2; y = canto y + altura.
PosicaoDeTile calcularTileDaPorta(const RetanguloEmTiles& areaDoLote);

// Lê e confere o vila_inicial.json. Cada problema vira um erro com o motivo.
VilaDoJogo lerVilaInicial(const std::string& textoDoArquivo, const MapaLocal& mapa,
                          const CatalogoDeConstrucoes& catalogoDeConstrucoes, ListaDeProblemasDosDados& listaDeProblemas);

// Regra 4.1, passo 3 (FL5-1): os tiles dos lotes ocupados deixam de dar para andar.
void bloquearOsLotesOcupados(MapaLocal& mapa, const VilaDoJogo& vila);
