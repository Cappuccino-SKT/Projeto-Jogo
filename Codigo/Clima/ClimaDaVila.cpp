#include "Clima/ClimaDaVila.h"

#include "Eventos/EventosDaVila.h"

namespace {

// As frases do console quando o clima muda.
std::string fraseDoClimaNovo(TipoDeClima clima)
{
    switch (clima) {
    case TipoDeClima::Limpo: return "o céu limpou";
    case TipoDeClima::Nublado: return "o céu fechou";
    case TipoDeClima::Chuva: return "começou a chover";
    case TipoDeClima::Tempestade: return "começou uma tempestade (os NPCs ficam em casa no tempo livre)";
    case TipoDeClima::Neve: return "começou a nevar";
    case TipoDeClima::Nevoa: return "baixou a névoa";
    case TipoDeClima::CalorForte: return "faz um calor forte";
    }
    return "o tempo mudou";
}

}  // namespace

void guardarODiaDeHojeNoClima(EstadoDoClima& clima, const RelogioDoMundo& relogio)
{
    clima.estacaoDeHoje = relogio.estacaoAtual();
    clima.diaDaEstacaoDeHoje = relogio.diaDaEstacaoAtual();
    clima.diasPorEstacao = relogio.diasPorEstacao();
}

std::vector<std::string> rodarOClimaDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio)
{
    EstadoDoClima& clima = contexto.clima;
    const RegrasDoClima& regras = contexto.dados.regrasDoClima;
    guardarODiaDeHojeNoClima(clima, relogio);
    clima.lenhaTiradaHoje = 0;
    clima.faltouLenhaHoje = false;

    const TipoDeClima climaDeOntem = clima.climaDeHoje;
    clima.climaDeOntem = climaDeOntem;
    clima.climaDeHoje = sortearClimaDoDia(climaDeOntem, clima.estacaoDeHoje, regras, contexto.sorteioDoClima);
    clima.climaMudouHoje = clima.climaDeHoje != climaDeOntem;
    if (!clima.climaMudouHoje) {
        return {};
    }
    return {"Clima: " + fraseDoClimaNovo(clima.climaDeHoje) + "."};
}

void consumirALenhaDoDia(ContextoDaProducao& contexto)
{
    EstadoDoClima& clima = contexto.clima;
    const RegrasDoClima& regras = contexto.dados.regrasDoClima;
    clima.lenhaTiradaHoje = 0;
    clima.faltouLenhaHoje = false;

    // Passo 6: no dia 1 da Primavera, a dívida e os dias em déficit zeram.
    if (clima.estacaoDeHoje != Estacao::Inverno) {
        if (clima.estacaoDeHoje == Estacao::Primavera && clima.diaDaEstacaoDeHoje == 1) {
            clima.lenhaDevidaEmCentesimos = 0;
            clima.diasEmDeficitDeLenha = 0;
        }
        return;
    }
    const std::optional<int> tora = contexto.dados.catalogoDeItens.procurarNumeroDoItem(regras.itemDaLenha);
    if (!tora) {
        return;
    }
    // Passos 1 a 5.
    const int lenhaDeHoje = calcularLenhaDoDiaEmCentesimos(contexto.dinheiro.quantidadeDeCivis, clima.climaDeHoje,
                                                           multiplicadorDaLenhaPorEventos(contexto), regras);
    const int devidaAntes = clima.lenhaDevidaEmCentesimos + lenhaDeHoje;
    const int torasNoEstoque = contexto.inventario.quantidadeDe(*tora, DonoDoItem::Cidade);
    const int torasTiradas = pagarALenhaDevida(clima, lenhaDeHoje, torasNoEstoque);
    if (torasTiradas > 0) {
        contexto.inventario.tentarTirar(*tora, DonoDoItem::Cidade, torasTiradas);
        contexto.consumoMedido.anotarOQueSaiu(*tora, torasTiradas);
        // A classe paga cada Tora à Cidade, pelo preço da loja (Economia, regra 4.1).
        const int64_t precoDaLoja = calcularPrecoDaLojaDaCidade(precoLocalDoItem(contexto, *tora), contexto.dados.regras.economia);
        DinheiroDaVila& dinheiro = contexto.dinheiro;
        const int64_t preco = precoDaLoja * torasTiradas;
        const int64_t pago = std::min(preco, std::max<int64_t>(dinheiro.dinheiroDaClasseCivil, 0));
        dinheiro.dinheiroDaClasseCivil -= pago;
        dinheiro.bancoDaCidade += pago;
        dinheiro.consumoNaoPagoHoje += preco - pago;
        dinheiro.gastoDaCestaDeHoje += preco;
    }
    clima.lenhaTiradaHoje = torasTiradas;
    if (clima.faltouLenhaHoje) {
        // A lenha que faltou também é demanda: o Posto aprende a fazer mais Tora.
        const int torasQueFaltaram = devidaAntes / 100 - torasTiradas;
        contexto.consumoMedido.anotarADemandaQueFaltou(*tora, static_cast<int64_t>(torasQueFaltaram) * 100);
    }
}

int64_t bonusDoEstoqueParaOInvernoEmCentesimos(const ContextoDaProducao& contexto, int numeroDoItem)
{
    const EstadoDoClima& clima = contexto.clima;
    const RegrasDoClima& regras = contexto.dados.regrasDoClima;
    const bool estaNosUltimosDiasDoOutono = clima.estacaoDeHoje == Estacao::Outono
        && clima.diaDaEstacaoDeHoje > clima.diasPorEstacao - regras.diasDoFimDoOutonoParaEstocar;
    if (!estaNosUltimosDiasDoOutono) {
        return 0;
    }
    if (contexto.dados.catalogoDeItens.procurarNumeroDoItem(regras.itemDaLenha) == numeroDoItem) {
        // Moradores × 25 × 5 ÷ 100, em Toras inteiras (15 Civis: 18).
        const int64_t toras = static_cast<int64_t>(contexto.dinheiro.quantidadeDeCivis) * regras.lenhaPorMoradorPorDiaEmCentesimos
            * regras.diasDeLenhaEstocada / 100;
        return toras * 100;
    }
    if (contexto.dados.catalogoDeItens.procurarNumeroDoItem(regras.itemDoGraoEstocado) == numeroDoItem) {
        return contexto.consumoMedido.consumoPrevistoDoDiaEmCentesimos(numeroDoItem) * regras.diasDeGraoEstocado;
    }
    return 0;
}

int pontosDoClimaNoConsumoAtendido(const ContextoDaProducao& contexto)
{
    return contexto.clima.faltouLenhaHoje ? contexto.dados.regrasDoClima.felicidadeSemLenha : 0;
}

int pontosDoClimaNosEventos(const ContextoDaProducao& contexto)
{
    return contexto.clima.climaDeHoje == TipoDeClima::CalorForte ? contexto.dados.regrasDoClima.felicidadeNoCalorForte : 0;
}

bool periodoLivreEmCasaHoje(const EstadoDoClima& clima, const RegrasDoClima& regras)
{
    return clima.climaDeHoje == TipoDeClima::Tempestade && regras.periodoLivreEmCasaNaTempestade;
}
