#include "interpreter.h"
#include <string.h>
int is_stack_cell_true(struct stack_cell s){
  switch(s.type){
    case t_int:
      return s.data.number!=0;
    case t_float:
      return s.data.fnumber!=0.0;
    case t_string:
      return s.data.str&&strlen(s.data.str)>0;
    default:
      return 0;
  }
}
