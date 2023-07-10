#include "trie.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Función: createNewTrieNode
 * Crea un nuevo nodo del Trie.
 *
 * Descripción:
 * Esta función crea un nuevo nodo del Trie y devuelve un puntero a ese nodo.
 * El nodo se inicializa con un marcador "last" igual a 0 y un array de punteros
 * a nodos hijos, todos inicializados a NULL.
 *
 * Retorna:
 * - Puntero al nodo del Trie recién creado.
 *
 * Precondiciones:
 * - Ninguna.
 *
 * Postcondiciones:
 * - Se crea y se devuelve un nuevo nodo del Trie.
 * - El nodo se inicializa con un marcador "last" igual a 0 y un array de
 * punteros a nodos hijos, todos inicializados a NULL.
 */
Trie *createNewTrieNode() {
  Trie *newNode = NULL;

  newNode = (Trie *)malloc(sizeof(Trie));
  if (!newNode) {
    printf("No se pudo crear el Trie\n");
  }
  newNode->last = 0;
  for (int i = 0; i < 26; i++) {
    newNode->children[i] = NULL;
  }
  return newNode;
}

/*
 * Función: insert
 * Inserta una palabra y su significado en el Trie.
 *
 * Descripción:
 * Esta función inserta una palabra y su significado en el Trie especificado.
 * Si el Trie no existe, se crea uno nuevo. La palabra se inserta letra por
 * letra, creando nuevos nodos en el Trie según sea necesario. Al finalizar la
 * inserción de la palabra, se marca el último nodo como "last" y se guarda el
 * significado asociado a ese nodo.
 *
 * Parámetros:
 * - trie: Puntero al puntero al nodo raíz del Trie.
 * - word: Palabra a insertar.
 * - meaning: Significado de la palabra.
 *
 * Precondiciones:
 * - El puntero al puntero al nodo raíz del Trie (trie) debe ser válido.
 * - La palabra (word) y el significado (meaning) deben ser cadenas de
 * caracteres válidas.
 *
 * Postcondiciones:
 * - La palabra y su significado se insertan en el Trie especificado.
 * - Si el Trie no existe, se crea uno nuevo.
 */
void insert(Trie **trie, const char *word, const char *meaning) {
  if (!trie || !(*trie)) {
    *trie = createNewTrieNode();
  }

  Trie *currentNode = *trie;
  int wordLength = strlen(word);
  for (int i = 0; i < wordLength; i++) {
    int charIndex = word[i] - 'a';
    if (currentNode->children[charIndex] == NULL) {
      currentNode->children[charIndex] = createNewTrieNode();
    }

    currentNode = currentNode->children[charIndex];
  }
  currentNode->last = 1;
  strcpy(currentNode->meaning, meaning);
}

/*
 * Función: searchMeaning
 * Busca el significado de una palabra en un Trie dado.
 *
 * Descripción:
 * Esta función busca el significado de una palabra en un Trie dado y devuelve
 * el significado encontrado, si existe. La función recorre el Trie letra por
 * letra, avanzando al nodo hijo correspondiente en cada iteración. Si la
 * palabra se completa y se encuentra el nodo final del Trie, se devuelve el
 * significado asociado a ese nodo.
 *
 * Parámetros:
 * - trie: Puntero al Trie en el que se buscará la palabra.
 * - word: Palabra cuyo significado se desea buscar en el Trie.
 *
 * Retorna:
 * - Puntero al significado de la palabra buscada, si se encuentra.
 * - Cadena vacía si la palabra no se encuentra en el Trie o si el Trie es nulo.
 *
 * Precondiciones:
 * - El Trie (trie) debe ser un puntero válido que apunte a un Trie existente.
 * - La palabra (word) debe ser una cadena de caracteres válida.
 *
 * Postcondiciones:
 * - Se busca el significado de la palabra en el Trie y se devuelve el resultado
 * encontrado.
 * - Si la palabra no se encuentra en el Trie o si el Trie es nulo, se devuelve
 * una cadena vacía.
 */
char *searchMeaning(Trie *trie, const char *word) {
  if (!trie) {
    return "";
  }

  Trie *current = trie;
  int length = strlen(word);
  for (int i = 0; i < length; i++) {
    int charIndex = word[i] - 'a';
    current = current->children[charIndex];

    if (!current) {
      return "";
    }
  }

  return current && current->last ? current->meaning : "";
}

/*
 * Función: printWordsWithPrefix
 * Imprime las palabras con un prefijo dado en un Trie.
 *
 * Descripción:
 * Esta función imprime todas las palabras en un Trie que tienen un prefijo
 * dado. Comienza desde el nodo raíz del Trie y avanza a través de los nodos
 * correspondientes a las letras del prefijo. Luego, imprime todas las palabras
 * encontradas desde ese nodo en adelante, incluyendo el prefijo.
 *
 * Parámetros:
 * - trie: Puntero al Trie en el que se buscarán las palabras.
 * - prefix: Prefijo utilizado para filtrar las palabras a imprimir.
 *
 * Precondiciones:
 * - El Trie (trie) debe ser un puntero válido que apunte a un Trie existente.
 * - El prefijo (prefix) debe ser una cadena de caracteres válida.
 *
 * Postcondiciones:
 * - Se imprimen todas las palabras en el Trie que tienen el prefijo
 * especificado, incluyendo el prefijo mismo.
 */
void printWordsWithPrefix(Trie *trie, const char *prefix) {
  if (!trie) {
    return;
  }
  if (trie->last) {
    printf("%s: %s\n", prefix, trie->meaning);
  }
  for (int i = 0; i < 26; i++) {
    if (trie->children[i] != NULL) {
      char letter = 'a' + i;
      char *newPrefix = (char *)malloc((strlen(prefix) + 2) * sizeof(char));
      sprintf(newPrefix, "%s%c", prefix, letter);
      printWordsWithPrefix(trie->children[i], newPrefix);
      free(newPrefix);
    }
  }
}

/*
 * Función: searchWithPrefix
 * Busca palabras con un prefijo dado en un Trie.
 *
 * Descripción:
 * Esta función busca todas las palabras en un Trie que tienen un prefijo
 * dado. Comienza desde el nodo raíz del Trie y avanza a través de los nodos
 * correspondientes a las letras del prefijo. Luego, llama a la función
 * printWordsWithPrefix para imprimir las palabras encontradas.
 *
 * Parámetros:
 * - trie: Puntero al Trie en el que se buscarán las palabras.
 * - prefix: Prefijo utilizado para filtrar las palabras.
 *
 * Precondiciones:
 * - El Trie (trie) debe ser un puntero válido que apunte a un Trie existente.
 * - El prefijo (prefix) debe ser una cadena de caracteres válida.
 *
 * Postcondiciones:
 * - Se buscan y se imprimen todas las palabras en el Trie que tienen el prefijo
 *   especificado.
 * - Si no se encuentran palabras con el prefijo o el Trie es nulo, no se
 * realiza ninguna acción.
 */
void searchWithPrefix(Trie *trie, const char *prefix) {
  if (!trie) {
    return;
  }

  Trie *currentNode = trie;
  int prefixLength = strlen(prefix);

  for (int i = 0; i < prefixLength; i++) {
    int index = prefix[i] - 'a';
    if (currentNode->children[index] == NULL) {
      printf("No se encontraron palabras con el prefijo '%s'\n", prefix);
      return;
    }
    currentNode = currentNode->children[index];
  }

  printWordsWithPrefix(currentNode, prefix);
}

/*
 * Función: loadDictFile
 * Carga un archivo de diccionario en un Trie.
 *
 * Descripción:
 * Esta función carga un archivo de diccionario en un Trie dado. El archivo
 * debe contener una palabra y su significado por línea, separados por un
 * espacio. Si una línea comienza con el carácter '+', se considera que es parte
 * de la definición de la palabra anterior y se concatena con el significado
 * existente. La función crea nodos en el Trie para cada palabra y su
 * significado.
 *
 * Parámetros:
 * - trie: Puntero al puntero al nodo raíz del Trie.
 * - filename: Nombre del archivo de diccionario a cargar.
 *
 * Precondiciones:
 * - El puntero al puntero al nodo raíz del Trie (trie) debe ser válido.
 * - El nombre del archivo de diccionario (filename) debe ser una cadena de
 *   caracteres válida.
 * - El archivo de diccionario debe tener un formato válido con palabras y
 *   significados separados por un espacio.
 *
 * Postcondiciones:
 * - Se carga el archivo de diccionario en el Trie especificado, creando nodos
 *   para cada palabra y su significado.
 * - Si no se puede abrir el archivo o el Trie es nulo, no se realiza ninguna
 * acción.
 */
void loadDictFile(Trie **trie, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("No se pudo abrir el archivo.\n");
    return;
  }

  int isFirstLine = 1;
  char line[600];
  char word[200] = "";
  char meaning[400] = "";

  while (fgets(line, sizeof(line), file)) {
    if (line[0] == '+') {
      strncat(meaning, line + 1, strlen(line) - 2);
    } else {
      if (!isFirstLine) {
        insert(trie, word, meaning);
        memset(meaning, 0, sizeof(meaning));
      }
      sscanf(line, "%s %[^\n]", word, meaning);
      isFirstLine = 0;
    }
  }

  insert(trie, word, meaning);
  fclose(file);
}
