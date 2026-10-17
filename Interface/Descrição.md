> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Interface e Controles

**Descrição geral**

Define o que o jogador vê e como ele controla o jogo. O jogo tem muita informação (Felicidade por classe, economia, política, produção), então a interface segue quatro princípios:

1. **Informação em camadas.** Um resumo sempre visível; o detalhe só quando o jogador pede.
2. **Sempre o "por quê?".** Todo número importante pode mostrar de onde veio.
3. **Personalizável.** O jogador precisa poder adaptar a interface à resolução e às necessidades dele. Isso vem de uma experiência do autor: em jogos de gerenciamento, uma interface fixa pode atrapalhar quem tem uma resolução fora do padrão (o autor joga em 1600 × 900), sem nenhuma forma de ajustar.
4. **Nítida.** O mundo é pixel-art em escala inteira, mas o texto da interface é desenhado em resolução nativa, porque o jogador lê muito.

---

## Estilo e escala

### Resolução e texto

- O **mundo** é desenhado em pixel-art ampliada em escala inteira (ver [[Arquitetura/Descrição|Arquitetura]]).
- A **interface** é desenhada em **resolução nativa** da tela, nítida, **separada do mundo**.
- As **letras não são desenhadas à mão**, pelo menos no início: o jogo usa fontes comuns e legíveis, para atender pessoas com escalas diferentes do normal. Molduras, ícones e botões continuam feitos à mão, no estilo do mundo.
- **Resolução de referência: 1600 × 900.** É a do autor, e o layout padrão é testado nela. O jogo também deve funcionar em resoluções menores (por exemplo, 1280 × 720) e maiores (1920 × 1080 e acima).
- A **escala da interface** é ajustável, de 100% a 200%.

### Tema

- **Um tema único** (molduras, botões e fontes) na primeira versão.
- Temas por cultura (cores e molduras) ficam como possibilidade futura, se o autor quiser.

---

## HUD personalizável

O autor pediu um HUD **bastante modificável** pelo jogador.

### O que o jogador pode mudar

- **Mover, redimensionar e esconder** cada elemento do HUD (barra superior, retratos do grupo, minimapa, ordens, alertas, notificações, painel contextual).
- **Escala** e **opacidade** de cada elemento.
- **Ancoragem** nas bordas e cantos da tela, com encaixe (snap), para o layout acompanhar a resolução.
- **Salvar perfis de layout** (por exemplo, "compacto" e "completo") e **restaurar o padrão** com um botão.

### Como funciona

- Um **modo de edição da interface**: o jogador entra, arrasta os elementos e sai.
- Os layouts padrão são feitos para resoluções comuns (1280 × 720, 1600 × 900, 1920 × 1080).
- O layout fica num **arquivo de perfil do jogador**, separado do save do jogo, e vale para todas as partidas.
- **Nenhum elemento fixo**, nem o menu de pausa, que abre com Esc. As janelas de gestão também se movem e mudam de tamanho, e a posição fica salva no perfil do jogador (rodada V3-12).

---

## Telas

### HUD do Mapa Local (layout padrão)

| Posição | Elemento |
|---|---|
| Barra superior | Data e hora, clima do dia, velocidade e pausa, dinheiro, notificações |
| Canto inferior esquerdo | Retratos dos Controlados com Vida, Mana e Postura |
| Canto inferior direito | Minimapa |
| Barra inferior central | Ordens e modo de construção |
| Painel lateral contextual | Aparece ao selecionar algo (NPC, construção, item) |

Todos esses elementos são ajustáveis (ver acima).

### Painéis de gestão

- **Uma janela de gestão por vez**, com abas, no estilo de Songs of Syx.
- Abas: **Cidade, Construções, Inventário e Equipamento, Pesquisa, Facção, Contratos, Comércio** e **Diário**.
- Evita janelas flutuantes soltas, que complicam a interface e o código.
- A aba **Diário** tem o **calendário** (estações e festivais, com os botões de patrocinar e cancelar; ver [[Sistema de Religião/Descrição|Religião e Fé]]) e a **Crônica do mundo**, com a data do acontecimento e a data em que o jogador soube (ver [[Sistema de Notícias/Descrição|Notícias]]).

### Mapa Mundial

- **Lentes de mapa** trocam o que é destacado: **Política** (facções), **Cultura, Felicidade, Comércio, Perigo** e **Tecnologia**.
- **Busca** por nome e **Viagem Rápida** (já previstas em [[Sistema de Mapa/Descrição|Sistema de Mapa]]).
- As **notícias** atualizam a informação datada de cada mapa ("Cidade Autoritária ocupada pela Guerreira, soube há 2 dias").
- **Entidades em trânsito** visíveis e **névoa de exploração** por Mapa Local, com a data de cada informação vista; janela de encontro (Lutar, Fugir, Negociar, Render-se) (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).

### Modo construção

- Ao entrar no modo, os **lotes livres** ficam destacados.
- Clicar num lote abre uma lista **por categoria**, com custo, vagas e materiais (ver [[Sistema de Construção/Descrição|Sistema de Construção]]).
- Há pré-visualização, **fila de obras** e demolição.

### Combate e pausa ativa

- Seleção **individual ou em caixa**.
- **Barras de Vida** sobre os NPCs em combate.
- Indicadores "**EM COMBATE**" e "**PAUSADO**" bem visíveis.
- As **ordens enfileiradas** durante a pausa aparecem como linhas e ícones no mapa.
- Regras em [[Sistema de Combate/Descrição|Sistema de Combate]].

### Clima e hora na tela

- Camada de clima por cima do mapa (pingos, flocos e névoa translúcida), tom de cor das 4 fases do dia e paleta de cada estação (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).

### Painel de trabalho

- Uma aba **Trabalho**, no estilo RimWorld: matriz **Função × prioridade**.
- Vale para os **Escravos do jogador** e os **empregados** dele.
- Usa a Lista de Prioridades que já existe ([[Sistema de NPC/Descrição|Sistema de NPC]] e [[Sistema de Trabalho e Propriedade/Descrição|Sistema de Trabalho e Propriedade]]).

### Ordens de produção

- No painel de cada construção do jogador, uma **fila de ordens** no estilo RimWorld: **Fazer N**, **Manter estoque de N** e **Repetir sempre**, cada uma com prioridade (rodada IP1-7; ver [[Sistema de Produção/Descrição|Produção]]).
- Cada ordem mostra os insumos que faltam e o tempo estimado; uma ordem parada por falta de insumo vai para a barra de alertas.

---

## Controles

- **Só teclado e mouse.** Não haverá adaptação para controle de console.
- **Botão esquerdo** seleciona e **botão direito** dá ordem (padrão de RTS).
- **Câmera:** WASD, bordas da tela ou arrastar com o botão do meio.
- **Atalhos remapeáveis.**
- **Zoom** em **níveis inteiros** (1x a 4x) para a pixel-art continuar nítida; a roda do mouse troca de nível.

---

## Informação e feedback

### Notificações

Seguem os níveis definidos nos [[Sistema de Eventos/Descrição|Eventos]] (Info, Aviso e Crítico, com pausa no Crítico). Aparecem como **ícones empilhados à direita**, e clicar centraliza a câmera no local.

**Personalização pelo jogador.** Quando muita coisa acontece ao mesmo tempo, principalmente em velocidade alta, as notificações podem incomodar. O jogador pode ajustar:

- quais **categorias** aparecem, ficam silenciosas ou vão **só para o diário**;
- o **limite** de notificações na tela e o **agrupamento** de iguais ("Colheita farta ×3");
- o que aparece em **velocidade alta** (por exemplo, só Aviso e Crítico);
- o **tempo** que cada notificação fica na tela;
- se um aviso Crítico **pausa** o jogo ou só reduz a velocidade, por categoria.

Há uma categoria **Notícias** para as notícias do mundo que chegam ao jogador (ver [[Sistema de Notícias/Descrição|Notícias]]).

Padrão: tudo ligado, agrupamento ativo, só Aviso e Crítico acima de 4x, e Crítico pausa. **[Refinar depois]** lista exata de opções.

### "Por quê?"

Passar o mouse sobre um número (Felicidade, preço, produção, evento) mostra o **detalhamento** com os maiores fatores (ver [[Sistema de Felicidade e Estabilidade/Descrição|Sistema de Felicidade e Estabilidade]]). O jogador pode **fixar** o detalhamento na tela.

### Barra de alertas

Uma faixa discreta mostra os **problemas mais urgentes** da cidade do jogador ("déficit de comida", "sem vagas", "Criminalidade alta"). Clicar vai ao local ou abre o painel. Também é um elemento do HUD ajustável.

---

## Acessibilidade

Mínimo desde o início:

- **Escala da interface** ajustável (100% a 200%).
- **Nenhuma informação importante só por cor:** sempre ícone ou texto também.
- **Atalhos remapeáveis.**
- **HUD personalizável** (ver acima).

O resto (legendas, modo para daltônicos) entra nas **configurações**, depois.

## Ajuda

- Uma **enciclopédia no jogo gerada dos próprios dados** (itens, construções, termos do Glossário, tecnologias), com links a partir dos tooltips. É barata de manter, porque nasce dos JSON.
- O **tutorial guiado** fica para depois.

## Menus fora do jogo

- **Menu principal**, **novo jogo em 3 passos** (estilo de início, narrador e personagem; o **tamanho das unidades** da batalha fica nas opções do novo jogo), carregar e salvar com slots, **autosave a cada estação** e opções.
- Estilos de início em [[Sistema de Início de Jogo/Descrição|Sistema de Início de Jogo]] e narradores em Eventos.

### Fonte, painel "Por quê?" e idioma (rodada V3-12)

- **Fonte:** **Noto Sans**. É grátis, tem licença livre e tem os acentos e os alfabetos de outros idiomas. Tamanho padrão de 14 px em 1600 × 900, com escala de 80% a 150%.
- **Painel "Por quê?":** uma classe por vez, em abas, com um resumo das 5 classes no topo.
- **Idioma:** IDs de texto desde o primeiro dia, no arquivo `textos/pt-BR.json`.
- **Notificações:** 15 tipos, cada um com Desligada, Diário, Ícone ou Pausa; os padrões seguem o Aviso de cada evento.

---

## Parte técnica

- **Como é feita:** a interface do jogo é desenhada pelo **nosso próprio desenhista**, o mesmo que desenha o mundo, com os **layouts em arquivos JSON**. O **Dear ImGui** fica só para as ferramentas de depuração (painel de eventos, chances, forçar tecnologia). Assim a interface do jogador fica igual ao resto, e as ferramentas de teste não custam arte.
- **Perfil do jogador:** layout do HUD, escala, atalhos e opções de notificação num arquivo próprio, separado do save.
- **Fatia vertical:** HUD básico (data e hora, velocidade, dinheiro, retrato do personagem), painel da cidade e inventário. O modo de edição do HUD pode ficar para depois.

### Ligação com outros sistemas

| Sistema | Ligação |
|---|---|
| Eventos | Notificações, diário, "Por quê?" |
| Felicidade | Painel "Por quê?" por classe |
| Combate | Seleção, barras de Vida, ordens e pausa |
| Mapa | Lentes, busca, Viagem Rápida, minimapa |
| Companheiros e Diálogo | Janela de conversa sem retrato (boneco ampliado 4x, nome, título e Opinião); painel do grupo e dos vinculados (ver [[Sistema de Companheiros e Diálogo/Descrição\|Sistema de Companheiros e Diálogo]]) |
| Reputação | Painel de Reputação com faixas e "Por quê?"; notificação de mudança de faixa; Opinião no painel do Importante (ver [[Sistema de Reputação/Descrição\|Sistema de Reputação]]) |
| Construção | Modo construção, lotes, fila de obras |
| Trabalho e Propriedade | Painel de trabalho, contratação, preços |
| Produção | Fila de ordens no painel da construção |
| Tecnologia e Magia | Aba de Pesquisa |
| Facção e Diplomacia | Aba de Facção (leis) e painel de Diplomacia com a chance de aceite e o porquê (ver [[Sistema de Diplomacia/Descrição\|Sistema de Diplomacia]]) |
| Guerra e Batalha | Painel de guerra (Placar, Cansaço, exércitos); telas da batalha: pré-batalha, posicionamento, batalha e resultado (ver [[Sistema de Batalha/Descrição\|Sistema de Batalha]]) |
| Religião | Calendário com os festivais; patrocinar e cancelar; cobertura do Templo no painel da cidade (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| Clima e Estações | Clima do dia no HUD; camada de clima; tom do dia e paleta das estações (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |
| Notícias | Categoria "Notícias"; quadro da Taverna; Crônica do mundo; mapa atualizado (ver [[Sistema de Notícias/Descrição\|Notícias]]) |
| Itens | Botões "Usar" e "Ler"; ícones de Viciado e Doente no retrato (ver [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]]) |
| Escravidão | Painel de trabalho dos escravos; botões "Libertar" e "Vender"; tratamento (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) |
| Início de Jogo | Novo jogo em 3 passos |
| Arquitetura | Desenhista próprio, JSON, Dear ImGui só para depuração |

### Perguntas em aberto

1. **HUD:** nenhum elemento fixo; janelas móveis (rodada V3-12).
2. **Os 15 tipos de notificação:** listar junto com os eventos da rodada de conteúdo C3.
3. **Fonte:** Noto Sans, 14 px (rodada V3-12).
4. **Painel "Por quê?":** uma classe por vez, com resumo (rodada V3-12).
5. **Idioma:** IDs de texto desde o início, em `textos/pt-BR.json` (rodada V3-12).
