#ifndef _SWITCH_H
#define _SWITCH_H

#include "../structures/structures.h"

struct Value switch_case(struct Tree * ast, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth);

#endif
