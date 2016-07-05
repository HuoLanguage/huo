#ifndef _IF_BLOCK_H
#define _IF_BLOCK_H

#include "../structures/structures.h"

struct Value if_block(struct Tree * ast, hash_table *defined, struct Scopes * scopes, huo_depth_t max_depth);

#endif
