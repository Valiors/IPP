#include "eq_class.h"

// Returns energy of merged equivalence classes.
static uint64_t merged_energy(uint64_t a, uint64_t b);

Equivalence_Class *get_class(Class_Member *member) {
    return member->my_class;
}

void set_class_energy(Class_Member *member, uint64_t energy) {
    member->my_class->energy = energy;
}

uint64_t get_class_energy(Class_Member *member) {
    return member->my_class->energy;
}

Class_Member *create_class(void) {
    Equivalence_Class *newClass = malloc(sizeof(Equivalence_Class));
    IS_ALLOC_SUCCESSFUL(newClass);

    // New class has one member.
    newClass->size_of_class = 1;

    // Equivalence class with unassigned energy has 0 energy.
    newClass->energy = 0;

    // Creates first member of the new class.
    Class_Member *newMember = malloc(sizeof(Class_Member));
    IS_ALLOC_SUCCESSFUL(newMember);

    newMember->my_class = newClass;

    newMember->prev = NULL;
    newMember->next = NULL;

    newClass->first_member = newMember;

    return newMember;
}

void delete_class_member(Class_Member *to_delete) {
    // Connects previous element with the next after current.
    if (to_delete->prev != NULL) {
        to_delete->prev->next = to_delete->next;
    }
    else {
        to_delete->my_class->first_member = to_delete->next;
    }

    // Connects next element with the previous before current.
    if (to_delete->next != NULL) {
        to_delete->next->prev = to_delete->prev;
    }

    // Decreases size of class and deletes it if necessary.
    to_delete->my_class->size_of_class--;

    if (to_delete->my_class->size_of_class == 0) {
        free(to_delete->my_class);
    }

    // Deletes member from memory.
    free(to_delete);
}

void merge_classes(Equivalence_Class *first, Equivalence_Class *second) {
    // Smaller class should be joined with greater.
    if ((first->size_of_class) > (second->size_of_class)) {
        Equivalence_Class *tmp = first;
        first = second;
        second = tmp;
    }

    // Reattaches first's members to second class.
    Class_Member *current_member = first->first_member;

    while (current_member->next != NULL) {
        current_member->my_class = second;
        current_member = current_member->next;
    }

    current_member->my_class = second;

    /* Last element of first class is attached to the
     * first element of second class. */
    current_member->next = second->first_member;

    second->first_member->prev = current_member;

    /* First element of first class becomes
     * first element of second class. */
    second->first_member = first->first_member;

    // Calculates merged energy.
    second->energy = merged_energy(first->energy, second->energy);

    // Size of merged class is the sum of first and second class sizes.
    second->size_of_class = first->size_of_class + second->size_of_class;

    // Deletes first class from memory.
    free(first);
}

static uint64_t merged_energy(uint64_t a, uint64_t b) {
    /* If one class has unassigned energy,
     * then the other one's is the merged energy. */
    if (a == 0) {
        return b;
    }

    if (b == 0) {
        return a;
    }

    /* If both classes has assigned energy then
     * merged energy is the arithmetic mean of energies.
     * Divisions are made first to accommodate
     * possibility of integer overflow. */
    if (a % 2 == 1 && b % 2 == 1) {
        return ((a - 1) / 2) + ((b - 1) / 2) + 1;
    }

    return (a / 2) + (b / 2);
}