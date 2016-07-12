#ifndef _LET_BINDING_H
#define _LET_BINDING_H

#include "../structures/structures.h"

void store_let_value(struct Value * key, struct Value * value, struct Scopes * scopes);
void store_def_func(struct Value * key, huo_ast *func, struct Scopes * scopes);

#endif
