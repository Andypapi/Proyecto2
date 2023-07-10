#include "user_input.h"
#include "trie.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <ctype.h>

// Macros
#define CTRL_KEY(k) ((k)&0x1f)
#define SEQUENCE(k, p) write(STDOUT_FILENO, (k), (p))

// Colores
#define RESALTAR "\x1b[30m\x1b[47m"
#define HEADER "\x1b[95m"
#define GREEN "\x1b[92m"
#define RESET_COLOR "\x1b[0m"

struct termios orig_terminal;

/*
 * Función: disableRawMode
 * Deshabilita el modo raw.
 *
 * Descripción:
 * Esta función deshabilita el modo raw en la terminal. Restaura los ajustes
 * originales de la terminal, incluyendo la visualización del cursor.
 *
 * Parámetros:
 * - Ninguno.
 *
 * Precondiciones:
 * - Ninguna.
 *
 * Postcondiciones:
 * - Se deshabilita el modo raw en la terminal.
 * - Se restauran los ajustes originales de la terminal, incluyendo la
 *   visualización del cursor.
 */
void disableRawMode() {
  SEQUENCE("\x1b[?25h", 6); // Muestra el cursor en la terminal
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_terminal);
}

/*
 * Función: enableRawMode
 * Habilita el modo raw.
 *
 * Descripción:
 * Esta función habilita el modo raw en la terminal. Configura la terminal para
 * desactivar el eco de los caracteres ingresados, desactivar el modo canónico y
 * desactivar señales de control como Ctrl+C y Ctrl+Z. Además, oculta el cursor
 * en la terminal.
 *
 * Parámetros:
 * - Ninguno.
 *
 * Precondiciones:
 * - Ninguna.
 *
 * Postcondiciones:
 * - Se habilita el modo raw en la terminal.
 * - La terminal se configura para desactivar el eco, el modo canónico y las
 *   señales de control.
 * - El cursor se oculta en la terminal.
 */
void enableRawMode() {

  SEQUENCE("\x1b[?25l", 6); // Oculta el cursor en la terminal

  tcgetattr(STDIN_FILENO, &orig_terminal);
  atexit(disableRawMode);

  struct termios raw = orig_terminal;
  raw.c_iflag &= ~(ICRNL | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/*
 * Función: showHeader
 * Muestra el encabezado de la aplicación.
 *
 * Descripción:
 * Esta función imprime el encabezado de la aplicación en la terminal. El
 * encabezado contiene el nombre de la aplicación y las opciones disponibles
 * para el usuario.
 *
 * Parámetros:
 * - Ninguno.
 *
 * Precondiciones:
 * - Ninguna.
 *
 * Postcondiciones:
 * - Se muestra el encabezado de la aplicación en la terminal.
 */
void showHeader() {
  printf("╔════════════════════╗\r\n");
  printf("║  Application Name  ║\r\n");
  printf("╚════════════════════╝\r\n");

  printf("╔════════════════════════════════════════════════════════════════════"
         "═════════════════════════════════════════════════════╗\r\n");
  printf("║  [S] Buscar por palabra    [P] Buscar por prefijo    [L] Cargar "
         "Archivo    [H] Mostrar Ayuda    [Q] Salir del Programa  ║\r\n");
  printf("╚════════════════════════════════════════════════════════════════════"
         "═════════════════════════════════════════════════════╝\r\n");
  printf("\r\n"); // Margen
}

/*
 * Función: clearScreen
 * Limpia la pantalla y muestra el encabezado de la aplicación.
 *
 * Descripción:
 * Esta función limpia la pantalla de la terminal y muestra el encabezado de la
 * aplicación utilizando la función showHeader().
 *
 * Parámetros:
 * - Ninguno.
 *
 * Precondiciones:
 * - Ninguna.
 *
 * Postcondiciones:
 * - Se limpia la pantalla de la terminal.
 * - Se muestra el encabezado de la aplicación.
 */
void clearScreen() {
  SEQUENCE("\x1b[2J", 4);
  SEQUENCE("\x1b[H", 3);
  showHeader();
}

/*
 * Función: readKey
 * Lee una tecla presionada por el usuario.
 *
 * Descripción:
 * Esta función lee una tecla presionada por el usuario en la terminal y la
 * devuelve en minúsculas. Utiliza la función read() para leer el carácter
 * presionado y luego lo convierte a minúsculas utilizando la función tolower().
 *
 * Parámetros:
 * - Ninguno.
 *
 * Precondiciones:
 * - Ninguna.
 *
 * Postcondiciones:
 * - Se lee y devuelve la tecla presionada por el usuario en minúsculas.
 */
char readKey() {
  char key;
  read(STDIN_FILENO, &key, 1);
  return tolower(key);
}

/*
 * Función: toLowercase
 * Convierte una cadena de caracteres a minúsculas.
 *
 * Descripción:
 * Esta función convierte cada carácter de una cadena de caracteres a minúsculas
 * utilizando la función tolower(). Itera sobre cada carácter de la cadena hasta
 * llegar al carácter nulo ('\0').
 *
 * Parámetros:
 * - str: Puntero a la cadena de caracteres a convertir.
 *
 * Precondiciones:
 * - El puntero str debe apuntar a una cadena de caracteres válida.
 *
 * Postcondiciones:
 * - La cadena de caracteres se convierte a minúsculas.
 */
void toLowercase(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = tolower(str[i]);
  }
}

/*
 * Función: printGuide
 * Imprime la guía de comandos disponibles.
 *
 * Descripción:
 * Esta función imprime la guía de comandos disponibles en la terminal. Muestra
 * los comandos disponibles y una breve descripción de cada uno.
 *
 * Parámetros:
 * - Ninguno.
 *
 * Precondiciones:
 * - Ninguna.
 *
 * Postcondiciones:
 * - Se imprime la guía de comandos disponibles en la terminal.
 */
void printGuide() {
  clearScreen();
  printf(" [Usage]: programa.c\r\n");
  printf(" ╔══ [h] Ayuda:\r\n");
  printf(" ║\r\n");
  printf(" ╚══════ Comandos disponibles.\r\n");
  printf(" ╔══ [l] Cargar archivo:\r\n");
  printf(" ║\r\n");
  printf(" ╚══════ Carga e inicializa el diccionario.\r\n");
  printf(" ╔══ [s] Palabra:\r\n");
  printf(" ║\r\n");
  printf(" ╚══════ Busca una palabra en el diccionario.\r\n");
  printf(" ╔══ [p] Prefijo:\r\n");
  printf(" ║\r\n");
  printf(" ╚══════ Buscar palabras por medio de un prefijo.\r\n");
  printf(" ╔══ [q] Salir:\r\n");
  printf(" ║\r\n");
  printf(" ╚══════ Sale del programa.\r\n");
}

/*
 * Función: wordLookup
 * Realiza la búsqueda de una palabra en el diccionario.
 *
 * Descripción:
 * Esta función realiza la búsqueda de una palabra en el diccionario. Pide al
 * usuario que ingrese la palabra a buscar, la convierte a minúsculas y luego
 * utiliza la función searchMeaning() para obtener el significado asociado a la
 * palabra. Imprime la palabra y su significado en la terminal.
 *
 * Parámetros:
 * - trie: Puntero al Trie que contiene el diccionario.
 *
 * Precondiciones:
 * - El puntero trie debe apuntar a un Trie válido que contenga el diccionario.
 *
 * Postcondiciones:
 * - Se solicita al usuario que ingrese la palabra a buscar.
 * - Se convierte la palabra a minúsculas.
 * - Se busca la palabra en el diccionario utilizando la función
 * searchMeaning().
 * - Se imprime la palabra y su significado en la terminal.
 */
void wordLookup(Trie *trie) {
  disableRawMode();
  clearScreen();
  char input[200];
  printf("Palabra a buscar: ");
  scanf("%s", input);
  printf("─────────────────────────────────────────────────────────────────────"
         "───────────────\r\n");
  toLowercase(input);

  char *meaning = searchMeaning(trie, input);
  if (strcmp(meaning, "") != 0) {
    printf(" ┏━━ %s\r\n ┗━━━━━━━━━━ %s\r\n\r\n", input, meaning);
  } else {
    printf("Palabra no encontrada\r\n");
  }
}

/*
 * Función: wordLookupWithPrefix
 * Realiza la búsqueda de palabras por medio de un prefijo.
 *
 * Descripción:
 * Esta función realiza la búsqueda de palabras en el diccionario utilizando un
 * prefijo. Pide al usuario que ingrese el prefijo por el cual buscar, lo
 * convierte a minúsculas y utiliza la función searchWithPrefix() para obtener
 * las palabras que coincidan con el prefijo. Imprime las palabras encontradas
 * en la terminal.
 *
 * Parámetros:
 * - trie: Puntero al Trie que contiene el diccionario.
 *
 * Precondiciones:
 * - El puntero trie debe apuntar a un Trie válido que contenga el diccionario.
 *
 * Postcondiciones:
 * - Se solicita al usuario que ingrese el prefijo por el cual buscar.
 * - Se convierte el prefijo a minúsculas.
 * - Se busca las palabras que coincidan con el prefijo utilizando la función
 *   searchWithPrefix().
 * - Se imprime las palabras encontradas en la terminal.
 */
void wordLookupWithPrefix(Trie *trie) {
  clearScreen();
  char palabra[200];
  printf("Prefijo por el cual buscar: ");
  scanf("%s", palabra);
  toLowercase(palabra);
  searchWithPrefix(trie, palabra);
}

/*
 * Función: loadFile
 * Carga e inicializa el diccionario a partir de un archivo.
 *
 * Descripción:
 * Esta función carga e inicializa el diccionario a partir de un archivo de
 * entrada. Solicita al usuario que ingrese el nombre del archivo y utiliza la
 * función loadDictFile() para cargar el contenido del archivo en el
 * diccionario. Imprime un mensaje indicando que el diccionario ha sido creado.
 *
 * Parámetros:
 * - trie: Doble puntero al Trie donde se cargará el diccionario.
 *
 * Precondiciones:
 * - El puntero trie debe apuntar a un puntero válido que apunte a un Trie
 *   existente.
 *
 * Postcondiciones:
 * - Se solicita al usuario que ingrese el nombre del archivo de entrada.
 * - Se carga el contenido del archivo en el diccionario utilizando la función
 *   loadDictFile().
 * - Se imprime un mensaje indicando que el diccionario ha sido creado.
 */
void loadFile(Trie **trie) {
  disableRawMode();
  clearScreen();
  char dict[100];
  printf("Ingrese el archivo de entrada: ");
  scanf("%s", dict);
  loadDictFile(trie, dict);
  printf("El diccionario ha sido creado.\n");
}
