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
    huo_ast *ast;
    struct Scopes *scopes;
    huo_depth_t max_depth;
};

struct definition {
    bool is_func;
    union {
        struct Value *val;
        huo_ast *ast;
    } val;
};

#endif /* _STRUCTURES_H */
