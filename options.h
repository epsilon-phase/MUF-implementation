#ifndef OPTIONS_H
#define OPTIONS_H
struct options{
    size_t filecount;
    char** files;
    int print_tokens:1,print_ast:1,print_function_names:1;
};
#endif
