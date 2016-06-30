#ifndef _FOR_LOOP_H
#define _FOR_LOOP_H

#include "../structures/structures.h"

void for_loop(struct Tree * ast, hash_table *defined, struct Scopes * scopes, int max_depth);

#endif
