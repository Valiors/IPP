#include "trie.h"

Equivalence_Class *get_node_class(Trie *node) {
    return get_class(node->inClassRepresentant);
}

void set_node_energy(Trie *node, uint64_t energy) {
    set_class_energy(node->inClassRepresentant, energy);
}

uint64_t get_node_energy(Trie *node) {
    return get_class_energy(node->inClassRepresentant);
}

Trie *create_trie_node(void) {
    Trie *new_node = malloc(sizeof(Trie));
    IS_ALLOC_SUCCESSFUL(new_node);

    for (int i = 0; i < 4; i++) {
        new_node->son[i] = NULL;
    }

    new_node->inClassRepresentant = create_class();

    return new_node;
}

void trie_insert(Trie *root, char *to_insert) {
    size_t string_length = strlen(to_insert);

    Trie *current_node = root;

    // Walk down the tree.
    for (size_t i = 0; i < string_length; i++) {
        int son_number = CHAR_DIGIT_TO_INT(to_insert[i]);

        // Create new node if one is not present already.
        if (current_node->son[son_number] == NULL) {
            current_node->son[son_number] = create_trie_node();
        }

        current_node = current_node->son[son_number];
    }
}

Trie *find_string_in_trie(Trie *root, char *to_find) {
    size_t string_length = strlen(to_find);

    Trie *current_node = root;

    for (size_t i = 0; i < string_length; i++) {
        int son_number = CHAR_DIGIT_TO_INT(to_find[i]);

        // If the next node does not exist, then C-string is not present.
        if (current_node->son[son_number] != NULL) {
            current_node = current_node->son[son_number];
        }
        else {
            return NULL;
        }
    }

    return current_node;
}

void remove_trie_subtree(Trie *root) {
    // Recursively erase all children.
    for (int i = 0; i < 4; i++) {
        if (root->son[i] != NULL) {
            remove_trie_subtree(root->son[i]);
            root->son[i] = NULL;
        }
    }

    // Delete corresponding class member.
    delete_class_member(root->inClassRepresentant);

    // Delete root of Trie subtree from memory.
    free(root);
}

void remove_trie_subtree_of_given_prefix(Trie *root, char *to_remove) {
    size_t string_length = strlen(to_remove);

    Trie *current_node = root;

    // Find second to last node of C-string.
    for (size_t i = 0; i < string_length - 1; i++) {
        int son_number = CHAR_DIGIT_TO_INT(to_remove[i]);

        if (current_node->son[son_number] != NULL) {
            current_node = current_node->son[son_number];
        }
        else {
            return;
        }
    }

    int last_son_number = CHAR_DIGIT_TO_INT(to_remove[string_length - 1]);

    // Delete whole subtree.
    if (current_node->son[last_son_number] != NULL) {
        remove_trie_subtree(current_node->son[last_son_number]);

        current_node->son[last_son_number] = NULL;
    }
}