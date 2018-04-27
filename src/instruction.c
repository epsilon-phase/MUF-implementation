#define INSTRUCTION_C
#include "instruction.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
PRIM_SIG(p_mark){
  struct stack_cell r;
  r.type=t_mark;
  push_data_stack(frame->stack,r);
  return frame;
}
PRIM_SIG(p_mark_end){
  size_t i;
  for(i=0;frame->stack->size-i>0;i++){
    if(frame->stack->stack[frame->stack->size-1-i].type==t_mark){
      break;
    }
  }
  for(size_t r=frame->stack->size-i-1;r<frame->stack->size-1;r--){
    frame->stack->stack[r]=frame->stack->stack[r+1];
  }
  push_data_stack(frame->stack,create_prim_int((int)i));
  return frame;
}
PRIM_SIG(p_dup) {
  struct stack_cell x = pop_data_stack(frame->stack);
  push_data_stack(frame->stack, x);
  push_data_stack(frame->stack, x);
  free_stack_cell(x);
  return frame;
}
PRIM_SIG(p_dupn) {
  struct stack_cell n = pop_data_stack(frame->stack);
  assert(n.type == t_int && n.data.number > -0);
  size_t original = frame->stack->size;
  for (size_t i = frame->stack->size - n.data.number; i < original; i++) {
    push_data_stack(frame->stack, copy_stack_cell(frame->stack->stack[i]));
  }
  free_stack_cell(n);
  return frame;
}
PRIM_SIG(p_pop) { pop_data_stack(frame->stack); return frame;}
PRIM_SIG(p_popn) {
  struct stack_cell n = pop_data_stack(frame->stack);
  assert(n.type == t_int && n.data.number >= 0);
  for (int i = 0; i < n.data.number; i++)
    pop_data_stack(frame->stack);
  return frame;
}
PRIM_SIG(p_depth) {
  struct stack_cell n;
  n.type = t_int;
  n.data.number = frame->stack->size;
  push_data_stack(frame->stack, n);
  return frame;
}
PRIM_SIG(p_dereference){
  struct stack_cell a=pop_data_stack(frame->stack),r;
  assert(a.type==t_svar||a.type==t_var||a.type==t_lvar);
  switch(a.type){
    case t_svar:
      r=frame->svars[a.data.number];
      break;
    default:
      break;
  }
  push_data_stack(frame->stack,copy_stack_cell(r));
  return frame;
}
PRIM_SIG(p_plus) {
  // The order here is wrong, but since it's addition, that doesn't even matter
  // a little bit.
  struct stack_cell x = pop_data_stack(frame->stack),
                    y = pop_data_stack(frame->stack);
  struct stack_cell r;
  if ((x.type == t_int || x.type == t_float) &&
      (y.type == t_int || y.type == t_float)) {
    r.type = x.type == t_float || y.type == t_float ? t_float : t_int;
    if (r.type == t_float) {
      double dat;
      switch (x.type) {
      case t_int:
        dat = x.data.number;
        break;
      case t_float:
        dat = x.data.fnumber;
        break;
      }
      switch (y.type) {
      case t_int:
        dat += y.data.number;
        break;
      case t_float:
        dat += y.data.fnumber;
        break;
      }
      r.data.fnumber = dat;
      r.type = t_float;
    } else {
      r.data.number = x.data.number + y.data.number;
      r.type = t_int;
    }
  } else {
    r.type = t_invalid;
    r=create_prim_invalid("Error! Invalid types to add!");
  }
  push_data_stack(frame->stack, r);
  return frame;
}
PRIM_SIG(p_minus) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  struct stack_cell r;
  if ((x.type == t_int || x.type == t_float) &&
      (y.type == t_int || y.type == t_float)) {
    r.type = x.type == t_float || y.type == t_float ? t_float : t_int;
    if (r.type == t_float) {
      double dat;
      switch (x.type) {
      case t_int:
        dat = x.data.number;
        break;
      case t_float:
        dat = x.data.fnumber;
        break;
      }
      switch (y.type) {
      case t_int:
        dat -= y.data.number;
        break;
      case t_float:
        dat -= y.data.fnumber;
        break;
      }
      r.type = t_float;
      r.data.fnumber = dat;
    } else {
      r.type = t_int;
      r.data.number = x.data.number - y.data.number;
    }
  } else {
    r = create_prim_invalid("Error, invalid type to subtract!");
  }
  push_data_stack(frame->stack, r);
  return frame;
}
PRIM_SIG(p_multiply) {
  struct stack_cell x = pop_data_stack(frame->stack),
                    y = pop_data_stack(frame->stack);
  struct stack_cell r;
  if ((x.type == t_int || x.type == t_float) &&
      (y.type == t_int || y.type == t_float)) {
    r.type = x.type == t_float || y.type == t_float ? t_float : t_int;
    if (r.type == t_float) {
      double dat;
      switch (x.type) {
      case t_int:
        dat = x.data.number;
        break;
      case t_float:
        dat = x.data.fnumber;
        break;
      }
      switch (y.type) {
      case t_int:
        dat *= y.data.number;
        break;
      case t_float:
        dat *= y.data.fnumber;
        break;
      }
      r.type = t_float;
      r.data.fnumber = dat;
    } else {
      r.type = t_int;
      r.data.number = x.data.number * y.data.number;
    }
  } else {
    r.type = t_invalid;
    r=create_prim_invalid("Error, invalid type to multiply!");
  }
  push_data_stack(frame->stack, r);
  return frame;
}
PRIM_SIG(p_divide) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  struct stack_cell r;
  if ((x.type == t_int || x.type == t_float) &&
      (y.type == t_int || y.type == t_float)) {
    r.type = x.type == t_float || y.type == t_float ? t_float : t_int;
    if (r.type == t_float) {
      double dat;
      switch (x.type) {
      case t_int:
        dat = x.data.number;
        break;
      case t_float:
        dat = x.data.fnumber;
        break;
      }
      switch (y.type) {
      case t_int:
        dat /= y.data.number;
        break;
      case t_float:
        dat /= y.data.fnumber;
        break;
      }
      r.type = t_float;
      r.data.fnumber = dat;
    } else {
      r.type = t_int;
      r.data.number = x.data.number / y.data.number;
    }
  } else {
    r.type = t_invalid;
    r=create_prim_invalid("Error, invalid type to divide!");
  }
  free_stack_cell(x);
  free_stack_cell(y);
  push_data_stack(frame->stack, r);
  return frame;
}
PRIM_SIG(p_power) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  assert(x.type == t_int || x.type == t_float);
  assert(y.type == t_int || y.type == t_float);
  struct stack_cell r;
  if ((x.type == t_int || x.type == t_float) &&
      (y.type == t_int || y.type == t_float)) {
    r.type = x.type == t_float || y.type == t_float ? t_float : t_int;
    if (r.type == t_float) {
      double dat;
      switch (x.type) {
      case t_int:
        dat = x.data.number;
        break;
      case t_float:
        dat = x.data.fnumber;
        break;
      }
      switch (y.type) {
      case t_int:
        dat = pow(dat, y.data.number);
        break;
      case t_float:
        dat = pow(dat, y.data.fnumber);
        break;
      }
      r.type = t_float;
      r.data.fnumber = dat;
    } else {
      r.type = t_int;
      r.data.number = (int)pow(x.data.number, y.data.number);
    }
  } else {
    r.type = t_invalid;
    r=create_prim_invalid("Incorrect type for power function");
  }
  push_data_stack(frame->stack, r);
  return frame;
}
PRIM_SIG(p_increment){
    struct stack_cell *x=&frame->stack->stack[frame->stack->size-1];
    if(x->type==t_int)
        x->data.number++;
    else if(x->type==t_float)
        x->data.fnumber++;
    return frame;
}
PRIM_SIG(p_decrement){
  struct stack_cell *x=&frame->stack->stack[frame->stack->size-1];
  assert(x->type==t_int||x->type==t_float);
  if(x->type==t_int)
      x->data.number--;
  else if(x->type==t_float)
      x->data.fnumber--;
  return frame;
}
PRIM_SIG(p_modulo){
  struct stack_cell y=pop_data_stack(frame->stack),
                    *x=&frame->stack->stack[frame->stack->size-1];
  assert(x->type==t_int&&y.type==t_int);
  x->data.number%=y.data.number;
  return frame;
}
PRIM_SIG(p_rot) {
  struct stack_cell z = pop_data_stack(frame->stack),
                    y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  push_data_stack(frame->stack, y);
  push_data_stack(frame->stack, z);
  push_data_stack(frame->stack, x);
  return frame;
}
PRIM_SIG(p_rotate) {
  struct stack_cell n = pop_data_stack(frame->stack);
  assert(n.type == t_int);
  struct stack_cell r;
  if (n.data.number > 0) {
    r = frame->stack->stack[frame->stack->size - n.data.number];
    for (size_t i = frame->stack->size - n.data.number;
         i < frame->stack->size - 1; i++) {
      frame->stack->stack[i] = frame->stack->stack[i + 1];
    }
    frame->stack->stack[frame->stack->size - 1] = r;
  } else {
    n.data.number = -n.data.number;
    r = frame->stack->stack[frame->stack->size - 1];
    for (size_t i = frame->stack->size - 1;
         i > frame->stack->size - n.data.number + 1; i--) {
      frame->stack->stack[i] = frame->stack->stack[i - 1];
    }
    frame->stack->stack[frame->stack->size - n.data.number] = r;
  }
  return frame;
}
PRIM_SIG(p_swap){
    //struct stack_cell y=pop_data_stack(frame->stack),
    //                  x=pop_data_stack(frame->stack);
    //push_data_stack(frame->stack,y);
    //push_data_stack(frame->stack,x);
    //free_stack_cell(x);
    //free_stack_cell(y);
  struct stack_cell tmp=frame->stack->stack[frame->stack->size-1];
  frame->stack->stack[frame->stack->size-1]=frame->stack->stack[frame->stack->size-2];
  frame->stack->stack[frame->stack->size-2]=tmp;
  return frame;
}
PRIM_SIG(p_exit){
    struct frame *tmp=frame->parent;
    if(frame->parent){
        free_frame(frame);
        free(frame);
        return tmp;
    }
    return frame;
}
PRIM_SIG(p_jmp) {
    // Minus one because the next instruction will be stepped to after this.
  frame->instr_pointer =
      frame->program->bytecode[frame->instr_pointer].data.address - 1;
  return frame;
}
PRIM_SIG(p_jmp_if) {
  struct stack_cell x = pop_data_stack(frame->stack);
  if (is_stack_cell_true(x))
    // Minus one because the next instruction will be stepped to after this.
    frame->instr_pointer =
        frame->program->bytecode[frame->instr_pointer].data.address - 1;
  free_stack_cell(x);
  return frame;
}
PRIM_SIG(p_jmp_not_if) {
  struct stack_cell x = pop_data_stack(frame->stack);
  if (!is_stack_cell_true(x)) {
    // Minus one because the next instruction will be stepped to after this.
    frame->instr_pointer =
        frame->program->bytecode[frame->instr_pointer].data.address - 1;
  }
  free_stack_cell(x);
  return frame;
}
PRIM_SIG(p_assert){
  struct stack_cell fail_msg=pop_data_stack(frame->stack),
                    succ_msg=pop_data_stack(frame->stack),
                    value=pop_data_stack(frame->stack);
  if(!is_stack_cell_true(value)){
    fprintf(stderr,"ASSERT FAILED! %s\n",fail_msg.data.str->str);
  }else{
    fprintf(stderr,"ASSERT SUCCESS! %s\n",succ_msg.data.str->str);
  }
  free_stack_cell(fail_msg);
  free_stack_cell(succ_msg);
  free_stack_cell(value);
  return frame;
}
PRIM_SIG(p_assign){
  struct stack_cell var=pop_data_stack(frame->stack),
                    value=pop_data_stack(frame->stack);
  switch(var.type){
    case t_svar:
      free_stack_cell(frame->svars[var.data.number]);
      frame->svars[var.data.number]=copy_stack_cell(value);
      break;
    default:
      break;
  }
  free_stack_cell(var);
  free_stack_cell(value);
  return frame;
}
PRIM_SIG(p_break){
    return p_jmp(frame);
}
PRIM_SIG(p_continue){
    return p_jmp(frame);
}
PRIM_SIG(p_strcat) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  struct stack_cell z;
  z.type=t_string;
  z.data.str =
      calloc(1,sizeof(struct shared_string)+ strlen(x.data.str->str) + strlen(y.data.str->str) + 1);
  z.data.str->links=1;
  z.data.str->length=strlen(z.data.str->str);
  strcat(z.data.str->str, x.data.str->str);
  strcat(z.data.str->str, y.data.str->str);
  free_stack_cell(y);
  push_data_stack(frame->stack,z);
  free_stack_cell(x);
  free_stack_cell(z);
  return frame;
}
PRIM_SIG(p_strlen) {
  struct stack_cell r=pop_data_stack(frame->stack);
  assert(r.type==t_string);
  int rlen=strlen(r.data.str->str);
  free_stack_cell(r);
  r.type=t_int;
  r.data.str=NULL;
  r.data.number=rlen;
  push_data_stack(frame->stack,r);
  return frame;
}
PRIM_SIG(p_strcmp) {
  struct stack_cell b=pop_data_stack(frame->stack),
                    a=pop_data_stack(frame->stack);
  assert(b.type==t_string&&a.type==t_string);
  struct stack_cell result={.type=t_int,.data.number=strcmp(a.data.str->str,b.data.str->str)};
  push_data_stack(frame->stack,result);
  free_stack_cell(b);
  free_stack_cell(a);
  return frame;
}
PRIM_SIG(p_forpush) {
  struct stack_cell step = pop_data_stack(frame->stack),
                    end = pop_data_stack(frame->stack),
                    start = pop_data_stack(frame->stack);
  assert(step.type == t_int);
  assert(end.type == t_int);
  assert(start.type == t_int);
  struct for_vars r;
  r.start = start.data.number;
  r.end = end.data.number;
  r.step = step.data.number;
  push_for_vars_stack(frame->fstack, r);
  free_stack_cell(step);
  free_stack_cell(end);
  free_stack_cell(start);
  return frame;
}
PRIM_SIG(p_forpop) {
 pop_for_vars_stack(frame->fstack); 
  return frame;
}
PRIM_SIG(p_foriter) {
  /*  struct for_vars* current=peek_for_vars_stack(frame->fstack);

    int s=current->start,
        e=current->end,
        st=current->step,
        tmp=s;
    struct stack_cell n;
    n.type=t_int;
    n.data.number=tmp;
    s+=st;
    if(!((st>0&&e>s)||(st<0&&e<s))){
      frame->instr_pointer=frame->program->bytecode[frame->instr_pointer].
          data.address-1;
    }*/
  struct stack_cell r = iter_for_vars_stack(frame->fstack);
  if (r.type != t_invalid) {
    push_data_stack(frame->stack, r);
  } else {
    frame->instr_pointer =
        frame->program->bytecode[frame->instr_pointer].data.address;
  }
  return frame;
}
PRIM_SIG(p_gt) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  assert(x.type == t_int || x.type == t_float || x.type == t_dbref);
  assert(y.type == t_int || x.type == t_float || x.type == t_dbref);
  switch (x.type) {
  case t_dbref:
  case t_int:
    switch (y.type) {
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number > y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number > y.data.fnumber));
      break;
    default:
      goto error;
    }
    break;
  case t_float:
    switch (y.type) {
    case t_dbref:
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber > y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber > y.data.fnumber));
      break;
    default:
      goto error;
      break;
    }
  }
  if (0) {
  error:
    push_data_stack(frame->stack,
                    create_prim_invalid("Invalid type for comparison"));
  }
  return frame;
}
PRIM_SIG(p_lt) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  assert(x.type == t_int || x.type == t_float || x.type == t_dbref);
  assert(y.type == t_int || x.type == t_float || x.type == t_dbref);
  switch (x.type) {
  case t_dbref:
  case t_int:
    switch (y.type) {
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number < y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number < y.data.fnumber));
      break;
    default:
      goto error;
    }
    break;
  case t_float:
    switch (y.type) {
    case t_dbref:
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber < y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber < y.data.fnumber));
      break;
    default:
      goto error;
      break;
    }
  }
  if (0) {
  error:
    push_data_stack(frame->stack,
                    create_prim_invalid("Invalid type for comparison"));
  }
  return frame;
}
PRIM_SIG(p_lte) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  assert(x.type == t_int || x.type == t_float || x.type == t_dbref);
  assert(y.type == t_int || x.type == t_float || x.type == t_dbref);
  switch (x.type) {
  case t_dbref:
  case t_int:
    switch (y.type) {
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number <= y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number <= y.data.fnumber));
      break;
    default:
      goto error;
    }
    break;
  case t_float:
    switch (y.type) {
    case t_dbref:
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber <= y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber <= y.data.fnumber));
      break;
    default:
      goto error;
      break;
    }
  }
  if (0) {
  error:
    push_data_stack(frame->stack,
                    create_prim_invalid("Invalid type for comparison"));
  }
  return frame;
}
PRIM_SIG(p_gte) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  assert(x.type == t_int || x.type == t_float || x.type == t_dbref);
  assert(y.type == t_int || x.type == t_float || x.type == t_dbref);
  switch (x.type) {
  case t_dbref:
  case t_int:
    switch (y.type) {
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number >= y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number >= y.data.fnumber));
      break;
    default:
      goto error;
    }
    break;
  case t_float:
    switch (y.type) {
    case t_dbref:
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber >= y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber >= y.data.fnumber));
      break;
    default:
      goto error;
      break;
    }
  }
  if (0) {
  error:
    push_data_stack(frame->stack,
                    create_prim_invalid("Invalid type for comparison"));
  }
  return frame;
}
PRIM_SIG(p_equal) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  assert(x.type == t_int || x.type == t_float || x.type == t_dbref);
  assert(y.type == t_int || x.type == t_float || x.type == t_dbref);
  switch (x.type) {
  case t_dbref:
  case t_int:
    switch (y.type) {
    case t_int:
    case t_dbref:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number == y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number == y.data.fnumber));
      break;
    default:
      goto error;
    }
    break;
  case t_float:
    switch (y.type) {
    case t_dbref:
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber == y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber == y.data.fnumber));
      break;
    default:
      goto error;
      break;
    }
  }
  if (0) {
  error:
    push_data_stack(frame->stack,
                    create_prim_invalid("Invalid type for comparison"));
  }
  return frame;
}
PRIM_SIG(p_not_equal) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    x = pop_data_stack(frame->stack);
  assert(x.type == t_int || x.type == t_float || x.type == t_dbref);
  assert(y.type == t_int || x.type == t_float || x.type == t_dbref);
  switch (x.type) {
  case t_dbref:
  case t_int:
    switch (y.type) {
    case t_int:
    case t_dbref:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number != y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.number != y.data.fnumber));
      break;
    default:
      goto error;
    }
    break;
  case t_float:
    switch (y.type) {
    case t_dbref:
    case t_int:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber != y.data.number));
      break;
    case t_float:
      push_data_stack(frame->stack,
                      create_prim_int(x.data.fnumber != y.data.fnumber));
      break;
    default:
      goto error;
      break;
    }
  }
  if (0) {
  error:
    push_data_stack(frame->stack,
                    create_prim_invalid("Invalid type for comparison"));
  }
  return frame;
}
PRIM_SIG(p_or){
  struct stack_cell y=pop_data_stack(frame->stack),
                    x=pop_data_stack(frame->stack),
                    result={.type=t_int};
  if(is_stack_cell_true(x)||is_stack_cell_true(y)){
    result.data.number=1;
  }else{
    result.data.number=0;
  }
  push_data_stack(frame->stack,result);
  free_stack_cell(y);
  free_stack_cell(x);
  return frame;
}
PRIM_SIG(p_and){
  struct stack_cell y=pop_data_stack(frame->stack),
                    x=pop_data_stack(frame->stack),
                    result={.type=t_int};
  if(is_stack_cell_true(x)&&is_stack_cell_true(y)){
    result.data.number=1;
  }else{
    result.data.number=0;
  }
  push_data_stack(frame->stack,result);
  free_stack_cell(y);
  free_stack_cell(x);
  return frame;
}
PRIM_SIG(p_xor){
  struct stack_cell y=pop_data_stack(frame->stack),
                    x=pop_data_stack(frame->stack),
                    result={.type=t_int};
  if(!is_stack_cell_true(x)!=!is_stack_cell_true(y)){
    result.data.number=1;
  }else{
    result.data.number=0;
  }
  push_data_stack(frame->stack,result);
  free_stack_cell(y);
  free_stack_cell(x);
  return frame;
}
PRIM_SIG(p_not){
  struct stack_cell y=pop_data_stack(frame->stack),
                    result={.type=t_int};
  if(!is_stack_cell_true(y)){
    result.data.number=1;
  }else{
    result.data.number=0;
  }
  push_data_stack(frame->stack,result);
  free_stack_cell(y);
  return frame;
}
PRIM_SIG(p_push_primitive) {
  size_t istr = frame->instr_pointer;
  struct stack_cell result =
      copy_stack_cell(frame->program->bytecode[istr].data.information);
  push_data_stack(frame->stack, result);
  free_stack_cell(result);
  return frame;
}
PRIM_SIG(p_notify) {
  struct stack_cell r = pop_data_stack(frame->stack);
  assert(r.type == t_string);
  printf("%s\n", r.data.str->str);
  free_stack_cell(r);
  return frame;
}
PRIM_SIG(p_read) {
  char *line = NULL;
  size_t size = 100;
  if (-1 == getline(&line, &size, stdin)) {
    fprintf(stderr, "Fack!\n");
  }
  struct stack_cell r;
  r = create_prim_string(line);
  if(line)free(line);
    push_data_stack(frame->stack, r);
  free_stack_cell(r);
  return frame;
}
PRIM_SIG(p_atoi) {
  struct stack_cell r = pop_data_stack(frame->stack);
  assert(r.type == t_string);
  struct stack_cell result;
  result.type = t_int;
  result.data.number = atoi(r.data.str->str);
  push_data_stack(frame->stack, result);
  free_stack_cell(r);
  return frame;
}
PRIM_SIG(p_strtod){
    struct stack_cell s=pop_data_stack(frame->stack);
    assert(s.type==t_string);
    char *end=NULL;
    double l=strtod(s.data.str->str,&end);
    struct stack_cell result;
    result.type=t_float;
    result.data.fnumber=l;
    push_data_stack(frame->stack,result);
    free_stack_cell(s);
  return frame;
}
PRIM_SIG(p_split){
  struct stack_cell y=pop_data_stack(frame->stack),
                    x=pop_data_stack(frame->stack),
                    a,b;
  a.type=t_string;
  b.type=t_string;
  char *found=strstr(x.data.str->str,y.data.str->str);
  if(found){
    a.data.str=malloc(sizeof(struct shared_string)+(found-x.data.str->str)+1);
    a.data.str->length=found-x.data.str->str;
    memcpy(a.data.str->str,x.data.str->str,found-x.data.str->str);
    a.data.str->str[found-x.data.str->str]=0;
    a.data.str->links=1;
    b.data.str=malloc(sizeof(struct shared_string)+strlen(x.data.str->str)-(found-x.data.str->str));
    strncpy(b.data.str->str,x.data.str->str+(found-x.data.str->str)+1,x.data.str->length-(found-x.data.str->str));
    b.data.str->length=strlen(b.data.str->str);
    b.data.str->links=1;
    free_stack_cell(x);
    free_stack_cell(y);
    push_data_stack(frame->stack,a);
    push_data_stack(frame->stack,b);
    free_stack_cell(a);
    free_stack_cell(b);
  }else{
    push_data_stack(frame->stack,x);
    b=create_prim_string("");
    push_data_stack(frame->stack,b);
    free_stack_cell(y);
    free_stack_cell(b);
  }
  return frame;
}
PRIM_SIG(p_rsplit){
  struct stack_cell y=pop_data_stack(frame->stack),
                    x=pop_data_stack(frame->stack),
                    a,b;
  a.type=t_string;
  b.type=t_string;
  char* found,*tmp=strstr(x.data.str->str,y.data.str->str);
  if(tmp){
    while(tmp){
        found=tmp;
        tmp=strstr(tmp+y.data.str->length,y.data.str->str);
    }
    a.data.str=malloc(sizeof(struct shared_string)+(found-x.data.str->str)+1);
    a.data.str->length=found-x.data.str->str;
    memcpy(a.data.str->str,x.data.str->str,found-x.data.str->str);
    a.data.str->str[found-x.data.str->str]=0;
    a.data.str->links=1;
    b.data.str=malloc(sizeof(struct shared_string)+strlen(x.data.str->str)-(found-x.data.str->str));
    strncpy(b.data.str->str,x.data.str->str+(found-x.data.str->str)+1,x.data.str->length-(found-x.data.str->str));
    b.data.str->length=strlen(b.data.str->str);
    b.data.str->links=1;
    free_stack_cell(x);
    free_stack_cell(y);
    push_data_stack(frame->stack,a);
    push_data_stack(frame->stack,b);
    free_stack_cell(a);
    free_stack_cell(b);
  }else{
    push_data_stack(frame->stack,x);
    b=create_prim_string("");
    push_data_stack(frame->stack,b);
    free_stack_cell(y);
    free_stack_cell(a);
    free_stack_cell(b);
  }
  return frame;

}
PRIM_SIG(p_call){
    struct frame* result=malloc(sizeof(struct frame));
    struct stack_cell r=pop_data_stack(frame->stack);
    *result=create_frame(frame->program,NULL,frame);
    for(size_t i=0;i<result->program->word_count;i++){
        if(!strcmp(result->program->words[i].name,r.data.str->str)){
            result->instr_pointer=result->program->words[i].position-1;
            result->svars=malloc(result->program->words[i].local_vars*sizeof(struct stack_cell));
            result->svar_count=result->program->words[i].local_vars;
            break;
        }
    }
    free_stack_cell(r);
    return result;
}
PRIM_SIG(p_explode){
  struct stack_cell y=pop_data_stack(frame->stack),
                    x=pop_data_stack(frame->stack),
                    a;
  a.type=t_string;
  char *thing=x.data.str->str,
       *thing2;
  int count=0;
  while((thing2=strstr(thing,y.data.str->str))){
    a.data.str=malloc(sizeof(struct shared_string)+thing2-thing+1);
    a.data.str->links=1;
    a.data.str->length=thing2-thing;
    memcpy(a.data.str->str,thing,thing2-thing);
    a.data.str->str[thing2-thing]=0;
    push_data_stack(frame->stack,a);
    free_stack_cell(a);
    thing=thing2+strlen(y.data.str->str);count++;
  }
  if(thing){
    a.data.str=malloc(sizeof(struct shared_string)+strlen(thing)+1);
    a.data.str->links=1;
    a.data.str->length=strlen(thing);
    memcpy(a.data.str->str,thing,strlen(thing));
    a.data.str->str[strlen(thing)]=0;
    push_data_stack(frame->stack,a);
    free_stack_cell(a);
    count++;
  }
  push_data_stack(frame->stack,create_prim_int(count));
  free_stack_cell(x);
  free_stack_cell(y);
  return frame;
}
PRIM_SIG(p_intostr) {
  struct stack_cell r = pop_data_stack(frame->stack);
  struct stack_cell f;
  f.type = t_string;
  int newlen;
  switch (r.type) {
  case t_int:
    f.data.str = malloc(sizeof(struct shared_string)+12);
    f.data.str->length=12;
    f.data.str->links=1;
    sprintf(f.data.str->str, "%d", r.data.number);
    break;
  case t_float:
    newlen=snprintf(NULL, 0, "%f", r.data.fnumber) ;
    f.data.str = malloc(sizeof(struct shared_string)+newlen
        + 1);
    f.data.str->links=1;
    f.data.str->length=newlen;
    sprintf(f.data.str->str, "%f", r.data.fnumber);
    break;
  case t_string:
    f=copy_stack_cell(r);
    break;
  }
  push_data_stack(frame->stack, f);
  free_stack_cell(r);
  free_stack_cell(f);
  return frame;
}
PRIM_SIG(p_while){
  struct stack_cell t=pop_data_stack(frame->stack);
  if(!is_stack_cell_true(t)){
    p_break(frame);
  }
  free_stack_cell(t);
  return frame;
}
PRIM** instructions=NULL;
#define ASSOCIATE(NaMe) instructions[i_##NaMe]=p_##NaMe
//Now this is an ugly hack <:(
#define i_rotate i_rotn
PRIM** get_instructions(){
    if(instructions==NULL){
        instructions=calloc(LARGEST_INSTRUCTION_CODE+1,sizeof(PRIM*));
        ASSOCIATE(push_primitive);
        ASSOCIATE(mark);
        ASSOCIATE(mark_end);
        ASSOCIATE(dup);
        ASSOCIATE(dupn);
        ASSOCIATE(pop);
        ASSOCIATE(popn);
        ASSOCIATE(depth);
        ASSOCIATE(plus);
        ASSOCIATE(minus);
        ASSOCIATE(divide);
        ASSOCIATE(multiply);
        ASSOCIATE(power);
        ASSOCIATE(increment);
        ASSOCIATE(decrement);
        ASSOCIATE(modulo);
        ASSOCIATE(rot);
        ASSOCIATE(rotate);
        ASSOCIATE(swap);
        ASSOCIATE(exit);
        ASSOCIATE(jmp);
        ASSOCIATE(jmp_if);
        ASSOCIATE(jmp_not_if);
        instructions[i_break]=p_break;
        instructions[i_continue]=p_continue;
        ASSOCIATE(strcat);
        ASSOCIATE(strlen);
        ASSOCIATE(strcmp);
        ASSOCIATE(forpush);
        ASSOCIATE(forpop);
        ASSOCIATE(foriter);
        ASSOCIATE(gt);
        ASSOCIATE(lt);
        ASSOCIATE(gte);
        ASSOCIATE(lte);
        ASSOCIATE(equal);
        ASSOCIATE(not_equal);
        ASSOCIATE(or);
        ASSOCIATE(and);
        ASSOCIATE(xor);
        ASSOCIATE(not);
        ASSOCIATE(notify);
        ASSOCIATE(read);
        ASSOCIATE(atoi);
        ASSOCIATE(strtod);
        ASSOCIATE(split);
        ASSOCIATE(rsplit);
        ASSOCIATE(call);
        ASSOCIATE(explode);
        ASSOCIATE(intostr);
        ASSOCIATE(assert);
        ASSOCIATE(assign);
        ASSOCIATE(dereference);
        instructions[i_while]=p_while;
    }
    return instructions;
}
#undef i_rotate
