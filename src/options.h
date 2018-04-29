#ifndef OPTIONS_H
#define OPTIONS_H
struct arguments{
  char *file;
  char **other_files;
  int print_bytecode,
      print_stack,
      run_tests,
      print_lexer;
};
#endif
