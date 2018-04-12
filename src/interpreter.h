#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "token.h"
#include "ast.h"
#ifndef MAX_VAR_COUNT
#define MAX_VAR_COUNT 32
#endif
#ifndef MAX_LVAR_COUNT
#define MAX_LVAR_COUNT 32
#endif
struct word{
  size_t position;
  const char* name;
};
struct stack_cell;
struct variable{
  struct stack_cell *contents;
};
enum{
    t_int,
    t_float,
    t_string,
    t_address,
    t_var,
    t_svar,
    t_lvar,
    t_lock,
    t_dbref,
    t_array,
    t_invalid
} stack_type;
struct stack_cell{
  char type;
  union{
    int number;
    double fnumber;
    char* str;
    size_t address;
    struct variable* var;
  } data;
};
enum instruction_opcode{
  i_push_primitive,
  i_pop,
  i_popn,
  i_dup,
  i_dupn,
  i_plus,
  i_minus,
  i_divide,
  i_multiply,
  i_increment,
  i_jmp,
  i_jmp_if,
  i_jmp_not_if,
  i_strcat,
  i_strlen,
  i_smatch,
  i_strcmp,
  i_equal,
  i_gt,
  i_lt,
  i_gte,
  i_lte,
  i_not_equal,
  i_not,
  i_and,
  i_or,
  i_rot,
  i_rotn,
  i_over,
  i_nip,
  i_swap,
  i_exit,
  i_abort,
  i_explode,
  i_reverse,
  i_split,
  i_rsplit,
  i_call,
  i_foriter,
  i_forpush,
  i_forpop,
  i_break,
  i_continue,
  i_notify,
  i_read,
  i_intostr
};
struct instruction{
  char type;
  union{
    size_t address;
    struct stack_cell information;
  } data;
} __attribute__((packed));
struct program{
  struct word **words;
  size_t word_count;
  struct variable vars[MAX_VAR_COUNT];
  struct variable lvars[MAX_LVAR_COUNT];
  struct instruction *bytecode;
  size_t bytecode_size;
  size_t bytecode_capacity;
};
struct for_vars{
  int start,end,step;
};
struct program* build(struct tokenlist* tl);
void print_bytecode(struct program* p);
struct instruction simple_instruction_from_type(int t);
struct stack_cell *stack_ptr_from_rval(struct stack_cell n);
struct stack_cell copy_stack_cell(struct stack_cell n);
void free_stack_cell(struct stack_cell);
int is_stack_cell_true(struct stack_cell);
struct stack_cell create_prim_int(int);
struct stack_cell create_prim_double(double);
struct stack_cell create_prim_string(const char*);
struct stack_cell create_prim_invalid(const char*);
void print_stack_cell(struct stack_cell*);
const char* obtain_bytecode_name(char);
struct variable* create_variable(const char* name);
void free_program(struct program** p);
#endif
