#ifndef _EVALUATE_H
#define _EVALUATE_H

#include "../structures/structures.h"

struct Value eval(struct Value * string, struct Tree_map * defined, struct Scopes * scopes, int max_depth);

#endif
