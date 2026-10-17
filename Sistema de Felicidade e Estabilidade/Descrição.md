> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.

## Sistema de Felicidade e Estabilidade

**Descrição geral**

A Felicidade era citada em vários documentos (Cidade, Cultura, Crime, IA, Eventos, População, Trabalho), mas nenhum definia a conta. Este documento reúne os fatores e transforma tudo em números.

São duas coisas separadas:

- **Felicidade:** o quanto as pessoas estão satisfeitas. Existe **uma por classe social** em cada cidade.
- **Estabilidade:** a Felicidade somada ao **Controle** (guardas, guarnição e Medo). É ela que decide se a **revolta** acontece.

Assim uma cidade pode estar infeliz e mesmo assim quieta, por repressão, o que dá peso à cultura Autoritária. Mas a repressão tem custo (ver "Controle").

---

## Felicidade

### Um valor por classe

- Cada cidade tem **5 valores de Felicidade, de 0 a 100, um por classe social** (Escravo, Civil, Rico, Nobreza e Realeza). As classes reagem de formas diferentes (ver "Faixas e reações").
- A **Felicidade da cidade** é a **média ponderada pela população** das cinco.
- **Individual** só para Controlados e Importantes (ver "Controlados e Importantes").

### Fatores

A **Felicidade-alvo** de cada classe é **50 mais as contribuições** abaixo. Cada fator tem um limite máximo de contribuição. Os valores são provisórios **[Refinar depois]**, para ajustar em testes.

| Fator | Contribuição (valores iniciais) | De onde vem |
|---|---|---|
| **Consumo atendido** | −25 a +5, conforme a classe (Comida, Materiais, Luxo, Exclusivos, Artefatos); falta de lenha no inverno, −10 | [[Sistema de Classe Social/Descrição\|Classe Social]]; estoques da cidade; [[Sistema de Clima e Estações/Descrição\|Clima e Estações]] |
| **Leis × cultura** | −30 a +10 | [[Sistema de Cultura/Descrição\|Cultura]] (ver abaixo) |
| **Criminalidade** | 0 a −15 | [[Sistema de Crime e Justiça/Descrição\|Crime e Justiça]] |
| **Desemprego e renda** | −10 a +10 | [[Sistema de Trabalho e Propriedade/Descrição\|Trabalho e Propriedade]] |
| **Moradia** | 0 a −10 (sem casa) | [[Sistema de População/Descrição\|População]] |
| **Conscrição alta** | 0 a −10 | [[Sistema de Facção/Descrição\|Facção]] |
| **Guerra e ameaça** | 0 a −15 (−Cansaço de guerra × 0,15) | [[Sistema de Guerra/Descrição\|Guerra]]; Eventos |
| **Imposto** | 0 a −10 | Lei de Imposto |
| **Eventos duradouros** | −20 a +20 | [[Sistema de Eventos/Descrição\|Eventos]] (festival, luto, peste...) |
| **Religião** | −5 a +10 | Cobertura do Templo × Devoção da cultura; os festivais entram como eventos duradouros (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| **Repressão** | 0 a −10, só Escravo e Civil | Controle (ver abaixo) |

O resultado é limitado entre 0 e 100.

**Guerra na cidade** (ver [[Sistema de Guerra/Descrição|Sistema de Guerra]]): ocupada −20; conquistada −15 (some em 1 ano); saqueada −30 por 1 ano (−40 se também ocupada); cerco com fome. São modificadores próprios, fora do limite dos eventos duradouros. A **superextensão** tira 3 de Estabilidade da facção por cidade conquistada há menos de 1 ano (ver [[Sistema de Diplomacia/Descrição|Sistema de Diplomacia]]).

### Inércia

- A Felicidade **não pula** para o alvo: a cada dia ela anda uma fração em direção a ele (10% por dia).
- Isso evita oscilação, dá tempo de o jogador reagir e combina com a histerese dos eventos.
- Em **LOD 2**, aplica-se o efeito acumulado de uma estação (16 dias) de uma só vez.

### Leis × cultura

- A contribuição é a soma, por lei, de **peso × distância** entre o valor da lei da facção e o ideal da cultura (ver [[Sistema de Cultura/Descrição|Sistema de Cultura]]).
- O **pensamento extremo respeitado** dá bônus grande; o **violado** dá penalidade grande.
- Em cidades com **várias culturas**, a conta usa a proporção de cada uma.
- **Minorias com afinidade hostil** sofrem uma penalidade extra (por exemplo, a discriminação aos Magos na cultura Arcana).
- **Lei de Tolerância religiosa:** com Perseguição, as minorias perdem 15 de Felicidade (na proporção delas na classe) e a cidade ganha 5 de Controle (ver [[Sistema de Religião/Descrição|Religião e Fé]]).
- **Clima:** calor forte (−2 no dia) e onda de calor (−5) entram como modificadores com causa (ver [[Sistema de Clima e Estações/Descrição|Clima e Estações]]).

---

## Faixas e reações

### Faixas de Felicidade

| Faixa | Valor | Efeito |
|---|---|---|
| **Contente** | acima de 70 | Produtividade levemente maior; atrai migrantes |
| **Estável** | 40 a 70 | Normal |
| **Inquieta** | 25 a 40 | Produtividade menor; pedidos de lei |
| **Protesto** | 10 a 25 | Greves e manifestações; mais crime |
| **Revolta** | abaixo de 10 | Risco de revolta (ver "Estabilidade e revolta") |

### Reação por classe

| Classe | Reação quando infeliz |
|---|---|
| **Escravo** | Fuga e rebelião local |
| **Civil** | Greve, protesto e migração |
| **Rico** | Empresas fecham ou saem da cidade |
| **Nobreza** | Pressiona mudança de lei e assembleia extraordinária |
| **Realeza** | Golpe ou crise de sucessão |

Essas reações são **eventos condicionais** (ver [[Sistema de Eventos/Descrição|Sistema de Eventos]]): têm duração mínima e histerese, e guardam a causa.

---

## Controle e Estabilidade

### Controle

- O **Controle** vem de **guardas**, **Esquadrões estacionados** e do **Medo** agregado da cidade (ver [[Sistema de IA/Descrição|Sistema de IA]]).
- Vale de 0 a 30.
- **Custo:** guardas e guarnição custam dinheiro (Banco da Cidade e da Facção) e **reduzem a Felicidade das classes baixas** (Escravo e Civil), menos nas culturas Autoritárias, que aprovam isso nas leis desejadas. Assim reprimir tem preço.

### Estabilidade e revolta

**Estabilidade = Felicidade ponderada + Controle − 10.**

- A **revolta** dispara quando a Estabilidade fica **abaixo de zero por alguns dias seguidos** (com histerese).
- Sem Controle, isso coincide com a faixa Revolta (Felicidade abaixo de 10).
- O Controle só segura a **revolta aberta**. As outras reações (protesto, fuga de escravos, empresas saindo, pressão de Nobres) seguem a Felicidade de cada classe.
- A resposta da facção à revolta continua sendo o **valor calculado** já definido (ver [[Sistema de Facção/Descrição|Sistema de Facção]]).

### Estabilidade da facção

- A facção tem uma **Estabilidade** própria: média ponderada das cidades mais a **legitimidade do líder** (lei de Poder do líder e personalidade).
- Estabilidade baixa dispara **assembleia extraordinária**, **crise de sucessão** ou **golpe**.

---

## Controlados e Importantes

A Felicidade individual do grupo do jogador usa **6 fatores**:

1. salário;
2. comida e descanso;
3. vitórias e derrotas;
4. companheiros mortos;
5. tratamento (por exemplo, ser Escravo);
6. as leis do lugar onde está.

Consequências:

- Abaixo de **20**, o jogador recebe um **aviso**.
- No **0**, o NPC **pede para sair**, mas só vai embora depois de alguns dias, o que dá chance de resolver com diálogo ou pagamento.
- A **Opinião** do companheiro muda essa regra: com +60 ou mais ele ainda fica 1 estação; com −60 ou menos pode desertar (ver [[Sistema de Companheiros e Diálogo/Descrição|Sistema de Companheiros e Diálogo]]).

---

## Transparência

- Toda mudança de Felicidade vinda de eventos entra como **modificador temporário com causa** (festival e luto, por exemplo, são eventos duradouros).
- O painel **"Por quê?"** mostra a Felicidade de cada classe com os **5 maiores fatores** e seus valores (por exemplo, "−12 Criminalidade, +8 Festival"). É a mesma transparência já prevista nos eventos.

---

## Parte técnica

- **LOD:** por cidade e por classe, uma vez por **dia** em LOD 0 e 1 e uma vez por **estação** em LOD 2, sem NPC a NPC.
- **Dados:** pesos e limites dos fatores, faixas e parâmetros do Controle em JSON.
- **Save:** Felicidade por classe e por cidade, Controle, contadores de dias em faixa, modificadores ativos e Estabilidade da facção.
- **Fatia vertical:** só os fatores de consumo, moradia e desemprego, sem revolta nem Controle.

### Ligação com outros sistemas

| Sistema | Ligação |
|---|---|
| Cidade | Felicidade por classe e Estabilidade como atributos |
| Cultura | Distância leis × cultura, pensamento extremo, religião, minorias |
| Classe Social | Consumo por classe; reações por classe |
| Crime e Justiça | Criminalidade reduz a Felicidade; desemprego e Felicidade influenciam a Criminalidade |
| Trabalho e Propriedade | Desemprego, renda e salários |
| População | Moradia, migração por Felicidade, natalidade |
| Facção | Conscrição, imposto, Poder do líder, Estabilidade da facção, resposta à revolta |
| Eventos | Faixas e Estabilidade como condições; modificadores com causa |
| IA | Felicidade dos Controlados; Medo agregado |
| NPC | Revolta do Escravo (Obediência e Felicidade) |
| Escravidão | Fuga e rebelião local; tratamento dos escravos (ver [[Sistema de Escravidão/Descrição\|Sistema de Escravidão]]) |
| Itens | Comida, bebida e narcóticos no fator "comida e descanso" dos Controlados; vício (ver [[Sistema de Itens/Efeitos de Itens\|Efeitos de Itens]]) |
| Combate | Esquadrões estacionados somam ao Controle |
| Religião | Fator Religião (−5 a +10); festivais; Perseguição das minorias (ver [[Sistema de Religião/Descrição\|Religião e Fé]]) |
| Clima e Estações | Lenha no inverno; calor forte e onda de calor (ver [[Sistema de Clima e Estações/Descrição\|Clima e Estações]]) |

### Perguntas em aberto

1. **Valores a testar:** contribuição de cada fator, velocidade da inércia, faixa do Controle, número de dias para a revolta, efeitos numéricos de cada faixa.
2. **Medo agregado:** como se calcula o Medo de uma cidade em LOD 2 e o que o aumenta (execuções, guarnição, derrotas).
3. **Fuga e rebelião local de Escravos:** resolvida no [[Sistema de Escravidão/Descrição|Sistema de Escravidão]] (fuga com Obediência abaixo de 30; rebelião com Felicidade dos Escravos abaixo de 10 e Controle abaixo de 10 por 4 dias).
4. **Greves e manifestações:** efeito exato na produção e na Criminalidade.
5. **Interface:** layout do painel "Por quê?" (as 5 classes de uma vez ou uma por vez; ver [[Interface/Descrição|Interface e Controles]]).
