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
char *read_file(const char* file);
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
  tokenlist_free(tokens);
  free_ast_node(func);
  const char* i3=": hello 1 15 1 for 1 + repeat ;";
  if(tokens=lexer(i3)){
      tokenlist_print(tokens);
  }
  func=parse_function(tokens,&func_end);
  ast_print(func);
  tokenlist_free(tokens);
  free_ast_node(func);
  if(tokens=lexer(": eee var e3 e3 @ var pec ;")){
      tokenlist_print(tokens);
  }
  func=parse_function(tokens,&func_end);
  tokenlist_free(tokens);
  ast_print(func);
  free_ast_node(func);
  printf("Attempting to parse file: bigger_test.muf\n");
  char* buffer=read_file("bigger_test.muf");
  printf("Read:\n");
  tokens=NULL;
  if(tokens=lexer(buffer)){
      tokenlist_print(tokens);
  }else{ fprintf(stderr,"Tokenizing bigger_test.muf failed :(\n");}
  func=parse_file(tokens,&func_end);
  if(!func){
      fprintf(stderr,"Whoa, that failed >.<\n");
  }
  tokenlist_free(tokens);
  ast_print(func);
  free_ast_node(func);
  free(buffer);

}
char *read_file(const char* fn){
    char *output;
    FILE *f=fopen(fn,"rb");
    if(!f){fprintf(stderr,"Failed to open file %s",fn);}
    fseek(f,0,SEEK_END);
    long long fsize=ftell(f);
    fseek(f,0,SEEK_SET);
    output=malloc(sizeof(char)*(fsize+1));
    fread(output,sizeof(char),fsize,f);
    fclose(f);
    output[fsize]=0;
    printf("File of %d bytes read.\n",fsize);
    return output;
}
