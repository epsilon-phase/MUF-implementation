#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define eprintf(args...) fprintf(stderr, args...);
struct ast_node *create_ast_node() {
  struct ast_node *result = malloc(sizeof(struct ast_node));
  result->children = NULL;
  result->immediate = NULL;
  result->childcount = 0;
  result->immediatecount = 0;
  result->type = -1;
  return result;
}
void ast_add_child(struct ast_node *parent, struct ast_node *child) {
  if (!parent)
    return;
  if (parent->children) {
    parent->children = realloc(parent->children, sizeof(struct ast_node *) *
                                                     (parent->childcount + 1));
  } else {
    parent->children = malloc(sizeof(struct ast_node *));
  }
  parent->children[parent->childcount] = child;
  parent->childcount++;
}
void remove_child(struct ast_node *parent, int index) {}
void free_ast_node(struct ast_node *item) {
  if (!item)
    return;
  int i = 0;
  for (i = 0; i < item->childcount; i++) {
    free_ast_node(item->children[i]);
  }
  for (i = 0; i < item->immediatecount; i++) {
    free_token(item->immediate[i]);
  }
  free(item);
}
#define cleanupandnull                                                         \
  free_ast_node(result);                                                       \
  return NULL
struct ast_node *parse_function(struct tokenlist *start,
                                struct tokenlist **end) {
  if (!start || !start->token)
    return NULL;
  struct tokenlist * current=start;
  struct ast_node *result = create_ast_node();
  if (current->token->type != LEXER_FUNC_START) {
      //Not necessarily an error, but he isn't a function jim.
    cleanupandnull;
  }
  current=current->next;
  if(ast_add_token(result,current->token)){
      fprintf(stderr,"Failure to add token(this occurred when collecting the function name)\n");
  }
  current=current->next;
  if(current->next){
      struct tokenlist *next;
      struct ast_node* body=parse_body(current->next,&next);
      if(!body){
          fprintf(stderr,"Failed to parse function body!\n");
          cleanupandnull;
      }
      ast_add_child(result,body);
      current=next;
  }
  if(!current->token){
      fprintf(stderr,"Error, function not completed(Maybe an EOF)!\n");
      cleanupandnull;
  }else if(current->token->type!=LEXER_FUNC_END){
      fprintf(stderr,"Error, function not closed.\n");
      cleanupandnull;
  }
  current=current->next;
  *end=current;

  return result;
}
struct ast_node *parse_body(struct tokenlist* start,
                            struct tokenlist** end){
    struct ast_node* result=create_ast_node();
    struct tokenlist *current=start;
    while(current&&current->next&&current->next->token&&
            current->next->token->type!=LEXER_FUNC_END
            &&current->next->token->type!=LEXER_THEN
            &&current->next->token->type!=LEXER_ELSE){
        if(current->token->type==LEXER_IF){
            struct tokenlist *next;
            struct ast_node* ifbody=parse_if(current,&next);
            if(!ifbody){
                fprintf(stderr,"Error, if did not parse properly.\n");
                cleanupandnull;
            }
            current=next;
        }
        current=current->next;
    }
    *end=current->next;
    return result;
}
struct ast_node* parse_if(struct tokenlist *start,
          struct tokenlist**end){
    struct tokenlist *nextblock;
    struct ast_node* result=create_ast_node();
    if(start->token->type!=LEXER_IF){
        cleanupandnull;
    }
    struct ast_node* success_body=parse_body(start->next,&nextblock);
    if(success_body){
        ast_add_child(result,success_body);
    }else{
        fprintf(stderr,"Success body failed to parse\n");
        cleanupandnull;
    }
    if(nextblock->token->type!=LEXER_THEN){
        if(nextblock->token->type!=LEXER_ELSE){
            fprintf(stderr,"Failed to end if block. What a pity >.>\n");
            cleanupandnull;
        }else{
            struct ast_node* failbody=parse_body(nextblock->next,&nextblock);
        }
    }
    return result;

}
int ast_add_token(struct ast_node *parent, struct token *token) {
  if (!parent) {
    fprintf(stderr, "Err, null ast_node passed to ast_add_token.\n");
    return 1;
  }
  if (!token) {
    fprintf(stderr, "Error, null token passed to ast_add_token\n");
    return 1;
  }
  parent->immediate = realloc(parent->immediate, parent->immediatecount + 1);
  parent->immediate[parent->immediatecount] = copy_token(token);
  parent->immediatecount++;
  return 0;
}
struct ast_node *ast_simple_node_from_token(struct token *token) {
  if (!token)
    return NULL;
  struct ast_node *result = create_ast_node();
  result->type = ast_simple;
  if(ast_add_token(result, token)){
      fprintf(stderr,"Error, issue with creating node from token.\n");
  }
  return result;
}
