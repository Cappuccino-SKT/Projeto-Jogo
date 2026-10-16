> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Facção

**Descrição geral**

A Facção é a entidade política que agrupa cidades (e, através delas, Mapas Locais, NPCs, dinheiro e soldados) sob uma liderança, um conjunto de leis e uma cultura dominante. Quase todos os outros sistemas já referenciam a Facção (Mercado da Facção, Itens proibidos, Mapa de Guerra, eventos de mudança de leis da Classe Social), mas nenhum definia quem é dono desses dados. Este documento define isso.

O mundo começa com **6 facções principais**, cada uma representando uma das **6 culturas** iniciais e uma das **6 regiões** do mapa (ver Sistema de Cultura). As cidades, a riqueza, os Esquadrões e as relações de cada facção no primeiro dia estão no [[Mundo e Estado Inicial/Descrição|Mundo e Estado Inicial]].

### Atributos

1. Nome
2. Líder — NPC marcado como Importante, de classe Realeza.
3. Dinastia — ver seção própria.
4. Cidades — lista das cidades vinculadas. A quantidade é derivada da lista.
5. Leis — ver seção própria.
6. Cultura dominante — **[Proposta]** a cultura com maior população somada nas cidades da facção, recalculada no fim de cada estação (assim uma conquista muda a cultura aos poucos, não de um dia para o outro).
7. Relações diplomáticas — ver seção própria.
8. **PIB** — dinheiro total da facção: soma do dinheiro de todas as cidades vinculadas. Serve para comparar facções entre si (IA, diplomacia, medição de poder). Não é gastável.
9. **Banco da Facção** — caixa próprio da facção. Acumula o que é arrecadado de impostos e é gasto em soldo do exército, obras, tributos e guerra. Os gastos devolvem dinheiro às cidades (salário de soldados, obras), fechando o ciclo do dinheiro. É o destino concreto do sumidouro de impostos previsto no Sistema Econômico.
10. **Exército** — conjunto dos Esquadrões pertencentes às cidades da facção. Veja a seção Conscritos, Esquadrões e Exército.

**Valores derivados.** **[Proposta]** Quantidade de cidades, PIB e total do Exército não são armazenados na facção: são somados a partir das cidades e mantidos em cache, atualizados só quando uma cidade muda. Isso evita ter o mesmo dado em dois lugares e dessincronizar. O Banco da Facção é o único valor financeiro armazenado na facção.

### Escala de população

Um NPC real (aquele com que o jogador interage no Mapa Local) representa várias pessoas no Mapa Mundial. Exemplo do autor: **1 NPC real = 100 pessoas**. Chamamos esse número de **Fator de Escala**.

- O Fator de Escala é **global** (um único valor para o jogo todo).
- A quantidade de NPCs reais de cada cidade vem do nível dela (ver Sistema de Cidade); a população total é NPCs reais × Fator de Escala.
- O valor ainda não foi definido: será testado e discutido. Deve ficar em arquivo de dados, fácil de ajustar.
- Vale para a população do Mapa Local e do Mapa Mundial ("População total da região") e para os conscritos. **[Proposta]** Documentar também no Sistema de Mapa.

### Conscritos, Esquadrões e Exército

**Conscritos** são um dos recursos para produzir tropas.

- **Conscritos disponíveis da cidade** (derivado): população × lei de Conscrição (%) ÷ Fator de Escala, menos os já alistados.
- **Alistar retira NPCs reais da cidade**, o que reduz o número de trabalhadores e causa perda de produtividade. Exemplo com fator 100: um exército de 2000 soldados consome 20 NPCs.
- **Quem pode ser alistado** é definido pelas leis de cada facção. Padrão: adultos com Função. **NPCs Importantes são sempre ignorados.**
- **Guardas não fazem parte do sistema de soldados de guerra.** NPCs com a Função de guarda continuam NPCs, fazendo a segurança da cidade, e nunca são alistados.
- Convocações altas reduzem a Felicidade da cidade enquanto durarem.
- **[Proposta]** O alistamento e a formação de Esquadrões acontecem no **Quartel** (ver Sistema de Construção).

**Esquadrão.** No Mapa de Guerra, como no Total War, o jogador não controla soldados individuais, mas **esquadrões**. Um exército de 2000 soldados é um conjunto de 20 esquadrões. Cada esquadrão é a unidade de comando.

- **Tamanho.** Inicialmente, 1 Esquadrão = 1 NPC removido = Fator de Escala soldados (mesma escala da população). O número final de soldados por esquadrão depende de testes de desempenho e pode mudar depois.
- **Soldados de guerra deixam de ser NPCs.** Ao serem alistados, os NPCs saem da simulação de NPCs e passam a existir apenas como parte de um Esquadrão.
- **Sem classe social.** Soldados não têm Classe Social. O consumo deles é definido pelo próprio Esquadrão, não pela Classe Social.
- **Manutenção.** Cada Esquadrão exige três tipos de manutenção:
  - **financeira** (soldo, pago pelo Banco da Facção);
  - **de comida** (Ração Militar, que já existe na lista de itens);
  - **de equipamentos** (materiais para manter armas e armaduras).
- **Dissolução.** Ao desfazer o exército, os soldados voltam a compor a população da cidade, e **novos NPCs são gerados** (pela Geração de NPC) nas classes **Escravo e Civil**. Os NPCs originais não voltam.
- **Baixas** em guerra são permanentes: a população da cidade diminui.

**[Proposta]** O Esquadrão é uma entidade própria, não um NPC. Os atributos que hoje estão em "Semi-Controlados — Exército" no Sistema de NPC (ID do Esquadrão, Formação, Alvo de combate, Moral, Disciplina e Vínculo) migram para ele, junto com: Tamanho (quantidade de soldados), Cidade de origem, Equipamento e Manutenção. Os 9 tipos de Esquadrão (com kit de equipamento), a Experiência e os números de batalha estão no [[Sistema de Batalha/Descrição|Sistema de Batalha]]; a formação no Quartel (2 dias), o General e os mercenários, no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

**Manutenção de equipamentos.** Consumo periódico de materiais (por exemplo, barras de ferro e couro), proporcional ao tamanho e ao equipamento do Esquadrão, sem durabilidade item a item. Isso é coerente com soldados deixarem de ser NPCs individuais.

**Suprimento e Caravana Militar.** O exército **carrega o próprio suprimento** durante excursões militares: Ração Militar e os materiais de manutenção de equipamento. A **Caravana** é a unidade auxiliar que armazena esses recursos.

- A Caravana faz parte do **sistema militar** e é de **uso exclusivo militar**. As caravanas civis (comércio) estão no [[Sistema de Comércio/Descrição|Sistema de Comércio]].
- **[Proposta]** A Caravana usa o Inventário (herdando do Inventário abstrato), não luta, mas precisa acompanhar o exército.
- **Suprimento esgotado:** a Moral cai gradualmente a cada dia sem suprimento. Abaixo de um limiar, os Esquadrões desertam, e não todos de uma vez.
- **[Proposta]** Se a Caravana é capturada ou destruída, o exército perde o que ela carregava (saque).
- Consumo por Esquadrão, capacidade da Caravana (60 unidades), reabastecimento, forrageio e comboio estão no [[Sistema de Guerra/Descrição|Sistema de Guerra]].

### Leis

Cada facção tem um conjunto de leis, e cada lei se liga a um sistema que já existe:

- **Itens proibidos** — alimenta o atributo Proibição dos itens e a lista de Itens proibidos da cidade.
- **Ajustes de preço por categoria** — alimenta o Mercado da Facção.
- **Escravidão** (permitida / proibida) — define se o atributo Escravo e a classe Escravo existem nas cidades da facção.
- **Imposto** (% da receita das cidades: salários e lucros) — a cidade retém uma parte para obras e o resto enche o Banco da Facção; Escravos não pagam (ver [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]]).
- **Conscrição** (% da população que pode ser alistada e quem é elegível) — define o teto de conscritos disponíveis em cada cidade.
- **Magia** (livre / restrita / proibida) — define como tratar NPCs Magos. Inclui a opção "privilégio dos magos", para a cultura Arcana: a magia e o poder ficam com os NPCs que têm o atributo Mago.
- **Crimes e penas** — define quais crimes valem, as punições (que variam por classe social) e a Recompensa. Ver Sistema de Crime e Justiça.
- **Poder do líder** — define se o líder pode barrar a mudança de leis. **[Proposta]** Autoritário (o líder decide e pode bloquear), Moderado (o líder tem veto) e Fraco (a assembleia decide).
- **Sistema de votação** — define como os votos das cidades são contados na assembleia (ver Assembleia e mudança de leis).
- **Tarifa** — imposto sobre caravanas civis que entram no território da facção; vai para o Banco da Facção. Acordo comercial zera a tarifa; embargo e guerra bloqueiam o comércio (ver [[Sistema de Comércio/Descrição|Sistema de Comércio]]).

**Herança total.** Toda cidade vinculada a uma facção adota todas as leis dela. Não há leis locais por cidade. O atributo "Itens proibidos" da Cidade passa a ser derivado das leis da facção.

### Assembleia e mudança de leis

A mudança de leis é decidida por **votação em assembleia**, que depende da quantidade de cidades da facção e da cultura de cada uma. **[Refinar depois]**

- Cada dono de cidade tem uma **cultura base**.
- Cada Cultura está vinculada a um **conjunto de leis que ela deseja** (ver Sistema de Cultura).
- A assembleia acontece **uma vez por ano** (64 dias de jogo), mais **assembleias extraordinárias** por evento. Qualquer cidade pode propor uma lei.
- Cada lei em pauta passa por votação. O peso de cada cultura corresponde à quantidade de cidades dela na facção. Muitas cidades de uma mesma cultura formam um bloco forte.
- **A forma de contar os votos não é fixa:** depende da lei **Sistema de votação** da facção. Opções: maioria simples, maioria qualificada (2/3), ponderada pela população da cidade, ponderada pelo nível da cidade, e decisão só do conselho de nobres.
- Se o jogador é dono de uma cidade da facção, ele também participa da votação.
- **O líder pode barrar uma lei**, mas isso depende de **dois fatores juntos**: a **personalidade do líder** (**[Proposta]** Agressividade alta e Sociabilidade baixa aumentam a chance de barrar) e se a lei **Poder do líder** da facção **permite** essa ação.
- Eventos de pressão de Nobres e Realeza (previstos na Classe Social) também podem forçar uma assembleia ou uma mudança de lei.

**Cidades pequenas** (como o assentamento nível 1 do jogador) votam com o peso definido pelo Sistema de votação ativo da facção: em alguns, o peso é igual por cidade; em outros, proporcional ao nível ou à população.

### Dinastia

**[Proposta]** A Dinastia é a lista de NPCs nobres ligados ao líder (família e nobres vinculados), todos marcados como Importante, com um papel: Líder, Herdeiro ou Nobre.

- A **regra de sucessão** é definida pela Cultura: primogenitura, eleição entre os nobres, duelo ou mérito, ou escolha do líder.
- Quando o líder morre, a sucessão acontece. Se não houver herdeiro, há uma crise: guerra civil, cidades que se declaram independentes ou dissolução da facção.
- Se o líder for o personagem do jogador, o herdeiro passa a ser o NPC Controlado, em vez de fim de jogo. Sem herdeiro, o jogador escolhe um companheiro com Opinião de +60 ou mais; as dinastias da IA casam e têm filhos por estação (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]). Isso conecta os atributos Grávida, Estágio e Relação que já existem.
- Nobres da dinastia podem governar cidades (cargo de governador).

### Relações diplomáticas

Cada par de facções em contato tem um **valor de relação** (−100 a +100). Só são guardados os pares que têm contato (fronteira em comum ou rota de comércio), não todas as combinações.

O valor muda por: fronteira compartilhada, diferença de cultura (afinidade entre as culturas, ver Sistema de Cultura), personalidade dos líderes (Agressividade, Ganância e Lealdade), eventos (crimes contra cidadãos, comércio, casamentos entre dinastias), tratados e guerras.

O **estado** (Paz, Guerra ou Trégua), os **tratados**, a **atitude** (faixa da relação), a vassalagem, os casamentos, os reféns, a Confiabilidade e as decisões da IA estão no [[Sistema de Diplomacia/Descrição|Sistema de Diplomacia]]. A guerra em si (motivos, exércitos, cerco, paz) está no [[Sistema de Guerra/Descrição|Sistema de Guerra]]; o Mapa de Guerra só é criado quando o jogador participa.

### Território: Mapas Locais sem cidade

Cada Mapa Local comporta **apenas uma cidade**, mas nem todo Mapa Local tem uma.

- **Mapas adjacentes a uma cidade** (os 4 vizinhos na malha do Mapa Mundial) são vinculados à facção dessa cidade. Esses mapas podem ser tratados como **vilas ou entrepostos em desenvolvimento**.
- **Mapas sem dono** existem para o jogador explorar. O jogador pode fundar ali seu assentamento e, mais tarde, fazê-lo crescer até uma cidade independente.
- **Mapa em disputa.** Um Mapa Local adjacente a cidades de **facções diferentes** fica **em disputa**: causa guerras constantes entre elas até que uma o domine, ou até que o jogador interfira e influencie o resultado.
- Em guerras entre facções de IA sobre um mapa em disputa, o resultado é gerado **automaticamente**, por um **cálculo rápido que analisa a força dos dois exércitos** em conflito, sem gerar um Mapa de Guerra. O Mapa de Guerra só é criado quando o jogador participa.
- O mapa em disputa **guarda quem o domina** (exceção à regra do derivado): domínio = vencer a última batalha ali e ficar 4 dias sem oposição. O mapa só muda de dono no tratado de paz (ver [[Sistema de Guerra/Descrição|Sistema de Guerra]]).
- **[Proposta]** O vínculo de um mapa adjacente é derivado (não armazenado): se a cidade mudar de facção por conquista, os mapas adjacentes seguem. O Mapa Local tem quatro estados possíveis: Sem dono, Território de facção, Em disputa e Com cidade.

**Assentamento do jogador em território de facção.** O jogador **pode** fundar um assentamento num Mapa Local vizinho de uma cidade de facção. Ele fica **vinculado à facção dominante daquele território**. Isso abre um estilo de jogo: o **dono de vila** que tenta ascender sua vila a uma grande cidade independente (ver Criação de facção).

**Assentamento em mapa em disputa.** Fica vinculado à facção que domina o mapa no momento da fundação, e passa a seguir o novo dono se o mapa for cedido num tratado de paz.

### Vinculação do jogador a uma facção existente

**Pertencimento por vínculo.** A facção de um Mapa Local com cidade é a facção da cidade dele. Uma cidade pertence a uma facção por **vontade** (oferecendo lealdade), por **conquista**, ou por ter sido **fundada** em território de facção. Em todos os casos ela passa a fazer parte da facção e a seguir **todas as leis dela** (ver Herança total).

**Membro por Rito de Admissão.** Cada facção tem um **Rito de Admissão** que depende da cultura. Exemplos do autor: uma facção escravista e gananciosa vende o título de nobre por dinheiro; uma facção guerreira exige vencer uma batalha.

O Rito de Admissão é definido por dados na Cultura, escolhendo entre tipos prontos (um ou uma combinação):

- Pagar título (dinheiro)
- Provar-se em combate (vencer batalha ou duelo)
- Serviço (missões ou tempo de serviço)
- Casamento com membro da dinastia
- Tributo (entregar itens)
- Indicação (reputação com um nobre)

**Líder de cidade vinculada.** O jogador que possui uma cidade vinculada participa das assembleias e pode votar nas leis (ver Assembleia e mudança de leis).

**Obrigações e benefícios.** **[Proposta]**

- **Obrigações:** tributo (% do imposto) e chamado às armas (conscritos).
- **Benefícios:** proteção diplomática e militar, preços do Mercado da Facção, acesso a recrutamento e cargos.

**Revolta.** O jogador que é dono de uma cidade e tenta se desvincular da facção dominante causa uma **revolta**.

A resposta da facção depende de um **valor calculado**, e esse valor decide a ação. O cálculo considera vários fatores, entre eles:

- se a facção tem **capacidade de impedir** a revolta (força militar disponível contra a da cidade);
- a **personalidade do líder** da facção (Agressividade, Lealdade, Ganância);
- **[Proposta]** o valor da cidade para a facção (PIB, recursos, posição estratégica);
- **[Proposta]** se a facção já está em outras guerras;
- a reputação do jogador na facção e a Opinião do líder sobre ele (ver [[Sistema de Reputação/Descrição|Sistema de Reputação]]).

**[Proposta]** Ações possíveis conforme o valor: aceitar a independência, negociar (por exemplo, tributo reduzido), declarar guerra (ver [[Sistema de Guerra/Descrição|Sistema de Guerra]]) ou bloquear o comércio.

**[Proposta]** Um jogador só pertence a uma facção por vez. Estados possíveis: Independente, Membro (nobre), Vassalo, Líder. A diferença entre Membro e Vassalo está no [[Sistema de Diplomacia/Descrição|Sistema de Diplomacia]].

### Criação de facção pelo jogador

Requisitos:

- Possuir uma cidade de **nível 3 (Cidade)**. É o maior nível que o jogador consegue construir por conta própria, então fundar uma facção é a conclusão natural da progressão.
- Ter **pelo menos 2 nobres** na dinastia: o líder e mais um companheiro. Os companheiros são promovidos à classe Nobre no ato da fundação, como exceção à regra de mobilidade social rara.
- A cidade não pode estar vinculada a outra facção. Se estiver, o jogador precisa antes se desvincular (o que causa uma revolta).

**[Proposta]** Demais requisitos:

- Valor mínimo de dinheiro (Banco da Facção inicial).
- Escolher a Cultura dominante entre as presentes na cidade, o nome e as leis iniciais.

**Caminho do dono de vila.** Fundar um assentamento em território de facção (nível 1), crescer até o nível 3, desvincular-se da facção dominante (revolta e possível guerra), reunir 2 nobres e fundar a própria facção.

### Fim de uma facção

**[Proposta]** Uma facção com 0 cidades é dissolvida. O líder e a dinastia viram NPCs sem facção (exilados) e podem refundar uma facção se conseguirem uma cidade.

### Ligação com outros sistemas

| Sistema | O que a Facção fornece ou recebe |
|---|---|
| Mapa | Facção do Mapa Local = facção da cidade ou da cidade adjacente; estados Sem dono / Território / Em disputa / Com cidade; participantes do Mapa de Guerra; Fator de Escala na População |
| Cidade | Facção, itens proibidos (derivado das leis), conscritos (derivado), impostos, voto na assembleia |
| NPC | Atributo Facção; alistar retira NPCs e cria Esquadrão; guardas ficam de fora; **[Proposta]** Título/Cargo para NPCs Importantes |
| Cultura | Leis desejadas, Rito de Admissão, regra de sucessão, afinidades, arte (ver Sistema de Cultura) |
| Econômico | Mercado da Facção, impostos para o Banco da Facção, soldo dos Esquadrões, cunhagem, tecnologias; Tarifa e caravanas civis (ver Sistema de Comércio) |
| Classe Social | Eventos de mudança de lei; soldados não têm classe; dissolução gera NPCs Escravo e Civil |
| Produção | Alistamento reduz trabalhadores e, portanto, produção; Ração Militar e materiais de manutenção para o exército |
| Inventário | Caravana Militar herda do Inventário abstrato |
| IA | IA de Facção (diplomacia, guerra, resposta à revolta, disputa de mapas) em baixa frequência |
| Tempo | Frequência das decisões da facção e das assembleias (uma por ano) |
| Eventos | Assembleia extraordinária, revolta, crise de sucessão e escassez em tempo de guerra são eventos condicionais, com causas registradas (ver [[Sistema de Eventos/Descrição\|Sistema de Eventos]]). Nos eventos com escolha, a IA decide pela Personalidade do líder |
| Combate | Se o personagem principal morre, o herdeiro assume; se todo o grupo cai, o jogador é capturado, sem Game Over (ver [[Sistema de Combate/Descrição\|Sistema de Combate]]) |
| Felicidade | A facção tem uma Estabilidade própria (média ponderada das cidades + legitimidade do líder); Estabilidade baixa dispara assembleia extraordinária, crise de sucessão ou golpe (ver [[Sistema de Felicidade e Estabilidade/Descrição\|Sistema de Felicidade e Estabilidade]]) |
| Guerra | Motivos, exércitos, suprimento, cerco, ocupação, saque, paz e cálculo rápido (ver [[Sistema de Guerra/Descrição\|Sistema de Guerra]]) |
| Batalha | Tipos de Esquadrão, kit, Moral, Experiência e Comitiva no Mapa de Guerra (ver [[Sistema de Batalha/Descrição\|Sistema de Batalha]]) |
| Diplomacia | Estado, tratados, atitude, vassalagem, casamentos, reféns, Confiabilidade e IA (ver [[Sistema de Diplomacia/Descrição\|Sistema de Diplomacia]]) |

### Perguntas em aberto

1. **Detalhes da assembleia** (marcado para refinar depois).
