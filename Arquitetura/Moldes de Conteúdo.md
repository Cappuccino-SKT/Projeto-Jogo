> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> **Ideia do autor (rodada V3-13):** as listas grandes (tecnologias, leis, feitiços, missões...) viram **estruturas modulares**: um molde pronto, que depois é só copiar e modificar. As listas completas ficam para mais adiante (rodadas de conteúdo C1 a C3); agora só os moldes. Os moldes novos são **[Proposta]** até a confirmação na rodada V4.

## Moldes de Conteúdo

**Descrição geral**

Um **molde** é a ficha padrão de um tipo de conteúdo: quais campos ele tem, quais valores cada campo aceita e um exemplo preenchido. Todo conteúdo do jogo é **dado**, não código ([[Arquitetura/Descrição|Arquitetura]]):

- cada tipo vira um arquivo na pasta `data/`;
- cada entrada é uma cópia do molde com outros valores.

Para criar uma tecnologia nova, por exemplo, basta copiar uma que já existe, trocar o ID, o nome e os valores, e rodar o `jogo --validar`.

**Regras de todos os moldes:**

1. **ID de texto legível** e único, com o prefixo do tipo (`tec.estradas`, `lei.imposto`). O nome que aparece na tela fica no arquivo de textos (`textos/pt-BR.json`), para o nome mudar sem mexer no ID.
2. **Listas fechadas:** os campos que "fazem algo" (efeitos, desbloqueios, condições) só aceitam valores de uma lista fechada, como os efeitos dos eventos. Um valor novo na lista exige código novo; um conteúdo novo, não.
3. **Referências por ID:** um molde cita outro pelo ID (a tecnologia libera `receita.mosquete_aco`). O validador avisa referência quebrada.
4. **Formato:** **JSON** para os moldes com estrutura (listas dentro de listas); **CSV** para as tabelas grandes e simples (itens, receitas, preços), editadas em planilha ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]).

## Moldes que já existem

Estes já estão definidos nos seus sistemas; aqui fica só a referência.

| Molde | Arquivo | Onde está definido |
|---|---|---|
| Item | `itens.csv` | [[Sistema de Itens/Descrição\|Itens]] (atributos e atributos por tipo) |
| Receita | `receitas.csv` (com moldes de armas e armaduras expandidos ao carregar) | [[Sistema de Produção/Receitas\|Receitas]] |
| Construção | `construcoes.json` | [[Sistema de Construção/Descrição\|Construção]] (atributos e os 3 portes) |
| Evento | `eventos.json` | [[Sistema de Eventos/Descrição\|Eventos]] (ID, Tipo, Categoria, Escopo, Gatilho, Condições, Duração, Efeitos, Peso, Cooldown, Aviso, Escolhas) |
| Estilo de início | `estilos.json` | [[Sistema de Início de Jogo/Descrição\|Início de Jogo]] (cultura, local, dinheiro, itens, companheiros, relação, bônus; custo em pontos) |
| Traço de cultura | `culturas.json` | [[Sistema de Cultura/Descrição\|Cultura]] (leis desejadas, Rito de Admissão, sucessão, Personalidade, pensamento extremo, afinidades) |

## Moldes novos **[Proposta]**

### Tecnologia (`tecnologias.json`)

| Campo | O que aceita |
|---|---|
| ID | `tec.` + nome |
| Ramo | Agricultura e Produção, Construção, Militar, Economia, Administração |
| Era | 1 a 4 |
| Custo | Pontos de Pesquisa |
| Pré-requisitos | Lista de IDs de tecnologias |
| Libera | Lista fechada: construção, nível de construção, receita, item, lei (ou opção de lei), nível de cidade, bônus percentual (raro) |
| Cultura inicial | Opcional: culturas que podem recebê-la no começo do jogo |
| Descrição | Texto (no arquivo de textos) |

*Exemplo* (copiar e modificar):

```json
{
  "id": "tec.estradas",
  "ramo": "Construcao",
  "era": 1,
  "custo": 200,
  "pre_requisitos": [],
  "libera": [ { "tipo": "obra", "id": "obra.estrada" } ],
  "cultura_inicial": ["Autoritaria", "Tecnocrata"]
}
```

### Marco mundial (`marcos.json`)

| Campo | O que aceita |
|---|---|
| ID | `marco.` + nome |
| Tipo | Maior (abre uma era) ou menor |
| Janela | Ano mínimo e máximo |
| Adiantado por | Lista de IDs de tecnologias de facção |
| Abre a era | 2, 3 ou 4 (só nos maiores) |
| Libera | Mesma lista fechada da Tecnologia |
| Evento | ID do evento "Descoberta de Tecnologia" que ele dispara |

### Lei (`leis.json`)

| Campo | O que aceita |
|---|---|
| ID | `lei.` + nome |
| Opções | Lista de opções; cada uma com ID, nome e **efeitos** |
| Efeitos | Lista fechada: taxa (imposto, tarifa), permitir ou proibir (escravidão, magia, item), modificador (Felicidade, Controle, preço por categoria, conscrição), regra de assembleia (votação, poder do líder) |
| Desejo por cultura | Para cada cultura, a opção ideal e o peso (é a conta "leis × cultura" da Felicidade) |
| Exige | Opcional: tecnologia que libera a lei |

*Exemplo:* `lei.imposto`, com as opções Baixo (taxa 10%, Felicidade 0), Normal (20%, −5) e Alto (30%, −10). A Escravista deseja Alto (peso 2); a Guerreira, Baixo (peso 1).

### Feitiço (`feiticos.json`)

| Campo | O que aceita |
|---|---|
| ID | `feitico.` + nome |
| Escola | Combate, Suporte, Ofício |
| Custo | Mana; Pedra de Mana (opcional) |
| Tempo | Conjuração (s) e recarga (s) |
| Alvo | Um alvo, área (raio em tiles), o próprio Mago, a cidade |
| Efeitos | Lista fechada: dano, cura, modificador temporário, controle (lentidão, atordoamento), efeito na cidade (produção, clima) |
| Requisitos | Inteligência mínima; pesquisa arcana que libera |

*Exemplo:* `feitico.bencao_colheita` — Suporte; alvo: a cidade; efeito: Fazenda +20% por 1 estação; limite: 1 por cidade por estação (V3-11).

### Contrato e missão longa (`contratos.json`, `missoes.json`)

| Campo | O que aceita |
|---|---|
| ID | `contrato.` ou `missao.` + nome |
| Quem oferece | Quadro da Taverna, NPC Importante, facção, loja ilegal |
| Nasce de | Condição do mundo (lista fechada, igual à dos eventos condicionais): Criminalidade alta, caravana em risco, masmorra cheia... |
| Etapas | 1 nos contratos; 3 a 5 nas missões longas. Cada etapa tem um **objetivo** da lista fechada: entregar, escoltar, matar ou capturar, limpar masmorra, investigar, convencer |
| Requisitos | Renome mínimo, reputação mínima, Confiança (missões de confiança) |
| Recompensa | Moedas, Renome, reputação, Confiança, item |
| Prazo e falha | Dias de jogo; o que acontece se falhar |

*Exemplo:* `contrato.entrega_ilegal` — oferecido pela loja ilegal; 1 etapa (entregar mercadoria proibida a outra cidade); recompensa +25 de Confiança (V3-3).

### Ambição (`ambicoes.json`)

| Campo | O que aceita |
|---|---|
| ID | `ambicao.` + nome |
| Condições | Lista fechada: dinheiro, título, cidade ou região possuída, Renome, Importante derrotado, masmorra lendária limpa, feitiço ou habilidade |
| Importante ligado | Opcional (o Arquimago, o Rei Exilado...) |
| Epílogo | Texto do fim, no arquivo de textos |

### Narrador (`narradores.json`)

| Campo | O que aceita |
|---|---|
| ID | `narrador.` + nome |
| Foco | Peso de cada categoria de evento ocasional |
| Ritmo da tensão | Velocidade de subida e de alívio |
| Intensidade | Padrão 100% (o jogador ajusta de 50% a 150%; V3-5) |

### Masmorra (`masmorras.json`)

| Campo | O que aceita |
|---|---|
| ID e posição | `masmorra.` + nome; Mapa Local (coluna, linha) |
| Dificuldade | 1 a 5 (5 = lendária) |
| Tipo | Descritivo (mina abandonada, cripta...) |
| Inimigos e loot | IDs de tabelas de sorteio |
| Repovoamento | Dias de jogo |

---

## Parte técnica

- **Pasta:** `data/`, um arquivo por tipo; os textos em `textos/pt-BR.json`.
- **Validação:** o `jogo --validar` confere os campos obrigatórios, os valores das listas fechadas e as referências por ID.
- **Ferramenta:** um editor de texto comum (VS Code) para o JSON e uma planilha para o CSV ([[Pipeline de Arte e Conteúdo/Descrição|Pipeline]]).
- **Fatia vertical:** usa só os moldes de Item, Receita, Construção e Evento (12 eventos; V3-6).

## Ligação com outros sistemas

| Sistema | Molde |
|---|---|
| Tecnologia | Tecnologia e Marco mundial |
| Facção e Cultura | Lei e Traço de cultura |
| Magia | Feitiço |
| Objetivo e Progressão | Contrato, Missão longa, Ambição, Masmorra |
| Eventos | Evento e Narrador |
| Itens, Produção, Construção | Item, Receita, Construção |
| Início de Jogo | Estilo de início |

## Perguntas em aberto

1. **Confirmar os moldes novos** (rodada V4).
2. **Listas completas** nas rodadas de conteúdo, mais para frente:
   - **C1:** tecnologias e marcos;
   - **C2:** feitiços e árvore arcana;
   - **C3:** contratos, missões, Ambições e mais eventos.
