#ifndef _EXECUTE_H
#define _EXECUTE_H

#include "structures/structures.h"

struct Value reduce(huo_ast * ast, hash_table *defined, struct Scopes * scopes);
struct Value execute(struct Execution_bundle * exec_bundle);

#endif
