#ifndef _EXECUTE_H
#define _EXECUTE_H

#include "structures.h"

struct Value reduce(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes);
struct Value execute (struct Tree *, struct Tree_map *, struct Scopes * scopes);
struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b);
struct Value execute_defined_func(struct Tree * ast, struct Tree_map * defined, struct Map * scopes, struct Map * local_scopes, int idx);

#endif
