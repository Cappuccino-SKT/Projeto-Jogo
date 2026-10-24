#include <algorithm>
#include "Simulacao/RelatorioDaSimulacao.h"

#include <chrono>

#include "Economia/FelicidadeDaClasse.h"
#include "Npc/ComponentesDoNpc.h"
#include "Save/ArquivoDeSave.h"
#include "Comercio/Comercio.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const char* nomeDaEstacaoNoRelatorio(Estacao estacao)
{
    switch (estacao) {
    case Estacao::Primavera: return "Primavera";
    case Estacao::Verao: return "Verão";
    case Estacao::Outono: return "Outono";
    case Estacao::Inverno: return "Inverno";
    }
    return "?";
}

// "item.moveis_madeira" vira "moveis_madeira".
std::string nomeDaColunaDoItem(const std::string& identificadorDoItem)
{
    const std::string prefixo = "item.";
    return identificadorDoItem.rfind(prefixo, 0) == 0 ? identificadorDoItem.substr(prefixo.size()) : identificadorDoItem;
}

std::string escreverDecimos(int64_t valorEmDecimos)
{
    return escreverDinheiroEmMoedas(static_cast<int>(valorEmDecimos));
}

}  // namespace

LinhaDoRelatorio montarLinhaDoRelatorio(const ContextoDaProducao& contexto, int64_t diaQueAcabouContandoDoZero, Estacao estacaoDoDia,
                                        int diaDaEstacao)
{
    LinhaDoRelatorio linha;
    linha.dia = diaQueAcabouContandoDoZero + 1;
    linha.estacao = estacaoDoDia;
    linha.diaDaEstacao = diaDaEstacao;
    for (const DefinicaoDeItem& item : contexto.dados.catalogoDeItens.todosOsItens()) {
        linha.estoquePorItem.push_back(contexto.inventario.quantidadeTotalDe(item.numeroInterno));
    }
    if (const std::optional<int> pao = contexto.dados.catalogoDeItens.procurarNumeroDoItem(identificadorDoPao)) {
        linha.precoDoPaoEmDecimos = precoLocalDoItem(contexto, *pao);
    }
    const DinheiroDaVila& dinheiro = contexto.dinheiro;
    linha.bancoDaCidade = dinheiro.bancoDaCidade;
    linha.dinheiroDoJogador = dinheiro.dinheiroDoJogador;
    linha.riquezaMediaDosCivis = dinheiro.quantidadeDeCivis > 0 ? dinheiro.dinheiroDaClasseCivil / dinheiro.quantidadeDeCivis : 0;
    linha.felicidadeEmDecimos = contexto.felicidade.felicidadeEmDecimos;
    for (const auto [npc, identidade, casaETrabalho, trabalhoDoDia] :
         contexto.registroDasEntidades.view<IdentidadeDoNpc, CasaETrabalhoDoNpc, TrabalhoDoDia>().each()) {
        (casaETrabalho.indiceDoTrabalho >= 0 ? linha.empregados : linha.desempregados) += 1;
    }
    linha.diasEmDeficitDeComida = dinheiro.diasEmDeficitDeComida;
    linha.diasEmDeficitDeMateriais = dinheiro.diasEmDeficitDeMateriais;
    linha.consumoNaoPago = dinheiro.consumoNaoPagoHoje;
    linha.eventosDoDia = contexto.eventos.mudancasDeHoje;
    linha.clima = nomeDoClimaNoArquivo(contexto.clima.climaDeOntem);
    linha.lenhaDoDia = contexto.clima.lenhaTiradaHoje;
    linha.diasEmDeficitDeLenha = contexto.clima.diasEmDeficitDeLenha;
    linha.contratacoes = contexto.trabalho.contratacoesDeHoje;
    linha.npcs = static_cast<int>(contexto.npcsDaVila.size());
    linha.populacaoTotal = linha.npcs * contexto.dados.regras.populacao.fatorDeEscala;
    linha.acumulado = contexto.populacao.acumuladoDeCrescimentoEmPessoas;
    linha.nascimentos = contexto.populacao.nascimentosDeHoje;
    linha.mortes = contexto.populacao.mortesDeHoje;
    linha.migracao = contexto.populacao.migracaoDeHoje;
    linha.trocas = contexto.trabalho.trocasDeHoje;
    if (contexto.trabalho.leilao.estaAberto()) {
        linha.leilao = contexto.vila.construcoes[contexto.trabalho.leilao.indiceDaConstrucao].identificador;
    }
    return linha;
}

std::string escreverCabecalhoDoRelatorio(const DadosDoJogo& dados)
{
    std::string cabecalho = "dia;estacao;dia_da_estacao";
    for (const DefinicaoDeItem& item : dados.catalogoDeItens.todosOsItens()) {
        cabecalho += ";" + nomeDaColunaDoItem(item.identificadorDeTexto);
    }
    cabecalho += ";preco_do_pao;banco;jogador;riqueza_media;felicidade;empregados;desempregados;deficit_comida;deficit_materiais;"
                 "consumo_nao_pago;clima;lenha_do_dia;deficit_lenha;contratacoes;trocas;leilao;npcs;populacao;acumulado;nascimentos;mortes;migracao;eventos_do_dia";
    return cabecalho;
}

std::string escreverLinhaDoRelatorio(const LinhaDoRelatorio& linha)
{
    std::string texto = std::to_string(linha.dia) + ";" + nomeDaEstacaoNoRelatorio(linha.estacao) + ";" + std::to_string(linha.diaDaEstacao);
    for (const int estoque : linha.estoquePorItem) {
        texto += ";" + std::to_string(estoque);
    }
    texto += ";" + escreverDecimos(linha.precoDoPaoEmDecimos) + ";" + escreverDecimos(linha.bancoDaCidade) + ";" + escreverDecimos(linha.dinheiroDoJogador)
        + ";" + escreverDecimos(linha.riquezaMediaDosCivis) + ";" + escreverDecimos(linha.felicidadeEmDecimos) + ";" + std::to_string(linha.empregados)
        + ";" + std::to_string(linha.desempregados) + ";" + std::to_string(linha.diasEmDeficitDeComida) + ";"
        + std::to_string(linha.diasEmDeficitDeMateriais) + ";" + escreverDecimos(linha.consumoNaoPago) + ";" + linha.clima + ";"
        + std::to_string(linha.lenhaDoDia) + ";" + std::to_string(linha.diasEmDeficitDeLenha) + ";" + std::to_string(linha.contratacoes) + ";"
        + std::to_string(linha.trocas) + ";" + linha.leilao + ";" + std::to_string(linha.npcs) + ";" + std::to_string(linha.populacaoTotal) + ";"
        + std::to_string(linha.acumulado) + ";" + std::to_string(linha.nascimentos) + ";" + std::to_string(linha.mortes) + ";"
        + std::to_string(linha.migracao) + ";\"";
    for (std::size_t evento = 0; evento < linha.eventosDoDia.size(); ++evento) {
        texto += (evento > 0 ? ", " : "") + linha.eventosDoDia[evento];
    }
    return texto + "\"";
}

void conferirOsCriteriosDoDia(ConferenciaDoTesteDeUmAno& conferencia, const LinhaDoRelatorio& linha)
{
    if (conferencia.falhou) {
        return;  // fica o primeiro problema
    }
    const std::string noDia = " no dia " + std::to_string(linha.dia);
    const bool paoCaro = linha.precoDoPaoEmDecimos > maiorPrecoDoPaoQuePassaEmDecimos;
    conferencia.diasSeguidosComOPaoCaro = paoCaro ? conferencia.diasSeguidosComOPaoCaro + 1 : 0;
    if (linha.diasEmDeficitDeComida >= diasSeguidosDeFomeQueFalham) {
        conferencia.motivoDaFalha = "fome por " + std::to_string(linha.diasEmDeficitDeComida) + " dias seguidos" + noDia;
    } else if (linha.bancoDaCidade <= 0) {
        conferencia.motivoDaFalha = "o Banco zerou" + noDia;
    } else if (linha.bancoDaCidade < menorBancoQuePassaEmDecimos) {
        conferencia.motivoDaFalha = "o Banco caiu para " + escreverDecimos(linha.bancoDaCidade) + noDia;
    } else if (linha.precoDoPaoEmDecimos < menorPrecoDoPaoQuePassaEmDecimos) {
        conferencia.motivoDaFalha = "o preço do Pão foi a " + escreverDecimos(linha.precoDoPaoEmDecimos) + noDia;
    } else if (conferencia.diasSeguidosComOPaoCaro >= diasSeguidosComOPaoCaroQueFalham) {
        conferencia.motivoDaFalha = "o preço do Pão ficou acima de 2,6 por " + std::to_string(conferencia.diasSeguidosComOPaoCaro)
                                    + " dias seguidos (" + escreverDecimos(linha.precoDoPaoEmDecimos) + noDia + ")";
    } else {
        return;
    }
    conferencia.falhou = true;
}

void conferirOBancoNoFimDoAno(ConferenciaDoTesteDeUmAno& conferencia, const std::vector<LinhaDoRelatorio>& linhas)
{
    if (conferencia.falhou || static_cast<int>(linhas.size()) < diasDeUmAno) {
        return;
    }
    const int64_t bancoDoDia1 = linhas.front().bancoDaCidade;
    const int64_t bancoNoFimDoAno = linhas[diasDeUmAno - 1].bancoDaCidade;
    if (bancoNoFimDoAno < bancoDoDia1 - maiorQuedaDoBancoNoAnoEmDecimos) {
        conferencia.falhou = true;
        conferencia.motivoDaFalha = "o Banco caiu de " + escreverDecimos(bancoDoDia1) + " para " + escreverDecimos(bancoNoFimDoAno) + " no ano";
    }
}

ResultadoDaSimulacaoSemTela rodarASimulacaoSemTela(const DadosDoJogo& dados, int quantidadeDeDias, std::optional<uint64_t> semente, int npcsAMais,
                                                   std::optional<int> salvarECarregarNoDia)
{
    ResultadoDaSimulacaoSemTela resultado;
    auto partida = std::make_unique<SimulacaoDaVila>(dados, RegrasDoTempo{}, 2400, semente);
    resultado.linhasDoConsole = partida->avisosAoMontar;
    if (npcsAMais > 0) {
        partida->criarNpcsSoParaMedirODesempenho(npcsAMais);
    }
    resultado.textoDoRelatorio = escreverCabecalhoDoRelatorio(dados) + "\n";

    // Fase B2 (Comércio, teste 4): o dinheiro do mundo só muda pelo que entra de fora e pelo que os bandidos levam.
    const auto entrouDeFora = [](const SimulacaoDaVila& vila) {
        int64_t total = vila.dinheiro.dinheiroQueEntrouDeFora;
        for (const auto& cidade : vila.outrasCidades) {
            total += cidade->dinheiro.dinheiroQueEntrouDeFora;
        }
        return total;
    };
    const int64_t dinheiroDoMundoNoComeco = dinheiroDoMundo(*partida) - entrouDeFora(*partida);

    // Passo 3: tick a tick, sem esperar o tempo real, até a meia-noite do último dia.
    const auto comeco = std::chrono::steady_clock::now();
    while (static_cast<int>(resultado.linhas.size()) < quantidadeDeDias) {
        const std::vector<std::string> linhasDoTick = partida->avancarUmTick();
        ++resultado.ticksRodados;
        if (!partida->relogio.comecouUmDiaNovo()) {
            resultado.linhasDoConsole.insert(resultado.linhasDoConsole.end(), linhasDoTick.begin(), linhasDoTick.end());
            continue;
        }
        // Passo 4: a linha do relatório e os critérios (os resumos ficam de fora do console).
        for (const std::string& linha : linhasDoTick) {
            if (linha.rfind("Resumo", 0) != 0) {
                resultado.linhasDoConsole.push_back("Dia " + std::to_string(resultado.linhas.size() + 1) + ": " + linha);
            }
        }
        const LinhaDoRelatorio& linha = *partida->linhaDoRelatorioDoUltimoDia;
        resultado.linhas.push_back(linha);
        resultado.textoDoRelatorio += escreverLinhaDoRelatorio(linha) + "\n";
        conferirOsCriteriosDoDia(resultado.conferencia, linha);

        // Marco 9 (FL9-7): salva, carrega e segue com a partida carregada.
        if (salvarECarregarNoDia.has_value() && static_cast<int>(resultado.linhas.size()) == *salvarECarregarNoDia - 1) {
            const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(*partida, "Teste", "teste");
            ResultadoDoCarregamento carregada = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
            if (!carregada.deuCerto()) {
                resultado.conferencia.falhou = true;
                resultado.conferencia.motivoDaFalha = "o save do dia " + std::to_string(*salvarECarregarNoDia) + " não carregou: " + carregada.motivoDaRecusa;
                break;
            }
            partida = std::move(carregada.partida);
        }
    }
    conferirOBancoNoFimDoAno(resultado.conferencia, resultado.linhas);
    if (!partida->outrasCidades.empty()) {
        resultado.linhasDoConsole.push_back(resumoDoComercio(*partida));
        // Fase B3: as notícias do mundo.
        resultado.linhasDoConsole.push_back("Notícias: " + std::to_string(partida->noticias.proximoNumeroDaNoticia - 1) + " criadas, "
                                            + std::to_string(partida->noticias.ativas.size()) + " ativas no último dia, "
                                            + std::to_string(partida->noticias.cronicaDoMundo.size()) + " na Crônica do jogador");
        // Fase B5: a pesquisa do mundo.
        int descobertas = 0;
        for (const std::vector<bool>& tem : partida->tecnologia.temATecnologia) {
            descobertas += static_cast<int>(std::count(tem.begin(), tem.end(), true));
        }
        int pontosNoAno = 0;
        for (const int64_t pontos : partida->tecnologia.pontosAcumulados) {
            pontosNoAno += static_cast<int>(pontos);
        }
        resultado.linhasDoConsole.push_back("Tecnologia: era " + std::to_string(partida->tecnologia.eraAtualDoMundo) + ", " + std::to_string(descobertas)
                                            + " tecnologias das facções, " + std::to_string(pontosNoAno) + " pontos guardados");
        // As outras cidades no fim: quantas passam fome (CO2-1: as de comida escassa dependem das caravanas).
        int cidadesComFome = 0;
        int piorMapa = 0;
        int piorDias = 0;
        for (const auto& cidade : partida->outrasCidades) {
            const int dias = cidade->dinheiro.diasEmDeficitDeComida;
            cidadesComFome += dias > 0 ? 1 : 0;
            if (dias > piorDias) {
                piorDias = dias;
                piorMapa = cidade->numeroDoMapaDaCidade;
            }
        }
        resultado.linhasDoConsole.push_back("Outras cidades: " + std::to_string(cidadesComFome) + " com fome no último dia"
                                            + (piorDias > 0 ? " (a pior, o mapa " + std::to_string(piorMapa) + ", há " + std::to_string(piorDias) + " dias)" : "") + ".");
        const int64_t diferenca = dinheiroDoMundo(*partida) - entrouDeFora(*partida) + partida->estatisticasDoComercio.dinheiroPerdido - dinheiroDoMundoNoComeco;
        resultado.linhasDoConsole.push_back("Dinheiro do mundo: " + std::string(diferenca == 0 ? "conservado" : "MUDOU " + std::to_string(diferenca) + " décimos")
                                            + " (fora o que entrou de fora e o que os bandidos levaram).");
    }
    const auto fim = std::chrono::steady_clock::now();
    resultado.segundosQueLevou = std::chrono::duration<double>(fim - comeco).count();
    resultado.microssegundosPorTick = resultado.ticksRodados > 0 ? resultado.segundosQueLevou * 1e6 / static_cast<double>(resultado.ticksRodados) : 0;
    return resultado;
}
