#include "stack_cell.h"
#include "avl_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
void free_array(struct array* arr){
    if(arr->packed){
      for(unsigned int i=0;i<arr->size;i++){
        free_stack_cell(arr->data.packed[i]);
      }
      free(arr->data.packed);
    }else{
      avl_free(arr->data.dictionary);
      free(arr->data.dictionary);
    }
    free(arr);
}
struct array* create_array(struct stack_cell *stack_segment,size_t count,int is_dict){
  struct array *result=malloc(sizeof(struct array));
  result->packed=!is_dict;
  result->size=count;
  result->links=1;
  if(result->size){
    if(!is_dict){
      result->data.packed=malloc(sizeof(struct stack_cell)*count);
      for(unsigned int i=0;i<count;i++){
        result->data.packed[i]=copy_stack_cell(stack_segment[i]);
      }
    }else{
      result->data.dictionary=malloc(sizeof(struct avl_tree));
      result->data.dictionary->root=NULL;
      for(unsigned int i=0;i<count;i++){
        struct stack_cell key=stack_segment[0],
                          value=stack_segment[1];
        stack_segment+=2;
        result->size+=avl_insert(result->data.dictionary,key,value);
      }
    }
  }else{
    if(!is_dict)
      result->data.packed=NULL;
    else{
      result->data.dictionary=malloc(sizeof(struct avl_tree));
      result->data.dictionary->root=NULL;
    }
  }
  return result;
}
struct stack_cell array_get_element(struct array *array,struct stack_cell key){
  struct stack_cell result;
  if(array->packed){
    switch(key.type){
      case t_int:
        if(key.data.number>=array->size){
          result.type=t_int;
          result.data.number=0;
        }else{
          result=copy_stack_cell(array->data.packed[key.data.number]);
        }
        break;
      case t_string:
        result.type=t_int;
        result.data.number=0;
        break;
    }
  }else{
    struct stack_cell* potential=find_avl_value(array->data.dictionary,key);
    if(potential){
      result=copy_stack_cell(*potential);
    }else{
      result.type=t_int;
      result.data.number=0;
    }
  }
  return result;
}
struct array* copy_array(struct array* original){
  struct array* result=malloc(sizeof(struct array));
  result->packed=original->packed;
  if(result->packed){
    result->data.packed=malloc(sizeof(struct stack_cell)*original->size);
    for(unsigned i=0;i<original->size;i++){
      result->data.packed[i]=copy_stack_cell(original->data.packed[i]);
    }
  }else{
    result->data.dictionary=copy_tree(original->data.dictionary);
  }
  result->size=original->size;
  result->links=0;
  return result;
}
struct array* set_array_item(struct stack_cell value, struct array* array,struct stack_cell key){
  struct array *result;
  if(array->links>1){
    result=copy_array(array);
  }else{
    result=array;
  }
  if(array->packed){
    if(key.type!=t_int){//Error somehow
    }else{
      if(result->size==key.data.number){
        if(result->data.packed){
          struct stack_cell *tmp
            =malloc(sizeof(struct stack_cell)*(result->size+1));
          memcpy(tmp,result->data.packed,result->size*sizeof(struct stack_cell));
          free(result->data.packed);
          result->data.packed=tmp;
        }else{
          result->data.packed=malloc(sizeof(struct stack_cell));
        }
        result->data.packed[key.data.number]=copy_stack_cell(value);
        result->size++;
      }else if(result->size>key.data.number){
        free_stack_cell(result->data.packed[key.data.number]);
        result->data.packed[key.data.number]=copy_stack_cell(value);
      }
    }
  }else{
      result->size+=avl_insert(result->data.dictionary,key,value);
  }
  return result;
}
struct array* delete_array_item(struct array* array, struct stack_cell key){
  struct array* result;
  if(array->links>1){
    result=copy_array(array);
  }else{
    result=array;
  }
  if(result->packed){
    if(key.data.number<result->size){
      free_stack_cell(result->data.packed[key.data.number]);
    }
    for(unsigned int i=key.data.number;i<result->size-1;i++){
      result->data.packed[i]=result->data.packed[i+1];
    }
    result->data.packed=realloc(result->data.packed,result->size-1);
    result->size--;
  }else{
    result->size-=avl_delete(array->data.dictionary,key);
  }
  return result;
}
void print_array(struct array* arr){
  printf("%d{",arr->size);
  if(arr->packed){
    for(int i=0;i<arr->size;i++){
      if(i>0)
        printf(",");
      print_stack_cell(&arr->data.packed[i]);
    }
  }else{
    struct avl_iterator iterator=create_iterator(arr->data.dictionary);
    struct avl_node* node=NULL;
    int n=0;
    while((node=next(&iterator))){
      if(n++){
        printf(",");
      }
      printf("(");
      print_stack_cell(&node->key);
      printf(",");
      print_stack_cell(&node->value);
      printf(")");
    }
  }
  printf("}");
}
void dump_array(struct array* arr,const char* fn){
  if(!arr->packed){
    pid_t proc=fork();
    if(!proc){
      dump_avl_tree(arr->data.dictionary,fn);
      char *buffer=malloc(strlen("dot -Tpng -O ")+strlen(fn)+1);
      memset(buffer,0,strlen(fn)+1);
      sprintf(buffer,"dot -Tpng -O %s",fn);
      system(buffer);
      exit(0);
    }
  }
}
struct stack_cell sum_array(struct array* arr){
  struct stack_cell result={.type=t_int,.data.number=0};
  if(arr->packed){
    for(int i=0;i<arr->size;i++){
      switch(arr->data.packed[i].type){
        case t_int:
          if(result.type==t_int)
            result.data.number+=arr->data.packed[i].data.number;
          else
            result.data.fnumber+=arr->data.packed[i].data.fnumber;
          break;
        case t_float:
          if(result.type==t_int){
            result.type=t_float;
            result.data.fnumber+=result.data.number+arr->data.packed[i].data.fnumber;
          }else{
            result.data.fnumber+=arr->data.packed[i].data.fnumber;
          }
          break;
      }
    }
  }
  return result;
}
