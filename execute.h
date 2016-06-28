#ifndef _EXECUTE_H
#define _EXECUTE_H

#include "structures/structures.h"

struct Value reduce(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes);
struct Value execute (struct Tree *, struct Tree_map *, struct Scopes * scopes, int max_depth);
struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b);

#endif
