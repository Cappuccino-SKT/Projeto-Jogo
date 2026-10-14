> Nota do autor: esta descrição do projeto ainda não está finalizada. A maioria dos sistemas é abstrata — faltam muitas ideias e a formulação de sistemas mais robustos e específicos. Qualquer sugestão ou ajuda é bem-vinda.

> **Este arquivo agora é um índice.** A descrição completa de cada sistema fica **só na pasta do próprio sistema**, para existir uma única fonte da verdade. A versão anterior deste arquivo (uma cópia dos sistemas) foi substituída.

## Descrição do Projeto

Um jogo 2D de gerenciamento e RPG, com **engine própria em C++** e foco em **desempenho**: ele deve rodar com facilidade em qualquer PC.

- **Inspiração de jogabilidade:** RimWorld, Kenshi, Total War, Mount & Blade, The Guild, Songs of Syx e Sword & Banner.
- **Inspiração gráfica:** Songs of Syx, Sword & Banner, Romestead, Dimraeth e Stardew Valley.
- Toda a parte gráfica é feita à mão, sem nada procedural ou aleatório.

## Sistemas

### Mundo

- [[Sistema de Mapa/Descrição|Sistema de Mapa]] — Mundial, Local, Interno e de Guerra
- [[Sistema de Cidade/Descrição|Sistema de Cidade]]
- [[Sistema de Recursos/Descrição|Sistema de Recursos]] e [[Sistema de Recursos/Recursos Existentes|Recursos Existentes]]
- [[Sistema de Tempo/Descrição|Sistema de Tempo]]

### Pessoas e política

- [[Sistema de NPC/Descrição|Sistema de NPC]]
- [[Sistema de Classe Social/Descrição|Sistema de Classe Social]]
- [[Sistema de IA/Descrição|Sistema de IA]]
- [[Sistema de Cultura/Descrição|Sistema de Cultura]] — 6 culturas iniciais
- [[Sistema de Facção/Descrição|Sistema de Facção]] — leis, assembleia, diplomacia, exércitos
- [[Sistema de Crime e Justiça/Descrição|Sistema de Crime e Justiça]]
- [[Sistema de Cura/Descrição|Sistema de Cura]]
- [[Sistema de Combate/Descrição|Sistema de Combate]] — combate local do jogador, armas, derrota e captura
- [[Sistema de Eventos/Descrição|Sistema de Eventos e Narrador]] — eventos ocasionais e condicionais, narrador

### Economia e produção

- [[Sistema Econômico/Descrição|Sistema Econômico]]
- [[Sistema de Itens/Descrição|Sistema de Itens]]
- [[Sistema de Inventário/Descrição|Sistema de Inventário]]
- [[Sistema de Produção/Descrição|Sistema de Produção]]
- [[Sistema de Construção/Descrição|Sistema de Construção]]

### Jogo e engine

- [[Sistema de Início de Jogo/Descrição|Sistema de Início de Jogo]]
- [[Arquitetura/Descrição|Arquitetura]] — decisões técnicas e fatia vertical

## Próximos sistemas a escrever

Em ordem de prioridade:

1. **Tecnologia e Pesquisa**
2. **Magia** — inclui encantamento e feitiços
3. **Objetivo do jogo e progressão** — inclui masmorras e missões longas
4. **Comércio civil e caravanas**

## Primeira meta de desenvolvimento

Uma **fatia vertical**: 1 cidade, 1 cadeia de produção (Fazenda → Cozinha → Mercado), NPCs com rotina, consumo e Felicidade, 1 personagem controlável e sem guerra. Detalhes na [[Arquitetura/Descrição|Arquitetura]].

## Estado atual

O resumo do que já foi decidido e do que está em aberto está em [[Estado do Projeto]].
