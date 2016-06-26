#ifndef _PARALLEL_EXECUTION_H
#define _PARALLEL_EXECUTION_H

#include "../structures/structures.h"

void parallel_execution(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth);

#endif
