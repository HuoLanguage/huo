#ifndef _PROCESS_DEFS_H
#define _PROCESS_DEFS_H

#include "structures/structures.h"

void make_args_map(struct Execution_bundle * exec_bundle, huo_ast * function);
huo_ast *get_defined_body(huo_ast * function);
huo_ast *get_defined_func(struct Scopes *defined, struct String key);
struct Value *get_letted_value(struct Scopes *defined, struct String key);

#endif
