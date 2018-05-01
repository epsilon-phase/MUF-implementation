#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "stack_cell.h"
struct avl_node{
  struct stack_cell key,value;
  struct avl_node* right, *left;
};
struct avl_tree{
  struct avl_node *root;
};
int avl_node_height(struct avl_node* node);
int avl_balance_factor(struct avl_node* node);
struct avl_node* avl_rotate_leftleft(struct avl_node* node);
struct avl_node* avl_rotate_leftright(struct avl_node* node);
#endif
