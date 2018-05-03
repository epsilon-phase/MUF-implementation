#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include <stdarg.h>
void free_trie(struct trie* t){
  for(int i=0;i<93;i++){
    if(t->children[i]){
      free_trie(t->children[i]);
      free(t->children[i]);
    }
  }
}

struct trie* create_trie(){
  struct trie* result=malloc(sizeof(struct trie));
  memset(result->children,0,sizeof(struct trie*)*93);
  return result;
}
void add_to_trie(struct trie* t,const char* key,int value){
  if(key[1]){
    if(!t->children[key[0]-33]){
      t->children[key[0]-33]=create_trie();
    }
    add_to_trie(t->children[key[0]-33],key+1,value);
  }else{
    t->value=value;
  }   
}

int get_value(struct trie* t,const char* c){
  while(c[1]){
    if(!t)return -1;
    t=t->children[*(c++)-33];
  }
  return t->value;
}
void add_many(struct trie* t,...){
  va_list args;
  va_start(args,t);
  const char* n=va_arg(args,const char*);
  if(t){
    while(n){
      int val=va_arg(args,int);
      add_to_trie(t,n,val);
      n=va_arg(args,const char*);
    }
  }
  va_end(args);
}
