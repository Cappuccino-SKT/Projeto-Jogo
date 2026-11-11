// Os testes da cultura: a tabela da parte 8 da Ficha Lógica: Cultura (fase C2).

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Cultura/Cultura.h"
#include "Dados/DadosDoJogo.h"
#include "Npc/ComponentesDoNpc.h"
#include "Save/ArquivoDeSave.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

std::vector<int> leisIniciaisDe(const DadosDoJogo& dados, const std::string& cultura)
{
    for (const DefinicaoDaFaccao& faccao : dados.faccoes.faccoes) {
        if (faccao.cultura == cultura) {
            return faccao.leisIniciais;
        }
    }
    return {};
}

void testarOFator(const DadosDoJogo& dados)
{
    // Teste 1: cada cultura com as leis iniciais da sua facção: fator 0.
    bool todasEmZero = true;
    for (const DefinicaoDaFaccao& faccao : dados.faccoes.faccoes) {
        // Rodada LE1-2: a Arcana (Uniao Magica) comeca no extremo dela ("so magos governam"): +10.
        todasEmZero = todasEmZero && fatorDaCulturaContraAsLeis(dados, faccao.leisIniciais, faccao.cultura) == (faccao.cultura == "arcana" ? 10 : 0);
    }
    conferir(todasEmZero, "cada cultura com as leis iniciais da sua facção: fator 0; a Arcana, no extremo dela, +10 (LE1-2)");
    // Teste 2 (parte 5): Tecnocratas com a Magia em "privilégio dos magos": −12 e o extremo violado (−15).
    std::vector<int> leis = leisIniciaisDe(dados, "tecnocrata");
    leis[static_cast<std::size_t>(dados.faccoes.posicaoDaLei("magia"))] = 1;
    conferir(fatorDaCulturaContraAsLeis(dados, leis, "tecnocrata") == -27, "Tecnocratas com a Magia em privilégio: −27");
    // Teste 3 (parte 10.2): 80 Arcanos e 20 Religiosos (hostis) na Arcana: −2,8 e −2,0 = −4,8.
    const std::vector<int> arcanas = leisIniciaisDe(dados, "arcana");
    conferir(fatorDaCulturaContraAsLeis(dados, arcanas, "religiosa") == -16
                 && fatorLeisECulturaDaClasseEmDecimos(dados, arcanas, {{"arcana", 80}, {"religiosa", 20}}, "arcana") == 28,
             "a mistura (leis da LE1): Religiosos −16; a classe, com 80% de Arcanos em +10, fica em +2,8");
    // Teste 4: o teto de −30.
    conferir(fatorDaCulturaContraAsLeis(dados, arcanas, "tecnocrata") == -30, "o teto: os Tecnocratas nas leis Arcanas ficam em −30");
    conferir(dados.culturas.afinidade("arcana", "religiosa") == AfinidadeEntreCulturas::Hostil && dados.culturas.afinidade("autoritaria", "tecnocrata") == AfinidadeEntreCulturas::Afim,
             "as afinidades do Mundo: Arcana e Religiosa hostis; Autoritária e Tecnocrata afins");
    conferir(dados.culturas.afinidade("tecnocrata", "guerreira") == AfinidadeEntreCulturas::Hostil && dados.culturas.afinidade("religiosa", "tecnocrata") == AfinidadeEntreCulturas::Hostil,
             "a rodada HI3-3: a Tecnocrata é hostil à Guerreira e à Religiosa");
}

void testarOMundo(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    // 4.1: os NPCs de cada cidade têm a cultura base dela; a vila, a da região (CT1-3).
    const SimulacaoDaVila& capital = *vila.outrasCidades.front();
    bool cidadeCerta = true;
    for (const entt::entity npc : capital.npcsDaVila) {
        cidadeCerta = cidadeCerta && capital.registroDasEntidades.get<IdentidadeDoNpc>(npc).cultura == "guerreira";
    }
    const std::string daVila = vila.registroDasEntidades.get<IdentidadeDoNpc>(vila.npcsDaVila.front()).cultura;
    conferir(cidadeCerta && daVila == culturaDaRegiao(dados, dados.mundo.mapa(dados.mundo.mapaDaVila).regiao) && !daVila.empty(),
             "os NPCs da capital Guerreira são Guerreiros; os da vila, da cultura da região");
    // Teste 1 no mundo: as cidades começam com o fator 0; a vila, sem facção, também.
    int foraDeZero = 0;
    for (const auto& cidade : vila.outrasCidades) {
        // Rodada LE1-2: as cidades da Arcana começam no extremo dela (+10,0).
        const bool ehArcana = faccaoDaCidade(dados, cidade->numeroDoMapaDaCidade) >= 0
            && dados.faccoes.faccoes[static_cast<std::size_t>(faccaoDaCidade(dados, cidade->numeroDoMapaDaCidade))].cultura == "arcana";
        foraDeZero += cidade->fatorLeisECulturaEmDecimos != (ehArcana ? 100 : 0) ? 1 : 0;
    }
    conferir(foraDeZero == 0 && vila.fatorLeisECulturaEmDecimos == 0, "no começo, todas as cidades com o fator 0; as da Arcana, com +10");
    // Teste 6: em 1.000 migrantes da capital Arcana, cerca de 750 da cultura da região.
    const int mapaArcano = dados.mundo.numeroDoMapaNa(8, 1);
    int daRegiao = 0;
    for (int numero = 0; numero < 1000; ++numero) {
        daRegiao += escolherCulturaDoMigrante(dados, mapaArcano, 7, numero) == "arcana" ? 1 : 0;
    }
    std::printf("    migrantes da região: %d em 1.000\n", daRegiao);
    conferir(daRegiao >= 700 && daRegiao <= 800, "o migrante: cerca de 750 em 1.000 da cultura da região");
    // O save leva a cultura de cada NPC.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(vila, "teste", "2026-10-24");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr
                 && carregado.partida->outrasCidades.front()->registroDasEntidades.get<IdentidadeDoNpc>(carregado.partida->outrasCidades.front()->npcsDaVila.front()).cultura
                     == "guerreira",
             "salvar e carregar: a cultura dos NPCs");
}

}  // namespace

void testarCultura()
{
    std::printf("Cultura: o fator Leis × cultura, as minorias, as afinidades e o migrante (fase C2)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.dadosEstaoProntos(), "os dados do jogo, com o culturas.json completo, não têm erro");
    testarOFator(dados);
    testarOMundo(dados);
}
