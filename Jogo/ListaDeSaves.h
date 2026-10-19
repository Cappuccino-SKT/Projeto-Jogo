#pragma once

// A lista de saves na tela (Ficha Lógica: Save, parte 4.3, marco 9): os 3 espaços
// manuais e o automático, com o que o cabeçalho de cada arquivo diz. Mais a
// pasta dos saves (FL9-4), a data real de agora e o desenho da lista em ImGui,
// usado pela tela inicial e pelo menu do Esc.

#include <optional>
#include <string>
#include <vector>

#include "Save/ArquivoDeSave.h"

// O que uma tela de save pede ao laço principal.
enum class TipoDePedidoDeSave {
    Nenhum,
    NovoJogo,
    Continuar,             // o save mais novo
    CarregarEspaco,        // "espaco" = 1 a 3, ou 0 para o automático
    SalvarNoEspaco,        // "espaco" = 1 a 3
    SairParaATelaInicial,
    FecharOJogo,
};

struct PedidoDeSave {
    TipoDePedidoDeSave tipo = TipoDePedidoDeSave::Nenhum;
    int espaco = 0;
};

struct EspacoDeSave {
    int espaco = 0;                          // 1 a 3; 0 = o automático
    std::string rotulo;                      // "Espaço 1", "Automático"
    std::string caminhoDoArquivo;
    std::optional<CabecalhoDoSave> cabecalho;  // vazio = espaço vazio (ou arquivo quebrado)

    bool temSave() const { return cabecalho.has_value(); }
};

// A pasta dos saves, com a barra no fim: SDL_GetPrefPath + "saves/" (no Windows,
// C:\Users\<usuário>\AppData\Roaming\Projeto-Jogo\saves\). Vazia se a SDL não achar a pasta.
std::string descobrirAPastaDosSaves();

// Os 3 espaços manuais e depois o automático.
std::vector<EspacoDeSave> listarOsEspacosDeSave(const std::string& pastaDosSaves);

// O mais novo pela data real (no empate, pelo tick do jogo), ou vazio se não há nenhum save.
std::optional<EspacoDeSave> acharOSaveMaisNovo(const std::vector<EspacoDeSave>& espacos);

// "2026-10-19 21:40", pela hora do computador.
std::string escreverADataRealDeAgora();

// "Espaço 1: Dia 12 do Verão, Ano 412, 14h (salvo em 2026-10-19 21:40)" ou "Espaço 2: (vazio)".
std::string escreverUmEspacoParaALista(const EspacoDeSave& espaco);

// Desenha a lista com um botão por espaço e devolve o clicado (o "espaco" dele), ou
// -1 se nada foi clicado. Para salvar, o automático não aparece; para carregar,
// os espaços vazios ficam desligados.
int desenharAListaDeEspacos(const std::vector<EspacoDeSave>& espacos, bool paraSalvar);
