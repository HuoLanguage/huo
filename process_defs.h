#ifndef _PROCESS_DEFS_H
#define _PROCESS_DEFS_H

#include "structures/structures.h"

void make_args_map(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes, int idx, int max_depth);
struct Tree * get_defined_body(struct Tree * function);
int is_defined_func(struct Tree_map * defined, struct String key);

#endif
