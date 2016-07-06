#ifndef _WHILE_LOOP_H
#define _WHILE_LOOP_H

#include "../structures/structures.h"

void while_loop(struct Tree * ast, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth);

#endif
