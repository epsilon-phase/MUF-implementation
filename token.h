#ifndef TOKEN_H
#define TOKEN_H
enum lexer_token {
  LEXER_INT,
  LEXER_DOUBLE,
  LEXER_STRING,
  LEXER_FUNC_START,
  LEXER_FUNC_END,
  LEXER_IF,
  LEXER_ELSE,
  LEXER_THEN,
  LEXER_FOR,
  LEXER_BEGIN,
  LEXER_FOREACH,
  LEXER_REPEAT,
  LEXER_UNTIL,
  LEXER_WORD,
  LEXER_INVALID,
  LEXER_NOTHING = -1
};
#define LEXER_LOOP_END(X) (X == LEXER_REPEAT || X == LEXER_UNTIL)
#define LEXER_BLOCK_END(X)                                                     \
  (X == LEXER_FUNC_END || X == LEXER_ELSE || X == LEXER_THEN||X==LEXER_UNTIL||X==LEXER_REPEAT)
#define LEXER_LOOP_START(X) (X == LEXER_FOR || LEXER_FOREACH || LEXER_BEGIN)
struct token {
  unsigned int line;
  unsigned int start;
  unsigned int end;
  unsigned int type;
  char *name;
};
struct tokenlist {
  struct token *token;
  struct tokenlist *next;
};
void tokenlist_free(struct tokenlist *start);
void tokenlist_print(struct tokenlist *start);
struct tokenlist *lexer(const char *token);
void post_process_lexer(struct tokenlist *start);
void free_token(struct token *token);
struct token *create_token();
struct token *copy_token(struct token *token);
#endif
