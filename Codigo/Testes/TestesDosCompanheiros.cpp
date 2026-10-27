// Testes das dinastias (Ficha Lógica: Companheiros e Diálogo, parte 8, marco D2).

#include <algorithm>
#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Cultura/Cultura.h"
#include "Dados/DadosDoJogo.h"
#include "Companheiros/Companheiros.h"
#include "Faccao/Dinastia.h"
#include "Viagem/Viagem.h"
#include "Vila/SimulacaoDaVila.h"
#include "Tempo/SorteioComSemente.h"

namespace {

const std::string pastaDosDadosDoJogo = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

MembroDaDinastia membroDeTeste(int numero, int idade, int pai)
{
    MembroDaDinastia membro;
    membro.numero = numero;
    membro.nome = "M" + std::to_string(numero);
    membro.idade = idade;
    membro.pai = pai;
    return membro;
}

CulturaDaDinastia culturaDe(const DadosDoJogo& dados, const std::string& id)
{
    const DefinicaoDaCultura* cultura = dados.culturas.procurar(id);
    return cultura == nullptr ? CulturaDaDinastia{} : CulturaDaDinastia{cultura->nomesDeHomem, cultura->nomesDeMulher, cultura->sucessao};
}

// Teste 5 e a herança: quem herda em cada regra.
void testarOHerdeiro()
{
    DinastiaDaFaccao dinastia;
    dinastia.numeroDoLider = 0;
    dinastia.membros = {membroDeTeste(0, 30, -1), membroDeTeste(1, 9, 0), membroDeTeste(2, 9, 0), membroDeTeste(3, 7, 0), membroDeTeste(4, 3, 0), membroDeTeste(5, 20, -1)};
    dinastia.membros[1].corpoACorpo = 40;
    dinastia.membros[2].corpoACorpo = 40;
    dinastia.membros[3].corpoACorpo = 75;
    dinastia.membros[4].corpoACorpo = 99;  // criança: não entra
    dinastia.membros[5].corpoACorpo = 90;
    dinastia.membros[5].ehNobre = true;    // nobre: só sobe na crise
    conferir(escolherHerdeiro(dinastia, "duelo", 5) == 3, "a Guerreira (duelo) escolhe o adulto de maior Corpo a Corpo, sem a criança nem o nobre");
    conferir(escolherHerdeiro(dinastia, "hereditaria", 5) == 1, "a herança: o filho adulto mais velho; os gêmeos (9 e 9), o de menor número");
    dinastia.membros = {membroDeTeste(0, 30, -1), membroDeTeste(4, 3, 0), membroDeTeste(5, 20, -1)};
    dinastia.membros[2].ehNobre = true;
    conferir(escolherHerdeiro(dinastia, "hereditaria", 5) == -1, "sem filho adulto: ninguém herda (crise)");
}

// O começo (CP1-5) e a Vida prolongada de Cassius (HI11-1).
void testarOComecoEAVidaProlongada(const DadosDoJogo& dados)
{
    const RegrasDaPopulacao& populacao = dados.regras.populacao;
    SorteioComSemente sorteio(42, 8);
    LiderDoComeco yasmin;
    yasmin.nome = "Sultana Yasmin bint Qadir";
    yasmin.ehMulher = true;
    yasmin.idade = 14;
    const DinastiaDaFaccao escravista = montarADinastiaDoComeco(yasmin, culturaDe(dados, "escravista"), dados.regrasDaFamilia, populacao, sorteio);
    int filhosAdultos = 0;
    int nobres = 0;
    for (const MembroDaDinastia& membro : escravista.membros) {
        filhosAdultos += membro.mae == escravista.numeroDoLider && membro.idade >= populacao.idadeAdulto ? 1 : 0;
        nobres += membro.ehNobre ? 1 : 0;
    }
    const MembroDaDinastia* marido = escravista.procurar(escravista.lider().conjuge);
    conferir(marido != nullptr && !marido->ehMulher && filhosAdultos >= 1 && filhosAdultos <= 2 && nobres == 2,
             "CP1-5: Yasmin (14) começa casada, com 1 ou 2 filhos adultos e 2 nobres");

    LiderDoComeco cassius;
    cassius.nome = "Grão-Mestre Cassius Varro";
    cassius.idade = 30;
    cassius.vidaProlongada = true;
    DinastiaDaFaccao arcana = montarADinastiaDoComeco(cassius, culturaDe(dados, "arcana"), dados.regrasDaFamilia, populacao, sorteio);
    bool liderMorreu = false;
    for (int estacao = 0; estacao < 40; ++estacao) {
        liderMorreu = passarUmaEstacaoNaDinastia(arcana, culturaDe(dados, "arcana"), dados.regrasDaFamilia, populacao, estacao % 4 == 0, true, sorteio).liderMorreu
                      || liderMorreu;
    }
    const bool naoEnvelheceu = !liderMorreu && arcana.lider().idade == 30;
    passarUmaEstacaoNaDinastia(arcana, culturaDe(dados, "arcana"), dados.regrasDaFamilia, populacao, false, false, sorteio);
    const MembroDaDinastia* depoisDaTorre = arcana.procurar(0);
    conferir(naoEnvelheceu && (depoisDaTorre == nullptr || (depoisDaTorre->idade == populacao.idadeVelho && !depoisDaTorre->vidaProlongada)),
             "HI11-1: com a Torre de pé, Cassius fica nos 30 por 10 anos; sem ela, vai a 31 de uma vez e perde o traço");
}

// Teste 4: em 1.000 mundos, as mortes de líder sem herdeiro adulto ficam abaixo de 10% nos primeiros 20 anos.
void testarAsDinastiasEm1000Mundos(const DadosDoJogo& dados)
{
    const RegrasDaPopulacao& populacao = dados.regras.populacao;
    const int divisor = dados.faccoes.anosDaHistoriaPorAnoDoJogoEmDecimos;
    int mortesDeLider = 0;
    int crises = 0;
    for (uint64_t mundo = 0; mundo < 1000; ++mundo) {
        for (const DefinicaoDaFaccao& faccao : dados.faccoes.faccoes) {
            const CulturaDaDinastia cultura = culturaDe(dados, faccao.cultura);
            SorteioComSemente sorteio(mundo, 8);
            LiderDoComeco lider;
            lider.nome = faccao.lider.nome;
            lider.ehMulher = faccao.lider.ehMulher;
            lider.vidaProlongada = faccao.lider.vidaProlongada;
            lider.idade = std::clamp((faccao.lider.idadeNaHistoria * 10 + divisor / 2) / divisor, 8, 30);
            DinastiaDaFaccao dinastia = montarADinastiaDoComeco(lider, cultura, dados.regrasDaFamilia, populacao, sorteio);
            for (int estacao = 1; estacao <= 80; ++estacao) {
                const ResultadoDaEstacaoDaDinastia resultado = passarUmaEstacaoNaDinastia(dinastia, cultura, dados.regrasDaFamilia, populacao, estacao % 4 == 0, true, sorteio);
                mortesDeLider += resultado.liderMorreu ? 1 : 0;
                crises += resultado.crise ? 1 : 0;
            }
        }
    }
    std::printf("    dinastias em 1.000 mundos e 20 anos: %d mortes de líder, %d sem herdeiro adulto (%d por mil)\n", mortesDeLider, crises,
                mortesDeLider > 0 ? crises * 1000 / mortesDeLider : 0);
    conferir(mortesDeLider > 0 && crises * 10 < mortesDeLider, "teste 4: em 1.000 mundos, menos de 10% das mortes de líder ficam sem herdeiro adulto");
}


// Testes 1 e o presente: a fórmula do diálogo e da Opinião.
void testarODialogo(const DadosDoJogo& dados)
{
    const RegrasDosCompanheiros& regras = dados.regrasDosCompanheiros;
    Personalidade gananciosa;
    gananciosa.ganancia = 80;
    conferir(chanceDaAcaoDeDialogo(AcaoDeDialogo::Persuadir, 70, 20, gananciosa, regras) == 55, "teste 1: Carisma 70, Opinião +20, Ganância 80: 50 + 10 + 5 − 10 = 55%");
    conferir(chanceDaAcaoDeDialogo(AcaoDeDialogo::Subornar, 100, 100, gananciosa, regras) == 95, "a chance fica entre 5% e 95%");
    conferir(opiniaoDoPresente(100, 80, regras) == 6 && opiniaoDoPresente(1000, 80, regras) == 10, "o presente: 100 moedas a quem tem Ganância 80 dão +6; o teto é +10");
    Companheiro fiel;
    fiel.personalidade.lealdade = 80;
    fiel.opiniao = 20;
    mudarOpiniao(fiel, -10);
    conferir(fiel.opiniao == 13, "teste 2: com Lealdade 80, o salário atrasado tira 7 (10 × 70%)");
}

// Testes 2 e 3 numa partida de verdade: os limites, a entrada e o salário.
void testarOGrupo(const DadosDoJogo& dados)
{
    SimulacaoDaVila vila(dados, RegrasDoTempo{});
    const int mapa = vila.outrasCidades.front()->numeroDoMapaDaCidade;
    EntidadeEmTransito& grupo = vila.viagem.grupoDoJogador;
    grupo.mapaAtual = mapa;
    grupo.dinheiroEmDecimos = 100000;
    std::vector<Companheiro>& naTaverna = vila.companheiros.recrutaveisPorMapa[static_cast<std::size_t>(mapa - 1)];
    naTaverna.clear();
    for (int numero = 100; numero < 111; ++numero) {
        Companheiro recrutavel;
        recrutavel.numero = numero;
        recrutavel.nome = "R" + std::to_string(numero);
        recrutavel.mapaDeOrigem = mapa;
        recrutavel.personalidade.lealdade = 80;
        naTaverna.push_back(recrutavel);
    }
    std::vector<std::string> linhas;
    for (int numero = 100; numero < 110; ++numero) {
        fazerAcaoComOCompanheiro(vila, AcaoComOCompanheiro::Recrutar, numero, 0, linhas);
    }
    const std::string recusa = fazerAcaoComOCompanheiro(vila, AcaoComOCompanheiro::Recrutar, 110, 0, linhas);
    const EstadoDosCompanheiros& estado = vila.companheiros;
    conferir(estado.vinculados.size() == 10 && estado.vinculados[3].noGrupo && !estado.vinculados[4].noGrupo && !recusa.empty() && grupo.pessoas == 5
                 && grupo.lutadoresDoGrupo.size() == 4 && ladoDoGrupo(dados.regrasDoCombate, grupo).size() == 5,
             "teste 3: com Renome 0, o 5º recruta fica fora do grupo e o 11º não entra; o grupo luta com 5");
    conferir(grupo.dinheiroEmDecimos == 100000 - 10 * 1000, "a entrada: 100 moedas por recruta comum");

    // Teste 2: sem dinheiro, a 1ª estação atrasa e a 2ª manda todos embora.
    grupo.dinheiroEmDecimos = 0;
    vila.dinheiro.dinheiroDoJogador = 0;
    pagarVinculados(vila, linhas);
    const bool atrasouUma = vila.companheiros.vinculados.size() == 10 && vila.companheiros.vinculados[0].opiniao == 13;
    pagarVinculados(vila, linhas);
    conferir(atrasouUma && vila.companheiros.vinculados.empty(), "teste 2: sem dinheiro por 2 estações, os companheiros saem");
}

// Regra 4.8, passo 3: quem assume quando o jogador morre.
void testarOSucessorDoJogador()
{
    EstadoDosCompanheiros estado;
    Companheiro amigo;
    amigo.numero = 1;
    amigo.opiniao = 70;
    Companheiro outro;
    outro.numero = 2;
    outro.opiniao = 40;
    estado.vinculados = {amigo, outro};
    const bool peloMaiorOpiniao = escolherSucessorDoJogador(estado) == 1;
    estado.sucessorEscolhido = 2;  // com Opinião 40, a escolha não vale
    const bool escolhaSemOpiniao = escolherSucessorDoJogador(estado) == 1;
    Companheiro filho;
    filho.numero = 3;
    filho.idade = 6;
    filho.ehDaFamilia = true;
    filho.ehFilhoDoJogador = true;
    filho.opiniao = 0;
    estado.vinculados.push_back(filho);
    conferir(peloMaiorOpiniao && escolhaSemOpiniao && escolherSucessorDoJogador(estado) == 3 && escolherSucessorDoJogador(EstadoDosCompanheiros{}) == -1,
             "o sucessor do jogador: o filho adulto; sem filho, o de maior Opinião; sem ninguém, fim da partida");
}
}  // namespace

void testarCompanheiros()
{
    std::printf("Companheiros: as dinastias, a sucessão e a Vida prolongada (fase D2)\n");
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDoJogo);
    conferir(dados.regrasDaFamilia.chanceDeFilhoPorEstacaoEmPorcento == 10 && dados.regrasDaFamilia.fertilAte == 30, "o companheiros.json: 10% de filho por estação, fértil até 30");
    testarOHerdeiro();
    testarOComecoEAVidaProlongada(dados);
    testarAsDinastiasEm1000Mundos(dados);
    testarODialogo(dados);
    testarOGrupo(dados);
    testarOSucessorDoJogador();
}
