#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
  switch(sc.type){
    case t_invalid:
    case t_string:
      if(sc.data.str&&!(--sc.data.str->links))
        free(sc.data.str);
      break;
    case t_array:
      if(!sc.data.array)
        break;
      if(!(--sc.data.array->links)){
        printf("Freeing array at address %p\n",sc.data.array);
        free_array(sc.data.array);
      }else{
        printf("Not freeing array at address %p\n",sc.data.array);
      }
      break;
  }
  
}
double get_double(struct stack_cell sc){
  switch(sc.type){
    case t_int:
      return (double)sc.data.number;
    case t_float:
      return sc.data.fnumber;
  }
  return 0.0;
}
struct stack_cell copy_stack_cell(struct stack_cell n){
  switch(n.type){
    case t_string:
      n.data.str->links++;
      break;
    case t_array:
      n.data.array->links++;
      break;
     default:
      break;
  }
  return n;
}
int stack_cell_cmp(struct stack_cell a,struct stack_cell b){
  if((a.type==t_int||a.type==t_float)&&(b.type==t_int||b.type==t_float)){
      double x=get_double(a),y=get_double(b);
      if(x>y){
        return 1;
      }else if(x<y){
        return -1;
      }else{
        return 0;
      }
  }else if(a.type==b.type&&a.type==t_string){
    return strcmp(a.data.str->str,b.data.str->str);
  }else if((a.type==t_int||a.type==t_float)&&b.type==t_string)
    return -1;
  else if(a.type==t_string&&(b.type==t_int||b.type==t_float))
    return 1;
  return 0;
}
