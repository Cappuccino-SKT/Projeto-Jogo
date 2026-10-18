#include "Dados/ProblemasDosDados.h"

std::string ProblemaDosDados::textoParaMostrar() const
{
    std::string texto = nomeDoArquivo;
    if (numeroDaLinha > 0) {
        texto += ", linha " + std::to_string(numeroDaLinha);
    }
    return texto + ": " + motivo;
}

void ListaDeProblemasDosDados::anotarErro(const std::string& nomeDoArquivo, int numeroDaLinha, const std::string& motivo)
{
    problemas.push_back({true, nomeDoArquivo, numeroDaLinha, motivo});
}

void ListaDeProblemasDosDados::anotarAviso(const std::string& nomeDoArquivo, int numeroDaLinha, const std::string& motivo)
{
    problemas.push_back({false, nomeDoArquivo, numeroDaLinha, motivo});
}

int ListaDeProblemasDosDados::quantidadeDeErros() const
{
    int quantidade = 0;
    for (const ProblemaDosDados& problema : problemas) {
        quantidade += problema.ehErro ? 1 : 0;
    }
    return quantidade;
}

int ListaDeProblemasDosDados::quantidadeDeAvisos() const
{
    return static_cast<int>(problemas.size()) - quantidadeDeErros();
}

bool ListaDeProblemasDosDados::algumErroContem(const std::string& trecho) const
{
    for (const ProblemaDosDados& problema : problemas) {
        if (problema.ehErro && problema.textoParaMostrar().find(trecho) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool ListaDeProblemasDosDados::algumAvisoContem(const std::string& trecho) const
{
    for (const ProblemaDosDados& problema : problemas) {
        if (!problema.ehErro && problema.textoParaMostrar().find(trecho) != std::string::npos) {
            return true;
        }
    }
    return false;
}
