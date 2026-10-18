#include "Vila/VilaInicial.h"

#include <map>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

namespace {

const char* const nomeDoArquivoDaVila = "vila_inicial.json";

// As Casas que o catálogo não descreve (os testes antigos) ficam com 4 moradores (V2-12).
constexpr int moradoresQuandoOCatalogoNaoDiz = 4;

int ladoDoLoteEmTiles(PorteDoLote porte)
{
    switch (porte) {
    case PorteDoLote::Pequeno: return 4;
    case PorteDoLote::Medio: return 6;
    case PorteDoLote::Grande: return 8;
    }
    return 4;
}

std::string textoDoTile(PosicaoDeTile tile)
{
    return "(" + std::to_string(tile.tileX) + ", " + std::to_string(tile.tileY) + ")";
}

// Lê um texto de um objeto. Se faltar, anota o erro e devolve vazio.
std::string lerTextoObrigatorio(const Json& objeto, const char* nomeDoCampo, const std::string& ondeEsta,
                                ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end() || !campo->is_string()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, ondeEsta + ": falta o texto \"" + nomeDoCampo + "\"");
        return "";
    }
    return campo->get<std::string>();
}

// Lê um inteiro opcional entre o mínimo e o máximo. Sem o campo, fica o valor
// que já estava (o padrão). Fora da faixa, anota o erro.
void lerInteiroOpcional(const Json& objeto, const char* nomeDoCampo, int valorMinimo, int valorMaximo, int& valorLido,
                        const std::string& ondeEsta, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto campo = objeto.find(nomeDoCampo);
    if (campo == objeto.end()) {
        return;
    }
    if (!campo->is_number_integer() || campo->get<int>() < valorMinimo || campo->get<int>() > valorMaximo) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, ondeEsta + ": \"" + nomeDoCampo + "\" precisa ser um número inteiro de "
                                    + std::to_string(valorMinimo) + " a " + std::to_string(valorMaximo));
        return;
    }
    valorLido = campo->get<int>();
}

// O número interno de um item pelo ID. Sem o item, anota o erro e devolve -1.
int procurarItemObrigatorio(const std::string& identificador, const CatalogoDeItens& catalogoDeItens, const std::string& ondeEsta,
                            ListaDeProblemasDosDados& listaDeProblemas)
{
    const std::optional<int> numero = catalogoDeItens.procurarNumeroDoItem(identificador);
    if (!numero.has_value()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, ondeEsta + ": o item \"" + identificador + "\" não existe no itens.csv");
        return -1;
    }
    return *numero;
}

// As ordens da Carpintaria do jogador (FL6-3), até a interface do marco 8.
void lerOrdensDoJogador(const Json& objeto, const std::string& nome, const CatalogoDeItens& catalogoDeItens, ConstrucaoDaVila& construcao,
                        ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto ordens = objeto.find("ordens");
    if (ordens == objeto.end()) {
        return;
    }
    if (!ordens->is_array()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": \"ordens\" precisa ser uma lista");
        return;
    }
    if (!construcao.donoEhOJogador) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": só as construções do jogador têm \"ordens\"");
        return;
    }
    for (const Json& objetoDaOrdem : *ordens) {
        const std::string onde = nome + ", ordem " + std::to_string(construcao.ordensDoJogador.size() + 1);
        if (!objetoDaOrdem.is_object()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, onde + " não é um objeto { }");
            continue;
        }
        OrdemDoJogador ordem;
        const std::string tipo = lerTextoObrigatorio(objetoDaOrdem, "tipo", onde, listaDeProblemas);
        if (tipo == "manter_estoque") {
            ordem.tipo = TipoDeOrdem::ManterEstoque;
        } else if (tipo == "fazer_n") {
            ordem.tipo = TipoDeOrdem::FazerQuantidade;
        } else if (tipo == "repetir") {
            ordem.tipo = TipoDeOrdem::RepetirSempre;
        } else {
            if (!tipo.empty()) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, onde + ": o tipo precisa ser \"manter_estoque\", \"fazer_n\" ou \"repetir\"");
            }
            continue;
        }
        ordem.numeroDoItem = procurarItemObrigatorio(lerTextoObrigatorio(objetoDaOrdem, "item", onde, listaDeProblemas), catalogoDeItens, onde,
                                                     listaDeProblemas);
        if (ordem.tipo != TipoDeOrdem::RepetirSempre) {
            ordem.quantidade = -1;
            lerInteiroOpcional(objetoDaOrdem, "quantidade", 1, 1'000'000, ordem.quantidade, onde, listaDeProblemas);
            if (ordem.quantidade < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, onde + ": falta a \"quantidade\" (1 ou mais)");
                continue;
            }
        }
        if (ordem.numeroDoItem >= 0) {
            construcao.ordensDoJogador.push_back(ordem);
        }
    }
}

void lerConstrucoes(const Json& documento, const MapaLocal& mapa, const CatalogosParaAVila& catalogos, VilaDoJogo& vila,
                    ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto construcoes = documento.find("construcoes");
    if (construcoes == documento.end() || !construcoes->is_array()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, "falta a lista \"construcoes\"");
        return;
    }
    std::map<int, std::string> construcaoDeCadaLote;
    for (const Json& objeto : *construcoes) {
        const std::string onde = "a construção " + std::to_string(vila.construcoes.size() + 1);
        if (!objeto.is_object()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, onde + " não é um objeto { }");
            continue;
        }
        ConstrucaoDaVila construcao;
        construcao.identificador = lerTextoObrigatorio(objeto, "id", onde, listaDeProblemas);
        construcao.tipo = lerTextoObrigatorio(objeto, "tipo", onde, listaDeProblemas);
        const std::string dono = lerTextoObrigatorio(objeto, "dono", onde, listaDeProblemas);
        const std::string nome = construcao.identificador.empty() ? onde : "a construção \"" + construcao.identificador + "\"";

        // Passo 2: o tipo existe, o id não repete, o dono é "cidade" ou "jogador".
        if (!construcao.tipo.empty() && !catalogos.catalogoDeConstrucoes.existeConstrucao(construcao.tipo)) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": o tipo \"" + construcao.tipo + "\" não existe no construcoes.json");
        }
        for (const ConstrucaoDaVila& outra : vila.construcoes) {
            if (!construcao.identificador.empty() && outra.identificador == construcao.identificador) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": o id repete");
            }
        }
        if (dono != "cidade" && dono != "jogador" && !dono.empty()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": o dono precisa ser \"cidade\" ou \"jogador\"");
        }
        construcao.donoEhOJogador = dono == "jogador";

        // Marco 6: o nível (1 se faltar) e as ordens do jogador.
        lerInteiroOpcional(objeto, "nivel", 1, catalogos.regrasDasConstrucoes.nivelMaximo(), construcao.nivel, nome, listaDeProblemas);
        lerOrdensDoJogador(objeto, nome, catalogos.catalogoDeItens, construcao, listaDeProblemas);

        // O lote existe no mapa e está livre.
        const auto lote = objeto.find("lote");
        if (lote == objeto.end() || !lote->is_number_integer()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": falta o número inteiro \"lote\"");
            continue;
        }
        construcao.numeroDoLote = lote->get<int>();
        const LoteDeConstrucao* loteDoMapa = nullptr;
        for (const LoteDeConstrucao& candidato : mapa.lotesDeConstrucao) {
            if (candidato.identificadorDoLote == construcao.numeroDoLote) {
                loteDoMapa = &candidato;
            }
        }
        if (loteDoMapa == nullptr) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": o lote " + std::to_string(construcao.numeroDoLote) + " não existe no mapa");
            continue;
        }
        const auto [loteJaUsado, loteEstavaLivre] = construcaoDeCadaLote.emplace(construcao.numeroDoLote, nome);
        if (!loteEstavaLivre) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": o lote " + std::to_string(construcao.numeroDoLote)
                                                                    + " já tem " + loteJaUsado->second);
        }
        const int ladoDoLote = ladoDoLoteEmTiles(loteDoMapa->porteDoLote);
        construcao.areaDoLote = {loteDoMapa->tileDoCantoSuperiorEsquerdo.tileX, loteDoMapa->tileDoCantoSuperiorEsquerdo.tileY, ladoDoLote, ladoDoLote};
        construcao.tileDaPorta = calcularTileDaPorta(construcao.areaDoLote);
        if (!mapa.tileDaParaAndar(construcao.tileDaPorta.tileX, construcao.tileDaPorta.tileY)) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": a porta " + textoDoTile(construcao.tileDaPorta)
                                                                    + " está fora do mapa ou bloqueada");
        }
        vila.construcoes.push_back(construcao);
    }
}

int procurarConstrucaoPeloId(const VilaDoJogo& vila, const std::string& identificador)
{
    for (std::size_t indice = 0; indice < vila.construcoes.size(); ++indice) {
        if (vila.construcoes[indice].identificador == identificador) {
            return static_cast<int>(indice);
        }
    }
    return -1;
}

int moradoresDaConstrucao(const ConstrucaoDaVila& construcao, const CatalogoDeConstrucoes& catalogoDeConstrucoes)
{
    const DefinicaoDeConstrucao* definicao = catalogoDeConstrucoes.procurarConstrucao(construcao.tipo);
    return definicao != nullptr && definicao->ehMoradia() ? definicao->moradores : moradoresQuandoOCatalogoNaoDiz;
}

void lerNpcs(const Json& documento, const CatalogosParaAVila& catalogos, VilaDoJogo& vila, ListaDeProblemasDosDados& listaDeProblemas)
{
    const auto npcs = documento.find("npcs");
    if (npcs == documento.end() || !npcs->is_array()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, "falta a lista \"npcs\"");
        return;
    }
    std::vector<int> moradoresDeCadaConstrucao(vila.construcoes.size(), 0);
    std::vector<int> empregadosDeCadaConstrucao(vila.construcoes.size(), 0);
    for (const Json& objeto : *npcs) {
        const std::string onde = "o NPC " + std::to_string(vila.npcsIniciais.size() + 1);
        if (!objeto.is_object()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, onde + " não é um objeto { }");
            continue;
        }
        NpcDaVilaInicial npc;
        npc.nome = lerTextoObrigatorio(objeto, "nome", onde, listaDeProblemas);
        npc.classe = lerTextoObrigatorio(objeto, "classe", onde, listaDeProblemas);
        const std::string casa = lerTextoObrigatorio(objeto, "casa", onde, listaDeProblemas);
        const std::string trabalho = lerTextoObrigatorio(objeto, "trabalho", onde, listaDeProblemas);
        const std::string nome = npc.nome.empty() ? onde : npc.nome;
        if (npc.classe != "civil" && !npc.classe.empty()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": a fatia só tem a classe \"civil\"");
        }

        // Passo 4: a casa precisa ser uma Casa, com no máximo 4 moradores.
        npc.indiceDaCasa = procurarConstrucaoPeloId(vila, casa);
        if (npc.indiceDaCasa < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": a casa \"" + casa + "\" não existe");
        } else if (!vila.construcoes[npc.indiceDaCasa].ehCasa()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": \"" + casa + "\" não é uma Casa");
        } else {
            const int moradoresQueCabem = moradoresDaConstrucao(vila.construcoes[npc.indiceDaCasa], catalogos.catalogoDeConstrucoes);
            if (++moradoresDeCadaConstrucao[npc.indiceDaCasa] > moradoresQueCabem) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": a casa \"" + casa + "\" já tem "
                                                                        + std::to_string(moradoresQueCabem) + " moradores");
            }
        }

        // O trabalho: vazio (desempregado) ou uma construção que não é Casa,
        // sem passar das vagas do nível dela (ficha da Construção, regra 4.1, passo 3).
        if (!trabalho.empty()) {
            npc.indiceDoTrabalho = procurarConstrucaoPeloId(vila, trabalho);
            if (npc.indiceDoTrabalho < 0) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": o trabalho \"" + trabalho + "\" não existe");
            } else if (vila.construcoes[npc.indiceDoTrabalho].ehCasa()) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": \"" + trabalho + "\" é uma Casa, não um trabalho");
                npc.indiceDoTrabalho = -1;
            } else {
                const ConstrucaoDaVila& construcaoDoTrabalho = vila.construcoes[npc.indiceDoTrabalho];
                const int vagas = catalogos.regrasDasConstrucoes.vagasPorNivel[construcaoDoTrabalho.nivel - 1];
                if (++empregadosDeCadaConstrucao[npc.indiceDoTrabalho] > vagas) {
                    listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": \"" + trabalho + "\" no nível "
                                                + std::to_string(construcaoDoTrabalho.nivel) + " só tem " + std::to_string(vagas) + " vagas");
                }
            }
        }

        // As habilidades (FL6-6). Sem o campo, ficam em 50.
        const auto habilidades = objeto.find("habilidades");
        if (habilidades != objeto.end()) {
            if (!habilidades->is_object()) {
                listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, nome + ": \"habilidades\" precisa ser um objeto { }");
            } else {
                lerInteiroOpcional(*habilidades, "bracal", 0, 100, npc.habilidadeBracal, nome, listaDeProblemas);
                lerInteiroOpcional(*habilidades, "manufaturado", 0, 100, npc.habilidadeManufaturado, nome, listaDeProblemas);
            }
        }
        vila.npcsIniciais.push_back(npc);
    }
}

// O dinheiro e o estoque do primeiro dia (FL6-4).
void lerDinheiroEEstoque(const Json& documento, const CatalogoDeItens& catalogoDeItens, VilaDoJogo& vila,
                         ListaDeProblemasDosDados& listaDeProblemas)
{
    const int muitoDinheiro = 1'000'000'000;
    lerInteiroOpcional(documento, "banco_da_cidade", 0, muitoDinheiro, vila.bancoDaCidadeEmDecimos, "a vila", listaDeProblemas);
    lerInteiroOpcional(documento, "dinheiro_do_jogador", 0, muitoDinheiro, vila.dinheiroDoJogadorEmDecimos, "a vila", listaDeProblemas);
    lerInteiroOpcional(documento, "dinheiro_inicial_por_civil", 0, muitoDinheiro, vila.dinheiroInicialPorCivilEmDecimos, "a vila", listaDeProblemas);

    const auto estoque = documento.find("estoque_inicial");
    if (estoque == documento.end()) {
        return;
    }
    if (!estoque->is_array()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, "\"estoque_inicial\" precisa ser uma lista");
        return;
    }
    for (const Json& objeto : *estoque) {
        const std::string onde = "o estoque inicial " + std::to_string(vila.estoqueInicial.size() + 1);
        if (!objeto.is_object()) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, onde + " não é um objeto { }");
            continue;
        }
        ItemDoEstoqueInicial item;
        item.numeroDoItem = procurarItemObrigatorio(lerTextoObrigatorio(objeto, "item", onde, listaDeProblemas), catalogoDeItens, onde,
                                                    listaDeProblemas);
        const std::string dono = lerTextoObrigatorio(objeto, "dono", onde, listaDeProblemas);
        if (dono != "cidade" && dono != "jogador") {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, onde + ": o dono precisa ser \"cidade\" ou \"jogador\"");
        }
        item.dono = dono == "jogador" ? DonoDoItem::Jogador : DonoDoItem::Cidade;
        item.quantidade = -1;
        lerInteiroOpcional(objeto, "quantidade", 0, 1'000'000, item.quantidade, onde, listaDeProblemas);
        if (item.quantidade < 0) {
            listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, onde + ": falta a \"quantidade\" (0 ou mais)");
            continue;
        }
        if (item.numeroDoItem >= 0) {
            vila.estoqueInicial.push_back(item);
        }
    }
}

}  // namespace

int VilaDoJogo::indiceDoMercado() const
{
    for (std::size_t indice = 0; indice < construcoes.size(); ++indice) {
        if (construcoes[indice].tipo == tipoDoMercado) {
            return static_cast<int>(indice);
        }
    }
    return -1;
}

int VilaDoJogo::quantidadeDeEmpregados() const
{
    int empregados = 0;
    for (const NpcDaVilaInicial& npc : npcsIniciais) {
        empregados += npc.indiceDoTrabalho >= 0 ? 1 : 0;
    }
    return empregados;
}

PosicaoDeTile calcularTileDaPorta(const RetanguloEmTiles& areaDoLote)
{
    return {areaDoLote.tileX + areaDoLote.larguraEmTiles / 2, areaDoLote.tileY + areaDoLote.alturaEmTiles};
}

VilaDoJogo lerVilaInicial(const std::string& textoDoArquivo, const MapaLocal& mapa, const CatalogosParaAVila& catalogos,
                          ListaDeProblemasDosDados& listaDeProblemas)
{
    VilaDoJogo vila;
    const Json documento = Json::parse(textoDoArquivo, nullptr, false);
    if (documento.is_discarded() || !documento.is_object()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, "o arquivo não é um JSON válido");
        return vila;
    }
    const auto semente = documento.find("semente_da_partida");
    if (semente == documento.end() || !semente->is_number_unsigned()) {
        listaDeProblemas.anotarErro(nomeDoArquivoDaVila, 0, "falta o número inteiro \"semente_da_partida\" (0 ou mais)");
    } else {
        vila.sementeDaPartida = semente->get<uint64_t>();
    }
    lerConstrucoes(documento, mapa, catalogos, vila, listaDeProblemas);
    lerNpcs(documento, catalogos, vila, listaDeProblemas);
    lerDinheiroEEstoque(documento, catalogos.catalogoDeItens, vila, listaDeProblemas);
    return vila;
}

void bloquearOsLotesOcupados(MapaLocal& mapa, const VilaDoJogo& vila)
{
    for (std::size_t indice = 0; indice < vila.construcoes.size(); ++indice) {
        const ConstrucaoDaVila& construcao = vila.construcoes[indice];
        for (LoteDeConstrucao& lote : mapa.lotesDeConstrucao) {
            if (lote.identificadorDoLote == construcao.numeroDoLote) {
                lote.idDaConstrucaoQueOcupa = static_cast<int>(indice);
            }
        }
        const RetanguloEmTiles& area = construcao.areaDoLote;
        for (int tileY = area.tileY; tileY < area.tileY + area.alturaEmTiles; ++tileY) {
            for (int tileX = area.tileX; tileX < area.tileX + area.larguraEmTiles; ++tileX) {
                if (mapa.tileEstaDentroDoMapa(tileX, tileY)) {
                    mapa.passagemPorTile[mapa.indiceDoTileNaLista(tileX, tileY)] = 0;
                }
            }
        }
    }
}
