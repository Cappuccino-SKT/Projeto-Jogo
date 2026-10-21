#pragma once

// O data/viagem.json (Ficha Lógica: Viagem, parte 2). Os valores padrão são os da ficha.

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Dados/ProblemasDosDados.h"

struct RegrasDaViagem {
    int ticksBasePorMapa = 3200;  // 4 horas a pé
    int ticksMinimosPorMapa = 800;
    std::map<std::string, int> terrenoEmPorcento;
    std::map<std::string, int> infraestruturaEmPorcento;
    std::vector<std::pair<int, int>> tamanhoDoGrupo;  // [até tantas pessoas, multiplicador]
    int montadosEmPorcento = 67;
    int cargaAcimaDoLimiteEmPorcento = 125;
    int caravanaEmPorcento = 133;
    std::map<std::string, int> climaEmPorcento;  // pelo nome do clima no clima.json; o que falta vale 100
    int lamaNaPrimaveraSemEstradaEmPorcento = 110;
    int invernoNoBiomaFrioEmPorcento = 150;
    int invernoNosOutrosBiomasEmPorcento = 110;
    std::set<std::string> biomasFrios = {"frio"};
    int tetoDoClimaVezesEstacaoEmPorcento = 200;
    int horaDeAcampar = 22;
    int horaDeRetomar = 6;
    int refeicoesPorPessoaPorDia = 1;
    int encontroNoTerritorioPorMil = 50;  // vale também para os mapas com cidade
    int encontroEmDisputaPorMil = 150;
    int encontroSemDonoPorMil = 200;
    int marchaForcadaEncontroEmPorcento = 150;
    int pesoDosBandidos = 75;
    int pesoDoMercadorViajante = 25;
    int bandoDeBandidosMinimo = 3;
    int bandoDeBandidosSorteioAte = 9;
    std::map<std::string, int> forcaPorPessoa;
    int bandidosNaoAtacamSeOGrupoForVezesMaisForte = 2;
    int subornoPorBandidoEmDecimos = 250;
    int carismaDoJogadorAteOsAtributos = 40;  // até o Combate (fase D) dar os atributos
    int fugaBaseEmPorcento = 50;
    int fugaMinimaEmPorcento = 10;
    int fugaMaximaEmPorcento = 90;
    int perdaAoSeRenderEmPorcento = 50;
    int perdaAoPerderALutaEmPorcento = 75;
    int lutaRapidaDuraTicks = 800;
    int perdaDeCargaDaCaravanaMinimaEmPorcento = 30;

    int forcaDe(const std::string& tipoDePessoa) const;
};

RegrasDaViagem lerArquivoDaViagem(const std::string& textoDoArquivo, ListaDeProblemasDosDados& listaDeProblemas);
