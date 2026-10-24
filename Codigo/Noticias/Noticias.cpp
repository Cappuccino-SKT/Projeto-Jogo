#include "Noticias/Noticias.h"

#include <algorithm>
#include <cstdlib>

#include <nlohmann/json.hpp>

#include "Comercio/Comercio.h"
#include "Dados/ProblemasDosDados.h"
#include "Vila/SimulacaoDaVila.h"

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDasNoticias = "noticias.json";

void lerInteiro(const Json& objeto, const char* nomeDoCampo, int& valor, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end()) {
        return;
    }
    if (!campo->is_number_integer() || campo->get<int>() < 0) {
        listaDeProblemas.anotarErro(nomeDoArquivoDasNoticias, 0, "\"" + std::string(nomeDoCampo) + "\" precisa ser um inteiro de 0 ou mais");
        return;
    }
    valor = campo->get<int>();
}

int indiceDaImportancia(ImportanciaDaNoticia importancia)
{
    return static_cast<int>(importancia);
}

bool ehImportante(ImportanciaDaNoticia importancia)
{
    return importancia == ImportanciaDaNoticia::Regional || importancia == ImportanciaDaNoticia::Mundial;
}

std::string escreverComMilhar(int64_t numero)
{
    std::string texto = std::to_string(numero);
    for (int posicao = static_cast<int>(texto.size()) - 3; posicao > 0; posicao -= 3) {
        texto.insert(static_cast<std::size_t>(posicao), ".");
    }
    return texto;
}

std::string escreverDecimos(int valorEmDecimos)
{
    return std::to_string(valorEmDecimos / 10) + "," + std::to_string(valorEmDecimos % 10);
}

// O nome da cidade do mapa ("Cidade Religiosa"), ou as coordenadas sem cidade.
std::string nomeDoLugar(const MundoDoJogo& mundo, int numeroDoMapa)
{
    if (const CidadeDoMundo* cidade = mundo.cidadeNoMapa(numeroDoMapa)) {
        return cidade->nome;
    }
    return mundo.nomeDoMapa(numeroDoMapa);
}

NoticiaQueOJogadorSabe* procurarOQueOJogadorSabe(EstadoDasNoticias& noticias, int64_t numeroDaNoticia)
{
    for (NoticiaQueOJogadorSabe& sabida : noticias.noticiasQueOJogadorSabe) {
        if (sabida.numeroDaNoticia == numeroDaNoticia) {
            return &sabida;
        }
    }
    return nullptr;
}

// Tira a notícia da lista e do que o jogador sabe (a Crônica fica).
void tirarANoticia(EstadoDasNoticias& noticias, std::size_t posicao)
{
    const int64_t numero = noticias.ativas[posicao].numeroDaNoticia;
    noticias.ativas.erase(noticias.ativas.begin() + static_cast<std::ptrdiff_t>(posicao));
    std::erase_if(noticias.noticiasQueOJogadorSabe, [&](const NoticiaQueOJogadorSabe& sabida) { return sabida.numeroDaNoticia == numero; });
}

// Regra 4.6, passo 4: a notícia de preço atualiza o preço que o jogador conhece daquele item,
// se a cidade já foi vista e a notícia é mais nova que a visita.
void anotarOPrecoDaNoticia(SimulacaoDaVila& vila, const Noticia& noticia)
{
    if (noticia.numeroDoItem < 0 || noticia.mapaDeOrigem < 1 || noticia.mapaDeOrigem > static_cast<int>(vila.viagem.precosConhecidos.size())) {
        return;
    }
    PrecosConhecidos& precos = vila.viagem.precosConhecidos[static_cast<std::size_t>(noticia.mapaDeOrigem - 1)];
    if (precos.diaEmQueFoiVisto < 0 || precos.diaEmQueFoiVisto > noticia.diaDeOrigem
        || noticia.numeroDoItem >= static_cast<int>(precos.precoPorItemEmDecimos.size())) {
        return;
    }
    precos.precoPorItemEmDecimos[static_cast<std::size_t>(noticia.numeroDoItem)] = noticia.precoEmDecimos;
}

void atualizarACronica(EstadoDasNoticias& noticias, const Noticia& noticia, const std::string& texto)
{
    for (LinhaDaCronica& linha : noticias.cronicaDoMundo) {
        if (linha.numeroDaNoticia == noticia.numeroDaNoticia) {
            linha.texto = texto;
        }
    }
}

}  // namespace

ImportanciaDaNoticia lerImportanciaDaNoticia(const std::string& texto)
{
    if (texto == "local") {
        return ImportanciaDaNoticia::Local;
    }
    if (texto == "regional") {
        return ImportanciaDaNoticia::Regional;
    }
    if (texto == "mundial") {
        return ImportanciaDaNoticia::Mundial;
    }
    return ImportanciaDaNoticia::Nenhuma;
}

RegrasDasNoticias lerArquivoDasNoticias(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas)
{
    RegrasDasNoticias regras;
    Json documento;
    try {
        documento = Json::parse(textoDoArquivo);
    } catch (const Json::parse_error& erro) {
        listaDeProblemas.anotarErro(nomeDoArquivoDasNoticias, 0, std::string("o JSON está quebrado: ") + erro.what());
        return regras;
    }
    if (!documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDasNoticias, 0, "o arquivo precisa ser um objeto { ... }");
        return regras;
    }
    lerInteiro(documento, "horas_por_mapa", regras.horasPorMapa, listaDeProblemas);
    if (const auto importancias = documento.find("importancias"); importancias != documento.end() && importancias->is_object()) {
        for (const char* nome : {"local", "regional", "mundial"}) {
            if (const auto bloco = importancias->find(nome); bloco != importancias->end() && bloco->is_object()) {
                const int indice = indiceDaImportancia(lerImportanciaDaNoticia(nome));
                lerInteiro(*bloco, "alcance_em_mapas", regras.alcanceEmMapas[indice], listaDeProblemas);
                lerInteiro(*bloco, "dias_no_quadro", regras.diasNoQuadro[indice], listaDeProblemas);
            } else {
                listaDeProblemas.anotarErro(nomeDoArquivoDasNoticias, 0, std::string("falta a importância \"") + nome + "\"");
            }
        }
    } else {
        listaDeProblemas.anotarErro(nomeDoArquivoDasNoticias, 0, "falta o objeto \"importancias\"");
    }
    lerInteiro(documento, "precisao_exata_ate_mapas", regras.precisaoExataAteMapas, listaDeProblemas);
    lerInteiro(documento, "precisao_arredondada_ate_mapas", regras.precisaoArredondadaAteMapas, listaDeProblemas);
    lerInteiro(documento, "preco_fora_da_media_em_porcento", regras.precoForaDaMediaEmPorcento, listaDeProblemas);
    lerInteiro(documento, "preco_mesma_noticia_depois_de_dias", regras.precoMesmaNoticiaDepoisDeDias, listaDeProblemas);
    lerInteiro(documento, "preco_noticias_por_cidade_por_dia", regras.precoNoticiasPorCidadePorDia, listaDeProblemas);
    lerInteiro(documento, "limite_de_noticias_ativas", regras.limiteDeNoticiasAtivas, listaDeProblemas);
    return regras;
}

int distanciaEmMapas(const MundoDoJogo& mundo, int mapaA, int mapaB)
{
    if (!mundo.mapaExiste(mapaA) || !mundo.mapaExiste(mapaB)) {
        return 999;
    }
    const MapaDoMundo& primeiro = mundo.mapa(mapaA);
    const MapaDoMundo& segundo = mundo.mapa(mapaB);
    return std::abs(primeiro.coluna - segundo.coluna) + std::abs(primeiro.linha - segundo.linha);
}

int64_t tickDeChegada(const Noticia& noticia, int numeroDoMapa, const MundoDoJogo& mundo, const RegrasDasNoticias& regras, int ticksPorHora)
{
    const int distancia = distanciaEmMapas(mundo, noticia.mapaDeOrigem, numeroDoMapa);
    if (distancia > regras.alcanceEmMapas[indiceDaImportancia(noticia.importancia)]) {
        return -1;
    }
    return noticia.tickDeOrigem + static_cast<int64_t>(distancia) * regras.horasPorMapa * ticksPorHora;
}

std::string numeroComPrecisao(int64_t numero, int distancia, const RegrasDasNoticias& regras)
{
    if (distancia <= regras.precisaoExataAteMapas || (numero < 10 && distancia <= regras.precisaoArredondadaAteMapas)) {
        return escreverComMilhar(numero);
    }
    if (distancia <= regras.precisaoArredondadaAteMapas) {
        // 1 algarismo, arredondado (a metade sobe): 1.450 → 1.000; 1.500 → 2.000.
        int64_t casa = 1;
        while (casa * 10 <= numero) {
            casa *= 10;
        }
        return "cerca de " + escreverComMilhar((numero + casa / 2) / casa * casa);
    }
    if (numero < 10) {
        return "alguns";
    }
    if (numero < 100) {
        return "dezenas de";
    }
    if (numero < 1000) {
        return "centenas de";
    }
    return "milhares de";
}

std::string textoComPrecisao(const Noticia& noticia, int distancia, const RegrasDasNoticias& regras)
{
    std::string texto = noticia.titulo;
    if (noticia.numero >= 0) {
        texto += ": " + numeroComPrecisao(noticia.numero, distancia, regras) + " " + noticia.rotuloDoNumero;
    }
    if (!noticia.porQue.empty()) {
        texto += " (" + noticia.porQue + ")";
    }
    return texto;
}

int64_t criarNoticia(SimulacaoDaVila& vila, Noticia noticia)
{
    EstadoDasNoticias& noticias = vila.noticias;
    const RegrasDasNoticias& regras = vila.dados.regrasDasNoticias;
    // Passo 2: com a lista cheia, sai a Local mais velha; sem Local, a Regional mais velha.
    if (static_cast<int>(noticias.ativas.size()) >= regras.limiteDeNoticiasAtivas && !noticias.ativas.empty()) {
        std::size_t posicao = noticias.ativas.size();
        for (const ImportanciaDaNoticia importancia : {ImportanciaDaNoticia::Local, ImportanciaDaNoticia::Regional, ImportanciaDaNoticia::Mundial}) {
            for (std::size_t indice = 0; indice < noticias.ativas.size() && posicao == noticias.ativas.size(); ++indice) {
                posicao = noticias.ativas[indice].importancia == importancia ? indice : posicao;
            }
        }
        tirarANoticia(noticias, posicao);
    }
    noticia.numeroDaNoticia = noticias.proximoNumeroDaNoticia++;
    noticia.diaEmQueSaiDoQuadro = noticia.diaDeOrigem + regras.diasNoQuadro[indiceDaImportancia(noticia.importancia)];
    noticias.ativas.push_back(std::move(noticia));
    return noticias.ativas.back().numeroDaNoticia;
}

void recolherOsAcontecimentosDoDia(SimulacaoDaVila& vila)
{
    std::vector<SimulacaoDaVila*> cidades{&vila};
    for (const std::unique_ptr<SimulacaoDaVila>& cidade : vila.outrasCidades) {
        cidades.push_back(cidade.get());
    }
    for (SimulacaoDaVila* cidade : cidades) {
        for (const AcontecimentoParaNoticia& acontecimento : cidade->eventos.acontecimentosDeHoje) {
            if (acontecimento.importancia == ImportanciaDaNoticia::Nenhuma) {
                continue;
            }
            Noticia noticia;
            noticia.tipo = acontecimento.tipo;
            noticia.importancia = acontecimento.importancia;
            noticia.mapaDeOrigem = cidade->numeroDoMapaNoMundo();
            noticia.tickDeOrigem = vila.relogio.tickAtual();
            noticia.diaDeOrigem = vila.relogio.diaDaPartida();
            noticia.titulo = acontecimento.titulo + " em " + nomeDoLugar(vila.dados.mundo, noticia.mapaDeOrigem);
            noticia.numero = acontecimento.numero;
            noticia.rotuloDoNumero = acontecimento.rotuloDoNumero;
            criarNoticia(vila, std::move(noticia));
        }
        cidade->eventos.acontecimentosDeHoje.clear();
    }
}

int escolherOItemDaNoticiaDePreco(const std::vector<int>& precosDaCidade, const std::vector<int64_t>& somaDaRegiao, int64_t cidadesDaRegiao,
                                  const std::vector<int64_t>& diaDaUltimaNoticia, int64_t hoje, const RegrasDasNoticias& regras, int64_t& diferencaEmPorcento)
{
    int melhorItem = -1;
    diferencaEmPorcento = 0;
    for (std::size_t item = 0; item < precosDaCidade.size(); ++item) {
        const int64_t soma = somaDaRegiao[item];
        const int64_t dia = diaDaUltimaNoticia[item];
        if (soma <= 0 || cidadesDaRegiao < 2 || (dia >= 0 && hoje - dia < regras.precoMesmaNoticiaDepoisDeDias)) {
            continue;
        }
        // A diferença em % da média: (preço × cidades − soma) × 100 ÷ soma.
        const int64_t diferenca = (precosDaCidade[item] * cidadesDaRegiao - soma) * 100 / soma;
        if (std::abs(diferenca) >= regras.precoForaDaMediaEmPorcento && std::abs(diferenca) > std::abs(diferencaEmPorcento)) {
            melhorItem = static_cast<int>(item);
            diferencaEmPorcento = diferenca;
        }
    }
    return melhorItem;
}

void criarNoticiasDePreco(SimulacaoDaVila& vila)
{
    const DadosDoJogo& dados = vila.dados;
    const RegrasDasNoticias& regras = dados.regrasDasNoticias;
    const MundoDoJogo& mundo = dados.mundo;
    const int quantidadeDeItens = dados.catalogoDeItens.quantidadeDeItens();
    EstadoDasNoticias& noticias = vila.noticias;
    if (noticias.diaDaUltimaNoticiaDePreco.size() != mundo.mapas.size()) {
        noticias.diaDaUltimaNoticiaDePreco.assign(mundo.mapas.size(), std::vector<int64_t>(static_cast<std::size_t>(quantidadeDeItens), -1));
    }
    const std::vector<SimulacaoDaVila*> cidades = cidadesDoMundo(vila);
    // Os preços locais de hoje, por cidade, e a soma por região (passo 1).
    std::vector<std::vector<int>> precos;
    std::vector<std::vector<int64_t>> somaPorRegiao(7, std::vector<int64_t>(static_cast<std::size_t>(quantidadeDeItens), 0));
    std::vector<int> cidadesPorRegiao(7, 0);
    for (SimulacaoDaVila* cidade : cidades) {
        const ContextoDaProducao contexto = cidade->contextoDaProducao();
        const int regiao = mundo.mapa(cidade->numeroDoMapaNoMundo()).regiao;
        std::vector<int> daCidade(static_cast<std::size_t>(quantidadeDeItens), 0);
        for (int item = 0; item < quantidadeDeItens; ++item) {
            daCidade[static_cast<std::size_t>(item)] = precoLocalDoItem(contexto, item);
            somaPorRegiao[static_cast<std::size_t>(regiao)][static_cast<std::size_t>(item)] += daCidade[static_cast<std::size_t>(item)];
        }
        ++cidadesPorRegiao[static_cast<std::size_t>(regiao)];
        precos.push_back(std::move(daCidade));
    }
    const int64_t hoje = vila.relogio.diaDaPartida();
    for (std::size_t posicao = 0; posicao < cidades.size(); ++posicao) {
        const int numeroDoMapa = cidades[posicao]->numeroDoMapaNoMundo();
        const int regiao = mundo.mapa(numeroDoMapa).regiao;
        const int64_t quantas = cidadesPorRegiao[static_cast<std::size_t>(regiao)];
        std::vector<int64_t>& ultimoDia = noticias.diaDaUltimaNoticiaDePreco[static_cast<std::size_t>(numeroDoMapa - 1)];
        for (int noticiaDoDia = 0; noticiaDoDia < regras.precoNoticiasPorCidadePorDia; ++noticiaDoDia) {
            int64_t melhorDiferenca = 0;
            const int melhorItem = escolherOItemDaNoticiaDePreco(precos[posicao], somaPorRegiao[static_cast<std::size_t>(regiao)], quantas, ultimoDia, hoje, regras,
                                                                 melhorDiferenca);
            if (melhorItem < 0) {
                break;
            }
            ultimoDia[static_cast<std::size_t>(melhorItem)] = hoje;
            Noticia noticia;
            noticia.tipo = "comercio.preco";
            noticia.importancia = ImportanciaDaNoticia::Local;
            noticia.mapaDeOrigem = numeroDoMapa;
            noticia.tickDeOrigem = vila.relogio.tickAtual();
            noticia.diaDeOrigem = hoje;
            noticia.numeroDoItem = melhorItem;
            noticia.precoEmDecimos = precos[posicao][static_cast<std::size_t>(melhorItem)];
            noticia.titulo = dados.tabelaDeTextos.textoDe(dados.catalogoDeItens.todosOsItens()[static_cast<std::size_t>(melhorItem)].identificadorDeTexto) + " em "
                + nomeDoLugar(mundo, numeroDoMapa) + ": " + escreverDecimos(noticia.precoEmDecimos);
            noticia.porQue = (melhorDiferenca > 0 ? "+" : "") + std::to_string(melhorDiferenca) + "% da média da região";
            criarNoticia(vila, std::move(noticia));
        }
    }
}

const NoticiaQueOJogadorSabe* oQueOJogadorSabe(const EstadoDasNoticias& noticias, int64_t numeroDaNoticia)
{
    return procurarOQueOJogadorSabe(const_cast<EstadoDasNoticias&>(noticias), numeroDaNoticia);
}

std::vector<std::string> conferirNoticiasDoJogador(SimulacaoDaVila& vila, const std::vector<int>& mapasDaRede, bool aoEntrarNumMapa)
{
    EstadoDasNoticias& noticias = vila.noticias;
    const RegrasDasNoticias& regras = vila.dados.regrasDasNoticias;
    const MundoDoJogo& mundo = vila.dados.mundo;
    const int ticksPorHora = vila.contextoDaProducao().ticksPorHoraDeJogo;
    const int64_t agora = vila.relogio.tickAtual();
    std::vector<std::string> linhas;
    int novasImportantes = 0;
    for (const Noticia& noticia : noticias.ativas) {
        // Passos 1 a 3: a menor distância entre os mapas da rede aonde a notícia já chegou.
        int melhorDistancia = -1;
        for (const int mapa : mapasDaRede) {
            const int64_t chegada = tickDeChegada(noticia, mapa, mundo, regras, ticksPorHora);
            const int distancia = distanciaEmMapas(mundo, noticia.mapaDeOrigem, mapa);
            const bool dentroDoAlcance = distancia <= regras.alcanceEmMapas[indiceDaImportancia(noticia.importancia)];
            if (dentroDoAlcance && chegada <= agora && (melhorDistancia < 0 || distancia < melhorDistancia)) {
                melhorDistancia = distancia;
            }
        }
        if (melhorDistancia < 0) {
            continue;
        }
        if (NoticiaQueOJogadorSabe* sabida = procurarOQueOJogadorSabe(noticias, noticia.numeroDaNoticia)) {
            if (melhorDistancia < sabida->distancia) {  // a versão mais precisa, sem aviso novo
                sabida->distancia = melhorDistancia;
                atualizarACronica(noticias, noticia, textoComPrecisao(noticia, melhorDistancia, regras));
            }
            continue;
        }
        noticias.noticiasQueOJogadorSabe.push_back({noticia.numeroDaNoticia, vila.relogio.diaDaPartida(), melhorDistancia});
        anotarOPrecoDaNoticia(vila, noticia);
        if (!ehImportante(noticia.importancia)) {
            continue;  // as Locais vão para a lista e o Mapa Mundial, sem aviso (NO1-2)
        }
        const std::string texto = textoComPrecisao(noticia, melhorDistancia, regras);
        noticias.cronicaDoMundo.push_back({noticia.numeroDaNoticia, noticia.diaDeOrigem, vila.relogio.diaDaPartida(), texto});
        // Os eventos da própria vila já avisaram pelo console dos eventos.
        if (noticia.mapaDeOrigem == mundo.mapaDaVila && noticia.tipo.rfind("evento.", 0) == 0) {
            continue;
        }
        ++novasImportantes;
        if (!aoEntrarNumMapa) {
            linhas.push_back("Notícia: " + texto + ".");
        }
    }
    if (aoEntrarNumMapa && novasImportantes > 0 && !mapasDaRede.empty()) {
        linhas.push_back("Notícia: " + std::to_string(novasImportantes) + (novasImportantes == 1 ? " notícia nova em " : " notícias novas em ")
                         + nomeDoLugar(mundo, mapasDaRede.front()) + ".");
    }
    return linhas;
}

std::vector<std::string> avancarAsNoticiasUmTick(SimulacaoDaVila& vila)
{
    EstadoDasNoticias& noticias = vila.noticias;
    std::vector<std::string> linhas;
    // A rede do jogador nesta versão: só o mapa do grupo (as caravanas do jogador ainda não existem).
    const std::vector<int> mapasDaRede{vila.viagem.grupoDoJogador.mapaAtual};
    if (vila.viagem.grupoDoJogador.mapaAtual != noticias.mapaDoJogadorNaUltimaConferencia) {
        noticias.mapaDoJogadorNaUltimaConferencia = vila.viagem.grupoDoJogador.mapaAtual;
        linhas = conferirNoticiasDoJogador(vila, mapasDaRede, true);
    }
    if (vila.relogio.comecouUmaHoraNova()) {
        // 4.7: saem as notícias que passaram do prazo.
        for (std::size_t posicao = noticias.ativas.size(); posicao-- > 0;) {
            if (noticias.ativas[posicao].diaEmQueSaiDoQuadro <= vila.relogio.diaDaPartida()) {
                tirarANoticia(noticias, posicao);
            }
        }
        const std::vector<std::string> daHora = conferirNoticiasDoJogador(vila, mapasDaRede, false);
        linhas.insert(linhas.end(), daHora.begin(), daHora.end());
    }
    return linhas;
}
