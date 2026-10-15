## Sistema de Inventário

**Descrição geral**

O inventário guarda quais itens estão armazenados, a quantidade de cada um, o dinheiro e outras informações relevantes. No código ele é um **componente** reaproveitado por quem precisa dele (composição, ver [[Arquitetura/Descrição|Arquitetura]]), não uma cadeia de herança. Abaixo, o que cada dono de inventário acrescenta ao básico.

- **Inventário NPC geral.** Acrescenta o sistema de equipamento, que permite usar os equipamentos que estão no inventário do NPC.
- **Inventário NPC jogador (Controlado).** É o inventário com mais funcionalidade, por ser o principal ponto de interação do jogador. Acrescenta a parte gráfica e um limite de **peso**: a quantidade de itens que ele carrega depende da Capacidade de peso, influenciada pela Força do NPC.
- **Inventário da Cidade.** Passa informações para outros sistemas, como o Sistema de Eventos, para causar eventos nas cidades e nas regiões (por exemplo, dias em déficit de consumo; ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]). Cada cidade está vinculada a N inventários de construções que estejam dentro dela.
- **Inventário de Construção.** Tem um sistema de limitação: a capacidade muda conforme o tipo de construção. Cada inventário de construção está vinculado a apenas um inventário de cidade, e passa a ele as informações do que há dentro, para o Sistema de Eventos funcionar.
- **Inventário da Caravana.** A Caravana Militar guarda Ração Militar, materiais de manutenção de equipamento e munição do exército (ver Sistema de Facção). A Caravana Civil carrega mercadorias por peso (ver [[Sistema de Comércio/Descrição|Sistema de Comércio]]). O Esquadrão tem Equipamento próprio, que não é um inventário de NPC.
