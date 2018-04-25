#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "frame.h"
#include "interpreter.h"
#define PRIM_SIG(N) struct frame* N (struct frame* frame)
typedef struct frame* PRIM(struct frame*);
PRIM_SIG(p_push_primitive);
PRIM_SIG(p_mark);
PRIM_SIG(p_mark_end);
PRIM_SIG(p_dup);
PRIM_SIG(p_dupn);
PRIM_SIG(p_pop);
PRIM_SIG(p_popn);
PRIM_SIG(p_depth);
PRIM_SIG(p_plus);
PRIM_SIG(p_minus);
PRIM_SIG(p_multiply);
PRIM_SIG(p_divide);
PRIM_SIG(p_power);
PRIM_SIG(p_increment);
PRIM_SIG(p_decrement);
PRIM_SIG(p_modulo);
PRIM_SIG(p_rot);
PRIM_SIG(p_rotate);
PRIM_SIG(p_swap);
PRIM_SIG(p_exit);
PRIM_SIG(p_jmp);
PRIM_SIG(p_jmp_if);
PRIM_SIG(p_jmp_not_if);
PRIM_SIG(p_break);
PRIM_SIG(p_continue);
PRIM_SIG(p_strcat);
PRIM_SIG(p_strlen);
PRIM_SIG(p_strcmp);
PRIM_SIG(p_forpush);
PRIM_SIG(p_forpop);
PRIM_SIG(p_foriter);
PRIM_SIG(p_gt);
PRIM_SIG(p_lt);
PRIM_SIG(p_lte);
PRIM_SIG(p_gte);
PRIM_SIG(p_equal);
PRIM_SIG(p_not_equal);
PRIM_SIG(p_notify);
PRIM_SIG(p_read);
PRIM_SIG(p_atoi);
PRIM_SIG(p_strtod);
PRIM_SIG(p_split);
PRIM_SIG(p_call);
PRIM_SIG(p_explode);
PRIM_SIG(p_intostr);
PRIM** get_instructions();
# ifndef INSTRUCTION_C
#   undef PRIM_SIG
# endif
#endif
