#include "Viagem/RegrasDaViagem.h"

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDaViagem = "viagem.json";

void lerInteiro(const Json& objeto, const char* nomeDoCampo, int& valor, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end()) {
        return;
    }
    if (!campo->is_number_integer() || campo->get<int>() < 0) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, "\"" + std::string(nomeDoCampo) + "\" precisa ser um inteiro de 0 ou mais");
        return;
    }
    valor = campo->get<int>();
}

void lerTabela(const Json& objeto, const char* nomeDoCampo, std::map<std::string, int>& tabela, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end() || !campo->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, "falta o objeto \"" + std::string(nomeDoCampo) + "\"");
        return;
    }
    for (const auto& [nome, valor] : campo->items()) {
        if (!valor.is_number_integer() || valor.get<int>() < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, "\"" + nome + "\" em \"" + nomeDoCampo + "\" precisa ser um inteiro de 0 ou mais");
            continue;
        }
        tabela[nome] = valor.get<int>();
    }
}

void lerPar(const Json& objeto, const char* nomeDoCampo, int& primeiro, int& segundo, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end()) {
        return;
    }
    if (!campo->is_array() || campo->size() != 2 || !(*campo)[0].is_number_integer() || !(*campo)[1].is_number_integer()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, "\"" + std::string(nomeDoCampo) + "\" precisa ser um par de inteiros");
        return;
    }
    primeiro = (*campo)[0].get<int>();
    segundo = (*campo)[1].get<int>();
}

}  // namespace

int RegrasDaViagem::forcaDe(const std::string& tipoDePessoa) const
{
    const auto forca = forcaPorPessoa.find(tipoDePessoa);
    return forca == forcaPorPessoa.end() ? 1 : forca->second;
}

RegrasDaViagem lerArquivoDaViagem(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDaViagem regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    if (!documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, "o arquivo precisa ser um objeto { ... }");
        return regras;
    }
    lerInteiro(documento, "ticks_base_por_mapa", regras.ticksBasePorMapa, listaDeProblemas);
    lerInteiro(documento, "ticks_minimos_por_mapa", regras.ticksMinimosPorMapa, listaDeProblemas);
    lerTabela(documento, "terreno_em_porcento", regras.terrenoEmPorcento, listaDeProblemas);
    lerTabela(documento, "infraestrutura_em_porcento", regras.infraestruturaEmPorcento, listaDeProblemas);
    if (const auto tamanho = documento.find("tamanho_do_grupo"); tamanho != documento.end() && tamanho->is_array()) {
        for (const Json& par : *tamanho) {
            if (par.is_array() && par.size() == 2 && par[0].is_number_integer() && par[1].is_number_integer()) {
                regras.tamanhoDoGrupo.emplace_back(par[0].get<int>(), par[1].get<int>());
            } else {
                listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, "cada item de \"tamanho_do_grupo\" precisa ser [pessoas, multiplicador]");
            }
        }
    } else {
        listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, "falta a lista \"tamanho_do_grupo\"");
    }
    lerInteiro(documento, "montados_em_porcento", regras.montadosEmPorcento, listaDeProblemas);
    lerInteiro(documento, "carga_acima_do_limite_em_porcento", regras.cargaAcimaDoLimiteEmPorcento, listaDeProblemas);
    lerInteiro(documento, "caravana_em_porcento", regras.caravanaEmPorcento, listaDeProblemas);
    lerTabela(documento, "clima_em_porcento", regras.climaEmPorcento, listaDeProblemas);
    if (const auto estacao = documento.find("estacao_em_porcento"); estacao != documento.end() && estacao->is_object()) {
        lerInteiro(*estacao, "lama_na_primavera_sem_estrada", regras.lamaNaPrimaveraSemEstradaEmPorcento, listaDeProblemas);
        lerInteiro(*estacao, "inverno_bioma_frio", regras.invernoNoBiomaFrioEmPorcento, listaDeProblemas);
        lerInteiro(*estacao, "inverno_outros", regras.invernoNosOutrosBiomasEmPorcento, listaDeProblemas);
    }
    if (const auto frios = documento.find("biomas_frios"); frios != documento.end() && frios->is_array()) {
        regras.biomasFrios.clear();
        for (const Json& bioma : *frios) {
            if (bioma.is_string()) {
                regras.biomasFrios.insert(bioma.get<std::string>());
            }
        }
    }
    lerInteiro(documento, "teto_clima_vezes_estacao_em_porcento", regras.tetoDoClimaVezesEstacaoEmPorcento, listaDeProblemas);
    lerPar(documento, "acampar_das_horas", regras.horaDeAcampar, regras.horaDeRetomar, listaDeProblemas);
    lerInteiro(documento, "refeicoes_por_pessoa_por_dia", regras.refeicoesPorPessoaPorDia, listaDeProblemas);
    if (const auto chance = documento.find("chance_de_encontro_por_mil"); chance != documento.end() && chance->is_object()) {
        lerInteiro(*chance, "territorio", regras.encontroNoTerritorioPorMil, listaDeProblemas);
        lerInteiro(*chance, "em_disputa", regras.encontroEmDisputaPorMil, listaDeProblemas);
        lerInteiro(*chance, "sem_dono", regras.encontroSemDonoPorMil, listaDeProblemas);
    }
    lerInteiro(documento, "marcha_forcada_encontro_em_porcento", regras.marchaForcadaEncontroEmPorcento, listaDeProblemas);
    if (const auto pesos = documento.find("pesos_dos_encontros"); pesos != documento.end() && pesos->is_object()) {
        lerInteiro(*pesos, "bandidos", regras.pesoDosBandidos, listaDeProblemas);
        lerInteiro(*pesos, "mercador_viajante", regras.pesoDoMercadorViajante, listaDeProblemas);
    }
    lerInteiro(documento, "bando_bandidos_minimo", regras.bandoDeBandidosMinimo, listaDeProblemas);
    lerInteiro(documento, "bando_bandidos_sorteio_ate", regras.bandoDeBandidosSorteioAte, listaDeProblemas);
    lerTabela(documento, "forca_por_pessoa", regras.forcaPorPessoa, listaDeProblemas);
    lerInteiro(documento, "bandidos_nao_atacam_se_o_grupo_for_vezes_mais_forte", regras.bandidosNaoAtacamSeOGrupoForVezesMaisForte, listaDeProblemas);
    lerInteiro(documento, "suborno_por_bandido_em_decimos", regras.subornoPorBandidoEmDecimos, listaDeProblemas);
    lerInteiro(documento, "carisma_do_jogador_ate_os_atributos", regras.carismaDoJogadorAteOsAtributos, listaDeProblemas);
    lerInteiro(documento, "fuga_base_em_porcento", regras.fugaBaseEmPorcento, listaDeProblemas);
    lerPar(documento, "fuga_limites_em_porcento", regras.fugaMinimaEmPorcento, regras.fugaMaximaEmPorcento, listaDeProblemas);
    lerInteiro(documento, "perda_ao_se_render_em_porcento", regras.perdaAoSeRenderEmPorcento, listaDeProblemas);
    lerInteiro(documento, "perda_ao_perder_a_luta_em_porcento", regras.perdaAoPerderALutaEmPorcento, listaDeProblemas);
    lerInteiro(documento, "luta_rapida_dura_ticks", regras.lutaRapidaDuraTicks, listaDeProblemas);
    lerInteiro(documento, "perda_de_carga_da_caravana_minima_em_porcento", regras.perdaDeCargaDaCaravanaMinimaEmPorcento, listaDeProblemas);
    if (regras.ticksMinimosPorMapa < 1 || regras.tamanhoDoGrupo.empty()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaViagem, 0, "\"ticks_minimos_por_mapa\" precisa ser 1 ou mais, e \"tamanho_do_grupo\" não pode ser vazio");
    }
    return regras;
}
