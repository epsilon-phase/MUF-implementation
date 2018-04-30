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
  size_t position,
         local_vars;
  char* name;
};
struct stack_cell;
struct variable{
  struct stack_cell *contents;
};
struct shared_string{
  unsigned int links;
  unsigned int length;
  char str[];
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
    t_mark,
    t_invalid
} stack_type;
struct stack_cell{
  union{
    int number;
    double fnumber;
    struct shared_string *str;
    size_t address;
    struct variable* var;
  } data;
  unsigned char type;
};
enum instruction_opcode{
  i_abort,
  i_and,
  i_andn,
  i_assert,
  i_assign,
  i_atoi,
  i_break,
  i_call,
  i_continue,
  i_decrement,
  i_depth,
  i_dereference,
  i_divide,
  i_dup,
  i_dupn,
  i_equal,
  i_exit,
  i_explode,
  i_foriter,
  i_forpop,
  i_forpush,
  i_gt,
  i_gte,
  i_increment,
  i_instr,
  i_intostr,
  i_jmp,
  i_jmp_if,
  i_jmp_not_if,
  i_lt,
  i_lte,
  i_mark,
  i_mark_end,
  i_minus,
  i_modulo,
  i_multiply,
  i_nip,
  i_not,
  i_not_equal,
  i_notify,
  i_or,
  i_orn,
  i_over,
  i_plus,
  i_pop,
  i_popn,
  i_power,
  i_push_primitive,
  i_read,
  i_reverse,
  i_rot,
  i_rotn,
  i_rsplit,
  i_smatch,
  i_split,
  i_strcat,
  i_strcmp,
  i_striplead,
  i_striptail,
  i_strip,
  i_strlen,
  i_strtod,
  i_subst,
  i_swap,
  i_tolower,
  i_while,
  i_xor,
};
#define LARGEST_INSTRUCTION_CODE i_xor
struct instruction{
  union{
    size_t address;
    struct stack_cell information;
  } data;
  unsigned char type;
} __attribute__((packed));
struct instruction_range{
    size_t start,
           end,
           line;
};
struct program{
  struct word *words;
  size_t word_count;
  struct variable lvars[MAX_LVAR_COUNT];
  struct instruction *bytecode;
  size_t bytecode_size;
  size_t bytecode_capacity;
  size_t entry_point;
  struct instruction_range *lines;
  size_t range_count;
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
int are_stack_cell_equal(struct stack_cell,struct stack_cell);
struct stack_cell create_prim_int(int);
struct stack_cell create_prim_double(double);
struct stack_cell create_prim_string(const char*);
struct stack_cell create_prim_invalid(const char*);
void print_stack_cell(struct stack_cell*);
const char* obtain_bytecode_name(unsigned char);
struct variable* create_variable(const char* name);
void free_program(struct program** p);
#endif
