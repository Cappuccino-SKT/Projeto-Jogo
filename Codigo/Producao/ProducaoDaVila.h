#pragma once

// A produção da vila (Ficha Lógica: Produção, marco 6): cada construção de
// produção tem uma receita; quem está trabalhando soma ticks; a cada hora, o
// trabalho vira progresso do ciclo, e o ciclo terminado põe o lote no
// inventário. A Cidade escolhe a receita todo dia; o jogador, pela fila de
// ordens. Nada aqui abre janela: o teste sem tela roda isto.
//
// O marco 7 (parte 10) muda três coisas: o consumo previsto passa a ser
// medido (ConsumoMedidoDaVila), os preços são os do Mercado Local, e a
// construção com estoque de sobra fica de folga.

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include "Dados/DadosDoJogo.h"
#include "Mapa/MapaLocal.h"
#include "Vila/VilaInicial.h"
#include "Economia/DinheiroDaVila.h"
#include "Inventario/InventarioDaCidade.h"
#include "Producao/ConsumoMedidoDaVila.h"
#include "Producao/EstadoDoJogador.h"
#include "Producao/OrdemDoJogador.h"
#include "Tempo/RelogioDoMundo.h"
#include "Tempo/SorteioComSemente.h"
#include "Vila/RegrasDoTrabalho.h"
#include "Vila/RegrasDaPopulacao.h"

// O estado do marco 7 que mora em outros arquivos (Economia/ e Eventos/).
struct MercadoLocalDaVila;
struct FelicidadeDaClasse;
struct EstadoDosEventos;
struct EstadoDaViagem;

// ---------------------------------------------------------------------------
// Os componentes do NPC (parte 2).
// ---------------------------------------------------------------------------

struct HabilidadesDoNpc {
    int bracal = 50;
    int manufaturado = 50;

    // A habilidade usada num Trabalho. TE2-1: até os Ricos chegarem (marco A4), a Especialista é a Manufaturado.
    int habilidadeNoTrabalho(TrabalhoDaReceita trabalho) const;
};

struct TrabalhoDoDia {
    int ticksTrabalhadosNestaHora = 0;
    int ticksTrabalhadosHoje = 0;  // os salários do dia (ficha da Economia)
};

// ---------------------------------------------------------------------------
// O estado de cada construção da vila (parte 2): uma por construção, na mesma
// ordem da vila. As que não produzem (Casa, Mercado, Construtor) só usam o
// gerente e o salário atrasado.
// ---------------------------------------------------------------------------

struct ProducaoDaConstrucao {
    bool ehDeProducao = false;                    // o tipo tem receitas no receitas.csv
    int receitaDoCiclo = -1;                      // a posição no catálogo de receitas; -1 = nenhuma
    int receitaEscolhidaParaOProximoCiclo = -1;   // a escolha do dia, nas da Cidade
    bool cicloComecou = false;                    // os insumos já saíram
    int64_t progressoDoCiclo = 0;                 // em ticks × porcentagem (parte 5)
    int pontosDePesquisaGuardados = 0;            // fase B5: os pontos do Laboratório que o passo 7b ainda não somou
    int restoDaProducaoBracal = 0;                // de 0 a 99: a fração de unidade guardada
    bool estaParada = false;
    std::string motivoDaParada;                   // "sem Água", "o estoque encheu", "de folga"...
    bool estaDeFolgaHoje = false;                 // regra 10.2: decidido à meia-noite
    std::vector<OrdemDoJogador> ordens;           // só nas do jogador (regra 4.4)

    // Marco 8 (só nas do jogador, exceto manutencaoAtrasada e emObra).
    NivelDeSalario nivelDeSalario = NivelDeSalario::Normal;
    int diasSeguidosComSalarioBaixo = 0;
    bool manutencaoAtrasada = false;              // produz 75% (Construção, regra 10.6)
    bool emObra = false;                          // a construção nova ainda não ficou pronta

    int numeroDoGerente = -1;                     // ficha da Construção, regra 4.2
    int diasDeSalarioAtrasado = 0;                // ficha da Economia, regra 4.2
    // Fase A2 (ficha do Trabalho e Propriedade).
    int diasSemEmpregados = 0;                    // regra 4.4: 32 dias, e a do jogador vai a leilão
    bool modoAutomatico = false;                  // regra 4.3: a do jogador segue a IA da Cidade
    int64_t salarioDevidoEmDecimos = 0;           // regra 4.4, passo 0: os salários não pagos

    // O campo dos Braçais (regra 4.1): os tiles livres da área do recurso da
    // receita atual, de cima para baixo e da esquerda para a direita. Vazio =
    // os empregados trabalham dentro da construção.
    int areaDoCampoNoMapa = -1;
    std::vector<PosicaoDeTile> tilesDeTrabalhoNoCampo;

    // O que saiu hoje, por item (só para o console e a tecla F4).
    std::map<int, int> producaoDeHojePorItem;
};

// Tudo o que as regras da produção e da economia usam, junto.
struct ContextoDaProducao {
    entt::registry& registroDasEntidades;
    const DadosDoJogo& dados;
    InventarioDaCidade& inventario;
    DinheiroDaVila& dinheiro;
    std::vector<ProducaoDaConstrucao>& producaoDasConstrucoes;
    // Marco 7.
    ConsumoMedidoDaVila& consumoMedido;
    MercadoLocalDaVila& mercadoLocal;
    FelicidadeDaClasse& felicidade;
    EstadoDosEventos& eventos;
    SorteioComSemente& sorteioDosEventos;
    // Marco 8: a vila e o mapa que mudam com as obras (cópias da simulação),
    // as decisões do jogador e a fila de obras.
    VilaDoJogo& vila;
    MapaLocal& mapa;
    DecisoesDoJogador& decisoes;
    ObrasDaVila& obras;
    // Fase A1 (ficha do Clima): o clima de hoje e o sorteio dele (sequência 3).
    EstadoDoClima& clima;
    SorteioComSemente& sorteioDoClima;
    // Fase A2 (ficha do Trabalho): o leilão e as contas do dia.
    EstadoDoTrabalho& trabalho;
    // Fase A3 (ficha da População): o acumulado, o sorteio (sequência 4) e a lista dos NPCs da vila.
    EstadoDaPopulacao& populacao;
    SorteioComSemente& sorteioDaPopulacao;
    std::vector<entt::entity>& npcsDaVila;
    int ticksPorHoraDeJogo = 800;
    // Fase B1 (ficha da Viagem): o grupo do jogador, os sorteios das sequências 5 e 9 e o relógio.
    EstadoDaViagem* viagem = nullptr;
    SorteioComSemente* sorteioDaViagem = nullptr;
    SorteioComSemente* sorteioDoClimaDasOutrasRegioes = nullptr;
    const RelogioDoMundo* relogio = nullptr;
    // Fase B5 (Tecnologia, 4.5): as receitas que esta cidade pode usar (nulo = todas).
    const std::vector<bool>* receitasLiberadas = nullptr;
    // Fase C1 (Facção): o imposto da lei (−1 = o do regras.json), a Felicidade das leis e os
    // itens proibidos (nulo = nenhum).
    int impostoDaCidadeEmPorcento = -1;
    int felicidadeDaFaccaoEmPontos = 0;
    const std::vector<bool>* itensProibidos = nullptr;
};

// ---------------------------------------------------------------------------
// As contas da parte 5.
// ---------------------------------------------------------------------------

// horas × 800 × 100: um Pão (4 horas) precisa de 320.000.
int64_t progressoNecessarioDoCiclo(const DefinicaoDeReceita& receita, int ticksPorHoraDeJogo);

// ticks × velocidade × bônus de nível, com a velocidade = 50 + habilidade (%)
// e o bônus = 100 + 10 × (nível − nível mínimo) (%). Uma hora (800 ticks) de
// habilidade 60 no nível mínimo: 800 × 110 × 100 ÷ 100 = 88.000.
int64_t calcularProgressoDoTrabalho(int ticksTrabalhados, int habilidade, int nivelDaConstrucao, int nivelMinimoDaReceita);

// O déficit do item, em %: 100 − estoque × 100 ÷ alvo, preso entre 0 e 100. O
// alvo vem em centésimos de unidade (1,875 Móveis = 187); com alvo 0, é 0.
int calcularDeficitDoItem(int estoque, int64_t estoqueAlvoEmCentesimos);

// A nota da regra 4.3: (preço − custo dos insumos) × (100 + déficit), em
// décimos × %, sem dividir por 100 (assim não se perde a fração).
int64_t calcularNotaDaReceita(int precoDoProdutoEmDecimos, int custoDosInsumosEmDecimos, int deficitEmPorcento);

// O preço de venda da loja da cidade: 110% do preço, com a metade subindo (1,7 vira 1,9).
int calcularPrecoDaLojaDaCidade(int precoEmDecimos, const RegrasDaEconomia& regras);

// Regra 10.1, passo 2: a parte de um ciclo que faltou numa hora parada, em
// centésimos: quantidade × ticks × 100 ÷ (horas × 800). 2 Toras, 1.600 ticks,
// receita de 4 horas: 100 (1 Tora).
int64_t calcularDemandaQueFaltouNaHora(int quantidadeDoInsumo, int ticksTrabalhadosNaHora, int horasDaReceita, int ticksPorHoraDeJogo);

// O preço local do item (Economia, regra 10.4); o preço base antes do Mercado Local.
int precoLocalDoItem(const ContextoDaProducao& contexto, int numeroDoItem);

// O estoque-alvo: 5 dias do consumo previsto (a média medida, regra 10.1), em centésimos.
int64_t estoqueAlvoDoItemEmCentesimos(const ContextoDaProducao& contexto, int numeroDoItem);

// A previsão do marco 6 (a cesta e os insumos das receitas atuais num dia
// cheio), em centésimos. No marco 7, só enche os 5 dias guardados do começo.
std::vector<int64_t> preverOConsumoPelasReceitasAtuais(const ContextoDaProducao& contexto);

// Os tiles livres de uma área de recurso (os que dão para andar e não são
// porta), de cima para baixo e da esquerda para a direita.
std::vector<PosicaoDeTile> montarTilesDeTrabalhoDaArea(const MapaLocal& mapa, const VilaDoJogo& vila, const AreaDeRecurso& area);

// ---------------------------------------------------------------------------
// As regras da parte 4.
// ---------------------------------------------------------------------------

// Ao montar a vila: põe as habilidades e o trabalho do dia em cada NPC (na
// ordem da vila), monta o inventário (a capacidade e o estoque inicial), o
// dinheiro do primeiro dia e a produção de cada construção, e faz a primeira
// escolha das receitas. Devolve os avisos (um estoque inicial que não coube).
std::vector<std::string> montarAProducaoDaVila(ContextoDaProducao& contexto, const std::vector<entt::entity>& npcsDaVila);

// Se o NPC está trabalhando agora (regra 4.1): Trabalhar (ou Comer, o almoço,
// FL6-7) no lugar de trabalho, dentro da construção ou no tile do campo.
bool npcEstaTrabalhandoAgora(const ContextoDaProducao& contexto, entt::entity npc);

// Regra 4.1, a cada tick: quem está trabalhando soma 1 tick.
void contarOsTicksTrabalhados(ContextoDaProducao& contexto);

// Regra 4.2, a cada hora: o ciclo de cada construção.
void rodarOsCiclosDaHora(ContextoDaProducao& contexto, const RelogioDoMundo& relogio);

// Regra 4.3, a cada dia (e ao montar a vila): a receita de cada construção
// da Cidade. Com a regra 10.2: a receita com estoque de sobra não é
// candidata, e a construção sem candidata (ou a do jogador sem ordem que
// queira produzir) fica de folga no dia seguinte.
void escolherAsReceitasDaCidade(ContextoDaProducao& contexto);

// Regra 4.4: a receita que a fila de ordens do jogador pede agora, ou -1. Não
// muda nada (nem compra): só olha.
int escolherReceitaPelaFilaDeOrdens(const ContextoDaProducao& contexto, int indiceDaConstrucao);

// Ficha da Construção, regra 4.2: o gerente de cada construção com empregados.
void escolherOsGerentes(ContextoDaProducao& contexto);

// O Trabalho de quem trabalha na construção: o da receita atual, ou o
// "trabalho_sem_receita" (Lojista, Operário).
TrabalhoDaReceita trabalhoDaConstrucao(const ContextoDaProducao& contexto, int indiceDaConstrucao);

// A tabela da tecla F4 (FL6-8): o estoque de cada dono, a capacidade, cada
// construção (receita, progresso, motivo da parada) e o dinheiro.
std::string montarTabelaDaProducao(const ContextoDaProducao& contexto);
