#pragma once

// Os componentes do NPC usados no marco 4 (Ficha Lógica: NPC, parte 2).
// Cada NPC é uma entidade da EnTT (um número), e estes são os dados presos a ela:
//   parado             -> só a PosicaoDoNpc;
//   andando            -> PosicaoDoNpc e CaminhoDoNpc;
//   esperando o cálculo -> PosicaoDoNpc e PedidoDeCaminho.
//
// A posição dentro do passo é medida em milésimos de tile (1 tile = 1.000),
// em números inteiros: a mesma partida dá sempre o mesmo resultado.

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "Mapa/MapaLocal.h"

// Nesta ordem, a das linhas da folha do boneco: baixo, cima, direita, esquerda.
enum class DirecaoDoOlhar { Baixo = 0, Cima = 1, Direita = 2, Esquerda = 3 };

// Onde o NPC está: num tile ou a caminho do vizinho.
struct LugarDoNpc {
    PosicaoDeTile tileAtual;
    PosicaoDeTile proximoTile;                     // igual ao tileAtual quando está parado
    int progressoAteOProximoTileEmMilesimos = 0;   // de 0 até o comprimento do passo (1.000 ou 1.414)

    bool estaParado() const { return proximoTile == tileAtual; }
};

struct PosicaoDoNpc {
    LugarDoNpc lugarAtual;
    DirecaoDoOlhar direcaoDoOlhar = DirecaoDoOlhar::Baixo;

    // Só para a animação de andar (não vai no save): volta a 0 a cada ciclo.
    int distanciaNoCicloDeAndarEmMilesimos = 0;

    // A posição no tick anterior, para o desenho suave entre os ticks (FL4-5). Não vai no save.
    LugarDoNpc lugarNoTickAnteriorParaDesenho;
};

struct CaminhoDoNpc {
    std::vector<PosicaoDeTile> tilesDoCaminho;   // não vai no save: é recalculado
    std::size_t indiceDoProximoTileDoCaminho = 0;  // os tiles antes deste já foram andados
    PosicaoDeTile tileDeDestino;

    bool acabou() const { return indiceDoProximoTileDoCaminho >= tilesDoCaminho.size(); }
};

struct PedidoDeCaminho {
    PosicaoDeTile tileDeDestino;
    int64_t tickDoPedido = 0;
};

// ---------------------------------------------------------------------------
// Marco 5 (ficha da IA, parte 2): quem é o NPC, onde mora e trabalha, e o que
// está fazendo.
// ---------------------------------------------------------------------------

// As ações da fatia (FL5-3). "Nenhuma" só existe ao nascer e quando o NPC
// precisa decidir de novo do zero.
enum class AcaoDoNpc { Nenhuma, Dormir, Trabalhar, Comer, Comprar, Passear };

struct IdentidadeDoNpc {
    std::string nome;
    std::string classe;   // "civil" na fatia
    int numeroDoNpc = 0;  // a posição na vila inicial: decide o turno da reavaliação e o dia de compra
};

struct CasaETrabalhoDoNpc {
    int indiceDaCasa = -1;      // posição na lista de construções da vila
    int indiceDoTrabalho = -1;  // -1 = desempregado
    // Fase A2 (Trabalho, regra 4.1, passo 3): a construção que ele deixou por
    // salário atrasado nesta estação (a Cidade não o contrata de volta), ou -1.
    int saiuPorAtrasoDe = -1;
};

struct DecisaoDoNpc {
    AcaoDoNpc acaoAtual = AcaoDoNpc::Nenhuma;
    int indiceDaConstrucaoOndeEsta = -1;     // -1 = na rua (visível)
    int indiceDaConstrucaoDoDestino = -1;    // -1 = um tile da rua (o passeio) ou nenhum
    bool estaACaminho = false;               // pediu um caminho e ainda não chegou
    int64_t tickEmQueAEsperaAcaba = -1;      // o fim da compra ou da parada do passeio
    int64_t diaDaUltimaCompra = -1;

    bool estaDentroDeUmaConstrucao() const { return indiceDaConstrucaoOndeEsta >= 0; }
};

// Marco 7 (ficha dos Eventos): o NPC doente da Febre fica em casa (a ação
// Dormir ganha sempre) e não trabalha. Sai quando os dias acabam.
struct DoencaDoNpc {
    int diasQueFaltam = 0;
};
