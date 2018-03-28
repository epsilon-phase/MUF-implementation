#ifndef AST_H
#include "token.h"
enum ast_node_type{
    ast_none=-1,
    ast_file=1,
    ast_function,
    ast_body,
    ast_if,
    ast_for,
    ast_foreach,
    ast_begin,
    ast_simple,
    ast_variable
};
struct ast_node{
    struct ast_node **children;
    struct token **tokens;
    int tokencount;
    int childcount;
    int type;
    unsigned char *ordering;
};
struct ast_node* create_ast_node();
struct ast_node* parse_file(struct tokenlist *start, struct tokenlist **end);
struct ast_node* parse_variable(struct tokenlist *start,struct tokenlist **end);
/**
 * Attempt to parse a function. Places the last position in the token list in the end
 * parameter.
 * Returns null if the function fails to parse the parameters, in this case,
 * the end parameter is not modified.
 */
struct ast_node* parse_function(struct tokenlist *start,struct tokenlist **end);
struct ast_node* parse_body(struct tokenlist* start,struct tokenlist**end);
struct ast_node* parse_loop(struct tokenlist* start,struct tokenlist **end);
struct ast_node* parse_if(struct tokenlist* start, struct tokenlist **end);
void free_ast_node(struct ast_node*);
int ast_add_token(struct ast_node* parent,struct token* token);
struct ast_node* ast_simple_node_from_token(struct token *token);
int ast_add_simple_node_from_token(struct ast_node* parent,struct token *token);
void ast_print(struct ast_node* node);
#endif
