#ifndef FRAME_H
#define FRAME_H
#include "interpreter.h"
#include "options.h"
struct for_vars_stack{
  struct for_vars *stuff;
  size_t size,capacity;
};
struct for_vars_stack *create_for_vars_stack();
void pop_for_vars_stack(struct for_vars_stack*);
void push_for_vars_stack(struct for_vars_stack*,struct for_vars);
void free_for_vars_stack(struct for_vars_stack*);
struct for_vars* peek_for_vars_stack(struct for_vars_stack*);
struct stack_cell iter_for_vars_stack(struct for_vars_stack*);
//RC stack type
struct data_stack{
  int links;
  struct stack_cell* stack;
  size_t capacity,size;
};
struct data_stack* create_data_stack(size_t capacity);
void release_data_stack(struct data_stack* st);
struct stack_cell pop_data_stack(struct data_stack* st);
void push_data_stack(struct data_stack *st, struct stack_cell sc);
void push_data_stack_nocopy(struct data_stack *st, struct stack_cell sc);
struct frame{
  struct program* program;
  struct data_stack* stack;
  struct for_vars_stack* fstack;
  struct frame* parent;
  size_t instr_pointer;
  size_t svar_count;
  struct stack_cell *svars;
};
struct frame create_frame(struct program* prog,const char* arguments,struct frame* parent);
void execute_program(struct frame* frame,struct arguments*);
void free_frame(struct frame* frame);
#endif
