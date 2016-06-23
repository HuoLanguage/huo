#ifndef _BASE_UTIL_H
#define _BASE_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

/* Macro because it makes printf errors easier to detect at compile time */
/* Minor hack because c99 doesn't allow zero-length varargs for macros */
#define ERROR(...) do {fprintf(stderr, "Error at %s:%s:%i: ", __FILE__, __func__, __LINE__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(1);} while (0);

int string_contains(char ch, struct String* string);
void copy_value(struct Value * a, struct Value * b);
int string_matches(struct String base, struct String compare);
void printTree(struct Tree *tree);
struct Tree * duplicate_tree(struct Tree * a);
void copy_array(struct Value * a, struct Value_array * b);

#endif
