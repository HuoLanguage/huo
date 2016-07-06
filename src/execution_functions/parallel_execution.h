#ifndef _PARALLEL_EXECUTION_H
#define _PARALLEL_EXECUTION_H

#include "../structures/structures.h"

void parallel_execution(struct Tree * ast, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth);

#endif
