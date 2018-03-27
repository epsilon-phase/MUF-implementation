#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "token.h"
// return the end of the match, -1 for not a word
int glob_word(const char* input, int start) {
  int end = start;
  if (!input[end]) return -1;
  while (input[end] &&
         (input[end] != '"' && !isspace(input[end]) && input[end] != ':'&&input[end]!=';')) {
    end++;
  }
  if (end == start) return -1;
  return end-1 ;
}
// Same as above, but different.
int glob_integer(const char* input, int start) {
  int end = start;
  while (input[end] && isdigit(input[end])) {
    end++;
  }
  if (end == start) return -1;
  switch (input[end]) {
    case '\n':
    case ' ':
    case '\r':
    case '\t':
    case 0:
      return end - 1;
    default:
      return -1;
  }
}
int glob_float(const char* input, int start) {
  int end = start;
  int seen_period = 0;
  while (input[end] &&
         (isdigit(input[end]) || (!seen_period && input[end] == '.'))) {
    if (input[end] == '.') {
      seen_period = 1;
    }
    end++;
  }
  if (end == start) return -1;
  return isspace(input[end]) ? end - 1 : -1;
}
struct tokenlist* lexer(const char* input) {
  struct tokenlist* start = malloc(sizeof(struct tokenlist));
  struct tokenlist* current_tok = start;
  size_t current_position = 0;
  char buffer[64];
  memset(buffer, 0,64);
  int bufferpos = 0;
  int curstart = 0;
  int line = 0;
  int offset = 0;
  int current_guess;
  int instring = 0;
  int escaping = false;
  char curchar;
  while ((curchar = input[current_position]) != 0) {
    if (isspace(curchar) && !instring && current_guess != LEXER_NOTHING) {
      if (current_guess!=-1) {
        struct token* thistoken = malloc(sizeof(struct token));
        thistoken->name = calloc(2 + bufferpos, sizeof(char));
        memcpy(thistoken->name, buffer, bufferpos + 1);
        thistoken->name[bufferpos + 1] = 0;  // terminate the string
        thistoken->type = current_guess;
        thistoken->line = line;
        thistoken->end = offset;
        thistoken->start = curstart;
        memset(buffer, 0, bufferpos);
        bufferpos = 0;
        current_tok->token = thistoken;
        current_tok->next = malloc(sizeof(struct tokenlist));
        current_tok = current_tok->next;
        current_tok->next = NULL;
      }
      current_guess = -1;
    }
    if (instring) {
      if (escaping) {
        switch (curchar) {
          case '"':
          case '\\':
            buffer[bufferpos++] = curchar;
            break;
          case 'n':
            buffer[bufferpos++] = '\n';
            break;
          case 'r':
            buffer[bufferpos++] = '\r';
            break;
          case 't':
            buffer[bufferpos++] = '\t';
            break;
        }
      } else {  // Not escaping the next character;
        if (curchar == '"') {
          instring = false;
        } else
          buffer[bufferpos++] = curchar;
      }
    } else {
      if (curchar == '"') {
        current_guess = LEXER_STRING;
        curstart = current_position;
        instring = true;
      } else if (curchar == '\n') {
        line++;
        offset = 0;
      } else {
        int globbed;
        curstart = current_position;
        globbed = glob_integer(input, current_position);
        if (globbed != -1) {
          current_guess = LEXER_INT;
          memcpy(buffer, input + curstart, 1+globbed - curstart);
        } else if ((globbed = glob_float(input, current_position)) != -1) {
          current_guess = LEXER_DOUBLE;
          memcpy(buffer, input + curstart, globbed - curstart);
        } else if ((globbed = glob_word(input, curstart)) != -1) {
          current_guess = LEXER_WORD;
          memcpy(buffer, input + curstart, 1+globbed - curstart);
        } else if (curchar == ':') {
          current_guess = LEXER_FUNC_START;
          buffer[0] = ':';
          bufferpos = 0;
        } else if (curchar == ';') {
          current_guess = LEXER_FUNC_END;
          buffer[0] = ';';
          bufferpos = 0;
        } else if(!isspace(curchar)) {
          current_guess = LEXER_INVALID;
          printf("Unknown token (%c): at %d, %d\n",curchar,offset,line);
        }
        if (globbed != -1) {
          bufferpos = globbed - curstart;
          current_position = globbed;
          offset=globbed;
        }
      }
    }
    ++current_position;
    ++offset;
  }
  if (current_guess!=-1) {
    struct token* thistoken = malloc(sizeof(struct token));
    thistoken->name = calloc(2 + bufferpos, sizeof(char));
    memcpy(thistoken->name, buffer, bufferpos + 1);
    thistoken->name[bufferpos + 1] = 0;  // terminate the string
    thistoken->type = current_guess;
    thistoken->line = line;
    thistoken->end = offset;
    thistoken->start = curstart;
    memset(buffer, 0, bufferpos);
    bufferpos = 0;
    current_tok->token = thistoken;
    current_tok->next = NULL;
  }
  post_process_lexer(start);
  return start;
}
void tokenlist_print(struct tokenlist* start) {
  while (start != NULL && start->token != NULL) {
    switch (start->token->type) {
      case LEXER_INT:
        printf("INTEGER:");
        break;
      case LEXER_DOUBLE:
        printf("DOUBLE:");
        break;
      case LEXER_STRING:
        printf("STRING:");
        break;
      case LEXER_WORD:
        printf("WORD:");
        break;
      case LEXER_FUNC_START:
        printf("FUNC_START:");
        break;
      case LEXER_FUNC_END:
        printf("FUNC_END:");
        break;
      case LEXER_IF:
        printf("IF:");
        break;
      case LEXER_ELSE:
        printf("ELSE:");
        break;
      case LEXER_THEN:
        printf("THEN:");
        break;
      default:
        printf("UNKNOWN:");
        break;
    }
    printf(" %s (%d,%d)\n", start->token->name, start->token->start,
           start->token->line);
    start = start->next;
  }
}
void tokenlist_free(struct tokenlist* start){
  while(start){
    struct tokenlist *tmp=start->next;
    struct token* tok=start->token;
    if(tok){
      if(tok->name)
        free(tok->name);
      free(start->token);
    }
    free(start);
    start=tmp;
  }
}
void post_process_lexer(struct tokenlist* start){
  while(start){
    struct token *current=start->token;
    start=start->next;
    if(!current){
        continue;
    }
    int i=0;
    while(current->name[i]){
        current->name[i]=tolower(current->name[i]);
        i++;
    }
    if(!strcmp(current->name,"if")){
        current->type=LEXER_IF;
    }else if(!strcmp(current->name,"else")){
        current->type=LEXER_ELSE;
    }else if(!strcmp(current->name,"then")){
        current->type=LEXER_THEN;
    }
  }
}
void free_token(struct token *token){
    if(!token)
        return;
    if(token->name)
        free(token->name);
    free(token);
}
struct token* create_token(){
    struct token *result=malloc(sizeof(struct token));
    result->type=0;
    result->start=0;
    result->end=0;
    result->name=NULL;
    return result;
}
struct token * copy_token(struct token *token){
    struct token* result=create_token();
    result->name=malloc(strlen(token->name)+1);
//    printf("%s\n",token->name);
    strcpy(result->name,token->name);
    result->type=token->type;
    result->end=token->end;
    result->start=token->start;
    result->line=token->line;
    return result;
}
