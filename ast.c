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
void add_child(struct ast_node *parent, struct ast_node *child) {
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
  return NULL;
struct ast_node *parse_function(struct tokenlist *start,
                                struct tokenlist **end) {
  if (!start || !start->token)
    return NULL;
  struct ast_node *result = create_ast_node();
  if (start->token->type != LEXER_FUNC_START) {
    cleanupandnull
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
  parent->immediate[parent->immediatecount] = token;
  parent->immediatecount++;
  return 0;
}
struct ast_node *ast_simple_node_from_token(struct token *token) {
  if (!token)
    return NULL;
  struct ast_node *result = create_ast_node();
  result->type = ast_simple;
  ast_add_token(result, token);
  return result;
}
