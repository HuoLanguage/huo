#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include "string.h"
#include "value.h"
#include "hash_table.h"

struct Token {
    char type;
    struct String data;
};

struct Tokens {
    struct Token *tokens;
    int length;
    int counter;
};

struct Value_array {
    int size;
    struct Value * values[10000];
};

struct Keyval {
    struct Value * key;
    struct Value * val;
};

struct Map {
    int size;
    struct Keyval * members[200];
};

struct Scopes {
    hash_table ** scopes;
    int size;
    int current;
};

struct Tree {
    char type; // [o]pen, [f]unction [k]eyword [c]lose [s]tring [n]umber [b]racket [e]nd bracket
    int size;
    struct Value content;
    struct Tree * children[200];
    struct Tree * parent;
};

struct Execution_bundle {
    struct Tree * ast;
    hash_table *defined;
    struct Scopes * scopes;
    int max_depth;
};

#endif /* _STRUCTURES_H */
