#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
struct program* parse_from_token_list(struct tokenlist* tl){
    struct tokenlist *current_token=tl;
    struct instruction* current=allocate_instruction();

}

struct instruction* allocate_instruction(){
    struct instruction *result=malloc(sizeof(struct instruction));
    return result;
}
