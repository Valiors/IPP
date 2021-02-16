#ifndef IPP_INPUT_H
#define IPP_INPUT_H

#include "auxiliaries.h"
#include "trie.h"

/* Reads line of input and returns number of
 * bytes read (including delimeters). Returns -1
 * if EOF is set. */
long read_line(char **line);

/* Splits line into command with two optional arguments
 * and performs simple checks for validity. Returns 1 if
 * successful, 0 if line should be ignored or -1 if error. */
int parse_line(char *line,
               size_t line_length,
               char **command,
               char **arg1,
               char **arg2);

// Set of functions to handle input commands.
int command_declare(Trie *root, char *arg1, char *arg2);

int command_remove(Trie *root, char *arg1, char *arg2);

int command_valid(Trie *root, char *arg1, char *arg2);

int command_set_energy(Trie *root, char *arg1, char *arg2);

int command_get_energy(Trie *root, char *arg1);

int command_equal(Trie *root, char *arg1, char *arg2);

#endif //IPP_INPUT_H
