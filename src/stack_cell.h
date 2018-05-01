#ifndef STACK_CELL_H
#define STACK_CELL_H
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
void free_stack_cell(struct stack_cell);
int is_stack_cell_true(struct stack_cell);
int are_stack_cell_equal(struct stack_cell,struct stack_cell);
void print_stack_cell(struct stack_cell*);
struct stack_cell copy_stack_cell(struct stack_cell n);
#endif
