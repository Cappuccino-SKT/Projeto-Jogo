> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Cultura

**Descrição geral**

A Cultura é o conjunto de ideologia, leis desejadas, ritos e estilo visual de um povo. Ela já existe como atributo de NPC, Cidade, Item, Mapa Local e Facção, mas até agora não tinha documento próprio. A Cultura é o que torna as facções diferentes entre si, não só em aparência, mas em como pensam e governam.

Religião **não é um sistema à parte**: ela faz parte da Cultura. Uma cultura religiosa é uma cultura que busca ideais religiosos.

### As 6 culturas iniciais

- O jogo começa com **6 culturas**, cada uma representando **uma das 6 regiões** do mapa.
- Existem **6 facções principais**, uma para cada cultura.
- Os nomes das culturas e a distribuição nas regiões **ainda não foram definidos**.
- Cada cultura tem uma **ideologia completamente diferente** das outras.
- Cada cultura tem **pelo menos 1 pensamento extremo**.
- As 6 ideologias: **Escravista**, **Religiosa**, **Autoritária**, **Tecnocrata**, **Guerreira** e **Arcana**.
- A cultura **Escravista já é a mercantil**: ela trata tudo como mercadoria, inclusive pessoas. Por isso não existe uma cultura Mercantil separada.
- Cada cultura está vinculada a uma **estrutura de leis já pronta**, que é o conjunto de leis iniciais da sua facção principal.

### Atributos

1. Nome
2. Região de origem — uma das 6 regiões do mapa.
3. Traços — as estruturas pré-feitas que compõem a cultura (ver seção Traços).
4. Pensamentos extremos — ver seção própria.
5. Leis desejadas — para cada lei, um valor ideal e um peso de importância.
6. Rito de Admissão — derivado dos traços.
7. Regra de sucessão — derivada dos traços.
8. Afinidade com outras culturas — afim, neutra ou hostil.
9. Modificadores de NPC — ajustes na Geração de NPC.
10. Arte — textura de itens, armas e roupas, e arte própria de construções.

### Traços (estruturas pré-feitas)

As culturas não são escritas do zero. Existe uma lista de **Traços** prontos, cada um com seus valores padrão, e cada cultura é uma **combinação** de traços. Depois é possível sobrescrever valores de uma cultura específica.

Cada traço define valores padrão de:

- **Leis desejadas** (valor ideal e peso);
- **Rito de Admissão**;
- **Regra de sucessão**;
- **tendências de Personalidade** dos NPCs.

**[Proposta]** Primeira lista de traços, partindo das ideologias que você já escolheu. São exemplos para discutir, não definições finais.

| Traço | Leis desejadas (exemplos) | Rito de Admissão | Sucessão | Pensamento extremo (exemplo) |
|---|---|---|---|---|
| **Escravista** (mercantil) | Escravidão permitida e ampla; comércio livre (tudo é mercadoria); imposto alto; poucos itens proibidos | Pagar título | Herança | A liberdade de escravos é crime; qualquer pessoa pode ser propriedade |
| **Religiosa** | Narcóticos (Fumo, Pó Espiritual) proibidos; magia restrita; leis severas contra heresia | Serviço (peregrinação, missões) | Escolha do clero ou eleição entre nobres | Heresia é crime capital; guerra santa contra culturas hostis |
| **Autoritária** | Poder do líder autoritário; conscrição alta; assembleia limitada | Indicação (lealdade jurada) | Escolha do líder ou herança | O líder é inquestionável; pedir mudança de lei é crime |
| **Tecnocrata** | Tecnologia e pólvora incentivadas; imposto para pesquisa; mérito acima de nascimento | Serviço (provar competência) | Mérito | Magia proibida; magos são banidos |
| **Guerreira** | Conscrição alta; guerra como honra; benefícios a veteranos | Provar-se em combate | Duelo | Só vale quem luta; civis sem serviço militar perdem direitos; recusa paz permanente |
| **Arcana** | Magia como privilégio; leis diferentes para NPCs Magos e não Magos; poder concentrado nos magos | Provar-se na magia (precisa ser Mago) | Poder mágico (Magia e Mana) | Só magos governam; não Magos são cidadãos de segunda classe |

**Cultura Arcana e o atributo Mago.** Todo NPC já tem a booleana Mago (capaz de usar magia ou não). Na cultura Arcana essa diferença ganha peso político: ser Mago ou não cria **desigualdade social e discriminação** dentro da facção, o que dá história e conflito interno a ela. Isso é modelado como lei da facção (a opção "privilégio dos magos" na lei Magia), sem criar uma nova classe social.

**Outros traços candidatos** para combinações futuras e para novas culturas: Agrária/Naturalista (contra a tecnologia) e Erudita.

### Pensamentos extremos

Toda cultura tem **pelo menos um pensamento extremo**: uma ideia levada ao limite que a distingue de qualquer outra.

**[Proposta]** Mecânica: um pensamento extremo é uma **lei com valor ideal no limite e peso máximo**. Efeitos:

- Se a facção desrespeita o pensamento extremo, a Felicidade das cidades dessa cultura cai muito mais que o normal, e o risco de revolta sobe.
- Se a facção respeita, a cultura recebe um bônus próprio ligado ao tema.
- O pensamento extremo também influencia a relação diplomática: culturas com extremos opostos tendem à hostilidade.

### Leis desejadas

Para cada lei do Sistema de Facção, a cultura tem:

- um **valor ideal** (o que ela quer);
- um **peso de importância** (o quanto ela se importa).

Exemplo: a cultura Escravista tem Escravidão = permitida, com peso alto.

Isso é usado:

- na **assembleia**, onde cada cultura vota pelo seu ideal;
- na **Felicidade da cidade** (ver abaixo);
- como **leis iniciais** da facção principal daquela cultura.

### Cultura e Felicidade

A Cultura afeta a Felicidade da cidade: quanto **maior a distância** entre as leis da facção e as leis desejadas pela cultura da cidade, **menor a Felicidade**. Isso alimenta os eventos políticos e de revolta já previstos, e faz a conquista de uma cultura diferente ter custo real. A conta e os pesos estão no [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]].

### Cultura da cidade

A cultura de uma cidade é a **cultura do dono (base)**, mais uma **proporção agregada de outras culturas**, sem registrar NPC por NPC. Após uma conquista, essa proporção muda **devagar**, ao longo de anos.

### Cultura do NPC

O NPC **nasce com a cultura da cidade**. Migrantes e escravos capturados **mantêm a cultura de origem**, o que permite minorias dentro de uma cidade.

### Efeitos nos atributos do NPC

Na Geração de NPC (que já usa Cultura e Classe Social), a cultura aplica:

- **modificadores pequenos** nas faixas de habilidade (por exemplo, ±10 a 20);
- uma **tendência nos modelos de Personalidade** (por exemplo, cultura guerreira com Agressividade e Coragem maiores).

### Relações entre culturas

Cada par de culturas tem **três níveis de afinidade**: **afim**, **neutra** ou **hostil**, em vez de uma matriz numérica completa. Isso influencia:

- a relação diplomática entre as facções;
- a Felicidade de cidades com cultura mista.

### Rito de Admissão e Sucessão

Vêm dos **traços**, com valor padrão por traço e possibilidade de sobrescrever por cultura.

- **Rito de Admissão** (tipos prontos): pagar título, provar-se em combate, serviço, casamento com membro da dinastia, tributo, indicação.
- **Regra de sucessão**: primogenitura, eleição entre nobres, duelo ou mérito, escolha do líder, escolha do clero.

### Arte da cultura

- **Itens, armas e roupas:** o item existe como um único item no jogo (por exemplo, uma espada), e a **textura muda conforme a cultura que o produziu**, como já definido no Sistema de Itens.
- **Construções:** **cada cultura tem sua própria arte de construção.** Isso multiplica o volume de arte: hoje os canvases têm 22 tipos de construção (7 civis e 15 de produção), a maioria com 5 níveis, vezes 6 culturas. **[Em aberto, risco de escopo]** Vale planejar um jeito de reaproveitar partes (telhados, paredes, portas) entre os níveis e culturas, para a arte feita à mão ser viável.

### Início de jogo e Cultura

O início do jogo será parecido com o do **Kenshi**: o jogador escolhe entre vários **estilos de início**.

- Os estilos **já vêm com uma cultura definida**.
- Haverá também **estilos personalizados**, onde o jogador decide tudo, incluindo a cultura.
- O estilo de início **não altera nenhuma mecânica**: ele só dá bônus iniciais ao jogador.

Os estilos de início têm documento próprio: **Sistema de Início de Jogo**.

### Ligação com outros sistemas

| Sistema | O que a Cultura fornece ou recebe |
|---|---|
| Facção | Leis desejadas, Rito de Admissão, sucessão, cultura dominante, assembleia |
| Cidade | Cultura (do dono) e proporção de outras culturas; Felicidade |
| NPC | Atributo Cultura; modificadores de Personalidade e habilidades na geração |
| Itens | Textura por cultura em armas, equipamentos e roupas |
| Construção | Arte própria por cultura |
| Classe Social | Faixas de habilidade por Cultura + Classe (já existente) |
| Mapa | Cultura da região; 6 regiões iniciais |
| Diplomacia | Afinidade entre culturas e pensamentos extremos |
| Eventos | Violar um pensamento extremo, ou uma Felicidade baixa por muito tempo, dispara eventos condicionais duradouros com a causa registrada (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]) |
| Combate | O Duelo é um modo de combate usado como rito de "provar-se em combate" (ver [[Sistema de Combate/Descrição|Sistema de Combate]]) |

### Perguntas em aberto

1. **Nomes das culturas e regiões.** Ainda não definidos.
2. **Arte de construções.** Como reaproveitar partes entre culturas e níveis?
