#pragma once

// Os números das construções que valem para todos os tipos (Ficha Lógica:
// Construção, parte 2), lidos do data/regras.json: as vagas por nível, a
// capacidade por porte e o custo das obras.

#include <array>
#include <vector>

// O custo de uma obra (V2-8). Só é lido e conferido no marco 6: as obras
// entram com a interface do jogador (FL6-1).
struct CustoDaObra {
    int tabuas = 0;
    int blocosDePedra = 0;
    int barrasDeFerro = 0;
    int horasDeTrabalho = 0;
};

struct RegrasDasConstrucoes {
    // Do nível 1 ao 5 (IP3-11): 2, 3, 5, 7 e 10 vagas.
    std::vector<int> vagasPorNivel = {2, 3, 5, 7, 10};
    // Pequeno, médio e grande: a capacidade de cada nível (IT-4). Uma
    // construção média no nível 2 guarda 200 × 2 = 400.
    std::array<int, 3> capacidadePorNivelDeCadaPorte = {100, 200, 400};
    // Pequeno, médio e grande.
    std::array<CustoDaObra, 3> obraDeCadaPorte = {};

    // FL8-4 e FL8-5 (Construção, parte 10): o Engenheiro, a manutenção por estação.
    int nivelMaximoComOEngenheiro = 3;
    int manutencaoEmPorcento = 5;
    int producaoComManutencaoAtrasadaEmPorcento = 75;
    int nivelMaximo() const { return static_cast<int>(vagasPorNivel.size()); }
};
