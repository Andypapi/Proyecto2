#ifndef TRIE_H
#define TRIE_H

typedef struct Trie {
  struct Trie *children[26];
  char meaning[600];
  int last;
} Trie;

Trie *createNewTrieNode();

void insert(Trie **trie, const char *word, const char *meaning);
char *searchMeaning(Trie *trie, const char *word);

void printWordsWithPrefix(Trie *trie, const char *prefix);
void searchWithPrefix(Trie *trie, const char *prefix);
void loadDictFile(Trie **trie, const char *filename);
#endif
