#include <ctype.h>
#include <libgccjit.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <error.h>
#include "token.h"
#include "ast.h"
#include "options.h"
#include "interpreter.h"
#include "frame.h"
#include "version.h"
/**
 *  Inst type Prototype(since the JIT doesn't make it easy to get a normal C
 *struct in there)
 *struct variable;
 * struct inst{
 *  int type;
 *  union{
 *    int number;
 *    double fnumber;
 *    const char* string;
 *    variable* var;
 *  } data;
 *};
 * struct variable{
 *  inst *value;
 *  const char* name;
 * };
 * struct scope{
 *  scope * parent;
 *  int vcount;
 *  variable** vars;
 *  int scount;
 *  scope **scopes;
 *}
 */
#define C_INT_T 1
#define C_DOUBLE_T 2
#define C_STRING_T 3
#define MAX_STACK_DEPTH 32
char *read_file(const char* file);
const char *argp_program_version=MUFJIT_VERSION;
const char *argp_program_bug_address="<alexwhite42000@gmail.com>";
static char args_doc[]="FILE1 [input file] ARGUMENTS [passed in on the stack]";
static char doc[]=
  "MUFJIT a MUF interpreter for the system, and one day, a JIT system/compiler for it.\
   Written without reference to anything but the Protomuck documentation.\
   \vHexaferrum (c) 2018";
static struct argp_option options[]=
  {
    {"print-bytecode",'p',0,0,"Print a representation of the bytecode to the terminal",2},
    {"print-stack",   's',0,0,"Print the stack after each instruction.",2},
    {"tests",         't',0,0,"Run the tests.",2},
    {"log-output",    'l',"OUTPUT",0,"Write program output/stack information to a file.", 0}
  }
;
struct arguments{
  char *file;
  char **other_files;
  int print_bytecode,
      print_stack,
      run_tests;
};
static error_t parse_opt(int key,char* arg, struct argp_state *state);
static struct argp argp={.options=options,.parser=parse_opt,.args_doc=args_doc,.doc=doc};
int main(int argc, char** argv) {
  struct arguments arguments={.file=NULL,.other_files=NULL,.print_bytecode=0,.print_stack=0,.run_tests=0};
  argp_parse(&argp,argc,argv,0,0,&arguments);
  
  return 0;
}
char *read_file(const char* fn){
    char *output=NULL;;
    FILE *f;
    f=fopen(fn,"rb");
    if(!f){
        fprintf(stderr,"Failed to open file %s",fn);
    }else{
    fseek(f,0,SEEK_END);
    long long fsize=ftell(f);
    fseek(f,0,SEEK_SET);
    output=malloc(sizeof(char)*(fsize+1));
    memset(output,0,fsize+1);
    fread(output,sizeof(char),fsize,f);
    fclose(f);
    output[fsize]=0;
    printf("File of %lld bytes read.\n",fsize);
    }
    return output;
}
static error_t parse_opt(int key,char* arg, struct argp_state *state){
  struct arguments *arguments=state->input;
  switch(key){
    case 'p':
      arguments->print_bytecode=1;
      break;
    case 's':
      arguments->print_stack=1;
      break;
    case 't':
      arguments->run_tests=1;
      break;
    case ARGP_KEY_NO_ARGS:
      break;
    case ARGP_KEY_ARG:
      arguments->file=arg;
      arguments->other_files=&state->argv[state->next];
      state->next=state->argc;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
