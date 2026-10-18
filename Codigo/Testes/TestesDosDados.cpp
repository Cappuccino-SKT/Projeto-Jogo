// Os testes dos dados: a parte 8 das fichas dos Itens, dos Recursos e das Receitas.
// Usam os dados de verdade da fatia (Jogo/data) e textos pequenos, montados aqui,
// com um defeito de propósito.

#include <cstdio>
#include <string>

#include "ConferenciaDosTestes.h"
#include "Dados/DadosDoJogo.h"
#include "Dados/TabelaCsv.h"

namespace {

const std::string pastaDosDadosDaFatia = std::string(PASTA_DOS_TESTES) + "/../../Jogo/data";

const char* const cabecalhoDosItens = "id;categoria;tipo;tier;rank_min;rank_max;peso;multiplicador_do_preco\n";
const char* const itensDaFatia =
    "item.grao;materia_prima;bruto_vegetal;0;;;1;100\n"
    "item.agua;materia_prima;bruto_mineral;0;;;1;100\n"
    "item.tora;materia_prima;bruto_vegetal;0;;;3;100\n"
    "item.pao;bem_de_consumo;alimento;1;;;1;100\n"
    "item.tabua;materia_prima;manufaturado;1;;;2;100\n"
    "item.moveis_madeira;bem_de_consumo;mobilia;2;;;8;100\n";

const char* const cabecalhoDasReceitas = "id;produto;lote;insumos;construcao;nivel_min;trabalho;hab_min;horas;era\n";
const char* const receitasDaFatia =
    "receita.grao;item.grao;2;;construcao.fazenda;1;bracal;10;1;1\n"
    "receita.agua;item.agua;2;;construcao.posto_de_coleta;1;bracal;10;1;1\n"
    "receita.tora;item.tora;2;;construcao.posto_de_coleta;1;bracal;10;1;1\n"
    "receita.pao;item.pao;4;item.grao:2|item.agua:1;construcao.cozinheiro;1;manufaturado;10;4;1\n"
    "receita.tabua;item.tabua;4;item.tora:2;construcao.carpinteiro;1;manufaturado;10;4;1\n"
    "receita.moveis_madeira;item.moveis_madeira;2;item.tabua:3;construcao.carpinteiro;2;manufaturado;25;4;1\n";

TabelaDeTextos textosDosItensDaFatia()
{
    TabelaDeTextos textos;
    for (const char* identificador : {"item.grao", "item.agua", "item.tora", "item.pao", "item.tabua", "item.moveis_madeira"}) {
        textos.guardarTexto(identificador, identificador);
    }
    return textos;
}

CatalogoDeConstrucoes construcoesDaFatia()
{
    CatalogoDeConstrucoes construcoes;
    for (const char* identificador : {"construcao.fazenda", "construcao.posto_de_coleta", "construcao.cozinheiro", "construcao.carpinteiro"}) {
        construcoes.acrescentarConstrucao(identificador);
    }
    return construcoes;
}

// Lê só o itens.csv e devolve os problemas.
ListaDeProblemasDosDados problemasDosItens(const std::string& textoDoArquivo)
{
    ListaDeProblemasDosDados problemas;
    lerArquivoDeItens(textoDoArquivo, textosDosItensDaFatia(), problemas);
    return problemas;
}

// Lê os itens da fatia e as receitas dadas, calcula os preços e devolve tudo.
struct ResultadoDasReceitas {
    CatalogoDeItens itens;
    ListaDeProblemasDosDados problemas;
};

ResultadoDasReceitas lerReceitasECalcularPrecos(const std::string& textoDasReceitas)
{
    ResultadoDasReceitas resultado;
    resultado.itens = lerArquivoDeItens(std::string(cabecalhoDosItens) + itensDaFatia, textosDosItensDaFatia(), resultado.problemas);
    const CatalogoDeReceitas receitas = lerArquivoDeReceitas(textoDasReceitas, resultado.itens, construcoesDaFatia(), resultado.problemas);
    calcularPrecosBaseDosItens(resultado.itens, receitas, RegrasDoPreco{}, resultado.problemas);
    conferirEscadaDeNiveis(resultado.itens, receitas, resultado.problemas);
    return resultado;
}

int precoDoItem(const CatalogoDeItens& itens, const char* identificador)
{
    const auto numero = itens.procurarNumeroDoItem(identificador);
    return numero.has_value() ? itens.itemPeloNumero(*numero).precoBaseEmDecimos : -1;
}

void testarLeitorDeCsv()
{
    const TabelaCsv tabela = lerTabelaCsv("\xEF\xBB\xBF" "a;b;c\r\n# comentário\r\n\r\n1;;3\r\n4\r\n");
    conferir(tabela.nomesDasColunas.size() == 3 && tabela.nomesDasColunas[0] == "a", "o CSV ignora o BOM do Excel e lê o cabeçalho");
    conferir(tabela.linhas.size() == 2, "o CSV ignora comentários e linhas vazias");
    conferir(tabela.linhas[0].numeroDaLinhaNoArquivo == 4, "o CSV guarda o número da linha como a planilha mostra");
    conferir(tabela.valorNaLinha(tabela.linhas[0], "b").empty() && tabela.valorNaLinha(tabela.linhas[0], "c") == "3",
             "o CSV lê colunas vazias no meio");
    conferir(tabela.valorNaLinha(tabela.linhas[1], "c").empty(), "uma linha mais curta tem as colunas do fim vazias");

    int numero = 0;
    conferir(lerNumeroInteiroDoTexto("12", numero) && numero == 12, "\"12\" é o número 12");
    conferir(!lerNumeroInteiroDoTexto("12 ", numero) && !lerNumeroInteiroDoTexto("1,5", numero) && !lerNumeroInteiroDoTexto("", numero),
             "\"12 \", \"1,5\" e \"\" não são inteiros");
    conferir(identificadorEhValido("item.pao", "item.") && !identificadorEhValido("Item.Pão", "item.") && !identificadorEhValido("item.", "item."),
             "o ID item.pao vale; Item.Pão e item. não valem");
}

void testarItens()
{
    {
        ListaDeProblemasDosDados problemas;
        const CatalogoDeItens itens = lerArquivoDeItens(std::string(cabecalhoDosItens) + itensDaFatia, textosDosItensDaFatia(), problemas);
        conferir(!problemas.temErro() && itens.quantidadeDeItens() == 6, "o itens.csv da fatia tem 6 itens, sem erro");
        conferir(itens.procurarNumeroDoItem("item.pao") == 3, "item.pao é o número 3");
        conferir(itens.procurarNumeroDoItem("item.grao") == 0 && !itens.procurarNumeroDoItem("item.farinha").has_value(),
                 "item.grao é o 0; item.farinha não existe");
    }
    {
        ListaDeProblemasDosDados problemas;
        const std::string colunasTrocadas = "peso;id;tipo;categoria;tier\n1;item.grao;bruto_vegetal;materia_prima;0\n";
        TabelaDeTextos textos;
        textos.guardarTexto("item.grao", "Grão");
        const CatalogoDeItens itens = lerArquivoDeItens(colunasTrocadas, textos, problemas);
        conferir(!problemas.temErro() && itens.quantidadeDeItens() == 1, "as colunas em outra ordem são lidas igual");
    }
    conferir(problemasDosItens(std::string(cabecalhoDosItens) + itensDaFatia + "item.pao;bem_de_consumo;alimento;1;;;1;100\n")
                 .algumErroContem("item.pao aparece duas vezes (linhas 5 e 8)"),
             "item.pao duas vezes dá erro com as duas linhas");
    conferir(problemasDosItens(std::string(cabecalhoDosItens) + "Item.Pão;bem_de_consumo;alimento;1;;;1;100\n").algumErroContem("precisa começar"),
             "o ID Item.Pão dá erro");
    conferir(problemasDosItens(std::string(cabecalhoDosItens) + "item.grao;materia_prima;alimento;0;;;1;100\n").algumErroContem("não existe na categoria"),
             "o tipo alimento na matéria-prima dá erro");
    conferir(problemasDosItens(std::string(cabecalhoDosItens) + "item.pao;bem_de_consumo;alimento;;;;1;100\n").algumErroContem("falta o Tier"),
             "o Tier vazio num bem de consumo dá erro");
    conferir(problemasDosItens(std::string(cabecalhoDosItens) + "item.pao;bem_de_consumo;alimento;5;;;1;100\n").algumErroContem("o Tier vai de 0 a 4"),
             "o Tier 5 dá erro");
    conferir(problemasDosItens(std::string(cabecalhoDosItens) + "item.pao;bem_de_consumo;alimento;1;;;0;100\n").algumErroContem("peso"),
             "o peso 0 dá erro");
    conferir(problemasDosItens("id;categoria;tipo;tier\nitem.pao;bem_de_consumo;alimento;1\n").algumErroContem("falta a coluna \"peso\""),
             "sem a coluna peso, dá erro de cabeçalho");
    {
        ListaDeProblemasDosDados problemas;
        TabelaDeTextos textos = textosDosItensDaFatia();
        TabelaDeTextos textosSemTabua;
        for (const auto& [identificador, texto] : textos.todosOsTextos()) {
            if (identificador != "item.tabua") {
                textosSemTabua.guardarTexto(identificador, texto);
            }
        }
        textosSemTabua.guardarTexto("item.velho", "Item Velho");
        lerArquivoDeItens(std::string(cabecalhoDosItens) + itensDaFatia, textosSemTabua, problemas);
        conferir(problemas.algumErroContem("item.tabua não tem nome"), "item.tabua sem nome dá erro");
        conferir(problemas.algumAvisoContem("item.velho") && !problemas.algumErroContem("item.velho"), "um texto item.velho sem item é aviso, não erro");
    }
}

void testarReceitasEPrecos()
{
    {
        const ResultadoDasReceitas resultado = lerReceitasECalcularPrecos(std::string(cabecalhoDasReceitas) + receitasDaFatia);
        conferir(resultado.problemas.todosOsProblemas().empty(), "as 6 receitas da fatia não dão erro nem aviso");
        conferir(precoDoItem(resultado.itens, "item.grao") == 5 && precoDoItem(resultado.itens, "item.agua") == 5
                     && precoDoItem(resultado.itens, "item.tora") == 5,
                 "Grão, Água e Tora custam 0,5");
        conferir(precoDoItem(resultado.itens, "item.pao") == 17, "o Pão custa 1,7");
        conferir(precoDoItem(resultado.itens, "item.tabua") == 15, "a Tábua custa 1,5");
        conferir(precoDoItem(resultado.itens, "item.moveis_madeira") == 51, "os Móveis de Madeira custam 5,1");
    }
    {
        // As mesmas receitas, de baixo para cima.
        const std::string receitas = std::string(receitasDaFatia);
        std::string invertidas;
        std::size_t fim = receitas.size();
        while (fim > 0) {
            const std::size_t inicio = receitas.rfind('\n', fim - 2);
            const std::size_t comeco = inicio == std::string::npos ? 0 : inicio + 1;
            invertidas += receitas.substr(comeco, fim - comeco);
            fim = comeco;
        }
        const ResultadoDasReceitas resultado = lerReceitasECalcularPrecos(std::string(cabecalhoDasReceitas) + invertidas);
        conferir(precoDoItem(resultado.itens, "item.moveis_madeira") == 51 && precoDoItem(resultado.itens, "item.pao") == 17,
                 "a ordem das linhas não muda os preços");
    }
    auto errosDeUmaReceitaDoPao = [](const std::string& insumos, const std::string& trabalho = "manufaturado", const std::string& lote = "4") {
        const std::string linhaDoPao = "receita.pao;item.pao;" + lote + ";" + insumos + ";construcao.cozinheiro;1;" + trabalho + ";10;4;1\n";
        return lerReceitasECalcularPrecos(std::string(cabecalhoDasReceitas) + "receita.grao;item.grao;2;;construcao.fazenda;1;bracal;10;1;1\n"
                                          + "receita.agua;item.agua;2;;construcao.posto_de_coleta;1;bracal;10;1;1\n"
                                          + "receita.tora;item.tora;2;;construcao.posto_de_coleta;1;bracal;10;1;1\n" + linhaDoPao)
            .problemas;
    };
    conferir(errosDeUmaReceitaDoPao("item.farinha:2").algumErroContem("o insumo item.farinha não existe"), "um insumo que não existe dá erro");
    conferir(errosDeUmaReceitaDoPao("item.grao:1|item.agua:1|item.tora:1|item.tabua:1").algumErroContem("o máximo é 3"),
             "4 tipos de insumo dão erro");
    conferir(errosDeUmaReceitaDoPao("item.grao:1|item.grao:1").algumErroContem("aparece duas vezes"), "o mesmo insumo duas vezes dá erro");
    conferir(errosDeUmaReceitaDoPao("item.pao:1").algumErroContem("insumo dele mesmo"), "o Pão como insumo do Pão dá erro");
    conferir(errosDeUmaReceitaDoPao("item.grao:2", "bracal ").algumErroContem("o trabalho \"bracal \" não existe"), "trabalho com espaço dá erro");
    conferir(errosDeUmaReceitaDoPao("item.grao:2", "Braçal").algumErroContem("não existe"), "trabalho com acento dá erro");
    conferir(errosDeUmaReceitaDoPao("item.grao:2", "manufaturado", "0").algumErroContem("o lote"), "lote 0 dá erro");
    {
        const ResultadoDasReceitas resultado = lerReceitasECalcularPrecos(std::string(cabecalhoDasReceitas) + receitasDaFatia
                                                                          + "receita.pao_2;item.pao;4;item.grao:2;construcao.cozinheiro;1;manufaturado;10;4;1\n");
        conferir(resultado.problemas.algumErroContem("item.pao já tem a receita receita.pao (linha 5)"), "duas receitas para o Pão dão erro");
    }
    {
        // Laço: a Tábua precisa dos Móveis, e os Móveis precisam da Tábua.
        const std::string receitasComLaco = std::string(cabecalhoDasReceitas)
            + "receita.grao;item.grao;2;;construcao.fazenda;1;bracal;10;1;1\n"
            + "receita.agua;item.agua;2;;construcao.posto_de_coleta;1;bracal;10;1;1\n"
            + "receita.tora;item.tora;2;;construcao.posto_de_coleta;1;bracal;10;1;1\n"
            + "receita.pao;item.pao;4;item.grao:2|item.agua:1;construcao.cozinheiro;1;manufaturado;10;4;1\n"
            + "receita.tabua;item.tabua;4;item.moveis_madeira:1;construcao.carpinteiro;1;manufaturado;10;4;1\n"
            + "receita.moveis_madeira;item.moveis_madeira;2;item.tabua:3;construcao.carpinteiro;2;manufaturado;25;4;1\n";
        const ResultadoDasReceitas resultado = lerReceitasECalcularPrecos(receitasComLaco);
        conferir(resultado.problemas.algumErroContem("laço") && resultado.problemas.algumErroContem("item.tabua")
                     && resultado.problemas.algumErroContem("item.moveis_madeira"),
                 "o laço Tábua <-> Móveis dá erro citando os dois");
        conferir(precoDoItem(resultado.itens, "item.pao") == 17, "com o laço, os outros itens ainda ganham preço");
    }
    {
        const std::string semMoveis = std::string(cabecalhoDasReceitas) + "receita.grao;item.grao;2;;construcao.fazenda;1;bracal;10;1;1\n";
        conferir(lerReceitasECalcularPrecos(semMoveis).problemas.algumErroContem("item.moveis_madeira não tem receita"), "um item sem receita dá erro");
    }
    {
        const std::string moveisNoNivel1 = std::string(cabecalhoDasReceitas) + receitasDaFatia;
        std::string texto = moveisNoNivel1;
        const std::string trechoDoNivel2 = "carpinteiro;2;manufaturado;25";
        texto.replace(texto.find(trechoDoNivel2), trechoDoNivel2.size(), "carpinteiro;1;manufaturado;10");
        const ResultadoDasReceitas resultado = lerReceitasECalcularPrecos(texto);
        conferir(!resultado.problemas.temErro() && resultado.problemas.algumAvisoContem("o Tier 2 pede nível 2"),
                 "os Móveis no nível 1 dão aviso da escada, não erro");
    }
    conferir(arredondarMetadeParaCima(33, 2) == 17 && arredondarMetadeParaCima(9, 2) == 5 && arredondarMetadeParaCima(31, 2) == 16,
             "a metade sobe: 16,5 vira 17; 4,5 vira 5; 15,5 vira 16");
    {
        ResultadoDasReceitas resultado;
        std::string itensComCavalo = std::string(cabecalhoDosItens) + itensDaFatia;
        const std::string trechoDoMultiplicador = "mobilia;2;;;8;100";
        itensComCavalo.replace(itensComCavalo.find(trechoDoMultiplicador), trechoDoMultiplicador.size(), "mobilia;2;;;8;300");
        resultado.itens = lerArquivoDeItens(itensComCavalo, textosDosItensDaFatia(), resultado.problemas);
        const CatalogoDeReceitas receitas = lerArquivoDeReceitas(std::string(cabecalhoDasReceitas) + receitasDaFatia, resultado.itens,
                                                                 construcoesDaFatia(), resultado.problemas);
        calcularPrecosBaseDosItens(resultado.itens, receitas, RegrasDoPreco{}, resultado.problemas);
        conferir(precoDoItem(resultado.itens, "item.moveis_madeira") == 153, "o multiplicador 300 triplica o preço (51 vira 153)");
    }
}

void testarRecursos()
{
    ListaDeProblemasDosDados problemas;
    TabelaDeTextos textos = textosDosItensDaFatia();
    const CatalogoDeItens itens = lerArquivoDeItens(std::string(cabecalhoDosItens) + itensDaFatia, textos, problemas);
    RegrasDoJogo regras;
    regras.producaoPorEstacaoEmPorcento["fazenda"] = {75, 100, 150, 25};
    regras.producaoPorEstacaoEmPorcento["agua"] = {100, 100, 100, 100};
    regras.producaoPorEstacaoEmPorcento["madeira"] = {100, 100, 100, 75};
    const std::string cabecalho = "id;item_gerado;construcao;area_no_tiled;grupo_da_estacao\n";
    const std::string recursosDaFatia = "recurso.grao;item.grao;construcao.fazenda;campo_de_grao;fazenda\n"
                                        "recurso.agua;item.agua;construcao.posto_de_coleta;poco;agua\n"
                                        "recurso.madeira;item.tora;construcao.posto_de_coleta;bosque;madeira\n";
    {
        ListaDeProblemasDosDados problemasDosRecursos;
        const CatalogoDeRecursos recursos = lerArquivoDeRecursos(cabecalho + recursosDaFatia, itens, construcoesDaFatia(), regras, problemasDosRecursos);
        conferir(!problemasDosRecursos.temErro() && recursos.quantidadeDeRecursos() == 3, "o recursos.csv da fatia tem 3 recursos, sem erro");
        conferir(recursos.procurarRecursoPelaArea("poco") == 1, "a área poco é o recurso Água");

        MapaLocal mapa;
        AreaDeRecurso areaDoPoco;
        areaDoPoco.tipoDeRecurso = "poco";
        AreaDeRecurso areaComErro;
        areaComErro.tipoDeRecurso = "pocoo";
        AreaDeRecurso areaDeDestaque;
        areaDeDestaque.tipoDeRecurso = "campo_de_grao";
        areaDeDestaque.textoDaProdutividade = "destaque";
        AreaDeRecurso areaComProdutividadeErrada;
        areaComProdutividadeErrada.tipoDeRecurso = "bosque";
        areaComProdutividadeErrada.textoDaProdutividade = "muita";
        mapa.areasDeRecurso = {areaDoPoco, areaComErro, areaDeDestaque, areaComProdutividadeErrada};
        ListaDeProblemasDosDados problemasDoMapa;
        ligarAreasDoMapaAosRecursos(mapa, "mapa_de_teste", recursos, regras, problemasDoMapa);
        conferir(mapa.areasDeRecurso[0].numeroDoRecurso == 1 && mapa.areasDeRecurso[0].produtividadeEmPorcento == 100,
                 "a área poco liga à Água, com 100%");
        conferir(problemasDoMapa.algumErroContem("\"pocoo\", que nenhum recurso usa"), "a área pocoo dá erro");
        conferir(mapa.areasDeRecurso[2].produtividadeEmPorcento == 150, "uma área de destaque rende 150%");
        conferir(problemasDoMapa.algumErroContem("\"muita\""), "a produtividade muita dá erro");
    }
    auto errosDosRecursos = [&](const std::string& linha) {
        ListaDeProblemasDosDados problemasDosRecursos;
        lerArquivoDeRecursos(cabecalho + recursosDaFatia + linha, itens, construcoesDaFatia(), regras, problemasDosRecursos);
        return problemasDosRecursos;
    };
    conferir(errosDosRecursos("recurso.carvao;item.carvao;construcao.fazenda;mina;fazenda\n").algumErroContem("recursos.csv, linha 5: o item item.carvao não existe"),
             "um item gerado que não existe dá erro com o arquivo e a linha");
    conferir(errosDosRecursos("recurso.pedra;item.tora;construcao.mina;pedreira;madeira\n").algumErroContem("a construção construcao.mina não existe"),
             "uma construção que não existe dá erro");
    conferir(errosDosRecursos("recurso.agua2;item.agua;construcao.posto_de_coleta;poco;agua\n").algumErroContem("já é de outro recurso"),
             "duas vezes a área poco dá erro");
    conferir(errosDosRecursos("recurso.lama;item.agua;construcao.posto_de_coleta;lamacal;pantano\n").algumErroContem("\"pantano\" não existe"),
             "o grupo pantano dá erro");

    const std::array<int, 4> producaoDaFazenda = {75, 100, 150, 25};
    conferir(calcularFatorDeProducaoEmPorcento(100, producaoDaFazenda, Estacao::Outono) == 150.0, "o Grão normal no outono rende 150%");
    conferir(calcularFatorDeProducaoEmPorcento(150, producaoDaFazenda, Estacao::Inverno) == 37.5, "o Grão de destaque no inverno rende 37,5%");
    conferir(calcularFatorDeProducaoEmPorcento(100, {100, 100, 100, 100}, Estacao::Verao) == 100.0, "a Água rende 100% no verão");
}

void testarDadosDaFatia()
{
    const DadosDoJogo dados = carregarDadosDoJogo(pastaDosDadosDaFatia);
    for (const ProblemaDosDados& problema : dados.listaDeProblemas.todosOsProblemas()) {
        std::printf("    %s: %s\n", problema.ehErro ? "erro" : "aviso", problema.textoParaMostrar().c_str());
    }
    conferir(dados.dadosEstaoProntos() && dados.listaDeProblemas.quantidadeDeAvisos() == 0, "os dados da fatia (Jogo/data) não têm erro nem aviso");
    conferir(dados.catalogoDeItens.quantidadeDeItens() == 6 && dados.catalogoDeReceitas.quantidadeDeReceitas() == 6
                 && dados.catalogoDeRecursos.quantidadeDeRecursos() == 3 && dados.catalogoDeConstrucoes.quantidadeDeConstrucoes() == 7,
             "a fatia tem 6 itens, 6 receitas, 3 recursos e 7 construções");
    conferir(precoDoItem(dados.catalogoDeItens, "item.pao") == 17, "nos dados da fatia, o Pão custa 1,7");
    bool todasAsAreasLigadas = dados.mapaDaVila.areasDeRecurso.size() == 3;
    for (const AreaDeRecurso& area : dados.mapaDaVila.areasDeRecurso) {
        todasAsAreasLigadas = todasAsAreasLigadas && area.numeroDoRecurso >= 0 && area.produtividadeEmPorcento == 100;
    }
    conferir(todasAsAreasLigadas, "as 3 áreas do mapa da vila estão ligadas aos recursos, com 100%");
    conferir(montarResumoDosDados(dados).find("0 erros, 0 avisos") != std::string::npos, "o resumo diz 0 erros e 0 avisos");
    conferir(escreverDinheiroEmMoedas(17) == "1,7" && escreverDinheiroEmMoedas(5) == "0,5" && escreverDinheiroEmMoedas(153) == "15,3",
             "o dinheiro em décimos aparece como 1,7, 0,5 e 15,3");

    const DadosDoJogo dadosDeUmaPastaErrada = carregarDadosDoJogo(pastaDosDadosDaFatia + "/nao_existe");
    conferir(dadosDeUmaPastaErrada.listaDeProblemas.algumErroContem("itens.csv: não foi possível abrir"), "sem os arquivos, dá erro sem travar");
}

}  // namespace

void testarDadosDoJogo()
{
    std::printf("Dados do jogo\n");
    testarLeitorDeCsv();
    testarItens();
    testarReceitasEPrecos();
    testarRecursos();
    testarDadosDaFatia();
}
