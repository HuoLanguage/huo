#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include <stdint.h>
#include "array.h"
#include "string.h"
#include "value.h"
#include "hash_table.h"
typedef uint_fast16_t huo_depth_t;

struct Token {
    char type;
    struct String data;
};

struct Tokens {
    struct Token *tokens;
    size_t length;
    size_t counter;
};

struct Scopes {
    hash_table ** scopes;
    size_t size;
    size_t current;
};

struct Tree {
    char type; // [o]pen, [f]unction [k]eyword [c]lose [s]tring [n]umber [b]racket [e]nd bracket
    size_t size;
    struct Value content;
    struct Tree ** children;
    struct Tree * parent;
};

struct Execution_bundle {
    struct Tree *ast;
    hash_table *defined;
    struct Scopes *scopes;
    struct Value_array *function_names;
    huo_depth_t max_depth;
};

#endif /* _STRUCTURES_H */
