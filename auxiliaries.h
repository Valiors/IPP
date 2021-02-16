#ifndef IPP_DEFINES_H
#define IPP_DEFINES_H

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#define _X_OPEN_SOURCE 700

#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// Minimum and maximum energy.
#define MIN_ENERGY "1"
#define MAX_ENERGY "18446744073709551615"

// Macros for command names.
#define COMMAND_DECLARE "DECLARE"
#define COMMAND_REMOVE "REMOVE"
#define COMMAND_VALID "VALID"
#define COMMAND_ENERGY "ENERGY"
#define COMMAND_EQUAL "EQUAL"

#define OPERATION_SUCCESSFUL fprintf(stdout, "OK\n")

#define VALID_YES fprintf(stdout, "YES\n")

#define VALID_NO fprintf(stdout, "NO\n")

#define OUTPUT_ERROR fprintf(stderr, "ERROR\n")

#define CHAR_DIGIT_TO_INT(c) ((int)c - (int)'0')

// Ends program if memory allocation fails.
#define IS_ALLOC_SUCCESSFUL(pointer) if((pointer) == NULL) { \
            exit(1); \
        }

/* Ends program if memory allocation for input string fails
 * or other fatal reading error occurs. */
#define IS_READING_SUCCESSFUL(stream) if(ferror(stream)) { \
            exit(1); \
        }

#endif // IPP_DEFINES_H
