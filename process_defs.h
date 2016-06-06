#ifndef _PROCESS_DEFS_H
#define _PROCESS_DEFS_H

#include "structures.h"

struct Map * make_args_map(struct Tree * ast, struct Tree_map * defined, int idx);
struct Tree * get_defined_body(struct Tree * function);
struct Tree * populate_args(struct Map * arguments, struct Tree * ast);
int is_defined_func(struct Tree_map * defined, struct String key);

#endif
