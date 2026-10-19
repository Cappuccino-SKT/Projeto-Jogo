> Instruções para o assistente (Claude) em qualquer sessão deste projeto. Lido automaticamente no início de cada sessão.

## Antes de tudo

1. Ler a [[Retomada]] (onde paramos, como trabalhamos, as regras do código).
2. Rodar `git fetch` e conferir se o último pull request já está no `main`.
3. Para programar, ler só a ficha do marco da vez e as partes das fichas que ele usa (procurar com grep e ler o trecho, não o arquivo inteiro).
4. Não ler o [[Estado do Projeto]], a [[Rodada Pendente]] nem a pasta `Documentos/` sem necessidade.

## Economia de tokens (pedido do autor, 2026-10-19)

As sessões estavam caras. Obrigações:

- **Fazer só o que é importante para o desenvolvimento.** O que não ajuda o marco ou a rodada da vez fica para depois, sem perguntar.
- **Ler pouco:** grep e trechos, nunca o mesmo arquivo duas vezes; nada de varrer pastas ou documentos "para garantir".
- **Comandos com saída curta:** filtrar com `tail`, `grep` ou `head`; compilar e testar só o necessário, e o `ctest` completo só no fim do marco.
- **Sem experimentos extras** (várias sementes, anos a mais, builds paralelos) a não ser para responder uma dúvida que impeça o trabalho; se for grande, perguntar antes.
- **Sem subagentes.**
- **Documentar uma vez:** cada fato num lugar só (a ficha ou o `Jogo/LEIA-ME.md`); a Retomada só ganha uma linha de estado, e o que já foi resolvido sai dela.
- **Respostas curtas:** o resumo final em poucas linhas; sem repetir o que o autor já sabe.
- **Pode seguir na mesma sessão** de uma etapa para a outra (pedido do autor, 2026-10-19). A cada etapa nova, conferir se o modelo bate com a tabela abaixo e avisar o autor se não bater.

## Modelo e esforço (pedido do autor, 2026-10-19)

| Tarefa | Modelo e esforço |
|---|---|
| Programar um marco pelas fichas, corrigir testes, aplicar as respostas de uma rodada | **Sonnet, esforço médio** |
| Escrever fichas e rodadas de design; um erro difícil que o Sonnet não resolveu | **Opus, esforço médio** |
| Esforço alto | Só quando o autor pedir |

**Obrigação:** na primeira mensagem de cada sessão **e no começo de cada etapa nova** (uma rodada, uma ficha ou um marco), antes de qualquer outra coisa, conferir o modelo em uso (está nas instruções do sistema) e a tarefa pedida. Se não bater com a tabela, **avisar o autor em uma linha** ("Esta tarefa pede o Sonnet com esforço médio; troque no seletor de modelo do app e me mande de novo.") e **parar até ele responder**. Se ele mandar seguir assim mesmo, seguir. O esforço não aparece nas instruções: lembrar a tabela só quando o modelo estiver errado.

## Regras

- Sempre em **português do Brasil**, em linguagem simples: o autor tem pouca experiência em C++.
- **Documentação primeiro, código marco a marco** (FT2-1). Um problema de design achado no código vira pergunta numa rodada, e os documentos são atualizados.
- **Nomes no código:** português sem acento e **sempre descritivos** (FT2-4).
- **Pastas:** `Documentos/` → `Ficha Logica/` → `Codigo/` (sem janela) → `Jogo/` (programa). Um sistema vira ficha pouco antes do marco que o usa.
- Cada rodada ou marco vira **uma branch e um pull request**.
- Perguntas **concretas e completas**, no padrão da seção 2 da [[Retomada]], escritas na [[Rodada Pendente]].
