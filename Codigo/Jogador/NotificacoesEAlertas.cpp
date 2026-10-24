#include "Jogador/NotificacoesEAlertas.h"

void NotificacoesDaVila::acrescentar(NivelDaNotificacao nivel, const std::string& texto, int64_t diaDaPartida, int64_t tickAtual)
{
    diario.push_back(Notificacao{nivel, texto, diaDaPartida, tickAtual});
    if (diario.size() > notificacoesNoDiarioNoMaximo) {
        diario.erase(diario.begin());
    }
}

std::vector<const Notificacao*> NotificacoesDaVila::naTela(int64_t tickAtual) const
{
    std::vector<const Notificacao*> naTela;
    for (auto notificacao = diario.rbegin(); notificacao != diario.rend() && static_cast<int>(naTela.size()) < notificacoesNaTelaNoMaximo; ++notificacao) {
        if (tickAtual - notificacao->tickDeEntrada < ticksQueUmaNotificacaoFicaNaTela) {
            naTela.push_back(&*notificacao);
        }
    }
    return naTela;
}

bool classificarALinhaDoConsole(const ContextoDaProducao& contexto, const std::string& linha, NivelDaNotificacao& nivel)
{
    const auto comeca = [&](const char* prefixo) { return linha.rfind(prefixo, 0) == 0; };
    if (comeca("Aviso:")) {
        nivel = NivelDaNotificacao::Aviso;
        return true;
    }
    if (comeca("Obra pronta:") || comeca("Obra encomendada:") || comeca("Notícia:")) {
        nivel = NivelDaNotificacao::Info;
        return true;
    }
    if (comeca("Evento:") || comeca("Fim do evento:")) {
        nivel = NivelDaNotificacao::Info;
        for (const DefinicaoDeEvento& evento : contexto.dados.catalogoDeEventos.todosOsEventos()) {
            if (evento.negativo && linha.find(contexto.dados.tabelaDeTextos.textoDe(evento.identificadorDeTexto)) != std::string::npos) {
                nivel = NivelDaNotificacao::Aviso;
            }
        }
        return true;
    }
    return false;
}

std::vector<std::string> calcularOsAlertasDaVila(const ContextoDaProducao& contexto)
{
    std::vector<std::string> alertas;
    // Comida para menos de 2 dias do consumo previsto (o alvo é de 5 dias).
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        if (!item.ehComida()) {
            continue;
        }
        const int64_t estoqueEmCentesimos = static_cast<int64_t>(contexto.inventario.quantidadeTotalDe(item.numeroInterno)) * 100;
        if (estoqueEmCentesimos * 5 < estoqueAlvoDoItemEmCentesimos(contexto, item.numeroInterno) * 2) {
            alertas.push_back(contexto.dados.tabelaDeTextos.textoDe(item.identificadorDeTexto) + " para menos de 2 dias");
        }
    }
    // O Banco abaixo de 10% do que tinha no começo da estação.
    if (contexto.dinheiro.bancoNoComecoDaEstacao > 0 && contexto.dinheiro.bancoDaCidade * 10 < contexto.dinheiro.bancoNoComecoDaEstacao) {
        alertas.push_back("Banco baixo");
    }
    for (std::size_t indice = 0; indice < contexto.vila.construcoes.size(); ++indice) {
        const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indice];
        const ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[indice];
        if (construcao.donoEhOJogador && producao.estaParada && producao.motivoDaParada.rfind("sem ", 0) == 0) {
            alertas.push_back(construcao.identificador + " parada: " + producao.motivoDaParada);
        }
        if (producao.manutencaoAtrasada) {
            alertas.push_back("Manutenção atrasada: " + construcao.identificador);
        }
    }
    return alertas;
}

std::string escreverDinheiroComMilhar(int64_t valorEmDecimos)
{
    const bool negativo = valorEmDecimos < 0;
    const int64_t valor = negativo ? -valorEmDecimos : valorEmDecimos;
    std::string inteiro = std::to_string(valor / 10);
    for (int posicao = static_cast<int>(inteiro.size()) - 3; posicao > 0; posicao -= 3) {
        inteiro.insert(static_cast<std::size_t>(posicao), ".");
    }
    return (negativo ? "-" : "") + inteiro + "," + std::to_string(valor % 10);
}
