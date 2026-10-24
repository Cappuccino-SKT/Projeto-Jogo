#include "Viagem/ViagemDaVila.h"

#include <algorithm>

#include "Clima/ClimaDaVila.h"

namespace {

const char* const identificadorDoPaoDaViagem = "item.pao";

// Regra 4.1, passo 3: o grupo entra no primeiro trecho da rota.
void entrarNoTrecho(ContextoDaProducao& contexto, EntidadeEmTransito& grupo, int proximoMapa, Estacao estacao)
{
    entrarNoTrecho(grupo, proximoMapa, contexto.dados.mundo, contexto.dados.regrasDaViagem, contexto.viagem->climaDeHojePorRegiao, estacao,
                   *contexto.sorteioDaViagem);
}

// O grupo volta à vila: o que ele leva volta para o jogador.
std::vector<std::string> devolverOQueOGrupoLeva(ContextoDaProducao& contexto, EntidadeEmTransito& grupo)
{
    std::vector<std::string> linhas;
    contexto.dinheiro.dinheiroDoJogador += grupo.dinheiroEmDecimos;
    grupo.dinheiroEmDecimos = 0;
    for (int numeroDoItem = 0; numeroDoItem < static_cast<int>(grupo.quantidadePorItem.size()); ++numeroDoItem) {
        int& quantidade = grupo.quantidadePorItem[numeroDoItem];
        if (quantidade > 0 && !contexto.inventario.tentarPor(numeroDoItem, DonoDoItem::Jogador, quantidade)) {
            linhas.push_back("Aviso: o armazém do jogador não tem espaço para " + std::to_string(quantidade) + " "
                             + contexto.dados.catalogoDeItens.itemPeloNumero(numeroDoItem).identificadorDeTexto + " da viagem; ficaram perdidos.");
        }
        quantidade = 0;
    }
    return linhas;
}

}  // namespace

void montarAViagemDaPartidaNova(EstadoDaViagem& viagem, const DadosDoJogo& dados, const EstadoDoClima& climaDaVila)
{
    const MundoDoJogo& mundo = dados.mundo;
    viagem = EstadoDaViagem{};
    viagem.grupoDoJogador.mapaAtual = mundo.mapaDaVila;
    viagem.grupoDoJogador.quantidadePorItem.assign(static_cast<std::size_t>(dados.catalogoDeItens.quantidadeDeItens()), 0);
    viagem.nevoa.assign(mundo.mapas.size(), RegistroDaNevoa{});
    viagem.precosConhecidos.assign(mundo.mapas.size(), PrecosConhecidos{});
    std::fill(viagem.climaDeHojePorRegiao.begin(), viagem.climaDeHojePorRegiao.end(), climaDaVila.climaDeHoje);
    if (!mundo.mapaExiste(mundo.mapaDaVila)) {
        return;
    }
    for (const CidadeDoMundo& cidade : mundo.cidades) {
        if (cidade.nivel == 4) {
            marcarMapaComoVisto(viagem, mundo, cidade.numeroDoMapa, 0);  // as capitais, conhecidas desde o início
        }
    }
    marcarMapaComoVisto(viagem, mundo, mundo.mapaDaVila, 0);
}

bool grupoEstaNaVila(const EstadoDaViagem& viagem, const DadosDoJogo& dados)
{
    return !viagem.grupoDoJogador.emTransito && viagem.grupoDoJogador.mapaAtual == dados.mundo.mapaDaVila;
}

void marcarMapaComoVisto(EstadoDaViagem& viagem, const MundoDoJogo& mundo, int numeroDoMapa, int64_t diaDaPartida)
{
    std::vector<int> mapasVistos = mundo.vizinhos(numeroDoMapa);
    mapasVistos.insert(mapasVistos.begin(), numeroDoMapa);
    for (const int mapa : mapasVistos) {
        if (mapa < 1 || mapa > static_cast<int>(viagem.nevoa.size())) {
            continue;
        }
        RegistroDaNevoa& registro = viagem.nevoa[mapa - 1];
        registro.diaEmQueFoiVisto = diaDaPartida;
        const CidadeDoMundo* cidade = mundo.cidadeNoMapa(mapa);
        registro.dono = cidade != nullptr ? cidade->faccao : "";
        registro.nivelDaCidade = cidade != nullptr ? cidade->nivel : 0;
        registro.estado = mundo.estadoDoTerritorio(mapa);
    }
}

std::string partirEmViagem(ContextoDaProducao& contexto, const RelogioDoMundo& relogio, int destino, int paesLevados, int64_t dinheiroLevadoEmDecimos,
                           bool marchaForcada)
{
    EstadoDaViagem& viagem = *contexto.viagem;
    EntidadeEmTransito& grupo = viagem.grupoDoJogador;
    const MundoDoJogo& mundo = contexto.dados.mundo;
    if (grupo.emTransito) {
        return "o grupo já está viajando";
    }
    if (!mundo.mapaExiste(destino) || destino == grupo.mapaAtual) {
        return "o destino precisa ser outro mapa";
    }
    const bool saiDaVila = grupoEstaNaVila(viagem, contexto.dados);
    const std::optional<int> pao = contexto.dados.catalogoDeItens.procurarNumeroDoItem(identificadorDoPaoDaViagem);
    if (saiDaVila) {
        if (paesLevados < 0 || dinheiroLevadoEmDecimos < 0) {
            return "a quantidade não pode ser negativa";
        }
        if (paesLevados > 0 && (!pao || contexto.inventario.quantidadeDe(*pao, DonoDoItem::Jogador) < paesLevados)) {
            return "o jogador não tem " + std::to_string(paesLevados) + " Pães no armazém";
        }
        if (dinheiroLevadoEmDecimos > contexto.dinheiro.dinheiroDoJogador) {
            return "o jogador não tem esse dinheiro";
        }
    }
    grupo.marchaForcada = marchaForcada;
    std::vector<int> rota = buscarRotaMaisRapida(grupo.mapaAtual, destino, grupo, mundo, contexto.dados.regrasDaViagem);
    if (rota.empty()) {
        return "não há caminho até o destino";
    }
    if (saiDaVila) {
        if (paesLevados > 0) {
            contexto.inventario.tentarTirar(*pao, DonoDoItem::Jogador, paesLevados);
            grupo.quantidadePorItem[*pao] += paesLevados;
        }
        contexto.dinheiro.dinheiroDoJogador -= dinheiroLevadoEmDecimos;
        grupo.dinheiroEmDecimos += dinheiroLevadoEmDecimos;
    }
    grupo.emTransito = true;
    const int primeiroMapa = rota.front();
    grupo.rota.assign(rota.begin() + 1, rota.end());
    entrarNoTrecho(contexto, grupo, primeiroMapa, relogio.estacaoAtual());
    return "";
}

ResultadoDoTickDaViagem avancarAViagemUmTick(ContextoDaProducao& contexto, const RelogioDoMundo& relogio)
{
    ResultadoDoTickDaViagem resultado;
    EstadoDaViagem& viagem = *contexto.viagem;
    EntidadeEmTransito& grupo = viagem.grupoDoJogador;
    const MundoDoJogo& mundo = contexto.dados.mundo;
    const RegrasDaViagem& regras = contexto.dados.regrasDaViagem;
    const PassoDaEntidade passo = andarUmTick(grupo, mundo, regras, viagem.climaDeHojePorRegiao, relogio.horaDoDiaAtual(), relogio.estacaoAtual(),
                                              contexto.ticksPorHoraDeJogo, *contexto.sorteioDaViagem);
    if (passo == PassoDaEntidade::EncontroNaMetade) {
        const int bandidos = grupo.encontroMarcado.bandidos;
        if (grupo.encontroMarcado.tipo == TipoDeEncontro::MercadorViajante) {
            // A troca com os preços de outra região: o grupo pode comerciar nas cidades (B2); o mercador fica para depois.
            resultado.linhasParaOConsole.push_back("Viagem: um mercador viajante cruza o caminho do grupo.");
            grupo.encontroMarcado = {};
        } else if (!bandidosAtacam(forcaDoGrupo(grupo, regras), bandidos, regras)) {
            resultado.linhasParaOConsole.push_back("Viagem: um bando de " + std::to_string(bandidos) + " bandidos observa vocês de longe e vai embora.");
            grupo.encontroMarcado = {};
        } else {
            grupo.encontroAcontecendo = true;
            resultado.encontroComecou = true;
            resultado.linhasParaOConsole.push_back("Encontro: um bando de " + std::to_string(bandidos) + " bandidos cerca o grupo em "
                                                   + mundo.nomeDoMapa(grupo.proximoMapa) + ".");
        }
        return resultado;
    }
    if (passo != PassoDaEntidade::ChegouAoProximoMapa && passo != PassoDaEntidade::ChegouAoDestino) {
        return resultado;
    }
    marcarMapaComoVisto(viagem, mundo, grupo.mapaAtual, relogio.diaDaPartida());
    resultado.mapaOndeChegou = grupo.mapaAtual;
    if (passo == PassoDaEntidade::ChegouAoProximoMapa) {
        return resultado;
    }
    resultado.chegouAoDestino = true;
    resultado.linhasParaOConsole.push_back("Viagem: o grupo chegou a " + mundo.nomeDoMapa(grupo.mapaAtual) + ".");
    if (grupo.mapaAtual == mundo.mapaDaVila) {
        const std::vector<std::string> avisos = devolverOQueOGrupoLeva(contexto, grupo);
        resultado.linhasParaOConsole.insert(resultado.linhasParaOConsole.end(), avisos.begin(), avisos.end());
    } else if (mundo.mapa(grupo.mapaAtual).arquivoTiled.empty()) {
        resultado.linhasParaOConsole.push_back("Viagem: este mapa ainda não tem desenho; o grupo fica no Mapa Mundial.");  // VI1-8
    }
    return resultado;
}

std::vector<std::string> rodarAViagemDoDia(ContextoDaProducao& contexto)
{
    std::vector<std::string> linhas;
    EstadoDaViagem& viagem = *contexto.viagem;
    const MundoDoJogo& mundo = contexto.dados.mundo;
    viagem.grupoSemComida = false;

    // Regra 4.8: a região da vila usa o clima que o passo "clima" já sorteou; as outras
    // sorteiam com a sequência 9, da região 1 à 6, pulando a da vila.
    const int regiaoDaVila = mundo.mapaExiste(mundo.mapaDaVila) ? mundo.mapa(mundo.mapaDaVila).regiao : 0;
    for (int regiao = 1; regiao <= static_cast<int>(viagem.climaDeHojePorRegiao.size()); ++regiao) {
        TipoDeClima& climaDaRegiao = viagem.climaDeHojePorRegiao[regiao - 1];
        if (regiao == regiaoDaVila) {
            climaDaRegiao = contexto.clima.climaDeHoje;
        } else {
            climaDaRegiao = sortearClimaDoDia(climaDaRegiao, contexto.clima.estacaoDeHoje, contexto.dados.regrasDoClima, *contexto.sorteioDoClimaDasOutrasRegioes);
        }
    }

    // Regra 4.4, passo 2: fora da vila, cada pessoa come 1 refeição do que o grupo leva,
    // a comida de maior Tier primeiro.
    EntidadeEmTransito& grupo = viagem.grupoDoJogador;
    if (grupoEstaNaVila(viagem, contexto.dados)) {
        return linhas;
    }
    int refeicoesQueFaltam = grupo.pessoas * contexto.dados.regrasDaViagem.refeicoesPorPessoaPorDia;
    while (refeicoesQueFaltam > 0) {
        int melhorComida = -1;
        for (int numeroDoItem = 0; numeroDoItem < static_cast<int>(grupo.quantidadePorItem.size()); ++numeroDoItem) {
            const DefinicaoDeItem& item = contexto.dados.catalogoDeItens.itemPeloNumero(numeroDoItem);
            if (grupo.quantidadePorItem[numeroDoItem] > 0 && item.ehComida()
                && (melhorComida < 0 || item.tier.value_or(0) > contexto.dados.catalogoDeItens.itemPeloNumero(melhorComida).tier.value_or(0))) {
                melhorComida = numeroDoItem;
            }
        }
        if (melhorComida < 0) {
            break;
        }
        const int comidas = std::min(refeicoesQueFaltam, grupo.quantidadePorItem[melhorComida]);
        grupo.quantidadePorItem[melhorComida] -= comidas;
        refeicoesQueFaltam -= comidas;
    }
    if (refeicoesQueFaltam > 0) {
        // A Felicidade e a Vida do grupo entram com os companheiros e o Combate (fase D).
        viagem.grupoSemComida = true;
        linhas.push_back("Aviso: o grupo está sem comida.");
    }
    return linhas;
}

std::string resolverOEncontroDoGrupo(ContextoDaProducao& contexto, EscolhaNoEncontro escolha, std::vector<std::string>& linhas, bool* venceuALuta)
{
    EntidadeEmTransito& grupo = contexto.viagem->grupoDoJogador;
    if (!grupo.encontroAcontecendo) {
        return "não há encontro agora";
    }
    const std::string linha = resolverEncontroComBandidos(grupo, escolha, contexto.dados.regrasDaViagem, *contexto.sorteioDaViagem, venceuALuta);
    if (linha.empty()) {
        return "o grupo não tem o dinheiro do suborno";
    }
    linhas.push_back(linha);
    return "";
}
