#ifndef _REDUCE_AST_H
#define _REDUCE_AST_H

#include "../structures/structures.h"

struct Value reduce_ast(struct Tree * ast, hash_table *defined, struct Scopes * scopes, huo_depth_t max_depth);

#endif
