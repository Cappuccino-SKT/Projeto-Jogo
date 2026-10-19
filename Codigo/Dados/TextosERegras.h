#pragma once

// Os dois arquivos JSON lidos antes de tudo (ficha dos Itens, parte 3):
//   textos/pt-BR.json -> os nomes que aparecem na tela;
//   regras.json       -> os números das rodadas (salários, margem, estações).

#include <array>
#include <map>
#include <string>

#include "Construcao/RegrasDasConstrucoes.h"
#include "Dados/ProblemasDosDados.h"
#include "Economia/FelicidadeDaClasse.h"
#include "Economia/RegrasDaEconomia.h"
#include "Npc/RegrasDaRotina.h"

// Os nomes da tela, pelo ID: "item.pao" -> "Pão".
class TabelaDeTextos {
public:
    void guardarTexto(const std::string& identificador, const std::string& textoParaMostrar);
    bool temTexto(const std::string& identificador) const;
    // O texto, ou o próprio ID se faltar (assim um nome esquecido aparece na tela, sem quebrar nada).
    std::string textoDe(const std::string& identificador) const;
    const std::map<std::string, std::string>& todosOsTextos() const { return textosPorIdentificador; }

private:
    std::map<std::string, std::string> textosPorIdentificador;
};

// As regras do preço base (ficha das Receitas, FL3-3).
struct RegrasDoPreco {
    int salarioPorHoraBracalEmCentesimos = 75;
    int salarioPorHoraManufaturadoEmCentesimos = 100;
    int salarioPorHoraEspecialistaEmCentesimos = 150;
    int margemDoDonoEmPorcento = 120;
};

// Os números do regras.json usados até o marco 7.
struct RegrasDoJogo {
    RegrasDoPreco regrasDoPreco;
    // Grupo da estação -> produção na Primavera, Verão, Outono e Inverno, em % (FL3-6).
    std::map<std::string, std::array<int, 4>> producaoPorEstacaoEmPorcento;
    int produtividadeNormalEmPorcento = 100;
    int produtividadeDeDestaqueEmPorcento = 150;
    // O movimento dos NPCs (marco 4, ficha do NPC): a velocidade de andar
    // (FL4-1) e quantos caminhos a busca calcula por tick (FT1-3).
    int velocidadeDeAndarEmTilesPorHora = 128;
    int caminhosCalculadosPorTick = 4;
    // A rotina, as notas da Utilidade e o passeio (marco 5, ficha da IA, FL5-3).
    RegrasDaVidaDosNpcs vidaDosNpcs;
    // As vagas, a capacidade e as obras (marco 6, ficha da Construção).
    RegrasDasConstrucoes construcoes;
    // A cesta mínima, a loja da cidade, os salários e a ordem do dia (marco 6,
    // ficha da Economia), e o Mercado Local, o gasto extra, o imposto e o
    // estabilizador (marco 7).
    RegrasDaEconomia economia;
    // A Felicidade dos Civis (marco 7, ficha da Felicidade).
    RegrasDaFelicidade felicidade;
};

TabelaDeTextos lerArquivoDeTextos(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);
RegrasDoJogo lerArquivoDeRegras(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);
