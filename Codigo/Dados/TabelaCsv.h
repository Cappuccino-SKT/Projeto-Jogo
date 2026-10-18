#pragma once

// O leitor de CSV próprio (rodada FT2-2): as planilhas salvas em CSV, com
// ponto e vírgula separando as colunas (o padrão do Excel e do LibreOffice
// em português). Usado pelos itens, recursos e receitas.

#include <string>
#include <vector>

struct LinhaDaTabelaCsv {
    int numeroDaLinhaNoArquivo = 0;   // como a planilha mostra: o cabeçalho é a linha 1
    std::vector<std::string> valoresDasColunas;
};

struct TabelaCsv {
    std::vector<std::string> nomesDasColunas;
    std::vector<LinhaDaTabelaCsv> linhas;  // sem o cabeçalho, as vazias e os comentários

    // A posição da coluna pelo nome, ou -1 se ela não existe.
    int posicaoDaColuna(const std::string& nomeDaColuna) const;

    // O valor de uma coluna numa linha; "" se a coluna não existe ou a linha é mais curta
    // (a planilha costuma cortar as colunas vazias do fim).
    std::string valorNaLinha(const LinhaDaTabelaCsv& linha, const std::string& nomeDaColuna) const;
};

// Separa o texto do arquivo em linhas e colunas. Ignora a marca BOM que o
// Excel põe no começo, as linhas vazias e as que começam com '#'.
TabelaCsv lerTabelaCsv(const std::string& textoDoArquivo);

// Lê um número inteiro sem nada antes nem depois ("12" sim; "12 ", "1,5" e "" não).
bool lerNumeroInteiroDoTexto(const std::string& texto, int& numeroLido);

// O ID tem o prefixo certo e só letras minúsculas sem acento, números e '_'
// depois dele. Exemplo: identificadorEhValido("item.pao", "item.") é verdadeiro.
bool identificadorEhValido(const std::string& identificador, const std::string& prefixoEsperado);
