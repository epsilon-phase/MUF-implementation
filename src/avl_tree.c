#include <stdlib.h>
#include "avl_tree.h"
#define clear_height(X) X->height=-1
static struct avl_node* create_node(struct stack_cell a,struct stack_cell b){
  struct avl_node* result=malloc(sizeof(struct avl_node));
  result->key=a;
  result->value=b;
  result->height=-1;
  result->right=NULL;
  result->left=NULL;
  result->parent=NULL;
  return result;
}
void free_avl_node(struct avl_node* n){
  free_stack_cell(n->key);
  free_stack_cell(n->value);
  free(n);
}
int avl_node_height(struct avl_node* node)
{
  int height_left,height_right;
  if(node->left){
    if(node->left->height==-1){
      height_left=avl_node_height(node->left);
      node->left->height=height_left;
    }else{
      height_left=node->left->height;
    }
  }
  if(node->right){
    if(node->right->height==-1){
      height_right=avl_node_height(node->right);
      node->right->height=height_right;
    }else{
      height_right=node->right->height;
    }
  }
  node->height=1+(height_left>height_right?height_left:height_right);
  return node->height;
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
  b->parent=a->parent;
  a->parent=b;
  clear_height(a);
  clear_height(b);
  return b;
}
struct avl_node* avl_rotate_leftright(struct avl_node* node){
  struct avl_node* a=node,
    *b=a->left,
    *c=b->right;
  a->left=c->right;
  c->left->parent=a;
  b->right=c->left;
  b->right->parent=b;
  c->left=b;
  b->parent=c;
  c->right=a;
  c->parent=a->parent;
  a->parent=c;
  clear_height(a);
  clear_height(b);
  clear_height(c);
  return c;
}
struct avl_node* avl_rotate_rightleft(struct avl_node* node){
  struct avl_node *a=node,*b=a->right,*c=b->left;
  a->right=c->left;
  if(a->right)
    a->right->parent=a;
  b->left=c->right;
  if(b->left)
    b->left->parent=b;
  c->right=b;
  b->parent=c;
  c->left=a;
  c->parent=a->parent;
  a->parent=c;
  clear_height(a);
  clear_height(b);
  clear_height(c);
  return c;
}
struct avl_node* avl_rotate_rightright(struct avl_node* node){
  struct avl_node *a=node,*b=a->right;
  a->right=b->left;
  a->right->parent=a;
  b->left=a;
  b->parent=a->parent;
  a->parent=b;
  clear_height(a);
  clear_height(b);
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
void avl_insert(struct avl_tree* tree,struct stack_cell key, struct stack_cell value){
  if(tree->root==NULL){
    tree->root=create_node(key,value);
  }else{
    struct avl_node *next=tree->root,*last;
    int lastdir;
    while(next!=NULL){
      last=next;
      next->height=-1;
      switch(stack_cell_cmp(key,next->key)){
        case -1:
          next=next->left;
          lastdir=-1;
          break;
        case 1:
          next=next->right;
          lastdir=1;
          break;
        case 0:
          free_stack_cell(next->value);
          next->value=value;
          return;
      }
    }
    if(lastdir==1)
      last->right=create_node(key,value);
    else 
      last->left=create_node(key,value);
  }
  avl_balance(tree);
}
void avl_replace_node_in_parent(struct avl_node* node, struct avl_node* replacement){
  if(node->parent){
    if(node->parent->left==node){
      node->parent->left=replacement;
    }else{
      node->parent->right=replacement;
    }
    if(replacement){
      replacement->parent=node->parent;
      replacement->height=-1;
    }
    free_avl_node(node);
  }
}
struct avl_node* find_min(struct avl_node* n){
  while(n->left){
    n=n->left;
  }
  return n;
}
int delete_avl_node(struct avl_node *n, struct stack_cell key){
  if(!n)
    return 0;
  n->height=-1;
  switch(stack_cell_cmp(n->key,key)){
    case 1:
      if(delete_avl_node(n->right,key)){
        avl_balance_node(n->right);
        return 1;
      }
      return 0;
    case -1:
      if(delete_avl_node(n->left,key)){
        return 1;
      }
      return 0;
  }
  if(n->left&&n->right){
    struct avl_node *successor=find_min(n);
    free_stack_cell(n->key);
    free_stack_cell(n->value);
    n->key=copy_stack_cell(successor->key);
    n->value=copy_stack_cell(successor->value);
    delete_avl_node(successor,successor->key);
  }else if(n->left){
    avl_replace_node_in_parent(n,n->left);
  }else if(n->right){
    avl_replace_node_in_parent(n,n->right);
  }else{
    avl_replace_node_in_parent(n,NULL);
  }
  return 1;
}
void avl_delete(struct avl_tree* tree,struct stack_cell key){
  delete_avl_node(tree->root,key);
}
struct stack_cell* find_avl_value(struct avl_tree *tree, struct stack_cell key){
  struct avl_node *n=tree->root;
  while(n){
    switch(stack_cell_cmp(key,n->key)){
      case 0:
        return &n->value;
      case 1:
        n=n->right;
        break;
      case -1:
        n=n->left;
        break;
    }
  }
  return NULL;
}
