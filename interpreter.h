#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "token.h"
#include "ast.h"
#define MAX_CALL_STACK 1000
#define MAX_DATA_STACK 1000
#define MAX_VARIABLES 100
struct program;
struct stack_cell;
struct instruction;
struct instruction_pointer;
struct scope_vars;
struct variables;
struct lvariables;
struct function;
struct frame{
    size_t stack_depth;
    size_t call_stack_depth;
    size_t variable_count;
    struct instruction_pointer *callstack[MAX_CALL_STACK];
    struct scope_vars *scope_stack[MAX_CALL_STACK];
};

struct program{
    struct instruction *bytecode;
    size_t bytecode_length;
    struct function *functions;
    size_t function_address;
    struct variables *gvars;
    size_t gvar_count;
    struct lvariables *lvars;
    size_t lvar_count;
};
struct stack_cell{
    enum {
        int_cell,
        string_cell,
        float_cell,
        svar_cell,
        lvar_cell,
        address_cell,
        mark,
        array_cell
    }cell_type;
    union{
        int number;
        double fnumber;
        const char *string;
        struct stack_cell *scope_variable;
        struct instruction_pointer* address;
    }data;
};
struct instruction{
    enum{
        push_primitive,
        jmp_if,
        jmp,
        pop,
        popn,
        dup,
        dupn,
        intostr,
        m_atoi,
        stof,
        stod,
        cmp_gt,
        cmp_ge,
        cmp_lt,
        cmp_le,
        cmp_eq,
        cmp_strcmp,
        cmp_instr,
        m_tolower,
        m_toupper,
        explode,
        reverse,
        rotate,
        pick,
        over,
        tuck,
        dereference,
        assign
    }type;
    union{
        size_t jmp_offset;
        struct stack_cell* cell;
    } data;
};
struct instruction_pointer{
    struct program* exec_context;
    size_t position;
};
struct scope_vars{

};
struct variable{
};
struct program* compile_program(struct tokenlist* tl);
struct instruction* allocate_instruction();
#endif
