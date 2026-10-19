#include "Eventos/EventosDaVila.h"

#include <algorithm>
#include <utility>

#include "Npc/ComponentesDoNpc.h"

namespace {

std::string nomeDoEvento(const ContextoDaProducao& contexto, const DefinicaoDeEvento& evento)
{
    return contexto.dados.tabelaDeTextos.textoDe(evento.identificadorDeTexto);
}

std::string nomeDoItemDoEvento(const ContextoDaProducao& contexto, int numeroDoItem)
{
    return contexto.dados.tabelaDeTextos.textoDe(contexto.dados.catalogoDeItens.itemPeloNumero(numeroDoItem).identificadorDeTexto);
}

std::string escreverComSinal(int valor)
{
    return (valor >= 0 ? "+" : "") + std::to_string(valor);
}

int quantidadeDeNegativosAtivos(const ContextoDaProducao& contexto)
{
    int negativos = 0;
    for (const EventoAtivo& ativo : contexto.eventos.eventosAtivos) {
        negativos += contexto.dados.catalogoDeEventos.todosOsEventos()[ativo.indiceDoEvento].negativo ? 1 : 0;
    }
    return negativos;
}

// O efeito "mudar_estoque" (pontual): tira uma porcentagem (arredondada para
// baixo) ou põe uma quantidade de um item da Cidade, se couber.
std::string mudarOEstoque(ContextoDaProducao& contexto, const EfeitoDoEvento& efeito)
{
    InventarioDaCidade& inventario = contexto.inventario;
    const std::string nomeDoItem = nomeDoItemDoEvento(contexto, efeito.numeroDoItem);
    int quantidade = efeito.quantidade;
    if (efeito.porcentagem != 0) {
        quantidade = inventario.quantidadeDe(efeito.numeroDoItem, DonoDoItem::Cidade) * efeito.porcentagem / 100;
    }
    if (quantidade < 0) {
        inventario.tentarTirar(efeito.numeroDoItem, DonoDoItem::Cidade, -quantidade);
        return "tirou " + std::to_string(-quantidade) + " " + nomeDoItem;
    }
    const CategoriaDoItem categoria = contexto.dados.catalogoDeItens.itemPeloNumero(efeito.numeroDoItem).categoriaDoItem;
    const int quantidadeQueCabe = std::min(quantidade, std::max(0, inventario.espacoLivreNaCategoria(categoria)));
    inventario.tentarPor(efeito.numeroDoItem, DonoDoItem::Cidade, quantidadeQueCabe);
    return "pôs " + std::to_string(quantidadeQueCabe) + " " + nomeDoItem;
}

// O efeito "mercador_compra" (pontual): leva o item da Cidade com mais dias
// de estoque (estoque ÷ consumo previsto; os itens sem consumo ficam de fora),
// a 90% do preço local; o dinheiro entra no Banco, vindo de fora da vila.
std::string venderAoMercador(ContextoDaProducao& contexto, const EfeitoDoEvento& efeito)
{
    int itemEscolhido = -1;
    int64_t maisDiasEmMilesimos = 0;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        const int estoque = contexto.inventario.quantidadeDe(item.numeroInterno, DonoDoItem::Cidade);
        const int64_t consumo = contexto.consumoMedido.consumoPrevistoDoDiaEmCentesimos(item.numeroInterno);
        if (estoque <= 0 || consumo <= 0) {
            continue;
        }
        const int64_t diasEmMilesimos = static_cast<int64_t>(estoque) * 100 * 1000 / consumo;
        if (diasEmMilesimos > maisDiasEmMilesimos) {
            maisDiasEmMilesimos = diasEmMilesimos;
            itemEscolhido = item.numeroInterno;
        }
    }
    if (itemEscolhido < 0) {
        return "não achou nada para comprar";
    }
    const int quantidade = std::min(efeito.quantidade, contexto.inventario.quantidadeDe(itemEscolhido, DonoDoItem::Cidade));
    const long long valor = arredondarMetadeParaCima(static_cast<long long>(quantidade) * precoLocalDoItem(contexto, itemEscolhido)
                                                         * contexto.dados.regras.economia.lojaDaCidadeCompraEmPorcento,
                                                     100);
    contexto.inventario.tentarTirar(itemEscolhido, DonoDoItem::Cidade, quantidade);
    contexto.dinheiro.bancoDaCidade += valor;
    contexto.dinheiro.dinheiroQueEntrouDeFora += valor;
    return "comprou " + std::to_string(quantidade) + " " + nomeDoItemDoEvento(contexto, itemEscolhido) + " por "
        + escreverDinheiroEmMoedas(static_cast<int>(valor));
}

// O efeito "npcs_doentes": sorteia N Civis com emprego, que ficam em casa pelos dias do evento.
std::string deixarNpcsDoentes(ContextoDaProducao& contexto, const EfeitoDoEvento& efeito, int diasDoEvento)
{
    entt::registry& registro = contexto.registroDasEntidades;
    std::vector<std::pair<int, entt::entity>> candidatos;
    for (const auto [npc, identidade, casaETrabalho, trabalhoDoDia] : registro.view<IdentidadeDoNpc, CasaETrabalhoDoNpc, TrabalhoDoDia>().each()) {
        if (casaETrabalho.indiceDoTrabalho >= 0 && !registro.all_of<DoencaDoNpc>(npc)) {
            candidatos.emplace_back(identidade.numeroDoNpc, npc);
        }
    }
    std::sort(candidatos.begin(), candidatos.end(), [](const auto& primeiro, const auto& segundo) { return primeiro.first < segundo.first; });
    std::string nomes;
    for (int doente = 0; doente < efeito.quantidade && !candidatos.empty(); ++doente) {
        const int sorteado = contexto.sorteioDosEventos.sortearNumeroInteiroEntre(0, static_cast<int>(candidatos.size()) - 1);
        const entt::entity npc = candidatos[sorteado].second;
        registro.emplace<DoencaDoNpc>(npc, DoencaDoNpc{std::max(diasDoEvento, 1)});
        nomes += (nomes.empty() ? "" : " e ") + registro.get<IdentidadeDoNpc>(npc).nome;
        candidatos.erase(candidatos.begin() + sorteado);
    }
    return nomes.empty() ? "ninguém ficou doente" : nomes + " doentes em casa";
}

// O texto de um efeito que dura (a produção e a Felicidade).
std::string descreverEfeitoQueDura(const ContextoDaProducao& contexto, const EfeitoDoEvento& efeito)
{
    if (efeito.tipo == TipoDeEfeito::ProducaoDaConstrucao) {
        return "a " + contexto.dados.tabelaDeTextos.textoDe(efeito.tipoDaConstrucao) + " produz " + std::to_string(efeito.porcentagem) + "%";
    }
    return "Felicidade " + escreverComSinal(efeito.pontosDeFelicidade);
}

// Começa o evento (regra 4.3, passo 2): os efeitos pontuais acontecem agora,
// e o resto vale enquanto ele estiver ativo.
void comecarOEvento(ContextoDaProducao& contexto, int indiceDoEvento, int64_t hoje)
{
    const DefinicaoDeEvento& evento = contexto.dados.catalogoDeEventos.todosOsEventos()[indiceDoEvento];
    EstadoDosEventos& estado = contexto.eventos;
    std::string descricao;
    for (const EfeitoDoEvento& efeito : evento.efeitos) {
        std::string parte;
        switch (efeito.tipo) {
        case TipoDeEfeito::MudarEstoque: parte = mudarOEstoque(contexto, efeito); break;
        case TipoDeEfeito::MercadorCompra: parte = venderAoMercador(contexto, efeito); break;
        case TipoDeEfeito::NpcsDoentes: parte = deixarNpcsDoentes(contexto, efeito, evento.duracaoEmDias); break;
        case TipoDeEfeito::ProducaoDaConstrucao:
        case TipoDeEfeito::Felicidade: parte = descreverEfeitoQueDura(contexto, efeito); break;
        }
        descricao += (descricao.empty() ? "" : ", ") + parte;
    }
    if (evento.tipo == TipoDeEvento::Condicional) {
        descricao += ", enquanto valer";
        estado.eventosAtivos.push_back({indiceDoEvento, 0});
    } else if (evento.ehPontual()) {
        estado.diaEmQuePodeSairDeNovo[indiceDoEvento] = hoje + evento.cooldownEmDias;
    } else {
        descricao += " por " + std::to_string(evento.duracaoEmDias) + (evento.duracaoEmDias == 1 ? " dia" : " dias");
        estado.eventosAtivos.push_back({indiceDoEvento, evento.duracaoEmDias});
    }
    estado.linhasParaOConsole.push_back("Evento: " + nomeDoEvento(contexto, evento) + " (" + descricao + ").");
    estado.mudancasDeHoje.push_back(nomeDoEvento(contexto, evento) + (evento.ehPontual() ? "" : " começou"));
}

void terminarOEvento(ContextoDaProducao& contexto, std::size_t posicaoNosAtivos, int64_t hoje)
{
    EstadoDosEventos& estado = contexto.eventos;
    const int indiceDoEvento = estado.eventosAtivos[posicaoNosAtivos].indiceDoEvento;
    const DefinicaoDeEvento& evento = contexto.dados.catalogoDeEventos.todosOsEventos()[indiceDoEvento];
    estado.eventosAtivos.erase(estado.eventosAtivos.begin() + static_cast<std::ptrdiff_t>(posicaoNosAtivos));
    estado.diaEmQuePodeSairDeNovo[indiceDoEvento] = hoje + evento.cooldownEmDias;
    estado.diasSeguidosNaCondicao[indiceDoEvento] = 0;
    estado.linhasParaOConsole.push_back("Fim do evento: " + nomeDoEvento(contexto, evento) + ".");
    estado.mudancasDeHoje.push_back(nomeDoEvento(contexto, evento) + " terminou");
}

// A condição de entrada de um condicional vale hoje?
bool condicaoDeEntradaVale(const ContextoDaProducao& contexto, const CondicaoDoEvento& condicao)
{
    if (condicao.tipo == TipoDeCondicao::ComidaAbaixoDoAlvo) {
        return comidaEmPorcentoDoAlvo(contexto) < condicao.entraAbaixoDoAlvoEmPorcento;
    }
    return contexto.dinheiro.diasEmDeficitDeMateriais > 0;
}

// A condição de saída (a histerese): a Fome só sai acima de 30% do alvo.
bool condicaoDeSaidaVale(const ContextoDaProducao& contexto, const CondicaoDoEvento& condicao)
{
    if (condicao.tipo == TipoDeCondicao::ComidaAbaixoDoAlvo) {
        return comidaEmPorcentoDoAlvo(contexto) > condicao.saiAcimaDoAlvoEmPorcento;
    }
    return contexto.dinheiro.diasEmDeficitDeMateriais == 0;
}

// Regra 4.3, passo 1: o ocasional pode sair hoje?
bool ocasionalPodeSair(const ContextoDaProducao& contexto, int indiceDoEvento, const RelogioDoMundo& relogio)
{
    const DefinicaoDeEvento& evento = contexto.dados.catalogoDeEventos.todosOsEventos()[indiceDoEvento];
    const bool estacaoBate = evento.estacoes.empty()
        || std::find(evento.estacoes.begin(), evento.estacoes.end(), relogio.estacaoAtual()) != evento.estacoes.end();
    const bool diaBate = evento.diasDaEstacao.empty()
        || std::find(evento.diasDaEstacao.begin(), evento.diasDaEstacao.end(), relogio.diaDaEstacaoAtual()) != evento.diasDaEstacao.end();
    return estacaoBate && diaBate && !contexto.eventos.eventoEstaAtivo(indiceDoEvento)
        && relogio.diaDaPartida() >= contexto.eventos.diaEmQuePodeSairDeNovo[indiceDoEvento]
        && (!evento.negativo || quantidadeDeNegativosAtivos(contexto) < eventosNegativosAtivosNoMaximo);
}

}  // namespace

bool EstadoDosEventos::eventoEstaAtivo(int indiceDoEvento) const
{
    return std::any_of(eventosAtivos.begin(), eventosAtivos.end(), [&](const EventoAtivo& ativo) { return ativo.indiceDoEvento == indiceDoEvento; });
}

void comecarOsEventos(ContextoDaProducao& contexto)
{
    const std::size_t quantidade = contexto.dados.catalogoDeEventos.todosOsEventos().size();
    contexto.eventos = EstadoDosEventos{};
    contexto.eventos.diaEmQuePodeSairDeNovo.assign(quantidade, 0);
    contexto.eventos.diasSeguidosNaCondicao.assign(quantidade, 0);
}

void rodarOsEventosDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio)
{
    EstadoDosEventos& estado = contexto.eventos;
    const std::vector<DefinicaoDeEvento>& eventos = contexto.dados.catalogoDeEventos.todosOsEventos();
    const int64_t hoje = relogio.diaDaPartida();

    // 4.1. Os ativos descontam 1 dia (os condicionais, não: duram enquanto valer).
    for (std::size_t posicao = estado.eventosAtivos.size(); posicao-- > 0;) {
        EventoAtivo& ativo = estado.eventosAtivos[posicao];
        if (eventos[ativo.indiceDoEvento].tipo == TipoDeEvento::Condicional) {
            continue;
        }
        if (--ativo.diasQueFaltam <= 0) {
            terminarOEvento(contexto, posicao, hoje);
        }
    }
    // Os NPCs doentes descontam 1 dia; com 0, voltam à rotina.
    entt::registry& registro = contexto.registroDasEntidades;
    std::vector<entt::entity> curados;
    for (const auto [npc, doenca] : registro.view<DoencaDoNpc>().each()) {
        if (--doenca.diasQueFaltam <= 0) {
            curados.push_back(npc);
        }
    }
    for (const entt::entity npc : curados) {
        registro.remove<DoencaDoNpc>(npc);
    }

    // 4.2. Os condicionais e as datas fixas.
    for (std::size_t indice = 0; indice < eventos.size(); ++indice) {
        const DefinicaoDeEvento& evento = eventos[indice];
        const int indiceDoEvento = static_cast<int>(indice);
        if (evento.tipo == TipoDeEvento::Condicional && evento.condicao.has_value()) {
            const CondicaoDoEvento& condicao = *evento.condicao;
            if (estado.eventoEstaAtivo(indiceDoEvento)) {
                if (condicaoDeSaidaVale(contexto, condicao)) {
                    const auto ativo = std::find_if(estado.eventosAtivos.begin(), estado.eventosAtivos.end(),
                                                    [&](const EventoAtivo& outro) { return outro.indiceDoEvento == indiceDoEvento; });
                    terminarOEvento(contexto, static_cast<std::size_t>(ativo - estado.eventosAtivos.begin()), hoje);
                }
                continue;
            }
            estado.diasSeguidosNaCondicao[indice] = condicaoDeEntradaVale(contexto, condicao) ? estado.diasSeguidosNaCondicao[indice] + 1 : 0;
            if (estado.diasSeguidosNaCondicao[indice] >= condicao.diasSeguidos && hoje >= estado.diaEmQuePodeSairDeNovo[indice]) {
                comecarOEvento(contexto, indiceDoEvento, hoje);
            }
        } else if (evento.tipo == TipoDeEvento::DataFixa) {
            if (relogio.estacaoAtual() == evento.estacaoDaDataFixa && relogio.diaDaEstacaoAtual() == evento.diaDaDataFixa
                && !estado.eventoEstaAtivo(indiceDoEvento)) {
                comecarOEvento(contexto, indiceDoEvento, hoje);
            }
        }
    }

    // 4.3. Os ocasionais, na ordem do arquivo. Só os que podem sair gastam um
    // número do sorteio: assim a ordem dos sorteios é sempre a mesma.
    for (std::size_t indice = 0; indice < eventos.size(); ++indice) {
        const int indiceDoEvento = static_cast<int>(indice);
        if (eventos[indice].tipo != TipoDeEvento::Ocasional || !ocasionalPodeSair(contexto, indiceDoEvento, relogio)) {
            continue;
        }
        const int numeroSorteado = contexto.sorteioDosEventos.sortearNumeroInteiroEntre(1, 1000);
        if (numeroSorteado <= eventos[indice].chancePorDiaEmPorMil) {
            comecarOEvento(contexto, indiceDoEvento, hoje);
        }
    }
}

int multiplicadorDaProducaoPorEventos(const ContextoDaProducao& contexto, const std::string& tipoDaConstrucao)
{
    int multiplicadorEmPorcento = 100;
    for (const EventoAtivo& ativo : contexto.eventos.eventosAtivos) {
        for (const EfeitoDoEvento& efeito : contexto.dados.catalogoDeEventos.todosOsEventos()[ativo.indiceDoEvento].efeitos) {
            if (efeito.tipo == TipoDeEfeito::ProducaoDaConstrucao && efeito.tipoDaConstrucao == tipoDaConstrucao) {
                multiplicadorEmPorcento = multiplicadorEmPorcento * efeito.porcentagem / 100;
            }
        }
    }
    return multiplicadorEmPorcento;
}

int felicidadeDosEventosAtivos(const ContextoDaProducao& contexto)
{
    int pontos = 0;
    for (const EventoAtivo& ativo : contexto.eventos.eventosAtivos) {
        for (const EfeitoDoEvento& efeito : contexto.dados.catalogoDeEventos.todosOsEventos()[ativo.indiceDoEvento].efeitos) {
            pontos += efeito.tipo == TipoDeEfeito::Felicidade ? efeito.pontosDeFelicidade : 0;
        }
    }
    return pontos;
}

int comidaEmPorcentoDoAlvo(const ContextoDaProducao& contexto)
{
    int64_t comidaNoEstoque = 0;
    int64_t alvoDaComidaEmCentesimos = 0;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        if (item.ehComida()) {
            comidaNoEstoque += contexto.inventario.quantidadeTotalDe(item.numeroInterno);
            alvoDaComidaEmCentesimos += estoqueAlvoDoItemEmCentesimos(contexto, item.numeroInterno);
        }
    }
    if (alvoDaComidaEmCentesimos <= 0) {
        return 100;
    }
    return static_cast<int>(comidaNoEstoque * 100 * 100 / alvoDaComidaEmCentesimos);
}
