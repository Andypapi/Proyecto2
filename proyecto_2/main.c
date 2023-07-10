#include "trie.h"
#include "user_input.h"
#include <stdio.h>

int main() {

  enableRawMode();
  Trie *trie = createNewTrieNode();

  char key;

  do {
    clearScreen();
    key = readKey();
    switch (key) {
      case 'l':
        loadFile(&trie);
        break;
      case 's':
        wordLookup(trie);
        break;
      case 'p':
        wordLookupWithPrefix(trie);
        break;
      case 'h':
        printGuide();
        break;
      case 'q':
        printf("Saliendo del programa\r\n");
        break;
      default:
        printf("Opcion no existente\r\n");
        break;
    }
    // Espera por entrada de usuario (pausa)
    printf("\r\nPresione cualquier tecla para continuar\r\n");
    readKey();
    printf("\r\n");
  } while (key != 'q');
  clearScreen();
  disableRawMode();
  return 0;
}
