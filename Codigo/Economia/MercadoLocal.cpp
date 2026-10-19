#include "Economia/MercadoLocal.h"

#include <algorithm>

int calcularFatorDoMercadoLocal(int estoque, int64_t estoqueAlvoEmCentesimos, const RegrasDoMercadoLocal& regras)
{
    if (estoqueAlvoEmCentesimos <= 0) {
        return 100;
    }
    const int64_t fator = 100 + (estoqueAlvoEmCentesimos - static_cast<int64_t>(estoque) * 100) * 100 / estoqueAlvoEmCentesimos;
    return static_cast<int>(std::clamp<int64_t>(fator, regras.fatorMinimoEmPorcento, regras.fatorMaximoEmPorcento));
}

int calcularPrecoDesejado(int precoBaseEmDecimos, int fatorEmPorcento)
{
    return static_cast<int>(arredondarMetadeParaCima(static_cast<long long>(precoBaseEmDecimos) * fatorEmPorcento, 100));
}

int darOPassoDoPrecoLocal(int precoLocalEmDecimos, int precoDesejadoEmDecimos, const RegrasDoMercadoLocal& regras)
{
    const int passoMaximo =
        std::max(1, static_cast<int>(arredondarMetadeParaCima(static_cast<long long>(precoLocalEmDecimos) * regras.passoMaximoPorDiaEmPorcento, 100)));
    if (precoDesejadoEmDecimos > precoLocalEmDecimos) {
        return std::min(precoDesejadoEmDecimos, precoLocalEmDecimos + passoMaximo);
    }
    return std::max(precoDesejadoEmDecimos, precoLocalEmDecimos - passoMaximo);
}

void comecarOMercadoLocal(ContextoDaProducao& contexto)
{
    std::vector<int>& precos = contexto.mercadoLocal.precoLocalPorItemEmDecimos;
    precos.clear();
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        precos.push_back(item.precoBaseEmDecimos);
    }
}

void rodarOMercadoLocalDoDia(ContextoDaProducao& contexto)
{
    const RegrasDoMercadoLocal& regras = contexto.dados.regras.economia.mercadoLocal;
    std::vector<int>& precos = contexto.mercadoLocal.precoLocalPorItemEmDecimos;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        const int numero = item.numeroInterno;
        // O estoque conta os dois donos.
        const int fator = calcularFatorDoMercadoLocal(contexto.inventario.quantidadeTotalDe(numero), estoqueAlvoDoItemEmCentesimos(contexto, numero), regras);
        const int desejado = calcularPrecoDesejado(item.precoBaseEmDecimos, fator);
        precos[numero] = darOPassoDoPrecoLocal(precos[numero], desejado, regras);
    }
}
