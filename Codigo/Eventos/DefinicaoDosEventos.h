#pragma once

// As definições dos eventos (Ficha Lógica: Eventos, parte 2), lidas do
// data/eventos.json: o tipo, quando cada um pode sair, a chance, a duração,
// o cooldown e os efeitos. O que muda durante o jogo (os ativos) está em
// EventosDaVila.h.

#include <optional>
#include <string>
#include <vector>

#include "Noticias/Noticias.h"
#include "Dados/Construcoes.h"
#include "Dados/Itens.h"
#include "Dados/ProblemasDosDados.h"
#include "Dados/TextosERegras.h"
#include "Tempo/RelogioDoMundo.h"

enum class TipoDeEvento { Ocasional, Condicional, DataFixa };

// A lista fechada de efeitos da fatia (regra 4.4).
// Fase A6 (ficha da Cura): os doentes em porcento, as mortes da Epidemia e o Remédio.
enum class TipoDeEfeito { ProducaoDaConstrucao, Felicidade, MudarEstoque, NpcsDoentes, MercadorCompra, Lenha,
                          NpcsDoentesEmPorcento, MortesDaPopulacao, Remedio };

struct EfeitoDoEvento {
    TipoDeEfeito tipo = TipoDeEfeito::Felicidade;
    std::string tipoDaConstrucao;   // ProducaoDaConstrucao: "construcao.fazenda"
    int numeroDoItem = -1;          // MudarEstoque
    int porcentagem = 100;          // ProducaoDaConstrucao (50 = metade), MudarEstoque (−10 = tira 10%) e Lenha (200 = o dobro)
    int quantidade = 0;             // MudarEstoque (+20 Toras), NpcsDoentes (2) e MercadorCompra (20)
    int pontosDeFelicidade = 0;     // Felicidade
    // Fase A6: NpcsDoentesEmPorcento usa a porcentagem (20 = 20% dos Civis); MortesDaPopulacao,
    // a quantidade (10 por mil da população por dia); Remedio, a quantidade (1 a cada 10 NPCs)
    // e a porcentagem das mortes com ele (50).
};

// Fase A6 (ficha da Cura, CU1-1): um modificador da chance de um ocasional.
enum class TipoDeModificadorDaChance { OcupacaoMinima, ComidaEmDeficit, ExisteConstrucao };
struct ModificadorDaChance {
    TipoDeModificadorDaChance tipo = TipoDeModificadorDaChance::ComidaEmDeficit;
    int ocupacaoMinimaEmPorcento = 0;
    std::vector<std::string> construcoes;  // ExisteConstrucao: basta uma delas na vila
    int porcentagem = 100;
};

// A condição de um evento condicional. A fatia tem duas.
enum class TipoDeCondicao { ComidaAbaixoDoAlvo, MateriaisEmDeficit };

struct CondicaoDoEvento {
    TipoDeCondicao tipo = TipoDeCondicao::ComidaAbaixoDoAlvo;
    int entraAbaixoDoAlvoEmPorcento = 0;  // a Fome: 10
    int saiAcimaDoAlvoEmPorcento = 0;     // a Fome: 30 (a histerese)
    int diasSeguidos = 1;
};

struct DefinicaoDeEvento {
    std::string identificadorDeTexto;  // "evento.seca"; o nome está no textos/pt-BR.json
    TipoDeEvento tipo = TipoDeEvento::Ocasional;
    bool negativo = false;             // conta no limite de 2 negativos ativos
    ImportanciaDaNoticia importanciaDaNoticia = ImportanciaDaNoticia::Nenhuma;  // fase B3: "noticia"
    std::vector<Estacao> estacoes;     // vazio = qualquer estação
    std::vector<int> diasDaEstacao;    // vazio = qualquer dia (de 1 a 16)
    int chancePorDiaEmPorMil = 0;      // 40 = 4% por dia
    int cooldownEmDias = 0;
    int duracaoEmDias = 0;             // 0 = pontual
    std::optional<CondicaoDoEvento> condicao;  // só nos condicionais
    Estacao estacaoDaDataFixa = Estacao::Primavera;  // só nos de data fixa
    int diaDaDataFixa = 1;
    std::vector<EfeitoDoEvento> efeitos;
    std::vector<ModificadorDaChance> modificadoresDaChance;  // fase A6: multiplicam a chance, um depois do outro

    bool ehPontual() const { return tipo != TipoDeEvento::Condicional && duracaoEmDias == 0; }
};

class CatalogoDeEventos {
public:
    const std::vector<DefinicaoDeEvento>& todosOsEventos() const { return eventos; }
    int quantidadeDeEventos() const { return static_cast<int>(eventos.size()); }
    void acrescentarEvento(DefinicaoDeEvento evento) { eventos.push_back(std::move(evento)); }

private:
    std::vector<DefinicaoDeEvento> eventos;  // na ordem do arquivo: é a ordem dos sorteios
};

// O que a leitura confere: os nomes, os itens e as construções citados.
struct CatalogosParaOsEventos {
    const TabelaDeTextos& tabelaDeTextos;
    const CatalogoDeItens& catalogoDeItens;
    const CatalogoDeConstrucoes& catalogoDeConstrucoes;
};

// Lê e confere o eventos.json. Um item, uma construção ou uma estação que não
// existe vira um erro com o motivo.
CatalogoDeEventos lerArquivoDeEventos(const std::string& textoDoArquivo, const CatalogosParaOsEventos& catalogos,
                                      ListaDeProblemasDosDados& listaDeProblemas);

// "primavera", "verao", "outono" ou "inverno".
std::optional<Estacao> lerEstacaoDoTexto(const std::string& texto);
