#ifndef _IF_BLOCK_H
#define _IF_BLOCK_H

#include "../structures.h"

struct Value if_block(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes);

#endif
