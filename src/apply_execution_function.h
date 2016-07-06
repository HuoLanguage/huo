#ifndef _APPLY_EXECUTION_FUNCTION_H
#define _APPLY_EXECUTION_FUNCTION_H

#include "structures/structures.h"

struct Value apply_execution_function(struct Tree * ast, hash_table * defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth);

#endif
