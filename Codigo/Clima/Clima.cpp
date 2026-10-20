#include "Clima/Clima.h"

#include <nlohmann/json.hpp>

// ordered_json: guarda a ordem do arquivo (o sorteio anda pela tabela nessa ordem).
using Json = nlohmann::ordered_json;

namespace {

const char* const nomeDoArquivoDoClima = "clima.json";

struct NomeDoClima {
    TipoDeClima clima;
    const char* nome;
};

const NomeDoClima nomesDosClimas[] = {
    {TipoDeClima::Limpo, "limpo"},           {TipoDeClima::Nublado, "nublado"}, {TipoDeClima::Chuva, "chuva"},
    {TipoDeClima::Tempestade, "tempestade"}, {TipoDeClima::Neve, "neve"},       {TipoDeClima::Nevoa, "nevoa"},
    {TipoDeClima::CalorForte, "calor_forte"},
};

const char* const nomesDasEstacoes[] = {"primavera", "verao", "outono", "inverno"};

// Lê um inteiro opcional: se o campo existe, precisa ser inteiro entre o mínimo e o máximo.
void lerInteiroOpcional(const Json& objeto, const char* nomeDoCampo, int minimo, int maximo, int& valor, const std::string& ondeEsta,
                        ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end()) {
        return;
    }
    if (!campo->is_number_integer() || campo->get<int>() < minimo || campo->get<int>() > maximo) {
        listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, "\"" + std::string(nomeDoCampo) + "\" em " + ondeEsta + " precisa ser um inteiro de "
                                    + std::to_string(minimo) + " a " + std::to_string(maximo));
        return;
    }
    valor = campo->get<int>();
}

}  // namespace

std::optional<TipoDeClima> climaPeloNome(const std::string& nome)
{
    for (const NomeDoClima& nomeDoClima : nomesDosClimas) {
        if (nome == nomeDoClima.nome) {
            return nomeDoClima.clima;
        }
    }
    return std::nullopt;
}

std::string nomeDoClimaNoArquivo(TipoDeClima clima)
{
    for (const NomeDoClima& nomeDoClima : nomesDosClimas) {
        if (nomeDoClima.clima == clima) {
            return nomeDoClima.nome;
        }
    }
    return "limpo";
}

std::string identificadorDeTextoDoClima(TipoDeClima clima)
{
    return "clima." + nomeDoClimaNoArquivo(clima);
}

RegrasDoClima lerArquivoDoClima(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDoClima regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    if (!documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, "o arquivo precisa ser um objeto { ... }");
        return regras;
    }

    lerInteiroOpcional(documento, "chance_de_repetir_o_clima_em_por_mil", 0, 1000, regras.chanceDeRepetirOClimaEmPorMil, "o arquivo", listaDeProblemas);

    // As chances de cada estação: os climas na ordem do arquivo, somando 1.000.
    const auto chances = documento.find("chances_por_estacao_em_por_mil");
    if (chances == documento.end() || !chances->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, "falta o objeto \"chances_por_estacao_em_por_mil\"");
    } else {
        for (int estacao = 0; estacao < 4; ++estacao) {
            const std::string nomeDaEstacao = nomesDasEstacoes[estacao];
            const auto daEstacao = chances->find(nomeDaEstacao);
            if (daEstacao == chances->end() || !daEstacao->is_object()) {
                listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, "faltam as chances da estação \"" + nomeDaEstacao + "\"");
                continue;
            }
            int soma = 0;
            for (const auto& [nomeDoClima, chance] : daEstacao->items()) {
                const std::optional<TipoDeClima> clima = climaPeloNome(nomeDoClima);
                if (!clima) {
                    listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, "o clima \"" + nomeDoClima + "\" (em " + nomeDaEstacao + ") não existe");
                    continue;
                }
                if (!chance.is_number_integer() || chance.get<int>() < 0) {
                    listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, "a chance de \"" + nomeDoClima + "\" em " + nomeDaEstacao + " precisa ser um inteiro de 0 ou mais");
                    continue;
                }
                regras.chancesPorEstacao[estacao].emplace_back(*clima, chance.get<int>());
                soma += chance.get<int>();
            }
            if (soma != 1000) {
                listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, "as chances de " + nomeDaEstacao + " somam " + std::to_string(soma) + ", e não 1000");
            }
        }
    }
    // Os efeitos do clima na vila.
    if (const auto efeitos = documento.find("efeitos_do_clima"); efeitos != documento.end() && efeitos->is_object()) {
        if (const auto tempestade = efeitos->find("tempestade"); tempestade != efeitos->end() && tempestade->is_object()) {
            lerInteiroOpcional(*tempestade, "trabalho_ao_ar_livre_em_porcento", 0, 100, regras.trabalhoAoArLivreNaTempestadeEmPorcento, "tempestade", listaDeProblemas);
            if (const auto emCasa = tempestade->find("periodo_livre_em_casa"); emCasa != tempestade->end() && emCasa->is_boolean()) {
                regras.periodoLivreEmCasaNaTempestade = emCasa->get<bool>();
            }
        }
        if (const auto neve = efeitos->find("neve"); neve != efeitos->end() && neve->is_object()) {
            lerInteiroOpcional(*neve, "lenha_em_porcento", 0, 1000, regras.lenhaNaNeveEmPorcento, "neve", listaDeProblemas);
        }
        if (const auto calor = efeitos->find("calor_forte"); calor != efeitos->end() && calor->is_object()) {
            lerInteiroOpcional(*calor, "felicidade", -100, 100, regras.felicidadeNoCalorForte, "calor_forte", listaDeProblemas);
        }
    }
    if (const auto aquecimento = documento.find("aquecimento"); aquecimento != documento.end() && aquecimento->is_object()) {
        lerInteiroOpcional(*aquecimento, "lenha_por_morador_por_dia_em_centesimos", 0, 10'000, regras.lenhaPorMoradorPorDiaEmCentesimos, "aquecimento", listaDeProblemas);
        lerInteiroOpcional(*aquecimento, "lenha_do_bioma_em_porcento", 0, 1000, regras.lenhaDoBiomaEmPorcento, "aquecimento", listaDeProblemas);
        lerInteiroOpcional(*aquecimento, "felicidade_sem_lenha", -100, 0, regras.felicidadeSemLenha, "aquecimento", listaDeProblemas);
    }
    if (const auto estocar = documento.find("estocar_para_o_inverno"); estocar != documento.end() && estocar->is_object()) {
        lerInteiroOpcional(*estocar, "ultimos_dias_do_outono", 0, 16, regras.diasDoFimDoOutonoParaEstocar, "estocar_para_o_inverno", listaDeProblemas);
        lerInteiroOpcional(*estocar, "dias_de_lenha", 0, 64, regras.diasDeLenhaEstocada, "estocar_para_o_inverno", listaDeProblemas);
        lerInteiroOpcional(*estocar, "dias_de_grao", 0, 64, regras.diasDeGraoEstocado, "estocar_para_o_inverno", listaDeProblemas);
    }
    if (const auto primeiro = documento.find("clima_do_primeiro_dia"); primeiro != documento.end()) {
        const std::optional<TipoDeClima> clima = primeiro->is_string() ? climaPeloNome(primeiro->get<std::string>()) : std::nullopt;
        if (!clima) {
            listaDeProblemas.anotarErro(nomeDoArquivoDoClima, 0, "o \"clima_do_primeiro_dia\" não é um clima da lista");
        } else {
            regras.climaDoPrimeiroDia = *clima;
        }
    }
    return regras;
}

int chanceDoClimaNaEstacao(TipoDeClima clima, Estacao estacao, const RegrasDoClima& regras)
{
    for (const auto& [climaDaLista, chance] : regras.chancesPorEstacao[static_cast<int>(estacao)]) {
        if (climaDaLista == clima) {
            return chance;
        }
    }
    return 0;
}

TipoDeClima escolherClimaDoDia(TipoDeClima climaDeOntem, Estacao estacaoDeHoje, int numeroDeRepetir, int numeroDaTabela,
                               const RegrasDoClima& regras)
{
    // Passo 2: repete o de ontem, se ele existe na estação de hoje.
    if (numeroDeRepetir < regras.chanceDeRepetirOClimaEmPorMil && chanceDoClimaNaEstacao(climaDeOntem, estacaoDeHoje, regras) > 0) {
        return climaDeOntem;
    }
    // Passo 3: anda pela tabela na ordem do arquivo, somando as chances.
    const auto& tabela = regras.chancesPorEstacao[static_cast<int>(estacaoDeHoje)];
    int soma = 0;
    for (const auto& [clima, chance] : tabela) {
        soma += chance;
        if (numeroDaTabela < soma) {
            return clima;
        }
    }
    return tabela.empty() ? climaDeOntem : tabela.back().first;
}

TipoDeClima sortearClimaDoDia(TipoDeClima climaDeOntem, Estacao estacaoDeHoje, const RegrasDoClima& regras, SorteioComSemente& sorteio)
{
    const int numeroDeRepetir = sorteio.sortearNumeroInteiroEntre(0, 999);
    const int numeroDaTabela = sorteio.sortearNumeroInteiroEntre(0, 999);
    return escolherClimaDoDia(climaDeOntem, estacaoDeHoje, numeroDeRepetir, numeroDaTabela, regras);
}

int porcentagemDoTrabalhoAoArLivre(TipoDeClima climaDeHoje, const RegrasDoClima& regras)
{
    return climaDeHoje == TipoDeClima::Tempestade ? regras.trabalhoAoArLivreNaTempestadeEmPorcento : 100;
}

int calcularLenhaDoDiaEmCentesimos(int moradores, TipoDeClima climaDeHoje, int multiplicadorDosEventosEmPorcento, const RegrasDoClima& regras)
{
    int lenha = moradores * regras.lenhaPorMoradorPorDiaEmCentesimos;
    lenha = lenha * regras.lenhaDoBiomaEmPorcento / 100;
    if (climaDeHoje == TipoDeClima::Neve) {
        lenha = lenha * regras.lenhaNaNeveEmPorcento / 100;
    }
    return lenha * multiplicadorDosEventosEmPorcento / 100;
}

int pagarALenhaDevida(EstadoDoClima& estado, int lenhaDeHojeEmCentesimos, int torasNoEstoque)
{
    estado.lenhaDevidaEmCentesimos += lenhaDeHojeEmCentesimos;
    int torasTiradas = 0;
    while (estado.lenhaDevidaEmCentesimos >= 100 && torasTiradas < torasNoEstoque) {
        ++torasTiradas;
        estado.lenhaDevidaEmCentesimos -= 100;
    }
    estado.faltouLenhaHoje = estado.lenhaDevidaEmCentesimos >= 100;
    if (estado.faltouLenhaHoje) {
        // Passo 4: o frio de hoje não volta; fica só o resto.
        estado.lenhaDevidaEmCentesimos %= 100;
        ++estado.diasEmDeficitDeLenha;
    } else {
        estado.diasEmDeficitDeLenha = 0;
    }
    return torasTiradas;
}
