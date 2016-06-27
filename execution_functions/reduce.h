#ifndef _REDUCE_H
#define _REDUCE_H

#include "../structures.h"

struct Value reduce_array(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes, int max_depth);

#endif
