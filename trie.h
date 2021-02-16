#ifndef IPP_TRIE_H
#define IPP_TRIE_H

#include "auxiliaries.h"
#include "eq_class.h"

/* TRIE data structure to validate histories.
 * Each node stores 4 pointers to sons and a
 * pointer to corresponding class member. */
typedef struct Trie {
    struct Class_Member *inClassRepresentant;
    struct Trie *son[4];
} Trie;

// Returns node equivalence class.
Equivalence_Class *get_node_class(Trie *node);

// Sets node's class energy.
void set_node_energy(Trie *node, uint64_t energy);

// Returns node's class energy.
uint64_t get_node_energy(Trie *node);

// Creates new Trie node.
Trie *create_trie_node(void);

// Inserts C-string into Trie.
void trie_insert(Trie *root, char *to_insert);

/* Finds given C-string in Trie. Returns pointer to
 * corresponding node or NULL pointer if not found. */
Trie *find_string_in_trie(Trie *root, char *to_find);

// Removes Trie subtree.
void remove_trie_subtree(Trie *root);

// Removes every C-string of given prefix.
void remove_trie_subtree_of_given_prefix(Trie *root, char *to_remove);

#endif //IPP_TRIE_H
