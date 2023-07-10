#ifndef USER_INPUT
#define USER_INPUT

#include "trie.h"

void disableRawMode();
void enableRawMode();

void showHeader();
void clearScreen();
void printGuide();
char readKey();

void wordLookup(Trie* trie);
void wordLookupWithPrefix(Trie *trie);
void loadFile(Trie** trie);
#endif
