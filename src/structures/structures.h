#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include <stdint.h>

#include "string.h"
#include "value.h"
#include "huo_ast.h"
#include "array.h"
#include "hash_table.h"
#include "token.h"

struct Scopes {
    hash_table ** scopes;
    size_t size;
    size_t current;
};

struct Execution_bundle {
    struct Tree *ast;
    hash_table *defined;
    struct Scopes *scopes;
    struct Value_array *function_names;
    huo_depth_t max_depth;
};

#endif /* _STRUCTURES_H */
