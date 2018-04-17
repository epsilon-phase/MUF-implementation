#define INSTRUCTION_C
#include "instruction.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  double rd;
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
    struct stack_cell y=pop_data_stack(frame->stack),
                      x=pop_data_stack(frame->stack);
    push_data_stack(frame->stack,y);
    push_data_stack(frame->stack,x);
    free_stack_cell(x);
    free_stack_cell(y);
  return frame;
}
PRIM_SIG(p_jmp) {
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
    frame->instr_pointer =
        frame->program->bytecode[frame->instr_pointer].data.address - 1;
  }
  free_stack_cell(x);
  return frame;
}
PRIM_SIG(p_strcat) {
  struct stack_cell y = pop_data_stack(frame->stack),
                    *x = &frame->stack->stack[frame->stack->size - 1];
  x->data.str =
      realloc(x->data.str, sizeof(struct shared_string)+ strlen(x->data.str->str) + strlen(y.data.str->str) + 1);
  strcat(x->data.str->str, y.data.str->str);
  free_stack_cell(y);
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
  return frame;
}
PRIM_SIG(p_for_push) {
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
PRIM_SIG(p_for_pop) {
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
PRIM_SIG(p_equals) {
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
PRIM_SIG(p_not_equals) {
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
