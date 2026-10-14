
## Sistema de Tempo (Horário)

O jogo terá um ciclo de tempo anual. Cada ano tem um ciclo de 64 dias, dividido em 4 estações de 16 dias cada, representando as estações reais do ano: primavera, verão, outono e inverno.

Um dia tem 24 horas, sendo que 24 horas no jogo equivalem a 16 minutos reais na velocidade 1x. O jogo terá até 4 velocidades (1x, 2x, 4x, 8x), sendo a 8x exclusiva do Mapa Mundial.

A frequência de tick é amarrada ao LOD do Mapa Local, em vez de um valor fixo global: Mapas em LOD 0 rodam a cada tick "real" (frequência a definir com testes de performance). Mapas em LOD 1 rodam a cada N ticks. Mapas em LOD 2 avançam em blocos de tempo maiores (ex: a cada hora de jogo, não a cada tick). Isso também define o que pausa na velocidade 8x: pathfinding fino e decisão individual de Não-Controlados pausam; posição de Exércitos em trânsito e os agregados por cidade (Bens de Consumo/Felicidade) continuam avançando, fazendo 1 dia durar 2 minutos reais nessa velocidade.

### Atributos do Sistema de Tempo

- Ano (inicia com um valor correspondente à história do mundo)
- Estação
- Dia
- Hora

### Períodos longos

Não existem meses no jogo. Períodos longos usam a **Estação** (16 dias) ou o **Ano** (64 dias). Por exemplo, manutenção e produção de construções em LOD 2 acontecem por estação, e a assembleia de leis das facções acontece uma vez por ano.
### Eventos

Os eventos ocasionais são sorteados uma vez por **dia** em mapas LOD 0 e 1 e uma vez por **estação** em LOD 2. As condições dos eventos condicionais são avaliadas por sinal ou em varredura diária ou sazonal (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]).

### Combate

Quando um combate começa, o jogo reduz sozinho a velocidade para 1x. A pausa ativa vale durante o combate (ver [[Sistema de Combate/Descrição|Sistema de Combate]]).
