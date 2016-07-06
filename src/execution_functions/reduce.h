#ifndef _REDUCE_H
#define _REDUCE_H

#include "../structures/structures.h"

struct Value reduce_array(struct Tree * ast, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth);

#endif
