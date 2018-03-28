#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Macro magic. Touchy shit.
#define eprintf(args, ...) fprintf(stderr, args, ##__VA_ARGS__);
#define print_token(TOK)                                                       \
  print_token_name(stderr, TOK);                                               \
  fprintf(stderr, "%s (%d,%d)\n", TOK->name, TOK->line, TOK->start)
void ast_record_order(struct ast_node *parent, unsigned char is_child);

struct ast_node *create_ast_node() {
  struct ast_node *result = malloc(sizeof(struct ast_node));
  result->children = NULL;
  result->tokens = NULL;
  result->childcount = 0;
  result->tokencount = 0;
  result->ordering = NULL;
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
  ast_record_order(parent, 1);
}
void remove_child(struct ast_node *parent, int index) {}
void free_ast_node(struct ast_node *item) {
  if (!item)
    return;
  int i = 0;
  for (i = 0; i < item->childcount; i++) {
    free_ast_node(item->children[i]);
  }
  if (item->children)
    free(item->children);
  for (i = 0; i < item->tokencount; i++) {
    free_token(item->tokens[i]);
  }
  if (item->tokens)
    free(item->tokens);
  free(item->ordering);
  free(item);
}
#define clean_up_and_null                                                      \
  free_ast_node(result);                                                       \
  return NULL
struct ast_node *parse_file(struct tokenlist *start, struct tokenlist **end) {
  struct ast_node *result = create_ast_node();
  struct tokenlist *next;
  while (start&&start->token) {
    if (start->token->type == LEXER_FUNC_START) {
      struct ast_node *func = parse_function(start, &next);
      if(!func){eprintf("Failed to parse function\n");}
      ast_add_child(result, func);
      start=next;
    } else if (start->token->type == LEXER_LVAR ||
               start->token->type == LEXER_VAR) {
      struct ast_node *var = parse_variable(start, &next);
      ast_add_child(result, var);
      start=next;
    } else {
      eprintf(
          "Hmm... This isn't what I can recognize as a top level statement.\n");
      print_token(start->token);
      start = start->next;
    }
  }
  if (start&&start->token) {
    eprintf("parse_file did not consume all available tokens. This could be an "
            "issue\n");
    eprintf("Remaining token: "); print_token(start->token);
  }
  result->type=ast_file;
  return result;
}
struct ast_node *parse_variable(struct tokenlist *start,
                                struct tokenlist **end) {
  struct ast_node *result = create_ast_node();
  eprintf("parse variable called\n");
  if (start->token->type != LEXER_LVAR && start->token->type != LEXER_VAR) {
    eprintf("Why did this get parsed as a variable statement?\n");
    print_token(start->token);
    clean_up_and_null;
  }
  eprintf("Collected variable lexer token.\n");
  ast_add_token(result, start->token);
  start = start->next;

  if (start->token->type != LEXER_WORD) {
    eprintf("What... This isn't a valid variable name.\n");
    print_token(start->token);
    clean_up_and_null;
  }
//  ast_add_simple_node_from_token(result, start->token);
  ast_add_token(result,start->token);
  result->type = ast_variable;
  *end = start->next;
  eprintf("Finished parsing variable.\n");
  return result;
}
struct ast_node *parse_function(struct tokenlist *start,
                                struct tokenlist **end) {
  if (!start || !start->token)
    return NULL;
  struct tokenlist *current = start;
  struct ast_node *result = create_ast_node();
  if (current->token->type != LEXER_FUNC_START) {
    // Not necessarily an error, but he isn't a function jim.
    clean_up_and_null;
  }
  ast_add_simple_node_from_token(result,current->token);
  current = current->next;
  if (ast_add_token(result, current->token)) {
    eprintf("Failure to add token(this occurred when collecting the function "
            "name)\n");
  }
  eprintf("Function Name: ");print_token(current->token);
  current = current->next;
  if (current->next) {
    struct tokenlist *next;
    struct ast_node *body = parse_body(current, &next);
    if (!body) {
      eprintf("Failed to parse function body!\n");
      clean_up_and_null;
    }
    ast_add_child(result, body);
    if (current->next)
      current = next;
  }
  if (!current || !current->token) {
    eprintf("Error, function not completed(Maybe an EOF)!\n");
    clean_up_and_null;
  } else if (current->token->type != LEXER_FUNC_END) {
    eprintf("Error, function not closed.\n");
    eprintf("Found %s instead\n", current->token->name);
    if (current->next)
      eprintf("Next token was %s\n", current->next->token->name);
    clean_up_and_null;
  }
  ast_add_token(result,current->token);
  if (current->next)
    current = current->next;
  *end = current;
  result->type = ast_function;

  return result;
}
struct ast_node *parse_body(struct tokenlist *start, struct tokenlist **end) {
  struct ast_node *result = create_ast_node();
  struct tokenlist *current = start;
  eprintf("First token:");
  print_token(current->token);
  while (current && current->next && current->next->token &&
         /*current->next->token->type!=LEXER_FUNC_END
         &&current->next->token->type!=LEXER_THEN
         &&current->next->token->type!=LEXER_ELSE*/
         !LEXER_BLOCK_END(current->next->token->type)) {
    if (current->token->type == LEXER_VAR) {
      struct tokenlist *next;
      struct ast_node *potential_var = parse_variable(current, &next);
      eprintf("Noticed Lexer var\n");
      if (potential_var) {
        ast_add_child(result, potential_var);
        current = next;
      }else{
          eprintf("Something went wrong with the variable.\n");
          clean_up_and_null;
      }
    }else if (current->token->type == LEXER_IF) {
      struct tokenlist *next;
      struct ast_node *ifbody = parse_if(current, &next);
      if (!ifbody) {
        eprintf("Error, if did not parse properly.\n");
        clean_up_and_null;
      }
      ast_add_child(result, ifbody);
      current = next;
    }else if (LEXER_LOOP_START(current->token->type)) {
      struct tokenlist *next;
      struct ast_node *loop_body = parse_loop(current, &next);
      if (!loop_body) {
        eprintf("Error, loop did not parse properly.\n");
        clean_up_and_null;
      }
      ast_add_child(result, loop_body);
      current = next;
    }else{
//      print_token(current->token);
      if(current->token->type!=LEXER_WORD
         &&current->token->type!=LEXER_INT
         &&current->token->type!=LEXER_STRING
         &&current->token->type!=LEXER_DOUBLE)continue;
      ast_add_simple_node_from_token(result, current->token);
      if (current->next)
        current = current->next;
    }
  }
  if (!LEXER_BLOCK_END(current->token->type)) {
    ast_add_simple_node_from_token(result, current->token);
    *end = current->next;
  } else
    *end = current;
  result->type = ast_body;
  return result;
}
struct ast_node *parse_loop(struct tokenlist *start, struct tokenlist **end) {
  struct tokenlist *nextblock;
  struct ast_node *result = create_ast_node();
  if (ast_add_simple_node_from_token(result, start->token)) {
    eprintf("Failed to add token to node, how odd.\n");
    clean_up_and_null;
  }
  struct ast_node* body=parse_body(start->next, &nextblock);
  if(!body){
      eprintf("Failed to parse loop body\n");
      clean_up_and_null;
  }
  ast_add_child(result,body);
  if (!LEXER_LOOP_END(nextblock->token->type)) {
    eprintf("Failed to parse end of loop. Is it missing?\n");
    clean_up_and_null;
  }
  ast_add_simple_node_from_token(result, nextblock->token);
  *end = nextblock->next;
  switch(result->children[0]->type){
      case LEXER_FOR:
          result->type=ast_for;
          break;
      case LEXER_FOREACH:
          result->type=ast_foreach;
          break;
    case LEXER_BEGIN:
              result->type=ast_begin;
              break;
  }
  return result;
}
struct ast_node *parse_if(struct tokenlist *start, struct tokenlist **end) {
  struct tokenlist *nextblock;
  struct ast_node *result = create_ast_node();
  if (start->token->type != LEXER_IF) {
    clean_up_and_null;
  }
  ast_add_simple_node_from_token(result, start->token);
  struct ast_node *success_body = parse_body(start->next, &nextblock);
  if (success_body) {
    ast_add_child(result, success_body);
  } else {
    eprintf("Success body failed to parse\n");
    clean_up_and_null;
  }
  if (nextblock->token->type != LEXER_THEN) {
    if (nextblock->token->type != LEXER_ELSE) {
      eprintf("Failed to end if block. What a pity >.>\n");
      clean_up_and_null;
    } else {
      ast_add_simple_node_from_token(result, nextblock->token);
      struct ast_node *failbody = parse_body(nextblock->next, &nextblock);
      if (!failbody) {
        eprintf("Failed to parse body of else.\n");
        clean_up_and_null;
      }
      ast_add_child(result, failbody);
      if (nextblock->token->type != LEXER_THEN) {
        eprintf("Failed to find if clause termination.\n");
        clean_up_and_null;
      }
    }
  }
  if (nextblock->token->type == LEXER_THEN) {
    ast_add_simple_node_from_token(result, nextblock->token);
    *end = nextblock->next;
  } else
    *end = nextblock;
  result->type = ast_if;
  return result;
}
int ast_add_token(struct ast_node *parent, struct token *token) {
  if (!parent) {
    eprintf("Err, null ast_node passed to ast_add_token.\n");
    return 1;
  }
  if (!token) {
    eprintf("Error, null token passed to ast_add_token\n");
    return 1;
  }
  //  printf("DABOOG: %s\n",token->name);
  if (parent->tokens)
    parent->tokens = realloc(parent->tokens,
                                sizeof(void *) * (parent->tokencount + 1));
  else
    parent->tokens = malloc(sizeof(void *) * (parent->tokencount + 1));
  parent->tokens[parent->tokencount] = copy_token(token);
  parent->tokencount++;
  ast_record_order(parent, 0);
  return 0;
}
struct ast_node *ast_simple_node_from_token(struct token *token) {
  if (!token)
    return NULL;
  struct ast_node *result = create_ast_node();
  result->type = ast_simple;
  if (ast_add_token(result, token)) {
    eprintf("Error, issue with creating node from token.\n");
    clean_up_and_null;
  }
  return result;
}
int ast_add_simple_node_from_token(struct ast_node *parent,
                                   struct token *token) {

  struct ast_node *result = ast_simple_node_from_token(token);
  if (!result) {
    free_ast_node(result);
    return 1;
  }
  ast_add_child(parent, result);
  return 0;
}
void ast_node_type_to_string(char *stuffz, struct ast_node *node) {
    if(!node){
        sprintf(stuffz,"NULL>><:");
        return;
    }
  switch (node->type) {
  case ast_none:
    sprintf(stuffz, "None");
    break;
  case ast_file:
    sprintf(stuffz, "File");
    break;
  case ast_function:
    sprintf(stuffz, "Function");
    break;
  case ast_body:
    sprintf(stuffz, "Body");
    break;
  case ast_if:
    sprintf(stuffz, "If");
    break;
  case ast_for:
    sprintf(stuffz, "For");
    break;
  case ast_begin:
    sprintf(stuffz, "Begin");
    break;
  case ast_simple:
    sprintf(stuffz, "Simple");
    break;
  case ast_variable:
    sprintf(stuffz, "Variable");
    break;
  }
}
#define indent                                                                 \
  for (int j = 0; j < depth; j++)                                              \
    printf(" ");
#define indentTok                                                              \
  for (int j = 0; j < depth + 2; j++)                                          \
    printf(" ");
void ast_inner_print(struct ast_node *node, int depth) {
  char buffer[40];
  ast_node_type_to_string(buffer, node);
  indent int child_iter = 0;
  int token_iter = 0;
  int i = 0;
  if (depth > 0)
    indent printf("Node:%s,%p(C:%d,T:%d)\n", buffer, (void *)node,
                  node->childcount, node->tokencount);
  while (i < node->tokencount + node->childcount) {
    if (node->ordering[i] > 0) {
      //            printf("\n");
      ast_inner_print(node->children[child_iter], depth + 1);
      child_iter++;
    } else {
      indentTok printf("Token:");
      printf("%s %d,%d\n", node->tokens[token_iter]->name,
             node->tokens[token_iter]->line,
             node->tokens[token_iter]->start);
      token_iter++;
    }
    i++;
  }
}
void ast_print(struct ast_node *node) { ast_inner_print(node, 0); }
void ast_record_order(struct ast_node *parent, unsigned char is_child) {
  if (parent->ordering) {
    unsigned char *tmp = malloc(parent->childcount + parent->tokencount);
    for (int i = 0; i < parent->childcount + parent->tokencount - 1; i++) {
      tmp[i] = parent->ordering[i];
    }
    free(parent->ordering);
    parent->ordering = tmp;
  } else {
    parent->ordering = malloc(parent->childcount + parent->tokencount);
  }
  parent->ordering[parent->childcount + parent->tokencount - 1] = is_child;
}
