#ifndef _LET_BINDING_H
#define _LET_BINDING_H

#include "../structures/structures.h"

void store_let_binding(struct Tree * key, struct Tree * value, struct Execution_bundle * exec_bundle);
void store_let_value(struct Value * key, struct Value * value, struct Scopes * scopes);

#endif
