// Os testes da viagem: a tabela da parte 8 da Ficha Lógica: Viagem (fase B1).

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Jogador/PedidosDoJogador.h"
#include "Save/ArquivoDeSave.h"
#include "Viagem/ViagemDaVila.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

// Um mundo de 8 × 6 só de planície selvagem, sem cidades: cada teste muda o que precisa.
MundoDoJogo mundoDePlanicie()
{
    MundoDoJogo mundo;
    for (int linha = 1; linha <= mundo.linhas; ++linha) {
        for (int coluna = 1; coluna <= mundo.colunas; ++coluna) {
            MapaDoMundo mapa;
            mapa.numeroDoMapa = mundo.numeroDoMapaNa(coluna, linha);
            mapa.coluna = coluna;
            mapa.linha = linha;
            mundo.mapas.push_back(mapa);
        }
    }
    mundo.mapaDaVila = 1;
    return mundo;
}

void testarOsTicksDoTrecho(const RegrasDaViagem& regras)
{
    MundoDoJogo mundo = mundoDePlanicie();
    mundo.cidades.push_back(CidadeDoMundo{"Capital", 2, 4, "guerreira", false});
    mundo.mapas[2].terreno = "floresta";
    mundo.mapas[3].infraestrutura = "estrada";
    mundo.mapas[4].terreno = "montanha";

    EntidadeEmTransito mensageiro;
    mensageiro.montados = true;
    conferir(calcularTicksDoTrecho(mensageiro, mundo, 2, 100, regras) == 1393, "mensageiro a cavalo, cidade nível 4, planície: 1.393 ticks");
    EntidadeEmTransito aPe;
    conferir(calcularTicksDoTrecho(aPe, mundo, 3, 100, regras) == 4000, "jogador a pé, floresta sem estrada: 4.000 ticks");
    EntidadeEmTransito caravana;
    caravana.tipoDeEntidade = "caravana";
    caravana.pessoas = 15;
    conferir(calcularTicksDoTrecho(caravana, mundo, 4, 100, regras) == 3511, "caravana de 15, estrada na planície: 3.511 ticks");
    EntidadeEmTransito exercito;
    exercito.pessoas = 2000;
    conferir(calcularTicksDoTrecho(exercito, mundo, 5, 100, regras) == 8400, "exército de 20 Esquadrões, montanha: 8.400 ticks");

    // Parte 10.3: a mesma caravana no inverno, com Neve: 150 × 110 = 165.
    const int climaEEstacao = multiplicadorDeClimaEEstacao(TipoDeClima::Neve, Estacao::Inverno, mundo, 4, regras);
    conferir(climaEEstacao == 165 && calcularTicksDoTrecho(caravana, mundo, 4, climaEEstacao, regras) == 5793,
             "caravana no inverno com Neve: clima e estação 165, 5.793 ticks");
    conferir(multiplicadorDeClimaEEstacao(TipoDeClima::Tempestade, Estacao::Inverno, mundo, 4, regras) == 165
                 && multiplicadorDeClimaEEstacao(TipoDeClima::Neve, Estacao::Primavera, mundo, 3, regras) == 165,
             "a lama da Primavera só vale sem estrada");
}

void testarARota(const RegrasDaViagem& regras)
{
    MundoDoJogo mundo = mundoDePlanicie();
    const EntidadeEmTransito aPe;
    const int de11 = mundo.numeroDoMapaNa(1, 1);
    const int de12 = mundo.numeroDoMapaNa(1, 2);
    const int de31 = mundo.numeroDoMapaNa(3, 1);
    conferir(buscarRotaMaisRapida(de11, de31, aPe, mundo, regras) == std::vector<int>{2, 3}, "de (1,1) a (3,1) em planície: passa por (2,1)");
    conferir(buscarRotaMaisRapida(de12, de31, aPe, mundo, regras) == std::vector<int>{1, 2, 3},
             "empate de (1,2) a (3,1): o caminho que passa primeiro pelo mapa de menor número");
    mundo.mapas[1].terreno = "floresta_densa";
    mundo.mapas[mundo.numeroDoMapaNa(3, 2) - 1].infraestrutura = "estrada";
    conferir(buscarRotaMaisRapida(de12, de31, aPe, mundo, regras) == std::vector<int>{10, 11, 3},
             "com floresta densa em (2,1) e estrada em (3,2): a rota desvia");
    conferir(buscarRotaMaisRapida(de11, de11, aPe, mundo, regras).empty(), "destino = o mapa atual: sem rota");
}

void testarOEncontro(const RegrasDaViagem& regras)
{
    // Parte 10.2: 87 < 150 → encontro; 412 < 750 → bandidos; 3 + 3 = 6.
    const EncontroMarcado encontro = escolherOEncontro(150, 87, 412, 3, regras);
    conferir(encontro == EncontroMarcado{TipoDeEncontro::Bandidos, 6}, "sorteios 87, 412 e 3 num mapa em disputa: 6 bandidos");
    conferir(escolherOEncontro(50, 503, 0, 0, regras).tipo == TipoDeEncontro::Nenhum, "503 contra 50 por mil: nada");
    conferir(escolherOEncontro(200, 10, 900, 0, regras).tipo == TipoDeEncontro::MercadorViajante, "tipo 900: o mercador viajante");

    conferir(!bandidosAtacam(25, 6, regras) && bandidosAtacam(24, 6, regras), "força 25 contra 6 bandidos (12): não atacam; força 24: atacam");
    conferir(precoDoSubornoEmDecimos(8, 50, regras) == 2000 && precoDoSubornoEmDecimos(6, 40, regras) == 1650,
             "suborno: 8 bandidos e Carisma 50, 200 moedas; 6 e Carisma 40, 165");
    EntidadeEmTransito sozinho;
    conferir(chanceDeFugirEmPorcento(sozinho, 6, regras) == 54, "fugir sozinho e a pé de 6 bandidos: 54%");
    conferir(resolverPorCalculoRapidoDeForcas(3, 12, 199) && !resolverPorCalculoRapidoDeForcas(3, 12, 200), "força 3 contra 12: vence com 20% (de 0 a 199)");

    // A perda arredondada para baixo: 3 Pães e 75% → fica 1; 150 moedas → 37,5.
    sozinho.quantidadePorItem = {3};
    sozinho.dinheiroEmDecimos = 1500;
    aplicarAPerdaDoGrupo(sozinho, 75);
    conferir(sozinho.quantidadePorItem[0] == 1 && sozinho.dinheiroEmDecimos == 375, "perder a luta: 3 Pães viram 1; 150 moedas viram 37,5");

    // O suborno some quando falta o dinheiro (a escolha é recusada).
    SorteioComSemente sorteio(1, sequenciaDoSorteioDaViagem);
    sozinho.encontroMarcado = {TipoDeEncontro::Bandidos, 6};
    sozinho.encontroAcontecendo = true;
    conferir(resolverEncontroComBandidos(sozinho, EscolhaNoEncontro::Subornar, regras, RegrasDoCombate{}, sorteio).empty() && sozinho.encontroAcontecendo,
             "suborno de 165 moedas com 37,5: recusado, o encontro continua");
}

int64_t ticksAteAHora(const SimulacaoDaVila& simulacao, int hora, int minuto)
{
    return static_cast<int64_t>(hora * 60 + minuto) * 800 / 60 - (simulacao.relogio.horaDoDiaAtual() * 60 + simulacao.relogio.minutoDaHoraAtual()) * 800 / 60;
}

void testarAViagemNaPartida(const DadosDoJogo& dados)
{
    const MundoDoJogo& mundo = dados.mundo;
    conferir(mundo.mapas.size() == 48 && mundo.mapaDaVila == mundo.numeroDoMapaNa(4, 3), "o mundo tem 48 mapas, e a vila fica em (4,3)");
    int comCidade = 0, territorio = 0, emDisputa = 0, semDono = 0;
    for (const MapaDoMundo& mapa : mundo.mapas) {
        switch (mundo.estadoDoTerritorio(mapa.numeroDoMapa)) {
        case EstadoDoTerritorio::ComCidade: ++comCidade; break;
        case EstadoDoTerritorio::Territorio: ++territorio; break;
        case EstadoDoTerritorio::EmDisputa: ++emDisputa; break;
        case EstadoDoTerritorio::SemDono: ++semDono; break;
        }
    }
    conferir(comCidade == 22 && territorio == 16 && emDisputa == 6 && semDono == 4, "o primeiro dia: 22 com cidade, 16 de território, 6 em disputa, 4 sem dono");

    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    const int pao = *dados.catalogoDeItens.procurarNumeroDoItem("item.pao");
    const int destino = mundo.numeroDoMapaNa(5, 3);  // colinas sem dono, lama da Primavera: 4.400 ticks
    conferir(simulacao.viagem.nevoa[mundo.numeroDoMapaNa(8, 6) - 1].diaEmQueFoiVisto == 0 && simulacao.viagem.nevoa[destino - 1].diaEmQueFoiVisto == 0
                 && simulacao.viagem.nevoa[mundo.numeroDoMapaNa(6, 3) - 1].diaEmQueFoiVisto == -1,
             "a névoa do início: as capitais e os vizinhos da vila");

    // Partir: o grupo leva 3 Pães e 50 moedas do jogador.
    const bool colocouOsPaes = simulacao.inventario.tentarPor(pao, DonoDoItem::Jogador, 3);
    const int64_t dinheiroDoJogador = simulacao.dinheiro.dinheiroDoJogador;
    const int64_t dinheiroDaVilaAntes = simulacao.dinheiro.dinheiroTotal() - simulacao.dinheiro.dinheiroQueEntrouDeFora;
    simulacao.pedidos.acrescentar(PedidoPartirEmViagem{destino, 3, 500, false});
    simulacao.aplicarOsPedidosPendentes();
    EntidadeEmTransito& grupo = simulacao.viagem.grupoDoJogador;
    conferir(colocouOsPaes && grupo.emTransito && grupo.proximoMapa == destino && grupo.ticksDoTrecho == 4400 && grupo.quantidadePorItem[pao] == 3
                 && grupo.dinheiroEmDecimos == 500 && simulacao.dinheiro.dinheiroDoJogador == dinheiroDoJogador - 500,
             "partir para (5,3): 4.400 ticks (com a lama da Primavera), com 3 Pães e 50 moedas do jogador");
    simulacao.pedidos.acrescentar(PedidoPartirEmViagem{destino, 0, 0, false});
    const std::vector<std::string> recusa = simulacao.aplicarOsPedidosPendentes();
    conferir(recusa.size() == 1 && recusa[0] == "Aviso: pedido recusado (o grupo já está viajando).", "partir de novo no meio da viagem: recusado");

    // O tempo fica fixo (VI1-3) e o encontro acontece na metade do trecho.
    grupo.encontroMarcado = {TipoDeEncontro::Bandidos, 6};
    int ticksDaMetade = -1;
    for (int tick = 0; tick < 3000 && !grupo.encontroAcontecendo; ++tick) {
        simulacao.avancarUmTick();
        ticksDaMetade = grupo.ticksAndadosNoTrecho;
    }
    conferir(grupo.encontroAcontecendo && ticksDaMetade == 2200 && simulacao.relogio.jogoEstaPausado(), "o encontro acontece com 2.200 de 4.400 ticks, e o jogo pausa");

    // Save no meio do trecho, com o encontro esperando: carregar continua igual.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(simulacao, "teste", "2026-10-20");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->viagem.grupoDoJogador.encontroAcontecendo
                 && carregado.partida->viagem.grupoDoJogador.ticksAndadosNoTrecho == 2200 && carregado.partida->viagem.nevoa == simulacao.viagem.nevoa,
             "salvar e carregar no meio do trecho: o mesmo tick do trecho, o encontro e a névoa");
    if (carregado.partida == nullptr) {
        std::printf("    motivo: %s\n", carregado.motivoDaRecusa.c_str());
        return;
    }
    for (SimulacaoDaVila* partida : {&simulacao, carregado.partida.get()}) {
        partida->pedidos.acrescentar(PedidoResolverEncontro{EscolhaNoEncontro::Fugir});
        for (int tick = 0; tick < 2000; ++tick) {
            partida->avancarUmTick();
        }
    }
    const EntidadeEmTransito& grupoCarregado = carregado.partida->viagem.grupoDoJogador;
    conferir(grupoCarregado.mapaAtual == grupo.mapaAtual && grupoCarregado.dinheiroEmDecimos == grupo.dinheiroEmDecimos
                 && grupoCarregado.quantidadePorItem == grupo.quantidadePorItem && grupoCarregado.ticksParadoNaLuta == grupo.ticksParadoNaLuta,
             "as duas partidas resolvem a fuga com o mesmo sorteio");

    // Chegar a um mapa sem desenho: o grupo fica no Mapa Mundial (VI1-8). Volta para a vila.
    for (int tick = 0; tick < 3000 && grupo.emTransito; ++tick) {
        simulacao.avancarUmTick();
    }
    conferir(!grupo.emTransito && grupo.mapaAtual == destino && simulacao.viagem.nevoa[mundo.numeroDoMapaNa(6, 3) - 1].diaEmQueFoiVisto == 0,
             "chegou a (5,3); o vizinho (6,3) foi visto");
    const int paesNaViagem = grupo.quantidadePorItem[pao];
    const int64_t dinheiroNaViagem = grupo.dinheiroEmDecimos;
    simulacao.pedidos.acrescentar(PedidoPartirEmViagem{mundo.mapaDaVila, 0, 0, false});
    simulacao.aplicarOsPedidosPendentes();
    grupo.encontroMarcado = {};
    for (int tick = 0; tick < 6000 && grupo.emTransito; ++tick) {
        simulacao.avancarUmTick();
    }
    conferir(grupoEstaNaVila(simulacao.viagem, dados) && grupo.dinheiroEmDecimos == 0 && grupo.quantidadePorItem[pao] == 0
                 && simulacao.inventario.quantidadeDe(pao, DonoDoItem::Jogador) >= paesNaViagem
                 && simulacao.dinheiro.dinheiroTotal() - simulacao.dinheiro.dinheiroQueEntrouDeFora == dinheiroDaVilaAntes - (500 - dinheiroNaViagem),
             "de volta à vila: o que o grupo leva volta para o jogador");
}

void testarOAcampamento(const DadosDoJogo& dados)
{
    // Parte 10.2: um trecho de 2.080 ticks começado às 20 h 45 termina às 7 h 21 do dia seguinte.
    SimulacaoDaVila simulacao(dados, RegrasDoTempo{});
    for (int64_t tick = ticksAteAHora(simulacao, 20, 45); tick > 0; --tick) {
        simulacao.avancarUmTick();
    }
    EntidadeEmTransito& grupo = simulacao.viagem.grupoDoJogador;
    grupo.emTransito = true;
    grupo.proximoMapa = dados.mundo.numeroDoMapaNa(5, 3);
    grupo.ticksDoTrecho = 2080;
    const int64_t diaDaPartida = simulacao.relogio.diaDaPartida();
    int andadoAs22 = -1;
    for (int tick = 0; tick < 20000 && grupo.emTransito; ++tick) {
        simulacao.avancarUmTick();
        if (simulacao.relogio.horaDoDiaAtual() == 22 && andadoAs22 < 0) {
            andadoAs22 = grupo.ticksAndadosNoTrecho;
        }
    }
    conferir(andadoAs22 == 999 && !grupo.emTransito && simulacao.relogio.diaDaPartida() == diaDaPartida + 1 && simulacao.relogio.horaDoDiaAtual() == 7
                 && simulacao.relogio.minutoDaHoraAtual() == 21,
             "acampa às 22 h com 999 andados (o tick das 20 h 45 já passou) e chega às 7 h 21 do dia seguinte");
}

}  // namespace

void testarViagem()
{
    std::printf("Viagem: o Mapa Mundial e o grupo do jogador (fase B1)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com o mundo e o viagem.json, não têm erro");
    testarOsTicksDoTrecho(dados.regrasDaViagem);
    testarARota(dados.regrasDaViagem);
    testarOEncontro(dados.regrasDaViagem);
    testarAViagemNaPartida(dados);
    testarOAcampamento(dados);
}
