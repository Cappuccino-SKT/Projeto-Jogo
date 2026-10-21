#include "Cidades/OutrasCidades.h"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "Cidades/RegrasDasOutrasCidades.h"
#include "Npc/ComponentesDoNpc.h"
#include "Npc/RotinaDosNpcs.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDeRegras = "regras.json";
constexpr int sequenciaDaCriacaoDaCidade = 7;

int valorDoNivel(const std::vector<int>& tabela, int nivel, int padrao)
{
    if (tabela.empty()) {
        return padrao;
    }
    return tabela[static_cast<std::size_t>(std::clamp(nivel, 1, static_cast<int>(tabela.size())) - 1)];
}

// A produtividade que dimensiona uma construção Braçal de comida (Mundo, MU1-2): a menor
// entre os recursos que ela extrai e que entram numa receita de comida; 100 se não há.
// Devolve 0 se ela extrai recursos e o mapa não tem nenhum deles (Comércio, CO1-1).
int produtividadeParaDimensionar(const std::string& tipo, const MapaDoMundo& mapaDoMundo, const DadosDoJogo& dados)
{
    bool extraiAlgum = false;
    bool oMapaTemAlgum = false;
    int menor = 100;
    for (const DefinicaoDeRecurso& recurso : dados.catalogoDeRecursos.todosOsRecursos()) {
        if (recurso.construcaoQueExtrai != tipo) {
            continue;
        }
        extraiAlgum = true;
        const auto produtividade = mapaDoMundo.produtividadeDosRecursos.find(recurso.identificadorDeTexto);
        if (produtividade == mapaDoMundo.produtividadeDosRecursos.end()) {
            continue;
        }
        oMapaTemAlgum = true;
        bool entraNaComida = false;
        for (const DefinicaoDeReceita& receita : dados.catalogoDeReceitas.todasAsReceitas()) {
            const bool fazComida = dados.catalogoDeItens.itemPeloNumero(receita.numeroDoProduto).ehComida();
            for (const InsumoDaReceita& insumo : receita.insumos) {
                entraNaComida = entraNaComida || (fazComida && insumo.numeroDoItem == recurso.numeroDoItemGerado);
            }
        }
        if (entraNaComida) {
            menor = std::min(menor, produtividade->second);
        }
    }
    if (extraiAlgum && !oMapaTemAlgum) {
        return 0;
    }
    return menor;
}

}  // namespace

RegrasDasOutrasCidades lerRegrasDasOutrasCidades(const std::string& textoDoRegras, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDasOutrasCidades regras;
    Json documento;
    try {
        documento = Json::parse(textoDoRegras);
    } catch (const Json::parse_error&) {
        return regras;  // a leitura das regras já anotou o erro
    }
    const auto bloco = documento.find("outras_cidades");
    if (bloco == documento.end() || !bloco->is_object()) {
        return regras;
    }
    const auto lerInteiro = [&](const char* nome, int& valor) {
        const auto campo = bloco->find(nome);
        if (campo == bloco->end()) {
            return;
        }
        if (!campo->is_number_integer() || campo->get<int>() < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"outras_cidades\": \"" + std::string(nome) + "\" precisa ser um inteiro de 0 ou mais");
            return;
        }
        valor = campo->get<int>();
    };
    const auto lerLista = [&](const char* nome, std::vector<int>& lista) {
        const auto campo = bloco->find(nome);
        if (campo == bloco->end()) {
            return;
        }
        if (!campo->is_array() || campo->size() != 4) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"outras_cidades\": \"" + std::string(nome) + "\" precisa ser uma lista de 4 inteiros");
            return;
        }
        lista = campo->get<std::vector<int>>();
    };
    lerInteiro("trabalho_presumido_em_porcento", regras.trabalhoPresumidoEmPorcento);
    lerLista("npcs_reais_por_nivel", regras.npcsReaisPorNivel);
    lerLista("dinheiro_inicial_do_banco_por_nivel", regras.dinheiroInicialDoBancoPorNivel);
    lerInteiro("verba_da_coroa_por_npc_por_estacao_em_decimos", regras.verbaDaCoroaPorNpcPorEstacaoEmDecimos);
    lerInteiro("construcoes_de_producao_a_cada_npcs", regras.construcoesDeProducaoACadaNpcs);
    if (regras.construcoesDeProducaoACadaNpcs < 1) {
        listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"outras_cidades\": \"construcoes_de_producao_a_cada_npcs\" precisa ser 1 ou mais");
        regras.construcoesDeProducaoACadaNpcs = 15;
    }
    return regras;
}

uint64_t sequenciaDoSorteioDaCidade(int numeroDoMapa, int tipo)
{
    return 100 + static_cast<uint64_t>(numeroDoMapa) * 10 + static_cast<uint64_t>(tipo);
}

VilaDoJogo gerarAVilaDaCidade(const CidadeDoMundo& cidade, const DadosDoJogo& dados, uint64_t sementeDaPartida)
{
    const RegrasDasOutrasCidades& regras = dados.regrasDasOutrasCidades;
    const VilaDoJogo& vilaInicial = dados.vilaInicial;
    const MapaDoMundo& mapaDoMundo = dados.mundo.mapa(cidade.numeroDoMapa);
    SorteioComSemente sorteio(sementeDaPartida, sequenciaDoSorteioDaCidade(cidade.numeroDoMapa, sequenciaDaCriacaoDaCidade));

    VilaDoJogo vila;
    vila.sementeDaPartida = sementeDaPartida;
    vila.nivelDaCidade = cidade.nivel;
    vila.temPostoComercial = cidade.temPostoComercial;
    const int npcs = valorDoNivel(regras.npcsReaisPorNivel, cidade.nivel, 15);
    const int blocos = (npcs + regras.construcoesDeProducaoACadaNpcs - 1) / regras.construcoesDeProducaoACadaNpcs;

    // 1. As construções: o conjunto da vila inicial a cada 15 NPCs, todas da Cidade.
    int casasDaVila = 0;
    for (const ConstrucaoDaVila& modelo : vilaInicial.construcoes) {
        if (modelo.ehCasa()) {
            ++casasDaVila;
            continue;
        }
        const int produtividade = produtividadeParaDimensionar(modelo.tipo, mapaDoMundo, dados);
        if (produtividade == 0) {
            continue;  // uma Braçal de um recurso que o mapa não tem
        }
        const int copias = (blocos * 100 + produtividade - 1) / produtividade;
        for (int copia = 0; copia < copias; ++copia) {
            ConstrucaoDaVila construcao = modelo;
            construcao.identificador = modelo.identificador + "_" + std::to_string(copia + 1);
            construcao.numeroDoLote = -1;  // os lotes só quando o jogador entrar (4.5)
            construcao.areaDoLote = RetanguloEmTiles{};
            construcao.tileDaPorta = PosicaoDeTile{0, 0};
            construcao.donoEhOJogador = false;
            construcao.ordensDoJogador.clear();
            vila.construcoes.push_back(construcao);
        }
    }
    // 3. As casas: na mesma proporção da vila inicial (4 para 15).
    const int npcsDaVila = std::max<int>(1, static_cast<int>(vilaInicial.npcsIniciais.size()));
    const int casas = std::max(1, (npcs * casasDaVila + npcsDaVila - 1) / npcsDaVila);
    std::vector<int> indiceDasCasas;
    for (int casa = 0; casa < casas; ++casa) {
        ConstrucaoDaVila construcao;
        construcao.identificador = "casa_" + std::to_string(casa + 1);
        construcao.tipo = tipoDaCasa;
        construcao.numeroDoLote = -1;
        indiceDasCasas.push_back(static_cast<int>(vila.construcoes.size()));
        vila.construcoes.push_back(construcao);
    }

    // 2. Os NPCs: idade de 5 a 19, habilidades da faixa Civil, todos Civis e sem emprego
    //    (a contratação da Cidade roda ao montar a simulação, passo 4).
    const RegrasDaPopulacao& populacao = dados.regras.populacao;
    for (int numero = 0; numero < npcs; ++numero) {
        NpcDaVilaInicial npc;
        npc.nome = "Morador " + std::to_string(numero + 1);
        npc.classe = "civil";
        npc.indiceDaCasa = indiceDasCasas[static_cast<std::size_t>(numero % casas)];
        npc.idade = sorteio.sortearNumeroInteiroEntre(5, 19);
        npc.habilidadeBracal = sorteio.sortearNumeroInteiroEntre(populacao.habilidadeDoMigranteMinima, populacao.habilidadeDoMigranteMaxima);
        npc.habilidadeManufaturado = sorteio.sortearNumeroInteiroEntre(populacao.habilidadeDoMigranteMinima, populacao.habilidadeDoMigranteMaxima);
        vila.npcsIniciais.push_back(npc);
    }

    // 5 e 6. O estoque (o da Cidade na vila inicial, na proporção dos NPCs) e o dinheiro.
    for (const ItemDoEstoqueInicial& item : vilaInicial.estoqueInicial) {
        if (item.dono == DonoDoItem::Cidade) {
            vila.estoqueInicial.push_back(ItemDoEstoqueInicial{item.numeroDoItem, DonoDoItem::Cidade, item.quantidade * npcs / npcsDaVila});
        }
    }
    vila.bancoDaCidadeEmDecimos = valorDoNivel(regras.dinheiroInicialDoBancoPorNivel, cidade.nivel, 400) * 10;
    vila.dinheiroDoJogadorEmDecimos = 0;
    vila.dinheiroInicialPorCivilEmDecimos = vilaInicial.dinheiroInicialPorCivilEmDecimos;
    vila.verbaDaCoroaPorEstacaoEmDecimos = static_cast<int64_t>(regras.verbaDaCoroaPorNpcPorEstacaoEmDecimos) * npcs;
    return vila;
}

MapaLocal montarOMapaDaCidadeEmLod2(const MapaDoMundo& mapaDoMundo, const DadosDoJogo& dados)
{
    MapaLocal mapa;
    mapa.larguraEmTiles = 1;
    mapa.alturaEmTiles = 1;
    mapa.passagemPorTile = {1};
    const std::vector<DefinicaoDeRecurso>& recursos = dados.catalogoDeRecursos.todosOsRecursos();
    for (std::size_t recurso = 0; recurso < recursos.size(); ++recurso) {
        const auto produtividade = mapaDoMundo.produtividadeDosRecursos.find(recursos[recurso].identificadorDeTexto);
        if (produtividade == mapaDoMundo.produtividadeDosRecursos.end()) {
            continue;
        }
        AreaDeRecurso area;
        area.tipoDeRecurso = recursos[recurso].nomeDaAreaNoTiled;
        area.numeroDoRecurso = static_cast<int>(recurso);
        area.produtividadeEmPorcento = produtividade->second;
        mapa.areasDeRecurso.push_back(area);
    }
    return mapa;
}

int ticksPresumidosPorHora(const DadosDoJogo& dados, int ticksPorHoraDeJogo)
{
    return ticksPorHoraDeJogo * dados.regrasDasOutrasCidades.trabalhoPresumidoEmPorcento / 100;
}

bool empregadoPresenteNestaHora(const ContextoDaProducao& contexto, entt::entity npc, int horaQueAcabou)
{
    const RegrasDaRotina& rotina = contexto.dados.regras.vidaDosNpcs.rotinaDoCivil;
    if (contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho < 0 || contexto.registroDasEntidades.all_of<DoencaDoNpc>(npc)) {
        return false;
    }
    return descobrirPeriodoDaRotina(horaQueAcabou, rotina) == PeriodoDaRotina::Trabalhar;
}

void presumirOTrabalhoDaHora(ContextoDaProducao& contexto, int horaQueAcabou)
{
    const int ticks = ticksPresumidosPorHora(contexto.dados, contexto.ticksPorHoraDeJogo);
    for (const entt::entity npc : contexto.npcsDaVila) {
        if (empregadoPresenteNestaHora(contexto, npc, horaQueAcabou)) {
            TrabalhoDoDia& trabalho = contexto.registroDasEntidades.get<TrabalhoDoDia>(npc);
            trabalho.ticksTrabalhadosNestaHora = ticks;
            trabalho.ticksTrabalhadosHoje += ticks;
        }
    }
}
