#ifndef _EXECUTE_H
#define _EXECUTE_H

#include "structures/structures.h"

struct Value reduce(struct Tree * ast, hash_table *defined, struct Scopes * scopes);
struct Value execute (struct Tree *, hash_table *, struct Scopes * scopes, struct Value_array *, huo_depth_t max_depth);
struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b);

#endif
