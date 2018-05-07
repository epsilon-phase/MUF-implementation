#include "interpreter.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
struct position_stack {
  size_t *position;
  size_t capacity;
  size_t size;
};
struct block {
  enum { bs_if, bs_else, bs_begin, bs_for, bs_foreach } type;
  size_t position;
};
struct block_stack {
  struct block *stuff;
  size_t size, capacity;
};
struct block_stack *create_block_stack();
void push_block(struct block_stack *stack, int type, size_t pos);
struct block pop_block(struct block_stack *stack);
struct block peek_block(struct block_stack *stack);
void free_block_stack(struct block_stack *s);
struct position_stack *create_position_stack();
void push_position_stack(struct position_stack *s, size_t pos);
size_t pop_position_stack(struct position_stack *s);
size_t peek_position_stack(struct position_stack *s);
void free_position_stack(struct position_stack *s);
struct instruction instruction_from_i(int type);
void close_loop(struct program *, struct block, struct position_stack *);
static void add_line_range(struct program *prog, struct token *token) {
  struct instruction_range *r = NULL;
  if (!prog->range_count) {
    prog->lines = malloc(sizeof(struct instruction_range));
    prog->lines[0].start = prog->bytecode_size;
    prog->lines[0].end = prog->bytecode_size;
    prog->lines[0].line = token->line;
    prog->range_count++;
  }
  r = &prog->lines[prog->range_count - 1];
  if (r->line != token->line) {
    prog->lines = realloc(prog->lines, (sizeof(struct instruction_range) *
                                        (++prog->range_count)));
    r = &prog->lines[prog->range_count - 1];
    r->start = prog->bytecode_size;
    r->end = prog->bytecode_size;
    r->line = token->line;
  } else {
    r->end = prog->bytecode_size;
  }
}
void add_instruction(struct program *prog, struct instruction i) {
  if (!prog) return;
  if (!prog->bytecode) {
    prog->bytecode = calloc(100, sizeof(struct instruction));
    prog->bytecode_capacity = 10;
  }
  if (prog->bytecode_size == prog->bytecode_capacity - 1) {
    prog->bytecode = realloc(prog->bytecode, sizeof(struct instruction) *
                                                 prog->bytecode_capacity * 2);
    prog->bytecode_capacity *= 2;
  }
  // memcpy(&prog->bytecode[prog->bytecode_size], i, sizeof(struct
  // instruction));
  prog->bytecode[prog->bytecode_size].type = i.type;
  if (i.type == i_push_primitive)
    prog->bytecode[prog->bytecode_size].data.information = i.data.information;
  if (i.type == i_jmp || i.type == i_jmp_if || i.type == i_jmp_not_if)
    prog->bytecode[prog->bytecode_size].data.address = i.data.address;
  prog->bytecode_size++;
}
#define COMPILER_STATE_IN_FUNC 1 << 0
#define COMPILER_STATE_IN_VAR 1 << 1
#define COMPILER_STATE_IN_LOOP 1 << 2
#define BUILTIN_WORD_MATCH(T, V, S) \
  {                                 \
    if (!strcmp(T->name, V)) {      \
      p = instruction_from_i(S);    \
      p_useful = 1;                 \
      break;                        \
    }                               \
  }
struct program *build(struct tokenlist *tl, struct arguments *op) {
  struct program *result = malloc(sizeof(struct program));
  result->words = NULL;
  result->word_count = 0;
  memset(result, 0, sizeof(struct program));
  struct token *current_token = tl->token;
  struct block_stack *blocks = create_block_stack();
  struct position_stack *unfilled_breaks = create_position_stack();
  result->entry_point = 0;
  unsigned char p_useful = 0;
  struct trie *matching = create_trie();
  add_many_to_trie(
      matching, "@", i_dereference, "!", i_assign, "!=", i_not_equal, "%",
      i_modulo, "*", i_multiply, "+", i_plus, "++", i_increment, "-", i_minus,
      "--", i_decrement, "/", i_divide, "<", i_lt, "<=", i_lte, "=", i_equal,
      ">", i_gt, ">=", i_gte, "^", i_power, "abs", i_abs, "and", i_and, "andn",
      i_andn, "array_count", i_array_count, "array_dump", i_array_dump,
      "array_make", i_array_make, "array_make_dict", i_array_make_dict,
      "array_getitem", i_array_getitem, "array_setitem", i_array_setitem,
      "array_delitem",i_array_delitem,"array_next",i_array_next,
      "array_appenditem", i_array_appenditem, "array_sum", i_array_sum, "cos",
      i_cos, "sin", i_sin, "tan", i_tan, "acos", i_acos, "asin", i_asin, "atan",
      i_tan, "atan2", i_atan2, "assert", i_assert, "atoi", i_atoi, "call",
      i_call, "ceil", i_ceil, "fabs", i_fabs, "floor", i_floor, "depth",
      i_depth, "dup", i_dup, "dupn", i_dupn, "exit", i_exit, "explode",
      i_explode, "intostr", i_intostr, "instr", i_instr, "instring", i_instring,
      "midstr", i_midstr, "nip", i_nip, "not", i_not, "notify", i_notify, "or",
      i_or, "over", i_over,"pick",i_pick, "pop", i_pop, "popn", i_popn, "pow", i_power,
      "read", i_read, "reverse", i_reverse, "log", i_log, "log10", i_log10,
      "lreverse", i_lreverse, "rot", i_rot, "rotate", i_rotn, "rotn", i_rotn,
      "rsplit", i_rsplit, "split", i_split, "sqrt", i_sqrt, "strcat", i_strcat,
      "strcmp", i_strcmp, "stringpfx", i_stringpfx, "striplead", i_striplead,
      "striptail", i_striptail, "strlen", i_strlen, "strtod", i_strtod, "subst",
      i_subst, "swap", i_swap, "tolower", i_tolower, "toupper", i_toupper,
      "while", i_while, "xor", i_xor, "{", i_mark, "}", i_mark_end, NULL);
  if (op->dump_trie) dump(matching, "trie-graph.dot");
  struct {
    char **variables;
    size_t count;
  } local_var_names = {.variables = NULL, .count = 0};
  bool in_function = false;
  while (current_token) {
#define last_word result->words[result->word_count - 1]
    struct instruction p;
    switch (current_token->type) {
      case LEXER_FUNC_START: {
        struct word w = {.position = result->bytecode_size,
                         .name = strdup(tl->next->token->name)};
        tl = tl->next;
        if (result->words)
          result->words = realloc(result->words,
                                  sizeof(struct word) * (++result->word_count));
        else
          result->words = malloc(sizeof(struct word) * (++result->word_count));
        result->words[result->word_count - 1] = w;
        result->entry_point = w.position;
        in_function = true;
      } break;
      case LEXER_FUNC_END:
        if (blocks->size != 0) {
        }
        add_instruction(result, simple_instruction_from_type(i_exit));
        in_function = false;
        last_word.local_vars = local_var_names.count;
        for (size_t i = 0; i < local_var_names.count; i++) {
          free(local_var_names.variables[i]);
        }
        free(local_var_names.variables);
        local_var_names.count = 0;
        break;
      case LEXER_INT:
        p.type = i_push_primitive;
        p.data.information = create_prim_int(atoi(current_token->name));
        p_useful = 1;
        break;
      case LEXER_DOUBLE:
        p.type = i_push_primitive;
        p.data.information = create_prim_double(atof(current_token->name));
        p_useful = 1;
        break;
      case LEXER_STRING:
        p.type = i_push_primitive;
        p.data.information = create_prim_string(current_token->name);
        p_useful = 1;
        break;
      /**
       * 1 2 > if 3 else 4 then
       * |push 1|
       * |push 2|
       * |gt    |
       * |if    |
       * |push 3|
       * |else  |
       * |push 4|
       * |then  |
       *
       * 1  |push 1      |
       * 2  |push 2      |
       * 3  |gt          |
       * 4  |jmp_not_if 7|
       * 5  |push 3      |
       * 6  |else        |
       * 7  |push 4      |
       * 8  |then        |
       *
       * 1 2 > if 3 then
       * |push 1|
       * |push 2|
       * |gt    |
       * |if    |
       * |push 3|
       * |then  |
       *
       * 1  |push 1      |
       * 2  |push 2      |
       * 3  |gt          |
       * 4  |jmp_not_if 6|
       * 5  |push 3      |
       * 6  |then        |
       * */
      case LEXER_IF:
        push_block(blocks, bs_if, result->bytecode_size);
        add_instruction(result, simple_instruction_from_type(i_jmp_not_if));
        break;
      case LEXER_ELSE: {
        if (peek_block(blocks).type != bs_if) {
          // handle the error
        }
        struct block tmp = pop_block(blocks);
        push_block(blocks, bs_else, result->bytecode_size);
        add_instruction(result, simple_instruction_from_type(i_jmp));
        result->bytecode[tmp.position].data.address = result->bytecode_size;
      } break;
      case LEXER_THEN:
        if (peek_block(blocks).type == bs_if ||
            peek_block(blocks).type == bs_else) {
          struct block tmp = pop_block(blocks);
          result->bytecode[tmp.position].data.address = result->bytecode_size;
        }
        break;

      case LEXER_BEGIN:
        push_block(blocks, bs_begin, result->bytecode_size);
        break;
      case LEXER_UNTIL:
      case LEXER_REPEAT:
        if (peek_block(blocks).type == bs_begin ||
            peek_block(blocks).type == bs_foreach ||
            peek_block(blocks).type == bs_for) {
          struct block tmp = pop_block(blocks);
          add_instruction(result, simple_instruction_from_type(i_jmp));
          result->bytecode[result->bytecode_size - 1].data.address =
              tmp.position;
          close_loop(result, tmp, unfilled_breaks);
        }
        break;
      case LEXER_BREAK:
        push_position_stack(unfilled_breaks, result->bytecode_size);
        add_instruction(result, simple_instruction_from_type(i_break));
        break;
      case LEXER_CONTINUE:
        push_position_stack(unfilled_breaks, result->bytecode_size);
        add_instruction(result, simple_instruction_from_type(i_continue));
      case LEXER_FOREACH:
        break;
      case LEXER_FOR:
        add_instruction(result, simple_instruction_from_type(i_forpush));
        push_block(blocks, bs_for, result->bytecode_size);
        push_position_stack(unfilled_breaks, result->bytecode_size);
        add_instruction(result, simple_instruction_from_type(i_foriter));
        break;
      case LEXER_LVAR:
        break;
      case LEXER_VAR:
        if (in_function) {
          if (local_var_names.variables) {
            local_var_names.variables =
                realloc(local_var_names.variables,
                        sizeof(char *) * (local_var_names.count + 1));
          } else {
            local_var_names.variables = malloc(sizeof(char *));
          }
          local_var_names.variables[local_var_names.count] =
              strdup(tl->next->token->name);
          local_var_names.count++;
          tl = tl->next;
        }
        if (current_token->name[3] == '!') {
          struct stack_cell z = {.type = t_svar,
                                 .data.number = local_var_names.count - 1};
          struct instruction aa = {.type = i_push_primitive,
                                   .data.information = z};
          add_instruction(result, aa);
          aa.type = i_assign;
          aa.data.address = (size_t)NULL;
          add_instruction(result, aa);
        }
        break;
      case LEXER_WORD:
#define match(S, I) BUILTIN_WORD_MATCH(current_token, S, I);
        if (current_token->name[0] != '\\') {
          for (size_t i = 0; i < local_var_names.count; i++) {
            if (!strcmp(local_var_names.variables[i], current_token->name)) {
              p.type = i_push_primitive;
              p.data.information.type = t_svar;
              p.data.information.data.number = i;
              p_useful = 1;
              break;
            }
          }
          if (!p_useful) {
            // call words etc
            int trie_match = get_value(matching, current_token->name);
            p.type = trie_match;
            p_useful = trie_match != -1;
          }
        } else {
          current_token->name = current_token->name + 1;
        }
        if (p_useful) {
          break;
        }
        break;
    }
    if (p_useful) {
      if (p.type == i_while) {
        push_position_stack(unfilled_breaks, result->bytecode_size);
      }
      add_instruction(result, p);
      p_useful = 0;
    }
    add_line_range(result, current_token);
    if (tl->next) {
      tl = tl->next;
      current_token = tl->token;
    } else {
      current_token = NULL;
    }
  }
  free_position_stack(unfilled_breaks);
  free_block_stack(blocks);
  free_trie(matching);
  return result;
}
#undef match
#undef BUILTIN_WORD_MATCH
struct position_stack *create_position_stack() {
  struct position_stack *result = malloc(sizeof(struct position_stack));
  result->position = calloc(sizeof(size_t), 10);
  result->capacity = 10;
  result->size = 0;
  return result;
}
void push_position_stack(struct position_stack *s, size_t pos) {
  if (s->size == s->capacity) {
    s->capacity *= 2;
    s->position = realloc(s->position, s->capacity * sizeof(size_t));
  }
  s->position[s->size++] = pos;
}
size_t pop_position_stack(struct position_stack *s) {
  size_t r = s->position[s->size - 1];
  s->size--;
  return r;
}
size_t peek_position_stack(struct position_stack *s) {
  return s->position[s->size - 1];
}
void free_position_stack(struct position_stack *s) {
  if (!s) return;
  free(s->position);
  free(s);
}
struct instruction instruction_from_i(int type) {
  struct instruction r;
  r.type = type;
  return r;
}
struct stack_cell create_prim_int(int n) {
  struct stack_cell result;
  result.type = t_int;
  result.data.number = n;
  return result;
}
struct stack_cell create_prim_double(double f) {
  struct stack_cell result;
  result.type = t_float;
  result.data.fnumber = f;
  return result;
}
struct stack_cell create_prim_string(const char *data) {
  struct stack_cell result;
  result.type = t_string;
  result.data.str = malloc(sizeof(struct shared_string) + strlen(data) + 1);
  result.data.str->length = strlen(data);
  result.data.str->links = 1;
  strncpy(result.data.str->str, data, strlen(data) + 1);
  return result;
}
struct stack_cell create_prim_invalid(const char *message) {
  struct stack_cell result;
  result.type = t_invalid;
  if (message) {
    result.data.str =
        malloc(sizeof(struct shared_string) + strlen(message) + 1);
    result.data.str->links = 1;
    result.data.str->length = strlen(message);
    strncpy(result.data.str->str, message, result.data.str->length);
  } else {
    result.data.str = NULL;
  }
  return result;
}
void print_stack_cell(struct stack_cell *sc) {
  switch (sc->type) {
    case t_int:
    case t_dbref:
      printf("INT:%i", sc->data.number);
      break;
    case t_float:
      printf("FLOAT:%f", sc->data.fnumber);
      break;
    case t_string:
      printf("STRING:%s", sc->data.str->str);
      break;
    case t_svar:
      printf("SV%d", sc->data.number);
      break;
    case t_invalid:
      printf("INVALID:");
      if (sc->data.str) {
        printf("%s", sc->data.str->str);
      }
      break;
    case t_array:
      print_array(sc->data.array);
      break;
    default:
      printf("OTHER");
      break;
  }
}
struct stack_cell *stack_ptr_from_rval(struct stack_cell n) {
  struct stack_cell *result = malloc(sizeof(struct stack_cell));
  result->type = n.type;
  result->data = n.data;
  return result;
}
struct instruction simple_instruction_from_type(int t) {
  struct instruction result;
  result.type = t;
  return result;
}
struct block_stack *create_block_stack() {
  struct block_stack *result = malloc(sizeof(struct block_stack));
  result->stuff = malloc(sizeof(struct block) * 10);
  result->capacity = 10;
  result->size = 0;
  memset(result->stuff, 0, sizeof(struct block) * 10);
  return result;
}
void push_block(struct block_stack *stack, int type, size_t pos) {
  size_t s = stack->size;
  if (stack->size + 1 == stack->capacity) {
    stack->stuff =
        realloc(stack->stuff, stack->capacity * 2 * sizeof(struct block));
    stack->capacity *= 2;
  }
  stack->stuff[s].type = type;
  stack->stuff[s].position = pos;
  stack->size++;
}
struct block pop_block(struct block_stack *stack) {
  struct block r = stack->stuff[--stack->size];
  return r;
}

struct block peek_block(struct block_stack *stack) {
  struct block result;
  result = stack->stuff[stack->size - 1];
  return result;
}
void free_block_stack(struct block_stack *s) {
  free(s->stuff);
  free(s);
}
const char *obtain_bytecode_name(unsigned char t) {
  const char *inames[] = {"i_abort",
                          "i_abs",
                          "i_acos",
                          "i_and",
                          "i_andn",
                          "i_array_appenditem",
                          "i_array_count",
                          "i_array_delitem",
                          "i_array_dump",
                          "i_array_make",
                          "i_array_make_dict",
                          "i_array_next",
                          "i_array_getitem",
                          "i_array_setitem",
                          "i_array_sum",
                          "i_asin",
                          "i_assert",
                          "i_assign",
                          "i_atan",
                          "i_atan2",
                          "i_atoi",
                          "i_break",
                          "i_call",
                          "i_continue",
                          "i_ceil",
                          "i_cos",
                          "i_decrement",
                          "i_depth",
                          "i_dereference",
                          "i_divide",
                          "i_dup",
                          "i_dupn",
                          "i_equal",
                          "i_exit",
                          "i_explode",
                          "i_fabs",
                          "i_floor",
                          "i_foriter",
                          "i_forpop",
                          "i_forpush",
                          "i_gt",
                          "i_gte",
                          "i_increment",
                          "i_instr",
                          "i_instring",
                          "i_intostr",
                          "i_jmp",
                          "i_jmp_if",
                          "i_jmp_not_if",
                          "i_log",
                          "i_log10",
                          "i_lreverse",
                          "i_lt",
                          "i_lte",
                          "i_mark",
                          "i_mark_end",
                          "i_midstr",
                          "i_minus",
                          "i_modulo",
                          "i_multiply",
                          "i_nip",
                          "i_not",
                          "i_not_equal",
                          "i_notify",
                          "i_or",
                          "i_orn",
                          "i_over",
                          "i_pick",
                          "i_plus",
                          "i_pop",
                          "i_popn",
                          "i_power",
                          "i_push_primitive",
                          "i_read",
                          "i_reverse",
                          "i_rot",
                          "i_rotn",
                          "i_rsplit",
                          "i_sin",
                          "i_smatch",
                          "i_split",
                          "i_sqrt",
                          "i_strcat",
                          "i_strcmp",
                          "i_stringpfx",
                          "i_strip",
                          "i_striplead",
                          "i_striptail",
                          "i_strlen",
                          "i_strtod",
                          "i_subst",
                          "i_swap",
                          "i_tan",
                          "i_tolower",
                          "i_toupper",
                          "i_while",
                          "i_xor"};
  return inames[t];
}
void print_bytecode(struct program *p) {
  unsigned int max_opcode_name = 0;
  for (int i = 0; i <= LARGEST_INSTRUCTION_CODE; i++) {
    max_opcode_name = strlen(obtain_bytecode_name(i)) > max_opcode_name
                          ? strlen(obtain_bytecode_name(i))
                          : max_opcode_name;
  }
  struct instruction_range *r = p->lines;
  printf("Address |%*s| argument\n", max_opcode_name, "Opcode");
  for (size_t i = 0; i < p->bytecode_size; i++) {
    struct instruction *current = &p->bytecode[i];
    if (i >= r->end) {
      r++;
      printf("entering line %zi\n", r->line);
    }
    printf("%-8zi|%-*s|", i, max_opcode_name,
           obtain_bytecode_name(current->type));
    if (current->type == i_push_primitive) {
      switch (current->data.information.type) {
        case t_string:
          printf("\"%s\"", current->data.information.data.str->str);
          break;
        case t_int:
          printf("%8i", current->data.information.data.number);
          break;
        case t_svar:
          printf("SV%i", current->data.information.data.number);
          break;
        case t_var:
          printf("V%i", current->data.information.data.number);
          break;
        case t_lvar:
          printf("LV%i", current->data.information.data.number);
          break;
        case t_float:
          printf("%e", current->data.information.data.fnumber);
          break;
      }
    } else if (current->type == i_jmp || current->type == i_jmp_if ||
               current->type == i_jmp_not_if || current->type == i_foriter ||
               current->type == i_break || current->type == i_continue) {
      printf("%8zi", current->data.address);
    }
    printf("\n");
  }
}
void free_program(struct program **pr) {
  struct program *p = *pr;
  for (size_t i = 0; i < p->bytecode_size; i++) {
    if (p->bytecode[i].type == i_push_primitive) {
      free_stack_cell(p->bytecode[i].data.information);
    }
  }
  for (size_t i = 0; i < (*pr)->word_count; i++) {
    free((*pr)->words[i].name);
  }
  free((*pr)->words);
  free(p->bytecode);
  free(p->lines);
  free(*pr);
}
void close_loop(struct program *result, struct block tmp,
                struct position_stack *unfilled_breaks) {
  while (unfilled_breaks->size > 0) {
    // See what loops existed at the time of the creation of these breaks
    size_t cur = peek_position_stack(unfilled_breaks);
    if (cur >= tmp.position) {
      pop_position_stack(unfilled_breaks);
      if (result->bytecode[cur].type == i_break) {
        result->bytecode[cur].data.address = result->bytecode_size;
      } else if (result->bytecode[cur].type == i_continue) {
        result->bytecode[cur].data.address = tmp.position - 1;
      } else if (result->bytecode[cur].type == i_foriter) {
        // This is... appropriate, but it's not uniform, which is silly.
        result->bytecode[cur].data.address = result->bytecode_size - 1;
      } else if (result->bytecode[cur].type == i_while) {
        result->bytecode[cur].data.address = result->bytecode_size - 1;
      }
    } else
      break;
  }
  if (tmp.type == bs_for) {
    add_instruction(result, simple_instruction_from_type(i_forpop));
  }
}
