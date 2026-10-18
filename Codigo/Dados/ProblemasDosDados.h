#pragma once

// A lista de erros e avisos achados ao ler os dados. Cada problema diz o
// arquivo, a linha e o motivo, para o autor achar e corrigir na planilha.
// Um erro não para a leitura: o jogo lê tudo e mostra todos de uma vez.

#include <string>
#include <vector>

struct ProblemaDosDados {
    bool ehErro = true;          // false = só um aviso
    std::string nomeDoArquivo;   // por exemplo, "itens.csv"
    int numeroDaLinha = 0;       // 0 quando o problema não é de uma linha
    std::string motivo;

    // Por exemplo: "itens.csv, linha 4: o tipo "pão" não existe".
    std::string textoParaMostrar() const;
};

class ListaDeProblemasDosDados {
public:
    void anotarErro(const std::string& nomeDoArquivo, int numeroDaLinha, const std::string& motivo);
    void anotarAviso(const std::string& nomeDoArquivo, int numeroDaLinha, const std::string& motivo);

    bool temErro() const { return quantidadeDeErros() > 0; }
    int quantidadeDeErros() const;
    int quantidadeDeAvisos() const;
    const std::vector<ProblemaDosDados>& todosOsProblemas() const { return problemas; }

    // Procura um trecho no motivo de algum erro (usado nos testes).
    bool algumErroContem(const std::string& trecho) const;
    bool algumAvisoContem(const std::string& trecho) const;

private:
    std::vector<ProblemaDosDados> problemas;
};
