#ifndef IPP_EQ_CLASS_H
#define IPP_EQ_CLASS_H

#include "auxiliaries.h"

/* Equivalence class implemented as
 * doubly-linked list, storing its' size
 * and class energy. */
typedef struct Equivalence_Class {
    int size_of_class;
    uint64_t energy;
    struct Class_Member *first_member;
} Equivalence_Class;

/* Class member as member of doubly-linked
 * list contains pointer to next and previous
 * members. Contains also pointer to member's class. */
typedef struct Class_Member {
    struct Equivalence_Class *my_class;
    struct Class_Member *prev;
    struct Class_Member *next;
} Class_Member;

// Returns member's class.
Equivalence_Class *get_class(Class_Member *member);

// Sets member's class energy.
void set_class_energy(Class_Member *member, uint64_t energy);

// Returns energy of member's class.
uint64_t get_class_energy(Class_Member *member);

// Creates new class with one element. Returns pointer to this element.
Class_Member *create_class(void);

// Deletes member from class.
void delete_class_member(Class_Member *to_delete);

// Merges two classes into one.
void merge_classes(Equivalence_Class *first, Equivalence_Class *second);

#endif //IPP_EQ_CLASS_H
