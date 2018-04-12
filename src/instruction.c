#define INSTRUCTION_C
#include "instruction.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
PRIM_SIG(p_dup){
  struct stack_cell x=pop_data_stack(frame->stack);
  push_data_stack(frame->stack,x);
  push_data_stack(frame->stack,x);
}
PRIM_SIG(p_pop){
  pop_data_stack(frame->stack);
}
PRIM_SIG(p_plus){
  struct stack_cell x=pop_data_stack(frame->stack),
                    y=pop_data_stack(frame->stack);
  struct stack_cell r;
  if((x.type==t_int||x.type==t_float)
      &&(y.type==t_int||y.type==t_float)){
    r.type==x.type==t_float||y.type==t_float?t_float:t_int;
    if(r.type==t_float){
      double dat;
      switch(x.type){
        case t_int:
          dat=x.data.number;
          break;
        case t_float:
          dat=x.data.fnumber;
          break;
      }
      switch(y.type){
        case t_int:
          dat+=y.data.number;
          break;
        case t_float:
          dat+=y.data.fnumber;
          break;
      }
      r.data.fnumber=dat;
    }else{
      r.data.number=x.data.number+y.data.number;
    }
  }else{
    r.type=t_invalid;
    r.data.str=strdup("Error! Invalid types to add!");
  }
  push_data_stack(frame->stack,r);
}
PRIM_SIG(p_minus){
  struct stack_cell y=pop_data_stack(frame->stack),
                    x=pop_data_stack(frame->stack);
  struct stack_cell r;
  if((x.type==t_int||x.type==t_float)
      &&(y.type==t_int||y.type==t_float)){
    r.type==x.type==t_float||y.type==t_float?t_float:t_int;
    if(r.type==t_float){
      double dat;
      switch(x.type){
        case t_int:
          dat=x.data.number;
          break;
        case t_float:
          dat=x.data.fnumber;
          break;
      }
      switch(y.type){
        case t_int:
          dat-=y.data.number;
          break;
        case t_float:
          dat-=y.data.fnumber;
          break;
      }
      r.data.fnumber=dat;
    }else{
      r.data.number=x.data.number-y.data.number;
    }
  }else{
    r.type=t_invalid;
    r.data.str=strdup("Error, invalid type to subtract!");
  }
  push_data_stack(frame->stack,r);
}
PRIM_SIG(p_multiply){
  struct stack_cell x=pop_data_stack(frame->stack),
                    y=pop_data_stack(frame->stack);
  struct stack_cell r;
  if((x.type==t_int||x.type==t_float)
      &&(y.type==t_int||y.type==t_float)){
    r.type==x.type==t_float||y.type==t_float?t_float:t_int;
    if(r.type==t_float){
      double dat;
      switch(x.type){
        case t_int:
          dat=x.data.number;
          break;
        case t_float:
          dat=x.data.fnumber;
          break;
      }
      switch(y.type){
        case t_int:
          dat*=y.data.number;
          break;
        case t_float:
          dat*=y.data.fnumber;
          break;
      }
      r.data.fnumber=dat;
    }else{
      r.data.number=x.data.number*y.data.number;
    }
  }else{
    r.type=t_invalid;
    r.data.str=strdup("Error, invalid type to subtract!");
  }
  push_data_stack(frame->stack,r);
}
PRIM_SIG(p_divide){
  struct stack_cell y=pop_data_stack(frame->stack),
                    x=pop_data_stack(frame->stack);
  struct stack_cell r;
  if((x.type==t_int||x.type==t_float)
      &&(y.type==t_int||y.type==t_float)){
    r.type==x.type==t_float||y.type==t_float?t_float:t_int;
    if(r.type==t_float){
      double dat;
      switch(x.type){
        case t_int:
          dat=x.data.number;
          break;
        case t_float:
          dat=x.data.fnumber;
          break;
      }
      switch(y.type){
        case t_int:
          dat/=y.data.number;
          break;
        case t_float:
          dat/=y.data.fnumber;
          break;
      }
      r.data.fnumber=dat;
    }else{
      r.data.number=x.data.number/y.data.number;
    }
  }else{
    r.type=t_invalid;
    r.data.str=strdup("Error, invalid type to subtract!");
  }
  free_stack_cell(x);
  free_stack_cell(y);
  push_data_stack(frame->stack,r);
}
PRIM_SIG(p_jmp){
  frame->instr_pointer=frame->program->bytecode[frame->instr_pointer].data.address-1;
}
PRIM_SIG(p_jmp_if){
  struct stack_cell x=pop_data_stack(frame->stack);
  if(is_stack_cell_true(x))
    frame->instr_pointer=frame->program->bytecode[frame->instr_pointer].data.address-1;
  free_stack_cell(x);
  
}
PRIM_SIG(p_jmp_not_if){
  struct stack_cell x=pop_data_stack(frame->stack);
  if(!is_stack_cell_true(x))
    frame->instr_pointer=frame->program->bytecode[frame->instr_pointer].data.address-1;
  free_stack_cell(x);
}
PRIM_SIG(p_for_push){
  struct stack_cell step=pop_data_stack(frame->stack),
                    end=pop_data_stack(frame->stack),
                    start=pop_data_stack(frame->stack);
  assert(step.type==t_int);
  assert(end.type==t_int);
  assert(start.type==t_int);
  struct for_vars r;
  r.start=start.data.number;
  r.end=end.data.number;
  r.step=end.data.number;
  push_for_vars_stack(frame->fstack,r);
  free_stack_cell(step);
  free_stack_cell(end);
  free_stack_cell(start);
}
PRIM_SIG(p_for_pop){
}
PRIM_SIG(p_foriter){
}
PRIM_SIG(p_gt){
}
PRIM_SIG(p_lt){
}
PRIM_SIG(p_lte){
}
PRIM_SIG(p_gte){
}
PRIM_SIG(p_equals){
}
PRIM_SIG(p_not_equals){
}
PRIM_SIG(p_push_primitive){
  size_t istr=frame->instr_pointer;
  push_data_stack(frame->stack,copy_stack_cell(frame->program->bytecode[istr].data.information));
}
