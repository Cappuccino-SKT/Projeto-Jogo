#include "Companheiros/Companheiros.h"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "Cultura/Cultura.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/ProblemasDosDados.h"
#include "Reputacao/Reputacao.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/Populacao.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDosCompanheiros = "companheiros.json";
constexpr uint64_t sequenciaDoSorteioDosCompanheiros = 9;  // a partir do dia (nada a guardar no save)
constexpr int renomeDoJogador = 0;                         // CP1-2: até o Objetivo e Progressão

SimulacaoDaVila* cidadeDoMapa(SimulacaoDaVila& vila, int numeroDoMapa)
{
    if (vila.numeroDoMapaNoMundo() == numeroDoMapa) {
        return &vila;
    }
    for (const std::unique_ptr<SimulacaoDaVila>& cidade : vila.outrasCidades) {
        if (cidade->numeroDoMapaDaCidade == numeroDoMapa) {
            return cidade.get();
        }
    }
    return nullptr;
}

// O dinheiro do jogador: o que o grupo leva e o que ficou na vila.
int64_t dinheiroDoJogador(const SimulacaoDaVila& vila)
{
    return vila.viagem.grupoDoJogador.dinheiroEmDecimos + vila.dinheiro.dinheiroDoJogador;
}

// Paga primeiro com o dinheiro do grupo, depois com o da vila; o valor vai para a classe Civil
// da cidade de origem (CP1-3: o dinheiro fica no mundo). Sem o valor todo, não paga nada.
bool pagarParaACidade(SimulacaoDaVila& vila, int64_t valorEmDecimos, int mapaDeOrigem)
{
    if (dinheiroDoJogador(vila) < valorEmDecimos) {
        return false;
    }
    int64_t& doGrupo = vila.viagem.grupoDoJogador.dinheiroEmDecimos;
    const int64_t peloGrupo = std::min(doGrupo, valorEmDecimos);
    doGrupo -= peloGrupo;
    vila.dinheiro.dinheiroDoJogador -= valorEmDecimos - peloGrupo;
    SimulacaoDaVila* cidade = cidadeDoMapa(vila, mapaDeOrigem);
    (cidade != nullptr ? cidade->dinheiro.dinheiroDaClasseCivil : vila.dinheiro.dinheiroDaClasseCivil) += valorEmDecimos;
    return true;
}

std::string escolherNomeDaCultura(const DadosDoJogo& dados, const std::string& cultura, bool ehMulher, SorteioComSemente& sorteio)
{
    const DefinicaoDaCultura* definicao = dados.culturas.procurar(cultura);
    const std::vector<std::string>* lista = definicao == nullptr ? nullptr : (ehMulher ? &definicao->nomesDeMulher : &definicao->nomesDeHomem);
    if (lista == nullptr || lista->empty()) {
        return "Viajante";
    }
    return (*lista)[static_cast<std::size_t>(sorteio.sortearNumeroInteiroEntre(0, static_cast<int>(lista->size()) - 1))];
}

// Regra 4.1, passo 1: o melhor que o Renome deixa, com 50% de cair um degrau.
int sortearAQualidade(const RegrasDosCompanheiros& regras, SorteioComSemente& sorteio)
{
    int melhor = 0;
    for (std::size_t posicao = 0; posicao < regras.qualidades.size(); ++posicao) {
        melhor = regras.qualidades[posicao].renomeMinimo <= renomeDoJogador ? static_cast<int>(posicao) : melhor;
    }
    return melhor > 0 && sorteio.sortearNumeroInteiroEntre(0, 1) == 1 ? melhor - 1 : melhor;
}

Companheiro criarRecrutavel(SimulacaoDaVila& vila, int numeroDoMapa, SorteioComSemente& sorteio)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDosCompanheiros& regras = dados.regrasDosCompanheiros;
    const RegrasDaFamilia& familia = dados.regrasDaFamilia;
    Companheiro novo;
    novo.numero = vila.companheiros.proximoNumero++;
    novo.cultura = culturaBaseDaCidade(dados, numeroDoMapa);
    novo.ehMulher = sorteio.sortearNumeroInteiroEntre(0, 1) == 1;
    novo.nome = escolherNomeDaCultura(dados, novo.cultura, novo.ehMulher, sorteio);
    novo.idade = sorteio.sortearNumeroInteiroEntre(dados.regras.populacao.idadeAdulto, std::max(dados.regras.populacao.idadeAdulto, 20));
    novo.foco = static_cast<FocoDoCompanheiro>(sorteio.sortearNumeroInteiroEntre(0, 2));
    novo.qualidade = sortearAQualidade(regras, sorteio);
    const auto traco = [&]() { return sorteio.sortearNumeroInteiroEntre(familia.tracoMinimo, familia.tracoMaximo); };
    novo.personalidade = {traco(), traco(), traco(), traco(), traco()};
    novo.carisma = traco();
    novo.corpoACorpo = traco();
    novo.especialista = traco();
    if (!regras.qualidades.empty()) {
        const QualidadeDoCompanheiro& qualidade = regras.qualidades[static_cast<std::size_t>(novo.qualidade)];
        const int principal = sorteio.sortearNumeroInteiroEntre(qualidade.habilidadeMinima, qualidade.habilidadeMaxima);
        (novo.foco == FocoDoCompanheiro::Combate ? novo.corpoACorpo : novo.foco == FocoDoCompanheiro::Comercio ? novo.carisma : novo.especialista) = principal;
        novo.salarioPorEstacaoEmDecimos = qualidade.salarioPorEstacaoEmMoedas * 10;
    }
    novo.mapaDeOrigem = numeroDoMapa;
    novo.mapaOndeEsta = numeroDoMapa;
    novo.opiniao = regras.opiniaoInicial;
    return novo;
}

// Regra 4.1, passo 1: os recrutáveis da estação em cada Taverna (do nível 2 em diante).
void gerarRecrutaveisDasTavernas(SimulacaoDaVila& vila, SorteioComSemente& sorteio)
{
    EstadoDosCompanheiros& estado = vila.companheiros;
    estado.recrutaveisPorMapa.assign(vila.dados.mundo.mapas.size(), {});
    const std::vector<int>& porNivel = vila.dados.regrasDosCompanheiros.recrutaveisPorNivelDaCidade;
    for (const std::unique_ptr<SimulacaoDaVila>& cidade : vila.outrasCidades) {
        const int nivel = cidade->vila.nivelDaCidade;
        const int quantos = nivel >= 1 && nivel <= static_cast<int>(porNivel.size()) ? porNivel[static_cast<std::size_t>(nivel - 1)] : 0;
        for (int vez = 0; vez < quantos; ++vez) {
            estado.recrutaveisPorMapa[static_cast<std::size_t>(cidade->numeroDoMapaDaCidade - 1)].push_back(criarRecrutavel(vila, cidade->numeroDoMapaDaCidade, sorteio));
        }
    }
}

int quantosNoGrupo(const EstadoDosCompanheiros& estado)
{
    return static_cast<int>(std::count_if(estado.vinculados.begin(), estado.vinculados.end(), [](const Companheiro& companheiro) { return companheiro.noGrupo; }));
}

// O companheiro sai (salário, Felicidade 0, deserção ou morte).
void tirarOCompanheiro(EstadoDosCompanheiros& estado, int numero)
{
    std::erase_if(estado.vinculados, [&](const Companheiro& companheiro) { return companheiro.numero == numero; });
    estado.numeroDoConjuge = estado.numeroDoConjuge == numero ? -1 : estado.numeroDoConjuge;
    estado.sucessorEscolhido = estado.sucessorEscolhido == numero ? -1 : estado.sucessorEscolhido;
}

// O filho que fez 5 anos vira um Controlado da família, com a habilidade do foco no topo da faixa.
Companheiro filhoAdulto(const SimulacaoDaVila& vila, const FilhoDoJogador& filho)
{
    const RegrasDosCompanheiros& regras = vila.dados.regrasDosCompanheiros;
    Companheiro adulto;
    adulto.numero = filho.numero;
    adulto.nome = filho.nome;
    adulto.ehMulher = filho.ehMulher;
    adulto.idade = filho.idade;
    adulto.cultura = culturaBaseDaCidade(vila.dados, vila.dados.mundo.mapaDaVila);
    adulto.foco = filho.focoDeEducacao;
    switch (filho.focoDeEducacao) {
    case FocoDoCompanheiro::Combate: adulto.corpoACorpo = regras.habilidadeDoFilhoEducado; break;
    case FocoDoCompanheiro::Comercio:
    case FocoDoCompanheiro::Governo: adulto.carisma = regras.habilidadeDoFilhoEducado; break;
    default: adulto.especialista = regras.habilidadeDoFilhoEducado; break;  // Ofício e Magia (até a Magia, D4)
    }
    adulto.salarioPorEstacaoEmDecimos = 0;
    adulto.mapaDeOrigem = vila.dados.mundo.mapaDaVila;
    adulto.mapaOndeEsta = vila.dados.mundo.mapaDaVila;
    adulto.ehDaFamilia = true;
    adulto.ehFilhoDoJogador = true;
    adulto.opiniao = 60;  // [Proposta] o filho começa fiel
    return adulto;
}

// O dia 1 de cada estação (não no primeiro dia da partida).
void rodarAEstacaoNova(SimulacaoDaVila& vila, SorteioComSemente& sorteio, std::vector<std::string>& linhas)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDosCompanheiros& regras = dados.regrasDosCompanheiros;
    const RegrasDaPopulacao& populacao = dados.regras.populacao;
    EstadoDosCompanheiros& estado = vila.companheiros;
    estado.mortosNaEstacao = 0;
    // A idade (na Primavera) e a velhice (População, 4.1).
    if (vila.relogio.estacaoAtual() == Estacao::Primavera) {
        for (Companheiro& companheiro : estado.vinculados) {
            ++companheiro.idade;
        }
        for (FilhoDoJogador& filho : estado.filhos) {
            ++filho.idade;
        }
    }
    std::vector<int> mortos;
    for (const Companheiro& companheiro : estado.vinculados) {
        if (sorteio.sortearNumeroInteiroEntre(0, 99) < chanceDeMorrerDeVelhice(companheiro.idade, false, populacao)) {
            mortos.push_back(companheiro.numero);
            linhas.push_back("Companheiros: " + companheiro.nome + " morreu de velhice aos " + std::to_string(companheiro.idade) + " anos.");
        }
    }
    for (const int numero : mortos) {
        tirarOCompanheiro(estado, numero);
        ++estado.mortosNaEstacao;
        for (Companheiro& outro : estado.vinculados) {
            mudarOpiniao(outro, regras.opiniaoCompanheiroMorto);
        }
    }
    // Os filhos que viraram adultos (4.8, passo 2).
    for (const FilhoDoJogador& filho : estado.filhos) {
        if (filho.idade >= populacao.idadeAdulto) {
            estado.vinculados.push_back(filhoAdulto(vila, filho));
            linhas.push_back("Companheiros: " + filho.nome + " ficou adulto e entra na família como Controlado.");
        }
    }
    std::erase_if(estado.filhos, [&](const FilhoDoJogador& filho) { return filho.idade >= populacao.idadeAdulto; });
    // O cônjuge: o parto e a gravidez nova (4.6, passo 2).
    if (Companheiro* conjuge = estado.procurar(estado.numeroDoConjuge)) {
        if (conjuge->estacoesDeGravidez > 0 && --conjuge->estacoesDeGravidez == 0) {
            FilhoDoJogador filho;
            filho.numero = estado.proximoNumero++;
            filho.ehMulher = sorteio.sortearNumeroInteiroEntre(0, 1) == 1;
            filho.nome = escolherNomeDaCultura(dados, culturaBaseDaCidade(dados, dados.mundo.mapaDaVila), filho.ehMulher, sorteio);
            estado.filhos.push_back(filho);
            linhas.push_back("Companheiros: nasceu " + filho.nome + ", " + (filho.ehMulher ? "sua filha." : "seu filho."));
        } else if (conjuge->estacoesDeGravidez == 0 && conjuge->idade >= populacao.idadeAdulto && conjuge->idade <= dados.regrasDaFamilia.fertilAte
                   && sorteio.sortearNumeroInteiroEntre(0, 99) < dados.regrasDaFamilia.chanceDeFilhoPorEstacaoEmPorcento) {
            conjuge->estacoesDeGravidez = dados.regrasDaFamilia.gravidezEmEstacoes;
            linhas.push_back("Companheiros: vem aí um filho seu com " + conjuge->nome + ".");
        }
    }
    // O aumento pedido na estação passada e não respondido conta como recusado (4.3).
    for (Companheiro& companheiro : estado.vinculados) {
        if (companheiro.pediuAumento) {
            companheiro.pediuAumento = false;
            mudarOpiniao(companheiro, regras.opiniaoAumentoRecusado);
        }
    }
    pagarVinculados(vila, linhas);
    // A Opinião de −20 a −59: pede +25% de salário (4.3).
    for (Companheiro& companheiro : estado.vinculados) {
        if (!companheiro.ehDaFamilia && companheiro.opiniao <= -20 && companheiro.opiniao > -60) {
            companheiro.pediuAumento = true;
            linhas.push_back("Companheiros: " + companheiro.nome + " pede " + std::to_string(regras.aumentoPedidoEmPorcento) + "% a mais de salário.");
        }
    }
    gerarRecrutaveisDasTavernas(vila, sorteio);
}

}  // namespace

std::string nomeDoFoco(FocoDoCompanheiro foco)
{
    switch (foco) {
    case FocoDoCompanheiro::Combate: return "Combate";
    case FocoDoCompanheiro::Comercio: return "Comércio";
    case FocoDoCompanheiro::Oficio: return "Ofício";
    case FocoDoCompanheiro::Magia: return "Magia";
    case FocoDoCompanheiro::Governo: return "Governo";
    }
    return "?";
}

const Companheiro* EstadoDosCompanheiros::procurar(int numero) const
{
    for (const Companheiro& companheiro : vinculados) {
        if (companheiro.numero == numero) {
            return &companheiro;
        }
    }
    return nullptr;
}

Companheiro* EstadoDosCompanheiros::procurar(int numero)
{
    for (Companheiro& companheiro : vinculados) {
        if (companheiro.numero == numero) {
            return &companheiro;
        }
    }
    return nullptr;
}

RegrasDosCompanheiros lerRegrasDosCompanheiros(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDosCompanheiros regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error&) {
        return regras;  // o erro já foi anotado por lerArquivoDosCompanheiros
    }
    const Json grupo = documento.value("grupo_controlado", Json::object());
    regras.grupoBase = grupo.value("base", regras.grupoBase);
    regras.grupoRenomePorUm = std::max(1, grupo.value("renome_por_um", regras.grupoRenomePorUm));
    const Json vinculados = documento.value("vinculados", Json::object());
    regras.vinculadosBase = vinculados.value("base", regras.vinculadosBase);
    regras.vinculadosRenomePorUm = std::max(1, vinculados.value("renome_por_um", regras.vinculadosRenomePorUm));
    regras.recrutaveisPorNivelDaCidade = documento.value("recrutaveis_por_nivel_da_cidade", regras.recrutaveisPorNivelDaCidade);
    for (const char* id : {"comum", "bom", "excelente"}) {
        const Json objeto = documento.value("qualidades", Json::object()).value(id, Json::object());
        if (objeto.empty()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDosCompanheiros, 0, std::string("falta a qualidade \"") + id + "\" em \"qualidades\"");
            continue;
        }
        QualidadeDoCompanheiro qualidade;
        qualidade.id = id;
        qualidade.renomeMinimo = objeto.value("renome_minimo", 0);
        const std::vector<int> faixa = objeto.value("habilidade_principal", std::vector<int>{20, 50});
        if (faixa.size() != 2 || faixa[0] > faixa[1]) {
            listaDeProblemas.anotarErro(nomeDoArquivoDosCompanheiros, 0, std::string(id) + ": \"habilidade_principal\" precisa ser [mínimo, máximo]");
            continue;
        }
        qualidade.habilidadeMinima = faixa[0];
        qualidade.habilidadeMaxima = faixa[1];
        qualidade.entradaEmMoedas = objeto.value("entrada", qualidade.entradaEmMoedas);
        qualidade.salarioPorEstacaoEmMoedas = objeto.value("salario_por_estacao", qualidade.salarioPorEstacaoEmMoedas);
        regras.qualidades.push_back(qualidade);
    }
    regras.estacoesSemSalarioParaSair = documento.value("estacoes_sem_salario_para_sair", regras.estacoesSemSalarioParaSair);
    const Json opiniao = documento.value("opiniao", Json::object());
    regras.opiniaoInicial = opiniao.value("inicial", regras.opiniaoInicial);
    regras.opiniaoVitoriaJunto = opiniao.value("vitoria_junto", regras.opiniaoVitoriaJunto);
    regras.opiniaoSalarioEmDia = opiniao.value("salario_em_dia", regras.opiniaoSalarioEmDia);
    regras.opiniaoSalarioAtrasado = opiniao.value("salario_atrasado", regras.opiniaoSalarioAtrasado);
    regras.opiniaoCompanheiroMorto = opiniao.value("companheiro_morto", regras.opiniaoCompanheiroMorto);
    regras.opiniaoPresentePorMoedas = std::max(1, opiniao.value("presente_por_moedas", regras.opiniaoPresentePorMoedas));
    regras.opiniaoPresenteMaximo = opiniao.value("presente_maximo", regras.opiniaoPresenteMaximo);
    regras.opiniaoAumentoRecusado = opiniao.value("aumento_recusado", regras.opiniaoAumentoRecusado);
    regras.aumentoPedidoEmPorcento = opiniao.value("aumento_pedido_em_porcento", regras.aumentoPedidoEmPorcento);
    const Json felicidade = documento.value("felicidade_do_controlado", Json::object());
    regras.felicidadeSalarioEmDia = felicidade.value("salario_em_dia", regras.felicidadeSalarioEmDia);
    regras.felicidadeSalarioAtrasado = felicidade.value("salario_atrasado", regras.felicidadeSalarioAtrasado);
    regras.felicidadeSemComida = felicidade.value("sem_comida", regras.felicidadeSemComida);
    regras.felicidadePorCompanheiroMorto = felicidade.value("por_companheiro_morto_na_estacao", regras.felicidadePorCompanheiroMorto);
    regras.felicidadeMinimoDosMortos = felicidade.value("minimo_mortos", regras.felicidadeMinimoDosMortos);
    const Json dialogo = documento.value("dialogo", Json::object());
    regras.dialogoBase = dialogo.value("base", regras.dialogoBase);
    regras.dialogoHabilidadeDivide = std::max(1, dialogo.value("habilidade_divide", regras.dialogoHabilidadeDivide));
    regras.dialogoOpiniaoDivide = std::max(1, dialogo.value("opiniao_divide", regras.dialogoOpiniaoDivide));
    regras.dialogoTracoDivide = std::max(1, dialogo.value("traco_divide", regras.dialogoTracoDivide));
    const std::vector<int> limites = dialogo.value("limites", std::vector<int>{regras.dialogoMinimo, regras.dialogoMaximo});
    if (limites.size() == 2 && limites[0] <= limites[1]) {
        regras.dialogoMinimo = limites[0];
        regras.dialogoMaximo = limites[1];
    }
    regras.falhaPersuadir = dialogo.value("falha_persuadir", regras.falhaPersuadir);
    regras.falhaIntimidar = dialogo.value("falha_intimidar", regras.falhaIntimidar);
    regras.habilidadeDoFilhoEducado = documento.value("familia", Json::object()).value("habilidade_do_filho_educado", regras.habilidadeDoFilhoEducado);
    return regras;
}

void pagarVinculados(SimulacaoDaVila& vila, std::vector<std::string>& linhas)
{
    const RegrasDosCompanheiros& regras = vila.dados.regrasDosCompanheiros;
    EstadoDosCompanheiros& estado = vila.companheiros;
    std::vector<int> saem;
    for (Companheiro& companheiro : estado.vinculados) {
        if (companheiro.ehDaFamilia) {
            continue;
        }
        if (pagarParaACidade(vila, companheiro.salarioPorEstacaoEmDecimos, companheiro.mapaDeOrigem)) {
            companheiro.estacoesSemSalario = 0;
            companheiro.salarioAtrasado = false;
            mudarOpiniao(companheiro, regras.opiniaoSalarioEmDia);
            continue;
        }
        ++companheiro.estacoesSemSalario;
        companheiro.salarioAtrasado = true;
        mudarOpiniao(companheiro, regras.opiniaoSalarioAtrasado);
        if (companheiro.estacoesSemSalario >= regras.estacoesSemSalarioParaSair) {
            saem.push_back(companheiro.numero);
            linhas.push_back("Companheiros: " + companheiro.nome + " foi embora: " + std::to_string(companheiro.estacoesSemSalario) + " estações sem salário.");
        } else {
            linhas.push_back("Aviso: faltou dinheiro para o salário de " + companheiro.nome + " (Opinião " + std::to_string(companheiro.opiniao) + ").");
        }
    }
    for (const int numero : saem) {
        tirarOCompanheiro(estado, numero);
    }
}

int limiteDoGrupo(const RegrasDosCompanheiros& regras, int renome)
{
    return regras.grupoBase + std::max(0, renome) / regras.grupoRenomePorUm;
}

int limiteDosVinculados(const RegrasDosCompanheiros& regras, int renome)
{
    return regras.vinculadosBase + std::max(0, renome) / regras.vinculadosRenomePorUm;
}

int vinculadosQueContam(const EstadoDosCompanheiros& estado)
{
    return static_cast<int>(std::count_if(estado.vinculados.begin(), estado.vinculados.end(), [](const Companheiro& companheiro) { return !companheiro.ehDaFamilia; }));
}

void mudarOpiniao(Companheiro& companheiro, int valor)
{
    const int mudanca = valor < 0 ? valor * (150 - companheiro.personalidade.lealdade) / 100 : valor;
    companheiro.opiniao = std::clamp(companheiro.opiniao + mudanca, -100, 100);
}

int felicidadeAlvoDoControlado(const Companheiro& companheiro, bool grupoSemComida, int mortosNaEstacao, const RegrasDosCompanheiros& regras)
{
    int alvo = 50;
    if (!companheiro.ehDaFamilia) {
        alvo += companheiro.salarioAtrasado ? regras.felicidadeSalarioAtrasado : regras.felicidadeSalarioEmDia;
    }
    alvo += companheiro.noGrupo && grupoSemComida ? regras.felicidadeSemComida : 0;
    alvo += std::max(regras.felicidadeMinimoDosMortos, mortosNaEstacao * regras.felicidadePorCompanheiroMorto);
    return std::clamp(alvo, 0, 100);
}

int passoDaFelicidade(int felicidade, int alvo, int opiniao)
{
    if (felicidade == alvo) {
        return felicidade;
    }
    int passo = (alvo - felicidade) / 10;
    if (alvo < felicidade && opiniao > -20 && opiniao < 20) {
        passo = passo * 125 / 100;
    }
    if (passo == 0) {
        passo = alvo > felicidade ? 1 : -1;
    }
    return std::clamp(felicidade + passo, 0, 100);
}

int chanceDaAcaoDeDialogo(AcaoDeDialogo acao, int habilidadeDoJogador, int opiniao, const Personalidade& personalidade, const RegrasDosCompanheiros& regras)
{
    // O traço e o sinal (4.9, passo 1; CP1-7): Persuadir e Barganhar, −Ganância; Intimidar, −Coragem; Subornar, +Ganância.
    int traco = 0;
    switch (acao) {
    case AcaoDeDialogo::Persuadir:
    case AcaoDeDialogo::Barganhar: traco = -(personalidade.ganancia - 50); break;
    case AcaoDeDialogo::Intimidar: traco = -(personalidade.coragem - 50); break;
    case AcaoDeDialogo::Subornar: traco = personalidade.ganancia - 50; break;
    }
    const int chance = regras.dialogoBase + (habilidadeDoJogador - 50) / regras.dialogoHabilidadeDivide + opiniao / regras.dialogoOpiniaoDivide + traco / regras.dialogoTracoDivide;
    return std::clamp(chance, regras.dialogoMinimo, regras.dialogoMaximo);
}

int opiniaoDoPresente(int moedas, int ganancia, const RegrasDosCompanheiros& regras)
{
    return std::clamp(moedas / regras.opiniaoPresentePorMoedas * (100 + ganancia - 50) / 100, 0, regras.opiniaoPresenteMaximo);
}

int escolherSucessorDoJogador(const EstadoDosCompanheiros& estado)
{
    const Companheiro* escolhido = nullptr;
    for (const Companheiro& companheiro : estado.vinculados) {
        if (companheiro.ehFilhoDoJogador && (escolhido == nullptr || companheiro.idade > escolhido->idade)) {
            escolhido = &companheiro;
        }
    }
    if (escolhido != nullptr) {
        return escolhido->numero;
    }
    if (const Companheiro* preferido = estado.procurar(estado.sucessorEscolhido); preferido != nullptr && preferido->opiniao >= 60) {
        return preferido->numero;
    }
    for (const Companheiro& companheiro : estado.vinculados) {
        if (escolhido == nullptr || companheiro.opiniao > escolhido->opiniao) {
            escolhido = &companheiro;
        }
    }
    return escolhido == nullptr ? -1 : escolhido->numero;
}

void montarOsCompanheirosDaPartidaNova(SimulacaoDaVila& vila)
{
    vila.companheiros = EstadoDosCompanheiros{};
    SorteioComSemente sorteio(vila.vila.sementeDaPartida, sequenciaDoSorteioDosCompanheiros);
    gerarRecrutaveisDasTavernas(vila, sorteio);
}

std::vector<std::string> rodarOsCompanheirosNoDiaNovo(SimulacaoDaVila& vila)
{
    std::vector<std::string> linhas;
    const int64_t hoje = vila.relogio.diaDaPartida();
    SorteioComSemente sorteio(vila.vila.sementeDaPartida + static_cast<uint64_t>(hoje) * 16, sequenciaDoSorteioDosCompanheiros);
    if (hoje > 0 && vila.relogio.diaDaEstacaoAtual() == 1) {
        rodarAEstacaoNova(vila, sorteio, linhas);
    }
    // Todo dia: a Felicidade (4.4), quem sai com Felicidade 0 e quem deserta (4.3).
    const RegrasDosCompanheiros& regras = vila.dados.regrasDosCompanheiros;
    EstadoDosCompanheiros& estado = vila.companheiros;
    std::vector<int> saem;
    for (Companheiro& companheiro : estado.vinculados) {
        const int alvo = felicidadeAlvoDoControlado(companheiro, vila.viagem.grupoSemComida, estado.mortosNaEstacao, regras);
        companheiro.felicidade = passoDaFelicidade(companheiro.felicidade, alvo, companheiro.opiniao);
        if (companheiro.ehDaFamilia) {
            continue;
        }
        if (companheiro.felicidade == 0) {
            // A Opinião +60 segura mais 1 estação.
            ++companheiro.diasComFelicidadeZero;
            if (companheiro.opiniao < 60 || companheiro.diasComFelicidadeZero > vila.relogio.diasPorEstacao()) {
                saem.push_back(companheiro.numero);
                linhas.push_back("Companheiros: " + companheiro.nome + " foi embora, infeliz.");
            }
            continue;
        }
        companheiro.diasComFelicidadeZero = 0;
        if (companheiro.opiniao <= -60 && sorteio.sortearNumeroInteiroEntre(0, 99) < 25) {
            saem.push_back(companheiro.numero);
            linhas.push_back("Companheiros: " + companheiro.nome + " desertou à noite, com o equipamento.");
        }
    }
    for (const int numero : saem) {
        tirarOCompanheiro(estado, numero);
    }
    atualizarOGrupoPelosCompanheiros(vila);
    return linhas;
}

void atualizarOGrupoPelosCompanheiros(SimulacaoDaVila& vila)
{
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    grupo.lutadoresDoGrupo.clear();
    for (const Companheiro& companheiro : vila.companheiros.vinculados) {
        if (companheiro.noGrupo) {
            grupo.lutadoresDoGrupo.push_back({companheiro.foco == FocoDoCompanheiro::Combate ? "guarda" : "civil", companheiro.corpoACorpo * 100,
                                               companheiro.personalidade.coragem});
        }
    }
    grupo.pessoas = 1 + static_cast<int>(grupo.lutadoresDoGrupo.size());
}

void anotarAVitoriaDoGrupo(SimulacaoDaVila& vila)
{
    for (Companheiro& companheiro : vila.companheiros.vinculados) {
        if (companheiro.noGrupo) {
            mudarOpiniao(companheiro, vila.dados.regrasDosCompanheiros.opiniaoVitoriaJunto);
        }
    }
}

std::string fazerAcaoComOCompanheiro(SimulacaoDaVila& vila, AcaoComOCompanheiro acao, int numero, int valor, std::vector<std::string>& linhas)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDosCompanheiros& regras = dados.regrasDosCompanheiros;
    EstadoDosCompanheiros& estado = vila.companheiros;
    const EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    const bool grupoParado = !grupo.emTransito;

    if (acao == AcaoComOCompanheiro::Recrutar) {
        if (!grupoParado || grupo.mapaAtual < 1 || grupo.mapaAtual > static_cast<int>(estado.recrutaveisPorMapa.size())) {
            return "o grupo precisa estar parado numa cidade com Taverna";
        }
        std::vector<Companheiro>& naTaverna = estado.recrutaveisPorMapa[static_cast<std::size_t>(grupo.mapaAtual - 1)];
        const auto recrutavel = std::find_if(naTaverna.begin(), naTaverna.end(), [&](const Companheiro& companheiro) { return companheiro.numero == numero; });
        if (recrutavel == naTaverna.end()) {
            return "esse recrutável não está nesta Taverna";
        }
        if (vinculadosQueContam(estado) >= limiteDosVinculados(regras, renomeDoJogador)) {
            return "o limite de " + std::to_string(limiteDosVinculados(regras, renomeDoJogador)) + " vinculados já foi atingido";
        }
        const int entrada = regras.qualidades.empty() ? 0 : regras.qualidades[static_cast<std::size_t>(recrutavel->qualidade)].entradaEmMoedas * 10;
        if (!pagarParaACidade(vila, entrada, recrutavel->mapaDeOrigem)) {
            return "falta dinheiro para a entrada (" + std::to_string(entrada / 10) + " moedas)";
        }
        Companheiro novo = *recrutavel;
        naTaverna.erase(recrutavel);
        novo.noGrupo = quantosNoGrupo(estado) < limiteDoGrupo(regras, renomeDoJogador);
        novo.mapaOndeEsta = grupo.mapaAtual;
        linhas.push_back("Companheiros: " + novo.nome + " foi recrutado (" + nomeDoFoco(novo.foco) + ", " + std::to_string(novo.salarioPorEstacaoEmDecimos / 10)
                         + " moedas por estação)" + (novo.noGrupo ? "." : "; o grupo está cheio e ele espera aqui."));
        estado.vinculados.push_back(std::move(novo));
        atualizarOGrupoPelosCompanheiros(vila);
        return "";
    }
    if (acao == AcaoComOCompanheiro::EscolherOFocoDoFilho) {
        for (FilhoDoJogador& filho : estado.filhos) {
            if (filho.numero == numero) {
                filho.focoDeEducacao = static_cast<FocoDoCompanheiro>(std::clamp(valor, 0, 4));
                return "";
            }
        }
        return "esse filho não existe";
    }
    Companheiro* companheiro = estado.procurar(numero);
    if (companheiro == nullptr) {
        return "esse companheiro não está com você";
    }
    const bool juntoDoGrupo = companheiro->noGrupo || (grupoParado && companheiro->mapaOndeEsta == grupo.mapaAtual);
    switch (acao) {
    case AcaoComOCompanheiro::ChamarParaOGrupo:
        if (companheiro->noGrupo || !juntoDoGrupo) {
            return "ele precisa estar na cidade onde o grupo está";
        }
        if (quantosNoGrupo(estado) >= limiteDoGrupo(regras, renomeDoJogador)) {
            return "o grupo já tem " + std::to_string(limiteDoGrupo(regras, renomeDoJogador)) + " companheiros";
        }
        companheiro->noGrupo = true;
        break;
    case AcaoComOCompanheiro::DeixarNaCidade:
        if (!companheiro->noGrupo || !grupoParado) {
            return "o grupo precisa estar parado";
        }
        companheiro->noGrupo = false;
        companheiro->mapaOndeEsta = grupo.mapaAtual;
        break;
    case AcaoComOCompanheiro::Dispensar:
        if (companheiro->ehDaFamilia) {
            return "a família não é dispensada";
        }
        linhas.push_back("Companheiros: " + companheiro->nome + " foi dispensado.");
        tirarOCompanheiro(estado, numero);
        break;
    case AcaoComOCompanheiro::Presentear: {
        if (!juntoDoGrupo || valor <= 0) {
            return "ele precisa estar junto do grupo";
        }
        const int mapaDeOrigem = companheiro->mapaDeOrigem;
        if (!pagarParaACidade(vila, static_cast<int64_t>(valor) * 10, mapaDeOrigem)) {
            return "falta dinheiro para o presente";
        }
        const int pontos = opiniaoDoPresente(valor, companheiro->personalidade.ganancia, regras);
        mudarOpiniao(*companheiro, pontos);
        linhas.push_back("Companheiros: " + companheiro->nome + " gostou do presente (Opinião +" + std::to_string(pontos) + ", agora " + std::to_string(companheiro->opiniao) + ").");
        break;
    }
    case AcaoComOCompanheiro::PedirEmCasamento: {
        // 4.8, passo 1: Opinião +60. O costume da cultura (dote, duelo, cerimônia...) [Refinar depois].
        if (estado.numeroDoConjuge >= 0 || companheiro->ehDaFamilia) {
            return "você já é casado, ou ele é da família";
        }
        if (companheiro->opiniao < 60) {
            return "a Opinião dele precisa ser +60 (hoje " + std::to_string(companheiro->opiniao) + ")";
        }
        companheiro->ehDaFamilia = true;
        companheiro->pediuAumento = false;
        estado.numeroDoConjuge = numero;
        linhas.push_back("Companheiros: você se casou com " + companheiro->nome + ".");
        const std::string faixa = mudarReputacao(vila, donoDaCidadeDoMapa(dados.mundo, companheiro->mapaDeOrigem), 20, "o casamento com " + companheiro->nome);
        if (!faixa.empty()) {
            linhas.push_back(faixa);
        }
        break;
    }
    case AcaoComOCompanheiro::AceitarOAumento:
        if (!companheiro->pediuAumento) {
            return "ele não pediu aumento";
        }
        companheiro->pediuAumento = false;
        companheiro->salarioPorEstacaoEmDecimos = companheiro->salarioPorEstacaoEmDecimos * (100 + regras.aumentoPedidoEmPorcento) / 100;
        break;
    case AcaoComOCompanheiro::RecusarOAumento:
        if (!companheiro->pediuAumento) {
            return "ele não pediu aumento";
        }
        companheiro->pediuAumento = false;
        mudarOpiniao(*companheiro, regras.opiniaoAumentoRecusado);
        break;
    case AcaoComOCompanheiro::EscolherOSucessor:
        if (companheiro->opiniao < 60) {
            return "o sucessor precisa de Opinião +60";
        }
        estado.sucessorEscolhido = numero;
        break;
    default:
        break;
    }
    atualizarOGrupoPelosCompanheiros(vila);
    return "";
}
