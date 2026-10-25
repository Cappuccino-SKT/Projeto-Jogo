// Os testes do marco 9 (Ficha Lógica: Save, parte 8): ida e volta, continuar
// igual, versão errada, ID que sumiu, arquivo quebrado e gravação segura.

#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Jogador/ObrasDaVila.h"
#include "Save/ArquivoDeSave.h"
#include "Simulacao/RelatorioDaSimulacao.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDaFatia = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";
constexpr int64_t ticksDeUmDia = 19'200;

void avancarOsTicks(SimulacaoDaVila& simulacao, int64_t quantidadeDeTicks)
{
    for (int64_t tick = 0; tick < quantidadeDeTicks; ++tick) {
        simulacao.avancarUmTick();
    }
}

ResultadoDoCarregamento carregar(const DadosDoJogo& dados, const std::vector<uint8_t>& bytes)
{
    return carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
}

bool mensagemTem(const std::string& mensagem, const char* pedaco) { return mensagem.find(pedaco) != std::string::npos; }

// Teste 1 (Fatia Vertical, parte 5): salvar, carregar e salvar de novo dá o mesmo arquivo.
void testarIdaEVolta(const DadosDoJogo& dados)
{
    SimulacaoDaVila partida(dados, RegrasDoTempo{});
    avancarOsTicks(partida, 10 * ticksDeUmDia + 5'300);  // 10 dias e um pedaço do dia 11: NPCs no meio de caminhos
    const std::vector<uint8_t> primeiro = gravarAPartidaEmBytes(partida, "Teste", "2026-10-19 21:40");
    conferir(!primeiro.empty(), "o save tem bytes");

    ResultadoDoCarregamento carregada = carregar(dados, primeiro);
    conferir(carregada.deuCerto(), "o save de 10 dias carrega");
    if (!carregada.deuCerto()) {
        std::printf("  motivo: %s\n", carregada.motivoDaRecusa.c_str());
        return;
    }
    const std::vector<uint8_t> segundo = gravarAPartidaEmBytes(*carregada.partida, "Teste", "2026-10-19 21:40");
    conferir(primeiro == segundo, "salvar, carregar e salvar de novo dá o mesmo arquivo, byte a byte");
    conferir(carregada.partida->relogio.tickAtual() == partida.relogio.tickAtual(), "o tick carregado é o gravado");
    conferir(carregada.partida->mapa.passagemPorTile == partida.mapa.passagemPorTile, "o mapa bloqueado é recalculado igual");

    // O cabeçalho para a lista de saves.
    const std::string pasta = (std::filesystem::temp_directory_path() / "teste_do_save_cabecalho").string();
    const std::string caminho = pasta + "/espaco1.save";
    conferir(gravarAPartidaNoArquivo(partida, caminho, "Espaço 1", "2026-10-19 21:40").empty(), "grava o espaço 1 no disco");
    const std::optional<CabecalhoDoSave> cabecalho = lerOCabecalhoDoArquivo(caminho);
    conferir(cabecalho.has_value() && cabecalho->nomeDoEspaco == "Espaço 1" && cabecalho->dataReal == "2026-10-19 21:40" &&
                 cabecalho->versaoDoFormato == versaoDoFormatoDoSave && cabecalho->tickAtual == partida.relogio.tickAtual(),
             "o cabeçalho tem o nome, a data real, a versão e o tick");
    conferir(cabecalho.has_value() && cabecalho->dataDoJogo == "Dia 11 da Primavera, Ano 412, 12h", "o cabeçalho tem a data do jogo em texto");

    // Teste 6: gravação segura.
    conferir(!std::filesystem::exists(caminho + ".tmp"), "o arquivo .tmp some depois de salvar");
    conferir(gravarAPartidaNoArquivo(partida, caminho, "Espaço 1", "2026-10-19 21:41").empty(), "salvar de novo troca o save antigo");
    conferir(!std::filesystem::exists(caminho + ".tmp") && lerOCabecalhoDoArquivo(caminho)->dataReal == "2026-10-19 21:41",
             "o save novo substituiu o antigo e o .tmp sumiu");
    ResultadoDoCarregamento doDisco = carregarAPartidaDoArquivo(dados, RegrasDoTempo{}, 2400, caminho);
    conferir(doDisco.deuCerto(), "carrega do arquivo do disco");
    std::filesystem::remove_all(pasta);
    conferir(!carregarAPartidaDoArquivo(dados, RegrasDoTempo{}, 2400, caminho).deuCerto(), "um arquivo que não existe é recusado");
}

// Teste 2 (FL9-7): salvar no meio e continuar dá o mesmo relatório.
void testarContinuarIgual(const DadosDoJogo& dados)
{
    const ResultadoDaSimulacaoSemTela semSave = rodarASimulacaoSemTela(dados, 40, 1, 0);
    const ResultadoDaSimulacaoSemTela comSave = rodarASimulacaoSemTela(dados, 40, 1, 0, 20);
    conferir(!comSave.conferencia.falhou, "a partida com save e carregamento no dia 20 passa nos critérios");
    conferir(semSave.linhas.size() == comSave.linhas.size(), "as duas partidas têm o mesmo número de dias");
    conferir(semSave.textoDoRelatorio == comSave.textoDoRelatorio, "salvar e carregar no dia 20 dá o mesmo relatório, linha por linha");
    if (semSave.textoDoRelatorio != comSave.textoDoRelatorio) {
        for (std::size_t dia = 0; dia < semSave.linhas.size() && dia < comSave.linhas.size(); ++dia) {
            if (!(semSave.linhas[dia] == comSave.linhas[dia])) {
                std::printf("  a primeira linha diferente é a do dia %d:\n    sem save: %s\n    com save: %s\n", static_cast<int>(dia + 1),
                            escreverLinhaDoRelatorio(semSave.linhas[dia]).c_str(), escreverLinhaDoRelatorio(comSave.linhas[dia]).c_str());
                break;
            }
        }
    }
}

// O erro do exemplo da FL9-7: uma obra do jogador, o lote bloqueado e a fila de obras
// voltam do save, e as duas partidas seguem iguais.
void testarObraDoJogador(const DadosDoJogo& dados)
{
    SimulacaoDaVila continua(dados, RegrasDoTempo{});
    avancarOsTicks(continua, 2 * ticksDeUmDia);
    // A Fazenda no lote 5 (como no teste das obras do marco 8): 10 Tábuas, 10 Blocos e 40 h.
    continua.inventario.tentarPor(dados.catalogoDeItens.procurarNumeroDoItem("item.tabua").value_or(0), DonoDoItem::Jogador, 10);
    continua.dinheiro.dinheiroDoJogador += 5'000;
    ContextoDaProducao contexto = continua.contextoDaProducao();
    const bool obraEncomendada = encomendarUmaConstrucaoNova(contexto, 5, "construcao.fazenda").empty();
    conferir(obraEncomendada, "o teste conseguiu encomendar uma obra");
    avancarOsTicks(continua, ticksDeUmDia / 2);

    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(continua, "Teste", "teste");
    ResultadoDoCarregamento carregada = carregar(dados, bytes);
    conferir(carregada.deuCerto(), "o save com uma obra em andamento carrega");
    if (!carregada.deuCerto()) {
        std::printf("  motivo: %s\n", carregada.motivoDaRecusa.c_str());
        return;
    }
    SimulacaoDaVila& recarregada = *carregada.partida;
    conferir(recarregada.vila.construcoes.size() == continua.vila.construcoes.size(), "a construção nova volta na lista");
    conferir(recarregada.mapa.passagemPorTile == continua.mapa.passagemPorTile, "o lote da obra volta bloqueado");
    conferir(recarregada.obras.fila.size() == continua.obras.fila.size(), "a fila de obras volta");

    avancarOsTicks(continua, 12 * ticksDeUmDia);
    avancarOsTicks(recarregada, 12 * ticksDeUmDia);
    conferir(gravarAPartidaEmBytes(continua, "Teste", "teste") == gravarAPartidaEmBytes(recarregada, "Teste", "teste"),
             "12 dias depois, a partida carregada e a que não foi salva têm o mesmo estado");
    conferir(recarregada.mapa.passagemPorTile == continua.mapa.passagemPorTile, "12 dias depois, o mapa bloqueado é o mesmo");
}

void testarRecusas(const DadosDoJogo& dados)
{
    SimulacaoDaVila partida(dados, RegrasDoTempo{});
    avancarOsTicks(partida, ticksDeUmDia);
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(partida, "Teste", "teste");

    // Teste 3: versão errada.
    nlohmann::json arvore = nlohmann::json::from_msgpack(bytes);
    arvore["cabecalho"]["versaoDoFormato"] = 0;
    ResultadoDoCarregamento outraVersao = carregar(dados, nlohmann::json::to_msgpack(arvore));
    conferir(!outraVersao.deuCerto(), "um save de outra versão é recusado");
    conferir(mensagemTem(outraVersao.motivoDaRecusa, "outra versão do jogo (versão 0; o jogo usa a 11)"), "a mensagem diz as duas versões");

    // Teste 4: ID que sumiu dos dados.
    arvore = nlohmann::json::from_msgpack(bytes);
    nlohmann::json& cidade = arvore["inventario"]["cidade"];
    cidade["item.vinho"] = cidade["item.pao"];
    cidade.erase("item.pao");
    ResultadoDoCarregamento semOItem = carregar(dados, nlohmann::json::to_msgpack(arvore));
    conferir(!semOItem.deuCerto(), "um save com um item que não existe é recusado");
    conferir(mensagemTem(semOItem.motivoDaRecusa, "item.vinho"), "a mensagem diz o ID que sumiu");

    // Teste 5: arquivo cortado pela metade.
    std::vector<uint8_t> cortado(bytes.begin(), bytes.begin() + static_cast<std::ptrdiff_t>(bytes.size() / 2));
    ResultadoDoCarregamento quebrado = carregar(dados, cortado);
    conferir(!quebrado.deuCerto() && mensagemTem(quebrado.motivoDaRecusa, "Não foi possível ler o save"), "um save cortado pela metade é recusado, sem travar");
    conferir(!carregar(dados, {}).deuCerto(), "um arquivo vazio é recusado");
    conferir(!carregar(dados, {0x01, 0x02, 0x03}).deuCerto(), "bytes sem sentido são recusados");

    // Uma seção que falta.
    arvore = nlohmann::json::from_msgpack(bytes);
    arvore.erase("npcs");
    conferir(!carregar(dados, nlohmann::json::to_msgpack(arvore)).deuCerto(), "um save sem a seção dos NPCs é recusado");

    // A partida antiga continua como estava.
    conferir(partida.relogio.tickAtual() == 4'800 + ticksDeUmDia, "a recusa não muda a partida atual");

    // O comando --save-para-json.
    bool deuCerto = false;
    const std::string texto = converterOSaveEmTexto(bytes, deuCerto);
    conferir(deuCerto && mensagemTem(texto, "\"item.pao\""), "o save convertido em texto mostra os IDs");
}

}  // namespace

void testarSave()
{
    std::printf("Testes do Save (marco 9)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    conferir(dados.dadosEstaoProntos(), "os dados da fatia estão certos");
    testarIdaEVolta(dados);
    testarRecusas(dados);
    testarObraDoJogador(dados);
    testarContinuarIgual(dados);
}
