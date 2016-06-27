#ifndef _REDUCE_AST_H
#define _REDUCE_AST_H

#include "../structures.h"

struct Value reduce_ast(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes);

#endif
