#include "interpreter.h"
#include <stdlib.h>
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
    }return 0;
}
void free_stack_cell(struct stack_cell sc){
  if((sc.type==t_invalid||sc.type==t_string)&&sc.data.str){
    if(!(--sc.data.str->links))
      free(sc.data.str);
  }
  sc.data.str=NULL;
  
}
struct stack_cell copy_stack_cell(struct stack_cell n){
  struct stack_cell copy;
  copy.type=n.type;
  switch(n.type){
    case t_string:
      copy.data.str=n.data.str;
      copy.data.str->links++;
      break;
    case t_int:
    case t_svar:
    case t_var:
    case t_lvar:
      copy.data.number=n.data.number;
      break;
    case t_float:
      copy.data.fnumber=n.data.fnumber;
      break;
    case t_address:
      copy.data.address=n.data.address;
      break;
     default:
      break;
  }
  return copy;
}
int stack_cell_cmp(struct stack_cell a,struct stack_cell b){
  return 0;
}
