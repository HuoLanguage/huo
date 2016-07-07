#ifndef _LET_BINDING_H
#define _LET_BINDING_H

#include "../structures/structures.h"

void store_let_binding(struct Tree * key, struct Tree * value, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, huo_depth_t max_depth);
void store_let_value(struct Value * key, struct Value * value, struct Scopes * scopes);

#endif
