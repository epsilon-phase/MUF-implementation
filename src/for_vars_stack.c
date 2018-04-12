#include "frame.h"
#include <stdlib.h>
struct for_vars_stack * create_for_vars_stack(){
  struct for_vars_stack* result=malloc(sizeof(struct for_vars_stack));
  result->size=0;
  result->stuff=malloc(sizeof(struct for_vars)*10);
  result->capacity=10;
  return result;
}
struct stack_cell iter_for_vars_stack(struct for_vars_stack* st){
  struct for_vars* r=&st->stuff[st->size-1];
  int start=r->start,
      end=r->end,
      step=r->step;
  struct stack_cell result;printf("FOR %i <= %i, (%i)",start,end,step);
  if((start<=end&&step>0)||(start>=end&&step<0)){
    result.type=t_int;
    result.data.number=start;
    r->start+=step;
  }else{
    result.type=t_invalid;
  }
  return result;
}
void pop_for_vars_stack(struct for_vars_stack* st){
  struct for_vars* cur=&st->stuff[--st->size];
  cur->start=0;
  cur->end=0;
  cur->step=0;
}
void push_for_vars_stack(struct for_vars_stack* st,struct for_vars fv){
  if(st->size==st->capacity){
    st->capacity*=2;
    st->stuff=realloc(st->stuff,sizeof(struct for_vars)*st->capacity);
  }
  st->stuff[st->size++]=fv;
}
struct for_vars* peek_for_vars_stack(struct for_vars_stack* fs){
  return &fs->stuff[fs->size-1];
}
void free_for_vars_stack(struct for_vars_stack* e){
  free(e->stuff);
}
