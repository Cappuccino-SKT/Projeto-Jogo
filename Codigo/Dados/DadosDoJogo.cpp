#include "Dados/DadosDoJogo.h"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>

namespace {

// Lê um arquivo inteiro. Se ele não existir, anota o erro e devolve vazio.
std::optional<std::string> lerArquivoInteiro(const std::string& pastaDosDados, const std::string& nomeDoArquivo,
                                             ListaDeProblemasDosDados& listaDeProblemas)
{
    const std::filesystem::path caminho = std::filesystem::path(pastaDosDados) / nomeDoArquivo;
    std::ifstream arquivo(caminho, std::ios::binary);
    if (!arquivo) {
        listaDeProblemas.anotarErro(nomeDoArquivo, 0, "não foi possível abrir o arquivo (" + caminho.string() + ")");
        return std::nullopt;
    }
    return std::string((std::istreambuf_iterator<char>(arquivo)), std::istreambuf_iterator<char>());
}

const char* const nomeDoArquivoDoMapaDaVila = "mapas/vila.tmj";

}  // namespace

DadosDoJogo carregarDadosDoJogo(const std::string& pastaDosDados)
{
    DadosDoJogo dados;
    ListaDeProblemasDosDados& problemas = dados.listaDeProblemas;

    // 1 e 2. Os nomes e as regras.
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "textos/pt-BR.json", problemas)) {
        dados.tabelaDeTextos = lerArquivoDeTextos(*texto, problemas);
    }
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "regras.json", problemas)) {
        dados.regras = lerArquivoDeRegras(*texto, problemas);
    }

    // 3 e 4. Os itens e as construções.
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "itens.csv", problemas)) {
        dados.catalogoDeItens = lerArquivoDeItens(*texto, dados.tabelaDeTextos, problemas);
    }
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "construcoes.json", problemas)) {
        dados.catalogoDeConstrucoes = lerArquivoDeConstrucoes(*texto, dados.tabelaDeTextos, problemas);
    }

    // 5 e 6. Os recursos e as receitas, que citam os itens e as construções.
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "recursos.csv", problemas)) {
        dados.catalogoDeRecursos = lerArquivoDeRecursos(*texto, dados.catalogoDeItens, dados.catalogoDeConstrucoes, dados.regras, problemas);
    }
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "receitas.csv", problemas)) {
        dados.catalogoDeReceitas = lerArquivoDeReceitas(*texto, dados.catalogoDeItens, dados.catalogoDeConstrucoes, problemas);
    }
    conferirTrabalhoDasConstrucoes(dados.catalogoDeConstrucoes, dados.catalogoDeReceitas, problemas);

    // 7. Os preços base e a escada de níveis.
    calcularPrecosBaseDosItens(dados.catalogoDeItens, dados.catalogoDeReceitas, dados.regras.regrasDoPreco, problemas);
    conferirEscadaDeNiveis(dados.catalogoDeItens, dados.catalogoDeReceitas, problemas);

    // 8. O mapa da vila, e a ligação das áreas aos recursos.
    const std::string caminhoDoMapa = (std::filesystem::path(pastaDosDados) / nomeDoArquivoDoMapaDaVila).string();
    ResultadoDaLeituraDoMapa leituraDoMapa = carregarMapaDoArquivoDoTiled(caminhoDoMapa);
    for (const std::string& erro : leituraDoMapa.listaDeErrosDoMapa) {
        problemas.anotarErro(nomeDoArquivoDoMapaDaVila, 0, erro);
    }
    for (const std::string& aviso : leituraDoMapa.listaDeAvisosDoMapa) {
        problemas.anotarAviso(nomeDoArquivoDoMapaDaVila, 0, aviso);
    }
    dados.mapaDaVila = std::move(leituraDoMapa.mapa);
    ligarAreasDoMapaAosRecursos(dados.mapaDaVila, nomeDoArquivoDoMapaDaVila, dados.catalogoDeRecursos, dados.regras, problemas);

    // 9. A vila inicial (marco 5): as construções nos lotes e os NPCs. Os lotes
    //    ocupados passam a bloquear a passagem (ficha da IA, regra 4.1).
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "vila_inicial.json", problemas)) {
        const CatalogosParaAVila catalogos{dados.catalogoDeConstrucoes, dados.catalogoDeItens, dados.regras.construcoes};
        dados.vilaInicial = lerVilaInicial(*texto, dados.mapaDaVila, catalogos, problemas);
        bloquearOsLotesOcupados(dados.mapaDaVila, dados.vilaInicial);
    }

    // 10. Os eventos (marco 7, ficha dos Eventos), que citam os itens e as construções.
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "eventos.json", problemas)) {
        const CatalogosParaOsEventos catalogos{dados.tabelaDeTextos, dados.catalogoDeItens, dados.catalogoDeConstrucoes};
        dados.catalogoDeEventos = lerArquivoDeEventos(*texto, catalogos, problemas);
    }

    // 11. O clima (fase A1, ficha do Clima e Estações).
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "clima.json", problemas)) {
        dados.regrasDoClima = lerArquivoDoClima(*texto, problemas);
    }

    // 12. O Mapa Mundial e a Viagem (fase B1, fichas do Mundo e da Viagem).
    const auto textoDosMapas = lerArquivoInteiro(pastaDosDados, "mundo/mapas.json", problemas);
    const auto textoDasCidades = lerArquivoInteiro(pastaDosDados, "mundo/cidades_iniciais.json", problemas);
    if (textoDosMapas && textoDasCidades) {
        dados.mundo = lerArquivosDoMundo(*textoDosMapas, *textoDasCidades, problemas);
    }
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "viagem.json", problemas)) {
        dados.regrasDaViagem = lerArquivoDaViagem(*texto, problemas);
    }
    // 13. As outras cidades (fase B1b): o bloco "outras_cidades" do regras.json.
    if (const auto texto = lerArquivoInteiro(pastaDosDados, "regras.json", problemas)) {
        dados.regrasDasOutrasCidades = lerRegrasDasOutrasCidades(*texto, problemas);
        dados.regrasDoComercio = lerRegrasDoComercio(*texto, problemas);  // fase B2: o bloco "comercio"
    }
    return dados;
}

std::string escreverDinheiroEmMoedas(int valorEmDecimos)
{
    const char* sinal = valorEmDecimos < 0 ? "-" : "";
    const int valorPositivo = valorEmDecimos < 0 ? -valorEmDecimos : valorEmDecimos;
    return sinal + std::to_string(valorPositivo / 10) + "," + std::to_string(valorPositivo % 10);
}

std::string montarResumoDosDados(const DadosDoJogo& dados)
{
    const int erros = dados.listaDeProblemas.quantidadeDeErros();
    const int avisos = dados.listaDeProblemas.quantidadeDeAvisos();
    return "Dados conferidos: " + std::to_string(dados.catalogoDeItens.quantidadeDeItens()) + " itens, "
        + std::to_string(dados.catalogoDeConstrucoes.quantidadeDeConstrucoes()) + " construções, "
        + std::to_string(dados.catalogoDeRecursos.quantidadeDeRecursos()) + " recursos, "
        + std::to_string(dados.catalogoDeReceitas.quantidadeDeReceitas()) + " receitas, 1 mapa, 1 vila ("
        + std::to_string(dados.vilaInicial.construcoes.size()) + " construções e "
        + std::to_string(dados.vilaInicial.npcsIniciais.size()) + " NPCs), "
        + std::to_string(dados.catalogoDeEventos.quantidadeDeEventos()) + " eventos. "
        + std::to_string(erros) + (erros == 1 ? " erro, " : " erros, ")
        + std::to_string(avisos) + (avisos == 1 ? " aviso." : " avisos.");
}

std::string montarTabelaDePrecosBase(const DadosDoJogo& dados)
{
    std::string tabela;
    for (const DefinicaoDeItem& item : dados.catalogoDeItens.todosOsItens()) {
        tabela += "  " + dados.tabelaDeTextos.textoDe(item.identificadorDeTexto) + ": "
            + escreverDinheiroEmMoedas(item.precoBaseEmDecimos) + " moeda\n";
    }
    return tabela;
}
