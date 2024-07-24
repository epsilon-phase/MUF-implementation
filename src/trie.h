#ifndef TRIE_H
#define TRIE_H
#include <limits.h>
struct trie{
  //Why 93? because it's the number of printable characters in ascii.
  struct trie *children[UCHAR_MAX];
  int value;
};
void free_trie(struct trie*);
struct trie* create_trie();
void add_to_trie(struct trie*,const char*,int);
int get_value(struct trie*,const char*);
/*
 * Takes arguments of form (const char*,int)+,NULL
 */
void add_many_to_trie(struct trie*,...);
void dump(struct trie*,const char* file);

#endif
