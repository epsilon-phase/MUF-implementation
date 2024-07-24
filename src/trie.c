#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include <stdarg.h>
#include <stdio.h>

void free_trie(struct trie* t){
  for(int i=0;i<UCHAR_MAX;i++){
    if(t->children[i]){
      free_trie(t->children[i]);
    }
  }
  free(t);
}

struct trie* create_trie(){
  struct trie* result=malloc(sizeof(struct trie));
  memset(result->children,0,sizeof(struct trie*)*UCHAR_MAX);
  return result;
}
void add_to_trie(struct trie* t,const char* key,int value){
  if(key[0]){
    if(!t->children[key[0]]){
      t->children[key[0]]=create_trie();
    }
    add_to_trie(t->children[key[0]],key+1,value);
  }else{
    t->value=value;
  }   
}

int get_value(struct trie* t,const char* c){
  while(c[0]){
    if(!t)return -1;
    t=t->children[*(c++)];
  }
  return t->value;
}
void add_many_to_trie(struct trie* t,...){
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
int childcount(struct trie* t){
  if(!t)return 0;
  int r=0;
  for(int i=0;i<UCHAR_MAX;i++)if(t->children[i])r++;
  return r;
}
int dump_t(struct trie* t,FILE* f,int n,int g){
  int nn=g,first=1,order[UCHAR_MAX],ids[UCHAR_MAX],filled=0;
  memset(order,0,sizeof(int)*UCHAR_MAX);
  fprintf(f,"struct%d [label=\"",n);
  fprintf(f,"{{<top> struct%d\n}|{",n);
  for(int i=0;i<UCHAR_MAX;i++){
    if(t->children[i]){
      if(!first){
        fprintf(f,"|");
      }else{first=0;}
      order[filled]=i;
      ids[filled]=++nn;
      fprintf(f,"<f%i> &#%i;",i,33+i);
      filled++;
    }
  }
  fprintf(f,"}}\"]\n");
  for(int i=0;i<filled;i++){
    if(childcount(t->children[order[i]])){
      nn=dump_t(t->children[order[i]],f,ids[i],nn);
      fprintf(f,"struct%i:f%i -> struct%i:top\n",n,order[i],ids[i]);
    }
  }
  return nn;
}
void dump(struct trie* t,const char* file){
  FILE* f=fopen(file,"w");
  fprintf(f,"digraph T{\n");
  fprintf(f,"node [shape=record];\n");
  dump_t(t,f,0,1);
  fprintf(f,"}");
  fclose(f);
}
