#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include "array.h"
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

struct Scopes {
    hash_table ** scopes;
    int size;
    int current;
};

struct Tree {
    char type; // [o]pen, [f]unction [k]eyword [c]lose [s]tring [n]umber [b]racket [e]nd bracket
    int size;
    struct Value content;
    struct Tree ** children;
    struct Tree * parent;
};

struct Execution_bundle {
    struct Tree * ast;
    hash_table *defined;
    struct Scopes * scopes;
    int max_depth;
};

#endif /* _STRUCTURES_H */
