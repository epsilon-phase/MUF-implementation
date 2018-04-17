#include "frame.h"
#include <stdlib.h>

struct data_stack* create_data_stack(size_t capacity){
  struct data_stack* result=malloc(sizeof(struct data_stack));
  result->links=1;
  result->stack=malloc(sizeof(struct stack_cell)*capacity);
  result->capacity=capacity;
  result->size=0;
  for(size_t i=0;i<capacity;i++){
    result->stack[i].type=t_invalid;
    result->stack[i].data.number=0;
    result->stack[i].data.str=NULL;
  }
  return result;
}
void release_data_stack(struct data_stack *st){
  if((--st->links)==0){
    for(size_t i=0;i<st->capacity;i++){
      free_stack_cell(st->stack[i]);
    }
    free(st->stack);
    free(st);//Hmm... This could be dangerous, but since it's not like you ever get it as not a pointer, maybe it's okay?
  }
}
struct stack_cell pop_data_stack(struct data_stack *st){
  struct stack_cell result;
  if(st->size>0){
    result=copy_stack_cell(st->stack[st->size-1]);
    free_stack_cell(st->stack[st->size-1]);
    st->stack[st->size-1].data.str=NULL;
    st->size--;
  }else{
    result.type=t_invalid;
    result.data.str=NULL;
  }
  return result;
}
void push_data_stack(struct data_stack *st,struct stack_cell sc){
  if(st->size==st->capacity){
    //Oh no!!!!
    st->stack=realloc(st->stack,st->capacity*2*sizeof(struct stack_cell));
    st->capacity*=2;
  }
  st->stack[st->size++]=copy_stack_cell(sc);
  //Does it make sense to free the caller's stack_cell?
 // free_stack_cell(sc);
}
void push_data_stack_nocopy(struct data_stack *st,struct stack_cell sc){
//    if((sc.type==t_string||sc.type==t_invalid)&&sc.data.str)
//      sc.data.str->links++;
    st->stack[st->size++]=sc;
    
}
