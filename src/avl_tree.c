#include <stdlib.h>
#include "avl_tree.h"
#define clear_height(X) X->height=-1
#include <stdio.h>
#include <assert.h>
static size_t balanced_count=0;
static struct avl_node* create_node( struct stack_cell a,struct stack_cell b){
  struct avl_node* result=malloc(sizeof(struct avl_node));
  result->key=copy_stack_cell(a);
  result->value=copy_stack_cell(b);
  result->height=1;
  result->right=NULL;
  result->left=NULL;
  result->parent=NULL;
  return result;
}
void free_avl_node(struct avl_node* n){
  if(!n)return;
  free_stack_cell(n->key);
  free_stack_cell(n->value);
  free_avl_node(n->right);
  free_avl_node(n->left);
  free(n);
}
void avl_free(struct avl_tree* tree){
  free_avl_node(tree->root);
}
int avl_node_height(struct avl_node* node)
{
  if(!node)return 0;
  return node->height;
  //int height_left=0,height_right=0;
  //if(node->left){
  //  if(node->left->height==-1){
  //    height_left=avl_node_height(node->left);
  //    node->left->height=height_left;
  //  }else{
  //    height_left=node->left->height;
  //  }
  //}
  //if(node->right){
  //  if(node->right->height==-1){
  //    height_right=avl_node_height(node->right);
  //    node->right->height=height_right;
  //  }else{
  //    height_right=node->right->height;
  //  }
  //}
  //node->height=1+(height_left>height_right?height_left:height_right);
  //return 1+(height_left>height_right?height_left:height_right);
}
int avl_max(int a,int b){return a>b?a:b;}
void avl_update_height(struct avl_node* node){
  node->height=avl_max(avl_node_height(node->left),avl_node_height(node->right))+1;
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
  avl_update_height(a);
  avl_update_height(b);
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
  avl_update_height(a);
  avl_update_height(b);
  avl_update_height(c);
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
  avl_update_height(a);
  avl_update_height(b);
  avl_update_height(c);
  return c;
}
struct avl_node* avl_rotate_rightright(struct avl_node* node){
  struct avl_node *a=node,*b=a->right;
  assert(a&&b);
  a->right=b->left;
  if(a->right)
  a->right->parent=a;
  b->left=a;
  b->parent=a->parent;
  a->parent=b;
  avl_update_height(a);
  avl_update_height(b);
  return b;
}
struct avl_node* avl_balance_node(struct avl_node* node){
  struct avl_node* newroot=NULL;
  balanced_count++;
  struct avl_node* tmp_parent=node->parent;
  if(node->left)
    node->left=avl_balance_node(node->left);
  if(node->right)
    node->right=avl_balance_node(node->right);
  int bf=avl_balance_factor(node);
  if(bf>=1){
    if(avl_balance_factor(node->left)<=-1)
      newroot=avl_rotate_leftright(node);
    else
      newroot=avl_rotate_leftleft(node);
  }else if(bf <=-1){
    if(avl_balance_factor(node->right)>=1)
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
struct avl_node* insert_node(struct avl_node* root,struct stack_cell key, struct stack_cell value,int *inserted){
  if(root==NULL){
    *inserted=1;
    return create_node(key,value);
  }
  switch(stack_cell_cmp(key,root->key)){
    case -1:
      root->left=insert_node(root->left,key,value,inserted);
      root->left->parent=root;
      break;
    case 1:
      root->right=insert_node(root->right,key,value,inserted);
      root->right->parent=root;
      break;
    case 0:
      free_stack_cell(root->value);
      root->value=value;
      *inserted=0;
      return root;
  }
  int hl=avl_node_height(root->left),hr=avl_node_height(root->right);
  root->height=1+(hl>hr?hl:hr);
  int balance=avl_balance_factor(root);
  if(balance>=2){
    //if(avl_balance_factor(root->left)<= -1)
    if(stack_cell_cmp(key,root->left->key)==1)
      return avl_rotate_leftright(root);
    else if(stack_cell_cmp(key,root->left->key)==-1)
      return avl_rotate_leftleft(root);
  }else if(balance <= -2){
//    if(avl_balance_factor(root->right)>=1)
    if(stack_cell_cmp(key,root->right->key)==1)
      return avl_rotate_rightright(root);
    else if(stack_cell_cmp(key,root->right->key)==-1)
    return avl_rotate_rightleft(root);
  }
  return root;
}
int avl_insert(struct avl_tree* tree,struct stack_cell key, struct stack_cell value){
  int inserted=0;
  tree->root=insert_node(tree->root,key,value,&inserted);
  return inserted;
  //Prior non-in-place balanced
#if 0
  if(tree->root==NULL){
    tree->root=create_node(key,value);
    inserted=1;
  }else{
    struct avl_node *next=tree->root,*last;
    int lastdir;
    while(next!=NULL){
      last=next;
      //next->height=-1;
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
          return 0;
      }
    }
    if(lastdir==1){
      last->right=create_node(key,value);
      last->right->parent=last;
      inserted=1;
    }else{
      last->left=create_node(key,value);
      last->left->parent=last;
      inserted=1;
    }
    if(inserted){
      while(last){
        last->height=-1;
        last=last->parent;
      }
    }
  }
  avl_balance(tree);
  return inserted;
#endif
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
int avl_delete(struct avl_tree* tree,struct stack_cell key){
  return delete_avl_node(tree->root,key);
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
void free_avl_nodes(struct avl_node* tree){
  if(tree->right){
    free_avl_nodes(tree->right);
  }
  if(tree->left)
    free_avl_nodes(tree->left);
  free_avl_node(tree);
}
void free_avl_tree(struct avl_tree* tree){
  if(tree->root)
    free_avl_nodes(tree->root);
  free(tree);
}
struct avl_node* copy_node(struct avl_node* original){
  if(!original)return NULL;
  struct avl_node* result=malloc(sizeof(struct avl_node));
  result->key=copy_stack_cell(original->key);
  result->value=copy_stack_cell(original->value);
  result->right=copy_node(original->right);
  result->left=copy_node(original->left);
  result->height=original->height;
  return result;
}
struct avl_tree* copy_tree(struct avl_tree* original){
  struct avl_tree* result=malloc(sizeof(struct avl_tree));
  result->root=copy_node(original->root);
  return result;
}
static void iterator_push(struct avl_iterator* iterator, struct avl_node* node){
  while(iterator->size==iterator->capacity){
    iterator->capacity*=2;
    iterator->current=realloc(iterator->current,iterator->capacity);
  }
  iterator->current[iterator->size++]=node;
}
static struct avl_node* iterator_pop(struct avl_iterator* iterator){
  return iterator->current[--iterator->size];
}
struct avl_iterator create_iterator(struct avl_tree* tree){
  struct avl_iterator result;
  struct avl_node* current=tree->root;
  result.size=0;
  result.capacity=10;
  result.current=malloc(sizeof(struct avl_node**)*10);
  while(result.size||current){
    if(current!=NULL){
      iterator_push(&result,current);
      current=current->left;
    }else{
      //Not here...
      break;
    }
  }
  result.n=NULL;
  return result;
}
struct avl_node* next(struct avl_iterator* iter){
  struct avl_node* n=iter->n;
  while(iter->size||n){
    if(n){
      iterator_push(iter,n);
      iter->n=n->left;
      n=n->left;
    }else{
      n=iterator_pop(iter);
      iter->n=n->right;
      return n;
    }
  }
  free(iter->current);
  return NULL;
}
void dump_node_label(struct avl_node* node,FILE *f){
  switch(node->key.type){
    case t_int:
      fprintf(f,"%d",node->key.data.number);
      break;
    case t_string:
      fprintf(f,"\"\\\"%s\\\"\"",node->key.data.str->str);
      break;
  }
}
void dump_node_label_extra_escaped(struct avl_node* node, FILE* f){
  switch(node->key.type){
    case t_int:dump_node_label(node,f);break;
    case t_string:
               fprintf(f,"\\\"%s\\\"",node->key.data.str->str);
               break;
  }
}
int dump_avl_node(struct avl_node* node, FILE* f,int n){
  n++;
  int nn,print_which_graphs=0;
  fprintf(f,"node [label=\"<f0> | <f1> ");dump_node_label_extra_escaped(node,f);
  fprintf(f,"|<f2> \", fillcolor=\"/spectral11/%d\"]",n+1);
  dump_node_label(node,f);fprintf(f,";\n");
  if(node->left){
    nn=dump_avl_node(node->left,f,n);
    dump_node_label(node,f);
    fprintf(f,":f0 -> ");
    dump_node_label(node->left,f);
    fprintf(f,":f1");
    fprintf(f,"[label = l]\n");
    print_which_graphs=1;
//    fprintf(f,"%d->%d;\n",n,nn);
  }else nn=n;
  if(node->right){
    nn=dump_avl_node(node->right,f,n);
    dump_node_label(node,f);
    fprintf(f,":f2");
    fprintf(f," -> ");
    dump_node_label(node->right,f);
    fprintf(f,":f1");
    fprintf(f,"[label = r] \n");
    print_which_graphs|=1<<1;
//    fprintf(f,"%d->%d;\n",n,nn);
  }
  return n;
}
void reset_heights(struct avl_node* n){
  if(n){
    n->height=-1;
    reset_heights(n->left);
    reset_heights(n->right);
  }
}
void dump_avl_tree(struct avl_tree* tree,const char* fn){
  if(tree){
    FILE* f=fopen(fn,"w");
    fprintf(f,"digraph T{\nnode [shape=record,style=filled];\n");
    dump_avl_node(tree->root,f,0);
    fprintf(f,"}");
    fclose(f);
  }
}
