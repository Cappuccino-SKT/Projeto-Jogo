> Instruções para o assistente (Claude) em qualquer sessão deste projeto. Lido automaticamente no início de cada sessão.

## Antes de tudo

1. Ler a [[Retomada]]: onde paramos, como trabalhamos e as regras do código. É curta de propósito.
2. Rodar `git fetch` e conferir se o último pull request já está no `main`.
3. Para programar, ler a [[Ficha Logica/Fatia Vertical|Ficha Lógica]], o marco da vez e a ficha de cada sistema que ele usa.
4. O [[Estado do Projeto]] tem a situação geral e os números-chave. Não ler a pasta `Documentos/Histórico/` sem necessidade.

## Regras

- Sempre em **português do Brasil**, em linguagem simples: o autor tem pouca experiência em C++.
- **Documentação primeiro, código marco a marco** (rodada FT2-1). Um problema de design achado no código vira pergunta numa rodada, e os documentos são atualizados.
- **Nomes no código:** português sem acento e **sempre descritivos**. O nome diz o motivo da variável, da função ou da classe; nada abstrato demais (pedido do autor, FT2-4).
- **Pastas:** `Documentos/` (o design) → `Ficha Logica/` (o design virado lógica de programação) → `Codigo/` (as regras em C++, sem janela) → `Jogo/` (o programa: janela, desenho, entrada). Um sistema vira ficha pouco antes do marco que o usa, não todos de uma vez.
- Cada rodada ou marco vira **uma branch e um pull request**.
- Manter a Retomada e o Estado **enxutos** (pedido do autor): o detalhe fica no documento de cada sistema, e o que já foi resolvido não volta para os arquivos de contexto.

## Padrão das perguntas

Pedido do autor: perguntas e sugestões **detalhadas, concretas e completas**, nunca abstratas. Cada pergunta traz:

- contexto;
- uma pergunta concreta, com números e um exemplo de jogo;
- opções com prós e contras;
- uma sugestão completa e pronta para usar;
- os conflitos com decisões anteriores.

O detalhe está na seção 2 da [[Retomada]].
