#ifndef _IF_BLOCK_H
#define _IF_BLOCK_H

#include "../structures/structures.h"

struct Value if_block(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth);

#endif
