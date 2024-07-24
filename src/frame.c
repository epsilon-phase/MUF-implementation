#include "frame.h"
#include "instruction.h"
#include <stdlib.h>
#include <string.h>
struct frame create_frame(struct program* prog,const char* arguments,struct frame* parent){
  struct frame result;
  result.fstack=create_for_vars_stack();
  result.parent=parent;
  if(!parent){
    result.stack=create_data_stack(30);
  }else{
    result.stack=parent->stack;
    result.stack->links++;
  }
  if(arguments){
    push_data_stack(result.stack,create_prim_string(arguments));
  }
  result.instr_pointer=0;
  result.program=prog;
  result.svar_count=0;
  result.svars=NULL;
  return result;
}
void execute_program(struct frame* frame,struct arguments* args){
  int counter=0;
  frame->instr_pointer=frame->program->entry_point;
  if(frame->program->words[frame->program->word_count-1].local_vars)
  frame->svars=malloc(sizeof(struct stack_cell)*
      frame->program->words[frame->program->word_count-1].local_vars);
  if(frame->svars){
    memset(frame->svars,0,sizeof(struct stack_cell)*frame->program->words[frame->program->word_count-1].local_vars);
  }
  PRIM** instructions=get_instructions();
  unsigned int max_opcode_name=0;
  for(int i=0;i<=LARGEST_INSTRUCTION_CODE;i++){
    max_opcode_name=strlen(obtain_bytecode_name(i))>max_opcode_name?strlen(obtain_bytecode_name(i)):max_opcode_name;
  }
  while(frame->instr_pointer<frame->program->bytecode_size){
//    if(counter!=0&&counter%40==0){
//      printf("\x1b[J");
//    }
    if(args->print_stack&&counter%args->print_stack==0){
      printf("Instruction [%*s]  at address %zi ",
             max_opcode_name,
             obtain_bytecode_name(frame->program->bytecode[frame->instr_pointer].type),
             frame->instr_pointer);
      for(unsigned int i=0;i<frame->stack->size;i++){
        print_stack_cell(&frame->stack->stack[i]);
        printf(",");
      }
      printf("\n");
    }
//    if(counter%40>0&&counter%20==0){
//      printf("\x1b[20A\x1b[50G");
//    }
//    if(counter%40<20){
//      printf("\n");
//    }else{
//      printf("\x1b[50G\x1b[B");
//    }
#if 0
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
        p_equal(frame);
        break;
      case i_not_equal:
        p_not_equal(frame);
        break;
      case i_break:
        p_jmp(frame);
        break;
      case i_continue:
        p_jmp(frame);
        break;
      case i_forpop:
        p_forpop(frame);
        break;
      case i_forpush:
        p_forpush(frame);
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
      case i_depth:
        p_depth(frame);
        break;
      case i_intostr:
        p_intostr(frame);
        break;
      case i_read:
        p_read(frame);
        break;
      case i_atoi:
        p_atoi(frame);
        break;
      case i_strtod:
        p_strtod(frame);
        break;
      case i_notify:
        p_notify(frame);
        break;
      case i_swap:
        p_swap(frame);
        break;
      case i_mark:
        p_mark(frame);
        break;
      case i_mark_end:
        p_mark_end(frame);
        break;
      case i_strcat:
        p_strcat(frame);
        break;
      case i_split:
        p_split(frame);
        break;
      case i_call:
        frame=p_call(frame);
        break;
      case i_exit:
        frame=p_exit(frame);
        break;
      case i_explode:
        p_explode(frame);
        break;
    }
#endif
    if(instructions[frame->program->bytecode[frame->instr_pointer].type]){
        frame=(*instructions[frame->program->bytecode[frame->instr_pointer].type])(frame);
    }else{
        printf("Unimplemented instruction [%s]  at address %zi ",
               obtain_bytecode_name(frame->program->bytecode[frame->instr_pointer].type),
                frame->instr_pointer);
    }
    counter++;
    frame->instr_pointer++;
  }
    for(unsigned int i=0;i<frame->stack->size;i++){
      print_stack_cell(&frame->stack->stack[i]);
      printf(",");
    }
    printf("\n");
    free(instructions);
//    printf("\x1b[20B\x1b[50G");
}
void free_frame(struct frame* frame){
  free_for_vars_stack(frame->fstack);
  release_data_stack(frame->stack);
  for(size_t i=0;i<frame->svar_count;i++){
    free_stack_cell(frame->svars[i]);
  }
  free(frame->svars);
}
