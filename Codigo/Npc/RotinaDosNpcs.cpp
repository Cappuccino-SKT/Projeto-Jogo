#include "Npc/RotinaDosNpcs.h"

#include <algorithm>
#include <cstdlib>
#include <utility>

#include "Npc/MovimentoDosNpcs.h"
#include "Producao/ProducaoDaVila.h"

namespace {

constexpr int tentativasDeSortearOPasseio = 20;

// A situação de um NPC agora, para as notas.
SituacaoDoNpc montarSituacaoDoNpc(const ContextoDaRotina& contexto, const RelogioDoMundo& relogio, const IdentidadeDoNpc& identidade,
                                  const CasaETrabalhoDoNpc& casaETrabalho, const DecisaoDoNpc& decisao)
{
    const RegrasDaUtilidade& utilidade = contexto.regras.utilidade;
    SituacaoDoNpc situacao;
    situacao.periodo = descobrirPeriodoDaRotina(relogio.horaDoDiaAtual(), contexto.regras.rotinaDoCivil);
    situacao.temEmprego = casaETrabalho.indiceDoTrabalho >= 0;
    situacao.estaNaHoraDeComer = estaNaHoraDeComer(relogio.horaDoDiaAtual(), relogio.minutoDaHoraAtual(), utilidade);
    situacao.ehDiaDeCompra = ehDiaDeCompraDoNpc(relogio.diaDaPartida(), identidade.numeroDoNpc, utilidade.diasEntreCompras);
    situacao.jaComprouHoje = decisao.diaDaUltimaCompra == relogio.diaDaPartida();
    situacao.vilaTemMercado = contexto.vila.indiceDoMercado() >= 0;
    return situacao;
}

// Começa um passeio: sorteia o tile e pede o caminho. Sem tile, tenta de novo
// depois da menor espera do passeio.
void comecarUmPasseio(ContextoDaRotina& contexto, const RelogioDoMundo& relogio, entt::entity npc, DecisaoDoNpc& decisao)
{
    const PosicaoDoNpc& posicao = contexto.registroDasEntidades.get<PosicaoDoNpc>(npc);
    const std::optional<PosicaoDeTile> tileDoPasseio =
        sortearTileDoPasseio(contexto.mapa, contexto.vila, posicao.lugarAtual.proximoTile, contexto.regras.passeio, contexto.sorteioDosNpcs);
    decisao.indiceDaConstrucaoDoDestino = -1;
    if (tileDoPasseio.has_value()) {
        pedirCaminhoParaONpc(contexto.registroDasEntidades, npc, *tileDoPasseio, relogio.tickAtual());
        decisao.estaACaminho = true;
    } else {
        decisao.estaACaminho = false;
        decisao.tickEmQueAEsperaAcaba =
            relogio.tickAtual() + converterMinutosEmTicks(contexto.regras.passeio.menorEsperaEmMinutos, contexto.ticksPorHoraDeJogo);
    }
}

// Regra 4.4, passo 1: entra na construção (e some da tela). Na compra, a espera começa.
void entrarNaConstrucao(const ContextoDaRotina& contexto, const RelogioDoMundo& relogio, DecisaoDoNpc& decisao, int indiceDaConstrucao)
{
    decisao.indiceDaConstrucaoOndeEsta = indiceDaConstrucao;
    decisao.estaACaminho = false;
    if (decisao.acaoAtual == AcaoDoNpc::Comprar) {
        decisao.tickEmQueAEsperaAcaba =
            relogio.tickAtual() + converterMinutosEmTicks(contexto.regras.utilidade.minutosDaCompra, contexto.ticksPorHoraDeJogo);
    }
}

// Os tiles do campo do trabalho do NPC (marco 6), ou nullptr se ele trabalha dentro.
const std::vector<PosicaoDeTile>* tilesDoCampoDoTrabalho(const ContextoDaRotina& contexto, const CasaETrabalhoDoNpc& casaETrabalho)
{
    if (contexto.producaoDasConstrucoes == nullptr || casaETrabalho.indiceDoTrabalho < 0) {
        return nullptr;
    }
    const std::vector<PosicaoDeTile>& tiles = (*contexto.producaoDasConstrucoes)[casaETrabalho.indiceDoTrabalho].tilesDeTrabalhoNoCampo;
    return tiles.empty() ? nullptr : &tiles;
}

// O tile do Braçal no campo (Produção, regra 4.1): o primeiro empregado fica
// no primeiro tile livre, o segundo no segundo, e assim por diante.
PosicaoDeTile tileDoNpcNoCampo(const ContextoDaRotina& contexto, entt::entity npc, const std::vector<PosicaoDeTile>& tilesDoCampo)
{
    const entt::registry& registro = contexto.registroDasEntidades;
    const int indiceDoTrabalho = registro.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
    const int numeroDoNpc = registro.get<IdentidadeDoNpc>(npc).numeroDoNpc;
    int colegasAntesDele = 0;
    for (const auto [outro, identidade, casaETrabalho] : registro.view<IdentidadeDoNpc, CasaETrabalhoDoNpc>().each()) {
        colegasAntesDele += casaETrabalho.indiceDoTrabalho == indiceDoTrabalho && identidade.numeroDoNpc < numeroDoNpc ? 1 : 0;
    }
    return tilesDoCampo[static_cast<std::size_t>(colegasAntesDele) % tilesDoCampo.size()];
}

// O Braçal já está parado no tile dele do campo?
bool estaNoTileDoCampo(const ContextoDaRotina& contexto, entt::entity npc, const DecisaoDoNpc& decisao, const std::vector<PosicaoDeTile>& tilesDoCampo)
{
    const LugarDoNpc& lugar = contexto.registroDasEntidades.get<PosicaoDoNpc>(npc).lugarAtual;
    return !decisao.estaDentroDeUmaConstrucao() && !decisao.estaACaminho && lugar.estaParado()
        && lugar.tileAtual == tileDoNpcNoCampo(contexto, npc, tilesDoCampo);
}

// Marco 6: o Braçal sai da construção (se estiver dentro) e anda até o tile dele no campo.
void irParaOCampo(ContextoDaRotina& contexto, const RelogioDoMundo& relogio, entt::entity npc, DecisaoDoNpc& decisao,
                  const std::vector<PosicaoDeTile>& tilesDoCampo)
{
    const PosicaoDeTile tileDoCampo = tileDoNpcNoCampo(contexto, npc, tilesDoCampo);
    const LugarDoNpc& lugar = contexto.registroDasEntidades.get<PosicaoDoNpc>(npc).lugarAtual;
    decisao.indiceDaConstrucaoDoDestino = -1;
    decisao.indiceDaConstrucaoOndeEsta = -1;
    if (lugar.estaParado() && lugar.tileAtual == tileDoCampo) {
        decisao.estaACaminho = false;
        return;
    }
    pedirCaminhoParaONpc(contexto.registroDasEntidades, npc, tileDoCampo, relogio.tickAtual());
    decisao.estaACaminho = true;
}

// Regra 4.3: vai até a construção de destino (ou passeia, se o destino é -1).
void irParaODestino(ContextoDaRotina& contexto, const RelogioDoMundo& relogio, entt::entity npc, DecisaoDoNpc& decisao,
                    int indiceDoDestino, bool vaiPassear)
{
    decisao.indiceDaConstrucaoDoDestino = indiceDoDestino;
    // Já está lá dentro (o Lojista que compra no próprio Mercado, quem come no
    // trabalho): não sai, só começa o que veio fazer.
    if (indiceDoDestino >= 0 && indiceDoDestino == decisao.indiceDaConstrucaoOndeEsta) {
        entrarNaConstrucao(contexto, relogio, decisao, indiceDoDestino);
        return;
    }
    // Sai pela porta: a PosicaoDoNpc já está nela.
    decisao.indiceDaConstrucaoOndeEsta = -1;
    if (indiceDoDestino >= 0) {
        pedirCaminhoParaONpc(contexto.registroDasEntidades, npc, contexto.vila.construcoes[indiceDoDestino].tileDaPorta, relogio.tickAtual());
        decisao.estaACaminho = true;
    } else if (vaiPassear) {
        comecarUmPasseio(contexto, relogio, npc, decisao);
    } else {
        decisao.estaACaminho = false;  // sem casa ou sem trabalho: fica onde está
    }
}

// Regra 4.2 inteira para um NPC.
void reavaliarUmNpc(ContextoDaRotina& contexto, const RelogioDoMundo& relogio, entt::entity npc)
{
    entt::registry& registro = contexto.registroDasEntidades;
    const IdentidadeDoNpc& identidade = registro.get<IdentidadeDoNpc>(npc);
    const CasaETrabalhoDoNpc& casaETrabalho = registro.get<CasaETrabalhoDoNpc>(npc);
    DecisaoDoNpc& decisao = registro.get<DecisaoDoNpc>(npc);

    const SituacaoDoNpc situacao = montarSituacaoDoNpc(contexto, relogio, identidade, casaETrabalho, decisao);
    const AcaoDoNpc acaoNova = escolherAcaoPelaUtilidade(calcularNotasDasAcoes(situacao, contexto.regras.utilidade));
    const std::vector<PosicaoDeTile>* tilesDoCampo = tilesDoCampoDoTrabalho(contexto, casaETrabalho);
    if (acaoNova == decisao.acaoAtual) {
        // Passo 4: a mesma ação, o NPC continua o que fazia. No trabalho, o
        // lugar pode ter mudado com a receita (marco 6): o Braçal vai para o
        // campo novo, e quem perdeu o campo entra na construção.
        if (acaoNova == AcaoDoNpc::Trabalhar && !decisao.estaACaminho) {
            if (tilesDoCampo != nullptr && !estaNoTileDoCampo(contexto, npc, decisao, *tilesDoCampo)) {
                irParaOCampo(contexto, relogio, npc, decisao, *tilesDoCampo);
            } else if (tilesDoCampo == nullptr && contexto.producaoDasConstrucoes != nullptr
                       && decisao.indiceDaConstrucaoOndeEsta != casaETrabalho.indiceDoTrabalho) {
                irParaODestino(contexto, relogio, npc, decisao, casaETrabalho.indiceDoTrabalho, false);
            }
        }
        return;
    }
    decisao.acaoAtual = acaoNova;
    decisao.tickEmQueAEsperaAcaba = -1;

    switch (acaoNova) {
    case AcaoDoNpc::Dormir:
        irParaODestino(contexto, relogio, npc, decisao, casaETrabalho.indiceDaCasa, false);
        break;
    case AcaoDoNpc::Trabalhar:
        if (tilesDoCampo != nullptr) {
            irParaOCampo(contexto, relogio, npc, decisao, *tilesDoCampo);  // o Braçal trabalha no campo (marco 6)
        } else {
            irParaODestino(contexto, relogio, npc, decisao, casaETrabalho.indiceDoTrabalho, false);
        }
        break;
    case AcaoDoNpc::Comprar:
        irParaODestino(contexto, relogio, npc, decisao, contexto.vila.indiceDoMercado(), false);
        break;
    case AcaoDoNpc::Comer: {
        // FL6-7: o Braçal parado no tile dele almoça no campo.
        if (tilesDoCampo != nullptr && estaNoTileDoCampo(contexto, npc, decisao, *tilesDoCampo)) {
            decisao.indiceDaConstrucaoDoDestino = -1;
            break;
        }
        // FL5-4: dentro da casa ou do trabalho, come lá; na rua, vai para casa.
        const bool estaEmCasaOuNoTrabalho = decisao.estaDentroDeUmaConstrucao()
            && (decisao.indiceDaConstrucaoOndeEsta == casaETrabalho.indiceDaCasa
                || decisao.indiceDaConstrucaoOndeEsta == casaETrabalho.indiceDoTrabalho);
        const int ondeVaiComer = estaEmCasaOuNoTrabalho ? decisao.indiceDaConstrucaoOndeEsta : casaETrabalho.indiceDaCasa;
        irParaODestino(contexto, relogio, npc, decisao, ondeVaiComer, false);
        break;
    }
    case AcaoDoNpc::Passear:
    case AcaoDoNpc::Nenhuma:
        irParaODestino(contexto, relogio, npc, decisao, -1, true);
        break;
    }
}

// Os NPCs que cumprem a condição, em ordem pelo número (para os sorteios
// acontecerem sempre na mesma ordem).
template <typename Condicao>
std::vector<entt::entity> juntarNpcsEmOrdem(const entt::registry& registro, Condicao condicao)
{
    std::vector<std::pair<int, entt::entity>> npcsComNumero;
    for (const auto [npc, identidade, decisao] : registro.view<IdentidadeDoNpc, DecisaoDoNpc>().each()) {
        if (condicao(npc, decisao)) {
            npcsComNumero.emplace_back(identidade.numeroDoNpc, npc);
        }
    }
    std::sort(npcsComNumero.begin(), npcsComNumero.end(),
              [](const auto& primeiro, const auto& segundo) { return primeiro.first < segundo.first; });
    std::vector<entt::entity> npcsEmOrdem;
    for (const auto& [numero, npc] : npcsComNumero) {
        npcsEmOrdem.push_back(npc);
    }
    return npcsEmOrdem;
}

}  // namespace

bool horaEstaNoPeriodo(int horaDoDia, int horaDeInicio, int horaDeFim)
{
    if (horaDeInicio <= horaDeFim) {
        return horaDoDia >= horaDeInicio && horaDoDia < horaDeFim;
    }
    // Passa da meia-noite: das 22h às 6h são as 22h, 23h, 0h... 5h.
    return horaDoDia >= horaDeInicio || horaDoDia < horaDeFim;
}

PeriodoDaRotina descobrirPeriodoDaRotina(int horaDoDia, const RegrasDaRotina& rotina)
{
    if (horaEstaNoPeriodo(horaDoDia, rotina.horaQueComecaADormir, rotina.horaQueAcabaDeDormir)) {
        return PeriodoDaRotina::Dormir;
    }
    if (horaEstaNoPeriodo(horaDoDia, rotina.horaQueComecaATrabalhar, rotina.horaQueAcabaDeTrabalhar)) {
        return PeriodoDaRotina::Trabalhar;
    }
    return PeriodoDaRotina::Livre;
}

bool estaNaHoraDeComer(int horaDoDia, int minutoDaHora, const RegrasDaUtilidade& utilidade)
{
    const int minutoDoDia = horaDoDia * 60 + minutoDaHora;
    for (const int horaDaRefeicao : utilidade.horasDasRefeicoes) {
        const int inicioDaRefeicao = horaDaRefeicao * 60;
        if (minutoDoDia >= inicioDaRefeicao && minutoDoDia < inicioDaRefeicao + utilidade.minutosDaRefeicao) {
            return true;
        }
    }
    return false;
}

bool ehDiaDeCompraDoNpc(int64_t diaDaPartida, int numeroDoNpc, int diasEntreCompras)
{
    return diasEntreCompras > 0 && (diaDaPartida + numeroDoNpc) % diasEntreCompras == 0;
}

NotasDasAcoes calcularNotasDasAcoes(const SituacaoDoNpc& situacao, const RegrasDaUtilidade& utilidade)
{
    const int notaDoPeriodo = utilidade.notaDaAcaoDoPeriodo;
    NotasDasAcoes notas;
    if (situacao.periodo == PeriodoDaRotina::Dormir) {
        notas.dormir = notaDoPeriodo;
    }
    if (situacao.periodo == PeriodoDaRotina::Trabalhar && situacao.temEmprego) {
        notas.trabalhar = notaDoPeriodo;
    }
    // FL5-3: o Passear é a ação do Período Livre, e também a de quem não tem emprego no período de trabalho.
    if (situacao.periodo == PeriodoDaRotina::Livre || (situacao.periodo == PeriodoDaRotina::Trabalhar && !situacao.temEmprego)) {
        notas.passear = notaDoPeriodo;
    }
    if (situacao.estaNaHoraDeComer) {
        notas.comer += utilidade.bonusDeComer;
    }
    if (situacao.periodo == PeriodoDaRotina::Livre && situacao.ehDiaDeCompra && !situacao.jaComprouHoje && situacao.vilaTemMercado) {
        notas.comprar += utilidade.bonusDeComprar;
    }
    return notas;
}

AcaoDoNpc escolherAcaoPelaUtilidade(const NotasDasAcoes& notas)
{
    // A ordem da lista é a do desempate: a primeira com a maior nota ganha.
    const std::pair<AcaoDoNpc, int> acoesNaOrdemDoDesempate[] = {
        {AcaoDoNpc::Comer, notas.comer},         {AcaoDoNpc::Comprar, notas.comprar}, {AcaoDoNpc::Trabalhar, notas.trabalhar},
        {AcaoDoNpc::Dormir, notas.dormir},       {AcaoDoNpc::Passear, notas.passear},
    };
    AcaoDoNpc acaoEscolhida = AcaoDoNpc::Passear;
    int maiorNota = -1;
    for (const auto& [acao, nota] : acoesNaOrdemDoDesempate) {
        if (nota > maiorNota) {
            maiorNota = nota;
            acaoEscolhida = acao;
        }
    }
    return acaoEscolhida;
}

int converterMinutosEmTicks(int minutos, int ticksPorHoraDeJogo)
{
    return minutos * ticksPorHoraDeJogo / 60;
}

int calcularTicksEntreReavaliacoes(const RegrasDaUtilidade& utilidade, int ticksPorHoraDeJogo)
{
    return (utilidade.minutosEntreReavaliacoes * ticksPorHoraDeJogo + 59) / 60;
}

const char* nomeDaAcaoParaMostrar(AcaoDoNpc acao)
{
    switch (acao) {
    case AcaoDoNpc::Nenhuma: return "Nenhuma";
    case AcaoDoNpc::Dormir: return "Dormir";
    case AcaoDoNpc::Trabalhar: return "Trabalhar";
    case AcaoDoNpc::Comer: return "Comer";
    case AcaoDoNpc::Comprar: return "Comprar";
    case AcaoDoNpc::Passear: return "Passear";
    }
    return "?";
}

std::vector<entt::entity> criarOsNpcsDaVila(ContextoDaRotina& contexto, const RelogioDoMundo& relogio)
{
    entt::registry& registro = contexto.registroDasEntidades;
    std::vector<entt::entity> npcsCriados;
    for (std::size_t numero = 0; numero < contexto.vila.npcsIniciais.size(); ++numero) {
        const NpcDaVilaInicial& npcInicial = contexto.vila.npcsIniciais[numero];
        // Nasce dentro da casa: a posição é a porta dela.
        const bool temCasa = npcInicial.indiceDaCasa >= 0;
        const PosicaoDeTile tileOndeNasce = temCasa ? contexto.vila.construcoes[npcInicial.indiceDaCasa].tileDaPorta
                                                    : PosicaoDeTile{contexto.mapa.larguraEmTiles / 2, contexto.mapa.alturaEmTiles / 2};
        const entt::entity npc = criarNpcNoTile(registro, tileOndeNasce);
        registro.emplace<IdentidadeDoNpc>(npc, IdentidadeDoNpc{npcInicial.nome, npcInicial.classe, static_cast<int>(numero)});
        registro.emplace<CasaETrabalhoDoNpc>(npc, CasaETrabalhoDoNpc{npcInicial.indiceDaCasa, npcInicial.indiceDoTrabalho});
        DecisaoDoNpc decisao;
        decisao.indiceDaConstrucaoOndeEsta = npcInicial.indiceDaCasa;
        registro.emplace<DecisaoDoNpc>(npc, decisao);
        npcsCriados.push_back(npc);
    }
    // Cada um já escolhe a ação, na ordem da vila.
    for (const entt::entity npc : npcsCriados) {
        reavaliarUmNpc(contexto, relogio, npc);
    }
    return npcsCriados;
}

void rodarARotinaAntesDeAndar(ContextoDaRotina& contexto, const RelogioDoMundo& relogio)
{
    const int64_t tickAtual = relogio.tickAtual();
    const int ticksEntreReavaliacoes = calcularTicksEntreReavaliacoes(contexto.regras.utilidade, contexto.ticksPorHoraDeJogo);
    entt::registry& registro = contexto.registroDasEntidades;

    // Passo 1: os NPCs da vez, um a cada 67 ticks: (tick + número) resto 67 = 0.
    const std::vector<entt::entity> npcsDaVez = juntarNpcsEmOrdem(registro, [&](entt::entity npc, const DecisaoDoNpc&) {
        return (tickAtual + registro.get<IdentidadeDoNpc>(npc).numeroDoNpc) % ticksEntreReavaliacoes == 0;
    });
    for (const entt::entity npc : npcsDaVez) {
        reavaliarUmNpc(contexto, relogio, npc);
    }

    // Passo 2 (regra 4.5): as esperas que acabaram.
    const std::vector<entt::entity> npcsComEsperaNoFim = juntarNpcsEmOrdem(registro, [&](entt::entity, const DecisaoDoNpc& decisao) {
        return decisao.tickEmQueAEsperaAcaba >= 0 && tickAtual >= decisao.tickEmQueAEsperaAcaba;
    });
    for (const entt::entity npc : npcsComEsperaNoFim) {
        DecisaoDoNpc& decisao = registro.get<DecisaoDoNpc>(npc);
        decisao.tickEmQueAEsperaAcaba = -1;
        if (decisao.acaoAtual == AcaoDoNpc::Comprar) {
            decisao.diaDaUltimaCompra = relogio.diaDaPartida();
            reavaliarUmNpc(contexto, relogio, npc);
        } else if (decisao.acaoAtual == AcaoDoNpc::Passear) {
            comecarUmPasseio(contexto, relogio, npc, decisao);
        }
    }
}

void cuidarDasChegadas(ContextoDaRotina& contexto, const RelogioDoMundo& relogio)
{
    entt::registry& registro = contexto.registroDasEntidades;
    const std::vector<entt::entity> npcsQueChegaram = juntarNpcsEmOrdem(registro, [&](entt::entity npc, const DecisaoDoNpc& decisao) {
        return decisao.estaACaminho && !registro.all_of<CaminhoDoNpc>(npc) && !registro.all_of<PedidoDeCaminho>(npc);
    });
    for (const entt::entity npc : npcsQueChegaram) {
        DecisaoDoNpc& decisao = registro.get<DecisaoDoNpc>(npc);
        const LugarDoNpc& lugar = registro.get<PosicaoDoNpc>(npc).lugarAtual;
        decisao.estaACaminho = false;
        const int indiceDoDestino = decisao.indiceDaConstrucaoDoDestino;
        if (indiceDoDestino >= 0) {
            const bool estaNaPorta = lugar.estaParado() && lugar.tileAtual == contexto.vila.construcoes[indiceDoDestino].tileDaPorta;
            if (!estaNaPorta) {
                // Não chegou (sem caminho): decide de novo no próximo turno dele.
                decisao.acaoAtual = AcaoDoNpc::Nenhuma;
                continue;
            }
            entrarNaConstrucao(contexto, relogio, decisao, indiceDoDestino);
        } else if (decisao.acaoAtual == AcaoDoNpc::Passear) {
            const RegrasDoPasseio& passeio = contexto.regras.passeio;
            const int minutosDeEspera = contexto.sorteioDosNpcs.sortearNumeroInteiroEntre(passeio.menorEsperaEmMinutos, passeio.maiorEsperaEmMinutos);
            decisao.tickEmQueAEsperaAcaba = relogio.tickAtual() + converterMinutosEmTicks(minutosDeEspera, contexto.ticksPorHoraDeJogo);
        }
    }
}

std::optional<PosicaoDeTile> sortearTileDoPasseio(const MapaLocal& mapa, const VilaDoJogo& vila, PosicaoDeTile tileAtual,
                                                  const RegrasDoPasseio& passeio, SorteioComSemente& sorteio)
{
    const int distancia = passeio.distanciaMaximaEmTiles;
    for (int tentativa = 0; tentativa < tentativasDeSortearOPasseio; ++tentativa) {
        const int tileX = sorteio.sortearNumeroInteiroEntre(tileAtual.tileX - distancia, tileAtual.tileX + distancia);
        const int tileY = sorteio.sortearNumeroInteiroEntre(tileAtual.tileY - distancia, tileAtual.tileY + distancia);
        if (!mapa.tileDaParaAndar(tileX, tileY)) {
            continue;
        }
        const bool ehPortaDeConstrucao = std::any_of(vila.construcoes.begin(), vila.construcoes.end(), [&](const ConstrucaoDaVila& construcao) {
            return construcao.tileDaPorta == PosicaoDeTile{tileX, tileY};
        });
        if (!ehPortaDeConstrucao) {
            return PosicaoDeTile{tileX, tileY};
        }
    }
    return std::nullopt;
}

std::string montarTabelaDasAcoesDosNpcs(const ContextoDaRotina& contexto, const RelogioDoMundo& relogio)
{
    const entt::registry& registro = contexto.registroDasEntidades;
    std::vector<std::pair<int, std::string>> linhasComNumero;
    for (const auto [npc, identidade, casaETrabalho, decisao] : registro.view<IdentidadeDoNpc, CasaETrabalhoDoNpc, DecisaoDoNpc>().each()) {
        const NotasDasAcoes notas = calcularNotasDasAcoes(montarSituacaoDoNpc(contexto, relogio, identidade, casaETrabalho, decisao),
                                                          contexto.regras.utilidade);
        std::string ondeEsta = "na rua";
        const std::vector<PosicaoDeTile>* tilesDoCampo = tilesDoCampoDoTrabalho(contexto, casaETrabalho);
        if (tilesDoCampo != nullptr && estaNoTileDoCampo(contexto, npc, decisao, *tilesDoCampo)) {
            ondeEsta = "no campo de " + contexto.vila.construcoes[casaETrabalho.indiceDoTrabalho].identificador;
        } else if (decisao.estaDentroDeUmaConstrucao()) {
            ondeEsta = "dentro de " + contexto.vila.construcoes[decisao.indiceDaConstrucaoOndeEsta].identificador;
        } else if (decisao.estaACaminho) {
            ondeEsta = decisao.indiceDaConstrucaoDoDestino >= 0
                ? "indo para " + contexto.vila.construcoes[decisao.indiceDaConstrucaoDoDestino].identificador
                : "indo passear";
        }
        const std::string linha = "  " + identidade.nome + ": " + nomeDaAcaoParaMostrar(decisao.acaoAtual) + ", " + ondeEsta
            + " (notas: dormir " + std::to_string(notas.dormir) + ", trabalhar " + std::to_string(notas.trabalhar)
            + ", comer " + std::to_string(notas.comer) + ", comprar " + std::to_string(notas.comprar)
            + ", passear " + std::to_string(notas.passear) + ")\n";
        linhasComNumero.emplace_back(identidade.numeroDoNpc, linha);
    }
    std::sort(linhasComNumero.begin(), linhasComNumero.end());
    std::string tabela;
    for (const auto& [numero, linha] : linhasComNumero) {
        tabela += linha;
    }
    return tabela;
}
