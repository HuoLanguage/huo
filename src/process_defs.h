#ifndef _PROCESS_DEFS_H
#define _PROCESS_DEFS_H

#include "structures/structures.h"

void make_args_map(struct Execution_bundle * exec_bundle, struct Tree * function);
struct Tree *get_defined_body(struct Tree * function);
struct Tree *get_defined_func(hash_table *defined, struct String key);

#endif
