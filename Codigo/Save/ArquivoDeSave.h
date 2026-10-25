#pragma once

// Salvar e carregar a partida (Ficha Lógica: Save, marco 9). Este é o ponto de
// entrada: gravar a vila num arquivo ou em bytes, carregar de volta, e ler só o
// cabeçalho para a lista de saves.
//
// O save é uma árvore de seções com nome, convertida em MessagePack pela
// nlohmann/json (FL9-1). Só o estado vai no arquivo; as definições (itens,
// receitas, construções, eventos) vêm de data/, e o save guarda os IDs em texto
// delas (regra de ouro da ficha).

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Dados/DadosDoJogo.h"
#include "Tempo/RegrasDoTempo.h"
#include "Vila/SimulacaoDaVila.h"

// Sobe a cada mudança no que é gravado (a Felicidade ganha um fator, um campo novo...).
constexpr int versaoDoFormatoDoSave = 12;  // 2: o clima (fase A1); 3: o trabalho e o leilão (fase A2); 4: a população (fase A3); 5: a viagem (fase B1); 6: o comércio (fase B2); 7: as notícias (fase B3); 8: a reputação (fase B4); 9: a tecnologia (fase B5); 10: as facções (fase C1); 11: a cultura (fase C2); 12: o crime (fase C3)
constexpr const char* versaoDoJogoNoSave = "0.9";
constexpr int espacosDeSaveManual = 3;
constexpr const char* extensaoDoArquivoDeSave = ".save";
constexpr const char* nomeDoArquivoDoSaveAutomatico = "automatico.save";

// "espaco1.save" a "espaco3.save" (o espaço conta a partir de 1).
std::string nomeDoArquivoDoEspaco(int espaco);

// O que a lista de saves mostra de cada arquivo (a seção "cabecalho").
struct CabecalhoDoSave {
    int versaoDoFormato = 0;
    std::string versaoDoJogo;
    std::string dataReal;         // "2026-10-19 21:40"
    std::string dataDoJogo;       // "Dia 12 do Verão, Ano 412, 14h"
    std::string nomeDoEspaco;     // "Espaço 1", "Automático"
    int64_t tickAtual = 0;
};

struct ResultadoDoCarregamento {
    std::unique_ptr<SimulacaoDaVila> partida;  // vazia se o save foi recusado
    std::string motivoDaRecusa;                // "Este save é de outra versão do jogo (versão 1; o jogo usa a 2)."

    bool deuCerto() const { return partida != nullptr; }
};

// "Dia 12 do Verão, Ano 412, 14h".
std::string escreverADataDoJogoParaOSave(const RelogioDoMundo& relogio);

// Regra 4.1: monta a árvore e a converte em bytes. As datas entram no
// cabeçalho; passar as mesmas dá os mesmos bytes (o teste "ida e volta").
// A fila de pedidos do jogador não vai: quem salva aplica antes
// (SimulacaoDaVila::aplicarOsPedidosPendentes).
std::vector<uint8_t> gravarAPartidaEmBytes(const SimulacaoDaVila& simulacao, const std::string& nomeDoEspaco, const std::string& dataReal);

// Regra 4.2: lê os bytes e monta uma partida nova. Nunca mexe na partida
// antiga: se algo falhar, devolve só o motivo da recusa.
ResultadoDoCarregamento carregarAPartidaDeBytes(const DadosDoJogo& dados, const RegrasDoTempo& regrasDoTempo,
                                                int distanciaDoCicloDeAndarEmMilesimos, const std::vector<uint8_t>& bytes);

// Regra 4.1, passo 4: grava num arquivo temporário (caminho + ".tmp") e só
// depois troca pelo arquivo de verdade. Devolve o erro em texto, ou vazio se deu certo.
std::string gravarAPartidaNoArquivo(const SimulacaoDaVila& simulacao, const std::string& caminhoDoArquivo, const std::string& nomeDoEspaco,
                                    const std::string& dataReal);

ResultadoDoCarregamento carregarAPartidaDoArquivo(const DadosDoJogo& dados, const RegrasDoTempo& regrasDoTempo,
                                                  int distanciaDoCicloDeAndarEmMilesimos, const std::string& caminhoDoArquivo);

// Só o cabeçalho (para a lista de saves). Vazio se o arquivo não existe ou está quebrado.
std::optional<CabecalhoDoSave> lerOCabecalhoDoArquivo(const std::string& caminhoDoArquivo);

// O save em texto (JSON com recuo), para investigar: "jogo --save-para-json arquivo.save".
// Devolve o motivo no lugar do texto se o arquivo não pôde ser lido; "deuCerto" diz qual dos dois.
std::string converterOSaveEmTexto(const std::vector<uint8_t>& bytes, bool& deuCerto);

// Os bytes de um arquivo (vazio se não existe) e a gravação segura.
std::optional<std::vector<uint8_t>> lerOsBytesDoArquivo(const std::string& caminhoDoArquivo);
std::string gravarOsBytesNoArquivo(const std::vector<uint8_t>& bytes, const std::string& caminhoDoArquivo);
