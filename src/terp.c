#include <ctype.h>
#include <libgccjit.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast.h"
#include "options.h"
#include "interpreter.h"
#include "frame.h"
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
    struct options* options=calloc(sizeof(struct options),1);
    options->files=NULL;
 for(int i=1;i<argc;i++){
     if(args[i][0]!='-'){
//         if(options->files)
           options->files=realloc(options->files,sizeof(void*)*(options->filecount+1));
//         else
//             options->files=malloc(sizeof(char*));
       options->files[options->filecount]=malloc(strlen(args[i])+1);
       strcpy(options->files[options->filecount],args[i]);
       options->filecount++;
     }else if(!strcmp(args[i],"-tokens")){
         options->print_tokens=1;
     }else if(!strcmp(args[i],"-parse")){
         options->print_ast=1;
     }
 }/*
 if(options->filecount==0){
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
 for(int i=0;i<options->filecount;i++){
    
     char* buffer=read_file(options->files[i]);
     if(!buffer)continue;
     struct tokenlist* tokens=lexer(buffer),*end;
     if(tokens&&options->print_tokens)tokenlist_print(tokens);
     struct ast_node* file=parse_file(tokens,&end);
     if(file&&options->print_ast)ast_print(file);
     free_ast_node(file);
     tokenlist_free(tokens);
     free(buffer);
 }*/
 for(int i=0;i<options->filecount;i++){
     free(options->files[i]);
 }
 free(options->files);
 free(options);
 //struct tokenlist *test=create_tokenlist();
 //test->token=create_populated_token("dup",LEXER_WORD);
 //append_token(test,create_populated_token("pop",LEXER_WORD));
 struct tokenlist *test=lexer("1 if 2 else 4 then dup");
 struct program* prog=build(test);
 printf("bytecode opcode size: %zd\n",sizeof(struct instruction));
 print_bytecode(prog);
 tokenlist_free(test);
 test=lexer("1 2 * if 3 3 + 4 > if 2 then 5 / then +");
 tokenlist_print(test);
 free_program(&prog);
 prog=build(test);
 print_bytecode(prog);
 tokenlist_free(test);
 free_program(&prog);
 test=lexer("1 1 4 5 for + repeat");
 prog=build(test);
 print_bytecode(prog);
 free_program(&prog);
 tokenlist_free(test);
 test=lexer("1 1 4 5 for + dup 2 % 0 = if break else continue then repeat");
 prog=build(test);
 print_bytecode(prog);
 free_program(&prog);
 tokenlist_free(test);
 printf("\x1b[2J");
 test=lexer("1 1 5 1 for + dup + repeat 1 ");
 prog=build(test);
 print_bytecode(prog);
 struct frame f=create_frame(prog,NULL,NULL);
 execute_program(&f);
 free_frame(&f);

 free_program(&prog);
 tokenlist_free(test);
}
char *read_file(const char* fn){
    char *output=NULL;;
    FILE *f;
    int seat_of_the_pants=0;
        seat_of_the_pants=1;
        f=fopen(fn,"rb");
    if(!f){
        fprintf(stderr,"Failed to open file %s",fn);
    }else{
    fseek(f,0,SEEK_END);
    long long fsize=ftell(f);
    fseek(f,0,SEEK_SET);
    output=malloc(sizeof(char)*(fsize+1));
    memset(output,0,fsize+1);
    fread(output,sizeof(char),fsize,f);
    fclose(f);
    output[fsize]=0;
    printf("File of %lld bytes read.\n",fsize);
    }
    return output;
}
