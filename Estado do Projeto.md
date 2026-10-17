> Situação do projeto em poucas linhas. O detalhe de cada sistema está no documento dele (índice em [[Descrição do Projeto]]); o jeito de trabalhar e as regras do código, na [[Retomada]]. Atualizado em 2026-10-17.

## Fase atual

**Começo do código: marco 0.** O design está completo e revisado, e a [[Ficha Logica/Fatia Vertical|Ficha Lógica: Fatia Vertical]] está pronta. O autor liberou o código na rodada FT2, marco a marco. As fichas do marco 1 ([[Ficha Logica/Tempo|Tempo]] e [[Ficha Logica/Mapa|Mapa]]) estão escritas, e a rodada FL1 aguarda respostas.

## O que está pronto

| Área | Situação |
|---|---|
| Design | Cerca de 45 documentos de sistema, todos escritos e revisados. Todas as propostas foram confirmadas pelo autor |
| Itens e produção | Cerca de 180 itens ([[Sistema de Itens/Descrição\|Itens]]); receitas de todos ([[Sistema de Produção/Receitas\|Receitas]]); [[Sistema de Produção/Funções e Construções\|Funções e Construções]] nível por nível |
| Economia | Moeda, preços pela receita, salários, consumo por classe, Mercado Local, imposto ([[Sistema Econômico/Descrição\|Econômico]]) |
| Conteúdo | Moldes prontos para copiar e modificar ([[Arquitetura/Moldes de Conteúdo\|Moldes de Conteúdo]]); as listas completas ficam para depois |
| Fatia vertical | Vila de 15 Civis; o jogador com 100 moedas e uma Carpintaria; 10 marcos (0 a 9); teste de 1 ano sem tela |

## Números-chave

| Tema | Valor |
|---|---|
| Tempo | 1 hora de jogo = 40 s reais a 1x; 1 dia = 16 min; estação = 16 dias; ano = 64 dias; 20 ticks por segundo; 1 tick = 4,5 s de jogo |
| Escala | 1 NPC real = 1 trabalhador e 1 consumidor; o Fator de Escala (100) só muda a população exibida |
| Cidades | NPCs reais por nível: 15, 40, 100 e 300 |
| Dinheiro | Inteiro em décimos de moeda; preço = (insumos + horas × salário) ÷ lote × 1,2 |
| Salário por hora | Braçal 0,75; Manufaturado 1,0; Especialista 1,5; Mago de Ofício 2,5 |
| Arte e mapa | Tile de 16 × 16 px; boneco de 16 × 24; mapa da fatia com 64 × 64 tiles; zoom de 2x, 3x e 4x |

## O que falta

- **Valores a testar:** quase todos os números, primeiro no teste sem tela da fatia vertical.
- **Rodadas de conteúdo, mais para frente:**
  - **C1:** tecnologias e marcos;
  - **C2:** feitiços e árvore arcana;
  - **C3:** contratos, missões, Ambições e eventos.
- **No início do código:** escolher os pacotes de arte provisórios e a ferramenta de atlas. O artista definitivo também segue sem escolha.
- **A história do mundo:** nomes, personagens e os nomes de fantasia dos itens.
- **Cerca de 80 marcações** [Em aberto] e [Refinar depois] nos documentos: detalhes e ideias futuras, sem pressa.
- **Ideias futuras:** vários núcleos (multithreading, ver [[Arquitetura/Descrição|Arquitetura]]), Títulos de Nobreza, outras raças, arte própria por cultura, arte extra das estações e os sistemas P2 (áudio, tutorial, configurações, localização, modding, conquistas).

## Próximos passos

1. **Marco 0:** o ambiente e um "olá, janela" com o SDL3. Em paralelo, as respostas da rodada FL1 (fichas do marco 1).
2. **Marcos 1 a 9** da fatia vertical, um por pull request.
3. **Depois da fatia:** ajustar os números pelo teste, as rodadas de conteúdo e o resto do jogo.

## Histórico

As revisões ficam na pasta `Documentos/Histórico/`, e cada rodada está no Git:

- [[Revisão do Projeto]] (2026-10-14);
- [[Revisão Completa]] (2026-10-17, rodadas V1 a V5 e IP1 a IP3).
