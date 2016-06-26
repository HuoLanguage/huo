#ifndef _REDUCE_AST_H
#define _REDUCE_AST_H

#include "../structures/structures.h"

struct Value reduce_ast(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth);

#endif
