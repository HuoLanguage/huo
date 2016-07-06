#ifndef _PROCESS_DEFS_H
#define _PROCESS_DEFS_H

#include "structures/structures.h"

void make_args_map(struct Tree * ast, hash_table * defined, struct Scopes * scopes, struct Value_array * function_names, struct Tree *function, int max_depth);
struct Tree *get_defined_body(struct Tree * function);
struct Tree *get_defined_func(hash_table *defined, struct String key);

#endif
