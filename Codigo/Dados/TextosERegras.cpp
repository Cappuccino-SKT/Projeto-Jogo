#include "Dados/TextosERegras.h"

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDeTextos = "textos/pt-BR.json";
const char* const nomeDoArquivoDeRegras = "regras.json";

// Lê um número inteiro de um campo do JSON. Se faltar ou não for inteiro, anota o erro.
bool lerInteiroObrigatorio(const Json& objeto, const char* nomeDoCampo, int& valorLido, const std::string& ondeEsta,
                           ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end() || !campo->is_number_integer()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta o número inteiro \"" + std::string(nomeDoCampo) + "\" em " + ondeEsta);
        return false;
    }
    valorLido = campo->get<int>();
    return true;
}

}  // namespace

void TabelaDeTextos::guardarTexto(const std::string& identificador, const std::string& textoParaMostrar)
{
    textosPorIdentificador[identificador] = textoParaMostrar;
}

bool TabelaDeTextos::temTexto(const std::string& identificador) const
{
    return textosPorIdentificador.count(identificador) > 0;
}

std::string TabelaDeTextos::textoDe(const std::string& identificador) const
{
    const auto encontrado = textosPorIdentificador.find(identificador);
    return encontrado == textosPorIdentificador.end() ? identificador : encontrado->second;
}

TabelaDeTextos lerArquivoDeTextos(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    TabelaDeTextos tabela;
    const Json documento = Json::parse(textoDoArquivo, nullptr, false);
    if (documento.is_discarded() || !documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeTextos, 0, "o arquivo não é um JSON válido (precisa ser { \"id\": \"texto\", ... })");
        return tabela;
    }
    for (const auto& [identificador, valor] : documento.items()) {
        if (!valor.is_string()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeTextos, 0, "o texto de \"" + identificador + "\" não está entre aspas");
            continue;
        }
        tabela.guardarTexto(identificador, valor.get<std::string>());
    }
    return tabela;
}

RegrasDoJogo lerArquivoDeRegras(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDoJogo regras;
    const Json documento = Json::parse(textoDoArquivo, nullptr, false);
    if (documento.is_discarded() || !documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "o arquivo não é um JSON válido");
        return regras;
    }

    // O salário por hora, em centésimos de moeda (FL3-3).
    const auto salarios = documento.find("salario_por_hora_em_centesimos");
    if (salarios == documento.end() || !salarios->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"salario_por_hora_em_centesimos\"");
    } else {
        const std::string onde = "salario_por_hora_em_centesimos";
        lerInteiroObrigatorio(*salarios, "bracal", regras.regrasDoPreco.salarioPorHoraBracalEmCentesimos, onde, listaDeProblemas);
        lerInteiroObrigatorio(*salarios, "manufaturado", regras.regrasDoPreco.salarioPorHoraManufaturadoEmCentesimos, onde, listaDeProblemas);
        lerInteiroObrigatorio(*salarios, "especialista", regras.regrasDoPreco.salarioPorHoraEspecialistaEmCentesimos, onde, listaDeProblemas);
    }
    lerInteiroObrigatorio(documento, "margem_do_dono_em_porcento", regras.regrasDoPreco.margemDoDonoEmPorcento, "regras.json", listaDeProblemas);

    // A produção por estação de cada grupo: 4 números (Primavera, Verão, Outono, Inverno).
    const auto estacoes = documento.find("producao_por_estacao_em_porcento");
    if (estacoes == documento.end() || !estacoes->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"producao_por_estacao_em_porcento\"");
    } else {
        for (const auto& [nomeDoGrupo, porcentagens] : estacoes->items()) {
            bool listaCerta = porcentagens.is_array() && porcentagens.size() == 4;
            std::array<int, 4> porcentagemPorEstacao = {0, 0, 0, 0};
            for (std::size_t estacao = 0; listaCerta && estacao < 4; ++estacao) {
                listaCerta = porcentagens[estacao].is_number_integer() && porcentagens[estacao].get<int>() >= 0;
                if (listaCerta) {
                    porcentagemPorEstacao[estacao] = porcentagens[estacao].get<int>();
                }
            }
            if (!listaCerta) {
                listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "o grupo \"" + nomeDoGrupo + "\" precisa de 4 porcentagens inteiras (Primavera, Verão, Outono, Inverno)");
                continue;
            }
            regras.producaoPorEstacaoEmPorcento[nomeDoGrupo] = porcentagemPorEstacao;
        }
    }

    const auto produtividade = documento.find("produtividade_do_recurso_em_porcento");
    if (produtividade == documento.end() || !produtividade->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"produtividade_do_recurso_em_porcento\"");
    } else {
        const std::string onde = "produtividade_do_recurso_em_porcento";
        lerInteiroObrigatorio(*produtividade, "normal", regras.produtividadeNormalEmPorcento, onde, listaDeProblemas);
        lerInteiroObrigatorio(*produtividade, "destaque", regras.produtividadeDeDestaqueEmPorcento, onde, listaDeProblemas);
    }

    // O movimento dos NPCs (marco 4). Os dois precisam ser maiores que zero.
    if (lerInteiroObrigatorio(documento, "velocidade_de_andar_em_tiles_por_hora", regras.velocidadeDeAndarEmTilesPorHora, "regras.json", listaDeProblemas)
        && regras.velocidadeDeAndarEmTilesPorHora <= 0) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"velocidade_de_andar_em_tiles_por_hora\" precisa ser maior que zero");
    }
    if (lerInteiroObrigatorio(documento, "caminhos_calculados_por_tick", regras.caminhosCalculadosPorTick, "regras.json", listaDeProblemas)
        && regras.caminhosCalculadosPorTick <= 0) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"caminhos_calculados_por_tick\" precisa ser maior que zero");
    }
    return regras;
}
