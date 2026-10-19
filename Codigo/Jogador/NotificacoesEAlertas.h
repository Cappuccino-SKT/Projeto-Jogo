#pragma once

// As notificações, o Diário e a barra de alertas (Ficha Lógica: Interface,
// regras 10.3 e 10.4, marco 8), mais o texto do dinheiro. Nada aqui desenha:
// o Jogo/ só mostra o que estas funções devolvem.

#include <cstdint>
#include <string>
#include <vector>

#include "Producao/ProducaoDaVila.h"

// FL8-9: Info para as boas, Aviso para as ruins.
enum class NivelDaNotificacao { Info, Aviso };

struct Notificacao {
    NivelDaNotificacao nivel = NivelDaNotificacao::Info;
    std::string texto;
    int64_t diaDaPartida = 0;   // contando do 0
    int64_t tickDeEntrada = 0;  // o tick do relógio em que entrou
};

// 5 na tela por 20 segundos reais a 1x (400 ticks), e as 300 últimas no Diário.
constexpr int notificacoesNaTelaNoMaximo = 5;
constexpr int64_t ticksQueUmaNotificacaoFicaNaTela = 400;
constexpr std::size_t notificacoesNoDiarioNoMaximo = 300;

class NotificacoesDaVila {
public:
    void acrescentar(NivelDaNotificacao nivel, const std::string& texto, int64_t diaDaPartida, int64_t tickAtual);
    // As da tela agora: as mais novas primeiro, até 5, as que entraram há menos de 400 ticks.
    std::vector<const Notificacao*> naTela(int64_t tickAtual) const;
    // O Diário: da mais nova para a mais velha.
    const std::vector<Notificacao>& doDiario() const { return diario; }

private:
    std::vector<Notificacao> diario;  // da mais velha para a mais nova
};

// Uma linha do console vira notificação? "Evento: ...", "Fim do evento: ...",
// "Aviso: ...", "Obra pronta: ..." e "Obra encomendada: ..." viram. Devolve
// false para as outras (o resumo do dia, por exemplo). O evento ruim é Aviso.
bool classificarALinhaDoConsole(const ContextoDaProducao& contexto, const std::string& linha, NivelDaNotificacao& nivel);

// Regra 10.4: os alertas que valem agora (a cada hora de jogo).
std::vector<std::string> calcularOsAlertasDaVila(const ContextoDaProducao& contexto);

// Dinheiro para mostrar, com ponto de milhar: 39500 vira "3.950,0".
std::string escreverDinheiroComMilhar(int64_t valorEmDecimos);
