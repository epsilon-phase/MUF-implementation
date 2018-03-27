#ifndef AST_H
#include "token.h"
struct ast_node{
    struct ast_node **children;
    struct token **immediate;
    int immediatecount;
    int childcount;
    int type;
};
struct ast_node* create_ast_node();
struct ast_node* parse_function(struct tokenlist *start);
void free_ast_node(struct ast_node*);
#endif
