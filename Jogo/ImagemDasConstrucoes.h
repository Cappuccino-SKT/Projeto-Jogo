#pragma once

// As imagens das construções (Ficha Lógica: Construção, regra 4.3; Guia da
// Arte, parte 7; FA1-5). Só as contas, sem a placa de vídeo, para o teste.
//
// Cada construção que o autor desenhar aparece no jogo sem mexer no código:
// basta salvar data/imagens/construcoes/<tipo>_nivel_<n>.png. Sem a imagem, o
// jogo desenha o retângulo de cor do marco 5.

#include <string>

#include "Mapa/MapaLocal.h"

// A imagem tem 32 px a mais que o lote (o telhado e a parede da frente
// "sobem" na vista 3/4): pequena 64 × 96, média 96 × 128 (FA1-5).
constexpr int alturaDaImagemAcimaDoLoteEmPixels = 32;

// "construcao.casa" no nível 1 -> "imagens/construcoes/casa_nivel_1.png" (dentro da pasta dos dados).
std::string caminhoDaImagemDaConstrucao(const std::string& tipoDaConstrucao, int nivel);

// A largura é a do lote; a altura, a do lote mais 32 px.
bool tamanhoDaImagemDaConstrucaoEstaCerto(int larguraEmPixels, int alturaEmPixels, const RetanguloEmTiles& areaDoLote, int tamanhoDoTileEmPixels);
