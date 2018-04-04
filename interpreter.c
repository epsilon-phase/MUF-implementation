#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
struct position_stack {
  size_t *position;
  size_t capacity;
  size_t size;
};
struct position_stack *new_position_stack();
void push_position_stack(struct position_stack *s, size_t pos);
size_t pop_position_stack(struct position_stack *s);
void free_position_stack(struct position_stack *s);
void add_instruction(struct program *prog, struct instruction *i) {
  if (!prog)
    return;
  if(!prog->bytecode){
      prog->bytecode=calloc(10,sizeof(struct instruction*));
      prog->bytecode_capacity=10;
  }
  if (prog->bytecode_size == prog->bytecode_capacity) {
    prog->bytecode = realloc(prog->bytecode, sizeof(struct instruction) *
                                                 prog->bytecode_capacity * 2);
    prog->bytecode_capacity *= 2;
  }
  //memcpy(&prog->bytecode[prog->bytecode_size], i, sizeof(struct instruction));
  prog->bytecode[prog->bytecode_size].type=i->type;
  prog->bytecode[prog->bytecode_size].data=i->data;
  prog->bytecode_size++;
}
struct program *build(struct tokenlist *tl) {
  struct program *result = malloc(sizeof(struct program));
  memset(result, 0, sizeof(struct program));
  return result;
}
struct position_stack *new_position_stack() {
  struct position_stack *result = malloc(sizeof(struct position_stack));
  result->position = calloc(sizeof(size_t), 10);
  result->capacity = 10;
  result->size = 0;
}
void push_position_stack(struct position_stack *s, size_t pos) {
  if (s->size == s->capacity) {
    s->capacity *= 2;
    s->position = realloc(s->position, s->capacity * sizeof(size_t));
  }
  s->position[s->size++] = pos;
}
size_t pop_position_stack(struct position_stack *s) {
  size_t r = s->position[s->size - 1];
  s->size--;
  return r;
}
void free_position_stack(struct position_stack *s){
    if(!s)return;
    free(s->position);
    free(s);
}
