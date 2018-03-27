#ifndef AST_H
#include "token.h"
struct ast_node;
struct ast_if{
    struct ast_node **success_body,
                    **fail_body;
    unsigned int success_count;
    unsigned int fail_count;
};
struct ast_function{
    struct ast_node **children;
    const char* function_name;
    unsigned int childcount;
};
struct ast_file{
    struct ast_function **functions;
    unsigned int count;
};
struct ast_node{
    union{
        struct ast_if *ifbody;
        struct token *simple;
        struct ast_function *func;
        struct ast_file *file;
    } body;
    int type;
};
#endif
