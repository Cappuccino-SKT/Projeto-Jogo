#pragma once

// O estado que as decisões do jogador mudam (Ficha Lógica: Economia, parte 11,
// e Construção, parte 10; marco 8): o preço dos itens dele, o nível de
// salário e a fila de obras. Só dados; as regras estão em Jogador/.

#include <map>
#include <string>
#include <vector>

// FL8-7: o nível de salário de uma construção do jogador.
enum class NivelDeSalario { Baixo, Normal, Alto };

// FL8-6: a porcentagem do preço de cada item do jogador (de -30 a +30; sem
// linha = 0). O mesmo preço vale para todas as construções que fazem o item.
struct DecisoesDoJogador {
    std::map<int, int> porcentagemDoPrecoPorItem;

    int porcentagemDoPrecoDe(int numeroDoItem) const
    {
        const auto linha = porcentagemDoPrecoPorItem.find(numeroDoItem);
        return linha == porcentagemDoPrecoPorItem.end() ? 0 : linha->second;
    }
};

// FL8-3 e FL8-5: as três coisas que a fila de obras faz.
enum class TipoDeObra { ConstrucaoNova, SubirDeNivel, Manutencao };

struct ObraNaFila {
    TipoDeObra tipo = TipoDeObra::Manutencao;
    int indiceDaConstrucao = -1;        // a construção da obra (a nova já nasce na vila, "em obra")
    int nivelDeDestino = 1;             // 1 na nova; nível + 1 ao subir; o atual na manutenção
    int64_t centesimosDeHoraQueFaltam = 0;  // 4000 = 40 h (a velocidade não arredonda)
    bool esperaMaterial = false;        // só a manutenção: faltam Tábuas ou Blocos
};

struct ObrasDaVila {
    std::vector<ObraNaFila> fila;
    std::vector<std::string> linhasParaOConsole;  // "Obra pronta: ..." (o laço da simulação esvazia)
};
