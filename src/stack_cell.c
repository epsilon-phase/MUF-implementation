#include "interpreter.h"
#include <string.h>
int is_stack_cell_true(struct stack_cell s){
  switch(s.type){
    case t_int:
      return s.data.number!=0;
    case t_float:
      return s.data.fnumber!=0.0;
    case t_string:
      return s.data.str&&strlen(s.data.str->str)>0;
    default:
      return 0;
  }
}
int are_stack_cell_equal(struct stack_cell a,struct stack_cell b){
    if(a.type!=b.type)return 0;
    switch(a.type){
        case t_int:
            return a.data.number==b.data.number;
        case t_float:
            return a.data.fnumber==b.data.fnumber;
        case t_string:
            return a.data.str&&b.data.str&&!strcmp(a.data.str->str,b.data.str->str);
    }
}
