> O que não tem marcação foi decidido pelo autor. **[Proposta]** = sugestão ainda não decidida.
>
> Reescrito em 2026-10-26 para o **estilo RimWorld** (rodada AR1). O guia antigo, da pixel art (Pixelorama, Resurrect 64), está no Git.

# Guia da Arte

## 1. O estilo em uma frase

Bonecos simples vistos de cima, como no RimWorld: **cabeça grande e redonda com um rosto simples, corpo de manequim, contorno escuro grosso, cores lisas e sombra leve**; as roupas, as armaduras, os chapéus e as armas vão por cima, em camadas. As referências estão em `referencias/`:

| Arquivo | O que mostra |
|---|---|
| `manequins_do_autor_gemini.jpg` | O corpo: 5 tons de pele, homem, mulher e criança, de perto e de cima |
| `roupas_do_autor_gemini.jpg` | Nobres, piqueiros, damas e camponeses, de perto e de cima |
| `manequim_oficial_homem.webp` e `manequim_oficial_mulher.webp` | **O manequim oficial** (2026-10-26, feito no ChatGPT): 4 tons de pele, nas 3 direções. É a forma de todos os bonecos |

## 2. As regras do desenho

1. **Proporção:** a cabeça tem cerca de **40% da altura** do boneco; os pés quase não aparecem.
2. **Rosto simples, numa camada à parte** (decisão do autor, 2026-10-26): olhos, sobrancelhas e boca com poucos traços, como no RimWorld. O rosto **não é desenhado no corpo**: é uma camada própria, por cima da cabeça, para poder ser trocado (vários rostos no mesmo corpo) e animado depois (piscar, falar, expressões). Só aparece na frente e no lado; nas costas e na visão de longe, não.
3. **Contorno:** quase preto (marrom muito escuro), grosso e do mesmo peso em todas as peças.
4. **Cor:** lisa, com **uma** sombra suave (mais escura embaixo e à direita); sem brilho forte, sem textura de foto.
5. **Luz:** vem de cima à esquerda, sempre.
6. **Cores das culturas:** as do [[Pipeline de Arte e Conteúdo/Descrição|Pipeline]] (Guerreira vermelho-sangue e ferro, Arcana roxo e azul-noite, e assim por diante).

## 3. As duas visões (rodada AR1)

|                | **De perto**                                         | **De longe**                                                    |
| -------------- | ---------------------------------------------------- | --------------------------------------------------------------- |
| Como se vê     | Em 3/4: o corpo inteiro, a roupa com detalhe         | De cima para baixo: quase só a cabeça (ou o chapéu) e os ombros |
| Para que serve | Ver as pessoas, a roupa, o trabalho, a luta de perto | Ver a quantidade de gente, as construções e as batalhas grandes |
| Construções    | A fachada                                            | O telhado                                                       |
| Animação       | Andar (pernas e braços), golpe, ações simples        | Quase nada: no máximo a arma se movendo na luta                 |

Entre as duas há uma **transição** de zoom. [Proposta] De perto até cerca de 1,5x; abaixo de 1,25x, de longe; entre 1,25x e 1,5x, uma imagem some enquanto a outra aparece.

**Cada peça é desenhada nas duas visões.** Exemplo: o chapéu de pena do nobre, de perto, é visto de frente; de longe, é um círculo com a pena por cima.

## 4. O formato dos arquivos

| Item                         | Regra                                                                                                                                                           |
| ---------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Formato                      | PNG com **fundo transparente**                                                                                                                                  |
| Boneco                       | Uma folha por peça e por visão: **3 colunas** (frente, costas, lado direito), cada célula com **128 × 128**                                                     |
| Encaixe                      | Em todas as peças, o boneco fica no **mesmo lugar** da célula: os pés na mesma linha, o centro da cabeça no mesmo ponto. É isso que faz a roupa servir no corpo |
| Tile do chão                 | **64 × 64**; os mapas continuam em tiles de 16 no Tiled (o jogo reduz)                                                                                          |
| Camadas                      | Corpo (sem rosto), **rosto**, roupa de baixo, roupa de cima, armadura, cabelo, chapéu, arma: **um arquivo para cada**. O rosto fica entre o corpo e o cabelo    |
| Rosto                        | Folha de **2 colunas** (frente e lado direito), 128 × 128, os traços no lugar exato da cabeça do manequim oficial; fundo transparente                           |
| Animação de perto [Proposta] | Andar: 4 quadros; golpe: 3 quadros; nas outras ações, 2 quadros; uma folha por ação, com as 3 direções nas linhas                                               |
| Nomes [Proposta]             | `peca_visao_cultura.png`, por exemplo `chapeu_pena_perto_autoritaria.png`                                                                                       |

## 5. Como gerar no ChatGPT sem perder a coerência

**A ferramenta (decisão do autor, 2026-10-26):** o **ChatGPT**, que manteve o boneco bem mais igual que o Gemini. **O problema de toda IA:** ela tende a criar um boneco novo a cada pedido. **A solução** tem três partes: (1) **anexar sempre a mesma imagem de referência**, (2) **colar sempre o mesmo bloco de estilo** e (3) pedir **edições** de uma imagem que já deu certo, no lugar de imagens novas.

### 5.1 Antes de começar

1. Anexe sempre o **manequim oficial** (`referencias/manequim_oficial_homem.webp` ou `manequim_oficial_mulher.webp`). Ele é a "forma" de todos os bonecos.
2. Use **uma conversa do ChatGPT por coleção** (por exemplo, "roupas da Coroa"). Na mesma conversa ele lembra melhor o que já fez.
3. Peça **uma peça por vez**.

### 5.2 O bloco de estilo (colar no começo de todo pedido)

```
ESTILO FIXO (não mude nada disto):
- Arte de jogo de simulação vista de cima, no estilo do RimWorld.
- Use EXATAMENTE o manequim da imagem anexada: a mesma forma, a mesma proporção (cabeça grande e redonda, cerca de 40% da altura), o mesmo tamanho e a mesma posição na célula. Não redesenhe o corpo.
- O manequim e as roupas vêm SEM rosto (a cabeça lisa): o rosto é desenhado à parte.
- Contorno marrom muito escuro, grosso e do mesmo peso em tudo.
- Cores lisas com uma única sombra suave embaixo e à direita; luz vindo de cima à esquerda. Sem textura de foto, sem brilho, sem degradê forte.
- Fundo magenta puro (#FF00FF), liso, SEM sombra no chão embaixo dos pés.
- Folha com 3 colunas na ordem: frente, costas, lado direito. Cada célula quadrada, do mesmo tamanho, com o boneco centralizado e os pés na mesma linha.
- Sem texto, sem números, sem moldura.
```

**Por que o fundo magenta:** a IA quase nunca entrega fundo transparente, e o branco se confunde com as golas e os tecidos claros. O magenta não aparece em roupa nenhuma, e o Krita o apaga com um clique (seção 6).

### 5.3 Os pedidos (sempre depois do bloco de estilo, com a imagem anexada)

**O manequim de outro tom ou tipo de corpo:**
```
Usando o manequim anexado, faça a mesma folha com pele [tom: muito clara / média / escura / muito escura] e corpo [de homem / de mulher com roupa de baixo bege / de criança, menor]. Visão DE PERTO (3/4).
```

**Uma roupa (de perto):**
```
Vista o manequim anexado com [descrição: gibão azul-escuro com bordados dourados, gola branca franzida e mangas bufantes listradas], no estilo da cultura [Coroa de Valdren: romano imperial, dourado e preto]. Mostre o manequim por baixo só onde a roupa não cobre. Visão DE PERTO, 3 colunas.
```

**A mesma roupa, de longe:**
```
Agora a MESMA roupa da imagem anterior, na visão DE LONGE: vista de cima para baixo, quase só a cabeça e os ombros, como as linhas de "cabeças vistas de cima" da referência. Mesmas cores, mesmo contorno, mesmas 3 colunas.
```

**A peça sozinha (para a camada):**
```
Agora desenhe SÓ a roupa da imagem anterior, sem o corpo e sem a cabeça, na mesma posição exata em cada célula, como se o manequim estivesse invisível. Fundo magenta.
```
*Esta é a parte mais difícil para a IA. Se não sair certo, use a imagem vestida e apague a pele no Krita (seção 6).*

**Um rosto (camada à parte):**
```
Desenhe SÓ um rosto simples para a cabeça do manequim anexado, sem a cabeça e sem o corpo: [olhos pequenos e redondos, sobrancelhas grossas, boca reta e séria]. Poucos traços, na cor do contorno, como os rostos do RimWorld. Folha com 2 colunas: frente e lado direito, com os traços no lugar exato onde ficariam na cabeça do manequim. Fundo magenta.
```
*Para as expressões (piscar, falar, raiva), peça edições do mesmo rosto: "Edite o rosto anterior: os olhos fechados. Não mude mais nada."*

**Uma construção:**
```
Uma [casa de camponês / ferraria / templo] da cultura [nome e estilo], no mesmo estilo de contorno e cor. Duas imagens lado a lado: DE PERTO (a fachada em 3/4) e DE LONGE (só o telhado, visto de cima). Fundo magenta.
```

**Para corrigir sem recomeçar:**
```
Edite a imagem anterior: [mude só a cor da capa para vermelho / deixe a gola menor]. Não mude mais nada.
```

### 5.4 Conferir antes de aceitar

- [ ] A cabeça e o corpo têm o mesmo tamanho e a mesma posição do manequim oficial?
- [ ] O contorno tem a mesma grossura das outras peças?
- [ ] A luz vem de cima à esquerda?
- [ ] As 3 colunas estão na ordem frente, costas, lado direito?
- [ ] A cabeça veio lisa (o rosto é outra camada), sem texto e sem chão?
- [ ] **Sem a sombra embaixo dos pés?** No manequim oficial ela veio (uma mancha magenta mais escura); o "Selecionar da cor" do Krita não a pega, e ela vira uma borda suja. Apague-a à mão ou peça "edite: tire a sombra do chão".

Se algo falhou, peça uma **edição** (5.3), não uma imagem nova.

## 6. Acertar no Krita

1. Abra a imagem e corte a folha em 3 células de 128 × 128 (Imagem → Tamanho da tela, e a ferramenta de corte com tamanho fixo).
2. **Tirar o magenta:** Selecionar → Selecionar da cor → clique no magenta → Delete. Salve como PNG com transparência.
3. **Encaixe:** ponha a folha do manequim oficial numa camada embaixo, com transparência, e mova a peça até assentar no corpo.
4. **Separar a camada:** se a peça veio vestida, apague a pele que aparece com a borracha, deixando só a roupa.

## 7. O que falta decidir

- O número de quadros de cada animação e os nomes de arquivo (marcados [Proposta] na seção 4).
- O ponto exato da transição entre as visões (seção 3).
