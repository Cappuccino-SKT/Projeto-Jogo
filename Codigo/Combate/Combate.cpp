#include "Combate/Combate.h"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "Dados/ProblemasDosDados.h"
#include "Tempo/SorteioComSemente.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDoCombate = "combate.json";

int vantagemEmPorcento(const RegrasDoCombate& regras, const std::string& classeDaArma, const std::string& peso)
{
    const auto daArma = regras.vantagemEmPorcento.find(classeDaArma);
    if (daArma == regras.vantagemEmPorcento.end()) {
        return 0;
    }
    const auto contra = daArma->second.find(peso);
    return contra == daArma->second.end() ? 0 : contra->second;
}

int multiplicadorDoRank(const RegrasDoCombate& regras, int rank)
{
    return 100 + regras.danoPorRankEmPorcento * (std::max(1, rank) - 1);
}

// O peso mais comum de um lado (cada membro pelo peso da sua armadura; empate: Média).
std::string pesoMaisComumDoLado(const std::vector<Combatente>& lado)
{
    std::map<std::string, int> contagem;
    for (const Combatente& combatente : lado) {
        ++contagem[classeDePesoDaArmadura(combatente.armadura)];
    }
    std::string melhor = "media";
    int maior = -1;
    bool empate = false;
    for (const auto& [peso, quantos] : contagem) {
        if (quantos > maior) {
            melhor = peso;
            maior = quantos;
            empate = false;
        } else if (quantos == maior) {
            empate = true;
        }
    }
    return empate ? "media" : melhor;
}

}  // namespace

const DefinicaoDaArma& RegrasDoCombate::arma(const std::string& id) const
{
    static const DefinicaoDaArma soco;
    const auto encontrada = armas.find(id);
    return encontrada == armas.end() ? soco : encontrada->second;
}

RegrasDoCombate lerArquivoDoCombate(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDoCombate regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDoCombate, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    regras.vidaMaxima = documento.value("vida_maxima", regras.vidaMaxima);
    const Json armas = documento.value("armas", Json::object());
    for (const auto& [id, objeto] : armas.items()) {
        DefinicaoDaArma arma;
        const std::vector<int> dano = objeto.value("dano", std::vector<int>{2, 4});
        if (dano.size() != 2 || dano[0] > dano[1] || dano[0] < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDoCombate, 0, "a arma \"" + id + "\" precisa de \"dano\": [mínimo, máximo]");
            continue;
        }
        arma.danoMinimo = dano[0];
        arma.danoMaximo = dano[1];
        arma.intervaloEmTicks = std::max(1, objeto.value("intervalo_em_ticks", arma.intervaloEmTicks));
        arma.alcanceEmTiles = objeto.value("alcance_em_tiles", arma.alcanceEmTiles);
        arma.habilidade = objeto.value("habilidade", arma.habilidade);
        arma.classe = objeto.value("classe", std::string());
        regras.armas[id] = arma;
    }
    regras.danoPorRankEmPorcento = documento.value("dano_por_rank_em_porcento", regras.danoPorRankEmPorcento);
    regras.defesaPorPecaRank1 = documento.value("defesa_por_peca_rank_1", std::map<std::string, int>{{"leve", 1}, {"media", 2}, {"pesada", 3}});
    regras.vantagemEmPorcento = documento.value("vantagem_em_porcento", std::map<std::string, std::map<std::string, int>>{});
    const Json acerto = documento.value("acerto_a_distancia", Json::object());
    regras.acertoBase = acerto.value("base", regras.acertoBase);
    regras.acertoPorTileAcimaDe3 = acerto.value("por_tile_acima_de_3", regras.acertoPorTileAcimaDe3);
    regras.acertoPorPontoDeHabilidadeEmDecimos = acerto.value("por_ponto_de_habilidade_acima_de_50_em_decimos", regras.acertoPorPontoDeHabilidadeEmDecimos);
    const std::vector<int> limites = acerto.value("limites", std::vector<int>{10, 95});
    if (limites.size() == 2) {
        regras.acertoMinimo = limites[0];
        regras.acertoMaximo = limites[1];
    }
    regras.acertoNaChuva = acerto.value("chuva", regras.acertoNaChuva);
    regras.acertoNaTempestade = acerto.value("tempestade", regras.acertoNaTempestade);
    const Json medo = documento.value("medo", Json::object());
    regras.medoPorVidaPerdidaEmDecimos = medo.value("por_vida_perdida_em_decimos", regras.medoPorVidaPerdidaEmDecimos);
    regras.medoPorAliadoCaido = medo.value("por_aliado_caido", regras.medoPorAliadoCaido);
    regras.medoInimigoDuasVezesMaisForte = medo.value("inimigo_2_vezes_mais_forte", regras.medoInimigoDuasVezesMaisForte);
    regras.coragemPadrao = medo.value("coragem_padrao", regras.coragemPadrao);
    regras.rendicaoAcimaDaCoragem = medo.value("rendicao_acima_da_coragem", regras.rendicaoAcimaDaCoragem);
    regras.evolucaoPorGolpeEmCentesimos = documento.value("evolucao_por_golpe_em_centesimos", regras.evolucaoPorGolpeEmCentesimos);
    regras.perdaDoGrupoDerrotadoEmPorcento = documento.value("perda_do_grupo_derrotado_em_porcento", regras.perdaDoGrupoDerrotadoEmPorcento);
    regras.vidaMaximaPerdidaPeloVencedorEmPorcento = documento.value("vida_maxima_perdida_pelo_vencedor_em_porcento", regras.vidaMaximaPerdidaPeloVencedorEmPorcento);
    const Json kits = documento.value("kits", Json::object());
    for (const auto& [id, objeto] : kits.items()) {
        KitDeCombate kit;
        kit.arma = objeto.value("arma", kit.arma);
        kit.armadura = objeto.value("armadura", std::vector<std::string>{});
        kit.habilidade = objeto.value("habilidade", kit.habilidade);
        if (regras.armas.find(kit.arma) == regras.armas.end()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDoCombate, 0, "o kit \"" + id + "\" usa a arma \"" + kit.arma + "\", que não existe");
        }
        regras.kits[id] = kit;
    }
    for (const char* necessario : {"jogador", "civil", "bandido", "guarda"}) {
        if (regras.kits.find(necessario) == regras.kits.end()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDoCombate, 0, std::string("falta o kit \"") + necessario + "\"");
        }
    }
    return regras;
}

Combatente combatenteDoKit(const RegrasDoCombate& regras, const std::string& kit, int vidaEmPorcento)
{
    Combatente combatente;
    const auto encontrado = regras.kits.find(kit);
    if (encontrado != regras.kits.end()) {
        combatente.arma = encontrado->second.arma;
        combatente.armadura = encontrado->second.armadura;
        combatente.habilidadeEmCentesimos = encontrado->second.habilidade * 100;
    }
    combatente.vida = regras.vidaMaxima * std::clamp(vidaEmPorcento, 0, 100) / 100;
    combatente.coragem = regras.coragemPadrao;
    return combatente;
}

std::vector<Combatente> ladoDoKit(const RegrasDoCombate& regras, const std::string& kit, int quantos, int vidaEmPorcento)
{
    return std::vector<Combatente>(static_cast<std::size_t>(std::max(0, quantos)), combatenteDoKit(regras, kit, vidaEmPorcento));
}

std::string classeDePesoDaArmadura(const std::vector<std::string>& armadura)
{
    if (armadura.empty()) {
        return "leve";  // CB1-2
    }
    std::map<std::string, int> contagem;
    for (const std::string& peca : armadura) {
        ++contagem[peca];
    }
    std::string melhor = "media";
    int maior = -1;
    bool empate = false;
    for (const auto& [peso, quantos] : contagem) {
        if (quantos > maior) {
            melhor = peso;
            maior = quantos;
            empate = false;
        } else if (quantos == maior) {
            empate = true;
        }
    }
    return empate ? "media" : melhor;
}

int defesaDaArmaduraEmCentesimos(const RegrasDoCombate& regras, const Combatente& combatente)
{
    int defesa = 0;
    for (const std::string& peca : combatente.armadura) {
        const auto valor = regras.defesaPorPecaRank1.find(peca);
        defesa += valor == regras.defesaPorPecaRank1.end() ? 0 : valor->second * multiplicadorDoRank(regras, combatente.rank);
    }
    return defesa;
}

int calcularDanoDoGolpe(const RegrasDoCombate& regras, const Combatente& atacante, const Combatente& alvo, int danoSorteado)
{
    const DefinicaoDaArma& arma = regras.arma(atacante.arma);
    const int vantagem = vantagemEmPorcento(regras, arma.classe, classeDePesoDaArmadura(alvo.armadura));
    // Em décimos de milésimo: dano × Rank% × habilidade (centésimos) ÷ 5000 × (100 + vantagem)%.
    const int64_t numerador = static_cast<int64_t>(danoSorteado) * multiplicadorDoRank(regras, atacante.rank) * atacante.habilidadeEmCentesimos * (100 + vantagem);
    const int64_t denominador = 100LL * 5000 * 100;
    const int64_t defesa = defesaDaArmaduraEmCentesimos(regras, alvo);  // em centésimos
    return static_cast<int>(std::max<int64_t>(1, (numerador * 100 - defesa * denominador) / (denominador * 100)));
}

int danoMedioEmCentesimos(const RegrasDoCombate& regras, const Combatente& atacante, const std::string& pesoDoAlvo, int defesaDoAlvoEmCentesimos)
{
    const DefinicaoDaArma& arma = regras.arma(atacante.arma);
    const int vantagem = vantagemEmPorcento(regras, arma.classe, pesoDoAlvo);
    const int64_t medioEmCentesimos = (arma.danoMinimo + arma.danoMaximo) * 50LL;
    const int64_t dano = medioEmCentesimos * multiplicadorDoRank(regras, atacante.rank) / 100 * atacante.habilidadeEmCentesimos / 5000 * (100 + vantagem) / 100;
    return static_cast<int>(std::max<int64_t>(100, dano - defesaDoAlvoEmCentesimos));
}

int chanceDeAcertoADistancia(const RegrasDoCombate& regras, int habilidade, int distanciaEmTiles, bool chuva, bool tempestade)
{
    int chance = regras.acertoBase + regras.acertoPorTileAcimaDe3 * std::max(0, distanciaEmTiles - 3) + (habilidade - 50) * regras.acertoPorPontoDeHabilidadeEmDecimos / 10;
    chance += chuva ? regras.acertoNaChuva : 0;
    chance += tempestade ? regras.acertoNaTempestade : 0;
    return std::clamp(chance, regras.acertoMinimo, regras.acertoMaximo);
}

int calcularMedo(const RegrasDoCombate& regras, int vidaEmPorcento, int aliadosCaidos, bool inimigoDuasVezesMaisForte)
{
    return (100 - std::clamp(vidaEmPorcento, 0, 100)) * regras.medoPorVidaPerdidaEmDecimos / 10 + regras.medoPorAliadoCaido * aliadosCaidos
        + (inimigoDuasVezesMaisForte ? regras.medoInimigoDuasVezesMaisForte : 0);
}

int evoluirHabilidade(const RegrasDoCombate& regras, int habilidadeEmCentesimos)
{
    // 10 × (100 − habilidade) ÷ 100 centésimos: com 20, sobe 8.
    return std::min(10000, habilidadeEmCentesimos + regras.evolucaoPorGolpeEmCentesimos * (100 - habilidadeEmCentesimos / 100) / 100);
}

int chanceDeSerVistoPorMil(int testemunhas, bool noite)
{
    if (testemunhas <= 0) {
        return 0;
    }
    if (!noite) {
        return 1000;
    }
    int ninguemVe = 1000;
    for (int testemunha = 0; testemunha < testemunhas; ++testemunha) {
        ninguemVe = ninguemVe * 3 / 10;  // cada uma deixa de ver com 30%
    }
    return 1000 - ninguemVe;
}

int64_t poderDoLado(const RegrasDoCombate& regras, const std::vector<Combatente>& lado, const std::vector<Combatente>& ladoOposto)
{
    if (lado.empty()) {
        return 0;
    }
    const std::string peso = ladoOposto.empty() ? "leve" : pesoMaisComumDoLado(ladoOposto);
    int64_t defesaMedia = 0;
    for (const Combatente& outro : ladoOposto) {
        defesaMedia += defesaDaArmaduraEmCentesimos(regras, outro);
    }
    defesaMedia = ladoOposto.empty() ? 0 : defesaMedia / static_cast<int64_t>(ladoOposto.size());
    int64_t danoPorTickEmMilesimos = 0;
    int64_t vida = 0;
    for (const Combatente& combatente : lado) {
        danoPorTickEmMilesimos += static_cast<int64_t>(danoMedioEmCentesimos(regras, combatente, peso, static_cast<int>(defesaMedia))) * 10 / regras.arma(combatente.arma).intervaloEmTicks;
        vida += combatente.vida;
    }
    return danoPorTickEmMilesimos * vida;
}

ResultadoDoCalculoRapido resolverPorCalculoRapido(const RegrasDoCombate& regras, const std::vector<Combatente>& ladoA, const std::vector<Combatente>& ladoB,
                                                  uint32_t sementeDaLuta)
{
    ResultadoDoCalculoRapido resultado;
    if (ladoA.empty() || ladoB.empty()) {
        resultado.venceuOLadoA = !ladoA.empty();
        return resultado;
    }
    SorteioComSemente sorteio(sementeDaLuta, 1);
    const auto sortear = [&](int minimo, int maximo) { return sorteio.sortearNumeroInteiroEntre(minimo, maximo); };
    const ResultadoDoDuelo duelo = simularOCombate(regras, ladoA, ladoB, true, sortear);
    resultado.venceuOLadoA = duelo.venceuOLadoA;
    // A perda do vencedor: a Vida que o lado tinha menos a que sobrou, em % da Vida máxima de cada membro.
    const std::vector<Combatente>& antes = duelo.venceuOLadoA ? ladoA : ladoB;
    const std::vector<int>& depois = duelo.venceuOLadoA ? duelo.vidaDoLadoA : duelo.vidaDoLadoB;
    int64_t vidaPerdida = 0;
    for (std::size_t posicao = 0; posicao < antes.size(); ++posicao) {
        vidaPerdida += antes[posicao].vida - depois[posicao];
    }
    const int64_t perdaEmPorcento = vidaPerdida * 100 / (static_cast<int64_t>(regras.vidaMaxima) * static_cast<int64_t>(antes.size()));
    resultado.perdaDeVidaDoVencedorEmPorcento = static_cast<int>(std::clamp<int64_t>(perdaEmPorcento, 0, regras.vidaMaximaPerdidaPeloVencedorEmPorcento));
    return resultado;
}

int chanceDeVencerEmPorMil(const RegrasDoCombate& regras, const std::vector<Combatente>& ladoA, const std::vector<Combatente>& ladoB, int lutas)
{
    if (lutas <= 0) {
        return 0;
    }
    int vitorias = 0;
    for (int luta = 0; luta < lutas; ++luta) {
        vitorias += resolverPorCalculoRapido(regras, ladoA, ladoB, static_cast<uint32_t>(luta)).venceuOLadoA ? 1 : 0;
    }
    return vitorias * 1000 / lutas;
}

ResultadoDoDuelo simularOCombate(const RegrasDoCombate& regras, std::vector<Combatente> ladoA, std::vector<Combatente> ladoB, bool comMedo,
                                 const std::function<int(int, int)>& sortear)
{
    // Cada combatente: fora (caído ou fugido) e os ticks até o próximo golpe (todos golpeiam no tick 0).
    std::vector<bool> foraA(ladoA.size(), false);
    std::vector<bool> foraB(ladoB.size(), false);
    std::vector<int> esperaA(ladoA.size(), 0);
    std::vector<int> esperaB(ladoB.size(), 0);
    ResultadoDoDuelo resultado;
    const auto algumDentro = [](const std::vector<bool>& fora) { return std::find(fora.begin(), fora.end(), false) != fora.end(); };
    const auto golpear = [&](std::vector<Combatente>& atacantes, std::vector<bool>& foraDosAtacantes, std::vector<int>& espera, std::vector<Combatente>& alvos,
                             std::vector<bool>& foraDosAlvos, bool ehOLadoA) {
        for (std::size_t atacante = 0; atacante < atacantes.size(); ++atacante) {
            if (foraDosAtacantes[atacante] || espera[atacante]-- > 0) {
                continue;
            }
            // O alvo: o primeiro do outro lado ainda na luta (e com Vida).
            std::size_t alvo = 0;
            while (alvo < alvos.size() && (foraDosAlvos[alvo] || alvos[alvo].vida <= 0)) {
                ++alvo;
            }
            if (alvo >= alvos.size()) {
                return;
            }
            const DefinicaoDaArma& arma = regras.arma(atacantes[atacante].arma);
            alvos[alvo].vida -= calcularDanoDoGolpe(regras, atacantes[atacante], alvos[alvo], sortear(arma.danoMinimo, arma.danoMaximo));
            resultado.golpesDoLadoA += ehOLadoA ? 1 : 0;
            alvos[alvo].vida = std::max(0, alvos[alvo].vida);
            espera[atacante] = arma.intervaloEmTicks - 1;
        }
    };
    const auto vivos = [](const std::vector<Combatente>& lado, const std::vector<bool>& fora) {
        std::vector<Combatente> dentro;
        for (std::size_t posicao = 0; posicao < lado.size(); ++posicao) {
            if (!fora[posicao]) {
                dentro.push_back(lado[posicao]);
            }
        }
        return dentro;
    };
    const auto pensarNoMedo = [&](std::vector<Combatente>& lado, std::vector<bool>& fora, const std::vector<Combatente>& outroLado, const std::vector<bool>& foraDoOutro) {
        const std::vector<Combatente> meus = vivos(lado, fora);
        const std::vector<Combatente> deles = vivos(outroLado, foraDoOutro);
        const bool maisFortes = poderDoLado(regras, deles, meus) > 2 * poderDoLado(regras, meus, deles);
        const int caidos = static_cast<int>(std::count(fora.begin(), fora.end(), true));
        for (std::size_t posicao = 0; posicao < lado.size(); ++posicao) {
            if (!fora[posicao] && calcularMedo(regras, lado[posicao].vida * 100 / regras.vidaMaxima, caidos, maisFortes) > lado[posicao].coragem) {
                fora[posicao] = true;  // foge (sem o mapa, sai da luta na hora)
            }
        }
    };
    for (int tick = 0; tick < 100000 && algumDentro(foraA) && algumDentro(foraB); ++tick) {
        // Os golpes do mesmo tick valem juntos: quem cai neste tick ainda golpeia (os dois lados por igual).
        golpear(ladoA, foraA, esperaA, ladoB, foraB, true);
        golpear(ladoB, foraB, esperaB, ladoA, foraA, false);
        for (std::size_t posicao = 0; posicao < ladoA.size(); ++posicao) {
            foraA[posicao] = foraA[posicao] || ladoA[posicao].vida <= 0;  // Inconsciente (4.3)
        }
        for (std::size_t posicao = 0; posicao < ladoB.size(); ++posicao) {
            foraB[posicao] = foraB[posicao] || ladoB[posicao].vida <= 0;
        }
        if (comMedo && tick % 10 == 9) {
            pensarNoMedo(ladoA, foraA, ladoB, foraB);
            pensarNoMedo(ladoB, foraB, ladoA, foraA);
        }
        resultado.ticks = tick + 1;
    }
    resultado.venceuOLadoA = algumDentro(foraA) && !algumDentro(foraB);
    for (const Combatente& combatente : ladoA) {
        resultado.vidaDoLadoA.push_back(combatente.vida);
    }
    for (const Combatente& combatente : ladoB) {
        resultado.vidaDoLadoB.push_back(combatente.vida);
    }
    return resultado;
}
