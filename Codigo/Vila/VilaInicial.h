#pragma once

// A vila inicial (ficha da IA, regra 4.1, FL5-1 e FL5-2): as construções nos
// lotes do mapa e os NPCs com casa e emprego, lidas do data/vila_inicial.json.
// No marco 5, a construção é só um lugar (a porta, quem mora, quem trabalha);
// o que ela produz entra no marco 6.

#include <cstdint>
#include <string>
#include <vector>

#include "Construcao/RegrasDasConstrucoes.h"
#include "Dados/Construcoes.h"
#include "Dados/Itens.h"
#include "Dados/ProblemasDosDados.h"
#include "Inventario/DonoDoItem.h"
#include "Mapa/MapaLocal.h"
#include "Producao/OrdemDoJogador.h"

constexpr const char* tipoDaCasa = "construcao.casa";
constexpr const char* tipoDoMercado = "construcao.mercado";

struct ConstrucaoDaVila {
    std::string identificador;    // "casa_1"
    std::string tipo;             // "construcao.casa"
    int numeroDoLote = 0;         // o "id" do lote no Tiled
    bool donoEhOJogador = false;
    RetanguloEmTiles areaDoLote;  // os tiles que a construção ocupa (e bloqueia)
    PosicaoDeTile tileDaPorta;    // logo abaixo do meio do lote
    bool emObra = false;          // marco 8: a construção nova ainda está sendo feita
    int nivel = 1;                // de 1 a 5 (marco 6; a Carpintaria do jogador começa no 2, FL6-2)
    std::vector<OrdemDoJogador> ordensDoJogador;  // só nas do jogador (FL6-3)

    bool ehCasa() const { return tipo == tipoDaCasa; }
};

struct NpcDaVilaInicial {
    std::string nome;
    std::string classe;        // "civil" na fatia
    int indiceDaCasa = -1;     // posição na lista de construções
    int indiceDoTrabalho = -1; // -1 = desempregado
    // As habilidades, de 0 a 100 (FL6-6: 50 para todos na fatia).
    int habilidadeBracal = 50;
    int habilidadeManufaturado = 50;
    int idade = 10;  // fase A3 (ficha da População, PO1-1): de 5 a 19 na vila inicial
};

// Uma linha do "estoque_inicial" (FL6-4).
struct ItemDoEstoqueInicial {
    int numeroDoItem = 0;
    DonoDoItem dono = DonoDoItem::Cidade;
    int quantidade = 0;
};

struct VilaDoJogo {
    uint64_t sementeDaPartida = 1;
    std::vector<ConstrucaoDaVila> construcoes;
    std::vector<NpcDaVilaInicial> npcsIniciais;
    // O dinheiro e o estoque do primeiro dia (marco 6, FL6-4), em décimos de moeda.
    int bancoDaCidadeEmDecimos = 0;
    int dinheiroDoJogadorEmDecimos = 0;
    int dinheiroInicialPorCivilEmDecimos = 0;
    std::vector<ItemDoEstoqueInicial> estoqueInicial;

    // A primeira construção do tipo Mercado, ou -1 se a vila não tem Mercado.
    int indiceDoMercado() const;
    int quantidadeDeEmpregados() const;
};

// A porta de um lote: x = canto x + largura ÷ 2; y = canto y + altura.
PosicaoDeTile calcularTileDaPorta(const RetanguloEmTiles& areaDoLote);

// O que a leitura da vila precisa conferir: os tipos de construção, os itens
// (do estoque e das ordens) e as vagas de cada nível.
struct CatalogosParaAVila {
    const CatalogoDeConstrucoes& catalogoDeConstrucoes;
    const CatalogoDeItens& catalogoDeItens;
    const RegrasDasConstrucoes& regrasDasConstrucoes;
};

// Lê e confere o vila_inicial.json. Cada problema vira um erro com o motivo.
VilaDoJogo lerVilaInicial(const std::string& textoDoArquivo, const MapaLocal& mapa, const CatalogosParaAVila& catalogos,
                          ListaDeProblemasDosDados& listaDeProblemas);

// Regra 4.1, passo 3 (FL5-1): os tiles dos lotes ocupados deixam de dar para andar.
void bloquearOsLotesOcupados(MapaLocal& mapa, const VilaDoJogo& vila);
