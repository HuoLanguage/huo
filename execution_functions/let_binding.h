#ifndef _LET_BINDING_H
#define _LET_BINDING_H

#include "../structures/structures.h"

void store_let_binding(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth);

#endif
