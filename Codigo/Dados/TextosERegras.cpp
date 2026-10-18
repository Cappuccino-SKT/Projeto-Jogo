#include "Dados/TextosERegras.h"

#include <vector>

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

// Lê uma lista de inteiros de tamanho certo, cada um entre o mínimo e o máximo.
bool lerListaDeInteiros(const Json& objeto, const char* nomeDoCampo, std::size_t tamanhoDaLista, int valorMinimo, int valorMaximo,
                        int* valoresLidos, const std::string& ondeEsta, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    bool listaCerta = campo != objeto.end() && campo->is_array() && campo->size() == tamanhoDaLista;
    for (std::size_t posicao = 0; listaCerta && posicao < tamanhoDaLista; ++posicao) {
        const Json& valor = (*campo)[posicao];
        listaCerta = valor.is_number_integer() && valor.get<int>() >= valorMinimo && valor.get<int>() <= valorMaximo;
        if (listaCerta) {
            valoresLidos[posicao] = valor.get<int>();
        }
    }
    if (!listaCerta) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"" + std::string(nomeDoCampo) + "\" em " + ondeEsta + " precisa de "
                                    + std::to_string(tamanhoDaLista) + " números inteiros de " + std::to_string(valorMinimo) + " a "
                                    + std::to_string(valorMaximo));
    }
    return listaCerta;
}

// O marco 5 (ficha da IA, FL5-3): "rotinas", "utilidade" e "passeio".
void lerRegrasDaVidaDosNpcs(const Json& documento, RegrasDaVidaDosNpcs& vida, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto rotinas = documento.find("rotinas");
    const bool temRotinaCivil = rotinas != documento.end() && rotinas->is_object() && rotinas->contains("civil") && (*rotinas)["civil"].is_object();
    if (!temRotinaCivil) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"rotinas\" com a rotina \"civil\"");
    } else {
        const Json& civil = (*rotinas)["civil"];
        int horasDeDormir[2] = {22, 6};
        int horasDeTrabalhar[2] = {7, 15};
        if (lerListaDeInteiros(civil, "dormir", 2, 0, 23, horasDeDormir, "rotinas.civil", listaDeProblemas)) {
            vida.rotinaDoCivil.horaQueComecaADormir = horasDeDormir[0];
            vida.rotinaDoCivil.horaQueAcabaDeDormir = horasDeDormir[1];
        }
        if (lerListaDeInteiros(civil, "trabalhar", 2, 0, 23, horasDeTrabalhar, "rotinas.civil", listaDeProblemas)) {
            vida.rotinaDoCivil.horaQueComecaATrabalhar = horasDeTrabalhar[0];
            vida.rotinaDoCivil.horaQueAcabaDeTrabalhar = horasDeTrabalhar[1];
        }
    }

    const auto utilidade = documento.find("utilidade");
    if (utilidade == documento.end() || !utilidade->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"utilidade\"");
    } else {
        const std::string onde = "utilidade";
        RegrasDaUtilidade& notas = vida.utilidade;
        lerInteiroObrigatorio(*utilidade, "nota_da_acao_do_periodo", notas.notaDaAcaoDoPeriodo, onde, listaDeProblemas);
        lerInteiroObrigatorio(*utilidade, "bonus_de_comer", notas.bonusDeComer, onde, listaDeProblemas);
        lerListaDeInteiros(*utilidade, "horas_das_refeicoes", 3, 0, 23, notas.horasDasRefeicoes, onde, listaDeProblemas);
        lerInteiroObrigatorio(*utilidade, "minutos_da_refeicao", notas.minutosDaRefeicao, onde, listaDeProblemas);
        lerInteiroObrigatorio(*utilidade, "bonus_de_comprar", notas.bonusDeComprar, onde, listaDeProblemas);
        lerInteiroObrigatorio(*utilidade, "dias_entre_compras", notas.diasEntreCompras, onde, listaDeProblemas);
        lerInteiroObrigatorio(*utilidade, "minutos_da_compra", notas.minutosDaCompra, onde, listaDeProblemas);
        lerInteiroObrigatorio(*utilidade, "minutos_entre_reavaliacoes", notas.minutosEntreReavaliacoes, onde, listaDeProblemas);
        if (notas.diasEntreCompras < 1 || notas.minutosEntreReavaliacoes < 1 || notas.minutosDaRefeicao < 1 || notas.minutosDaCompra < 1) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "os dias e os minutos da \"utilidade\" precisam ser maiores que zero");
        }
    }

    const auto passeio = documento.find("passeio");
    if (passeio == documento.end() || !passeio->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"passeio\"");
    } else {
        lerInteiroObrigatorio(*passeio, "distancia_maxima_em_tiles", vida.passeio.distanciaMaximaEmTiles, "passeio", listaDeProblemas);
        int minutosDeEspera[2] = {15, 45};
        if (lerListaDeInteiros(*passeio, "minutos_de_espera", 2, 1, 24 * 60, minutosDeEspera, "passeio", listaDeProblemas)) {
            vida.passeio.menorEsperaEmMinutos = minutosDeEspera[0];
            vida.passeio.maiorEsperaEmMinutos = minutosDeEspera[1];
            if (minutosDeEspera[0] > minutosDeEspera[1]) {
                listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"minutos_de_espera\" do passeio: o primeiro precisa ser o menor");
            }
        }
    }
}

// O marco 6 (ficha da Construção): as vagas, a capacidade e o custo das obras.
void lerRegrasDasConstrucoes(const Json& documento, RegrasDasConstrucoes& construcoes, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto vagas = documento.find("vagas_por_nivel");
    bool vagasCertas = vagas != documento.end() && vagas->is_array() && !vagas->empty();
    std::vector<int> vagasLidas;
    for (std::size_t nivel = 0; vagasCertas && nivel < vagas->size(); ++nivel) {
        const Json& valor = (*vagas)[nivel];
        vagasCertas = valor.is_number_integer() && valor.get<int>() > 0 && (vagasLidas.empty() || valor.get<int>() >= vagasLidas.back());
        if (vagasCertas) {
            vagasLidas.push_back(valor.get<int>());
        }
    }
    if (vagasCertas) {
        construcoes.vagasPorNivel = vagasLidas;
    } else {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"vagas_por_nivel\" precisa de uma lista de inteiros maiores que 0, um por nível, sem diminuir");
    }

    const char* const nomesDosPortes[3] = {"pequeno", "medio", "grande"};
    const auto capacidade = documento.find("capacidade_por_nivel");
    if (capacidade == documento.end() || !capacidade->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"capacidade_por_nivel\" (pequeno, medio e grande)");
    } else {
        for (int porte = 0; porte < 3; ++porte) {
            int& capacidadeDoPorte = construcoes.capacidadePorNivelDeCadaPorte[porte];
            if (lerInteiroObrigatorio(*capacidade, nomesDosPortes[porte], capacidadeDoPorte, "capacidade_por_nivel", listaDeProblemas)
                && capacidadeDoPorte < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "a capacidade não pode ser negativa");
            }
        }
    }

    const auto obras = documento.find("obra_por_porte");
    if (obras == documento.end() || !obras->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"obra_por_porte\" (pequeno, medio e grande)");
        return;
    }
    for (int porte = 0; porte < 3; ++porte) {
        const auto obra = obras->find(nomesDosPortes[porte]);
        const std::string onde = std::string("obra_por_porte.") + nomesDosPortes[porte];
        if (obra == obras->end() || !obra->is_object()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta " + onde);
            continue;
        }
        CustoDaObra& custo = construcoes.obraDeCadaPorte[porte];
        lerInteiroObrigatorio(*obra, "tabua", custo.tabuas, onde, listaDeProblemas);
        lerInteiroObrigatorio(*obra, "bloco_de_pedra", custo.blocosDePedra, onde, listaDeProblemas);
        lerInteiroObrigatorio(*obra, "barra_de_ferro", custo.barrasDeFerro, onde, listaDeProblemas);
        lerInteiroObrigatorio(*obra, "horas", custo.horasDeTrabalho, onde, listaDeProblemas);
        if (custo.tabuas < 0 || custo.blocosDePedra < 0 || custo.barrasDeFerro < 0 || custo.horasDeTrabalho <= 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, onde + ": os itens não podem ser negativos, e as horas precisam ser maiores que 0");
        }
    }
}

// O marco 6 (ficha da Economia): a cesta mínima, a loja da cidade, os salários e a ordem do dia.
void lerRegrasDaEconomia(const Json& documento, RegrasDaEconomia& economia, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto cesta = documento.find("cesta_minima_civil");
    if (cesta == documento.end() || !cesta->is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta \"cesta_minima_civil\"");
    } else {
        lerInteiroObrigatorio(*cesta, "comida_por_dia", economia.comidaPorDiaDoCivil, "cesta_minima_civil", listaDeProblemas);
        lerInteiroObrigatorio(*cesta, "materiais_a_cada_dias", economia.diasEntreMateriaisDoCivil, "cesta_minima_civil", listaDeProblemas);
        if (economia.comidaPorDiaDoCivil < 0 || economia.diasEntreMateriaisDoCivil < 1) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"cesta_minima_civil\": a comida não pode ser negativa, e os dias precisam ser 1 ou mais");
        }
    }
    const std::string onde = "regras.json";
    lerInteiroObrigatorio(documento, "loja_da_cidade_vende_em_porcento", economia.lojaDaCidadeVendeEmPorcento, onde, listaDeProblemas);
    lerInteiroObrigatorio(documento, "bonus_do_salario_por_nivel_em_porcento", economia.bonusDoSalarioPorNivelEmPorcento, onde, listaDeProblemas);
    lerInteiroObrigatorio(documento, "salario_do_gerente_em_porcento", economia.salarioDoGerenteEmPorcento, onde, listaDeProblemas);
    if (economia.lojaDaCidadeVendeEmPorcento < 1 || economia.bonusDoSalarioPorNivelEmPorcento < 0 || economia.salarioDoGerenteEmPorcento < 1) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "as porcentagens da loja, do bônus de nível e do gerente precisam ser positivas");
    }

    // A ordem do dia (FT1-5): um nome desconhecido é erro.
    const auto ordem = documento.find("ordem_do_dia");
    if (ordem == documento.end() || !ordem->is_array()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "falta a lista \"ordem_do_dia\"");
        return;
    }
    economia.ordemDoDia.clear();
    for (const Json& passo : *ordem) {
        const std::string nomeDoPasso = passo.is_string() ? passo.get<std::string>() : passo.dump();
        if (nomeDoPasso == "consumo") {
            economia.ordemDoDia.push_back(PassoDaOrdemDoDia::Consumo);
        } else if (nomeDoPasso == "salarios") {
            economia.ordemDoDia.push_back(PassoDaOrdemDoDia::Salarios);
        } else if (nomeDoPasso == "escolha_das_receitas") {
            economia.ordemDoDia.push_back(PassoDaOrdemDoDia::EscolhaDasReceitas);
        } else {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "o passo \"" + nomeDoPasso + "\" da \"ordem_do_dia\" não existe "
                                        "(os do marco 6: consumo, salarios, escolha_das_receitas)");
        }
    }
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

    lerRegrasDaVidaDosNpcs(documento, regras.vidaDosNpcs, listaDeProblemas);
    lerRegrasDasConstrucoes(documento, regras.construcoes, listaDeProblemas);
    lerRegrasDaEconomia(documento, regras.economia, listaDeProblemas);
    return regras;
}
