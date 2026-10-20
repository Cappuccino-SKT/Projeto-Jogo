---
tags: [ficha-logica]
---

> Rascunho em construção. O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida. **[Em aberto]** = pergunta que precisa de decisão. **[Refinar depois]** = ideia aceita em linhas gerais, com detalhes a definir mais adiante.
>
> Ficha escrita em 2026-10-19 (fase D4 do [[Ficha Logica/Plano das Fichas|Plano das Fichas]]), com a **parte 10: Simulação de mesa**. Os pontos marcados com MG1 vêm da **rodada MG1**, decidida em modo automático (seguir a sugestão; parte 10.7).

# Ficha Lógica: Magia

## 1. O que faz

Alguns NPCs nascem **Magos**. O Mago tem **Mana** e a habilidade **Magia**, conjura **feitiços** no combate (sem falha, com conjuração, recarga e área que acerta aliados), usa **cajado**, trabalha na **Torre Arcana** (Pontos Arcanos), na **Encantaria** e na **Forja Arcana**, e conjura 3 feitiços de Suporte fora do combate. A **lei de Magia** de cada facção decide o custo social.

Documento do sistema: [[Sistema de Magia/Descrição|Magia]].

**Esta versão:** quem nasce Mago, a Mana, os feitiços de exemplo, o cajado, a pesquisa arcana, o encantamento, os 3 feitiços fora do combate, a lei de Magia e os Magos no cálculo rápido. **A lista de verdade dos feitiços, a Forja Arcana e o Esquadrão de Magos** ficam na parte 9 (conteúdo C2 e Batalha).

## 2. Dados

### `data/magia.json` (arquivo novo)

```json
{
  "nascer_mago_em_porcento": { "sem_pais_magos": 3, "sem_pais_magos_arcana": 10, "um_pai_mago": 25, "dois_pais_magos": 60 },
  "proporcao_inicial_em_porcento": { "arcana": 15, "outras": 5 },
  "mana_maxima": { "base": 50, "por_ponto_de_magia": 1 },
  "feiticos_conhecidos": { "base": 2, "a_cada_pontos_de_magia": 20 },
  "regeneracao_de_mana_por_hora_em_porcento": 2,
  "pedra_de_mana_recupera": 50,
  "evolucao_fora_da_afinidade_em_porcento": 50,
  "cajado": { "dano": [8, 12], "intervalo_em_ticks": 30, "alcance_em_tiles": 10, "mana_por_disparo": 5 },
  "feiticos": {
    "raio":          { "escola": "combate", "mana": 20, "conjuracao_em_ticks": 20, "recarga_em_ticks": 60,  "dano": [15, 25], "alvo": "um" },
    "bola_de_fogo":  { "escola": "combate", "mana": 35, "conjuracao_em_ticks": 30, "recarga_em_ticks": 120, "dano": [10, 16], "area_em_tiles": 2 },
    "lentidao":      { "escola": "combate", "mana": 15, "conjuracao_em_ticks": 20, "recarga_em_ticks": 100, "intervalo_do_alvo_em_porcento": 150, "dura_ticks": 200 },
    "cura":          { "escola": "suporte", "mana": 20, "conjuracao_em_ticks": 30, "recarga_em_ticks": 60,  "vida": 20 },
    "coragem":       { "escola": "suporte", "mana": 25, "conjuracao_em_ticks": 30, "recarga_em_ticks": 200, "medo": -20, "area_em_tiles": 4 },
    "bencao_da_colheita":  { "escola": "suporte", "mana": 50, "fazenda_em_porcento": 120, "dura_dias": 16, "por_cidade_por_estacao": 1 },
    "acalmar_tempestade":  { "escola": "suporte", "mana": 30, "pedras_de_mana": 3 },
    "curar_ferimentos":    { "escola": "suporte", "mana": 20, "vida_por_hora_em_porcento": 10 }
  },
  "encantamento": { "rank_minimo": 3, "mana": 50, "pedras_de_mana": 2, "bonus_em_porcento_por_rank": { "3": 15, "4": 20, "5": 25 } },
  "lei_de_magia": {
    "restrita": { "feitico_de_combate_na_cidade": "crime.agressao" },
    "proibida": { "conjurar_visto": "crime.magia", "gravidade": 3 },
    "magos_banidos": { "mago_na_cidade": "crime.magia", "gravidade": 3 },
    "privilegio_dos_magos": { "felicidade_dos_nao_magos": -5 },
    "so_magos_governam": { "felicidade_dos_nao_magos": -10 }
  },
  "calculo_rapido": { "mago_de_combate_dano_vezes": 2, "mago_de_suporte_perdas_em_porcento": 85 }
}
```

Os feitiços são **exemplos** ([Proposta]); a lista vem na rodada de conteúdo C2. O dano e a cura × Magia ÷ 50, como as armas.

### O NPC ganha

| Campo | Quem | Exemplo |
|---|---|---|
| `mago` | Todos (sim/não, no nascimento) | sim |
| `manaAtual` | Magos | 70 de 90 |
| `habilidadeMagia` (em centésimos) | Magos | 4.000 |
| `escolaDeAfinidade` | Magos (sorteada) | combate |
| `feiticosConhecidos` | Magos Controlados e Importantes | raio, cura |
| `recargas` | Em combate (não vai no save) | — |

A **Inteligência** do documento ainda não existe: a Mana e a quantidade de feitiços usam a habilidade Magia (MG1-2).

**Por facção:** `pontosArcanos`, `projetoArcano`, `feiticosPesquisados`; **por cidade:** a última Bênção da Colheita.

## 3. Quando roda

| Quando | O quê |
|---|---|
| Ao nascer um NPC | Mago ou não (4.1) |
| **A cada tick**, em combate | Conjuração, recarga, cajado (4.3) |
| **A cada hora** | Regeneração de Mana (4.2); a Torre Arcana produz (4.5) |
| **A cada dia, passo 7b** | Os Pontos Arcanos (como a Tecnologia) |
| Quando o jogador manda | Encantar (4.6); os 3 feitiços fora do combate (4.7) |

## 4. Regras passo a passo

### 4.1 Nascer Mago

1. **Os NPCs comuns não têm pais registrados.** A chance usa a **proporção de Magos adultos da cidade** (s), como se os pais fossem sorteados entre eles: chance = base × (1 − s)² + 25% × 2s(1 − s) + 60% × s².
2. **Base:** 3%; na cultura **Arcana**, 10% (MG1-1).
3. **Os filhos de Importantes e do jogador** (que têm pais registrados): 3% (ou 10%), 25% com um pai Mago, 60% com os dois.
4. **O migrante:** Mago com a chance da proporção da região de onde vem.
5. **O começo:** 15% dos NPCs da Arcana e 5% dos outros são Magos.

### 4.2 A Mana

- Máxima = 50 + Magia (Magia 40: 90).
- Regenera 2% da máxima por hora fora de combate, × 2 dormindo ou descansando (como a Vida).
- **Pedra de Mana** (usar item): +50.

### 4.3 No combate

1. **Conjurar:** o Mago gasta a Mana no início; conjura por `conjuracao_em_ticks`; se **levar dano**, a conjuração é interrompida e a Mana **não volta** (MG1-3). Terminou: o efeito acontece, sem falha.
2. **Dano e cura** × Magia ÷ 50; o dano passa pela Defesa como uma arma da classe "arco" (sem vantagem).
3. **Área:** acerta **todos** no raio, aliados também.
4. **Recarga:** o feitiço só volta depois de `recarga_em_ticks`.
5. **Cajado:** uma arma do Combate com dano × Magia ÷ 50; cada disparo gasta 5 de Mana; sem Mana, não dispara (troca para a secundária).
6. **A IA dos Magos** (os não Controlados): usa o feitiço de dano mais forte que a Mana paga, com 3 ou mais inimigos na área para a bola de fogo e nenhum aliado; senão, o cajado.
7. **Evolução:** a Magia sobe como as outras habilidades (Combate, 4.5); fora da escola de afinidade, pela metade.

### 4.4 No cálculo rápido

O Mago de **Combate** conta com o dano por tick × 2; cada Mago de **Suporte** do lado vencedor tira 15% das perdas dele (× 85%, até × 50%) (MG1-4).

### 4.5 A pesquisa arcana

- A **Torre Arcana** tem a receita "Pontos Arcanos", como o Laboratório (Tecnologia, TE1-2), mas só **Magos** trabalham nela, e a velocidade usa a habilidade Magia.
- Os pontos vão para a árvore arcana da facção (15 a 25 feitiços, com eras pelos marcos mundiais). Custo, difusão e escolha: os da [[Ficha Logica/Tecnologia|Tecnologia]].
- A Tecnocrata não pesquisa magia (a lei dela proíbe; não tem Torre).

### 4.6 O encantamento

Na **Encantaria**, um Mago de Ofício encanta um item de **Rank 3 ou mais**: gasta 50 de Mana e 2 Pedras de Mana, e o item ganha um bônus fixo (Dano, Defesa ou Atributo Mágico) de 15% no Rank 3, 20% no 4 e 25% no 5. Um encantamento por item; o novo substitui.

### 4.7 Os 3 feitiços fora do combate

Só os **Magos Controlados** (o jogador manda), nesta versão (MG1-5):

| Feitiço | Efeito | Limite |
|---|---|---|
| Bênção da Colheita | As Fazendas da cidade × 120% por 16 dias (um modificador de produtividade, como os eventos) | 1 por cidade por estação |
| Acalmar Tempestade | Troca a Tempestade de hoje da região por Chuva | 3 Pedras de Mana |
| Curar Ferimentos | O alvo regenera 10% da Vida por hora (como o Hospital com Mago) | — |

### 4.8 A lei de Magia

| Lei | O que acontece |
|---|---|
| Livre | Nada |
| Restrita | Um feitiço de combate dentro de uma cidade é Agressão (Crime), mesmo sem dano em ninguém |
| Proibida (Tecnocrata) | Conjurar visto por uma testemunha (Combate, 4.6) é o crime **Magia** (gravidade 3) |
| Magos banidos (extremo) | Um Mago na cidade é o crime Magia, achado na revista (Crime, 4.4) |
| Privilégio dos magos (Arcana) | Não Magos: −5 de Felicidade (na proporção deles na classe) |
| Só magos governam (extremo) | Não Magos: −10 |

## 5. Contas

**A proporção de Magos depois de muitas gerações**, pela regra 4.1 (o ponto em que a chance de nascer = a proporção):

| Base sem pais Magos | Equilíbrio |
|---|---|
| 2% (documento, V3-11) | **3,7%** (nem os 5% do mundo, nem os 15% da Arcana) |
| 3% (MG1-1) | **5,4%** |
| 10% na Arcana (MG1-1) | **15%** |

*A conta:* com s = 0,15 e base b: 0,15 = b × 0,7225 + 0,25 × 0,255 + 0,6 × 0,0225 → b = 10%.

## 6. Conversa com outros sistemas

| Sistema | O quê |
|---|---|
| População | O nascimento do Mago (4.1); o migrante |
| Combate | Feitiços, cajado, conjuração interrompida, área, a IA do Mago, o cálculo rápido (4.4) |
| Cura | A regeneração da Mana; o Curar Ferimentos; o Hospital com Mago de Suporte |
| Tecnologia | A pesquisa arcana com a mesma mecânica |
| Religião | Os Magos da Torre Arcana são a cobertura da Arcana |
| Crime | O crime Magia; a revista acha o Mago onde ele é banido |
| Cultura, Felicidade | A lei de Magia; os não Magos na Arcana |
| Clima | Acalmar Tempestade |
| Produção | A Bênção da Colheita; a Encantaria |
| Save | Mago, Mana, Magia, escola, feitiços, pontos arcanos; o formato passa para a **versão 17** |

## 7. Nomes no código

| Nome | O que é |
|---|---|
| `chanceDeNascerMago(cidade, pais)` | Regra 4.1 |
| `manaMaxima(npc)`, `regenerarMana(npc)` | Regra 4.2 |
| `comecarConjuracao(mago, feitico, alvo)`, `interromperConjuracao(mago)` | Regra 4.3 |
| `aplicarFeitico(mago, feitico, alvoOuArea)` | Regra 4.3 |
| `encantarItem(mago, item, atributo)` | Regra 4.6 |
| `conjurarForaDoCombate(mago, feitico, alvo)` | Regra 4.7 |

## 8. Testes

1. **Equilíbrio:** uma cidade Arcana com 15% de Magos, 20 gerações sem migração, fica entre 13% e 17%; uma de outra cultura, entre 4% e 7%.
2. **Conjuração interrompida:** o Mago leva dano no tick 10 da bola de fogo: sem efeito, Mana gasta.
3. **Área acerta aliados:** a bola de fogo com um companheiro no raio dá dano nele.
4. **Mana:** Magia 40 → 90; 2 bolas de fogo (70) e 4 disparos do cajado (20) esgotam a Mana.
5. **Lei:** o Mago do jogador conjura numa cidade Tecnocrata com uma testemunha: crime Magia, multa 225.
6. **O teste de 1 ano não muda:** a vila não tem facção nem Magos ativos (os NPCs da vila não nascem Magos no teste).

## 9. Fora desta versão

- A lista de verdade de feitiços e da árvore arcana (rodada de conteúdo C2).
- A Forja Arcana e o Aço Mágico (receitas do conteúdo).
- O Esquadrão de Magos (Batalha, D6).
- O Grimório (item e cópia na Torre); aprender com um mestre.
- A Inteligência; a Joia e o Anel de Mana; o Pó Espiritual na Mana.
- Os Magos da IA conjurando fora do combate (Bênção da Colheita nas cidades da Arcana).

## 10. Simulação de mesa

### 10.1 Cenário

O jogador (Espada, Corpo a Corpo 50, Leve) e uma companheira Maga de Combate (Magia 40, Mana 90, sem armadura, raio e bola de fogo) contra **6 bandidos** (Clava 40, sem armadura), o encontro da Viagem.

### 10.2 Os primeiros 150 ticks

| Tick | O que acontece | Mana |
|---|---|---|
| 0 | A Maga começa a bola de fogo (30 ticks) nos 3 bandidos da frente; o jogador segura os outros | 90 → 55 |
| 30 | **Bola de fogo:** 13 × 40 ÷ 50 = 10,4 em cada um dos 3 (sem aliado no raio) | 55 |
| 30 | Um bandido chega na Maga e começa a bater | 55 |
| 40 | A Maga começa um raio (20 ticks) | 55 → 35 |
| 60 | O bandido acerta a Maga no tick 55: **raio interrompido**, 20 de Mana perdidos | 35 |
| 60 a 150 | A Maga recua (Medo) e usa o cajado: 10 × 0,8 = 8 a cada 30 ticks | 35 → 20 |

A magia acelera o começo (31 de dano em área no tick 30), mas a Maga sem armadura é frágil (os bandidos tiram 8,4 por golpe dela). O jogador precisa proteger a Maga: é a pausa ativa que o documento quer.

### 10.3 O cálculo rápido do mesmo encontro

- Jogador: 12,5 ÷ 20 = 0,625 por tick. Maga de Combate: cajado 8 ÷ 30 = 0,267 × 2 = **0,533**. Soma 1,158; Vida 200; poder **231,7**.
- Bandidos contra a Leve (a mais comum; Defesa média (3 + 0) ÷ 2 = 1,5): 8,4 − 1,5 = 6,9 ÷ 30 = 0,23 × 6 = 1,38; Vida 600; poder **828**.
- Chance do grupo: 231,7 ÷ 1.059,7 = **22%** (sozinho, 8,8%).

### 10.4 Verificações

| Pergunta | Resultado |
|---|---|
| A proporção de Magos se mantém? | **Só com a MG1-1.** **Achado:** com a base de 2% do documento, as duas taxas (5% e 15%) caem para 3,7% em algumas gerações. A Arcana perderia o que a define |
| A magia é forte demais? | **Não:** a bola de fogo dá cerca de 31 de dano em área a cada 120 ticks, mais ou menos o que a Espada dá em 50 ticks contra 1 alvo. O preço é a Mana (2 bolas e acabou) e a fragilidade |
| A conjuração interrompida é justa? | É a regra do documento; a Mana gasta no início (MG1-3) dá peso a proteger o Mago |
| O teste de 1 ano muda? | **Não** |
| As Torres Arcanas têm Magos? | **Sim:** a Cidade Arcana de 100 NPCs tem cerca de 15 Magos; a Torre precisa de 5 |

### 10.5 Casos extremos

| Caso | O que acontece |
|---|---|
| Mana 0 e só o cajado | Não dispara; troca para a arma secundária (ou o soco) |
| A bola de fogo com o próprio Mago no raio | Acerta ele também |
| Uma cidade sem nenhum Mago adulto (s = 0) | Chance = a base (3% ou 10%): os Magos voltam |
| Encantar um item já encantado | O novo substitui o antigo; a Mana e as Pedras do antigo não voltam |
| Acalmar Tempestade num dia sem Tempestade | Não deixa conjurar ("Não há tempestade") |
| Bênção da Colheita duas vezes na estação | A segunda não é permitida |

### 10.6 Decidido em modo automático (rodada MG1, seguir sugestão)

1. **MG1-1:** a chance de nascer Mago sem pais Magos é 3% (10% na Arcana), para as proporções do documento (5% e 15%) ficarem estáveis; os comuns usam a proporção da cidade como pais.
2. **MG1-2:** a Mana e o número de feitiços usam a habilidade Magia até existir a Inteligência.
3. **MG1-3:** a Mana é gasta no começo da conjuração e não volta se ela for interrompida.
4. **MG1-4:** no cálculo rápido, o Mago de Combate vale 2 × o dano dele, e o de Suporte tira 15% das perdas do lado (até 50%).
5. **MG1-5:** os 3 feitiços fora do combate só pelos Magos Controlados, nesta versão.
6. **MG1-6:** os feitiços desta ficha são exemplos; a lista é conteúdo (C2).
