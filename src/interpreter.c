#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
struct position_stack {
  size_t *position;
  size_t capacity;
  size_t size;
};
struct block {
  enum { bs_if, bs_else, bs_begin, bs_for, bs_foreach } type;
  size_t position;
};
struct block_stack {
  struct block *stuff;
  size_t size, capacity;
};
struct block_stack *create_block_stack();
void push_block(struct block_stack *stack, int type, size_t pos);
struct block pop_block(struct block_stack *stack);
struct block peek_block(struct block_stack *stack);
void free_block_stack(struct block_stack* s);
struct position_stack *create_position_stack();
void push_position_stack(struct position_stack *s, size_t pos);
size_t pop_position_stack(struct position_stack *s);
size_t peek_position_stack(struct position_stack *s);
void free_position_stack(struct position_stack *s);
struct instruction instruction_from_i(int type);
void close_loop(struct program*,
        struct block,
        struct position_stack*);
void add_instruction(struct program *prog, struct instruction i) {
  if (!prog) return;
  if (!prog->bytecode) {
    prog->bytecode = calloc(100, sizeof(struct instruction *));
    prog->bytecode_capacity = 10;
  }
  if (prog->bytecode_size == prog->bytecode_capacity-1) {
    prog->bytecode = realloc(prog->bytecode, sizeof(struct instruction) *
                                                 prog->bytecode_capacity * 2);
    prog->bytecode_capacity *= 2;
  }
  // memcpy(&prog->bytecode[prog->bytecode_size], i, sizeof(struct
  // instruction));
  prog->bytecode[prog->bytecode_size].type = i.type;
  if(i.type==i_push_primitive)
  prog->bytecode[prog->bytecode_size].data.information = i.data.information;
  if(i.type==i_jmp
      ||i.type==i_jmp_if
      ||i.type==i_jmp_not_if)
    prog->bytecode[prog->bytecode_size].data.address=i.data.address;
  prog->bytecode_size++;
}
#define COMPILER_STATE_IN_FUNC 1 << 0
#define COMPILER_STATE_IN_VAR 1 << 1
#define COMPILER_STATE_IN_LOOP 1 << 2
#define BUILTIN_WORD_MATCH(T, V, S)                   \
  {                                                   \
    if (!strcmp(T->name, V)) {                        \
      add_instruction(result, instruction_from_i(S)); \
      break;                                          \
    }                                                 \
  }
struct program *build(struct tokenlist *tl) {
  struct program *result = malloc(sizeof(struct program));
  result->words=NULL;
  result->word_count=0;
  memset(result, 0, sizeof(struct program));
  struct token *current_token = tl->token;
  struct block_stack *blocks = create_block_stack();
  struct position_stack *unfilled_breaks=create_position_stack();
  unsigned int state = 0;
  unsigned char p_useful = 0;
  while (current_token) {
    struct instruction p;
    switch (current_token->type) {
      case LEXER_FUNC_START:
          {
              struct word w={.position=result->bytecode_size,.name=strdup(tl->next->token->name)};
              tl=tl->next;
              if(result->words)
                  result->words=realloc(result->words,sizeof(struct word)*(++result->word_count));
              else
                  result->words=malloc(sizeof(struct word)*(++result->word_count));
              result->words[result->word_count-1]=w;
              result->entry_point=w.position;
          }
        break;
      case LEXER_FUNC_END:
        if(blocks->size!=0){
        }
        add_instruction(result,simple_instruction_from_type(i_exit));
        break;
      case LEXER_INT:
        p.type = i_push_primitive;
        p.data.information =create_prim_int(atoi(current_token->name));
        p_useful = 1;
        break;
      case LEXER_DOUBLE:
        p.type = i_push_primitive;
        p.data.information =create_prim_double(atof(current_token->name));
        p_useful = 1;
        break;
      case LEXER_STRING:
        p.type = i_push_primitive;
        p.data.information = create_prim_string(current_token->name);
        p_useful = 1;
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
        push_block(blocks, bs_if, result->bytecode_size);
        add_instruction(result, simple_instruction_from_type(i_jmp_not_if));
        break;
      case LEXER_ELSE: {
        if (peek_block(blocks).type != bs_if) {
          // handle the error
        }
        struct block tmp = pop_block(blocks);
        push_block(blocks, bs_else, result->bytecode_size);
        add_instruction(result, simple_instruction_from_type(i_jmp));
        result->bytecode[tmp.position].data.address = result->bytecode_size;
      } break;
      case LEXER_THEN:
        if (peek_block(blocks).type == bs_if ||
            peek_block(blocks).type == bs_else) {
          struct block tmp = pop_block(blocks);
          result->bytecode[tmp.position].data.address = result->bytecode_size;
        }
        break;
      case LEXER_BEGIN:
        push_block(blocks, bs_begin, result->bytecode_size);
        break;
      case LEXER_REPEAT:
        if (peek_block(blocks).type == bs_begin ||
            peek_block(blocks).type == bs_foreach ||
            peek_block(blocks).type == bs_for) {
          struct block tmp = pop_block(blocks);
          add_instruction(result, simple_instruction_from_type(i_jmp));
          result->bytecode[result->bytecode_size - 1].data.address =
              tmp.position;
          close_loop(result,tmp,unfilled_breaks);
        }
        break;
      case LEXER_BREAK:
        push_position_stack(unfilled_breaks,result->bytecode_size);
        add_instruction(result,simple_instruction_from_type(i_break));
        break;
      case LEXER_CONTINUE:
        push_position_stack(unfilled_breaks,result->bytecode_size);
        add_instruction(result,simple_instruction_from_type(i_continue));
      case LEXER_UNTIL:
        break;
      case LEXER_FOREACH:
        break;
      case LEXER_FOR:
        add_instruction(result,simple_instruction_from_type(i_forpush));
        push_block(blocks,bs_for,result->bytecode_size);
        push_position_stack(unfilled_breaks,result->bytecode_size);
        add_instruction(result,simple_instruction_from_type(i_foriter));
        break;
      case LEXER_LVAR:
        break;
      case LEXER_VAR:
        break;
      case LEXER_WORD:
#define match(S, I) BUILTIN_WORD_MATCH(current_token, S, I);
        match("dup", i_dup);
        match("dupn", i_dupn);
        match("pop", i_pop);
        match("popn", i_popn);
        match("swap", i_swap);
        match("+", i_plus);
        match("-", i_minus);
        match("*", i_multiply);
        match("/",i_divide);
        match("^",i_power);
        match("pow", i_power);
        match("<", i_lt);
        match(">", i_gt);
        match("intostr",i_intostr);
        match("call", i_call);
        match("atoi",i_atoi);
        match("strtod",i_strtod);
        match("notify", i_notify);
        match("read",i_read);
        match("rot",i_rot);
        match("rotate",i_rotn);
        match("explode",i_explode);
        match("exit",i_exit);
        match("swap",i_swap);
        match("over",i_over);
        match("or",i_or);
        match("not",i_not);
        match("nip",i_nip);
        match("<=",i_lte);
        match(">=",i_gte);
        match("=",i_equal);
        match("!=",i_not_equal);
        match("split",i_split);
        match("and",i_and);
        match("rot",i_rot);
        match("rotate",i_rotn);
        match("depth",i_depth);
        match("{",i_mark);
        match("}",i_mark_end);
        match("strcat",i_strcat);
        break;
    }
    if (p_useful) {
      add_instruction(result, p);
      p_useful = 0;
    }
    if (tl->next) {
      tl = tl->next;
      current_token = tl->token;
    } else {
      current_token = NULL;
    }
  }
  free_position_stack(unfilled_breaks);
  free_block_stack(blocks);
  return result;
}
#undef match
#undef BUILTIN_WORD_MATCH
struct position_stack *create_position_stack() {
  struct position_stack *result = malloc(sizeof(struct position_stack));
  result->position = calloc(sizeof(size_t), 10);
  result->capacity = 10;
  result->size = 0;
  return result;
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
size_t peek_position_stack(struct position_stack *s) {
  return s->position[s->size - 1];
}
void free_position_stack(struct position_stack *s) {
  if (!s) return;
  free(s->position);
  free(s);
}
struct instruction instruction_from_i(int type) {
  struct instruction r;
  r.type = type;
  return r;
}
struct stack_cell create_prim_int(int n) {
  struct stack_cell result;
  result.type = t_int;
  result.data.number = n;
  return result;
}
struct stack_cell create_prim_double(double f) {
  struct stack_cell result;
  result.type = t_float;
  result.data.fnumber = f;
  return result;
}
struct stack_cell create_prim_string(const char *data) {
  struct stack_cell result;
  result.type = t_string;
  result.data.str = malloc(sizeof(struct shared_string)+strlen(data)+1);
  result.data.str->length=strlen(data);
  result.data.str->links=1;
  strcpy(result.data.str->str,data);
  return result;
}
struct stack_cell create_prim_invalid(const char* message){
  struct stack_cell result;
  result.type=t_invalid;
  if(message){
    result.data.str=malloc(sizeof(struct shared_string)+strlen(message)+1);
    result.data.str->links=1;
    result.data.str->length=strlen(message);
    strcpy(result.data.str->str,message);
  }else
    result.data.str=NULL;
  return result;
}
void print_stack_cell(struct stack_cell* sc){
  switch(sc->type){
    case t_int:
    case t_dbref:
      printf("INT:%i",sc->data.number);
      break;
    case t_float:
      printf("FLOAT:%f",sc->data.fnumber);
      break;
    case t_string:
      printf("STRING:%s",sc->data.str->str);
      break;
    case t_invalid:
      printf("INVALID:");
      if(sc->data.str){
          printf("%s",sc->data.str->str);
      }
      break;
    default:
      printf("OTHER");
      break;
  }
}
struct stack_cell *stack_ptr_from_rval(struct stack_cell n) {
  struct stack_cell *result = malloc(sizeof(struct stack_cell));
  result->type = n.type;
  result->data = n.data;
  return result;
}
struct instruction simple_instruction_from_type(int t) {
  struct instruction result;
  result.type = t;
  return result;
}
struct block_stack *create_block_stack() {
  struct block_stack *result = malloc(sizeof(struct block_stack));
  result->stuff = malloc(sizeof(struct block) * 10);
  result->capacity = 10;
  result->size = 0;
  return result;
}
void push_block(struct block_stack *stack, int type, size_t pos) {
  size_t s = stack->size;
  if (stack->size + 1 == stack->capacity) {
    stack->stuff =
        realloc(stack->stuff, stack->capacity * 2 * sizeof(struct block));
    stack->capacity *= 2;
  }
  stack->stuff[s].type = type;
  stack->stuff[s].position = pos;
  stack->size++;
}
struct block pop_block(struct block_stack *stack) {
  struct block r = stack->stuff[--stack->size];
  return r;
}

struct block peek_block(struct block_stack *stack) {
  struct block result;
  result = stack->stuff[stack->size - 1];
  return result;
}
void free_block_stack(struct block_stack* s){
  free(s->stuff);
  free(s);
}
const char* obtain_bytecode_name(char t){
  const char *inames[]={
    "i_push_primitive",
    "i_mark",
    "i_mark_end",
    "i_pop",
    "i_popn",
    "i_dup",
    "i_dupn",
    "i_depth",
    "i_plus",
    "i_minus",
    "i_divide",
    "i_power",
    "i_multiply",
    "i_increment",
    "i_jmp",
    "i_jmp_if",
    "i_jmp_not_if",
    "i_strcat",
    "i_strlen",
    "i_smatch",
    "i_strcmp",
    "i_equal",
    "i_gt",
    "i_lt",
    "i_gte",
    "i_lte",
    "i_not_equal",
    "i_not",
    "i_and",
    "i_or",
    "i_rot",
    "i_rotn",
    "i_over",
    "i_nip",
    "i_swap",
    "i_exit",
    "i_abort",
    "i_explode",
    "i_reverse",
    "i_split",
    "i_rsplit",
    "i_call",
    "i_foriter",
    "i_forpush",
    "i_forpop",
    "i_break",
    "i_continue",
    "i_notify",
    "i_read",
    "i_atoi",
    "i_strtod",
    "i_intostr"
  };
  return inames[t];
}
void print_bytecode(struct program* p){
  int max_opcode_name=0;
  for(int i=0;i<=i_intostr;i++){
    max_opcode_name=strlen(obtain_bytecode_name(i))>max_opcode_name?strlen(obtain_bytecode_name(i)):max_opcode_name;
  }
  printf("Address |%*s| argument\n",max_opcode_name,"Opcode");
  for(size_t i=0;i<p->bytecode_size;i++){
    struct instruction *current=&p->bytecode[i];
    printf("%-8zi|%-*s|",i,max_opcode_name,obtain_bytecode_name(current->type));
    if(current->type==i_push_primitive){
      switch(current->data.information.type){
        case t_string:
        printf("\"%s\"",current->data.information.data.str->str);
      break;
        case t_int:
        printf("%8i",current->data.information.data.number);
        break;
        case t_float:
        printf("%e",current->data.information.data.fnumber);
        break;
    }
    }else if(current->type==i_jmp
        ||current->type==i_jmp_if
        ||current->type==i_jmp_not_if
        ||current->type==i_foriter
        ||current->type==i_break
        ||current->type==i_continue){
      printf("%8zi",current->data.address);
    }
    printf("\n");
  }
}
void free_stack_cell(struct stack_cell sc){
  if((sc.type==t_invalid||sc.type==t_string)&&sc.data.str){
    if(!(--sc.data.str->links))
      free(sc.data.str);
  }
  sc.data.str=NULL;
  
}
void free_program(struct program** pr){
  struct program* p=*pr;
  for(size_t i=0;i<p->bytecode_size;i++){
    if(p->bytecode[i].type==i_push_primitive){
      free_stack_cell(p->bytecode[i].data.information);
    }
  }
  free(p->bytecode);
  free(*pr);
}
void close_loop(struct program* result,
        struct block tmp,
        struct position_stack* unfilled_breaks){
          while(unfilled_breaks->size>0){
              //See what loops existed at the time of the creation of these breaks
              size_t cur=peek_position_stack(unfilled_breaks);
              if(cur>=tmp.position){
                  pop_position_stack(unfilled_breaks);
                  if(result->bytecode[cur].type==i_break){
                    result->bytecode[cur].data.address=result->bytecode_size;
                  }else if(result->bytecode[cur].type==i_continue){
                      result->bytecode[cur].data.address=tmp.position-1;
                  }else if(result->bytecode[cur].type==i_foriter){
                    result->bytecode[cur].data.address=result->bytecode_size-1;
                  }
              }else
                  break;
          }
          if(tmp.type==bs_for){
            add_instruction(result,simple_instruction_from_type(i_forpop));
          }
}
struct stack_cell copy_stack_cell(struct stack_cell n){
  struct stack_cell copy;
  copy.type=n.type;
  switch(n.type){
    case t_string:
      copy.data.str=n.data.str;
      copy.data.str->links++;
      break;
    case t_int:
      copy.data.number=n.data.number;
      break;
    case t_float:
      copy.data.fnumber=n.data.fnumber;
      break;
    case t_address:
      copy.data.address=n.data.address;
      break;
     default:
      break;
  }
  return copy;
}
