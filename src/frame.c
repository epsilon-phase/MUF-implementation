#include "frame.h"
#include "instruction.h"
struct frame create_frame(struct program* prog,const char* arguments,struct frame* parent){
  struct frame result;
  result.fstack=create_for_vars_stack();
  result.parent=parent;
  if(!parent){
    result.stack=create_data_stack(30);
  }else{
    result.stack=parent->stack;
  }
  if(arguments){
    push_data_stack(result.stack,create_prim_string(arguments));
  }
  result.instr_pointer=0;
  result.program=prog;
  return result;
}
void execute_program(struct frame* frame){
  while(frame->instr_pointer<frame->program->bytecode_size){
    printf("Instruction at address %zi ",frame->instr_pointer);
    for(int i=0;i<frame->stack->size;i++){
      print_stack_cell(&frame->stack->stack[i]);
      printf(",");
    }
    printf("\r");
    switch(frame->program->bytecode[frame->instr_pointer].type){
      case i_jmp:
        p_jmp(frame);
        break;
      case i_jmp_not_if:
        p_jmp_not_if(frame);
        break;
      case i_jmp_if:
        p_jmp_if(frame);
        break;
      case i_foriter:
        p_foriter(frame);
        break;
      case i_lt:
        p_lt(frame);
        break;
      case i_gt:
        p_gt(frame);
        break;
      case i_gte:
        p_gte(frame);
        break;
      case i_lte:
        p_lte(frame);
        break;
      case i_equal:
        p_equals(frame);
        break;
      case i_not_equal:
        p_not_equals(frame);
        break;
      case i_break:
        p_jmp(frame);
        break;
      case i_continue:
        p_jmp(frame);
        break;
      case i_forpop:
        p_for_pop(frame);
        break;
      case i_forpush:
        p_for_push(frame);
        break;
      case i_plus:
        p_plus(frame);
        break;
      case i_pop:
        p_pop(frame);
        break;
      case i_dup:
        p_dup(frame);
        break;
      case i_multiply:
        p_multiply(frame);
        break;
      case i_divide:
        p_divide(frame);
        break;
      case i_push_primitive:
        p_push_primitive(frame);
        break;
    }
    frame->instr_pointer++;
  }
    for(int i=0;i<frame->stack->size;i++){
      print_stack_cell(&frame->stack->stack[i]);
      printf(",");
    }
    printf("\n");
}
void free_frame(struct frame* frame){
  free_for_vars_stack(frame->fstack);
  release_data_stack(frame->stack);
}
