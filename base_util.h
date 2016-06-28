#ifndef _BASE_UTIL_H
#define _BASE_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "structures/structures.h"

/* Macro because it makes printf errors easier to detect at compile time */
/* Minor hack because c99 doesn't allow zero-length varargs for macros */
#define ERROR(...) do {\
    fprintf(stderr, "Error at %s:%s:%i: ", __FILE__, __func__, __LINE__);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(1);\
} while (0);

// Would be a function, but then So. Much. Casting.
#define RESIZE(ptr_to_arr, new_len) do {\
    if (new_len == 0) {\
        free(ptr_to_arr);\
        (ptr_to_arr) = NULL;\
    } else {\
        (ptr_to_arr) = realloc((ptr_to_arr), new_len * sizeof((ptr_to_arr)[0]));\
        if ((ptr_to_arr) == NULL) {\
            ERROR("Malloc failure!");\
        }\
    }\
} while (0)

int string_contains(char ch, struct String* string);
void printTree(struct Tree *tree);
struct Tree * duplicate_tree(struct Tree * a);
void copy_array(struct Value * a, struct Value_array * b);
void make_scope(struct Scopes * scopes);
void sub_vars(struct Value *v, struct Scopes *scopes, int max_depth);

#endif
