> Segunda revisão geral do projeto, feita em 2026-10-17, quando todos os sistemas P0 e P1 ficaram escritos. Pedido do autor (pergunta D2 da rodada P1-6): revisar o que já existe, achar o que está abstrato, levantar todas as pendências e conferir todas as propostas, para seguir com elas ou mudar. A primeira revisão (2026-10-14) está na [[Revisão do Projeto]], que fica como histórico. O que esta revisão encontrou vira as **rodadas V1 a V5** (seção 7), respondidas no `Respostas.txt` como as outras.

## 1. Resumo

| Item | Quantidade |
|---|---|
| Documentos de sistema | 43 (mais o índice, o Glossário e os documentos de estado) |
| Documentos no formato padrão | 30 (22 com todas as seções: legenda, parte técnica, ligações e perguntas; 8 sem a parte técnica) |
| Documentos no formato antigo, de antes do padrão | 13 |
| Marcações **[Proposta]** (sugestões ainda não confirmadas) | cerca de 128 |
| Marcações **[Em aberto]** e **[Refinar depois]** no texto | cerca de 44 |
| Itens nas listas "Perguntas em aberto" | cerca de 119 (cerca de 30 são só "valores a testar") |
| Inconsistências encontradas nesta revisão | 21 (seção 3) |
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
| I-10 | Itens × Funções e Construções | Itens sem produtor: Torta de Fruta, Legumes Cozidos, Carne Envelhecida, **Doce de Baunilha** (baunilha não é recurso), Anel de Cobre e de Ouro, Coroa, chapéus, camisas e calças com nome, arcos e bestas de Aço, Pedra de Mana, Joia Mágica | V1-7 |
| I-11 | Itens × Mundo (tom) | "Capuz Ninja", "Terno", "Camiseta Moderna", "Calça social" e "Cajado Moderno" soam modernos num mundo sombrio de chegada da pólvora | V1-8 |
| I-12 | Classe Social | Usa "Nobre" na lista de consumo e "Nobreza" no resto | Padronizar "Nobreza" |
| I-13 | Classe Social × Itens | Bens Exclusivos incluem o Licor, que na lista é "Alimento de Luxo"; o mapeamento Luxo × Exclusivo por Tier já estava em aberto | V2-3 |
| I-14 | NPC × IA × Felicidade | A IA usa **Fome** dos Controlados e a Felicidade tem um valor individual, mas os dois não estão na lista de atributos do NPC | V1-9 |
| I-15 | Comércio × Viagem | A pergunta "fluxo de caravanas em LOD 2" ficou obsoleta: a Viagem decidiu caravanas sempre reais. A pergunta "guardas reais ou abstratos" também foi respondida (viram NPCs só em LOD 0 e 1) | Fechar as duas perguntas |
| I-16 | Religião × Trabalho | O Templo tem 1, 2, 5, 10 e 15 vagas por nível; a tabela geral de vagas é 2, 3, 5, 7 e 10 | V1-10 |
| I-17 | Facção | Duas propostas já foram feitas e continuam marcadas: "documentar o Fator de Escala no Mapa" e "migrar os atributos do Esquadrão do NPC" | Limpar as marcações |
| I-18 | Itens × Combate | A regra da armadura mista está escrita duas vezes, como proposta, nos dois documentos | Confirmar uma vez (V4) e deixar só no Combate |
| I-19 | Escravidão × Combate | A rebelião de escravos usava Esquadrões no combate local | **Já corrigido nesta revisão:** combate local até 40; acima disso, Mapa de Guerra com 1 Esquadrão por NPC escravo |
| I-20 | Cultura × Crime | O pensamento extremo da Autoritária diz "pedir mudança de lei é crime", mas o crime não está na lista | Acrescentar o crime "Sedição" (gravidade 3) só nas facções com essa lei |
| I-21 | Itens × Funções e Construções | A lista de itens diz que as peças Leves e Pesadas são de metal e a Média é de couro (Costureiro); o canvas põe a "Armadura Leve" no Costureiro nível 3 e só a Pesada no Ferreiro | V1-7 |

## 4. Partes abstratas

O que existe só como ideia, sem regras ou números suficientes para virar código. As que **travam a ficha técnica da fatia vertical** vêm primeiro.

| Código | O que falta | Onde | Trava a fatia vertical? | Rodada |
|---|---|---|---|---|
| A-1 | **Moeda e preços base**: quanto vale cada item (por Tier e Rank) | Econômico, Itens | **Sim** | V2 |
| A-2 | **Receitas**: insumos, quantidades e tempo de cada produto (a fatia vertical precisa ao menos de Grão → Pão) | Produção | **Sim** | V2 |
| A-3 | **Consumo por classe**: quanto cada NPC real consome por dia (Comida, Materiais, Luxo...) | Classe Social | **Sim** | V2 |
| A-4 | **Construções**: custo, tempo de obra, manutenção por estação e níveis | Construção | **Sim** (Fazenda, Cozinheiro, Mercado, Casa) | V2 |
| A-5 | **Recursos**: produtividade base por recurso e qual item cada um gera | Recursos | **Sim** (Grão, Água) | V2 |
| A-6 | **Salários**: tabela por Função e nível | Trabalho | **Sim** | V2 |
| A-7 | **Rotina**: horas de dormir, trabalhar e Período Livre por classe | IA | **Sim** | V2 |
| A-8 | **IA de Utilidade**: quais ações o NPC escolhe e como os pesos se combinam | IA | Sim, mas simples na fatia vertical | V2 |
| A-9 | **Tick**: quantas vezes por segundo a simulação roda em cada LOD | Tempo, Arquitetura | Sim (valor de partida) | V2 |
| A-10 | **Leis iniciais** de cada facção, numa tabela única (hoje espalhadas) | Cultura, Facção | Não | V3 |
| A-11 | **Bônus dos estilos de início** e o custo do personalizado | Início de Jogo | Não | V3 |
| A-12 | **Listas de conteúdo**: 40 a 60 tecnologias, feitiços e árvore arcana, 10 a 15 eventos iniciais, contratos e missões | Tecnologia, Magia, Eventos, Objetivo | Só os eventos iniciais | V3 |

**Fora da revisão** (decidido que fica para depois): a história do mundo e os nomes; a arte extra das estações; os sistemas P2; os Títulos de Nobreza; tudo marcado como "futuro".

## 5. Pendências por sistema

Resumo das "Perguntas em aberto" e das marcações **[Em aberto]**. Os itens **"valores a testar"** não são decididos agora: ficam para os testes da fatia vertical e do modo sem tela.

| Sistema | Pendências que pedem decisão (não só teste) |
|---|---|
| Produção / Funções | Engenheiro e Operário sem construção; nomes e categorias das Funções; vagas por nível |
| Construção | Lista de níveis e custos das construções novas |
| Itens | Peça de armadura Média; Tier e Rank provisórios dos itens a cadastrar |
| Classe Social | Mapeamento Luxo e Exclusivo por Tier |
| Crime | Loja ilegal do jogador; tipos de missão de confiança; valor mínimo da recompensa para Caçadores |
| Cura | Quando o Inconsciente vira Morto (há proposta) |
| Eventos | Controle de intensidade do Narrador; nomes e focos dos narradores; formato do diário |
| Cultura | Nomes (história); traços finais |
| Facção | Detalhes da assembleia; guerra civil na crise de sucessão |
| Objetivo | Condições das Ambições; Renome por título; lista de contratos e missões |
| Comércio | Animais de carga (quais e de onde vêm) |
| População | Lotes por mapa e nível; capacidade das moradias; moradia dos Controlados; crianças decorativas |
| Trabalho | Gerente das construções da Cidade; leilão; estudo na Universidade |
| Interface | Elementos fixos do HUD; janelas de gestão móveis; fonte do texto; IDs de texto para outros idiomas |
| Pipeline | Artista; pacotes provisórios; ferramenta de atlas |
| Arquitetura | PC alvo mínimo e meta de quadros; meta de NPCs em LOD 0; aprovação das bibliotecas |
| Viagem | Construção de estradas; quantidade e custo das patrulhas |
| Guerra | Custo da Muralha; estradas destruídas; guerra civil |
| Diplomacia | Fim da coalizão; mensageiros (futuro) |
| Companheiros | Bônus dos cargos (fora o de comandante); costumes de casamento de Autoritária, Tecnocrata e Arcana |
| Magia | Como se obtém o Grimório; chance de filho de Mago; feitiços de Suporte fora do combate |
| Tecnologia | Listas iniciais por cultura; cultura Agrária; tabela de Rank por era |
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

| Rodada | Tema | O que resolve |
|---|---|---|
| **V1** | Inconsistências e faxina dos documentos | Seção 3 (as que pedem escolha) e o formato dos 13 documentos antigos |
| **V2** | Base numérica da economia e da fatia vertical | A-1 a A-9: moeda, preços, receitas, consumo, construções, recursos, salários, rotina, IA simples e tick |
| **V3** | Pendências de regra e listas de conteúdo | Seção 5 e A-10 a A-12 |
| **V4** | Confirmação das propostas, parte 1 | Facção, Mundo, Religião, Pipeline, Clima, Arquitetura e Efeitos |
| **V5** | Confirmação das propostas, parte 2 | Todos os outros documentos |

Depois da V5: atualizar o [[Estado do Projeto]], marcar esta revisão como concluída e começar a **ficha técnica lógica da fatia vertical**.
