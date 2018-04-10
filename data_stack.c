#include "frame.h"
#include <stdlib.h>

struct data_stack* create(size_t capacity){
  struct data_stack* result=malloc(sizeof(struct data_stack));
  result->links=1;
  result->stack=malloc(sizeof(struct stack_cell)*capacity);
  result->capacity=capacity;
  result->size=0;
  for(size_t i=0;i<capacity;i++){
    result->stack[i].type=t_invalid;
    result->stack[i].data.number=0;
  }
  return result;
}
void release_data_stack(struct data_stack *st){
  if((--st->links)==0){
    for(size_t i=0;i<st->capacity;i++){
      free_stack_cell(st->stack[i]);
    }
    free(st->stack);
    free(st);
  }
}
struct stack_cell pop_data_stack(struct data_stack *st){
  struct stack_cell result;
  if(st->size>0){
    result=copy_stack_cell(st->stack[st->size-1]);
    free_stack_cell(st->stack[st->size-1]);
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
  }
  st->stack[st->size++]=copy_stack_cell(sc);
  //Does it make sense to free the caller's stack_cell?
 // free_stack_cell(sc);
}
