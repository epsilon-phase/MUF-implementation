#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "stack_cell.h"
struct avl_node{
  struct stack_cell key,value;
  struct avl_node* right, *left,*parent;
  int height;
};
struct avl_tree{
  struct avl_node *root;
};
struct avl_iterator{
  struct avl_node** current,*n;
  unsigned int size,capacity;
};
int avl_node_height(struct avl_node* node);
int avl_balance_factor(struct avl_node* node);
int avl_insert(struct avl_tree *tree,struct stack_cell key, struct stack_cell value);
int avl_delete(struct avl_tree *tree,struct stack_cell key);
void avl_free(struct avl_tree* tree);
struct stack_cell* find_avl_value(struct avl_tree *tree, struct stack_cell val);
struct avl_tree* copy_tree(struct avl_tree*);
struct avl_iterator create_iterator_begin(struct avl_tree* tree);
struct avl_iterator create_iterator_at(struct avl_tree* tree,struct stack_cell t);
struct avl_node* next(struct avl_iterator*);
struct avl_node* prev(struct avl_iterator*);
void dump_avl_tree(struct avl_tree*,const char*);
#endif
