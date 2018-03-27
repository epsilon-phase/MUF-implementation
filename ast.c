#include "ast.h"
#include <string.h>
#include <stdlib.h>
struct ast_node* create_ast_node(){
    struct ast_node* result=malloc(sizeof(struct ast_node));
    result->children=NULL;
    result->immediate=NULL;
    result->childcount=0;
    result->immediatecount=0;
    return result;
}
void add_child(struct ast_node *parent,struct ast_node* child){
    if(!parent)return;
    if(parent->children){
        realloc(parent->children,sizeof(struct ast_node*)*(parent->childcount+1));
    }else{
        parent->children=malloc(sizeof(struct ast_node *));
    }
    parent->children[parent->childcount]=child;
    parent->childcount++;

}
void remove_child(struct ast_node *parent,int index){
    
}
void free_ast_node(struct ast_node* item){
    if(!item)
        return;
    int i=0;
    for(i=0;i<item->childcount;i++){
        free_ast_node(item->children[i]);
    }
    for(i=0;i<item->immediatecount;i++){
        free_token(item->immediate[i]);
    }
    free(item);
}
struct ast_node* parse_function(struct tokenlist *start){
    struct ast_node* result=malloc(sizeof(struct ast_node*));
    
}
