> **Histórico.** Este documento é registro do passado; não precisa ser lido para retomar o trabalho. A situação atual está no [[Estado do Projeto]].
>
> Segunda revisão geral do projeto, feita em 2026-10-17, quando todos os sistemas P0 e P1 ficaram escritos. Pedido do autor (pergunta D2 da rodada P1-6): revisar o que já existe, achar o que está abstrato, levantar todas as pendências e conferir todas as propostas, para seguir com elas ou mudar. A primeira revisão (2026-10-14) está na [[Revisão do Projeto]], que fica como histórico. O que esta revisão encontrou vira as **rodadas V1 a V5** (seção 7), respondidas no `Respostas.txt` como as outras. **Concluída em 2026-10-17**, com a V5; o passo seguinte é a [[Ficha Técnica/Fatia Vertical|Ficha Técnica: Fatia Vertical]].

## 1. Resumo

| Item | Quantidade |
|---|---|
| Documentos de sistema | 43 (mais o índice, o Glossário e os documentos de estado) |
| Documentos no formato padrão | 30 (22 com todas as seções: legenda, parte técnica, ligações e perguntas; 8 sem a parte técnica) |
| Documentos no formato antigo, de antes do padrão | 13 |
| Marcações **[Proposta]** (sugestões ainda não confirmadas) | cerca de 128 |
| Marcações **[Em aberto]** e **[Refinar depois]** no texto | cerca de 44 |
| Itens nas listas "Perguntas em aberto" | cerca de 119 (cerca de 30 são só "valores a testar") |
| Inconsistências encontradas nesta revisão | 22 (seção 3) |
| Partes abstratas que travam a ficha técnica | 12 (seção 4) |

**Conclusão em uma frase:** as **regras** dos sistemas estão quase todas escritas, mas a **base numérica da economia** (preços, receitas, consumo, salários e custos) não existe em nenhum documento, e a ficha técnica da fatia vertical não pode começar sem ela. Os 13 documentos antigos também precisam de uma faxina de formato.

## 2. Estado de cada documento

**Maturidade:** **Completo** = regras, números iniciais e parte técnica. **Parcial** = regras escritas, faltam números ou parte técnica. **Abstrato** = só a ideia ou uma lista de atributos, sem regras nem números.

| Documento | Formato | Maturidade | Observação |
|---|---|---|---|
| [[Sistema de Mapa/Descrição\|Mapa]] | Antigo | Parcial | Bem descrito, mas sem parte técnica nem perguntas; texto do Mapa de Guerra desatualizado (I-8) |
| [[Sistema de Cidade/Descrição\|Cidade]] | Antigo | Abstrato | Só a ideia dos níveis e a lista de atributos; as regras estão espalhadas em População, Felicidade e Guerra |
| [[Sistema de NPC/Descrição\|NPC]] | Antigo | Parcial | Lista de atributos completa; faltam Fome e Felicidade individual dos Controlados (I-14) |
| [[Sistema de IA/Descrição\|IA]] | Antigo | Abstrato | Rotina sem horas por classe; fórmula de dano antiga (I-1); Utilidade e Regras Fixas sem detalhe (A-8) |
| [[Sistema de Inventário/Descrição\|Inventário]] | Antigo | Abstrato | 12 linhas; contradiz o inventário único da cidade (I-7) |
| [[Sistema Econômico/Descrição\|Econômico]] | Antigo | Abstrato | Três mercados e ciclo do dinheiro, **sem nenhum número** (A-1) |
| [[Sistema de Itens/Descrição\|Itens]] | Antigo | Parcial | Lista grande, sem preço nem peso; itens sem produtor e nomes misturados (I-9, I-10) |
| [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]] | Padrão | Completo | Novo (P1-6) |
| [[Sistema de Produção/Descrição\|Produção]] | Antigo | Abstrato | Sem receitas (insumos, quantidades, tempo) (A-2) |
| [[Sistema de Produção/Funções e Construções\|Funções e Construções]] | Padrão | Parcial | Erros do canvas (I-2, I-3); Engenheiro e Operário sem construção |
| [[Sistema de Recursos/Descrição\|Recursos]] e [[Sistema de Recursos/Recursos Existentes\|Recursos Existentes]] | Antigo | Abstrato | 6 linhas e uma lista em maiúsculas; sem produtividade base nem ligação aos itens (A-5) |
| [[Sistema de Construção/Descrição\|Construção]] | Antigo | Parcial | Atributos e regras gerais; **sem custos, tempos, manutenção nem níveis** das construções (A-4) |
| [[Sistema de Classe Social/Descrição\|Classe Social]] | Antigo | Parcial | Sem quantidades de consumo (A-3); nome "Nobre" × "Nobreza" (I-12) |
| [[Sistema de Tempo/Descrição\|Tempo]] | Antigo | Parcial | Frequência do tick por LOD "a definir" (A-9) |
| [[Sistema de Cultura/Descrição\|Cultura]] | Padrão (sem parte técnica) | Parcial | Traços ainda como proposta; falta a tabela de leis iniciais de cada cultura (A-10) |
| [[Sistema de Facção/Descrição\|Facção]] | Padrão (sem parte técnica) | Parcial | 19 propostas, a maior quantidade; texto do Inventário da Caravana desatualizado (I-6) |
| [[Sistema de Crime e Justiça/Descrição\|Crime e Justiça]] | Padrão (sem parte técnica) | Parcial | Fórmula da Criminalidade só nos testes; valores de multa e pena ausentes |
| [[Sistema de Cura/Descrição\|Cura]] | Padrão (sem parte técnica) | Parcial | Curto; taxa de regeneração sem número |
| [[Sistema de Combate/Descrição\|Combate]] | Padrão (sem parte técnica) | Completo | A seção "Desempenho" faz o papel da parte técnica |
| [[Sistema de Início de Jogo/Descrição\|Início de Jogo]] | Padrão (sem parte técnica) | Parcial | Bônus dos estilos só como exemplo (A-11) |
| [[Sistema de Comércio/Descrição\|Comércio]] | Padrão | Parcial | Pergunta 4 (fluxo em LOD 2) ficou obsoleta com a Viagem (I-15) |
| [[Sistema de Magia/Descrição\|Magia]] | Padrão | Parcial | Lista de feitiços e árvore arcana não escritas (A-12) |
| [[Sistema de Tecnologia/Descrição\|Tecnologia]] | Padrão | Parcial | Regras completas; **a lista das 40 a 60 tecnologias não existe** (A-12) |
| [[Sistema de Eventos/Descrição\|Eventos]] | Padrão | Completo | A lista inicial de 10 a 15 eventos ainda não foi escrita (A-12) |
| [[Objetivo e Progressão/Descrição\|Objetivo]] | Padrão | Parcial | Condições das Ambições e Renome por título a definir |
| [[Arquitetura/Descrição\|Arquitetura]] | Padrão (sem parte técnica) | Parcial | Decisões técnicas como proposta; PC alvo em aberto |
| Felicidade, População, Trabalho, Interface, Mundo, Pipeline, Viagem, Reputação, Companheiros, Guerra, Batalha, Diplomacia, Religião, Clima, Notícias, Escravidão | Padrão | Completo | Escritos nas rodadas P0 e P1; só valores a testar e detalhes |

## 3. Inconsistências encontradas

Contradições entre documentos, ou textos que ficaram para trás depois de uma decisão. A maioria tem uma correção óbvia; as que pedem escolha estão na rodada **V1**.

| Código | Onde | O problema | Correção sugerida |
|---|---|---|---|
| I-1 | IA × Combate | A IA ainda diz "dano = ataque − Defesa"; o Combate decidiu "dano da arma × (habilidade ÷ 50) − Defesa" | A IA aponta para a fórmula do Combate |
| I-2 | Funções e Construções × Itens | Armeiro nível 4 faz "Rifle de Aço", que não existe (a lista tem Carabina de Aço e Rifle só de Aço Mágico); Carabina de Aço Mágico e Besta de Repetição não têm produtor | V1-2 |
| I-3 | Funções e Construções | Ferreiro repete "Machado de Ferro" e não faz Espada (já em aberto) | V1-2 |
| I-4 | Itens × Pasto, Viagem, Batalha | Galinha, Porco, Ovelha, Vaca e **Cavalo** são produtos do Pasto e recursos, mas **não são itens**; a Viagem (montaria) e a Cavalaria (kit) usam Cavalo | V1-3 |
| I-5 | Recursos × Mundo e Pipeline | O Peixe foi descartado, mas o vilarejo (6,6) é "porto de pesca" e o Tiled tem área de "pesca" | V1-4 |
| I-6 | Facção × Arquitetura | A Caravana "herda do Inventário abstrato"; a Arquitetura trocou herança por composição | Corrigir o texto |
| I-7 | Inventário × Econômico × Produção | O Inventário diz que a cidade tem "N inventários de construção"; o Econômico diz que há **um único inventário da cidade**; a Produção dá capacidade de armazenamento a cada construção | V1-5 |
| I-8 | Mapa × Batalha | O Mapa diz que a batalha acaba quando a "moral chega a 0"; a Batalha decidiu "todos fugindo ou destruídos" | Corrigir o texto |
| I-9 | Itens | Nomes misturados: "Musket" e "Musquete"; "Machado Curta", "Perneiras Pesado", "Calça rasgadas"; "Materias de Argila" × "Bens de Argila"; "Osso" × "Ossos"; "Carne" × "Pedaço de Carne"; "Ovo" × "Saco de Ovos"; o Costureiro faz "Roupa de Couro", mas a lista tem peças com nome | V1-6 |
| I-10 | Itens × Funções e Construções | Itens sem produtor: Torta de Fruta, Legumes Cozidos, Carne Envelhecida, **Doce de Baunilha** (baunilha não é recurso), Anel de Cobre e de Ouro, Coroa, chapéus, camisas e calças com nome, arcos e bestas de Aço, Pedra de Mana, Joia Mágica | **Resolvida na IP2** (lista final); os produtores e insumos saem na IP3 |
| I-11 | Itens × Mundo (tom) | "Capuz Ninja", "Terno", "Camiseta Moderna", "Calça social" e "Cajado Moderno" soam modernos num mundo sombrio de chegada da pólvora | **Resolvida na IP2** (regra dos nomes e conjuntos de roupa) |
| I-12 | Classe Social | Usa "Nobre" na lista de consumo e "Nobreza" no resto | Padronizar "Nobreza" |
| I-13 | Classe Social × Itens | Bens Exclusivos incluem o Licor, que na lista é "Alimento de Luxo"; o mapeamento Luxo × Exclusivo por Tier já estava em aberto | **Resolvida na IP2-11** (consumo pelo Tier) |
| I-14 | NPC × IA × Felicidade | A IA usa **Fome** dos Controlados e a Felicidade tem um valor individual, mas os dois não estão na lista de atributos do NPC | V1-9 |
| I-15 | Comércio × Viagem | A pergunta "fluxo de caravanas em LOD 2" ficou obsoleta: a Viagem decidiu caravanas sempre reais. A pergunta "guardas reais ou abstratos" também foi respondida (viram NPCs só em LOD 0 e 1) | Fechar as duas perguntas |
| I-16 | Religião × Trabalho | O Templo tem 1, 2, 5, 10 e 15 vagas por nível; a tabela geral de vagas é 2, 3, 5, 7 e 10 | V1-10 |
| I-17 | Facção | Duas propostas já foram feitas e continuam marcadas: "documentar o Fator de Escala no Mapa" e "migrar os atributos do Esquadrão do NPC" | Limpar as marcações |
| I-18 | Itens × Combate | A regra da armadura mista está escrita duas vezes, como proposta, nos dois documentos | Confirmar uma vez (V4) e deixar só no Combate |
| I-19 | Escravidão × Combate | A rebelião de escravos usava Esquadrões no combate local | **Já corrigido nesta revisão:** combate local até 40; acima disso, Mapa de Guerra com 1 Esquadrão por NPC escravo |
| I-20 | Cultura × Crime | O pensamento extremo da Autoritária diz "pedir mudança de lei é crime", mas o crime não está na lista | Acrescentar o crime "Sedição" (gravidade 3) só nas facções com essa lei |
| I-21 | Itens × Funções e Construções | A lista de itens diz que as peças Leves e Pesadas são de metal e a Média é de couro (Costureiro); o canvas põe a "Armadura Leve" no Costureiro nível 3 e só a Pesada no Ferreiro | **Resolvida na IP1-2:** Leves e Pesadas no Ferreiro, Média no Costureiro ; a IP2-7 trocou: Leve de couro no Costureiro, Média de malha no Ferreiro |
| I-22 | Funções e Construções × Construção | A Metalurgia nível 5 faz Barra de Aço Mágico, e a Forja Arcana também faz Aço Mágico (com Mago de Ofício) | **Resolvida na IP1-2:** só a Forja Arcana faz Aço Mágico |

## 4. Partes abstratas

O que existe só como ideia, sem regras ou números suficientes para virar código. As que **travam a ficha técnica da fatia vertical** vêm primeiro.

| Código | O que falta | Onde | Trava a fatia vertical? | Rodada |
|---|---|---|---|---|
| A-1 | **Moeda e preços base**: quanto vale cada item (por Tier e Rank) | Econômico, Itens | **Sim** | **Resolvida na V2** |
| A-2 | **Receitas**: insumos, quantidades e tempo de cada produto (a fatia vertical precisa ao menos de Grão → Pão) | Produção | **Sim** | **Resolvida na IP3** ([[Sistema de Produção/Receitas\|Receitas]]) |
| A-3 | **Consumo por classe**: quanto cada NPC real consome por dia (Comida, Materiais, Luxo...) | Classe Social | **Sim** | **Resolvida na V2** |
| A-4 | **Construções**: custo, tempo de obra, manutenção por estação e níveis | Construção | **Sim** (Fazenda, Cozinheiro, Mercado, Casa) | **Resolvida na V2** |
| A-5 | **Recursos**: produtividade base por recurso e qual item cada um gera | Recursos | **Sim** (Grão, Água) | **Resolvida na V2** |
| A-6 | **Salários**: tabela por Função e nível | Trabalho | **Sim** | **Resolvida na V2** |
| A-7 | **Rotina**: horas de dormir, trabalhar e Período Livre por classe | IA | **Sim** | **Resolvida na V2** |
| A-8 | **IA de Utilidade**: quais ações o NPC escolhe e como os pesos se combinam | IA | Sim, mas simples na fatia vertical | **Resolvida na V2** |
| A-9 | **Tick**: quantas vezes por segundo a simulação roda em cada LOD | Tempo, Arquitetura | Sim (valor de partida) | **Resolvida na V2** |
| A-10 | **Leis iniciais** de cada facção, numa tabela única (hoje espalhadas) | Cultura, Facção | Não | **Resolvida na V3-1** |
| A-11 | **Bônus dos estilos de início** e o custo do personalizado | Início de Jogo | Não | **Resolvida na V3-2** |
| A-12 | **Listas de conteúdo**: 40 a 60 tecnologias, feitiços e árvore arcana, 10 a 15 eventos iniciais, contratos e missões | Tecnologia, Magia, Eventos, Objetivo | Só os eventos iniciais | **Moldes prontos (V3-13)**; os 12 eventos da fatia vertical na V3-6; listas nas rodadas C1 a C3 |

**Fora da revisão** (decidido que fica para depois): a história do mundo e os nomes; a arte extra das estações; os sistemas P2; os Títulos de Nobreza; tudo marcado como "futuro".

## 5. Pendências por sistema

Resumo das "Perguntas em aberto" e das marcações **[Em aberto]**. Os itens **"valores a testar"** não são decididos agora: ficam para os testes da fatia vertical e do modo sem tela.

| Sistema | Pendências que pedem decisão (não só teste) |
|---|---|
| Produção / Funções | Resolvidas na IP3-11 (Operário e Engenheiro no Construtor; categoria pela receita; vagas pela tabela geral); falta a Destilaria no nível 2 (V3) |
| Construção | Resolvidas na V2-8 (portes, custos, manutenção, moradias) |
| Itens | Resolvidas na IP2 (lista final) e na IP3 (receitas) |
| Classe Social | Resolvidas na IP2-11 (mapeamento por Tier) e na V2-6 (quantidades) |
| Crime | Loja ilegal do jogador; tipos de missão de confiança; valor mínimo da recompensa para Caçadores |
| Cura | Quando o Inconsciente vira Morto (há proposta) |
| Eventos | Controle de intensidade do Narrador; nomes e focos dos narradores; formato do diário |
| Cultura | Nomes (história); traços finais |
| Facção | Detalhes da assembleia; guerra civil na crise de sucessão |
| Objetivo | Condições das Ambições; Renome por título; lista de contratos e missões |
| Comércio | Resolvida: o Cavalo é o animal de carga (V1-3); capacidade a testar |
| População | Lotes por nível, moradia dos Controlados e crianças decorativas (V3-8); capacidade das moradias decidida na V2-8 |
| Trabalho | Gerente das construções da Cidade; leilão; estudo na Universidade |
| Interface | Elementos fixos do HUD; janelas de gestão móveis; fonte do texto; IDs de texto para outros idiomas |
| Pipeline | Artista; pacotes provisórios; ferramenta de atlas |
| Arquitetura | PC alvo mínimo (V3-12); aprovação das bibliotecas (início do código); metas de quadros e de NPCs decididas na V2-11 |
| Viagem | Construção de estradas; quantidade e custo das patrulhas |
| Guerra | Custo da Muralha; estradas destruídas; guerra civil |
| Diplomacia | Fim da coalizão; mensageiros (futuro) |
| Companheiros | Bônus dos cargos (fora o de comandante); costumes de casamento de Autoritária, Tecnocrata e Arcana |
| Magia | Como se obtém o Grimório; chance de filho de Mago; feitiços de Suporte fora do combate |
| Tecnologia | Listas iniciais por cultura; cultura Agrária (a tabela de Rank por era foi decidida na IP1-3) |
| Religião, Clima, Notícias, Escravidão, Efeitos | Escritos agora; quase só valores a testar e "futuro" |

## 6. Propostas a confirmar

As cerca de 128 propostas vêm de sugestões aceitas com detalhes acrescentados pelo assistente, ou de sugestões que o autor ainda não viu uma por uma. Na revisão, cada uma recebe uma recomendação: **confirmar** (vira decisão do autor e perde a marcação), **mudar** (com o motivo) ou **adiar** (vira "valor a testar").

| Documento | Propostas | Recomendação geral |
|---|---|---|
| Facção | 19 | Confirmar quase todas; corrigir a da Caravana (I-6) e limpar duas já feitas (I-17) |
| Mundo e Estado Inicial | 14 | Confirmar (é o mapa que o autor já aprovou em linhas gerais) |
| Religião | 12 | Confirmar; alinhar as vagas do Templo (I-16) |
| Pipeline | 11 | Confirmar |
| Clima e Estações | 9 | Confirmar |
| Arquitetura | 8 | Confirmar; a aprovação das bibliotecas continua para o início do código |
| Efeitos de Itens | 6 | Confirmar |
| Notícias | 5 | Confirmar |
| Combate | 4 | Confirmar; o limite de 40 combatentes vira valor a testar |
| Crime | 4 | Confirmar |
| Viagem | 4 | Confirmar |
| Eventos, Guerra, Cura, Batalha | 3 cada | Confirmar |
| Escravidão, Diplomacia, Felicidade, Companheiros, Cultura, Itens | 2 cada | Confirmar |
| Os demais (Cidade, Interface, Objetivo, Início, Funções, Reputação, Tecnologia, Trabalho) | 1 cada | Confirmar |

A lista completa, proposta por proposta, com a recomendação, vai nas rodadas **V4** e **V5**. O autor pode responder "Confirmar todas, exceto..." para ir mais rápido.

## 7. Plano das rodadas de revisão

Plano refeito depois da V1: o autor pediu rodadas próprias de **Itens e Produção** (IP1 a IP3) antes da economia numérica.

| Rodada | Tema | O que resolve | Estado |
|---|---|---|---|
| **V1** | Inconsistências e faxina dos documentos | Seção 3 e o formato dos 13 documentos antigos | **Respondida e aplicada** (ver seção 8) |
| **IP1** | Padronização da produção | Estrutura da receita, família de cada construção, escada de níveis, Rank do produto, escala das unidades, tempo, ordens de produção, limites | **Respondida e aplicada** (ver seção 9) |
| **IP2** | Nomes e lista final dos itens | Regra dos nomes, matérias-primas, família da mana, intermediários, escada de materiais, armas, armaduras (peça Média), roupas, comida, outros bens, Luxo × Exclusivo (I-9, I-10, I-11, I-13) | **Respondida e aplicada** (ver seção 10) |
| **IP3** | Insumos de cada produto | Regras de quantidade e receitas de cada construção (A-2); Funções (Engenheiro, Operário, categorias); nova Funções e Construções | **Respondida e aplicada** (ver seção 11) |
| **V2** | Base numérica da economia e da fatia vertical | A-1 e A-3 a A-9: moeda, preços (a partir das receitas), consumo, construções, recursos, salários, rotina, IA simples e tick | **Respondida e aplicada** (ver seção 12) |
| **V3** | Pendências de regra | Seção 5, A-10 e A-11; quando fazer as listas da A-12 | **Respondida e aplicada** (ver seção 13) |
| **V4** | Confirmação das propostas, parte 1 | Facção, Mundo, Religião, Pipeline, Clima, Arquitetura e Efeitos, mais os Moldes de Conteúdo ("confirmar todas, exceto...") | **Respondida e aplicada** (ver seção 14) |
| **V5** | Confirmação das propostas, parte 2 | Todos os outros documentos (58 propostas) e o formato da ficha técnica | **Respondida e aplicada** (ver seção 15) |
| Ficha técnica | Ficha técnica lógica da fatia vertical | Estruturas, dados e regras da vila de 15 NPCs (V2-12) | **Pronta para o código** (FT1 aplicada): [[Ficha Técnica/Fatia Vertical\|Ficha Técnica: Fatia Vertical]]; a rodada FT2 decide o início |
| **C1 a C3** | Rodadas de conteúdo (ideia do autor na V3-13: primeiro os moldes) | C1 tecnologias e marcos; C2 feitiços; C3 contratos, missões, Ambições e eventos | Mais para frente |

Depois da V5: atualizar o [[Estado do Projeto]], marcar esta revisão como concluída e começar a **ficha técnica lógica da fatia vertical**.

## 8. Resultado da V1 (2026-10-17)

| Pergunta | Decisão do autor | O que foi feito |
|---|---|---|
| V1-1 Faxina e correções óbvias | Seguir sugestão | Os 13 documentos antigos ganharam o formato padrão; aplicadas I-1, I-6, I-8, I-12, I-15, I-17, I-18 e I-20 (crime de Sedição); a Cidade virou documento de entrada |
| V1-2 Armas sem produtor | Reformular Itens e Produção em rodadas próprias | Rodadas IP1 a IP3 |
| V1-3 Animais como itens | Seguir sugestão | Tipo Animal; Pasto, Rancho e Açougue; Cavalo como montaria e carga |
| V1-4 Peixe | **A pesca volta** (o mundo tem portos) | Recurso Peixe, construção Pesqueiro, Função Pescador |
| V1-5 Inventário | Seguir sugestão, com pedido de análise de eficiência | Inventário reescrito: tabela única por cidade com pilhas; análise na parte técnica do [[Sistema de Inventário/Descrição\|Inventário]] |
| V1-6 Nomes | Seguir sugestão | Mosquete, Machado Curto, Peças de Argila, Ossos etc. aplicados |
| V1-7 Itens sem produtor | Padronizar a produção e decidir os insumos em rodadas | IP1 e IP3 |
| V1-8 Nomes modernos | Reformular os nomes para o mundo numa rodada | IP2 |
| V1-9 Fome e Felicidade | Seguir sugestão | Fome, Felicidade individual e noites sem dormir no [[Sistema de NPC/Descrição\|NPC]] |
| V1-10 Templo | Seguir sugestão | Tabela geral de vagas; 2 Templos nas cidades de nível 4 |
| V1-11 Ordem | Seguir sugestão | "Confirmar todas, exceto..." nas V4 e V5 |

## 9. Resultado da IP1 (2026-10-17)

| Pergunta | Decisão do autor | O que foi feito |
|---|---|---|
| IP1-1 Receita padrão | Seguir sugestão | Uma receita fixa por produto, com 8 campos ([[Sistema de Produção/Descrição\|Produção]]) |
| IP1-2 Família por construção | Seguir sugestão | Cada item numa só construção; Aço Mágico só na Forja Arcana (resolve a I-22); Leves e Pesadas no Ferreiro e Média no Costureiro (resolve a I-21); Fumo e Pó Espiritual no Misturador |
| IP1-3 Escada de níveis | Seguir sugestão | Escada padrão de Tier e Rank por nível; Rank por era na [[Sistema de Tecnologia/Descrição\|Tecnologia]]; nas capitais, o nível alto na Era 1 só dá eficiência e vagas |
| IP1-4 Rank do produto | Seguir sugestão | Menor entre material, nível e habilidade (1 + habilidade ÷ 25); sorte de +1 (20%) e −1 (10%) |
| IP1-5 Escala | Seguir sugestão | 1 NPC real = 1 trabalhador e 1 consumidor; o Fator de Escala só muda a população exibida (Facção, Classe Social, Glossário) |
| IP1-6 Tempo | Seguir sugestão | Horas por unidade por Tier; velocidade 0,5 + habilidade ÷ 100; +10% por nível acima do mínimo |
| IP1-7 Ordens | Seguir sugestão | Fila de ordens do jogador (Fazer N, Manter estoque, Repetir) na [[Interface/Descrição\|Interface]]; nota da IA = lucro × (1 + déficit); comida com prioridade abaixo de 3 dias |
| IP1-8 Limites | Seguir sugestão | 3 insumos e 3 saídas; 4 passos; nenhum item órfão; sem ferramentas |
| IP1-9 Ordem das rodadas | Seguir sugestão | IP2 (nomes) → IP3 (insumos) → V2 |

**Consequências notadas ao aplicar:**

- A escada de níveis deixa alguns produtos do canvas abaixo do nível exigido: Licor, Ornamento e Joias, de Tier 4, aparecem antes do nível 5. O Remédio, de Tier 3, passou do Boticário nível 3 para o 4. A IP3 reposiciona todos.
- A IP2-7 propõe mudar um ponto da IP1-2: a armadura Leve de couro, no Costureiro, e a Média de malha, no Ferreiro.

## 10. Resultado da IP2 (2026-10-17)

| Pergunta | Decisão do autor | O que foi feito |
|---|---|---|
| IP2-1 Regra dos nomes | Seguir sugestão; **nomes de fantasia no futuro**, quando a história existir | 6 regras no [[Sistema de Itens/Descrição\|Itens]]; os nomes num arquivo de textos separado |
| IP2-2 Matérias-primas | Seguir sugestão | Sem embalagem (Grão, Tora, Minério de Ferro, Carne Crua, Ovo...) em todos os documentos |
| IP2-3 Mana | **Ideia do autor:** só a Pedra de Mana, como recurso bruto do kit dos Magos, sem refino; os derivados ficaram como sugestão | "Pedra Mágica" e a Pedra de Mana refinada saíram; Joia e Anel de Mana; derivados como **[Proposta]** (Aço Mágico = Barra de Aço + Pedra de Mana; joias; itens Arcanos) |
| IP2-4 Intermediários | Seguir sugestão | Bloco de Pedra, Tecido de Algodão e de Lã, Adubo, Pólvora; Barra de Cobre de Tier 1 |
| IP2-5 Escada de materiais | Seguir sugestão | de Madeira, Composto, Arcano; de Couro, Cravejado, Arcano |
| IP2-6 Armas | Seguir sugestão | Arcabuz, Fuzil Raiado, Cetro, Flecha, Virote, Cartucho, Bombarda e Canhão; sai o Arco Recurvo; Combate e Batalha atualizados |
| IP2-7 Armaduras | Seguir sugestão | Leve de couro (Costureiro), Média de malha e Pesada de placas (Ferreiro); kits da Batalha atualizados |
| IP2-8 Roupas | Seguir sugestão | Conjuntos por papel social (Pobre, Trabalhador, Mineiro, Artífice, Furtivo, Mercador, Nobre, Realeza) |
| IP2-9 Comida | Seguir sugestão | Vinho, Peixe Defumado, Assado Temperado, Presunto Curado e Doce de Especiarias; saem os Legumes Cozidos; Efeitos de Itens atualizados |
| IP2-10 Outros bens | Seguir sugestão | Cerâmica, Roupa Comum e Roupa Fina |
| IP2-11 Consumo por Tier | Seguir sugestão | Tabela nova na [[Sistema de Classe Social/Descrição\|Classe Social]]; resolve a I-13 |

**Documentos atualizados:** Itens (reescrito), Recursos Existentes, Funções e Construções (família e nomes; receitas na IP3), Produção, Classe Social, Efeitos de Itens, Cura, Magia, Mundo, Objetivo, Clima, Construção, Batalha, Combate e Glossário.

## 11. Resultado da IP3 (2026-10-17)

Todas as 11 perguntas: **Seguir sugestão**. As receitas viraram o documento [[Sistema de Produção/Receitas\|Receitas]], e a [[Sistema de Produção/Funções e Construções\|Funções e Construções]] foi refeita nível por nível.

| Pergunta | O que foi feito |
|---|---|
| IP3-1 Regras de quantidade | Lote pelo Tier (4, 2, 1, 1); horas pelo degrau (8, 12, 24); 3 insumos = 3 tipos; nível mínimo pelo Tier ou pelo degrau; munição em lote; exceção da Forja Arcana |
| IP3-2 Construções do mapa | Braçais sem insumo; Adubo opcional (+25%); Pasto e Rancho com "exige no estoque"; Açougue com uma receita por animal (única exceção) |
| IP3-3 a IP3-8 Oficinas | Receitas da Metalurgia, Forja Arcana, Pedreira, Carpinteiro, Ferreiro (moldes), Armeiro, Costureiro e Artesão |
| IP3-9 Cozinheiro e Destilaria | Receitas; **a era trava só os materiais de equipamento e os itens com Rank**: os bens de luxo existem desde a Era 1 (Tecnologia, Produção e Itens atualizados) |
| IP3-10 Misturador e Boticário | Receitas; Pólvora de Carvão e Dejetos Animais |
| IP3-11 Funções | Operário e Engenheiro no Construtor (fecha a pergunta da Inteligência na Construção); categoria pela receita; nomes confirmados; vagas pela tabela geral |

**Conferência:** a cadeia mais longa tem 4 passos (Aço Mágico); nenhum item órfão. **Achado novo:** a Destilaria não tem produto no nível 1 (a Cerveja é de Tier 2); a proposta é construí-la já no nível 2, a confirmar na V3.

## 12. Resultado da V2 (2026-10-17)

Todas as 12 perguntas: **Seguir sugestão**. As partes abstratas A-1 e A-3 a A-9 estão resolvidas.

| Pergunta | Onde ficou |
|---|---|
| V2-1 Moeda | [[Sistema Econômico/Descrição\|Econômico]] e [[Arquitetura/Descrição\|Arquitetura]]: 1 casa decimal, décimos inteiros por dentro |
| V2-2 Rotina | [[Sistema de IA/Descrição\|IA]]: horas por classe e turnos de 8 h |
| V2-3 Salários e soldo | [[Sistema de Trabalho e Propriedade/Descrição\|Trabalho]]: 0,75 / 1,0 / 1,5 / 2,5 por hora; soldo 8 por dia |
| V2-4 Preço base | Econômico: fórmula, multiplicadores de Rank e tabela de preços de partida |
| V2-5 Mercados | Econômico: ajuste da facção ±20%; fator do Mercado Local de 0,5 a 2,0; 10% por dia |
| V2-6 Consumo | [[Sistema de Classe Social/Descrição\|Classe Social]]: cesta mínima, regra do gasto (80%), composição por classe |
| V2-7 Bancos e imposto | Econômico, Trabalho e Facção: estabilizador, imposto 10/20/30%, cunhagem, 30% de empresas dos Ricos |
| V2-8 Construções | [[Sistema de Construção/Descrição\|Construção]] (3 portes) e [[Sistema de População/Descrição\|População]] (moradias) |
| V2-9 Recursos | [[Sistema de Recursos/Descrição\|Recursos]]: × 1,0 e × 1,5; limite por recurso; conta de comida |
| V2-10 IA simples | IA: 6 ações e notas |
| V2-11 Tick | [[Sistema de Tempo/Descrição\|Tempo]] e Arquitetura: 20 Hz; metas de 500 e 5.000 NPCs |
| V2-12 Fatia vertical | Arquitetura: vila de 15 NPCs e o teste que diz se o ciclo do dinheiro fecha |

## 13. Resultado da V3 (2026-10-17)

| Pergunta | Decisão do autor | Onde ficou |
|---|---|---|
| V3-1 Leis iniciais | Seguir sugestão, com **ideia própria: todas as facções proíbem os narcóticos, menos a Escravista, a única produtora inicial, com mão de obra escrava** | Tabela na [[Sistema de Facção/Descrição\|Facção]]; contrabando no Crime, Comércio, Escravidão, Efeitos e Classe Social |
| V3-2 Estilos de início | Seguir sugestão | [[Sistema de Início de Jogo/Descrição\|Início de Jogo]] |
| V3-3 Crime | Seguir sugestão | Multas, Caçadores (500), loja ilegal do jogador, missões de confiança |
| V3-4 Cura | Seguir sugestão | 4 h para acordar; 2 h de sangramento; Hospital |
| V3-5 Narradores | Seguir sugestão | 4 narradores, intensidade e diário no [[Sistema de Eventos/Descrição\|Eventos]] |
| V3-6 Eventos da fatia vertical | Seguir sugestão | 12 eventos no Eventos |
| V3-7 Assembleia e guerra civil | Seguir sugestão | Facção |
| V3-8 Cidade e trabalho | Seguir sugestão | Lotes e Controlados na População; gerente, leilão e Universidade no Trabalho |
| V3-9 Estradas e muralhas | Seguir sugestão | Viagem e Guerra |
| V3-10 Diplomacia e Companheiros | Seguir sugestão | Coalizão; bônus dos cargos; costumes confirmados |
| V3-11 Magia | Seguir sugestão | Grimório, chance de nascer Mago, 3 feitiços de Suporte |
| V3-12 Interface e PC alvo | Seguir sugestão | Interface (HUD sem fixos, Noto Sans, IDs de texto) e Arquitetura (PC mínimo) |
| V3-13 Listas e pontas | Seguir sugestão, com **ideia própria: listas como estruturas modulares; agora só os moldes** | Novo documento [[Arquitetura/Moldes de Conteúdo\|Moldes de Conteúdo]]; rodadas C1 a C3 depois; Destilaria no nível 2; Agrária fora da primeira versão |

**Pendências da seção 5** que a V3 fechou: Crime, Cura, Eventos (narradores e diário), Facção (assembleia), Comércio, População, Trabalho, Interface, Arquitetura (PC alvo), Viagem, Guerra, Diplomacia, Companheiros, Magia, Tecnologia (Agrária) e Produção (Destilaria). Ficam para depois: os nomes da história, as listas de conteúdo (C1 a C3) e as escolhas do Pipeline no início do código.

## 14. Resultado da V4 (2026-10-17)

| Documento | Decisão do autor | O que foi feito |
|---|---|---|
| Facção (17) | Confirmar todas | Marcações retiradas; F-14: fundar uma facção exige **2.000 moedas** |
| Mundo e Estado Inicial (14) | Confirmar todas | Marcações e a frase das propostas retiradas |
| Religião (11) | Confirmar todas | Marcações retiradas |
| Pipeline (11) | Confirmar todas | Marcações retiradas |
| Clima e Estações (9) | Confirmar todas | Marcações retiradas |
| Arquitetura (7) | O autor deixou a escolha com o assistente | Seguidas as recomendações: A-1, A-3, A-5 e A-6 confirmadas; A-2 (bibliotecas) adiada para o primeiro dia de código; A-4: **JSON e CSV**; A-7: marco novo da **simulação sem tela de 1 ano** |
| Efeitos de Itens (8) | Confirmar todas | Marcações retiradas |
| Moldes de Conteúdo | Seguir sugestão | Os 8 moldes novos confirmados |

## 15. Resultado da V5 e fim da revisão (2026-10-17)

| Grupo | Decisão do autor | O que foi feito |
|---|---|---|
| V5-1 a V5-8 (58 propostas) | Confirmar todas | Marcações retiradas de todos os documentos; IT-4: capacidade das construções (100, 200 ou 400 × nível pelo porte); IT-5: Saciedade pelo Tier (60, 80, 90, 100); os 4 focos antigos do Narrador trocados pelos 4 narradores da V3-5 |
| V5-9 Formato da ficha técnica | Seguir sugestão | Primeira versão da [[Ficha Técnica/Fatia Vertical\|Ficha Técnica: Fatia Vertical]] e a rodada FT1 |

**Revisão concluída.**

- Restam só as marcações **[Em aberto]** e **[Refinar depois]** (valores a testar, ideias futuras) e as propostas novas da ficha técnica.
- **Correção encontrada ao montar a ficha:** "1 tick = 2 s de jogo" (V2-11) estava errado; o certo é 4,5 s. O [[Sistema de Tempo/Descrição\|Tempo]] foi corrigido.
