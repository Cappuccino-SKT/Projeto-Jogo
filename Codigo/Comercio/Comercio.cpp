#include "Comercio/Comercio.h"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "Comercio/RegrasDoComercio.h"
#include "Reputacao/Reputacao.h"
#include "Faccao/Faccao.h"
#include "Dados/Receitas.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDeRegras = "regras.json";
const char* const identificadorDoPaoDoComercio = "item.pao";

int64_t precoQueALojaPagaAteOAlvo(int precoLocalEmDecimos, const RegrasDoComercio& regras)
{
    return arredondarMetadeParaCima(static_cast<long long>(precoLocalEmDecimos) * regras.lojaCompraEmPorcento, 100);
}

int64_t precoQueALojaPagaAcimaDoAlvo(int precoBaseEmDecimos, const RegrasDoComercio& regras)
{
    return arredondarMetadeParaCima(static_cast<long long>(precoBaseEmDecimos) * regras.acimaDoAlvoALojaPagaEmPorcentoDoBase, 100);
}

int64_t precoQueALojaCobra(int precoLocalEmDecimos, const RegrasDoComercio& regras)
{
    return arredondarMetadeParaCima(static_cast<long long>(precoLocalEmDecimos) * regras.lojaVendeEmPorcento, 100);
}

int pesoDoItem(const DadosDoJogo& dados, int numeroDoItem)
{
    return std::max(1, dados.catalogoDeItens.itemPeloNumero(numeroDoItem).pesoEmUnidades);
}

// As cidades do mundo na ordem do número do mapa: a vila entra no lugar dela.

SimulacaoDaVila* cidadeNoMapa(SimulacaoDaVila& vila, int numeroDoMapa)
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

// O que levar de "origem" para "destino" (regra 4.3, passos 1 e 2): os itens com sobra na
// origem e falta no destino, pelo maior ganho por unidade de peso, até a capacidade e o
// dinheiro. "ganhoPorUnidade" diz o ganho de cada item. Devolve a carga e soma o ganho.
template <typename Ganho>
std::vector<int> escolherACarga(const DadosDoJogo& dados, const ContextoDaProducao& origem, const ContextoDaProducao& destino,
                                const std::vector<int>& aCaminhoDoDestino, int capacidade, int64_t dinheiroParaPagar, Ganho ganhoPorUnidade,
                                const std::vector<int64_t>& custoPorUnidade, int64_t& ganhoTotal)
{
    const int itens = dados.catalogoDeItens.quantidadeDeItens();
    std::vector<int> carga(static_cast<std::size_t>(itens), 0);
    std::vector<int> candidatos;
    for (int item = 0; item < itens; ++item) {
        if (ganhoPorUnidade(item) > 0 && sobraDoItem(origem, item) > 0 && faltaDoItem(destino, aCaminhoDoDestino, item) > 0) {
            candidatos.push_back(item);
        }
    }
    // O maior ganho por peso primeiro (ganho de A × peso de B > ganho de B × peso de A); empate, o menor número.
    std::stable_sort(candidatos.begin(), candidatos.end(), [&](int um, int outro) {
        return ganhoPorUnidade(um) * pesoDoItem(dados, outro) > ganhoPorUnidade(outro) * pesoDoItem(dados, um);
    });
    int espaco = capacidade;
    for (const int item : candidatos) {
        int quantidade = std::min({sobraDoItem(origem, item), faltaDoItem(destino, aCaminhoDoDestino, item), espaco / pesoDoItem(dados, item)});
        const int64_t custo = custoPorUnidade[static_cast<std::size_t>(item)];
        if (custo > 0) {
            quantidade = static_cast<int>(std::min<int64_t>(quantidade, dinheiroParaPagar / custo));
            dinheiroParaPagar -= quantidade * custo;
        }
        if (quantidade <= 0) {
            continue;
        }
        carga[static_cast<std::size_t>(item)] = quantidade;
        espaco -= quantidade * pesoDoItem(dados, item);
        ganhoTotal += quantidade * ganhoPorUnidade(item);
    }
    return carga;
}

// Os ticks de uma rota com o clima e a estação de hoje (regra 4.3, passo 3).
int64_t ticksDaRota(int origem, const std::vector<int>& rota, const EntidadeEmTransito& entidade, const DadosDoJogo& dados,
                    const std::vector<TipoDeClima>& climas, Estacao estacao)
{
    (void)origem;
    int64_t ticks = 0;
    for (const int mapa : rota) {
        const int climaEEstacao = multiplicadorDeClimaEEstacao(climaDaRegiaoDoMapa(climas, dados.mundo, mapa), estacao, dados.mundo, mapa, dados.regrasDaViagem);
        ticks += calcularTicksDoTrecho(entidade, dados.mundo, mapa, climaEEstacao, dados.regrasDaViagem);
    }
    return ticks;
}

// A chance de perder dinheiro numa rota, em por mil (regra 4.3, passo 5).
int riscoDaRota(const std::vector<int>& rota, const EntidadeEmTransito& entidade, const DadosDoJogo& dados, const std::vector<int>& ameacaBandidaPorRegiao)
{
    const RegrasDaViagem& viagem = dados.regrasDaViagem;
    const RegrasDoComercio& comercio = dados.regrasDoComercio;
    const int pesos = std::max(1, viagem.pesoDosBandidos + viagem.pesoDoMercadorViajante);
    int64_t risco = 0;
    for (const int mapa : rota) {
        risco += static_cast<int64_t>(chanceDeEncontroPorMil(entidade, dados.mundo, mapa, viagem, ameacaBandidaPorRegiao)) * viagem.pesoDosBandidos / pesos
            * comercio.riscoChanceDePerderEmPorcento / 100 * comercio.riscoPerdaMediaEmPorcento / 100;
    }
    return static_cast<int>(std::min<int64_t>(risco, 1000));
}

void tirarDoACaminho(SimulacaoDaVila& destino, CaravanaDaCidade& caravana)
{
    for (std::size_t item = 0; item < caravana.marcadoNoDestino.size() && item < destino.aCaminhoPorItem.size(); ++item) {
        destino.aCaminhoPorItem[item] = std::max(0, destino.aCaminhoPorItem[item] - caravana.marcadoNoDestino[item]);
    }
    std::fill(caravana.marcadoNoDestino.begin(), caravana.marcadoNoDestino.end(), 0);
}

void partirPelaRota(CaravanaDaCidade& caravana, int origem, int destino, SimulacaoDaVila& casa, const SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = casa.dados;
    std::vector<int> rota = buscarRotaMaisRapida(origem, destino, caravana.entidade, dados.mundo, dados.regrasDaViagem);
    EntidadeEmTransito& entidade = caravana.entidade;
    entidade.mapaAtual = origem;
    if (rota.empty()) {
        entidade.emTransito = false;
        return;
    }
    entidade.emTransito = true;
    const int primeiro = rota.front();
    entidade.rota.assign(rota.begin() + 1, rota.end());
    entrarNoTrecho(entidade, primeiro, dados.mundo, dados.regrasDaViagem, vila.viagem.climaDeHojePorRegiao, vila.relogio.estacaoAtual(),
                   casa.sorteioDasCaravanas(), vila.viagem.ameacaBandidaPorRegiao);
}

// Regra 4.5: vende a carga, compra a volta e parte de casa.
void chegarAoDestino(SimulacaoDaVila& vila, SimulacaoDaVila& casa, CaravanaDaCidade& caravana)
{
    SimulacaoDaVila* destino = cidadeNoMapa(vila, caravana.mapaDeDestino);
    EntidadeEmTransito& entidade = caravana.entidade;
    if (destino != nullptr) {
        ContextoDaProducao loja = destino->contextoDaProducao();
        tirarDoACaminho(*destino, caravana);
        for (int item = 0; item < static_cast<int>(entidade.quantidadePorItem.size()); ++item) {
            if (entidade.quantidadePorItem[item] > 0) {
                const ResultadoDaLoja venda = venderNaLoja(loja, item, entidade.quantidadePorItem[item]);
                entidade.quantidadePorItem[item] -= venda.quantidade;
                entidade.dinheiroEmDecimos += venda.valorEmDecimos;
                vila.estatisticasDoComercio.valorVendido += venda.valorEmDecimos;
            }
        }
        // A volta, com os preços e os estoques de agora, sem passar do dinheiro da venda.
        const PlanoDaViagemDaCaravana volta = calcularLucroEsperadoDaViagem(casa, caravana, *destino);
        for (int item = 0; item < static_cast<int>(volta.volta.size()); ++item) {
            if (volta.volta[item] > 0) {
                // Fase C1 (FA1-7): na cidade da mesma facção, o ajuste de preço dela.
                const int faccaoDeCasa = faccaoDaCidade(casa.dados, casa.numeroDoMapaNoMundo());
                const int ajuste = faccaoDeCasa >= 0 && faccaoDeCasa == faccaoDaCidade(casa.dados, destino->numeroDoMapaNoMundo())
                    ? ajusteDePrecoDaFaccaoEmPorcento(casa.dados, faccaoDeCasa, item) : 0;
                const ResultadoDaLoja compra = comprarNaLoja(loja, item, volta.volta[item], entidade.dinheiroEmDecimos, false, 100 + ajuste);
                entidade.quantidadePorItem[item] += compra.quantidade;
                entidade.dinheiroEmDecimos -= compra.valorEmDecimos;
            }
        }
    }
    caravana.situacao = SituacaoDaCaravana::Voltando;
    partirPelaRota(caravana, caravana.mapaDeDestino, casa.numeroDoMapaNoMundo(), casa, vila);
}

// Regra 4.6: descarrega, devolve o dinheiro e paga o salário dos dias fora.
void chegarEmCasa(SimulacaoDaVila& vila, SimulacaoDaVila& casa, CaravanaDaCidade& caravana)
{
    const RegrasDoComercio& regras = casa.dados.regrasDoComercio;
    EntidadeEmTransito& entidade = caravana.entidade;
    for (int item = 0; item < static_cast<int>(entidade.quantidadePorItem.size()); ++item) {
        if (entidade.quantidadePorItem[item] > 0) {
            casa.inventario.tentarPor(item, DonoDoItem::Cidade, entidade.quantidadePorItem[item]);  // o que não cabe se perde
            entidade.quantidadePorItem[item] = 0;
        }
    }
    casa.dinheiro.bancoDaCidade += entidade.dinheiroEmDecimos;
    entidade.dinheiroEmDecimos = 0;
    const int64_t ticksPorDia = static_cast<int64_t>(24) * casa.contextoDaProducao().ticksPorHoraDeJogo;
    const int64_t diasFora = std::max<int64_t>(1, (vila.relogio.tickAtual() - caravana.tickDaPartida + ticksPorDia - 1) / ticksPorDia);
    const int64_t salario = diasFora * regras.pessoasDaCaravana() * regras.salarioPorDiaDaCaravanaEmDecimos;
    casa.dinheiro.bancoDaCidade -= salario;
    casa.dinheiro.dinheiroDaClasseCivil += salario;
    caravana.situacao = SituacaoDaCaravana::EmCasa;
    caravana.mapaDeDestino = 0;
    entidade.mapaAtual = casa.numeroDoMapaNoMundo();
}

// Fase B3 (ficha das Notícias): a caravana atacada vira uma notícia Local, no mapa do ataque.
void criarANoticiaDoAtaque(SimulacaoDaVila& vila, const SimulacaoDaVila& casa, const CaravanaDaCidade& caravana, const std::string& resultado)
{
    const MundoDoJogo& mundo = vila.dados.mundo;
    const CidadeDoMundo* cidade = mundo.cidadeNoMapa(casa.numeroDoMapaNoMundo());
    Noticia noticia;
    noticia.tipo = "comercio.caravana_atacada";
    noticia.importancia = ImportanciaDaNoticia::Local;
    noticia.mapaDeOrigem = caravana.entidade.mapaAtual;
    noticia.tickDeOrigem = vila.relogio.tickAtual();
    noticia.diaDeOrigem = vila.relogio.diaDaPartida();
    noticia.titulo = "Bandidos atacaram a caravana de " + (cidade != nullptr ? cidade->nome : mundo.nomeDoMapa(casa.numeroDoMapaNoMundo())) + " em "
        + mundo.nomeDoMapa(caravana.entidade.mapaAtual);
    noticia.porQue = resultado;
    criarNoticia(vila, std::move(noticia));
}

// Regra 4.11: a caravana atacada longe do jogador, pelo cálculo rápido.
void resolverOEncontroDaCaravana(SimulacaoDaVila& vila, SimulacaoDaVila& casa, CaravanaDaCidade& caravana)
{
    EntidadeEmTransito& entidade = caravana.entidade;
    const RegrasDaViagem& regras = casa.dados.regrasDaViagem;
    const RegrasDoCombate& combate = casa.dados.regrasDoCombate;
    const EncontroMarcado encontro = entidade.encontroMarcado;
    entidade.encontroMarcado = {};
    // Fase D1 (Combate, 4.8): 2 guardas e 2 condutores (Civis) contra o bando, com a Vida da caravana.
    std::vector<Combatente> daCaravana = ladoDoKit(combate, "guarda", casa.dados.regrasDoComercio.guardas, entidade.vidaEmPorcento);
    const std::vector<Combatente> condutores = ladoDoKit(combate, "civil", casa.dados.regrasDoComercio.condutores, entidade.vidaEmPorcento);
    daCaravana.insert(daCaravana.end(), condutores.begin(), condutores.end());
    if (encontro.tipo != TipoDeEncontro::Bandidos || !bandidosAtacamOLado(combate, regras, daCaravana, encontro.bandidos)) {
        return;
    }
    const std::vector<Combatente> bando = ladoDoKit(combate, "bandido", encontro.bandidos);
    entidade.ticksParadoNaLuta = regras.lutaRapidaDuraTicks;
    ++vila.estatisticasDoComercio.atacadas;
    const ResultadoDoCalculoRapido luta = resolverPorCalculoRapido(combate, daCaravana, bando, casa.sorteioDasCaravanas().sortearNumeroInteiroEntre(0, 999));
    if (luta.venceuOLadoA) {
        entidade.vidaEmPorcento = std::max(1, entidade.vidaEmPorcento - luta.perdaDeVidaDoVencedorEmPorcento);
        criarANoticiaDoAtaque(vila, casa, caravana, "os guardas venceram");
        return;
    }
    // Perde 30% + 70% × (poder do vencedor − poder do perdedor) ÷ poder do vencedor.
    const int64_t poderDoBando = poderDoLado(combate, bando, daCaravana);
    const int64_t poderDaCaravana = poderDoLado(combate, daCaravana, bando);
    const int perda = static_cast<int>(std::clamp<int64_t>(regras.perdaDeCargaDaCaravanaMinimaEmPorcento
                                                               + (100 - regras.perdaDeCargaDaCaravanaMinimaEmPorcento) * std::max<int64_t>(0, poderDoBando - poderDaCaravana)
                                                                     / std::max<int64_t>(1, poderDoBando),
                                                           0, 100));
    entidade.vidaEmPorcento = 10;
    const int64_t dinheiroAntes = entidade.dinheiroEmDecimos;
    aplicarAPerdaDoGrupo(entidade, perda);
    vila.estatisticasDoComercio.dinheiroPerdido += dinheiroAntes - entidade.dinheiroEmDecimos;
    ++vila.estatisticasDoComercio.perdidas;
    criarANoticiaDoAtaque(vila, casa, caravana, "perdeu " + std::to_string(perda) + "% da carga");
}

}  // namespace

std::vector<SimulacaoDaVila*> cidadesDoMundo(SimulacaoDaVila& vila)
{
    std::vector<SimulacaoDaVila*> cidades;
    for (const std::unique_ptr<SimulacaoDaVila>& cidade : vila.outrasCidades) {
        cidades.push_back(cidade.get());
    }
    cidades.push_back(&vila);
    std::sort(cidades.begin(), cidades.end(), [&](const SimulacaoDaVila* uma, const SimulacaoDaVila* outra) {
        return uma->numeroDoMapaNoMundo() < outra->numeroDoMapaNoMundo();
    });
    return cidades;
}

RegrasDoComercio lerRegrasDoComercio(const std::string& textoDoRegras, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDoComercio regras;
    Json documento;
    try {
        documento = Json::parse(textoDoRegras);
    } catch (const Json::parse_error&) {
        return regras;  // a leitura das regras já anotou o erro
    }
    const auto bloco = documento.find("comercio");
    if (bloco == documento.end() || !bloco->is_object()) {
        return regras;
    }
    const auto lerInteiro = [&](const Json& objeto, const char* nome, int& valor) {
        const auto campo = objeto.find(nome);
        if (campo == objeto.end()) {
            return;
        }
        if (!campo->is_number_integer() || campo->get<int>() < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDeRegras, 0, "\"comercio\": \"" + std::string(nome) + "\" precisa ser um inteiro de 0 ou mais");
            return;
        }
        valor = campo->get<int>();
    };
    lerInteiro(*bloco, "caravanas_por_posto_comercial", regras.caravanasPorPostoComercial);
    if (const auto caravana = bloco->find("caravana_da_cidade"); caravana != bloco->end() && caravana->is_object()) {
        lerInteiro(*caravana, "condutores", regras.condutores);
        lerInteiro(*caravana, "guardas", regras.guardas);
        lerInteiro(*caravana, "cavalos", regras.cavalos);
    }
    lerInteiro(*bloco, "capacidade_por_cavalo", regras.capacidadePorCavalo);
    lerInteiro(*bloco, "salario_por_dia_da_caravana_em_decimos", regras.salarioPorDiaDaCaravanaEmDecimos);
    lerInteiro(*bloco, "loja_compra_em_porcento", regras.lojaCompraEmPorcento);
    lerInteiro(*bloco, "loja_vende_em_porcento", regras.lojaVendeEmPorcento);
    lerInteiro(*bloco, "acima_do_alvo_a_loja_paga_em_porcento_do_base", regras.acimaDoAlvoALojaPagaEmPorcentoDoBase);
    lerInteiro(*bloco, "reserva_do_banco_em_decimos", regras.reservaDoBancoEmDecimos);
    lerInteiro(*bloco, "reserva_da_loja_em_dias_de_consumo", regras.reservaDaLojaEmDiasDeConsumo);
    lerInteiro(*bloco, "lucro_minimo_em_porcento_do_custo", regras.lucroMinimoEmPorcentoDoCusto);
    lerInteiro(*bloco, "risco_chance_de_perder_em_porcento", regras.riscoChanceDePerderEmPorcento);
    lerInteiro(*bloco, "risco_perda_media_em_porcento", regras.riscoPerdaMediaEmPorcento);
    lerInteiro(*bloco, "paes_por_pessoa_por_dia", regras.paesPorPessoaPorDia);
    return regras;
}

int estoqueAlvoDoItem(const ContextoDaProducao& cidade, int numeroDoItem)
{
    return static_cast<int>(estoqueAlvoDoItemEmCentesimos(cidade, numeroDoItem) / 100);
}

int sobraDoItem(const ContextoDaProducao& cidade, int numeroDoItem)
{
    return std::max(0, cidade.inventario.quantidadeDe(numeroDoItem, DonoDoItem::Cidade) - estoqueAlvoDoItem(cidade, numeroDoItem));
}

int faltaDoItem(const ContextoDaProducao& cidade, const std::vector<int>& aCaminhoPorItem, int numeroDoItem)
{
    const int aCaminho = numeroDoItem < static_cast<int>(aCaminhoPorItem.size()) ? aCaminhoPorItem[static_cast<std::size_t>(numeroDoItem)] : 0;
    return std::max(0, estoqueAlvoDoItem(cidade, numeroDoItem) - cidade.inventario.quantidadeDe(numeroDoItem, DonoDoItem::Cidade) - aCaminho);
}

ResultadoDaLoja calcularAVendaParaALoja(const ContextoDaProducao& loja, int numeroDoItem, int quantidade, int faixaEmPorcento)
{
    const RegrasDoComercio& regras = loja.dados.regrasDoComercio;
    if (loja.itensProibidos != nullptr && (*loja.itensProibidos)[static_cast<std::size_t>(numeroDoItem)]) {
        return {};  // fase C1: a loja legal não compra o item proibido
    }
    return calcularAVendaComOsNumeros(quantidade,
                                      std::max(0, estoqueAlvoDoItem(loja, numeroDoItem) - loja.inventario.quantidadeDe(numeroDoItem, DonoDoItem::Cidade)),
                                      precoLocalDoItem(loja, numeroDoItem), loja.dados.catalogoDeItens.itemPeloNumero(numeroDoItem).precoBaseEmDecimos,
                                      std::max<int64_t>(0, loja.dinheiro.bancoDaCidade - regras.reservaDoBancoEmDecimos), regras, faixaEmPorcento);
}

ResultadoDaLoja calcularAVendaComOsNumeros(int quantidade, int ateOAlvo, int precoLocalEmDecimos, int precoBaseEmDecimos, int64_t dinheiroAcimaDaReserva,
                                           const RegrasDoComercio& regras, int faixaEmPorcento)
{
    ResultadoDaLoja resultado;
    if (quantidade <= 0) {
        return resultado;
    }
    int64_t dinheiroDaLoja = dinheiroAcimaDaReserva;
    const int64_t precos[2] = {precoComAFaixa(precoQueALojaPagaAteOAlvo(precoLocalEmDecimos, regras), faixaEmPorcento),
                               precoComAFaixa(precoQueALojaPagaAcimaDoAlvo(precoBaseEmDecimos, regras), faixaEmPorcento)};
    const int quantidades[2] = {std::min(quantidade, ateOAlvo), quantidade - std::min(quantidade, ateOAlvo)};
    for (int faixa = 0; faixa < 2; ++faixa) {
        int nestaFaixa = quantidades[faixa];
        if (precos[faixa] > 0) {
            nestaFaixa = static_cast<int>(std::min<int64_t>(nestaFaixa, dinheiroDaLoja / precos[faixa]));
        }
        resultado.quantidade += nestaFaixa;
        resultado.valorEmDecimos += nestaFaixa * precos[faixa];
        dinheiroDaLoja -= nestaFaixa * precos[faixa];
        if (nestaFaixa < quantidades[faixa]) {
            break;  // o Banco chegou à reserva: a loja não compra mais hoje
        }
    }
    return resultado;
}

ResultadoDaLoja venderNaLoja(ContextoDaProducao& loja, int numeroDoItem, int quantidade, int faixaEmPorcento)
{
    // O que não cabe no armazém da Cidade não é comprado.
    for (int tentativa = quantidade; tentativa > 0; --tentativa) {
        const ResultadoDaLoja resultado = calcularAVendaParaALoja(loja, numeroDoItem, tentativa, faixaEmPorcento);
        if (resultado.quantidade == 0) {
            return {};
        }
        if (loja.inventario.tentarPor(numeroDoItem, DonoDoItem::Cidade, resultado.quantidade)) {
            loja.dinheiro.bancoDaCidade -= resultado.valorEmDecimos;
            return resultado;
        }
        tentativa = std::min(tentativa, resultado.quantidade);
    }
    return {};
}

ResultadoDaLoja calcularACompraNaLoja(const ContextoDaProducao& loja, int numeroDoItem, int quantidade, int64_t dinheiroEmDecimos, bool ehOJogador,
                                     int faixaEmPorcento)
{
    const RegrasDoComercio& regras = loja.dados.regrasDoComercio;
    ResultadoDaLoja resultado;
    int disponivel = loja.inventario.quantidadeDe(numeroDoItem, DonoDoItem::Cidade);
    if (loja.itensProibidos != nullptr && (*loja.itensProibidos)[static_cast<std::size_t>(numeroDoItem)]) {
        return resultado;  // fase C1: a loja legal não vende o item proibido
    }
    if (ehOJogador) {
        const int64_t reserva = loja.consumoMedido.consumoPrevistoDoDiaEmCentesimos(numeroDoItem) * regras.reservaDaLojaEmDiasDeConsumo / 100;
        disponivel = static_cast<int>(std::max<int64_t>(0, disponivel - reserva));
    }
    const int64_t preco = precoComAFaixa(precoQueALojaCobra(precoLocalDoItem(loja, numeroDoItem), regras), faixaEmPorcento);
    int compra = std::min(std::max(0, quantidade), disponivel);
    if (preco > 0) {
        compra = static_cast<int>(std::min<int64_t>(compra, std::max<int64_t>(0, dinheiroEmDecimos) / preco));
    }
    resultado.quantidade = compra;
    resultado.valorEmDecimos = compra * preco;
    return resultado;
}

ResultadoDaLoja comprarNaLoja(ContextoDaProducao& loja, int numeroDoItem, int quantidade, int64_t dinheiroEmDecimos, bool ehOJogador, int faixaEmPorcento)
{
    const ResultadoDaLoja resultado = calcularACompraNaLoja(loja, numeroDoItem, quantidade, dinheiroEmDecimos, ehOJogador, faixaEmPorcento);
    if (resultado.quantidade > 0 && loja.inventario.tentarTirar(numeroDoItem, DonoDoItem::Cidade, resultado.quantidade)) {
        loja.dinheiro.bancoDaCidade += resultado.valorEmDecimos;
        return resultado;
    }
    return {};
}

int forcaDaCaravana(const DadosDoJogo& dados)
{
    return dados.regrasDoComercio.condutores * dados.regrasDaViagem.forcaDe("civil") + dados.regrasDoComercio.guardas * dados.regrasDaViagem.forcaDe("guarda");
}

// Fase C1 (Facção, 4.3): a soma das tarifas de uma rota, com a carga de um valor só.
int64_t tarifasDaRota(const SimulacaoDaVila& casa, int mapaDeSaida, const std::vector<int>& rota, int64_t valorDaCarga)
{
    const int daCaravana = faccaoDaCidade(casa.dados, casa.numeroDoMapaNoMundo());
    int64_t total = 0;
    int anterior = mapaDeSaida;
    for (const int mapa : rota) {
        const int dona = faccaoDoMapa(casa.dados, mapa);
        if (dona >= 0 && dona != daCaravana && faccaoDoMapa(casa.dados, anterior) != dona && dona < static_cast<int>(casa.tarifaDasFaccoesEmPorcento.size())) {
            total += tarifaComOsNumeros(valorDaCarga, casa.tarifaDasFaccoesEmPorcento[static_cast<std::size_t>(dona)]);
        }
        anterior = mapa;
    }
    return total;
}

PlanoDaViagemDaCaravana calcularLucroEsperadoDaViagem(SimulacaoDaVila& casa, const CaravanaDaCidade& caravana, SimulacaoDaVila& destino)
{
    const DadosDoJogo& dados = casa.dados;
    const RegrasDoComercio& regras = dados.regrasDoComercio;
    const ContextoDaProducao origem = casa.contextoDaProducao();
    const ContextoDaProducao lojaDoDestino = destino.contextoDaProducao();
    PlanoDaViagemDaCaravana plano;
    plano.destino = destino.numeroDoMapaNoMundo();
    const int itens = dados.catalogoDeItens.quantidadeDeItens();

    // 1. A ida: o ganho é 90% do preço no destino menos o preço em casa.
    const std::vector<int64_t> semCusto(static_cast<std::size_t>(itens), 0);
    const auto ganhoDaIda = [&](int item) { return precoQueALojaPagaAteOAlvo(precoLocalDoItem(lojaDoDestino, item), regras) - precoLocalDoItem(origem, item); };
    plano.ida = escolherACarga(dados, origem, lojaDoDestino, destino.aCaminhoPorItem, regras.capacidadeDaCaravana(), 0, ganhoDaIda, semCusto, plano.ganhoDaIda);
    for (int item = 0; item < itens; ++item) {
        plano.valorDaVendaDaIda += plano.ida[static_cast<std::size_t>(item)] * precoQueALojaPagaAteOAlvo(precoLocalDoItem(lojaDoDestino, item), regras);
    }
    // A volta (na chegada, a carga já foi vendida): o ganho é o preço em casa menos 110% do preço
    // no destino, paga com o dinheiro da venda.
    int64_t dinheiroDaVolta = caravana.situacao == SituacaoDaCaravana::EmCasa ? plano.valorDaVendaDaIda : caravana.entidade.dinheiroEmDecimos;
    std::vector<int64_t> custoDaVolta(static_cast<std::size_t>(itens), 0);
    for (int item = 0; item < itens; ++item) {
        custoDaVolta[static_cast<std::size_t>(item)] = precoQueALojaCobra(precoLocalDoItem(lojaDoDestino, item), regras);
    }
    const auto ganhoDaVolta = [&](int item) { return precoLocalDoItem(origem, item) - custoDaVolta[static_cast<std::size_t>(item)]; };
    plano.volta = escolherACarga(dados, lojaDoDestino, origem, casa.aCaminhoPorItem, regras.capacidadeDaCaravana(), dinheiroDaVolta, ganhoDaVolta, custoDaVolta,
                                 plano.ganhoDaVolta);

    // 3 a 6. Os dias, o custo, o risco e o lucro esperado.
    const int mapaDeCasa = casa.numeroDoMapaNoMundo();
    const std::vector<int> rotaDeIda = buscarRotaMaisRapida(mapaDeCasa, plano.destino, caravana.entidade, dados.mundo, dados.regrasDaViagem);
    const std::vector<int> rotaDeVolta = buscarRotaMaisRapida(plano.destino, mapaDeCasa, caravana.entidade, dados.mundo, dados.regrasDaViagem);
    const std::vector<TipoDeClima>& climas = casa.climasDoMundo();
    const int64_t ticks = ticksDaRota(mapaDeCasa, rotaDeIda, caravana.entidade, dados, climas, casa.relogio.estacaoAtual())
        + ticksDaRota(plano.destino, rotaDeVolta, caravana.entidade, dados, climas, casa.relogio.estacaoAtual());
    const int64_t ticksDeUmDiaDeMarcha =
        static_cast<int64_t>(dados.regrasDaViagem.horaDeAcampar - dados.regrasDaViagem.horaDeRetomar) * origem.ticksPorHoraDeJogo;
    plano.dias = static_cast<int>(std::max<int64_t>(1, (ticks + ticksDeUmDiaDeMarcha - 1) / std::max<int64_t>(1, ticksDeUmDiaDeMarcha)));
    const std::optional<int> pao = dados.catalogoDeItens.procurarNumeroDoItem(identificadorDoPaoDoComercio);
    const int precoDoPao = pao ? precoLocalDoItem(origem, *pao) : 0;
    plano.custo = static_cast<int64_t>(plano.dias) * regras.pessoasDaCaravana() * (regras.salarioPorDiaDaCaravanaEmDecimos + precoDoPao);
    // Fase C1 (Facção, 4.3, passo 4): as tarifas da rota entram no custo.
    int64_t valorDaIda = 0;
    int64_t valorDaVolta = 0;
    for (int item = 0; item < itens; ++item) {
        valorDaIda += plano.ida[static_cast<std::size_t>(item)] * precoLocalDoItem(origem, item);
        valorDaVolta += plano.volta[static_cast<std::size_t>(item)] * precoLocalDoItem(lojaDoDestino, item);
    }
    plano.custo += tarifasDaRota(casa, mapaDeCasa, rotaDeIda, valorDaIda) + tarifasDaRota(casa, plano.destino, rotaDeVolta, valorDaVolta);
    std::vector<int> rotaToda = rotaDeIda;
    rotaToda.insert(rotaToda.end(), rotaDeVolta.begin(), rotaDeVolta.end());
    plano.riscoPorMil = riscoDaRota(rotaToda, caravana.entidade, dados, casa.viagem.ameacaBandidaPorRegiao);
    plano.lucroEsperado = (plano.ganhoDaIda + plano.ganhoDaVolta) * (1000 - plano.riscoPorMil) / 1000 - plano.custo;
    return plano;
}

std::vector<std::string> decidirAsViagensDasCaravanas(SimulacaoDaVila& vila)
{
    std::vector<std::string> linhas;
    const std::vector<SimulacaoDaVila*> cidades = cidadesDoMundo(vila);
    for (SimulacaoDaVila* casa : cidades) {
        const RegrasDoComercio& regras = casa->dados.regrasDoComercio;
        for (CaravanaDaCidade& caravana : casa->caravanas) {
            if (caravana.situacao != SituacaoDaCaravana::EmCasa) {
                continue;
            }
            // 1. O destino de maior lucro esperado; empate, o de menor número (a ordem da lista).
            PlanoDaViagemDaCaravana melhor;
            bool achou = false;
            for (SimulacaoDaVila* destino : cidades) {
                if (destino == casa) {
                    continue;
                }
                const PlanoDaViagemDaCaravana plano = calcularLucroEsperadoDaViagem(*casa, caravana, *destino);
                // CO1-6: só os destinos com dinheiro para pagar a venda.
                if (destino->dinheiro.bancoDaCidade < regras.reservaDoBancoEmDecimos + plano.valorDaVendaDaIda) {
                    continue;
                }
                if (!achou || plano.lucroEsperado > melhor.lucroEsperado) {
                    melhor = plano;
                    achou = true;
                }
            }
            // 2. Fica em casa se o lucro esperado não passa de 20% do custo, ou se o Banco não paga o salário.
            const int64_t salario = static_cast<int64_t>(melhor.dias) * regras.pessoasDaCaravana() * regras.salarioPorDiaDaCaravanaEmDecimos;
            if (!achou || melhor.lucroEsperado * 100 < melhor.custo * regras.lucroMinimoEmPorcentoDoCusto
                || casa->dinheiro.bancoDaCidade < regras.reservaDoBancoEmDecimos + salario) {
                continue;
            }
            // 3. Sai: a carga e a comida saem do estoque; a carga marca o a caminho do destino.
            SimulacaoDaVila* destino = cidadeNoMapa(vila, melhor.destino);
            caravana.marcadoNoDestino.assign(melhor.ida.size(), 0);
            for (int item = 0; item < static_cast<int>(melhor.ida.size()); ++item) {
                const int quantidade = melhor.ida[static_cast<std::size_t>(item)];
                if (quantidade > 0 && casa->inventario.tentarTirar(item, DonoDoItem::Cidade, quantidade)) {
                    caravana.entidade.quantidadePorItem[static_cast<std::size_t>(item)] += quantidade;
                    caravana.marcadoNoDestino[static_cast<std::size_t>(item)] = quantidade;
                    destino->aCaminhoPorItem[static_cast<std::size_t>(item)] += quantidade;
                }
            }
            if (const std::optional<int> pao = casa->dados.catalogoDeItens.procurarNumeroDoItem(identificadorDoPaoDoComercio)) {
                const int comida = std::min(melhor.dias * regras.pessoasDaCaravana() * regras.paesPorPessoaPorDia, casa->inventario.quantidadeDe(*pao, DonoDoItem::Cidade));
                casa->inventario.tentarTirar(*pao, DonoDoItem::Cidade, comida);
            }
            caravana.mapaDeDestino = melhor.destino;
            caravana.situacao = SituacaoDaCaravana::Indo;
            caravana.tickDaPartida = vila.relogio.tickAtual();
            partirPelaRota(caravana, casa->numeroDoMapaNoMundo(), melhor.destino, *casa, vila);
            ++vila.estatisticasDoComercio.viagens;
            vila.estatisticasDoComercio.valorDeIdaEsperado += melhor.valorDaVendaDaIda;
        }
    }
    return linhas;
}

void avancarAsCaravanasUmTick(SimulacaoDaVila& vila)
{
    const int hora = vila.relogio.horaDoDiaAtual();
    const Estacao estacao = vila.relogio.estacaoAtual();
    for (SimulacaoDaVila* casa : cidadesDoMundo(vila)) {
        for (CaravanaDaCidade& caravana : casa->caravanas) {
            if (caravana.situacao == SituacaoDaCaravana::EmCasa) {
                continue;
            }
            if (vila.relogio.comecouUmaHoraNova()) {
                recuperarAVidaDaHora(caravana.entidade, hora, casa->dados.regrasDaViagem);  // fase D1
            }
            const int mapaAntes = caravana.entidade.mapaAtual;
            const PassoDaEntidade passo = andarUmTick(caravana.entidade, casa->dados.mundo, casa->dados.regrasDaViagem, vila.viagem.climaDeHojePorRegiao, hora,
                                                      estacao, casa->contextoDaProducao().ticksPorHoraDeJogo, casa->sorteioDasCaravanas(), vila.viagem.ameacaBandidaPorRegiao);
            if (caravana.entidade.mapaAtual != mapaAntes) {
                cobrarTarifa(vila, *casa, caravana, mapaAntes, caravana.entidade.mapaAtual);  // fase C1 (Facção, 4.3)
            }
            if (passo == PassoDaEntidade::EncontroNaMetade) {
                resolverOEncontroDaCaravana(vila, *casa, caravana);
            } else if (passo == PassoDaEntidade::ChegouAoDestino) {
                if (caravana.situacao == SituacaoDaCaravana::Indo) {
                    chegarAoDestino(vila, *casa, caravana);
                } else {
                    chegarEmCasa(vila, *casa, caravana);
                }
            }
        }
    }
}

int64_t dinheiroDoMundo(const SimulacaoDaVila& vila)
{
    const auto daCidade = [](const SimulacaoDaVila& cidade) {
        int64_t total = cidade.dinheiro.dinheiroTotal();
        for (const CaravanaDaCidade& caravana : cidade.caravanas) {
            total += caravana.entidade.dinheiroEmDecimos;
        }
        return total;
    };
    int64_t total = daCidade(vila) + vila.viagem.grupoDoJogador.dinheiroEmDecimos;
    for (const int64_t banco : vila.faccoes.bancoDaFaccao) {
        total += banco;  // fase C1: o Banco da Facção
    }
    for (const std::unique_ptr<SimulacaoDaVila>& cidade : vila.outrasCidades) {
        total += daCidade(*cidade);
    }
    return total;
}

std::string resumoDoComercio(const SimulacaoDaVila& vila)
{
    const EstatisticasDoComercio& estatisticas = vila.estatisticasDoComercio;
    const int64_t perdaEmPorMil = estatisticas.valorDeIdaEsperado > 0 ? estatisticas.dinheiroPerdido * 1000 / estatisticas.valorDeIdaEsperado : 0;
    return "Comércio: " + std::to_string(estatisticas.viagens) + " viagens de caravana, " + std::to_string(estatisticas.atacadas) + " atacadas, "
        + std::to_string(estatisticas.perdidas) + " perderam a luta; vendido " + escreverDinheiroEmMoedas(static_cast<int>(estatisticas.valorVendido))
        + " moedas; dinheiro perdido para bandidos: " + std::to_string(perdaEmPorMil / 10) + "," + std::to_string(perdaEmPorMil % 10)
        + "% do valor levado.";
}

void anotarOsPrecosDaCidade(SimulacaoDaVila& vila, int numeroDoMapa)
{
    SimulacaoDaVila* cidade = cidadeNoMapa(vila, numeroDoMapa);
    if (cidade == nullptr || numeroDoMapa < 1 || numeroDoMapa > static_cast<int>(vila.viagem.precosConhecidos.size())) {
        return;
    }
    PrecosConhecidos& precos = vila.viagem.precosConhecidos[static_cast<std::size_t>(numeroDoMapa - 1)];
    precos.diaEmQueFoiVisto = vila.relogio.diaDaPartida();
    const ContextoDaProducao contexto = cidade->contextoDaProducao();
    precos.precoPorItemEmDecimos.clear();
    for (int item = 0; item < vila.dados.catalogoDeItens.quantidadeDeItens(); ++item) {
        precos.precoPorItemEmDecimos.push_back(precoLocalDoItem(contexto, item));
    }
}

std::string comerciarNaCidadeDoGrupo(SimulacaoDaVila& vila, int numeroDoItem, int quantidade, bool comprar, std::vector<std::string>& linhas)
{
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    if (grupo.emTransito || grupoEstaNaVila(vila.viagem, vila.dados)) {
        return "o grupo precisa estar parado numa outra cidade";
    }
    SimulacaoDaVila* cidade = cidadeNoMapa(vila, grupo.mapaAtual);
    if (cidade == nullptr) {
        return "não há cidade neste mapa";
    }
    if (numeroDoItem < 0 || numeroDoItem >= vila.dados.catalogoDeItens.quantidadeDeItens() || quantidade <= 0) {
        return "a quantidade ou o item não vale";
    }
    // Fase B4 (Reputação, 4.4): a faixa do jogador com o dono da cidade.
    const FaixaDaReputacao& faixa = faixaDoJogadorNaCidade(vila, grupo.mapaAtual);
    if (faixa.compraEmPorcento < 0 || faixa.vendaEmPorcento < 0) {
        return "a loja se recusa a negociar com você";
    }
    ContextoDaProducao loja = cidade->contextoDaProducao();
    const std::string nome = vila.dados.tabelaDeTextos.textoDe(vila.dados.catalogoDeItens.itemPeloNumero(numeroDoItem).identificadorDeTexto);
    ResultadoDaLoja resultado;
    if (comprar) {
        resultado = comprarNaLoja(loja, numeroDoItem, quantidade, grupo.dinheiroEmDecimos, true, faixa.compraEmPorcento);
        grupo.quantidadePorItem[static_cast<std::size_t>(numeroDoItem)] += resultado.quantidade;
        grupo.dinheiroEmDecimos -= resultado.valorEmDecimos;
    } else {
        const int temNoGrupo = std::min(quantidade, grupo.quantidadePorItem[static_cast<std::size_t>(numeroDoItem)]);
        resultado = venderNaLoja(loja, numeroDoItem, temNoGrupo, faixa.vendaEmPorcento);
        grupo.quantidadePorItem[static_cast<std::size_t>(numeroDoItem)] -= resultado.quantidade;
        grupo.dinheiroEmDecimos += resultado.valorEmDecimos;
    }
    if (resultado.quantidade == 0) {
        return comprar ? "a loja não vende (sem estoque acima da reserva, ou falta dinheiro)" : "a loja não compra (o grupo não tem, ou o Banco está na reserva)";
    }
    anotarOsPrecosDaCidade(vila, grupo.mapaAtual);
    linhas.push_back(std::string("Comércio: o grupo ") + (comprar ? "comprou " : "vendeu ") + std::to_string(resultado.quantidade) + " " + nome + " por "
                     + escreverDinheiroEmMoedas(static_cast<int>(resultado.valorEmDecimos)) + " moedas em " + vila.dados.mundo.nomeDoMapa(grupo.mapaAtual) + ".");
    return "";
}
