// Os testes das outras cidades: a tabela da parte 8 da Ficha Lógica: Outras cidades (fase B1b).

#include <cstdio>
#include <cstdlib>
#include <string>

#include "Cidades/OutrasCidades.h"
#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Npc/ComponentesDoNpc.h"
#include "Save/ArquivoDeSave.h"
#include "Vila/SimulacaoDaVila.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

int estoqueDe(const SimulacaoDaVila& simulacao, const char* item)
{
    return simulacao.inventario.quantidadeTotalDe(*simulacao.dados.catalogoDeItens.procurarNumeroDoItem(item));
}

int construcoesDoTipo(const VilaDoJogo& vila, const std::string& tipo)
{
    int quantidade = 0;
    for (const ConstrucaoDaVila& construcao : vila.construcoes) {
        quantidade += construcao.tipo == tipo ? 1 : 0;
    }
    return quantidade;
}

void testarAsContas(const DadosDoJogo& dados)
{
    // Parte 10.2: 784 ticks presumidos; habilidade 40 (90%) e 60 (110%): 156.800 por hora;
    // 8 horas = 1.254.400 → 3 ciclos de 320.000 = 12 Pães.
    conferir(ticksPresumidosPorHora(dados, 800) == 584, "o trabalho presumido medido (teste 1): 800 × 73 ÷ 100 = 584 ticks por hora");
    const int64_t porHora = calcularProgressoDoTrabalho(784, 40, 1, 1) + calcularProgressoDoTrabalho(784, 60, 1, 1);
    conferir(porHora == 156800 && porHora * 8 / 320000 * 4 == 12, "a Padaria da parte 10.2: 156.800 por hora, 12 Pães no turno");
    conferir(sequenciaDoSorteioDaCidade(1, 2) == 112 && sequenciaDoSorteioDaCidade(48, 4) == 584, "as sequências da cidade: 100 + mapa × 10 + tipo");
}

void testarACriacao(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    conferir(vila.outrasCidades.size() == 21, "as outras cidades: 21 (as 22 do mundo menos a vila do jogador)");
    int npcsDoMundo = static_cast<int>(vila.npcsDaVila.size());
    for (const auto& cidade : vila.outrasCidades) {
        npcsDoMundo += static_cast<int>(cidade->npcsDaVila.size());
    }
    // 6 × 300 + 6 × 100 + 7 × 40 + Vila Livre do Norte 40 + Vila Livre do Sul 15 + a vila (a Vila Livre do Centro) 15 = 2.750.
    conferir(npcsDoMundo == 2750, "o mundo tem 2.750 NPCs");

    const SimulacaoDaVila& capital = *vila.outrasCidades.front();  // (1,1), a capital Guerreira: Grão escasso (50%)
    conferir(capital.numeroDoMapaDaCidade == 1 && capital.npcsDaVila.size() == 300 && capital.lodDoMapa == 2, "a primeira é a capital Guerreira, com 300 NPCs, em LOD 2");
    conferir(construcoesDoTipo(capital.vila, "construcao.cozinheiro") == 20 && construcoesDoTipo(capital.vila, "construcao.fazenda") == 40,
             "a capital: 20 vezes a Padaria da vila; com o Grão a 50%, 40 Fazendas");
    int empregados = 0;
    for (const entt::entity npc : capital.npcsDaVila) {
        empregados += capital.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho >= 0 ? 1 : 0;
    }
    conferir(empregados >= 240, "a contratação da Cidade rodou: a capital começa com pelo menos 240 empregados");

    // Mesma semente, mesmo mundo.
    SimulacaoDaVila outraVila(dados, RegrasDoTempo{});
    bool iguais = outraVila.outrasCidades.size() == vila.outrasCidades.size();
    for (std::size_t cidade = 0; iguais && cidade < vila.outrasCidades.size(); ++cidade) {
        const VilaDoJogo& uma = vila.outrasCidades[cidade]->vila;
        const VilaDoJogo& outra = outraVila.outrasCidades[cidade]->vila;
        iguais = uma.construcoes.size() == outra.construcoes.size() && uma.npcsIniciais.size() == outra.npcsIniciais.size();
        for (std::size_t npc = 0; iguais && npc < uma.npcsIniciais.size(); ++npc) {
            iguais = uma.npcsIniciais[npc].idade == outra.npcsIniciais[npc].idade && uma.npcsIniciais[npc].habilidadeBracal == outra.npcsIniciais[npc].habilidadeBracal;
        }
    }
    conferir(iguais, "mesma semente, mesmo mundo: as mesmas 21 cidades");
}

void testarOPrimeiroMes(const DadosDoJogo& dados)
{
    // 16 dias com as cidades: a vila sai igual à vila sozinha (a vila roda primeiro e as
    // cidades sorteiam com as suas sequências), e nenhuma cidade passa fome.
    SimulacaoDaVila comCidades(dados, RegrasDoTempo{});
    SimulacaoDaVila sozinha(dados, RegrasDoTempo{});
    sozinha.outrasCidades.clear();
    bool vilaIgual = true;
    while (comCidades.relogio.diaDaPartida() < 16) {
        comCidades.avancarUmTick();
        sozinha.avancarUmTick();
        if (comCidades.relogio.comecouUmDiaNovo()) {
            vilaIgual = vilaIgual && comCidades.linhaDoRelatorioDoUltimoDia == sozinha.linhaDoRelatorioDoUltimoDia;
        }
    }
    conferir(vilaIgual, "16 dias: a vila com as 21 cidades é igual à vila sozinha, dia a dia");
    int cidadesComFome = 0;
    for (const auto& cidade : comCidades.outrasCidades) {
        if (std::getenv("MOSTRAR_AS_CIDADES") != nullptr) {
            std::printf("  mapa %2d: %3zu NPCs, Pão %5d, Grão %5d, Banco %7lld, Felicidade %d, dias de fome %d\n", cidade->numeroDoMapaDaCidade,
                        cidade->npcsDaVila.size(), estoqueDe(*cidade, "item.pao"), estoqueDe(*cidade, "item.grao"),
                        static_cast<long long>(cidade->dinheiro.bancoDaCidade / 10), cidade->felicidade.felicidadeEmDecimos / 10, cidade->dinheiro.diasEmDeficitDeComida);
        }
        cidadesComFome += cidade->dinheiro.diasEmDeficitDeComida > 0 ? 1 : 0;
    }
    conferir(cidadesComFome == 0, "16 dias: nenhuma das 21 cidades está passando fome");

    // O save leva as cidades: carregar e seguir 1 dia dá as mesmas cidades.
    const std::vector<uint8_t> bytes = gravarAPartidaEmBytes(comCidades, "teste", "2026-10-21");
    ResultadoDoCarregamento carregado = carregarAPartidaDeBytes(dados, RegrasDoTempo{}, 2400, bytes);
    conferir(carregado.partida != nullptr && carregado.partida->outrasCidades.size() == 21, "salvar e carregar com as 21 cidades");
    if (carregado.partida == nullptr) {
        std::printf("    motivo: %s\n", carregado.motivoDaRecusa.c_str());
        return;
    }
    for (int tick = 0; tick < 19200; ++tick) {
        comCidades.avancarUmTick();
        carregado.partida->avancarUmTick();
    }
    bool cidadesIguais = comCidades.linhaDoRelatorioDoUltimoDia == carregado.partida->linhaDoRelatorioDoUltimoDia;
    for (std::size_t cidade = 0; cidade < comCidades.outrasCidades.size(); ++cidade) {
        const SimulacaoDaVila& antes = *comCidades.outrasCidades[cidade];
        const SimulacaoDaVila& depois = *carregado.partida->outrasCidades[cidade];
        cidadesIguais = cidadesIguais && estoqueDe(antes, "item.pao") == estoqueDe(depois, "item.pao") && antes.dinheiro.bancoDaCidade == depois.dinheiro.bancoDaCidade
            && antes.felicidade.felicidadeEmDecimos == depois.felicidade.felicidadeEmDecimos && antes.npcsDaVila.size() == depois.npcsDaVila.size();
    }
    conferir(cidadesIguais, "1 dia depois de carregar: a vila e as 21 cidades iguais às da partida que não foi salva");
}

bool pertoDe(int64_t valor, int64_t referencia, int porcentagem)
{
    const int64_t diferenca = valor > referencia ? valor - referencia : referencia - valor;
    return diferenca * 100 <= (referencia < 0 ? -referencia : referencia) * porcentagem;
}

void testarAVilaEmLod2(const DadosDoJogo& dados)
{
    // Teste 1 da ficha (a calibragem do trabalho presumido, medido em 73%): a vila em LOD 2
    // por uma estação fica a no máximo 10% da vila em LOD 0 no Pão e na Felicidade, e no
    // Banco somado ao jogador (a Carpintaria do jogador vende mais em LOD 2; as outras
    // cidades não têm construções do jogador).
    SimulacaoDaVila emLod0(dados, RegrasDoTempo{});
    SimulacaoDaVila emLod2(dados, RegrasDoTempo{});
    emLod0.outrasCidades.clear();
    emLod2.outrasCidades.clear();
    while (emLod0.relogio.diaDaPartida() < 16) {
        emLod0.avancarUmTick();
        emLod2.avancarUmTickEmLod2();
    }
    if (std::getenv("MOSTRAR_AS_CIDADES") != nullptr) {
        std::printf("  LOD 0: Pão %d, Banco %lld, Felicidade %d | LOD 2: Pão %d, Banco %lld, Felicidade %d\n", estoqueDe(emLod0, "item.pao"),
                    static_cast<long long>(emLod0.dinheiro.bancoDaCidade), emLod0.felicidade.felicidadeEmDecimos, estoqueDe(emLod2, "item.pao"),
                    static_cast<long long>(emLod2.dinheiro.bancoDaCidade), emLod2.felicidade.felicidadeEmDecimos);
    }
    conferir(pertoDe(estoqueDe(emLod2, "item.pao"), estoqueDe(emLod0, "item.pao"), 10) && pertoDe(emLod2.dinheiro.bancoDaCidade + emLod2.dinheiro.dinheiroDoJogador, emLod0.dinheiro.bancoDaCidade + emLod0.dinheiro.dinheiroDoJogador, 10)
                 && pertoDe(emLod2.felicidade.felicidadeEmDecimos, emLod0.felicidade.felicidadeEmDecimos, 10),
             "a vila em LOD 2 por 16 dias fica a 10% da vila em LOD 0 (Pão, Banco + jogador e Felicidade)");
}

}  // namespace

void testarOutrasCidades()
{
    std::printf("Outras cidades: as 21 cidades do mundo em LOD 2 (fase B1b)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    testarAsContas(dados);
    testarACriacao(dados);
    testarOPrimeiroMes(dados);
    testarAVilaEmLod2(dados);
}
