#include "Eventos/DefinicaoDosEventos.h"

#include <functional>
#include <set>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDeEventos = "eventos.json";

// Lê um inteiro opcional; se o campo existe e não é inteiro (ou sai da faixa), é erro.
void lerInteiroOpcional(const Json& objeto, const char* nomeDoCampo, int valorMinimo, int valorMaximo, int& valorLido,
                        const std::function<void(const std::string&)>& anotarErro)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end()) {
        return;
    }
    if (!campo->is_number_integer() || campo->get<int>() < valorMinimo || campo->get<int>() > valorMaximo) {
        anotarErro("\"" + std::string(nomeDoCampo) + "\" precisa ser um número inteiro de " + std::to_string(valorMinimo) + " a "
                   + std::to_string(valorMaximo));
        return;
    }
    valorLido = campo->get<int>();
}

// Um efeito da lista fechada (regra 4.4). Devolve vazio se o efeito tem erro.
std::optional<EfeitoDoEvento> lerEfeito(const Json& objeto, const CatalogosParaOsEventos& catalogos,
                                        const std::function<void(const std::string&)>& anotarErro)
{
    EfeitoDoEvento efeito;
    bool temErro = false;
    const auto anotarErroDoEfeito = [&](const std::string& motivo) {
        anotarErro(motivo);
        temErro = true;
    };
    if (!objeto.is_object()) {
        anotarErro("cada efeito precisa ser um objeto { ... }");
        return std::nullopt;
    }
    if (objeto.contains("producao_da_construcao")) {
        efeito.tipo = TipoDeEfeito::ProducaoDaConstrucao;
        const Json& construcao = objeto["producao_da_construcao"];
        efeito.tipoDaConstrucao = construcao.is_string() ? construcao.get<std::string>() : construcao.dump();
        if (catalogos.catalogoDeConstrucoes.procurarConstrucao(efeito.tipoDaConstrucao) == nullptr) {
            anotarErroDoEfeito("a construção " + efeito.tipoDaConstrucao + " não existe no construcoes.json");
        }
        if (!objeto.contains("porcentagem")) {
            anotarErroDoEfeito("o efeito \"producao_da_construcao\" precisa da \"porcentagem\"");
        }
        lerInteiroOpcional(objeto, "porcentagem", 0, 1000, efeito.porcentagem, anotarErroDoEfeito);
    } else if (objeto.contains("felicidade")) {
        efeito.tipo = TipoDeEfeito::Felicidade;
        lerInteiroOpcional(objeto, "felicidade", -100, 100, efeito.pontosDeFelicidade, anotarErroDoEfeito);
    } else if (objeto.contains("mudar_estoque")) {
        efeito.tipo = TipoDeEfeito::MudarEstoque;
        const Json& item = objeto["mudar_estoque"];
        const std::string identificadorDoItem = item.is_string() ? item.get<std::string>() : item.dump();
        efeito.numeroDoItem = catalogos.catalogoDeItens.procurarNumeroDoItem(identificadorDoItem).value_or(-1);
        if (efeito.numeroDoItem < 0) {
            anotarErroDoEfeito("o item " + identificadorDoItem + " não existe no itens.csv");
        }
        efeito.porcentagem = 0;
        lerInteiroOpcional(objeto, "porcentagem", -100, 100, efeito.porcentagem, anotarErroDoEfeito);
        lerInteiroOpcional(objeto, "quantidade", -10'000, 10'000, efeito.quantidade, anotarErroDoEfeito);
        if ((efeito.porcentagem == 0) == (efeito.quantidade == 0)) {
            anotarErroDoEfeito("o efeito \"mudar_estoque\" precisa de uma \"porcentagem\" ou de uma \"quantidade\" (uma só)");
        }
    } else if (objeto.contains("npcs_doentes")) {
        efeito.tipo = TipoDeEfeito::NpcsDoentes;
        lerInteiroOpcional(objeto, "npcs_doentes", 1, 1000, efeito.quantidade, anotarErroDoEfeito);
    } else if (objeto.contains("npcs_doentes_em_porcento_dos_civis")) {
        efeito.tipo = TipoDeEfeito::NpcsDoentesEmPorcento;
        lerInteiroOpcional(objeto, "npcs_doentes_em_porcento_dos_civis", 1, 100, efeito.porcentagem, anotarErroDoEfeito);
    } else if (objeto.contains("mortes_por_dia_em_por_mil_da_populacao")) {
        efeito.tipo = TipoDeEfeito::MortesDaPopulacao;
        lerInteiroOpcional(objeto, "mortes_por_dia_em_por_mil_da_populacao", 0, 1000, efeito.quantidade, anotarErroDoEfeito);
    } else if (objeto.contains("remedio_por_dia_a_cada_npcs")) {
        efeito.tipo = TipoDeEfeito::Remedio;
        lerInteiroOpcional(objeto, "remedio_por_dia_a_cada_npcs", 1, 1000, efeito.quantidade, anotarErroDoEfeito);
        efeito.porcentagem = 50;
        lerInteiroOpcional(objeto, "mortes_com_remedio_em_porcento", 0, 100, efeito.porcentagem, anotarErroDoEfeito);
    } else if (objeto.contains("lenha_em_porcento")) {
        // Ficha do Clima, regra 4.5: o Inverno rigoroso dobra a lenha.
        efeito.tipo = TipoDeEfeito::Lenha;
        lerInteiroOpcional(objeto, "lenha_em_porcento", 0, 1000, efeito.porcentagem, anotarErroDoEfeito);
    } else if (objeto.contains("mercador_compra")) {
        efeito.tipo = TipoDeEfeito::MercadorCompra;
        lerInteiroOpcional(objeto, "mercador_compra", 1, 10'000, efeito.quantidade, anotarErroDoEfeito);
    } else {
        anotarErro("o efeito " + objeto.dump() + " não é da lista (producao_da_construcao, felicidade, mudar_estoque, npcs_doentes, mercador_compra, lenha_em_porcento, npcs_doentes_em_porcento_dos_civis, mortes_por_dia_em_por_mil_da_populacao, remedio_por_dia_a_cada_npcs)");
        return std::nullopt;
    }
    if (temErro) {
        return std::nullopt;
    }
    return efeito;
}

// A condição de um evento condicional.
std::optional<CondicaoDoEvento> lerCondicao(const Json& objeto, const std::function<void(const std::string&)>& anotarErro)
{
    if (!objeto.is_object()) {
        anotarErro("a \"condicao\" precisa ser um objeto { ... }");
        return std::nullopt;
    }
    CondicaoDoEvento condicao;
    if (objeto.contains("comida_abaixo_do_alvo_em_porcento")) {
        condicao.tipo = TipoDeCondicao::ComidaAbaixoDoAlvo;
        lerInteiroOpcional(objeto, "comida_abaixo_do_alvo_em_porcento", 0, 1000, condicao.entraAbaixoDoAlvoEmPorcento, anotarErro);
        condicao.saiAcimaDoAlvoEmPorcento = condicao.entraAbaixoDoAlvoEmPorcento;
        lerInteiroOpcional(objeto, "sai_acima_do_alvo_em_porcento", 0, 1000, condicao.saiAcimaDoAlvoEmPorcento, anotarErro);
        if (condicao.saiAcimaDoAlvoEmPorcento < condicao.entraAbaixoDoAlvoEmPorcento) {
            anotarErro("\"sai_acima_do_alvo_em_porcento\" não pode ser menor que \"comida_abaixo_do_alvo_em_porcento\"");
        }
    } else if (objeto.contains("materiais_em_deficit")) {
        condicao.tipo = TipoDeCondicao::MateriaisEmDeficit;
    } else {
        anotarErro("a condição " + objeto.dump() + " não existe (comida_abaixo_do_alvo_em_porcento ou materiais_em_deficit)");
        return std::nullopt;
    }
    lerInteiroOpcional(objeto, "dias_seguidos", 1, 1000, condicao.diasSeguidos, anotarErro);
    return condicao;
}

void lerCamposDoEvento(const Json& objeto, DefinicaoDeEvento& evento, const CatalogosParaOsEventos& catalogos,
                       ListaDeProblemasDosDados& listaDeProblemas)
{
    const std::function<void(const std::string&)> anotarErro = [&](const std::string& motivo) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeEventos, 0, evento.identificadorDeTexto + ": " + motivo);
    };

    const std::string tipo = objeto.contains("tipo") && objeto["tipo"].is_string() ? objeto["tipo"].get<std::string>() : "";
    if (tipo == "ocasional") {
        evento.tipo = TipoDeEvento::Ocasional;
    } else if (tipo == "condicional") {
        evento.tipo = TipoDeEvento::Condicional;
    } else if (tipo == "data_fixa") {
        evento.tipo = TipoDeEvento::DataFixa;
    } else {
        anotarErro("o \"tipo\" precisa ser \"ocasional\", \"condicional\" ou \"data_fixa\"");
    }
    evento.negativo = objeto.contains("negativo") && objeto["negativo"].is_boolean() && objeto["negativo"].get<bool>();

    if (const auto estacoes = objeto.find("estacoes"); estacoes != objeto.end()) {
        for (const Json& estacao : estacoes->is_array() ? *estacoes : Json::array({*estacoes})) {
            const std::optional<Estacao> estacaoLida = estacao.is_string() ? lerEstacaoDoTexto(estacao.get<std::string>()) : std::nullopt;
            if (!estacaoLida.has_value()) {
                anotarErro("a estação " + estacao.dump() + " não existe (primavera, verao, outono, inverno)");
                continue;
            }
            evento.estacoes.push_back(*estacaoLida);
        }
    }
    if (const auto dias = objeto.find("dias_da_estacao"); dias != objeto.end()) {
        for (const Json& dia : dias->is_array() ? *dias : Json::array({*dias})) {
            if (!dia.is_number_integer() || dia.get<int>() < 1 || dia.get<int>() > 16) {
                anotarErro("os \"dias_da_estacao\" vão de 1 a 16");
                continue;
            }
            evento.diasDaEstacao.push_back(dia.get<int>());
        }
    }
    lerInteiroOpcional(objeto, "chance_por_dia_em_por_mil", 0, 1000, evento.chancePorDiaEmPorMil, anotarErro);
    lerInteiroOpcional(objeto, "cooldown_dias", 0, 10'000, evento.cooldownEmDias, anotarErro);
    lerInteiroOpcional(objeto, "duracao_dias", 0, 10'000, evento.duracaoEmDias, anotarErro);
    // Fase A6 (ficha da Cura): os modificadores da chance.
    if (const auto modificadores = objeto.find("modificadores_da_chance"); modificadores != objeto.end() && modificadores->is_array()) {
        for (const Json& item : *modificadores) {
            ModificadorDaChance modificador;
            if (!item.is_object() || !item.contains("porcentagem") || !item["porcentagem"].is_number_integer()) {
                anotarErro("cada modificador da chance precisa de uma \"porcentagem\"");
                continue;
            }
            modificador.porcentagem = item["porcentagem"].get<int>();
            if (item.contains("ocupacao_minima_em_porcento") && item["ocupacao_minima_em_porcento"].is_number_integer()) {
                modificador.tipo = TipoDeModificadorDaChance::OcupacaoMinima;
                modificador.ocupacaoMinimaEmPorcento = item["ocupacao_minima_em_porcento"].get<int>();
            } else if (item.contains("comida_em_deficit")) {
                modificador.tipo = TipoDeModificadorDaChance::ComidaEmDeficit;
            } else if (item.contains("existe_construcao") && item["existe_construcao"].is_array()) {
                // As construções podem ainda não existir no construcoes.json (o Boticário vem com o conteúdo).
                modificador.tipo = TipoDeModificadorDaChance::ExisteConstrucao;
                for (const Json& construcao : item["existe_construcao"]) {
                    if (construcao.is_string()) {
                        modificador.construcoes.push_back(construcao.get<std::string>());
                    }
                }
            } else {
                anotarErro("o modificador da chance não é da lista (ocupacao_minima_em_porcento, comida_em_deficit, existe_construcao)");
                continue;
            }
            evento.modificadoresDaChance.push_back(modificador);
        }
    }

    if (evento.tipo == TipoDeEvento::Condicional) {
        if (!objeto.contains("condicao")) {
            anotarErro("um evento condicional precisa da \"condicao\"");
        } else {
            evento.condicao = lerCondicao(objeto["condicao"], anotarErro);
        }
    }
    if (evento.tipo == TipoDeEvento::DataFixa) {
        const std::string estacao = objeto.contains("estacao") && objeto["estacao"].is_string() ? objeto["estacao"].get<std::string>() : "";
        const std::optional<Estacao> estacaoLida = lerEstacaoDoTexto(estacao);
        if (!estacaoLida.has_value()) {
            anotarErro("um evento de data fixa precisa da \"estacao\" (primavera, verao, outono ou inverno)");
        } else {
            evento.estacaoDaDataFixa = *estacaoLida;
        }
        evento.diaDaDataFixa = 0;
        lerInteiroOpcional(objeto, "dia_da_estacao", 1, 16, evento.diaDaDataFixa, anotarErro);
        if (evento.diaDaDataFixa == 0) {
            anotarErro("um evento de data fixa precisa do \"dia_da_estacao\" (de 1 a 16)");
        }
    }

    const auto efeitos = objeto.find("efeitos");
    if (efeitos == objeto.end() || !efeitos->is_array() || efeitos->empty()) {
        anotarErro("falta a lista \"efeitos\"");
        return;
    }
    for (const Json& efeito : *efeitos) {
        if (const std::optional<EfeitoDoEvento> efeitoLido = lerEfeito(efeito, catalogos, anotarErro)) {
            evento.efeitos.push_back(*efeitoLido);
        }
    }
}

}  // namespace

std::optional<Estacao> lerEstacaoDoTexto(const std::string& texto)
{
    if (texto == "primavera") {
        return Estacao::Primavera;
    }
    if (texto == "verao") {
        return Estacao::Verao;
    }
    if (texto == "outono") {
        return Estacao::Outono;
    }
    if (texto == "inverno") {
        return Estacao::Inverno;
    }
    return std::nullopt;
}

CatalogoDeEventos lerArquivoDeEventos(const std::string& textoDoArquivo, const CatalogosParaOsEventos& catalogos,
                                      ListaDeProblemasDosDados& listaDeProblemas)
{
    CatalogoDeEventos catalogo;
    const Json documento = Json::parse(textoDoArquivo, nullptr, false, true);
    if (documento.is_discarded() || !documento.is_array()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeEventos, 0, "o arquivo não é um JSON válido (precisa ser uma lista [ { ... }, ... ])");
        return catalogo;
    }
    std::set<std::string> identificadoresJaLidos;
    for (const Json& objeto : documento) {
        if (!objeto.is_object() || !objeto.contains("id") || !objeto["id"].is_string()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeEventos, 0, "cada evento precisa ser um objeto com o \"id\"");
            continue;
        }
        DefinicaoDeEvento evento;
        evento.identificadorDeTexto = objeto["id"].get<std::string>();
        if (!identificadoresJaLidos.insert(evento.identificadorDeTexto).second) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeEventos, 0, evento.identificadorDeTexto + " aparece duas vezes");
            continue;
        }
        if (!catalogos.tabelaDeTextos.temTexto(evento.identificadorDeTexto)) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeEventos, 0, evento.identificadorDeTexto + " não tem nome em textos/pt-BR.json");
        }
        lerCamposDoEvento(objeto, evento, catalogos, listaDeProblemas);
        catalogo.acrescentarEvento(std::move(evento));
    }
    return catalogo;
}
