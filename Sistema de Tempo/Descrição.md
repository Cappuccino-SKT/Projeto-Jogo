
## Sistema de Tempo (Horário)

O jogo terá um ciclo de tempo anual. Cada ano tem um ciclo de 64 dias, dividido em 4 estações de 16 dias cada, representando as estações reais do ano: primavera, verão, outono e inverno.

Um dia tem 24 horas, sendo que 24 horas no jogo equivalem a 16 minutos reais na velocidade 1x. O jogo terá até 4 velocidades (1x, 2x, 4x, 8x), sendo a 8x exclusiva do Mapa Mundial. Nos Mapas Locais, Internos e de Guerra, o máximo é 4x. **Durante uma batalha no Mapa de Guerra, o relógio do mundo fica parado**; no fim, ele avança 2 horas fixas (ver [[Sistema de Batalha/Descrição|Sistema de Batalha]]).

A frequência de tick é amarrada ao LOD do Mapa Local, em vez de um valor fixo global: Mapas em LOD 0 rodam a cada tick "real" (frequência a definir com testes de performance). Mapas em LOD 1 rodam a cada N ticks. Mapas em LOD 2 avançam em blocos de tempo maiores (ex: a cada hora de jogo, não a cada tick). Isso também define o que pausa na velocidade 8x: pathfinding fino e decisão individual de Não-Controlados pausam; posição de Exércitos em trânsito e os agregados por cidade (Bens de Consumo/Felicidade) continuam avançando, fazendo 1 dia durar 2 minutos reais nessa velocidade.

### Atributos do Sistema de Tempo

- Ano (inicia com um valor correspondente à história do mundo)
- Estação
- Dia
- Hora
- Fase do dia (amanhecer, dia, entardecer, noite; derivada da hora)

### Períodos longos

Não existem meses no jogo. Períodos longos usam a **Estação** (16 dias) ou o **Ano** (64 dias). Por exemplo, manutenção e produção de construções em LOD 2 acontecem por estação, e a assembleia de leis das facções acontece uma vez por ano.

### Estações

As estações mudam a produção Braçal, o clima do dia, a viagem, a guerra e a cor do mapa; o inverno pede lenha. O dia tem 4 fases de luz (amanhecer, dia, entardecer e noite), e à noite a visão e a detecção caem 30%. Ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]. Os festivais de cada cultura têm datas fixas no calendário (ver [[Sistema de Religião/Descrição|Religião e Fé]]).

### Ligações com outros sistemas

- **Eventos:** os eventos ocasionais são sorteados uma vez por **dia** em mapas LOD 0 e 1 e uma vez por **estação** em LOD 2. As condições dos eventos condicionais são avaliadas por sinal ou em varredura diária ou sazonal (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]).
- **Combate:** quando um combate começa, o jogo reduz sozinho a velocidade para 1x. A pausa ativa vale durante o combate (ver [[Sistema de Combate/Descrição|Sistema de Combate]]).
- **Viagem:** atravessar um Mapa Local pelo Mapa Mundial leva 4 horas a pé, com multiplicadores; o grupo acampa à noite; encontros e chegadas voltam a velocidade para 1x (ver [[Sistema de Viagem/Descrição|Sistema de Viagem]]).
- **Clima e Estações:** produção por estação, clima sorteado 1 vez por dia por região, fases do dia (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).
- **Notícias:** as notícias andam 3 horas de jogo por mapa (ver [[Sistema de Notícias/Descrição|Notícias]]).
