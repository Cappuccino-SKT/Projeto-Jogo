#pragma once

// A tecnologia (Ficha Lógica: Tecnologia, fase B5): cada facção junta Pontos de Pesquisa nos
// seus Laboratórios e completa uma tecnologia por vez; os marcos mundiais chegam numa data
// sorteada dentro de uma janela, mais cedo quando as facções pesquisam as tecnologias ligadas.
// O que aparece num "libera" só pode ser usado por quem tem a tecnologia ou o marco (4.5).
// As listas (data/tecnologias.json e data/marcos.json) são provisórias até a rodada de
// conteúdo C1 (TE2-1).

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class SimulacaoDaVila;
struct DadosDoJogo;
class ListaDeProblemasDosDados;

// O que uma tecnologia ou um marco libera: { "tipo": "receita", "id": "receita.tabua" }.
struct LiberacaoDaTecnologia {
    std::string tipo;  // "construcao", "receita", "item" ou "nivel_da_cidade"
    std::string id;
};

struct DefinicaoDeTecnologia {
    std::string identificadorDeTexto;  // "tec.contabilidade"; o nome está no textos/pt-BR.json
    std::string ramo;                  // "economia"
    int era = 1;
    int custo = 0;                     // 0 = o custo_por_era da era
    std::vector<int> preRequisitos;    // as posições no catálogo
    std::vector<LiberacaoDaTecnologia> libera;
    std::vector<std::string> culturasQueComecamCom;
};

struct DefinicaoDeMarco {
    std::string identificadorDeTexto;  // "marco.era_2"
    int primeiroAno = 1;               // a janela, contando o ano 1 como o primeiro da partida
    int ultimoAno = 1;
    std::vector<int> tecnologiasLigadas;
    int abreEra = 0;                   // 0 = não abre era
    std::vector<LiberacaoDaTecnologia> libera;
};

// O bloco "tecnologia" do regras.json.
struct RegrasDaTecnologia {
    std::vector<int> custoPorEra{100, 200, 400, 800};
    int difusaoPorFaccaoEmPorcento = 10;
    int difusaoMaximaEmPorcento = 50;
    int vilaSemFaccaoCustoEmPorcento = 150;
    std::map<std::string, std::vector<std::string>> prioridadeDosRamosPorCultura;
    int horasPorPonto = 8;            // o ciclo da receita "Pontos de Pesquisa" (TE1-2)
    int tecnologiasPorDiaNoMaximo = 3;
    int nivelDoLaboratorio = 3;       // TE2-1: o Laboratório das capitais (5 vagas)
    std::vector<std::pair<int, int>> outrasCidadesComLaboratorio;  // (coluna, linha): a (3,6) da Tecnocrata
};

struct CatalogoDaTecnologia {
    std::vector<DefinicaoDeTecnologia> tecnologias;
    std::vector<DefinicaoDeMarco> marcos;
    RegrasDaTecnologia regras;
};

// Lê os dois arquivos e o bloco do regras.json; confere o que cada "libera" aponta (teste 7).
CatalogoDaTecnologia lerArquivosDaTecnologia(const std::string& textoDasTecnologias, const std::string& textoDosMarcos, const std::string& textoDoRegras,
                                             const DadosDoJogo& dados, ListaDeProblemasDosDados& listaDeProblemas);

// O estado (no save). Os pesquisadores são as facções, na ordem do cidades_iniciais.json, e a
// vila do jogador por último (como uma "facção" a mais).
struct EstadoDaTecnologia {
    std::vector<std::vector<bool>> temATecnologia;  // [pesquisador][tecnologia]
    std::vector<int> projetoAtual;                  // a posição da tecnologia; −1 = nenhum
    std::vector<int64_t> pontosAcumulados;
    int eraAtualDoMundo = 1;
    std::vector<int64_t> diaSorteadoPorMarco;       // o dia da partida, contando do 0
    std::vector<bool> marcoChegou;

    bool operator==(const EstadoDaTecnologia&) const = default;
};

// As facções que pesquisam ("tecnocrata"...), na ordem do cidades_iniciais.json.
std::vector<std::string> faccoesQuePesquisam(const DadosDoJogo& dados);
// O pesquisador da cidade do mapa: a facção dona, a vila do jogador (o último), ou −1 (Vila Livre).
int pesquisadorDaCidade(const DadosDoJogo& dados, int numeroDoMapa);

// Regra 4.4, passo 1, e 4.6: sorteia o dia de cada marco (sequência 6) e dá as tecnologias iniciais.
void montarATecnologiaDaPartidaNova(EstadoDaTecnologia& estado, const DadosDoJogo& dados, uint64_t sementeDaPartida);

// Regra 4.2, passo 3: o custo com a difusão (−10% por facção que já tem, até −50%) e × 150% na vila.
int custoAtualDaTecnologia(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int pesquisador, int tecnologia);
int custoComOsNumeros(int custoDaTecnologia, int faccoesQueJaTem, bool ehAVilaDoJogador, const RegrasDaTecnologia& regras);

// Regra 4.3: a disponível do ramo de maior prioridade da cultura; empate, a mais barata; depois a primeira.
int escolherProjetoDaFaccao(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int pesquisador);

// Regra 4.4, passo 2: dia sorteado − (dia sorteado − primeiro dia) × facções completas ÷ facções, para baixo.
int64_t dataDoMarcoComOsNumeros(int64_t diaSorteado, int64_t primeiroDia, int faccoesCompletas, int faccoes);
int64_t dataAtualDoMarco(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int marco);
int64_t primeiroDiaDoMarco(const DefinicaoDeMarco& marco);
int64_t ultimoDiaDoMarco(const DefinicaoDeMarco& marco);

// O passo 7b, na vila, no dia novo: soma os pontos dos Laboratórios de todas as cidades,
// completa as tecnologias, escolhe os projetos, confere os marcos e atualiza as travas.
std::vector<std::string> somarPontosEConcluirTecnologias(SimulacaoDaVila& vila);

// Regra 4.5: o que não aparece em nenhum "libera" é livre; o resto, só com a tecnologia ou o marco.
bool estaLiberado(const EstadoDaTecnologia& estado, const DadosDoJogo& dados, int pesquisador, const std::string& tipo, const std::string& id);
// As travas das receitas de cada cidade, pela facção dona (uma por receita do catálogo).
void atualizarAsTravasDasCidades(SimulacaoDaVila& vila);
