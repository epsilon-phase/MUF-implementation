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
  int counter=0;
  while(frame->instr_pointer<frame->program->bytecode_size){
    if(counter!=0&&counter%40==0){
      printf("\x1b[J");
    }
    printf("Instruction [%s]  at address %zi ",
           obtain_bytecode_name(frame->program->bytecode[frame->instr_pointer].type),
            frame->instr_pointer);
    for(int i=0;i<frame->stack->size;i++){
      print_stack_cell(&frame->stack->stack[i]);
      printf(",");
    }
    if(counter%40>0&&counter%20==0){
      printf("\x1b[20A\x1b[50G");
    }
    if(counter%40<20){
      printf("\n");
    }else{
      printf("\x1b[50G\x1b[B");
    }
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
      case i_rot:
        p_rot(frame);
        break;
      case i_rotn:
        p_rotate(frame);
        break;
      case i_dupn:
        p_dupn(frame);
        break;
      case i_popn:
        p_popn(frame);
        break;
    }counter++;
    frame->instr_pointer++;
  }
    for(int i=0;i<frame->stack->size;i++){
      print_stack_cell(&frame->stack->stack[i]);
      printf(",");
    }
    printf("\n");
    printf("\x1b[20B\x1b[50G");
}
void free_frame(struct frame* frame){
  free_for_vars_stack(frame->fstack);
  release_data_stack(frame->stack);
}
