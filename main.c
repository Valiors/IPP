#include "auxiliaries.h"
#include "input.h"

int main(void) {
    // Trie to check validity of histories.
    Trie *root = create_trie_node();

    // Line C-string to store currently analysed line of input.
    char *line = NULL;
    long bytes_read = 0;

    while ((bytes_read = read_line(&line)) != -1) {
        size_t line_length = (size_t) bytes_read;

        // Command, first argument and optional second argument.
        char *command = NULL;
        char *arg1 = NULL;
        char *arg2 = NULL;

        // Split line into command and its arguments.
        int parse_status = parse_line(line,
                                      line_length,
                                      &command,
                                      &arg1,
                                      &arg2);

        int command_status = 0;

        if (parse_status == 1) {
            command_status = -1;

            // Perform appropriate command.
            if (strcmp(command, COMMAND_DECLARE) == 0) {
                command_status = command_declare(root, arg1, arg2);
            }
            else if (strcmp(command, COMMAND_REMOVE) == 0) {
                command_status = command_remove(root, arg1, arg2);
            }
            else if (strcmp(command, COMMAND_VALID) == 0) {
                command_status = command_valid(root, arg1, arg2);
            }
            else if (strcmp(command, COMMAND_ENERGY) == 0) {
                // If second argument is present, set energy, else get energy.
                if (arg2 != NULL) {
                    command_status = command_set_energy(root, arg1, arg2);
                }
                else {
                    command_status = command_get_energy(root, arg1);
                }
            }
            else if (strcmp(command, COMMAND_EQUAL) == 0) {
                command_status = command_equal(root, arg1, arg2);
            }
        }

        // If parsing failed or arguments didn't meet requirements output error.
        if (parse_status == -1 || command_status == -1) {
            OUTPUT_ERROR;
        }

        // Free memory allocated by read_line.
        free(line);
        line = NULL;
    }

    // Free memory allocated by read line and delete Trie.
    free(line);
    remove_trie_subtree(root);

    return 0;
}