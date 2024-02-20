**Parser baserad på grammatik som matchar given text med hjälp av ett program**

EBNF för språket
  \<program> := \<expr>
  <expr> := <text>[<expr>] | <group>[<expr>] | <or>[<expr>] | <many>[<expr>] | <any>[<expr>] | <counter>[<expr>]
  <group> := (<expr>)
  <or> := <expr>+<expr>
  <many> := <expr>*
  <any> := .
  <counter> := <expr>{<number>}
  <text> := <char>[<text>]
  <char> := a-z|A-Z|0-9
