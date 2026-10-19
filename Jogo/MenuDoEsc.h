#pragma once

// O menu do Esc durante o jogo (Ficha Lógica: Save, parte 4.3, FL9-6): Voltar,
// Salvar, Carregar e Sair para a tela inicial. Sair ou carregar pede
// confirmação quando passou mais de 1 hora de jogo sem salvar.

#include <string>
#include <vector>

#include "ListaDeSaves.h"

class MenuDoEsc {
public:
    bool estaAberto() const { return aberto; }
    // Abre na tela principal e lê a pasta dos saves de novo.
    void abrir(const std::string& pastaDosSaves);
    // Abre já perguntando se pode fazer isto (o X da janela, o F9) com progresso não salvo.
    void abrirPerguntandoPor(const std::string& pastaDosSaves, const PedidoDeSave& pedido);
    void fechar() { aberto = false; }

    // Chamar dentro do quadro da ImGui. "horasSemSalvar" são as horas de jogo
    // desde o último save; acima de 1, os pedidos que perdem progresso confirmam antes.
    PedidoDeSave desenhar(float larguraDaTela, float alturaDaTela, int horasSemSalvar, const std::string& mensagem);

private:
    enum class Subtela { Principal, Salvar, Carregar, Confirmar };

    // Devolve o pedido já, ou guarda e vai para a confirmação.
    PedidoDeSave pedirComConfirmacao(PedidoDeSave pedido, int horasSemSalvar);

    bool aberto = false;
    Subtela subtela = Subtela::Principal;
    std::string pastaDosSaves;
    std::vector<EspacoDeSave> espacos;
    PedidoDeSave pedidoAguardandoConfirmacao;
};
