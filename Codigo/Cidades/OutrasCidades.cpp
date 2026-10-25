#include "Cidades/OutrasCidades.h"

#include "Cultura/Cultura.h"
#include "Religiao/Religiao.h"

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
    lerInteiro("cobertura_da_comida_escassa_em_porcento", regras.coberturaDaComidaEscassaEmPorcento);
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
        // CO2-1 (opção A): onde o recurso de comida é escasso, as Braçais cobrem só 75% do
        // consumo (a cidade compra o resto); onde é normal ou de destaque, o conjunto inteiro
        // (com 150%, sobra para vender). Antes (MU1-2), × 100 ÷ produtividade deixava toda
        // cidade autossuficiente, e o comércio quase não acontecia.
        const int coberturaDoEscasso = dados.regrasDasOutrasCidades.coberturaDaComidaEscassaEmPorcento;
        const int copias = produtividade < 100 ? (blocos * coberturaDoEscasso + produtividade - 1) / produtividade : blocos;
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
    // Fase B5 (Tecnologia, TE1-1): o Laboratório em todas as capitais e nas cidades da lista.
    const RegrasDaTecnologia& tecnologia = dados.tecnologia.regras;
    const bool temLaboratorio = cidade.nivel == 4
        || std::find(tecnologia.outrasCidadesComLaboratorio.begin(), tecnologia.outrasCidadesComLaboratorio.end(), std::make_pair(mapaDoMundo.coluna, mapaDoMundo.linha))
               != tecnologia.outrasCidadesComLaboratorio.end();
    if (temLaboratorio && dados.catalogoDeConstrucoes.existeConstrucao("construcao.laboratorio")) {
        ConstrucaoDaVila laboratorio;
        laboratorio.identificador = "laboratorio_1";
        laboratorio.tipo = "construcao.laboratorio";
        laboratorio.nivel = tecnologia.nivelDoLaboratorio;
        laboratorio.numeroDoLote = -1;
        // Primeiro na lista: a contratação da Cidade preenche as vagas na ordem das construções, e
        // nas cidades sem desempregados o Laboratório ficaria vazio.
        vila.construcoes.insert(vila.construcoes.begin(), laboratorio);
    }
    // Fase C4 (Religião, RL1-1): os Templos (a Torre Arcana na Arcana) com 60% ou mais de cobertura;
    // as capitais com 2. A Tecnocrata não tem (o culto dela é o Laboratório). Primeiro na fila da
    // contratação, como o Laboratório.
    const auto fe = dados.regrasDaReligiao.fes.find(cidade.faccao);
    if (!cidade.faccao.empty() && fe != dados.regrasDaReligiao.fes.end() && fe->second.devocao != Devocao::Nenhuma) {
        const std::string tipoDoCulto = fe->second.culto == "torre_arcana" ? "construcao.torre_arcana" : "construcao.templo";
        if (dados.catalogoDeConstrucoes.existeConstrucao(tipoDoCulto)) {
            const int quantos = cidade.nivel >= 4 ? 2 : 1;
            for (int templo = 0; templo < quantos; ++templo) {
                ConstrucaoDaVila culto;
                culto.identificador = (fe->second.culto == "torre_arcana" ? "torre_arcana_" : "templo_") + std::to_string(templo + 1);
                culto.tipo = tipoDoCulto;
                culto.nivel = std::clamp(cidade.nivel, 1, 5);
                culto.numeroDoLote = -1;
                vila.construcoes.insert(vila.construcoes.begin(), culto);
            }
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
        // Fase C2 (Cultura, 4.1 e 4.4): a cultura base da cidade e os pontos de habilidade dela.
        npc.cultura = culturaBaseDaCidade(dados, cidade.numeroDoMapa);
        npc.habilidadeBracal = habilidadeComACultura(dados, npc.cultura, "bracal", npc.habilidadeBracal);
        npc.habilidadeManufaturado = habilidadeComACultura(dados, npc.cultura, "manufaturado", npc.habilidadeManufaturado);
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
    // Fase C1 (FA1-3): as cidades de facção recebem a verba do Banco da Facção; as Vilas Livres, "de fora".
    vila.verbaDaCoroaPorEstacaoEmDecimos = cidade.faccao.empty() ? static_cast<int64_t>(regras.verbaDaCoroaPorNpcPorEstacaoEmDecimos) * npcs : 0;
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
    int ticks = ticksPresumidosPorHora(contexto.dados, contexto.ticksPorHoraDeJogo);
    if (contexto.efeitosDaReligiao != nullptr) {
        ticks = ticks * contexto.efeitosDaReligiao->producaoPresumidaEmPorcento / 100;  // fase C4: o primeiro dia do festival
    }
    for (const entt::entity npc : contexto.npcsDaVila) {
        if (empregadoPresenteNestaHora(contexto, npc, horaQueAcabou)) {
            TrabalhoDoDia& trabalho = contexto.registroDasEntidades.get<TrabalhoDoDia>(npc);
            trabalho.ticksTrabalhadosNestaHora = ticks;
            trabalho.ticksTrabalhadosHoje += ticks;
        }
    }
}
