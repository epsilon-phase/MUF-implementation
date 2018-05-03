#ifndef TRIE_H
#define TRIE_H
struct trie{
  struct trie *children[93];
  int value;
};
void free_trie(struct trie*);
struct trie* create_trie();
void add_to_trie(struct trie*,const char*,int);
int get_value(struct trie*,const char*);
void add_many(struct trie*,...);
void dump(struct trie*,const char* file);

#endif
