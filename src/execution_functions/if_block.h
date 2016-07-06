#ifndef _IF_BLOCK_H
#define _IF_BLOCK_H

#include "../structures/structures.h"

struct Value if_block(struct Tree * ast, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth);

#endif
