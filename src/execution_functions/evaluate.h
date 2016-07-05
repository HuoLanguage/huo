#ifndef _EVALUATE_H
#define _EVALUATE_H

#include "../structures/structures.h"

struct Value eval(struct Value * string, hash_table * defined, struct Scopes * scopes, huo_depth_t max_depth);

#endif
