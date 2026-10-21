#include "Vila/Populacao.h"

#include <algorithm>

#include "Construcao/ContasDasConstrucoes.h"
#include "Eventos/EventosDaVila.h"
#include "Npc/ComponentesDoNpc.h"
#include "Npc/MovimentoDosNpcs.h"

namespace {

const char* const identificadorDoInvernoRigoroso = "evento.inverno_rigoroso";

bool invernoRigorosoEstaAtivo(const ContextoDaProducao& contexto)
{
    for (const EventoAtivo& ativo : contexto.eventos.eventosAtivos) {
        if (contexto.dados.catalogoDeEventos.todosOsEventos()[ativo.indiceDoEvento].identificadorDeTexto == identificadorDoInvernoRigoroso) {
            return true;
        }
    }
    return false;
}

// Os NPCs da vila na ordem da vila (a lista da simulação).
int quantidadeDeNpcs(const ContextoDaProducao& contexto)
{
    return static_cast<int>(contexto.npcsDaVila.size());
}

int moradoresQueCabem(const ContextoDaProducao& contexto, int indiceDaConstrucao)
{
    const DefinicaoDeConstrucao* definicao = contexto.dados.catalogoDeConstrucoes.procurarConstrucao(contexto.vila.construcoes[indiceDaConstrucao].tipo);
    return definicao == nullptr ? 0 : definicao->moradores;
}

// Regra 4.5, passo 1: a Casa com mais camas livres (no empate, a primeira); -1 sem cama.
int casaComMaisCamasLivres(ContextoDaProducao& contexto)
{
    std::vector<int> moradores(contexto.vila.construcoes.size(), 0);
    for (const entt::entity npc : contexto.npcsDaVila) {
        const int casa = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDaCasa;
        if (casa >= 0) {
            ++moradores[casa];
        }
    }
    int melhorCasa = -1;
    int maisCamasLivres = 0;
    for (int indice = 0; indice < static_cast<int>(contexto.vila.construcoes.size()); ++indice) {
        if (contexto.vila.construcoes[indice].emObra) {
            continue;
        }
        const int camasLivres = moradoresQueCabem(contexto, indice) - moradores[indice];
        if (camasLivres > maisCamasLivres) {
            maisCamasLivres = camasLivres;
            melhorCasa = indice;
        }
    }
    return melhorCasa;
}

bool haVagaDeEmpregoLivre(ContextoDaProducao& contexto)
{
    std::vector<int> empregados(contexto.vila.construcoes.size(), 0);
    for (const entt::entity npc : contexto.npcsDaVila) {
        const int trabalho = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
        if (trabalho >= 0) {
            ++empregados[trabalho];
        }
    }
    for (int indice = 0; indice < static_cast<int>(contexto.vila.construcoes.size()); ++indice) {
        const ConstrucaoDaVila& construcao = contexto.vila.construcoes[indice];
        const DefinicaoDeConstrucao* definicao = contexto.dados.catalogoDeConstrucoes.procurarConstrucao(construcao.tipo);
        if (!construcao.emObra && definicao != nullptr && !definicao->funcao.empty()
            && empregados[indice] < calcularVagas(construcao.nivel, contexto.dados.regras.construcoes)) {
            return true;
        }
    }
    return false;
}

int contarDesempregados(ContextoDaProducao& contexto)
{
    int desempregados = 0;
    for (const entt::entity npc : contexto.npcsDaVila) {
        desempregados += contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho < 0 ? 1 : 0;
    }
    return desempregados;
}

std::string nomeDoNpc(ContextoDaProducao& contexto, entt::entity npc)
{
    return contexto.registroDasEntidades.get<IdentidadeDoNpc>(npc).nome;
}

int idadeDoNpc(ContextoDaProducao& contexto, entt::entity npc)
{
    const IdadeDoNpc* idade = contexto.registroDasEntidades.try_get<IdadeDoNpc>(npc);
    return idade == nullptr ? 0 : idade->anos;
}

// Regra 4.6, passo 1: o mais velho (no empate, o de maior número). Com uma
// doença que mata (a Epidemia, CU1-4), o mais velho entre os doentes.
entt::entity npcMaisVelho(ContextoDaProducao& contexto)
{
    entt::entity escolhido = entt::null;
    int maiorIdade = -1;
    int maiorNumero = -1;
    const bool soOsDoentes = algumaDoencaMataHoje(contexto) && std::any_of(contexto.npcsDaVila.begin(), contexto.npcsDaVila.end(), [&](entt::entity npc) {
        return contexto.registroDasEntidades.all_of<DoencaDoNpc>(npc);
    });
    for (const entt::entity npc : contexto.npcsDaVila) {
        if (soOsDoentes && !contexto.registroDasEntidades.all_of<DoencaDoNpc>(npc)) {
            continue;
        }
        const int idade = idadeDoNpc(contexto, npc);
        const int numero = contexto.registroDasEntidades.get<IdentidadeDoNpc>(npc).numeroDoNpc;
        if (idade > maiorIdade || (idade == maiorIdade && numero > maiorNumero)) {
            escolhido = npc;
            maiorIdade = idade;
            maiorNumero = numero;
        }
    }
    return escolhido;
}

// Regra 4.4, passo 2: o desempregado de menor habilidade (no empate, o de
// maior número); sem desempregados, o empregado da Cidade de menor habilidade.
entt::entity npcQueVaiEmbora(ContextoDaProducao& contexto)
{
    for (const bool procurarDesempregado : {true, false}) {
        entt::entity escolhido = entt::null;
        int menorHabilidade = 1000;
        int maiorNumero = -1;
        for (const entt::entity npc : contexto.npcsDaVila) {
            const int trabalho = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
            const bool candidato = procurarDesempregado ? trabalho < 0 : (trabalho >= 0 && !contexto.vila.construcoes[trabalho].donoEhOJogador);
            if (!candidato) {
                continue;
            }
            const HabilidadesDoNpc& habilidades = contexto.registroDasEntidades.get<HabilidadesDoNpc>(npc);
            const int habilidade = std::max(habilidades.bracal, habilidades.manufaturado);
            const int numero = contexto.registroDasEntidades.get<IdentidadeDoNpc>(npc).numeroDoNpc;
            if (habilidade < menorHabilidade || (habilidade == menorHabilidade && numero > maiorNumero)) {
                escolhido = npc;
                menorHabilidade = habilidade;
                maiorNumero = numero;
            }
        }
        if (escolhido != entt::null) {
            return escolhido;
        }
    }
    return entt::null;
}

// Regra 4.5 para o nascido (idade 4, habilidade 50) e para o migrante.
entt::entity trazerUmNpc(ContextoDaProducao& contexto, bool ehMigrante, std::vector<std::string>& linhas)
{
    const RegrasDaPopulacao& regras = contexto.dados.regras.populacao;
    if (!ehMigrante) {
        const entt::entity npc = criarNpcNaVila(contexto, regras.idadeAdulto, 50, 50);
        if (npc != entt::null) {
            ++contexto.populacao.nascimentosDeHoje;
            linhas.push_back("População: nasceu um novo morador, " + nomeDoNpc(contexto, npc) + ".");
        }
        return npc;
    }
    SorteioComSemente& sorteio = contexto.sorteioDaPopulacao;
    const int idade = sorteio.sortearNumeroInteiroEntre(regras.idadeDoMigranteMinima, regras.idadeDoMigranteMaxima);
    const int bracal = sorteio.sortearNumeroInteiroEntre(regras.habilidadeDoMigranteMinima, regras.habilidadeDoMigranteMaxima);
    const int manufaturado = sorteio.sortearNumeroInteiroEntre(regras.habilidadeDoMigranteMinima, regras.habilidadeDoMigranteMaxima);
    const entt::entity npc = criarNpcNaVila(contexto, idade, bracal, manufaturado);
    if (npc != entt::null) {
        ++contexto.populacao.migracaoDeHoje;
        linhas.push_back("População: " + nomeDoNpc(contexto, npc) + " chegou à vila.");
    }
    return npc;
}

// O acumulado passou de +100 ou de −100: chega ou morre 1 NPC (regras 4.2, passo 6, e 4.5 e 4.6).
void resolverOAcumulado(ContextoDaProducao& contexto, std::vector<std::string>& linhas)
{
    EstadoDaPopulacao& populacao = contexto.populacao;
    const int fator = contexto.dados.regras.populacao.fatorDeEscala;
    while (populacao.acumuladoDeCrescimentoEmPessoas >= fator) {
        if (quantidadeDeNpcs(contexto) >= capacidadeDaVila(contexto) || trazerUmNpc(contexto, false, linhas) == entt::null) {
            populacao.acumuladoDeCrescimentoEmPessoas = fator;  // a vila cheia (ou sem cama) segura em +100
            return;
        }
        populacao.acumuladoDeCrescimentoEmPessoas -= fator;
    }
    while (populacao.acumuladoDeCrescimentoEmPessoas <= -fator && quantidadeDeNpcs(contexto) > 0) {
        const entt::entity npc = npcMaisVelho(contexto);
        linhas.push_back("População: " + nomeDoNpc(contexto, npc) + " morreu (fome, frio ou doença) aos " + std::to_string(idadeDoNpc(contexto, npc)) + " anos.");
        tirarNpcDaVila(contexto, npc);
        ++populacao.mortesDeHoje;
        populacao.acumuladoDeCrescimentoEmPessoas += fator;
    }
    populacao.acumuladoDeCrescimentoEmPessoas = std::clamp(populacao.acumuladoDeCrescimentoEmPessoas, -fator, fator);
}

}  // namespace

int capacidadeDaVila(const ContextoDaProducao& contexto)
{
    // A vila é de nível 1 até a ficha da Cidade (fase A5); as outras cidades têm o nível delas (B1b).
    const std::vector<int>& capacidades = contexto.dados.regras.populacao.capacidadePorNivelDaCidade;
    if (capacidades.empty()) {
        return 15;
    }
    return capacidades[static_cast<std::size_t>(std::clamp(contexto.vila.nivelDaCidade, 1, static_cast<int>(capacidades.size())) - 1)];
}

int calcularCrescimentoDaEstacao(int npcsReais, int felicidadeEmDecimos, bool houveFome, const RegrasDaPopulacao& regras)
{
    const int populacaoTotal = npcsReais * regras.fatorDeEscala;
    // Felicidade × 2, entre 50% e 150% (50 = 100%); a fome na estação: 50%.
    const int modificadorDaFelicidade = std::clamp(felicidadeEmDecimos / 5, 50, 150);
    const int modificadorDaComida = houveFome ? 50 : 100;
    const int nascimentos = populacaoTotal * regras.natalidadePorEstacaoEmPorMil / 1000 * modificadorDaFelicidade / 100 * modificadorDaComida / 100;
    const int mortalidade = populacaoTotal * regras.mortalidadePorEstacaoEmPorMil / 1000;
    return nascimentos - mortalidade;
}

int chanceDeMorrerDeVelhice(int idade, bool invernoRigoroso, const RegrasDaPopulacao& regras)
{
    if (idade < regras.idadeVelho) {
        return 0;
    }
    const int chance = (idade - (regras.idadeVelho - 1)) * regras.chanceDeMorrerDeVelhicePorAnoAcimaDe20EmPorcento;
    return invernoRigoroso ? chance * regras.velhiceNoInvernoRigorosoEmPorcento / 100 : chance;
}

entt::entity criarNpcNaVila(ContextoDaProducao& contexto, int idade, int habilidadeBracal, int habilidadeManufaturado)
{
    const int casa = casaComMaisCamasLivres(contexto);
    if (casa < 0) {
        return entt::null;
    }
    const RegrasDaPopulacao& regras = contexto.dados.regras.populacao;
    EstadoDaPopulacao& populacao = contexto.populacao;
    const int numero = populacao.proximoNumeroDeNpc++;
    const std::vector<std::string>& nomes = regras.nomesDosNovosNpcs;
    // Os nomes da lista na ordem, a partir do primeiro NPC novo; quando a lista
    // acaba, eles se repetem com um número para não haver dois iguais.
    const std::size_t posicaoNaLista = static_cast<std::size_t>(std::max(0, numero - static_cast<int>(contexto.vila.npcsIniciais.size())));
    std::string nome = nomes.empty() ? "Morador" : nomes[posicaoNaLista % nomes.size()];
    if (!nomes.empty() && posicaoNaLista >= nomes.size()) {
        nome += " " + std::to_string(posicaoNaLista / nomes.size() + 1);
    }
    entt::registry& registro = contexto.registroDasEntidades;
    // Aparece na porta da casa (o ponto "entrada_do_mapa" do Tiled fica para quando o mapa o tiver).
    const entt::entity npc = criarNpcNoTile(registro, contexto.vila.construcoes[casa].tileDaPorta);
    registro.emplace<IdentidadeDoNpc>(npc, IdentidadeDoNpc{nome, "civil", numero});
    registro.emplace<CasaETrabalhoDoNpc>(npc, CasaETrabalhoDoNpc{casa, -1, -1});
    DecisaoDoNpc decisao;
    decisao.indiceDaConstrucaoOndeEsta = casa;
    registro.emplace<DecisaoDoNpc>(npc, decisao);
    registro.emplace<HabilidadesDoNpc>(npc, HabilidadesDoNpc{habilidadeBracal, habilidadeManufaturado});
    registro.emplace<TrabalhoDoDia>(npc);
    registro.emplace<IdadeDoNpc>(npc, IdadeDoNpc{idade});
    contexto.npcsDaVila.push_back(npc);
    ++contexto.dinheiro.quantidadeDeCivis;
    return npc;
}

void tirarNpcDaVila(ContextoDaProducao& contexto, entt::entity npc)
{
    const int trabalho = contexto.registroDasEntidades.get<CasaETrabalhoDoNpc>(npc).indiceDoTrabalho;
    contexto.registroDasEntidades.destroy(npc);
    contexto.npcsDaVila.erase(std::remove(contexto.npcsDaVila.begin(), contexto.npcsDaVila.end(), npc), contexto.npcsDaVila.end());
    contexto.dinheiro.quantidadeDeCivis = std::max(0, contexto.dinheiro.quantidadeDeCivis - 1);
    if (trabalho >= 0) {
        escolherOsGerentes(contexto);  // a vaga abre; o gerente é escolhido de novo
    }
}

std::vector<std::string> rodarAPopulacaoDoDia(ContextoDaProducao& contexto, const RelogioDoMundo& relogio)
{
    std::vector<std::string> linhas;
    const RegrasDaPopulacao& regras = contexto.dados.regras.populacao;
    EstadoDaPopulacao& populacao = contexto.populacao;
    populacao.nascimentosDeHoje = 0;
    populacao.mortesDeHoje = 0;
    populacao.migracaoDeHoje = 0;

    // Regra 4.3: a fome (do 4º dia seguido em diante) e o frio somam mortes.
    const DinheiroDaVila& dinheiro = contexto.dinheiro;
    const int comidaPorDia = std::max(contexto.dados.regras.economia.comidaPorDiaDoCivil, 1);
    if (dinheiro.diasEmDeficitDeComida > regras.diasDeFomeAntesDeMatar) {
        const int civisSemComida = std::min(dinheiro.quantidadeDeCivis, (dinheiro.comidaQueFaltouHoje + comidaPorDia - 1) / comidaPorDia);
        populacao.acumuladoDeCrescimentoEmPessoas -= civisSemComida * regras.fatorDeEscala * regras.fomeMataPorDiaEmPorcentoDeQuemFicouSemComida / 100;
    }
    if (contexto.clima.faltouLenhaHoje) {
        const int populacaoTotal = quantidadeDeNpcs(contexto) * regras.fatorDeEscala;
        populacao.acumuladoDeCrescimentoEmPessoas -=
            populacaoTotal * regras.mortalidadePorEstacaoEmPorMil / 1000 / relogio.diasPorEstacao() * (regras.mortalidadeSemLenhaEmPorcento - 100) / 100;
    }
    // Fase A6 (ficha da Cura, regra 4.2): as mortes da Epidemia, com ou sem o Remédio.
    populacao.acumuladoDeCrescimentoEmPessoas -= mortesDasDoencasHojeEmPessoas(contexto);
    if (dinheiro.diasEmDeficitDeComida > 0) {
        ++populacao.diasDeFomeNaEstacao;
    }

    if (relogio.diaDaEstacaoAtual() == 1 && relogio.diaDaPartida() > 0) {
        // Regra 4.4, passo 2, decidida antes das mortes e dos nascimentos de hoje:
        // vale a vila da estação que terminou (quem chega hoje chega desempregado,
        // e a Cidade só contrata amanhã; sem isso, uma reposição grande expulsava alguém).
        const int felicidadeDaEstacao = contexto.felicidade.felicidadeEmDecimos / 10;
        const int npcsDaEstacao = quantidadeDeNpcs(contexto);
        const bool alguemVaiEmbora = npcsDaEstacao > 0
            && (felicidadeDaEstacao < regras.migranteSaiComFelicidadeAbaixoDe
                || contarDesempregados(contexto) * 100 >= regras.migranteSaiComDesempregoEmPorcento * npcsDaEstacao);

        // Regra 4.1: a idade (dia 1 da Primavera) e a velhice, com a reposição.
        if (relogio.estacaoAtual() == Estacao::Primavera) {
            for (const entt::entity npc : contexto.npcsDaVila) {
                if (IdadeDoNpc* idade = contexto.registroDasEntidades.try_get<IdadeDoNpc>(npc)) {
                    ++idade->anos;
                }
            }
        }
        const bool rigoroso = invernoRigorosoEstaAtivo(contexto);
        const std::vector<entt::entity> npcsAntes = contexto.npcsDaVila;
        for (const entt::entity npc : npcsAntes) {
            const int idade = idadeDoNpc(contexto, npc);
            if (idade < regras.idadeVelho) {
                continue;
            }
            const int sorteio = contexto.sorteioDaPopulacao.sortearNumeroInteiroEntre(0, 99);
            if (sorteio < chanceDeMorrerDeVelhice(idade, rigoroso, regras)) {
                linhas.push_back("População: " + nomeDoNpc(contexto, npc) + " morreu de velhice aos " + std::to_string(idade) + " anos.");
                tirarNpcDaVila(contexto, npc);
                ++populacao.mortesDeHoje;
                trazerUmNpc(contexto, false, linhas);  // a reposição: um jovem na vaga, sem mexer no acumulado
            }
        }

        // Regra 4.2: nascimentos e mortalidade da estação.
        if (quantidadeDeNpcs(contexto) < capacidadeDaVila(contexto) && populacao.acumuladoDeCrescimentoEmPessoas < 0) {
            populacao.acumuladoDeCrescimentoEmPessoas = 0;  // PO1-4: depois de uma crise, a dívida não dura anos
        }
        populacao.acumuladoDeCrescimentoEmPessoas +=
            calcularCrescimentoDaEstacao(quantidadeDeNpcs(contexto), contexto.felicidade.felicidadeEmDecimos, populacao.diasDeFomeNaEstacao > 0, regras);

        // Regra 4.4: a migração.
        const int felicidade = contexto.felicidade.felicidadeEmDecimos / 10;
        const bool cabeMais = quantidadeDeNpcs(contexto) < capacidadeDaVila(contexto);
        if (cabeMais && felicidade >= regras.migranteChegaComFelicidadeMinima && populacao.diasDeFomeNaEstacao == 0 && haVagaDeEmpregoLivre(contexto)) {
            const bool vilaMeioVazia = quantidadeDeNpcs(contexto) * 2 < capacidadeDaVila(contexto);
            trazerUmNpc(contexto, true, linhas);
            if (vilaMeioVazia && quantidadeDeNpcs(contexto) < capacidadeDaVila(contexto)) {
                trazerUmNpc(contexto, true, linhas);  // CI1-5: abaixo de 50%, chega mais 1
            }
        }
        if (alguemVaiEmbora) {
            const entt::entity quemSai = npcQueVaiEmbora(contexto);
            if (quemSai != entt::null) {
                linhas.push_back("População: " + nomeDoNpc(contexto, quemSai) + " foi embora da vila.");
                tirarNpcDaVila(contexto, quemSai);
                --populacao.migracaoDeHoje;
            }
        }
        populacao.diasDeFomeNaEstacao = 0;
    }

    // Regras 4.2, passo 6, 4.5 e 4.6: o acumulado vira NPCs.
    resolverOAcumulado(contexto, linhas);
    return linhas;
}
