#include "Crime/Crime.h"

#include <algorithm>
#include <optional>

#include <nlohmann/json.hpp>

#include "Comercio/Comercio.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/ProblemasDosDados.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDosCrimes = "crimes.json";
const char* const identificadorDoLivroSagrado = "item.livro_sagrado";

SimulacaoDaVila* cidadeDoMapa(SimulacaoDaVila& vila, int numeroDoMapa)
{
    for (SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        if (cidade->numeroDoMapaNoMundo() == numeroDoMapa) {
            return cidade;
        }
    }
    return nullptr;
}

std::string nomeDoCrime(const std::string& id)
{
    if (id == "crime.contrabando") {
        return "Contrabando";
    }
    if (id == "crime.heresia_sem_livro") {
        return "Heresia (sem o Livro Sagrado)";
    }
    return id;
}

std::string opcaoDaLei(const SimulacaoDaVila& vila, int faccao, const char* lei)
{
    const int posicao = vila.dados.faccoes.posicaoDaLei(lei);
    if (faccao < 0 || posicao < 0 || faccao >= static_cast<int>(vila.faccoes.leisAtuais.size())) {
        return "";
    }
    return vila.dados.faccoes.leis[static_cast<std::size_t>(posicao)].opcoes[static_cast<std::size_t>(vila.faccoes.leisAtuais[static_cast<std::size_t>(faccao)][static_cast<std::size_t>(posicao)])];
}

// O confisco (4.4 e 4.6): os itens proibidos pela facção saem do grupo.
int confiscarOsItensProibidos(SimulacaoDaVila& vila, int faccao)
{
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    int confiscados = 0;
    for (int item = 0; item < static_cast<int>(grupo.quantidadePorItem.size()); ++item) {
        if (itemEhProibidoNaFaccao(vila.dados, faccao, item)) {
            confiscados += grupo.quantidadePorItem[static_cast<std::size_t>(item)];
            grupo.quantidadePorItem[static_cast<std::size_t>(item)] = 0;
        }
    }
    return confiscados;
}

// O crime não pago (4.5): a Recompensa sobe 2 × a multa, e a ficha guarda a gravidade e a fiança.
void anotarOCrimeNaoPago(SimulacaoDaVila& vila, int faccao, int gravidade, int multa)
{
    const RegrasDoCrime& regras = vila.dados.regrasDoCrime;
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)] += regras.recompensaVezesAMulta * multa;
    crime.gravidadesNaFicha[static_cast<std::size_t>(faccao)] += gravidade;
    crime.fiancaNaFicha[static_cast<std::size_t>(faccao)] += regras.fiancaVezesAMulta * multa;
}

// O grupo para onde está (a viagem acaba).
void pararOGrupo(EntidadeEmTransito& grupo)
{
    grupo.emTransito = false;
    grupo.rota.clear();
    grupo.proximoMapa = 0;
    grupo.ticksDoTrecho = 0;
    grupo.ticksAndadosNoTrecho = 0;
    grupo.encontroMarcado = {};
    grupo.encontroAcontecendo = false;
    grupo.ticksParadoNaLuta = 0;
}

std::vector<std::string> soltarOJogador(SimulacaoDaVila& vila)
{
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    const int faccao = crime.faccaoDaPrisao;
    std::vector<std::string> linhas;
    if (faccao >= 0) {
        crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)] = 0;
        crime.gravidadesNaFicha[static_cast<std::size_t>(faccao)] = 0;
        crime.fiancaNaFicha[static_cast<std::size_t>(faccao)] = 0;
        const int confiscados = confiscarOsItensProibidos(vila, faccao);
        linhas.push_back("Aviso: o grupo saiu da prisão " + nomeDaFaccaoComArtigo(vila.dados, faccao, "de") + "; a ficha está limpa"
                         + (confiscados > 0 ? " (" + std::to_string(confiscados) + " itens proibidos confiscados)." : "."));
    }
    crime.presoAteODia = -1;
    crime.faccaoDaPrisao = -1;
    return linhas;
}

}  // namespace

RegrasDoCrime lerArquivoDosCrimes(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDoCrime regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDosCrimes, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    if (!documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDosCrimes, 0, "o arquivo precisa ser um objeto { ... }");
        return regras;
    }
    const Json crimes = documento.value("crimes", Json::array());
    for (const Json& crime : crimes) {
        const std::string id = crime.value("id", std::string());
        const int gravidade = crime.value("gravidade", 0);
        if (id.empty() || gravidade < 1 || gravidade > 5) {
            listaDeProblemas.anotarErro(nomeDoArquivoDosCrimes, 0, "cada crime precisa de \"id\" e de uma \"gravidade\" de 1 a 5");
            continue;
        }
        regras.gravidadePorCrime[id] = gravidade;
    }
    const auto lerInteiro = [](const Json& objeto, const char* campo, int& valor) {
        if (const auto lido = objeto.find(campo); lido != objeto.end() && lido->is_number_integer()) {
            valor = lido->get<int>();
        }
    };
    lerInteiro(documento, "multa_por_gravidade_ao_quadrado", regras.multaPorGravidadeAoQuadrado);
    lerInteiro(documento, "recompensa_vezes_a_multa", regras.recompensaVezesAMulta);
    lerInteiro(documento, "recompensa_cai_por_estacao_em_porcento", regras.recompensaCaiPorEstacaoEmPorcento);
    lerInteiro(documento, "cacadores_a_partir_de", regras.cacadoresAPartirDe);
    lerInteiro(documento, "cacadores_chance_por_mil", regras.cacadoresChancePorMil);
    regras.revistaPorFaixaEmPorcento = documento.value("revista_por_faixa_em_porcento", std::map<std::string, int>{});
    lerInteiro(documento, "suborno_base_em_porcento", regras.subornoBaseEmPorcento);
    lerInteiro(documento, "suborno_por_multa_oferecida_em_porcento", regras.subornoPorMultaOferecidaEmPorcento);
    lerInteiro(documento, "suborno_maximo_em_porcento", regras.subornoMaximoEmPorcento);
    lerInteiro(documento, "dias_de_prisao_por_gravidade", regras.diasDePrisaoPorGravidade);
    lerInteiro(documento, "fianca_vezes_a_multa", regras.fiancaVezesAMulta);
    const Json criminalidade = documento.value("criminalidade", Json::object());
    lerInteiro(criminalidade, "base", regras.criminalidadeBase);
    lerInteiro(criminalidade, "por_ponto_de_felicidade_abaixo_de_50", regras.porPontoDeFelicidadeAbaixoDe50);
    lerInteiro(criminalidade, "por_ponto_da_taxa_de_desemprego", regras.porPontoDaTaxaDeDesemprego);
    lerInteiro(criminalidade, "sem_dinheiro_para_2_dias_da_cesta", regras.semDinheiroParaDoisDiasDaCesta);
    lerInteiro(criminalidade, "passo_por_dia_em_porcento", regras.passoPorDiaEmPorcento);
    lerInteiro(criminalidade, "felicidade_a_partir_de", regras.felicidadeAPartirDe);
    lerInteiro(criminalidade, "felicidade_por_ponto_acima_em_decimos", regras.felicidadePorPontoAcimaEmDecimos);
    lerInteiro(criminalidade, "furto_a_partir_de", regras.furtoAPartirDe);
    lerInteiro(criminalidade, "furto_por_mil_por_ponto", regras.furtoPorMilPorPonto);
    lerInteiro(criminalidade, "furto_tira_em_porcento", regras.furtoTiraEmPorcento);
    lerInteiro(criminalidade, "furto_cooldown_em_dias", regras.furtoCooldownEmDias);
    lerInteiro(criminalidade, "ameaca_bandida_a_partir_de", regras.ameacaBandidaAPartirDe);
    lerInteiro(criminalidade, "ameaca_bandida_por_mil_por_ponto", regras.ameacaBandidaPorMilPorPonto);
    return regras;
}

int calcularAlvoDaCriminalidade(int felicidadeEmDecimos, int civis, int desempregados, bool semDinheiroParaDoisDias, const RegrasDoCrime& regras)
{
    int alvo = 10 * regras.criminalidadeBase + std::max(0, 500 - felicidadeEmDecimos) * regras.porPontoDeFelicidadeAbaixoDe50;
    if (civis > 0) {
        alvo += 10 * regras.porPontoDaTaxaDeDesemprego * desempregados / civis;
    }
    alvo += semDinheiroParaDoisDias ? 10 * regras.semDinheiroParaDoisDiasDaCesta : 0;
    return std::clamp(alvo, 0, 1000);
}

int darOPassoDaCriminalidade(int criminalidadeEmDecimos, int alvoEmDecimos, const RegrasDoCrime& regras)
{
    const int diferenca = alvoEmDecimos - criminalidadeEmDecimos;
    if (diferenca == 0) {
        return criminalidadeEmDecimos;
    }
    const int tamanho = diferenca > 0 ? diferenca : -diferenca;
    const int passo = (tamanho * regras.passoPorDiaEmPorcento + 99) / 100;  // na direção do alvo
    return criminalidadeEmDecimos + (diferenca > 0 ? passo : -passo);
}

int felicidadeDaCriminalidadeEmDecimos(int criminalidadeEmDecimos, const RegrasDoCrime& regras)
{
    const int acima = criminalidadeEmDecimos - 10 * regras.felicidadeAPartirDe;
    return acima > 0 ? acima * regras.felicidadePorPontoAcimaEmDecimos / 10 : 0;
}

std::vector<std::string> rodarACriminalidadeDoDia(ContextoDaProducao& contexto, int civis, int desempregados, bool semDinheiroParaDoisDias)
{
    std::vector<std::string> linhas;
    const RegrasDoCrime& regras = contexto.dados.regrasDoCrime;
    FelicidadeDaClasse& felicidade = contexto.felicidade;
    if (civis <= 0) {
        return linhas;  // sem Civis, a Criminalidade fica como está
    }
    const int alvo = calcularAlvoDaCriminalidade(felicidade.felicidadeEmDecimos, civis, desempregados, semDinheiroParaDoisDias, regras);
    felicidade.criminalidadeEmDecimos = darOPassoDaCriminalidade(felicidade.criminalidadeEmDecimos, alvo, regras);
    // 4.2: o furto no armazém, só com 25 ou mais (CJ1-3).
    const int64_t hoje = contexto.relogio != nullptr ? contexto.relogio->diaDaPartida() : 0;
    if (felicidade.criminalidadeEmDecimos < 10 * regras.furtoAPartirDe || hoje < felicidade.diaDoProximoFurtoPossivel) {
        return linhas;
    }
    if (contexto.sorteioDosEventos.sortearNumeroInteiroEntre(0, 999) >= felicidade.criminalidadeEmDecimos / 10 * regras.furtoPorMilPorPonto) {
        return linhas;
    }
    int melhorItem = -1;
    int64_t maiorValor = 0;
    for (int item = 0; item < contexto.dados.catalogoDeItens.quantidadeDeItens(); ++item) {
        const int64_t valor = static_cast<int64_t>(contexto.inventario.quantidadeDe(item, DonoDoItem::Cidade)) * precoLocalDoItem(contexto, item);
        if (valor > maiorValor) {
            maiorValor = valor;
            melhorItem = item;
        }
    }
    felicidade.diaDoProximoFurtoPossivel = hoje + regras.furtoCooldownEmDias;
    if (melhorItem < 0) {
        return linhas;
    }
    const int levados = contexto.inventario.quantidadeDe(melhorItem, DonoDoItem::Cidade) * regras.furtoTiraEmPorcento / 100;
    if (levados > 0 && contexto.inventario.tentarTirar(melhorItem, DonoDoItem::Cidade, levados)) {
        linhas.push_back("Evento: Furto no armazém (sumiram " + std::to_string(levados) + " "
                         + contexto.dados.tabelaDeTextos.textoDe(contexto.dados.catalogoDeItens.itemPeloNumero(melhorItem).identificadorDeTexto) + ").");
    }
    return linhas;
}

int ameacaBandidaComOsNumeros(int mediaDaCriminalidadeEmDecimos, const RegrasDoCrime& regras)
{
    const int acima = mediaDaCriminalidadeEmDecimos - 10 * regras.ameacaBandidaAPartirDe;
    return acima > 0 ? acima * regras.ameacaBandidaPorMilPorPonto / 10 : 0;
}

void atualizarAAmeacaBandida(SimulacaoDaVila& vila)
{
    std::vector<int64_t> soma(vila.viagem.ameacaBandidaPorRegiao.size(), 0);
    std::vector<int64_t> cidades(soma.size(), 0);
    for (const SimulacaoDaVila* cidade : cidadesDoMundo(vila)) {
        const int regiao = vila.dados.mundo.mapa(cidade->numeroDoMapaNoMundo()).regiao;
        if (regiao >= 1 && regiao <= static_cast<int>(soma.size())) {
            soma[static_cast<std::size_t>(regiao - 1)] += cidade->felicidade.criminalidadeEmDecimos;
            ++cidades[static_cast<std::size_t>(regiao - 1)];
        }
    }
    for (std::size_t regiao = 0; regiao < soma.size(); ++regiao) {
        vila.viagem.ameacaBandidaPorRegiao[regiao] = cidades[regiao] > 0 ? ameacaBandidaComOsNumeros(static_cast<int>(soma[regiao] / cidades[regiao]), vila.dados.regrasDoCrime) : 0;
    }
}

void montarOCrimeDaPartidaNova(SimulacaoDaVila& vila)
{
    const std::size_t faccoes = vila.dados.faccoes.faccoes.size();
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    crime = EstadoDoCrimeDoJogador{};
    crime.recompensaPorFaccao.assign(faccoes, 0);
    crime.gravidadesNaFicha.assign(faccoes, 0);
    crime.fiancaNaFicha.assign(faccoes, 0);
    crime.estacaoDaUltimaHeresia.assign(faccoes, -1);
}

int chanceDaRevistaEmPorcento(const RegrasDoCrime& regras, const std::string& faixa, int recompensa)
{
    if (recompensa > 0) {
        return 100;
    }
    const auto chance = regras.revistaPorFaixaEmPorcento.find(faixa);
    return chance == regras.revistaPorFaixaEmPorcento.end() ? 0 : chance->second;
}

int chanceDoSubornoEmPorcento(int64_t ofertaEmDecimos, int multaEmMoedas, const RegrasDoCrime& regras)
{
    if (multaEmMoedas <= 0) {
        return regras.subornoMaximoEmPorcento;
    }
    const int64_t chance = regras.subornoBaseEmPorcento + regras.subornoPorMultaOferecidaEmPorcento * ofertaEmDecimos / (static_cast<int64_t>(multaEmMoedas) * 10);
    return static_cast<int>(std::min<int64_t>(chance, regras.subornoMaximoEmPorcento));
}

bool revistarJogador(SimulacaoDaVila& vila, int mapa, int mapaAnterior, std::vector<std::string>& linhas)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDoCrime& regras = dados.regrasDoCrime;
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    const int faccao = faccaoDaCidade(dados, mapa);
    if (faccao < 0 || crime.revista.ativa || crime.recompensaPorFaccao.empty()) {
        return false;
    }
    // Passo 1: a chance pela faixa (com Recompensa, certa).
    const int chance = chanceDaRevistaEmPorcento(regras, faixaDoJogadorNaCidade(vila, mapa).nome, crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)]);
    if (chance <= 0 || vila.sorteioDasCaravanas().sortearNumeroInteiroEntre(0, 99) >= chance) {
        return false;
    }
    // Passo 2: o que ela acha.
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    RevistaPendente revista;
    for (int item = 0; item < static_cast<int>(grupo.quantidadePorItem.size()); ++item) {
        if (grupo.quantidadePorItem[static_cast<std::size_t>(item)] > 0 && itemEhProibidoNaFaccao(dados, faccao, item)) {
            revista.crimes.push_back("crime.contrabando");
            break;
        }
    }
    const int estacao = static_cast<int>(vila.relogio.diaDaPartida() / std::max(1, vila.relogio.diasPorEstacao()));
    const std::optional<int> livro = dados.catalogoDeItens.procurarNumeroDoItem(identificadorDoLivroSagrado);
    const bool levaOLivro = livro && grupo.quantidadePorItem[static_cast<std::size_t>(*livro)] > 0;
    if (opcaoDaLei(vila, faccao, "livro_sagrado_obrigatorio") == "sim" && !levaOLivro && crime.estacaoDaUltimaHeresia[static_cast<std::size_t>(faccao)] != estacao) {
        revista.crimes.push_back("crime.heresia_sem_livro");
        crime.estacaoDaUltimaHeresia[static_cast<std::size_t>(faccao)] = estacao;
    }
    if (revista.crimes.empty()) {
        linhas.push_back("Viagem: revista em " + dados.mundo.nomeDoMapa(mapa) + ": nada encontrado.");
        return false;
    }
    for (const std::string& achado : revista.crimes) {
        const auto gravidade = regras.gravidadePorCrime.find(achado);
        revista.gravidade = std::max(revista.gravidade, gravidade == regras.gravidadePorCrime.end() ? 1 : gravidade->second);
    }
    revista.ativa = true;
    revista.faccao = faccao;
    revista.mapa = mapa;
    revista.mapaAnterior = mapaAnterior;
    revista.multa = regras.multaDaGravidade(revista.gravidade);
    // Passo 4: a reputação cai 2 × a gravidade (o crime pago também).
    const std::string faixa = mudarReputacao(vila, donoDaCidadeDoMapa(dados.mundo, mapa), -2 * revista.gravidade, "crime: " + nomeDoCrime(revista.crimes.front()) + " em " + dados.mundo.nomeDoMapa(mapa));
    std::string achados;
    for (const std::string& achado : revista.crimes) {
        achados += (achados.empty() ? "" : " e ") + nomeDoCrime(achado);
    }
    linhas.push_back("Aviso: revista em " + dados.mundo.nomeDoMapa(mapa) + ": " + achados + " (multa de " + std::to_string(revista.multa) + " moedas"
                     + (crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)] > 0 ? ", mais a Recompensa de " + std::to_string(crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)]) : "")
                     + ").");
    if (!faixa.empty()) {
        linhas.push_back(faixa);
    }
    crime.revista = std::move(revista);
    return true;
}

std::string resolverARevista(SimulacaoDaVila& vila, EscolhaNaRevista escolha, int64_t ofertaEmDecimos, std::vector<std::string>& linhas)
{
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    RevistaPendente& revista = crime.revista;
    if (!revista.ativa) {
        return "não há revista agora";
    }
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    const RegrasDoCrime& regras = vila.dados.regrasDoCrime;
    SimulacaoDaVila* cidade = cidadeDoMapa(vila, revista.mapa);
    const int faccao = revista.faccao;
    const std::string onde = vila.dados.mundo.nomeDoMapa(revista.mapa);
    if (escolha == EscolhaNaRevista::Pagar) {
        // 4.5, passo 2: com Recompensa, paga ela inteira e a multa.
        const int64_t valor = static_cast<int64_t>(crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)] + revista.multa) * 10;
        if (grupo.dinheiroEmDecimos < valor) {
            return "o grupo não leva " + std::to_string(valor / 10) + " moedas";
        }
        grupo.dinheiroEmDecimos -= valor;
        if (cidade != nullptr) {
            cidade->dinheiro.bancoDaCidade += valor;
        }
        const int confiscados = confiscarOsItensProibidos(vila, faccao);
        crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)] = 0;
        crime.gravidadesNaFicha[static_cast<std::size_t>(faccao)] = 0;
        crime.fiancaNaFicha[static_cast<std::size_t>(faccao)] = 0;
        linhas.push_back("Viagem: o grupo pagou " + std::to_string(valor / 10) + " moedas em " + onde + " e entrou"
                         + (confiscados > 0 ? " (" + std::to_string(confiscados) + " itens confiscados)." : "."));
        revista = {};
        return "";
    }
    if (escolha == EscolhaNaRevista::Subornar) {
        if (ofertaEmDecimos <= 0 || ofertaEmDecimos > grupo.dinheiroEmDecimos) {
            return "a oferta precisa ser de 0,1 até o dinheiro que o grupo leva";
        }
        const int chance = chanceDoSubornoEmPorcento(ofertaEmDecimos, revista.multa, regras);
        grupo.dinheiroEmDecimos -= ofertaEmDecimos;
        if (cidade != nullptr) {
            cidade->dinheiro.bancoDaCidade += ofertaEmDecimos;
        }
        if (vila.sorteioDasCaravanas().sortearNumeroInteiroEntre(0, 99) < chance) {
            linhas.push_back("Viagem: o guarda aceitou o suborno (chance de " + std::to_string(chance) + "%); o grupo entrou em " + onde + ".");
            revista = {};
            return "";
        }
        linhas.push_back("Aviso: o guarda recusou o suborno (chance de " + std::to_string(chance) + "%).");
        anotarOCrimeNaoPago(vila, faccao, revista.gravidade, revista.multa);
        revista = {};
        prenderJogador(vila, faccao, linhas);
        return "";
    }
    // Fugir: volta para o mapa de onde veio, sem entrar; a Recompensa sobe.
    anotarOCrimeNaoPago(vila, faccao, revista.gravidade, revista.multa);
    pararOGrupo(grupo);
    grupo.mapaAtual = revista.mapaAnterior > 0 ? revista.mapaAnterior : grupo.mapaAtual;
    linhas.push_back("Aviso: o grupo fugiu da revista e voltou a " + vila.dados.mundo.nomeDoMapa(grupo.mapaAtual) + ". Recompensa " + nomeDaFaccaoComArtigo(vila.dados, faccao, "de")
                     + ": " + std::to_string(crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)]) + " moedas.");
    revista = {};
    return "";
}

void prenderJogador(SimulacaoDaVila& vila, int faccao, std::vector<std::string>& linhas)
{
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    const RegrasDoCrime& regras = vila.dados.regrasDoCrime;
    const DadosDoJogo& dados = vila.dados;
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    const int dias = regras.diasDePrisaoPorGravidade * std::max(1, crime.gravidadesNaFicha[static_cast<std::size_t>(faccao)]);
    crime.presoAteODia = vila.relogio.diaDaPartida() + dias;
    crime.faccaoDaPrisao = faccao;
    crime.faccaoDosCacadores = -1;
    pararOGrupo(grupo);
    if (faccaoDaCidade(dados, grupo.mapaAtual) != faccao) {
        const DefinicaoDaFaccao& definicao = dados.faccoes.faccoes[static_cast<std::size_t>(faccao)];
        grupo.mapaAtual = dados.mundo.numeroDoMapaNa(definicao.colunaDaCapital, definicao.linhaDaCapital);  // levado para a capital
    }
    linhas.push_back("Aviso: o grupo foi preso " + nomeDaFaccaoComArtigo(dados, faccao, "por") + " em " + dados.mundo.nomeDoMapa(grupo.mapaAtual) + " por " + std::to_string(dias)
                     + " dias (fiança: " + std::to_string(crime.fiancaNaFicha[static_cast<std::size_t>(faccao)]) + " moedas).");
}

std::string pagarAFianca(SimulacaoDaVila& vila, std::vector<std::string>& linhas)
{
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    if (!crime.estaPreso(vila.relogio.diaDaPartida()) || crime.faccaoDaPrisao < 0) {
        return "o grupo não está preso";
    }
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    const int64_t valor = static_cast<int64_t>(crime.fiancaNaFicha[static_cast<std::size_t>(crime.faccaoDaPrisao)]) * 10;
    if (grupo.dinheiroEmDecimos < valor) {
        return "o grupo não leva " + std::to_string(valor / 10) + " moedas";
    }
    grupo.dinheiroEmDecimos -= valor;
    if (SimulacaoDaVila* cidade = cidadeDoMapa(vila, grupo.mapaAtual)) {
        cidade->dinheiro.bancoDaCidade += valor;
    }
    linhas.push_back("Viagem: o grupo pagou a fiança de " + std::to_string(valor / 10) + " moedas.");
    const std::vector<std::string> soltura = soltarOJogador(vila);
    linhas.insert(linhas.end(), soltura.begin(), soltura.end());
    return "";
}

int recompensaDepoisDaQueda(int recompensa, const RegrasDoCrime& regras)
{
    const int nova = recompensa * (100 - regras.recompensaCaiPorEstacaoEmPorcento) / 100;
    return nova < 10 ? 0 : nova;
}

std::vector<std::string> rodarOCrimeDoJogadorNoDiaNovo(SimulacaoDaVila& vila)
{
    std::vector<std::string> linhas;
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    if (crime.presoAteODia >= 0 && vila.relogio.diaDaPartida() > crime.presoAteODia) {
        linhas = soltarOJogador(vila);
    }
    if (vila.relogio.diaDaEstacaoAtual() == 1) {
        for (int& recompensa : crime.recompensaPorFaccao) {
            recompensa = recompensaDepoisDaQueda(recompensa, vila.dados.regrasDoCrime);
        }
    }
    return linhas;
}

int forcaDosCacadores(int recompensa)
{
    return recompensa / 100 * 3;
}

std::vector<std::string> conferirOsCacadores(SimulacaoDaVila& vila)
{
    std::vector<std::string> linhas;
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    if (!grupo.emTransito) {
        crime.proximoMapaNaUltimaConferencia = 0;
        return linhas;
    }
    if (grupo.proximoMapa == crime.proximoMapaNaUltimaConferencia) {
        return linhas;
    }
    crime.proximoMapaNaUltimaConferencia = grupo.proximoMapa;
    const int faccao = faccaoDoMapa(vila.dados, grupo.proximoMapa);
    if (faccao < 0 || crime.recompensaPorFaccao.empty() || crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)] < vila.dados.regrasDoCrime.cacadoresAPartirDe
        || grupo.encontroMarcado.tipo != TipoDeEncontro::Nenhum) {
        return linhas;
    }
    if (vila.sorteioDasCaravanas().sortearNumeroInteiroEntre(0, 999) < vila.dados.regrasDoCrime.cacadoresChancePorMil) {
        grupo.encontroMarcado = {TipoDeEncontro::Cacadores, forcaDosCacadores(crime.recompensaPorFaccao[static_cast<std::size_t>(faccao)])};
        crime.faccaoDosCacadores = faccao;
    }
    return linhas;
}

std::string resolverOsCacadores(SimulacaoDaVila& vila, EscolhaNoEncontro escolha, std::vector<std::string>& linhas)
{
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    EstadoDoCrimeDoJogador& crime = vila.crimeDoJogador;
    if (!grupo.encontroAcontecendo || grupo.encontroMarcado.tipo != TipoDeEncontro::Cacadores || crime.faccaoDosCacadores < 0) {
        return "não há Caçadores agora";
    }
    const int faccao = crime.faccaoDosCacadores;
    const int forca = grupo.encontroMarcado.bandidos;
    const RegrasDaViagem& regras = vila.dados.regrasDaViagem;
    grupo.encontroMarcado = {};
    grupo.encontroAcontecendo = false;
    if (escolha == EscolhaNoEncontro::Lutar) {
        grupo.ticksParadoNaLuta = regras.lutaRapidaDuraTicks;
        // Fase D1 (Combate, 4.8): os Caçadores com o kit de guarda, um a cada 3 de força.
        const RegrasDoCombate& combate = vila.dados.regrasDoCombate;
        const ResultadoDoCalculoRapido luta = resolverPorCalculoRapido(combate, ladoDoGrupo(combate, grupo), ladoDoKit(combate, "guarda", std::max(1, forca / 3)),
                                                                       vila.sorteioDasCaravanas().proximoNumero());
        if (luta.venceuOLadoA) {
            grupo.vidaEmPorcento = std::max(1, grupo.vidaEmPorcento - luta.perdaDeVidaDoVencedorEmPorcento);
            linhas.push_back("Viagem: o grupo venceu os Caçadores de Recompensa.");
            crime.faccaoDosCacadores = -1;
            return "";
        }
        linhas.push_back("Aviso: o grupo perdeu a luta contra os Caçadores de Recompensa.");
    } else {
        linhas.push_back("Aviso: o grupo se rendeu aos Caçadores de Recompensa.");
    }
    prenderJogador(vila, faccao, linhas);
    return "";
}
