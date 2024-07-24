#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "token.h"
#include "ast.h"
#include "stack_cell.h"
#include "options.h"
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
enum instruction_opcode{
  i_abort,
  i_abs,
  i_acos,
  i_and,
  i_andn,
  i_array_appenditem,
  i_array_count,
  i_array_delitem,
  i_array_dump,
  i_array_make,
  i_array_make_dict,
  i_array_next,
  i_array_prev,
  i_array_getitem,
  i_array_setitem,
  i_array_string_fragment,
  i_array_sum,
  i_asin,
  i_assert,
  i_assign,
  i_atan,
  i_atan2,
  i_atoi,
  i_break,
  i_call,
  i_continue,
  i_ceil,
  i_cos,
  i_decrement,
  i_depth,
  i_dereference,
  i_divide,
  i_dup,
  i_dupn,
  i_equal,
  i_exit,
  i_explode,
  i_fabs,
  i_floor,
  i_foriter,
  i_forpop,
  i_forpush,
  i_gt,
  i_gte,
  i_increment,
  i_instr,
  i_instring,
  i_intostr,
  i_is_str,
  i_jmp,
  i_jmp_if,
  i_jmp_not_if,
  i_log,
  i_log10,
  i_lreverse,
  i_lt,
  i_lte,
  i_mark,
  i_mark_end,
  i_midstr,
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
  i_pick,
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
  i_sin,
  i_smatch,
  i_split,
  i_sqrt,
  i_strcat,
  i_strcmp,
  i_stringpfx,
  i_strip,
  i_striplead,
  i_striptail,
  i_strlen,
  i_strtod,
  i_subst,
  i_swap,
  i_tan,
  i_tolower,
  i_toupper,
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
struct program* build(struct tokenlist* tl,struct arguments*);
void print_bytecode(struct program* p);
struct instruction simple_instruction_from_type(int t);
struct stack_cell *stack_ptr_from_rval(struct stack_cell n);
const char* obtain_bytecode_name(unsigned char);
struct variable* create_variable(const char* name);
void free_program(struct program** p);
#endif
