#include <ctype.h>
#include <libgccjit.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast.h"
/**
 *  Inst type Prototype(since the JIT doesn't make it easy to get a normal C
 *struct in there)
 *struct variable;
 * struct inst{
 *  int type;
 *  union{
 *    int number;
 *    double fnumber;
 *    const char* string;
 *    variable* var;
 *  } data;
 *};
 * struct variable{
 *  inst *value;
 *  const char* name;
 * };
 * struct scope{
 *  scope * parent;
 *  int vcount;
 *  variable** vars;
 *  int scount;
 *  scope **scopes;
 *}
 */
#define C_INT_T 1
#define C_DOUBLE_T 2
#define C_STRING_T 3
#define MAX_STACK_DEPTH 32

int main(int argc, const char** args) {
  const char* i = ": hello \"fun boy\" 123 ; \n : 5 5 5 ; : abcd if 2 then ; : ebcd ++ ;";
  struct tokenlist* tokens = NULL;
  if((tokens=lexer(i))){
  tokenlist_print(tokens);
  tokenlist_free(tokens);
  }
  printf("\n\nSecond thingy\n");
  const char *i2=": hello goodbye if eeee else 2 then ;";
  if((tokens=lexer(i2))){
      tokenlist_print(tokens);
  }
  struct tokenlist *func_end;
  struct ast_node* func=parse_function(tokens,&func_end);
  ast_print(func);
  free_ast_node(func);
  tokenlist_free(tokens);
}
