#ifndef _BASE_UTIL_H
#define _BASE_UTIL_H

#include "structures.h"

int string_contains(char ch, struct String* string);
void copy_value(struct Value * a, struct Value * b);
int string_matches(struct String base, struct String compare);
void printTree(struct Tree *tree);
struct Tree * duplicate_tree(struct Tree * a);
void copy_array(struct Value * a, struct Value_array * b);

#endif
