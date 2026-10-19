#include "Jogador/ObrasDaVila.h"

#include <algorithm>

#include "Construcao/ContasDasConstrucoes.h"
#include "Dados/Receitas.h"
#include "Inventario/InventarioDaCidade.h"
#include "Jogador/DecisoesDoJogador.h"
#include "Npc/ComponentesDoNpc.h"

namespace {

int dividirParaCima(int numerador, int denominador)
{
    return (numerador + denominador - 1) / denominador;
}

int numeroDoItemPeloIdentificador(const ContextoDaProducao& contexto, const char* identificador)
{
    return contexto.dados.catalogoDeItens.procurarNumeroDoItem(identificador).value_or(-1);
}

PorteDoLote porteDaConstrucao(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    const DefinicaoDeConstrucao* definicao = contexto.dados.catalogoDeConstrucoes.procurarConstrucao(contexto.vila.construcoes[indiceDaConstrucao].tipo);
    return definicao == nullptr ? PorteDoLote::Pequeno : definicao->porte;
}

// De onde saem as Tábuas e os Blocos de uma obra ou manutenção, e o que isso custa.
struct PlanoDeMateriais {
    int tabuasDoDono = 0;
    int tabuasComprarDaLoja = 0;     // o jogador compra da Cidade, a 110%
    int tabuasComprarDoJogador = 0;  // a Cidade compra do jogador, pelo preço dele
    int blocosDoDono = 0;
    int blocosComprarDaLoja = 0;
    int64_t custoDaCompraDaLoja = 0;     // o jogador paga ao Banco
    int64_t custoDaCompraDoJogador = 0;  // o Banco paga ao jogador
    bool cabe = false;
    std::string motivoDeNaoCaber;
};

// Planeja os materiais. O dono é o jogador (as obras dele) ou a Cidade (a
// manutenção das construções da Cidade). O dinheiro das horas entra na
// conta de quem é o jogador.
PlanoDeMateriais planejarOsMateriais(const ContextoDaProducao& contexto, bool donoEhOJogador, int tabuas, int blocos, int64_t dinheiroDasHoras)
{
    PlanoDeMateriais plano;
    const int tabua = numeroDoItemPeloIdentificador(contexto, "item.tabua");
    const int bloco = numeroDoItemPeloIdentificador(contexto, "item.bloco_de_pedra");
    const RegrasDaEconomia& economia = contexto.dados.regras.economia;
    const DonoDoItem dono = donoEhOJogador ? DonoDoItem::Jogador : DonoDoItem::Cidade;
    plano.tabuasDoDono = std::min(tabuas, contexto.inventario.quantidadeDe(tabua, dono));
    plano.blocosDoDono = std::min(blocos, contexto.inventario.quantidadeDe(bloco, dono));
    const int tabuasQueFaltam = tabuas - plano.tabuasDoDono;
    const int blocosQueFaltam = blocos - plano.blocosDoDono;
    if (donoEhOJogador) {
        // O que falta, o jogador compra da loja da cidade (só a Cidade tem Blocos).
        plano.tabuasComprarDaLoja = tabuasQueFaltam;
        plano.blocosComprarDaLoja = blocosQueFaltam;
        if (contexto.inventario.quantidadeDe(tabua, DonoDoItem::Cidade) < plano.tabuasComprarDaLoja
            || contexto.inventario.quantidadeDe(bloco, DonoDoItem::Cidade) < plano.blocosComprarDaLoja) {
            plano.motivoDeNaoCaber = "falta material na loja da cidade";
            return plano;
        }
        plano.custoDaCompraDaLoja = static_cast<int64_t>(plano.tabuasComprarDaLoja) * calcularPrecoDaLojaDaCidade(precoLocalDoItem(contexto, tabua), economia)
            + static_cast<int64_t>(plano.blocosComprarDaLoja) * calcularPrecoDaLojaDaCidade(precoLocalDoItem(contexto, bloco), economia);
        if (contexto.dinheiro.dinheiroDoJogador < plano.custoDaCompraDaLoja + dinheiroDasHoras) {
            plano.motivoDeNaoCaber = "falta dinheiro";
            return plano;
        }
    } else {
        // A Cidade tem os Blocos dela; as Tábuas, ela compra do jogador, pelo preço dele.
        if (blocosQueFaltam > 0) {
            plano.motivoDeNaoCaber = "faltam Blocos de Pedra";
            return plano;
        }
        plano.tabuasComprarDoJogador = tabuasQueFaltam;
        if (contexto.inventario.quantidadeDe(tabua, DonoDoItem::Jogador) < plano.tabuasComprarDoJogador) {
            plano.motivoDeNaoCaber = "faltam Tábuas";
            return plano;
        }
        plano.custoDaCompraDoJogador = static_cast<int64_t>(plano.tabuasComprarDoJogador)
            * calcularPrecoDoItemDoJogador(precoLocalDoItem(contexto, tabua), contexto.decisoes.porcentagemDoPrecoDe(tabua));
        if (contexto.dinheiro.bancoDaCidade < plano.custoDaCompraDoJogador) {
            plano.motivoDeNaoCaber = "falta dinheiro no Banco";
            return plano;
        }
    }
    plano.cabe = true;
    return plano;
}

void executarOPlanoDeMateriais(ContextoDaProducao& contexto, bool donoEhOJogador, const PlanoDeMateriais& plano)
{
    const int tabua = numeroDoItemPeloIdentificador(contexto, "item.tabua");
    const int bloco = numeroDoItemPeloIdentificador(contexto, "item.bloco_de_pedra");
    const DonoDoItem dono = donoEhOJogador ? DonoDoItem::Jogador : DonoDoItem::Cidade;
    contexto.inventario.tentarTirar(tabua, dono, plano.tabuasDoDono);
    contexto.inventario.tentarTirar(bloco, dono, plano.blocosDoDono);
    contexto.inventario.tentarTirar(tabua, DonoDoItem::Cidade, plano.tabuasComprarDaLoja);
    contexto.inventario.tentarTirar(bloco, DonoDoItem::Cidade, plano.blocosComprarDaLoja);
    contexto.inventario.tentarTirar(tabua, DonoDoItem::Jogador, plano.tabuasComprarDoJogador);
    contexto.dinheiro.dinheiroDoJogador -= plano.custoDaCompraDaLoja;
    contexto.dinheiro.bancoDaCidade += plano.custoDaCompraDaLoja;
    contexto.dinheiro.bancoDaCidade -= plano.custoDaCompraDoJogador;
    contexto.dinheiro.dinheiroDoJogador += plano.custoDaCompraDoJogador;
}

bool construtorPronto(const ConstrucaoDaVila& construcao)
{
    return construcao.tipo == tipoDoConstrutor && !construcao.emObra;
}

// Tenta pagar uma manutenção que espera. Devolve true se pagou.
bool tentarPagarAManutencao(ContextoDaProducao& contexto, ObraNaFila& obra)
{
    const ConstrucaoDaVila& construcao = contexto.vila.construcoes[obra.indiceDaConstrucao];
    const CustoDaObra custo = calcularCustoDaManutencao(porteDaConstrucao(contexto, obra.indiceDaConstrucao), construcao.nivel, contexto.dados.regras.construcoes);
    // As horas da Cidade já saem dos salários dos Operários; as do jogador, ele paga ao Banco.
    const int64_t horas = construcao.donoEhOJogador ? calcularPagamentoDasHorasDeObra(custo.horasDeTrabalho, contexto) : 0;
    const PlanoDeMateriais plano = planejarOsMateriais(contexto, construcao.donoEhOJogador, custo.tabuas, custo.blocosDePedra, horas);
    if (!plano.cabe) {
        return false;
    }
    executarOPlanoDeMateriais(contexto, construcao.donoEhOJogador, plano);
    contexto.dinheiro.dinheiroDoJogador -= horas;
    contexto.dinheiro.bancoDaCidade += horas;
    obra.esperaMaterial = false;
    obra.centesimosDeHoraQueFaltam = static_cast<int64_t>(custo.horasDeTrabalho) * 100;
    return true;
}

void terminarAObra(ContextoDaProducao& contexto, const ObraNaFila& obra)
{
    ConstrucaoDaVila& construcao = contexto.vila.construcoes[obra.indiceDaConstrucao];
    ProducaoDaConstrucao& producao = contexto.producaoDasConstrucoes[obra.indiceDaConstrucao];
    switch (obra.tipo) {
    case TipoDeObra::ConstrucaoNova:
        construcao.emObra = false;
        producao.emObra = false;
        for (const DefinicaoDeReceita& receita : contexto.dados.catalogoDeReceitas.todasAsReceitas()) {
            producao.ehDeProducao = producao.ehDeProducao || receita.construcaoQueProduz == construcao.tipo;
        }
        recalcularCapacidade(contexto.inventario, contexto.vila, contexto.dados.catalogoDeConstrucoes, contexto.dados.regras.construcoes);
        contexto.obras.linhasParaOConsole.push_back("Obra pronta: " + construcao.identificador + " (lote " + std::to_string(construcao.numeroDoLote) + ").");
        break;
    case TipoDeObra::SubirDeNivel:
        construcao.nivel = obra.nivelDeDestino;
        recalcularCapacidade(contexto.inventario, contexto.vila, contexto.dados.catalogoDeConstrucoes, contexto.dados.regras.construcoes);
        contexto.obras.linhasParaOConsole.push_back("Obra pronta: " + construcao.identificador + " subiu para o nível " + std::to_string(construcao.nivel) + ".");
        break;
    case TipoDeObra::Manutencao:
        producao.manutencaoAtrasada = false;
        contexto.obras.linhasParaOConsole.push_back("Obra pronta: manutenção de " + construcao.identificador + ".");
        break;
    }
}

int tamanhoDoLoteEmTiles(PorteDoLote porte)
{
    return porte == PorteDoLote::Pequeno ? 4 : (porte == PorteDoLote::Medio ? 6 : 8);
}

}  // namespace

CustoDaObra calcularCustoDaObra(PorteDoLote porte, int nivelDeDestino, const RegrasDasConstrucoes& regras)
{
    const CustoDaObra& base = regras.obraDeCadaPorte[static_cast<int>(porte)];
    return CustoDaObra{base.tabuas * nivelDeDestino, base.blocosDePedra * nivelDeDestino, base.barrasDeFerro * nivelDeDestino,
                       base.horasDeTrabalho * nivelDeDestino};
}

CustoDaObra calcularCustoDaManutencao(PorteDoLote porte, int nivelAtual, const RegrasDasConstrucoes& regras)
{
    const CustoDaObra materiais = calcularCustoDaObra(porte, nivelAtual, regras);
    const int porcento = regras.manutencaoEmPorcento;
    CustoDaObra custo;
    custo.tabuas = std::max(1, dividirParaCima(materiais.tabuas * porcento, 100));
    custo.blocosDePedra = dividirParaCima(materiais.blocosDePedra * porcento, 100);
    custo.barrasDeFerro = dividirParaCima(materiais.barrasDeFerro * porcento, 100);
    custo.horasDeTrabalho = dividirParaCima(materiais.horasDeTrabalho * porcento, 100);
    return custo;
}

int64_t calcularPagamentoDasHorasDeObra(int horas, const ContextoDaProducao& contexto)
{
    return arredondarMetadeParaCima(static_cast<long long>(horas) * contexto.dados.regras.regrasDoPreco.salarioPorHoraBracalEmCentesimos, 10);
}

bool construcaoTemObraNaFila(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    return std::any_of(contexto.obras.fila.begin(), contexto.obras.fila.end(),
                       [&](const ObraNaFila& obra) { return obra.indiceDaConstrucao == indiceDaConstrucao; });
}

std::string encomendarUmaConstrucaoNova(ContextoDaProducao& contexto, int numeroDoLote, const std::string& tipoDeConstrucao)
{
    const DefinicaoDeConstrucao* definicao = contexto.dados.catalogoDeConstrucoes.procurarConstrucao(tipoDeConstrucao);
    LoteDeConstrucao* lote = nullptr;
    for (LoteDeConstrucao& candidato : contexto.mapa.lotesDeConstrucao) {
        if (candidato.identificadorDoLote == numeroDoLote) {
            lote = &candidato;
        }
    }
    if (definicao == nullptr || lote == nullptr) {
        return "o lote ou o tipo de construção não existe";
    }
    if (lote->idDaConstrucaoQueOcupa != loteSemConstrucao) {
        return "o lote não está livre";
    }
    if (lote->porteDoLote != definicao->porte) {
        return "o tipo não cabe no porte do lote";
    }
    const CustoDaObra custo = calcularCustoDaObra(definicao->porte, 1, contexto.dados.regras.construcoes);
    if (custo.barrasDeFerro > 0) {
        return "a fatia não tem Barras de Ferro";
    }
    const int64_t horas = calcularPagamentoDasHorasDeObra(custo.horasDeTrabalho, contexto);
    const PlanoDeMateriais plano = planejarOsMateriais(contexto, true, custo.tabuas, custo.blocosDePedra, horas);
    if (!plano.cabe) {
        return plano.motivoDeNaoCaber;
    }
    executarOPlanoDeMateriais(contexto, true, plano);
    contexto.dinheiro.dinheiroDoJogador -= horas;
    contexto.dinheiro.bancoDaCidade += horas;

    ConstrucaoDaVila nova;
    nova.tipo = tipoDeConstrucao;
    nova.identificador = tipoDeConstrucao.substr(tipoDeConstrucao.find('.') + 1) + "_do_jogador_" + std::to_string(numeroDoLote);
    nova.numeroDoLote = numeroDoLote;
    nova.donoEhOJogador = true;
    nova.nivel = 1;
    nova.emObra = true;
    nova.areaDoLote = RetanguloEmTiles{lote->tileDoCantoSuperiorEsquerdo.tileX, lote->tileDoCantoSuperiorEsquerdo.tileY, tamanhoDoLoteEmTiles(definicao->porte),
                                       tamanhoDoLoteEmTiles(definicao->porte)};
    nova.tileDaPorta = calcularTileDaPorta(nova.areaDoLote);
    contexto.vila.construcoes.push_back(nova);
    ProducaoDaConstrucao producao;
    producao.emObra = true;
    contexto.producaoDasConstrucoes.push_back(producao);
    bloquearOsLotesOcupados(contexto.mapa, contexto.vila);

    ObraNaFila obra;
    obra.tipo = TipoDeObra::ConstrucaoNova;
    obra.indiceDaConstrucao = static_cast<int>(contexto.vila.construcoes.size()) - 1;
    obra.nivelDeDestino = 1;
    obra.centesimosDeHoraQueFaltam = static_cast<int64_t>(custo.horasDeTrabalho) * 100;
    contexto.obras.fila.push_back(obra);
    return "";
}

std::string encomendarUmaSubidaDeNivel(ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    if (indiceDaConstrucao < 0 || indiceDaConstrucao >= static_cast<int>(contexto.vila.construcoes.size())) {
        return "a construção não existe";
    }
    const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indiceDaConstrucao];
    if (!construcao.donoEhOJogador || construcao.emObra) {
        return "só as construções prontas do jogador sobem de nível";
    }
    if (construcaoTemObraNaFila(contexto, indiceDaConstrucao)) {
        return "a construção já tem uma obra na fila";
    }
    const int nivelDeDestino = construcao.nivel + 1;
    if (nivelDeDestino > contexto.dados.regras.construcoes.nivelMaximoComOEngenheiro) {
        return "o Engenheiro só faz obras até o nível " + std::to_string(contexto.dados.regras.construcoes.nivelMaximoComOEngenheiro);
    }
    const CustoDaObra custo = calcularCustoDaObra(porteDaConstrucao(contexto, indiceDaConstrucao), nivelDeDestino, contexto.dados.regras.construcoes);
    if (custo.barrasDeFerro > 0) {
        return "a fatia não tem Barras de Ferro";
    }
    const int64_t horas = calcularPagamentoDasHorasDeObra(custo.horasDeTrabalho, contexto);
    const PlanoDeMateriais plano = planejarOsMateriais(contexto, true, custo.tabuas, custo.blocosDePedra, horas);
    if (!plano.cabe) {
        return plano.motivoDeNaoCaber;
    }
    executarOPlanoDeMateriais(contexto, true, plano);
    contexto.dinheiro.dinheiroDoJogador -= horas;
    contexto.dinheiro.bancoDaCidade += horas;
    ObraNaFila obra;
    obra.tipo = TipoDeObra::SubirDeNivel;
    obra.indiceDaConstrucao = indiceDaConstrucao;
    obra.nivelDeDestino = nivelDeDestino;
    obra.centesimosDeHoraQueFaltam = static_cast<int64_t>(custo.horasDeTrabalho) * 100;
    contexto.obras.fila.push_back(obra);
    return "";
}

void andarAsObrasDaHora(ContextoDaProducao& contexto)
{
    // As horas dos Operários nesta hora, em centésimos de hora:
    // ticks × (50 + habilidade) ÷ ticks por hora (800 ticks a 100% = 100).
    int64_t centesimosDaHora = 0;
    for (const auto [npc, casaETrabalho, habilidades, trabalhoDoDia] :
         contexto.registroDasEntidades.view<CasaETrabalhoDoNpc, HabilidadesDoNpc, TrabalhoDoDia>().each()) {
        if (casaETrabalho.indiceDoTrabalho >= 0 && construtorPronto(contexto.vila.construcoes[casaETrabalho.indiceDoTrabalho])) {
            centesimosDaHora += static_cast<int64_t>(trabalhoDoDia.ticksTrabalhadosNestaHora) * (50 + habilidades.bracal) / contexto.ticksPorHoraDeJogo;
        }
    }
    if (centesimosDaHora <= 0) {
        return;
    }
    // A obra da vez: a primeira manutenção que já tem material; senão, a primeira obra.
    std::vector<ObraNaFila>& fila = contexto.obras.fila;
    auto daVez = std::find_if(fila.begin(), fila.end(), [](const ObraNaFila& obra) { return obra.tipo == TipoDeObra::Manutencao && !obra.esperaMaterial; });
    if (daVez == fila.end()) {
        daVez = std::find_if(fila.begin(), fila.end(), [](const ObraNaFila& obra) { return obra.tipo != TipoDeObra::Manutencao; });
    }
    if (daVez == fila.end()) {
        return;
    }
    daVez->centesimosDeHoraQueFaltam -= centesimosDaHora;
    if (daVez->centesimosDeHoraQueFaltam <= 0) {
        const ObraNaFila pronta = *daVez;
        fila.erase(daVez);
        terminarAObra(contexto, pronta);
    }
}

void rodarAManutencaoDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio)
{
    std::vector<ObraNaFila>& fila = contexto.obras.fila;
    if (relogio.diaDaEstacaoAtual() == 1) {
        for (int indice = 0; indice < static_cast<int>(contexto.vila.construcoes.size()); ++indice) {
            if (contexto.vila.construcoes[indice].emObra) {
                continue;
            }
            const bool temManutencaoNaFila = std::any_of(fila.begin(), fila.end(), [&](const ObraNaFila& obra) {
                return obra.indiceDaConstrucao == indice && obra.tipo == TipoDeObra::Manutencao;
            });
            if (temManutencaoNaFila) {
                // A da estação passada não ficou pronta: a construção está atrasada e a nova não entra.
                contexto.producaoDasConstrucoes[indice].manutencaoAtrasada = true;
                continue;
            }
            ObraNaFila obra;
            obra.tipo = TipoDeObra::Manutencao;
            obra.indiceDaConstrucao = indice;
            obra.nivelDeDestino = contexto.vila.construcoes[indice].nivel;
            obra.esperaMaterial = true;
            fila.push_back(obra);
        }
    }
    for (ObraNaFila& obra : fila) {
        if (obra.tipo == TipoDeObra::Manutencao && obra.esperaMaterial) {
            tentarPagarAManutencao(contexto, obra);
        }
    }
}
