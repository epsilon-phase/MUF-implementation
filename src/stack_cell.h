#ifndef STACK_CELL_H
#define STACK_CELL_H
#include <stddef.h>
struct array;
struct shared_string{
  unsigned int links;
  unsigned int length;
  char str[];
};
enum{
    t_int,
    t_float,
    t_string,
    t_address,
    t_var,
    t_svar,
    t_lvar,
    t_lock,
    t_dbref,
    t_array,
    t_mark,
    t_invalid
} stack_type;
struct stack_cell{
  union{
    int number;
    double fnumber;
    struct shared_string *str;
    size_t address;
    struct array* array;
  } data;
  unsigned char type;
};
struct avl_tree;
struct array{
  size_t size;
  int packed:1;
  union{
    struct stack_cell* packed;
    struct avl_tree* dictionary;
  } data;
  int links;
};
int stack_cell_cmp(struct stack_cell a,struct stack_cell b);
struct array *create_array(struct stack_cell *stack_segment,size_t element_count,int);
struct stack_cell array_get_element(struct array*,struct stack_cell key);
struct array* set_array_item(struct stack_cell value, struct array* array,struct stack_cell key);
struct array* delete_array_item(struct array*,struct stack_cell);
void dump_array(struct array*,const char* fn);
void print_array(struct array*);
void free_stack_cell(struct stack_cell);
void free_array(struct array* arr);
int is_stack_cell_true(struct stack_cell);
int are_stack_cell_equal(struct stack_cell,struct stack_cell);
void print_stack_cell(struct stack_cell*);
struct stack_cell copy_stack_cell(struct stack_cell n);
double get_double(struct stack_cell sc);
struct stack_cell sum_array(struct array*);
#endif
