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
int avl_node_height(struct avl_node* node);
int avl_balance_factor(struct avl_node* node);
void avl_insert(struct avl_tree *tree,struct stack_cell key, struct stack_cell value);
void avl_delete(struct avl_tree *tree,struct stack_cell key);
struct stack_cell* find_avl_value(struct avl_tree *tree, struct stack_cell val);
#endif
