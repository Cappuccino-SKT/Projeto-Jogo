## Sistema de Produção

**Descrição geral**

Baseado em 3 categorias — Produto Braçal, Produto Manufaturado e Produto Especializado correspondentes às habilidades dos NPCs. A ideia é que a produção ocorra em sequência: cada produto mais complexo exige produtos anteriores, formando um sistema de demanda.

**Produto Braçal** Toda produção feita diretamente a partir do mapa, gerando um recurso básico. Exemplos: Fazendeiro, Lenhador, Mineiro, Coletor, Rancheiro, Construtor. O que está disponível numa região é limitado pela "Lista de recursos" do Mapa Local — uma Mina só existe/produz onde há recurso mineral na região.

**Produção Manufaturada** Toda produção que envolve um produto básico para gerar um produto manufaturado. Exemplos: Ferreiro (nível básico e médio), Alfaiate (nível básico e médio), Cozinheiro, Oleiro, Pedreiro, Operário.

**Produção Especialista** Toda produção que resulta em um item de alto nível. Exemplos: Ferreiro (nível alto), Alfaiate (nível alto), Engenheiro, Joalheiro, Boticário, Produtor Mágico.

O sistema de produção será bastante completo, com exigências de produção para certos itens exigindo combinações de habilidades e de matérias-primas. Quanto maior o grau do item produzido, maior a exigência de habilidade e quantidade de material. Cada receita tem um **tempo de produção base** (ligado ao Sistema de Tempo), reduzido pela habilidade do NPC responsável (ex: Trabalho Especialista mais alto = produção mais rápida, Nivel do item mais alto também).

O sistema de produção será baseado em estruturas, desenvolvido junto com o Sistema de Construções: cada construção terá uma função de produção, acesso a um tipo de produção específico, uma **capacidade de armazenamento** (limite de estoque, para evitar acúmulo infinito) e uma quantidade de trabalhadores designados via Função do NPC. Exemplo: uma Mina produz diversos tipos de minério (conforme a região), que passam pelo Ferreiro (produzindo lingotes) e depois pelo Engenheiro (produzindo um item de alto grau) — seguindo uma cadeia de produção.

A produção terá um sistema logístico simplificado, detalhado no Sistema Econômico — a princípio, sem necessidade de "entregar" fisicamente a produção entre construções.

**Estações e clima.** A produção Braçal muda com a estação e o bioma (na tabela base, a Fazenda rende 150% no outono e 25% no inverno); a Manufaturada e a Especialista não mudam (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).

**Cadeia militar.** Ração Militar e materiais de manutenção de equipamento abastecem os exércitos (ver Sistema de Facção).

**Loja ilegal.** Qualquer construção de produção pode funcionar como loja ilegal disfarçada (ver Sistema de Crime e Justiça).

**Funções e construções.** A tabela mestre que liga cada Função à construção e ao que ela produz está em [[Sistema de Produção/Funções e Construções|Funções e Construções]]. Os salários, as vagas e os donos das construções estão no [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]].
