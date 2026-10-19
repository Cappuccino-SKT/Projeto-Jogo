#pragma once

// A tela inicial (Ficha Lógica: Save, parte 4.3, FL9-6): Continuar, Novo jogo,
// Carregar e Sair, em ImGui. Só mostra e devolve o que o jogador pediu; quem
// salva e carrega é o laço principal.

#include <string>
#include <vector>

#include "ListaDeSaves.h"

class TelaInicial {
public:
    // Lê a pasta dos saves de novo (ao abrir a tela e depois de salvar).
    void abrir(const std::string& pastaDosSaves);

    // Chamar dentro do quadro da ImGui. "mensagem" aparece embaixo (por
    // exemplo, "Este save é de outra versão do jogo...").
    PedidoDeSave desenhar(float larguraDaTela, float alturaDaTela, const std::string& mensagem);

private:
    std::string pastaDosSaves;
    std::vector<EspacoDeSave> espacos;
    bool mostrandoALista = false;
};
