#include "input.h"

/* Compares numbers represented as C-strings.
 * Return -1 if a < b, 0 if a == b, 1 if a > b. */
static int compare_numbers(char *a, char *b);

// Converts C-string to uint64_t.
static uint64_t string_to_uint64_t(char *number);

/* Checks if C-string represents history.
 * Returns 1 if so, 0 otherwise. */
static int is_history(char *history);

/* Checks if C-string represents energy.
 * Returns 1 if so, 0 otherwise. */
static int is_energy(char *energy);

long read_line(char **line) {
    size_t line_buffer_size = 0;

    long bytes_read = getline(line, &line_buffer_size, stdin);

    IS_READING_SUCCESSFUL(stdin);

    return bytes_read;
}

int parse_line(char *line,
               size_t line_length,
               char **command,
               char **arg1,
               char **arg2) {
    // If line begins with '#', ignore.
    if (line[0] == '#') {
        return 0;
    }

    // If the line does not end with '\n', error.
    if (line[line_length - 1] != '\n') {
        return -1;
    }

    // If line is empty (only '\n' character), ignore.
    if (line_length == 1) {
        return 0;
    }

    // If there is more than one '\0' in a line, error.
    if (line_length != strlen(line)) {
        return -1;
    }

    // What length should the line have.
    size_t proper_length = 0;

    *command = strtok(line, "\n ");
    if (*command != NULL) proper_length += strlen(*command) + 1;

    *arg1 = strtok(NULL, "\n ");
    if (*arg1 != NULL) proper_length += strlen(*arg1) + 1;

    *arg2 = strtok(NULL, "\n ");
    if (*arg2 != NULL) proper_length += strlen(*arg2) + 1;

    // If the line has excessive characters, error.
    if (line_length != proper_length) {
        return -1;
    }

    // If the first argument is not history, error.
    if (is_history(*arg1) == 0) {
        return -1;
    }

    // If the second argument is neither NULL/history/energy, error.
    if (*arg2 != NULL && is_history(*arg2) == 0 && is_energy(*arg2) == 0) {
        return -1;
    }

    return 1;
}

int command_declare(Trie *root, char *arg1, char *arg2) {
    if (arg2 == NULL) {
        trie_insert(root, arg1);
    }
    else {
        return -1;
    }

    OPERATION_SUCCESSFUL;
    return 1;
}

int command_remove(Trie *root, char *arg1, char *arg2) {
    if (arg2 == NULL) {
        remove_trie_subtree_of_given_prefix(root, arg1);
    }
    else {
        return -1;
    }

    OPERATION_SUCCESSFUL;
    return 1;
}

int command_valid(Trie *root, char *arg1, char *arg2) {
    if (arg2 == NULL) {
        Trie *node = find_string_in_trie(root, arg1);

        if (node != NULL) {
            VALID_YES;
        }
        else {
            VALID_NO;
        }
    }
    else {
        return -1;
    }

    return 1;
}

int command_set_energy(Trie *root, char *arg1, char *arg2) {
    Trie *node = find_string_in_trie(root, arg1);

    if (node == NULL) {
        return -1;
    }

    if (is_energy(arg2) == 1) {
        uint64_t energy_value = string_to_uint64_t(arg2);

        set_node_energy(node, energy_value);
    }
    else {
        return -1;
    }

    OPERATION_SUCCESSFUL;
    return 1;
}

int command_get_energy(Trie *root, char *arg1) {
    Trie *node = find_string_in_trie(root, arg1);

    if (node == NULL) {
        return -1;
    }

    uint64_t energy = get_node_energy(node);

    // If node has assigned energy, print it.
    if (energy > 0) {
        fprintf(stdout, "%" PRIu64 "\n", energy);
    }
    else {
        return -1;
    }

    return 1;
}

int command_equal(Trie *root, char *arg1, char *arg2) {
    if (is_history(arg2) == 1) {
        Trie *node1 = find_string_in_trie(root, arg1);
        Trie *node2 = find_string_in_trie(root, arg2);

        // Both histories shall be declared.
        if (node1 != NULL && node2 != NULL) {
            // If they are equal, then there is nothing to be done.
            if (strcmp(arg1, arg2) == 0) {
                OPERATION_SUCCESSFUL;
                return 1;
            }

            uint64_t energy1 = get_node_energy(node1);
            uint64_t energy2 = get_node_energy(node2);

            /* If at least one history has assigned energy,
             * place them in the same equivalence class. */
            if (energy1 > 0 || energy2 > 0) {
                Equivalence_Class *class1 = get_node_class(node1);
                Equivalence_Class *class2 = get_node_class(node2);

                // Merge only if they are in different classes.
                if (class1 != class2) {
                    merge_classes(class1, class2);
                }
            }
            else {
                return -1;
            }
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }

    OPERATION_SUCCESSFUL;
    return 1;
}

static int is_history(char *history) {
    if (history == NULL) {
        return 0;
    }

    size_t history_length = strlen(history);

    // History shall be a C-string consiting only of '0', '1', '2', '3'.
    for (size_t i = 0; i < history_length; i++) {
        if (!('0' <= history[i] && history[i] <= '3')) {
            return 0;
        }
    }

    return 1;
}

static int is_energy(char *energy) {
    if (energy == NULL) {
        return 0;
    }

    size_t energy_length = strlen(energy);

    // Energy shall be a C-string consisting only of digits
    for (size_t i = 0; i < energy_length; i++) {
        if (!('0' <= energy[i] && energy[i] <= '9')) {
            return 0;
        }
    }

    // Energy shall fit in [MIN_ENERGY, MAX_ENERGY]
    if (compare_numbers(energy, MIN_ENERGY) < 0) {
        return 0;
    }

    if (compare_numbers(energy, MAX_ENERGY) > 0) {
        return 0;
    }

    return 1;
}


static int compare_numbers(char *a, char *b) {
    // Skip leading zeroes
    while ((*a) == '0') {
        a++;
    }

    while ((*b) == '0') {
        b++;
    }

    size_t a_length = strlen(a);
    size_t b_length = strlen(b);

    // Shorter number is smaller
    if (a_length < b_length) {
        return -1;
    }
    else if (a_length > b_length) {
        return 1;
    }

    /* If numbers are of equal length
     * then smaller one is the one with
     * smaller digit at first differing
     * position */
    for (size_t i = 0; i < a_length; i++) {
        if (a[i] < b[i]) {
            return -1;
        }
        else if (a[i] > b[i]) {
            return 1;
        }
    }

    return 0;
}

static uint64_t string_to_uint64_t(char *number) {
    uint64_t converted_number = 0;

    size_t number_length = strlen(number);

    for (size_t i = 0; i < number_length; i++) {
        converted_number *= 10;
        converted_number += CHAR_DIGIT_TO_INT(number[i]);
    }

    return converted_number;
}