#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
struct position_stack {
  size_t *position;
  size_t capacity;
  size_t size;
};
struct position_stack *create_position_stack();
void push_position_stack(struct position_stack *s, size_t pos);
size_t pop_position_stack(struct position_stack *s);
size_t peek_position_stack(struct position_stack *s);
void free_position_stack(struct position_stack *s);
struct instruction instruction_from_i(int type);
void add_instruction(struct program *prog, struct instruction i) {
  if (!prog)
    return;
  if(!prog->bytecode){
      prog->bytecode=calloc(10,sizeof(struct instruction*));
      prog->bytecode_capacity=10;
  }
  if (prog->bytecode_size == prog->bytecode_capacity) {
    prog->bytecode = realloc(prog->bytecode, sizeof(struct instruction) *
                                                 prog->bytecode_capacity * 2);
    prog->bytecode_capacity *= 2;
  }
  //memcpy(&prog->bytecode[prog->bytecode_size], i, sizeof(struct instruction));
  prog->bytecode[prog->bytecode_size].type=i.type;
  prog->bytecode[prog->bytecode_size].data=i.data;
  prog->bytecode_size++;
}
#define COMPILER_STATE_IN_FUNC  1<<0
#define COMPILER_STATE_IN_VAR   1<<1
#define COMPILER_STATE_IN_LOOP  1<<2
#define BUILTIN_WORD_MATCH(T,V,S) \
  {\
      printf("Testing against %s\n",V);\
    if(!strcmp(T->name,V)){ \
      add_instruction(result,instruction_from_i(S));\
      printf("Found %s against %s\n",T->name,V);\
      break;}}
struct program *build(struct tokenlist *tl) {
  struct program *result = malloc(sizeof(struct program));
  memset(result, 0, sizeof(struct program));
  struct token* current_token=tl->token;
  struct position_stack *true_conditionals=create_position_stack(),
                        *false_conditionals=create_position_stack(),
                        *end_of_conditions=create_position_stack(),
                        *loops=create_position_stack();
  unsigned int state=0;
  unsigned char p_useful=0;
  while(current_token){
    struct instruction p;
    switch(current_token->type){
      case LEXER_INT:
        p.type=i_push_primitive;
        p.data.information=stack_ptr_from_rval(
            create_prim_int(atoi(current_token->name)));
        p_useful=1;
        break;
      case LEXER_DOUBLE:
        p.type=i_push_primitive;
        p.data.information=stack_ptr_from_rval(
            create_prim_double(atof(current_token->name)));
        p_useful=1;
        break;
      case LEXER_STRING:
        p.type=i_push_primitive;
        p.data.information=stack_ptr_from_rval(
            create_prim_string(current_token->name));
        p_useful=1;
        break;
/**
 * 1 2 > if 3 else 4 then
 * |push 1|
 * |push 2|
 * |gt    |
 * |if    |
 * |push 3|
 * |else  |
 * |push 4|
 * |then  |
 *
 * 1  |push 1      |
 * 2  |push 2      |
 * 3  |gt          |
 * 4  |jmp_not_if 7|
 * 5  |push 3      |
 * 6  |else        |
 * 7  |push 4      |
 * 8  |then        |
 *
 * 1 2 > if 3 then
 * |push 1|
 * |push 2|
 * |gt    |
 * |if    |
 * |push 3|
 * |then  |
 *
 * 1  |push 1      |
 * 2  |push 2      |
 * 3  |gt          |
 * 4  |jmp_not_if 6|
 * 5  |push 3      |
 * 6  |then        |
 * */
      case LEXER_IF:
        add_instruction(result,simple_instruction_from_type(i_jmp_not_if));
        push_position_stack(false_conditionals, result->bytecode_size-1)
        break;
      case LEXER_ELSE:
        {
          size_t instr_to_change=pop_position_stack(false_conditionals);
          //result->bytecode[instr_to_change].data.address=

        }          
        break;
      case LEXER_THEN:
        break;
      case LEXER_BEGIN:
        break;
      case LEXER_REPEAT:
        break;
      case LEXER_UNTIL:
        break;
      case LEXER_FOREACH:
        break;
      case LEXER_FOR:
        break;
      case LEXER_LVAR:
        break;
      case LEXER_VAR:
        break;
      case LEXER_WORD:
#define match(S,I)BUILTIN_WORD_MATCH(current_token,S,I);
        match("dup",i_dup);
        match("dupn",i_dupn);
        match("pop",i_pop);
        match("popn",i_popn);
        match("swap",i_swap);
        break;
    }
    if(p_useful){add_instruction(result,p);p_useful=0;}
    if(tl->next){
      tl=tl->next;
      current_token=tl->token;
    }else{
      current_token=NULL;
    }
  }
  return result;
}
#undef match
#undef BUILTIN_WORD_MATCH
struct position_stack *create_position_stack() {
  struct position_stack *result = malloc(sizeof(struct position_stack));
  result->position = calloc(sizeof(size_t), 10);
  result->capacity = 10;
  result->size = 0;
}
void push_position_stack(struct position_stack *s, size_t pos) {
  if (s->size == s->capacity) {
    s->capacity *= 2;
    s->position = realloc(s->position, s->capacity * sizeof(size_t));
  }
  s->position[s->size++] = pos;
}
size_t pop_position_stack(struct position_stack *s) {
  size_t r = s->position[s->size - 1];
  s->size--;
  return r;
}
size_t peek_position_stack(struct position_stack *s){
  return s->position[s->size-1];
}
void free_position_stack(struct position_stack *s){
    if(!s)return;
    free(s->position);
    free(s);
}
struct instruction instruction_from_i(int type){
  struct instruction r;
  r.type=type;
  return r;
}
struct stack_cell create_prim_int(int n){
  struct stack_cell result;
  result.type=t_int;
  result.data.number=n;
  return result;
}
struct stack_cell create_prim_double(double f){
  struct stack_cell result;
  result.type=t_float;
  result.data.fnumber=f;
  return result;
}
struct stack_cell create_prim_string(const char* data){
  struct stack_cell result;
  result.type=t_string;
  result.data.str=strdup(data);
  return result;
}
struct stack_cell *stack_ptr_from_rval(struct stack_cell n){
  struct stack_cell *result=malloc(sizeof(struct stack_cell));
  result->type=n.type;
  result->data=n.data;
  return result;
}
struct instruction simple_instruction_from_type(int t){
  struct instruction result;
  result.type=t;
  return result;
}
