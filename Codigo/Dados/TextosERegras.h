#pragma once

// Os dois arquivos JSON lidos antes de tudo (ficha dos Itens, parte 3):
//   textos/pt-BR.json -> os nomes que aparecem na tela;
//   regras.json       -> os números das rodadas (salários, margem, estações).

#include <array>
#include <map>
#include <string>

#include "Dados/ProblemasDosDados.h"

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

// Os números do regras.json usados no marco 3.
struct RegrasDoJogo {
    RegrasDoPreco regrasDoPreco;
    // Grupo da estação -> produção na Primavera, Verão, Outono e Inverno, em % (FL3-6).
    std::map<std::string, std::array<int, 4>> producaoPorEstacaoEmPorcento;
    int produtividadeNormalEmPorcento = 100;
    int produtividadeDeDestaqueEmPorcento = 150;
};

TabelaDeTextos lerArquivoDeTextos(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);
RegrasDoJogo lerArquivoDeRegras(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);
