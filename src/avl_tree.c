#include <stdlib.h>
#include "avl_tree.h"
int avl_node_height(struct avl_node* node)
{
  int height_left,height_right;
  if(node->left){height_left=avl_node_height(node->left);}
  if(node->right){height_right=avl_node_height(node->right);}
  return height_left>height_right?height_left:height_right;
}
int avl_balance_factor(struct avl_node* node){
  int bf=0;
  if(node->left){
    bf+=avl_node_height(node->left);
  }
  if(node->right){
    bf-=avl_node_height(node->right);
  }
  return bf;
}
struct avl_node* avl_rotate_leftleft(struct avl_node* node){
  struct avl_node *a=node;
  struct avl_node *b=node->left;
  a->left=b->right;
  b->right=a;
  return b;
}
struct avl_node* avl_rotate_leftright(struct avl_node* node){
  struct avl_node* a=node,
    *b=a->left,
    *c=b->right;
  a->left=c->right;
  b->right=c->left;
  c->left=b;
  c->right=a;
  return c;
}
struct avl_node* avl_rotate_rightleft(struct avl_node* node){
  struct avl_node *a=node,*b=a->right,*c=b->left;
  a->right=c->left;
  b->left=c->right;
  c->right=b;
  c->left=a;
  return c;
}
struct avl_node* avl_rotate_rightright(struct avl_node* node){
  struct avl_node *a=node,*b=a->right;
  a->right=b->left;
  b->left=a;
  return b;
}
struct avl_node* avl_balance_node(struct avl_node* node){
  struct avl_node* newroot=NULL;
  if(node->left)
    node->left=avl_balance_node(node->left);
  if(node->right)
    node->right=avl_balance_node(node->right);
  int bf=avl_balance_factor(node);
  if(bf>=2){
    if(avl_balance_factor(node->left)<=-1)
      newroot=avl_rotate_leftright(node);
    else
      newroot=avl_rotate_leftleft(node);
  }else if(bf <=-2){
    if(avl_balance_factor(node->left)<=1)
      newroot=avl_rotate_rightleft(node);
    else
      newroot=avl_rotate_rightright(node);
  }else{
    newroot=node;
  }
  return newroot;
}
void avl_balance(struct avl_tree* tree){
  tree->root=avl_balance_node(tree->root);
}
