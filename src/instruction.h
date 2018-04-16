#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "frame.h"
#include "interpreter.h"
#define PRIM_SIG(N) void N (struct frame* frame)
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
PRIM_SIG(p_rot);
PRIM_SIG(p_rotate);
PRIM_SIG(p_jmp);
PRIM_SIG(p_jmp_if);
PRIM_SIG(p_jmp_not_if);
PRIM_SIG(p_strcat);
PRIM_SIG(p_strlen);
PRIM_SIG(p_strcmp);
PRIM_SIG(p_for_push);
PRIM_SIG(p_for_pop);
PRIM_SIG(p_foriter);
PRIM_SIG(p_gt);
PRIM_SIG(p_lt);
PRIM_SIG(p_lte);
PRIM_SIG(p_gte);
PRIM_SIG(p_equals);
PRIM_SIG(p_not_equals);
PRIM_SIG(p_push_primitive);
PRIM_SIG(p_notify);
PRIM_SIG(p_intostr);
# ifndef INSTRUCTION_C
#   undef PRIM_SIG
# endif
#endif
