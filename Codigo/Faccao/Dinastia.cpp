#include "Faccao/Dinastia.h"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "Dados/ProblemasDosDados.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/Populacao.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDosCompanheiros = "companheiros.json";

int sortearTraco(const RegrasDaFamilia& familia, SorteioComSemente& sorteio)
{
    return sorteio.sortearNumeroInteiroEntre(familia.tracoMinimo, familia.tracoMaximo);
}

// Um nome da cultura que nenhum membro vivo usa; com todos em uso, o nome e o número.
std::string escolherNome(const DinastiaDaFaccao& dinastia, const CulturaDaDinastia& cultura, bool ehMulher, int numero, SorteioComSemente& sorteio)
{
    const std::vector<std::string>& lista = ehMulher ? cultura.nomesDeMulher : cultura.nomesDeHomem;
    if (lista.empty()) {
        return "Nobre " + std::to_string(numero);
    }
    std::vector<std::string> livres;
    for (const std::string& nome : lista) {
        const bool emUso = std::any_of(dinastia.membros.begin(), dinastia.membros.end(), [&](const MembroDaDinastia& membro) { return membro.nome == nome; });
        if (!emUso) {
            livres.push_back(nome);
        }
    }
    if (livres.empty()) {
        return lista[static_cast<std::size_t>(sorteio.sortearNumeroInteiroEntre(0, static_cast<int>(lista.size()) - 1))] + " " + std::to_string(numero);
    }
    return livres[static_cast<std::size_t>(sorteio.sortearNumeroInteiroEntre(0, static_cast<int>(livres.size()) - 1))];
}

// Um membro novo com os traços e as habilidades sorteados (de 20 a 80).
MembroDaDinastia criarMembro(DinastiaDaFaccao& dinastia, const CulturaDaDinastia& cultura, const RegrasDaFamilia& familia, bool ehMulher, int idade,
                             SorteioComSemente& sorteio)
{
    MembroDaDinastia membro;
    membro.numero = dinastia.proximoNumero++;
    membro.ehMulher = ehMulher;
    membro.idade = idade;
    membro.nome = escolherNome(dinastia, cultura, ehMulher, membro.numero, sorteio);
    membro.personalidade.coragem = sortearTraco(familia, sorteio);
    membro.personalidade.ganancia = sortearTraco(familia, sorteio);
    membro.personalidade.agressividade = sortearTraco(familia, sorteio);
    membro.personalidade.sociabilidade = sortearTraco(familia, sorteio);
    membro.personalidade.lealdade = sortearTraco(familia, sorteio);
    membro.carisma = sortearTraco(familia, sorteio);
    membro.corpoACorpo = sortearTraco(familia, sorteio);
    membro.especialista = sortearTraco(familia, sorteio);
    return membro;
}

// A idade de quem entra na dinastia: perto de "idadePerto" (±3), adulto e ainda não velho.
int idadeDeQuemEntra(int idadePerto, const RegrasDaPopulacao& populacao, SorteioComSemente& sorteio)
{
    return std::clamp(idadePerto + sorteio.sortearNumeroInteiroEntre(-3, 3), populacao.idadeAdulto, std::max(populacao.idadeAdulto, populacao.idadeVelho - 1));
}

void casar(MembroDaDinastia& um, MembroDaDinastia& outro)
{
    um.conjuge = outro.numero;
    outro.conjuge = um.numero;
}

void reporOsNobres(DinastiaDaFaccao& dinastia, const CulturaDaDinastia& cultura, const RegrasDaFamilia& familia, const RegrasDaPopulacao& populacao,
                   SorteioComSemente& sorteio)
{
    int nobres = static_cast<int>(std::count_if(dinastia.membros.begin(), dinastia.membros.end(), [](const MembroDaDinastia& membro) { return membro.ehNobre; }));
    for (; nobres < familia.nobresPorDinastia; ++nobres) {
        const bool ehMulher = sorteio.sortearNumeroInteiroEntre(0, 1) == 1;
        const int idade = sorteio.sortearNumeroInteiroEntre(populacao.idadeAdulto, std::max(populacao.idadeAdulto, populacao.idadeVelho - 11));
        MembroDaDinastia nobre = criarMembro(dinastia, cultura, familia, ehMulher, idade, sorteio);
        nobre.ehNobre = true;
        dinastia.membros.push_back(std::move(nobre));
    }
}

}  // namespace

const MembroDaDinastia* DinastiaDaFaccao::procurar(int numero) const
{
    for (const MembroDaDinastia& membro : membros) {
        if (membro.numero == numero) {
            return &membro;
        }
    }
    return nullptr;
}

MembroDaDinastia* DinastiaDaFaccao::procurar(int numero)
{
    for (MembroDaDinastia& membro : membros) {
        if (membro.numero == numero) {
            return &membro;
        }
    }
    return nullptr;
}

RegrasDaFamilia lerArquivoDosCompanheiros(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDaFamilia regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDosCompanheiros, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    const Json familia = documento.value("familia", Json::object());
    regras.gravidezEmEstacoes = std::max(1, familia.value("gravidez_estacoes", regras.gravidezEmEstacoes));
    regras.chanceDeFilhoPorEstacaoEmPorcento = familia.value("chance_de_filho_por_estacao_em_porcento", regras.chanceDeFilhoPorEstacaoEmPorcento);
    regras.chanceDeCasarPorEstacaoEmPorcento = familia.value("chance_de_casar_por_estacao_em_porcento", regras.chanceDeCasarPorEstacaoEmPorcento);
    regras.fertilAte = familia.value("fertil_ate", regras.fertilAte);
    const Json dinastia = documento.value("dinastia", Json::object());
    regras.nobresPorDinastia = std::max(1, dinastia.value("nobres", regras.nobresPorDinastia));
    regras.liderCasadoAPartirDe = dinastia.value("lider_casado_a_partir_de", regras.liderCasadoAPartirDe);
    regras.membrosNoMaximo = dinastia.value("membros_no_maximo", regras.membrosNoMaximo);
    regras.legitimidadeNaCrise = dinastia.value("legitimidade_na_crise", regras.legitimidadeNaCrise);
    const std::vector<int> tracos = dinastia.value("tracos_entre", std::vector<int>{regras.tracoMinimo, regras.tracoMaximo});
    if (tracos.size() != 2 || tracos[0] > tracos[1] || tracos[0] < 0 || tracos[1] > 100) {
        listaDeProblemas.anotarErro(nomeDoArquivoDosCompanheiros, 0, "\"tracos_entre\" precisa ser [mínimo, máximo], de 0 a 100");
    } else {
        regras.tracoMinimo = tracos[0];
        regras.tracoMaximo = tracos[1];
    }
    return regras;
}

DinastiaDaFaccao montarADinastiaDoComeco(const LiderDoComeco& liderDoComeco, const CulturaDaDinastia& cultura, const RegrasDaFamilia& familia,
                                         const RegrasDaPopulacao& populacao, SorteioComSemente& sorteio)
{
    DinastiaDaFaccao dinastia;
    MembroDaDinastia lider = criarMembro(dinastia, cultura, familia, liderDoComeco.ehMulher, liderDoComeco.idade, sorteio);
    lider.nome = liderDoComeco.nome;
    lider.personalidade = liderDoComeco.personalidade;
    lider.vidaProlongada = liderDoComeco.vidaProlongada;
    dinastia.numeroDoLider = lider.numero;
    dinastia.membros.push_back(lider);
    // CP1-5: com 12 anos ou mais, casado e com 1 ou 2 filhos adultos.
    if (liderDoComeco.idade >= familia.liderCasadoAPartirDe) {
        MembroDaDinastia conjuge = criarMembro(dinastia, cultura, familia, !liderDoComeco.ehMulher, idadeDeQuemEntra(liderDoComeco.idade, populacao, sorteio), sorteio);
        casar(dinastia.membros.front(), conjuge);
        const int pai = liderDoComeco.ehMulher ? conjuge.numero : lider.numero;
        const int mae = liderDoComeco.ehMulher ? lider.numero : conjuge.numero;
        dinastia.membros.push_back(conjuge);
        const int filhos = sorteio.sortearNumeroInteiroEntre(1, 2);
        const int idadeMaxima = std::max(populacao.idadeAdulto, std::min(liderDoComeco.idade, conjuge.idade) - populacao.idadeAdulto);
        for (int filho = 0; filho < filhos; ++filho) {
            const bool ehMulher = sorteio.sortearNumeroInteiroEntre(0, 1) == 1;
            MembroDaDinastia novo = criarMembro(dinastia, cultura, familia, ehMulher, sorteio.sortearNumeroInteiroEntre(populacao.idadeAdulto, idadeMaxima), sorteio);
            novo.pai = pai;
            novo.mae = mae;
            dinastia.membros.push_back(std::move(novo));
        }
    }
    reporOsNobres(dinastia, cultura, familia, populacao, sorteio);
    return dinastia;
}

int escolherHerdeiro(const DinastiaDaFaccao& dinastia, const std::string& sucessao, int idadeAdulto)
{
    const bool soOsFilhos = sucessao != "duelo" && sucessao != "merito" && sucessao != "eleicao_do_clero" && sucessao != "eleicao";
    const auto pontos = [&](const MembroDaDinastia& membro) {
        if (sucessao == "duelo") {
            return membro.corpoACorpo;
        }
        if (sucessao == "merito") {
            return membro.especialista;  // até a Magia (D4), a Arcana também vai pelo Especialista
        }
        if (sucessao == "eleicao_do_clero" || sucessao == "eleicao") {
            return membro.personalidade.sociabilidade;
        }
        return 0;  // a herança: só a idade decide
    };
    const MembroDaDinastia* herdeiro = nullptr;
    for (const MembroDaDinastia& membro : dinastia.membros) {
        if (membro.numero == dinastia.numeroDoLider || membro.ehNobre || membro.idade < idadeAdulto) {
            continue;
        }
        if (soOsFilhos && membro.pai != dinastia.numeroDoLider && membro.mae != dinastia.numeroDoLider) {
            continue;
        }
        // Ganha quem tem mais pontos; empate, o mais velho; depois, o menor número (os gêmeos, 10.5).
        if (herdeiro == nullptr || pontos(membro) > pontos(*herdeiro) || (pontos(membro) == pontos(*herdeiro) && membro.idade > herdeiro->idade)) {
            herdeiro = &membro;
        }
    }
    return herdeiro == nullptr ? -1 : herdeiro->numero;
}

ResultadoDaEstacaoDaDinastia passarUmaEstacaoNaDinastia(DinastiaDaFaccao& dinastia, const CulturaDaDinastia& cultura, const RegrasDaFamilia& familia,
                                                        const RegrasDaPopulacao& populacao, bool ehPrimavera, bool torreDePe, SorteioComSemente& sorteio)
{
    ResultadoDaEstacaoDaDinastia resultado;
    // 1. A Vida prolongada (HI11-1): sem a Torre, envelhece de uma vez até a idade de velho e perde o traço.
    for (MembroDaDinastia& membro : dinastia.membros) {
        if (membro.vidaProlongada && !torreDePe) {
            membro.idade = std::max(membro.idade, populacao.idadeVelho);
            membro.vidaProlongada = false;
        }
        if (ehPrimavera && !membro.vidaProlongada) {
            ++membro.idade;
        }
    }
    // 2. A velhice (População, 4.1): os mortos saem, e o viúvo fica solteiro.
    std::vector<int> mortos;
    for (const MembroDaDinastia& membro : dinastia.membros) {
        if (!membro.vidaProlongada && sorteio.sortearNumeroInteiroEntre(0, 99) < chanceDeMorrerDeVelhice(membro.idade, false, populacao)) {
            mortos.push_back(membro.numero);
            if (membro.numero == dinastia.numeroDoLider) {
                resultado.liderMorreu = true;
                resultado.nomeDoLiderMorto = membro.nome;
                resultado.idadeDoLiderMorto = membro.idade;
            }
        }
    }
    std::erase_if(dinastia.membros, [&](const MembroDaDinastia& membro) { return std::find(mortos.begin(), mortos.end(), membro.numero) != mortos.end(); });
    for (MembroDaDinastia& membro : dinastia.membros) {
        if (std::find(mortos.begin(), mortos.end(), membro.conjuge) != mortos.end()) {
            membro.conjuge = -1;
        }
    }
    // 3. Os partos: a gravidez dura 1 estação; o filho nasce criança (idade 0).
    std::vector<MembroDaDinastia> nascidos;
    for (MembroDaDinastia& mae : dinastia.membros) {
        if (mae.estacoesDeGravidez == 0 || --mae.estacoesDeGravidez > 0) {
            continue;
        }
        MembroDaDinastia filho = criarMembro(dinastia, cultura, familia, sorteio.sortearNumeroInteiroEntre(0, 1) == 1, 0, sorteio);
        filho.mae = mae.numero;
        filho.pai = mae.conjuge;
        nascidos.push_back(std::move(filho));
    }
    for (MembroDaDinastia& filho : nascidos) {
        dinastia.membros.push_back(std::move(filho));
    }
    // 4. Os casamentos (4.6, passo 1): o adulto solteiro da família casa com um nobre gerado da mesma cultura.
    const std::size_t membrosAntesDosCasamentos = dinastia.membros.size();
    for (std::size_t posicao = 0; posicao < membrosAntesDosCasamentos; ++posicao) {
        const MembroDaDinastia& solteiro = dinastia.membros[posicao];
        if (solteiro.ehNobre || solteiro.conjuge >= 0 || solteiro.idade < populacao.idadeAdulto) {
            continue;
        }
        if (static_cast<int>(dinastia.membros.size()) >= familia.membrosNoMaximo
            || sorteio.sortearNumeroInteiroEntre(0, 99) >= familia.chanceDeCasarPorEstacaoEmPorcento) {
            continue;
        }
        MembroDaDinastia conjuge = criarMembro(dinastia, cultura, familia, !solteiro.ehMulher, idadeDeQuemEntra(solteiro.idade, populacao, sorteio), sorteio);
        casar(dinastia.membros[posicao], conjuge);
        dinastia.membros.push_back(std::move(conjuge));  // depois do casar: o push_back pode mover a lista
    }
    // 5. As gravidezes novas (4.6, passo 2): a mulher casada, da idade de adulto até a fértil.
    for (MembroDaDinastia& mulher : dinastia.membros) {
        if (!mulher.ehMulher || mulher.conjuge < 0 || mulher.estacoesDeGravidez > 0 || mulher.idade < populacao.idadeAdulto || mulher.idade > familia.fertilAte) {
            continue;
        }
        if (static_cast<int>(dinastia.membros.size()) >= familia.membrosNoMaximo) {
            break;
        }
        if (sorteio.sortearNumeroInteiroEntre(0, 99) < familia.chanceDeFilhoPorEstacaoEmPorcento) {
            mulher.estacoesDeGravidez = familia.gravidezEmEstacoes;
        }
    }
    // 6. Os nobres que morreram são repostos (a crise sempre tem quem suba).
    reporOsNobres(dinastia, cultura, familia, populacao, sorteio);
    // 7. A sucessão (4.7).
    if (resultado.liderMorreu) {
        int herdeiro = escolherHerdeiro(dinastia, cultura.sucessao, populacao.idadeAdulto);
        if (herdeiro < 0) {
            // Crise: o nobre mais velho sobe e passa a ser da família.
            resultado.crise = true;
            MembroDaDinastia* escolhido = nullptr;
            for (MembroDaDinastia& membro : dinastia.membros) {
                if (membro.ehNobre && (escolhido == nullptr || membro.idade > escolhido->idade)) {
                    escolhido = &membro;
                }
            }
            escolhido->ehNobre = false;
            herdeiro = escolhido->numero;
            reporOsNobres(dinastia, cultura, familia, populacao, sorteio);
        }
        dinastia.numeroDoLider = herdeiro;
        resultado.nomeDoNovoLider = dinastia.lider().nome;
    }
    return resultado;
}
